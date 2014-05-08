
#include "internal.h"
#include "../driver/basefunc/basefunc.h"
#ifndef	V578CMD_DISABLE



typedef struct tag_TTestJpeg {
	TDataBuf	jbuf;
	TDataBuf	tbuf;
	TDataBuf	ibuf;
} TTestJpeg, *PTTestJpeg;

TTestJpeg gTestJpeg;

#define NUM_CAP_FRAME 100
#define IBUF_VFRAME_SIZE	2 //2BYTE


/************************************************************
*
* Because in our RTOS, there isn't a memory alloc function So
* divide gTransferBuf to 3 buf to debug capture and display.
*
*		gTransferBuf
*      |
*      |
*------|-------------|------------|----------------------------
*      |gIbufMemSize |gTbufMemSize|gJbufMemSize
*      |
*
*************************************************************/

static UINT32 GetThumbActSize(void);
static void GetMapBufSize(UINT8 mode, UINT32 *tsize, UINT32 *isize);
static void CapMapInit(UINT8 mode, TDataBuf *pdbuf);



void TestCap(TDataBuf *pdbuf, TJpegCapParm* pRetParm)
{
	UINT8 mode;
	if((pdbuf == NULL) || (pdbuf->pData == NULL))
	{
		V5_HJpegCap(NULL, NULL, NULL);
		return;
	}

	mode = V5_GetMode();

	CapMapInit(mode, pdbuf);

	V5_HJpegCap(&gTestJpeg.jbuf, &gTestJpeg.tbuf, &gTestJpeg.ibuf);

	pdbuf->Pos = gTestJpeg.ibuf.Len+gTestJpeg.tbuf.Len+gTestJpeg.jbuf.Pos;
	pRetParm->ilen = gTestJpeg.ibuf.Len;
	pRetParm->tlen = gTestJpeg.tbuf.Len;
	pRetParm->jlen = gTestJpeg.jbuf.Pos;
}


void TestSifCap(TDataBuf *pdbuf, TJpegCapParm* pRetParm)
{
	UINT8 mode;
	if((pdbuf == NULL) || (pdbuf->pData == NULL))
	{
		V5_HJpegSifCap(NULL, NULL, NULL);
		return;
	}

	mode = V5_GetMode();

	CapMapInit(mode, pdbuf);

	V5_HJpegSifCap(&gTestJpeg.jbuf, &gTestJpeg.tbuf, &gTestJpeg.ibuf);

	pdbuf->Pos = gTestJpeg.ibuf.Len+gTestJpeg.tbuf.Len+gTestJpeg.jbuf.Pos;
	pRetParm->ilen = gTestJpeg.ibuf.Len;
	pRetParm->tlen = gTestJpeg.tbuf.Len;
	pRetParm->jlen = gTestJpeg.jbuf.Pos;
}

void TestReCap(TDataBuf *pdbuf, TJpegCapParm* pRetParm, UINT8* cnt)
{
	UINT8 mode;
	if((pdbuf == NULL) || (pdbuf->pData == NULL))
	{
		V5_HJpegReCap(NULL, NULL, cnt);
		return;
	}

	mode = V5_GetMode();

	CapMapInit(mode, pdbuf);

	V5_HJpegReCap(&gTestJpeg.jbuf, &gTestJpeg.tbuf, cnt);

	pdbuf->Pos = gTestJpeg.ibuf.Len+gTestJpeg.tbuf.Len+gTestJpeg.jbuf.Pos;
	pRetParm->ilen = 0;
	pRetParm->tlen = gTestJpeg.tbuf.Len;
	pRetParm->jlen = gTestJpeg.jbuf.Pos;
}

void TestCapVidTimer(TDataBuf *pdbuf, UINT32 ms, TJpegCapParm* pRetParm)
{
	UINT8 mode;


	if((pdbuf == NULL) || (pdbuf->pData == NULL))
	{
		V5_HJpegCapVidTime(NULL, NULL, NULL, ms);
		return;
	}

	mode = V5_GetMode();

	CapMapInit(mode, pdbuf);

	V5_HJpegCapVidTime(&gTestJpeg.jbuf, &gTestJpeg.tbuf, &gTestJpeg.ibuf, ms);

	pdbuf->Pos = gTestJpeg.ibuf.Len+gTestJpeg.tbuf.Len+gTestJpeg.jbuf.Pos;
	pRetParm->ilen = gTestJpeg.ibuf.Len;
	pRetParm->tlen = gTestJpeg.tbuf.Len;
	pRetParm->jlen = gTestJpeg.jbuf.Pos;
}


