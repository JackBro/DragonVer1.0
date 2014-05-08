
#include "internal.h"
#ifndef	V578CMD_DISABLE

#include "../v568testdemo/testjpeg.h"



static UINT8 CalMulFrmRect(UINT8 cursel, TPoint *pt, TSize *size);
static void DrawMulFrm(UINT8 cursel, UINT32 color, UINT8 thick_sel);
static void DrawMulThum(UINT8 cursel, TSize thumsize, UINT8* pData);
static void DisplayMulThumb(UINT8* pData, UINT32 len);
static UINT8 CalMulSelMove(UINT8 cursel, UINT8 direct);
static void DispStillVideo(UINT8* pbuf, UINT32 len, UINT8 bVideo);
static void YuvAdjust(SINT8* pbuf, UINT32 len);
static void ReturnViewFinder(void);
static void FuncFinBeep(void);
static void JpegMenuKeyProc(TSmpKeyProc* pKeyProc, UINT8 num, UINT8 key);


#ifndef WIN32
#define SmPrintf(X)
#endif

#define TDEMO_TOTAL_SIZE 0x200000


static TSmJpegSet gSmJpegSet = 
{
	{
		9,//MulNum
		3,//rownum
		3,//colnum
		V5_FONT_SRC_FORE_COLOR1,//frmcolor
		V5_FONT_SRC_FORE_COLOR2,//selcolor
		2,//frmthick_sel
	},

#ifdef NCSC
     "./still.jpg",//StiFileName
     "./../../dat/disp_still.jpg",//StiDispFileName
     "./video.jpg",//VidFileName
     "./../../dat/disp_video.jpg",//VidDispFileName
     "./mul.jpg",//MulFileName
     "./../../dat/enc_still.jpg",//EncFileName
     "./enc.jpg",//EncResultFileName
     "./../../dat/dec_still.jpg",//DecFileName
     "./dec.bmp"//DecResultFileName
#else
	"/flash/still.jpg",//StiFileName
	"/flash/still.jpg",//StiDispFileName
	"/flash/video.jpg",//VidFileName
	"/flash/video.jpg",//VidDispFileName
	"/flash/mul.jpg",//MulFileName
	"/flash/still.jpg",//EncFileName
	"/flash/enc.jpg",//EncResultFileName
	"/flash/still.jpg",//DecFileName
	"/flash/dec.bmp"//DecResultFileName
#endif
};

static TSmJpegContext gSmJpegContext;

void SmJpegOpen(void)
{
	gSmJpegContext.set = &gSmJpegSet;
}

PTSmJpegSet SmJpegGetSetPt(void)
{
	return gSmJpegContext.set;
}

//Menu

