#include "driver.h"
#ifndef	V578CMD_DISABLE

#ifdef WIN32
#include "../v568env/v5initenv.h"
#endif

static TMarbContext gMarbContext;

static void MarbSetMemMap(void);
static void MarbSetJbufGap(void);
static void MarbSetTbufGap(void);


void V5_MarbInitContext(void)
{
	memset(&gMarbContext, 0, sizeof(gMarbContext));
	gMarbContext.setting.allocmode = DEFAULT_ALORDER;
	gMarbContext.setting.IndexSize = DEFAULT_INDEXSIZE;
	gMarbContext.sramstart = 0;
	gMarbContext.sramend = V5_SRAM_TOTALSIZE;
	gMarbContext.mode = 0x7f;
	gMarbContext.state = MARB_NOTINITED;
}

void V5_MarbSetInfo(TMarbInfo *info)
{
	//gMarbContext.pinputinfo = info;
	//memcpy(&(gMarbContext.setting), info, sizeof(TMarbInfo));
	gMarbContext.setting = *info;
	gMarbContext.state = MARB_INITED;
}

TMarbContext * V5_MarbGetContext(void)
{
	return &gMarbContext;
}

void V5_MarbClose(void)
{
}

void V5_MarbInitMemClk(void)
{
	UINT32 mclk;

	mclk = V5_CtrlGetMclk();
	mclk >>= 14;
	// mclk/16384 fclk/32767 get 32Khz marb clk
	if(mclk)
		mclk -= 1;
	if(mclk > 0xffff)
		mclk = 0xffff;

	V5B_MarbSetClkDiv((UINT16)mclk);
}

void V5_MarbOpen(void)
{
	V5B_MarbSubReset(0);
	V5_Delay(10);
	V5_MarbInitMemClk();
	MarbSetMemMap();
	V5_MarbIntDisable(MARB_INT_ALL);
	V5_MarbIntSetCallback(MARB_INT_ALL, NULL);
	V5_MarbSlvIntDisable(MARB_SLV_INT_ALL);
	V5_MarbSlvIntSetCallback(MARB_SLV_INT_ALL, NULL);
}

static void MarbMemPowerDown(UINT8 i)
{
	switch( i )
	{
	case 0:
		if(gMarbContext.setting.pwrsave == MARB_POWER_SAVE_CLK)
		{
			V5B_Marb1TCtrlMem0ZzbOff();
			V5B_MarbMem0ClkOff();
		}
		else
			V5B_MarbMem0PowerDown();

		break;
	case 1:
		if(gMarbContext.setting.pwrsave == MARB_POWER_SAVE_CLK)
		{
			V5B_Marb1TCtrlMem1ZzbOff();
			V5B_MarbMem1ClkOff();
		}
		else
			V5B_MarbMem1PowerDown();

		break;
	case 2:
		if(gMarbContext.setting.pwrsave == MARB_POWER_SAVE_CLK)
		{
			V5B_Marb1TCtrlMem2ZzbOff();
			V5B_MarbMem2ClkOff();
		}
		else
			V5B_MarbMem2PowerDown();

		break;
	}
}

static void MarbMemPowerOn(UINT8 i)
{
	switch( i )
	{
	case 0:
		if(gMarbContext.setting.pwrsave == MARB_POWER_SAVE_CLK)
		{
			V5B_Marb1TCtrlMem0ZzbOn();
			V5B_MarbMem0ClkOn();
		}
		else
		{
			V5B_MarbMem0PowerOn();
			//V5B_MarbResetMem0();
		}
		break;
	case 1:
		if(gMarbContext.setting.pwrsave == MARB_POWER_SAVE_CLK)
		{
			V5B_Marb1TCtrlMem1ZzbOn();
			V5B_MarbMem1ClkOn();
		}
		else
		{
			V5B_MarbMem1PowerOn();
			//V5B_MarbResetMem1();
		}
		break;
	case 2:
		if(gMarbContext.setting.pwrsave == MARB_POWER_SAVE_CLK)
		{
			V5B_Marb1TCtrlMem2ZzbOn();
			V5B_MarbMem2ClkOn();
		}
		else
		{
			V5B_MarbMem2PowerOn();
			//V5B_MarbResetMem2();
		}
		break;
	}
}

static void MarbSetMemPower(void)
{
	if(gMarbContext.map.jbufstart == gMarbContext.map.layerA0end)	//only B layer
	{
		if(gMarbContext.map.jbufend <= (V5_1TSRAM_SIZE << 1))
			V5B_MarbMem2PowerDown();
		
		if(gMarbContext.map.layerB0start == gMarbContext.map.layerB1end)
		{
			if(gMarbContext.sramstart == 0)
				MarbMemPowerDown(0);
			else
				MarbMemPowerOn(0);
			if(gMarbContext.map.layerB0start > V5_1TSRAM_SIZE)
				MarbMemPowerOn(1);
		}
		else
		{
			MarbMemPowerOn(0);
			if(gMarbContext.map.layerB1end > V5_1TSRAM_SIZE)
				MarbMemPowerOn(1);
		}
	}
	else
	{
		if(gMarbContext.map.layerA1end <= (V5_1TSRAM_SIZE << 1))
			MarbMemPowerDown(2);
		else
			MarbMemPowerOn(2);
		if( (gMarbContext.map.jbufstart >= (V5_1TSRAM_SIZE << 1)) &&
			(gMarbContext.map.layerB1end <= V5_1TSRAM_SIZE) )
			MarbMemPowerDown(1);
		else
			MarbMemPowerOn(1);
		if( (gMarbContext.map.jbufstart >= V5_1TSRAM_SIZE) &&
			(gMarbContext.map.layerB1end == 0) )
			MarbMemPowerDown(0);
		else
			MarbMemPowerOn(0);
	}
}

