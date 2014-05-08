#ifndef _TOOLSPANEL_INTERNAL_H_
#define _TOOLSPANEL_INTERNAL_H_

#ifdef _WINDLL
	#define TOOLSPANEL_DLL __declspec(dllexport)
#endif

#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolfile.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "toolmath.lib")
#pragma comment(lib, "tooljpg.lib")
#pragma comment(lib, "toolchar.lib")
#pragma comment(lib, "toolbit.lib")
#pragma comment(lib, "toolmpeg2i.lib")
#pragma comment(lib, "toolmpeg4.lib")
#pragma comment(lib, "toolwave.lib")
#pragma comment(lib, "toolaudiodev.lib")
#pragma comment(lib, "toolconvert.lib")
#pragma comment(lib, "toolv968alg.lib")
#pragma comment(lib, "BaseClass.lib")
#pragma comment(lib, "SillGui.lib")
#pragma comment(lib, "toolimgstat.lib")

//#pragma comment(lib,"trm568hapi.lib")
//#pragma comment(lib,"dbugtools.lib")
//#pragma comment(lib,"trmdriver.lib")
//#pragma comment(lib,"trmv568env.lib")
//#pragma comment(lib,"v568simpledemo.lib")
//#pragma comment(lib,"highapi.lib")
//#pragma comment(lib,"trmtools.lib")
//#pragma comment(lib,"trmuserapi.lib")
//
//#pragma comment(lib,"trm938driver.lib")
//#pragma comment(lib,"trm938highapi.lib")
//#pragma comment(lib,"trm938testdemo.lib")
//#pragma comment(lib,"trm938userapi.lib")
//#pragma comment(lib,"trm938tools.lib")
//#pragma comment(lib,"trmv938env.lib")
//
//#pragma comment(lib,"trmtestdemo.lib")
//#pragma comment(lib,"trmsetupenv.lib")
//#pragma comment(lib,"trmusb.lib")
//#pragma comment(lib,"trmMain.lib")
//#pragma comment(lib,"trmvstdio.lib")
//#pragma comment(lib,"trmyaffs.lib")
//#pragma comment(lib,"trmnandflash.lib")
//#pragma comment(lib,"trmfat.lib")


#include "stdafx.h"
#include "resource.h"

#include "toolconvert/normaltools.h"
#include "toolconvert/generaltools.h"

#include "toolwave/wavewf.h"

#include "toolv968alg/9683daudio.h"
#include "toolv968alg/968adpcm.h"
#include "toolv968alg/968Agc.h"

#include "toolimgstat/yuvstatf.h"

#include "BaseClass/Dlgres.h"
#include "BaseClass/GammaDlg.h"


#include "ToolsPanelDlg/Tools1PanelDlg.h"
#include "ToolsPanelDlg/Tools2PanelDlg.h"
#include "ToolsPanelDlg/Tools3PanelDlg.h"
#include "ToolsPanelDlg/Tools4PanelDlg.h"
#include "ToolsPanelDlg/Tools5PanelDlg.h"
#include "ToolsPanelDlg/Tools6PanelDlg.h"
#include "ToolsPanelDlg/Tools7PanelDlg.h"
#include "ToolsPanelDlg/Tools8PanelDlg.h"
#include "ToolsPanelDlg/Tools9PanelDlg.h"





#endif
