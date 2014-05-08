
#include "inheaders.h" 
#ifndef	V578CMD_DISABLE

#define MOD_CLK_SIFISP (MOD_CLK_SIF | MOD_CLK_ISP)

static TCmdDef gHapiCmdList[]=
{
	{ _V568_Open_Default},
	{ _V568_Open_Default},
	{ _V568_Open_WithContext},
	{ _V5M_GetHapiStdInfo},
	{ _V5M_SetBrightness},
	{ _V5M_SetContrast},
	{ _V5M_CameraZoom},
	{ _V5M_BrightnessUp},
	{ _V5M_BrightnessDown},
	{ _V5M_ContrastUp},
	{ _V5M_ContrastDown},
	{ _V5M_CameraZoomIn},
	{ _V5M_CameraZoomOut},
	{ _V5M_BlendratioUp},
	{ _V5M_BlendratioDown},
	{ _V5M_SetBlendratio},
	{ _V5M_AF_ON},
	{ _V5M_AF_OFF},
	{ _V5M_CapStill_start},
	{ _V5M_CapMultishot_start},
	{ _V5M_CapVideo_start},
	{ _V5M_getCaptureInfoToFile},
	{ _V5M_getcfgSnrInfo},
	{ _V5M_getcfgSnrSizeByIndex},
	{ _V5M_FrameON},
	{ _V5M_FrameOFF},
	{ _V5M_ThumbON},
	{ _V5M_ThumbOFF},
	{ _V5M_SetFrameInterval},
	{ _V5M_SetExposeMode},
	{ _V5M_SetSnrResoIndex},
	{ _V5M_SetVideoFileSize},
	{ _V5M_SetSpecialEffect},
	{ _V5M_SetRotateMode},
	{ _V5M_SpecialEffectOFF},
	{ _V5M_PreviewAuto},
	{ _V5M_SetCompRatio},
	{ _V5M_SetCompedJFileSize},
	{ _V5M_CapAsWallPicON},
	{ _V5M_CapAsWallPicOFF},
	{ _V5M_DisplayStill},
	{ _V5M_DisplayVideo},
	{ _V5M_DecodeFrameEncode},
	{ _V5M_DecodeIPPEncode}
};

static const TSize sizerinfo[]=
{
	{	1280,	960	},
	{	1024,	768	},
	{	800,	600	},
	{	640,	480	},
	{	352,	288	},
	{	320,	240	},
	{	176,	144	},
	{	160,	120	}
};

THapiContext gHapiContext;
UINT8 gpcdemoen;

UINT32 V5_SendCommand(UINT32 msg, void *param, UINT32 paramsize)
{
	return gHapiCmdList[msg].pcall(param, paramsize);
}

//----------------------------------------------------------------
//-----------------------------------------------------------
UINT32 V5M_CameraGetSizeByIndex(UINT8 index, TSize *size)
{
	if (index > V5M_QSIF)
		index = V5M_QSIF;
	*size = sizerinfo[index];
	return 0;
}

UINT32 _V568_Open_Default(void *param, UINT32 paramsize)
{
	V5M_Open();
	V5M_CameraOnWithDefault();

	param = param;
	paramsize = paramsize;
	return 0;
}

UINT32 _V568_Open_WithContext(void *param, UINT32 paramsize)
{
	V5M_Open();
	V5M_CameraOn();

	param = param;
	paramsize = paramsize;
	return 0;
}

UINT32 _V5M_GetHapiStdInfo(void *param, UINT32 paramsize)
{
	paramsize = paramsize;
	*((PTHapiStdInfo)param) = gHapiContext.hapiStdInfo;
	return 0;
}

UINT32 _V5M_SetBrightness(void *param, UINT32 paramsize)
{
	UINT8 value = *((UINT8 *) param);

	param = param;
	paramsize = paramsize;
	return V5M_CameraSetBrightness(value);
}

UINT32 _V5M_BrightnessUp(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	return V5M_CameraBrightnessUp();
}

UINT32 _V5M_BrightnessDown(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	return V5M_CameraBrightnessDown();
}


