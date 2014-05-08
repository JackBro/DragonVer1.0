#ifndef _CHANNELUSB_INTERNAL_H_
#define _CHANNELUSB_INTERNAL_H_


#ifdef _WINDLL
	#define CHANNELUSB_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "ChannelUartUsbBase.lib")
#pragma comment(lib, "toolbuf.lib")

#include "tools/global.h"

#include "UartUsbBaseChannel/UartUsbBaseChannel.h"

#include "../../terminal/VCMobi/mobiusr.h"

#include "usbchannel/usbchannel.h"

#endif
