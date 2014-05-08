#ifndef _COMCHANNEL_INTERNAL_H_
#define _COMCHANNEL_INTERNAL_H_

#ifdef _WINDLL
	#define COMCHANNEL_DLL  __declspec(dllexport)
#endif


#pragma comment(lib,"tools.lib")
#pragma comment(lib,"toolbuf.lib")
#pragma comment(lib,"toolchar.lib")
#pragma comment(lib,"toolfile.lib")
#pragma comment(lib,"ChannelUart.lib")
#pragma comment(lib,"ChannelUsb.lib")
#pragma comment(lib,"ChannelSimRegSram.lib")


#include "tools/global.h"

#include "toolchar/stringtools.h"


#include "ComChannel/ComChannel.h"
#include "ComChannel/simhost.h"

#endif
