
#include "internal.h"
#ifndef	V578CMD_DISABLE

#include "../driver/basefunc/basefunc.h"
#include "../driver/driver.h"

#include "../../s3c44box/s3c44box.h"
#include "../v568env/v5initenv.h"

#define _DEBUG_DISPLAY

#ifndef NCSC
#define DEC_REQTIME 0x200
#else
#define DEC_REQTIME 0x1000
#endif

#ifdef WIN32 
#define FUNC_REQ_TIME	0x100
#else
#define FUNC_REQ_TIME	0x2710 // 10 s
#endif
#define FIFO_DELAY	1

#define	LEN_DATA_PACKET  0x40000

#define BufInvalid(pbuf) ( (pbuf == NULL) || (pbuf->pData == NULL) || (pbuf->Len == 0) )
#define DataInvalid(pbuf) ( (pbuf->pData == NULL) || (pbuf->Len == 0) || (pbuf->Pos >= pbuf->Len) )

static THJpegContext gHJpegContext;

/*=========================================================================//
						Static functions
//=========================================================================*/
static void InitCapEncInt(UINT8 type);
static void JpegCapEncReq(void);
static UINT8 LbufIsOk(UINT8 bYuv);
static UINT8 DecIsOkErr(void);
static void ReadDecDataIppLcd(UINT16 jpgheight, UINT8 yuvtype, TSize size1, PTDataBuf pdst, UINT8 bYuv);
static void ReadDecData(UINT8 YUVType, TSize size1, PTDataBuf pdst);
static void WriteDecodeJpgData(UINT8 bDisp);
static void WriteDecodeYuvData(UINT8 bDisp);
static void SendYuvData(UINT8 *pbuf, UINT32 len);
static void JpegDecode(PTDataBuf psrc, PTDataBuf pdst, UINT8 type);
static void JpegDeocdeYuv(PTDataBuf psrc, PTDataBuf pdst, TSize size, UINT8 type);
static void JpegCallback(UINT8 msg, TDataBuf *dbuf);
static void LbufRead(UINT8* pbuf, UINT32 len, UINT8 num);
static void JpegDisplayFrame(PTJpegIndex pinfo, PTDataBuf psrc);

#ifdef WIN32 //for debug
static void JpegMapMem(PTDataBuf pjbuf, PTDataBuf ptbuf, PTDataBuf pibuf);
static void JpegSetMapAdr(PTDataBuf ptbuf, PTDataBuf pibuf);
#endif

static void	_ISR_JbufFifoCnt(void);
static void	_ISR_JpegDone(void);
static void	_ISR_JbufError(void);
static void	_ISR_JpegRecapError(void);
static void _ISR_JbufIntv(void);
static void	_ISR_TbufFifoCnt(void);
static void	_ISR_ThmbDone(void);
static void	_ISR_ThmbError(void);
static void	_ISR_DecDone(void);
static void	_ISR_DecError(void);


static void CapEncJbufFifoCnt(UINT8 bEnc, UINT32 gapsize); //bEnc 0 cap, 1 enc
static void CapTbufFifoCnt(UINT32 gapsize);
static void CapIndexReadData(UINT32 gapsize);
static void DispJbufFifoCnt(UINT32 gapleftsize);
static void WriteLstJpg(UINT8* pbuf, UINT32 length);




/*=========================================================================//
						Global functions & structure
//=========================================================================*/
//static THJpegContext gHJpegContext;

void V5_HJpegOpen(void)
{
	//type. default request
	V5_HJpegSetIntReq(1);
	//callback. default use JpegCallback function.
	V5_HJpegSetCallBack(JpegCallback);
}

THJpegContext * V5_HJpegGetContext(void)
{
	return &gHJpegContext;
}

void V5_HJpegSetCallBack(PJpegCallBack pcall)
{
	gHJpegContext.pcall = pcall;
}

void V5_HJpegSetIntReq(UINT8 intreq)
{
	gHJpegContext.intreq = intreq;

	if(intreq)
	{
		V5_CtrlIntSetCallback(INT_MARB0, NULL);
		V5_CtrlIntDisable(INT_MARB0);
	}
	else
	{
		V5_CtrlIntSetCallback(INT_MARB0, _ISR_V5MarbIntHandle);
		V5_CtrlIntEnable(INT_MARB0);
	}
}

UINT8 V5_HJpegGetIntReq(void)
{
	return gHJpegContext.intreq;
}

/******************************************************************************
	type :	0 - norm
			1 - sif
			2 - repeat
*******************************************************************************/
static void Capture(PTDataBuf pjbuf, PTDataBuf ptbuf, PTDataBuf pibuf, UINT8 type)
{
	memset(&gHJpegContext.fucbuf, 0, sizeof(THJpegFucBuf));
	gHJpegContext.fucbuf.cap.pjbuf = pjbuf;
	gHJpegContext.fucbuf.cap.ptbuf = ptbuf;
	gHJpegContext.fucbuf.cap.pibuf = pibuf;

	#ifdef WIN32 //for debug
	JpegSetMapAdr(ptbuf, pibuf);
	#endif

	InitCapEncInt(type);

	V5B_MarbStopVidHost();
	switch(type)
	{
	case JPEG_CAP_NORM:
		V5_JpegCapture();
		break;
	case JPEG_CAP_SIF:
		V5_JpegSifCapture();
		break;
	case JPEG_CAP_REP:
		V5_JpegReCapture();
		break;
	case JPEG_CAP_SOFT_REP:
		V5_JpegCapture();
		break;
	case JPEG_CAP_TIME:
		V5B_MarbStopVidTimer();
		V5_JpegCapture();
		break;

	}

	JpegCapEncReq();

	#ifdef WIN32 //for debug
	JpegMapMem(pjbuf, ptbuf, pibuf);
	#endif
}

void V5_HJpegCap(PTDataBuf pjbuf, PTDataBuf ptbuf, PTDataBuf pibuf)
{
	Capture(pjbuf, ptbuf, pibuf, JPEG_CAP_NORM);
}

#define RECAP_THD	3

void V5_HJpegReCap(PTDataBuf pjbuf, PTDataBuf ptbuf, UINT8* cnt)
{
	TSize size, size1;
	UINT8 fmt, tcr;
	UINT32 len, jbufsize;
	
	fmt = V5B_LbufGetYuvMode();
	V5B_JpegGetImgSize(&size);
	jbufsize = V5B_MarbGetJbufSize();

	GetJpegSize(fmt, size, &size1);
	len = GetYuvSize(fmt, size1);
	tcr = V5B_JpegGetTCR();
	if(tcr < 5)
		tcr = 5;

	len /= ((tcr-1)>>2);
	len += (len >> RECAP_THD);

	gHJpegContext.recapcnt = *cnt;
	gHJpegContext.recapactcnt = 0;
	if(len <= jbufsize)
	{
		V5B_MarbSetRpThd(*cnt);
		V5B_MarbSetRpSize(len);
		Capture(pjbuf, ptbuf, NULL, JPEG_CAP_REP);
		*cnt = V5B_MarbGetRpActTimes();
	}
	else
	{
		Capture(pjbuf, ptbuf, NULL, JPEG_CAP_SOFT_REP);
		*cnt = gHJpegContext.recapactcnt;
	}

}

void V5_HJpegSifCap(PTDataBuf pjbuf, PTDataBuf ptbuf, PTDataBuf pibuf)
{
	Capture(pjbuf, ptbuf, pibuf, JPEG_CAP_SIF);
}

