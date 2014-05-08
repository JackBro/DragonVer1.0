// V568SifDlg.cpp : 实现文件
//

#include "internal.h"
#include "..\include\v568plane\v568sifdlg.h"

#ifndef	V578CMD_DISABLE

// CV568SifDlg 对话框

IMPLEMENT_DYNAMIC(CV568SifDlg, CBaseCtrlDlg)
CV568SifDlg::CV568SifDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568SifDlg::IDD, pParent)
{
}

CV568SifDlg::~CV568SifDlg()
{
}

void CV568SifDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	// Check Box
	DDX_Control(pDX, IDC_SENSOR_EN_CHECK,		m_snr_en);
	DDX_Control(pDX, IDC_POLARITY_EN_CHECK,		m_polarity_en);
	DDX_Control(pDX, IDC_SYNC_GEN_EN_CHECK,		m_syncgen_en);
	DDX_Control(pDX, IDC_OB_REG_EN_CHECK,		m_obreg_en);
	DDX_Control(pDX, IDC_SNR_CLK_INV_CHECK,		m_snrclk_inv);
	DDX_Control(pDX, IDC_SI_CLK_RVS_CHECK,		m_siclk_rvs);
	DDX_Control(pDX, IDC_SYNC_OUT_EN_CHECK,		m_syncout_en);
	DDX_Control(pDX, IDC_SNR_PCLK_EN_CHECK,		m_snr_pclk_en);
	DDX_Control(pDX, IDC_VSYNC_DELAY_EN_CHECK,	m_vsyncdly_en);
	DDX_Control(pDX, IDC_FIFO_VSYNC_EN_CHECK,	m_fifodly_en);
	DDX_Control(pDX, IDC_PCLK_NEG_CHECK,		m_pclk_neg);
	DDX_Control(pDX, IDC_SNR_SHIFT_CHECK,		m_snrshift_en);

	// Combo Box
	DDX_Control(pDX, IDC_POLARITY_MODE_COMBO,	m_plmode_box);
	DDX_Control(pDX, IDC_SENSOR_DATA_COMBO,		m_snrdata_box);
	DDX_Control(pDX, IDC_SYNC_MODE_COMBO,		m_syncmode_box);
	DDX_Control(pDX, IDC_SYNC_DIR_COMBO,		m_syncdir_box);
	DDX_Control(pDX, IDC_BUS_SEL_COMBO,			m_bussel_box);
	DDX_Control(pDX, IDC_I2C_SEL_COMBO,			m_i2csel_box);
	DDX_Control(pDX, IDC_YUV_FMT_COMBO,			m_yuvfmt_box);
	DDX_Control(pDX, IDC_EDGE_CHOS_COMBO,		m_edgechos_box);
	DDX_Control(pDX, IDC_AE_MODE_COMBO,			m_aemode_box);
	DDX_Control(pDX, IDC_ENV_SEL_COMBO,			m_envsel_box);

	// ScrollBar
	DDX_Control(pDX, IDC_BAR_AEWIN_WIDTH,		m_aewinst_bar);
	DDX_Control(pDX, IDC_BAR_AEWIN_HEIGHT,		m_aewinsp_bar);

	// ScrollBar EditBox
	DDX_Control(pDX, IDC_EDIT_WIN_START,		m_aewinst);
	DDX_Control(pDX, IDC_EDIT_WIN_STOP,			m_aewinsp);

	// EditBox
	DDX_Control(pDX, IDC_SNR_CLK_EDIT,				m_snrclk);
	DDX_Control(pDX, IDC_PIXEL_RATE_EDIT,			m_pixelrate);
	DDX_Control(pDX, IDC_SI_CLK_EDIT,				m_siclk);
	DDX_Control(pDX, IDC_I2C_CLK_EDIT,				m_i2cclk);
	DDX_Control(pDX, IDC_I2C_INTERVAL_EDIT,			m_i2cinter);
	DDX_Control(pDX, IDC_HREF_START_EDIT,			m_hrefst);
	DDX_Control(pDX, IDC_HREF_LENGTH_EDIT,			m_hreflen);
	DDX_Control(pDX, IDC_VREF_START_EDIT,			m_vrefst);
	DDX_Control(pDX, IDC_VREF_STOP_EDIT,			m_vrefsp);
	DDX_Control(pDX, IDC_OB_REG_EDIT,				m_obreg);
	DDX_Control(pDX, IDC_EDIT_CAP_DELAY,			m_capdelay);
	DDX_Control(pDX, IDC_VSYNC_DELAY_VALUE_EDIT,	m_vsyncdly_val);

	DDX_Control(pDX, IDC_EDIT_ADDR1,				m_edit_addr[0]);
	DDX_Control(pDX, IDC_EDIT_ADDR1_VALUE1,			m_edit_value[0][0]);
	DDX_Control(pDX, IDC_EDIT_ADDR1_VALUE2,			m_edit_value[0][1]);
	DDX_Control(pDX, IDC_EDIT_ADDR1_VALUE3,			m_edit_value[0][2]);
	DDX_Control(pDX, IDC_EDIT_ADDR2,				m_edit_addr[1]);
	DDX_Control(pDX, IDC_EDIT_ADDR2_VALUE1,			m_edit_value[1][0]);
	DDX_Control(pDX, IDC_EDIT_ADDR2_VALUE2,			m_edit_value[1][1]);
	DDX_Control(pDX, IDC_EDIT_ADDR2_VALUE3,			m_edit_value[1][2]);
	DDX_Control(pDX, IDC_EDIT_ADDR3,				m_edit_addr[2]);
	DDX_Control(pDX, IDC_EDIT_ADDR3_VALUE1,			m_edit_value[2][0]);
	DDX_Control(pDX, IDC_EDIT_ADDR3_VALUE2,			m_edit_value[2][1]);
	DDX_Control(pDX, IDC_EDIT_ADDR3_VALUE3,			m_edit_value[2][2]);
	DDX_Control(pDX, IDC_EDIT_ADDR4,				m_edit_addr[3]);
	DDX_Control(pDX, IDC_EDIT_ADDR4_VALUE1,			m_edit_value[3][0]);
	DDX_Control(pDX, IDC_EDIT_ADDR4_VALUE2,			m_edit_value[3][1]);
	DDX_Control(pDX, IDC_EDIT_ADDR4_VALUE3,			m_edit_value[3][2]);
	DDX_Control(pDX, IDC_EDIT_ADDR5,				m_edit_addr[4]);
	DDX_Control(pDX, IDC_EDIT_ADDR5_VALUE1,			m_edit_value[4][0]);
	DDX_Control(pDX, IDC_EDIT_ADDR5_VALUE2,			m_edit_value[4][1]);
	DDX_Control(pDX, IDC_EDIT_ADDR5_VALUE3,			m_edit_value[4][2]);
	DDX_Control(pDX, IDC_EDIT_ADDR6,				m_edit_addr[5]);
	DDX_Control(pDX, IDC_EDIT_ADDR6_VALUE1,			m_edit_value[5][0]);
	DDX_Control(pDX, IDC_EDIT_ADDR6_VALUE2,			m_edit_value[5][1]);
	DDX_Control(pDX, IDC_EDIT_ADDR6_VALUE3,			m_edit_value[5][2]);
	DDX_Control(pDX, IDC_EDIT_ADDR7,				m_edit_addr[6]);
	DDX_Control(pDX, IDC_EDIT_ADDR7_VALUE1,			m_edit_value[6][0]);
	DDX_Control(pDX, IDC_EDIT_ADDR7_VALUE2,			m_edit_value[6][1]);
	DDX_Control(pDX, IDC_EDIT_ADDR7_VALUE3,			m_edit_value[6][2]);
	
	DDX_Control(pDX, IDC_EDIT_ET,					m_edit_et);
	DDX_Control(pDX, IDC_CHECK_FLASH_EN,			m_btnFlashEn);
	DDX_Control(pDX, IDC_EDIT_FLASH_Y_THD,			m_edit_flash_y);
	DDX_Control(pDX, IDC_EDIT_FLASH_ET,				m_edit_flash_et);
}

