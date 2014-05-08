#ifndef _CMPROPGATES_INTERNAL_H_
#define _CMPROPGATES_INTERNAL_H_

#ifdef _WINDLL
	#define CMPROPGATES_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "BaseClass.lib")
#pragma comment(lib, "toolchar.lib")
#pragma comment(lib, "CmBoxGates.lib")
#pragma comment(lib, "CmodelBase.lib")
#pragma comment(lib, "CmodelBoxData.lib")

#include "stdafx.h"
#include "resource.h"

#include "toolchar/stringtools.h"

#include "CModelBoxData/BoxData.h"

#include "CmBoxGates/addgate.h"
#include "CmBoxGates/addgate2.h"
#include "CmBoxGates/delaygate.h"
#include "CmBoxGates/arithmgate.h"
#include "CmPropGates/XorGatePropDlg.h"
#include "CmPropGates/AndGatePropDlg.h"
#include "CmPropGates/ComGatePropDlg.h"
#include "CmPropGates/DelayGatePropDlg.h"
#include "CmPropGates/NotGatePropDlg.h"
#include "CmPropGates/OrGatePropDlg.h"
#include "CmPropGates/AddGatePropDlg.h"
#include "CmPropGates/SubGatePropDlg.h"
#include "CmPropGates/add2gatepropdlg.h"
#include "CmPropGates/ArithmGatePropDlg.h"


#endif