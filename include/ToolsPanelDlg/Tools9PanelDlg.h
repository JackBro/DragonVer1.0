#ifndef __TOOLSPANEL_TOOLS9PANELDLG_H__
#define __TOOLSPANEL_TOOLS9PANELDLG_H__



#ifndef TOOLSPANEL_DLL
	#define TOOLSPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CTools9PanelDlg 对话框
typedef struct tag_TUserT9Option {
	char src[DEFAULT_FILENAME_LEN];
	char dest[DEFAULT_FILENAME_LEN];
} TUserT9Option, *PTUserT9Option;

class TOOLSPANEL_DLL CTools9PanelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CTools9PanelDlg)

public:
	CTools9PanelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTools9PanelDlg();

// 对话框数据
	enum { IDD = IDD_TOOLS9_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void AutoSetSrcfile(void);
	void DispOption(void);

private:
	TUserT9Option m_option;

private:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTools9miscButton();
};

#endif
