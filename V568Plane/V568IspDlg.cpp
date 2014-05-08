// V568IspDlg.cpp : 实现文件
//

#include "internal.h"
#include "..\include\v568plane\v568ispdlg.h"

#ifndef	V578CMD_DISABLE

// CV568IspDlg 对话框

IMPLEMENT_DYNAMIC(CV568IspDlg, CBaseCtrlDlg)
CV568IspDlg::CV568IspDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568IspDlg::IDD, pParent)
{
}

CV568IspDlg::~CV568IspDlg()
{
}

void CV568IspDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);

	// Check Box
	DDX_Control(pDX, IDC_GP_FIRST_CHECK,			m_gpfirst);
	DDX_Control(pDX, IDC_EDGE_EN_CHECK,				m_edge_en);
	DDX_Control(pDX, IDC_LFO_EN_CHECK,				m_lfo_en);
	DDX_Control(pDX, IDC_DPDC_EN_CHECK,				m_dpdc_en);
	DDX_Control(pDX, IDC_RGB_GAMMA_EN_CHECK,		m_gamma_en);
	DDX_Control(pDX, IDC_MATRIX_EN_CHECK,			m_matrix_en);
	DDX_Control(pDX, IDC_Y_GAMMA_EN_CHECK,			m_ygamma_en);
	DDX_Control(pDX, IDC_RGB_GAIN_EN_CHECK,			m_rgbgain_en);
	DDX_Control(pDX, IDC_AF_LINE_CHECK,				m_aflnmode);
	DDX_Control(pDX, IDC_GLOBAL_GAIN_EN_CHECK,		m_glgain_en);
	DDX_Control(pDX, IDC_ANTI_FLICKER_EN_CHECK,		m_aflk_en);
	DDX_Control(pDX, IDC_AWB_EN_CHECK,				m_awb_en);
	DDX_Control(pDX, IDC_NOISE_Y_EN_CHECK,			m_ynoise_en);
	DDX_Control(pDX, IDC_NOISE_UV_EN_CHECK,			m_uvnoise_en);
	DDX_Control(pDX, IDC_AUTO_FOCUS_EN_CHECK,		m_afocus_en);
	DDX_Control(pDX, IDC_TEST_BAR_EN_CHECK,			m_bar_en);
	DDX_Control(pDX, IDC_BLACK_IMAGE_EN_CHECK,		m_blkimg_en);
	DDX_Control(pDX, IDC_FOCUS_INT_EN_CHECK,		m_fcsint_en);
	DDX_Control(pDX, IDC_FLICKER_INT_EN_CHECK,		m_flkint_en);
	DDX_Control(pDX, IDC_Y_MEAN_INT_EN_CHECK,		m_ymnint_en);
	DDX_Control(pDX, IDC_HIST_EFFECT_EN_CHECK,		m_hist_eft_en);
	DDX_Control(pDX, IDC_HIST_MANUAL_EN_CHECK,		m_hist_mnl_en);
	DDX_Control(pDX, IDC_VSYNC_RST_CHECK,			m_vsync_rst);
	DDX_Control(pDX, IDC_HUE_EN_CHECK,				m_hue_en);

	// Combo Box
	DDX_Control(pDX, IDC_PIXEL_RATE_COMBO,			m_pratebox);
	DDX_Control(pDX, IDC_FIRST_LINE_COMBO,			m_fstlinebox);
	DDX_Control(pDX, IDC_SIF_FMT_COMBO,				m_siffmtbox);
	DDX_Control(pDX, IDC_FILTER_COMBO,				m_filterbox);
	DDX_Control(pDX, IDC_IMAGE_MODE_COMBO,			m_imgmodebox);
	DDX_Control(pDX, IDC_GLOBAL_GAIN_STEP_COMBO,	m_glstepbox);
	DDX_Control(pDX, IDC_GLOBAL_GAIN_DLY_COMBO,		m_gldlybox);
	DDX_Control(pDX, IDC_GAMMA_SEL_COMBO,			m_gmaselbox);
	DDX_Control(pDX, IDC_AF_INT_FREQ_COMBO,			m_intfreqbox);
	DDX_Control(pDX, IDC_AE_WEIGHT_COMBO,			m_aeweightbox);
	DDX_Control(pDX, IDC_WIN_WEIGHT_COMBO,			m_winweightbox);
	DDX_Control(pDX, IDC_AWB_MODE_COMBO,			m_awbmodebox);
	DDX_Control(pDX, IDC_AWB_WIN_COMBO,				m_awbwinbox);
	DDX_Control(pDX, IDC_EDGE_KERNEL_COMBO,			m_kernelbox);
	DDX_Control(pDX, IDC_FILTER_MODE_COMBO,			m_blfilterbox);
	DDX_Control(pDX, IDC_TEST_BAR_COMBO,			m_barmodebox);

	// ScrollBar

	// ScrollBar EditBox

	// EditBox
	DDX_Control(pDX, IDC_BARW_EDIT,					m_barw);
	DDX_Control(pDX, IDC_BARH_EDIT,					m_barh);
	DDX_Control(pDX, IDC_IMGW_EDIT,					m_imgw);
	DDX_Control(pDX, IDC_IMGH_EDIT,					m_imgh);

	DDX_Control(pDX, IDC_FEMM_EDIT,					m_femm);
	DDX_Control(pDX, IDC_FEMP_EDIT,					m_femp);
	DDX_Control(pDX, IDC_FEMX1_EDIT,				m_femx1);
	DDX_Control(pDX, IDC_FEMX2_EDIT,				m_femx2);
	DDX_Control(pDX, IDC_FEM_MIN_EDIT,				m_femmin);
	DDX_Control(pDX, IDC_FEM_MAX_EDIT,				m_femmax);

	DDX_Control(pDX, IDC_CENX_EDIT,					m_cenx);
	DDX_Control(pDX, IDC_CENY_EDIT,					m_ceny);
	DDX_Control(pDX, IDC_F2_EDIT,					m_f2);

	DDX_Control(pDX, IDC_LINE_CNT_EDIT,				m_aflncnt);
	DDX_Control(pDX, IDC_LINE_START_EDIT,			m_aflnst);
	DDX_Control(pDX, IDC_AF_STAT_EDIT,				m_afstat);
	DDX_Control(pDX, IDC_LINE_STEP_EDIT,			m_aflnstep);
	DDX_Control(pDX, IDC_TAIL_BLANK_EDIT,			m_blank);

	DDX_Control(pDX, IDC_RGAIN_EDIT,				m_rgain);
	DDX_Control(pDX, IDC_GGAIN_EDIT,				m_ggain);
	DDX_Control(pDX, IDC_BGAIN_EDIT,				m_bgain);
	DDX_Control(pDX, IDC_GLOBAL_GAIN_VAL_EDIT,		m_glgain);

	DDX_Control(pDX, IDC_CR_ADJ_EDIT,				m_cradj);
	DDX_Control(pDX, IDC_CB_ADJ_EDIT,				m_cbadj);
	DDX_Control(pDX, IDC_CR_OFT_EDIT,				m_croft);
	DDX_Control(pDX, IDC_CB_OFT_EDIT,				m_cboft);

	DDX_Control(pDX, IDC_MAT11_EDIT,				m_mat11);
	DDX_Control(pDX, IDC_MAT12_EDIT,				m_mat12);
	DDX_Control(pDX, IDC_MAT13_EDIT,				m_mat13);
	DDX_Control(pDX, IDC_MAT21_EDIT,				m_mat21);
	DDX_Control(pDX, IDC_MAT22_EDIT,				m_mat22);
	DDX_Control(pDX, IDC_MAT23_EDIT,				m_mat23);
	DDX_Control(pDX, IDC_MAT31_EDIT,				m_mat31);
	DDX_Control(pDX, IDC_MAT32_EDIT,				m_mat32);
	DDX_Control(pDX, IDC_MAT33_EDIT,				m_mat33);
	DDX_Control(pDX, IDC_ROFT_EDIT,					m_roft);
	DDX_Control(pDX, IDC_GOFT_EDIT,					m_goft);
	DDX_Control(pDX, IDC_BOFT_EDIT,					m_boft);

	DDX_Control(pDX, IDC_YSTART0_EDIT,				m_gmatbl0);
	DDX_Control(pDX, IDC_YSTART1_EDIT,				m_gmatbl1);
	DDX_Control(pDX, IDC_YSTART2_EDIT,				m_gmatbl2);
	DDX_Control(pDX, IDC_YSTART3_EDIT,				m_gmatbl3);
	DDX_Control(pDX, IDC_YSTART4_EDIT,				m_gmatbl4);
	DDX_Control(pDX, IDC_YSTART5_EDIT,				m_gmatbl5);
	DDX_Control(pDX, IDC_YSTART6_EDIT,				m_gmatbl6);
	DDX_Control(pDX, IDC_YSTART7_EDIT,				m_gmatbl7);
	DDX_Control(pDX, IDC_YSTART8_EDIT,				m_gmatbl8);
	DDX_Control(pDX, IDC_YSLOPE0_EDIT,				m_gmatbl9);
	DDX_Control(pDX, IDC_YSLOPE1_EDIT,				m_gmatbl10);
	DDX_Control(pDX, IDC_YSLOPE2_EDIT,				m_gmatbl11);
	DDX_Control(pDX, IDC_YSLOPE3_EDIT,				m_gmatbl12);
	DDX_Control(pDX, IDC_YSLOPE4_EDIT,				m_gmatbl13);
	DDX_Control(pDX, IDC_YSLOPE5_EDIT,				m_gmatbl14);
	DDX_Control(pDX, IDC_YSLOPE6_EDIT,				m_gmatbl15);
	DDX_Control(pDX, IDC_YSLOPE7_EDIT,				m_gmatbl16);
	DDX_Control(pDX, IDC_YSLOPE8_EDIT,				m_gmatbl17);

	DDX_Control(pDX, IDC_HSTARTA_EDIT,				m_hstarta);
	DDX_Control(pDX, IDC_HSTARTB_EDIT,				m_hstartb);
	DDX_Control(pDX, IDC_HSTARTC_EDIT,				m_hstartc);
	DDX_Control(pDX, IDC_HSTARTD_EDIT,				m_hstartd);
	DDX_Control(pDX, IDC_HSTARTE_EDIT,				m_hstarte);
	DDX_Control(pDX, IDC_VSTARTA_EDIT,				m_vstarta);
	DDX_Control(pDX, IDC_VSTARTB_EDIT,				m_vstartb);
	DDX_Control(pDX, IDC_VSTARTC_EDIT,				m_vstartc);
	DDX_Control(pDX, IDC_VSTARTD_EDIT,				m_vstartd);
	DDX_Control(pDX, IDC_VSTARTE_EDIT,				m_vstarte);

	DDX_Control(pDX, IDC_YBOT_EDIT,					m_ybot);
	DDX_Control(pDX, IDC_YTOP_EDIT,					m_ytop);
	DDX_Control(pDX, IDC_QTOP_EDIT,					m_qtop);
	DDX_Control(pDX, IDC_ITOP_EDIT,					m_itop);

	DDX_Control(pDX, IDC_AWB_GAIN_MIN_EDIT,			m_glow);
	DDX_Control(pDX, IDC_AWB_GAIN_MAX_EDIT,			m_gtop);

	DDX_Control(pDX, IDC_AWB_STEP_EDIT,				m_awbstep);
	DDX_Control(pDX, IDC_AWB_THD1_EDIT,				m_awbthd1);
	DDX_Control(pDX, IDC_AWB_THD2_EDIT,				m_awbthd2);

	DDX_Control(pDX, IDC_RMEAN_EDIT,				m_rmean);
	DDX_Control(pDX, IDC_GMEAN_EDIT,				m_gmean);
	DDX_Control(pDX, IDC_BMEAN_EDIT,				m_bmean);
	DDX_Control(pDX, IDC_YMEAN_EDIT,				m_ymean);

	DDX_Control(pDX, IDC_AF_STATUS_EDIT,			m_afstatus);
	DDX_Control(pDX, IDC_EDGE_SUM_EDIT,				m_edgesum);
	DDX_Control(pDX, IDC_HIST_GAIN_EDIT,			m_histgain);

	DDX_Control(pDX, IDC_BL_R_OFT_EDIT,				m_blroff);
	DDX_Control(pDX, IDC_BL_B_OFT_EDIT,				m_blboff);
	DDX_Control(pDX, IDC_BL_G1_OFT_EDIT,			m_blg1off);
	DDX_Control(pDX, IDC_BL_G2_OFT_EDIT,			m_blg2off);

	DDX_Control(pDX, IDC_DPT_THD_EDIT,				m_dpdthd);
	DDX_Control(pDX, IDC_DPT_UV_THD_EDIT,			m_dpduvthd);

	DDX_Control(pDX, IDC_YMEAN_TGT_EDIT,			m_ymeantgt);
	DDX_Control(pDX, IDC_YMEAN_THD_EDIT,			m_ymeanthd);
}