static void MarbSetMemMap(void)
{
	PTMarbMap map = &(gMarbContext.map);

	if( (gMarbContext.mode == BYPASS_MODE) || (gMarbContext.mode == THROUGH_MODE) )
		return;
	V5B_LcdcSetGbufBa0(map->layerB0start);
	V5B_LcdcSetGbufBa1(map->layerB1start);
	V5B_LcdcSetVbufBa0(map->layerA0start);
	V5B_LcdcSetVbufBa1(map->layerA1start);

	if(map->jbufend != map->jbufstart)
	{
		V5B_MarbSetJbufStart(map->jbufstart);
		V5B_MarbSetJbufEnd(map->jbufend - 4);
		MarbSetJbufGap();
	}
	//if(map->thumbstart != map->thumbend)
	{
		V5B_MarbSetTbufStart(map->thumbstart);
		V5B_MarbSetTbufEnd(map->thumbend - 4);
		MarbSetTbufGap();
	}

	V5B_MarbSetIdxStart(map->indexstart);
	//if(map->indexstart != map->indexend)
	{
		V5B_MarbSetIdxEnd(map->indexend - 4);
	}

	//if(map->lbuf1start != map->lbuf0start)
		V5B_LbufSetAddrInMarb(map->lbuf0start >> 2);
	MarbSetMemPower();
}

void V5_MarbSetAllocMode(UINT8 mode)
{
	if(mode > TWOFRAME_ONLY)
		mode = TWOFRAME_ONLY;
	gMarbContext.setting.allocmode = mode;
}

UINT8 V5_MarbGetAllocMode(void)
{
	return gMarbContext.setting.allocmode;
}

//=====================================================
static void MarbSetB0layerMem(UINT8 fmt, TSize size)
{
	UINT32 len;

	len = GetBmemSize(fmt, size);
	gMarbContext.map.layerB0start = gMarbContext.sramstart;
	gMarbContext.map.layerB0end = gMarbContext.sramstart + len;
}

static void SetB0layerMem(void)
{
	UINT8 fmt;
	TSize size;

	if( (gMarbContext.mode == BYPASS_MODE) || (gMarbContext.mode == THROUGH_MODE) )
		return;
	fmt = V5_DispGetB0format();
	V5_DispGetB0MemSize(&size);
	MarbSetB0layerMem(fmt, size);
}

static void MarbSetB1layerMem(UINT8 fmt, TSize size)
{
	UINT32 len;

	len = GetBmemSize(fmt, size);
	gMarbContext.map.layerB1start = gMarbContext.map.layerB0end;
	gMarbContext.map.layerB1end = gMarbContext.map.layerB0end + len;
}

static void SetB1layerMem(void)
{
	UINT8 fmt;
	TSize size;

	if( (gMarbContext.mode == BYPASS_MODE) || (gMarbContext.mode == THROUGH_MODE) )
		return;
	fmt = V5_DispGetB1format();
	V5_DispGetB1MemSize(&size);
	MarbSetB1layerMem(fmt, size);
}

static void SetDecodeLbufMem(UINT16 width0, UINT16 width1)
{
	UINT8 fmt;
	UINT32 len;

	fmt = V5B_LbufGetYuvMode();
	len = GetDecLbuf1Size(fmt, width1);
	gMarbContext.map.lbuf1start = gMarbContext.map.lbufend - len;
	gMarbContext.map.lbuf0start = gMarbContext.map.lbufend - len;
	len = GetDecLbuf0Size(fmt, width0);
	gMarbContext.map.lbuf0start -= len;
}

static void SetDecodeIppLbufMem(void)
{
	UINT16 width0, width1;

	width0 = V5B_LbufGetWidth();
	width1 = V5B_IppGetDispWidth();
	SetDecodeLbufMem(width0, width1);
}

static void SetDecodeFrmLbufMem(void)
{
	UINT16 width0, width1;

	width0 = V5B_LbufGetWidth();
	width1 = V5B_LcdcGetAw();
	SetDecodeLbufMem(width0, width1);
}

static void SetDecodeByMem(UINT16 width)
{
	UINT32 len;

	len = GetDecLbuf1Size(V5B_LBUF_YUV_422, width);
	gMarbContext.map.lbuf0start = gMarbContext.map.lbufend - len;
}

static void SetDecodeIppByLbufMem(void)
{
	UINT16 width;

	width = V5B_IppGetDispWidth();
	SetDecodeByMem(width);
}

static void SetDecodeFrmByLbufMem(void)
{
	UINT16 width;

	width = V5B_LcdcGetAw();
	SetDecodeByMem(width);
}

static void SetDisplayLbufMem(void)
{
	UINT16 width;

	width = V5B_LbufGetWidth();
	SetDecodeLbufMem(width, 0);
}

