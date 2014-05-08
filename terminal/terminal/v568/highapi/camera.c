#include "internal.h"
#ifndef	V578CMD_DISABLE

#include "../driver/basefunc/basefunc.h"
#include "../driver/driver.h"

TCameraContext gCameraContext;

void V5_CameraOpen(void)
{
	gCameraContext.state = CAMERA_OPEN;
	//gCameraContext.disppt.x = 0;
	//gCameraContext.disppt.y = 0;
	//gCameraContext.dispsize.cx = 128;
	//gCameraContext.dispsize.cy = 96;
}

TCameraContext * V5_CameraGetContext(void)
{
	return &gCameraContext;
}

void V5_CameraOn(UINT8 index)
{
	TSize size;
	TPoint pt = {0, 0};
	TALayerProty proty;

	gCameraContext.disppt.x = 0;
	gCameraContext.disppt.y = 0;
	V5_DispGetPanelSize(&gCameraContext.dispsize);
	proty.memsize = gCameraContext.dispsize;
	proty.pos2panel = gCameraContext.disppt;
	proty.win.ltp = gCameraContext.disppt;
	proty.win.size = gCameraContext.dispsize;

	V5_VispGetSize(index, &size);
	V5_DispSetAmemSize(&proty);
	V5_VispSetDispSizer(index, pt, size, proty.memsize);
	V5_VispSetEnv(FREQ_50HZ);
	V5_SetMode(PREVIEW_MODE);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_SifDisableSyncGen();
	V5_VispSetSnrState(SENSOR_POWERON);			//open sensor
	//V5_VispEnableAe();
	V5B_SifEnableSyncGen();
}

void V5_CameraOff(void)
{
	V5B_SifDisableSnrSyncGen();
	V5_VispSetSnrState(SENSOR_POWEROFF);
}

void V5_SetExposeMode(UINT8 mode)
{
	switch(mode)
	{
	case AE_AUTO:
		V5_VispSetMinEt(0);
		V5_VispSetMaxEt(V5_VispGetMaxEtDefault());
		V5_VispSetYTop(V5_VispGetYTopDefault());
		break;
	case AE_OUTDOOR:
		V5_VispSetMinEt(0);
		V5_VispSetMaxEt(20);
		V5_VispSetYTop(V5_VispGetYTarget()+V5_VispGetYThd());
		break;
	case AE_INDOOR:
		V5_VispSetMinEt(1);
		V5_VispSetMaxEt(V5_VispGetMaxEtDefault());
		V5_VispSetYTop(V5_VispGetYTopDefault());
		break;
	case AE_NIGHT:
		V5_VispSetMinEt(5);
		V5_VispSetMaxEt(255);
		//V5_VispSetYTop(0xe0);
		break;
	default:
		break;
	}
}

void V5_CameraOnForSnrTest(UINT8 index)
{
	TSize size;
	TPoint pt = {0, 0};
	TALayerProty proty;

	gCameraContext.disppt.x = 0;
	gCameraContext.disppt.y = 0;
	V5_DispGetPanelSize(&gCameraContext.dispsize);
	proty.memsize = gCameraContext.dispsize;
	proty.pos2panel = gCameraContext.disppt;
	proty.win.ltp = gCameraContext.disppt;
	proty.win.size = gCameraContext.dispsize;

	V5_VispGetSize(index, &size);
	V5_DispSetAmemSize(&proty);
	V5_VispSetDispSizer(index, pt, size, proty.memsize);
	V5_VispSetEnv(FREQ_50HZ);
	V5_SetMode(PREVIEW_MODE);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
}

#endif //V578CMD_DISABLE
