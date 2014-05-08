// V568IppDlg.cpp : 实现文件
//

#include "internal.h"
#include "..\include\v568plane\v568ippdlg.h"

#ifndef	V578CMD_DISABLE

// CV568IppDlg 对话框

IMPLEMENT_DYNAMIC(CV568IppDlg, CBaseCtrlDlg)
CV568IppDlg::CV568IppDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568IppDlg::IDD, pParent)
{
}

CV568IppDlg::~CV568IppDlg()
{
}

void CV568IppDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	// Check Box
	DDX_Control(pDX, IDC_SPECIAL_EN_CHECK,	m_spe_en);
	DDX_Control(pDX, IDC_FRAME_EN_CHECK,	m_frame_en);
	DDX_Control(pDX, IDC_TMS_EN_CHECK,		m_tms_en);
	DDX_Control(pDX, IDC_DPS_EN_CHECK,		m_dps_en);
	DDX_Control(pDX, IDC_CPUS_EN_CHECK,		m_cpus_en);
	DDX_Control(pDX, IDC_CPDS_EN_CHECK,		m_cpds_en);
	DDX_Control(pDX, IDC_TMH_BP_EN_CHECK,	m_tmhsbp_en);
	DDX_Control(pDX, IDC_TMV_BP_EN_CHECK,	m_tmvsbp_en);
	DDX_Control(pDX, IDC_DPH_BP_EN_CHECK,	m_dphsbp_en);
	DDX_Control(pDX, IDC_DPV_BP_EN_CHECK,	m_dpvsbp_en);
	DDX_Control(pDX, IDC_CPUH_BP_EN_CHECK,	m_cpuhsbp_en);
	DDX_Control(pDX, IDC_CPUV_BP_EN_CHECK,	m_cpuvsbp_en);
	DDX_Control(pDX, IDC_CPDH_BP_EN_CHECK,	m_cpdhsbp_en);
	DDX_Control(pDX, IDC_CPDV_BP_EN_CHECK,	m_cpdvsbp_en);

	// Combo Box
	DDX_Control(pDX, IDC_WORK_MODE_COMBO,	m_modebox);
	DDX_Control(pDX, IDC_DATA_SRC_COMBO,	m_datasrcbox);
	DDX_Control(pDX, IDC_TMB_SRC_COMBO,		m_tmbsrcbox);
	DDX_Control(pDX, IDC_SPE_TYPE_COMBO,	m_spebox);
	DDX_Control(pDX, IDC_HIST_SEL_COMBO,	m_histselbox);
	DDX_Control(pDX, IDC_THUMB_FMT_COMBO,	m_thumfmtbox);

	// ScrollBar
	DDX_Control(pDX, IDC_UOFFSET_SCROLLBAR,	m_uoffbar);
	DDX_Control(pDX, IDC_VOFFSET_SCROLLBAR,	m_voffbar);
	DDX_Control(pDX, IDC_UTOP_SCROLLBAR,	m_utopbar);
	DDX_Control(pDX, IDC_UBTM_SCROLLBAR,	m_ubtmbar);
	DDX_Control(pDX, IDC_VTOP_SCROLLBAR,	m_vtopbar);
	DDX_Control(pDX, IDC_VBTM_SCROLLBAR,	m_vbtmbar);
	DDX_Control(pDX, IDC_WINX_SCROLLBAR,	m_winxbar);
	DDX_Control(pDX, IDC_WINY_SCROLLBAR,	m_winybar);
	DDX_Control(pDX, IDC_WINW_SCROLLBAR,	m_winwbar);
	DDX_Control(pDX, IDC_WINH_SCROLLBAR,	m_winhbar);
	DDX_Control(pDX, IDC_TMSW_SCROLLBAR,	m_tmswbar);
	DDX_Control(pDX, IDC_TMSH_SCROLLBAR,	m_tmshbar);
	DDX_Control(pDX, IDC_CPSW_SCROLLBAR,	m_cpswbar);
	DDX_Control(pDX, IDC_CPSH_SCROLLBAR,	m_cpshbar);
	DDX_Control(pDX, IDC_DPSW_SCROLLBAR,	m_dpswbar);
	DDX_Control(pDX, IDC_DPSH_SCROLLBAR,	m_dpshbar);

	// ScrollBar EditBox
	DDX_Control(pDX, IDC_UOFFSET_EDIT,		m_uoff);
	DDX_Control(pDX, IDC_VOFFSET_EDIT,		m_voff);
	DDX_Control(pDX, IDC_UTOP_EDIT,			m_utop);
	DDX_Control(pDX, IDC_UBTM_EDIT,			m_ubtm);
	DDX_Control(pDX, IDC_VTOP_EDIT,			m_vtop);
	DDX_Control(pDX, IDC_VBTM_EDIT,			m_vbtm);
	DDX_Control(pDX, IDC_WINX_EDIT,			m_winx);
	DDX_Control(pDX, IDC_WINY_EDIT,			m_winy);
	DDX_Control(pDX, IDC_WINW_EDIT,			m_winw);
	DDX_Control(pDX, IDC_WINH_EDIT,			m_winh);
	DDX_Control(pDX, IDC_TMSW_EDIT,			m_tmsw);
	DDX_Control(pDX, IDC_TMSH_EDIT,			m_tmsh);
	DDX_Control(pDX, IDC_CPSW_EDIT,			m_cpsw);
	DDX_Control(pDX, IDC_CPSH_EDIT,			m_cpsh);
	DDX_Control(pDX, IDC_DPSW_EDIT,			m_dpsw);
	DDX_Control(pDX, IDC_DPSH_EDIT,			m_dpsh);

	// EditBox
	DDX_Control(pDX, IDC_IMGW_EDIT,			m_imgw);
	DDX_Control(pDX, IDC_IMGH_EDIT,			m_imgh);

	DDX_Control(pDX, IDC_DDROP_EDIT,		m_dispdrop);
	DDX_Control(pDX, IDC_CDROP_EDIT,		m_captdrop);

	DDX_Control(pDX, IDC_TMS_HW1_EDIT,		m_tms_hw1);
	DDX_Control(pDX, IDC_TMS_HW2_EDIT,		m_tms_hw2);
	DDX_Control(pDX, IDC_TMS_HS1_EDIT,		m_tms_hs1);
	DDX_Control(pDX, IDC_TMS_HS2_EDIT,		m_tms_hs2);
	DDX_Control(pDX, IDC_TMS_HC_EDIT,		m_tms_hc);
	DDX_Control(pDX, IDC_TMS_VH1_EDIT,		m_tms_vh1);
	DDX_Control(pDX, IDC_TMS_VH2_EDIT,		m_tms_vh2);
	DDX_Control(pDX, IDC_TMS_VS1_EDIT,		m_tms_vs1);
	DDX_Control(pDX, IDC_TMS_VS2_EDIT,		m_tms_vs2);
	DDX_Control(pDX, IDC_TMS_VC_EDIT,		m_tms_vc);

	DDX_Control(pDX, IDC_CPDS_HW1_EDIT,		m_cpds_hw1);
	DDX_Control(pDX, IDC_CPDS_HW2_EDIT,		m_cpds_hw2);
	DDX_Control(pDX, IDC_CPDS_HS1_EDIT,		m_cpds_hs1);
	DDX_Control(pDX, IDC_CPDS_HS2_EDIT,		m_cpds_hs2);
	DDX_Control(pDX, IDC_CPDS_HC_EDIT,		m_cpds_hc);
	DDX_Control(pDX, IDC_CPDS_VH1_EDIT,		m_cpds_vh1);
	DDX_Control(pDX, IDC_CPDS_VH2_EDIT,		m_cpds_vh2);
	DDX_Control(pDX, IDC_CPDS_VS1_EDIT,		m_cpds_vs1);
	DDX_Control(pDX, IDC_CPDS_VS2_EDIT,		m_cpds_vs2);
	DDX_Control(pDX, IDC_CPDS_VC_EDIT,		m_cpds_vc);
	DDX_Control(pDX, IDC_CPUS_HW1_EDIT,		m_cpus_hw1);
	DDX_Control(pDX, IDC_CPUS_HW2_EDIT,		m_cpus_hw2);
	DDX_Control(pDX, IDC_CPUS_HS_EDIT,		m_cpus_hs);
	DDX_Control(pDX, IDC_CPUS_VH1_EDIT,		m_cpus_vh1);
	DDX_Control(pDX, IDC_CPUS_VH2_EDIT,		m_cpus_vh2);
	DDX_Control(pDX, IDC_CPUS_VS_EDIT,		m_cpus_vs);

	DDX_Control(pDX, IDC_DPS_HW1_EDIT,		m_dps_hw1);
	DDX_Control(pDX, IDC_DPS_HW2_EDIT,		m_dps_hw2);
	DDX_Control(pDX, IDC_DPS_HS1_EDIT,		m_dps_hs1);
	DDX_Control(pDX, IDC_DPS_HS2_EDIT,		m_dps_hs2);
	DDX_Control(pDX, IDC_DPS_HC_EDIT,		m_dps_hc);
	DDX_Control(pDX, IDC_DPS_VH1_EDIT,		m_dps_vh1);
	DDX_Control(pDX, IDC_DPS_VH2_EDIT,		m_dps_vh2);
	DDX_Control(pDX, IDC_DPS_VS1_EDIT,		m_dps_vs1);
	DDX_Control(pDX, IDC_DPS_VS2_EDIT,		m_dps_vs2);
	DDX_Control(pDX, IDC_DPS_VC_EDIT,		m_dps_vc);

	DDX_Control(pDX, IDC_HIST_Q_EDIT,		m_hist_q);
	DDX_Control(pDX, IDC_HIST_X_CUR_EDIT,	m_hist_xcur);
	DDX_Control(pDX, IDC_HIST_X_AVG_EDIT,	m_hist_xavg);

	DDX_Control(pDX, IDC_HIST_R00_EDIT,		m_hist_r00);
	DDX_Control(pDX, IDC_HIST_R08_EDIT,		m_hist_r08);
	DDX_Control(pDX, IDC_HIST_R10_EDIT,		m_hist_r10);
	DDX_Control(pDX, IDC_HIST_R18_EDIT,		m_hist_r18);
	DDX_Control(pDX, IDC_HIST_R20_EDIT,		m_hist_r20);
	DDX_Control(pDX, IDC_HIST_R28_EDIT,		m_hist_r28);
	DDX_Control(pDX, IDC_HIST_R30_EDIT,		m_hist_r30);
	DDX_Control(pDX, IDC_HIST_R38_EDIT,		m_hist_r38);
	DDX_Control(pDX, IDC_HIST_R40_EDIT,		m_hist_r40);
	DDX_Control(pDX, IDC_HIST_R48_EDIT,		m_hist_r48);
	DDX_Control(pDX, IDC_HIST_R50_EDIT,		m_hist_r50);
	DDX_Control(pDX, IDC_HIST_R58_EDIT,		m_hist_r58);
	DDX_Control(pDX, IDC_HIST_R60_EDIT,		m_hist_r60);
	DDX_Control(pDX, IDC_HIST_R68_EDIT,		m_hist_r68);
	DDX_Control(pDX, IDC_HIST_R70_EDIT,		m_hist_r70);
	DDX_Control(pDX, IDC_HIST_R78_EDIT,		m_hist_r78);
	DDX_Control(pDX, IDC_HIST_R80_EDIT,		m_hist_r80);
	DDX_Control(pDX, IDC_HIST_R88_EDIT,		m_hist_r88);
	DDX_Control(pDX, IDC_HIST_R90_EDIT,		m_hist_r90);
	DDX_Control(pDX, IDC_HIST_R98_EDIT,		m_hist_r98);
	DDX_Control(pDX, IDC_HIST_RA0_EDIT,		m_hist_ra0);
	DDX_Control(pDX, IDC_HIST_RA8_EDIT,		m_hist_ra8);
	DDX_Control(pDX, IDC_HIST_RB0_EDIT,		m_hist_rb0);
	DDX_Control(pDX, IDC_HIST_RB8_EDIT,		m_hist_rb8);
	DDX_Control(pDX, IDC_HIST_RC0_EDIT,		m_hist_rc0);
	DDX_Control(pDX, IDC_HIST_RC8_EDIT,		m_hist_rc8);
	DDX_Control(pDX, IDC_HIST_RD0_EDIT,		m_hist_rd0);
	DDX_Control(pDX, IDC_HIST_RD8_EDIT,		m_hist_rd8);
	DDX_Control(pDX, IDC_HIST_RE0_EDIT,		m_hist_re0);
	DDX_Control(pDX, IDC_HIST_RE8_EDIT,		m_hist_re8);
	DDX_Control(pDX, IDC_HIST_RF0_EDIT,		m_hist_rf0);
	DDX_Control(pDX, IDC_HIST_RF8_EDIT,		m_hist_rf8);
}


