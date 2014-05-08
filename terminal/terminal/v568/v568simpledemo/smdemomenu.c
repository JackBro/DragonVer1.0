
#include "internal.h"
#ifndef	V578CMD_DISABLE

#include "../driver/driver.h"

#ifndef WIN32
#define SmPrintf(X)
#endif

static void SmpInitMenuContext(void);
static void SmpMenuOpen(void);

static UINT8 SmpFindFirstItem(void);
static UINT8 SmpFindLastItem(void);
static SINT32 SmpGetCurItemPosition(void);
//static UINT8 SmpGetMenuCount(void);
static UINT8 SmpGetMaxMenuCount(void);
static UINT8 SmpIsSamePage(UINT8 preindex, UINT8 curindex);
static UINT8 SmpIsSameBranch(UINT8 preindex, UINT8 curindex);

static void SmpDisplayMenu(void);
static void SmpDisplayMenuItem(UINT8 position, UINT8* menustr);
static void SmpDisplayHighlightMenuItem(UINT8 position, UINT8* menustr);
static void SmpDisplayUnHighlightMenuItem(UINT8 position, UINT8* menustr);

//static void SmpOnMenuExit(UINT8 key, UINT8* menustring);
static void SmpMenuTimerCall(void);
static void SmpKeyPressBeep(void);

