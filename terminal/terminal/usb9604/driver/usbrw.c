#include "internal.h"
#ifndef USBCMD_DISABLE

TUsbRW gUsbRW;

#ifdef WIN32

UINT8 Usb_GetAdr(void)
{
	return gUsbRW.adr;
}

void Usb_SetAdr(UINT8 adr)
{
	gUsbRW.adr = adr;
}

void Usb_SetReg(UINT8 adr, UINT8 val)
{
	SimUsb_SetReg(adr, val);
}

UINT8 Usb_GetReg(UINT8 adr)
{
	return SimUsb_GetReg(adr);
}

UINT8 Usb_WriteFifo(UINT8 adr, UINT8 *buf, UINT8 len)
{
	return SimUsb_WriteFifo(adr, buf, len);
}

UINT8 Usb_ReadFifo(UINT8 adr, UINT8 *buf, UINT8 len)
{
	return SimUsb_ReadFifo(adr, buf, len);
}

void Usb_Delay(UINT32 count)
{
	SimHost_Delay(count);
}

#else

UINT8 Usb_GetAdr(void)
{
	return gUsbRW.adr;
}

void Usb_SetAdr(UINT8 adr)
{
	gUsbRW.adr = adr;
	USB_SET_ADDR(adr);
}

void Usb_SetReg(UINT8 adr, UINT8 val)
{
	gUsbRW.adr = adr;
	USB_SET_ADDR(adr);
	USB_SET_DATA(val);
}

UINT8 Usb_GetReg(UINT8 adr)
{
	gUsbRW.adr = adr;
	USB_SET_ADDR(adr);
	return USB_GET_DATA();
}

UINT8 Usb_WriteFifo(UINT8 adr, UINT8 *buf, UINT8 len)
{
	UINT8 len1 = len;

	if(len == 0)
		return 0;
	gUsbRW.adr = adr;
	USB_SET_ADDR(adr);
	while(len--)
	{
		USB_SET_DATA(*(buf++));
	}
	return len1;
}

UINT8 Usb_ReadFifo(UINT8 adr, UINT8 *buf, UINT8 len)
{
	UINT8 len1 = len;

	if(len == 0)
		return 0;
	gUsbRW.adr = adr;
	USB_SET_ADDR(adr);
	while(len--)
	{
		*(buf++) = USB_GET_DATA();
	}
	return len1;
}

void Usb_Delay(UINT32 count)
{
	Delay((int)count);
}

#endif


#else
void nullBaserw(void)
{
}
#endif

