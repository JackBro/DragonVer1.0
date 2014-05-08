#include "driver.h"
#ifndef	V578CMD_DISABLE

#define DIV_ROUND(x, y)  ( (x + y - 1) / y )
#define ZERO_ROUND(x)  ( x = x ? x : 1 )


#define HABLAN_MAX 0x3FF

static 	const UINT8 gJpegIntPriority[V5_JPEG_JPEG_INT_NUM] = 
{ 
	INT_JPEG_DEC_ERR, INT_JPEG_ENC_DONE, INT_JPEG_DEC_DONE
};

static const UINT8 gLbufIntPriority[V5_JPEG_LBUF_INT_NUM] = { INT_LBUF_READ};

static const UINT8 gPixel[] = {1, 1, 3, 3, 7, 7, 7, 7, 0xb, 0xb, 0xb, 0xb, 0xf, 0xf, 0xf, 0xf};

static TJpegContext gJpegContext;

//===============================================================================================================
// Static functions
//===============================================================================================================

static void WriteQT(PTJpegIndex pinfo)
{
	UINT8 i, j;
	
	if(pinfo->QTCount == 0)
		return;
	V5B_JpegStartWriteQT();
	for(i = 0; i < pinfo->QTCount; i++)
	{
		for(j = 0; j < 64; j++)
			V5B_JpegSetQtable(pinfo->QT[i][j]);
	}
}

static void WriteHT(PTJpegIndex pinfo)
{
	UINT8 i, j;
	
	if(pinfo->HTCount == 0)
		return;
	V5B_JpegCustHTEnable();
	V5B_JpegStartWriteHT();
	for(i = 0; i < pinfo->HTCount; i++)
	{
		V5B_JpegSelectHT(i);
		for(j = 0; j < pinfo->HTLen[i]; j++)
			V5B_JpegSetHtable(pinfo->HT[i][j]);
	}
}

static void WriteComponents(PTJpegIndex pinfo)
{
	UINT8 i;
	
	if( (pinfo->QTCount == 0) || (pinfo->HTCount == 0) )
		return;
	
	V5B_JpegSetComponentNum(pinfo->CompCount | (pinfo->QTCount << 4));
	for(i = 0; i < 4; i++)
		V5B_JpegSetComponentParm(i, pinfo->Comp[i]);
}

static void DecodeJpg(PTJpegIndex pinfo)
{
	V5B_JpegDispVdDisable();
	V5B_JpegCustQTDisable();

	V5_JpegSetImage(pinfo->YUVType, pinfo->JpgSize);
	//Now change lbuf info. should set marb mem.
	V5_MarbAutoSetMem();

	WriteComponents(pinfo);

	//Use custom qt, resetstate before custom qt enable. because V5B_JpegCustQTEnable case JPEG to act state.
	V5B_JpegResetState();
	V5B_LbufCounterReset();

	V5B_JpegCustQTEnable();
	WriteQT(pinfo);
	WriteHT(pinfo);

	V5_MarbStartDecoder();
	V5B_JpegStartDecode();
	V5_Delay(1);
}

/********************************************************************************
  Description:
	Decode jpeg.
  Parameters:
	fmt		: yuv format
	size	: image size
	type	: 0 - Decode frame auto(Only vimicro's jpeg)
			  1 - Decode Video auto(Only vimicro's jpeg)
			  2 - Decode bypass
  Return:
  Remarks:
*********************************************************************************/
static void DecodeAuto(UINT8 fmt, TSize size, UINT8 type)
{
	//Don't delete it. has been configued in 578 chip.
	if( 1 == type )
		V5B_JpegDispVdEnable();
	else
		V5B_JpegDispVdDisable();

	V5B_JpegCustHTDisable();
	V5B_JpegCustQTDisable();

	V5_JpegSetImage(fmt, size);
	//Now change lbuf info. should set marb mem.
	V5_MarbAutoSetMem();

	//Not use custom qt, reset state before jpeg start decode.
	V5B_JpegResetState();
	V5B_LbufCounterReset();

	V5_MarbStartDecoder();
	V5B_JpegStartDecode();
	V5_Delay(1);
}

