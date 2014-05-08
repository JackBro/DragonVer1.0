#ifndef __TOOLSPANEL_TOOLS6PANELDLG_H__
#define __TOOLSPANEL_TOOLS6PANELDLG_H__



#ifndef TOOLSPANEL_DLL
	#define TOOLSPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CTools6PanelDlg 对话框

class TOOLSPANEL_DLL CTools6PanelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CTools6PanelDlg)

public:
	CTools6PanelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTools6PanelDlg();

// 对话框数据
	enum { IDD = IDD_TOOLS6_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGama();
	LRESULT OnGammaApply(WPARAM wParam , LPARAM lParam);
};

#endif
