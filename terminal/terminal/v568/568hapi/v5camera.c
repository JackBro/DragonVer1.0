#include "inheaders.h"
#ifndef	V578CMD_DISABLE

UINT32 V5M_CameraOnWithDefault(void)
{
	TSize Asize,size;
	TPoint pt = {0, 0};
	TALayerProty proty;
	UINT8 index=0;

	V5_DispGetPanelSize(&Asize);
	proty.pos2panel = pt;
	proty.memsize = Asize;
	proty.win.ltp = pt;
	proty.win.size = Asize;
	V5_DispSetAmemSize(&proty);

	V5M_CamSetDefaultValue();
	V5_VispGetSize(index, &size);
	V5_VispSetDispSizer(index, pt, size, proty.memsize);
	V5_VispSetEnv(FREQ_50HZ);
	V5M_SetMode(PREVIEWBRC_MODE);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_SifDisableSyncGen();
	V5_VispSetSnrState(SENSOR_POWERON);			//open sensor
	V5M_CamSetCamState(V5M_CAMERA_OPEN);
	//V5_VispEnableAe();
	V5B_SifEnableSyncGen();
	return 0;
}

void V5M_CamSetCamState(UINT8 state)
{
	gHapiContext.camera_state = state;
}

void V5M_CamSetDefaultValue(void)
{
	gHapiContext.hapiStdInfo.digitalzoom = 5 ;
	gHapiContext.hapiStdInfo.digitalzoomstep = 1;
	gHapiContext.hapiStdInfo.brightness = 32 ;
	gHapiContext.hapiStdInfo.brightstep = 2;
	gHapiContext.hapiStdInfo.contrast = 32 ;
	gHapiContext.hapiStdInfo.contraststep = 2;
}



UINT32 V5M_CameraOn(void)
{
	TSize Asize;
	TPoint pt = {0, 0};
	TALayerProty proty;
	UINT8 index=0;
	THapiStdInfo *pInfo;

	pInfo = &gHapiContext.hapiStdInfo;
	if (!pInfo)
		return V5M_CameraOnWithDefault();

	V5_DispGetPanelSize(&Asize);
	proty.pos2panel = pt;
	proty.memsize = Asize;
	proty.win.ltp = pt;
	proty.win.size = Asize;
	V5_DispSetAmemSize(&proty);

	//V5M_CameraGetSizeByIndex(pInfo->photores,&size);
	//index = V5_VispGetImageIndexBySize(size);
	V5M_SetSnrReso(index);
	//--other setting will be set later.

	//V5M_CameraZoom(pInfo->digitalzoom);
	V5M_SetExposeMode(pInfo->aemode);
	V5_VispSetBrightness(pInfo->brightness);
	V5_VispSetContrast(pInfo->contrast);

	
	V5M_SetMode(PREVIEW_MODE);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_SifDisableSyncGen();
	V5_VispSetSnrState(SENSOR_POWERON);			//open sensor
	//V5_VispEnableAe();
	V5B_SifEnableSyncGen();
	V5M_CamSetCamState(V5M_CAMERA_OPEN);

	//if (pInfo->isaf)						//how to judge whether sensor support AF£¿ 
	//	V5_VispStartAutoFocus();
	//else 
	//	V5_VispStopAutoFocus();
	//
	
	//---------
	return 0;
	
}

void V5M_ComputeOriginWinSize(TSize srcsize, TSize dispsize, TSize *winsize, TPoint *winpt )
{
	UINT32 ratiox, ratioy, ratio ;

	ratiox = (srcsize.cx << 10) / dispsize.cx;
	ratioy = (srcsize.cy << 10) / dispsize.cy;
	ratio = (ratiox < ratioy) ? ratiox :ratioy;
	winsize->cx = (UINT16) ((dispsize.cx * ratio) >> 10);
	winsize->cy = (UINT16) ((dispsize.cy * ratio) >> 10);
	winpt->x = ( srcsize.cx - winsize->cx ) >> 1;
	winpt->y = ( srcsize.cy - winsize->cy ) >> 1;
}

