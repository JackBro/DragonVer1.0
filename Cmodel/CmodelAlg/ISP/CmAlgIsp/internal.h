#ifndef _CMODELDPDDPC_INTERNAL_H_
#define _CMODELDPDDPC_INTERNAL_H_

#ifdef _WINDLL
	#define CMODELISPALG_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "toolconvert.lib")
#pragma comment(lib, "toolmath.lib")



#include "toolmath/baseconvert.h"

#include "CmAlgIsp/326DpdDpcAlg.h"
#include "CmAlgIsp/568DigitalGainAlg.h"
#include "CmAlgIsp/568DpdDpcAlg.h"
#include "CmAlgIsp/568LensFallOffAlg.h"
#include "CmAlgIsp/CfaExtract568Alg.h"
#include "CmAlgIsp/568ColorCorrectAlg.h"
#include "CmAlgIsp/568ColorConvertAlg.h"
#include "CmAlgIsp/568GammaCorrectAlg.h"
#include "CmAlgIsp/568EdgeEnhanceAlg.h"

#endif