BEGIN_MESSAGE_MAP(CV568IspDlg, CBaseCtrlDlg)
	ON_WM_HSCROLL()
	// Check Box
	ON_BN_CLICKED(IDC_GP_FIRST_CHECK,				OnBnClickedGpFirstCheck)
	ON_BN_CLICKED(IDC_EDGE_EN_CHECK,				OnBnClickedEdgeEnCheck)
	ON_BN_CLICKED(IDC_LFO_EN_CHECK,					OnBnClickedLfoEnCheck)
	ON_BN_CLICKED(IDC_DPDC_EN_CHECK,				OnBnClickedDpdcEnCheck)
	ON_BN_CLICKED(IDC_RGB_GAMMA_EN_CHECK,			OnBnClickedRgbGammaEnCheck)
	ON_BN_CLICKED(IDC_MATRIX_EN_CHECK,				OnBnClickedMatrixEnCheck)
	ON_BN_CLICKED(IDC_Y_GAMMA_EN_CHECK,				OnBnClickedYGammaEnCheck)
	ON_BN_CLICKED(IDC_RGB_GAIN_EN_CHECK,			OnBnClickedRgbGainEnCheck)
	ON_BN_CLICKED(IDC_AF_LINE_CHECK,				OnBnClickedAfLineCheck)
	ON_BN_CLICKED(IDC_GLOBAL_GAIN_EN_CHECK,			OnBnClickedGlobalGainEnCheck)
	ON_BN_CLICKED(IDC_ANTI_FLICKER_EN_CHECK,		OnBnClickedAntiFlickerEnCheck)
	ON_BN_CLICKED(IDC_AWB_EN_CHECK,					OnBnClickedAwbEnCheck)
	ON_BN_CLICKED(IDC_NOISE_Y_EN_CHECK,				OnBnClickedNoiseYEnCheck)
	ON_BN_CLICKED(IDC_NOISE_UV_EN_CHECK,			OnBnClickedNoiseUvEnCheck)
	ON_BN_CLICKED(IDC_AUTO_FOCUS_EN_CHECK,			OnBnClickedAutoFocusEnCheck)
	ON_BN_CLICKED(IDC_TEST_BAR_EN_CHECK,			OnBnClickedTestBarEnCheck)
	ON_BN_CLICKED(IDC_BLACK_IMAGE_EN_CHECK,			OnBnClickedBlackImageEnCheck)
	ON_BN_CLICKED(IDC_FOCUS_INT_EN_CHECK,			OnBnClickedFocusIntEnCheck)
	ON_BN_CLICKED(IDC_FLICKER_INT_EN_CHECK,			OnBnClickedFlickerIntEnCheck)
	ON_BN_CLICKED(IDC_Y_MEAN_INT_EN_CHECK,			OnBnClickedYMeanIntEnCheck)
	ON_BN_CLICKED(IDC_HIST_EFFECT_EN_CHECK,			OnBnClickedHistEffectEnCheck)
	ON_BN_CLICKED(IDC_HIST_MANUAL_EN_CHECK,			OnBnClickedHistManualEnCheck)
	ON_BN_CLICKED(IDC_VSYNC_RST_CHECK,				OnBnClickedVsyncRstCheck)
	ON_BN_CLICKED(IDC_HUE_EN_CHECK,					OnBnClickedHueEnCheck)

	// Combo Box
	ON_CBN_SELCHANGE(IDC_PIXEL_RATE_COMBO,			OnCbnSelchangePixelRateCombo)
	ON_CBN_SELCHANGE(IDC_FIRST_LINE_COMBO,			OnCbnSelchangeFirstLineCombo)
	ON_CBN_SELCHANGE(IDC_SIF_FMT_COMBO,				OnCbnSelchangeSifFmtCombo)
	ON_CBN_SELCHANGE(IDC_FILTER_COMBO,				OnCbnSelchangeFilterCombo)
	ON_CBN_SELCHANGE(IDC_IMAGE_MODE_COMBO,			OnCbnSelchangeImageModeCombo)
	ON_CBN_SELCHANGE(IDC_GLOBAL_GAIN_STEP_COMBO,	OnCbnSelchangeGlobalGainStepCombo)
	ON_CBN_SELCHANGE(IDC_GLOBAL_GAIN_DLY_COMBO,		OnCbnSelchangeGlobalGainDlyCombo)
	ON_CBN_SELCHANGE(IDC_GAMMA_SEL_COMBO,			OnCbnSelchangeGammaSelCombo)
	ON_CBN_SELCHANGE(IDC_AF_INT_FREQ_COMBO,			OnCbnSelchangeAfIntFreqCombo)
	ON_CBN_SELCHANGE(IDC_AE_WEIGHT_COMBO,			OnCbnSelchangeAeWeightCombo)
	ON_CBN_SELCHANGE(IDC_WIN_WEIGHT_COMBO,			OnCbnSelchangeWinWeightCombo)
	ON_CBN_SELCHANGE(IDC_AWB_MODE_COMBO,			OnCbnSelchangeAwbModeCombo)
	ON_CBN_SELCHANGE(IDC_AWB_WIN_COMBO,				OnCbnSelchangeAwbWinCombo)
	ON_CBN_SELCHANGE(IDC_EDGE_KERNEL_COMBO,			OnCbnSelchangeEdgeKernelCombo)
	ON_CBN_SELCHANGE(IDC_FILTER_MODE_COMBO,			OnCbnSelchangeFilterModeCombo)
	ON_CBN_SELCHANGE(IDC_TEST_BAR_COMBO,			OnCbnSelchangeTestBarCombo)

	// Button
	ON_BN_CLICKED(IDC_BUTTON_ISP_APPLY,				OnBnClickedButtonIspApply)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH,				OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_GAMMA_GET_BUTTON,				OnBnClickedGammaGetButton)
	ON_BN_CLICKED(IDC_GAMMA_SET_BUTTON,				OnBnClickedGammaSetButton)
	ON_BN_CLICKED(IDC_GET_BLANK_BUTTON,				OnBnClickedGetBlankButton)
	ON_BN_CLICKED(IDC_SET_BLANK_BUTTON,				OnBnClickedSetBlankButton)