void V5_HJpegCapVidTime(PTDataBuf pjbuf, PTDataBuf ptbuf, PTDataBuf pibuf, UINT32 ms)
{
	V5_MarbSetTimerCfg(0, ms);
	Capture(pjbuf, ptbuf, pibuf, JPEG_CAP_TIME);
}

void V5_HJpegEnc(PTDataBuf psrc, PTDataBuf pdst, UINT8 fmt, TSize size)
{
	TSize size1;
	UINT32 len;

	memset(&gHJpegContext.fucbuf, 0, sizeof(THJpegFucBuf));
	gHJpegContext.fucbuf.enc.psrc = psrc;
	gHJpegContext.fucbuf.enc.pdst = pdst;


	GetJpegSize(fmt, size, &size1);
	len = GetYuvSize(fmt, size1);
	if(psrc->Len < len)
		return;

	#ifdef WIN32 //for debug
	JpegSetMapAdr(NULL, NULL);
	#endif

	InitCapEncInt(JPEG_CAP_NORM);

	V5_JpegEncode(fmt, size);

	SendYuvData(psrc->pData, len);

	JpegCapEncReq();

#ifdef WIN32 //for debug
	JpegMapMem(pdst, NULL, NULL);
#endif

}

static void DecodeReq(void)
{
	UINT16 flg = DEC_REQTIME;

	while(flg--)
	{
		if(DecIsOkErr())
			break;
		WriteDecodeJpgData(1);
		V5_Delay(1);
	}
}

static void JpegDecReq(void)
{
	UINT16 intflg0, intflg1;

	//wait for done signal
	while(gHJpegContext.decdone--)
	{
		//Request
		if(gHJpegContext.intreq)
		{
			intflg0 = V5B_MarbGetMarbStatus();
			//intflg0 = V5B_CpmGetIntFlagSec(INT_MARB0);
			intflg1 = V5B_CpmGetIntFlagSec(INT_JPEG);

			if(intflg0 || intflg1)
			{
				if(intflg0)
					_ISR_V5MarbIntHandle(intflg0);
				if(intflg1)
					_ISR_V5JpegJpegIntHandle(intflg1);
			}
			else
			{
				V5_Delay(FIFO_DELAY);
			}
		}
		else {
			V5_Delay(FIFO_DELAY);
		}
	}
}

static void JpegDecVidReq(void)
{
	UINT16 intflg0, intflg1;

	//wait for done signal
	while(gHJpegContext.decdone--)
	{
		//Request
		if(gHJpegContext.intreq)
		{
			intflg0 = V5B_MarbGetMarbStatus();
			//intflg0 = V5B_CpmGetIntFlagSec(INT_MARB0);
			intflg1 = V5B_CpmGetIntFlagSec(INT_JPEG);

			if(intflg0 || intflg1)
			{
				if(intflg0)
					_ISR_V5MarbIntHandle(intflg0);
				if(intflg1)
					_ISR_V5JpegJpegIntHandle(intflg1);
			}
			else
			{
				V5_Delay(FIFO_DELAY);
			}
		}
		else {
			V5_Delay(FIFO_DELAY);
		}
		if( DataInvalid(gHJpegContext.fucbuf.disp.pjbuf) )
			break;
	}
}


static void JpegDisplayFrame(PTJpegIndex pinfo, PTDataBuf psrc)
{
	memset(&gHJpegContext.fucbuf, 0, sizeof(THJpegFucBuf));

	gHJpegContext.fucbuf.disp.pjbuf = psrc;
	gHJpegContext.fucbuf.disp.pjbuf->Pos = pinfo->offset;
	gHJpegContext.fucbuf.disp.pjbuf->Len = pinfo->eop;

	gHJpegContext.decdone = FUNC_REQ_TIME;

	V5_MarbIntSetCallback(INT_JBUF_FIFOCNT, _ISR_JbufFifoCnt);
	V5_JpegJpegIntSetCallback(INT_JPEG_DEC_DONE, _ISR_DecDone);
	V5_JpegJpegIntSetCallback(INT_JPEG_DEC_ERR, _ISR_DecError);

	V5_MarbIntEnable(INT_JBUF_FIFOCNT);
	V5_JpegJpegIntEnable(INT_JPEG_DEC_DONE);
	V5_JpegJpegIntEnable(INT_JPEG_DEC_ERR);

	V5_JpegDecode(pinfo);

	JpegDecReq();
}


void V5_HJpegDispFrm(PTDataBuf psrc)
{
	TJpegIndex info;

	if( BufInvalid(psrc) )
		return;
	JpegParse(psrc->pData, psrc->Len, &info);

	JpegDisplayFrame(&info, psrc);
}

void V5_HJpegDispFrmAuto(PTDataBuf psrc)
{
	TJpegIndex info;

	gHJpegContext.fucbuf.disp.pjbuf = psrc;

	if( BufInvalid(psrc) )
		return;
	JpegParse(psrc->pData, psrc->Len, &info);

	V5_JpegDecodeAuto(info.YUVType, info.JpgSize);

	WriteDecodeJpgData(1);

	DecodeReq();
}

void V5_HJpegDispFrmDirAuto(UINT8 fmt, TSize size, UINT32 jbufsize)
{
	UINT32 jbufstart;
	UINT8 dat[4] = {0xff, 0xff, 0xff, 0xd8};

	memset(&gHJpegContext.fucbuf, 0, sizeof(THJpegFucBuf));

	V5_JpegDecodeAuto(fmt, size);

	jbufstart = V5B_MarbGetJbufStart();
	jbufstart += jbufsize;
	jbufstart = ((jbufstart>>2)<<2);
	V5_WriteSram(jbufstart, dat, 4);

	DecodeReq();
}

void V5_HJpegDispNextFrm(void)
{
	V5_JpegDecodeNext();

	DecodeReq();
}

void V5_HJpegDispVidNorm(TDataBuf* pVid)
{
	TJpegIndex info;
	TDataBuf dbuf;
	UINT8* pbuf;
	UINT32 len;

	if( BufInvalid(pVid) )
		return;

	pbuf = pVid->pData;
	len = pVid->Len;

	while(len)
	{
		JpegParse(pbuf, len, &info);

		if(info.YUVType != JPEG_UNKNOWN)
		{
			dbuf.Len = (UINT32)info.eop;
			dbuf.Pos = 0;
			dbuf.pData = pbuf;

			JpegDisplayFrame(&info, &dbuf);

			//Delay for frame time control
			V5_Delay(50);
		}
		pbuf += info.eop;
		len -= info.eop;
	}

}

