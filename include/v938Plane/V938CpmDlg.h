#ifndef __V938PLANE_V938CPMDLG_H__
#define __V938PLANE_V938CPMDLG_H__


#ifndef V938PLANE_DLL
	#define V938PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MHexEdit.h"


enum CPM938TRIGTYPEenum
{
	CPM938_LEVELTRIG,
	CPM938_EDGETRIG
};

enum CPM938LEVELTRIGTYPEenum
{
	CPM938_LOWLEVELTRIG,
	CPM938_HIGHLEVELTRIG
};

enum CPM938EDGETRIGTYPEenum
{
	CPM938_RISEEDGETRIG,
	CPM938_FALLEDGETRIG
};

typedef struct tag_TCPM938Option
{
	/*file name*/
	char	        srcfile[DEFAULT_FILENAME_LEN];
	char	        dstfile[DEFAULT_FILENAME_LEN];

	/*clock*/
	unsigned int    mclk;
    char            pllMode;

	/*timer*/
	char            STRMTimer;
	unsigned short  timerBase;
	unsigned int    timerVal;
	unsigned int    timerCnt;
	
	/*interrupt*/
	char            allIntSelected;
	char            intLocked;
	char            trigerType;
	char            lowLevelTrig;
	char            fallEdgeTrig;
	char            trigWidth;

	/*DAC Setting*/
	char            datFromOffChip;
	char            deepth;
	char            mclkFreq;
	char            dacTest;
	char            iowl;
	char            nrst;
	char            sb;
	char            sbDac;
	char            mute;

	/*IIS Setting*/
	char            iisDiv2;
	char            iisDiv3;
	char            iisMasterMod;
	char            iisMclkToExtern;
	char            iisSclkPolyInv;

	/*GPIO Setting*/
	char           gpioFuncSel;
	char           gpioIdx;
	char           gpioCfg;
	char           gpioMod;
	char           gpioDir;
	unsigned char  gpioRdBkVal;

} TCPM938Option, *PTCPM938Option;


// CV938CpmDlg 对话框

class V938PLANE_DLL CV938CpmDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV938CpmDlg)

public:
	CV938CpmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV938CpmDlg();

// 对话框数据
	enum { IDD = IDD_V938CPM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void DispUserOption(void);
	void GetUserOption(void);
	void InitUserOption(void);

private:	
    TCPM938Option    m_option;

	CButton          m_pllBypassModCheck;
	CMHexEdit        m_mclkBox;
	CButton          m_pllSetBtn;
	CButton          m_clkStartBtn;
	CButton          m_clkStopBtn;
	CButton          m_clkResetBtn;

	CButton          m_timerSTMR;
	CMHexEdit        m_timerBaseBox;
	CMHexEdit        m_timerValBox;
	CMHexEdit        m_timerCntBox;
	CButton          m_timerStartBtn;
	CButton          m_timerStopBtn;
	CButton          m_timerRstBtn;

	CButton          m_allIntSelCheck;
	CButton          m_levelTrigCheck;
	CButton          m_lowLevelCheck;
	CButton          m_fallEdgeCheck;
	CMHexEdit        m_trigWidthBox;
	CButton          m_intLockBtn;
	CButton          m_intTestBtn;
	CListBox         m_intTypeList;

	CButton          m_dacDatFromOffChip;
    CMHexEdit        m_dacMclkFreq;
	CButton          m_dacTest;
	CButton          m_dacDeepth;
	CMHexEdit        m_dacIowl;
	CButton          m_dacNrst;
	CButton          m_dacSb;
	CButton          m_dacSbDac;
	CButton          m_dacMute;

	CMHexEdit        m_iisDiv2Box;
	CMHexEdit        m_iisDiv3Box;
	CButton          m_iisMasterModChk;
	CButton          m_iisMclkToExternChk;
	CButton          m_iisSclkPolyInvChk;
	CButton          m_iisSetBtn;

	CButton          m_gpioFuncSelRadio;
	CButton          m_gpioFuncSelRadio2;
	CButton          m_gpioFuncSelRadio3;
	CButton          m_gpioFuncSelRadio4;
	CComboBox        m_gpioPinIdxComBox;
	CButton          m_gpioCfgChk;
	CButton          m_gpioModChk;
	CButton          m_gpioDirChk;
	CButton          m_gpioSetBtn;
    CMHexEdit        m_gpioRdBkBox;
	CButton          m_gpioReadBtn;

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClicked938cpmpllsetButton();
	afx_msg void OnBnClicked938cpmtestinitButton();
	afx_msg void OnBnClicked938cpmintleveltrigCheck();
	afx_msg void OnBnClicked938cpmclkstartButton();
	afx_msg void OnBnClicked938cpmclkstopButton();
	afx_msg void OnBnClicked938cpmclkrstButton();
	afx_msg void OnBnClicked938cpmintlockButton();
	afx_msg void OnBnClicked938cpminttestButton();
	afx_msg void OnBnClicked938cpmintallselCheck();
	afx_msg void OnBnClicked938cpmtimerstartButton();
	afx_msg void OnBnClicked938cpmtimerstopButton();
	afx_msg void OnBnClicked938cpmtimerresetButton();
	afx_msg void OnBnClicked938cpmpllbymodeCheck();
	afx_msg void OnBnClicked938cpmrefreshButton();
	afx_msg void OnBnClickedDacsetButton();
	afx_msg void OnBnClickedIissetButton();
	afx_msg void OnBnClickedGpioset938Button();
	afx_msg void OnBnClickedGpioreadback938Button();
};

#endif
