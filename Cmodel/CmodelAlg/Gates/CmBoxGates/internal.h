#ifndef _CMBOXGATES_INTERNAL_H_
#define _CMBOXGATES_INTERNAL_H_

#ifdef _WINDLL
	#define CMBOXGATES_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "CmodelBase.lib")
#pragma comment(lib, "CmodelBoxData.lib")
#pragma comment(lib, "CmAlgGates.lib")


#include "CModelBoxData/BoxData.h"

#include "CmBoxGates/andgate.h"
#include "CmBoxGates/xorgate.h"
#include "CmBoxGates/comgate.h"
#include "CmBoxGates/delaygate.h"
#include "CmBoxGates/notgate.h"
#include "CmBoxGates/orgate.h"
#include "CmBoxGates/addgate.h"
#include "CmBoxGates/subgate.h"
#include "CmBoxGates/addgate2.h"
#include "CmBoxGates/arithmgate.h"

#endif