END_MESSAGE_MAP()


// CV568IspDlg 消息处理程序
BOOL CV568IspDlg::OnInitDialog(void)
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispOption();
	
	return TRUE;
}

void CV568IspDlg::InitUserOption()
{
	memset(&(this->m_option), 0, sizeof(TIspOption));
	this->InitCombo();
}

void CV568IspDlg::GetUserOption(char *tail)
{
	this->GetEditboxValue();
	this->GetComboValue(tail);
}

void CV568IspDlg::DispOption()
{
	this->DispCombo();
	this->DispEditBox();
	this->DispCheckBox();
	this->AdjustScrollBar();
}

/******************************************** Check Box Section ********************************************/

void CV568IspDlg::OnBnClickedGpFirstCheck()
{
	if(this->m_gpfirst.GetCheck())
	{
		this->m_option.ispctrl |= BIT4;
		V5B_IspEnableGFirst();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT4;
		V5B_IspDisableGFirst();
	}
}

void CV568IspDlg::OnBnClickedEdgeEnCheck()
{
	if(this->m_edge_en.GetCheck())
	{
		this->m_option.ispctrl |= BIT8;
		V5B_IspEnableEdgeEnh();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT8;
		V5B_IspDisableEdgeEnh();
	}
}

void CV568IspDlg::OnBnClickedLfoEnCheck()
{
	if(this->m_lfo_en.GetCheck())
	{
		this->m_option.ispctrl |= BIT9;
		V5B_IspEnableLensOff();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT9;
		V5B_IspDisableLensOff();
	}
}

void CV568IspDlg::OnBnClickedDpdcEnCheck()
{
	if(this->m_dpdc_en.GetCheck())
	{
		this->m_option.ispctrl |= BIT10;
		V5B_IspEnableDpc();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT10;
		V5B_IspDisableDpc();
	}
}

void CV568IspDlg::OnBnClickedRgbGammaEnCheck()
{
	if(this->m_gamma_en.GetCheck())
	{
		this->m_option.ispctrl |= BIT11;
		V5B_IspEnableRgbGamma();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT11;
		V5B_IspDisableRgbGamma();
	}
}

void CV568IspDlg::OnBnClickedMatrixEnCheck()
{
	if(this->m_matrix_en.GetCheck())
	{
		this->m_option.ispctrl |= BIT12;
		V5B_IspEnableColorMatrix();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT12;
		V5B_IspDisableColorMatrix();
	}
}

void CV568IspDlg::OnBnClickedYGammaEnCheck()
{
	if(this->m_ygamma_en.GetCheck())
	{
		this->m_option.ispctrl |= BIT13;
		V5B_IspEnableYGamma();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT13;
		V5B_IspDisableYGamma();
	}
}

void CV568IspDlg::OnBnClickedRgbGainEnCheck()
{
	if(this->m_rgbgain_en.GetCheck())
	{
		this->m_option.ispctrl |= BIT14;
		V5B_IspEnableRgbGain();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT14;
		V5B_IspDisableRgbGain();
	}
}

void CV568IspDlg::OnBnClickedAfLineCheck()
{
	if(this->m_aflnmode.GetCheck())
	{
		this->m_option.aflnctrl |= BIT15;
		V5B_IspEnableAfLnMode();
	}
	else
	{
		this->m_option.aflnctrl &= ~BIT15;
		V5B_IspDisableAfLnMode();
	}
}

void CV568IspDlg::OnBnClickedGlobalGainEnCheck()
{
	if(this->m_glgain_en.GetCheck())
	{
		this->m_option.glgainctrl |= BIT5;
		V5B_IspEnableGlbGain();
	}
	else
	{
		this->m_option.glgainctrl &= ~BIT5;
		V5B_IspDisableGlbGain();
	}
}

void CV568IspDlg::OnBnClickedAntiFlickerEnCheck()
{
	if(this->m_aflk_en.GetCheck())
	{
		this->m_option.afctrl |= BIT0;
		V5B_IspEnableAntiFlk();
	}
	else
	{
		this->m_option.afctrl &= ~BIT0;
		V5B_IspDisableAntiFlk();
	}
}

void CV568IspDlg::OnBnClickedAwbEnCheck()
{
	if(this->m_awb_en.GetCheck())
	{
		this->m_option.afctrl |= BIT8;
		V5B_IspEnableAwb();
	}
	else
	{
		this->m_option.afctrl &= ~BIT8;
		V5B_IspDisableAwb();
	}
}

void CV568IspDlg::OnBnClickedNoiseYEnCheck()
{
	if(this->m_ynoise_en.GetCheck())
	{
		this->m_option.afctrl |= BIT11;
		V5B_IspEnableNoiseRmv();
	}
	else
	{
		this->m_option.afctrl &= ~BIT11;
		V5B_IspDisableNoiseRmv();
	}
}

void CV568IspDlg::OnBnClickedNoiseUvEnCheck()
{
	if(this->m_uvnoise_en.GetCheck())
	{
		this->m_option.afctrl |= BIT12;
		V5B_IspEnableUvNoiseRmv();
	}
	else
	{
		this->m_option.afctrl &= ~BIT12;
		V5B_IspDisableUvNoiseRmv();
	}
}

