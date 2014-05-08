#ifndef _TOOLIMGSTAT_INTERNAL_H_
#define _TOOLIMGSTAT_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLIMGSTAT_DLL  __declspec(dllexport)
#endif


#pragma comment(lib,"toolfile.lib")
#pragma comment(lib,"toolbuf.lib")
#pragma comment(lib,"toolmath.lib")


#include "toolimgstat/rgbstat.h"
#include "toolimgstat/rgbstatf.h"
#include "toolimgstat/yuvstat.h"
#include "toolimgstat/yuvstatf.h"
#include "toolimgstat/rgb_yuvstruct.h"

#endif