static void JpegSetImage(UINT8 fmt, TSize size)
{
	UINT32 vwc;
	UINT16 blk, comp[3];
	UINT8 compnum;
	TSize size1;

	GetJpegSize(fmt, size, &size1);
	V5B_LbufSetYuvMode(fmt);
	V5B_LbufSetWidth(size1.cx);
	V5B_JpegSetImgSize(size);
	vwc = GetImageVwc(fmt, size);
	V5B_JpegSetVWC(vwc);
	blk = GetJpegBlockNum(fmt, size);
	V5B_JpegSetBlockNum(blk);

	compnum = GetComponent(fmt, comp);
	V5B_JpegSetComponentNum(compnum);
	V5B_JpegSetComponentParm(0, comp[0]);
	V5B_JpegSetComponentParm(1, comp[1]);
	V5B_JpegSetComponentParm(2, comp[2]);
}

static void JpegSetHblank(UINT16 hblank)
{
	V5B_JpegSetHblank(hblank+1);
	V5B_LbufSetHblank(hblank);
}

static void JpegSetPixelRate(UINT8 pixelrate)
{
	V5B_JpegSetDecBypStep(pixelrate);
	V5B_LbufSetPixrate(pixelrate);
}

/********************************************************************************
	In Work Mode:

	display
	display bypass
	decode frame
	decode frame bypass

	Use this functions to calculate pixrate.
********************************************************************************/
static UINT8 GetPixRate(UINT16 src_cy)
{
	UINT16 dst_cx, dst_cy, src_cx;
	UINT32 lcddelay;
	UINT32 rate;

	if( (JPEG_MODE_DISPLAY == gJpegContext.mode) || (JPEG_MODE_DISPLAY_BYPASS == gJpegContext.mode) )
	{
		if( LINEBUF_ONLY != V5_MarbGetAllocMode() )
			return 1;
	}

	src_cx = V5B_IppGetWinWidth();
	dst_cx = V5B_IppGetDispWidth();
	dst_cy = V5B_IppGetDispHeight();

	ZERO_ROUND(src_cx);
	ZERO_ROUND(src_cy);
	ZERO_ROUND(dst_cx);
	ZERO_ROUND(dst_cy);

	lcddelay = V5B_LcdcGetDEinteval() + 2;
	if( V5B_LcdcGetAlayerRM()&0x1 )
		lcddelay <<= 1;
	rate = DIV_ROUND( DIV_ROUND( lcddelay * dst_cy * dst_cx, src_cy ), src_cx);
	if(rate < 2)
		rate = 2;
	if(rate > 16)
		rate = 16;

	return gPixel[rate - 1];
}

/********************************************************************************
	In Work Mode:

	display
	display bypass
	decode frame
	decode frame bypass

	Use this functions to calculate hblank.
********************************************************************************/
static UINT16 GetHbanlk(UINT8 pixelrate, UINT16 src_cy)
{
	UINT16 dst_cx, dst_cy, src_cx;
	UINT32 lcddelay;
	UINT32 hblank;

	if( (JPEG_MODE_DISPLAY == gJpegContext.mode) || (JPEG_MODE_DISPLAY_BYPASS == gJpegContext.mode) )
	{
		if( LINEBUF_ONLY != V5_MarbGetAllocMode() )
			return 0;
	}

	src_cx = V5B_IppGetWinWidth();
	dst_cx = V5B_IppGetDispWidth();
	dst_cy = V5B_IppGetDispHeight();

	ZERO_ROUND(src_cx);
	ZERO_ROUND(src_cy);
	ZERO_ROUND(dst_cx);
	ZERO_ROUND(dst_cy);

	lcddelay = V5B_LcdcGetDEdelay() + V5B_LcdcGetLineinteval() + 4;
	hblank = DIV_ROUND( lcddelay * dst_cy, src_cy);
	hblank = DIV_ROUND( hblank, pixelrate);
	if(hblank > HABLAN_MAX)
		hblank = HABLAN_MAX;

	return (UINT16)hblank;
}