void V5_HJpegDispVidAuto(TDataBuf* pVid, UINT32 ms)
{
/*
	TJpegIndex info;
	UINT32 flg = 0xfffff;

	memset(&gHJpegContext.fucbuf, 0, sizeof(THJpegFucBuf));
	gHJpegContext.fucbuf.disp.pjbuf = pVid;

	JpegParse(gHJpegContext.fucbuf.disp.pjbuf->pData, gHJpegContext.fucbuf.disp.pjbuf->Len, &info);

	V5_JpegSetTimerCfg(ms);
	V5B_JpegAviFmtDisable();
	V5_JpegDecodeVideo(info.YUVType, info.JpgSize);

	WriteDecodeJpgData(1);

	while(flg--)
	{
		if( DataInvalid(gHJpegContext.fucbuf.disp.pjbuf) || (DecIsOkErr() & V5B_JPEG_STATUS_ER) )
			break;

		WriteDecodeJpgData(1);
		V5_Delay(1);
	}
*/
	TJpegIndex info;

	memset(&gHJpegContext.fucbuf, 0, sizeof(THJpegFucBuf));

	gHJpegContext.fucbuf.disp.pjbuf = pVid;

	gHJpegContext.decdone = FUNC_REQ_TIME;

	V5_MarbIntSetCallback(INT_JBUF_FIFOCNT, _ISR_JbufFifoCnt);
	V5_JpegJpegIntSetCallback(INT_JPEG_DEC_ERR, _ISR_DecError);

	V5_MarbIntEnable(INT_JBUF_FIFOCNT);
	V5_JpegJpegIntEnable(INT_JPEG_DEC_ERR);

	JpegParse(gHJpegContext.fucbuf.disp.pjbuf->pData, gHJpegContext.fucbuf.disp.pjbuf->Len, &info);

	V5_JpegSetTimerCfg(ms);
	V5B_JpegAviFmtDisable();
	V5_JpegDecodeVideo(info.YUVType, info.JpgSize);


	JpegDecVidReq();

}

void V5_HJpegDispAviAuto(TDataBuf* pVid, UINT32 ms)
{
	TJpegIndex info;
	UINT32 flg = 0xfffff;

	memset(&gHJpegContext.fucbuf, 0, sizeof(THJpegFucBuf));
	gHJpegContext.fucbuf.disp.pjbuf = pVid;

	JpegParse(gHJpegContext.fucbuf.disp.pjbuf->pData, gHJpegContext.fucbuf.disp.pjbuf->Len, &info);

	V5_JpegSetTimerCfg(ms);
	V5B_JpegAviFmtEnable();
	V5_JpegDecodeVideo(info.YUVType, info.JpgSize);

	WriteDecodeJpgData(1);

	while(flg--)
	{
		if( DataInvalid(gHJpegContext.fucbuf.disp.pjbuf) || (DecIsOkErr() & V5B_JPEG_STATUS_ER) )
			break;

		WriteDecodeJpgData(1);
		V5_Delay(1);
	}

}

void V5_HJpegDec(PTDataBuf psrc, PTDataBuf pdst)
{
	JpegDecode(psrc, pdst, 0);
}

void V5_HJpegDecIpp(PTDataBuf psrc, PTDataBuf pdst)
{
	JpegDecode(psrc, pdst, 1);
}

void V5_HJpegDecFrm(PTDataBuf psrc, PTDataBuf pdst)
{
	JpegDecode(psrc, pdst, 2);
}

void V5_HJpegYuvDisp(PTDataBuf psrc, TSize size)
{
	UINT16 flg = DEC_REQTIME;
	UINT32 len;

	memset(&gHJpegContext.fucbuf, 0, sizeof(THJpegFucBuf));
	gHJpegContext.fucbuf.disp.pjbuf = psrc;

	len = GetYuvSize(V5B_LBUF_YUV_422, size);
	if(psrc->Len < len)
		return;
	psrc->Len = len;

	V5_JpegYuvDecode(size);

	WriteDecodeYuvData(1);

	while(flg--)
	{
		if(DecIsOkErr())
		{
			//Delay for last 8 line to lcd.
			V5_Delay(5);
			break;
		}
		WriteDecodeYuvData(1);
		V5_Delay(1);
	}
}

void V5_HJpegYuvDecIpp(PTDataBuf psrc, PTDataBuf pdst, TSize size)
{
	JpegDeocdeYuv(psrc, pdst, size, 0);
}

void V5_HJpegYuvDecFrm(PTDataBuf psrc, PTDataBuf pdst, TSize size)
{
	JpegDeocdeYuv(psrc, pdst, size, 1);
}



/*=========================================================================//
						Static functions
//=========================================================================*/

void JpegCallback(UINT8 msg, TDataBuf *dbuf)
{
	UINT8 mode;

	mode = V5_GetMode();

	switch(msg)
	{
	case JMSG_CAP_JBUF_CNT:

		if(gHJpegContext.stopvid)
		{
#ifdef NCSC
			V5B_MarbStopCapture();
			gHJpegContext.stopvid = 0;
#else
			if(gHJpegContext.fucbuf.cap.pjbuf->Pos >= (gHJpegContext.fucbuf.cap.pjbuf->Len>>2))
			{
				V5B_MarbStopCapture();
				gHJpegContext.stopvid = 0;
			}
#endif
		}
		gHJpegContext.jpgdone = FUNC_REQ_TIME;
		break;

	case JMSG_DIS_JBUF_CNT:
		gHJpegContext.decdone = FUNC_REQ_TIME;

		break;

	case JMSG_ENC_JBUF_CNT:
		gHJpegContext.jpgdone = FUNC_REQ_TIME;

		break;

	case JMSG_JPEG_DONE:

		gHJpegContext.jpgdone = 0;

		break;
	case JMSG_JBUF_ERROR:

		gHJpegContext.jpgdone = 0;

		break;

	case JMSG_TBUF_CNT:
		//Only capture thumbnail.
		if(CAPTURETHUMB_MODE == mode)
		{
			gHJpegContext.jpgdone = FUNC_REQ_TIME; 
		}

		break;
	case JMSG_THUMB_DONE:
		//Only capture thumbnail.
		if(CAPTURETHUMB_MODE == mode)
		{
			gHJpegContext.jpgdone = 0;
		}
		break;

	case JMSG_THUMB_ERROR:
		//Only capture thumbnail.
		if(CAPTURETHUMB_MODE == mode)
		{
			gHJpegContext.jpgdone = 0;
		}
		break;

	case JMSG_IBUF_CNT:

		break;
	case JMSG_ENC_DONE:
		break;
	case JMSG_DEC_DONE:
	case JMSG_DEC_ERROR:
		gHJpegContext.decdone = 0;
		break;
	case JMSG_LBUF_READ:
		break;

	case JMSG_REQ_CAP_JBUF:
		break;

	case JMSG_REQ_ENC_JBUF:
		break;

	case JMSG_REQ_TBUF:
		break;
	case JMSG_REQ_IBUF:
		break;
	case JMSG_JPEG_RECAP:
		gHJpegContext.jpgdone = FUNC_REQ_TIME;
		break;

	case JMSG_JPEG_RECAP_ERR:
		gHJpegContext.jpgdone = 0;
		break;

	case JMSG_JPEG_JBUF_INTV:
		break;

	default:
		break;
	}

	//avoid warrning.
	dbuf = dbuf;
}

//UINT32 ReqData(TDataBuf *psrc, TDataBuf *pdes)
//{
//	UINT32 srclen;
//
//	if(pdes == NULL)
//		return 0;
//
//	pdes->Len = 0;
//	pdes->Pos = 0;
//
//	if((psrc == NULL)||(psrc->pData == NULL))
//		return 0;
//
//	srclen = psrc->Len - psrc->Pos;
//
//	if(srclen == 0)
//		return 0;
//
//	if(srclen < LEN_DATA_PACKET)
//	{
//		pdes->pData = psrc->pData+psrc->Pos;
//		pdes->Len = srclen;
//		pdes->Pos = 0;
//	}
//	else
//	{
//		pdes->pData = psrc->pData+psrc->Pos;
//		pdes->Len = LEN_DATA_PACKET;
//		pdes->Pos = 0;
//	}
//
//	return srclen;
//}

