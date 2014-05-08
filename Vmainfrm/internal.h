#ifndef _DRAGON_INTERNAL_H_
#define _DRAGON_INTERNAL_H_


//#pragma comment(lib, "trmyaffs.lib")
//#pragma comment(lib, "highapi.lib")
//#pragma comment(lib, "trmtestdemo.lib")
//#pragma comment(lib, "trmtools.lib")
//#pragma comment(lib, "trmuserapi.lib")
//#pragma comment(lib, "trm868driver.lib")
//#pragma comment(lib, "trm938driver.lib")
//#pragma comment(lib, "trmv938env.lib")
//#pragma comment(lib, "trm938highapi.lib")
//#pragma comment(lib, "trmvstdio.lib")
//#pragma comment(lib, "trmusb.lib")
//#pragma comment(lib, "trmOS.lib")
//#pragma comment(lib, "trmnandflash.lib")
//#pragma comment(lib, "trmmain.lib")
//#pragma comment(lib, "trmfat.lib")
//#pragma comment(lib, "trm938userapi.lib")
//#pragma comment(lib, "trm938tools.lib")
//#pragma comment(lib, "trm938testdemo.lib")
//#pragma comment(lib, "dbugtools.lib")
//#pragma comment(lib, "trmdriver.lib")
//#pragma comment(lib, "trmv568env.lib")
//#pragma comment(lib, "trm568hapi.lib")
//#pragma comment(lib, "trmtools.lib")
//#pragma comment(lib, "highapi.lib")
//#pragma comment(lib, "v568simpledemo.lib")
//#pragma comment(lib, "trmsetupenv.lib")
//
//#pragma comment(lib, "PlaneCtrl.lib")
//#pragma comment(lib, "PlaneV568.lib")
//#pragma comment(lib, "PlaneV938.lib")
//#pragma comment(lib, "BasicFunc.lib")




#pragma comment(lib, "RegList.lib")

#pragma comment(lib, "toolparse.lib")
#pragma comment(lib, "tools.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "toolchar.lib")
#pragma comment(lib, "toolfile.lib")

#pragma comment(lib, "toolmpeg2i.lib")
#pragma comment(lib, "toolmpeg4.lib")
#pragma comment(lib, "tooljpg.lib")
#pragma comment(lib, "toolconvert.lib")

#pragma comment(lib, "BaseClass.lib")

#pragma comment(lib, "PlaneTools.lib")
#pragma comment(lib, "PlaneCmodel.lib")

#pragma comment(lib, "ChannelCom.lib")
#pragma comment(lib, "ChannelUart.lib")
#pragma comment(lib, "ChannelUsb.lib")

#pragma comment(lib, "CmodelView.lib")
#pragma comment(lib, "SettingPanel.lib")
#pragma comment(lib, "headerpage.lib")
#pragma comment(lib, "EmuHardDisk.lib")


#define V568PLANE_DLL
#define CTRLPLANE_DLL
#define V938PLANE_DLL
//#define REGLIST_DLL


#include "stdafx.h"
#include "resource.h"

#include "BaseClass/dlgres.h"

#include "MainFrmDlg/UsbSettingDlg.h"

#include "ComChannel/SimHost.h"
#include "ComChannel/ComChannel.h"

#include "../terminal/terminal/main/main.h"

#include "Reglist/Reglist.h"
#include "BaseClass/dlgres.h"

#include "VmonitorDoc.h"
#include "MorFormView.h"
#include "ArmReglistView.h"
#include "CmodelFormView.h"
#include "cmodelView.h"
#include "toolsFormView.h"
#include "Usbreglistview.h"
#include "v558Reglistview.h"
#include "v568ReglistView.h"
#include "v578ReglistView.h"
#include "v938Formview.h"
#include "v938Reglistview.h"
#include "Vmonitorview.h"
#include "childFrm.h"
#include "mainFrm.h"
#include "vmonitor.h"





#endif
