	// ./CtrlPanelDlg/V568CpmDlg.cpp : 实现文件
//
#include "internal.h"
#include "..\include\v568plane\v568cpmdlg.h"


#ifndef	V578CMD_DISABLE




// CV568CpmDlg 对话框

IMPLEMENT_DYNAMIC(CV568CpmDlg, CBaseCtrlDlg)
CV568CpmDlg::CV568CpmDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568CpmDlg::IDD, pParent)
	, m_strListIntType(_T(""))
{
}

CV568CpmDlg::~CV568CpmDlg()
{
}

void CV568CpmDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INT_TYPE, m_CtrlListIntType);
	DDX_LBString(pDX, IDC_LIST_INT_TYPE, m_strListIntType);
	DDX_Control(pDX, IDC_EDIT_CLKIN, m_CtrlClkin);
	DDX_Control(pDX, IDC_EDIT_MCLK, m_CtrlMclk);
	DDX_Control(pDX, IDC_EDIT_M, m_CtrlM);
	DDX_Control(pDX, IDC_EDIT_N, m_CtrlN);
	DDX_Control(pDX, IDC_EDIT_OD0, m_CtrlOd0);
	DDX_Control(pDX, IDC_EDIT_OD1, m_CtrlOd1);
	DDX_Control(pDX, IDC_EDIT_CLKDIV, m_CtrlDiv);
	DDX_Control(pDX, IDC_LIST_CLOCK, m_CtrlListClkType);
	DDX_Control(pDX, IDC_LIST_RESET, m_CtrlListRstType);
	DDX_Control(pDX, IDC_GPIOCONFIG_EDIT, m_gpioconfigbox);
	DDX_Control(pDX, IDC_GPIOMODE_EDIT, m_gpiomodbox);
	DDX_Control(pDX, IDC_GPIODIR_EDIT, m_gpiodirbox);
	DDX_Control(pDX, IDC_GPIOWRITEVALUE_EDIT, m_gpiowrvalbox);
	DDX_Control(pDX, IDC_GPIOREADVALUE_EDIT, m_gpiordvalbox);
	DDX_Control(pDX, IDC_GPIOPINIDX_COMBO, m_GPIOPinIdxComBox);
	DDX_Control(pDX, IDC_EDIT_INT_WIDTH, m_IntSigValWidthBox);
	DDX_Control(pDX, IDC_CHECK_INTALL, m_IntAllBtn);
	DDX_Control(pDX, IDC_CHECK_CLK_ALL, m_ClkAllBtn);
	DDX_Control(pDX, IDC_CHECK_RESET_ALL, m_ResetAllBtn);
	DDX_Control(pDX, IDC_CHECK_CLK_AUTO, m_ClkAutoBtn);
	DDX_Control(pDX, IDC_CHECK_CLK_MONITOR, m_ClkMonitorBtn);
	DDX_Control(pDX, IDC_LEVELTRIGGER_CHECK, m_intLevelTrig);
	DDX_Control(pDX, IDC_LEVELTRIGGERLOW_CHECK, m_intLevelTrigLowAct);
	DDX_Control(pDX, IDC_EDGETRIGGERLOW_CHECK, m_intEdgeTrigLowAct);
	DDX_Control(pDX, IDC_EDIT_TIME, m_EdtTime);
	DDX_Control(pDX, IDC_CHECK_TIME_MODE, m_ChkTimeMode);
	DDX_Control(pDX, IDC_BUTTON_CLK_SET2, m_BtnStTime);
}


