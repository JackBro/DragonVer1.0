#ifndef _CMODELDPDDPC_INTERNAL_H_
#define _CMODELDPDDPC_INTERNAL_H_

#ifdef _WINDLL
	#define CMODELISP_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "tools.lib")
#pragma comment(lib, "CmAlgIsp.lib")
#pragma comment(lib, "BaseClass.lib")
#pragma comment(lib, "CmodelBase.lib")
#pragma comment(lib, "CmodelBoxData.lib")


#include "CModelBoxData/BoxData.h"

#include "CmBoxIsp/CfaExtract.h"
#include "CmBoxIsp/DigitalGain.h"
#include "CmBoxIsp/DpdDpc.h"
#include "CmBoxIsp/lensfalloff.h"
#include "CmBoxIsp/ColorConvert.h"
#include "CmBoxIsp/ColorCorrect.h"
#include "CmBoxIsp/EdgeEnhance.h"
#include "CmBoxIsp/GammaCorrect.h"



#endif
