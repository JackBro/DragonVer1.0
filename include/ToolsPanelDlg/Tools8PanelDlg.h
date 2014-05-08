#ifndef __TOOLSPANEL_TOOLS8PANELDLG_H__
#define __TOOLSPANEL_TOOLS8PANELDLG_H__


#ifndef TOOLSPANEL_DLL
	#define TOOLSPANEL_DLL _declspec(dllimport)
#endif

#include "BaseClass/BasePanelDlg.h"

// CTools8PanelDlg 对话框
typedef struct tag_TUserT8Option {
	char src[DEFAULT_FILENAME_LEN];
	char dest[DEFAULT_FILENAME_LEN];
} TUserT8Option, *PTUserT8Option;

class TOOLSPANEL_DLL CTools8PanelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CTools8PanelDlg)

public:
	CTools8PanelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTools8PanelDlg();

// 对话框数据
	enum { IDD = IDD_TOOLS8_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void AutoSetSrcfile(void);
	void DispOption(void);

private:
	TUserT8Option m_option;

private:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLexAnalysis();
	afx_msg void OnBnClickedSynParser();
	afx_msg void OnBnClickedHeadfileList();
	afx_msg void OnBnClickedMaplex();
	afx_msg void OnBnClickedMapsynParser();
};

#endif