void CV568IspDlg::OnBnClickedAutoFocusEnCheck()
{
	if(this->m_afocus_en.GetCheck())
	{
		this->m_option.afctrl |= BIT13;
		V5B_IspEnableAutoFocus();
	}
	else
	{
		this->m_option.afctrl &= ~BIT13;
		V5B_IspDisableAutoFocus();
	}
}

void CV568IspDlg::OnBnClickedTestBarEnCheck()
{
	if(this->m_bar_en.GetCheck())
	{
		this->m_option.ctrl1 |= BIT3;
		V5B_IspEnableTestBar();
	}
	else
	{
		this->m_option.ctrl1 &= ~BIT3;
		V5B_IspDisableTestBar();
	}
}

void CV568IspDlg::OnBnClickedBlackImageEnCheck()
{
	if(this->m_blkimg_en.GetCheck())
	{
		this->m_option.ctrl1 |= BIT0;
		V5B_IspEnableHomoImg();
	}
	else
	{
		this->m_option.ctrl1 &= ~BIT0;
		V5B_IspDisableHomoImg();
	}
}

void CV568IspDlg::OnBnClickedFocusIntEnCheck()
{
	if(this->m_fcsint_en.GetCheck())
		this->m_option.intflag |= BIT0;
	else
	{
		this->m_option.intflag &= ~BIT0;
		V5B_IspClrAFocusInt();
	}
}

void CV568IspDlg::OnBnClickedFlickerIntEnCheck()
{
	if(this->m_flkint_en.GetCheck())
		this->m_option.intflag |= BIT1;
	else
	{
		this->m_option.intflag &= ~BIT1;
		V5B_IspClrAFlickerInt();
	}
}

void CV568IspDlg::OnBnClickedYMeanIntEnCheck()
{
	if(this->m_ymnint_en.GetCheck())
		this->m_option.intflag |= BIT2;
	else
	{
		this->m_option.intflag &= ~BIT2;
		V5B_IspClrYMeanInt();
	}
}

void CV568IspDlg::OnBnClickedHistEffectEnCheck()
{
	if(this->m_hist_eft_en.GetCheck())
	{
		this->m_option.histctrl |= BIT0;
		V5B_IspEnableHistEffect();
	}
	else
	{
		this->m_option.histctrl &= ~BIT0;
		V5B_IspDisableHistEffect();
	}
}

void CV568IspDlg::OnBnClickedHistManualEnCheck()
{
	if(this->m_hist_mnl_en.GetCheck())
	{
		this->m_option.histctrl |= BIT1;
		V5B_IspEnableHistManual();
	}
	else
	{
		this->m_option.histctrl &= ~BIT1;
		V5B_IspDisableHistManual();
	}
}

void CV568IspDlg::OnBnClickedVsyncRstCheck()
{
	if(this->m_vsync_rst.GetCheck())
	{
		this->m_option.vsyncrst = 1;
		V5B_IspEnableVsyncReset();
	}
	else
	{
		this->m_option.vsyncrst = 0;
		V5B_IspDisableVsyncReset();
	}
}

void CV568IspDlg::OnBnClickedHueEnCheck()
{
	if(this->m_hue_en.GetCheck())
	{
		this->m_option.hue_en = 1;
		V5B_IspEnableHue();
	}
	else
	{
		this->m_option.hue_en = 0;
		V5B_IspDisableHue();
	}
}

void CV568IspDlg::RefreshCheckBox(void)
{
	this->m_option.intflag  = V5B_IspGetAfInt();
	this->m_option.histctrl = V5B_IspGetHistCtrl();
	this->m_option.vsyncrst = V5B_IspIsVsyncResetDisable();
	this->m_option.vsyncrst = (~this->m_option.vsyncrst) & 0x01;
	this->m_option.hue_en	= V5B_IspIsHueEnable();
}

void CV568IspDlg::DispCheckBox(void)
{
	if(this->m_option.ispctrl & BIT4)
		this->m_gpfirst.SetCheck(BST_CHECKED);
	else
		this->m_gpfirst.SetCheck(BST_UNCHECKED);

	if(this->m_option.ispctrl & BIT8)
		this->m_edge_en.SetCheck(BST_CHECKED);
	else
		this->m_edge_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ispctrl & BIT9)
		this->m_lfo_en.SetCheck(BST_CHECKED);
	else
		this->m_lfo_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ispctrl & BIT10)
		this->m_dpdc_en.SetCheck(BST_CHECKED);
	else
		this->m_dpdc_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ispctrl & BIT11)
		this->m_gamma_en.SetCheck(BST_CHECKED);
	else
		this->m_gamma_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ispctrl & BIT12)
		this->m_matrix_en.SetCheck(BST_CHECKED);
	else
		this->m_matrix_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ispctrl & BIT13)
		this->m_ygamma_en.SetCheck(BST_CHECKED);
	else
		this->m_ygamma_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ispctrl & BIT14)
		this->m_rgbgain_en.SetCheck(BST_CHECKED);
	else
		this->m_rgbgain_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.aflnctrl & BIT15)
		this->m_aflnmode.SetCheck(BST_CHECKED);
	else
		this->m_aflnmode.SetCheck(BST_UNCHECKED);

	if(this->m_option.glgainctrl & BIT5)
		this->m_glgain_en.SetCheck(BST_CHECKED);
	else
		this->m_glgain_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.afctrl & BIT0)
		this->m_aflk_en.SetCheck(BST_CHECKED);
	else
		this->m_aflk_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.afctrl & BIT8)
		this->m_awb_en.SetCheck(BST_CHECKED);
	else
		this->m_awb_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.afctrl & BIT11)
		this->m_ynoise_en.SetCheck(BST_CHECKED);
	else
		this->m_ynoise_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.afctrl & BIT12)
		this->m_uvnoise_en.SetCheck(BST_CHECKED);
	else
		this->m_uvnoise_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.afctrl & BIT13)
		this->m_afocus_en.SetCheck(BST_CHECKED);
	else
		this->m_afocus_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl1 & BIT3)
		this->m_bar_en.SetCheck(BST_CHECKED);
	else
		this->m_bar_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.ctrl1 & BIT0)
		this->m_blkimg_en.SetCheck(BST_CHECKED);
	else
		this->m_blkimg_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.intflag & BIT0)
		this->m_fcsint_en.SetCheck(BST_CHECKED);
	else
		this->m_fcsint_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.intflag & BIT1)
		this->m_flkint_en.SetCheck(BST_CHECKED);
	else
		this->m_flkint_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.intflag & BIT2)
		this->m_ymnint_en.SetCheck(BST_CHECKED);
	else
		this->m_ymnint_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.histctrl & BIT0)
		this->m_hist_eft_en.SetCheck(BST_CHECKED);
	else
		this->m_hist_eft_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.histctrl & BIT1)
		this->m_hist_mnl_en.SetCheck(BST_CHECKED);
	else
		this->m_hist_mnl_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.vsyncrst)
		this->m_vsync_rst.SetCheck(BST_CHECKED);
	else
		this->m_vsync_rst.SetCheck(BST_UNCHECKED);

	if(this->m_option.hue_en)
		this->m_hue_en.SetCheck(BST_CHECKED);
	else
		this->m_hue_en.SetCheck(BST_UNCHECKED);
}

/********************************************   Combo Section   ********************************************/

const static TPixelRate gPixelRate[] = {{ 0x01, " 1 Clock / Pixel" }, { 0x02, " 2 Clock / Pixel" },
										{ 0x03, " 3 Clock / Pixel" }, { 0x04, " 4 Clock / Pixel" },
										{ 0x05, " 5 Clock / Pixel" }, { 0x06, " 6 Clock / Pixel" },
										{ 0x07, " 7 Clock / Pixel" }, { 0x08, " 8 Clock / Pixel" },
										{ 0x09, " 9 Clock / Pixel" }, { 0x0a, "10 Clock / Pixel" },
										{ 0x0b, "11 Clock / Pixel" }, { 0x0c, "12 Clock / Pixel" },
										{ 0x0d, "13 Clock / Pixel" }, { 0x0e, "14 Clock / Pixel" },
										{ 0x0f, "15 Clock / Pixel" } };