//=================================================================================
// Static functions
//=================================================================================
UINT32 GetThumbActSize(void)
{
	TSize size;
			
	V5B_IppGetThumbSize(&size);

	return ((size.cx*size.cy)<<1); 
}

void GetMapBufSize(UINT8 mode, UINT32 *tsize, UINT32 *isize)
{
	*tsize = 0;
	*isize = 0;

	switch(mode)
	{
	case CAPTURESTILL_MODE:
	case CAPTUREFRAME_MODE:
	case CAPTUREVIDEO_MODE:
	case MULTISHOT_MODE:
	case MULTISHOTFRAME_MODE:
	case CAPTUREFRAMEVIDEO_MODE:

		break;

	case CAPTURESTILLTHUMB_MODE:
	case CAPTUREVIDEOTHUMB_MODE:
	case CAPTUREFRAMETHUMB_MODE:
	case CAPTUREFRAMEVIDEOTHUMB_MODE:
		*tsize = GetThumbActSize();
		break;
	case MULTISHOTTHUMB_MODE:
		{
			UINT8 cnt = V5B_MarbGetMshotFcnt()+1;
			*tsize = GetThumbActSize();
			*tsize *= cnt;
		}
		break;

	case CAPTUREAVI_MODE:
	case CAPTUREFRAMEAVI_MODE:
		*isize = IBUF_VFRAME_SIZE*NUM_CAP_FRAME;
		break;

	case CAPTUREAVITHUMB_MODE:
	case CAPTUREFRAMEAVITHUMB_MODE:
		*tsize = GetThumbActSize();
		*isize = IBUF_VFRAME_SIZE*NUM_CAP_FRAME;
		break;

	case CAPTURETHUMB_MODE: //only thumb buf used.
		*tsize = GetThumbActSize();
		break;
	default:
		break;
	}
}

void CapMapInit(UINT8 mode, TDataBuf *pdbuf)
{
	UINT32 tsize = 0, isize = 0;

	memset(&gTestJpeg.jbuf, 0, sizeof(TDataBuf));
	memset(&gTestJpeg.tbuf, 0, sizeof(TDataBuf));
	memset(&gTestJpeg.ibuf, 0, sizeof(TDataBuf));

	GetMapBufSize(mode, &tsize, &isize);

	gTestJpeg.ibuf.pData = pdbuf->pData;

	if(pdbuf->Len >= isize)
	{
		gTestJpeg.ibuf.Len = isize;

		gTestJpeg.tbuf.pData = pdbuf->pData+isize;

		if(pdbuf->Len >= (isize+tsize))
		{
			gTestJpeg.tbuf.Len = tsize;

			gTestJpeg.jbuf.pData = pdbuf->pData+isize+tsize;
			gTestJpeg.jbuf.Len = pdbuf->Len-isize-tsize;
		}
		else
		{
			gTestJpeg.tbuf.Len = pdbuf->Len-isize;

		}
	}
	else
	{
		gTestJpeg.ibuf.Len = pdbuf->Len;
	}
}



#ifdef HJPEG_RT_CAP
////////////////////////////////////////////////////////////////////////////////////////////////////
//Test capture
//This context move request marb function to arm enviorment. To avoid capture 300M jpeg fifo overflow
//Problem.
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32 
#define FUNC_REQ_TIME	0x100
#else
#define FUNC_REQ_TIME	0x1388 // 5 s
#endif
#define FIFO_DELAY	1

#define	LEN_DATA_PACKET  0x40000

#define DataInvalid(pbuf) ( (pbuf->pData == NULL) || (pbuf->Len == 0) || (pbuf->Pos >= pbuf->Len) )

enum RT_BIT_CTRL
{
	RT_BIT_VID,
	RT_BIT_AVI
};

typedef struct tag_TRtCapContext {
	UINT32 jbufstart;
	UINT32 jbufend;
	UINT32 jbufpos;
	UINT16 jpgdone;
	UINT8  bitctrl;
} TRtCapContext, *PTRtCapContext;

//static const UINT8 gRtCapIntPry[]	= 
//{ 
//	INT_JBUF_ERR, INT_JBUF_FIFOCNT, INT_JPEG_DONE
//};


TRtCapContext gRtCapContext;

static UINT32 RtReadBuf(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 end, UINT32 pos)
{
	UINT32 size, len1;

	while(len)
	{
		size = end - pos;
		len1 = (len > size) ? size : len;
		V5_ReadSram(pos, pbuf, len1);
		pbuf += len1;
		len -= len1;
		pos += len1;
		pos = (pos == end) ? start : pos;
	}
	return pos;
}