BEGIN_MESSAGE_MAP(CV568IppDlg, CBaseCtrlDlg)
	ON_WM_HSCROLL()
	// Check Box
	ON_BN_CLICKED(IDC_SPECIAL_EN_CHECK,		OnBnClickedSpecialEnCheck)
	ON_BN_CLICKED(IDC_FRAME_EN_CHECK,		OnBnClickedFrameEnCheck)
	ON_BN_CLICKED(IDC_TMS_EN_CHECK,			OnBnClickedTmsEnCheck)
	ON_BN_CLICKED(IDC_DPS_EN_CHECK,			OnBnClickedDpsEnCheck)
	ON_BN_CLICKED(IDC_CPUS_EN_CHECK,		OnBnClickedCpusEnCheck)
	ON_BN_CLICKED(IDC_CPDS_EN_CHECK,		OnBnClickedCpdsEnCheck)
	ON_BN_CLICKED(IDC_TMH_BP_EN_CHECK,		OnBnClickedTmhBpEnCheck)
	ON_BN_CLICKED(IDC_TMV_BP_EN_CHECK,		OnBnClickedTmvBpEnCheck)
	ON_BN_CLICKED(IDC_DPH_BP_EN_CHECK,		OnBnClickedDphBpEnCheck)
	ON_BN_CLICKED(IDC_DPV_BP_EN_CHECK,		OnBnClickedDpvBpEnCheck)
	ON_BN_CLICKED(IDC_CPUH_BP_EN_CHECK,		OnBnClickedCpuhBpEnCheck)
	ON_BN_CLICKED(IDC_CPUV_BP_EN_CHECK,		OnBnClickedCpuvBpEnCheck)
	ON_BN_CLICKED(IDC_CPDH_BP_EN_CHECK,		OnBnClickedCpdhBpEnCheck)
	ON_BN_CLICKED(IDC_CPDV_BP_EN_CHECK,		OnBnClickedCpdvBpEnCheck)

	// Combo Box
	ON_CBN_SELCHANGE(IDC_WORK_MODE_COMBO,	OnCbnSelchangeWorkModeCombo)
	ON_CBN_SELCHANGE(IDC_DATA_SRC_COMBO,	OnCbnSelchangeDataSrcCombo)
	ON_CBN_SELCHANGE(IDC_TMB_SRC_COMBO,		OnCbnSelchangeTmbSrcCombo)
	ON_CBN_SELCHANGE(IDC_SPE_TYPE_COMBO,	OnCbnSelchangeSpeTypeCombo)
	ON_CBN_SELCHANGE(IDC_HIST_SEL_COMBO,	OnCbnSelchangeHistSelCombo)
	ON_CBN_SELCHANGE(IDC_THUMB_FMT_COMBO,	OnCbnSelchangeThumbFmtCombo)

	// Button
	ON_BN_CLICKED(IDC_REFRESH_BUTTON,		OnBnClickedRefreshButton)
	ON_BN_CLICKED(IDC_APPLY_BUTTON,			OnBnClickedApplyButton)
	ON_BN_CLICKED(IDC_RESET_BUTTON,			OnBnClickedResetButton)
	ON_BN_CLICKED(IDC_T_SIZER_BUTTON,		OnBnClickedTSizerButton)
	ON_BN_CLICKED(IDC_C_SIZER_BUTTON,		OnBnClickedCSizerButton)
	ON_BN_CLICKED(IDC_D_SIZER_BUTTON,		OnBnClickedDSizerButton)
	ON_BN_CLICKED(IDC_IMAGE_SIZE_BUTTON,	OnBnClickedImageSizeButton)
	ON_BN_CLICKED(IDC_WIN_SIZE_BUTTON,		OnBnClickedWinSizeButton)
	ON_BN_CLICKED(IDC_HIST_GET_BUTTON,		OnBnClickedHistGetButton)
	ON_BN_CLICKED(IDC_HIST_SET_BUTTON,		OnBnClickedHistSetButton)
	ON_BN_CLICKED(IDC_HIST_EN_BUTTON, OnBnClickedHistEnButton)