BEGIN_MESSAGE_MAP(CV568CpmDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_BUTTON_CPM_INIT, OnBnClickedButtonCpmInit)
	ON_BN_CLICKED(IDC_CHECK_INTALL, OnBnClickedCheckIntall)
	ON_BN_CLICKED(IDC_BUTTON_TEST_INT, OnBnClickedButtonTestInt)
	ON_BN_CLICKED(IDC_CHECK_CLK_AUTO, OnBnClickedCheckClkAuto)
	ON_BN_CLICKED(IDC_BUTTON_TEST_CLOCK, OnBnClickedButtonTestClock)
	ON_BN_CLICKED(IDC_BUTTON_TEST_RESET, OnBnClickedButtonTestReset)
	ON_BN_CLICKED(IDC_CHECK_CLK_ALL, OnBnClickedCheckClkAll)
	ON_BN_CLICKED(IDC_CHECK_RESET_ALL, OnBnClickedCheckResetAll)
	ON_BN_CLICKED(IDC_BUTTON_RELEASE, OnBnClickedButtonRelease)
	ON_BN_CLICKED(IDC_GPIOREADVALUE_BUTTON, OnBnClickedGpioreadvalueButton)
	ON_BN_CLICKED(IDC_GPIOWRITEVALUE_BUTTON, OnBnClickedGpiowritevalueButton)
	ON_BN_CLICKED(IDC_GPIODIRSET_BUTTON, OnBnClickedGpiodirsetButton)
	ON_BN_CLICKED(IDC_GPIOMODESET_BUTTON, OnBnClickedGpiomodesetButton)
	ON_BN_CLICKED(IDC_GPIOCONFIGSET_BUTTON, OnBnClickedGpioconfigsetButton)
	ON_CBN_SELCHANGE(IDC_GPIOPINIDX_COMBO, OnCbnSelchangeGpiopinidxCombo)
	ON_BN_CLICKED(IDC_LEVELTRIGGER_CHECK, OnBnClickedLeveltriggerCheck)
	ON_BN_CLICKED(IDC_BUTTON_CLK_SET, OnBnClickedButtonClkSet)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CLK_SET2, OnBnClickedButtonClkSet2)
	ON_BN_CLICKED(IDC_CHECK_TIME_MODE, OnBnClickedCheckTimeMode)
END_MESSAGE_MAP()


// CV568CpmDlg 消息处理程序

void CV568CpmDlg::OnBnClickedButtonCpmInit()
{
	TestCtrlInit();
}

void CV568CpmDlg::OnBnClickedCheckIntall()
{
	this->GetUserOption( );
	
	if(m_option.bIntAll)
	{
		m_CtrlListIntType.EnableWindow(FALSE);
	}
	else
	{
		m_CtrlListIntType.EnableWindow();
	}
}

void CV568CpmDlg::GetCheckBtnValue(void)
{
	m_option.bIntAll      = (char)m_IntAllBtn.GetCheck( );
	m_option.IntEdgeType  = (char)m_intLevelTrig.GetCheck( );
	m_option.IntLevelType = (char)m_intLevelTrigLowAct.GetCheck( );
	m_option.IntEdgeType  = (char)m_intEdgeTrigLowAct.GetCheck( );

	m_option.bClkAll      = (char)m_ClkAllBtn.GetCheck( );

	m_option.bRstAll      = (char)m_ResetAllBtn.GetCheck( );

	m_option.bPllAuto     = (char)m_ClkAutoBtn.GetCheck( );

	this->m_option.timemode = this->m_ChkTimeMode.GetCheck();

}

void CV568CpmDlg::GetComboValue(void)
{
	m_option.gpioPinIdx = (char)m_GPIOPinIdxComBox.GetCurSel();
}

void CV568CpmDlg::GetEditboxValue(void)
{
    char str[20];
	int x;

	/*interrupt sigal valid width*/
	this->m_IntSigValWidthBox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.IntWidth = (char)x;

	/*gpio*/
	this->m_gpioconfigbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.gpioConfig = (char)x;

	this->m_gpiomodbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.gpioMod = (char)x;

	this->m_gpiodirbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.gpioDir = (char)x;

	this->m_gpiowrvalbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.gpioVal = (char)x;

	/*pll clock*/
	this->m_CtrlClkin.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.Clkin = x;

	this->m_CtrlMclk.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.Mclk = x;

	this->m_CtrlM.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.M = (char)x;

	this->m_CtrlN.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.N = (char)x;

	this->m_CtrlOd0.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.Od0 = (char)x;

	this->m_CtrlOd1.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.Od1 = (char)x;

	this->m_CtrlDiv.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.Div = (char)x;

	this->m_EdtTime.GetWindowText(str, 20);
	sscanf(str, "%d", &this->m_option.time);

}

void CV568CpmDlg::GetUserOption(void)
{
	this->GetCheckBtnValue( );
	this->GetComboValue( );
	this->GetEditboxValue( );
}

