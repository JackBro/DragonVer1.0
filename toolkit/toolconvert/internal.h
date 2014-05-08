#ifndef _TOOLCONVERT_INTERNAL_H_
#define _TOOLCONVERT_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLCONVERT_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolchar.lib")
#pragma comment(lib, "toolfile.lib")
#pragma comment(lib, "toolbit.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "toolmath.lib")
#pragma comment(lib, "tooljpg.lib")
#pragma comment(lib, "toolmpeg2i.lib")
#pragma comment(lib, "toolmpeg4.lib")


#include "toolbit/bittools.h"
#include "toolbit/bitfifo.h"

#include "tooljpg/jpegenc.h"

#include "toolmpeg2i/mpeg2ienc.h"

#include "toolconvert/normaltools.h"
#include "toolconvert/linebayerbuf.h"
#include "toolconvert/line400buf.h"
#include "toolconvert/line411buf.h"
#include "toolconvert/line422buf.h"
#include "toolconvert/line444buf.h"
#include "toolconvert/bayer5x5rgb.h"
#include "toolconvert/filter4x4base.h"
#include "toolconvert/filter4x4.h"
#include "toolconvert/raw2rgb.h"
#include "toolconvert/raw2rgbf.h"
#include "toolconvert/yuv444.h"
#include "toolconvert/yuv400.h"
#include "toolconvert/yuv411.h"
#include "toolconvert/yuv420.h"
#include "toolconvert/yuv422.h"
#include "toolconvert/rgb24convert.h"
#include "toolconvert/rgb2yiq.h"
#include "toolconvert/yiq2rgb.h"
#include "toolconvert/rgb2yiqf.h"
#include "toolconvert/yiq2rgbf.h"
#include "toolconvert/yuvblkbuf.h"
#include "toolconvert/yuv400f.h"
#include "toolconvert/yuv411f.h"
#include "toolconvert/yuv420f.h"
#include "toolconvert/yuv422f.h"
#include "toolconvert/yuv444f.h"
#include "toolconvert/generaltools.h"


#endif