END_MESSAGE_MAP()


// CV568IppDlg 消息处理程序

BOOL CV568IppDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispOption();

	return TRUE;
}

void CV568IppDlg::InitUserOption()
{
	memset(&(this->m_option), 0, sizeof(TIppOption));
	this->InitCombo();
}

void CV568IppDlg::GetUserOption(char *tail)
{
	this->GetEditboxValue();
	this->GetComboValue(tail);
}

void CV568IppDlg::DispOption()
{
	this->DispCombo();
	this->DispEditBox();
	this->DispCheckBox();
	this->AdjustScrollBar();
}

void CV568IppDlg::RefreshData(void)
{
	this->RefreshBar();
	this->RefreshCombo();
	this->RefreshEditBox();
	this->RefreshCheckBox();
	this->DispOption();
}

/******************************************** Check Box Section ********************************************/

void CV568IppDlg::OnBnClickedSpecialEnCheck()
{
	if(this->m_spe_en.GetCheck())
	{
		this->m_option.spe_en = 1;
		V5B_IppEnableSpecial();
	}
	else
	{
		this->m_option.spe_en = 0;
		V5B_IppDisableSpecial();
	}
}

void CV568IppDlg::OnBnClickedFrameEnCheck()
{
	if(this->m_frame_en.GetCheck())
	{
		this->m_option.ctrl0 |= IPP_FRAME_EN;
		V5B_IppEnableFrame();
	}
	else
	{
		this->m_option.ctrl0 &= ~IPP_FRAME_EN;
		V5B_IppDisableFrame();
	}
}

void CV568IppDlg::OnBnClickedTmsEnCheck()
{
	if(this->m_tms_en.GetCheck())
	{
		this->m_option.ctrl0 |= IPP_TMB_SIZER_EN;
		V5B_IppEnableThumbSizer();
	}
	else
	{
		this->m_option.ctrl0 &= ~IPP_TMB_SIZER_EN;
		V5B_IppDisableThumbSizer();
	}
}