static void LbufCapMem(UINT8 fmt, UINT16 width)
{
	UINT32 len;

	len = GetCapLbuf0Size(fmt, width);
	gMarbContext.map.lbuf1start = gMarbContext.map.lbufend - len;
	gMarbContext.map.lbuf0start = gMarbContext.map.lbuf1start - len;
}

static void SetCapFrmLbufMem(void)
{
	UINT8 fmt;
	UINT16 width;

	fmt = V5B_LbufGetYuvMode();
	width = V5B_LcdcGetAw();
	LbufCapMem(fmt, width);
}

static void SetCapLbufMem(void)
{
	UINT8 fmt;
	UINT16 width;

	fmt = V5B_LbufGetYuvMode();
	width = V5B_IppGetCapWidth();
	LbufCapMem(fmt, width);
}

typedef void (*PLbufMemCall)(void);

static const PLbufMemCall gLbufCall[] =
{
	NULL,				//PREVIEW_MODE,
	SetCapLbufMem,		//PREVIEWBRC_MODE,
	SetCapFrmLbufMem,	//PREVIEWFRAME_MODE,
	SetCapLbufMem,		//CAPTURESTILL_MODE,
	SetCapLbufMem,		//CAPTURESTILLTHUMB_MODE,
	SetCapLbufMem,		//CAPTUREVIDEO_MODE,
	SetCapLbufMem,		//CAPTUREVIDEOTHUMB_MODE,
	SetCapLbufMem,		//CAPTUREAVI_MODE,
	SetCapLbufMem,		//CAPTUREAVITHUMB_MODE,
	SetCapFrmLbufMem,	//CAPTUREFRAME_MODE,
	SetCapFrmLbufMem,	//CAPTUREFRAMEVIDEO_MODE,
	SetCapFrmLbufMem,	//CAPTUREFRAMEAVI_MODE,
	SetCapLbufMem,		//MULTISHOT_MODE,
	SetCapLbufMem,		//MULTISHOTTHUMB_MODE,
	SetDisplayLbufMem,	//DISPLAYSTILL_MODE,
	NULL,				//ENCODE_MODE,
	SetDisplayLbufMem,	//DECODE_MODE,
	SetDecodeIppLbufMem,//DECODEIPP_MODE,
	SetDecodeFrmLbufMem,//DECODEFRAME_MODE,
	SetCapFrmLbufMem,	//MULTISHOTFRAME_MODE,
	SetCapLbufMem,		//CAPTURETHUMB_MODE,
	SetDisplayLbufMem,	//DISPLAYVIDEO_MODE,
	SetDisplayLbufMem,	//DISPLAYAVI_MODE,
	NULL,				//DIRECTDISPLAY_MODE,
	NULL,				//BYPASS_MODE,
	NULL,				//THROUGH_MODE
	SetDecodeIppByLbufMem,//DECODEIPPBYPASS_MODE,
	SetDecodeFrmByLbufMem,//DECODEFRAMEBYPASS_MODE,
	NULL,				 //DISPLAYBYPASS_MODE
	SetCapFrmLbufMem,	//CAPTUREFRAMETHUMB_MODE,
	SetCapFrmLbufMem,	//CAPTUREFRAMEVIDEOTHUMB_MODE,
	SetCapFrmLbufMem	//CAPTUREFRAMEAVITHUMB_MODE
};

static void SetLbufMem(void)
{
	gMarbContext.map.lbuf0start = gMarbContext.map.thumbstart;
	gMarbContext.map.lbuf1start = gMarbContext.map.thumbstart;
	gMarbContext.map.lbufend = gMarbContext.map.thumbstart;
	if(gMarbContext.mode <= CAPTUREFRAMEAVITHUMB_MODE)
	{
		if(gLbufCall[gMarbContext.mode] != NULL)
			gLbufCall[gMarbContext.mode]();
	}
}

static void SetThumbMem(void)
{
	TSize size;
	UINT32 len;

	gMarbContext.map.thumbstart = gMarbContext.map.indexstart;
	gMarbContext.map.thumbend = gMarbContext.map.indexstart;

	if( (gMarbContext.mode == CAPTURESTILLTHUMB_MODE) ||
		(gMarbContext.mode == CAPTUREVIDEOTHUMB_MODE) ||
		(gMarbContext.mode == CAPTURETHUMB_MODE) ||
		(gMarbContext.mode == CAPTUREAVITHUMB_MODE)||
		(gMarbContext.mode == CAPTUREFRAMETHUMB_MODE) ||
		(gMarbContext.mode == CAPTUREFRAMEVIDEOTHUMB_MODE) ||
		(gMarbContext.mode == CAPTUREFRAMEAVITHUMB_MODE))
	{
		V5B_IppGetThumbSize(&size);
		len = GetYuvSize(V5B_LBUF_YUV_422, size);
		gMarbContext.map.thumbstart -= len + 4;
	}

	if(gMarbContext.mode == MULTISHOTTHUMB_MODE)
	{
		V5B_IppGetThumbSize(&size);
		len = GetYuvSize(V5B_LBUF_YUV_422, size);
		len *= gMarbContext.setting.thumbcnt;
		gMarbContext.map.thumbstart -= len + 4;
	}
}

static void SetIndexMem(void)
{
	gMarbContext.map.indexstart = gMarbContext.map.layerA0start;
	gMarbContext.map.indexend = gMarbContext.map.layerA0start;

	if( (gMarbContext.mode == CAPTUREAVI_MODE) ||
		(gMarbContext.mode == CAPTUREAVITHUMB_MODE) ||
		(gMarbContext.mode == CAPTUREFRAMEAVI_MODE) )
		gMarbContext.map.indexstart -= gMarbContext.setting.IndexSize;
}

