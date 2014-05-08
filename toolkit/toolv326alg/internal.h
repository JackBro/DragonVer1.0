#ifndef _TOOLV326ALG_INTERNAL_H_
#define _TOOLV326ALG_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLV326ALG_DLL  __declspec(dllexport)
#endif


#pragma comment(lib,"tools.lib")
#pragma comment(lib,"toolbuf.lib")
#pragma comment(lib,"toolfile.lib")
#pragma comment(lib,"toolconvert.lib")
#pragma comment(lib,"toolmath.lib")


#include "toolfile/filebin.h"

#include "toolmath/baseconvert.h"

#include "toolconvert/yuv444.h"
#include "toolconvert/yuv422.h"

#include "toolv326alg/ge.h"
#include "toolv326alg/isp.h"
#include "toolv326alg/ipp.h"
#include "toolv326alg/sif.h"
#include "toolv326alg/lcdc.h"

#endif
