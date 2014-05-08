#ifndef _CMODELPANEL_INTERNAL_H_
#define _CMODELPANEL_INTERNAL_H_

#ifdef _WINDLL
	#define CMODELPANEL_DLL  __declspec(dllexport)
#endif

#pragma comment(lib, "toolfile.lib")
#pragma comment(lib, "toolbuf.lib")
#pragma comment(lib, "tooljpg.lib")
#pragma comment(lib, "BaseClass.lib")
#pragma comment(lib, "toolconvert.lib")
#pragma comment(lib, "toolv326alg.lib")
#pragma comment(lib, "toolv702alg.lib")

#pragma comment(lib, "toolchar.lib")

#include "stdafx.h"
#include "resource.h"

#include "toolconvert/raw2rgbf.h"
#include "toolconvert/yiq2rgbf.h"
#include "toolconvert/yuv422f.h"
#include "toolconvert/yuv422.h"

#include "tooljpg/Jpegf.h"

#include "toolv326alg/ipp.h"
#include "toolv326alg/isp.h"
#include "toolv326alg/isp.h"
#include "toolv326alg/sif.h"
#include "toolv326alg/lcdc.h"
#include "toolv702alg/ipp.h"
#include "toolv702alg/tvenc.h"
#include "toolchar/charfifo.h"

#include "BaseClass/dlgres.h"

#include "CmodelPanelDlg/BaseCmodelPanelDlg.h"
#include "CmodelPanelDlg/BlkAudadcDlg.h"
#include "CmodelPanelDlg/BlkBiuDlg.h"
#include "CmodelPanelDlg/BlkCpmDlg.h"
#include "CmodelPanelDlg/BlkGeDlg.h"
#include "CmodelPanelDlg/BlkIppDlg.h"
#include "CmodelPanelDlg/BlkIspDlg.h"
#include "CmodelPanelDlg/BlkJpegDlg.h"
#include "CmodelPanelDlg/BlkLbufDlg.h"
#include "CmodelPanelDlg/BlkLcdcDlg.h"
#include "CmodelPanelDlg/BlkLcdifDlg.h"
#include "CmodelPanelDlg/BlkMarbDlg.h"
#include "CmodelPanelDlg/BlkSdcardDlg.h"
#include "CmodelPanelDlg/BlkSdifDlg.h"
#include "CmodelPanelDlg/BlkSensorDlg.h"
#include "CmodelPanelDlg/BlkSifDlg.h"
#include "CmodelPanelDlg/BlkTvencDlg.h"
#include "CmodelPanelDlg/BlkUsbcDlg.h"
#include "CmodelPanelDlg/CmodelDlg.h"
#include "CmodelPanelDlg/OldCmodelDlg.h"




#endif
