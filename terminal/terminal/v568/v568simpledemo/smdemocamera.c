#include "internal.h"
#ifndef	V578CMD_DISABLE

//#include "../driver/driver.h"

static void SmpCameraAeCall(void);
static void SmpCameraDecBrightnessCall(void);
static void SmpCameraAddBrightnessCall(void);
static void SmpCameraDecContrastCall(void);
static void SmpCameraAddContrastCall(void);
static void SmpCameraZoomOutCall(void);
static void SmpCameraZoomInCall(void);
static void SmpCameraZoomInFore(void);
static void SmpCameraZoomOutFore(void);
static void SmpCameraZoomInBack(void);
static void SmpCameraZoomOutBack(void);
static void SmpCameraSpecialEffectNormalCall(void);
static void SmpCameraSpecialEffectMonoCall(void);
static void SmpCameraSpecialEffectSephiaCall(void);
static void SmpCameraSpecialEffectSpecialColorCall(void);
static void SmpCameraSpecialEffectNegativeCall(void);
static void SmpCameraSpecialEffectSketchCall(void);
static void SmpCameraSpecialEffectReliefCall(void);
static void SmpCameraSetSXGACall(void);
static void SmpCameraSetVGACall(void);
static void SmpCameraSetQVGACall(void);
static void SmpCameraSetQQVGACall(void);
static void SmpCameraSettingForeModeCall(void);
static void SmpCameraSettingBackModeCall(void);
static void SmpCameraSettingEnvAutoCall(void);
static void SmpCameraSettingEnvIndoorCall(void);
static void SmpCameraSettingEnvOutdoorCall(void);
static void SmpCameraSettingEnvDarkCall(void);
static void SmpSettingCaptureStillSXGACall(void);
static void SmpSettingCaptureStillVGACall(void);
static void SmpSettingCaptureStill760960Call(void);
static void SmpSettingCaptureStill384480Call(void);
static void SmpSettingCaptureVideoQVGACall(void);
static void SmpSettingCaptureVideo12896Call(void);
static void SmpSettingCaptureVideo128160Call(void);
static void SmpTestCall(void);
static void SmpTest1Call(void);

static void SmpCameraRefreshSize(void);
void SmpSetAeMode(UINT8 mode);

TSmpCamera gSmpCamera;

void SmpCameraOpen(void)
{
	V5_CameraOn(0);
	V5_VispEnableAe();
	gSmpCamera.smpZoomMode = 1;
	gSmpCamera.smpBright = V5_VispGetBrightness();
	gSmpCamera.smpContrast = V5_VispGetContrast();
	SmpCameraRefreshSize();
	SmpSettingCaptureStillVGACall();
	SmpSettingCaptureVideo128160Call();
	SmpSetAeMode(AE_INDOOR);
}

void SmpCameraRefreshSize(void)
{
	V5B_IppGetImageSize(&gSmpCamera.aProty.imgSize);
	V5B_IppGetWinSize(&gSmpCamera.aProty.winSize);
	V5B_IppGetWinPoint(&gSmpCamera.aProty.pt);
	V5B_IppGetCapSize(&gSmpCamera.aProty.capSize);
	V5B_IppGetDispSize(&gSmpCamera.aProty.dispSize);
	V5B_IppGetThumbSize(&gSmpCamera.aProty.thumbSize);
	gSmpCamera.aProty.capStill.cx = gSmpCamera.aProty.capSize.cx;
	gSmpCamera.aProty.capStill.cy = gSmpCamera.aProty.capSize.cy;
	gSmpCamera.aProty.capVideo.cx = gSmpCamera.aProty.capSize.cx;
	gSmpCamera.aProty.capVideo.cy = gSmpCamera.aProty.capSize.cy;
}

void SmpCameraSetCurSize(void)
{
	UINT8 index, curIndex;

	index = V5_VispGetImageIndexBySize(gSmpCamera.aProty.imgSize);
	curIndex = V5_VispGetImageIndex();
	if(index == curIndex)
		V5_IppSetCapSizerEx1(gSmpCamera.aProty.pt, gSmpCamera.aProty.winSize, gSmpCamera.aProty.capSize);
	else
        V5_VispSetCapSizer(index, gSmpCamera.aProty.pt, gSmpCamera.aProty.winSize, gSmpCamera.aProty.capSize);
	V5_IppSetDispSizerEx1(gSmpCamera.aProty.pt, gSmpCamera.aProty.winSize, gSmpCamera.aProty.dispSize);
	V5_IppSetThumbSizerByWidth((UINT8)gSmpCamera.aProty.thumbSize.cx);
}

PTSmpSize SmpCameraGetCurSize(void)
{
	return &gSmpCamera.aProty;
}