//===============================================================================================================
// Global functions
//===============================================================================================================


/********************************************************************************
  Description:
	Initial jpeg context.
  Parameters:
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegInitContext(void)
{
	gJpegContext.state = JPEG_NOT_INIT;
}

/********************************************************************************
  Description:
	Set jpeg user infor.
  Parameters:
	info : the pointer to jpeg info.
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegSetInfo(TJpegInfo *info)
{
	gJpegContext.pinputinfo = info;

	gJpegContext.state = JPEG_INIT;
}

/********************************************************************************
  Description:
	Set jpeg user infor.
  Parameters:
  Return: The pointer to jpeg context.
  Remarks:
*********************************************************************************/
TJpegContext * V5_JpegGetContext(void)
{
	return &gJpegContext;
}

/********************************************************************************
  Description:
	Open jpeg module. Set register, set use configuration ect.
  Parameters:
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegOpen(void)
{
	gJpegContext.mode = 0xff;

	V5_JpegJpegIntDisable(JPEG_JPEG_INT_ALL);
	V5_JpegJpegIntSetCallback(JPEG_JPEG_INT_ALL, NULL);

	V5_JpegLbufIntDisable(JPEG_LBUF_INT_ALL);
	V5_JpegLbufIntSetCallback(JPEG_LBUF_INT_ALL, NULL);

//	V5B_JpegSetImageParm(gJpegContext.pinputinfo->jpegheaderlen, gJpegContext.pinputinfo->jpgheader);
	V5B_JpegSetBrcQF(gJpegContext.pinputinfo->qf);
	gJpegContext.ctrl = gJpegContext.pinputinfo->ctrl;
	if(gJpegContext.pinputinfo->ctrl & BRC_EN)
		V5B_JpegEnAbrc();
	else
		V5B_JpegDisAbrc();

	V5B_JpegSetTCR(gJpegContext.pinputinfo->tcr);

	V5_JpegSetRudeBrcPara(&gJpegContext.pinputinfo->rudebrc);
	if(gJpegContext.pinputinfo->ctrl & BRC_RUDE_UV_ONLY)
		V5B_JpegRudeBrcUvOnlyEnable();
	else
		V5B_JpegRudeBrcUvOnlyDisable();
	if(gJpegContext.pinputinfo->ctrl & BRC_RUDE_EN)
		V5B_JpegRudeBrcEnable();
	else
		V5B_JpegRudeBrcDisable();

	V5_JpegSetEnhBrcPara(&gJpegContext.pinputinfo->enhbrc);
	if(gJpegContext.pinputinfo->ctrl & BRC_ENH_EN)
		V5B_JpegEnhBrcEnable();
	else
		V5B_JpegEnhBrcDisable();

	gJpegContext.capdly.en = gJpegContext.pinputinfo->capdly.en;
	gJpegContext.capdly.feedval = gJpegContext.pinputinfo->capdly.feedval;


}

/********************************************************************************
  Description:
	Close jpeg module, release jpeg setting.
  Parameters:
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegClose(void)
{
}

/********************************************************************************
  Description:
	Set jpeg qfactor.
  Parameters:
	val : qfactor, act_q_table[i] = default_q_table[i]/qfactor
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegSetQFactor(UINT8 val)
{
	V5B_JpegSetBrcQF(val);
	V5B_JpegResetState();
}

void V5_JpegDisAbrc()
{
	V5B_JpegDisAbrc();
	V5_JpegSetQFactor(gJpegContext.pinputinfo->qf);
}

/*************************************************************
	Because 578 use two pipe lbuf to send captued data to jpeg.
	The blank between two frame should content to this prisple.
	Delay calcaulate by capture height and width.
**************************************************************/
static UINT16 CalCapDelay(UINT8 fmt, UINT16 cx, UINT16 cy, UINT8 feedval)
{
	UINT8	blkheight, comp;
	UINT32	dlycnt;

	switch(fmt)
	{
	case V5B_LBUF_YUV_420:
		//Because dlycnt in register should >> 3, now cx feed it.
		//cx = ((width + 15) >> 4) << 4;
		cx = ((cx + 15) >> 4) << 1;
		blkheight = 16;
		comp = 3;
		break;
	case V5B_LBUF_YUV_411:
		cx = ((cx + 31) >> 5) << 2;
		blkheight = 8;
		comp = 3;
		break;
	case V5B_LBUF_YUV_400:
		cx = (cx + 7) >> 3;
		blkheight = 8;
		comp = 2;
		break;
	default: //V5B_LBUF_YUV_422:
		cx = ((cx + 15) >> 4) << 1;
		blkheight = 8;
		comp = 4;
		break;
	}

	cy = cy % blkheight;
	//optimise these two lines to the next.
	//cy = ( ( cy + blkheight ) * comp ) >> 1;
	//dlycnt = cy * cx;
	cy = cy + blkheight;
	if(cy > feedval)
		cy = cy - feedval;
	else
		cy = 0;
	dlycnt = cy * comp;
	dlycnt = (dlycnt * cx) >> 1;

	//Because cx has been adjusted before ,now reduce it.
	//dlycnt >>= 3;
	if(dlycnt > 0x7fff)
		dlycnt = 0x7fff;
	
	return (UINT16)dlycnt;
}

