#include "inheaders.h"
#ifndef	V578CMD_DISABLE


#ifdef WIN32 
#define V5M_FUNC_REQ_TIME	0x100
#else
#define V5M_FUNC_REQ_TIME	0xA00
#endif
#define FIFO_DELAY	1

#define DEC_REQTIME			0x200

#define	V5M_LEN_DATA_PACKET			0x40000   // I will remove it in the future
#define V5M_JPEG_FIFO_ACESS_SIZE	1024

#define V5M_DataInvalid(pbuf) ((pbuf == NULL) || (pbuf->pData == NULL) || (pbuf->Len == 0))
#define V5M_BufIsFull(pbuf) (pbuf->Pos >= pbuf->Len)
#define V5M_BufIsEmpty(pbuf) (pbuf->Pos >= pbuf->Len)
#define V5M_DEFAULT_BUFSIZE			0x40000

#define V5M_NUM_CAP_FRAME		10000
#define V5M_IBUF_VFRAME_SIZE	2 //2BYTE


#define V5M_IPP_PREFILTER_DIS	0x0
#define V5M_IPP_PREFILTER_CAP	0x1 

static TV5MHJpegContext gV5MHJpegContext;

/*=========================================================================//
						Static functions
//=========================================================================*/
static void V5M_InitCapEncInt(void);
static void V5M_JpegCapEncReq(void);
static void V5M_CapEncJbufFifoCnt( UINT8 bEnc, UINT32 gapsize);
static void V5M_CapTbufFifoCnt(UINT32 gapsize);
static void V5M_CapIndexReadData(void);
static void V5M_capjbufcnt(void);
static void V5M_HandleUserJbufFull(void);
static UINT32 V5M_IndexThumbBufalloc(void);
static UINT32 V5M_Jbufalloc(UINT32 bufsizeOnetime);
//static void V5M_JpegCallback(UINT8 msg, TDataBuf *dbuf);

static void V5M_DecodeReq(void);
static void V5M_WriteDecodeJpgData(UINT8 bDisp);
static void V5M_DecodeReq(void);
static UINT8 GetDecLbuf0Height(UINT8 fmt);
static UINT32 GetDecYuvSize(UINT8 YUVType, TSize size);
static UINT8 LbufIsOkAftBlank(UINT8 bYuv);
static UINT8 LbufIsOk(UINT8 bYuv);
static void LbufRead(UINT8* pbuf, UINT32 len, UINT8 num);
static void SendYuvData(UINT8 *pbuf, UINT32 len);
static void YuvAdjust(SINT8* pbuf, UINT32 len);

#ifdef WIN32 //for debug
static void JpegMapMem(PTDataBuf pjbuf, PTDataBuf ptbuf, PTDataBuf pibuf);
static void JpegSetMapAdr(PTDataBuf ptbuf, PTDataBuf pibuf);
#endif


static void	_V5MISR_JbufFifoCnt(void);
static void	_V5MISR_JpegDone(void);
static void	_V5MISR_JbufError(void);
static void	_V5MISR_TbufFifoCnt(void);
static void	_V5MISR_ThmbDone(void);
static void	_V5MISR_ThmbError(void);

extern THapiContext gHapiContext;

void V5M_HJpegOpen(void)
{
	//default type.  query
	V5M_SetInterruptType(2);
	//V5M_SetFileOperation(gfileop);
	//V5M_SetMalloc(gpmalloc);
	//V5M_SetMfree(gpfree);
	V5M_SetVideoFileSize(gHapiContext.hapiStdInfo.initvideolength);
}

UINT32 V5M_SetInterruptType(UINT8 inttype) 
{
	gV5MHJpegContext.inttype = inttype;
	return 0;
}

UINT32 V5M_GetInterruptType(UINT8 *inttype) 
{
	*inttype = gV5MHJpegContext.inttype;
	return 0;
}


static void V5M_InitCapEncInt(void)
{
	UINT8 mode = V5M_GetMode();

	V5_MarbIntDisable(MARB_INT_ALL);
	V5B_CpmGetIntFlagSec(INT_MARB0); //Clear int status.
	V5_MarbIntSetCallback(MARB_INT_ALL, NULL);
	if(CAPTURETHUMB_MODE == mode)
	{
		V5_MarbIntSetCallback(INT_TBUF_FIFOCNT, _V5MISR_TbufFifoCnt);
		V5_MarbIntSetCallback(INT_THUMB_DONE, _V5MISR_ThmbDone);
		V5_MarbIntSetCallback(INT_THUMB_ERR, _V5MISR_ThmbError);

		V5_MarbIntEnable(INT_TBUF_FIFOCNT);
		V5_MarbIntEnable(INT_THUMB_DONE);
		V5_MarbIntEnable(INT_THUMB_ERR);
	}
	else
	{
		V5_MarbIntSetCallback(INT_JBUF_FIFOCNT, _V5MISR_JbufFifoCnt);
		V5_MarbIntSetCallback(INT_JPEG_DONE, _V5MISR_JpegDone);
		V5_MarbIntSetCallback(INT_JBUF_ERR, _V5MISR_JbufError);

		V5_MarbIntEnable(INT_JBUF_FIFOCNT);
		V5_MarbIntEnable(INT_JPEG_DONE);
		V5_MarbIntEnable(INT_JBUF_ERR);
	}

	gV5MHJpegContext.stopvid = 0;
	gV5MHJpegContext.jpgdone = V5M_FUNC_REQ_TIME;

	switch(mode)
	{
	case CAPTUREVIDEO_MODE:
	case CAPTUREAVI_MODE:
	case CAPTUREFRAMEVIDEO_MODE:
	case CAPTUREFRAMEAVI_MODE:
	case CAPTUREVIDEOTHUMB_MODE:
	case CAPTUREAVITHUMB_MODE:
	case CAPTUREFRAMEVIDEOTHUMB_MODE:
	case CAPTUREFRAMEAVITHUMB_MODE:
		gV5MHJpegContext.stopvid = 1;
		break;

	default:
		break;
	}

}
//-----------------------------------------------------------
static void V5M_JpegCapEncReq(void)
{
	UINT16 intflg;

	//wait for done signal
	while(gV5MHJpegContext.jpgdone--)
	{
		//Request
		if(gV5MHJpegContext.inttype)
		{
			intflg = V5B_CpmGetIntFlagSec(INT_MARB0);
			if(intflg)
				_ISR_V5MarbIntHandle(intflg);
			else
				V5_Delay(FIFO_DELAY);
		}
		else
			V5_Delay(FIFO_DELAY);
	}

}

