#ifndef __TOOLSPANEL_TOOLS6PANELDLG_H__
#define __TOOLSPANEL_TOOLS6PANELDLG_H__


#ifndef _WINDLL
#define TOOLSPANEL_DLL
#endif

#ifndef TOOLSPANEL_DLL
#define TOOLSPANEL_DLL _declspec(dllimport)
#endif


#include "../BaseClass/BasePanelDlg.h"
#include "../BaseClass/GammaDlg.h"

// CTools6PanelDlg �Ի���

class TOOLSPANEL_DLL CTools6PanelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CTools6PanelDlg)

public:
	CTools6PanelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTools6PanelDlg();

// �Ի�������
	enum { IDD = IDD_TOOLS6_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGama();
	LRESULT OnGammaApply(WPARAM wParam , LPARAM lParam);
};

#endif