void CV568IppDlg::OnBnClickedDpsEnCheck()
{
	if(this->m_dps_en.GetCheck())
	{
		this->m_option.ctrl0 |= IPP_DISP_SIZER_EN;
		V5B_IppEnableDispSizer();
	}
	else
	{
		this->m_option.ctrl0 &= ~IPP_DISP_SIZER_EN;
		V5B_IppDisableDispSizer();
	}
}

void CV568IppDlg::OnBnClickedCpusEnCheck()
{
	if(this->m_cpus_en.GetCheck())
	{
		this->m_option.ctrl0 |= IPP_CAP_USIZER_EN;
		V5B_IppEnableCapUpSizer();
	}
	else
	{
		this->m_option.ctrl0 &= ~IPP_CAP_USIZER_EN;
		this->m_option.ctrl1 &= ~(IPP_CAP_U_HSIZER_EN | IPP_CAP_U_VSIZER_EN);
		V5B_IppDisableCapUpSizer();
		V5B_IppDisableCPUHSBP();
		V5B_IppDisableCPUVSBP();
		this->m_cpuhsbp_en.SetCheck(BST_UNCHECKED);
		this->m_cpuvsbp_en.SetCheck(BST_UNCHECKED);
	}
}

void CV568IppDlg::OnBnClickedCpdsEnCheck()
{
	if(this->m_cpds_en.GetCheck())
	{
		this->m_option.ctrl0 |= IPP_CAP_DSIZER_EN;
		V5B_IppEnableCapDnSizer();
	}
	else
	{
		this->m_option.ctrl0 &= ~IPP_CAP_DSIZER_EN;
		this->m_option.ctrl1 &= ~(IPP_CAP_D_HSIZER_EN | IPP_CAP_D_VSIZER_EN);
		V5B_IppDisableCapDnSizer();
		V5B_IppDisableCPDHSBP();
		V5B_IppDisableCPDVSBP();
		this->m_cpdhsbp_en.SetCheck(BST_UNCHECKED);
		this->m_cpdvsbp_en.SetCheck(BST_UNCHECKED);
	}
}

void CV568IppDlg::OnBnClickedTmhBpEnCheck()
{
	if(this->m_tmhsbp_en.GetCheck())
	{
		this->m_option.ctrl1 |= IPP_TMB_HSIZER_EN;
		V5B_IppEnableTMHSBP();
	}
	else
	{
		this->m_option.ctrl1 &= ~IPP_TMB_HSIZER_EN;
		V5B_IppDisableTMHSBP();
	}
}

void CV568IppDlg::OnBnClickedTmvBpEnCheck()
{
	if(this->m_tmvsbp_en.GetCheck())
	{
		this->m_option.ctrl1 |= IPP_TMB_VSIZER_EN;
		V5B_IppEnableTMVSBP();
	}
	else
	{
		this->m_option.ctrl1 &= ~IPP_TMB_VSIZER_EN;
		V5B_IppDisableTMVSBP();
	}
}

void CV568IppDlg::OnBnClickedDphBpEnCheck()
{
	if(this->m_dphsbp_en.GetCheck())
	{
		this->m_option.ctrl1 |= IPP_DISP_HSIZER_EN;
		V5B_IppEnableDPHSBP();
	}
	else
	{
		this->m_option.ctrl1 &= ~IPP_DISP_HSIZER_EN;
		V5B_IppDisableDPHSBP();
	}
}

void CV568IppDlg::OnBnClickedDpvBpEnCheck()
{
	if(this->m_dpvsbp_en.GetCheck())
	{
		this->m_option.ctrl1 |= IPP_DISP_VSIZER_EN;
		V5B_IppEnableDPVSBP();
	}
	else
	{
		this->m_option.ctrl1 &= ~IPP_DISP_VSIZER_EN;
		V5B_IppDisableDPVSBP();
	}
}

void CV568IppDlg::OnBnClickedCpuhBpEnCheck()
{
	if(this->m_cpuhsbp_en.GetCheck())
	{
		this->m_option.ctrl1 |= IPP_CAP_U_HSIZER_EN;
		V5B_IppEnableCPUHSBP();
	}
	else
	{
		this->m_option.ctrl1 &= ~IPP_CAP_U_HSIZER_EN;
		V5B_IppDisableCPUHSBP();
	}
}

void CV568IppDlg::OnBnClickedCpuvBpEnCheck()
{
	if(this->m_cpuvsbp_en.GetCheck())
	{
		this->m_option.ctrl1 |= IPP_CAP_U_VSIZER_EN;
		V5B_IppEnableCPUVSBP();
	}
	else
	{
		this->m_option.ctrl1 &= ~IPP_CAP_U_VSIZER_EN;
		V5B_IppDisableCPUVSBP();
	}
}

void CV568IppDlg::OnBnClickedCpdhBpEnCheck()
{
	if(this->m_cpdhsbp_en.GetCheck())
	{
		this->m_option.ctrl1 |= IPP_CAP_D_HSIZER_EN;
		V5B_IppEnableCPDHSBP();
	}
	else
	{
		this->m_option.ctrl1 &= ~IPP_CAP_D_HSIZER_EN;
		V5B_IppDisableCPDHSBP();
	}
}

void CV568IppDlg::OnBnClickedCpdvBpEnCheck()
{
	if(this->m_cpdvsbp_en.GetCheck())
	{
		this->m_option.ctrl1 |= IPP_CAP_D_VSIZER_EN;
		V5B_IppEnableCPDVSBP();
	}
	else
	{
		this->m_option.ctrl1 &= ~IPP_CAP_D_VSIZER_EN;
		V5B_IppDisableCPDVSBP();
	}
}

void CV568IppDlg::RefreshCheckBox(void)
{
	this->m_option.spe_en  = V5B_IppIsSpecial();
	this->m_option.ctrl0   = V5B_IppGetSizerCtrl0();
	this->m_option.ctrl0   &= ( IPP_FRAME_EN | IPP_DISP_SIZER_EN | IPP_CAP_DSIZER_EN | IPP_CAP_USIZER_EN | IPP_TMB_SIZER_EN );
	this->m_option.ctrl1   = V5B_IppGetSizerCtrl1();
	this->m_option.ctrl1   &= ( IPP_TMB_VSIZER_EN | IPP_TMB_HSIZER_EN | IPP_DISP_VSIZER_EN | IPP_DISP_HSIZER_EN
							| IPP_CAP_U_VSIZER_EN | IPP_CAP_U_HSIZER_EN | IPP_CAP_D_VSIZER_EN | IPP_CAP_D_HSIZER_EN );
}