static TSmpMenuNode gSmpMenuTree[] =
{
	//up,				down,			menustr,		call
	{ MENU_NULL,		MENU_NULL,		"",				NULL},				//MENU_NULL

	//main
	{ MENU_CAMERA,		MENU_DISP,		"Camera",		SmpCamera},			//MENU_CAMERA
	{ MENU_CAMERA,		MENU_JPEG,		"Disp",			SmpDisp},			//MENU_DISP
	{ MENU_DISP,		MENU_SETTING,	"Jpeg",			SmpJpeg},			//MENU_JPEG
	{ MENU_JPEG,		MENU_TEST,		"Setting",		SmpSetting},		//MENU_SETTING
	{ MENU_SETTING,		MENU_EXIT,		"Test",			SmpTest},			//MENU_TEST
	{ MENU_TEST,		MENU_EXIT,		"Exit",			SmpExit},			//MENU_EXIT

	//JPEG
	{ MENU_JPEG_CAP_STI,	MENU_JPEG_DIS_STI,		"Cap Sti",		SmJpegCapSti},				//MENU_JPEG_CAP_STI,
	{ MENU_JPEG_CAP_STI,	MENU_JPEG_CAP_FRM,		"Dis Sti",		SmJpegDisSti},				//MENU_JPEG_DIS_STI,
	{ MENU_JPEG_DIS_STI,	MENU_JPEG_CAP_VID,		"Cap Frm",		SmJpegCapFrm},				//MENU_JPEG_CAP_FRM,
	{ MENU_JPEG_CAP_FRM,	MENU_JPEG_DIS_VID,		"Cap Vid",		SmJpegCapVid},				//MENU_JPEG_CAP_VID,
	{ MENU_JPEG_CAP_VID,	MENU_JPEG_MULTISHOT,	"Dis Vid",		SmJpegDisVid},				//MENU_JPEG_DIS_VID,
	{ MENU_JPEG_DIS_VID,	MENU_JPEG_ENC,			"Mul",			SmJpegMulShot},				//MENU_JPEG_MULTISHOT,
	{ MENU_JPEG_MULTISHOT,	MENU_JPEG_DEC,			"Enc",			SmJpegEnc},					//MENU_JPEG_ENC,
	{ MENU_JPEG_ENC,		MENU_JPEG_EXIT,			"Dec",			SmJpegDec},					//MENU_JPEG_DEC,
	{ MENU_JPEG_DEC,		MENU_JPEG_EXIT,			"Exit",			SmJpegExit},				//MENU_JPEG_EXIT,

	//JPEG MULTISHOT
	{ MENU_JPEG_MUL_SEL,	MENU_JPEG_MUL_SEL,		"",				SmJpegMulSel},				//MENU_JPEG_MUL_SEL,

	//JPEG MULTISHOT DISP
	{ MENU_JPEG_MUL_DISP,	MENU_JPEG_MUL_DISP,		"",				SmJpegMulDisp},				//MENU_JPEG_MUL_DISP,

	//Camera
	{ MENU_CAMERA_AUTOEXPOSE,	MENU_CAMERA_BRIGHT,		"Auto Exposure",	SmpCameraAe},			//MENU_CAMERA_AUTOEXPOSE
	{ MENU_CAMERA_AUTOEXPOSE,	MENU_CAMERA_CONTRAST,	"Brightness",		SmpCameraBrightness},	//MENU_CAMERA_BRIGHT
	{ MENU_CAMERA_BRIGHT,		MENU_CAMERA_ZOOM,		"Contrast",			SmpCameraContrast},		//MENU_CAMERA_CONTRAST
	{ MENU_CAMERA_CONTRAST,		MENU_CAMERA_EFFECT,		"Zoom in/out",		SmpCameraZoom},			//MENU_CAMERA_ZOOM
	{ MENU_CAMERA_ZOOM,			MENU_CAMERA_SIZE,		"Special Effect",	SmpCameraSpecialEffect},					//MENU_CAMERA_EFFECT
	{ MENU_CAMERA_EFFECT,		MENU_CAMERA_RETURN,		"Image Size",		SmpCameraImageSize},					//MENU_CAMERA_SIZE
	{ MENU_CAMERA_SIZE,			MENU_CAMERA_EXIT,		"Return",			SmpCameraReturn},					//MENU_CAMERA_RETURN
	{ MENU_CAMERA_RETURN,		MENU_CAMERA_EXIT,		"Exit",				SmpCameraExit},			//MENU_CAMERA_EXIT

	//Special Effect
	{ MENU_CAMERA_EFFECT_NORMAL,	MENU_CAMERA_EFFECT_MONO,		"Normal",		SmpCameraSpecialEffectNormal},			//MENU_CAMERA_EFFECT_NORMAL
	{ MENU_CAMERA_EFFECT_NORMAL,	MENU_CAMERA_EFFECT_SEPIA,		"Monochrome",	SmpCameraSpecialEffectMono},			//MENU_CAMERA_EFFECT_MONO
	{ MENU_CAMERA_EFFECT_MONO,		MENU_CAMERA_EFFECT_SPE_COLOR,	"Sepia",		SmpCameraSpecialEffectSephia},			//MENU_CAMERA_EFFECT_SEPIA
	{ MENU_CAMERA_EFFECT_SEPIA,		MENU_CAMERA_EFFECT_NEGA,		"Special Color",SmpCameraSpecialEffectSpecialColor},	//MENU_CAMERA_EFFECT_SPE_COLOR
	{ MENU_CAMERA_EFFECT_SPE_COLOR,	MENU_CAMERA_EFFECT_RELIEF,		"Negative",		SmpCameraSpecialEffectNegative},		//MENU_CAMERA_EFFECT_NEGA
	{ MENU_CAMERA_EFFECT_NEGA,		MENU_CAMERA_EFFECT_SKETCH,		"Relief",		SmpCameraSpecialEffectRelief},			//MENU_CAMERA_EFFECT_RELIEF
	{ MENU_CAMERA_EFFECT_RELIEF,	MENU_CAMERA_EFFECT_RETURN,		"Sketch",		SmpCameraSpecialEffectSketch},			//MENU_CAMERA_EFFECT_SKETCH
	{ MENU_CAMERA_EFFECT_SKETCH,	MENU_CAMERA_EFFECT_EXIT,		"Return",		SmpCameraSpecialEffectReturn},									//MENU_CAMERA_EFFECT_RETURN
	{ MENU_CAMERA_EFFECT_RETURN,	MENU_CAMERA_EFFECT_EXIT,		"Exit",			SmpCameraSpecialEffectExit},							//MENU_CAMERA_EFFECT_EXIT

	//Image Size
	{ MENU_CAMERA_SIZE_SXGA,	MENU_CAMERA_SIZE_VGA,		"1280*960",	SmpCameraSetSXGA},		//MENU_CAMERA_SIZE_SXGA
	{ MENU_CAMERA_SIZE_SXGA,	MENU_CAMERA_SIZE_QVGA,		"640*480",	SmpCameraSetVGA},		//MENU_CAMERA_SIZE_VGA
	{ MENU_CAMERA_SIZE_VGA,		MENU_CAMERA_SIZE_QQVGA,		"320*240",	SmpCameraSetQVGA},		//MENU_CAMERA_SIZE_QVGA
	{ MENU_CAMERA_SIZE_QVGA,	MENU_CAMERA_SIZE_RETURN,	"160*120",	SmpCameraSetQQVGA},		//MENU_CAMERA_SIZE_QQVGA
	{ MENU_CAMERA_SIZE_QQVGA,	MENU_CAMERA_SIZE_EXIT,		"Return",	SmpCameraSetSizeReturn},					//MENU_CAMERA_SIZE_RETURN
	{ MENU_CAMERA_SIZE_RETURN,	MENU_CAMERA_SIZE_EXIT,		"Exit",		SmpCameraSetSizeExit},			//MENU_CAMERA_SIZE_EXIT

	//Disp
	{ MENU_DISP_BLENDLING,	MENU_DISP_OVERLAY,	"Blending",	SmpDispBlend},			//MENU_DISP_BLENDLING
	{ MENU_DISP_BLENDLING,	MENU_DISP_RM,		"Overlay",	SmpDispOverlay},		//MENU_DISP_OVERLAY
	{ MENU_DISP_OVERLAY,	MENU_DISP_LINEDRAW,	"R / M",	SmpDispRM},				//MENU_DISP_RM
	{ MENU_DISP_RM,			MENU_DISP_BITBLT,	"Line Draw",SmpDispLineDraw},		//MENU_DISP_LINEDRAW
	{ MENU_DISP_LINEDRAW,	MENU_DISP_AWIN,		"BitBLT",	SmpDispBitblt},			//MENU_DISP_BITBLT
	{ MENU_DISP_BITBLT,		MENU_DISP_RETURN,	"A Window",	SmpDispAWin},			//MENU_DISP_AWIN
	{ MENU_DISP_AWIN,		MENU_DISP_EXIT,		"Return",	SmpDispReturn},			//MENU_DISP_RETURN
	{ MENU_DISP_RETURN,		MENU_DISP_EXIT,		"Exit",		SmpDispExit},			//MENU_DISP_EXIT

	//Blending
	{ MENU_DISP_BLENDLING_ADD,		MENU_DISP_BLENDLING_DEC,	"+",		SmpDispBldInc},			//MENU_DISP_BLENDLING_ADD
	{ MENU_DISP_BLENDLING_ADD,		MENU_DISP_BLENDLING_RETURN,	"-",		SmpDispBldDec},			//MENU_DISP_BLENDLING_DEC
	{ MENU_DISP_BLENDLING_DEC,		MENU_DISP_BLENDLING_EXIT,	"Return",	SmpDispBldReturn},		//MENU_DISP_BLENDLING_RETURN
	{ MENU_DISP_BLENDLING_RETURN,	MENU_DISP_BLENDLING_EXIT,	"Exit",		SmpDispBldExit},		//MENU_DISP_BLENDLING_EXIT

	//Overlay
	{ MENU_DISP_OVERLAY_TRAN,	MENU_DISP_OVERLAY_AND,		"Transparent"		,	SmpDispOlyTrans},	//MENU_DISP_OVERLAY_TRAN
	{ MENU_DISP_OVERLAY_TRAN,	MENU_DISP_OVERLAY_OR,		"And"				,	SmpDispOlyAnd},		//MENU_DISP_OVERLAY_AND
	{ MENU_DISP_OVERLAY_AND,	MENU_DISP_OVERLAY_INVERT,	"Or"				,	SmpDispOlyOr},		//MENU_DISP_OVERLAY_OR
	{ MENU_DISP_OVERLAY_OR,		MENU_DISP_OVERLAY_RETURN,	"Invert"			,	SmpDispOlyInvert},	//MENU_DISP_OVERLAY_INVERT
	{ MENU_DISP_OVERLAY_INVERT,	MENU_DISP_OVERLAY_EXIT,		"Return"			,	SmpDispOlyReturn},	//MENU_DISP_OVERLAY_RETURN
	{ MENU_DISP_OVERLAY_RETURN,	MENU_DISP_OVERLAY_EXIT,		"Exit"				,	SmpDispOlyExit},	//MENU_DISP_OVERLAY_EXIT

	//SETTING
	{ MENU_SETTING_ZOOM_MODE,	MENU_SETTING_ENV,		"Zoom Mode",	SmpCameraSettingZoomMode},				//MENU_SETTING_ZOOM_MODE
	{ MENU_SETTING_ZOOM_MODE,	MENU_SETTING_CAPSTILL,	"Environment",	SmpCameraSettingEnv},					//MENU_SETTING_ENV
	{ MENU_SETTING_ENV,			MENU_SETTING_CAPVIDEO,	"Capture Still",SmpSettingCaptureStill},				//MENU_SETTTING_CAPSTILL
	{ MENU_SETTING_CAPSTILL,	MENU_SETTING_RETURN,	"Capture Video",SmpSettingCaptureVideo},				//MENU_SETTTING_CAPVIDEO
	{ MENU_SETTING_CAPVIDEO,	MENU_SETTING_EXIT,		"Return",		SmpSettingReturn},					//MENU_SETTING_RETURN
	{ MENU_SETTING_RETURN,		MENU_SETTING_EXIT,		"Exit"	,		SmpSettingExit},			//MENU_SETTING_EXIT

	//SETTING ZOOM MODE
	{ MENU_SETTING_ZOOM_MODE_FORE,	MENU_SETTING_ZOOM_MODE_BACK,	"Fore Sizer",	SmpCameraSettingForeMode},			//MENU_SETTING_ZOOM_MODE_FORE
	{ MENU_SETTING_ZOOM_MODE_FORE,	MENU_SETTING_ZOOM_MODE_RETURN,	"Back Sizer",	SmpCameraSettingBackMode},			//MENU_SETTING_ZOOM_MODE_BACK
	{ MENU_SETTING_ZOOM_MODE_BACK,	MENU_SETTING_ZOOM_MODE_EXIT,	"Return",		SmpCameraSettingZoomModeReturn},								//MENU_SETTING_ZOOM_MODE_RETURN
	{ MENU_SETTING_ZOOM_MODE_RETURN,MENU_SETTING_ZOOM_MODE_EXIT,	"Exit",			SmpCameraSettingZoomModeExit},						//MENU_SETTING_ZOOM_MODE_EXIT

	//Setting Environment
	{ MENU_SETTING_ENV_AUTO,	MENU_SETTING_ENV_INDOOR,	"Auto",			SmpCameraSettingEnvAuto},					//MENU_SETTING_ENV_AUTO
	{ MENU_SETTING_ENV_AUTO,	MENU_SETTING_ENV_OUTDOOR,	"Indoor",		SmpCameraSettingEnvIndoor},					//MENU_SETTING_ENV_INDOOR
	{ MENU_SETTING_ENV_INDOOR,	MENU_SETTING_ENV_DARK,		"Outdoor",		SmpCameraSettingEnvOutdoor},				//MENU_SETTING_ENV_OUTDOOR
	{ MENU_SETTING_ENV_OUTDOOR,	MENU_SETTING_ENV_RETURN,	"Dark",			SmpCameraSettingEnvDark},					//MENU_SETTING_ENV_DARK
	{ MENU_SETTING_ENV_DARK,	MENU_SETTING_ENV_EXIT,		"Return",		SmpCameraSettingEnvReturn},										//MENU_SETTING_ENV_RETURN
	{ MENU_SETTING_ENV_RETURN,	MENU_SETTING_ENV_EXIT,		"Exit",			SmpCameraSettingEnvExit},							//MENU_SETTING_ENV_EXIT

	//Capture Still Setting
	{ MENU_SETTING_CAPSTILL_SXGA,	MENU_SETTING_CAPSTILL_VGA,		"1280*960",		SmpSettingCaptureStillSXGA},				//MENU_SETTING_CAPSTILL_SXGA
	{ MENU_SETTING_CAPSTILL_SXGA,	MENU_SETTING_CAPSTILL_760960,	"640*480",		SmpSettingCaptureStillVGA},					//MENU_SETTING_CAPSTILL_VGA
	{ MENU_SETTING_CAPSTILL_VGA,	MENU_SETTING_CAPSTILL_384480,	"760*960",		SmpSettingCaptureStill760960},				//MENU_SETTING_CAPSTILL_760960
	{ MENU_SETTING_CAPSTILL_760960,	MENU_SETTING_CAPSTILL_RETURN,	"384*480",		SmpSettingCaptureStill384480},				//MENU_SETTING_CAPSTILL_384480
	{ MENU_SETTING_CAPSTILL_384480,	MENU_SETTING_CAPSTILL_EXIT,		"Return",		SmpSettingCaptureStillReturn},				//MENU_SETTING_CAPSTILL_RETURN
	{ MENU_SETTING_CAPSTILL_RETURN,	MENU_SETTING_CAPSTILL_EXIT,		"Exit",			SmpSettingCaptureStillExit},				//MENU_SETTING_CAPSTILL_EXIT

	//Capture Video Setting
	{ MENU_SETTING_CAPVIDEO_QVGA,	MENU_SETTING_CAPVIDEO_12896,	"320*240",		SmpSettingCaptureVideoQVGA},				//MENU_SETTING_CAPVIDEO_QVGA
	{ MENU_SETTING_CAPVIDEO_QVGA,	MENU_SETTING_CAPVIDEO_128160,	"128*96",		SmpSettingCaptureVideo12896},			//MENU_SETTING_CAPVIDEO_12896
	{ MENU_SETTING_CAPVIDEO_12896,	MENU_SETTING_CAPVIDEO_RETURN,	"128*160",		SmpSettingCaptureVideo128160},				//MENU_SETTING_CAPVIDEO_128160
	{ MENU_SETTING_CAPVIDEO_128160,	MENU_SETTING_CAPVIDEO_EXIT,		"Return",		SmpSettingCaptureVideoReturn},				//MENU_SETTING_CAPVIDEO_RETURN
	{ MENU_SETTING_CAPVIDEO_RETURN,	MENU_SETTING_CAPVIDEO_EXIT,		"Exit",			SmpSettingCaptureVideoExit},				//MENU_SETTING_CAPVIDEO_EXIT

	//Test
	{ MENU_TEST1,	MENU_TEST2,		"Test1",			SmpTest1},							//Test scroll menu
	{ MENU_TEST1,	MENU_TEST3,		"Test2",			SmpTest2},							//Test scroll menu
	{ MENU_TEST2,	MENU_TEST4,		"Test3",			SmpTest3},							//Test scroll menu
	{ MENU_TEST3,	MENU_TEST5,		"Test4",			SmpTest4},							//Test scroll menu
	{ MENU_TEST4,	MENU_TEST6,		"Test5",			SmpTest5},							//Test scroll menu
	{ MENU_TEST5,	MENU_TEST7,		"Test6",			SmpTest6},							//Test scroll menu
	{ MENU_TEST6,	MENU_TEST8,		"Test7",			SmpTest7},							//Test scroll menu
	{ MENU_TEST7,	MENU_TEST9,		"Test8",			SmpTest8},							//Test scroll menu
	{ MENU_TEST8,	MENU_TEST10,	"Test9",			SmpTest9},							//Test scroll menu
	{ MENU_TEST9,	MENU_TEST11,	"Test10",			SmpTest10},							//Test scroll menu
	{ MENU_TEST10,	MENU_TEST12,	"Test11",			SmpTest11},							//Test scroll menu
	//{ MENU_TEST11,	MENU_TEST13,	"Test12",			SmpTest12},							//Test scroll menu
	//{ MENU_TEST12,	MENU_TEST14,	"Test13",			SmpTest13},							//Test scroll menu
	//{ MENU_TEST13,	MENU_TEST15,	"Test14",			SmpTest14},							//Test scroll menu
	//{ MENU_TEST14,	MENU_TEST15,	"Test15",			SmpTest15}							//Test scroll menu
	{ MENU_TEST11,	MENU_TEST13,	"Loop",				SmpDispTest12},							//Test scroll menu
	{ MENU_TEST12,	MENU_TEST14,	"B Sel",			SmpDispTest13},							//Test scroll menu
	{ MENU_TEST13,	MENU_TEST15,	"Clear B0",			SmpDispTest14},							//Test scroll menu
	{ MENU_TEST14,	MENU_TEST15,	"Clear B1",			SmpDispTest15}							//Test scroll menu

};