/*
	1 - ok,
	2 - error.
*/
UINT8 DecIsOkErr(void)
{
	UINT8 x = V5B_JpegGetStatus();
	
	return ( UINT8)(x & (V5B_JPEG_STATUS_DD|V5B_JPEG_STATUS_ER) );
}

void InitCapEncInt(UINT8 type)
{
	UINT8 mode = V5_GetMode();

	gHJpegContext.captype = type;

	V5_MarbIntDisable(MARB_INT_ALL);
	V5_MarbIntSetCallback(MARB_INT_ALL, NULL);
	V5_MarbSlvIntDisable(MARB_SLV_INT_ALL);
	V5_MarbSlvIntSetCallback(MARB_SLV_INT_ALL, NULL);

	V5B_CpmGetIntFlagSec(INT_MARB0); //Clear int status.
	V5B_CpmGetIntFlagSec(INT_MARB1); //Clear int status.

	gHJpegContext.stopvid = 0;
	gHJpegContext.jpgdone = FUNC_REQ_TIME;

	switch(mode)
	{
	case CAPTURESTILL_MODE:
	case CAPTURESTILLTHUMB_MODE:
	case CAPTUREFRAME_MODE:
	case CAPTUREFRAMETHUMB_MODE:
		if(gHJpegContext.captype == JPEG_CAP_REP) {
			V5_MarbIntSetCallback(INT_JPEG_DONE, _ISR_JpegDone);
			V5_MarbSlvIntSetCallback(INT_RECAP_ERR, _ISR_JpegRecapError);

			V5_MarbIntEnable(INT_JPEG_DONE);
			V5_MarbSlvIntEnable(INT_RECAP_ERR);
		}
		else
		{
			V5_MarbIntSetCallback(INT_JBUF_FIFOCNT, _ISR_JbufFifoCnt);
			V5_MarbIntSetCallback(INT_JPEG_DONE, _ISR_JpegDone);
			V5_MarbIntSetCallback(INT_JBUF_ERR, _ISR_JbufError);

			V5_MarbIntEnable(INT_JBUF_FIFOCNT);
			V5_MarbIntEnable(INT_JPEG_DONE);
			V5_MarbIntEnable(INT_JBUF_ERR);
		}

		break;
	case ENCODE_MODE:
		V5_MarbIntSetCallback(INT_JBUF_FIFOCNT, _ISR_JbufFifoCnt);
		V5_MarbIntSetCallback(INT_JPEG_DONE, _ISR_JpegDone);
		V5_MarbIntSetCallback(INT_JBUF_ERR, _ISR_JbufError);

		V5_MarbIntEnable(INT_JBUF_FIFOCNT);
		V5_MarbIntEnable(INT_JPEG_DONE);
		V5_MarbIntEnable(INT_JBUF_ERR);
		break;

	case CAPTUREVIDEO_MODE:
	case CAPTUREVIDEOTHUMB_MODE:
	case CAPTUREFRAMEVIDEO_MODE:
	case CAPTUREFRAMEVIDEOTHUMB_MODE:
		gHJpegContext.stopvid = 1;

		V5_MarbIntSetCallback(INT_JBUF_FIFOCNT, _ISR_JbufFifoCnt);
		V5_MarbIntSetCallback(INT_JPEG_DONE, _ISR_JpegDone);
		V5_MarbIntSetCallback(INT_JBUF_ERR, _ISR_JbufError);

		V5_MarbIntEnable(INT_JBUF_FIFOCNT);
		V5_MarbIntEnable(INT_JPEG_DONE);
		V5_MarbIntEnable(INT_JBUF_ERR);
		break;

	case CAPTUREAVI_MODE:
	case CAPTUREAVITHUMB_MODE:
	case CAPTUREFRAMEAVI_MODE:
	case CAPTUREFRAMEAVITHUMB_MODE:
		gHJpegContext.stopvid = 1;
		V5B_MarbSetFrameInterval(1);

		V5_MarbIntSetCallback(INT_JPEG_DONE, _ISR_JpegDone);
		V5_MarbIntSetCallback(INT_JBUF_ERR, _ISR_JbufError);
		V5_MarbIntSetCallback(INT_JBUF_INTV, _ISR_JbufIntv);

		V5_MarbIntEnable(INT_JPEG_DONE);
		V5_MarbIntEnable(INT_JBUF_ERR);
		V5_MarbIntEnable(INT_JBUF_INTV);
		break;

	case MULTISHOT_MODE:
	case MULTISHOTTHUMB_MODE:
	case MULTISHOTFRAME_MODE:
		V5_MarbIntSetCallback(INT_JBUF_FIFOCNT, _ISR_JbufFifoCnt);
		V5_MarbIntSetCallback(INT_JPEG_DONE, _ISR_JpegDone);
		V5_MarbIntSetCallback(INT_JBUF_ERR, _ISR_JbufError);

		V5_MarbIntEnable(INT_JBUF_FIFOCNT);
		V5_MarbIntEnable(INT_JPEG_DONE);
		V5_MarbIntEnable(INT_JBUF_ERR);
		break;
	case CAPTURETHUMB_MODE:
		V5_MarbIntSetCallback(INT_TBUF_FIFOCNT, _ISR_TbufFifoCnt);
		V5_MarbIntSetCallback(INT_THUMB_DONE, _ISR_ThmbDone);
		V5_MarbIntSetCallback(INT_THUMB_ERR, _ISR_ThmbError);

		V5_MarbIntEnable(INT_TBUF_FIFOCNT);
		V5_MarbIntEnable(INT_THUMB_DONE);
		V5_MarbIntEnable(INT_THUMB_ERR);
		break;

		break;
	}

}

void JpegCapEncReq(void)
{
	UINT16 intflg0, intflg1;

#ifdef WIN32

#ifdef HJPEG_RT_CAP
	UINT8 val = 0;
	UINT8 mode = V5_GetMode();

	switch(mode)
	{
	case CAPTURESTILL_MODE:
	case CAPTURESTILLTHUMB_MODE:
	case MULTISHOT_MODE:
	case MULTISHOTTHUMB_MODE:
		if(V5B_IppGetCapWidth() > 0x400 || V5B_IppGetCapHeight() > 0x300)
			val = 1;
		break;
	default:
		break;
	}

	if(gHJpegContext.captype == JPEG_CAP_REP || gHJpegContext.captype == JPEG_CAP_SOFT_REP)
		val = 0;

	if(val)
	{

		if(mode == CAPTURESTILLTHUMB_MODE || mode == MULTISHOTTHUMB_MODE)
		{
			if(gHJpegContext.fucbuf.cap.pjbuf)
				gHJpegContext.fucbuf.cap.pjbuf->Pos	+= RtReqJbuf(gHJpegContext.fucbuf.cap.ptbuf->Len, gHJpegContext.fucbuf.cap.pjbuf->Len);

			CapTbufFifoCnt(0);
			if(gHJpegContext.pcall)
				gHJpegContext.pcall(JMSG_TBUF_CNT, gHJpegContext.fucbuf.cap.ptbuf);
		}
		else
		{
			if(gHJpegContext.fucbuf.cap.pjbuf)
			gHJpegContext.fucbuf.cap.pjbuf->Pos	+= RtReqJbuf(0, gHJpegContext.fucbuf.cap.pjbuf->Len);
		}

		return;
	}

#endif

#endif

	//wait for done signal
	while(gHJpegContext.jpgdone--)
	{
		//Request
		if(gHJpegContext.intreq)
		{
			if(gHJpegContext.captype == JPEG_CAP_REP)
				intflg1 = V5B_CpmGetIntFlagSec(INT_MARB1);
			else
				intflg1 = 0;
			intflg0 = V5B_CpmGetIntFlagSec(INT_MARB0);

			if(intflg0 || intflg1)
			{
				if(intflg1)
					_ISR_V5MarbSlvIntHandle(intflg1);
				if(intflg0)
					_ISR_V5MarbIntHandle(intflg0);
			}
			else
			{
				V5_Delay(FIFO_DELAY);
			}
		}
		else {
			V5_Delay(FIFO_DELAY);
		}
	}


}

