#include "../../main/option.h"
#ifndef USBCMD_DISABLE

#include "../../v568/tools/type.h"
#include "../driver/driver.h"
#include "../../s3c44box/s3c44box.h"
#include "../../s3c44box/comm.h"

#include "usbapi.h"


void InitUsbEnv(void)
{
	BusInit(BUS_IDX_3, BUS8);
	if(0x02 != Usb_GetRevision())	// check usb chip exist or not
		return;
	InitUsbContext();
	SetPortCall(0, _ISR_UsbHandle);
	ExtInt_Init(EXINT0);
}

void OpenUsb(void)
{
	InitUsb9604();
	EnableExInt(EXINT0);
	Delay(100);
	tmSetCallback(CHANNEL_USB9604, UsbPipe2ReadBuf,  F_READ);
	tmSetCallback(CHANNEL_USB9604, UsbPipe2WriteBuf,  F_WRITE);
}

void CloseUsb(void)
{
	DisableExInt(EXINT0);
	Usb_DisableNodeAttached();
}


#else
void nullUsbenv(void)
{
}

#endif