BEGIN_MESSAGE_MAP(CV568SifDlg, CBaseCtrlDlg)
	ON_WM_HSCROLL()
	// Check Box
	ON_BN_CLICKED(IDC_SENSOR_EN_CHECK,			OnBnClickedSensorEnCheck)
	ON_BN_CLICKED(IDC_POLARITY_EN_CHECK,		OnBnClickedPolarityEnCheck)
	ON_BN_CLICKED(IDC_SYNC_GEN_EN_CHECK,		OnBnClickedSyncGenEnCheck)
	ON_BN_CLICKED(IDC_OB_REG_EN_CHECK,			OnBnClickedObRegEnCheck)
	ON_BN_CLICKED(IDC_SNR_CLK_INV_CHECK,		OnBnClickedSnrClkInvCheck)
	ON_BN_CLICKED(IDC_SI_CLK_RVS_CHECK,			OnBnClickedSiClkRvsCheck)
	ON_BN_CLICKED(IDC_SYNC_OUT_EN_CHECK,		OnBnClickedSyncOutEnCheck)
	ON_BN_CLICKED(IDC_SNR_PCLK_EN_CHECK,		OnBnClickedSnrPclkEnCheck)
	ON_BN_CLICKED(IDC_FIFO_VSYNC_EN_CHECK,		OnBnClickedFifoVsyncEnCheck)
	ON_BN_CLICKED(IDC_VSYNC_DELAY_EN_CHECK,		OnBnClickedVsyncDelayEnCheck)
	ON_BN_CLICKED(IDC_PCLK_NEG_CHECK,			OnBnClickedPclkNegCheck)
	ON_BN_CLICKED(IDC_SNR_SHIFT_CHECK,			OnBnClickedSnrShiftCheck)

	// Combo Box
	ON_CBN_SELCHANGE(IDC_POLARITY_MODE_COMBO,	OnCbnSelchangePolarityModeCombo)
	ON_CBN_SELCHANGE(IDC_SENSOR_DATA_COMBO,		OnCbnSelchangeSensorDataCombo)
	ON_CBN_SELCHANGE(IDC_SYNC_MODE_COMBO,		OnCbnSelchangeSyncModeCombo)
	ON_CBN_SELCHANGE(IDC_SYNC_DIR_COMBO,		OnCbnSelchangeSyncDirCombo)
	ON_CBN_SELCHANGE(IDC_BUS_SEL_COMBO,			OnCbnSelchangeBusSelCombo)
	ON_CBN_SELCHANGE(IDC_I2C_SEL_COMBO,			OnCbnSelchangeI2cSelCombo)
	ON_CBN_SELCHANGE(IDC_YUV_FMT_COMBO,			OnCbnSelchangeYuvFmtCombo)
	ON_CBN_SELCHANGE(IDC_EDGE_CHOS_COMBO,		OnCbnSelchangeEdgeChosCombo)
	ON_CBN_SELCHANGE(IDC_AE_MODE_COMBO,			OnCbnSelchangeAeModeCombo)
	ON_CBN_SELCHANGE(IDC_ENV_SEL_COMBO,			OnCbnSelchangeEnvSelCombo)

	// Button
	ON_BN_CLICKED(IDC_BUTTON_APPLY,				OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH,			OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_SENSOR_WRITE,		OnBnClickedButtonSensorWrite)
	ON_BN_CLICKED(IDC_SENSOR_RESET_BUTTON,		OnBnClickedSensorResetButton)
	ON_BN_CLICKED(IDC_BUTTON_POWERON,			OnBnClickedButtonPoweron)
	ON_BN_CLICKED(IDC_BUTTON_POWEROFF,			OnBnClickedButtonPoweroff)

	ON_BN_CLICKED(IDC_BUTTON_FLASH_CHARGE,		OnBnClickedButtonFlashCharge)
	ON_BN_CLICKED(IDC_BUTTON_STOP_FLASH_CHARGE, OnBnClickedButtonStopFlashCharge)
	ON_BN_CLICKED(IDC_BUTTON_START_FLASH,		OnBnClickedButtonStartFlash)
	ON_BN_CLICKED(IDC_BUTTON_STOP_FLASH,		OnBnClickedButtonStopFlash)
	ON_BN_CLICKED(IDC_CHECK_FLASH_EN,			OnBnClickedCheckFlashEn)
	ON_BN_CLICKED(IDC_BTN_SETFLASH,				OnBnClickedBtnSetflash)
	ON_BN_CLICKED(IDC_BTN_GET_FLASH,			OnBnClickedBtnGetFlash)
	ON_BN_CLICKED(IDC_BUTTON_SETET,				OnBnClickedButtonSetet)
	ON_BN_CLICKED(IDC_GET_ENV_BUTTON,			OnBnClickedGetEnvButton)
	ON_BN_CLICKED(IDC_JUDGE_ENV_BUTTON,			OnBnClickedJudgeEnvButton)
	ON_BN_CLICKED(IDC_START_AE_BUTTON,			OnBnClickedStartAeButton)
	ON_BN_CLICKED(IDC_SIM_AE_BUTTON,			OnBnClickedSimAeButton)