const UINT8 gAlayerFmt[] = { ALAYER_LINEBUF, ALAYER_1FRAMEBUF, ALAYER_2FRAMEBUF };

static void SetALayerMem(UINT8 fmt)
{
	TSize size;
	UINT32 len;

	V5B_IppGetDispSize(&size);
	len = GetAmemSize(gAlayerFmt[fmt], size);
	gMarbContext.map.layerA1start = gMarbContext.map.layerA1end - (len >> 1);
	gMarbContext.map.layerA0end = gMarbContext.map.layerA1start;
	gMarbContext.map.layerA0start = gMarbContext.map.layerA1end - len;
	V5B_LcdcSetAlayerFmt(gAlayerFmt[fmt]);
}

static void SetCapLayerAMem(void)
{
//	UINT8 fmt;

//	fmt = GetMinAlayerFmt();
	SetALayerMem(gMarbContext.setting.allocmode);
}

static void SetDispLayerAMem(void)
{
//	UINT8 fmt;

//	fmt = GetMinAlayerFmt();
	SetALayerMem(gMarbContext.setting.allocmode);
}

static void SetDecLayerAMem(void)
{
	UINT8 fmt;
	TSize size;
	UINT32 len;

	V5B_IppGetDispSize(&size);
	fmt = V5B_LbufGetYuvMode();
	len = GetDecAmemSize(fmt, size);
	gMarbContext.map.layerA0start = gMarbContext.map.layerA1end - len;
	V5B_LcdcSetAlayerFmt(ALAYER_1FRAMEBUF);
}

static void SetPreviewAMem(void)
{
	SetALayerMem(gMarbContext.setting.allocmode);
}

typedef void (*PJbufMemCall)(void);

static const PJbufMemCall gLayerACall[] =
{
	SetPreviewAMem,		//PREVIEW_MODE,
	SetPreviewAMem,		//PREVIEWBRC_MODE,
	SetPreviewAMem,		//PREVIEWFRAME_MODE,
	SetCapLayerAMem,	//CAPTURESTILL_MODE,
	SetCapLayerAMem,	//CAPTURESTILLTHUMB_MODE,
	SetCapLayerAMem,	//CAPTUREVIDEO_MODE,
	SetCapLayerAMem,	//CAPTUREVIDEOTHUMB_MODE,
	SetCapLayerAMem,	//CAPTUREAVI_MODE,
	SetCapLayerAMem,	//CAPTUREAVITHUMB_MODE,
	SetCapLayerAMem,	//CAPTUREFRAME_MODE,
	SetCapLayerAMem,	//CAPTUREFRAMEVIDEO_MODE,
	SetCapLayerAMem,	//CAPTUREFRAMEAVI_MODE,
	SetCapLayerAMem,	//MULTISHOT_MODE,
	SetCapLayerAMem,	//MULTISHOTTHUMB_MODE,
	SetDispLayerAMem,	//DISPLAYSTILL_MODE,
	SetCapLayerAMem,	//ENCODE_MODE,
	SetCapLayerAMem,	//DECODE_MODE,
	SetCapLayerAMem,	//DECODEIPP_MODE,
	SetDecLayerAMem,	//DECODEFRAME_MODE,
	SetCapLayerAMem,	//MULTISHOTFRAME_MODE,
	SetPreviewAMem,		//CAPTURETHUMB_MODE,
	SetDispLayerAMem,	//DISPLAYVIDEO_MODE,
	SetDispLayerAMem,	//DISPLAYAVI_MODE,
	NULL,				//DIRECTDISPLAY_MODE,
	NULL,				//BYPASS_MODE,
	NULL,				//THROUGH_MODE
	SetCapLayerAMem,	//DECODEIPPBYPASS_MODE,
	SetDecLayerAMem,		//DECODEFRAMEBYPASS_MODE,
	SetDispLayerAMem,	//DISPLAYBYPASS_MODE
	SetCapLayerAMem,	//CAPTUREFRAMETHUMB_MODE,
	SetCapLayerAMem,	//CAPTUREFRAMEVIDEOTHUMB_MODE,
	SetCapLayerAMem		//CAPTUREFRAMEAVITHUMB_MODE
};

static void SetLayerAMem(void)
{
	gMarbContext.map.layerA1end = gMarbContext.sramend;
	gMarbContext.map.layerA1start = gMarbContext.sramend;
	gMarbContext.map.layerA0end = gMarbContext.sramend;
	gMarbContext.map.layerA0start = gMarbContext.sramend;
	if(gMarbContext.mode <= CAPTUREFRAMEAVITHUMB_MODE)
	{
		if(gLayerACall[gMarbContext.mode] != NULL)
			gLayerACall[gMarbContext.mode]();
	}
}