void SmpJpeg(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_DISP,					NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING,				NULL},
		{ MENU_KEY_LEFT,	MENU_JPEG,					NULL},
		{ MENU_KEY_RIGHT,	MENU_JPEG_CAP_STI,			NULL},
		{ MENU_KEY_SELECT,	MENU_JPEG_CAP_STI,			NULL}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegCapSti(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_EXIT,			smJpegRetView},
		{ MENU_KEY_DOWN,	MENU_JPEG_DIS_STI,		smJpegRetView},
		{ MENU_KEY_LEFT,	MENU_JPEG,				smJpegRetView},
		{ MENU_KEY_RIGHT,	MENU_JPEG_CAP_STI,		smJpegCapStill},
		{ MENU_KEY_SELECT,	MENU_JPEG_CAP_STI,		smJpegCapStill}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegDisSti(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_CAP_STI,		smJpegRetView},
		{ MENU_KEY_DOWN,	MENU_JPEG_CAP_FRM,		smJpegRetView},
		{ MENU_KEY_LEFT,	MENU_JPEG,				smJpegRetView},
		{ MENU_KEY_RIGHT,	MENU_JPEG_DIS_STI,		smJpegDispStill},
		{ MENU_KEY_SELECT,	MENU_JPEG_DIS_STI,		smJpegDispStill}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegCapFrm(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_DIS_STI,		smJpegRetView},
		{ MENU_KEY_DOWN,	MENU_JPEG_CAP_VID,		smJpegRetView},
		{ MENU_KEY_LEFT,	MENU_JPEG,				smJpegRetView},
		{ MENU_KEY_RIGHT,	MENU_JPEG_CAP_FRM,		smJpegCapFrame},
		{ MENU_KEY_SELECT,	MENU_JPEG_CAP_FRM,		smJpegCapFrame}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegCapVid(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_CAP_FRM,		smJpegRetView},
		{ MENU_KEY_DOWN,	MENU_JPEG_DIS_VID,		smJpegRetView},
		{ MENU_KEY_LEFT,	MENU_JPEG,				smJpegRetView},
		{ MENU_KEY_RIGHT,	MENU_JPEG_CAP_VID,		smJpegCapVideo},
		{ MENU_KEY_SELECT,	MENU_JPEG_CAP_VID,		smJpegCapVideo}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegDisVid(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_CAP_VID,		smJpegRetView},
		{ MENU_KEY_DOWN,	MENU_JPEG_MULTISHOT,	smJpegRetView},
		{ MENU_KEY_LEFT,	MENU_JPEG,				smJpegRetView},
		{ MENU_KEY_RIGHT,	MENU_JPEG_DIS_VID,		smJpegDispVideo},
		{ MENU_KEY_SELECT,	MENU_JPEG_DIS_VID,		smJpegDispVideo}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegMulShot(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_DIS_VID,		smJpegRetView},
		{ MENU_KEY_DOWN,	MENU_JPEG_ENC,			smJpegRetView},
		{ MENU_KEY_LEFT,	MENU_JPEG,				smJpegRetView},
		{ MENU_KEY_RIGHT,	MENU_JPEG_MUL_SEL,		smJpegMultishot},
		{ MENU_KEY_SELECT,	MENU_JPEG_MUL_SEL,		smJpegMultishot}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegEnc(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_MULTISHOT,	smJpegRetView},
		{ MENU_KEY_DOWN,	MENU_JPEG_DEC,			smJpegRetView},
		{ MENU_KEY_LEFT,	MENU_JPEG,				smJpegRetView},
		{ MENU_KEY_RIGHT,	MENU_JPEG_ENC,			smJpegEncode},
		{ MENU_KEY_SELECT,	MENU_JPEG_ENC,			smJpegEncode}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegDec(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_ENC,			smJpegRetView},
		{ MENU_KEY_DOWN,	MENU_JPEG_EXIT,			smJpegRetView},
		{ MENU_KEY_LEFT,	MENU_JPEG,				smJpegRetView},
		{ MENU_KEY_RIGHT,	MENU_JPEG_DEC,			smJpegDecode},
		{ MENU_KEY_SELECT,	MENU_JPEG_DEC,			smJpegDecode}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegExit(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_DEC,			NULL},
		{ MENU_KEY_DOWN,	MENU_JPEG_CAP_STI,		NULL},
		{ MENU_KEY_LEFT,	MENU_JPEG,				NULL},
		{ MENU_KEY_RIGHT,	MENU_EXIT,				NULL},
		{ MENU_KEY_SELECT,	MENU_EXIT,				NULL}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegMulSel(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_MUL_SEL,		smJpegMulSelUp},
		{ MENU_KEY_DOWN,	MENU_JPEG_MUL_SEL,		smJpegMulSelDw},
		{ MENU_KEY_LEFT,	MENU_JPEG_MUL_SEL,		smJpegMulSelLf},
		{ MENU_KEY_RIGHT,	MENU_JPEG_MUL_SEL,		smJpegMulSelRg},
		{ MENU_KEY_SELECT,	MENU_JPEG_MUL_DISP,		smJpegMulSelDisp}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

void SmJpegMulDisp(UINT8 key, UINT8* menustring)
{
const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG_MUL_SEL,		smJpegRetMulSel},
		{ MENU_KEY_DOWN,	MENU_JPEG_MUL_SEL,		smJpegRetMulSel},
		{ MENU_KEY_LEFT,	MENU_JPEG_MUL_SEL,		smJpegRetMulSel},
		{ MENU_KEY_RIGHT,	MENU_JPEG_MUL_SEL,		smJpegRetMulSel},
		{ MENU_KEY_SELECT,	MENU_JPEG,				smJpegClearMenuRetView}
	};

	JpegMenuKeyProc((TSmpKeyProc*)keyProc, sizeof(keyProc)/sizeof(TSmpKeyProc), key);

	menustring = menustring;
}