static UINT32 RtCapJbufRead(UINT8 *pbuf, UINT32 len)
{
	UINT32 start, end, pos;

	end = gRtCapContext.jbufend;
	start = gRtCapContext.jbufstart;
	pos = gRtCapContext.jbufpos;
	gRtCapContext.jbufpos = RtReadBuf(pbuf, len, start, end, pos);
	return len;
}

static UINT32 RtCapGetJbufRWgap(void)
{
	UINT32 start, end, rp, wp;

	start = gRtCapContext.jbufstart;
	end = gRtCapContext.jbufend;

	if( gRtCapContext.bitctrl & RT_BIT_AVI )
		wp = V5B_MarbGetLstJbufPointer() + 4;
	else
		wp = V5B_MarbGetJbufPointer();

	rp = gRtCapContext.jbufpos;
	if(rp <= wp)
		return wp - rp;
	return end - rp + wp - start;
}

static void RtCapJbufFifoCnt( PTDataBuf pbuf, UINT32 gapsize)
{
	UINT32 size, size1, pos, Len;

	pos = pbuf->Pos;
	Len = (pos+LEN_DATA_PACKET) < pbuf->Len ? (pos+LEN_DATA_PACKET) : pbuf->Len;
	size = RtCapGetJbufRWgap();

	do{
		if(Len  == pbuf->Pos)
		{
			if(gRtCapContext.bitctrl & RT_BIT_VID)
			{
				if(pbuf->Pos >= (pbuf->Len>>1))
				{
					V5B_MarbStopCapture();
					gRtCapContext.bitctrl &= (~RT_BIT_VID);
				}
			}
			gRtCapContext.jpgdone = FUNC_REQ_TIME;

			if( DataInvalid(pbuf) )
				break;

			pos = pbuf->Pos;
			Len = (pos+LEN_DATA_PACKET) < pbuf->Len ? (pos+LEN_DATA_PACKET) : pbuf->Len;
		}

		size1 = Len - pbuf->Pos;
		size = (size > size1) ? size1 : size;
		size1 = RtCapJbufRead(pbuf->pData+pbuf->Pos, size);
		pbuf->Pos += size1;

		size = RtCapGetJbufRWgap();

	} while(size > gapsize);

	if(pbuf->Pos > pos)
	{
		if(gRtCapContext.bitctrl & RT_BIT_VID)
		{
			if(pbuf->Pos >= (pbuf->Len>>2))
			{
				V5B_MarbStopCapture();
				gRtCapContext.bitctrl &= (~RT_BIT_VID);
			}
		}
		gRtCapContext.jpgdone = FUNC_REQ_TIME;
	}

}

static void RtCapEncReq(PTDataBuf pbuf)
{
	UINT16 intflg;

	//wait for done signal
	while(gRtCapContext.jpgdone--)
	{
		intflg = V5B_CpmGetIntFlagSec(INT_MARB0);
		if(intflg)
		{
			if(intflg & (0x1 << INT_JBUF_ERR))
				gRtCapContext.jpgdone = 0;
			if(intflg & (0x1 << INT_JBUF_FIFOCNT))
				RtCapJbufFifoCnt(pbuf, JPEG_FIFO_ACESS_SIZE);
			if(intflg & (0x1 << INT_JPEG_DONE))
			{
				RtCapJbufFifoCnt(pbuf, 0);
				gRtCapContext.jpgdone = 0;
			}
		}
		else
		{
			V5_Delay(FIFO_DELAY);
		}
	}

}

static void RtCapInit(void)
{
	UINT8 val;

	gRtCapContext.jpgdone = FUNC_REQ_TIME;
	gRtCapContext.bitctrl = 0;

	gRtCapContext.jbufstart = V5B_MarbGetJbufStart();
	gRtCapContext.jbufend = V5B_MarbGetJbufEnd() + 4;
	gRtCapContext.jbufpos = gRtCapContext.jbufstart;

	val = V5B_MarbGetModeCtrl0();
	if( BIT1 & val )
		gRtCapContext.bitctrl |= RT_BIT_VID;
	if( !( BIT5 & val ) )
		gRtCapContext.bitctrl |= RT_BIT_AVI;
}

UINT32 RtReqCapJbuf(UINT32 jbufst, UINT32 jbuflen)
{
	TDataBuf buf;

	RtCapInit();

	buf.pData = (UINT8*)gTransferBuf + jbufst;
	buf.Pos = 0;
	buf.Len = jbuflen;

	RtCapEncReq(&buf);

	return buf.Pos;
}

#endif

#endif //V578CMD_DISABLE