/************************************************************************************************
*								JPEG Interrupt
************************************************************************************************/
void _ISR_EncDone(void)
{
	if(gHJpegContext.pcall)
	{
		gHJpegContext.pcall(JMSG_ENC_DONE, NULL);
	}
}

void _ISR_DecDone(void)
{
	if(gHJpegContext.pcall)
	{
		gHJpegContext.pcall(JMSG_DEC_DONE, NULL);
	}
}

void _ISR_DecError(void)
{
	if(gHJpegContext.pcall)
	{
		gHJpegContext.pcall(JMSG_DEC_ERROR, NULL);
	}
}

/************************************************************************************************
*								LBUF Interrupt
************************************************************************************************/
void _ISR_LbufRead(void)
{
	if(gHJpegContext.pcall)
	{
		gHJpegContext.pcall(JMSG_LBUF_READ, NULL);
	}
}

/************************************************************************************************
*MARB Interrupt
************************************************************************************************/


static void CapEncJbufFifoCnt(UINT8 bEnc, UINT32 gapsize) //bEnc 0 cap, 1 enc
{
	UINT32 size, size1, pos, Len;
	PTDataBuf pbuf;

	if(bEnc)
		pbuf = gHJpegContext.fucbuf.enc.pdst;
	else
		pbuf = gHJpegContext.fucbuf.cap.pjbuf;

	if(pbuf == NULL)
		return;

	if( DataInvalid(pbuf) )
	{
		if(gHJpegContext.pcall == NULL)
			return;

		if(bEnc)
			gHJpegContext.pcall(JMSG_REQ_ENC_JBUF, pbuf);
		else
			gHJpegContext.pcall(JMSG_REQ_CAP_JBUF, pbuf);

		if( DataInvalid(pbuf) )
			return;
	}

	pos = pbuf->Pos;
	Len = (pos+LEN_DATA_PACKET) < pbuf->Len ? (pos+LEN_DATA_PACKET) : pbuf->Len;
	size = V5_MarbGetJbufRWgap();

	do{
		if(Len  == pbuf->Pos)
		{
			if(gHJpegContext.pcall)
			{
				if(bEnc)
					gHJpegContext.pcall(JMSG_ENC_JBUF_CNT, pbuf);
				else
					gHJpegContext.pcall(JMSG_CAP_JBUF_CNT, pbuf);
			}
			if( Len >= pbuf->Len )
			{
				if(bEnc)
					gHJpegContext.pcall(JMSG_REQ_ENC_JBUF, pbuf);
				else
					gHJpegContext.pcall(JMSG_REQ_CAP_JBUF, pbuf);

				if( DataInvalid(pbuf) )
					break;
			}
			pos = pbuf->Pos;
			Len = (pos+LEN_DATA_PACKET) < pbuf->Len ? (pos+LEN_DATA_PACKET) : pbuf->Len;
		}

		size1 = Len - pbuf->Pos;
		size = (size > size1) ? size1 : size;
		size1 = V5_MarbJbufRead(pbuf->pData+pbuf->Pos, size);
		pbuf->Pos += size1;

		size = V5_MarbGetJbufRWgap();

	} while(size > gapsize);

	if(pbuf->Pos > pos)
	{
		if(gHJpegContext.pcall)
		{
			if(bEnc)
				gHJpegContext.pcall(JMSG_ENC_JBUF_CNT, pbuf);
			else
				gHJpegContext.pcall(JMSG_CAP_JBUF_CNT, pbuf);
		}
	}
}

static void CapTbufFifoCnt(UINT32 gapsize)
{
	UINT32 size, size1, pos, Len;
	PTDataBuf pbuf;

	pbuf = gHJpegContext.fucbuf.cap.ptbuf;
	if(pbuf == NULL)
		return;

	if( DataInvalid(pbuf) )
	{
			if(gHJpegContext.pcall)
				gHJpegContext.pcall(JMSG_REQ_TBUF, pbuf);

			if( DataInvalid(pbuf) )
				return;
	}

	pos = pbuf->Pos;
	Len = (pos+LEN_DATA_PACKET) < pbuf->Len ? (pos+LEN_DATA_PACKET) : pbuf->Len;
	size = V5_MarbGetThumbRWgap();

	do {
		if(Len  == pbuf->Pos)
		{
			if(gHJpegContext.pcall)
				gHJpegContext.pcall(JMSG_TBUF_CNT, pbuf);

			if( Len >= pbuf->Len )
			{
				gHJpegContext.pcall(JMSG_REQ_TBUF, pbuf);

				if( DataInvalid(pbuf) )
					break;
			}
			pos = pbuf->Pos;
			Len = (pos+LEN_DATA_PACKET) < pbuf->Len ? (pos+LEN_DATA_PACKET) : pbuf->Len;
		}

		size1 = Len - pbuf->Pos;
		size = (size > size1) ? size1 : size;
		size1 = V5_MarbThumbRead(pbuf->pData+pbuf->Pos, size);
		pbuf->Pos += size1;

		size = V5_MarbGetThumbRWgap();

	} while(size > gapsize);

	if(pbuf->Pos > pos)
	{
		if(gHJpegContext.pcall)
			gHJpegContext.pcall(JMSG_TBUF_CNT, pbuf);
	}

}

static void CapIndexReadData(UINT32 gapsize)
{
	UINT32 size, size1, pos, Len;
	PTDataBuf pbuf;

	pbuf = gHJpegContext.fucbuf.cap.pibuf;
	if(pbuf == NULL)
		return;

	if( DataInvalid(pbuf) )
	{
			if(gHJpegContext.pcall)
				gHJpegContext.pcall(JMSG_REQ_IBUF, pbuf);

			if( DataInvalid(pbuf) )
				return;
	}

	pos = pbuf->Pos;
	Len = (pos+LEN_DATA_PACKET) < pbuf->Len ? (pos+LEN_DATA_PACKET) : pbuf->Len;
	size = V5B_MarbGetFrmCount() << 1;

	do {
		if(Len  == pbuf->Pos)
		{
			if(gHJpegContext.pcall)
				gHJpegContext.pcall(JMSG_IBUF_CNT, pbuf);

			if( Len >= pbuf->Len )
			{
				gHJpegContext.pcall(JMSG_REQ_IBUF, pbuf);

				if( DataInvalid(pbuf) )
					break;
			}
			pos = pbuf->Pos;
			Len = (pos+LEN_DATA_PACKET) < pbuf->Len ? (pos+LEN_DATA_PACKET) : pbuf->Len;
		}
		size1 = Len - pbuf->Pos;
		size1 = (size > size1) ? size1 : size;
		size1 = V5_MarbIndexRead(pbuf->pData+pbuf->Pos, size1);
		pbuf->Pos += size1;

		size -= size1;

	} while(size > gapsize);

	if(pbuf->Pos > pos)
	{
		if(gHJpegContext.pcall)
			gHJpegContext.pcall(JMSG_IBUF_CNT, pbuf);
	}

}