END_MESSAGE_MAP()


// CV568SifDlg 消息处理程序

BOOL CV568SifDlg::OnInitDialog(void)
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispOption();

	return TRUE;
}

void CV568SifDlg::InitUserOption()
{
	memset(&(this->m_option), 0, sizeof(TSifOption));
	this->InitCombo();
}

void CV568SifDlg::GetUserOption(char *tail)
{
	this->GetEditboxValue();
	this->GetComboValue(tail);
}

void CV568SifDlg::DispOption()
{
	this->DispCombo();
	this->DispEditBox();
	this->DispCheckBox();
	this->AdjustScrollBar();
}

/******************************************** Check Box Section ********************************************/

void CV568SifDlg::OnBnClickedSensorEnCheck()
{
	if(this->m_snr_en.GetCheck())
		this->m_option.snrmode |= BIT1;
	else
		this->m_option.snrmode &= ~BIT1;
	V5B_SifSetSensorMode((UINT8)this->m_option.snrmode);
}

void CV568SifDlg::OnBnClickedPolarityEnCheck()
{
	if(this->m_polarity_en.GetCheck())
		this->m_option.snrmode |= BIT3;
	else
		this->m_option.snrmode &= ~BIT3;
	V5B_SifSetSensorMode((UINT8)this->m_option.snrmode);
}

void CV568SifDlg::OnBnClickedSyncGenEnCheck()
{
	if(this->m_syncgen_en.GetCheck())
		this->m_option.snrmode |= BIT5;
	else
		this->m_option.snrmode &= ~BIT5;
	V5B_SifSetSensorMode((UINT8)this->m_option.snrmode);
}

void CV568SifDlg::OnBnClickedObRegEnCheck()
{
	if(this->m_obreg_en.GetCheck())
		this->m_option.snropmode |= BIT1;
	else
		this->m_option.snropmode &= ~BIT1;
	V5B_SifSetSensorOpMode((UINT8)this->m_option.snropmode);
}