static const UINT8 gMarbConfig[][2] =
{
	{ 0xb8, 0x4 },	//PREVIEW_MODE
	{ 0xb8, 0x4 },	//PREVIEWBRC_MODE
	{ 0xb8, 0x4 },	//PREVIEWFRAME_MODE
	{ 0xb8, 0x4 },	//CAPTURESTILL_MODE
	{ 0xb0, 0x4 },	//CAPTURESTILLTHUMB_MODE
	{ 0xba, 0x4 },	//CAPTUREVIDEO_MODE
	{ 0xb2, 0x4 },	//CAPTUREVIDEOTHUMB_MODE
	{ 0x8a, 0x4 },	//CAPTUREAVI_MODE
	{ 0x82, 0x4 },	//CAPTUREAVITHUMB_MODE
	{ 0xb8, 0x4 },	//CAPTUREFRAME_MODE
	{ 0xba, 0x4 },	//CAPTUREFRAMEVIDEO_MODE
	{ 0x8a, 0x4 },	//CAPTUREFRAMEAVI_MODE
	{ 0xb9, 0x4 },	//MULTISHOT_MODE
	{ 0xb1, 0x4 },	//MULTISHOTTHUMB_MODE
	{ 0xbc, 0x4 },	//DISPLAYSTILL_MODE
	{ 0xb8, 0x4 },	//ENCODE_MODE
	{ 0xbc, 0x4 },	//DECODE_MODE
	{ 0xbc, 0x4 },	//DECODEIPP_MODE
	{ 0xbc, 0x4 },	//DECODEFRAME_MODE
	{ 0xb9, 0x4 },	//MULTISHOTFRAME_MODE
	{ 0xf0, 0x4 },	//CAPTURETHUMB_MODE
	{ 0xbc, 0x4 },	//DISPLAYVIDEO_MODE
	{ 0xbc, 0x4 },	//DISPLAYAVI_MODE
	{ 0xb8, 0x4 },	//DIRECTDISPLAY_MODE
	{ 0xb8, 0x4 },	//BYPASS_MODE
	{ 0xb8, 0x4 },	//THROUGH_MODE
	{ 0xbc, 0x4 },	//DECODEIPPBYPASS_MODE
	{ 0xbc, 0x4 },	//DECODEFRAMEBYPASS_MODE
	{ 0xbc, 0x4 },	//DISPLAYBYPASS_MODE
	{ 0xb0, 0x4 },	//CAPTUREFRAMETHUMB_MODE,
	{ 0xb2, 0x4 },	//CAPTUREFRAMEVIDEOTHUMB_MODE,
	{ 0x82, 0x4 }	//CAPTUREFRAMEAVITHUMB_MODE
};

static void ConfigMarb(void)
{
	UINT8 x;

	if( (gMarbContext.mode == BYPASS_MODE) || (gMarbContext.mode == THROUGH_MODE) )
	{
		x = V5B_MarbGetMemCtrl();
		V5B_MarbSetMemCtrl(x & 0x30);
		return;
	}
	if(gMarbContext.mode > CAPTUREFRAMEAVITHUMB_MODE)
		return;
	V5B_MarbSetModeCtrl0(gMarbConfig[gMarbContext.mode][0]);
	V5B_MarbSetModeCtrl1(gMarbConfig[gMarbContext.mode][1]);
}

void V5_MarbSetMode(UINT8 mode)
{
	if(gMarbContext.mode == mode)
		return;
	gMarbContext.mode = mode;
	ConfigMarb();
}

static void SetJbufMem(void)
{
	gMarbContext.map.jbufend = gMarbContext.map.lbuf0start;
	gMarbContext.map.jbufstart = gMarbContext.map.layerB1end;
	if( (gMarbContext.mode == PREVIEW_MODE) ||
		(gMarbContext.mode == PREVIEWBRC_MODE) ||
		(gMarbContext.mode == PREVIEWFRAME_MODE) ||
		(gMarbContext.mode == BYPASS_MODE) ||
		(gMarbContext.mode == THROUGH_MODE) )
		gMarbContext.map.jbufstart = gMarbContext.map.lbuf0start;
}

void V5_MarbAutoSetMem(void)
{
	SetB0layerMem();
	SetB1layerMem();
	SetLayerAMem();
	SetIndexMem();
	SetThumbMem();
	SetLbufMem();
	SetJbufMem();
	MarbSetMemMap();
	V5B_LcdcResetState();
}

void	V5_MarbSetJbufStartAdr(UINT32 adr)
{
	if( (gMarbContext.map.jbufend > adr) && (gMarbContext.map.jbufstart < adr) )
	{
		gMarbContext.map.jbufstart = adr;
		V5B_MarbSetJbufStart(adr);
	}
}

void V5_MarbSetIndexMem(UINT32 len)
{
	gMarbContext.setting.IndexSize = len;
}

void V5_MarbSetGapRatio(UINT8 ratio)
{
	ratio = (ratio == 0) ? DEFAULT_GAPRATIO : 
			(ratio > MAX_GAPRATIO) ? MAX_GAPRATIO : ratio;
	gMarbContext.setting.gapratio = ratio;
	MarbSetJbufGap();
	MarbSetTbufGap();
}

UINT8 V5_MarbGetMode(void)
{
	return gMarbContext.mode;
}

UINT32 V5_MarbGetIndexMem(void)
{
	return gMarbContext.setting.IndexSize;
}

UINT8	V5_MarbGetGapRatio(void)
{
	return gMarbContext.setting.gapratio;
}

//=============================