static void DispJbufFifoCnt(UINT32 gapleftsize)
{
	UINT32 size, size1;
	PTDataBuf pbuf;

	pbuf = gHJpegContext.fucbuf.disp.pjbuf;
	if(pbuf == NULL)
		return;

	if( DataInvalid(pbuf) )
	{
			if(gHJpegContext.pcall)
				gHJpegContext.pcall(JMSG_DIS_JBUF_CNT, pbuf);

			if( DataInvalid(pbuf) )
				return;
	}

	size = V5_MarbGetJbufRWgap();

	do {
		tmDbgPrintf("size = %x\n", size);
		if(pbuf->Len  == pbuf->Pos)
		{
			if(gHJpegContext.pcall)
				gHJpegContext.pcall(JMSG_DIS_JBUF_CNT, pbuf);
			if( DataInvalid(pbuf) )
				break;
		}

		size1 = pbuf->Len - pbuf->Pos;
		if(size1 > size)
		{
			V5_MarbJbufWrite(pbuf->pData+pbuf->Pos, size);
			pbuf->Pos += size;
		}
		else
		{
			WriteLstJpg(pbuf->pData+pbuf->Pos, size1);
			pbuf->Pos += size1;
		}

		size = V5_MarbGetJbufRWgap();

	} while(size > gapleftsize);

}

void _ISR_JbufFifoCnt(void)
{
	UINT8 mode;

	mode = V5_GetMode();

	switch(mode)
	{
	case PREVIEW_MODE:
	case PREVIEWBRC_MODE:
	case PREVIEWFRAME_MODE:

	case DIRECTDISPLAY_MODE:
	case BYPASS_MODE:
	case THROUGH_MODE:

		break;

	case CAPTUREAVITHUMB_MODE:
	case CAPTUREAVI_MODE:
	case CAPTUREFRAMEAVI_MODE:
	case MULTISHOTTHUMB_MODE:
	case CAPTUREVIDEOTHUMB_MODE:
	case CAPTURESTILLTHUMB_MODE:
	case CAPTURESTILL_MODE:
	case CAPTUREVIDEO_MODE:
	case CAPTUREFRAME_MODE:
	case CAPTUREFRAMEVIDEO_MODE:
	case MULTISHOT_MODE:
	case MULTISHOTFRAME_MODE:
	case CAPTUREFRAMETHUMB_MODE:
	case CAPTUREFRAMEVIDEOTHUMB_MODE:
	case CAPTUREFRAMEAVITHUMB_MODE:
		CapEncJbufFifoCnt(0, JPEG_FIFO_ACESS_SIZE);

		break;

	case ENCODE_MODE:
		CapEncJbufFifoCnt(1, JPEG_FIFO_ACESS_SIZE);

		break;

	case CAPTURETHUMB_MODE: //only thumb buf used.
		break;

	case DISPLAYSTILL_MODE:
		DispJbufFifoCnt(JPEG_FIFO_ACESS_SIZE);
		break;
	case DECODE_MODE:
	case DECODEIPP_MODE:
	case DECODEFRAME_MODE:
	case DECODEIPPBYPASS_MODE:
	case DECODEFRAMEBYPASS_MODE:
	case DISPLAYBYPASS_MODE:

		break;

	case DISPLAYVIDEO_MODE:
	case DISPLAYAVI_MODE:

		DispJbufFifoCnt(JPEG_FIFO_ACESS_SIZE);
		break;

	default:
		break;
	}
}

void _ISR_JpegDone(void)
{
	UINT8 mode;

	mode = V5_GetMode();

	switch(mode)
	{
	case PREVIEW_MODE:
	case PREVIEWBRC_MODE:

	case DIRECTDISPLAY_MODE:
	case BYPASS_MODE:
	case THROUGH_MODE:
	break;

	case CAPTUREAVITHUMB_MODE:
	case CAPTUREFRAMEAVITHUMB_MODE:
		{
			CapEncJbufFifoCnt(0, 0);
			CapTbufFifoCnt(0);
			CapIndexReadData(0);
			if(gHJpegContext.pcall)
			{
				gHJpegContext.pcall(JMSG_CAP_JBUF_CNT, gHJpegContext.fucbuf.cap.pjbuf);
				gHJpegContext.pcall(JMSG_TBUF_CNT, gHJpegContext.fucbuf.cap.ptbuf);
				gHJpegContext.pcall(JMSG_IBUF_CNT, gHJpegContext.fucbuf.cap.pibuf);

				gHJpegContext.pcall(JMSG_JPEG_DONE, gHJpegContext.fucbuf.cap.pjbuf);
			}

		}
		break;

	case CAPTUREAVI_MODE:
	case CAPTUREFRAMEAVI_MODE:
		{
			CapEncJbufFifoCnt(0, 0);
			CapIndexReadData(0);
			if(gHJpegContext.pcall)
			{
				gHJpegContext.pcall(JMSG_CAP_JBUF_CNT, gHJpegContext.fucbuf.cap.pjbuf);
				gHJpegContext.pcall(JMSG_IBUF_CNT, gHJpegContext.fucbuf.cap.pibuf);

				gHJpegContext.pcall(JMSG_JPEG_DONE, gHJpegContext.fucbuf.cap.pjbuf);
			}

		}
		break;

	case MULTISHOTTHUMB_MODE:
	case CAPTUREVIDEOTHUMB_MODE:
	case CAPTURESTILLTHUMB_MODE:
	case CAPTUREFRAMETHUMB_MODE:
	case CAPTUREFRAMEVIDEOTHUMB_MODE:
		{
			CapEncJbufFifoCnt(0, 0);
			CapTbufFifoCnt(0);
			if(gHJpegContext.pcall)
			{
				gHJpegContext.pcall(JMSG_CAP_JBUF_CNT, gHJpegContext.fucbuf.cap.pjbuf);
				gHJpegContext.pcall(JMSG_TBUF_CNT, gHJpegContext.fucbuf.cap.ptbuf);

				gHJpegContext.pcall(JMSG_JPEG_DONE, gHJpegContext.fucbuf.cap.pjbuf);
			}

		}
		break;

	case CAPTURESTILL_MODE:
	case CAPTUREVIDEO_MODE:
	case CAPTUREFRAME_MODE:
	case CAPTUREFRAMEVIDEO_MODE:
	case MULTISHOT_MODE:
	case MULTISHOTFRAME_MODE:
		{
			CapEncJbufFifoCnt(0, 0);
			if(gHJpegContext.pcall)
			{
				gHJpegContext.pcall(JMSG_CAP_JBUF_CNT, gHJpegContext.fucbuf.cap.pjbuf);

				gHJpegContext.pcall(JMSG_JPEG_DONE, gHJpegContext.fucbuf.cap.pjbuf);
			}

		}
		break;

	case CAPTURETHUMB_MODE:
		{
			CapTbufFifoCnt(0);
			if(gHJpegContext.pcall)
			{
				gHJpegContext.pcall(JMSG_TBUF_CNT, gHJpegContext.fucbuf.cap.ptbuf);

				gHJpegContext.pcall(JMSG_JPEG_DONE, NULL);
			}

		}
		break;

	case ENCODE_MODE:
		{
			CapEncJbufFifoCnt(1, 0); //encode
			if(gHJpegContext.pcall)
			{
				gHJpegContext.pcall(JMSG_ENC_JBUF_CNT, gHJpegContext.fucbuf.enc.pdst);

				gHJpegContext.pcall(JMSG_JPEG_DONE, gHJpegContext.fucbuf.enc.pdst);
			}

		}
		break;

	case DISPLAYSTILL_MODE:
	case DECODE_MODE:
	case DECODEIPP_MODE:
	case DECODEFRAME_MODE:
	case DECODEIPPBYPASS_MODE:

	case DISPLAYVIDEO_MODE:
	case DISPLAYAVI_MODE:
	case DISPLAYBYPASS_MODE:
		break;

	default:
		break;
	}
}