void CV568SifDlg::OnBnClickedSnrClkInvCheck()
{
	if(this->m_snrclk_inv.GetCheck())
		this->m_option.snropmode |= BIT3;
	else
		this->m_option.snropmode &= ~BIT3;
	V5B_SifSetSensorOpMode((UINT8)this->m_option.snropmode);
}

void CV568SifDlg::OnBnClickedSiClkRvsCheck()
{
	if(this->m_siclk_rvs.GetCheck())
		this->m_option.snropmode |= BIT4;
	else
		this->m_option.snropmode &= ~BIT4;
	V5B_SifSetSensorOpMode((UINT8)this->m_option.snropmode);
}

void CV568SifDlg::OnBnClickedSyncOutEnCheck()
{
	if(this->m_syncout_en.GetCheck())
		this->m_option.snropmode |= BIT6;
	else
		this->m_option.snropmode &= ~BIT6;
	V5B_SifSetSensorOpMode((UINT8)this->m_option.snropmode);
}

void CV568SifDlg::OnBnClickedCheckFlashEn()
{
	if(this->m_btnFlashEn.GetCheck())
	{
		this->m_option.enflash = 1;
		V5_VispEnableFlash();
	}
	else
	{
		this->m_option.enflash = 0;
		V5_VispDisableFlash();
	}
}

void CV568SifDlg::OnBnClickedSnrPclkEnCheck()
{
	if(this->m_snr_pclk_en.GetCheck())
	{
		this->m_option.snrpclk = 1;
		V5B_SifEnableSnrPClk();
	}
	else
	{
		this->m_option.snrpclk = 0;
		V5B_SifDisableSnrPClk();
	}
}

void CV568SifDlg::OnBnClickedFifoVsyncEnCheck()
{
	if(this->m_fifodly_en.GetCheck())
	{
		this->m_option.fifo_vsync_en = 1;
		V5B_SifEnableFifoVSyncDly();
	}
	else
	{
		this->m_option.fifo_vsync_en = 0;
		V5B_SifDisableFifoVSyncDly();
	}
}

void CV568SifDlg::OnBnClickedVsyncDelayEnCheck()
{
	if(this->m_vsyncdly_en.GetCheck())
	{
		this->m_option.vsyncdly_en = 1;
		V5B_SifEnableVSyncDly();
	}
	else
	{
		this->m_option.vsyncdly_en = 0;
		V5B_SifDisableVSyncDly();
	}
}

void CV568SifDlg::OnBnClickedPclkNegCheck()
{
	if(this->m_pclk_neg.GetCheck())
	{
		this->m_option.pclk_neg = 1;
		V5B_SifSetPClkNegEdge();
	}
	else
	{
		this->m_option.pclk_neg = 0;
		V5B_SifSetPClkPosEdge();
	}
}

void CV568SifDlg::OnBnClickedSnrShiftCheck()
{
	if(this->m_snrshift_en.GetCheck())
	{
		this->m_option.snrshift = 1;
		V5B_SifEnableSnrDataShift();
	}
	else
	{
		this->m_option.snrshift = 0;
		V5B_SifDisableSnrDataShift();
	}
}

void CV568SifDlg::RefreshCheckBox(void)
{
	this->m_option.snrpclk = V5B_SifIsSnrPClkEn();
	this->m_option.vsyncdly_en   = V5B_SifIsVSyncDlyEn();
	this->m_option.fifo_vsync_en = V5B_SifIsFifoVSyncDlyEn();
	this->m_option.pclk_neg = V5B_SifGetPClkEdge();
	this->m_option.snrshift = V5B_SifIsSnrDataShiftEn();
}

void CV568SifDlg::DispCheckBox(void)
{
	if(this->m_option.snrmode & BIT1)
		this->m_snr_en.SetCheck(BST_CHECKED);
	else
		this->m_snr_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.snrmode & BIT3)
		this->m_polarity_en.SetCheck(BST_CHECKED);
	else
		this->m_polarity_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.snrmode & BIT5)
		this->m_syncgen_en.SetCheck(BST_CHECKED);
	else
		this->m_syncgen_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.snropmode & BIT1)
		this->m_obreg_en.SetCheck(BST_CHECKED);
	else
		this->m_obreg_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.snropmode & BIT3)
		this->m_snrclk_inv.SetCheck(BST_CHECKED);
	else
		this->m_snrclk_inv.SetCheck(BST_UNCHECKED);

	if(this->m_option.snropmode & BIT4)
		this->m_siclk_rvs.SetCheck(BST_CHECKED);
	else
		this->m_siclk_rvs.SetCheck(BST_UNCHECKED);

	if(this->m_option.snropmode & BIT6)
		this->m_syncout_en.SetCheck(BST_CHECKED);
	else
		this->m_syncout_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.enflash)
		this->m_btnFlashEn.SetCheck(BST_CHECKED);
	else
		this->m_btnFlashEn.SetCheck(BST_UNCHECKED);

	if(this->m_option.snrpclk)
		this->m_snr_pclk_en.SetCheck(BST_CHECKED);
	else
		this->m_snr_pclk_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.vsyncdly_en)
		this->m_vsyncdly_en.SetCheck(BST_CHECKED);
	else
		this->m_vsyncdly_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.fifo_vsync_en)
		this->m_fifodly_en.SetCheck(BST_CHECKED);
	else
		this->m_fifodly_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.pclk_neg)
		this->m_pclk_neg.SetCheck(BST_CHECKED);
	else
		this->m_pclk_neg.SetCheck(BST_UNCHECKED);

	if(this->m_option.snrshift)
		this->m_snrshift_en.SetCheck(BST_CHECKED);
	else
		this->m_snrshift_en.SetCheck(BST_UNCHECKED);
}

