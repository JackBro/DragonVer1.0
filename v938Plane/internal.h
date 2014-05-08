#ifndef _V938PLANE_INTERNAL_H_
#define _V938PLANE_INTERNAL_H_

#ifdef _WINDLL
	#define V938PLANE_DLL __declspec(dllexport)
#else
	#define V938PLANE_DLL 
#endif

//#pragma comment(lib, "ChannelCom.lib")

//#pragma comment(lib, "toolchar.lib")
//#pragma comment(lib, "toolfile.lib")
//#pragma comment(lib, "toolbuf.lib")
//#pragma comment(lib, "BaseClass.lib")
//#pragma comment(lib, "Simhost.lib")
//
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
//#pragma comment(lib,"trmv938env.lib")
//#pragma comment(lib,"trmtestdemo.lib")
//#pragma comment(lib,"trmsetupenv.lib")
//#pragma comment(lib,"trm868driver.lib")
//#pragma comment(lib,"trmusb.lib")
//#pragma comment(lib,"trmMain.lib")
//#pragma comment(lib,"trmvstdio.lib")
//#pragma comment(lib,"trmyaffs.lib")
//#pragma comment(lib,"trmnandflash.lib")
//#pragma comment(lib,"trmfat.lib")

#include "stdafx.h"
#include "resource.h"

#include "../terminal/terminal/main/main.h"

#include "v938Plane/V938CpmDlg.h"
#include "v938Plane/V938DemoDlg.h"
#include "v938Plane/V938HostIfDlg.h"
#include "v938Plane/V938Mp3Dlg.h"
#include "v938Plane/V938SeqDlg.h"
#include "v938Plane/V938WsDlg.h"



#endif