/********************************************************************************
  Description:
	Set jpeg image size and format
  Parameters:
	fmt : format, see as V5B_LBUF_YUVenum
	size : size
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegSetImage(UINT8 fmt, TSize size)
{
	UINT16 blank;
	UINT8 rate;
	UINT8 capdly;

	switch(gJpegContext.mode)
	{
	case JPEG_MODE_IDLE:
		break;

	case JPEG_MODE_CAPTURE:
	case JPEG_MODE_CAPTUREFRAME:
		JpegSetImage(fmt, size);
		if(gJpegContext.capdly.en)
		{
			if(V5B_LBUF_YUV_411 == fmt)
				capdly = (UINT8)( size.cy % 16 );
			else
				capdly = (UINT8)( size.cy % 8 );

			if(capdly)
			{
				blank = CalCapDelay(fmt, size.cx, size.cy, gJpegContext.capdly.feedval);
				V5B_LbufSetCapDly(blank);

				V5B_LbufCapDlyEnable();
				break;
			}
		}
		
		V5B_LbufCapDlyDisable();
		break;

	case JPEG_MODE_ENCODE:
		JpegSetImage(fmt, size);
		break;

	case JPEG_MODE_DISPLAY:
	case JPEG_MODE_DISPLAY_BYPASS:
	case JPEG_MODE_DECODEFRAME:
	case JPEG_MODE_DECODEFRAME_BYPASS:
		JpegSetImage(fmt, size);
		rate = GetPixRate(size.cy);
		JpegSetPixelRate(rate);
		blank = GetHbanlk(rate, size.cy);
		JpegSetHblank(blank);
		break;

	case JPEG_MODE_DECODEIPP:
	case JPEG_MODE_DECODEIPP_BYPASS:
		JpegSetImage(fmt, size);
		JpegSetPixelRate(0x1);
		JpegSetHblank(0x0);
		break;

	case JPEG_MODE_DECODE:
		JpegSetImage(fmt, size);
		JpegSetPixelRate(0x1);
		JpegSetHblank(0x0);
		break;

	default:
		break;
	}
}

static void JpegResetState(void)
{
	UINT8 val;

	if( gJpegContext.ctrl & BRC_EN )
	{
		val = V5B_JpegGetQfactor();
		V5B_JpegSetBrcQF(val);
	}
	V5B_JpegResetState();
}

/********************************************************************************
  Description:
	Set jpeg work mode
  Parameters:
	mode : work mode, see as JPEGMODEenum 
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegSetMode(UINT8 mode)
{
	if(gJpegContext.mode == mode)
		return;


	switch(mode)
	{
	case JPEG_MODE_IDLE:
		break;

	case JPEG_MODE_CAPTURE:
		V5B_LbufSetMode(V5B_LBUF_MODE_CAPTURE);
		V5B_JpegSetMod(V5B_JPEG_WORKMODE_CAP);
		JpegResetState();
		V5B_LbufSetDataFromIpp();
		V5B_LbufDecBypDisable();
		V5B_JpegDecBypDisable();


		break;

	case JPEG_MODE_CAPTUREFRAME:
		V5B_LbufSetMode(V5B_LBUF_MODE_CAPTURE);
		V5B_JpegSetMod(V5B_JPEG_WORKMODE_CAP);
		JpegResetState();
		V5B_LbufSetDataFromLcd();
		V5B_JpegDecBypDisable();
		V5B_LbufDecBypDisable();
		break;

	case JPEG_MODE_ENCODE:
		V5B_LbufSetMode(V5B_LBUF_MODE_CAPTURE);
		V5B_JpegSetMod(V5B_JPEG_WORKMODE_ENC);
		JpegResetState();
		V5B_JpegDecBypDisable();
		V5B_LbufDecBypDisable();
		V5B_LbufCapDlyDisable();
		break;

	case JPEG_MODE_DISPLAY:
		V5B_LbufSetMode(V5B_LBUF_MODE_DISPLAY);
		V5B_JpegSetMod(V5B_JPEG_WORKMODE_DISP);
		JpegResetState();
		V5B_JpegDecBypDisable();
		V5B_LbufDecBypDisable();
		V5B_LbufCapDlyDisable();
		break;

	case JPEG_MODE_DISPLAY_BYPASS:
		V5B_LbufSetMode(V5B_LBUF_MODE_DISPLAY);
		V5B_JpegSetMod(V5B_JPEG_WORKMODE_DISP);
		JpegResetState();
		V5B_JpegDecBypEnable();
		V5B_LbufDecBypEnable();
		V5B_LbufCapDlyDisable();
		break;

	case JPEG_MODE_DECODE:
		V5B_LbufSetMode(V5B_LBUF_MODE_DECODER_NORM);
		V5B_JpegSetMod(V5B_JPEG_WORKMODE_DEC);
		JpegResetState();
		V5B_JpegDecBypDisable();
		V5B_LbufDecBypDisable();
		V5B_LbufCapDlyDisable();
		break;

	case JPEG_MODE_DECODEIPP:
		V5B_LbufSetMode(V5B_LBUF_MODE_DECODER_IPP);
		V5B_JpegSetMod(V5B_JPEG_WORKMODE_DEC);
		JpegResetState();
		V5B_LbufSetDataFromIpp();
		V5B_JpegDecBypDisable();
		V5B_LbufDecBypDisable();
		V5B_LbufCapDlyDisable();
		break;

	case JPEG_MODE_DECODEIPP_BYPASS:
		V5B_LbufSetMode(V5B_LBUF_MODE_DECODER_IPP);
		V5B_JpegSetMod(V5B_JPEG_WORKMODE_DEC);
		JpegResetState();
		V5B_LbufSetDataFromIpp();
		V5B_JpegDecBypEnable();
		V5B_LbufDecBypEnable();
		V5B_LbufCapDlyDisable();
		break;

	case JPEG_MODE_DECODEFRAME:
		V5B_LbufSetMode(V5B_LBUF_MODE_DECODER_IPP_LCD);
		V5B_JpegSetMod(V5B_JPEG_WORKMODE_DEC);
		JpegResetState();
		V5B_LbufSetDataFromLcd();
		V5B_JpegDecBypDisable();
		V5B_LbufDecBypDisable();
		V5B_LbufCapDlyDisable();
		break;

	case JPEG_MODE_DECODEFRAME_BYPASS:
		V5B_LbufSetMode(V5B_LBUF_MODE_DECODER_IPP_LCD);
		V5B_JpegSetMod(V5B_JPEG_WORKMODE_DEC);
		JpegResetState();
		V5B_LbufSetDataFromLcd();
		V5B_JpegDecBypEnable();
		V5B_LbufDecBypEnable();
		V5B_LbufCapDlyDisable();
		break;

	default:
		return;
	}
	gJpegContext.mode = mode;
}

/********************************************************************************
  Description:
	Get jpeg work mode
  Parameters:
  Return: Jpeg work mode
  Remarks:
*********************************************************************************/
UINT8 V5_JpegGetMode(void)
{
	return gJpegContext.mode;
}

