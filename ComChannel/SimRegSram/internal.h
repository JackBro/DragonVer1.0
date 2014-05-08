#ifndef _CHANNELSIMREGSRAM_INTERNAL_H_
#define _CHANNELSIMREGSRAM_INTERNAL_H_

#ifdef _WINDLL
	#define CHANNELSIMREGSRAM_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "EmuSimpleChip.lib")
#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolbuf.lib")

#include "tools/global.h"

#include "EmuSimpleChip/chiprw.h"

#include "../../terminal/terminal/s3c44box/option.h"

#include "SimRegSram/SimRegSram.h"
#include "SimRegSram/shell.h"
#include "SimRegSram/cmdlist.h"



#endif