//-----------------------------------------------------------
UINT32 V5M_StartCapture(void)
{
	gV5MHJpegContext.curvideosize = 0;
	V5M_InitCapEncInt();
	V5_JpegCapture();
	return 0;
}


//-----------------------------------------------------------
static void _V5MISR_JbufFifoCnt(void)
{
	UINT8 jpegmode;

	jpegmode = V5_JpegGetMode();

	switch(jpegmode)
	{
	case JPEG_MODE_CAPTURE:
	case JPEG_MODE_CAPTUREFRAME:
		V5M_CapEncJbufFifoCnt(0, V5M_JPEG_FIFO_ACESS_SIZE);
		break;

	case JPEG_MODE_ENCODE:
		V5M_CapEncJbufFifoCnt(1, V5M_JPEG_FIFO_ACESS_SIZE);
		break;

	case DISPLAYVIDEO_MODE:
	case DISPLAYAVI_MODE:
		//V5M_DispJbufFifoCnt(V5M_JPEG_FIFO_ACESS_SIZE);
		break;

	default:
		break;
	}
}
//-----------------------------------------------------------
static void _V5MISR_JpegDone(void)
{
	UINT8 mode;

	mode = V5M_GetMode();
	switch(mode)
	{
	case CAPTUREAVITHUMB_MODE:
	case CAPTUREFRAMEAVITHUMB_MODE:
			V5M_CapEncJbufFifoCnt(0, 0);
			V5M_CapTbufFifoCnt(0);
			V5M_CapIndexReadData();		
		break;

	case CAPTUREAVI_MODE:
	case CAPTUREFRAMEAVI_MODE:
			V5M_CapEncJbufFifoCnt(0, 0);
			V5M_CapIndexReadData();
		break;

	case MULTISHOTTHUMB_MODE:
	case CAPTUREVIDEOTHUMB_MODE:
	case CAPTURESTILLTHUMB_MODE:
	case CAPTUREFRAMETHUMB_MODE:
	case CAPTUREFRAMEVIDEOTHUMB_MODE:
			V5M_CapEncJbufFifoCnt(0, 0);
			V5M_CapTbufFifoCnt(0);
		break;

	case CAPTURESTILL_MODE:
	case CAPTUREVIDEO_MODE:
	case CAPTUREFRAME_MODE:
	case CAPTUREFRAMEVIDEO_MODE:
	case MULTISHOT_MODE:
	case MULTISHOTFRAME_MODE:
		V5M_CapEncJbufFifoCnt(0, 0);
		break;

	case CAPTURETHUMB_MODE: 
		V5M_CapTbufFifoCnt(0);
		break;

	case ENCODE_MODE:
		V5M_CapEncJbufFifoCnt(1, 0); //encode
		break;

	default:
		break;
	}
	gV5MHJpegContext.jpgdone = 0;
}
//----------------------------------------------------------------------------
static void	_V5MISR_JbufError(void)
{
	gV5MHJpegContext.jpgdone = 0;
}
//-------------------------------------------------------------------------
static void _V5MISR_TbufFifoCnt(void) 
{
	V5M_CapTbufFifoCnt(V5M_JPEG_FIFO_ACESS_SIZE);
}
//----------------------------------------------------------------------------
static void _V5MISR_ThmbDone(void)  
{
	//if(CAPTURETHUMB_MODE == mode)
	//	gHJpegContext.jpgdone = 0;
}
//--------------------------------------------------------------------------
static void	_V5MISR_ThmbError(void)
{
	//if(CAPTURETHUMB_MODE == mode)
	//	gHJpegContext.jpgdone = 0;
}
//----------------------------------------------------------------------------
static void V5M_CapEncJbufFifoCnt( UINT8 bEnc, UINT32 gapsize) //bEnc 0 cap, 1 enc
{
	UINT32 size, size1, pos, Len;
	PTDataBuf pbuf;

	if(bEnc)
		pbuf = gV5MHJpegContext.fucbuf.enc.pdst;  //will handle it later
	else
		pbuf = &gV5MHJpegContext.fucbuf.cap.jbuf;

	if ( V5M_DataInvalid(pbuf))
		return ;
	if (V5M_BufIsFull(pbuf))
		{
			V5M_HandleUserJbufFull();
			if (V5M_BufIsFull(pbuf))
				return ;
		}

	pos = pbuf->Pos;
	Len = (pos + V5M_LEN_DATA_PACKET) < pbuf->Len ? (pos + V5M_LEN_DATA_PACKET) : pbuf->Len;
	size = V5_MarbGetJbufRWgap();

	do{
		if (V5M_BufIsFull(pbuf))
		{
			V5M_HandleUserJbufFull();
			if (V5M_BufIsFull(pbuf))
				return ;
		}
			
		size1 = Len - pbuf->Pos;
		size = (size > size1) ? size1 : size;
		size1 = V5_MarbJbufRead(pbuf->pData+pbuf->Pos, size);
		pbuf->Pos += size1;

		size = V5_MarbGetJbufRWgap();
	} while(size > gapsize);

	if(pbuf->Pos > pos)
		V5M_capjbufcnt();
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static void V5M_CapTbufFifoCnt(UINT32 gapsize)
{
	UINT32 size, size1, pos, Len;
	PTDataBuf pbuf;

	pbuf = &gV5MHJpegContext.fucbuf.cap.tbuf;
	if(pbuf == NULL)
		return ;

	if ( V5M_DataInvalid(pbuf) || V5M_BufIsFull(pbuf))
		return ;

	pos = pbuf->Pos;
	Len = (pos + V5M_LEN_DATA_PACKET) < pbuf->Len ? (pos + V5M_LEN_DATA_PACKET) : pbuf->Len;
	size = V5_MarbGetThumbRWgap();

	do {
		if (V5M_BufIsFull(pbuf))
			return ;

		size1 = Len - pbuf->Pos;
		size = (size > size1) ? size1 : size;
		size1 = V5_MarbThumbRead(pbuf->pData+pbuf->Pos, size);
		pbuf->Pos += size1;

		size = V5_MarbGetThumbRWgap();

	} while(size > gapsize);
}

//----------------------------------------------------------------------------
static void V5M_CapIndexReadData()
{
	UINT32 size, size1;
	PTDataBuf pbuf;

	pbuf = &gV5MHJpegContext.fucbuf.cap.ibuf;

	if ( V5M_DataInvalid(pbuf) || V5M_BufIsFull(pbuf))
		return ;
	if (pbuf->Pos)
		return;

	size = V5B_MarbGetFrmCount() << 1;
	if  (pbuf->Len < size)
		return;
	size1 = V5_MarbIndexRead(pbuf->pData, size);
	pbuf->Pos += size1;
}

//----------------------------------------------------------------------------
static void V5M_capjbufcnt(void)
{
	if(gV5MHJpegContext.stopvid)
	{
#ifdef NCSC
			V5B_MarbStopCapture();
			gV5MHJpegContext.stopvid = 0;
#else

		if((gV5MHJpegContext.curvideosize + gV5MHJpegContext.fucbuf.cap.jbuf.Pos) >= gV5MHJpegContext.videosize)  //will change. not pos,but total file size take effect
			{
				V5B_MarbStopCapture();
				gV5MHJpegContext.stopvid = 0;
			}
#endif
		}
		gV5MHJpegContext.jpgdone = V5M_FUNC_REQ_TIME;
}

//----------------------------------------------------------------------------
void V5M_SetVideoFileSize(UINT8 filesize)
{
	gV5MHJpegContext.videosize = (filesize << 10);
}
//---------------------------------------------------------------------------
UINT32 V5M_getCaptureInfoToFile(char *filename, UINT32 bufsizeOnetime )
{
	UINT32 errcode;
	#ifdef WIN32 //for debug
	TV5MHJpegCapBuf *ptempcap;
	#endif
	
	errcode = V5M_IndexThumbBufalloc();
	if (errcode)
		return errcode;

	errcode = V5M_Jbufalloc(bufsizeOnetime);
	if (errcode)
		return errcode;

	strcpy(gV5MHJpegContext.jfilename,filename);
	
	gV5MHJpegContext.fucbuf.cap.jbufhead = NULL;
	gV5MHJpegContext.fucbuf.cap.jbuflast = NULL;

	#ifdef WIN32 //for debug
	ptempcap = &gV5MHJpegContext.fucbuf.cap;
	JpegSetMapAdr(&(ptempcap->tbuf), &(ptempcap->ibuf));
	#endif

	V5M_JpegCapEncReq(); 


	#ifdef WIN32 //for debug
	JpegMapMem(&(ptempcap->jbuf), &(ptempcap->tbuf), &(ptempcap->ibuf));
	#endif

	V5M_SaveJbufToFile();
	V5M_JbufFree();
	V5M_SaveIbufToFile();
	V5M_SaveTbufToFile();
	V5M_IndexThumbBufFree();
	return 0;
}

//-----------------------------------------------------------------------------
//void V5M_SaveJbufToFile(void)
//{
//	TDataBuf *pbuf;
//	UINT8 mode;
//	char filename[200];
//	V5MF_HANDLE fp;
//	TV5MHJpegMemList *p, *q;
//
//	p = gV5MHJpegContext.fucbuf.cap.jbufhead;
//	pbuf = &gV5MHJpegContext.fucbuf.cap.jbuf;
//	if ((p == NULL) && ((pbuf == NULL) || (pbuf->Pos == 0) ||(pbuf->pData == NULL))) 
//		return ;
//
//	mode = V5B_MarbGetCaptureMode();
//	strcpy(filename, gV5MHJpegContext.jfilename);
//	if ( (V5M_CAPTURE_STILL == mode)||( V5M_CAPTURE_MULTISHOT == mode) )
//		strcat(filename, ".jpg");
//	else if ( V5M_CAPTURE_AVI == mode)
//		strcat(filename, ".avi");
//	else return ;
//
//	fp = gHapiContext.fileop.fopen(filename,"wb"); 
//	if (fp)
//	{
//		while (p)
//		{
//			gHapiContext.fileop.fwrite(p->pData, pbuf->Len, fp);
//			q = p;
//			p = (TV5MHJpegMemList *) p->next;
//			gHapiContext.pfree(q->pData, pbuf->Len);
//			gHapiContext.pfree(q, sizeof(TV5MHJpegMemList));
//		}
//		
//		if ((pbuf->Len) && (pbuf->Pos) && (pbuf->pData))
//			gHapiContext.fileop.fwrite(pbuf->pData, pbuf->Pos, fp);
//		gHapiContext.fileop.fclose(fp);
//	}
//
//}

void V5M_SaveJbufToFile(void)
{
	TDataBuf *pbuf;
	UINT8 mode;
	char filename[200];
	TV5MHJpegMemList *p;

	p = gV5MHJpegContext.fucbuf.cap.jbufhead;
	pbuf = &gV5MHJpegContext.fucbuf.cap.jbuf;
	if ((p == NULL) && ((pbuf == NULL) || (pbuf->Pos == 0) ||(pbuf->pData == NULL))) 
		return ;

	mode = V5B_MarbGetCaptureMode();
	strcpy(filename, gV5MHJpegContext.jfilename);
	if ( (V5M_CAPTURE_STILL == mode)||( V5M_CAPTURE_MULTISHOT == mode) )
		strcat(filename, ".jpg");
	else if ( V5M_CAPTURE_AVI == mode)
		strcat(filename, ".avi");
	else return ;

	V5M_SaveImageFile(filename,(char *) pbuf->pData, pbuf->Pos);
}


//----------------------------------------------------------------------------
//void V5M_SaveIbufToFile()
//{
//	TDataBuf *pbuf;
//	char filename[200];
//	V5MF_HANDLE fp;
//
//	pbuf = &gV5MHJpegContext.fucbuf.cap.ibuf;
//	if ((pbuf->Len) && (pbuf->Pos) && (pbuf->pData))
//	{
//		strcpy(filename, gV5MHJpegContext.jfilename);
//		strcat(filename, ".idx");
//	  
//		fp = gHapiContext.fileop.fopen(filename, "wb");
//		if (fp)
//		{
//			gHapiContext.fileop.fwrite(pbuf->pData, pbuf->Pos,fp);
//			gHapiContext.fileop.fclose(fp);
//		}
//	}
//}


void V5M_SaveIbufToFile()
{
	TDataBuf *pbuf;
	char filename[200];

	pbuf = &gV5MHJpegContext.fucbuf.cap.ibuf;
	if ((pbuf->Len) && (pbuf->Pos) && (pbuf->pData))
	{
		strcpy(filename, gV5MHJpegContext.jfilename);
		strcat(filename, ".idx");
	  
		V5M_SaveImageFile(filename, (char *) pbuf->pData, pbuf->Pos);
	}
}
//----------------------------------------------------------------------------
//void V5M_SaveTbufToFile()
//{
//	TDataBuf *pbuf;
//	char filename[200];
//	V5MF_HANDLE fp;
//
//	pbuf = &gV5MHJpegContext.fucbuf.cap.tbuf;
//	if ((pbuf->Len) && (pbuf->Pos) && (pbuf->pData))
//	{
//		strcpy(filename, gV5MHJpegContext.jfilename);
//		strcat(filename, ".yuv");  
//		// note : yuv data can be convert to other format file firstly , and then save
//		fp = gHapiContext.fileop.fopen(filename, "wb");
//		if (fp)
//		{
//			gHapiContext.fileop.fwrite(pbuf->pData, pbuf->Pos,fp);
//			gHapiContext.fileop.fclose(fp);
//		}
//	}
//}
//----------------------------------------------------------------------------
void V5M_SaveTbufToFile()
{
	TDataBuf *pbuf;
	char filename[200];

	pbuf = &gV5MHJpegContext.fucbuf.cap.tbuf;
	if ((pbuf->Len) && (pbuf->Pos) && (pbuf->pData))
	{
		strcpy(filename, gV5MHJpegContext.jfilename);
		strcat(filename, ".yuv");  
		// note : yuv data can be convert to other format file firstly , and then save
		V5M_SaveImageFile(filename, (char *) pbuf->pData, pbuf->Pos);
	}
}


//----------------------------------------------------------------------------

static void V5M_HandleUserJbufFull(void)
{
	TDataBuf *pbuf;
	UINT32 errcode;
	TV5MHJpegMemList *newnode;
	TV5MHJpegMemList **pjbufhead;
	
	pbuf = &gV5MHJpegContext.fucbuf.cap.jbuf;
	pjbufhead = &gV5MHJpegContext.fucbuf.cap.jbufhead;
	newnode = (TV5MHJpegMemList *) gHapiContext.pmalloc(sizeof(TV5MHJpegMemList));
	newnode->pData = pbuf->pData;
	newnode->next = NULL;
	gV5MHJpegContext.curvideosize += pbuf->Len;

	if (*pjbufhead == NULL)
		*pjbufhead = newnode;
	else
		gV5MHJpegContext.fucbuf.cap.jbuflast->next = (UINT8 *) newnode;
	gV5MHJpegContext.fucbuf.cap.jbuflast = newnode;

	errcode = V5M_Jbufalloc(pbuf->Len);
	if (errcode)
		return ;

	//fp = gHapiContext.fileop.fopen(filename,"ab"); 
	//if (fp)
	//{
	//	gHapiContext.fileop.fwrite(pbuf->pData, pbuf->Pos, fp);
	//	gHapiContext.fileop.fclose(fp);
	//	gV5MHJpegContext.curvideosize += pbuf->Pos;
	//	pbuf->pData = 0;
	//	pbuf->Pos =0;
	//}
}

//---------------------------------------------------------------------------
static UINT32 V5M_IndexThumbBufalloc(void)
{
	UINT8 mode, *buf;
	UINT32 tsize, isize;
	TDataBuf tbuf, ibuf;
	
	mode = V5M_GetMode();
	V5M_GetIndexThumbBufSize(mode, &tsize, &isize);

	buf = gHapiContext.pmalloc(tsize);
	if ( NULL == buf)
		return CANNOT_ALLOC_MEMORY;
	//memset(buf, 0, tsize);
	tbuf.Len = tsize;
	tbuf.Pos = 0;
	tbuf.pData = buf;
	gV5MHJpegContext.fucbuf.cap.tbuf = tbuf;

	buf = gHapiContext.pmalloc(isize);
	if ( NULL == buf)
		return CANNOT_ALLOC_MEMORY;
	//memset(buf, 0, isize);
	ibuf.Len = isize;
	ibuf.Pos = 0;
	ibuf.pData = buf;
	gV5MHJpegContext.fucbuf.cap.ibuf = ibuf;
	return 0;
}
//--------------------------------------------------
void V5M_IndexThumbBufFree(void)
{
	TDataBuf *pbuf;

	pbuf = &gV5MHJpegContext.fucbuf.cap.ibuf;
	if (pbuf->pData)
		gHapiContext.pfree(pbuf->pData, pbuf->Len);	
	pbuf->pData = 0;
	pbuf->Len =0;
	pbuf->Pos =0;

	pbuf = &gV5MHJpegContext.fucbuf.cap.tbuf;
	if (pbuf->pData)
		gHapiContext.pfree(pbuf->pData, pbuf->Len);
	pbuf->pData = 0;
	pbuf->Len =0;
	pbuf->Pos =0;
}
//----------------------------------------------------------------------------
void V5M_JbufFree(void)
{
	TDataBuf *pbuf;

	pbuf = &gV5MHJpegContext.fucbuf.cap.jbuf;
	if (pbuf->pData)
		gHapiContext.pfree(pbuf->pData, pbuf->Len);

	pbuf->pData = 0;
	pbuf->Len =0;
	pbuf->Pos =0;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static UINT32 V5M_Jbufalloc(UINT32 bufsizeOnetime)
{
	UINT8  *buf;
	TDataBuf jbuf;

	if (bufsizeOnetime == 0 ) 
		bufsizeOnetime = V5M_DEFAULT_BUFSIZE;

	buf = gHapiContext.pmalloc(bufsizeOnetime);
	if ( NULL == buf)
		return CANNOT_ALLOC_MEMORY;

	//memset(buf, 0, bufsizeOnetime);
	jbuf.Len = bufsizeOnetime;
	jbuf.Pos = 0;
	jbuf.pData = buf;
	gV5MHJpegContext.fucbuf.cap.jbuf = jbuf;
	return 0;
}
//----------------------------------------------------------------------------
UINT32 V5M_GetIndexThumbBufSize(UINT8 mode, UINT32 *tsize, UINT32 *isize)
{
	*tsize = 0;
	*isize = 0;

	switch(mode)
	{
	case CAPTURESTILLTHUMB_MODE:
	case CAPTUREVIDEOTHUMB_MODE:
	case CAPTUREFRAMETHUMB_MODE:
	case CAPTUREFRAMEVIDEOTHUMB_MODE:
		*tsize = V5M_GetThumbActSize();
		break;
	case MULTISHOTTHUMB_MODE:
		{
			UINT8 cnt = V5B_MarbGetMshotFcnt()+1;
			*tsize = V5M_GetThumbActSize();
			*tsize *= cnt;
		}
		break;

	case CAPTUREAVI_MODE:
	case CAPTUREFRAMEAVI_MODE:
		*isize = V5M_IBUF_VFRAME_SIZE * V5M_NUM_CAP_FRAME;
		break;

	case CAPTUREAVITHUMB_MODE:
	case CAPTUREFRAMEAVITHUMB_MODE:
		*tsize = V5M_GetThumbActSize();
		*isize = V5M_IBUF_VFRAME_SIZE * V5M_NUM_CAP_FRAME;
		break;

	case CAPTURETHUMB_MODE: //only thumb buf used.
		*tsize = V5M_GetThumbActSize();
		break;
	default:
		break;
	}
	return 0;
}
//----------------------------------------------------------------------------
UINT32 V5M_GetThumbActSize(void)
{
	TSize size;
			
	V5B_IppGetThumbSize(&size);
	return ((size.cx * size.cy)<<1); 
}
//----------------------------------------------------------------------------

void V5M_SetCompRatio(UINT8 value)
{
	UINT8 value2;
	value2 = value * 4 + 1;
	if (value2 > 255) 
		return ;

	V5B_JpegEnAbrc();
	V5B_JpegSetTCR(value2);
	gHapiContext.hapiStdInfo.compresssizetype = V5M_COMPRESSSIZE_RATIO;
	gHapiContext.hapiStdInfo.jpegcomprate = value;
}

void V5M_SetCompedJFileSize(UINT16 filesize)
{
	TSize capsize;
	UINT8 ratio;
	//UINT32 len;

	if (!filesize)
		return;
	//V5M_AutoSetCapSizer();     //temp, maybe it will be removed
	/*V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();*/
	V5B_IppGetCapSize(&capsize);
	ratio = (UINT8)(((UINT32) (capsize.cx * capsize.cy / filesize)) >> 9);  //filesize unit is KByte, yuv422 data format
	V5M_SetCompRatio(ratio);
	gHapiContext.hapiStdInfo.compresssizetype = V5M_COMPRESSSIZE_FILESIZE;
	gHapiContext.hapiStdInfo.jpegfilesize = filesize;
}
//void V5M_SetTWC(UINT32 value)
//{
//	value = value / 4;
//	V5B_JpegEnAbrc();
//	//V5B_JpegSetTWC(value);
//}
#ifdef WIN32

#include "../../s3c44box/s3c44box.h"
#include "../v568env/v5initenv.h"

static void SendYuvData(UINT8 *pbuf, UINT32 len)
{
	WriteBuf(0, (char *)pbuf, len);
	RepeatWriteReg(V5_REG_JPEG_DCT_DATA, len);
}

#else
static void SendYuvData(UINT8 *pbuf, UINT32 len)
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

//----------------------------------------------------------------------------

void V5M_HJpegDisplayFrame(PTJpegIndex pinfo, PTDataBuf psrc)
{
	//gV5MHJpegContext.fucbuf.disp.pjbuf = psrc;
	gV5MHJpegContext.fucbuf.disp.jbuf.pData = psrc->pData;
	gV5MHJpegContext.fucbuf.disp.jbuf.Pos = pinfo->offset;
	gV5MHJpegContext.fucbuf.disp.jbuf.Len = pinfo->eop;

	V5_JpegDecode(pinfo);

	V5M_WriteDecodeJpgData(1);
	V5M_DecodeReq();
}
//----------------------------------------------------------------------------
UINT8 V5M_DecIsOk(void)
{
	UINT8 x = V5B_JpegGetStatus();
	
	return (UINT8)(x & V5B_JPEG_STATUS_DD);
}
//---------------------------------------------------------
UINT8 V5M_LbufIsOk(UINT8 bYuv)
{
	if(bYuv)
		return V5B_Lbuf0Isok();
	else
		return V5B_Lbuf1Isok();
}
//------------------------------------------------------

static void V5M_DecodeReq(void)
{
	UINT16 flg = DEC_REQTIME;

	while(flg--)
	{
		if(V5M_DecIsOk())
		{
			//Delay for last 8 line to lcd.
			V5_Delay(5);
			break;
		}

		V5M_WriteDecodeJpgData(1);
		V5_Delay(1);
	}
}
//---------------------------------------------------------------------------------------
static void V5M_WriteDecodeJpgData(UINT8 bDisp)
{
	UINT32 length, marblen;
	UINT16 leftlen;
	UINT8 *ptempbuf;
	PTDataBuf pbuf;
	UINT8 dat[4] = {0xff, 0xff, 0xff, 0xd9};

	if(bDisp)
		pbuf = &gV5MHJpegContext.fucbuf.disp.jbuf;
	else
		pbuf = gV5MHJpegContext.fucbuf.dec.psrc;

	if (V5M_DataInvalid(pbuf) || (V5M_BufIsEmpty(pbuf)))
		return ;

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
		leftlen = (UINT16)(length&0x3);
		length = (length>>2)<<2;
		if(!leftlen)//dat[3,4] = {0xff, 0xd9}
		{
			dat[0] = ptempbuf[length - 4];
			dat[1] = ptempbuf[length - 3];
			length -= 4;
		}
		if(leftlen == 3)
		{
			dat[0] = ptempbuf[length];
		}
		V5_MarbJbufWrite(ptempbuf, length);
		V5_MarbJbufWrite(dat, 4);
	}
}

//----------------------------------------------------------------------------
//void V5M_Display(char *filename, UINT8 bVideo )
//{
//	V5MF_HANDLE fp;
//	UINT32 size, count;
//	UINT8 *pbuf;
//	TJpegIndex info;
//	TPoint winpt;
//	TSize dispSize, winsize;
//	TDataBuf dbuf;
//	
//	fp = gHapiContext.fileop.fopen(filename, "rb");
//	if (!fp)
//		return;
//
//	size = gHapiContext.fileop.flen(fp);  
//	pbuf = (UINT8 *) gHapiContext.pmalloc(size);
//	if  (pbuf== NULL)
//		return;
//	count = gHapiContext.fileop.fread( pbuf, size,fp);
//	gHapiContext.fileop.fclose(fp);
//
//	V5M_SetModeAction(DISPLAYSTILL_MODE);
//
//	JpegParse(pbuf, size, &info);
//	winpt.x = 0;
//	winpt.y = 0;
//	winsize.cx = (info.ImageSize.cx >> 1) << 1;
//	winsize.cy = (info.ImageSize.cy >> 1) << 1;
//	V5B_IppGetDispSize(&dispSize);
//	V5_IppSetDispSizerEx2(info.JpgSize, winpt, winsize, dispSize);
//
//	dbuf.Len = size;
//	dbuf.Pos = 0;
//	dbuf.pData = pbuf;
//
//	if(!bVideo)
//	{
//		//V5M_SetModeAction(DISPLAYSTILL_MODE);
//		V5M_HJpegDisplayFrame(&info, &dbuf);
//	}
//	else
//	{
//		//V5M_SetModeAction(DISPLAYAVI_MODE);
//		V5M_HJpegDispVidNorm(&dbuf);
//	}
//	gHapiContext.pfree(pbuf, size);
//
//}

void V5M_Display(char *filename, UINT8 bVideo )
{
	UINT32 size;
	UINT8 *pbuf;
	TJpegIndex info;
	TPoint winpt;
	TSize dispSize, winsize;
	TDataBuf dbuf;
	
	pbuf = (UINT8 *) V5M_LoadImageFile(filename, &size);
	if  (pbuf== NULL)
		return;

	V5M_SetModeAction(DISPLAYSTILL_MODE);

	JpegParse(pbuf, size, &info);
	winpt.x = 0;
	winpt.y = 0;
	winsize.cx = (info.ImageSize.cx >> 1) << 1;
	winsize.cy = (info.ImageSize.cy >> 1) << 1;
	V5B_IppGetDispSize(&dispSize);
	V5_IppSetDispSizerEx2(info.JpgSize, winpt, winsize, dispSize);

	dbuf.Len = size;
	dbuf.Pos = 0;
	dbuf.pData = pbuf;

	if(!bVideo)
	{
		//V5M_SetModeAction(DISPLAYSTILL_MODE);
		V5M_HJpegDisplayFrame(&info, &dbuf);
	}
	else
	{
		//V5M_SetModeAction(DISPLAYAVI_MODE);
		V5M_HJpegDispVidNorm(&dbuf);
	}
	gHapiContext.pfree(pbuf, size);

}
//----------------------------------------------------------------------------
void V5M_HJpegDispVidNorm(TDataBuf* pVid)
{
	TJpegIndex info;
	TDataBuf dbuf;
	UINT8* pbuf;
	UINT32 len;

	if(pVid == NULL || pVid->pData == NULL || pVid->Len == 0)
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

			V5M_HJpegDisplayFrame(&info, &dbuf);

			//Delay for frame time control
			V5_Delay(50);
		}
		pbuf += info.eop;
		len -= info.eop;
	}

}

