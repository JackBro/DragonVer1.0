#ifndef _CMODEL_STILLALG_INTERNAL_H_
#define _CMODEL_STILLALG_INTERNAL_H_

#ifdef _WINDLL
	#define CMODELSTALG_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "toolmath.lib")

#include "tools/typeprop.h"
#include "CModelBoxData/BoxData.h"
#include "toolmath/yuv2rgb.h"

#include "CmAlgStill/bmp2rgbAlg.h"
#include "CmAlgStill/rgb2yuvAlg.h"
#include "CmAlgStill/yuv2yuvAlg.h"
#include "CmAlgStill/yuv2rgbAlg.h"
#include "CmAlgStill/yuvdivAlg.h"
#include "CmAlgStill/yuvmergeAlg.h"
#include "CmAlgStill/fmtconvertAlg.h"


#endif