void CV568CpmDlg::InitUserOption(void)
{
    int i = 0;

	const char *pinIdx[] = 
	{
		"Pin 0",
		"Pin 1",
		"Pin 2",
		"Pin 3",
		"Pin 4"
	};

    memset( &(this->m_option), 0, sizeof(TCPMOption) );
	
	m_option.bPllAuto = 1;
	m_option.Clkin    = gCtrlInfo.clkin;
	m_option.Mclk     = gCtrlInfo.mclk;
	m_option.M        = gCtrlInfo.pll_m;
	m_option.N        = gCtrlInfo.pll_n;
	m_option.Od0      = gCtrlInfo.pll_od0;
	m_option.Od1      = gCtrlInfo.pll_od1;
	m_option.Div      = gCtrlInfo.clkoutdiv;
	
	m_option.IntTrigeType = (gCtrlInfo.intprop) & 0x1;
	m_option.IntLevelType = ((gCtrlInfo.intprop) >> 0x1) & 0x1;
	m_option.IntEdgeType  = ((gCtrlInfo.intprop) >> 0x2) & 0x1;
	m_option.IntWidth     = ((gCtrlInfo.intprop) >> 0x4) & 0x1f;

	m_option.gpioPinIdx = 0;
	m_option.gpioConfig = 1;
	m_option.gpioMod    = 1;
	m_option.gpioDir    = 0;
	m_option.gpioVal    = 1;

	for(i=0; i< gIntnum; i++)
	{
		m_CtrlListIntType.AddString(LPCTSTR(gIntisr[i].Desc));
	}

	for(i=0; i< gClknum; i++)
	{
		m_CtrlListClkType.AddString(LPCTSTR(gClkType[i]));
	}

	for(i=0; i< gRstnum; i++)
	{
		m_CtrlListRstType.AddString(LPCTSTR(gRstType[i]));
	}
   
	for(i = 0; i < sizeof(pinIdx) / sizeof(char *); i++)
		this->m_GPIOPinIdxComBox.InsertString(i, pinIdx[i]);
	this->m_GPIOPinIdxComBox.SetCurSel(0);

	m_option.timemode = 0;
	m_option.time = 100;
}

void CV568CpmDlg::DispUserOption(void)
{
	int i = 0;
	char str[10] = {0};

	/*pll clock*/
	if(m_option.bPllAuto)
	{
		m_ClkAutoBtn.SetCheck(BST_CHECKED);
	}

	i = this->m_option.Clkin;
	sprintf(str, "%d", i);
	this->m_CtrlClkin.SetWindowText(str);

	i = this->m_option.Mclk;
	sprintf(str, "%d", i);
	this->m_CtrlMclk.SetWindowText(str);

	i = this->m_option.M;
	sprintf(str, "%d", i);
	this->m_CtrlM.SetWindowText(str);

	i = this->m_option.N;
	sprintf(str, "%d", i);
	this->m_CtrlN.SetWindowText(str);

	i = this->m_option.Od0;
	sprintf(str, "%d", i);
	this->m_CtrlOd0.SetWindowText(str);


	i = this->m_option.Div;
	sprintf(str, "%d", i);
	this->m_CtrlDiv.SetWindowText(str);

	/*GPIO*/
	i = this->m_option.gpioConfig;
	sprintf(str, "%d", i);
	this->m_gpioconfigbox.SetWindowText(str);

	i = this->m_option.gpioMod;
	sprintf(str, "%d", i);
	this->m_gpiomodbox.SetWindowText(str);

	i = this->m_option.gpioDir;
	sprintf(str, "%d", i);
	this->m_gpiodirbox.SetWindowText(str);

	i = this->m_option.gpioVal;
	sprintf(str, "%d", i);
	this->m_gpiowrvalbox.SetWindowText(str);

	this->m_gpiordvalbox.SetWindowText(str);

	/*interrupt sigal valid width*/
	i = m_option.IntWidth;
    sprintf(str, "%d", i);
	this->m_IntSigValWidthBox.SetWindowText(str);

	this->m_intLevelTrig.SetCheck(this->m_option.IntTrigeType);
	i = this->m_option.Od1;
	sprintf(str, "%d", i);
	this->m_CtrlOd1.SetWindowText(str);
	this->m_intEdgeTrigLowAct.SetCheck(this->m_option.IntEdgeType);
	this->m_intLevelTrigLowAct.SetCheck(this->m_option.IntLevelType);

	if(m_option.IntTrigeType)/*0:edge,  1:level*/
	{
		m_intLevelTrigLowAct.EnableWindow(TRUE);
		m_intEdgeTrigLowAct.EnableWindow(FALSE);
	}
	else
	{
		m_intLevelTrigLowAct.EnableWindow(FALSE);
		m_intEdgeTrigLowAct.EnableWindow(TRUE);
	}


	sprintf(str, "%d", this->m_option.time);
	this->m_EdtTime.SetWindowText(str);
	this->m_ChkTimeMode.SetCheck(this->m_option.timemode);
}

