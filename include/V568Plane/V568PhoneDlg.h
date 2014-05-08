#ifndef __V568PLANE_V568PHONEDLG_H__
#define __V568PLANE_V568PHONEDLG_H__



#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"



// CV568PhoneDlg 对话框
typedef struct tag_TPhoneOption {
	int pcdemoen;
	int openstate;
	int poweronstate;
} TPhoneOption, *PTPhoneOption;

class V568PLANE_DLL CV568PhoneDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568PhoneDlg)

public:
	CV568PhoneDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568PhoneDlg();

// 对话框数据
	enum { IDD = IDD_V568PHONE_DLG };

protected:
	virtual void InitUserOption(void);
	virtual void DispOption(void);
	virtual void GetUserOption(void);
	virtual void HSendKey(int key);

private:
	TPhoneOption m_option;
	CButton m_openbtn;
	CButton m_poweronbtn;
	CButton m_pcdemobtn;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPoweronCheck();
	afx_msg void OnBnClickedOpenCheck();
	afx_msg void OnBnClickedDemobuttonUp();
	afx_msg void OnBnClickedDemobuttonDown();
	afx_msg void OnBnClickedDemobuttonLeft();
	afx_msg void OnBnClickedDemobuttonRight();
	afx_msg void OnBnClickedDemobuttonMenu();
	afx_msg void OnBnClickedMenudisappearCheck();
	afx_msg void OnBnClickedPcdemoCheck();
	afx_msg void OnBnClickedMenuinitCheck();
	afx_msg void OnBnClickedDisppearmenuBtn();
	afx_msg void OnBnClickedCleardemoBtn();
};

#endif
