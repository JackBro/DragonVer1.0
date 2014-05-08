#ifndef __V568PLANE_V568MARBDLG_H__
#define __V568PLANE_V568MARBDLG_H__



#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif


#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MHexEdit.h"


// CV568MarbDlg 对话框
typedef struct tag_TMarbOption {
	int mode;
	int almode;
	int	gapratio;
	int isize;
	int	ramstart;
	int ramend;
	TMarbMap map;
} TMarbOption, *PMarbOption;

class V568PLANE_DLL CV568MarbDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568MarbDlg)

public:
	CV568MarbDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568MarbDlg();

// 对话框数据
	enum { IDD = IDD_V568MARB_DLG };

private:
	CMHexEdit m_a1startbox;
	CMHexEdit m_a1endbox;
	CMHexEdit m_a0startbox;
	CMHexEdit m_a0endbox;
	CMHexEdit m_jbufstartbox;
	CMHexEdit m_jbufendbox;
	CMHexEdit m_indexstartbox;
	CMHexEdit m_indexendbox;
	CMHexEdit m_thumbstartbox;
	CMHexEdit m_thumbendbox;
	CMHexEdit m_lbuf1startbox;
	CMHexEdit m_lbuf1endbox;
	CMHexEdit m_lbuf0startbox;
	CMHexEdit m_lbuf0endbox;
	CMHexEdit m_b1startbox;
	CMHexEdit m_b1endbox;
	CMHexEdit m_b0startbox;
	CMHexEdit m_b0endbox;
	CMHexEdit m_ibufsizebox;
	CMHexEdit m_ramstartbox;
	CMHexEdit m_ramendbox;

private:
	TMarbOption m_option;
	CMComboBox m_workmodebox;
	CMComboBox m_allocmodebox;
	CMComboBox m_gapratiobox;

protected:
	void InitUserOption(void);
	void DispOption(void);
	void GetUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRefreshmapButton();
	afx_msg void OnBnClickedIbufsizeButton();
	afx_msg void OnCbnSelchangeGapratioCombo();
	afx_msg void OnCbnSelchangeWorkmodeCombo();
	afx_msg void OnCbnSelchangeAllocmodeCombo();
	afx_msg void OnBnClickedRamstartButton();
	afx_msg void OnBnClickedRamendButton();
};

#endif