void V5M_InitWinInfo(TSize winsize) //(TPoint winpt, TSize winsize)
{
	gHapiContext.originwinsize = winsize;
	//gHapiContext.curwinsize = winsize;
	//gHapiContext.curwinpoint = winpt;
}
/********************************************************************************
	when zoom in
	1. window size will be smaller by step ,but cannot be smaller than the display size
	   in this way, user eyewindow is wider, while things are smaller.
	2. display Size is unchanged.
*********************************************************************************/

UINT32 V5M_CameraZoomIn(void)
{
	UINT8 digitalzoom;
	digitalzoom = gHapiContext.hapiStdInfo.digitalzoom + gHapiContext.hapiStdInfo.digitalzoomstep;
	//gHapiContext.hapiStdInfo.digitalzoom = + gHapiContext.hapiStdInfo.digitalzoomstep;
	return V5M_CameraZoom(digitalzoom);
}

UINT32 V5M_CameraZoomOut(void)
{
	UINT8 digitalzoom;
	digitalzoom = gHapiContext.hapiStdInfo.digitalzoom - gHapiContext.hapiStdInfo.digitalzoomstep;
	//gHapiContext.hapiStdInfo.digitalzoom = - gHapiContext.hapiStdInfo.digitalzoomstep;
	return V5M_CameraZoom(digitalzoom);
}

UINT32 V5M_CameraZoom(UINT8 digitalzoom)
{
	TSize srcSize, dispSize, originwinSize, newwinSize; //capSize, 
	TPoint newwinpt;
	
	if ((digitalzoom > V5M_MAX_DIGITALZOOM) || (digitalzoom < V5M_MIN_DIGITALZOOM))
		return DIGITALZOOM_OUTOFRANGE;
	V5B_IppGetImageSize(&srcSize);
	V5B_IppGetDispSize(&dispSize);
	//V5B_IppGetCapSize(&capSize);

	originwinSize = gHapiContext.originwinsize;
	newwinSize.cx = (originwinSize.cx * digitalzoom) / 20;
	if ((newwinSize.cx > originwinSize.cx) || (newwinSize.cx < dispSize.cx))//|| ( newwinSize.cx < capSize.cx))
		return DIGITALZOOM_OUTOFRANGE;

	newwinSize.cy = (originwinSize.cy * digitalzoom) / 20;
	if ((newwinSize.cy > originwinSize.cy) || (newwinSize.cy < dispSize.cy)) //|| ( newwinSize.cy < capSize.cy))
		return DIGITALZOOM_OUTOFRANGE;
	newwinpt.x = (srcSize.cx - newwinSize.cx) >> 1;
	newwinpt.y = (srcSize.cy - newwinSize.cy) >> 1;


	if (!(gHapiContext.hapiStdInfo.featureflag & V5M_BIT_CAPASWALLPIC))
		V5_IppSetCapSizerEx1(newwinpt, newwinSize, newwinSize);	

	V5_IppSetDispSizerEx1(newwinpt, newwinSize, dispSize);
	   //capture Size change with digital zoom
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	//gHapiContext.curwinsize = newwinSize;
	//gHapiContext.curwinpoint = newwinpt;
	gHapiContext.hapiStdInfo.digitalzoom = digitalzoom;
	return 0;
}
//
//UINT32 V5M_CameraZoom(UINT8 digitalzoom)
//{
//	TSize srcSize, dispSize, newwinSize; //capSize, 
//	TPoint newwinpt;
//	//UINT8 index = 0;
//
//	if ((digitalzoom > V5M_MAX_DIGITALZOOM) || (digitalzoom < V5M_MIN_DIGITALZOOM))
//		return DIGITALZOOM_OUTOFRANGE;
//
//	V5B_IppGetImageSize(&srcSize);
//	V5B_IppGetDispSize(&dispSize);
//	//V5B_IppGetCapSize(&capSize);
//
//	newwinSize.cx = dispSize.cx * digitalzoom;
//	newwinSize.cy = dispSize.cy * digitalzoom;
//
//	if ((newwinSize.cx > srcSize.cx) || (newwinSize.cx < dispSize.cx))//|| ( newwinSize.cx < capSize.cx))
//		return DIGITALZOOM_OUTOFRANGE;
//	if ((newwinSize.cy > srcSize.cy) || (newwinSize.cy < dispSize.cy)) //|| ( newwinSize.cy < capSize.cy))
//		return DIGITALZOOM_OUTOFRANGE;
//
//	newwinpt.x = (srcSize.cx - newwinSize.cx) >> 1;
//	newwinpt.y = (srcSize.cy - newwinSize.cy) >> 1;
//	
//	V5_IppSetDispSizerEx1(newwinpt, newwinSize, dispSize);
//	//V5_IppSetCapSizerEx1(newwinpt, newwinSize, newwinSize);   //capture Size change with digital zoom
//	//V5_JpegAutoSetImage();
//	//V5_MarbAutoSetMem();
//	gHapiContext.hapiStdInfo.digitalzoom = digitalzoom;
//	return 0;
//}