static TSmpMenuContext gSmpMenuContext;

void SmpInitMenuContext(void)
{
	memset(&gSmpMenuContext, 0, sizeof(TSmpMenuContext));
	
	gSmpMenuContext.timerSetting = SM_DEFAULT_TIMEOUT;
	gSmpMenuContext.maxcount = SmpGetMaxMenuCount();
}

static UINT8 SmpFindFirstItem(void)
{
	UINT8 index = gSmpMenuContext.index;
	UINT8 i = 0;

	while(index != gSmpMenuTree[index].up)
		index = gSmpMenuTree[index].up;
	
	for(i = 0; i < gSmpMenuContext.offset; i++)
		index = gSmpMenuTree[index].down;

	return index;
}

static UINT8 SmpFindLastItem(void)
{
	UINT8 index, i = 1;

	index = SmpFindFirstItem();

	while((index != gSmpMenuTree[index].down) && (i < gSmpMenuContext.maxcount))
	{
		index = gSmpMenuTree[index].down;
		i++;
	}

	return index;
}

#if 0
UINT8 SmpGetMenuCount(void)
{
	UINT8 index = gSmpMenuContext.index;
	UINT8 count = 1;

	while(index != gSmpMenuTree[index].up)
	{
		index = gSmpMenuTree[index].up;
		count++;
	}
	index = gSmpMenuContext.index;
	while(index != gSmpMenuTree[index].down)
	{
		index = gSmpMenuTree[index].down;
		count++;
	}

	return count;
}
#endif