//Functions

//#define _TEST_BRC
//#define	_TEST_CAP_NOT_READ_DAT

#ifndef _TEST_CAP_NOT_READ_DAT

void smJpegCapStill(void)
{
	TDataBuf dbuf;
	TJpegCapParm capparm;
#ifdef _TEST_BRC
	UINT8 brc;
	static brcval = 0x71;
#endif

	SmpDisappearMenu();


	dbuf.pData = (UINT8*)gTransferBuf;
	dbuf.Pos = 0;
	dbuf.Len = TDEMO_TOTAL_SIZE;


	//SmpCameraSetSXGA(MENU_KEY_SELECT, NULL);

	V5_SetMode(CAPTURESTILL_MODE);

	SmpSetCaptureStill();

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

#ifdef _TEST_BRC
	brc = V5B_JpegGetBrc();
	V5B_JpegSetBrc(brcval);
#endif

	TestCap(&dbuf, &capparm);

#ifdef _TEST_BRC
	V5B_JpegSetBrc(brc);

	if(brcval == 0x71)
	{
		brcval = 0x21;
	}
	else
	{
		brcval = 0x71;
	}
#endif

	if(dbuf.Pos == 0)
		return;

	SaveImageFile(gSmJpegContext.set->StiFileName, (char*)dbuf.pData, (int)dbuf.Pos);

	FuncFinBeep();

}

#else
/*
#define LCD_W 128
#define LCD_H 160
static void DispAuto(TSize imgsize, UINT32 jbufstart)
{
	TALayerProty proty;
	TSize dispsize;
	UINT32 jbufsize;
	TPoint ipppt;

	dispsize.cx = (imgsize.cx > LCD_W) ? LCD_W : imgsize.cx;
	dispsize.cy = (imgsize.cy > LCD_H) ? LCD_H : imgsize.cy;
	ipppt.x = 0;
	ipppt.y = 0;

	proty.memsize = dispsize;
	proty.pos2panel = ipppt;
	proty.win.ltp = ipppt;
	proty.win.size = dispsize;

	V5_SetMode(DISPLAYSTILL_MODE);

	V5_IppSetDispSizerEx2(imgsize, ipppt, imgsize, dispsize);
	V5_DispSetAmemSize(&proty);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	jbufsize = V5B_MarbGetJbufSize()-8;
	V5_HJpegDisplayFrameAuto(jbufstart, jbufsize);

	FuncFinBeep();
}

void smJpegCapStill(void)
{
	TSize size;
	TPoint pt;
	UINT32 jbufstart;

	size.cx = 48;
	size.cy = 48;

	SmpDisappearMenu();
	//SmpCameraSetSXGA(MENU_KEY_SELECT, NULL);

	V5_SetMode(CAPTURESTILL_MODE);

	SmpSetCaptureStill();
	V5_IppSetCapSizer(size);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	V5_HJpegCapture(NULL, NULL, NULL);

	jbufstart = V5B_MarbGetJbufStart();

	DispAuto(size, jbufstart);

	FuncFinBeep();


}
*/

#define LCD_W 128
#define LCD_H 160

static void DecIppAuto(TSize imgsize, UINT32 jbufstart, PTDataBuf pdst)
{
	TALayerProty proty;
	TSize dispsize;
	UINT32 jbufsize;
	TPoint ipppt;

	//memset(&proty, 0, sizeof(TALayerProty));
	dispsize.cx = (imgsize.cx > LCD_W) ? LCD_W : imgsize.cx;
	dispsize.cy = (imgsize.cy > LCD_H) ? LCD_H : imgsize.cy;
	ipppt.x = 0;
	ipppt.y = 0;

	proty.memsize = dispsize;
	proty.pos2panel = ipppt;
	proty.win.ltp = ipppt;
	proty.win.size = dispsize;

	V5_SetMode(DECODEIPP_MODE);

	V5_IppSetDispSizerEx2(imgsize, ipppt, imgsize, imgsize);
	V5_DispSetAmemSize(&proty);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	jbufsize = V5B_MarbGetJbufSize()-8;

	V5_HJpegDecodeIppAuto(jbufstart, jbufsize, pdst);

	FuncFinBeep();
}