//===================================
static UINT32 MarbReadBuf(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 end, UINT32 pos)
{
	UINT32 size, len1;

	while(len)
	{
		size = end - pos;
		len1 = (len > size) ? size : len;
#ifdef WIN32
		Sram2Gbuf(pos, len1);
#else
		V5_ReadSram(pos, pbuf, len1);
#endif
		pbuf += len1;
		len -= len1;
		pos += len1;
		pos = (pos == end) ? start : pos;
	}
	return pos;
}

static UINT32 MarbWriteBuf(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 end, UINT32 pos)
{
	UINT32 size, len1;

	while(len)
	{
		size = end - pos;
		len1 = (len > size) ? size : len;
		V5_WriteSram(pos, pbuf, len1);
		pbuf += len1;
		len -= len1;
		pos += len1;
		pos = (pos == end) ? start : pos;
	}
	return pos;
}

UINT32 V5_MarbWriteSpanBuf(UINT8 *pbuf, UINT32 len, UINT32 adr, UINT32 span, UINT32 size)
{
	if(span == size)
		return V5_WriteSram(adr, pbuf, len);
	while(len)
	{
		size = len > size ? size : len;
		V5_WriteSram(adr, pbuf, size);
		adr += span;
		len -= size;
		pbuf += size;
	}
	return len;
}

UINT32	V5_MarbWriteSpanBufRepeat(UINT32 val, UINT32 len, UINT32 adr, UINT32 span, UINT32 size)
{
	if(span == size)
		return V5_WriteSramOnWord(adr, val, len);
	while(len)
	{
		size = len > size ? size : len;
		V5_WriteSramOnWord(adr, val, size);
		adr += span;
		len -= size;
	}
	return len;
}

UINT32 V5_MarbWriteSpanBufRev(UINT8 *pbuf, UINT32 len, UINT32 adr, UINT32 span, UINT32 size)
{
	if(span == size)
		return V5_WriteSramRev(adr, pbuf, len);
	while(len)
	{
		size = len > size ? size : len;
		V5_WriteSramRev(adr, pbuf, size);
		adr += span;
		len -= size;
		pbuf += size;
	}
	return len;
}

UINT32 V5_MarbReadSpanBuf(UINT8 *pbuf, UINT32 len, UINT32 adr, UINT32 span, UINT32 size)
{
	if(span == size)
		return V5_ReadSram(adr, pbuf, len);
	while(len)
	{
		size = len > size ? size : len;
		V5_ReadSram(adr, pbuf, size);
		adr += span;
		len -= size;
		pbuf += size;
	}
	return len;
}

UINT32 V5_MarbGetJbufRWgap(void)
{
	UINT32 start, end, rp, wp;

	start = gMarbContext.map.jbufstart;
	end = gMarbContext.map.jbufend;

	if(V5B_MarbGetModeCtrl0() & V5B_MARB_MODECTRL_DECODE)
	{
		rp = V5B_MarbGetJbufPointer();
		wp = gMarbContext.jbufpos;
		if(wp < rp) //(rp - wp) > 4
			return rp - wp - 4;
		return end - wp + rp - start -4;
	}
	switch(gMarbContext.mode)
	{
	case CAPTUREAVI_MODE:
	case CAPTUREAVITHUMB_MODE:
	case CAPTUREFRAMEAVI_MODE:

		wp = V5B_MarbGetLstJbufPointer() + 4;

		break;
	default:
		wp = V5B_MarbGetJbufPointer();
		break;
	}

	rp = gMarbContext.jbufpos;
	if(rp <= wp)
		return wp - rp;
	return end - rp + wp - start;
}

UINT32 V5_MarbJbufRead(UINT8 *pbuf, UINT32 len)
{
	UINT32 start, end, pos;

	end = gMarbContext.map.jbufend;
	start = gMarbContext.map.jbufstart;
	pos = gMarbContext.jbufpos;
#ifdef WIN32
	SetGbufMode(JBUF_MODE);
#endif
	gMarbContext.jbufpos = MarbReadBuf(pbuf, len, start, end, pos);
	return len;
}

UINT32 V5_MarbJbufWrite(UINT8 *pbuf, UINT32 len)
{
	UINT32 start, end, pos;

	end = gMarbContext.map.jbufend;
	start = gMarbContext.map.jbufstart;
	pos = gMarbContext.jbufpos;

	gMarbContext.jbufpos = MarbWriteBuf(pbuf, len, start, end, pos);
	return len;
}

UINT32 V5_MarbGetThumbRWgap(void)
{
	UINT32 start, end, rp, wp;//, size;

	start = gMarbContext.map.thumbstart;
	end = gMarbContext.map.thumbend;
//	size = end - start;
	wp = V5B_MarbGetTbufPointer();
	rp = gMarbContext.thumbpos;
	if(rp <= wp)
		rp = wp - rp;
	else
		rp = end - rp + wp - start;
	return rp;
//	return (rp > size) ? size : rp;
}

UINT32 V5_MarbThumbRead(UINT8 *pbuf, UINT32 len)
{
	UINT32 start, end, pos;

	end = gMarbContext.map.thumbend;
	start = gMarbContext.map.thumbstart;
	pos = gMarbContext.thumbpos;

#ifdef WIN32
	SetGbufMode(TBUF_MODE);
#endif
	gMarbContext.thumbpos = MarbReadBuf(pbuf, len, start, end, pos);
	return len;
}

UINT32 V5_MarbIndexRead(UINT8 *pbuf, UINT32 len)
{
	return V5_ReadSram(gMarbContext.map.indexstart, pbuf, len);
}

