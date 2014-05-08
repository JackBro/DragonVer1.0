#ifndef _CMODELVIEW_INTERNAL_H_
#define _CMODELVIEW_INTERNAL_H_

#ifdef _WINDLL
	#define CMODELVIEW_DLL  __declspec(dllexport)
#endif

#pragma comment (lib, "gdiplus.lib")
#pragma comment (lib, "tools.lib")
#pragma comment (lib, "toolchar.lib")
#pragma comment (lib, "toolfile.lib")
#pragma comment (lib, "tooltree.lib")
#pragma comment (lib, "BaseClass.lib")
#pragma comment (lib, "CmodelBase.lib")
#pragma comment (lib, "CmodelContainer.lib")
#pragma comment (lib, "CmodelBoxData.lib")

#include "stdafx.h"

#include "tools/typeprop.h"

#include "toolchar/stringtools.h"

#include "CModelBoxData/BoxData.h"

#include "BaseClass/dlgres.h"

#include "CmodelView/PinPropDlg.h"
#include "CmodelView/InportPropDlg.h"
#include "CmodelView/OutportPropDlg.h"
#include "CmodelView/BaseCmViewDlg.h"


#endif