/********************************************************************************
  Description:
	Auto adjust jpeg size, when Ipp capture/display size changed.
  Parameters:
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegAutoSetImage(void)
{
	TSize size;
	UINT8 fmt;

	switch(gJpegContext.mode)
	{
	case JPEG_MODE_IDLE:
		break;
	case JPEG_MODE_CAPTURE:
		V5B_IppGetCapSize(&size);
		fmt = V5B_LbufGetYuvMode();
		V5_JpegSetImage(fmt,size);
		break;

	case JPEG_MODE_CAPTUREFRAME:
		V5B_LcdcGetAWinSize(&size);
		fmt = V5B_LbufGetYuvMode();
		V5_JpegSetImage(fmt,size);
		break;

	case JPEG_MODE_ENCODE:
		break;

	case JPEG_MODE_DECODE:
	case JPEG_MODE_DISPLAY:
	case JPEG_MODE_DISPLAY_BYPASS:
	case JPEG_MODE_DECODEIPP:
	case JPEG_MODE_DECODEFRAME:
	case JPEG_MODE_DECODEIPP_BYPASS:
	case JPEG_MODE_DECODEFRAME_BYPASS:
		V5B_IppGetImageSize(&size);
		fmt = V5B_LbufGetYuvMode();
		V5_JpegSetImage(fmt,size);
		break;

	default:
		break;
	}
}

/********************************************************************************
  Description:
	Jpeg capture function,it is included such functions:
		CAPTURE STILL
		CAPTURE FRAME
		CAPTURE VIDEO
		CAPTURE FRAME VIDEO
		CAPTURE AVI
		CAPTURE FRAME AVI
		CAPTURE STILL WITH THUMBNAIL
		CAPTURE VIDEO WITH THUMBNAIL
		CAPTURE AVI WITH THUMBNAIL
		MULTISHOT
		MULTISHOT WITH FRAME
		MULTISHOT WITH THUMBNAIL.

  Parameters:
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegCapture(void)
{
	V5B_MarbRpDisable();
	V5B_LbufCounterReset();
	V5_MarbStartCapture();
}

/********************************************************************************
  Description:
	Jpeg sif capture function,it is included such functions:
		CAPTURE STILL
		CAPTURE FRAME
		CAPTURE STILL WITH THUMBNAIL

  Parameters:
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegSifCapture(void)
{
	V5B_MarbRpDisable();
	V5B_SifDisableSyncGen();
	V5_Delay(1);
	V5_JpegCapture();
	V5_VispFlashLight();
	V5B_LbufCounterReset();
	V5B_SifStartCapture();
}

/********************************************************************************
  Description:
	Jpeg repeat capture function,it is included such functions:
		CAPTURE STILL
		CAPTURE FRAME
		CAPTURE STILL WITH THUMBNAIL

  Parameters:
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegReCapture(void)
{
	V5B_MarbRpEnable();
	V5B_LbufCounterReset();
	V5_MarbStartCapture();
}

/********************************************************************************
  Description:
	Jpeg encode function
  Parameters:
  fmt : format, see as V5B_LBUF_YUVenum.
  size : size.
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegEncode(UINT8 fmt, TSize size)
{
	V5B_JpegCustQTDisable();

	V5_JpegSetImage(fmt, size);
	V5_MarbAutoSetMem();

	V5B_JpegResetState();

	V5_MarbStartCapture();
	V5B_JpegStartEncode();
	V5_Delay(1);
}

/********************************************************************************
  Description:
	Decode jpeg one frame. write jpeg huffman table and q_table
  Parameters:
	pinfo :	jpeg info. include jpeg huffman table and q_table
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegDecode(PTJpegIndex pinfo)
{
	DecodeJpg(pinfo);
}

/********************************************************************************
  Description:
	Decode jpeg video
  Parameters:
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegDecodeVideo(UINT8 fmt, TSize size)
{
	DecodeAuto(fmt, size, 1);
}

/********************************************************************************
  Description:
	Decode jpeg , whose data is in 578 sram jbuf. 
  Parameters:
	jbufstart :	start address of jpeg data in 578 sram.
	jbufsize : size of jpeg data in 578 sram.
  Return:
  Remarks: Jpeg data must be captured/encoded by v578 chip
*********************************************************************************/
void V5_JpegDecodeAuto(UINT8 fmt, TSize size)
{
	DecodeAuto(fmt, size, 0);
}