void SmpSetCaptureStill(void)
{
	TPoint pt;

	if((gSmpCamera.aProty.capStill.cx > gSmpCamera.aProty.imgSize.cx) || (gSmpCamera.aProty.capStill.cy > gSmpCamera.aProty.imgSize.cy))
	{
		pt.x = 0;
		pt.y = 0;
		V5_IppSetCapSizerEx1(pt, gSmpCamera.aProty.imgSize, gSmpCamera.aProty.imgSize);
		V5_IppSetDispSizerEx1(pt, gSmpCamera.aProty.imgSize, gSmpCamera.aProty.dispSize);
		V5_IppSetThumbSizerByWidth((UINT8)gSmpCamera.aProty.capThumbSize.cx);

	}
	else
	{
		pt.x = (((gSmpCamera.aProty.imgSize.cx-gSmpCamera.aProty.capStill.cx)/2)>>1)<<1;
		pt.y = (((gSmpCamera.aProty.imgSize.cy-gSmpCamera.aProty.capStill.cy)/2)>>1)<<1;
		V5_IppSetCapSizerEx1(pt, gSmpCamera.aProty.capStill, gSmpCamera.aProty.capStill);
		V5_IppSetThumbSizerByWidth((UINT8)gSmpCamera.aProty.capThumbSize.cx);
		V5_IppSetDispSizerEx1(pt, gSmpCamera.aProty.capStill, gSmpCamera.aProty.dispSize);
	}
}

void SmpSetCaptureVideo(void)
{
	TPoint pt;

	if((gSmpCamera.aProty.capVideo.cx > gSmpCamera.aProty.imgSize.cx) || (gSmpCamera.aProty.capVideo.cy > gSmpCamera.aProty.imgSize.cy))
	{
		pt.x = 0;
		pt.y = 0;
		V5_IppSetCapSizerEx1(pt, gSmpCamera.aProty.imgSize, gSmpCamera.aProty.imgSize);
		V5_IppSetDispSizerEx1(pt, gSmpCamera.aProty.imgSize, gSmpCamera.aProty.dispSize);
	}
	else
	{
		pt.x = (((gSmpCamera.aProty.imgSize.cx-gSmpCamera.aProty.capVideo.cx)/2)>>1)<<1;
		pt.y = (((gSmpCamera.aProty.imgSize.cy- gSmpCamera.aProty.capVideo.cy)/2)>>1)<<1;
		V5_IppSetCapSizerEx1(pt, gSmpCamera.aProty.capVideo, gSmpCamera.aProty.capVideo);
		V5_IppSetDispSizerEx1(pt, gSmpCamera.aProty.capVideo, gSmpCamera.aProty.dispSize);
	}
}

void SmpSetCaptureFrame(void)
{
	TPoint pt;

	if((gSmpCamera.aProty.dispSize.cx > gSmpCamera.aProty.imgSize.cx) || (gSmpCamera.aProty.dispSize.cy > gSmpCamera.aProty.imgSize.cy))
	{
		pt.x = 0;
		pt.y = 0;
		V5_IppSetCapSizerEx1(pt, gSmpCamera.aProty.imgSize, gSmpCamera.aProty.imgSize);
		V5_IppSetDispSizerEx1(pt, gSmpCamera.aProty.imgSize, gSmpCamera.aProty.dispSize);
		V5_IppSetThumbSizerByWidth((UINT8)gSmpCamera.aProty.capThumbSize.cx);
	}
	else
	{
		pt.x = (((gSmpCamera.aProty.imgSize.cx-gSmpCamera.aProty.capStill.cx)/2)>>1)<<1;
		pt.y = (((gSmpCamera.aProty.imgSize.cy-gSmpCamera.aProty.capStill.cy)/2)>>1)<<1;
		V5_IppSetCapSizerEx1(pt, gSmpCamera.aProty.capStill, gSmpCamera.aProty.dispSize);
		V5_IppSetThumbSizerByWidth((UINT8)gSmpCamera.aProty.capThumbSize.cx);
		V5_IppSetDispSizerEx1(pt, gSmpCamera.aProty.capStill, gSmpCamera.aProty.dispSize);
	}
}

void SmpSetAeMode(UINT8 mode)
{
	gSmpCamera.smpAeMode = mode;
	V5_SetExposeMode(mode);
}