BOOL CV568CpmDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption( );
    this->DispUserOption( );

	return TRUE;  
}

void CV568CpmDlg::OnBnClickedButtonTestInt()
{
	CWaitCursor	cursor;
	UINT8 i;

	this->GetUserOption( );

	V5_CtrlIntDisable(INT_ALL);

	V5_CtrlSetIntCtrl((BYTE)m_option.IntTrigeType, (BYTE)m_option.IntLevelType, (BYTE)m_option.IntEdgeType, m_option.IntWidth);

	testIntInit();

	if(m_option.bIntAll)
	{
		for(i=0; i<gIntnum; i++)
		{
			V5B_CpmSetIntEnSec(i, 0xff);
		}
	}
	else
	{
		for(i=0; i<gIntnum; i++)
		{
			V5B_CpmSetIntEnSec(i, 0);
		}

		for(i=0; i< gIntnum; i++)
		{
			if(m_CtrlListIntType.GetSel(i))
			{
				V5B_CpmSetIntEnSec(i, 0xff);
			}
		}
	}

	V5_CtrlIntEnable(INT_ALL);
}

void CV568CpmDlg::OnBnClickedButtonClkSet()
{
	CWaitCursor	cursor;
	SMIC_PLL_PARM pllparm;

	this->GetUserOption( );

	if(m_option.bPllAuto)
	{
		m_option.Mclk = V5_CtrlSetChipClk(m_option.Clkin, m_option.Mclk);
		
		//Set clk;
	    if(SMIC_PllCalculate(m_option.Clkin,  m_option.Mclk, &pllparm))
		{
	        m_option.Mclk = pllparm.clkout;
		    m_option.M    = pllparm.m;
		    m_option.N    = pllparm.n;
		    m_option.Od0  = pllparm.od0;
		    m_option.Od1  = pllparm.od1;
		    m_option.Div  = pllparm.outdiv;
		}
		else
		{
            m_option.Mclk = 0;
		    m_option.M    = 0;
		    m_option.N    = 0;
		    m_option.Od0  = 0;
		    m_option.Od1  = 0;
		    m_option.Div  = 0;
		}
	}
	else
	{
		m_option.Mclk = V5_CtrlSetChipClkDirect(m_option.Clkin, m_option.M, m_option.N, m_option.Od0, m_option.Od1, m_option.Div);
	}

	/*update gCtrlInfo parameter*/
	gCtrlInfo.clkin     = m_option.Clkin;
	gCtrlInfo.mclk      = m_option.Mclk;
	gCtrlInfo.pll_m     = m_option.M;
	gCtrlInfo.pll_n     = m_option.N;
	gCtrlInfo.pll_od0   = m_option.Od0;  
	gCtrlInfo.pll_od1   = m_option.Od1;
	gCtrlInfo.clkoutdiv = m_option.Div;


	this->DispUserOption();
}

void CV568CpmDlg::OnBnClickedCheckClkAuto()
{
	 CWaitCursor	cursor;

	this->GetUserOption( );

	if(m_option.bPllAuto)
	{
		m_CtrlM.EnableWindow(FALSE);
		m_CtrlN.EnableWindow(FALSE);
		m_CtrlOd0.EnableWindow(FALSE);
		m_CtrlOd1.EnableWindow(FALSE);
		m_CtrlDiv.EnableWindow(FALSE);
		m_CtrlMclk.EnableWindow();

	}
	else
	{
		m_CtrlM.EnableWindow();
		m_CtrlN.EnableWindow();
		m_CtrlOd0.EnableWindow();
		m_CtrlOd1.EnableWindow();
		m_CtrlDiv.EnableWindow();
		m_CtrlMclk.EnableWindow(FALSE);

	}
}

