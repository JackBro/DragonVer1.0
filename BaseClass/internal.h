#ifndef _BASECLASS_INTERNAL_H_
#define _BASECLASS_INTERNAL_H_

#ifdef _WINDLL
	#define BASECLASS_DLL  __declspec(dllexport)
#endif

#pragma comment (lib, "gdiplus")

#pragma comment(lib,"tools.lib")
#pragma comment(lib,"toolbuf.lib")
#pragma comment(lib,"toolconvert.lib")
#pragma comment(lib,"toolfile.lib")
#pragma comment(lib,"toolmath.lib")
#pragma comment(lib,"toolchar.lib")
#pragma comment(lib,"toolparse.lib")

#include "stdafx.h"
#include "resource.h"


#include "toolchar/stringtools.h"

#include "toolconvert/raw2rgbf.h"
#include "toolconvert/rgb24convert.h"


#include "BaseClass/ImageFrame.h"
#include "BaseClass/MBaseEdit.h"
#include "BaseClass/MClipB.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MDecEdit.h"
#include "BaseClass/MHexEdit.h"
#include "BaseClass/MToolTip.h"
#include "BaseClass/BaseDlg.h"
#include "BaseClass/BasePanelDlg.h"
#include "BaseClass/AudioCtrl.h"
#include "BaseClass/BaseTabCtrlView.h"
#include "BaseClass/BaseStatusBar.h"
#include "BaseClass/BaseToolBar.h"
#include "BaseClass/GammaDlg.h"
#include "BaseClass/PictureBox.h"
#include "BaseClass/RawVideoFrm.h"

#endif
