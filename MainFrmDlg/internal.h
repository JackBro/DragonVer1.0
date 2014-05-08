#ifndef _SETTINGPANEL_INTERNAL_H_
#define _SETTINGPANEL_INTERNAL_H_


#ifdef _WINDLL
	#define SETTINGPANEL_DLL  __declspec(dllexport)
#endif

#pragma comment(lib,  "BaseClass.lib")
#pragma comment (lib, "setupapi.lib")

#include <stdafx.h>
#include "resource.h"
#include <setupapi.h>

#include "tools/global.h"


#include "MainFrmDlg/ComSettingDlg.h"
#include "MainFrmDlg/UsbSettingDlg.h"



#endif

