#ifndef _TOOLV702ALG_INTERNAL_H_
#define _TOOLV702ALG_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLV702ALG_DLL  __declspec(dllexport)
#endif

#pragma comment(lib,"tools.lib")
#pragma comment(lib,"toolbuf.lib")
#pragma comment(lib,"toolfile.lib")
#pragma comment(lib,"toolconvert.lib")
#pragma comment(lib,"toolmath.lib")


#include "toolmath/baseconvert.h"
#include "toolmath/yuv2rgb.h"

#include "toolv702alg/tvenc.h"
#include "toolv702alg/ipp.h"

#endif
