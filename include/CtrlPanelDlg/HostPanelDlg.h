#ifndef __CTRLPLANE_HOSTPANELDLG_H__
#define __CTRLPLANE_HOSTPANELDLG_H__


#ifndef CTRLPLANE_DLL
	#define CTRLPLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MDecEdit.h"
#include "BaseClass/MHexEdit.h"

typedef struct tag_THostOption
{
	char srcfile[DEFAULT_FILENAME_LEN];
	char dstfile[DEFAULT_FILENAME_LEN];
	char armfile[DEFAULT_FILENAME_LEN];
	int buflen;
	int mclk;
	int i2sclk;
	int tmrint;
	int timersel;
	int flag;
	unsigned wdtcnt;
} THostOption, *PTHostOption;

// CHostPanelDlg 对话框

class CTRLPLANE_DLL CHostPanelDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CHostPanelDlg)

public:
	CHostPanelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHostPanelDlg();

// 对话框数据
	enum { IDD = IDD_HOSTPANEL_DLG };

protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void GetEditboxValue(void);
	void DispOption(void);
	void GetComboValue(char *tail = FILE_TAIL_BIN);

private:
	THostOption m_option;

	CMComboBox m_armfileBox;

	CMComboBox m_exint0Box;
	CMComboBox m_exint1Box;
	CMComboBox m_exint2Box;
	CMComboBox m_exint3Box;
	CMComboBox m_exint4567Box;
	CMComboBox m_timerselBox;

	CButton m_beepbtn;
	CButton m_keyprocbtn;
	CButton m_wdtenbtn;

	CMHexEdit m_buflen;

	CMDecEdit m_mclkBox;
	CMDecEdit m_i2sclkEdit;
	CMDecEdit m_t5intbox;

public:
	afx_msg void OnBnClickedWritebufButton();
	afx_msg void OnBnClickedReadbufButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDownloadButton();
	afx_msg void OnBnClickedSetmclkButton();
	afx_msg void OnBnClickedSaveButton();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedLoadButton();
	afx_msg void OnBnClickedSavegbufButton();
	afx_msg void OnBnClickedLoadgbufButton();
	afx_msg void OnBnClickedSeti2sclkButton();
	afx_msg void OnBnClickedBeepCheck();
	afx_msg void OnBnClickedKeyprocessCheck();
	afx_msg void OnCbnSelchangeExtint4567Combo();
	afx_msg void OnCbnSelchangeExtint3Combo();
	afx_msg void OnCbnSelchangeExtint2Combo();
	afx_msg void OnCbnSelchangeExtint1Combo();
	afx_msg void OnCbnSelchangeExtint0Combo();
	afx_msg void OnCbnSelchangeTimerCombo();
	afx_msg void OnBnClickedSettimerButton();
	afx_msg void OnBnClickedRefreshButton();
	afx_msg void OnBnClickedWdtButton();
	afx_msg void OnBnClickedWdtenCheck();
};

#endif