UINT32 _V5M_SetContrast(void *param, UINT32 paramsize)
{
	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;
	return V5M_CameraSetContrast(value);
}

UINT32 _V5M_ContrastUp(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	return V5M_CameraContrastUp();
}

UINT32 _V5M_ContrastDown(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	return V5M_CameraContrastDown();
}


UINT32 _V5M_SetExposeMode(void *param, UINT32 paramsize)
{
	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;
	return V5M_SetExposeMode(value);
}

UINT32 _V5M_CameraZoomIn(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	return V5M_CameraZoomIn();
}

UINT32 _V5M_CameraZoomOut(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	return V5M_CameraZoomOut();
}


UINT32 _V5M_CameraZoom(void *param, UINT32 paramsize)
{
	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;
	return V5M_CameraZoom(value);
}


UINT32 _V5M_BlendratioUp(void *param, UINT32 paramsize)
{
	UINT16 value;

	param = param;
	paramsize = paramsize;
	value = gHapiContext.hapiStdInfo.blendratio + gHapiContext.hapiStdInfo.blendratiostep;
	if (value < 255)
	{
		V5B_LcdcSetB0BlendRatio((UINT8) value);
		gHapiContext.hapiStdInfo.blendratio = (UINT8) value;
	}
	return 0;
}

UINT32 _V5M_BlendratioDown(void *param, UINT32 paramsize)
{
	UINT16 value;

	param = param;
	paramsize = paramsize;
	value = gHapiContext.hapiStdInfo.blendratio - gHapiContext.hapiStdInfo.blendratiostep;
	if (value > 0)
	{
		V5B_LcdcSetB0BlendRatio((UINT8) value);
		gHapiContext.hapiStdInfo.blendratio = (UINT8) value;
	}
	return 0;
}

UINT32 _V5M_SetBlendratio(void *param, UINT32 paramsize)
{
	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;
	V5B_LcdcSetB0BlendRatio(value);
	gHapiContext.hapiStdInfo.blendratio = value;
	return 0;
}


UINT32 _V5M_AF_ON(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	V5_VispStartAutoFocus();
	return 0;
}

UINT32 _V5M_AF_OFF(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	V5_VispStopAutoFocus();
	return 0;
}

UINT32 _V5M_CapStill_start(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	return V5M_CapStill_start();
}

UINT32 _V5M_CapMultishot_start(void *param, UINT32 paramsize)
{
	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;
	return V5M_CapMultishot_start( value);
}

UINT32 _V5M_CapVideo_start(void *param, UINT32 paramsize) 
{
	param = param;
	paramsize = paramsize;
	return V5M_CapVideo_start();
}

UINT32 _V5M_getCaptureInfoToFile(void *param, UINT32 paramsize)
{
	TgetCaptureInfoToFileParam *pparam = (TgetCaptureInfoToFileParam *) (param);
	paramsize = paramsize;
	return V5M_getCaptureInfoToFile(pparam->filename, pparam->bufsizeOnetime );
}

UINT32 _V5M_getcfgSnrInfo(void *param, UINT32 paramsize)
{
	UINT8 *p ,i;
	
	p = (UINT8 *)param;
	paramsize = paramsize;
	for (i = 0; i<=gHapiStdInfo.sensorres[0] ;i++,p++)
		*p = gHapiStdInfo.sensorres[i];
	return 0;
}

UINT32 _V5M_getcfgSnrSizeByIndex(void *param, UINT32 paramsize)
{
	TgetSnrSizeByIndexParam *pparam = (TgetSnrSizeByIndexParam *)(param);
	paramsize = paramsize;
	V5_VispGetSize(pparam->index, &(pparam->size));
	return 0;
}

UINT32 _V5M_FrameON(void *param, UINT32 paramsize)
{
	paramsize = paramsize;
	V5M_FrameON((char *)param);
	return 0;
}


UINT32 _V5M_FrameOFF(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	V5M_FrameOFF();
	return 0;
}



UINT32 _V5M_ThumbON(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	V5M_ThumbON();
	return 0;
}

