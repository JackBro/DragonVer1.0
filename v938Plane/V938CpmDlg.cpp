// V568UsbcDlg.cpp : 实现文件
//

#include "internal.h"


#ifndef	V938CMD_DISABLE


// CV938CpmDlg 对话框

IMPLEMENT_DYNAMIC(CV938CpmDlg, CBaseCtrlDlg)
CV938CpmDlg::CV938CpmDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV938CpmDlg::IDD, pParent)
{
}

CV938CpmDlg::~CV938CpmDlg()
{
}

void CV938CpmDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_938CPMPLLBYMODE_CHECK, m_pllBypassModCheck);
	DDX_Control(pDX, IDC_938CPMMCLK_EDIT, m_mclkBox);
	DDX_Control(pDX, IDC_938CPMPLLSET_BUTTON, m_pllSetBtn);
	DDX_Control(pDX, IDC_938CPMCLKSTART_BUTTON, m_clkStartBtn);
	DDX_Control(pDX, IDC_938CPMCLKSTOP_BUTTON, m_clkStopBtn);
	DDX_Control(pDX, IDC_938CPMCLKRST_BUTTON, m_clkResetBtn);

	DDX_Control(pDX, IDC_938CPMTIMERTYPE_CHECK, m_timerSTMR);
	DDX_Control(pDX, IDC_938CPMTIMERBASE_EDIT, m_timerBaseBox);
	DDX_Control(pDX, IDC_938CPMTIMERVAL_EDIT, m_timerValBox);
	DDX_Control(pDX, IDC_938CPMTIMERCNT_EDIT, m_timerCntBox);
    DDX_Control(pDX, IDC_938CPMTIMERSTART_BUTTON, m_timerStartBtn);
	DDX_Control(pDX, IDC_938CPMTIMERSTOP_BUTTON, m_timerStopBtn);
	DDX_Control(pDX, IDC_938CPMTIMERRESET_BUTTON, m_timerRstBtn);

	DDX_Control(pDX, IDC_938CPMINTALLSEL_CHECK, m_allIntSelCheck);
	DDX_Control(pDX, IDC_938CPMINTLEVELTRIG_CHECK, m_levelTrigCheck);
	DDX_Control(pDX, IDC_938CPMINTLOWLEVEL_CHECK, m_lowLevelCheck);
	DDX_Control(pDX, IDC_938CPMINTFALLTRIG_CHECK, m_fallEdgeCheck);
	DDX_Control(pDX, IDC_938CPMTRIGWIDTH_EDIT, m_trigWidthBox);
	DDX_Control(pDX, IDC_938CPMINTLOCK_BUTTON, m_intLockBtn);
	DDX_Control(pDX, IDC_938CPMINTTEST_BUTTON, m_intTestBtn);
	DDX_Control(pDX, IDC_INTTYPE_LIST, m_intTypeList);

	DDX_Control(pDX, IDC_DACDATASRCOFFCHIP_CHECK, m_dacDatFromOffChip);
    DDX_Control(pDX, IDC_DACMCLKFREQ_EDIT, m_dacMclkFreq);
    DDX_Control(pDX, IDC_DACTEST_CHECK, m_dacTest);
    DDX_Control(pDX, IDC_DACDEEPTH_CHECK, m_dacDeepth);
	DDX_Control(pDX, IDC_DACIOWL_EDIT, m_dacIowl);
	DDX_Control(pDX, IDC_DACNRST_CHECK, m_dacNrst);
	DDX_Control(pDX, IDC_DACSB_CHECK, m_dacSb);
    DDX_Control(pDX, IDC_DACSBDAC_CHECK, m_dacSbDac);
    DDX_Control(pDX, IDC_DACMUTE_CHECK, m_dacMute);

	DDX_Control(pDX, IDC_IISDIV2_EDIT, m_iisDiv2Box);
	DDX_Control(pDX, IDC_IISDIV3_EDIT, m_iisDiv3Box);
	DDX_Control(pDX, IDC_IISMASTERMODE_CHECK, m_iisMasterModChk);
	DDX_Control(pDX, IDC_IISMCLKTOEXTERN_CHECK, m_iisMclkToExternChk);
	DDX_Control(pDX, IDC_IISSCLKINVERT_CHECK, m_iisSclkPolyInvChk);
	DDX_Control(pDX, IDC_IISSET_BUTTON, m_iisSetBtn);
	
	DDX_Control(pDX, IDC_GPIOFUNCSEL938_RADIO, m_gpioFuncSelRadio);
	DDX_Control(pDX, IDC_GPIOFUNCSEL938_RADIO2, m_gpioFuncSelRadio2);
	DDX_Control(pDX, IDC_GPIOFUNCSEL938_RADIO3, m_gpioFuncSelRadio3);
	DDX_Control(pDX, IDC_GPIOFUNCSEL938_RADIO4, m_gpioFuncSelRadio4);

	DDX_Control(pDX, IDC_GPIOPINIDX938_COMBO, m_gpioPinIdxComBox);
	DDX_Control(pDX, IDC_GPIOCFG_CHECK, m_gpioCfgChk);
	DDX_Control(pDX, IDC_GPIOMOD938_CHECK, m_gpioModChk);
	DDX_Control(pDX, IDC_GPIODIR938_CHECK, m_gpioDirChk);
	DDX_Control(pDX, IDC_GPIOSET938_BUTTON, m_gpioSetBtn);
	DDX_Control(pDX, IDC_GPIOREADBACKVAL938_EDIT, m_gpioRdBkBox);
	DDX_Control(pDX, IDC_GPIOREADBACK938_BUTTON, m_gpioReadBtn);

}


