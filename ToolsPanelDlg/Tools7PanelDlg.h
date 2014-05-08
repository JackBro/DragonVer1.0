#ifndef __TOOLSPANEL_TOOLS7PANELDLG_H__
#define __TOOLSPANEL_TOOLS7PANELDLG_H__


#ifndef _WINDLL
#define TOOLSPANEL_DLL
#endif

#ifndef TOOLSPANEL_DLL
#define TOOLSPANEL_DLL _declspec(dllimport)
#endif


#include "../BaseClass/BasePanelDlg.h"

// CTools7PanelDlg �Ի���
typedef struct tag_TUserT7Option {
	char src[DEFAULT_FILENAME_LEN];
	char dest[DEFAULT_FILENAME_LEN];
} TUserT7Option, *PTUserT7Option;

class TOOLSPANEL_DLL CTools7PanelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CTools7PanelDlg)

public:
	CTools7PanelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTools7PanelDlg();

// �Ի�������
	enum { IDD = IDD_TOOLS7_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void AutoSetSrcfile(void);
	void DispOption(void);

private:
	TUserT7Option m_option;

private:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTools7miscButton();
};

#endif
