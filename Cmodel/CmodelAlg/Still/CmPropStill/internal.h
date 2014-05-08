#ifndef _CMODEL_STILLPROP_INTERNAL_H_
#define _CMODEL_STILLPROP_INTERNAL_H_

#ifdef _WINDLL
	#define CMODELSTPROP_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "BaseClass.lib")
#pragma comment(lib, "CmodelBoxData.lib")
#pragma comment(lib, "CmodelBase.lib")
#pragma comment(lib, "CmBoxStill.lib")
#pragma comment(lib, "tools.lib")

#include "stdafx.h"
#include "resource.h"

#include "tools/typeprop.h"

#include "CModelBoxData/BoxData.h"

#include "CmBoxStill/yuv2yuvbox.h"
#include "CmBoxStill/fmtconvertbox.h"

#include "CmPropStill/yuv2yuvPropDlg.h"
#include "CmPropStill/rgb2yuvPropDlg.h"
#include "CmPropStill/BmpboxPropDlg.h"
#include "CmPropStill/yuv2rgbPropDlg.h"
#include "CmPropStill/yuvdivPropDlg.h"
#include "CmPropStill/yuvmergePropDlg.h"
#include "CmPropStill/fmtconvertPropDlg.h"


#endif
