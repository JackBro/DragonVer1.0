#include "../../main/option.h"
#ifndef USBCMD_DISABLE

#include "../../v568/tools/type.h"
#include "../driver/driver.h"

#include "usbapi.h"

////////////////////////////////////////////////////////////////////////////////////////
// Main body

int UsbPipe0WriteBuf(char *pt, int len)
{
	return (int)UWrTxBuf(0, (UINT8*)pt, (UINT32)len);
}

int UsbPipe0ReadBuf(char *pt, int len)
{
	return (int)URdRxBuf(0, (UINT8*)pt, (UINT32)len);
}

int UsbPipe1WriteBuf(char *pt, int len)
{
	return (int)UWrTxBuf(1, (UINT8*)pt, (UINT32)len);
}

int UsbPipe1ReadBuf(char *pt, int len)
{
	return (int)URdRxBuf(1, (UINT8*)pt, (UINT32)len);
}

int UsbPipe2WriteBuf(char *pt, int len)
{
	return (int)UWrTxBuf(2, (UINT8*)pt, (UINT32)len);
}

int UsbPipe2ReadBuf(char *pt, int len)
{
	return (int)URdRxBuf(2, (UINT8*)pt, (UINT32)len);
}

int UsbPipe3WriteBuf(char *pt, int len)
{
	return (int)UWrTxBuf(3, (UINT8*)pt, (UINT32)len);
}

int UsbPipe3ReadBuf(char *pt, int len)
{
	return (int)URdRxBuf(3, (UINT8*)pt, (UINT32)len);
}


#endif