static void DispYuvBypass(TSize imgsize, PTDataBuf pdst)
{
	TALayerProty proty;
	TSize dispsize;
	UINT32 jbufsize;
	TPoint ipppt;

	dispsize.cx = (imgsize.cx > LCD_W) ? LCD_W : imgsize.cx;
	dispsize.cy = (imgsize.cy > LCD_H) ? LCD_H : imgsize.cy;
	ipppt.x = 0;
	ipppt.y = 0;

	proty.memsize = dispsize;
	proty.pos2panel = ipppt;
	proty.win.ltp = ipppt;
	proty.win.size = dispsize;

	V5_SetMode(DISPLAYBYPASS_MODE);

	V5_IppSetDispSizerEx2(imgsize, ipppt, imgsize, dispsize);
	V5_DispSetAmemSize(&proty);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	V5_HJpegYuvDisplay(pdst, imgsize);

	FuncFinBeep();
}

void smJpegCapStill(void)
{
	TDataBuf dbuf;
	TSize size;
	TPoint pt;
	UINT32 jbufstart;

	size.cx = 48;
	size.cy = 48;

	SmpDisappearMenu();
	//SmpCameraSetSXGA(MENU_KEY_SELECT, NULL);

	V5_SetMode(CAPTURESTILL_MODE);

	SmpSetCaptureStill();
	V5_IppSetCapSizer(size);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	V5_HJpegCapture(NULL, NULL, NULL);

	jbufstart = V5B_MarbGetJbufStart();

	dbuf.pData = (UINT8*)gTransferBuf;
	dbuf.Pos = 0;
	dbuf.Len = TDEMO_TOTAL_SIZE;

	DecIppAuto(size, jbufstart, &dbuf);

	if(dbuf.Pos)
	{
		dbuf.Len = dbuf.Pos;
		dbuf.Pos = 0;
		DispYuvBypass(size, &dbuf);
	}

	FuncFinBeep();


}

#endif

void smJpegCapVideo(void)
{
	TDataBuf dbuf;
	TJpegCapParm capparm;

	SmpDisappearMenu();

	dbuf.pData = (UINT8*)gTransferBuf;
	dbuf.Pos = 0;
	dbuf.Len = TDEMO_TOTAL_SIZE;

	V5_SetMode(CAPTUREVIDEO_MODE);

	SmpSetCaptureVideo();

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	TestCap(&dbuf, &capparm);

	if(dbuf.Pos == 0)
		return;

	SaveImageFile(gSmJpegContext.set->VidFileName, (char*)dbuf.pData, (int)dbuf.Pos);

	FuncFinBeep();

}

void smJpegCapFrame(void)
{
	TDataBuf dbuf;
	TJpegCapParm capparm;

	SmpDisappearMenu();

	dbuf.pData = (UINT8*)gTransferBuf;
	dbuf.Pos = 0;
	dbuf.Len = TDEMO_TOTAL_SIZE;

	V5_SetMode(CAPTUREFRAME_MODE);

	SmpSetCaptureFrame();

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	TestCap(&dbuf, &capparm);

	if(dbuf.Pos == 0)
		return;

	SaveImageFile(gSmJpegContext.set->StiFileName, (char*)dbuf.pData, (int)dbuf.Pos);

	FuncFinBeep();

}


void smJpegDispStill(void)
{
	UINT32 len;

//	SmpDisappearMenu();

	len = (UINT32)LoadImageFile(gSmJpegContext.set->StiDispFileName, gTransferBuf, TDEMO_TOTAL_SIZE);

	if(len == 0)
		return;

	DispStillVideo((UINT8*)gTransferBuf, len, 0);

}

void smJpegDispVideo(void)
{
	UINT32 len;

//	SmpDisappearMenu();

	len = LoadImageFile(gSmJpegContext.set->VidDispFileName, gTransferBuf, TDEMO_TOTAL_SIZE);

	if(len == 0)
		return;

	DispStillVideo((UINT8*)gTransferBuf, len, 1);

}


