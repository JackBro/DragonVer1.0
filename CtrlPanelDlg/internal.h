#ifndef _CTRLPLANE_INTERNAL_H_
#define _CTRLPLANE_INTERNAL_H_

#ifdef _WINDLL
	#define CTRLPLANE_DLL  __declspec(dllexport)
#else
	#define CTRLPLANE_DLL  
#endif

//#pragma comment(lib, "toolbuf.lib")
//#pragma comment(lib, "toolfile.lib")
////#pragma comment(lib, "ChannelSimRegSram.lib")
////#pragma comment(lib, "ChannelCom.lib")
////#pragma comment(lib, "BasicFunc.lib")
//#pragma comment(lib, "BaseClass.lib")


//#pragma comment(lib, "toolparse.lib")
//#pragma comment(lib, "tools.lib")
//#pragma comment(lib, "ChannelCom.lib")



//#pragma comment(lib,"trm568hapi.lib")
//#pragma comment(lib,"dbugtools.lib")
//#pragma comment(lib,"trmdriver.lib")
//#pragma comment(lib,"trmv568env.lib")
//#pragma comment(lib,"v568simpledemo.lib")
//#pragma comment(lib,"highapi.lib")
//#pragma comment(lib,"trmtools.lib")
//#pragma comment(lib,"trmuserapi.lib")
//#pragma comment(lib,"trm938driver.lib")
//#pragma comment(lib,"trm938highapi.lib")
//#pragma comment(lib,"trm938testdemo.lib")
//#pragma comment(lib,"trm938userapi.lib")
//#pragma comment(lib,"trm938tools.lib")
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

#include "tools/global.h"

#include "../terminal/terminal/main/main.h"

#include "EmuHardDisk/EmuHardDiskF.h"

#include "SimRegSram/SimregSram.h"

#include "ComChannel/ComChannel.h"
#include "ComChannel/SimHost.h"

#include "CtrlPanelDlg/HostPanelDlg.h"
#include "CtrlPanelDlg/NandFatDlg.h"
#include "CtrlPanelDlg/TestFpgaDlg.h"
#include "CtrlPanelDlg/UsbChipDlg.h"
#include "CtrlPanelDlg/V568SdcardDlg.h"
#include "CtrlPanelDlg/V568SensorDlg.h"


#endif

