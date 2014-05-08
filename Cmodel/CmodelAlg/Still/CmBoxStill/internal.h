#ifndef _CMODEL_STILLBOX_INTERNAL_H_
#define _CMODEL_STILLBOX_INTERNAL_H_

#ifdef _WINDLL
	#define CMODELSTBOX_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "CmodelBase.lib")
#pragma comment(lib, "CmodelBoxData.lib")
#pragma comment(lib, "CmAlgStill.lib")

#include "CModelBoxData/BoxData.h"

#include "CmBoxStill/bmp2rgbbox.h"
#include "CmBoxStill/rgb2yuvbox.h"
#include "CmBoxStill/yuv2yuvbox.h"
#include "CmBoxStill/yuv2rgbbox.h"
#include "CmBoxStill/yuvdivbox.h"
#include "CmBoxStill/yuvmergebox.h"
#include "CmBoxStill/fmtconvertbox.h"


#endif
