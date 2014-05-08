#ifndef _TOOLMPEG4_INTERNAL_H_
#define _TOOLMPEG4_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLMPEG4_DLL  __declspec(dllexport)
#endif

#pragma comment(lib,"tooljpg.lib")
#pragma comment(lib,"toolfile.lib")
#pragma comment(lib,"toolbit.lib")
#pragma comment(lib,"toolbuf.lib")

#include "tools/global.h"

#include "toolmpeg2i/mpeg2table.h"

#include "toolmpeg4/mpeg4base.h"
#include "toolmpeg4/mpeg4table.h"
#include "toolmpeg4/mpeg4decbase.h"
#include "toolmpeg4/mpeg4dec.h"
#include "toolmpeg4/mpeg4.h"
#include "toolmpeg4/mpeg4f.h"

#endif