/********************************************   Combo Section   ********************************************/

void CV568SifDlg::OnCbnSelchangePolarityModeCombo()
{
	int sel = 0;

	sel = this->m_plmode_box.GetCurSel();
	this->m_option.snrmode &= ~BIT4;
	this->m_option.snrmode |= ((sel << 4) & BIT4);
	V5B_SifSetSensorMode((UINT8)this->m_option.snrmode);
}

void CV568SifDlg::OnCbnSelchangeSensorDataCombo()
{
	int sel = 0;

	sel = this->m_snrdata_box.GetCurSel();
	this->m_option.snrmode &= ~BIT6;
	this->m_option.snrmode |= ((sel << 6) & BIT6);
	V5B_SifSetSensorMode((UINT8)this->m_option.snrmode);
}

void CV568SifDlg::OnCbnSelchangeSyncModeCombo()
{
	int sel = 0;

	sel = this->m_syncmode_box.GetCurSel();
	this->m_option.snrmode &= ~BIT7;
	this->m_option.snrmode |= ((sel << 7) & BIT7);
	V5B_SifSetSensorMode((UINT8)this->m_option.snrmode);
}

void CV568SifDlg::OnCbnSelchangeSyncDirCombo()
{
	int sel = 0;

	sel = this->m_syncdir_box.GetCurSel();
	this->m_option.snropmode &= ~BIT0;
	this->m_option.snropmode |= (sel & BIT0);
	V5B_SifSetSensorMode((UINT8)this->m_option.snropmode);
}

void CV568SifDlg::OnCbnSelchangeBusSelCombo()
{
	int sel = 0;

	sel = this->m_bussel_box.GetCurSel();
	this->m_option.snropmode &= ~BIT5;
	this->m_option.snropmode |= ((sel << 5) & BIT5);
	V5B_SifSetSensorMode((UINT8)this->m_option.snropmode);
}

void CV568SifDlg::OnCbnSelchangeI2cSelCombo()
{
	int sel = 0;

	sel = this->m_i2csel_box.GetCurSel();
	this->m_option.snropmode &= ~BIT2;
	this->m_option.snropmode |= ((sel << 2) & BIT2);
	V5B_SifSetSensorMode((UINT8)this->m_option.snropmode);
}

void CV568SifDlg::OnCbnSelchangeYuvFmtCombo()
{
	this->m_option.yuvfmt = this->m_yuvfmt_box.GetCurSel();
	V5B_SifSetYuvFormat((UINT8)this->m_option.yuvfmt);
}

void CV568SifDlg::OnCbnSelchangeEdgeChosCombo()
{
	this->m_option.edgechos = this->m_edgechos_box.GetCurSel();
	V5B_SifSetEdgeChos((UINT8)this->m_option.edgechos);
}

void CV568SifDlg::OnCbnSelchangeAeModeCombo()
{
	this->m_option.aemode = this->m_aemode_box.GetCurSel();
	V5_SetExposeMode((UINT8)this->m_option.aemode);
}

void CV568SifDlg::OnCbnSelchangeEnvSelCombo()
{
	this->m_option.envsel = this->m_envsel_box.GetCurSel();
	V5_VispSetEnv((UINT8)this->m_option.envsel);
}

void CV568SifDlg::InitCombo(void)
{
	int i = 0;
	const static char *plmode[]   = { "Pull Down",		"Pull Up" };
	const static char *snrdata[]  = { "Bayer Pattern",	"YUV 4:2:2" };
	const static char *syncmode[] = { "Normal Sync",	"CCIR656 Sync" };
	const static char *syncdir[]  = { "Sync To Snr",	"Sync From Snr" };
	const static char *bussel[]   = { "I2C Ctrl Bus",	"Serial Bus" };
	const static char *i2ssel[]   = { "Normal Type",	"OV Sensor" };
	const static char *yuvfmt[]   = { "UYVY", "VYUY", "YUYV", "YVYU" };
	const static char *edgechos[]  = { "Pos Edge", "Neg Edge" };
	const static char *aemode[]   = { "Auto", "Out Door", "In Door", "Night" };
	const static char *envsel[]   = { "50 Hz", "60 Hz", "Out Door", "Not Sure" };

	for(i = 0; i < sizeof(plmode) / sizeof(char *); i++)
		this->m_plmode_box.InsertString(i, plmode[i]);
	for(i = 0; i < sizeof(snrdata) / sizeof(char *); i++)
		this->m_snrdata_box.InsertString(i, snrdata[i]);
	for(i = 0; i < sizeof(syncmode) / sizeof(char *); i++)
		this->m_syncmode_box.InsertString(i, syncmode[i]);
	for(i = 0; i < sizeof(syncdir) / sizeof(char *); i++)
		this->m_syncdir_box.InsertString(i, syncdir[i]);
	for(i = 0; i < sizeof(bussel) / sizeof(char *); i++)
		this->m_bussel_box.InsertString(i, bussel[i]);
	for(i = 0; i < sizeof(i2ssel) / sizeof(char *); i++)
		this->m_i2csel_box.InsertString(i, i2ssel[i]);
	for(i = 0; i < sizeof(yuvfmt) / sizeof(char *); i++)
		this->m_yuvfmt_box.InsertString(i, yuvfmt[i]);
	for(i = 0; i < sizeof(edgechos) / sizeof(char *); i++)
		this->m_edgechos_box.InsertString(i, edgechos[i]);
	for(i = 0; i < sizeof(aemode) / sizeof(char *); i++)
		this->m_aemode_box.InsertString(i, aemode[i]);
	for(i = 0; i < sizeof(envsel) / sizeof(char *); i++)
		this->m_envsel_box.InsertString(i, envsel[i]);
}