UINT32 V5M_CameraBrightnessUp(void)
{
	UINT8 value;

	value = gHapiContext.hapiStdInfo.brightness + gHapiContext.hapiStdInfo.brightstep;
	V5M_CameraSetBrightness(value);
	return 0;
}

UINT32 V5M_CameraBrightnessDown(void)
{
	UINT8 value;

	value = gHapiContext.hapiStdInfo.brightness - gHapiContext.hapiStdInfo.brightstep;
	V5M_CameraSetBrightness(value);
	return 0;
}

UINT32 V5M_CameraSetBrightness(UINT8 value)
{
	if ((value > V5M_MAX_BRIGHT_CONTRAST) || (value < V5M_MIN_BRIGHT_CONTRAST))
		return BRIGHT_CONTRAST_OUTOFRANGE;

	V5_VispSetBrightness(value);
	gHapiContext.hapiStdInfo.brightness = value;
	return 0;
}

//UINT32 V5M_SetBlendratio(UINT8 value)
//{
//	V5B_LcdcSetB0BlendRatio(value);
//	gHapiContext.hapiStdInfo.blendratio = value;
//	return 0;
//}

UINT32 V5M_CameraContrastUp(void)
{
	UINT8 value;

	value = gHapiContext.hapiStdInfo.contrast + gHapiContext.hapiStdInfo.contraststep;
	V5M_CameraSetContrast(value);
	return 0;
}

UINT32 V5M_CameraContrastDown(void)
{
	UINT8 value;

	value = gHapiContext.hapiStdInfo.contrast - gHapiContext.hapiStdInfo.contraststep;
	V5M_CameraSetContrast(value);
	return 0;
}

UINT32 V5M_CameraSetContrast(UINT8 value)
{
	if ((value > V5M_MAX_BRIGHT_CONTRAST) || (value < V5M_MIN_BRIGHT_CONTRAST))
		return BRIGHT_CONTRAST_OUTOFRANGE;
	//if (gHapiContext.camera_state - V5M_CAMERA_OPEN ) 
	//	return CAMERA_ISNOT_OPEN;
	V5_VispSetContrast(value);
	gHapiContext.hapiStdInfo.contrast = value;
	return 0;
}


UINT32 V5M_SetExposeMode(UINT8 mode)
{
	switch(mode)
	{
	case V5M_AE_AUTO:
		V5_VispSetMinEt(0);
		V5_VispSetMaxEt(V5_VispGetMaxEtDefault());
		V5_VispSetYTop(V5_VispGetYTopDefault());
		break;
	case V5M_AE_OUTDOOR:
		V5_VispSetMinEt(0);
		V5_VispSetMaxEt(20);
		V5_VispSetYTop(V5_VispGetYTarget()+V5_VispGetYThd());
		break;
	case V5M_AE_INDOOR_50HZ:
		V5_VispSetEnv(FREQ_50HZ);
		break;
	case V5M_AE_INDOOR_60HZ:
		V5_VispSetEnv(FREQ_60HZ);
		break;
	case V5M_AE_NIGHT:
		V5_VispSetMinEt(5);
		V5_VispSetMaxEt(255);
		//V5_VispSetYTop(0xe0);
		break;
	default:
		break;
	}
	return 0;
}

