// SummonDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CSummonApp:
// �йش����ʵ�֣������ SummonDlg.cpp
//

class CSummonApp : public CWinApp
{
public:
	CSummonApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSummonApp theApp;
