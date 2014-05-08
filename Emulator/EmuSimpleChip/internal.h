#ifndef _EMUSIMPLECHIP_INTERNAL_H_
#define _EMUSIMPLECHIP_INTERNAL_H_

#ifdef _WINDLL
	#define EMUSIMPLECHIP_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "RegList.lib")
#pragma comment(lib, "toolbuf.lib")

#include "tools/global.h"

#include "reglist/reglist.h"

#include "EmuSimpleChip/chiprw.h"


#endif