void CV568SifDlg::GetComboValue(char *tail)
{
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CV568SifDlg::RefreshCombo(void)
{
	this->m_option.yuvfmt   = V5B_SifGetYuvFormat();
	this->m_option.edgechos = V5B_SifGetEdgeChos();
}

void CV568SifDlg::DispCombo(void)
{
	this->m_plmode_box.SetCurSel(((this->m_option.snrmode & BIT4) >> 4));
	this->m_snrdata_box.SetCurSel(((this->m_option.snrmode & BIT6) >> 6));
	this->m_syncmode_box.SetCurSel(((this->m_option.snrmode & BIT7) >> 7));
	this->m_syncdir_box.SetCurSel((this->m_option.snropmode & BIT0));
	this->m_bussel_box.SetCurSel(((this->m_option.snropmode & BIT5) >> 5));
	this->m_i2csel_box.SetCurSel(((this->m_option.snropmode & BIT2) >> 2));
	this->m_yuvfmt_box.SetCurSel(this->m_option.yuvfmt);
	this->m_edgechos_box.SetCurSel(this->m_option.edgechos);
	this->m_aemode_box.SetCurSel(this->m_option.aemode);
	this->m_envsel_box.SetCurSel(this->m_option.envsel);
}

/******************************************** ScrollBar Section ********************************************/

void CV568SifDlg::GetBarValue()
{
	this->m_option.aewinst = this->m_aewinst_bar.GetScrollPos();
	this->m_option.aewinsp = this->m_aewinsp_bar.GetScrollPos();
}

void CV568SifDlg::DispBarEditBox(void)
{
	char str[10];

	sprintf(str, "%x", this->m_option.aewinst);
	this->m_aewinst.SetWindowText(str);
	sprintf(str, "%x", this->m_option.aewinsp);
	this->m_aewinsp.SetWindowText(str);
}

void CV568SifDlg::RefreshBar(void)
{
	this->m_option.aewinst = V5B_SifGetAeWinStart();
	this->m_option.aewinsp = V5B_SifGetAeWinStop();
}

void CV568SifDlg::AdjustScrollBar(void)
{
	TSize size;

	if(gVispContext.valid)
		V5_VispGetCurrentSize(&size);
	else
	{
		size.cx = 0;
		size.cy = 0;
	}
	this->m_aewinst_bar.SetScrollRange(0,this->m_option.aewinsp);
	this->m_aewinst_bar.SetScrollPos(this->m_option.aewinst);
	this->m_aewinsp_bar.SetScrollRange(this->m_option.aewinst, size.cx);
	this->m_aewinsp_bar.SetScrollPos(this->m_option.aewinsp);

	this->GetBarValue();
	this->DispBarEditBox();
}

void CV568SifDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWaitCursor cursor;
	int pos;

	if(pScrollBar != NULL)
	{
		switch(nSBCode)
		{
		case SB_LINELEFT:
			pos = pScrollBar->GetScrollPos();
			pScrollBar->SetScrollPos(pos - 2);
			break;
		case SB_LINERIGHT:
			pos = pScrollBar->GetScrollPos();
			pScrollBar->SetScrollPos(pos + 2);
			break;
		case SB_PAGELEFT:
			pos = pScrollBar->GetScrollPos();
			pScrollBar->SetScrollPos(pos - 10);
			break;
		case SB_PAGERIGHT:
			pos = pScrollBar->GetScrollPos();
			pScrollBar->SetScrollPos(pos + 10);
			break;
		case SB_THUMBTRACK:
			nPos = ((nPos + 1) >> 1) << 1;
			pScrollBar->SetScrollPos(nPos);
			break;
		default:
			break;
		}
		this->GetBarValue();
		this->DispOption();
	}
	CBaseCtrlDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

/********************************************  EditBox Section  ********************************************/

void CV568SifDlg::GetEditboxValue()
{
	int i = 0, j = 0;
	char str[10];

	this->m_snrclk.GetWindowText(str, 10);
	sscanf(str, "%x", &(this->m_option.snrclk));
	this->m_pixelrate.GetWindowText(str, 10);
	sscanf(str, "%x", &(this->m_option.pixelrate));
	this->m_siclk.GetWindowText(str, 10);
	sscanf(str, "%x", &(this->m_option.siclk));
	this->m_i2cclk.GetWindowText(str, 10);
	sscanf(str, "%x", &(this->m_option.i2cclk));
	this->m_i2cinter.GetWindowText(str, 10);
	sscanf(str, "%x", &(this->m_option.i2cinter));

	this->m_hrefst.GetWindowText(str, 10);
	sscanf(str, "%x", &(this->m_option.hrefst));
	this->m_hreflen.GetWindowText(str, 10);
	sscanf(str, "%x", &(this->m_option.hreflen));
	this->m_vrefst.GetWindowText(str, 10);
	sscanf(str, "%x", &(this->m_option.vrefst));
	this->m_vrefsp.GetWindowText(str, 10);
	sscanf(str, "%x", &(this->m_option.vrefsp));

	this->m_obreg.GetWindowText(str, 10);
	sscanf(str, "%ld", &(this->m_option.obreg));
	this->m_capdelay.GetWindowText(str, 10);
	sscanf(str, "%ld", &(this->m_option.capdelay));

	this->m_vsyncdly_val.GetWindowText(str, 10);
	sscanf(str, "%ld", &(this->m_option.vsyncdly_val));

	this->m_edit_et.GetWindowText(str, 10);
	sscanf(str, "%ld", &(this->m_option.et));
	this->m_edit_flash_et.GetWindowText(str, 10);
	sscanf(str, "%ld", &(this->m_option.flashstep));
	this->m_edit_flash_y.GetWindowText(str, 10);
	sscanf(str, "%ld", &(this->m_option.yflashthd));

	this->m_option.count = 0;
	for(i = 0; i < 7; i++)
	{
		this->m_option.valuecount[i] = 0;
		if(this->m_edit_addr[i].GetWindowText(str, 10))
		{
			this->m_option.count ++;
			sscanf(str, "%x", &this->m_option.addr[i]);
			for(j = 0; j < 3; j++)
			{
				if(this->m_edit_value[i][j].GetWindowText(str, 10))
				{
					this->m_option.valuecount[i] ++;
					sscanf(str, "%x", &this->m_option.value[i][j]);
				}
				else
					break;
			}
		}
		else
			break;
	}
}

void CV568SifDlg::RefreshEditBox(void)
{
	this->m_option.snrclk	 = V5B_SifGetSnrClk();
	this->m_option.pixelrate = V5B_SifGetPixelRate();
	this->m_option.siclk     = V5B_SifGetSerialclk();
	this->m_option.i2cclk    = V5B_SifGetI2cclk();
	this->m_option.i2cinter  = V5B_SifGetI2cInterval();

	this->m_option.hrefst   = V5B_SifGetHrefStart();
	this->m_option.hreflen  = V5B_SifGetHrefLength();
	this->m_option.vrefst   = V5B_SifGetVrefStart();
	this->m_option.vrefsp   = V5B_SifGetVrefStop();

	this->m_option.obreg    = V5B_SifGetOB();
	this->m_option.capdelay = V5B_SifGetCaptureDelay();

	this->m_option.vsyncdly_val = V5B_SifGetVSyncDelay();

	this->m_option.enflash   = (int)gVispContext.enflash;
	this->m_option.flashstep = (int)gVispContext.flashparm.flashstep;
	this->m_option.yflashthd = (int)gVispContext.flashparm.yflashthd;
}

void CV568SifDlg::DispEditBox(void)
{
	char str[10];

	sprintf(str, "%x", this->m_option.snrclk);
	this->m_snrclk.SetWindowText(str);
	sprintf(str, "%x", this->m_option.pixelrate);
	this->m_pixelrate.SetWindowText(str);
	sprintf(str, "%x", this->m_option.siclk);
	this->m_siclk.SetWindowText(str);
	sprintf(str, "%x", this->m_option.i2cclk);
	this->m_i2cclk.SetWindowText(str);
	sprintf(str, "%x", this->m_option.i2cinter);
	this->m_i2cinter.SetWindowText(str);

	sprintf(str, "%x", this->m_option.hrefst);
	this->m_hrefst.SetWindowText(str);
	sprintf(str, "%x", this->m_option.hreflen);
	this->m_hreflen.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vrefst);
	this->m_vrefst.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vrefsp);
	this->m_vrefsp.SetWindowText(str);

	sprintf(str, "%x", this->m_option.obreg);
	this->m_obreg.SetWindowText(str);
	sprintf(str, "%d", this->m_option.capdelay);
	this->m_capdelay.SetWindowText(str);

	sprintf(str, "%d", this->m_option.vsyncdly_val);
	this->m_vsyncdly_val.SetWindowText(str);

	sprintf(str, "%x", this->m_option.yflashthd);
	this->m_edit_flash_y.SetWindowText(str);
	sprintf(str, "%d", this->m_option.flashstep);
	this->m_edit_flash_et.SetWindowText(str);
}

