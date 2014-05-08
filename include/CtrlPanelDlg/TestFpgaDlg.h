#ifndef __CTRLPLANE_TESTFPGADLG_H__
#define __CTRLPLANE_TESTFPGADLG_H__


#ifndef CTRLPLANE_DLL
	#define CTRLPLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"

#define CMD_LENGTH		0X20

#define PARAM_LENGTH	0x50

typedef struct tag_TFpgaOption
{
	char srcfile[DEFAULT_FILENAME_LEN];
	char dstfile[DEFAULT_FILENAME_LEN];
	int regtype;
	int regval;
	int regaddr;
	int regdat;
	char cmd[CMD_LENGTH];
	char para[7][PARAM_LENGTH];
} TFpgaOption, *PTFpgaOption;

// CTestFpgaDlg 对话框

class CTRLPLANE_DLL CTestFpgaDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CTestFpgaDlg)

public:
	CTestFpgaDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestFpgaDlg();

// 对话框数据
	enum { IDD = IDD_TESTFPGA_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOpenbatchfileBtn();
	virtual BOOL OnInitDialog();

protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);

public:
	afx_msg void OnBnClickedWriteregButton();
	afx_msg void OnBnClickedRegreadButton();
	afx_msg void OnDeltaposRegaddrSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBatchwriteButton();

private:
	int         m_BusTypeSel;
	TFpgaOption	m_option;
	CMComboBox	m_regtype;
	CMHexEdit	m_regaddr;
	CMHexEdit	m_regval;
	CMHexEdit	m_regdat;

	CButton		m_instant;

	CButton		m_sndcmdBtn;
	CMComboBox m_cmdBox;
	CEdit m_para0Edit;
	CEdit m_para1Edit;
	CEdit m_para2Edit;
	CEdit m_para3Edit;
	CEdit m_para4Edit;
	CEdit m_para5Edit;
	CEdit m_para6Edit;

	CEdit *m_pParaEdit[7];

public:
	afx_msg void OnCbnSelchangeCmdlistCombo();
	afx_msg void OnBnClickedSendcmdButton();
};

#endif
