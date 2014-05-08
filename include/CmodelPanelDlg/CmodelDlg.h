#ifndef __CMODELPANEL_CMODELDLG_H__
#define __CMODELPANEL_CMODELDLG_H__



#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CCmodelDlg 对话框

class CMODELPANEL_DLL CCmodelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CCmodelDlg)

public:
	CCmodelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCmodelDlg();

// 对话框数据
	enum { IDD = IDD_CMODEL_DLG };

private:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartcmodelBtn();
	afx_msg void OnBnClickedPausecmodelBtn();
	afx_msg void OnBnClickedStopcmodelBtn();
	afx_msg void OnBnClickedCmodelmiscBtn();
	virtual BOOL OnInitDialog();
};

#endif
