// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件

#ifndef __CMODELVIEW_STDAFX_H__
#define __CMODELVIEW_STDAFX_H__



//#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将为显式的

//#ifndef VC_EXTRALEAN
//#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
//#endif

//#include <afx.h>
//#include <afxwin.h>         // MFC 核心组件和标准组件
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxpriv.h>		// required for WM_SETMESSAGESTRING
#include <afxwin.h>

#include "resource.h"

#endif
