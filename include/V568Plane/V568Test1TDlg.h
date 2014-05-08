#ifndef __V568PLANE_V568TEST1TDLG_H__
#define __V568PLANE_V568TEST1TDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"



// CV568Test1TDlg 对话框

class V568PLANE_DLL CV568Test1TDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568Test1TDlg)

public:
	CV568Test1TDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568Test1TDlg();

// 对话框数据
	enum { IDD = IDD_V568TEST1T_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked1ttestButton();
};

#endif