UINT32 V5_MarbLbuf0Read(UINT8 *pbuf, UINT32 len)
{
	MarbReadBuf(pbuf, len, gMarbContext.map.lbuf0start, 
		gMarbContext.map.lbuf1start, gMarbContext.map.lbuf0start);
	return len;
//	return V5_ReadSram(gMarbContext.map.lbuf0start, pbuf, len);
}

UINT32 V5_MarbLbuf1Read(UINT8 *pbuf, UINT32 len)
{
	MarbReadBuf(pbuf, len, gMarbContext.map.lbuf1start, 
		gMarbContext.map.lbufend, gMarbContext.map.lbuf1start);
	return len;
//	return V5_ReadSram(gMarbContext.map.lbuf1start, pbuf, len);
}

UINT32	V5_MarbB0Write(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size)
{
	UINT32 adr;

	adr = gMarbContext.map.layerB0start + start;
	return V5_MarbWriteSpanBuf(pbuf, len, adr, span, size);
}

UINT32	V5_MarbB0WriteRepeat(UINT32 val, UINT32 len, UINT32 start, UINT32 span, UINT32 size)
{
	UINT32 adr;

	adr = gMarbContext.map.layerB0start + start;
	return V5_MarbWriteSpanBufRepeat(val, len, adr, span, size);
}

UINT32	V5_MarbB0WriteRev(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size)
{
	UINT32 adr;

	adr = gMarbContext.map.layerB0start + start;
	return V5_MarbWriteSpanBufRev(pbuf, len, adr, span, size);
}

UINT32	V5_MarbB1Write(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size)
{
	UINT32 adr;

	adr = gMarbContext.map.layerB1start + start;
	return V5_MarbWriteSpanBuf(pbuf, len, adr, span, size);
}

UINT32	V5_MarbB0Read(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size)
{
	UINT32 adr;

	adr = gMarbContext.map.layerB0start + start;
	return V5_MarbReadSpanBuf(pbuf, len, adr, span, size);
}

UINT32	V5_MarbB1Read(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size)
{
	UINT32 adr;

	adr = gMarbContext.map.layerB1start + start;
	return V5_MarbReadSpanBuf(pbuf, len, adr, span, size);
}

UINT32	V5_MarbGetB0StartAddr(void)
{
	return gMarbContext.map.layerB0start;
}

UINT32	V5_MarbGetB1StartAddr(void)
{
	return gMarbContext.map.layerB1start;
}

static void MarbSetJbufGap(void)
{
	UINT32 size, len;
	UINT8 fmt, tcr;
	TSize capsize;

	size = gMarbContext.map.jbufend - gMarbContext.map.jbufstart;
	size *= gMarbContext.setting.gapratio;
	size /= MAX_GAPRATIO;

	V5B_MarbSetJbufUp(size + MARB_GAPINTERVAL);
	V5B_MarbSetJbufLow(size - MARB_GAPINTERVAL);

	fmt = V5B_LbufGetYuvMode();
	V5B_IppGetCapSize(&capsize);
	tcr = V5B_JpegGetTCR();
	len = CalCapFrmSize(fmt, capsize, tcr);
	if(len == 0)
		len = 1;
	len = size / len;
	if(len > 0xff)
		len = 0xff;
	if(len == 0)
		len = 1;
	V5B_MarbSetFrameInterval( (UINT8)len );
}

static void MarbSetTbufGap(void)
{
	UINT32 size;

	size = gMarbContext.map.thumbend - gMarbContext.map.thumbstart;
	size *= gMarbContext.setting.gapratio;
	size /= MAX_GAPRATIO;
	V5B_MarbSetTbufUp(size + MARB_GAPINTERVAL);
	V5B_MarbSetTbufLow(size - MARB_GAPINTERVAL);
}

void V5_MarbGetMap(TMarbMap *map)
{
	*map = gMarbContext.map;
}

void V5_MarbStartCapture(void)
{
	V5B_MarbResetIpp();
	V5B_MarbResetJpeg();
	gMarbContext.jbufpos = gMarbContext.map.jbufstart;
	gMarbContext.thumbpos = gMarbContext.map.thumbstart;
	V5B_MarbStartCapture();
}

void V5_MarbStartDecoder(void)
{
	V5B_MarbResetIpp();
	V5B_MarbResetJpeg();
	gMarbContext.jbufpos = gMarbContext.map.jbufstart;
	gMarbContext.thumbpos = gMarbContext.map.thumbstart;
	V5B_MarbStartDecoder();
}

void V5_MarbSetMemStart(UINT32 adr)
{
	gMarbContext.sramstart = adr;
	if( (adr > V5_1TSRAM_SIZE) || (adr > gMarbContext.sramend - V5_1TSRAM_SIZE) )
		gMarbContext.sramstart = 0;
}

UINT32 V5_MarbGetMemStart(void)
{
	return gMarbContext.sramstart;
}

void V5_MarbSetMemEnd(UINT32 adr)
{
	gMarbContext.sramend = adr;
	if( (adr > V5_SRAM_TOTALSIZE) || (adr < gMarbContext.sramstart + V5_1TSRAM_SIZE) )
		gMarbContext.sramend = V5_SRAM_TOTALSIZE;
}

UINT32 V5_MarbGetMemEnd(void)
{
	return gMarbContext.sramend;
}

