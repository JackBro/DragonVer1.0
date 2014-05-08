#include "../driver/v5api.h"
#include "../highapi/highapi.h"
#include "../../s3c44box/s3c44box.h"

#include "./guidemo/guidemo.h"
#include "v568phone.h"


//	extern char *gTransferBuf;

#ifdef WIN32
static void DispDesktop(void)
{
	FILE *fp = NULL;

	fp = fopen("desk.bmp", "rb");
	if(fp == NULL)
		return;
	fread(gTransferBuf, 1, 0x10000, fp);
	V5_DisplayB0Bmp((UINT8 *)gTransferBuf);
	fclose(fp);
}

static void DispAntennaState(void)
{
	FILE *fp = NULL;

	fp = fopen("desk.bmp", "rb");
	if(fp == NULL)
		return;
	fread(gTransferBuf, 1, 0x10000, fp);
	fclose(fp);
}

static void DispPhoneState(void)
{
}

static void DispPowerState(void)
{
}

static void DispMessageState(void)
{
}

void PhonePoweron(void)
{
	V5_Open();
	V5_SetMode(DIRECTDISPLAY_MODE);
	V5_DispSelectPlane0();
	DispDesktop();
	DispAntennaState();
	DispPhoneState();
	DispMessageState();
	DispPowerState();
}

#else

void PhonePoweron(void)
{
}

#endif
