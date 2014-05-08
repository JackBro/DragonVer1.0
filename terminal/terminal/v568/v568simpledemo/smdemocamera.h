#ifndef _SMDEMOCAMERA_H_
#define _SMDEMOCAMERA_H_

#ifdef __cplusplus
extern "C" {
#endif


#define BRIGHT_CONTRAST_STEP	2
#define ZOOM_STEP				16

#define SMP_MAX_DISPLAY_X		511
#define SMP_MAX_DISPLAY_Y		511

typedef struct tag_TSmpSize
{
	TSize imgSize;
	TSize winSize;
	TSize capSize;
	TSize dispSize;
	TSize thumbSize;
	TPoint pt;
	TSize capStill;
	TSize capVideo;
	TSize capThumbSize;
} TSmpSize, *PTSmpSize;

typedef struct tag_TSmpCamera
{
	UINT8 smpZoomMode;
	UINT8 smpBright;
	UINT8 smpContrast;
	UINT8 smpAeMode;
	TSmpSize aProty;
} TSmpCamera, *PTSmpCamera;

void SmpCameraOpen(void);
void SmpCamera(UINT8 key, UINT8* menustring);
void SmpSetting(UINT8 key, UINT8* menustring);
void SmpExit(UINT8 key, UINT8* menustring);
void SmpCameraAe(UINT8 key, UINT8* menustring);
void SmpCameraBrightness(UINT8 key, UINT8* menustring);
void SmpCameraContrast(UINT8 key, UINT8* menustring);
void SmpCameraZoom(UINT8 key, UINT8* menustring);
void SmpCameraSpecialEffect(UINT8 key, UINT8* menustring);
void SmpCameraSpecialEffectNormal(UINT8 key, UINT8* menustring);
void SmpCameraSpecialEffectMono(UINT8 key, UINT8* menustring);
void SmpCameraSpecialEffectSephia(UINT8 key, UINT8* menustring);
void SmpCameraSpecialEffectSpecialColor(UINT8 key, UINT8* menustring);
void SmpCameraSpecialEffectNegative(UINT8 key, UINT8* menustring);
void SmpCameraSpecialEffectSketch(UINT8 key, UINT8* menustring);
void SmpCameraSpecialEffectRelief(UINT8 key, UINT8* menustring);
void SmpCameraSpecialEffectReturn(UINT8 key, UINT8* menustring);
void SmpCameraSpecialEffectExit(UINT8 key, UINT8* menustring);
void SmpCameraImageSize(UINT8 key, UINT8* menustring);
void SmpCameraSetSXGA(UINT8 key, UINT8* menustring);
void SmpCameraSetVGA(UINT8 key, UINT8* menustring);
void SmpCameraSetQVGA(UINT8 key, UINT8* menustring);
void SmpCameraSetQQVGA(UINT8 key, UINT8* menustring);
void SmpCameraSetSizeReturn(UINT8 key, UINT8* menustring);
void SmpCameraSetSizeExit(UINT8 key, UINT8* menustring);
void SmpCameraReturn(UINT8 key, UINT8* menustring);
void SmpCameraExit(UINT8 key, UINT8* menustring);
void SmpCameraSettingZoomMode(UINT8 key, UINT8* menustring);
void SmpCameraSettingEnv(UINT8 key, UINT8* menustring);
void SmpSettingCaptureStill(UINT8 key, UINT8* menustring);
void SmpSettingCaptureVideo(UINT8 key, UINT8* menustring);
void SmpSettingReturn(UINT8 key, UINT8* menustring);
void SmpSettingExit(UINT8 key, UINT8* menustring);
void SmpCameraSettingForeMode(UINT8 key, UINT8* menustring);
void SmpCameraSettingBackMode(UINT8 key, UINT8* menustring);
void SmpCameraSettingZoomModeReturn(UINT8 key, UINT8* menustring);
void SmpCameraSettingZoomModeExit(UINT8 key, UINT8* menustring);
void SmpCameraSettingEnvAuto(UINT8 key, UINT8* menustring);
void SmpCameraSettingEnvIndoor(UINT8 key, UINT8* menustring);
void SmpCameraSettingEnvOutdoor(UINT8 key, UINT8* menustring);
void SmpCameraSettingEnvDark(UINT8 key, UINT8* menustring);
void SmpCameraSettingEnvReturn(UINT8 key, UINT8* menustring);
void SmpCameraSettingEnvExit(UINT8 key, UINT8* menustring);
void SmpSettingCaptureStillSXGA(UINT8 key, UINT8* menustring);
void SmpSettingCaptureStillVGA(UINT8 key, UINT8* menustring);
void SmpSettingCaptureStill760960(UINT8 key, UINT8* menustring);
void SmpSettingCaptureStill384480(UINT8 key, UINT8* menustring);
void SmpSettingCaptureStillReturn(UINT8 key, UINT8* menustring);
void SmpSettingCaptureStillExit(UINT8 key, UINT8* menustring);
void SmpSettingCaptureVideoQVGA(UINT8 key, UINT8* menustring);
void SmpSettingCaptureVideo12896(UINT8 key, UINT8* menustring);
void SmpSettingCaptureVideo128160(UINT8 key, UINT8* menustring);
void SmpSettingCaptureVideoReturn(UINT8 key, UINT8* menustring);
void SmpSettingCaptureVideoExit(UINT8 key, UINT8* menustring);
//Test
void SmpTest(UINT8 key, UINT8* menustring);
void SmpTest1(UINT8 key, UINT8* menustring);
void SmpTest2(UINT8 key, UINT8* menustring);
void SmpTest3(UINT8 key, UINT8* menustring);
void SmpTest4(UINT8 key, UINT8* menustring);
void SmpTest5(UINT8 key, UINT8* menustring);
void SmpTest6(UINT8 key, UINT8* menustring);
void SmpTest7(UINT8 key, UINT8* menustring);
void SmpTest8(UINT8 key, UINT8* menustring);
void SmpTest9(UINT8 key, UINT8* menustring);
void SmpTest10(UINT8 key, UINT8* menustring);
void SmpTest11(UINT8 key, UINT8* menustring);
void SmpTest12(UINT8 key, UINT8* menustring);
void SmpTest13(UINT8 key, UINT8* menustring);
void SmpTest14(UINT8 key, UINT8* menustring);
void SmpTest15(UINT8 key, UINT8* menustring);


void SmpCameraSetCurSize(void);
PTSmpSize SmpCameraGetCurSize(void);
void SmpSetCaptureStill(void);
void SmpSetCaptureVideo(void);
void SmpSetCaptureFrame(void);

#ifdef __cplusplus
}
#endif

#endif