void CV568IppDlg::DispCheckBox(void)
{
	if(this->m_option.spe_en)
		this->m_spe_en.SetCheck(BST_CHECKED);
	else
		this->m_spe_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl0 & IPP_FRAME_EN)
		this->m_frame_en.SetCheck(BST_CHECKED);
	else
		this->m_frame_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl0 & IPP_DISP_SIZER_EN)
		this->m_dps_en.SetCheck(BST_CHECKED);
	else
		this->m_dps_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl0 & IPP_CAP_DSIZER_EN)
		this->m_cpds_en.SetCheck(BST_CHECKED);
	else
		this->m_cpds_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl0 & IPP_CAP_USIZER_EN)
		this->m_cpus_en.SetCheck(BST_CHECKED);
	else
		this->m_cpus_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl0 & IPP_TMB_SIZER_EN)
		this->m_tms_en.SetCheck(BST_CHECKED);
	else
		this->m_tms_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl1 & IPP_TMB_HSIZER_EN)
		this->m_tmhsbp_en.SetCheck(BST_CHECKED);
	else
		this->m_tmhsbp_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl1 & IPP_TMB_VSIZER_EN)
		this->m_tmvsbp_en.SetCheck(BST_CHECKED);
	else
		this->m_tmvsbp_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl1 & IPP_DISP_HSIZER_EN)
		this->m_dphsbp_en.SetCheck(BST_CHECKED);
	else
		this->m_dphsbp_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl1 & IPP_DISP_VSIZER_EN)
		this->m_dpvsbp_en.SetCheck(BST_CHECKED);
	else
		this->m_dpvsbp_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl1 & IPP_CAP_U_HSIZER_EN)
		this->m_cpuhsbp_en.SetCheck(BST_CHECKED);
	else
		this->m_cpuhsbp_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl1 & IPP_CAP_U_VSIZER_EN)
		this->m_cpuvsbp_en.SetCheck(BST_CHECKED);
	else
		this->m_cpuvsbp_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl1 & IPP_CAP_D_HSIZER_EN)
		this->m_cpdhsbp_en.SetCheck(BST_CHECKED);
	else
		this->m_cpdhsbp_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl1 & IPP_CAP_D_VSIZER_EN)
		this->m_cpdvsbp_en.SetCheck(BST_CHECKED);
	else
		this->m_cpdvsbp_en.SetCheck(BST_UNCHECKED);
}

/******************************************** Combo Section ********************************************/

void CV568IppDlg::InitCombo(void)
{
	int i = 0;
	const static char *workmode[] = 
	{
		"ViewFinder Mode", "Capture Mode", "Decoder Mode", "Display Mode"
	};
	const static char *ippsrc[] = 
	{
		"LBUF Data", "SIF Data", "ISP Data"
	};
	const static char *tmbsrc[] = 
	{
		"Special Effect", "Display Sizer", "Capture Sizer"
	};
	const static char *spetype[] = 
	{
		"Sephia", "Special Color", "Negative", "Sketch", "Mono Color", "Four Block", "TV", "Color Grid"
	};
	const static char *histsel[] = 
	{
		"Y  Input", "Cb Input", "Cr Input"
	};
	const static char *thumfmt[] = 
	{
		"YUV422 Format", "RGB565 Format"
	};

	for(i = 0; i < sizeof(workmode) / sizeof(char *); i++)
		this->m_modebox.InsertString(i, workmode[i]);
	for(i = 0; i < sizeof(ippsrc) / sizeof(char *); i++)
		this->m_datasrcbox.InsertString(i, ippsrc[i]);
	for(i = 0; i < sizeof(tmbsrc) / sizeof(char *); i++)
		this->m_tmbsrcbox.InsertString(i, tmbsrc[i]);
	for(i = 0; i < sizeof(spetype) / sizeof(char *); i++)
		this->m_spebox.InsertString(i, spetype[i]);
	for(i = 0; i < sizeof(histsel) / sizeof(char *); i++)
		this->m_histselbox.InsertString(i, histsel[i]);
	for(i = 0; i < sizeof(thumfmt) / sizeof(char *); i++)
		this->m_thumfmtbox.InsertString(i, thumfmt[i]);
}

