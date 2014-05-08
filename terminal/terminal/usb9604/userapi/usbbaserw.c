#include "../../main/option.h"
#ifndef USBCMD_DISABLE

#include "../../v568/tools/type.h"
#include "../../s3c44box/s3c44box.h"
#include "usbbaserw.h"

#ifdef	WIN32

void USB_SET_ADDR(UINT8 val)
{
	val = val;
}

void USB_SET_DATA(UINT8 val)
{
	val = val;
}

UINT8 USB_GET_DATA(void)
{
	return 0;
}

#else


#endif


#endif

