#ifndef _CMODELCONTAINER_INTERNAL_H_
#define _CMODELCONTAINER_INTERNAL_H_

#ifdef _WINDLL
	#define CMODELCONTAINER_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "tooltree.lib")

#pragma comment(lib, "CmodelBase.lib")
//#pragma comment(lib, "CmodelDigitalGain.lib")

//#pragma comment(lib, "CmodelAutoFocus.lib")
//#pragma comment(lib, "CmodelBlackLevel.lib")
//#pragma comment(lib, "CmodelCfaExtract.lib")
//#pragma comment(lib, "CmodelColorConvert.lib")
//#pragma comment(lib, "CmodelColorCorrect.lib")


//#pragma comment(lib, "CmodelDpdDpc.lib")
//#pragma comment(lib, "CmodelEdgeEnhance.lib")
//#pragma comment(lib, "CmodelGammaCorrect.lib")
//#pragma comment(lib, "CmodelTestPattern.lib")

#pragma comment(lib, "CmBoxIsp.lib")
#pragma comment(lib, "CmPropIsp.lib")

#pragma comment(lib, "CmBoxGates.lib")
#pragma comment(lib, "CmPropGates.lib")

#pragma comment(lib, "CmBoxIPP.lib")
#pragma comment(lib, "CmPropIPP.lib")

#pragma comment(lib, "CmBoxStill.lib")
#pragma comment(lib, "CmPropStill.lib")


#include "stdafx.h"
#include "BaseClass/dlgres.h"

#include "CModelBoxData/BoxData.h"


#include "CmodelContainer/propcontainer.h"
#include "CmodelContainer/unitcontainer.h"
#include "CmodelContainer/mapgov.h"

#endif