void CV568IppDlg::GetComboValue(char *tail)
{
	this->m_option.workmode = this->m_modebox.GetCurSel();
	this->m_option.ippsrc   = this->m_datasrcbox.GetCurSel();
	this->m_option.tmbsrc   = this->m_tmbsrcbox.GetCurSel();
	this->m_option.spetype  = this->m_spebox.GetCurSel();
	this->m_option.histsel  = this->m_histselbox.GetCurSel();
	this->m_option.thumfmt  = this->m_thumfmtbox.GetCurSel();

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CV568IppDlg::RefreshCombo(void)
{
	this->m_option.workmode = V5B_IppGetWorkMode();
	this->m_option.ippsrc   = V5B_IppGetDataSelect();
	this->m_option.tmbsrc   = V5B_IppGetThumbInput();
	this->m_option.spetype  = V5B_IppGetSpecialEffect();
	this->m_option.histsel  = V5B_IppGetHistIn();
	this->m_option.thumfmt  = V5B_IppGetThumOutFmt();
}

void CV568IppDlg::DispCombo(void)
{
	this->m_modebox.SetCurSel(this->m_option.workmode);
	this->m_datasrcbox.SetCurSel(this->m_option.ippsrc);
	this->m_tmbsrcbox.SetCurSel(this->m_option.tmbsrc);
	this->m_spebox.SetCurSel(this->m_option.spetype);
	this->m_histselbox.SetCurSel(this->m_option.histsel);
	this->m_thumfmtbox.SetCurSel(this->m_option.thumfmt);
}

void CV568IppDlg::OnCbnSelchangeWorkModeCombo()
{
	this->m_option.workmode = this->m_modebox.GetCurSel();
	V5B_IppSetWorkMode((UINT8)this->m_option.workmode);
}

void CV568IppDlg::OnCbnSelchangeDataSrcCombo()
{
	this->m_option.ippsrc = this->m_datasrcbox.GetCurSel();
	V5B_IppSetDataSelect((UINT8)this->m_option.ippsrc);
}

void CV568IppDlg::OnCbnSelchangeTmbSrcCombo()
{
	this->m_option.tmbsrc = this->m_tmbsrcbox.GetCurSel();
	V5B_IppSetThumbInput((UINT8)this->m_option.tmbsrc);
}

void CV568IppDlg::OnCbnSelchangeSpeTypeCombo()
{
	this->m_option.spetype = this->m_spebox.GetCurSel();
	V5B_IppSetSpecialEffect((UINT8)this->m_option.spetype);
}

void CV568IppDlg::OnCbnSelchangeHistSelCombo()
{
	this->m_option.histsel = this->m_histselbox.GetCurSel();
	V5B_IppSetHistIn((UINT8)this->m_option.histsel);
}

void CV568IppDlg::OnCbnSelchangeThumbFmtCombo()
{
	this->m_option.thumfmt = this->m_thumfmtbox.GetCurSel();
	V5B_IppSetThumOutFmt((UINT8)this->m_option.thumfmt);
}

/******************************************** ScrollBar Section ********************************************/

void CV568IppDlg::GetBarValue(void)
{
	this->m_option.uoff = this->m_uoffbar.GetScrollPos();
	this->m_option.voff = this->m_voffbar.GetScrollPos();
	this->m_option.utop = this->m_utopbar.GetScrollPos();
	this->m_option.ubtm = this->m_ubtmbar.GetScrollPos();
	this->m_option.vtop = this->m_vtopbar.GetScrollPos();
	this->m_option.vbtm = this->m_vbtmbar.GetScrollPos();

	this->m_option.winpt.x  = (UINT16)this->m_winxbar.GetScrollPos();
	this->m_option.winpt.y  = (UINT16)this->m_winybar.GetScrollPos();
	this->m_option.winsz.cx = (UINT16)this->m_winwbar.GetScrollPos();
	this->m_option.winsz.cy = (UINT16)this->m_winhbar.GetScrollPos();

	this->m_option.thmsz.cx = (UINT16)this->m_tmswbar.GetScrollPos();
	this->m_option.thmsz.cy = (UINT16)this->m_tmshbar.GetScrollPos();
	this->m_option.capsz.cx = (UINT16)this->m_cpswbar.GetScrollPos();
	this->m_option.capsz.cy = (UINT16)this->m_cpshbar.GetScrollPos();
	this->m_option.dspsz.cx = (UINT16)this->m_dpswbar.GetScrollPos();
	this->m_option.dspsz.cy = (UINT16)this->m_dpshbar.GetScrollPos();
}

void CV568IppDlg::RefreshBar(void)
{
	this->m_option.uoff = V5B_IppGetUOffset();
	this->m_option.voff = V5B_IppGetVOffset();
	this->m_option.utop = V5B_IppGetUTopThrd();
	this->m_option.ubtm = V5B_IppGetUBottomThrd();
	this->m_option.vtop = V5B_IppGetVTopThrd();
	this->m_option.vbtm = V5B_IppGetVBottomThrd();

	this->m_option.winpt.x  = V5B_IppGetWinStartX();
	this->m_option.winpt.y  = V5B_IppGetWinStartY();
	this->m_option.winsz.cx = V5B_IppGetWinWidth();
	this->m_option.winsz.cy = V5B_IppGetWinHeight();

	this->m_option.thmsz.cx = V5B_IppGetThumbWidth();
	this->m_option.thmsz.cy = V5B_IppGetThumbHeight();
	this->m_option.capsz.cx = V5B_IppGetCapWidth();
	this->m_option.capsz.cy = V5B_IppGetCapHeight();
	this->m_option.dspsz.cx = V5B_IppGetDispWidth();
	this->m_option.dspsz.cy = V5B_IppGetDispHeight();
}

void CV568IppDlg::DispBarEditBox(void)
{
	char str[20];

	sprintf(str, "%x", this->m_option.uoff);
	this->m_uoff.SetWindowText(str);
	sprintf(str, "%x", this->m_option.voff);
	this->m_voff.SetWindowText(str);
	sprintf(str, "%x", this->m_option.utop);
	this->m_utop.SetWindowText(str);
	sprintf(str, "%x", this->m_option.ubtm);
	this->m_ubtm.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vtop);
	this->m_vtop.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vbtm);
	this->m_vbtm.SetWindowText(str);

	sprintf(str, "%x", this->m_option.winpt.x);
	this->m_winx.SetWindowText(str);
	sprintf(str, "%x", this->m_option.winpt.y);
	this->m_winy.SetWindowText(str);
	sprintf(str, "%x", this->m_option.winsz.cx);
	this->m_winw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.winsz.cy);
	this->m_winh.SetWindowText(str);

	sprintf(str, "%x", this->m_option.thmsz.cx);
	this->m_tmsw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.thmsz.cy);
	this->m_tmsh.SetWindowText(str);
	sprintf(str, "%x", this->m_option.capsz.cx);
	this->m_cpsw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.capsz.cy);
	this->m_cpsh.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dspsz.cx);
	this->m_dpsw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dspsz.cy);
	this->m_dpsh.SetWindowText(str);
}

