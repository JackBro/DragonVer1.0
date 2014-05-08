#ifndef _CMODELDPDDPC_INTERNAL_H_
#define _CMODELDPDDPC_INTERNAL_H_

#ifdef _WINDLL
	#define CMODELISPBOXPROP_DLL  __declspec(dllexport)
#endif


#pragma comment(lib, "CmBoxIsp.lib")
#pragma comment(lib, "BaseClass.lib")
#pragma comment(lib, "CmodelBase.lib")

#include "stdafx.h"
#include "resource.h"

#include "CModelBoxData/BoxData.h"

#include "CmBoxIsp/CfaExtract.h"
#include "CmBoxIsp/DigitalGain.h"
#include "CmBoxIsp/DpdDpc.h"
#include "CmBoxIsp/lensfalloff.h"
#include "CmBoxIsp/ColorCorrect.h"


#include "CmPropIsp/CfaExtractPropDlg.h"
#include "CmPropIsp/DigitalGainBoxPropDlg.h"
#include "CmPropIsp/DpdDpcBoxPropDlg.h"
#include "CmPropIsp/LensFallOffBoxPropDlg.h"
#include "CmPropIsp/ColorConvertPropDlg.h"
#include "CmPropIsp/ColorCorrectPropDlg.h"
#include "CmPropIsp/EdgeEnhancePropDlg.h"
#include "CmPropIsp/GammaCorrectPropDlg.h"




#endif
