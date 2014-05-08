#ifndef __V568PLANE_V568CPMDLG_H__
#define __V568PLANE_V568CPMDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MHexEdit.h"

// CV568CpmDlg 对话框

typedef struct tag_TCPMOption
{
	/*interrupt*/
	char     bIntAll;
	char     IntWidth;
    char     IntTrigeType;
	char     IntLevelType;
	char     IntEdgeType;

	/*clock off*/
	char     bClkAll;

	/*reset*/
	char     bRstAll;
	
	/*pll*/
	char     bClkMonitor;
	char     bPllAuto;	
	char	 M;
	char     N;
	char     Od0;
	char     Od1;
	char     Div;
	int      Clkin;
	int      Mclk;
	
	/*GPIO*/
	char     gpioPinIdx;
	char     gpioConfig;
	char     gpioMod;
	char     gpioDir;
	char     gpioVal;

	int		timemode;
	int		time;


} TCPMOption, *PTCPMOption;

class V568PLANE_DLL CV568CpmDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568CpmDlg)

public:
	CV568CpmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568CpmDlg();

// 对话框数据
	enum { IDD = IDD_V568CPM_DLG };

protected:
	void DispUserOption(void);

	void GetUserOption(void);
	void InitUserOption(void);
	void GetCheckBtnValue(void);
	void GetComboValue(void);
	void GetEditboxValue(void);

protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	TCPMOption m_option;

	CListBox  m_CtrlListIntType;
	CListBox  m_CtrlListClkType;
	CListBox  m_CtrlListRstType;
	CString   m_strListIntType;
    CComboBox m_GPIOPinIdxComBox;

	CMHexEdit m_CtrlClkin;
	CMHexEdit m_CtrlMclk;
	CMHexEdit m_CtrlM;
	CMHexEdit m_CtrlN;
	CMHexEdit m_CtrlOd0;
	CMHexEdit m_CtrlOd1;
	CMHexEdit m_CtrlDiv;
	CMHexEdit m_gpioconfigbox;
	CMHexEdit m_gpiomodbox;
	CMHexEdit m_gpiodirbox;
	CMHexEdit m_gpiowrvalbox;
	CMHexEdit m_gpiordvalbox;
	CMHexEdit m_IntSigValWidthBox;

	CButton m_IntAllBtn;
	CButton m_ClkAllBtn;
	CButton m_ResetAllBtn;
	CButton m_ClkAutoBtn;
	CButton m_ClkMonitorBtn;
	CButton m_intLevelTrig;
	CButton m_intLevelTrigLowAct;
	CButton m_intEdgeTrigLowAct;

	CButton m_ChkTimeMode;
	CMDecEdit m_EdtTime;
	CButton m_BtnStTime;


public:
	afx_msg void OnBnClickedButtonCpmInit();
	afx_msg void OnBnClickedCheckIntall();
	afx_msg void OnBnClickedButtonTestInt();
	afx_msg void OnBnClickedCheckClkAuto();
	afx_msg void OnBnClickedButtonTestClock();
	afx_msg void OnBnClickedButtonTestReset();
	afx_msg void OnBnClickedCheckClkAll();
	afx_msg void OnBnClickedCheckResetAll();
	afx_msg void OnBnClickedButtonRelease();
	afx_msg void OnBnClickedGpioreadvalueButton();
	afx_msg void OnBnClickedGpiowritevalueButton();
	afx_msg void OnBnClickedGpiodirsetButton();
	afx_msg void OnBnClickedGpiomodesetButton();
	afx_msg void OnBnClickedGpioconfigsetButton();	
	afx_msg void OnCbnSelchangeGpiopinidxCombo();
	
	afx_msg void OnBnClickedLeveltriggerCheck();
	afx_msg void OnBnClickedButtonClkSet();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonClkSet2();
	afx_msg void OnBnClickedCheckTimeMode();
};

#endif
