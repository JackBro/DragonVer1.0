// Vmonitor.h : Vmonitor 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif



// CVmonitorApp:
// 有关此类的实现，请参阅 Vmonitor.cpp
//

#define COMPANY_KEY			"Vimicro Monitor"
#define VMONITOR_BAR		"VMonitor Control Bar States"
#define SETTINGS_KEY		"Settings"
#define WINDOWPLACEMENT_KEY	"Window Placement"

class CVmonitorApp : public CWinApp
{
public:
	CVmonitorApp();
	virtual ~CVmonitorApp();

protected:
	void	InitDocTemplate(void);

private:
	CMultiDocTemplate* m_pTxtDocTemplate;
	CMultiDocTemplate* m_pCmdDocTemplate;
	CMultiDocTemplate* m_pControlDocTemplate;
	CMultiDocTemplate* m_pRegListDocTemplate;
	CMultiDocTemplate* m_pToolsDocTemplate;
	CMultiDocTemplate* m_pCmodelDocTemplate;
	CMultiDocTemplate* m_pV558RegTemplate;
	CMultiDocTemplate* m_pArmRegTemplate;
	CMultiDocTemplate* m_pUsbRegTemplate;
	CMultiDocTemplate* m_pV938RegTemplate;
	CMultiDocTemplate* m_pV578RegTemplate;
	CMultiDocTemplate* m_pV938PanelTemplate;
	CMultiDocTemplate* m_pCmodelPrjTemplate;

public:
	CDocument *m_pDoc;

// 重写
public:
	virtual BOOL InitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNewCmdWin();
	afx_msg void OnNewcontrolPanel();
	afx_msg void OnNewRegList();
	afx_msg void OnNewToolsPanel();
	afx_msg void OnNewCmodelPanel();
	afx_msg void OnV558reglist();
	afx_msg void OnNewArmReglist();
	afx_msg void OnNewUsbReglist();
	afx_msg void OnNewV938Reglist();
	afx_msg void OnNewV938Panel();
	afx_msg void OnNewTxtFile();

private:
	ULONG_PTR m_GdiplusToken;

public:
	afx_msg void OnNewCmodelPrj();
	afx_msg void OnNewV578reglist();
};

extern CVmonitorApp theApp;
