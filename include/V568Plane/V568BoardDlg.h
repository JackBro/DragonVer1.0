#ifndef __V568PLANE_V568BOARDDLG_H__
#define __V568PLANE_V568BOARDDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MHexEdit.h"

typedef struct tag_TBoardOption
{
	char srcfile[DEFAULT_FILENAME_LEN];
	char dstfile[DEFAULT_FILENAME_LEN];
	int length;
	int ramaddr;
	int bustype;
	int regaddr;
} TBoardOption, *PTBoardOption;



// CV568BoardDlg 对话框

class V568PLANE_DLL CV568BoardDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568BoardDlg)

public:
	CV568BoardDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568BoardDlg();

// 对话框数据
	enum { IDD = IDD_V568BOARD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);

private:
	TBoardOption m_option;
	CMHexEdit	m_length;
	CMHexEdit	m_ramaddr;
	CButton		m_mul8Btn;
	CButton		m_mul16Btn;
	CButton		m_sepBtn;
	CButton		m_autoBtn;
	CMHexEdit	m_regaddr;


public:
	afx_msg void OnBnClickedButtonTestSensor();
	afx_msg void OnBnClickedWritesramButton();
	afx_msg void OnBnClickedReadsramButton();
	afx_msg void OnBnClickedCheckAutoselbustype();
	afx_msg void OnBnClickedRadioMul8();
	afx_msg void OnBnClickedRadioMul16();
	afx_msg void OnBnClickedRadioSep();
	afx_msg void OnBnClickedTestregrwButton();
};

#endif