UINT32 _V5M_ThumbOFF(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	V5M_ThumbOFF();
	return 0;
}

UINT32 _V5M_SetFrameInterval(void *param, UINT32 paramsize)
{
	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;
	V5B_MarbSetFrameInterval(value);
	return 0;
}

UINT32 _V5M_SetSnrResoIndex(void *param, UINT32 paramsize)
{
	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;
	V5M_SetSnrReso(value);
	return 0;
}

UINT32 _V5M_SetVideoFileSize(void *param, UINT32 paramsize)
{
	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;
	V5M_SetVideoFileSize(value);
	return 0;
}

UINT32 _V5M_SetSpecialEffect(void *param, UINT32 paramsize)
{
	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;

	if (value < 7)
	{
		V5B_IppEnableSpecial();
		V5B_IppSetSpecialEffect(value);
	}
	return 0;
}

UINT32 _V5M_SpecialEffectOFF(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	V5B_IppDisableSpecial();
	return 0;
}

UINT32 _V5M_SetRotateMode(void *param, UINT32 paramsize) 
{
	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;
	V5B_LcdcSetRotateMode(value);
	return 0;
}



//UINT32 _V5M_SetCapSizer(void *param, UINT32 paramsize) 
//{
//	TSize size;
//	UINT8 value = *((UINT8 *) param);
//	paramsize = paramsize;
//
//	V5M_CameraGetSizeByIndex(value,&size);
//	//if (size.cx <   // i will correct it later.
//	V5_IppSetCapSizer(size);  //zhan
//	return 0;
//}

UINT32 _V5M_PreviewAuto(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	V5M_PreviewAuto();
	return 0;
}


UINT32 _V5M_SetCompRatio(void *param, UINT32 paramsize)
{

	UINT8 value = *((UINT8 *) param);
	paramsize = paramsize;
	V5M_SetCompRatio(value);
	return 0;
}

UINT32 _V5M_SetCompedJFileSize(void *param, UINT32 paramsize)
{
	UINT16 value = *((UINT16 *) param);
	paramsize = paramsize;
	V5M_SetCompedJFileSize(value);
	return 0;
}


UINT32 _V5M_CapAsWallPicON(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	gHapiContext.hapiStdInfo.featureflag |= V5M_BIT_CAPASWALLPIC;
	return 0;
}

UINT32 _V5M_CapAsWallPicOFF(void *param, UINT32 paramsize)
{
	param = param;
	paramsize = paramsize;
	gHapiContext.hapiStdInfo.featureflag &= ~V5M_BIT_CAPASWALLPIC;
	return 9;
}

UINT32 _V5M_DisplayStill(void *param, UINT32 paramsize)
{
	paramsize = paramsize;
	V5M_Display((char *)param, 0);
	return 0;
}

UINT32 _V5M_DisplayVideo(void *param, UINT32 paramsize)
{
	paramsize = paramsize;
	V5M_Display((char *)param, 1);
	return 0;
}


UINT32 _V5M_DecodeFrameEncode(void *param, UINT32 paramsize)
{
	TDecodeFrameEncodeParam *filen;
	filen = (TDecodeFrameEncodeParam *) param;
	paramsize = paramsize;
	if (V5M_FrameIsON())
		V5M_DecodeEncode(filen->srcfile , filen->dstfile, V5M_DECODEFRAME); 
	return 0;
}