BEGIN_MESSAGE_MAP(CV938CpmDlg, CBaseCtrlDlg)

	ON_BN_CLICKED(IDC_938CPMPLLSET_BUTTON, OnBnClicked938cpmpllsetButton)
	ON_BN_CLICKED(IDC_938CPMTESTINIT_BUTTON, OnBnClicked938cpmtestinitButton)
	ON_BN_CLICKED(IDC_938CPMINTLEVELTRIG_CHECK, OnBnClicked938cpmintleveltrigCheck)
	ON_BN_CLICKED(IDC_938CPMCLKSTART_BUTTON, OnBnClicked938cpmclkstartButton)
	ON_BN_CLICKED(IDC_938CPMCLKSTOP_BUTTON, OnBnClicked938cpmclkstopButton)
	ON_BN_CLICKED(IDC_938CPMCLKRST_BUTTON, OnBnClicked938cpmclkrstButton)
	ON_BN_CLICKED(IDC_938CPMINTLOCK_BUTTON, OnBnClicked938cpmintlockButton)
	ON_BN_CLICKED(IDC_938CPMINTTEST_BUTTON, OnBnClicked938cpminttestButton)
	ON_BN_CLICKED(IDC_938CPMINTALLSEL_CHECK, OnBnClicked938cpmintallselCheck)
	ON_BN_CLICKED(IDC_938CPMTIMERSTART_BUTTON, OnBnClicked938cpmtimerstartButton)
	ON_BN_CLICKED(IDC_938CPMTIMERSTOP_BUTTON, OnBnClicked938cpmtimerstopButton)
	ON_BN_CLICKED(IDC_938CPMTIMERRESET_BUTTON, OnBnClicked938cpmtimerresetButton)
	ON_BN_CLICKED(IDC_938CPMPLLBYMODE_CHECK, OnBnClicked938cpmpllbymodeCheck)
	ON_BN_CLICKED(IDC_938CPMREFRESH_BUTTON, OnBnClicked938cpmrefreshButton)
	ON_BN_CLICKED(IDC_DACSET_BUTTON, OnBnClickedDacsetButton)
	ON_BN_CLICKED(IDC_IISSET_BUTTON, OnBnClickedIissetButton)
	ON_BN_CLICKED(IDC_GPIOSET938_BUTTON, OnBnClickedGpioset938Button)
	ON_BN_CLICKED(IDC_GPIOREADBACK938_BUTTON, OnBnClickedGpioreadback938Button)

END_MESSAGE_MAP()