void CV568CpmDlg::OnBnClickedButtonTestClock()
{
	CWaitCursor	cursor;
	UINT8 i;

	this->GetUserOption( );

	V5_CtrlSetModClkOn(MOD_CLK_ALL);

	if(m_option.bClkAll)
	{
		V5_CtrlSetModClkOff(MOD_CLK_ALL);
	}
	else
	{
		for(i=0; i< gClknum; i++)
		{
			if(m_CtrlListClkType.GetSel(i))
			{
				V5_CtrlSetModClkOff(0x1<<i);
			}
		}
	}
}

void CV568CpmDlg::OnBnClickedButtonTestReset()
{
	CWaitCursor	cursor;
	UINT8 i;


	this->GetUserOption( );

	if(m_option.bRstAll)
	{
		V5_CtrlSwReset(SW_RESET_GLB);
	}
	else
	{
		for(i=0; i< gRstnum; i++)
		{
			if(m_CtrlListRstType.GetSel(i))
			{
				V5_CtrlSwReset(0x1<<i);
			}
		}
	}
}

void CV568CpmDlg::OnBnClickedCheckClkAll()
{
	CWaitCursor	cursor;

	this->GetUserOption( );

	if(m_option.bClkAll)
	{
		m_CtrlListClkType.EnableWindow(FALSE);
	}
	else
	{
		m_CtrlListClkType.EnableWindow();
	}
}

void CV568CpmDlg::OnBnClickedCheckResetAll()
{
	CWaitCursor	cursor;

	this->GetUserOption( );

	if(m_option.bRstAll)
	{
		m_CtrlListRstType.EnableWindow(FALSE);
	}
	else
	{
		m_CtrlListRstType.EnableWindow();
	}
}

void CV568CpmDlg::OnBnClickedButtonRefresh()
{
	this->DispUserOption();
}

void CV568CpmDlg::OnBnClickedButtonRelease()
{
	CWaitCursor	cursor;

	CString str, strdata = "";

	char version[100];

	SYSTEMTIME time;
	GetSystemTime(&time);

	V5_GetRdkVersion(version);

	strdata.Format("Release time %d/%d/%d\r\n\r\n", time.wYear, time.wMonth, time.wDay);


	CFileDialog dlg(TRUE, "txt", "releasenotes", 
					OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_PATHMUSTEXIST,
					_T("All Files (*.*)|*.*||"), this);

	dlg.m_ofn.lpstrTitle = _T("Open File");
	if (dlg.DoModal() == IDOK) 
	{
		str = dlg.GetPathName();
	}

	if(str.IsEmpty())
		return;

	CFile file;

	if (!file.Open(str, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate))
	{
		return;
	}

	DWORD chpty, mver, bver, cha;

	sscanf(version, "V5_%02d_%02d_%02d_%02d", &chpty, &mver, &bver, &cha);

	const char* chp[] = {"558","558L","568", "568L", "578"};
	const char* rel[] = {"final","belta","alpha"};

	strdata += "Current rdk version :\r\n";
	str.Format("                  %s\r\n",version);
	strdata += str;
	str.Format("	serial chip : %d\r\n", 5);
	strdata += str;
	str.Format("	chip type   : %s\r\n", chp[chpty]);
	strdata += str;
	str.Format("	rdk version : %d.%d\r\n", mver, bver);
	strdata += str;
	if(cha > 2)
		cha = 2;
	str.Format("	charactor   : %s - release\r\n",rel[cha]);
	strdata += str;

    file.Write(strdata, (UINT)strlen(strdata));

	file .Close();

}

void CV568CpmDlg::OnBnClickedGpioreadvalueButton()
{
	CWaitCursor	cursor;
	char str[20] = {0};
	char x = 0;

	/*get gpio register value*/
	this->m_option.gpioVal =  V5B_CpmGetGpioData( );
	x =  (UINT8)(this->m_option.gpioVal) & ( (UINT8)( 0x1 << (this->m_option.gpioPinIdx) ) );
    x =  x >> (this->m_option.gpioPinIdx);

    sprintf(str, "%x", x);
	this->m_gpiordvalbox.SetWindowText(str);
}