/********************************************   Button Section  ********************************************/

void CV568SifDlg::OnBnClickedButtonRefresh()
{
	this->m_option.snrmode   = V5B_SifGetSensorMode();
	this->m_option.snropmode = V5B_SifGetSensorOpMode();

	this->RefreshBar();
	this->RefreshCombo();
	this->RefreshEditBox();
	this->RefreshCheckBox();
	this->DispOption();
}

void CV568SifDlg::OnBnClickedButtonApply()
{
	this->GetUserOption();

	V5B_SifSetSnrClk((UINT8)this->m_option.snrclk);
	V5B_SifSetPixelRate((UINT8)this->m_option.pixelrate);
	V5B_SifSetSerialclk((UINT8)this->m_option.siclk);
	V5B_SifSetI2cclk((UINT16)this->m_option.i2cclk);
	V5B_SifSetI2cInterval((UINT16)this->m_option.i2cinter);

	V5B_SifSetHrefStart((UINT16)this->m_option.hrefst);
	V5B_SifSetHrefLength((UINT16)this->m_option.hreflen);
	V5B_SifSetVrefStart((UINT16)this->m_option.vrefst);
	V5B_SifSetVrefStop((UINT16)this->m_option.vrefsp);

	V5B_SifSetOB((UINT8)this->m_option.obreg);
	V5B_SifSetCaptureDelay((UINT8)this->m_option.capdelay);

	V5B_SifSetVSyncDelay((UINT16)this->m_option.vsyncdly_val);

	V5B_SifSetAeWinStart((UINT16)this->m_option.aewinst);
	V5B_SifSetAeWinStop((UINT16)this->m_option.aewinsp);
}