// CV938CpmDlg 消息处理程序
void CV938CpmDlg::InitUserOption(void)
{
	char i;

	const char *pinIdx[] = 
	{
		"Pin 0",
		"Pin 1",
		"Pin 2",
		"Pin 3",
		"Pin 4",
		"Pin 5",
		"Pin 6",
		"Pin 7"
	};
	
	memset(&(this->m_option), 0, sizeof(this->m_option));
	this->m_option.timerBase = 0x2ee0;
	this->m_option.mclk = 24000000;
	this->m_option.trigerType = 1;
	this->m_option.lowLevelTrig = 1;
	this->m_option.trigWidth = 2;
	this->m_option.sb = 1;
	this->m_option.sbDac = 1;
	this->m_option.mute = 1;
	this->m_option.gpioCfg = 1;
	
    for(i=0; i< gIntNum938; i++)
	{
		m_intTypeList.InsertString(i, LPCTSTR(gCpmIntType[i]));
	}

	for(i = 0; i < sizeof(pinIdx) / sizeof(char *); i++)
		this->m_gpioPinIdxComBox.InsertString(i, pinIdx[i]);
	this->m_gpioPinIdxComBox.SetCurSel(0);

}

void CV938CpmDlg::DispUserOption(void)
{
	char str[11] = {0};
	char i;
	unsigned int val;

	/*clock*/
	this->m_pllBypassModCheck.SetCheck(this->m_option.pllMode);

	val = this->m_option.mclk;
	sprintf(str, "%d", val);
	this->m_mclkBox.SetWindowText(str);


	/*timer*/
	this->m_timerSTMR.SetCheck(this->m_option.STRMTimer);

	val = this->m_option.timerBase;
	sprintf(str, "%d", val);
	this->m_timerBaseBox.SetWindowText(str);

	val = this->m_option.timerVal;
	sprintf(str, "%d", val);
	this->m_timerValBox.SetWindowText(str);

	val = this->m_option.timerCnt;
	sprintf(str, "%d", val);
	this->m_timerCntBox.SetWindowText(str);

	this->m_timerCntBox.EnableWindow(FALSE);

    /*interrupt*/
	this->m_allIntSelCheck.SetCheck(this->m_option.allIntSelected);
	this->m_levelTrigCheck.SetCheck(this->m_option.trigerType);
	this->m_lowLevelCheck.SetCheck(this->m_option.lowLevelTrig);
	this->m_fallEdgeCheck.SetCheck(this->m_option.fallEdgeTrig);

	i = this->m_option.trigWidth;
	sprintf(str, "%d", i);
	this->m_trigWidthBox.SetWindowText(str);

	if(this->m_option.trigerType)/*level triger*/
	{
		this->m_lowLevelCheck.EnableWindow(TRUE);
		this->m_fallEdgeCheck.EnableWindow(FALSE);
	}
	else/*edge triger*/
	{
		this->m_lowLevelCheck.EnableWindow(FALSE);
		this->m_fallEdgeCheck.EnableWindow(TRUE);
	}

	/* DAC Setting*/
	this->m_dacDatFromOffChip.SetCheck(this->m_option.datFromOffChip);

	i = this->m_option.mclkFreq;
	sprintf(str, "%d", i);
	this->m_dacMclkFreq.SetWindowText(str);

	this->m_dacTest.SetCheck(this->m_option.dacTest);
	this->m_dacDeepth.SetCheck(this->m_option.deepth);

	i = this->m_option.iowl;
	sprintf(str, "%d", i);
	this->m_dacIowl.SetWindowText(str);

	this->m_dacNrst.SetCheck(this->m_option.nrst);
	this->m_dacSb.SetCheck(this->m_option.sb);
	this->m_dacSbDac.SetCheck(this->m_option.sbDac);
	this->m_dacMute.SetCheck(this->m_option.mute);

	/* IIS Setting*/
	i = this->m_option.iisDiv2;
	sprintf(str, "%d", i);
	this->m_iisDiv2Box.SetWindowText(str);

	i = this->m_option.iisDiv3;
	sprintf(str, "%d", i);
	this->m_iisDiv3Box.SetWindowText(str);

	this->m_iisMasterModChk.SetCheck(this->m_option.iisMasterMod);
	this->m_iisMclkToExternChk.SetCheck(this->m_option.iisMclkToExtern);
	this->m_iisSclkPolyInvChk.SetCheck(this->m_option.iisSclkPolyInv);

	/* GPIO Setting */
	if(this->m_option.gpioFuncSel == 0)
	{
		this->m_gpioFuncSelRadio.SetCheck(0x1);
		this->m_gpioFuncSelRadio.SetCheck(0x0);
		this->m_gpioFuncSelRadio.SetCheck(0x0);
		this->m_gpioFuncSelRadio.SetCheck(0x0);
	}
	else if(this->m_option.gpioFuncSel == 1)
	{
		this->m_gpioFuncSelRadio.SetCheck(0x0);
		this->m_gpioFuncSelRadio.SetCheck(0x1);
		this->m_gpioFuncSelRadio.SetCheck(0x0);
		this->m_gpioFuncSelRadio.SetCheck(0x0);
	}
	else if(this->m_option.gpioFuncSel == 2)
    {
		this->m_gpioFuncSelRadio.SetCheck(0x0);
		this->m_gpioFuncSelRadio.SetCheck(0x0);
		this->m_gpioFuncSelRadio.SetCheck(0x1);
		this->m_gpioFuncSelRadio.SetCheck(0x0);
	}
	else if(this->m_option.gpioFuncSel == 8)
    {
		this->m_gpioFuncSelRadio.SetCheck(0x0);
		this->m_gpioFuncSelRadio.SetCheck(0x0);
		this->m_gpioFuncSelRadio.SetCheck(0x0);
		this->m_gpioFuncSelRadio.SetCheck(0x1);
	}

	this->m_gpioPinIdxComBox.SetCurSel(this->m_option.gpioIdx);
	this->m_gpioCfgChk.SetCheck(this->m_option.gpioCfg);
	this->m_gpioDirChk.SetCheck(this->m_option.gpioDir);
	this->m_gpioModChk.SetCheck(this->m_option.gpioMod);

	i = this->m_option.gpioRdBkVal;
	sprintf(str, "%d", i);
	this->m_gpioRdBkBox.SetWindowText(str);

}

