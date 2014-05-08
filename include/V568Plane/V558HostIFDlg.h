#ifndef __V568PLANE_V558HOSTIFDLG_H__
#define __V568PLANE_V558HOSTIFDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MHexEdit.h"


// CV558HostIFDlg 对话框

/****** define initial and end register address of all module of 568*****/
#define BIU_REG_INIT_ADDR 0x1802

#define BIU_REG_END_ADDR  0x18fa

#define GE_REG_INIT_ADDR 0x1000
#define GE_REG_END_ADDR  0x103b

#define SIF_REG_INIT_ADDR 0x800
#define SIF_REG_END_ADDR  0x88d

#define CTRL_REG_INIT_ADDR 0x1400
#define CTRL_REG_END_ADDR  0x1444

#define ISP_REG_INIT_ADDR 0x2000
#define ISP_REG_END_ADDR  0x2175

#define IPP_REG_INIT_ADDR 0x1c00
#define IPP_REG_END_ADDR  0x1c31

#define LCDIF_REG_INIT_ADDR 0x400
#define LCDIF_REG_END_ADDR  0x45f

#define JPEG_REG_INIT_ADDR 0x0
#define JPEG_REG_END_ADDR  0x4d

#define LBUF_REG_INIT_ADDR 0xc00
#define LBUF_REG_END_ADDR  0xc15

#define LCDC_REG_INIT_ADDR 0x2800
#define LCDC_REG_END_ADDR  0x28cc

#define MARB_REG_INIT_ADDR 0x2400
#define MARB_REG_END_ADDR  0x2483


typedef struct tag_THostIFOption
{
	/*file name*/
	char	        srcfile[DEFAULT_FILENAME_LEN];
	char	        dstfile[DEFAULT_FILENAME_LEN];

	/*delay setting*/
	int             delayAddr;
	int             delayOe;
	int             delayDatain;
	int             delayDataout;
	
	/*sram*/
	int             busTypeSel;
	int             sramAddr;
	int             sramSize;
	int             totalTime;
	int             byteTime;
	char            bSramTimeW;
	char            bPortAcess;
	
	/*continous register read*/
	unsigned int    initRegAddr;
    unsigned int    endRegAddr;
	int             allRegTst;

	/*bypass*/
	char            bypassSelected;
	char            bypassSelByGPIO;
	char            lcdType;
	int             lcdCtlVal;
	unsigned char   bypassStartCoorX;
	unsigned char   bypassStartCoorY;
	unsigned char   bypassEndCoorX;	
	unsigned char   bypassEndCoorY;
	unsigned short  bypassLineColor;	

	/*bus speed*/
	int             busSpeed;

} THostIFOption, *PTHostIFOption;

class V568PLANE_DLL CV558HostIFDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV558HostIFDlg)

public:
	CV558HostIFDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV558HostIFDlg();

public:
	void InitUserOption(void); 
	void DispUserOption(void);
	void GetUserOption(void);
	void GetComboValue(void);
	void GetEditBoxValue(void);
	void GetCheckAndRadioBtnValue(void);

	virtual BOOL OnInitDialog();
	
// 对话框数据
	enum { IDD = IDD_V558HOSTIF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	THostIFOption   m_option;
	CMHexEdit       m_bypassStartCoorBoxX;
	CMHexEdit       m_bypassStartCoorBoxY;
	CMHexEdit       m_bypassEndCoorBoxX;
	CMHexEdit       m_bypassLineColorBox;
	CMHexEdit       m_CtrlSramAddr;
	CMHexEdit       m_CtrlSramSize;	
	CMHexEdit       m_sramTotalTimeBox;
	CMHexEdit       m_sramByteTimeBox;	
	CMHexEdit       m_delayAddrBox;
	CMHexEdit       m_delayInBox;
	CMHexEdit       m_delayOutBox;
	CMHexEdit       m_delayOeBox;	
	CMHexEdit       m_initRegAddrBox;
	CMHexEdit       m_endRegAddrBox;
	CMHexEdit       m_busSpeedBox;
	CButton         m_allRegTstCheck;
	CButton         m_sramWriteCheck;
	CButton         m_buTypeRadioMul8;
	CButton         m_buTypeRadioMul16;
	CButton         m_buTypeRadioSep;
	CButton         m_noPortCheck;
	CButton         m_bypassModeCheck;
	CButton         m_bypassSelByGPIOCheck;
    CButton         m_LCD16Check;
	CButton         m_bypassInitBtn;
	CButton         m_bypassCsChangeBtn;
	CButton         m_bypassDrawLineBtn;
	CButton         m_bypassDrawBmpBtn;

public:
	afx_msg void OnBnClickedButtonTestDelaysel();
	afx_msg void OnBnClickedRadioMul8Hostif();
	afx_msg void OnBnClickedRadioMul16Hostif();
	afx_msg void OnBnClickedRadioSepHostif();	
	afx_msg void OnBnClickedBackwriteButton();
	afx_msg void OnBnClickedBypassmodeCheck();
	afx_msg void OnBnClickedNoportCheck();
	afx_msg void OnBnClickedSramwriteCheck();
	afx_msg void OnBnClickedBypassinitButton();
	afx_msg void OnBnClickedBypassdrawlineButton();
	afx_msg void OnBnClickedBypassdrawbmpButton();
	afx_msg void OnBnClickedBypasscschangeButton();
	afx_msg void OnBnClickedButtonTestRegconread();
	afx_msg void OnBnClickedAllregtestCheck();
	afx_msg void OnBnClickedGpioselbypassCheck();
	afx_msg void OnBnClickedHostifrefreshButton();
	afx_msg void OnBnClickedHostifbusspeedtestButton();
};

#endif