static SINT32 SmpGetCurItemPosition(void)
{
	SINT32 position = 0;
	UINT8 index = gSmpMenuContext.index;

	while(index != gSmpMenuTree[index].up)
		index = gSmpMenuTree[index].up;

	position = (SINT32)(position - (SINT32)gSmpMenuContext.offset);
	while(index != gSmpMenuContext.index)
	{
		index = gSmpMenuTree[index].down;
		position++;
	}

	return position;
}

static UINT8 SmpIsSamePage(UINT8 preindex, UINT8 curindex)
{
	UINT8 gIndex = gSmpMenuContext.index;
	UINT8 fIndex, lIndex;
	UINT8 flag = 0;

	gSmpMenuContext.index = preindex;
	fIndex = SmpFindFirstItem();
	lIndex = SmpFindLastItem();

	if(lIndex == curindex)
		flag = 1;
	else
	{
		while(fIndex != lIndex)
		{
			if(fIndex == curindex)
			{
				flag = 1;
				break;
			}
			fIndex = gSmpMenuTree[fIndex].down;
		}
	}
	gSmpMenuContext.index = gIndex;
	return flag;
}

static UINT8 SmpIsSameBranch(UINT8 preindex, UINT8 curindex)
{
	UINT8 index = preindex;
	UINT8 flag = 0;

	while(index != gSmpMenuTree[index].up)
	{
		if(index == curindex)
		{
			flag = 1;
			return flag;
		}
		index = gSmpMenuTree[index].up;
	}
	if(index == curindex)
	{
		flag = 1;
		return flag;
	}

	index = preindex;
	while(index != gSmpMenuTree[index].down)
	{
		index = gSmpMenuTree[index].down;
		if(index == curindex)
		{
			flag = 1;
			return flag;
		}
	}
	if(index == curindex)
	{
		flag = 1;
		return flag;
	}
	return flag;
}