UINT32 V5M_getSnrResoInfo(UINT8 *snrinfo)
{
	*snrinfo = *(gHapiContext.hapiStdInfo.sensorres);
	return 0;
}

//void V5M_FrameON(char *filename)
//{
//	V5MF_HANDLE fp;
//	UINT32 size, count;
//	UINT8 *bmpbuf;
//
//	fp = gHapiContext.fileop.fopen(filename,"rb");
//	if (fp)
//	{
//		size = gHapiContext.fileop.flen(fp);
//		bmpbuf = (UINT8 *) gHapiContext.pmalloc(size);
//		count = gHapiContext.fileop.fread( bmpbuf,size ,fp);
//		gHapiContext.fileop.fclose(fp);
//		if (count > 0) 
//		{
//			V5M_DisplayB0Bmp(bmpbuf);
//			V5B_LcdcEnableBlending();
//			V5B_LcdcSetB0BlendRatio(gHapiContext.hapiStdInfo.blendratio);
//		}
//		gHapiContext.hapiStdInfo.featureflag |= V5M_BIT_FRAME ;
//
//		gHapiContext.pfree(bmpbuf, size);
//	}
//}


void V5M_FrameON(char *filename)
{
	UINT32 size;
	char *bmpbuf;

	bmpbuf = V5M_LoadImageFile(filename, &size);
	if (bmpbuf)
	{
			V5M_DisplayB0Bmp((UINT8 *)bmpbuf);
			V5B_LcdcEnableBlending();
			V5B_LcdcSetB0BlendRatio(gHapiContext.hapiStdInfo.blendratio);

			gHapiContext.hapiStdInfo.featureflag |= V5M_BIT_FRAME ;
			gHapiContext.pfree(bmpbuf, size);
	}
}

void V5M_FrameOFF(void)
{
	gHapiContext.hapiStdInfo.featureflag &= ~V5M_BIT_FRAME;
	V5B_LcdcDisableBlending();
}

void V5M_ThumbON(void)
{
	gHapiContext.hapiStdInfo.featureflag |= V5M_BIT_THUMB;
}

void V5M_ThumbOFF(void)
{
	gHapiContext.hapiStdInfo.featureflag &= ~V5M_BIT_THUMB;
}

UINT8 V5M_FrameIsON(void)
{
	return (UINT8)(gHapiContext.hapiStdInfo.featureflag & V5M_BIT_FRAME);
}

void V5M_SetSnrReso(UINT8 index)
{
	TSize srcsize,winsize,dispsize;
	TPoint winpt;

	V5_VispGetSize(index, &srcsize);
	V5B_IppGetWinSize( &winsize);
	V5_DispGetPanelSize(&dispsize);

	V5_VispSetImageIndex(index);
	V5B_IppSetImageSize(srcsize);
	
	V5M_ComputeOriginWinSize(srcsize, dispsize , &winsize ,&winpt);
	V5_IppSetCapSizerEx1(winpt, winsize, winsize);
	V5_VispSetDispSizer(index, winpt, winsize, dispsize);
	V5M_InitWinInfo(winsize);
}

void V5M_AutoSetCapSizer(void)
{
	TSize capsize, dispsize, winsize;

	V5B_IppGetCapSize(&capsize);
	if (gHapiContext.hapiStdInfo.featureflag & V5M_BIT_CAPASWALLPIC)
	{
		V5B_IppGetDispSize(&dispsize);
		if (capsize.cx - dispsize.cx)
			V5_IppSetCapSizer(dispsize);		
	}
	else
	{
		V5B_IppGetWinSize(&winsize);
		if (winsize.cx - capsize.cx )
			V5_IppSetCapSizer(winsize);	
	}
}


#endif //V578CMD_DISABLE
//