void smJpegMultishot(void)
{
	TDataBuf dbuf;
	TJpegCapParm capparm;

	dbuf.pData = (UINT8*)gTransferBuf;
	dbuf.Pos = 0;
	dbuf.Len = TDEMO_TOTAL_SIZE;

	V5_SetMode(MULTISHOTTHUMB_MODE);
	V5B_MarbSetMshotFcnt(gSmJpegContext.set->MulSet.MulNum-1);


	SmpSetCaptureStill();

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	TestCap(&dbuf, &capparm);

	if(dbuf.Pos == 0)
		return;

	V5B_IppGetThumbSize(&gSmJpegContext.thumActSize);		
	gSmJpegContext.curThumbSel = 0;

	SaveImageFile(gSmJpegContext.set->MulFileName, (char*)dbuf.pData+capparm.ilen, (int)(capparm.tlen+capparm.jlen));

	DisplayMulThumb((dbuf.pData+capparm.ilen), (capparm.tlen+capparm.jlen));

	FuncFinBeep();

}

void smJpegEncode(void)
{
	PTSmpSize pSmpSize;
	TJpegIndex info;
	TDataBuf src;
	TDataBuf dst;

	UINT32 len;

	len = LoadImageFile(gSmJpegContext.set->EncFileName, gTransferBuf, TDEMO_TOTAL_SIZE);
	if(len == 0)
		return;

	JpegParse((UINT8*)gTransferBuf, len, &info);

	if(info.YUVType == JPEG_UNKNOWN)
		return;

	src.pData = (UINT8*)gTransferBuf;
	src.Len = len;
	src.Pos = 0;

	dst.pData = (UINT8*)gTransferBuf;
	dst.Pos = 0;
	dst.Len = TDEMO_TOTAL_SIZE;

	pSmpSize = SmpCameraGetCurSize();

	V5_SetMode(DECODEIPP_MODE);

	V5_IppSetDispSizerEx2(info.JpgSize, pSmpSize->pt, info.ImageSize, pSmpSize->dispSize);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	V5_HJpegDecIpp(&src, &dst);

	if(dst.Pos == 0)
		return;

	//switch(info.YUVType)
	//{
	//case V5B_LBUF_YUV_422:

		//add 128
		YuvAdjust((char*)dst.pData, dst.Pos);

		len = Yuv422_Blkorder(gTransferBuf, gTransferBuf+dst.Pos, YUV422_UYVY, pSmpSize->dispSize.cx, pSmpSize->dispSize.cy);
		dst.pData = (UINT8*)gTransferBuf+dst.Pos;
		dst.Len = len;
		dst.Pos = 0;
	//	break;
	//case V5B_LBUF_YUV_420:

	//	//add 128
	//	YuvAdjust((char*)dst.pData, dst.Pos);

	//	len = Yuv420_Blkorder(gTransferBuf, gTransferBuf+dst.Pos, YUV420_UYVY, pSmpSize->dispSize.cx, pSmpSize->dispSize.cy);
	//	memcpy(gTransferBuf, gTransferBuf+dst.Pos, (unsigned int)len);
	//	dst.Len = len;
	//	dst.Pos = 0;
	//	break;
	//case V5B_LBUF_YUV_411:

	//	//add 128
	//	YuvAdjust((char*)dst.pData, dst.Pos);

	//	len = Yuv411_Blkorder(gTransferBuf, gTransferBuf+dst.Pos, YUV411_YYYYUV, pSmpSize->dispSize.cx, pSmpSize->dispSize.cy);
	//	memcpy(gTransferBuf, gTransferBuf+dst.Pos, (unsigned int)len);
	//	dst.Len = len;
	//	dst.Pos = 0;
	//	break;
	//case V5B_LBUF_YUV_400:

	//	//add 128
	//	YuvAdjust((char*)dst.pData, dst.Pos);

	//	len = Lbuf2Yuv(gTransferBuf, gTransferBuf+dst.Pos, pSmpSize->dispSize.cx, pSmpSize->dispSize.cy);
	//	memcpy(gTransferBuf, gTransferBuf+dst.Pos, (unsigned int)len);
	//	dst.Len = len;
	//	dst.Pos = 0;
	//	break;
	//default :
	//	return;
	//}

	src.pData = (UINT8*)gTransferBuf;
	src.Pos = 0;
	src.Len = TDEMO_TOTAL_SIZE;

	V5_SetMode(ENCODE_MODE);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

//	V5_HJpegEnc(&dst, &src, info.YUVType, pSmpSize->dispSize);
	V5_HJpegEnc(&dst, &src, V5B_LBUF_YUV_422, pSmpSize->dispSize);

	if(src.Pos == 0)
		return;

	SaveImageFile(gSmJpegContext.set->EncResultFileName, (char*)src.pData, (int)src.Pos);

	DispStillVideo(src.pData, src.Pos, 0);

	FuncFinBeep();

}