/********************************************************************************
  Description:
	Decode jpeg frame if the data format is YUV422_UYVY. 
  Parameters:
	size : data size.
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegYuvDecode(TSize size)
{
	DecodeAuto(V5B_LBUF_YUV_422, size, 2);
}

/********************************************************************************
  Description:
	Decode jpeg next frame after V5_JpegDecode or V5_JpegDecodeAuto first frame. it don't need to write jpeg huffman table and q_table again.
  Parameters:
  Return:
  Remarks: Jpeg data must be captured/encoded by v578 chip
*********************************************************************************/
void V5_JpegDecodeNext(void)
{
	//Use default qt, resetstate after disable qt.
	V5B_JpegResetState();
	V5B_LbufCounterReset();

	V5B_JpegStartDecode();
}

/********************************************************************************
  Description:
	Set rude brc para. 
  Parameters:
	pRude : see PTJpegRudeBrc.
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegSetRudeBrcPara(PTJpegRudeBrc pRude)
{
	UINT8 i;

	for(i=0; i<6; i++)
		V5B_JpegSetRudeCompLevel(i, pRude->CompLevel[i]);
	V5B_JpegSetRudeAc(pRude->RudeAc);
	V5B_JpegSetRudeLevel(pRude->RudeLevel);
	V5B_JpegSetRudeThdWc(pRude->ThdWc);
}

/********************************************************************************
  Description:
	Set enhanced brc para. 
  Parameters:
	pRude : see PTJpegEnhBrc.
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegSetEnhBrcPara(PTJpegEnhBrc pEnh)
{
	V5B_JpegSetEnhThdR1(pEnh->ThdR1);
	V5B_JpegSetEnhThdR2(pEnh->ThdR2);
	V5B_JpegSetEnhThdL1(pEnh->ThdL1);
	V5B_JpegSetEnhThdL2(pEnh->ThdL2);
	V5B_JpegSetEnhShfR1(pEnh->ShfR1);
	V5B_JpegSetEnhShfR2(pEnh->ShfR2);
	V5B_JpegSetEnhShfL1(pEnh->ShfL1);
	V5B_JpegSetEnhShfL2(pEnh->ShfL2);
}


//=================================Interrupt handle==============================================

/************************************************************************************************
*								JPEG Interrupt
************************************************************************************************/