void CV568IspDlg::OnCbnSelchangePixelRateCombo()
{
	int sel = 0;

	sel = this->m_pratebox.GetCurSel();
	this->m_option.ispctrl &= ~0x0f;
	this->m_option.ispctrl |= (gPixelRate[sel].idx & 0x0f);
	V5B_IspSetBasctl((UINT16)this->m_option.ispctrl);
}

void CV568IspDlg::OnCbnSelchangeFirstLineCombo()
{
	int sel = 0;

	sel = this->m_fstlinebox.GetCurSel();
	if(sel & 0x01)
	{
		this->m_option.ispctrl |= BIT5;
		V5B_IspEnableBgFirst();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT5;
		V5B_IspDisableBgFirst();
	}
}

void CV568IspDlg::OnCbnSelchangeSifFmtCombo()
{
	int sel = 0;

	sel = this->m_siffmtbox.GetCurSel();
	if(sel & 0x01)
	{
		this->m_option.ispctrl |= BIT6;
		V5B_IspEnableYuvFmt();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT6;
		V5B_IspDisableYuvFmt();
	}
}

void CV568IspDlg::OnCbnSelchangeFilterCombo()
{
	int sel = 0;

	sel = this->m_filterbox.GetCurSel();
	if(sel & 0x01)
	{
		this->m_option.ispctrl |= BIT7;
		V5B_IspEnableHPF();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT7;
		V5B_IspEnableLPF();
	}
}

void CV568IspDlg::OnCbnSelchangeImageModeCombo()
{
	int sel = 0;

	sel = this->m_imgmodebox.GetCurSel();
	if(sel & 0x01)
	{
		this->m_option.ispctrl |= BIT15;
		V5B_IspEnableGrayImage();
	}
	else
	{
		this->m_option.ispctrl &= ~BIT15;
		V5B_IspDisableGrayImage();
	}
}

void CV568IspDlg::OnCbnSelchangeGlobalGainStepCombo()
{
	int sel = 0;

	sel = this->m_glstepbox.GetCurSel();
	this->m_option.glgainctrl &= 0xfc;
	this->m_option.glgainctrl |= (sel & 0x03);
	V5B_IspSetGlbGainStep((UINT8)(sel & 0x03));
}

void CV568IspDlg::OnCbnSelchangeGlobalGainDlyCombo()
{
	int sel = 0;

	sel = this->m_glstepbox.GetCurSel();
	this->m_option.glgainctrl &= 0xe3;
	this->m_option.glgainctrl |= ((sel & 0x07) << 2);
	V5B_IspSetGlbGainDelay((UINT8)(sel & 0x07));
}

void CV568IspDlg::OnCbnSelchangeGammaSelCombo()
{
	this->m_option.gmasel = this->m_gmaselbox.GetCurSel();
}

void CV568IspDlg::OnCbnSelchangeAfIntFreqCombo()
{
	int sel = 0;

	sel = this->m_intfreqbox.GetCurSel();
	this->m_option.afctrl &= 0xfff1;
	this->m_option.afctrl |= ((sel & 0x07) << 1);
	V5B_IspSetAfIntFrq((UINT8)(sel & 0x07));
}

void CV568IspDlg::OnCbnSelchangeAeWeightCombo()
{
	int sel = 0;

	sel = this->m_aeweightbox.GetCurSel();
	this->m_option.afctrl &= 0xff3f;
	this->m_option.afctrl |= ((sel & 0x03) << 6);
	V5B_IspSetAeWeightMode((UINT8)(sel & 0x03));
}

void CV568IspDlg::OnCbnSelchangeWinWeightCombo()
{
	int sel = 0;

	sel = this->m_winweightbox.GetCurSel();
	if(sel & 0x01)
	{
		this->m_option.afctrl |= BIT4;
		V5B_IspEnableAeSmallYrange();
	}
	else
	{
		this->m_option.afctrl &= ~BIT4;
		V5B_IspEnableAeLargeYrange();
	}
}

void CV568IspDlg::OnCbnSelchangeAwbModeCombo()
{
	int sel = 0;

	sel = this->m_awbmodebox.GetCurSel();
	if(sel & 0x01)
	{
		this->m_option.afctrl |= BIT9;
		V5B_IspEnableAwbRev();
	}
	else
	{
		this->m_option.afctrl &= ~BIT9;
		V5B_IspDisableAwbRev();
	}
}

void CV568IspDlg::OnCbnSelchangeAwbWinCombo()
{
	int sel = 0;

	sel = this->m_awbwinbox.GetCurSel();
	if(sel & 0x01)
	{
		this->m_option.afctrl |= BIT10;
		V5B_IspEnableAwbOneWin();
	}
	else
	{
		this->m_option.afctrl &= ~BIT10;
		V5B_IspDisableAwbOneWin();
	}
}

void CV568IspDlg::OnCbnSelchangeEdgeKernelCombo()
{
	int sel = 0;

	sel = this->m_kernelbox.GetCurSel();
	this->m_option.afctrl &= 0xbfff;
	this->m_option.afctrl |= ((sel & 0x01) << 14);
	V5B_IspSetEdgeMatrix((UINT8)(sel & 0x01));
}

void CV568IspDlg::OnCbnSelchangeFilterModeCombo()
{
	int sel = 0;

	sel = this->m_blfilterbox.GetCurSel();
	this->m_option.ctrl1 &= 0xfd;
	this->m_option.ctrl1 |= ((sel & 0x01) << 1);
	V5B_IspSetFilterMode((UINT8)(sel & 0x01));
}

void CV568IspDlg::OnCbnSelchangeTestBarCombo()
{
	int sel = 0;

	sel = this->m_barmodebox.GetCurSel();
	this->m_option.ctrl1 &= 0xfb;
	this->m_option.ctrl1 |= ((sel & 0x01) << 2);
	V5B_IspSetTestBarPtn((UINT8)(sel & 0x01));
}