//-----------------------------------------------------------------------------------------------
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


static UINT32 GetDecYuvSize(UINT8 YUVType, TSize size)
{
		if(YUVType == V5B_LBUF_YUV_444)
			return GetYuvSize(V5B_LBUF_YUV_422, size);
		else
			return GetYuvSize(YUVType, size);
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

static UINT8 LbufIsOk(UINT8 bYuv)
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
//-----------------------------------------------------------------------------------------------
//type 0-decode, 1-decode ipp, 2-decode frame
void V5M_JpegDecode(PTDataBuf psrc, PTDataBuf pdst, UINT8 type)
{
	TSize decsize;
	TJpegIndex	info;

	ClearGbufPoint();
	
	gV5MHJpegContext.fucbuf.dec.psrc = psrc;
	gV5MHJpegContext.fucbuf.dec.pdst = pdst;

	JpegParse(psrc->pData, psrc->Len, &info);
	if(info.YUVType == JPEG_UNKNOWN)
		return;

	V5_JpegDecode(&info);

	gV5MHJpegContext.fucbuf.dec.psrc->Len = info.eop;
	gV5MHJpegContext.fucbuf.dec.psrc->Pos = info.offset;

	V5M_WriteDecodeJpgData(0);

	switch(type)
	{
	case V5M_DECODE:
		V5M_ReadWriteDecData(info.YUVType, info.JpgSize, pdst);
		break;
	case V5M_DECODEIPP:
		V5B_IppGetDispSize(&decsize);
		V5M_ReadWriteDecDataIppLcd(info.JpgSize.cy, info.YUVType, decsize, pdst, 0);
		break;
	case V5M_DECODEFRAME:
		V5B_LcdcGetAWinSize(&decsize);
		V5M_ReadWriteDecDataIppLcd(info.JpgSize.cy, info.YUVType, decsize, pdst, 0);
		break;
	}

}
//-----------------------------------------------------------------------------------------------
void V5M_ReadWriteDecDataIppLcd(UINT16 jpgheight, UINT8 yuvtype, TSize finalImageSize, PTDataBuf pdst, UINT8 bYuv)
{
	UINT32 vwc;
	TSize size;
	UINT8 lbufheight;
	UINT16 i = 0, j = 0, flag = DEC_REQTIME;
	
	lbufheight = GetDecLbuf0Height(yuvtype);
	i = i + lbufheight;
	pdst->Pos = 0;
	size.cx = finalImageSize.cx;
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
						i = i + lbufheight;
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
			return;

		if(bYuv)
			LbufRead(pdst->pData + pdst->Pos, vwc, 0);
		else
			LbufRead(pdst->pData + pdst->Pos, vwc, 1);

		pdst->Pos += vwc;
		j = j + size.cy;

		/*if(bYuv)
			WriteDecodeYuvData(0);
		else*/
			V5M_WriteDecodeJpgData(0);

		if(j >= finalImageSize.cy)
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

}

//-----------------------------------------------------------------------------------------------
void V5M_ReadWriteDecData(UINT8 YUVType, TSize jpegsize, PTDataBuf pdst)
{
	UINT32 vwc;
	TSize size;
	UINT16 i = 0, flag = DEC_REQTIME;

	pdst->Pos = 0;
	size = jpegsize;
	while( (i < jpegsize.cy) && (flag --) )
	{
		if(V5B_Lbuf0Isok() == 0)
		{
			V5_Delay(1);
			continue;
		}

		size.cy = GetDecLbuf0Height(YUVType);
		vwc = GetDecYuvSize(YUVType, size);

		if((pdst->Pos + vwc) > pdst->Len)
			return;

		LbufRead(pdst->pData + pdst->Pos, vwc, 0);
		pdst->Pos += vwc;
		i = (UINT16)(i + size.cy);
		
		if(i < jpegsize.cy)
		{
			V5B_JpegRestartDecode();
			//reset flag
			flag = DEC_REQTIME;
		}
		V5M_WriteDecodeJpgData(0);

	}

	V5B_JpegRestartDecode();

}
//-----------------------------------------------------------------
static void YuvAdjust(SINT8* pbuf, UINT32 len)
{
	UINT32 i;
	SINT16 val;
	
	for(i=0; i<len; i++)
	{
		val = (SINT16)(*(pbuf+i));
		val += 128;
		*(pbuf+i) = (SINT8)val;
	}
}
//-----------------------------------------------------------------------------------------------
//void V5M_DecodeEncode(char *srcfile, char *dstfile, UINT8 dectype)
//{
//	V5MF_HANDLE fp;
//	UINT32 size, count, decsize, len;
//	UINT8 *pbuf;
//	TSize dispSize;
//	TJpegIndex info;
//	TDataBuf dsrc,dtemp, dtemp2;
//	
//	fp = gHapiContext.fileop.fopen(srcfile, "rb");
//	if (!fp)
//		return;
//
//	size = gHapiContext.fileop.flen(fp);  
//	pbuf = (UINT8 *) gHapiContext.pmalloc(size);
//	if  (pbuf== NULL)
//		return;
//	count = gHapiContext.fileop.fread( pbuf, size,fp);
//	gHapiContext.fileop.fclose(fp);
//	if (count == 0)
//		return;
//	JpegParse(pbuf, size, &info);
//	dsrc.pData = pbuf;
//	dsrc.Len = size;
//	dsrc.Pos = 0;
//
//	decsize = 20 * size;
//	pbuf = (UINT8 *) gHapiContext.pmalloc(decsize);
//	if  (pbuf== NULL)
//		return;
//	dtemp.pData = pbuf;
//	dtemp.Len = decsize;
//	dtemp.Pos = 0;
//
//	pbuf = (UINT8 *) gHapiContext.pmalloc(decsize);
//	if  (pbuf== NULL)
//		return;
//	dtemp2.pData = pbuf;
//	dtemp2.Len = decsize;
//	dtemp2.Pos = 0;
//
//	V5M_SetDecodeMode(dectype);
//	V5M_JpegDecode(&dsrc, &dtemp, dectype);
//	dsrc.Pos = 0;
//
//	V5B_IppGetDispSize(&dispSize);
//	YuvAdjust((char *) dtemp.pData, dtemp.Len);
//	len = Yuv422_Blkorder((char *)dtemp.pData, (char *)dtemp2.pData, YUV422_UYVY, dispSize.cx, dispSize.cy);
//	dtemp2.Pos = len;
//	V5M_SetModeAction(ENCODE_MODE);
//	V5M_JpegEncode(&dtemp2, &dsrc, V5B_LBUF_YUV_422, dispSize);
//
//	fp = gHapiContext.fileop.fopen(dstfile, "wb");
//	if (fp)
//	{
//		gHapiContext.fileop.fwrite(dsrc.pData, dsrc.Pos, fp) ;
//		gHapiContext.fileop.fclose(fp);
//	}
//	gHapiContext.pfree(dsrc.pData, dsrc.Len);
//	gHapiContext.pfree(dtemp.pData, dtemp.Len);
//	gHapiContext.pfree(dtemp2.pData, dtemp2.Len);
//
//}

void V5M_DecodeEncode(char *srcfile, char *dstfile, UINT8 dectype)
{
	UINT32 size, decsize, len;
	UINT8 *pbuf;
	TSize dispSize;
	TJpegIndex info;
	TDataBuf dsrc,dtemp, dtemp2;
	
	pbuf = (UINT8 *) V5M_LoadImageFile(srcfile, &size);
	if  (pbuf== NULL)
		return;

	JpegParse(pbuf, size, &info);
	dsrc.pData = pbuf;
	dsrc.Len = size;
	dsrc.Pos = 0;

	//decsize = 20 * size;
	decsize = GetYuvSize(V5B_LBUF_YUV_422, info.JpgSize);
	pbuf = (UINT8 *) gHapiContext.pmalloc(decsize);
	if  (pbuf== NULL)
		return;
	dtemp.pData = pbuf;
	dtemp.Len = decsize;
	dtemp.Pos = 0;

	pbuf = (UINT8 *) gHapiContext.pmalloc(decsize);
	if  (pbuf== NULL)
		return;
	dtemp2.pData = pbuf;
	dtemp2.Len = decsize;
	dtemp2.Pos = 0;

	V5M_SetDecodeMode(dectype);
	V5M_JpegDecode(&dsrc, &dtemp, dectype);
	dsrc.Pos = 0;

	V5B_IppGetDispSize(&dispSize);
	YuvAdjust((char *) dtemp.pData, dtemp.Len);
	len = Yuv422_Blkorder((char *)dtemp.pData, (char *)dtemp2.pData, YUV422_UYVY, dispSize.cx, dispSize.cy);
	dtemp2.Pos = len;
	V5M_SetModeAction(ENCODE_MODE);
	V5M_JpegEncode(&dtemp2, &dsrc, V5B_LBUF_YUV_422, dispSize);

	V5M_SaveImageFile(dstfile, (char *) dsrc.pData, dsrc.Pos);

	gHapiContext.pfree(dsrc.pData, dsrc.Len);
	gHapiContext.pfree(dtemp.pData, dtemp.Len);
	gHapiContext.pfree(dtemp2.pData, dtemp2.Len);

}
//-----------------------------------------------------------------
void V5M_SetDecodeMode(UINT8 dectype)
{
    switch(dectype)
	{
	case V5M_DECODEIPP:
		V5M_SetModeAction(DECODEIPP_MODE);
		break;
	case V5M_DECODEFRAME:
		V5M_SetModeAction(DECODEFRAME_MODE);
		break;
	default:
		V5M_SetModeAction(DECODE_MODE);
	}
}
//-----------------------------------------------------------------------------------------------
void V5M_JpegEncode(PTDataBuf psrc, PTDataBuf pdst, UINT8 fmt, TSize size)
{
	TSize size1;
	UINT32 len;

	gV5MHJpegContext.fucbuf.enc.psrc = psrc;
	gV5MHJpegContext.fucbuf.enc.pdst = pdst;

	GetJpegSize(fmt, size, &size1);
	len = GetYuvSize(fmt, size1);
	if(psrc->Len < len)
		return;

	#ifdef WIN32 //for debug
	JpegSetMapAdr(NULL, NULL);
	#endif

	V5M_InitCapEncInt();
	V5_JpegEncode(fmt, size);
	SendYuvData(psrc->pData, len);
	V5M_JpegCapEncReq();

	#ifdef WIN32 //for debug
	JpegMapMem(pdst, NULL, NULL);
	#endif

}
//-----------------------------------------------------------------------------------------------
//void V5M_SetDecodeIPPSizer(char *srcfile, UINT8 ratio)
//{
//	V5MF_HANDLE fp;
//	UINT32 size, count;
//	UINT8 *pbuf;
//	TJpegIndex info;
//	TSize decsize;
//
//	fp = gHapiContext.fileop.fopen(srcfile, "rb");
//
//	if (!fp)
//		return;
//
//	size = gHapiContext.fileop.flen(fp);  
//	pbuf = (UINT8 *) gHapiContext.pmalloc(size);
//	if  (pbuf== NULL)
//		return;
//	count = gHapiContext.fileop.fread( pbuf, size,fp);
//	gHapiContext.fileop.fclose(fp);
//	if (count == 0)
//		return;
//	JpegParse(pbuf, size, &info);
//	gHapiContext.pfree(pbuf, size);
//
//	decsize.cx = ((info.ImageSize.cx * ratio /10) >> 1 ) << 1;
//	decsize.cy = ((info.ImageSize.cy * ratio /10) >> 1 ) << 1;
//	V5_IppSetDispSizer(decsize) ;
//	//V5_DispSetAmemSize(&(this->m_option.layera));
//	//V5_JpegAutoSetImage();
//	//V5_MarbAutoSetMem();
//}
void V5M_SetDecodeIPPSizer(char *srcfile, UINT8 ratio)
{
	UINT32 size;
	UINT8 *pbuf;
	TJpegIndex info;
	TSize decsize;

	pbuf = (UINT8 *) V5M_LoadImageFile(srcfile, &size);
	if (pbuf)
	{
		JpegParse(pbuf, size, &info);
		gHapiContext.pfree(pbuf, size);

		decsize.cx = ((info.ImageSize.cx * ratio /10) >> 1 ) << 1;
		decsize.cy = ((info.ImageSize.cy * ratio /10) >> 1 ) << 1;
		V5_IppSetDispSizer(decsize) ;
	}
	//V5_DispSetAmemSize(&(this->m_option.layera));
	//V5_JpegAutoSetImage();
	//V5_MarbAutoSetMem();
}
//-----------------------------------------------------------------------------------------------

#endif //V578CMD_DISABLE