void SmpDisplayMenu(void)
{
	UINT8 i = 0, firstItem, lastItem;
	SINT32 curPosition, prePosition;
	UINT8 index = gSmpMenuContext.index;

	if(gSmpMenuContext.index == MENU_NULL)
	{
		SmpMenuOpen();
		SmpDispRefreshB0();
	}
	
	if((gSmpMenuContext.index == gSmpMenuContext.preindex) && gSmpMenuContext.menuVisual)
		return;
	
	if(gSmpMenuContext.index == MENU_SEL_EXIT)
	{
		SmpExitMenu();
		return;
	}
	
	if(gSmpMenuContext.menuVisual)
	{

		if(SmpIsSamePage(gSmpMenuContext.preindex, gSmpMenuContext.index))
		{
			curPosition = SmpGetCurItemPosition();
			gSmpMenuContext.index = gSmpMenuContext.preindex;
			prePosition = SmpGetCurItemPosition();
			gSmpMenuContext.index = index;

			SmpDisplayHighlightMenuItem((UINT8)curPosition, gSmpMenuTree[gSmpMenuContext.index].menustr);
			SmpDisplayUnHighlightMenuItem((UINT8)prePosition, gSmpMenuTree[gSmpMenuContext.preindex].menustr);
			return;
		}
		else
		{
			if(SmpIsSameBranch(gSmpMenuContext.preindex, gSmpMenuContext.index))
			{
				curPosition = SmpGetCurItemPosition();
				if(curPosition > (SINT32)gSmpMenuContext.maxcount-1)
					gSmpMenuContext.offset = (UINT8)(gSmpMenuContext.offset + curPosition + 1 - gSmpMenuContext.maxcount);
				else if(curPosition < 0)
					gSmpMenuContext.offset = (UINT8)(gSmpMenuContext.offset + curPosition);
				else
					gSmpMenuContext.offset = gSmpMenuContext.offset;
			}
			else
				gSmpMenuContext.offset = 0;
		}
	}

	SmpDispRefreshB0();
	firstItem = SmpFindFirstItem();
	lastItem = SmpFindLastItem();

	while(firstItem != lastItem)
	{
		SmpDisplayMenuItem(i, gSmpMenuTree[firstItem].menustr);
		firstItem = gSmpMenuTree[firstItem].down;
		i++;
	}
	SmpDisplayMenuItem(i, gSmpMenuTree[lastItem].menustr);
	SmpDisplayHighlightMenuItem((UINT8)SmpGetCurItemPosition(), gSmpMenuTree[gSmpMenuContext.index].menustr);
	gSmpMenuContext.menuVisual = 1;
	SmpEnableTimer();
}

