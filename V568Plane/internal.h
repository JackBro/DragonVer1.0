#ifndef _V568PLANE_INTERNAL_H_
#define _V568PLANE_INTERNAL_H_

#ifdef _WINDLL
	#define V568PLANE_DLL __declspec(dllexport)
#else
	#define V568PLANE_DLL 
#endif

//#pragma comment(lib, "tools.lib")
//#pragma comment(lib, "toolchar.lib")

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
//#pragma comment(lib,"trm868driver.lib")
//#pragma comment(lib,"trmusb.lib")
//#pragma comment(lib,"trmMain.lib")
//#pragma comment(lib,"trmvstdio.lib")
//#pragma comment(lib,"trmyaffs.lib")
//#pragma comment(lib,"trmnandflash.lib")
//#pragma comment(lib,"trmfat.lib")


#include "stdafx.h"
#include "resource.h"
#include <olectl.h>

#include "tools/global.h"

#include "../terminal/terminal/main/main.h"

#include "toolconvert/generaltools.h"

#include "ComChannel/SimHost.h"

#include "V568Plane/V558DemoDlg.h"
#include "V568Plane/V558HostIFDlg.h"
#include "V568Plane/V568AudioAdcDlg.h"
#include "V568Plane/V568BoardDlg.h"
#include "V568Plane/V568CpmDlg.h"
#include "V568Plane/V568Demo1Dlg.h"
#include "V568Plane/V568GeDlg.h"
#include "V568Plane/V568HighapiDlg.h"
#include "V568Plane/V568IppDlg.h"
#include "V568Plane/V568IspDlg.h"
#include "V568Plane/V568JpegDlg.h"
#include "V568Plane/V568LbufDlg.h"
#include "V568Plane/V568LcdcDlg.h"
#include "V568Plane/V568LcdifDlg.h"
#include "V568Plane/V568MarbDlg.h"
#include "V568Plane/V568PhoneDlg.h"
#include "V568Plane/V568SdifDlg.h"
#include "V568Plane/V568SifDlg.h"
#include "V568Plane/V568Test1TDlg.h"
#include "V568Plane/V568TestChipDlg.h"
#include "V568Plane/V568UsbcDlg.h"





#endif