void CV568IspDlg::InitCombo(void)
{
	int i = 0;
	const static char *line[]     = { "RG Line First", "BG Line First" };
	const static char *siffmt[]   = { "Bayer Format", "YUV Format" };
	const static char *filter[]   = { "Low Pass Filter",  "High Pass Filter" };
	const static char *imgmode[]  = { "Colorful Image",  "Gray Image" };
	const static char *gainstep[] = { "GLB_GAIN / 0x40",  "GLB_GAIN / 0x20", "GLB_GAIN / 0x10", "GLB_GAIN / 0x08" };
	const static char *gaindly[]  = { "0 Frame Delay",  "1 Frame Delay",  "2 Frame Delay",  "3 Frame Delay", 
								  	  "4 Frame Delay",  "5 Frame Delay",  "6 Frame Delay",  "7 Frame Delay" };
	const static char *gmasel[]   = { "Y Gamma",  "R Gamma", "G Gamma", "B Gamma" };
	const static char *intfreq[]  = { "0 INT / 1 Frame",   "1 INT / 3 Frame",   "2 INT / 5 Frame",	 "3 INT / 9 Frame", 
									  "4 INT / 17 Frame",  "5 INT / 33 Frame",  "6 INT / 65 Frame",  "7 INT / 129 Frame" };
	const static char *aewt[]     = { "Block Weight Only",  "Window Weight Only", "Block + Window", "Weightless Mode" };
	const static char *winwt[]    = { "Large Y Range", "Small Y Range" };
	const static char *awbmode[]  = { "Normal Mode", "Reverse Mode" };
	const static char *awbwin[]   = { "16 Windows", "1 Window Only" };
	const static char *kernel[]   = { "Edge 3x3 Matrix", "Edge 5x5 Matrix" };
	const static char *blfilter[] = { "Normal R B G1 G2", "G1/G2 Offset is R/B" };
	const static char *barmode[]  = { "Full Bars", "Video Window" };

	for(i = 0; i < sizeof(gPixelRate) / sizeof(TPixelRate); i++)
		this->m_pratebox.InsertString(i, gPixelRate[i].str);
	for(i = 0; i < sizeof(line) / sizeof(char *); i++)
		this->m_fstlinebox.InsertString(i, line[i]);
	for(i = 0; i < sizeof(siffmt) / sizeof(char *); i++)
		this->m_siffmtbox.InsertString(i, siffmt[i]);
	for(i = 0; i < sizeof(filter) / sizeof(char *); i++)
		this->m_filterbox.InsertString(i, filter[i]);
	for(i = 0; i < sizeof(imgmode) / sizeof(char *); i++)
		this->m_imgmodebox.InsertString(i, imgmode[i]);
	for(i = 0; i < sizeof(gainstep) / sizeof(char *); i++)
		this->m_glstepbox.InsertString(i, gainstep[i]);
	for(i = 0; i < sizeof(gaindly) / sizeof(char *); i++)
		this->m_gldlybox.InsertString(i, gaindly[i]);
	for(i = 0; i < sizeof(gmasel) / sizeof(char *); i++)
		this->m_gmaselbox.InsertString(i, gmasel[i]);
	for(i = 0; i < sizeof(intfreq) / sizeof(char *); i++)
		this->m_intfreqbox.InsertString(i, intfreq[i]);
	for(i = 0; i < sizeof(aewt) / sizeof(char *); i++)
		this->m_aeweightbox.InsertString(i, aewt[i]);
	for(i = 0; i < sizeof(winwt) / sizeof(char *); i++)
		this->m_winweightbox.InsertString(i, winwt[i]);
	for(i = 0; i < sizeof(awbmode) / sizeof(char *); i++)
		this->m_awbmodebox.InsertString(i, awbmode[i]);
	for(i = 0; i < sizeof(awbwin) / sizeof(char *); i++)
		this->m_awbwinbox.InsertString(i, awbwin[i]);
	for(i = 0; i < sizeof(kernel) / sizeof(char *); i++)
		this->m_kernelbox.InsertString(i, kernel[i]);
	for(i = 0; i < sizeof(blfilter) / sizeof(char *); i++)
		this->m_blfilterbox.InsertString(i, blfilter[i]);
	for(i = 0; i < sizeof(barmode) / sizeof(char *); i++)
		this->m_barmodebox.InsertString(i, barmode[i]);

	this->m_option.ispctrl |= 0x01;
}

void CV568IspDlg::GetComboValue(char *tail)
{
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CV568IspDlg::RefreshCombo(void)
{
}

void CV568IspDlg::DispCombo(void)
{
	int i = 0, cnt = 0;

	cnt = sizeof(gPixelRate) / sizeof(TPixelRate);
	for(i = 0; i < cnt; i++)
	{
		if((this->m_option.ispctrl & 0x0f) == gPixelRate[i].idx)
			break;
	}
	this->m_pratebox.SetCurSel(i);
	this->m_fstlinebox.SetCurSel(((this->m_option.ispctrl & BIT5) >> 5));
	this->m_siffmtbox.SetCurSel(((this->m_option.ispctrl & BIT6) >> 6));
	this->m_filterbox.SetCurSel(((this->m_option.ispctrl & BIT7) >> 7));
	this->m_imgmodebox.SetCurSel(((this->m_option.ispctrl & BIT15) >> 15));
	this->m_glstepbox.SetCurSel((this->m_option.glgainctrl & 0x03));
	this->m_gldlybox.SetCurSel(((this->m_option.glgainctrl & 0x1c) >> 2));
	this->m_gmaselbox.SetCurSel(this->m_option.gmasel);
	this->m_intfreqbox.SetCurSel(((this->m_option.afctrl & 0x0e) >> 1));
	this->m_aeweightbox.SetCurSel(((this->m_option.afctrl & 0xc0) >> 6));
	this->m_winweightbox.SetCurSel(((this->m_option.afctrl & BIT4) >> 4));
	this->m_awbmodebox.SetCurSel(((this->m_option.afctrl & BIT9) >> 9));
	this->m_awbwinbox.SetCurSel(((this->m_option.afctrl & BIT10) >> 10));
	this->m_kernelbox.SetCurSel(((this->m_option.afctrl & 0x4000) >> 14));
	this->m_blfilterbox.SetCurSel(((this->m_option.ctrl1 & BIT1) >> 1));
	this->m_barmodebox.SetCurSel(((this->m_option.ctrl1 & BIT2) >> 2));
}

/******************************************** ScrollBar Section ********************************************/

void CV568IspDlg::GetBarValue(void)
{
}

void CV568IspDlg::RefreshBar(void)
{
}

void CV568IspDlg::DispBarEditBox(void)
{
}

void CV568IspDlg::AdjustScrollBar(void)
{
	this->GetBarValue();
	this->DispBarEditBox();
}

void CV568IspDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	CWaitCursor cursor;
	int pos;

	if(pScrollBar != NULL)
	{
		switch(nSBCode)
		{
		case SB_LINELEFT:
			pos = pScrollBar->GetScrollPos();
			pScrollBar->SetScrollPos(pos - 1);
			break;
		case SB_LINERIGHT:
			pos = pScrollBar->GetScrollPos();
			pScrollBar->SetScrollPos(pos + 1);
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

void CV568IspDlg::GetEditboxValue(void)
{
	int temp = 0;
	char str[20];

	this->m_barw.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.barsz.cx));
	this->m_barh.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.barsz.cy));
	this->m_imgw.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.imgsz.cx));
	this->m_imgh.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.imgsz.cy));

	this->m_femm.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.edge.femm));
	this->m_femp.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.edge.femp));
	this->m_femx1.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.edge.femx1));
	this->m_femx2.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.edge.femx2));
	this->m_femmin.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.edge.femmin));
	this->m_femmax.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.edge.femmax));

	this->m_cenx.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.lfo.lfcenx));
	this->m_ceny.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.lfo.lfceny));
	this->m_f2.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.lfo.lffrec));

	this->m_aflnst.GetWindowText(str, 20);
	sscanf(str, "%x", &temp);
	this->m_option.aflnctrl &= 0xfe00;
	this->m_option.aflnctrl |= (temp & 0x1ff);
	this->m_aflncnt.GetWindowText(str, 20);
	sscanf(str, "%x", &temp);
	this->m_option.aflnctrl &= 0x81ff;
	this->m_option.aflnctrl |= ((temp << 9) & 0x7e00);
	this->m_afstat.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.afstat));
	this->m_aflnstep.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.aflnstep));

	this->m_rgain.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.rgbgain[0]));
	this->m_ggain.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.rgbgain[1]));
	this->m_bgain.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.rgbgain[2]));

	this->m_glgain.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.glgain));

	this->m_cradj.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrcvt.cradj));
	this->m_cbadj.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrcvt.cbadj));
	this->m_croft.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.grayimg.croft));
	this->m_cboft.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.grayimg.cboft));

	this->m_mat11.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.cmat11));
	this->m_mat12.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.cmat12));
	this->m_mat13.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.cmat13));
	this->m_mat21.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.cmat21));
	this->m_mat22.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.cmat22));
	this->m_mat23.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.cmat23));
	this->m_mat31.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.cmat31));
	this->m_mat32.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.cmat32));
	this->m_mat33.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.cmat33));
	this->m_roft.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.ofsr));
	this->m_goft.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.ofsg));
	this->m_boft.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.clrmat.ofsb));

	this->m_gmatbl0.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[0]));
	this->m_gmatbl1.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[1]));
	this->m_gmatbl2.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[2]));
	this->m_gmatbl3.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[3]));
	this->m_gmatbl4.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[4]));
	this->m_gmatbl5.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[5]));
	this->m_gmatbl6.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[6]));
	this->m_gmatbl7.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[7]));
	this->m_gmatbl8.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[8]));
	this->m_gmatbl9.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[9]));
	this->m_gmatbl10.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[10]));
	this->m_gmatbl11.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[11]));
	this->m_gmatbl12.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[12]));
	this->m_gmatbl13.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[13]));
	this->m_gmatbl14.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[14]));
	this->m_gmatbl15.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[15]));
	this->m_gmatbl16.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[16]));
	this->m_gmatbl17.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gmatbl[17]));

	this->m_hstarta.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.hstart[0]));
	this->m_hstartb.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.hstart[1]));
	this->m_hstartc.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.hstart[2]));
	this->m_hstartd.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.hstart[3]));
	this->m_hstarte.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.hstart[4]));
	this->m_vstarta.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.vstart[0]));
	this->m_vstartb.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.vstart[1]));
	this->m_vstartc.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.vstart[2]));
	this->m_vstartd.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.vstart[3]));
	this->m_vstarte.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.vstart[4]));

	this->m_ybot.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.awblimit.ybot));
	this->m_ytop.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.awblimit.ytop));
	this->m_qtop.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.awblimit.qtop));
	this->m_itop.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.awblimit.itop));

	this->m_glow.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.awbgain.glow));
	this->m_gtop.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.awbgain.gtop));

	this->m_awbstep.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.awbstep));
	this->m_awbthd1.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.awbthd1));
	this->m_awbthd2.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.awbthd2));

	this->m_histgain.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.histgain));

	this->m_blroff.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.blroff));
	this->m_blboff.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.blboff));
	this->m_blg1off.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.blg1off));
	this->m_blg2off.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.blg2off));

	this->m_dpdthd.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.dpdthd));
	this->m_dpduvthd.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.dpduvthd));

	this->m_ymeantgt.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.ymeantgt));
	this->m_ymeanthd.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.ymeanthd));
}