void CV568SifDlg::OnBnClickedSensorResetButton()
{
	V5_VispResetSensor();
}

void CV568SifDlg::OnBnClickedButtonPoweron()
{
	V5_VispSetSnrState(SENSOR_POWERON);
}

void CV568SifDlg::OnBnClickedButtonPoweroff()
{
	V5_VispSetSnrState(SENSOR_POWEROFF);
}

void CV568SifDlg::OnBnClickedButtonSensorWrite()
{
	int i = 0, j = 0;
	TI2cBatch AeData[7];

	this->GetUserOption();
	for(i = 0 ;i < this->m_option.count; i++)
	{
		AeData[i].RegBytes = (UINT8)this->m_option.valuecount[i];
		AeData[i].RegAddr = (UINT8)this->m_option.addr[i];
		for(j = 0; j < this->m_option.valuecount[i]; j++)
			AeData[i].RegVal[j]= (UINT8)this->m_option.value[i][j];
	}
	V5B_SifI2cAeBatch((UINT8)this->m_option.count, AeData);
}

void CV568SifDlg::OnBnClickedButtonFlashCharge()
{
	V5_VispStartFlashCharge();
}

void CV568SifDlg::OnBnClickedButtonStopFlashCharge()
{
	V5_VispStopFlashCharge();
}

void CV568SifDlg::OnBnClickedButtonStartFlash()
{
	V5_VispFlashLight();
}

void CV568SifDlg::OnBnClickedButtonStopFlash()
{
	V5_VispStopFlashLight();
}

void CV568SifDlg::OnBnClickedBtnSetflash()
{
	this->GetEditboxValue();
	gVispContext.flashparm.flashstep = (UINT8)this->m_option.flashstep;
	gVispContext.flashparm.yflashthd = (UINT8)this->m_option.yflashthd;
}

void CV568SifDlg::OnBnClickedBtnGetFlash()
{
	this->RefreshEditBox();
	this->DispOption();
}

void CV568SifDlg::OnBnClickedGetEnvButton()
{
	this->m_option.envsel = V5_VispGetEnv();
	this->m_envsel_box.SetCurSel(this->m_option.envsel);
}

void CV568SifDlg::OnBnClickedJudgeEnvButton()
{
	TestAutoDetectEnv();
}

void CV568SifDlg::OnBnClickedStartAeButton()
{
	TestAutoExpose();
}

void CV568SifDlg::OnBnClickedSimAeButton()
{
	AdjustAe(1, 1, 1);
}

/******************************************  Function From VC0568 ******************************************/

void CV568SifDlg::OnBnClickedButtonSetet()
{
	TSize size;
	UINT32 et;
	UINT32 snrclock;

	GetEditboxValue();

	V5B_IspSetGlbGainDelay(gVispContext.pinfo->aeparm.gdelay);

	if(V5_VispGetCurrentSize(&size) != 0)
		return;
	snrclock = V5_CtrlGetMclk()/(gVispContext.pinfo->snrSizeCfg.pSizeCfg[gVispContext.index].snrpixelratio+1);
	snrclock *= 1000;
	et = (UINT32)(snrclock/gVispContext.ae.freq)*this->m_option.et;
	if(gVispContext.pinfo->snrSetETCall)
		gVispContext.pinfo->snrSetETCall(gVispContext.env, et, size, snrclock);
	V5B_IspSetGlbGain((UINT8)gVispContext.ae.gain);
}

#endif //V578CMD_DISABLE