void CV568IppDlg::AdjustScrollBar(void)
{
	TSize size;

	this->m_uoffbar.SetScrollRange(0, 255);
	this->m_uoffbar.SetScrollPos(this->m_option.uoff);
	this->m_voffbar.SetScrollRange(0, 255);
	this->m_voffbar.SetScrollPos(this->m_option.voff);

	this->m_utopbar.SetScrollRange(0, 255);
	this->m_utopbar.SetScrollPos(this->m_option.utop);
	this->m_vtopbar.SetScrollRange(0, 255);
	this->m_vtopbar.SetScrollPos(this->m_option.vtop);

	this->m_ubtmbar.SetScrollRange(0, 255);
	this->m_ubtmbar.SetScrollPos(this->m_option.ubtm);
	this->m_vbtmbar.SetScrollRange(0, 255);
	this->m_vbtmbar.SetScrollPos(this->m_option.vbtm);

	this->m_winxbar.SetScrollRange(0, (this->m_option.imgsz.cx - this->m_option.winsz.cx));
	this->m_winxbar.SetScrollPos(this->m_option.winpt.x);
	this->m_winybar.SetScrollRange(0, (this->m_option.imgsz.cy - this->m_option.winsz.cy));
	this->m_winybar.SetScrollPos(this->m_option.winpt.y);

	this->m_winwbar.SetScrollRange(0, this->m_option.imgsz.cx);
	this->m_winwbar.SetScrollPos(this->m_option.winsz.cx);
	this->m_winhbar.SetScrollRange(0, this->m_option.imgsz.cy);
	this->m_winhbar.SetScrollPos(this->m_option.winsz.cy);

	switch(this->m_option.tmbsrc)
	{
	case IPP_TMB_SPECIAL :
		size = this->m_option.winsz;
		break;
	case IPP_TMB_DSIZER :
		size = this->m_option.dspsz;
		break;
	default :
		size = this->m_option.capsz;
		break;
	}
	this->m_tmswbar.SetScrollRange(IPP_TSIZER_HOUT_MIN, ((size.cx > IPP_TSIZER_HOUT_MAX) ? IPP_TSIZER_HOUT_MAX : size.cx));
	this->m_tmswbar.SetScrollPos(((this->m_option.thmsz.cx > IPP_TSIZER_HOUT_MAX) ? IPP_TSIZER_HOUT_MAX : this->m_option.thmsz.cx));
	this->m_tmshbar.SetScrollRange(IPP_TSIZER_VOUT_MIN, ((size.cy > IPP_TSIZER_VOUT_MAX) ? IPP_TSIZER_VOUT_MAX : size.cy));
	this->m_tmshbar.SetScrollPos(((this->m_option.thmsz.cy > IPP_TSIZER_VOUT_MAX) ? IPP_TSIZER_VOUT_MAX : this->m_option.thmsz.cy));

	size.cx = (UINT16)((this->m_option.winsz.cx > IPP_CSIZER_HIN_MAX) ? this->m_option.winsz.cx : (this->m_option.winsz.cx << 1));
	size.cy = (UINT16)(((this->m_option.winsz.cy << 1) > IPP_CSIZER_VOUT_MAX) ? IPP_CSIZER_VOUT_MAX : (this->m_option.winsz.cy << 1));
	this->m_cpswbar.SetScrollRange(0, size.cx);
	this->m_cpswbar.SetScrollPos(this->m_option.capsz.cx);
	this->m_cpshbar.SetScrollRange(0, size.cy);
	this->m_cpshbar.SetScrollPos(this->m_option.capsz.cy);

	size.cx = (UINT16)((this->m_option.winsz.cx > 510) ? 510 : this->m_option.winsz.cx);
	size.cy = (UINT16)((this->m_option.winsz.cy > 512) ? 512 : this->m_option.winsz.cy);
	if(this->m_option.workmode == IPP_MODE_DEC)
		size = this->m_option.winsz;
	this->m_dpswbar.SetScrollRange(0, size.cx);
	this->m_dpswbar.SetScrollPos(this->m_option.dspsz.cx);
	this->m_dpshbar.SetScrollRange(0, size.cy);
	this->m_dpshbar.SetScrollPos(this->m_option.dspsz.cy);

	this->GetBarValue();
	this->DispBarEditBox();
}

void CV568IppDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int pos = 0;
	CWaitCursor cursor;

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
	if(pScrollBar == &(this->m_uoffbar))
		V5B_IppSetUOffset((UINT8)this->m_option.uoff);
	else if(pScrollBar == &(this->m_voffbar))
		V5B_IppSetVOffset((UINT8)this->m_option.voff);
	else if(pScrollBar == &(this->m_utopbar))
		V5B_IppSetUTopThrd((UINT8)this->m_option.utop);
	else if(pScrollBar == &(this->m_ubtmbar))
		V5B_IppSetUBottomThrd((UINT8)this->m_option.ubtm);
	else if(pScrollBar == &(this->m_vtopbar))
		V5B_IppSetVTopThrd((UINT8)this->m_option.vtop);
	else if(pScrollBar == &(this->m_vbtmbar))
		V5B_IppSetVBottomThrd((UINT8)this->m_option.vbtm);

	CBaseCtrlDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

/********************************************* EditBox Section *********************************************/

void CV568IppDlg::GetEditboxValue(void)
{
	char str[20];

	this->m_imgw.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.imgsz.cx));
	this->m_imgh.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.imgsz.cy));

	this->m_dispdrop.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.ddrop));
	this->m_captdrop.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.cdrop));

	this->m_hist_q.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.hist_q));
}

void CV568IppDlg::RefreshEditBox(void)
{
	this->m_option.ddrop = V5B_IppGetDispFrmDrop();
	this->m_option.cdrop = V5B_IppGetCapFrmDrop();

	V5B_IppGetImageSize(&(this->m_option.imgsz));
	V5B_IppGetThumbHFactor(&(this->m_option.tmhsfct));
	V5B_IppGetThumbVFactor(&(this->m_option.tmvsfct));
	V5B_IppGetCapUpHFactor(&(this->m_option.cpuhsfct));
	V5B_IppGetCapUpVFactor(&(this->m_option.cpuvsfct));
	V5B_IppGetCapDnHFactor(&(this->m_option.cpdhsfct));
	V5B_IppGetCapDnVFactor(&(this->m_option.cpdvsfct));
	V5B_IppGetDispHFactor(&(this->m_option.dphsfct));
	V5B_IppGetDispVFactor(&(this->m_option.dpvsfct));
}