void _ISR_V5JpegJpegIntHandle(UINT16 intflg)
{
	UINT8 x, i;

	for(i=0; i<V5_JPEG_JPEG_INT_NUM; i++)
	{
		x = gJpegIntPriority[i];
		if(TSTINT(intflg, x))
		{
			if(gJpegContext.jpegintisr[x])
				gJpegContext.jpegintisr[x]();
		}
	}
}

void V5_JpegJpegIntSetCallback(UINT8 index, PVJpegIsrCallback pcall)
{
	UINT8 i;

	if(JPEG_JPEG_INT_ALL <= index)
	{
		for(i = 0; i < JPEG_JPEG_INT_ALL; i++)
		{
			gJpegContext.jpegintisr[i] = pcall;
		}
	}
	else
	{
		gJpegContext.jpegintisr[index] = pcall;
	}
}

void V5_JpegJpegIntEnable(UINT8 index)
{
	UINT16 inten;

	if(JPEG_JPEG_INT_ALL <= index)
	{
		V5B_CpmSetIntEnSec(INT_JPEG, 0xffff);
	}
	else
	{
		inten = V5B_CpmGetIntEnSec(INT_JPEG);
		inten |= (0x1<<index);

		V5B_CpmSetIntEnSec(INT_JPEG, inten);
	}
}

