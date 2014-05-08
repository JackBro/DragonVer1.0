#ifndef _TOOLMPEG2I_INTERNAL_H_
#define _TOOLMPEG2I_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLMPEG2I_DLL  __declspec(dllexport)
#endif


#pragma comment(lib,"toolfile.lib")
#pragma comment(lib,"toolbit.lib")
#pragma comment(lib,"toolbuf.lib")
#pragma comment(lib,"tooljpg.lib")

#include "tools/global.h"

#include "toolmpeg2i/mpeg2base.h"
#include "toolmpeg2i/mpeg2decbase.h"
#include "toolmpeg2i/mpeg2idec.h"
#include "toolmpeg2i/mpeg2i.h"
#include "toolmpeg2i/mpeg2idec.h"
#include "toolmpeg2i/mpeg2ienc.h"
#include "toolmpeg2i/mpeg2if.h"
#include "toolmpeg2i/mpeg2table.h"

#endif