void CV938CpmDlg::GetUserOption(void)
{
	int x;
	char str[20] = {0};

	/*clock*/
	this->m_option.pllMode = (char)this->m_pllBypassModCheck.GetCheck();

	this->m_mclkBox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.mclk = (unsigned int)x;

	/*timer*/
	this->m_option.STRMTimer = (char)this->m_timerSTMR.GetCheck();

	this->m_timerBaseBox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.timerBase = (unsigned short)x;

	this->m_timerValBox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.timerVal = (unsigned int)x;

	this->m_timerCntBox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.timerCnt = (unsigned int)x;

	/*interrupt*/
	this->m_option.allIntSelected = (char)this->m_allIntSelCheck.GetCheck();
	this->m_option.trigerType     = (char)this->m_levelTrigCheck.GetCheck();
	this->m_option.lowLevelTrig   = (char)this->m_lowLevelCheck.GetCheck();
	this->m_option.fallEdgeTrig   = (char)this->m_fallEdgeCheck.GetCheck();

	this->m_trigWidthBox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.trigWidth = (char)x;

	/* DAC Setting */
	this->m_option.datFromOffChip = (char)this->m_dacDatFromOffChip.GetCheck();
	
	this->m_dacMclkFreq.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	if(x > 3)
		x = 0;
	this->m_option.mclkFreq = (char)x;

	this->m_option.dacTest = (char)this->m_dacTest.GetCheck();
	this->m_option.deepth = (char)this->m_dacDeepth.GetCheck();

	this->m_dacIowl.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	if(x > 3)
		x = 0;
	this->m_option.iowl = (char)x;

	this->m_option.nrst = (char)this->m_dacNrst.GetCheck();
	this->m_option.sb = (char)this->m_dacSb.GetCheck();
	this->m_option.sbDac = (char)this->m_dacSbDac.GetCheck();
	this->m_option.mute = (char)this->m_dacMute.GetCheck();

	/* IIS Setting */
	this->m_iisDiv2Box.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	if(x > 3)
		x = 0;
	this->m_option.iisDiv2 = (char)x;

	this->m_iisDiv3Box.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	if(x > 3)
		x = 0;
	this->m_option.iisDiv3 = (char)x;

	this->m_option.iisMasterMod = (char)this->m_iisMasterModChk.GetCheck();
	this->m_option.iisMclkToExtern = (char)this->m_iisMclkToExternChk.GetCheck();
	this->m_option.iisSclkPolyInv = (char)this->m_iisSclkPolyInvChk.GetCheck();

	/* GPIO Setting */
	if(this->m_gpioFuncSelRadio.GetCheck())
	{
		this->m_option.gpioFuncSel = 0;
	}
	else if(this->m_gpioFuncSelRadio2.GetCheck())
	{
		this->m_option.gpioFuncSel = 1;
	}
	else if(this->m_gpioFuncSelRadio3.GetCheck())
	{
		this->m_option.gpioFuncSel = 2;
	}
	else if(this->m_gpioFuncSelRadio4.GetCheck())
	{
		this->m_option.gpioFuncSel = 4;
	}

	this->m_option.gpioIdx = (char)this->m_gpioPinIdxComBox.GetCurSel();
	this->m_option.gpioCfg = (char)this->m_gpioCfgChk.GetCheck();
	this->m_option.gpioDir = (char)this->m_gpioDirChk.GetCheck();
	this->m_option.gpioMod = (char)this->m_gpioModChk.GetCheck();

	this->m_gpioRdBkBox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.mclkFreq = (unsigned char)x;

}

