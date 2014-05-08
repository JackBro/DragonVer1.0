
#ifndef _CMODEL_IPPALG_INTERNAL_H_
#define _CMODEL_IPPALG_INTERNAL_H_

#ifdef _WINDLL
#define CMODELIPPALG_DLL  __declspec(dllexport)
#endif

#define SIZER_COEFF_FMT		9
#define HOR_COEFF_FMT		9
#define VER_COEFF_FMT		8
#define SIZER_RND			0		// 1

#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "toolconvert.lib")



#include "CmAlgIPP/702VsizerAlg.h"
#include "CmAlgIPP/702HsizerAlg.h"



#endif