UINT32 _V5M_DecodeIPPEncode(void *param, UINT32 paramsize)
{
	TDecodeIPPEncodeParam *pa;
	TSize Asize;

	pa = (TDecodeIPPEncodeParam *) param;
	paramsize = paramsize;
	//if ((pa->ratio >= V5M_MIN_DECODESIZER) && (pa->ratio < V5M_MAX_DECODESIZER))
		V5M_SetDecodeIPPSizer(pa->srcfile, pa->ratio);

	V5M_DecodeEncode(pa->srcfile , pa->dstfile, V5M_DECODEIPP);
	
	V5_DispGetPanelSize(&Asize);
	V5_IppSetDispSizer(Asize) ;

	return 0;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
void V5M_InitContext(void)
{
	V5_CtrlInitContext();
	V5_MarbInitContext();
	V5_VispInitContext();
	V5_DispInitContext();
	V5_JpegInitContext();

	V5_CtrlSetInfo((PTCtrlInfo)&gCtrlInfo);
	V5_DispSetPanelInfo((PTPanelInfo)&gPanelInfo);
	V5_MarbSetInfo((PTMarbInfo)&gMarbInfo);
	V5_JpegSetInfo((PTJpegInfo)&gJpegInfo);
	V5_VispSetSensorInfo();
	V5M_SetHapiStdInfo((PTHapiStdInfo) &gHapiStdInfo);

	gHapiContext.workmode = 0xff;
}

void V5M_SetHapiStdInfo(THapiStdInfo *pInfo)
{
	gHapiContext.hapiStdInfo = *pInfo;
}

void V5M_Open(void)
{
	V5B_CpmModGlbReset();
	V5M_InitContext();
	V5_CtrlOpen();
	V5_MarbOpen();
	V5_DispOpen();
	V5_VispOpen();
	V5_JpegOpen();	//order must be: ctrl, marb, disp, visp, jpeg
	V5M_DispSetPanelState(PANEL_POWERON, 1);  
	V5_VispSetSnrState(SENSOR_POWEROFF);
	V5M_HJpegOpen();

	V5M_SetFileOperation(gfileop);
	V5M_SetMalloc(gpmalloc);
	V5M_SetMfree(gpfree);

#ifdef	WIN32
	//PanelDbgInitContext(); //what does it mean?
#endif
}


UINT32 V5M_SetFileOperation(TFileOperations fileop)
{
	gHapiContext.fileop = fileop;
	return 0;
}

void V5M_SetMalloc(V5MMalloc *pmalloc)
{
	gHapiContext.pmalloc = pmalloc;
}

void V5M_SetMfree(V5MFree	*pfree)
{
	gHapiContext.pfree = pfree;
}




static const UINT16 gModeSetting[][5] =
{
	{ MOD_CLK_JPEG_CORE | MOD_CLK_JPEG | MOD_CLK_LBUF, 
						PREVIEW_MODE,				VISP_PREVIEW_MODE,			DISP_ABL_MODE, JPEG_MODE_IDLE },
	{ 0,				PREVIEWBRC_MODE,			VISP_PREVIEWBRC_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTURE },
	{ 0,				PREVIEWFRAME_MODE,			VISP_PREVIEWFRAME_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTUREFRAME },
	{ 0,				CAPTURESTILL_MODE,			VISP_CAPTURE_MODE,			DISP_ABL_MODE, JPEG_MODE_CAPTURE },
	{ 0,				CAPTURESTILLTHUMB_MODE,		VISP_CAPTURETHUMB_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTURE },
	{ 0,				CAPTUREVIDEO_MODE,			VISP_CAPTURE_MODE,			DISP_ABL_MODE, JPEG_MODE_CAPTURE },
	{ 0,				CAPTUREVIDEOTHUMB_MODE,		VISP_CAPTURETHUMB_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTURE },
	{ 0,				CAPTUREAVI_MODE,			VISP_CAPTURE_MODE,			DISP_ABL_MODE, JPEG_MODE_CAPTURE },
	{ 0,				CAPTUREAVITHUMB_MODE,		VISP_CAPTURETHUMB_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTURE },
	{ 0,				CAPTUREFRAME_MODE,			VISP_CAPTUREFRAME_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTUREFRAME },
	{ 0,				CAPTUREFRAMEVIDEO_MODE,		VISP_CAPTUREFRAME_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTUREFRAME },
	{ 0,				CAPTUREFRAMEAVI_MODE,		VISP_CAPTUREFRAME_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTUREFRAME },
	{ 0,				MULTISHOT_MODE,				VISP_CAPTURE_MODE,			DISP_ABL_MODE, JPEG_MODE_CAPTURE },
	{ 0,				MULTISHOTTHUMB_MODE,		VISP_CAPTURETHUMB_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTURE },
	{ MOD_CLK_SIFISP,	DISPLAYSTILL_MODE,			VISP_DISPLAY_MODE,			DISP_ABL_MODE, JPEG_MODE_DISPLAY },
	{ MOD_CLK_SIFISP,	ENCODE_MODE,				VISP_ENCODE_DECODE_MODE,	DISP_ABL_MODE, JPEG_MODE_ENCODE },
	{ MOD_CLK_SIFISP,	DECODE_MODE,				VISP_ENCODE_DECODE_MODE,	DISP_ABL_MODE, JPEG_MODE_DECODE },
	{ MOD_CLK_SIFISP,	DECODEIPP_MODE,				VISP_DECODEIPP_MODE,		DISP_ABL_MODE, JPEG_MODE_DECODEIPP },
	{ MOD_CLK_SIFISP,	DECODEFRAME_MODE,			VISP_DECODEFRAME_MODE,		DISP_DEF_MODE, JPEG_MODE_DECODEFRAME },
	{ 0,				MULTISHOTFRAME_MODE,		VISP_CAPTUREFRAME_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTUREFRAME },
	{ 0,				CAPTURETHUMB_MODE,			VISP_CAPTURETHUMB_MODE,		DISP_ABL_MODE, JPEG_MODE_IDLE },
	{ MOD_CLK_SIFISP,	DISPLAYVIDEO_MODE,			VISP_DISPLAY_MODE,			DISP_ABL_MODE, JPEG_MODE_DISPLAY },
	{ MOD_CLK_SIFISP,	DISPLAYAVI_MODE,			VISP_DISPLAY_MODE,			DISP_ABL_MODE, JPEG_MODE_DISPLAY },
	{ MOD_CLK_SIFISP | MOD_CLK_JPEG | MOD_CLK_JPEG_CORE | MOD_CLK_LBUF | MOD_CLK_IPP, 
						DIRECTDISPLAY_MODE,			VISP_NOSTREAM_MODE,			DISP_ABL_MODE, JPEG_MODE_IDLE },
	{ MOD_CLK_ALL,		BYPASS_MODE,				VISP_NOSTREAM_MODE,			DISP_IDLE_MODE,JPEG_MODE_IDLE },
	{ MOD_CLK_SIFISP | MOD_CLK_JPEG | MOD_CLK_JPEG_CORE | MOD_CLK_LBUF | MOD_CLK_IPP | MOD_CLK_LCDC | MOD_CLK_MARB_FAST | MOD_CLK_MARB | MOD_CLK_GE, 
						THROUGH_MODE,				VISP_NOSTREAM_MODE,			DISP_IDLE_MODE,JPEG_MODE_IDLE },
	{ MOD_CLK_SIFISP,	DECODEIPPBYPASS_MODE,		VISP_DECODEIPP_MODE,		DISP_ABL_MODE, JPEG_MODE_DECODEIPP_BYPASS },
	{ MOD_CLK_SIFISP,	DECODEFRAMEBYPASS_MODE,		VISP_DECODEFRAME_MODE,		DISP_DEF_MODE, JPEG_MODE_DECODEFRAME_BYPASS },
	{ MOD_CLK_SIFISP,	DISPLAYBYPASS_MODE,			VISP_DISPLAY_MODE,			DISP_ABL_MODE, JPEG_MODE_DISPLAY_BYPASS },
	{ 0,				CAPTUREFRAMETHUMB_MODE,		VISP_CAPTURETHUMB_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTUREFRAME },
	{ 0,				CAPTUREFRAMEVIDEOTHUMB_MODE,VISP_CAPTURETHUMB_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTUREFRAME },
	{ 0,				CAPTUREFRAMEAVITHUMB_MODE,	VISP_CAPTURETHUMB_MODE,		DISP_ABL_MODE, JPEG_MODE_CAPTUREFRAME }
};


void V5M_SetMode(UINT8 mode)
{
	if(mode > CAPTUREFRAMEAVITHUMB_MODE)
		mode = PREVIEW_MODE;

	if(gHapiContext.workmode == mode)
		return;
	
    if(gHapiContext.workmode == BYPASS_MODE)
		V5_CtrlBypassToNormal();

	V5B_CpmSetModClkVal(0);
	V5_MarbSetMode((UINT8)gModeSetting[mode][1]);
	V5_DispSetMode((UINT8)gModeSetting[mode][3]);
	V5_JpegSetMode((UINT8)gModeSetting[mode][4]);
	V5_VispSetMode((UINT8)gModeSetting[mode][2]);
	V5B_CpmSetModClkVal(gModeSetting[mode][0]);

#ifdef	WIN32
	//PanelDbgSetMode(mode); //what?
#endif

	if(mode == BYPASS_MODE)
	    V5_CtrlNormalToBypass();

	 gHapiContext.workmode = mode;
}

void V5M_SetModeAction(UINT8 mode)
{
	V5B_SifDisableSyncGen();
	V5M_SetMode(mode);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_SifEnableSyncGen();
}

UINT8 V5M_GetMode(void)
{
	return gHapiContext.workmode;
}

void V5M_Close(void)
{
	gHapiContext.workmode = 0xff;
}

//#endif //V578CMD_DISABLE

//-----------------------------------------------------------
UINT32 V5M_CapStill_start(void) 
{
	THapiStdInfo *pinfo;
	UINT8 mode;

	pinfo = &gHapiContext.hapiStdInfo;

	switch (pinfo->featureflag & 3)
	{
	case V5M_BIT_FRAMETHUMB:
		mode = CAPTUREFRAMETHUMB_MODE;
		break;
	case V5M_BIT_FRAME:
		mode = CAPTUREFRAME_MODE;
		break;
	case V5M_BIT_THUMB:
		mode = CAPTURESTILLTHUMB_MODE;
		break;
	default:
		mode = CAPTURESTILL_MODE;
		break;
	}
	
	//V5M_AutoSetCapSizer();
	V5M_SetModeAction( mode );
	V5M_StartCapture();
	return 0;
}
//-----------------------------------------------------------
UINT32 V5M_CapMultishot_start(UINT8 mshotcount)
{
	THapiStdInfo *pinfo;
	UINT8 mode;

	pinfo = &gHapiContext.hapiStdInfo;

	if (pinfo->featureflag & 1) 
			mode = MULTISHOTFRAME_MODE;
	else 
			mode = MULTISHOT_MODE;

	V5B_MarbSetMshotFcnt( mshotcount - 1);
	//V5M_AutoSetCapSizer();
	V5M_SetModeAction( mode );
	V5M_StartCapture();
	return 0;
}
//-----------------------------------------------------------
UINT32 V5M_CapVideo_start(void )  //it save video as a file
{
	THapiStdInfo *pinfo; 
	UINT8 mode;

	pinfo = &gHapiContext.hapiStdInfo;

	switch (pinfo->featureflag & 3)
	{
	case 0:
		mode = CAPTUREAVI_MODE;
		break;
	case V5M_BIT_FRAMETHUMB: //temp set
	case V5M_BIT_FRAME:
		mode = CAPTUREFRAMEAVI_MODE;
		break;
	case V5M_BIT_THUMB:
		mode = CAPTUREAVITHUMB_MODE;
		break;
	default:	
		mode = CAPTUREAVI_MODE;
		break;
	}

	//V5M_AutoSetCapSizer();
	V5M_SetModeAction( mode );
	V5M_StartCapture();
	return 0;
}
//-----------------------------------------------------------


void  V5M_PreviewAuto(void)
{
	if (gHapiContext.hapiStdInfo.compresssizetype ==V5M_COMPRESSSIZE_AUTO )
	{
		V5B_JpegDisAbrc();
		V5M_SetModeAction(PREVIEW_MODE);
	}
	else 
	{
		V5B_JpegEnAbrc();
		V5M_SetModeAction(PREVIEWBRC_MODE);
	}
}


//UINT32 V5M_GetPhotoToBuf()
//{
//
//}
//
//UINT32 V5M_GetPhotoToFile()
//{
//}

#endif //V578CMD_DISABLE