void CV568IppDlg::DispEditBox(void)
{
	char str[20];

	sprintf(str, "%x", this->m_option.imgsz.cx);
	this->m_imgw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.imgsz.cy);
	this->m_imgh.SetWindowText(str);

	sprintf(str, "%x", this->m_option.ddrop);
	this->m_dispdrop.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cdrop);
	this->m_captdrop.SetWindowText(str);

	sprintf(str, "%x", this->m_option.tmhsfct.w1);
	this->m_tms_hw1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.tmhsfct.w2);
	this->m_tms_hw2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.tmhsfct.s1);
	this->m_tms_hs1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.tmhsfct.s2);
	this->m_tms_hs2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.tmhsfct.c);
	this->m_tms_hc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.tmvsfct.w1);
	this->m_tms_vh1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.tmvsfct.w2);
	this->m_tms_vh2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.tmvsfct.s1);
	this->m_tms_vs1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.tmvsfct.s2);
	this->m_tms_vs2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.tmvsfct.c);
	this->m_tms_vc.SetWindowText(str);

	sprintf(str, "%x", this->m_option.cpdhsfct.w1);
	this->m_cpds_hw1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpdhsfct.w2);
	this->m_cpds_hw2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpdhsfct.s1);
	this->m_cpds_hs1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpdhsfct.s2);
	this->m_cpds_hs2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpdhsfct.c);
	this->m_cpds_hc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpdvsfct.w1);
	this->m_cpds_vh1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpdvsfct.w2);
	this->m_cpds_vh2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpdvsfct.s1);
	this->m_cpds_vs1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpdvsfct.s2);
	this->m_cpds_vs2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpdvsfct.c);
	this->m_cpds_vc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpuhsfct.w1);
	this->m_cpus_hw1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpuhsfct.w2);
	this->m_cpus_hw2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpuhsfct.s);
	this->m_cpus_hs.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpuvsfct.w1);
	this->m_cpus_vh1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpuvsfct.w2);
	this->m_cpus_vh2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cpuvsfct.s);
	this->m_cpus_vs.SetWindowText(str);

	sprintf(str, "%x", this->m_option.dphsfct.w1);
	this->m_dps_hw1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dphsfct.w2);
	this->m_dps_hw2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dphsfct.s1);
	this->m_dps_hs1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dphsfct.s2);
	this->m_dps_hs2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dphsfct.c);
	this->m_dps_hc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dpvsfct.w1);
	this->m_dps_vh1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dpvsfct.w2);
	this->m_dps_vh2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dpvsfct.s1);
	this->m_dps_vs1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dpvsfct.s2);
	this->m_dps_vs2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dpvsfct.c);
	this->m_dps_vc.SetWindowText(str);

	sprintf(str, "%x", this->m_option.hist_q);
	this->m_hist_q.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.x_cur);
	this->m_hist_xcur.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.x_avg);
	this->m_hist_xavg.SetWindowText(str);

	sprintf(str, "%x", this->m_option.histstat.buf[0]);
	this->m_hist_r00.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[1]);
	this->m_hist_r08.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[2]);
	this->m_hist_r10.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[3]);
	this->m_hist_r18.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[4]);
	this->m_hist_r20.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[5]);
	this->m_hist_r28.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[6]);
	this->m_hist_r30.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[7]);
	this->m_hist_r38.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[8]);
	this->m_hist_r40.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[9]);
	this->m_hist_r48.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[10]);
	this->m_hist_r50.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[11]);
	this->m_hist_r58.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[12]);
	this->m_hist_r60.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[13]);
	this->m_hist_r68.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[14]);
	this->m_hist_r70.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[15]);
	this->m_hist_r78.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[16]);
	this->m_hist_r80.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[17]);
	this->m_hist_r88.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[18]);
	this->m_hist_r90.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[19]);
	this->m_hist_r98.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[20]);
	this->m_hist_ra0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[21]);
	this->m_hist_ra8.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[22]);
	this->m_hist_rb0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[23]);
	this->m_hist_rb8.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[24]);
	this->m_hist_rc0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[25]);
	this->m_hist_rc8.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[26]);
	this->m_hist_rd0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[27]);
	this->m_hist_rd8.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[28]);
	this->m_hist_re0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[29]);
	this->m_hist_re8.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[30]);
	this->m_hist_rf0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.histstat.buf[31]);
	this->m_hist_rf8.SetWindowText(str);
}

/********************************************** Button Section *********************************************/

void CV568IppDlg::OnBnClickedRefreshButton()
{
	this->RefreshData();
}

void CV568IppDlg::OnBnClickedApplyButton()
{
	this->GetUserOption();

	V5B_IppSetDispFrmDrop((UINT16)this->m_option.ddrop);
	V5B_IppSetCapFrmDrop((UINT16)this->m_option.cdrop);
}

void CV568IppDlg::OnBnClickedResetButton()
{
	V5B_IppReset();
}

void CV568IppDlg::OnBnClickedTSizerButton()
{
	this->GetUserOption();

	V5B_IppSetThumbSize(this->m_option.thmsz);
	V5_IppThumbSizerUpdate();

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();

	this->RefreshData();
}

void CV568IppDlg::OnBnClickedCSizerButton()
{
	this->GetUserOption();

	V5B_LcdcDisableAlayer();
	V5B_IppSetThumbSize(this->m_option.thmsz);
	V5_IppSetCapSizerEx2(this->m_option.imgsz, this->m_option.winpt, this->m_option.winsz, this->m_option.capsz);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_LcdcEnableAlayer();

	this->RefreshData();
}

void CV568IppDlg::OnBnClickedDSizerButton()
{
	this->GetUserOption();

	V5B_LcdcDisableAlayer();
	V5B_IppSetThumbSize(this->m_option.thmsz);
	V5_IppSetDispSizerEx2(this->m_option.imgsz, this->m_option.winpt, this->m_option.winsz, this->m_option.dspsz);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_LcdcEnableAlayer();

	this->RefreshData();
}

void CV568IppDlg::OnBnClickedImageSizeButton()
{
	this->GetUserOption();

	V5B_LcdcDisableAlayer();
	V5B_IppSetThumbSize(this->m_option.thmsz);
	V5_IppSetDispSizerEx2(this->m_option.imgsz, this->m_option.winpt, this->m_option.winsz, this->m_option.dspsz);
	if(this->m_option.workmode == IPP_MODE_CAP)
		V5_IppSetCapSizerEx2(this->m_option.imgsz, this->m_option.winpt, this->m_option.winsz, this->m_option.capsz);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_LcdcEnableAlayer();

	this->RefreshData();
}

void CV568IppDlg::OnBnClickedWinSizeButton()
{
	this->GetUserOption();

	V5B_LcdcDisableAlayer();
	V5B_IppSetThumbSize(this->m_option.thmsz);
	V5_IppSetDispSizerEx1(this->m_option.winpt, this->m_option.winsz, this->m_option.dspsz);
	if(this->m_option.workmode == IPP_MODE_CAP)
		V5_IppSetCapSizerEx1(this->m_option.winpt, this->m_option.winsz, this->m_option.capsz);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_LcdcEnableAlayer();

	this->RefreshData();
}

void CV568IppDlg::OnBnClickedHistGetButton()
{
	this->m_option.hist_q = V5B_IppGetHistQ();
	V5_VispGetHistStat(&(this->m_option.histstat));
	this->DispEditBox();
}

void CV568IppDlg::OnBnClickedHistSetButton()
{
	this->GetEditboxValue();
	V5B_IppSetHistQ((UINT32)this->m_option.hist_q);
}

void CV568IppDlg::OnBnClickedHistEnButton()
{
	V5B_IppEnableHist();
}

#endif //V578CMD_DISABLE