void smJpegDecode(void)
{
	PTSmpSize pSmpSize;
	TDataBuf src;
	TDataBuf dst;

	TALayerProty proty;

	TJpegIndex info;
	UINT32 len;

	len = LoadImageFile(gSmJpegContext.set->DecFileName, gTransferBuf, TDEMO_TOTAL_SIZE);
	if(len == 0)
		return;

	JpegParse((UINT8*)gTransferBuf, len, &info);

	if(info.YUVType == JPEG_UNKNOWN)
		return;


	pSmpSize = SmpCameraGetCurSize();

	proty.memsize = pSmpSize->dispSize;
	proty.pos2panel = pSmpSize->pt;
	proty.win.ltp = pSmpSize->pt;
	proty.win.size = pSmpSize->dispSize;

	V5_SetMode(DECODEFRAME_MODE);

	V5_IppSetDispSizerEx2(info.JpgSize, pSmpSize->pt, info.ImageSize, pSmpSize->dispSize);
	V5_DispSetAmemSize(&proty);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	src.pData = (UINT8*)gTransferBuf;
	src.Len = len;
	src.Pos = 0;

	dst.pData = (UINT8*)gTransferBuf;
	dst.Pos = 0;
	dst.Len = TDEMO_TOTAL_SIZE;

	V5_HJpegDecFrm(&src, &dst);

	if(dst.Pos == 0)
		return;

	SaveImageFile(gSmJpegContext.set->DecResultFileName, (char*)dst.pData, (int)dst.Pos);

	FuncFinBeep();

}

void smJpegMulSelUp(void)
{
	PTSmJpegMulSet pMulSet;

	pMulSet = &gSmJpegContext.set->MulSet;

	DrawMulFrm(gSmJpegContext.curThumbSel, pMulSet->frmcolor, pMulSet->frmthick_sel);

	gSmJpegContext.curThumbSel = CalMulSelMove(gSmJpegContext.curThumbSel, 0);

	DrawMulFrm(gSmJpegContext.curThumbSel, pMulSet->selcolor, pMulSet->frmthick_sel);
}

void smJpegMulSelDw(void)
{
	PTSmJpegMulSet pMulSet;

	pMulSet = &gSmJpegContext.set->MulSet;

	DrawMulFrm(gSmJpegContext.curThumbSel, pMulSet->frmcolor, pMulSet->frmthick_sel);

	gSmJpegContext.curThumbSel = CalMulSelMove(gSmJpegContext.curThumbSel, 1);

	DrawMulFrm(gSmJpegContext.curThumbSel, pMulSet->selcolor, pMulSet->frmthick_sel);
}

void smJpegMulSelLf(void)
{
	PTSmJpegMulSet pMulSet;

	pMulSet = &gSmJpegContext.set->MulSet;

	DrawMulFrm(gSmJpegContext.curThumbSel, pMulSet->frmcolor, pMulSet->frmthick_sel);

	gSmJpegContext.curThumbSel = CalMulSelMove(gSmJpegContext.curThumbSel, 2);

	DrawMulFrm(gSmJpegContext.curThumbSel, pMulSet->selcolor, pMulSet->frmthick_sel);
}

void smJpegMulSelRg(void)
{
	PTSmJpegMulSet pMulSet;

	pMulSet = &gSmJpegContext.set->MulSet;

	DrawMulFrm(gSmJpegContext.curThumbSel, pMulSet->frmcolor, pMulSet->frmthick_sel);

	gSmJpegContext.curThumbSel = CalMulSelMove(gSmJpegContext.curThumbSel, 3);

	DrawMulFrm(gSmJpegContext.curThumbSel, pMulSet->selcolor, pMulSet->frmthick_sel);
}