void CV938CpmDlg::OnBnClicked938cpmpllbymodeCheck()
{
    this->m_option.pllMode = (char)this->m_pllBypassModCheck.GetCheck();
}

void CV938CpmDlg::OnBnClicked938cpmpllsetButton()
{
	this->GetUserOption( );

	V9_CtrlSetPll((UINT32)this->m_option.mclk);
}

void CV938CpmDlg::OnBnClicked938cpmclkstartButton()
{
	this->GetUserOption( );

	V9_CtrlStartClk(this->m_option.pllMode);
}

void CV938CpmDlg::OnBnClicked938cpmclkstopButton()
{
	V9_CtrlStopClk();
}

void CV938CpmDlg::OnBnClicked938cpmclkrstButton()
{
	V9_CtrlResetClk();
}

void CV938CpmDlg::OnBnClicked938cpmtestinitButton()
{
	V9_ChipCtrlOpen();
	testIntInit938();
}

BOOL CV938CpmDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispUserOption();

	return TRUE;  
}

void CV938CpmDlg::OnBnClicked938cpmintleveltrigCheck()
{
	if(this->m_levelTrigCheck.GetCheck())/*level triger*/
	{
		this->m_lowLevelCheck.EnableWindow(TRUE);
		this->m_fallEdgeCheck.EnableWindow(FALSE);
	}
	else/*edge triger*/
	{
		this->m_lowLevelCheck.EnableWindow(FALSE);
		this->m_fallEdgeCheck.EnableWindow(TRUE);
	}
}

void CV938CpmDlg::OnBnClicked938cpmintallselCheck()
{
	this->GetUserOption( );
	
	if(this->m_option.allIntSelected)
	{
		this->m_intTypeList.EnableWindow(FALSE);
	}
	else
	{
		this->m_intTypeList.EnableWindow(TRUE);
	}
}

void CV938CpmDlg::OnBnClicked938cpmintlockButton()
{
	if(this->m_option.intLocked == 0)
	{
		this->m_option.intLocked = 1;
	    V9_CtrlSetIntLock();
		this->m_intLockBtn.SetWindowText("UnLock");
	}
	else
	{
		this->m_option.intLocked = 0;
        V9_CtrlClearIntLock();
		this->m_intLockBtn.SetWindowText("Lock");
	}
}