void CV568IspDlg::RefreshEditBox(void)
{
	V5B_IspGetTestBarWin(&(this->m_option.barsz));
	V5B_IspGetImageSize(&(this->m_option.imgsz));
	V5B_IspGetEdgeParm(&(this->m_option.edge));
	V5B_IspGetLensParm(&(this->m_option.lfo));
	V5B_IspGetColorCvt(&(this->m_option.clrcvt));
	V5B_IspGetGrayImg(&(this->m_option.grayimg));
	V5B_IspGetColorMatrix(&(this->m_option.clrmat));
	V5B_IspGetAHStart(this->m_option.hstart);
	V5B_IspGetAVStart(this->m_option.vstart);
	V5B_IspGetColorRange(&(this->m_option.awblimit));
	V5B_IspGetAwbGainLmt(&(this->m_option.awbgain));

	this->m_option.afstat     = V5B_IspGetAfSta();
	this->m_option.aflnstep   = V5B_IspGetAfLnStep();

	this->m_option.rgbgain[0] = V5B_IspGetRgain();
	this->m_option.rgbgain[1] = V5B_IspGetGgain();
	this->m_option.rgbgain[2] = V5B_IspGetBgain();

	this->m_option.glgain     = V5B_IspGetGlbGain();
	this->m_option.awbstep    = V5B_IspGetAwbStep();
	this->m_option.awbthd1    = V5B_IspGetAwbThd1();
	this->m_option.awbthd2    = V5B_IspGetAwbThd2();

	this->m_option.mean[0]    = V5B_IspGetRMean();
	this->m_option.mean[1]    = V5B_IspGetGMean();
	this->m_option.mean[2]    = V5B_IspGetBMean();
	this->m_option.mean[3]    = V5B_IspGetYMean();

	this->m_option.afstatus   = V5B_IspGetAfStatus();
	this->m_option.edgesum    = V5B_IspGetEdgeSum();

	this->m_option.histgain   = V5B_IspGetHistGain();

	this->m_option.blroff     = V5B_IspGetBLROffset();
	this->m_option.blboff     = V5B_IspGetBLBOffset();
	this->m_option.blg1off    = V5B_IspGetBLG1Offset();
	this->m_option.blg2off    = V5B_IspGetBLG2Offset();

	this->m_option.dpdthd     = V5B_IspGetDpdThd();
	this->m_option.dpduvthd   = V5B_IspGetDpdThdUV();

	this->m_option.ymeantgt   = V5B_IspGetYMeanTarget();
	this->m_option.ymeanthd   = V5B_IspGetYMeanThd();
}

void CV568IspDlg::DispEditBox(void)
{
	int temp = 0;
	char str[20];

	sprintf(str, "%x", this->m_option.barsz.cx);
	this->m_barw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.barsz.cy);
	this->m_barh.SetWindowText(str);
	sprintf(str, "%x", this->m_option.imgsz.cx);
	this->m_imgw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.imgsz.cy);
	this->m_imgh.SetWindowText(str);

	sprintf(str, "%x", this->m_option.edge.femm);
	this->m_femm.SetWindowText(str);
	sprintf(str, "%x", this->m_option.edge.femp);
	this->m_femp.SetWindowText(str);
	sprintf(str, "%x", this->m_option.edge.femx1);
	this->m_femx1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.edge.femx2);
	this->m_femx2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.edge.femmin);
	this->m_femmin.SetWindowText(str);
	sprintf(str, "%x", this->m_option.edge.femmax);
	this->m_femmax.SetWindowText(str);

	sprintf(str, "%x", this->m_option.lfo.lfcenx);
	this->m_cenx.SetWindowText(str);
	sprintf(str, "%x", this->m_option.lfo.lfceny);
	this->m_ceny.SetWindowText(str);
	sprintf(str, "%x", this->m_option.lfo.lffrec);
	this->m_f2.SetWindowText(str);

	temp = this->m_option.aflnctrl & 0x1ff;
	sprintf(str, "%x", temp);
	this->m_aflnst.SetWindowText(str);
	temp = (this->m_option.aflnctrl >> 9) & 0x3f;
	sprintf(str, "%x", temp);
	this->m_aflncnt.SetWindowText(str);
	sprintf(str, "%x", this->m_option.afstat);
	this->m_afstat.SetWindowText(str);
	sprintf(str, "%x", this->m_option.aflnstep);
	this->m_aflnstep.SetWindowText(str);

	sprintf(str, "%x", this->m_option.rgbgain[0]);
	this->m_rgain.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rgbgain[1]);
	this->m_ggain.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rgbgain[2]);
	this->m_bgain.SetWindowText(str);

	sprintf(str, "%x", this->m_option.glgain);
	this->m_glgain.SetWindowText(str);

	sprintf(str, "%x", this->m_option.clrcvt.cradj);
	this->m_cradj.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrcvt.cbadj);
	this->m_cbadj.SetWindowText(str);
	sprintf(str, "%x", this->m_option.grayimg.croft);
	this->m_croft.SetWindowText(str);
	sprintf(str, "%x", this->m_option.grayimg.cboft);
	this->m_cboft.SetWindowText(str);

	sprintf(str, "%x", this->m_option.clrmat.cmat11);
	this->m_mat11.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.cmat12);
	this->m_mat12.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.cmat13);
	this->m_mat13.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.cmat21);
	this->m_mat21.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.cmat22);
	this->m_mat22.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.cmat23);
	this->m_mat23.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.cmat31);
	this->m_mat31.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.cmat32);
	this->m_mat32.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.cmat33);
	this->m_mat33.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.ofsr);
	this->m_roft.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.ofsg);
	this->m_goft.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clrmat.ofsb);
	this->m_boft.SetWindowText(str);

	sprintf(str, "%x", this->m_option.gmatbl[0]);
	this->m_gmatbl0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[1]);
	this->m_gmatbl1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[2]);
	this->m_gmatbl2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[3]);
	this->m_gmatbl3.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[4]);
	this->m_gmatbl4.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[5]);
	this->m_gmatbl5.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[6]);
	this->m_gmatbl6.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[7]);
	this->m_gmatbl7.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[8]);
	this->m_gmatbl8.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[9]);
	this->m_gmatbl9.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[10]);
	this->m_gmatbl10.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[11]);
	this->m_gmatbl11.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[12]);
	this->m_gmatbl12.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[13]);
	this->m_gmatbl13.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[14]);
	this->m_gmatbl14.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[15]);
	this->m_gmatbl15.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[16]);
	this->m_gmatbl16.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gmatbl[17]);
	this->m_gmatbl17.SetWindowText(str);

	sprintf(str, "%x", this->m_option.hstart[0]);
	this->m_hstarta.SetWindowText(str);
	sprintf(str, "%x", this->m_option.hstart[1]);
	this->m_hstartb.SetWindowText(str);
	sprintf(str, "%x", this->m_option.hstart[2]);
	this->m_hstartc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.hstart[3]);
	this->m_hstartd.SetWindowText(str);
	sprintf(str, "%x", this->m_option.hstart[4]);
	this->m_hstarte.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vstart[0]);
	this->m_vstarta.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vstart[1]);
	this->m_vstartb.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vstart[2]);
	this->m_vstartc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vstart[3]);
	this->m_vstartd.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vstart[4]);
	this->m_vstarte.SetWindowText(str);

	sprintf(str, "%x", this->m_option.awblimit.ybot);
	this->m_ybot.SetWindowText(str);
	sprintf(str, "%x", this->m_option.awblimit.ytop);
	this->m_ytop.SetWindowText(str);
	sprintf(str, "%x", this->m_option.awblimit.qtop);
	this->m_qtop.SetWindowText(str);
	sprintf(str, "%x", this->m_option.awblimit.itop);
	this->m_itop.SetWindowText(str);

	sprintf(str, "%x", this->m_option.awbgain.glow);
	this->m_glow.SetWindowText(str);
	sprintf(str, "%x", this->m_option.awbgain.gtop);
	this->m_gtop.SetWindowText(str);

	sprintf(str, "%x", this->m_option.awbstep);
	this->m_awbstep.SetWindowText(str);
	sprintf(str, "%x", this->m_option.awbthd1);
	this->m_awbthd1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.awbthd2);
	this->m_awbthd2.SetWindowText(str);

	sprintf(str, "%x", this->m_option.mean[0]);
	this->m_rmean.SetWindowText(str);
	sprintf(str, "%x", this->m_option.mean[1]);
	this->m_gmean.SetWindowText(str);
	sprintf(str, "%x", this->m_option.mean[2]);
	this->m_bmean.SetWindowText(str);
	sprintf(str, "%x", this->m_option.mean[3]);
	this->m_ymean.SetWindowText(str);

	sprintf(str, "%x", this->m_option.afstatus);
	this->m_afstatus.SetWindowText(str);
	sprintf(str, "%x", this->m_option.edgesum);
	this->m_edgesum.SetWindowText(str);

	sprintf(str, "%x", this->m_option.histgain);
	this->m_histgain.SetWindowText(str);

	sprintf(str, "%x", this->m_option.blroff);
	this->m_blroff.SetWindowText(str);
	sprintf(str, "%x", this->m_option.blboff);
	this->m_blboff.SetWindowText(str);
	sprintf(str, "%x", this->m_option.blg1off);
	this->m_blg1off.SetWindowText(str);
	sprintf(str, "%x", this->m_option.blg2off);
	this->m_blg2off.SetWindowText(str);

	sprintf(str, "%x", this->m_option.dpdthd);
	this->m_dpdthd.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dpduvthd);
	this->m_dpduvthd.SetWindowText(str);

	sprintf(str, "%x", this->m_option.ymeantgt);
	this->m_ymeantgt.SetWindowText(str);
	sprintf(str, "%x", this->m_option.ymeanthd);
	this->m_ymeanthd.SetWindowText(str);
}