//V5_PreDispFont(GE_B0);
//V5_DispUpdateB0Win(PTWinPos pwin)
void smJpegMulSelDisp(void)
{
	TJpegIndex info;
	UINT8* pbuf;
	UINT32 len;
	UINT8 i=0;

	len = (UINT32)LoadImageFile(gSmJpegContext.set->MulFileName, gTransferBuf, TDEMO_TOTAL_SIZE);

	if(len == 0)
		return;

	V5B_LcdcLayerAFirst();

	pbuf = (UINT8*)gTransferBuf;

	while(len)
	{
		JpegParse(pbuf, len, &info);

		if(info.YUVType != JPEG_UNKNOWN)
		{
			if(i == gSmJpegContext.curThumbSel)
			{
				DispStillVideo(pbuf, (UINT32)info.eop, 0);

				break;
			}
			i++;
		}
		pbuf += info.eop;
		len -= info.eop;
	}

	FuncFinBeep();
}

void smJpegRetView(void)
{
	ReturnViewFinder();
}

void smJpegClearMenuRetView(void)
{
	ReturnViewFinder();
	SmpDispLoadAndDispFrame(SMP_FRAME_NULL);

	V5B_LcdcLayerB0First();
}

void smJpegRetMulSel(void)
{
	ReturnViewFinder();
	V5B_LcdcLayerB0First();

	SmpDispRefreshB0();
}



//Static functions

UINT8 CalMulFrmRect(UINT8 cursel, TPoint *pt, TSize *size)
{
	PTSmJpegMulSet pMulSet;
	UINT8 row, col;
	PTSmpSize pSmpSize;

	pSmpSize = SmpCameraGetCurSize();

	pMulSet = &gSmJpegContext.set->MulSet;

	row = cursel/pMulSet->colnum;
	if((row+1) > pMulSet->rownum)
		return 0;
	col = cursel%pMulSet->colnum;

	size->cx = pSmpSize->dispSize.cx/pMulSet->rownum;
	size->cy = pSmpSize->dispSize.cy/pMulSet->colnum;

	if(size->cx == 0 || size->cy == 0)
		return 0;

	pt->x = size->cx * col;
	pt->y = size->cy * row;

	return 1;
}

void DrawMulFrm(UINT8 cursel, UINT32 color, UINT8 thick_sel)
{
	TPoint pt;
	TSize size;

	if(CalMulFrmRect(cursel, &pt, &size))
	{
        SmpDispThumbFrame(pt, size, color, thick_sel);
		SmpDispRefreshB0();
	}
}

void DrawMulThum(UINT8 cursel, TSize thumsize, UINT8* pData)
{
	TPoint pt;
	TSize size;

	if(CalMulFrmRect(cursel, &pt, &size))
	{
		V5_DispB1Image(pt, thumsize, pData);
		SmpDispRefreshB0();
	}
}

void DisplayMulThumb(UINT8* pData, UINT32 len)
{
	PTSmJpegMulSet pMulSet;
	SINT8* psrc, *pdst;
	UINT32 tranlen;
	UINT8 i;

	pMulSet = &gSmJpegContext.set->MulSet;


	for(i=0; i<pMulSet->MulNum; i++)
	{

		//YUV422_UYVY --> RGB24
		psrc = (SINT8*)pData + gSmJpegContext.thumActSize.cx*gSmJpegContext.thumActSize.cy*2*i;
		pdst = (SINT8*)pData + len;
		tranlen = MYuv422_Rgborder(psrc, pdst, gSmJpegContext.thumActSize.cx, gSmJpegContext.thumActSize.cy, YUV422_UYVY);

#ifdef	ADAPT_OLD_ASIC

		DrawMulThum(i, gSmJpegContext.thumActSize, (UINT8*)pdst);

#else

		//RGB24 --> RGB555
		psrc = pdst+tranlen;
		tranlen = Rgb24_Rgb555(pdst, psrc, gSmJpegContext.thumActSize.cx, gSmJpegContext.thumActSize.cy, 0);
		DrawMulThum(i, gSmJpegContext.thumActSize, (UINT8*)psrc);

#endif
	}
	for(i=0; i<pMulSet->MulNum; i++)
	{
		DrawMulFrm(i, pMulSet->frmcolor, pMulSet->frmthick_sel);
	}
	DrawMulFrm(gSmJpegContext.curThumbSel, pMulSet->selcolor, pMulSet->frmthick_sel);

}