void SmpDisplayMenuItem(UINT8 position, UINT8* menustr)
{
	TPoint startpt;

	startpt.x = 0;
	startpt.y = position << 4;

	V5_DisplayStr(startpt, menustr, OP_S_COLOR1);
}

void SmpDisplayHighlightMenuItem(UINT8 position, UINT8* menustr)
{
	TPoint startpt;

	startpt.x = 0;
	startpt.y = position << 4;

	V5_DisplayStr(startpt, menustr, OP_S_COLOR2);
}

void SmpDisplayUnHighlightMenuItem(UINT8 position, UINT8* menustr)
{
	SmpDisplayMenuItem(position, menustr);
}

void SmpMenuInit(void)
{
	UINT8 fmt;
	TBLayerProty proty;

	V5_DispGetPanelSize(&proty.memsize);
	proty.pos2panel.x = proty.pos2panel.y = 0;
	proty.win.ltp.x = proty.win.ltp.y = 0;
	proty.win.size = proty.memsize;
	proty.refwin.ltp.x = proty.refwin.ltp.y = 0;
	proty.refwin.size = proty.memsize;

	fmt = V5_DispGetB0format();
	V5_DispSetB1format(fmt);
	V5_DispSetB1Proty(&proty);
	V5_MarbAutoSetMem();

	SmpDispLoadAndDispFrame(SMP_FRAME_NULL);
}

