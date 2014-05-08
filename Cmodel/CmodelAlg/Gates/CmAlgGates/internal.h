#ifndef _CMALGGATES_INTERNAL_H_
#define _CMALGGATES_INTERNAL_H_

#ifdef _WINDLL
	#define CMALGGATES_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolbuf.lib")

#include "tools/typeprop.h"

#include "CmAlgGates/and1Alg.h"
#include "CmAlgGates/com1Alg.h"
#include "CmAlgGates/delay1Alg.h"
#include "CmAlgGates/not1Alg.h"
#include "CmAlgGates/or1Alg.h"
#include "CmAlgGates/xor1Alg.h"
#include "CmAlgGates/add1Alg.h"
#include "CmAlgGates/sub1Alg.h"
#include "CmAlgGates/add2Alg.h"
#include "CmAlgGates/arithm1Alg.h"


#endif