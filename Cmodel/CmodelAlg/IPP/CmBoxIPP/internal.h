
#ifndef _CMODEL_IPPBOX_INTERNAL_H_
#define _CMODEL_IPPBOX_INTERNAL_H_

#ifdef _WINDLL
#define CMODELIPPBOX_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "CmodelBase.lib")
#pragma comment(lib, "CmodelBoxData.lib")
#pragma comment(lib, "CmAlgIPP.lib")

#include "CModelBoxData/BoxData.h"

#include "CmBoxIPP/hsizerbox.h"
#include "CmBoxIPP/vsizerbox.h"


#endif