void CV938CpmDlg::OnBnClicked938cpminttestButton()
{
	CWaitCursor	cursor;
	UINT8 i;
	

	this->GetUserOption( );

	V9_CtrlIntDisable(CTRL_INT_ALL);

	V9_CtrlSetIntCtrl((UINT8)m_option.trigerType, (UINT8)m_option.lowLevelTrig, (UINT8)m_option.fallEdgeTrig, (UINT8)m_option.trigWidth);

	//testIntInit938();

	if(this->m_option.allIntSelected)
	{
		V9_CtrlIntEnable(CTRL_INT_ALL);
	}
	else
	{
		for(i=0; i< gIntNum938; i++)
		{
			if(m_intTypeList.GetSel(i))
			{
				V9_CtrlIntEnable(i);
			}
		}
	}

}

void CV938CpmDlg::OnBnClicked938cpmtimerstartButton()
{
	this->GetUserOption( );

	V9_CtrlSetTmrBase(this->m_option.timerBase);

	if(this->m_option.STRMTimer)
	{
		V9_CtrlSetSTmrTimer((UINT8)this->m_option.timerVal);
	}
	else
	{
		V9_CtrlSetTmrTimer(this->m_option.timerVal);
	}
}

void CV938CpmDlg::OnBnClicked938cpmtimerstopButton()
{
	this->GetUserOption( );

	if(this->m_option.STRMTimer)
	{
		this->m_option.timerCnt = V9_CtrlGetSTmrTimer();
		V9_CtrlStopSTmrTimer();
	}
	else
	{
		this->m_option.timerCnt = V9_CtrlGetTmrTimer();
		V9_CtrlStopTmrTimer();
	}

	this->DispUserOption( );
}

void CV938CpmDlg::OnBnClicked938cpmtimerresetButton()
{
    this->GetUserOption( );

	if(this->m_option.STRMTimer)
	{
		V9_CtrlRstTmrTimer();
	}
	else
	{
		V9_CtrlRstSTmrTimer();
	}
}

void CV938CpmDlg::OnBnClicked938cpmrefreshButton()
{
	this->DispUserOption();
}

void CV938CpmDlg::OnBnClickedDacsetButton()
{
	this->GetUserOption( );

	V9B_CpmSetDacCfg((this->m_option.datFromOffChip << 6) | (this->m_option.mclkFreq << 4));
	V9B_CpmSetDacCtrl((this->m_option.dacTest << 7) | (this->m_option.deepth << 6) | (this->m_option.iowl << 4)
		| (this->m_option.nrst << 3) | (this->m_option.sb << 2) | (this->m_option.sbDac << 1) |this->m_option.mute);
}


void CV938CpmDlg::OnBnClickedIissetButton()
{
	this->GetUserOption( );

	V9B_CpmSetIisCfg((this->m_option.iisDiv2 << 6) |(this->m_option.iisDiv3 << 4) | (this->m_option.iisMasterMod << 2)
		| (this->m_option.iisMclkToExtern << 1) | this->m_option.iisSclkPolyInv);
}

void CV938CpmDlg::OnBnClickedGpioset938Button()
{
	unsigned char val;

    this->GetUserOption( );

	V9B_CpmSetP0FuncMod(this->m_option.gpioFuncSel);

	val =  V9B_CpmGetGpioCfg();
	val &= ~(0x1 << this->m_option.gpioIdx);
	val |= this->m_option.gpioCfg << this->m_option.gpioIdx;
	V9B_CpmSetGpioCfg(val);

	val =  V9B_CpmGetP0Mod();
	val &= ~(0x1 << this->m_option.gpioIdx);
	val |= this->m_option.gpioMod << this->m_option.gpioIdx;
	V9B_CpmSetP0Mod(val);

	val =  V9B_CpmGetP0Dir();
	val &= ~(0x1 << this->m_option.gpioIdx);
	val |= this->m_option.gpioDir << this->m_option.gpioIdx;
	V9B_CpmSetP0Dir(val);
}

void CV938CpmDlg::OnBnClickedGpioreadback938Button()
{
	this->m_option.gpioRdBkVal = V9B_CpmGetPort0();
	
	this->DispUserOption();
}

#else

void null938CmpDlg(void)
{
}

#endif //V938CMD_DISABLE