void SmpMenuOpen(void)
{
	SmPrintf("open\n");
	
	V5_Open();
	V5_SetMode(PREVIEWBRC_MODE);

	SmpDispOpen();
	SmpCameraOpen();
	SmJpegOpen();

	SmpInitMenuContext();
	SmpMenuInit();
	
	SetTimerCall(5, SmpMenuTimerCall);
	SmpSetTimer(SM_DEFAULT_TIMEOUT);
	SmpDisableTimer();

#ifndef	NCSC
	EnableExInt(EXINT1);		//open interrupt
#endif

	gSmpMenuContext.index = MENU_CAMERA;		//change cur index to first menu item
}

void SmpSendKey(UINT8 key)
{
	gSmpMenuContext.timerCounter = 0;

	if(gSmpMenuContext.menuVisual)
	{
		if(gSmpMenuTree[gSmpMenuContext.index].pcall)
			gSmpMenuTree[gSmpMenuContext.index].pcall(key, gSmpMenuTree[gSmpMenuContext.index].menustr);
	}
	switch(key)
	{
	case MENU_KEY_UP:
	case MENU_KEY_DOWN:
	case MENU_KEY_LEFT:
	case MENU_KEY_RIGHT:
		SmpDisplayMenu();
		break;
	case MENU_KEY_SELECT:
		SmpDisplayMenu();
		SmpKeyPressBeep();
		break;
	case MENU_KEY_DISAPPEAR:
		SmpDisappearMenu();
		break;
	case MENU_KEY_EXIT:
		SmpExitMenu();
		break;
	case MENU_KEY_RESET:
		SmpResetMenu();
		break;
	default:
		break;
	}
}