/********************************************   Button Section  ********************************************/

void CV568IspDlg::OnBnClickedButtonIspApply()
{
	this->GetUserOption();

	V5B_IspSetTestBarWin(this->m_option.barsz);
	V5B_IspSetImageSize(this->m_option.imgsz);

	V5B_IspSetEdgeParm(this->m_option.edge);
	V5B_IspSetLensParm(this->m_option.lfo);

	V5B_IspSetAfLnCtl((UINT16)this->m_option.aflnctrl);
	V5B_IspSetAfLnStep((UINT8)this->m_option.aflnstep);

	V5B_IspSetRgain((UINT8)this->m_option.rgbgain[0]);
	V5B_IspSetGgain((UINT8)this->m_option.rgbgain[1]);
	V5B_IspSetBgain((UINT8)this->m_option.rgbgain[2]);

	V5B_IspSetGlbGain((UINT8)this->m_option.glgain);
	V5B_IspSetGlbGainCtrl((UINT8)this->m_option.glgainctrl);

	V5B_IspSetColorCvt(this->m_option.clrcvt);
	V5B_IspSetGrayImg(this->m_option.grayimg);
	V5B_IspSetColorMatrix(this->m_option.clrmat);

	V5B_IspSetAHStart(this->m_option.hstart);
	V5B_IspSetAVStart(this->m_option.vstart);

	V5B_IspSetColorRange(this->m_option.awblimit);
	V5B_IspSetAwbGainLmt(this->m_option.awbgain);

	V5B_IspSetAwbStep((UINT8)this->m_option.awbstep);
	V5B_IspSetAwbThd1((UINT8)this->m_option.awbthd1);
	V5B_IspSetAwbThd2((UINT8)this->m_option.awbthd2);

	V5B_IspSetHistGain((UINT8)this->m_option.histgain);

	V5B_IspSetBLROffset((UINT16)this->m_option.blroff);
	V5B_IspSetBLBOffset((UINT16)this->m_option.blboff);
	V5B_IspSetBLG1Offset((UINT16)this->m_option.blg1off);
	V5B_IspSetBLG2Offset((UINT16)this->m_option.blg2off);

	V5B_IspSetDpdThd((UINT8)this->m_option.dpdthd);
	V5B_IspSetDpdThdUV((UINT8)this->m_option.dpduvthd);

	V5B_IspSetYMeanTarget((UINT8)this->m_option.ymeantgt);
	V5B_IspSetYMeanThd((UINT8)this->m_option.ymeanthd);
}

void CV568IspDlg::OnBnClickedButtonRefresh()
{
	this->m_option.ispctrl    = V5B_IspGetBasctl();
	this->m_option.aflnctrl   = V5B_IspGetAfLnCtl();
	this->m_option.glgainctrl = V5B_IspGetGlbGainCtrl();
	this->m_option.afctrl	  = V5B_IspGetAfCtrl();
	this->m_option.ctrl1	  = V5B_IspGetCtrl1();

	this->RefreshBar();
	this->RefreshCombo();
	this->RefreshEditBox();
	this->RefreshCheckBox();
	this->DispOption();
}

void CV568IspDlg::OnBnClickedGammaGetButton()
{
	switch(this->m_option.gmasel)
	{
	case 0 :
		V5B_IspGetYGammaStart(&(this->m_option.gmatbl[0]));
		V5B_IspGetYGammaSlope(&(this->m_option.gmatbl[9]));
		break;
	case 1 :
		V5B_IspGetRGammaStart(&(this->m_option.gmatbl[0]));
		break;
	case 2 :
		V5B_IspGetGGammaStart(&(this->m_option.gmatbl[0]));
		break;
	case 3 :
		V5B_IspGetBGammaStart(&(this->m_option.gmatbl[0]));
		break;
	}
	this->DispEditBox();
}

void CV568IspDlg::OnBnClickedGammaSetButton()
{
	this->GetEditboxValue();

	switch(this->m_option.gmasel)
	{
	case 0 :
		V5B_IspSetYGammaStart(&(this->m_option.gmatbl[0]));
		V5B_IspSetYGammaSlope(&(this->m_option.gmatbl[9]));
		break;
	case 1 :
		V5B_IspSetRGammaStart(&(this->m_option.gmatbl[0]));
		break;
	case 2 :
		V5B_IspSetGGammaStart(&(this->m_option.gmatbl[0]));
		break;
	case 3 :
		V5B_IspSetBGammaStart(&(this->m_option.gmatbl[0]));
		break;
	}
}

void CV568IspDlg::OnBnClickedGetBlankButton()
{
	char str[20];

	this->m_option.blank = V5B_IspGetTailBlank();
	sprintf(str, "%x", this->m_option.blank);
	this->m_blank.SetWindowText(str);
}

void CV568IspDlg::OnBnClickedSetBlankButton()
{
	char str[20];

	this->m_blank.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.blank));
	V5B_IspSetTailBlank((UINT32)this->m_option.blank);
}

#endif //V578CMD_DISABLE