/************************************************************************************************
*MARB Interrupt
************************************************************************************************/

static const UINT8 gMarbIntPriority[V5_MARB_INT_NUM]	= 
{ 
	INT_JBUF_ERR, INT_THUMB_ERR, INT_JBUF_INTV, INT_JBUF_FIFOCNT, INT_JPEG_DONE, INT_TBUF_FIFOCNT, 
        INT_THUMB_DONE, INT_TBUF_INTV
};


void _ISR_V5MarbIntHandle(UINT16 intflg)
{
	UINT8 x, i;

	for(i=0; i<V5_MARB_INT_NUM; i++)
	{
		x = gMarbIntPriority[i];
		if(TSTINT(intflg, x))
		{
			if(gMarbContext.marbintisr[x])
				gMarbContext.marbintisr[x]();
		}
	}
}

void V5_MarbIntSetCallback(UINT8 index, PVJpegIsrCallback pcall)
{
	UINT8 i;

	if(V5_MARB_INT_NUM <= index)
	{
		for(i=0; i<V5_MARB_INT_NUM; i++)
		{
			gMarbContext.marbintisr[i] = pcall;
		}
	}
	else
	{
		gMarbContext.marbintisr[index] = pcall;
	}
}

void V5_MarbIntEnable(UINT8 index)
{
	UINT16 inten;

	if(V5_MARB_INT_NUM <= index)
	{
		V5B_CpmSetIntEnSec(INT_MARB0, 0xffff);
	}
	else
	{
		inten = V5B_CpmGetIntEnSec(INT_MARB0);
		inten |= (0x1<<index);

		V5B_CpmSetIntEnSec(INT_MARB0, inten);
	}
}

void V5_MarbIntDisable(UINT8 index)
{
	UINT16 inten;

	if(V5_MARB_INT_NUM <= index)
	{
		V5B_CpmSetIntEnSec(INT_MARB0, 0x0);
	}
	else
	{
		inten = V5B_CpmGetIntEnSec(INT_MARB0);
		inten &= (~(0x1<<index));

		V5B_CpmSetIntEnSec(INT_MARB0, inten);
	}
}

/************************************************************************************************
*MARB Slave Interrupt
************************************************************************************************/

static const UINT8 gMarbSlvIntPriority[V5_MARB_SLV_INT_NUM]	= 
{ 
	INT_RECAP_ERR, INT_RECAP, INT_MEM0_REF_ERR, INT_MEM1_REF_ERR, INT_MEM2_REF_ERR
};


void _ISR_V5MarbSlvIntHandle(UINT16 intflg)
{
	UINT8 x, i;

	for(i=0; i<V5_MARB_SLV_INT_NUM; i++)
	{
		x = gMarbSlvIntPriority[i];
		if(TSTINT(intflg, x))
		{
			if(gMarbContext.marbslvintisr[x])
				gMarbContext.marbslvintisr[x]();
		}
	}
}

void V5_MarbSlvIntSetCallback(UINT8 index, PVJpegIsrCallback pcall)
{
	UINT8 i;

	if(V5_MARB_SLV_INT_NUM <= index)
	{
		for(i=0; i<V5_MARB_SLV_INT_NUM; i++)
		{
			gMarbContext.marbslvintisr[i] = pcall;
		}
	}
	else
	{
		gMarbContext.marbslvintisr[index] = pcall;
	}
}

void V5_MarbSlvIntEnable(UINT8 index)
{
	UINT16 inten;

	if(V5_MARB_SLV_INT_NUM <= index)
	{
		V5B_CpmSetIntEnSec(INT_MARB1, 0xffff);
	}
	else
	{
		inten = V5B_CpmGetIntEnSec(INT_MARB1);
		inten |= (0x1<<index);

		V5B_CpmSetIntEnSec(INT_MARB1, inten);
	}
}

void V5_MarbSlvIntDisable(UINT8 index)
{
	UINT16 inten;

	if(V5_MARB_SLV_INT_NUM <= index)
	{
		V5B_CpmSetIntEnSec(INT_MARB1, 0x0);
	}
	else
	{
		inten = V5B_CpmGetIntEnSec(INT_MARB1);
		inten &= (~(0x1<<index));

		V5B_CpmSetIntEnSec(INT_MARB1, inten);
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
void V5_MarbSetTimerCfg(UINT8 mode, UINT32 ms)
{
	UINT32 temp;
	UINT8 num;
	UINT16 base;

	if(mode)
		V5B_MarbTimerRstAuto();
	else
		V5B_MarbTimerRstHost();

	temp = V5_CtrlGetMclk();
	num = (UINT8)(temp / 0xffff + 1);
	base = (UINT16)(temp/num);
	ms *= num;
	if(ms > 0xffffff)
		ms = 0xffffff;

	V5B_MarbSetTimerBase(base);
	V5B_MarbSetTimerVal(ms);
}

/********************************************************************************
  Description: Get current timer value.
  Parameters:
  Return:
	The timer value in millisecond.
  Remarks:
*********************************************************************************/
UINT32 V5_MarbGetTimerVal(void)
{
	UINT32 temp, ms;
	UINT8 num;

	ms = V5B_MarbGetCurTimerVal();
	temp = V5_CtrlGetMclk();
	num = (UINT8)(temp / 0xffff + 1);
	ms /= num;

	return ms;
}


#endif //V578CMD_DISABLE
