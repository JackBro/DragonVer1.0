#ifndef _TOOLMATH_INTERNAL_H_
#define _TOOLMATH_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLMATH_DLL  __declspec(dllexport)
#endif


#pragma comment(lib,"tools.lib")
#pragma comment(lib,"toolfile.lib")
#pragma comment(lib,"toolbuf.lib")

#include "tools/global.h"

#include "toolbuf/BufferBase.h"

#include "toolfile/filebin.h"

#include "toolmath/baseconvert.h"
#include "toolmath/basemath.h"
#include "toolmath/matrixtool.h"
#include "toolmath/dct.h"
#include "toolmath/yuv2rgb.h"
#include "toolmath/matrixtool.h"

#endif
