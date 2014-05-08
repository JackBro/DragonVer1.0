#ifndef _CMPROP_IMGADPCM_INTERNAL_H_
#define _CMPROP_IMGADPCM_INTERNAL_H_

#ifdef _WINDLL
	#define CMBOXIMGADPCM_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "BaseClass.lib")
#pragma comment(lib, "toolchar.lib")
#pragma comment(lib, "CmAlgGates.lib")
#pragma comment(lib, "CmBoxGates.lib")
#pragma comment(lib, "CmBoxImgAdpcm.lib")
#pragma comment(lib, "CmodelBase.lib")
#pragma comment(lib, "CmodelBoxData.lib")

#include "stdafx.h"
#include "resource.h"

#include "toolchar/stringtools.h"

#include "CmBoxImgAdpcm/predictBox.h"
#include "CmBoxGates/addgate.h"

#include "CmPropImgAdpcm/predictPropDlg.h"


#endif

