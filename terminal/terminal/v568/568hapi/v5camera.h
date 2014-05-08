#ifndef _V5CAMERA_H_
#define _V5CAMERA_H_

#ifdef __cplusplus
extern "C" {
#endif

#define V5M_MAX_BRIGHT_CONTRAST 64		//algorithmic require
#define V5M_MIN_BRIGHT_CONTRAST 1


void V5M_CamSetCamState(UINT8 state);
void V5M_CamSetDefaultValue(void);

UINT32 V5M_CameraGetSizeByIndex(UINT8 index, TSize *size);
UINT32 V5M_CameraOnWithDefault(void);
UINT32 V5M_CameraOn(void);

UINT32 V5M_SetExposeMode(UINT8 mode);
UINT32 V5M_CameraZoom(UINT8 digitalzoom);
UINT32 V5M_CameraZoomOut(void);
UINT32 V5M_CameraZoomIn(void);
UINT32 V5M_CameraSetBrightness(UINT8 value);
UINT32 V5M_CameraBrightnessUp(void);
UINT32 V5M_CameraBrightnessDown(void);
UINT32 V5M_CameraSetContrast(UINT8 value);
UINT32 V5M_CameraContrastUp(void);
UINT32 V5M_CameraContrastDown(void);
UINT8  V5M_FrameIsON(void);


void V5M_FrameON(char *filename);
void V5M_FrameOFF(void);
void V5M_ThumbON(void);
void V5M_ThumbOFF(void);
void V5M_SetSnrReso(UINT8 index);
void V5M_ComputeOriginWinSize(TSize srcsize, TSize dispsize, TSize *winsize, TPoint *winpt);
void V5M_InitWinInfo( TSize winsize);
void V5M_AutoSetCapSizer(void);

#ifdef __cplusplus
}
#endif

#endif