//direct 0-up, 1-down, 2-left, 3-right
UINT8 CalMulSelMove(UINT8 cursel, UINT8 direct)
{
	PTSmJpegMulSet pMulSet;
	UINT8 row, col;
	UINT8 sel;

	pMulSet = &gSmJpegContext.set->MulSet;

	row = cursel/pMulSet->colnum;
	col = cursel%pMulSet->colnum;

	switch(direct)
	{
	case 0:
		if(cursel == 0)
		{
			sel = pMulSet->MulNum-1;
		}
		else
		{
			if(row == 0)
			{
				row = pMulSet->rownum-1;
				col--;
			}
			else
			{
				row--;
			}
			sel = pMulSet->colnum*row + col;
		}
		
		break;

	case 1:
		if(cursel == (pMulSet->MulNum-1))
		{
			sel = 0;
		}
		else
		{
			if(row == (pMulSet->rownum-1))
			{
				row = 0;
				col++;
			}
			else
			{
				row++;
			}
			sel = pMulSet->colnum*row + col;
		}
		
		break;

	case 2:
		if(cursel == 0)
		{
			sel = pMulSet->MulNum-1;
		}
		else
		{
			if(col == 0)
			{
				col = pMulSet->colnum-1;
				row--;
			}
			else
			{
				col--;
			}
			sel = pMulSet->colnum*row + col;
		}
		
		break;

	case 3:
	default:
		if(cursel == (pMulSet->MulNum-1))
		{
			sel = 0;
		}
		else
		{
			if(col == (pMulSet->colnum-1))
			{
				col = 0;
				row++;
			}
			else
			{
				col++;
			}
			sel = pMulSet->colnum*row + col;
		}
		
		break;

	}

	return sel;
}

void DispStillVideo(UINT8* pbuf, UINT32 len, UINT8 bVideo)
{
	TALayerProty proty;
	TJpegIndex info;
	TDataBuf dbuf;
	PTSmpSize pSmpSize;

	if(len == 0)
		return;

	pSmpSize = SmpCameraGetCurSize();

	proty.memsize = pSmpSize->dispSize;
	proty.pos2panel = pSmpSize->pt;
	proty.win.ltp = pSmpSize->pt;
	proty.win.size = pSmpSize->dispSize;

	JpegParse(pbuf, len, &info);

	//Disapear menu.
	//SmpDispLoadFrame(GE_B0, SMP_FRAME_NULL);
	//V5_DispUpdateB0Win(NULL);

	SmpDisappearMenu();

	FuncFinBeep();

	V5_SetMode(DISPLAYSTILL_MODE);

	V5_IppSetDispSizerEx2(info.JpgSize, pSmpSize->pt, info.ImageSize, pSmpSize->dispSize);
	V5_DispSetAmemSize(&proty);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	dbuf.Len = len;
	dbuf.Pos = 0;
	dbuf.pData = pbuf;

	if(!bVideo)
	{
		V5_HJpegDispFrm(&dbuf);
	}
	else
	{
		V5_HJpegDispVidNorm(&dbuf);
	}

	FuncFinBeep();
}

void YuvAdjust(SINT8* pbuf, UINT32 len)
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

void ReturnViewFinder()
{
	TALayerProty proty;
	TSize dispsize;
	TPoint pt;

	dispsize.cx = 128;
	dispsize.cy = 160;
	pt.x = 0;
	pt.y = 0;

	proty.memsize = dispsize;
	proty.pos2panel = pt;
	proty.win.ltp = pt;
	proty.win.size = dispsize;

	V5_IppSetDispSizerEx2(dispsize, pt, dispsize, dispsize);
	V5_DispSetAmemSize(&proty);

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	SmpCameraSetCurSize();

	V5_SetMode((UINT8)(PREVIEWBRC_MODE));

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
}

void FuncFinBeep(void)
{
	beep(1);
	Delay(100);
	beep(0);
}

void JpegMenuKeyProc(TSmpKeyProc* pKeyProc, UINT8 num, UINT8 key)
{
	UINT8 i;

	for(i = 0; i < num; i++)
	{
		if(key == pKeyProc[i].key)
		{
			SmpSetIndex(pKeyProc[i].index);
			if(pKeyProc[i].pcall)
				pKeyProc[i].pcall();
		}
	}
}

#endif //V578CMD_DISABLE