void _ISR_JbufError(void)
{
	if(gHJpegContext.captype == JPEG_CAP_SOFT_REP)
	{
		gHJpegContext.recapactcnt++;
		if(gHJpegContext.recapactcnt > gHJpegContext.recapcnt)
		{
			if(gHJpegContext.pcall)
			{
				gHJpegContext.pcall(JMSG_JBUF_ERROR, NULL);
			}
		}
		else
		{
			if(gHJpegContext.fucbuf.cap.pjbuf)
				gHJpegContext.fucbuf.cap.pjbuf->Pos = 0;
			#ifdef WIN32 //for debug
			JpegSetMapAdr(gHJpegContext.fucbuf.cap.ptbuf, gHJpegContext.fucbuf.cap.pibuf);
			#endif

			if(gHJpegContext.pcall)
			{
				gHJpegContext.pcall(JMSG_JPEG_RECAP, NULL);
			}
			V5_JpegCapture();
		}
	}
	else
	{
		if(gHJpegContext.pcall)
		{
			gHJpegContext.pcall(JMSG_JBUF_ERROR, NULL);
		}
	}
}

static void	_ISR_JpegRecapError(void)
{
	if(gHJpegContext.pcall)
	{
		gHJpegContext.pcall(JMSG_JPEG_RECAP_ERR, NULL);
	}
}

static void _ISR_JbufIntv(void)
{
	CapEncJbufFifoCnt(0, JPEG_FIFO_ACESS_SIZE);

	if(gHJpegContext.pcall)
	{
		gHJpegContext.pcall(JMSG_JPEG_JBUF_INTV, NULL);
	}
}

void _ISR_TbufFifoCnt(void)
{
	CapTbufFifoCnt(JPEG_FIFO_ACESS_SIZE);
}

void _ISR_ThmbDone(void)
{
	CapTbufFifoCnt(0);
	if(gHJpegContext.pcall)
	{
		gHJpegContext.pcall(JMSG_TBUF_CNT, gHJpegContext.fucbuf.cap.ptbuf);

		gHJpegContext.pcall(JMSG_THUMB_DONE, gHJpegContext.fucbuf.cap.ptbuf);
	}

}

void _ISR_ThmbError(void)
{
	if(gHJpegContext.pcall)
	{
		gHJpegContext.pcall(JMSG_THUMB_ERROR, NULL);
	}
}

/************************************************************************************************
*JPEG static functions
************************************************************************************************/

UINT8 LbufIsOk(UINT8 bYuv)
{
	if(bYuv)
		return V5B_Lbuf0Isok();
	else
		return V5B_Lbuf1Isok();
}

static void LbufRead(UINT8* pbuf, UINT32 len, UINT8 num)
{
	ClearGbufPoint();

	if(num == 0)
		V5_MarbLbuf0Read(pbuf, len);
	else
		V5_MarbLbuf1Read(pbuf, len);

#ifdef WIN32
	ReadBuf(0, (char *)pbuf, len);
#endif
}

static UINT8 GetDecLbuf0Height(UINT8 fmt)
{
	switch(fmt)
	{
	case V5B_LBUF_YUV_420:
		return 16;
	case V5B_LBUF_YUV_422:
	case V5B_LBUF_YUV_411:
	case V5B_LBUF_YUV_444:
	case V5B_LBUF_YUV_400:
	default:
		return 8;
	}
}

static UINT8 LbufIsOkAftBlank(UINT8 bYuv)
{
	UINT8 flag = 0xa;

	while(flag--)
	{
		if(LbufIsOk(bYuv))
			return 1;
		V5_Delay(1);
	}

	return 0;
}

void ReadDecDataIppLcd(UINT16 jpgheight, UINT8 yuvtype, TSize size1, PTDataBuf pdst, UINT8 bYuv)
{
	UINT32 vwc;
	TSize size;
	UINT8 lbufheight;
	UINT16 i = 0, j = 0, flag = DEC_REQTIME;
	
	lbufheight = GetDecLbuf0Height(yuvtype);
	i = i+lbufheight;
	pdst->Pos = 0;
	size.cx = size1.cx;
	while((flag--))
	{
		if(LbufIsOk(bYuv) == 0)
		{
			if(V5B_LbufIsHblankDone())
			{
				if(LbufIsOkAftBlank(bYuv) == 0)
				{
					if(i < jpgheight)
					{
						V5B_JpegRestartDecode();
						i = i+lbufheight;
						flag = DEC_REQTIME;
						continue;
					}
					else
						break;
				}
			}
			else
				continue;
		}

		size.cy = V5B_LbufGetLinenum();
		vwc = GetYuvSize(V5B_LBUF_YUV_422, size);
		if((pdst->Pos + vwc) > pdst->Len)
		{
			if(gHJpegContext.pcall)
				gHJpegContext.pcall(JMSG_LBUF_READ, pdst);
			pdst->Pos = 0;
		}

		if(bYuv)
			LbufRead(pdst->pData + pdst->Pos, vwc, 0);
		else
			LbufRead(pdst->pData + pdst->Pos, vwc, 1);

		pdst->Pos += vwc;
		j = j+size.cy;

		if(bYuv)
			WriteDecodeYuvData(0);
		else
			WriteDecodeJpgData(0);

		if(j >= size1.cy)
			break;

		if(i < jpgheight)
		{
			V5B_JpegRestartDecode();
			i = i+lbufheight;
			//reset flag
			flag = DEC_REQTIME;
		}
		else
			break;
	}

	if(gHJpegContext.pcall)
		gHJpegContext.pcall(JMSG_LBUF_READ, pdst);
}

//type 0-decode, 1-decode ipp, 2-decode frame
void JpegDecode(PTDataBuf psrc, PTDataBuf pdst, UINT8 type)
{
	TSize decsize;
	TJpegIndex	info;

	ClearGbufPoint();

	memset(&gHJpegContext.fucbuf, 0, sizeof(THJpegFucBuf));
	gHJpegContext.fucbuf.dec.psrc = psrc;
	gHJpegContext.fucbuf.dec.pdst = pdst;

	JpegParse(psrc->pData, psrc->Len, &info);
	if(info.YUVType == JPEG_UNKNOWN)
		return;

	V5_JpegDecode(&info);

	gHJpegContext.fucbuf.dec.psrc->Len = info.eop;
	gHJpegContext.fucbuf.dec.psrc->Pos = info.offset;

	WriteDecodeJpgData(0);

	switch(type)
	{
	case 0:
		ReadDecData(info.YUVType, info.JpgSize, pdst);
		break;
	case 1:
		V5B_IppGetDispSize(&decsize);
		ReadDecDataIppLcd(info.JpgSize.cy, info.YUVType, decsize, pdst, 0);
		break;
	case 2:
		V5B_IppGetDispSize(&decsize);
		ReadDecDataIppLcd(info.JpgSize.cy, info.YUVType, decsize, pdst, 0);
		break;
	}

}

