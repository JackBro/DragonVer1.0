


#ifndef _RDK_CAMERA_H_
#define _RDK_CAMERA_H_

#ifdef __cplusplus
extern "C" {
#endif

enum CAMERASTATEenum {
	CAMERA_NOTINITED,
	CAMERA_INITED,
	CAMERA_OPEN,
	CAMERA_CLOSE
};

enum AEMODEenum
{
	AE_AUTO = 0x0,
	AE_OUTDOOR,
	AE_INDOOR,
	AE_NIGHT
};

typedef struct tag_TCameraContext {
	UINT8 state;
	TPoint disppt;
	TSize dispsize;
	UINT8 rotate;
} TCameraContext, *PTCameraContext;


void V5_CameraOpen(void);
TCameraContext * V5_CameraGetContext(void);

void V5_CameraOn(UINT8 index);
void V5_CameraOff(void);

void V5_CameraFullScreen(void);
void V5_CameraWinView(void);
void V5_CameraRotate(UINT8 rotate);
void V5_CameraZoomIn(UINT16 step);
void V5_CameraZoomOut(UINT16 step);
void V5_CameraRotate(UINT8 rotate);

void V5_SetExposeMode(UINT8 mode);

void V5_CameraOnForSnrTest(UINT8 index);
//================================
//=========================


//===================================

#ifdef __cplusplus
}
#endif

#endif 