void SmpCamera(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_EXIT,				NULL},
		{ MENU_KEY_DOWN,	MENU_DISP,				NULL},
		{ MENU_KEY_LEFT,	MENU_EXIT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_AUTOEXPOSE, NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_AUTOEXPOSE, NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSetting(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_JPEG,				NULL},
		{ MENU_KEY_DOWN,	MENU_TEST,				NULL},
		{ MENU_KEY_LEFT,	MENU_EXIT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ZOOM_MODE, NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ZOOM_MODE, NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA,		NULL},
		{ MENU_KEY_LEFT,	MENU_EXIT,			NULL},
		{ MENU_KEY_RIGHT,	MENU_EXIT,			NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,		NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraAe(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EXIT,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_BRIGHT,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA,			NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_AUTOEXPOSE, NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_AUTOEXPOSE, SmpCameraAeCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraAeCall(void)
{
	if(V5_VispIsAe())
		V5_VispDisableAe();
	else
		V5_VispEnableAe();
}

void SmpCameraAeQuery(void)
{
	return;
}

void SmpCameraBrightness(UINT8 key, UINT8 *menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_AUTOEXPOSE,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_CONTRAST,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_BRIGHT,			SmpCameraDecBrightnessCall},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_BRIGHT,			SmpCameraAddBrightnessCall},
		{ MENU_KEY_SELECT,	MENU_CAMERA_BRIGHT,			NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraDecBrightnessCall(void)
{
	gSmpCamera.smpBright -= BRIGHT_CONTRAST_STEP;
	V5_VispSetBrightness(gSmpCamera.smpBright);
}

void SmpCameraAddBrightnessCall(void)
{
	gSmpCamera.smpBright += BRIGHT_CONTRAST_STEP;
	V5_VispSetBrightness(gSmpCamera.smpBright);
}

void SmpCameraContrast(UINT8 key, UINT8 *menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_BRIGHT,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_ZOOM,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_CONTRAST,	SmpCameraDecContrastCall},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_CONTRAST,	SmpCameraAddContrastCall},
		{ MENU_KEY_SELECT,	MENU_CAMERA_CONTRAST,	NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraDecContrastCall(void)
{
	gSmpCamera.smpContrast -= BRIGHT_CONTRAST_STEP;
	V5_VispSetContrast(gSmpCamera.smpContrast);
}

void SmpCameraAddContrastCall(void)
{
	gSmpCamera.smpContrast += BRIGHT_CONTRAST_STEP;
	V5_VispSetContrast(gSmpCamera.smpContrast);
}

void SmpCameraZoom(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_CONTRAST,	NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EFFECT,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_ZOOM,		SmpCameraZoomOutCall},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_ZOOM,		SmpCameraZoomInCall},
		{ MENU_KEY_SELECT,	MENU_CAMERA_ZOOM,		NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraZoomOutCall(void)
{
	if(gSmpCamera.smpZoomMode == 0)		//prezoom
		SmpCameraZoomOutFore();
	else
		SmpCameraZoomOutBack();
}	

void SmpCameraZoomInCall(void)
{
	if(gSmpCamera.smpZoomMode == 0)
		SmpCameraZoomInFore();
	else
		SmpCameraZoomInBack();
}

void SmpCameraZoomInFore(void)		//放大，减少window size
{
	TSize winSize, decSize, dispSize;
	TPoint pt;

	winSize.cx = gSmpCamera.aProty.winSize.cx;
	winSize.cy = gSmpCamera.aProty.winSize.cy;
	pt.x = gSmpCamera.aProty.pt.x;
	pt.y = gSmpCamera.aProty.pt.y;
	dispSize.cx = gSmpCamera.aProty.dispSize.cx;
	dispSize.cy = gSmpCamera.aProty.dispSize.cy;

	VComputerSizeByWidth(winSize, ZOOM_STEP, &decSize);
	if(((winSize.cx - decSize.cx) < dispSize.cx ) || ((winSize.cy - decSize.cy) < dispSize.cy))
		return;
	if(((winSize.cx - decSize.cx) < gSmpCamera.aProty.capSize.cx) || ((winSize.cy - decSize.cy) < gSmpCamera.aProty.capSize.cy))
		return;

	winSize.cx = winSize.cx - decSize.cx;
	winSize.cy = winSize.cy - decSize.cy;
	winSize.cx = (winSize.cx>>1)<<1;
	winSize.cy = (winSize.cy>>1)<<1;
	if(winSize.cx == 0)
		winSize.cx = ZOOM_STEP;
	if(winSize.cy == 0)
		winSize.cy = ZOOM_STEP;
	pt.x += (decSize.cx/2);
	pt.y += (decSize.cy/2);
	pt.x = (pt.x>>1)<<1;
	pt.y = (pt.y>>1)<<1;

	gSmpCamera.aProty.winSize.cx = winSize.cx;
	gSmpCamera.aProty.winSize.cy = winSize.cy;
	V5_IppSetDispSizerEx1(pt, winSize, dispSize);
	V5_IppSetCapSizerEx1(pt, winSize, gSmpCamera.aProty.capSize);
}

void SmpCameraZoomOutFore(void)		//缩小，增加window size
{
	TSize imgSize, winSize, decSize, dispSize;
	TPoint pt;

	imgSize.cx = gSmpCamera.aProty.imgSize.cx;
	imgSize.cy = gSmpCamera.aProty.imgSize.cy;
	winSize.cx = gSmpCamera.aProty.winSize.cx;
	winSize.cy = gSmpCamera.aProty.winSize.cy;
	pt.x = gSmpCamera.aProty.pt.x;
	pt.y = gSmpCamera.aProty.pt.y;
	dispSize.cx = gSmpCamera.aProty.dispSize.cx;
	dispSize.cy = gSmpCamera.aProty.dispSize.cy;

	VComputerSizeByWidth(winSize, ZOOM_STEP, &decSize);
	if(((winSize.cx + decSize.cx) >= imgSize.cx) || ((winSize.cy + decSize.cy) >= imgSize.cy))
		return;

	winSize.cx = winSize.cx + decSize.cx;
	winSize.cy = winSize.cy + decSize.cy;
	winSize.cx = (winSize.cx>>1)<<1;
	winSize.cy = (winSize.cy>>1)<<1;
	if((pt.x > decSize.cx/2) && (pt.y > decSize.cy/2))
	{
		pt.x -= (decSize.cx/2);
		pt.y -= (decSize.cy/2);
		pt.x = (pt.x>>1)<<1;
		pt.y = (pt.y>>1)<<1;
	}

	gSmpCamera.aProty.winSize.cx = winSize.cx;
	gSmpCamera.aProty.winSize.cy = winSize.cy;
	V5_IppSetDispSizerEx1(pt, winSize, dispSize);
	V5_IppSetCapSizerEx1(pt, winSize, gSmpCamera.aProty.capSize);
}

void SmpCameraZoomOutBack(void)		//缩小，减小display sizer
{
	TSize winSize, decSize, dispSize, lcdSize;
	TPoint pt;
	TALayerProty proty;

	winSize.cx = gSmpCamera.aProty.winSize.cx;
	winSize.cy = gSmpCamera.aProty.winSize.cy;
	pt.x = gSmpCamera.aProty.pt.x;
	pt.y = gSmpCamera.aProty.pt.y;
	dispSize.cx = gSmpCamera.aProty.dispSize.cx;
	dispSize.cy = gSmpCamera.aProty.dispSize.cy;

	V5_DispGetAmemSize(&proty);
	V5_DispGetPanelSize(&lcdSize);
	
	VComputerSizeByWidth(dispSize, ZOOM_STEP, &decSize);
	if(((dispSize.cx-decSize.cx) < (proty.win.size.cx+proty.win.ltp.x)) || ((dispSize.cy-decSize.cy) < (proty.win.size.cy+proty.win.ltp.y)))
		return;
	if((dispSize.cx < lcdSize.cx) || (dispSize.cy < lcdSize.cy))
		return;
	
	dispSize.cx = dispSize.cx - decSize.cx;
	dispSize.cy = dispSize.cy - decSize.cy;
	dispSize.cx = (dispSize.cx>>1)<<1;
	dispSize.cy = (dispSize.cy>>1)<<1;
	if(dispSize.cx == 0)
		dispSize.cx = ZOOM_STEP;
	if(dispSize.cy == 0)
		dispSize.cy = ZOOM_STEP;
	
	gSmpCamera.aProty.dispSize.cx = dispSize.cx;
	gSmpCamera.aProty.dispSize.cy = dispSize.cy;
	V5_IppSetDispSizerEx1(pt, winSize, dispSize);
	proty.memsize.cx = dispSize.cx;
	proty.memsize.cy = dispSize.cy;
	V5_DispSetAmemSize(&proty);
}

void SmpCameraZoomInBack(void)		//放大，增加display size
{
	TSize imgSize, winSize, decSize, dispSize;
	TPoint pt;
	TALayerProty proty;

	imgSize.cx = gSmpCamera.aProty.imgSize.cx;
	imgSize.cy = gSmpCamera.aProty.imgSize.cy;
	winSize.cx = gSmpCamera.aProty.winSize.cx;
	winSize.cy = gSmpCamera.aProty.winSize.cy;
	pt.x = gSmpCamera.aProty.pt.x;
	pt.y = gSmpCamera.aProty.pt.y;
	dispSize.cx = gSmpCamera.aProty.dispSize.cx;
	dispSize.cy = gSmpCamera.aProty.dispSize.cy;

	V5_DispGetAmemSize(&proty);

	VComputerSizeByWidth(dispSize, ZOOM_STEP, &decSize);
	if(((dispSize.cx + decSize.cx) > winSize.cx) || ((dispSize.cy + decSize.cy) > winSize.cy))
		return;
	if(((dispSize.cx + decSize.cx) > SMP_MAX_DISPLAY_X) || ((dispSize.cy + decSize.cy) > SMP_MAX_DISPLAY_Y))
		return;
	
	dispSize.cx = dispSize.cx + decSize.cx;
	dispSize.cy = dispSize.cy + decSize.cy;
	dispSize.cx = (dispSize.cx>>1)<<1;
	dispSize.cy = (dispSize.cy>>1)<<1;
	
	gSmpCamera.aProty.dispSize.cx = dispSize.cx;
	gSmpCamera.aProty.dispSize.cy = dispSize.cy;
	V5_IppSetDispSizerEx1(pt, winSize, dispSize);
	proty.memsize.cx = dispSize.cx;
	proty.memsize.cy = dispSize.cy;
	V5_DispSetAmemSize(&proty);
}

void SmpCameraSpecialEffect(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_ZOOM,			NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_SIZE,			NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EFFECT_NORMAL,	NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_EFFECT_NORMAL,	NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSpecialEffectNormal(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EFFECT_EXIT,	NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EFFECT_MONO,	NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_EFFECT,			NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EFFECT_NORMAL,	NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_EFFECT_NORMAL,	SmpCameraSpecialEffectNormalCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSpecialEffectNormalCall(void)
{
	V5B_IppDisableSpecial();
}

void SmpCameraSpecialEffectMono(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EFFECT_NORMAL,	NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EFFECT_SEPIA,	NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_EFFECT,			NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EFFECT_MONO,	NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_EFFECT_MONO,	SmpCameraSpecialEffectMonoCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSpecialEffectMonoCall(void)
{
	V5B_IppEnableSpecial();
	V5B_IppSetSpecialEffect(IPP_SPE_MONO);
}

void SmpCameraSpecialEffectSephia(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EFFECT_MONO,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EFFECT_SPE_COLOR,	NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_EFFECT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EFFECT_SEPIA,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_EFFECT_SEPIA,		SmpCameraSpecialEffectSephiaCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSpecialEffectSephiaCall(void)
{
	V5B_IppEnableSpecial();
	V5B_IppSetSpecialEffect(IPP_SPE_SEPHIA);
}

void SmpCameraSpecialEffectSpecialColor(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EFFECT_SEPIA,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EFFECT_NEGA,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_EFFECT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EFFECT_SPE_COLOR,	NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_EFFECT_SPE_COLOR,	SmpCameraSpecialEffectSpecialColorCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSpecialEffectSpecialColorCall(void)
{
	V5B_IppEnableSpecial();
	V5B_IppSetSpecialEffect(IPP_SPE_COLOR);
}

void SmpCameraSpecialEffectNegative(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EFFECT_SPE_COLOR,	NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EFFECT_RELIEF,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_EFFECT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EFFECT_NEGA,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_EFFECT_NEGA,		SmpCameraSpecialEffectNegativeCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSpecialEffectNegativeCall(void)
{
	V5B_IppEnableSpecial();
	V5B_IppSetSpecialEffect(IPP_SPE_NEGATIVE);
}

void SmpCameraSpecialEffectSketch(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EFFECT_RELIEF,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EFFECT_RETURN,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_EFFECT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EFFECT_SKETCH,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_EFFECT_SKETCH,		SmpCameraSpecialEffectSketchCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSpecialEffectSketchCall(void)
{
	V5B_IppEnableSpecial();
	V5B_IppSetSpecialEffect(IPP_SPE_SKETCH);
}

void SmpCameraSpecialEffectRelief(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EFFECT_NEGA,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EFFECT_SKETCH,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_EFFECT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EFFECT_RELIEF,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_EFFECT_RELIEF,		SmpCameraSpecialEffectReliefCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSpecialEffectReliefCall(void)
{
//	V5B_IppEnableSpecial();
//	V5B_IppSetSpecialEffect(V5B_IPPRELIEF);
}

void SmpCameraSpecialEffectReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EFFECT_SKETCH,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EFFECT_EXIT,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_EFFECT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EFFECT_RETURN,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_EFFECT,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSpecialEffectExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EFFECT_RETURN,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EFFECT_NORMAL,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_EFFECT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EFFECT_EXIT,		NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,		NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraImageSize(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_EFFECT,			NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_RETURN,			NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_SIZE_SXGA,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_SIZE_SXGA,		NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSetSXGA(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_SIZE_EXIT,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_SIZE_VGA,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_SIZE,			NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_SIZE_SXGA,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_SIZE_SXGA,		SmpCameraSetSXGACall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSetSXGACall(void)
{
	UINT8 index, oldIndex;
	TSize imgSize;

	oldIndex = V5_VispGetImageIndex();
	index = V5_VispGetImageIndexByWidth(1280);
	if((index == 0xff) || (index == oldIndex))
	{
		return;
	}
	else
	{
		V5_VispGetSize(index, &imgSize);
		V5_VispSetCapSizer(index, gSmpCamera.aProty.pt, imgSize, gSmpCamera.aProty.capSize);
		V5_VispSetDispSizer(index, gSmpCamera.aProty.pt, imgSize, gSmpCamera.aProty.dispSize);
		gSmpCamera.aProty.imgSize.cx = imgSize.cx;
		gSmpCamera.aProty.imgSize.cy = imgSize.cy;
		gSmpCamera.aProty.winSize.cx = imgSize.cx;
		gSmpCamera.aProty.winSize.cy = imgSize.cy;
	}
}

void SmpCameraSetVGA(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_SIZE_SXGA,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_SIZE_QVGA,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_SIZE,			NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_SIZE_VGA,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_SIZE_VGA,		SmpCameraSetVGACall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSetVGACall(void)
{
	UINT8 index, oldIndex;
	TSize imgSize;

	oldIndex = V5_VispGetImageIndex();
	index = V5_VispGetImageIndexByWidth(640);
	if((index == 0xff) || (index == oldIndex))
	{
		return;
	}
	else
	{
		V5_VispGetSize(index, &imgSize);
		V5_VispSetCapSizer(index, gSmpCamera.aProty.pt, imgSize, gSmpCamera.aProty.capSize);
		V5_VispSetDispSizer(index, gSmpCamera.aProty.pt, imgSize, gSmpCamera.aProty.dispSize);
		gSmpCamera.aProty.imgSize.cx = imgSize.cx;
		gSmpCamera.aProty.imgSize.cy = imgSize.cy;
		gSmpCamera.aProty.winSize.cx = imgSize.cx;
		gSmpCamera.aProty.winSize.cy = imgSize.cy;
	}
}

void SmpCameraSetQVGA(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_SIZE_VGA,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_SIZE_QQVGA,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_SIZE,			NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_SIZE_QVGA,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_SIZE_QVGA,		SmpCameraSetQVGACall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSetQVGACall(void)
{
	UINT8 index, oldIndex;
	TSize imgSize;

	oldIndex = V5_VispGetImageIndex();
	index = V5_VispGetImageIndexByWidth(320);
	if((index == 0xff) || (index == oldIndex))
	{
		return;
	}
	else
	{
		V5_VispGetSize(index, &imgSize);
		V5_VispSetCapSizer(index, gSmpCamera.aProty.pt, imgSize, gSmpCamera.aProty.capSize);
		V5_VispSetDispSizer(index, gSmpCamera.aProty.pt, imgSize, gSmpCamera.aProty.dispSize);
		gSmpCamera.aProty.imgSize.cx = imgSize.cx;
		gSmpCamera.aProty.imgSize.cy = imgSize.cy;
		gSmpCamera.aProty.winSize.cx = imgSize.cx;
		gSmpCamera.aProty.winSize.cy = imgSize.cy;
	}
}

void SmpCameraSetQQVGA(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_SIZE_QVGA,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_SIZE_RETURN,	NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_SIZE,			NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_SIZE_QQVGA,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_SIZE_QQVGA,		SmpCameraSetQQVGACall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSetQQVGACall(void)
{
	UINT8 index, oldIndex;
	TSize imgSize;

	oldIndex = V5_VispGetImageIndex();
	index = V5_VispGetImageIndexByWidth(160);
	if((index == 0xff) || (index == oldIndex))
	{
		return;
	}
	else
	{
		V5_VispGetSize(index, &imgSize);
		V5_VispSetCapSizer(index, gSmpCamera.aProty.pt, imgSize, gSmpCamera.aProty.capSize);
		V5_VispSetDispSizer(index, gSmpCamera.aProty.pt, imgSize, gSmpCamera.aProty.dispSize);
		gSmpCamera.aProty.imgSize.cx = imgSize.cx;
		gSmpCamera.aProty.imgSize.cy = imgSize.cy;
		gSmpCamera.aProty.winSize.cx = imgSize.cx;
		gSmpCamera.aProty.winSize.cy = imgSize.cy;
	}
}

void SmpCameraSetSizeReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_SIZE_QQVGA,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_SIZE_EXIT,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_SIZE,			NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_SIZE_RETURN,	NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA_SIZE,			NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSetSizeExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_SIZE_RETURN,	NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_SIZE_SXGA,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA_SIZE,			NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_SIZE_EXIT,		NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_SIZE,		NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_EXIT,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA,			NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_RETURN,		NULL},
		{ MENU_KEY_SELECT,	MENU_CAMERA,			NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_CAMERA_RETURN,			NULL},
		{ MENU_KEY_DOWN,	MENU_CAMERA_AUTOEXPOSE,		NULL},
		{ MENU_KEY_LEFT,	MENU_CAMERA,				NULL},
		{ MENU_KEY_RIGHT,	MENU_CAMERA_EXIT,			NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingZoomMode(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_EXIT,				NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ENV,				NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING,					NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ZOOM_MODE_FORE,	NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ZOOM_MODE_FORE,	NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingEnv(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ZOOM_MODE,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPSTILL,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ENV_AUTO,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ENV_AUTO,		NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureStill(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ENV,				NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPVIDEO,			NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING,					NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPSTILL_SXGA,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPSTILL_SXGA,		NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureVideo(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPSTILL,			NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_RETURN,			NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING,					NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPVIDEO_QVGA,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPVIDEO_QVGA,		NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPVIDEO,	NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_EXIT,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_RETURN,	NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING,			NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_RETURN,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ZOOM_MODE,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_EXIT,			NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingForeMode(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ZOOM_MODE_EXIT,	NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ZOOM_MODE_BACK,	NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_ZOOM_MODE,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ZOOM_MODE_FORE,	NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ZOOM_MODE_FORE,	SmpCameraSettingForeModeCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingForeModeCall(void)
{
	gSmpCamera.smpZoomMode = 0;
}

void SmpCameraSettingBackMode(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ZOOM_MODE_FORE,	NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ZOOM_MODE_RETURN,	NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_ZOOM_MODE,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ZOOM_MODE_BACK,	NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ZOOM_MODE_BACK,	SmpCameraSettingBackModeCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingBackModeCall(void)
{
	gSmpCamera.smpZoomMode = 1;
}

void SmpCameraSettingZoomModeReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ZOOM_MODE_BACK,	NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ZOOM_MODE_EXIT,	NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_ZOOM_MODE,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ZOOM_MODE_RETURN,	NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ZOOM_MODE,			NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingZoomModeExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ZOOM_MODE_RETURN,	NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ZOOM_MODE_FORE,	NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_ZOOM_MODE,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ZOOM_MODE_EXIT,	NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,					NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingEnvAuto(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ENV_EXIT,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ENV_INDOOR,	NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_ENV,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ENV_AUTO,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ENV_AUTO,		SmpCameraSettingEnvAutoCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingEnvAutoCall(void)
{
	SmpSetAeMode(AE_AUTO);
}

void SmpCameraSettingEnvIndoor(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ENV_AUTO,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ENV_OUTDOOR,	NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_ENV,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ENV_INDOOR,	NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ENV_INDOOR,	SmpCameraSettingEnvIndoorCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingEnvIndoorCall(void)
{
	SmpSetAeMode(AE_INDOOR);
}

void SmpCameraSettingEnvOutdoor(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ENV_INDOOR,	NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ENV_DARK,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_ENV,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ENV_OUTDOOR,	NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ENV_OUTDOOR,	SmpCameraSettingEnvOutdoorCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingEnvOutdoorCall(void)
{
	SmpSetAeMode(AE_OUTDOOR);
}

void SmpCameraSettingEnvDark(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ENV_OUTDOOR,	NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ENV_RETURN,	NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_ENV,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ENV_DARK,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ENV_DARK,		SmpCameraSettingEnvDarkCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingEnvDarkCall(void)
{
	SmpSetAeMode(AE_NIGHT);
}

void SmpCameraSettingEnvReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ENV_DARK,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ENV_EXIT,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_ENV,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ENV_RETURN,	NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_ENV,			NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpCameraSettingEnvExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_ENV_RETURN,	NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_ENV_AUTO,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_ENV,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_ENV_EXIT,		NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureStillSXGA(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPSTILL_EXIT,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPSTILL_VGA,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPSTILL,			NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPSTILL_SXGA,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPSTILL_SXGA,		SmpSettingCaptureStillSXGACall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureStillSXGACall(void)
{
	gSmpCamera.aProty.capStill.cx = 1280;
	gSmpCamera.aProty.capStill.cy = 960;
	gSmpCamera.aProty.capThumbSize.cx = 40;
	gSmpCamera.aProty.capThumbSize.cy = 30;
}

void SmpSettingCaptureStillVGA(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPSTILL_SXGA,			NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPSTILL_760960,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPSTILL,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPSTILL_VGA,			NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPSTILL_VGA,			SmpSettingCaptureStillVGACall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureStillVGACall(void)
{
	gSmpCamera.aProty.capStill.cx = 640;
	gSmpCamera.aProty.capStill.cy = 480;
	gSmpCamera.aProty.capThumbSize.cx = 40;
	gSmpCamera.aProty.capThumbSize.cy = 30;
}

void SmpSettingCaptureStill760960(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPSTILL_VGA,			NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPSTILL_384480,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPSTILL,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPSTILL_760960,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPSTILL_760960,		SmpSettingCaptureStill760960Call}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureStill760960Call(void)
{
	gSmpCamera.aProty.capStill.cx = 760;
	gSmpCamera.aProty.capStill.cy = 960;
	gSmpCamera.aProty.capThumbSize.cx = 40;
	gSmpCamera.aProty.capThumbSize.cy = 50;
}

void SmpSettingCaptureStill384480(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPSTILL_760960,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPSTILL_RETURN,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPSTILL,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPSTILL_384480,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPSTILL_384480,		SmpSettingCaptureStill384480Call}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureStill384480Call(void)
{
	gSmpCamera.aProty.capStill.cx = 384;
	gSmpCamera.aProty.capStill.cy = 480;
	gSmpCamera.aProty.capThumbSize.cx = 40;
	gSmpCamera.aProty.capThumbSize.cy = 50;
}

void SmpSettingCaptureStillReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPSTILL_384480,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPSTILL_EXIT,			NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPSTILL,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPSTILL_RETURN,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPSTILL,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureStillExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPSTILL_RETURN,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPSTILL_SXGA,			NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPSTILL,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPSTILL_EXIT,			NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,						NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureVideoQVGA(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPVIDEO_EXIT,			NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPVIDEO_12896,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPVIDEO,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPVIDEO_QVGA,			NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPVIDEO_QVGA,			SmpSettingCaptureVideoQVGACall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureVideoQVGACall(void)
{
	gSmpCamera.aProty.capVideo.cx = 320;
	gSmpCamera.aProty.capVideo.cy = 240;
}

void SmpSettingCaptureVideo12896(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPVIDEO_QVGA,			NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPVIDEO_128160,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPVIDEO,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPVIDEO_12896,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPVIDEO_12896,		SmpSettingCaptureVideo12896Call}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureVideo12896Call(void)
{
	gSmpCamera.aProty.capVideo.cx = 128;
	gSmpCamera.aProty.capVideo.cy = 96;
}

void SmpSettingCaptureVideo128160(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPVIDEO_12896,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPVIDEO_RETURN,		NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPVIDEO,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPVIDEO_128160,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPVIDEO_128160,		SmpSettingCaptureVideo128160Call}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureVideo128160Call(void)
{
	gSmpCamera.aProty.capVideo.cx = 128;
	gSmpCamera.aProty.capVideo.cy = 160;
}

void SmpSettingCaptureVideoReturn(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPVIDEO_128160,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPVIDEO_EXIT,			NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPVIDEO,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPVIDEO_RETURN,		NULL},
		{ MENU_KEY_SELECT,	MENU_SETTING_CAPVIDEO,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpSettingCaptureVideoExit(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING_CAPVIDEO_RETURN,		NULL},
		{ MENU_KEY_DOWN,	MENU_SETTING_CAPVIDEO_QVGA,			NULL},
		{ MENU_KEY_LEFT,	MENU_SETTING_CAPVIDEO,				NULL},
		{ MENU_KEY_RIGHT,	MENU_SETTING_CAPVIDEO_EXIT,			NULL},
		{ MENU_KEY_SELECT,	MENU_SEL_EXIT,						NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

//Test menu scroll
void SmpTest(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_SETTING,			NULL},
		{ MENU_KEY_DOWN,	MENU_EXIT,				NULL},
		{ MENU_KEY_LEFT,	MENU_EXIT,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST1,				SmpTestCall},
		{ MENU_KEY_SELECT,	MENU_TEST1,				SmpTestCall}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTestCall(void)
{
	UINT32 et;
	UINT8 gain, ymean, env, ytarget, ythd, ytop;

	ymean = V5B_IspGetYMean();	//1
	et = V5_VispGetCurEt();		//2
	gain = V5_VispGetCurGain(); //3
	ytarget = V5_VispGetYTarget();//4
	ythd = V5_VispGetYThd();//5
	ytop = V5_VispGetYTop();//6
	env = V5_VispGetEnv();//7

	sprintf((SINT8*)SmpGetString(MENU_TEST1), "%x", ymean);
	sprintf((SINT8*)SmpGetString(MENU_TEST2), "%ld", et);
	sprintf((SINT8*)SmpGetString(MENU_TEST3), "%x", gain);
	sprintf((SINT8*)SmpGetString(MENU_TEST4), "%x", ytarget);
	sprintf((SINT8*)SmpGetString(MENU_TEST5), "%x", ythd);
	sprintf((SINT8*)SmpGetString(MENU_TEST6), "%x", ytop);
	switch(env)
	{
	case FREQ_50HZ:
        sprintf((SINT8*)SmpGetString(MENU_TEST7), "%s", "50HZ");
		break;
	case FREQ_60HZ:
		sprintf((SINT8*)SmpGetString(MENU_TEST7), "%s", "60HZ");
		break;
	case FREQ_NONE:
		sprintf((SINT8*)SmpGetString(MENU_TEST7), "%s", "Outdoor");
		break;
	default:
		break;
	}
	switch(gSmpCamera.smpAeMode)
	{
	case AE_AUTO:
		sprintf((SINT8*)SmpGetString(MENU_TEST8), "%s", "Auto");
		break;
	case AE_INDOOR:
		sprintf((SINT8*)SmpGetString(MENU_TEST8), "%s", "Indoor");
		break;
	case AE_OUTDOOR:
		sprintf((SINT8*)SmpGetString(MENU_TEST8), "%s", "Outdoor");
		break;
	case AE_NIGHT:
		sprintf((SINT8*)SmpGetString(MENU_TEST8), "%s", "Night");
		break;
	default:
		break;
	}
	//SmpSetTimer(0xff);
	//SmpDisableTimer();
}

void SmpTest1(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST15,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST2,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				SmpTest1Call},
		{ MENU_KEY_RIGHT,	MENU_TEST1,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST1,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest1Call(void)
{
	SmpSetTimer(SM_DEFAULT_TIMEOUT);
	//SmpEnableTimer();
}

void SmpTest2(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST1,				NULL},
		{ MENU_KEY_DOWN,	MENU_TEST3,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST2,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST2,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest3(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST2,				NULL},
		{ MENU_KEY_DOWN,	MENU_TEST4,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST3,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST3,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest4(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST3,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST5,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST4,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST4,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest5(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST4,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST6,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST5,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST5,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest6(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST5,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST7,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST6,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST6,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest7(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST6,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST8,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST7,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST7,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest8(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST7,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST9,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST8,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST8,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest9(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST8,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST10,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST9,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST9,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest10(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST9,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST11,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST10,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST10,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest11(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST10,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST12,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST11,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST11,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest12(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST11,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST13,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST12,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST12,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest13(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST12,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST14,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST13,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST13,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest14(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST13,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST15,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST14,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST14,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

void SmpTest15(UINT8 key, UINT8* menustring)
{
	UINT8 i;

	const TSmpKeyProc keyProc[] = 
	{
		{ MENU_KEY_UP,		MENU_TEST14,			NULL},
		{ MENU_KEY_DOWN,	MENU_TEST1,				NULL},
		{ MENU_KEY_LEFT,	MENU_TEST,				NULL},
		{ MENU_KEY_RIGHT,	MENU_TEST15,				NULL},
		{ MENU_KEY_SELECT,	MENU_TEST15,				NULL}
	};

	for(i = 0; i < sizeof(keyProc)/sizeof(TSmpKeyProc); i++)
	{
		if(key == keyProc[i].key)
		{
			SmpSetIndex(keyProc[i].index);
			if(keyProc[i].pcall)
				keyProc[i].pcall();
		}
	}
	menustring = menustring;		//just for avoid warning
}

#endif //V578CMD_DISABLE