void CV568CpmDlg::OnBnClickedGpiowritevalueButton()
{
	CWaitCursor	cursor;
	char str[20] = {0};
	int  x = 0;
	UINT8 temp = 0;

	this->m_gpiowrvalbox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.gpioVal = (UINT8)x;

	temp = V5B_CpmGetGpioData( );
	if(this->m_option.gpioVal)
	{
		temp |= ( (UINT8)0x1 ) << (this->m_option.gpioPinIdx);
	}
	else
	{
		temp &= ~( ( (UINT8)0x1 ) << (this->m_option.gpioPinIdx) );
	}

    V5B_CpmSetGpioData( temp );
}

void CV568CpmDlg::OnBnClickedGpiodirsetButton()
{
	CWaitCursor	cursor;
    char str[20] = {0};
	int  x = 0;
	UINT8 temp = 0;

	this->m_gpiodirbox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.gpioDir = (UINT8)x;

	temp = V5B_CpmGetGpioDirection( );
	if(this->m_option.gpioDir)
	{
		temp |= ( (UINT8)0x1 ) << (this->m_option.gpioPinIdx);
	}
	else
	{
		temp &= ~( ( (UINT8)0x1 ) << (this->m_option.gpioPinIdx) );
	}

    V5B_CpmSetGpioDirection( temp );
}

void CV568CpmDlg::OnBnClickedGpiomodesetButton()
{
	CWaitCursor	cursor;
	char str[20] = {0};
	int  x = 0;
	UINT8 temp = 0;

	this->m_gpiomodbox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.gpioMod = (UINT8)x;
    
	temp = V5B_CpmGetGpioMode( );
	if(this->m_option.gpioMod)
	{
		temp |= ( (UINT8)0x1 ) << (this->m_option.gpioPinIdx);
	}
	else
	{
		temp &= ~( ( (UINT8)0x1 ) << (this->m_option.gpioPinIdx) );
	}

    V5B_CpmSetGpioMode( temp );
}

void CV568CpmDlg::OnBnClickedGpioconfigsetButton()
{
	CWaitCursor	cursor;
	char str[20] = {0};
	int  x = 0;
	UINT8 temp = 0;

	this->m_gpioconfigbox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.gpioConfig = (UINT8)x;

	temp = V5B_CpmGetGpioCfg( );
	if(this->m_option.gpioConfig)
	{
		temp |= ( (UINT8)0x1 ) << (this->m_option.gpioPinIdx);
	}
	else
	{
		temp &= ~( ( (UINT8)0x1 ) << (this->m_option.gpioPinIdx) );
	}

    V5B_CpmSetGpioCfg( temp );
}

void CV568CpmDlg::OnCbnSelchangeGpiopinidxCombo()
{
	CWaitCursor cursor;
	int x = 0;

	x = this->m_GPIOPinIdxComBox.GetCurSel();
	this->m_option.gpioPinIdx = (char)x;
}

void CV568CpmDlg::OnBnClickedLeveltriggerCheck()
{
    CWaitCursor cursor;

	m_option.IntTrigeType = (char)m_intLevelTrig.GetCheck( );

	if(m_option.IntTrigeType)/*0:edge,  1:level*/
	{
		m_intLevelTrigLowAct.EnableWindow(TRUE);
		m_intEdgeTrigLowAct.EnableWindow(FALSE);
	}
	else
	{
		m_intLevelTrigLowAct.EnableWindow(FALSE);
		m_intEdgeTrigLowAct.EnableWindow(TRUE);
	}
}

void CV568CpmDlg::OnBnClickedButtonClkSet2()
{
	CWaitCursor	cursor;
	static BOOL bSt = FALSE;

	this->GetUserOption( );

	if(!bSt) {
		bSt = TRUE;
		V5_MarbSetTimerCfg((UINT8)m_option.timemode, m_option.time);
		V5B_MarbStartTimer();
		this->m_BtnStTime.SetWindowText("stop");
	}
	else {
		bSt = FALSE;
		V5B_MarbStopTimer();
		this->m_BtnStTime.SetWindowText("start");
	}
}

void CV568CpmDlg::OnBnClickedCheckTimeMode()
{
}
#endif //V578CMD_DISABLE