void V5_JpegJpegIntDisable(UINT8 index)
{
	UINT16 inten;

	if(JPEG_JPEG_INT_ALL <= index)
	{
		V5B_CpmSetIntEnSec(INT_JPEG, 0x0);
	}
	else
	{
		inten = V5B_CpmGetIntEnSec(INT_JPEG);
		inten &= (~(0x1<<index));

		V5B_CpmSetIntEnSec(INT_JPEG, inten);
	}
}
/************************************************************************************************
*								LBUF Interrupt
************************************************************************************************/

void _ISR_V5JpegLbufIntHandle(UINT16 intflg)
{
	UINT8 x, i;

	for(i=0; i<V5_JPEG_LBUF_INT_NUM; i++)
	{
		x = gLbufIntPriority[i];
		if(TSTINT(intflg, x))
		{
			if(gJpegContext.lbufintisr[x])
				gJpegContext.lbufintisr[x]();
		}
	}
}

void V5_JpegLbufIntSetCallback(UINT8 index, PVJpegIsrCallback pcall)
{
	UINT8 i;

	if(JPEG_LBUF_INT_ALL <= index)
	{
		for(i=0; i<JPEG_LBUF_INT_ALL; i++)
		{
			gJpegContext.lbufintisr[i] = pcall;
		}
	}
	else
	{
		gJpegContext.lbufintisr[index] = pcall;
	}
}

void V5_JpegLbufIntEnable(UINT8 index)
{
	UINT16 inten;

	if(JPEG_LBUF_INT_ALL <= index)
	{
		V5B_CpmSetIntEnSec(INT_LBUF, 0xffff);
	}
	else
	{
		inten = V5B_CpmGetIntEnSec(INT_LBUF);
		inten |= (0x1<<index);

		V5B_CpmSetIntEnSec(INT_LBUF, inten);
	}
}

void V5_JpegLbufIntDisable(UINT8 index)
{
	UINT16 inten;

	if(JPEG_LBUF_INT_ALL <= index)
	{
		V5B_CpmSetIntEnSec(INT_LBUF, 0x0);
	}
	else
	{
		inten = V5B_CpmGetIntEnSec(INT_LBUF);
		inten &= (~(0x1<<index));

		V5B_CpmSetIntEnSec(INT_LBUF, inten);
	}
}

/********************************************************************************
  Description: Config timer.
  Parameters:
	mode : 0 - timer restart by host, timer restart auto.
	ms : timer value in millisecond.
  Return:
  Remarks:
*********************************************************************************/
void V5_JpegSetTimerCfg(UINT32 ms)
{
	UINT32 temp;

	temp = V5_CtrlGetMclk();
	temp *= ms;

	V5B_JpegSetTimer(temp);
}

#endif //V578CMD_DISABLE