//void SmpOnMenuExit(UINT8 key, UINT8* menustring)
//{
//	if((key == MENU_KEY_SELECT) || (key == MENU_KEY_RIGHT))
//		SmpDisappearMenu();
//
//	menustring = menustring;
//}

void SmpMenuTimerCall(void)
{
	gSmpMenuContext.timerCounter++;
	if(gSmpMenuContext.timerCounter >= gSmpMenuContext.timerSetting)
		SmpDisappearMenu();
}

void SmpDisappearMenu(void)
{
	SmpDispRefreshB0();
	gSmpMenuContext.menuVisual = 0;
	gSmpMenuContext.timerCounter = 0;
	SmpDisableTimer();
}

void SmpResetMenu(void)
{
	SmpDisappearMenu();
	gSmpMenuContext.index = MENU_NULL;
}

void SmpExitMenu(void)
{
	SmpDisappearMenu();
	gSmpMenuContext.index = MENU_CAMERA;
}

void SmpSetTimer(UINT8 val)
{
	gSmpMenuContext.timerSetting = val;
}

UINT8 SmpGetTimer(void)
{
	return gSmpMenuContext.timerSetting;
}

void SmpSetIndex(UINT8 index)
{
	gSmpMenuContext.preindex = gSmpMenuContext.index;
	gSmpMenuContext.index = index;
}

void SmpEnableTimer(void)
{
#ifndef NCSC
	SetHostTimer(TIMER5, SMP_TIMER_UINT);
#endif
}

void SmpDisableTimer(void)
{
#ifndef NCSC
	SetHostTimer(TIMER5, 0);
#endif
}

UINT8 SmpGetIndex(void)
{
	return gSmpMenuContext.index;
}

UINT8 SmpGetMaxMenuCount()
{
	TSize panelSize;
	TSize fontSize;

	V5_DispGetPanelSize(&panelSize);
	V5_DispGetCurFontSize(&fontSize);

	return (UINT8)(panelSize.cy / fontSize.cy);		// should get font size
}

void SmpKeyPressBeep(void)
{
#ifndef NCSC
	beep(1);
	V5_Delay(100);
	beep(0);
#endif
}

UINT8*	SmpGetString(UINT8 index)
{
	return gSmpMenuTree[index].menustr;
}

#endif //V578CMD_DISABLE
