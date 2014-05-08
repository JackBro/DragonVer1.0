#ifndef _CMODELBASE_INTERNAL_H_
#define _CMODELBASE_INTERNAL_H_

#ifdef _WINDLL
	#define CMODELBASE_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "toolfile.lib")
#pragma comment(lib, "toolchar.lib")
#pragma comment(lib, "tooltree.lib")
#pragma comment(lib, "BaseClass.lib")
#pragma comment(lib, "CmodelBoxData.lib")

#include "stdafx.h"

#include "tools/typeprop.h"
#include "toolchar/stringtools.h"

#include "CModelBoxData/BoxData.h"

#include "CmodelBase/pinbase.h"
#include "CmodelBase/Inport.h"
#include "CmodelBase/Outport.h"
#include "CmodelBase/baseunit.h"
#include "CmodelBase/regbase.h"
#include "CmodelBase/UnitRect.h"
#include "CmodelBase/maptable.h"
#include "CmodelBase/MapInfo.h"
#include "CmodelBase/imageunit.h"
#include "CmodelBase/mapbase.h"
#include "CmodelBase/modulebase.h"
#include "CmodelBase/BaseBoxPropDlg.h"
#include "CmodelBase/BasePinPropDlg.h"
#include "CmodelBase/imageboxpropdlg.h"
//#include "CmodelBase/cmparmtree.h"


#endif