//type 0-decode ipp, 1-decode frame
static void JpegDeocdeYuv(PTDataBuf psrc, PTDataBuf pdst, TSize size, UINT8 type)
{
	UINT32 len;
	TSize decsize;

	ClearGbufPoint();

	memset(&gHJpegContext.fucbuf, 0, sizeof(THJpegFucBuf));
	gHJpegContext.fucbuf.dec.psrc = psrc;
	gHJpegContext.fucbuf.dec.pdst = pdst;

	len = GetYuvSize(V5B_LBUF_YUV_422, size);
	if(psrc->Len < len)
		return;
	psrc->Len = len;

	V5_JpegYuvDecode(size);

	WriteDecodeYuvData(0);

	if(type == 0)
	{
		V5B_IppGetDispSize(&decsize);
		ReadDecDataIppLcd(size.cy, V5B_LBUF_YUV_422, decsize, pdst, 1);
	}
	else
	{
		V5B_LcdcGetAWinSize(&decsize);
		ReadDecDataIppLcd(size.cy, V5B_LBUF_YUV_422, decsize, pdst, 1);
	}

}


void ReadDecData(UINT8 YUVType, TSize size1, PTDataBuf pdst)
{
	UINT32 vwc;
	TSize size;
	UINT16 i = 0, flag = DEC_REQTIME;

	pdst->Pos = 0;
	size = size1;
	while( (i < size1.cy) && (flag --) )
	{
		if(V5B_Lbuf0Isok() == 0)
		{
			V5_Delay(1);
			continue;
		}

		size.cy = 8;
		if(YUVType == JPEG_420)
			size.cy = 16;

		if(YUVType == V5B_LBUF_YUV_444)
			vwc = GetYuvSize(V5B_LBUF_YUV_422, size);
		else
			vwc = GetYuvSize(YUVType, size);

		if((pdst->Pos + vwc) > pdst->Len)
		{
			if(gHJpegContext.pcall)
				gHJpegContext.pcall(JMSG_LBUF_READ, pdst);
			pdst->Pos = 0;
		}

		LbufRead(pdst->pData + pdst->Pos, vwc, 0);
		pdst->Pos += vwc;
		i = (UINT16)(i + size.cy);
		
		if(i < size1.cy)
		{
			V5B_JpegRestartDecode();
			//reset flag
			flag = DEC_REQTIME;
		}
		WriteDecodeJpgData(0);

	}

	V5B_JpegRestartDecode();

	if(gHJpegContext.pcall)
		gHJpegContext.pcall(JMSG_LBUF_READ, pdst);
}

static void WriteLstJpg(UINT8* pbuf, UINT32 length)
{
	UINT16 leftlen;
	UINT8 dat[4] = {0xff, 0xff, 0xff, 0xd9};

	leftlen = (UINT16)(length&0x3);
	length = (length>>2)<<2;

	if(!leftlen)//dat[3,4] = {0xff, 0xd9}
	{
		dat[0] = pbuf[length - 4];
		dat[1] = pbuf[length - 3];

		length -= 4;
	}

	if(leftlen == 3)
	{
		dat[0] = pbuf[length];
	}

	V5_MarbJbufWrite(pbuf, length);
	V5_MarbJbufWrite(dat, 4);
}

void WriteDecodeJpgData(UINT8 bDisp)
{
	UINT32 length, marblen;
	UINT8 *ptempbuf;
	PTDataBuf pbuf;

	if(bDisp)
		pbuf = gHJpegContext.fucbuf.disp.pjbuf;
	else
		pbuf = gHJpegContext.fucbuf.dec.psrc;

	if(pbuf == NULL)
		return;

	if( DataInvalid(pbuf) )
	{
		if(gHJpegContext.pcall == NULL)
			return;

		if(bDisp)
			gHJpegContext.pcall(JMSG_REQ_DISP_BUF, pbuf);
		else
			gHJpegContext.pcall(JMSG_REQ_DEC_BUF, pbuf);

		if( DataInvalid(pbuf) )
			return;
	}

	length = pbuf->Len - pbuf->Pos;

	marblen = V5_MarbGetJbufRWgap();

	if( marblen < (V5B_MarbGetJbufSize()>>1) )
		return;

	ptempbuf = pbuf->pData + pbuf->Pos;

	if(marblen<length)
	{
		V5_MarbJbufWrite(ptempbuf, marblen);
		pbuf->Pos += marblen;
	}
	else
	{
		WriteLstJpg(ptempbuf, length);
		pbuf->Pos += length;
	}

}

void WriteDecodeYuvData(UINT8 bDisp)
{
	UINT32 length, marblen;
	UINT8 *ptempbuf;
	PTDataBuf pbuf;

	if(bDisp)
		pbuf = gHJpegContext.fucbuf.disp.pjbuf;
	else
		pbuf = gHJpegContext.fucbuf.dec.psrc;

	if(pbuf == NULL)
		return;

	if( DataInvalid(pbuf) )
	{
		//if(gHJpegContext.pcall == NULL)
		//	return;

		//if(bDisp)
		//	gHJpegContext.pcall(JMSG_REQ_DISP_BUF, pbuf);
		//else
		//	gHJpegContext.pcall(JMSG_REQ_DEC_BUF, pbuf);

		//if( DataInvalid(pbuf) )
		return;
	}

	length = pbuf->Len - pbuf->Pos;

	marblen = V5_MarbGetJbufRWgap();

	if( marblen < (V5B_MarbGetJbufSize()>>1) )
		return;
	
	ptempbuf = pbuf->pData + pbuf->Pos;
	if(marblen<length)
	{
		pbuf->Pos += marblen;

		V5_MarbJbufWrite(ptempbuf, marblen);
	}
	else
	{
		pbuf->Pos += length;

		V5_MarbJbufWrite(ptempbuf, length);
	}
}

#ifdef WIN32

#include "../../s3c44box/s3c44box.h"
#include "../v568env/v5initenv.h"

void SendYuvData(UINT8 *pbuf, UINT32 len)
{
	WriteBuf(0, (char *)pbuf, len);
	RepeatWriteReg(V5_REG_JPEG_DCT_DATA, len);
}

#else
void SendYuvData(UINT8 *pbuf, UINT32 len)
{
	UINT32 i = 0;

	while(i < len)
	{
		V5B_JpegSetDctData(pbuf[i]);
		i++;
	}
}

#endif

#ifdef WIN32 //for debug
void JpegMapMem(PTDataBuf pjbuf, PTDataBuf ptbuf, PTDataBuf pibuf)
{
	UINT32 x1 = 0, x2 = 0;

	if(pibuf != NULL)
		x1 = pibuf->Len;
	if(ptbuf != NULL)
		x2 = ptbuf->Len;

	if((pjbuf!=NULL) && pjbuf->Pos)
        ReadBuf(x1+x2, (char *)pjbuf->pData, pjbuf->Pos);
	if((ptbuf!=NULL) && ptbuf->Pos)
        ReadBuf(x1, (char *)ptbuf->pData, ptbuf->Pos);
	if((pibuf!=NULL) && pibuf->Pos)
        ReadBuf(0, (char *)pibuf->pData, pibuf->Pos);
}
void JpegSetMapAdr(PTDataBuf ptbuf, PTDataBuf pibuf)
{
	UINT32 x1 = 0, x2 = 0;

	if(pibuf != NULL)
		x1 = pibuf->Len;
	if(ptbuf != NULL)
		x2 = ptbuf->Len;
	SetGbufAdrInfo( 0, x1+x2, x1, 0);
}

#endif




#endif //V578CMD_DISABLE
