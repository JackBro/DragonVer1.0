// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�

#ifndef __CMODELBASE_STDAFX_H__
#define __CMODELBASE_STDAFX_H__

#ifndef _WINDLL
#define CMODELBASE_DLL
#endif

#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif




//#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��

//#ifndef VC_EXTRALEAN
//#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
//#endif

//#include <afx.h>
//#include <afxwin.h>         // MFC ��������ͱ�׼���
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxpriv.h>		// required for WM_SETMESSAGESTRING
#include <afxwin.h>


#endif
