#ifndef __BASECLASS_REGLISTDLG_H__
#define __BASECLASS_REGLISTDLG_H__


#include "BasicFunc/basectrl.h"
#include "ReglistCtrl.h"

#define REGLIST_TITLE_LEN	100

class CRegListDlg : public CDialog, protected CBaseCtrl
{
	DECLARE_DYNAMIC(CRegListDlg)

public:
	CRegListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegListDlg();

// 对话框数据
	enum { IDD = IDD_BASEREGLIST_DLG };

protected:
	void	SetRegListPos(void);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	void	SetContent(PTRegUnit pContent, int count, char *title = NULL);

private:
	CRegListCtrl m_reglist;
	CButton m_refreshbtn;
	CButton m_testregbtn;
	CButton m_saveregbtn;
	char	m_title[REGLIST_TITLE_LEN];

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedRefreshBtn();
	afx_msg void OnBnClickedTestregBtn();
	afx_msg void OnBnClickedSaveregBtn();
};


#endif
