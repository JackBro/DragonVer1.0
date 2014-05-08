// V568LcdcDlg.cpp : 实现文件
//
#include "internal.h"
#include "..\include\v568plane\v568lcdcdlg.h"


#ifndef	V578CMD_DISABLE


// CV568LcdcDlg 对话框

IMPLEMENT_DYNAMIC(CV568LcdcDlg, CBaseCtrlDlg)
CV568LcdcDlg::CV568LcdcDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568LcdcDlg::IDD, pParent)
{
}

CV568LcdcDlg::~CV568LcdcDlg()
{
}

void CV568LcdcDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	
	// Check Box
	DDX_Control(pDX, IDC_LAYERAOP_CHECK,	m_aoperation_en);
	DDX_Control(pDX, IDC_LAYERADISP_CHECK,	m_adisp_en);
	DDX_Control(pDX, IDC_B0ENABLE_CHECK,	m_b0_en);
	DDX_Control(pDX, IDC_B0FIRST_CHECK,		m_b0_pri);
	DDX_Control(pDX, IDC_B1ENABLE_CHECK,	m_b1_en);
	DDX_Control(pDX, IDC_B1FIRST_CHECK,		m_b1_pri);
	DDX_Control(pDX, IDC_BLENDEN_CHECK,		m_blend_en);
	DDX_Control(pDX, IDC_OVERLAYEN_CHECK,	m_overlay_en);
	DDX_Control(pDX, IDC_DITHEREN_CHECK,	m_dither_en);
	DDX_Control(pDX, IDC_GAMMAEN_CHECK,		m_gamma_en);
	DDX_Control(pDX, IDC_B0OKCM_CHECK,		m_b0okcm);
	DDX_Control(pDX, IDC_B1OKCM_CHECK,		m_b1okcm);

	// Combo Box
	DDX_Control(pDX, IDC_GAMMASEL_COMBO,		m_gammaselbox);
	DDX_Control(pDX, IDC_OVERLAP_COMBO,			m_overlapbox);
	DDX_Control(pDX, IDC_AROTATION_COMBO,		m_arotbox);
	DDX_Control(pDX, IDC_BROTATION_COMBO,		m_brotbox);
	DDX_Control(pDX, IDC_LBFMT_COMBO,			m_bfmtbox);
	DDX_Control(pDX, IDC_DITHERSEL_COMBO,		m_ditherselbox);
	DDX_Control(pDX, IDC_B0OVERLAYMODE_COMBO,	m_b0olybox);
	DDX_Control(pDX, IDC_B1OVERLAYMODE_COMBO,	m_b1olybox);

	// ScrollBar
	DDX_Control(pDX, IDC_BX0_SCROLLBAR,		m_bx0bar);
	DDX_Control(pDX, IDC_BY0_SCROLLBAR,		m_by0bar);
	DDX_Control(pDX, IDC_BW0_SCROLLBAR,		m_bw0bar);
	DDX_Control(pDX, IDC_BH0_SCROLLBAR,		m_bh0bar);
	DDX_Control(pDX, IDC_BDX0_SCROLLBAR,	m_bdx0bar);
	DDX_Control(pDX, IDC_BDY0_SCROLLBAR,	m_bdy0bar);
	DDX_Control(pDX, IDC_BMW0_SCROLLBAR,	m_bmw0bar);
	DDX_Control(pDX, IDC_BMH0_SCROLLBAR,	m_bmh0bar);

	DDX_Control(pDX, IDC_BRX0_SCROLLBAR,	m_brx0bar);
	DDX_Control(pDX, IDC_BRY0_SCROLLBAR,	m_bry0bar);
	DDX_Control(pDX, IDC_BRW0_SCROLLBAR,	m_brw0bar);
	DDX_Control(pDX, IDC_BRH0_SCROLLBAR,	m_brh0bar);

	DDX_Control(pDX, IDC_BX1_SCROLLBAR,		m_bx1bar);
	DDX_Control(pDX, IDC_BY1_SCROLLBAR,		m_by1bar);
	DDX_Control(pDX, IDC_BW1_SCROLLBAR,		m_bw1bar);
	DDX_Control(pDX, IDC_BH1_SCROLLBAR,		m_bh1bar);
	DDX_Control(pDX, IDC_BDX1_SCROLLBAR,	m_bdx1bar);
	DDX_Control(pDX, IDC_BDY1_SCROLLBAR,	m_bdy1bar);
	DDX_Control(pDX, IDC_BMW1_SCROLLBAR,	m_bmw1bar);
	DDX_Control(pDX, IDC_BMH1_SCROLLBAR,	m_bmh1bar);

	DDX_Control(pDX, IDC_BRX1_SCROLLBAR,	m_brx1bar);
	DDX_Control(pDX, IDC_BRY1_SCROLLBAR,	m_bry1bar);
	DDX_Control(pDX, IDC_BRW1_SCROLLBAR,	m_brw1bar);
	DDX_Control(pDX, IDC_BRH1_SCROLLBAR,	m_brh1bar);

	DDX_Control(pDX, IDC_AX_SCROLLBAR,		m_axbar);
	DDX_Control(pDX, IDC_AY_SCROLLBAR,		m_aybar);
	DDX_Control(pDX, IDC_AW_SCROLLBAR,		m_awbar);
	DDX_Control(pDX, IDC_AH_SCROLLBAR,		m_ahbar);
	DDX_Control(pDX, IDC_ADX_SCROLLBAR,		m_adxbar);
	DDX_Control(pDX, IDC_ADY_SCROLLBAR,		m_adybar);
	DDX_Control(pDX, IDC_AMW_SCROLLBAR,		m_amwbar);
	DDX_Control(pDX, IDC_AMH_SCROLLBAR,		m_amhbar);

	DDX_Control(pDX, IDC_BLEND0_SCROLLBAR,	m_blend0bar);
	DDX_Control(pDX, IDC_BLEND1_SCROLLBAR,	m_blend1bar);

	DDX_Control(pDX, IDC_BGX_SCROLLBAR,		m_bgxbar);
	DDX_Control(pDX, IDC_BGY_SCROLLBAR,		m_bgybar);
	DDX_Control(pDX, IDC_BGW_SCROLLBAR,		m_bgwbar);
	DDX_Control(pDX, IDC_BGH_SCROLLBAR,		m_bghbar);

	DDX_Control(pDX, IDC_DEDLY_SCROLLBAR,	m_dedlybar);
	DDX_Control(pDX, IDC_DEINTV_SCROLLBAR,	m_deintvbar);
	DDX_Control(pDX, IDC_LINEINTV_SCROLLBAR,	m_lineintvbar);

	DDX_Control(pDX, IDC_BRIGHTNESS_SCROLLBAR,	m_brightnessbar);
	DDX_Control(pDX, IDC_CONTRAST_SCROLLBAR,	m_contrastbar);

	DDX_Control(pDX, IDC_LCDCWSC_SCROLLBAR,	m_wscbar);
	DDX_Control(pDX, IDC_LCDCWRC_SCROLLBAR, m_wrcbar);
	DDX_Control(pDX, IDC_LCDCDHC_SCROLLBAR, m_dhcbar);

	// ScrollBar EditBox
	DDX_Control(pDX, IDC_BX0_EDIT,		m_bx0);
	DDX_Control(pDX, IDC_BY0_EDIT,		m_by0);
	DDX_Control(pDX, IDC_BW0_EDIT,		m_bw0);
	DDX_Control(pDX, IDC_BH0_EDIT,		m_bh0);
	DDX_Control(pDX, IDC_BDX0_EDIT,		m_bdx0);
	DDX_Control(pDX, IDC_BDY0_EDIT,		m_bdy0);
	DDX_Control(pDX, IDC_BMW0_EDIT,		m_bmw0);
	DDX_Control(pDX, IDC_BMH0_EDIT,		m_bmh0);

	DDX_Control(pDX, IDC_BRX0_EDIT,		m_brx0);
	DDX_Control(pDX, IDC_BRY0_EDIT,		m_bry0);
	DDX_Control(pDX, IDC_BRW0_EDIT,		m_brw0);
	DDX_Control(pDX, IDC_BRH0_EDIT,		m_brh0);

	DDX_Control(pDX, IDC_BX1_EDIT,		m_bx1);
	DDX_Control(pDX, IDC_BY1_EDIT,		m_by1);
	DDX_Control(pDX, IDC_BW1_EDIT,		m_bw1);
	DDX_Control(pDX, IDC_BH1_EDIT,		m_bh1);
	DDX_Control(pDX, IDC_BDX1_EDIT,		m_bdx1);
	DDX_Control(pDX, IDC_BDY1_EDIT,		m_bdy1);
	DDX_Control(pDX, IDC_BMW1_EDIT,		m_bmw1);
	DDX_Control(pDX, IDC_BMH1_EDIT,		m_bmh1);

	DDX_Control(pDX, IDC_BRX1_EDIT,		m_brx1);
	DDX_Control(pDX, IDC_BRY1_EDIT,		m_bry1);
	DDX_Control(pDX, IDC_BRW1_EDIT,		m_brw1);
	DDX_Control(pDX, IDC_BRH1_EDIT,		m_brh1);

	DDX_Control(pDX, IDC_AX_EDIT,		m_ax);
	DDX_Control(pDX, IDC_AY_EDIT,		m_ay);
	DDX_Control(pDX, IDC_AW_EDIT,		m_aw);
	DDX_Control(pDX, IDC_AH_EDIT,		m_ah);
	DDX_Control(pDX, IDC_ADX_EDIT,		m_adx);
	DDX_Control(pDX, IDC_ADY_EDIT,		m_ady);
	DDX_Control(pDX, IDC_AMW_EDIT,		m_amw);
	DDX_Control(pDX, IDC_AMH_EDIT,		m_amh);

	DDX_Control(pDX, IDC_BLEND0_EDIT,	m_blend0);
	DDX_Control(pDX, IDC_BLEND1_EDIT,	m_blend1);

	DDX_Control(pDX, IDC_BGX_EDIT,		m_bgx);
	DDX_Control(pDX, IDC_BGY_EDIT,		m_bgy);
	DDX_Control(pDX, IDC_BGW_EDIT,		m_bgw);
	DDX_Control(pDX, IDC_BGH_EDIT,		m_bgh);

	DDX_Control(pDX, IDC_DEDLY_EDIT,	m_dedly);
	DDX_Control(pDX, IDC_DEINTV_EDIT,	m_deintv);
	DDX_Control(pDX, IDC_LINEINTV_EDIT, m_lineintv);

	DDX_Control(pDX, IDC_BRIGHTNESS_EDIT,	m_brightness);
	DDX_Control(pDX, IDC_CONTRAST_EDIT,		m_contrast);

	DDX_Control(pDX, IDC_LCDCWSC_EDIT,	m_wsc);
	DDX_Control(pDX, IDC_LCDCWRC_EDIT,	m_wrc);
	DDX_Control(pDX, IDC_LCDCDHC_EDIT,	m_dhc);

	// EditBox
	DDX_Control(pDX, IDC_KCOLOR0_EDIT, m_kcolor0);
	DDX_Control(pDX, IDC_KCOLOR1_EDIT, m_kcolor1);
	DDX_Control(pDX, IDC_BGCOLOR_EDIT, m_bgcolor);

	DDX_Control(pDX, IDC_VALUE00_EDIT, m_gammaval00);
	DDX_Control(pDX, IDC_VALUE01_EDIT, m_gammaval01);
	DDX_Control(pDX, IDC_VALUE02_EDIT, m_gammaval02);
	DDX_Control(pDX, IDC_VALUE03_EDIT, m_gammaval03);
	DDX_Control(pDX, IDC_VALUE04_EDIT, m_gammaval04);
	DDX_Control(pDX, IDC_VALUE05_EDIT, m_gammaval05);
	DDX_Control(pDX, IDC_VALUE06_EDIT, m_gammaval06);
	DDX_Control(pDX, IDC_VALUE07_EDIT, m_gammaval07);
	DDX_Control(pDX, IDC_VALUE08_EDIT, m_gammaval08);
	DDX_Control(pDX, IDC_VALUE09_EDIT, m_gammaval09);
	DDX_Control(pDX, IDC_VALUE10_EDIT, m_gammaval10);
	DDX_Control(pDX, IDC_VALUE11_EDIT, m_gammaval11);
	DDX_Control(pDX, IDC_VALUE12_EDIT, m_gammaval12);
	DDX_Control(pDX, IDC_VALUE13_EDIT, m_gammaval13);
	DDX_Control(pDX, IDC_VALUE14_EDIT, m_gammaval14);
	DDX_Control(pDX, IDC_VALUE15_EDIT, m_gammaval15);
	DDX_Control(pDX, IDC_VALUE16_EDIT, m_gammaval16);
}


BEGIN_MESSAGE_MAP(CV568LcdcDlg, CBaseCtrlDlg)
	ON_WM_HSCROLL()

	// Check Box
	ON_BN_CLICKED(IDC_LAYERAOP_CHECK,	OnBnClickedLayeraopCheck)
	ON_BN_CLICKED(IDC_LAYERADISP_CHECK, OnBnClickedLayeradispCheck)
	ON_BN_CLICKED(IDC_B0ENABLE_CHECK,	OnBnClickedB0enableCheck)
	ON_BN_CLICKED(IDC_B0FIRST_CHECK,	OnBnClickedB0firstCheck)
	ON_BN_CLICKED(IDC_B1ENABLE_CHECK,	OnBnClickedB1enableCheck)
	ON_BN_CLICKED(IDC_B1FIRST_CHECK,	OnBnClickedB1firstCheck)
	ON_BN_CLICKED(IDC_BLENDEN_CHECK,	OnBnClickedBlendenCheck)
	ON_BN_CLICKED(IDC_OVERLAYEN_CHECK,	OnBnClickedOverlayenCheck)
	ON_BN_CLICKED(IDC_DITHEREN_CHECK,	OnBnClickedDitherenCheck)
	ON_BN_CLICKED(IDC_GAMMAEN_CHECK,	OnBnClickedGammaenCheck)
	ON_BN_CLICKED(IDC_B0OKCM_CHECK,		OnBnClickedB0okcmCheck)
	ON_BN_CLICKED(IDC_B1OKCM_CHECK,		OnBnClickedB1okcmCheck)

	// Combo Box
	ON_CBN_SELCHANGE(IDC_GAMMASEL_COMBO,		OnCbnSelchangeGammaselCombo)
	ON_CBN_SELCHANGE(IDC_OVERLAP_COMBO,			OnCbnSelchangeOverlapCombo)
	ON_CBN_SELCHANGE(IDC_AROTATION_COMBO,		OnCbnSelchangeArotationCombo)
	ON_CBN_SELCHANGE(IDC_BROTATION_COMBO,		OnCbnSelchangeBrotationCombo)
	ON_CBN_SELCHANGE(IDC_LBFMT_COMBO,			OnCbnSelchangeLbfmtCombo)
	ON_CBN_SELCHANGE(IDC_DITHERSEL_COMBO,		OnCbnSelchangeDitherselCombo)
	ON_CBN_SELCHANGE(IDC_B0OVERLAYMODE_COMBO,	OnCbnSelchangeB0overlaymodeCombo)
	ON_CBN_SELCHANGE(IDC_B1OVERLAYMODE_COMBO,	OnCbnSelchangeB1overlaymodeCombo)

	// Button
	ON_BN_CLICKED(IDC_REFRESH_BUTTON,		OnBnClickedRefreshButton)
	ON_BN_CLICKED(IDC_SET_BUTTON,			OnBnClickedSetButton)
	ON_BN_CLICKED(IDC_LCDCRST_BUTTON,		OnBnClickedLcdcrstButton)
	ON_BN_CLICKED(IDC_TEST_BUTTON,			OnBnClickedTestButton)
	ON_BN_CLICKED(IDC_SET_A_BUTTON,			OnBnClickedSetAButton)
	ON_BN_CLICKED(IDC_SET_B0_BUTTON,		OnBnClickedSetB0Button)
	ON_BN_CLICKED(IDC_UPDATE_B0_BUTTON,		OnBnClickedUpdateB0Button)
	ON_BN_CLICKED(IDC_SET_B1_BUTTON,		OnBnClickedSetB1Button)
	ON_BN_CLICKED(IDC_UPDATE_B1_BUTTON,		OnBnClickedUpdateB1Button)
	ON_BN_CLICKED(IDC_SET_BG_BUTTON,		OnBnClickedSetBgButton)
	ON_BN_CLICKED(IDC_UPDATE_BG_BUTTON,		OnBnClickedUpdateBgButton)
	ON_BN_CLICKED(IDC_KCOLOR0_BUTTON,		OnBnClickedKcolor0Button)
	ON_BN_CLICKED(IDC_KCOLOR1_BUTTON,		OnBnClickedKcolor1Button)
	ON_BN_CLICKED(IDC_BG_COLOR_BUTTON,		OnBnClickedBgColorButton)
END_MESSAGE_MAP()


// CV568LcdcDlg 消息处理程序


BOOL CV568LcdcDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CV568LcdcDlg::InitUserOption(void)
{
	memset(&(this->m_option), 0, sizeof(TLcdcOption));
	this->m_option.laybfmt[0] = BLAYER_RGB565;
	this->m_option.laybfmt[1] = BLAYER_RGB565;
	this->InitCombo();
}

void CV568LcdcDlg::GetUserOption(char *tail)
{
	this->GetEditboxValue();
	this->GetComboValue(tail);
}

void CV568LcdcDlg::DispOption(void)
{
	this->DispCombo();
	this->DispEditBox();
	this->DispCheckBox();
	this->AdjustScrollBar();
}

/******************************************** Check Box Section ********************************************/

void CV568LcdcDlg::OnBnClickedLayeraopCheck()
{
	if(this->m_aoperation_en.GetCheck())
	{
		this->m_option.config |= CFG_LA_OP_EN;
		if(this->m_option.config & CFG_LA_DISP_EN)
			V5B_LcdcEnableAlayer();
		else
			V5B_LcdcEnableAOperation();
	}
	else
	{
		this->m_option.config &= ~CFG_LA_OP_EN;
		this->m_option.config &= ~CFG_LA_DISP_EN;
		this->m_adisp_en.SetCheck(BST_UNCHECKED);
		V5B_LcdcDisableAlayer();
	}
}

void CV568LcdcDlg::OnBnClickedLayeradispCheck()
{
	if(this->m_adisp_en.GetCheck())
	{
		this->m_option.config |= CFG_LA_OP_EN;
		this->m_option.config |= CFG_LA_DISP_EN;
		this->m_aoperation_en.SetCheck(BST_CHECKED);
		V5B_LcdcEnableAlayer();
	}
	else
	{
		this->m_option.config &= ~CFG_LA_DISP_EN;
		if(this->m_option.config & CFG_LA_OP_EN)
			V5B_LcdcEnableAOperation();
		else
			V5B_LcdcDisableAlayer();
	}
}

void CV568LcdcDlg::OnBnClickedB0enableCheck()
{
	if(this->m_b0_en.GetCheck())
	{
		this->m_option.config |= CFG_LB0_EN;
		V5B_LcdcEnableB0();
	}
	else
	{
		this->m_option.config &= ~CFG_LB0_EN;
		V5B_LcdcDisableB0();
	}
}

void CV568LcdcDlg::OnBnClickedB0firstCheck()
{
	if(this->m_b0_pri.GetCheck())
	{
		this->m_option.config |= CFG_LB0_PRI;
		V5B_LcdcLayerB0First();
	}
	else
	{
		this->m_option.config &= ~CFG_LB0_PRI;
		V5B_LcdcLayerB0FstOff();
	}
}

void CV568LcdcDlg::OnBnClickedB1enableCheck()
{
	if(this->m_b1_en.GetCheck())
	{
		this->m_option.config |= CFG_LB1_EN;
		V5B_LcdcEnableB1();
	}
	else
	{
		this->m_option.config &= ~CFG_LB1_EN;
		V5B_LcdcDisableB1();
	}
}

void CV568LcdcDlg::OnBnClickedB1firstCheck()
{
	if(this->m_b1_pri.GetCheck())
	{
		this->m_option.config |= CFG_LB1_PRI;
		V5B_LcdcLayerB1First();
	}
	else
	{
		this->m_option.config &= ~CFG_LB1_PRI;
		V5B_LcdcLayerB1FstOff();
	}
}

void CV568LcdcDlg::OnBnClickedBlendenCheck()
{
	if(this->m_blend_en.GetCheck())
	{
		this->m_option.config |= CFG_BLEND_EN;
		V5B_LcdcEnableBlending();
	}
	else
	{
		this->m_option.config &= ~CFG_BLEND_EN;
		V5B_LcdcDisableBlending();
	}
}

void CV568LcdcDlg::OnBnClickedOverlayenCheck()
{
	if(this->m_overlay_en.GetCheck())
	{
		this->m_option.config |= CFG_OVERLAY_EN;
		V5B_LcdcEnableOverlay();
	}
	else
	{
		this->m_option.config &= ~CFG_OVERLAY_EN;
		V5B_LcdcDisableOverlay();
	}
}

void CV568LcdcDlg::OnBnClickedDitherenCheck()
{
	if(this->m_dither_en.GetCheck())
	{
		this->m_option.config |= CFG_DITHER_EN;
		V5B_LcdcEnableDither();
	}
	else
	{
		this->m_option.config &= ~CFG_DITHER_EN;
		V5B_LcdcDisableDither();
	}
}

void CV568LcdcDlg::OnBnClickedGammaenCheck()
{
	if(this->m_gamma_en.GetCheck())
	{
		this->m_option.config |= CFG_GAMMA_EN;
		V5B_LcdcEnableGamma();
	}
	else
	{
		this->m_option.config &= ~CFG_GAMMA_EN;
		V5B_LcdcDisableGamma();
	}
}

void CV568LcdcDlg::OnBnClickedB0okcmCheck()
{
	if(this->m_b0okcm.GetCheck())
	{
		this->m_option.config |= CFG_LB0_OKCM;
		V5B_LcdcSetB0OkcMode(0x01);
	}
	else
	{
		this->m_option.config &= ~CFG_LB0_OKCM;
		V5B_LcdcSetB0OkcMode(0);
	}
}

void CV568LcdcDlg::OnBnClickedB1okcmCheck()
{
	if(this->m_b1okcm.GetCheck())
	{
		this->m_option.config |= CFG_LB1_OKCM;
		V5B_LcdcSetB1OkcMode(0x01);
	}
	else
	{
		this->m_option.config &= ~CFG_LB1_OKCM;
		V5B_LcdcSetB1OkcMode(0);
	}
}

void CV568LcdcDlg::RefreshCheckBox(void)
{
	UINT16 tmp;

	tmp = V5B_LcdcGetDe();
	if(tmp & AOPERATION_EN)
		this->m_option.config |= CFG_LA_OP_EN;
	else
		this->m_option.config &= ~CFG_LA_OP_EN;

	if(tmp & ALAYER_EN)
		this->m_option.config |= CFG_LA_DISP_EN;
	else
		this->m_option.config &= ~CFG_LA_DISP_EN;

	if(tmp & B0LAYER_EN)
		this->m_option.config |= CFG_LB0_EN;
	else
		this->m_option.config &= ~CFG_LB0_EN;

	if(tmp & BACKGROUND_EN)
		this->m_option.config |= CFG_BG_EN;
	else
		this->m_option.config &= ~CFG_BG_EN;

	if(tmp & B1LAYER_EN)
		this->m_option.config |= CFG_LB1_EN;
	else
		this->m_option.config &= ~CFG_LB1_EN;

	tmp = V5B_LcdcGetDc();
	if(tmp & DITHER_EN)
		this->m_option.config |= CFG_DITHER_EN;
	else
		this->m_option.config &= ~CFG_DITHER_EN;

	if(tmp & GAMMA_EN)
		this->m_option.config |= CFG_GAMMA_EN;
	else
		this->m_option.config &= ~CFG_GAMMA_EN;

	if(tmp & OVERMODE_EN)
		this->m_option.config |= CFG_OVERLAY_EN;
	else
		this->m_option.config &= ~CFG_OVERLAY_EN;

	if(tmp & BLEND_EN)
		this->m_option.config |= CFG_BLEND_EN;
	else
		this->m_option.config &= ~CFG_BLEND_EN;

	if(tmp & B0_PRIORITY)
		this->m_option.config |= CFG_LB0_PRI;
	else
		this->m_option.config &= ~CFG_LB0_PRI;

	tmp = V5B_LcdcGetUpdate();
	if(tmp & 0x40)
		this->m_option.config |= CFG_LB1_PRI;
	else
		this->m_option.config &= ~CFG_LB1_PRI;

	tmp = V5B_LcdcGetB0OkcMode();
	if(tmp & 0x01)
		this->m_option.config |= CFG_LB0_OKCM;
	else
		this->m_option.config &= ~CFG_LB0_OKCM;

	tmp = V5B_LcdcGetB1OkcMode();
	if(tmp & 0x01)
		this->m_option.config |= CFG_LB1_OKCM;
	else
		this->m_option.config &= ~CFG_LB1_OKCM;
}

void CV568LcdcDlg::DispCheckBox(void)
{
	if(this->m_option.config & CFG_LA_OP_EN)
		this->m_aoperation_en.SetCheck(BST_CHECKED);
	else
		this->m_aoperation_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_LA_DISP_EN)
		this->m_adisp_en.SetCheck(BST_CHECKED);
	else
		this->m_adisp_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_LB0_EN)
		this->m_b0_en.SetCheck(BST_CHECKED);
	else
		this->m_b0_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_LB0_PRI)
		this->m_b0_pri.SetCheck(BST_CHECKED);
	else
		this->m_b0_pri.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_LB1_EN)
		this->m_b1_en.SetCheck(BST_CHECKED);
	else
		this->m_b1_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_LB1_PRI)
		this->m_b1_pri.SetCheck(BST_CHECKED);
	else
		this->m_b1_pri.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_BLEND_EN)
		this->m_blend_en.SetCheck(BST_CHECKED);
	else
		this->m_blend_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_OVERLAY_EN)
		this->m_overlay_en.SetCheck(BST_CHECKED);
	else
		this->m_overlay_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_DITHER_EN)
		this->m_dither_en.SetCheck(BST_CHECKED);
	else
		this->m_dither_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_GAMMA_EN)
		this->m_gamma_en.SetCheck(BST_CHECKED);
	else
		this->m_gamma_en.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_LB0_OKCM)
		this->m_b0okcm.SetCheck(BST_CHECKED);
	else
		this->m_b0okcm.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & CFG_LB1_OKCM)
		this->m_b1okcm.SetCheck(BST_CHECKED);
	else
		this->m_b1okcm.SetCheck(BST_UNCHECKED);
}

/******************************************** Combo Section ********************************************/

void CV568LcdcDlg::InitCombo(void)
{
	int i = 0;
	const char *gammatable[] = { "R Gamma", "G Gamma", "B Gamma" };

	for(i = 0; i < sizeof(gammatable) / sizeof(char *); i++)
		this->m_gammaselbox.InsertString(i, gammatable[i]);
	for(i = 0; i < gTotalOvlpMode; i++)
		this->m_overlapbox.InsertString(i, gOvlpMode[i]);
	for(i = 0; i < gTotalRotateMode; i++)
		this->m_arotbox.InsertString(i, gRotateMode[i]);
	for(i = 0; i < gTotalRotateMode; i++)
		this->m_brotbox.InsertString(i, gRotateMode[i]);
	for(i = 0; i < gV568Bfmtcount; i++)
		this->m_bfmtbox.InsertString(i, gV568BlayerFmtDesc[i]);
	for(i = 0; i < gV568DitherCount; i++)
		this->m_ditherselbox.InsertString(i, gV568DitherSelDesc[i]);
	for(i = 0; i < gTotalOverMode; i++)
		this->m_b0olybox.InsertString(i, gOverMode[i]);
	for(i = 0; i < gTotalOverMode; i++)
		this->m_b1olybox.InsertString(i, gOverMode[i]);

	this->m_gammaselbox.SetCurSel(R_TABLE);
}

void CV568LcdcDlg::GetComboValue(char *tail)
{
	this->m_option.gamma_sel = this->m_gammaselbox.GetCurSel();
	this->m_option.overlap	 = this->m_overlapbox.GetCurSel();
	this->m_option.arotmode	 = this->m_arotbox.GetCurSel();
	this->m_option.brotmode	 = this->m_brotbox.GetCurSel();
	this->m_option.bfmt		 = this->m_bfmtbox.GetCurSel();
	this->m_option.dithersel = this->m_ditherselbox.GetCurSel();
	this->m_option.b0olymode = this->m_b0olybox.GetCurSel();
	this->m_option.b1olymode = this->m_b1olybox.GetCurSel();

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CV568LcdcDlg::RefreshCombo(void)
{
	this->m_option.overlap	 = V5B_LcdcGetOLP();
	this->m_option.arotmode	 = V5B_LcdcGetAlayerRM();
	this->m_option.brotmode	 = V5B_LcdcGetBlayerRM();
	this->m_option.bfmt		 = V5B_LcdcGetGbufFormat();
	this->m_option.dithersel = V5B_LcdcGetDitherSel();
	this->m_option.b0olymode = V5B_LcdcGetB0OverlayMode();
	this->m_option.b1olymode = V5B_LcdcGetB1OverlayMode();
}

void CV568LcdcDlg::DispCombo(void)
{
	this->m_gammaselbox.SetCurSel(this->m_option.gamma_sel);
	this->m_overlapbox.SetCurSel(this->m_option.overlap);
	this->m_arotbox.SetCurSel(this->m_option.arotmode);
	this->m_brotbox.SetCurSel(this->m_option.brotmode);
	this->m_bfmtbox.SetCurSel(this->m_option.bfmt);
	this->m_ditherselbox.SetCurSel(this->m_option.dithersel);
	this->m_b0olybox.SetCurSel(this->m_option.b0olymode);
	this->m_b1olybox.SetCurSel(this->m_option.b1olymode);
}

void CV568LcdcDlg::OnCbnSelchangeGammaselCombo()
{
	this->m_option.gamma_sel = this->m_gammaselbox.GetCurSel();
}

void CV568LcdcDlg::OnCbnSelchangeOverlapCombo()
{
	this->m_option.overlap = this->m_overlapbox.GetCurSel();
	V5B_LcdcSetOLP((UINT8)this->m_option.overlap);
}

void CV568LcdcDlg::OnCbnSelchangeArotationCombo()
{
	int oldmode = 0;
	UINT8 x = 0;
	UINT16 flag = 0xffff;
	TSize ippdisp, size = {0, 0};

	oldmode = this->m_option.arotmode;
	this->m_option.arotmode	= this->m_arotbox.GetCurSel();

	V5B_LcdcDisableAlayer();
	x = 0xff;
	while(x && flag)
	{
		x = V5B_LcdcGetDe();
		x &= 0x12;
		flag --;
	}
	if((this->m_option.arotmode % 2) != (oldmode % 2))
	{
		V5B_IppGetDispSize(&ippdisp);
		size.cx = ippdisp.cy;
		size.cy = ippdisp.cx;
//		V5_IppSetDispSizer(size);
	}
	//V5B_LcdcSetAMSize(size);
	//V5_MarbAutoSetMem();

	V5B_LcdcSetAWinSize(size);
	V5B_LcdcSetAlayerRM((UINT8)this->m_option.arotmode);
	V5B_LcdcEnableAlayer();

	this->DispOption();
}

void CV568LcdcDlg::OnCbnSelchangeBrotationCombo()
{
	this->m_option.brotmode	= this->m_brotbox.GetCurSel();
	V5B_LcdcSetBlayerRM((UINT8)this->m_option.brotmode);
}

void CV568LcdcDlg::OnCbnSelchangeLbfmtCombo()
{
	this->m_option.bfmt	= this->m_bfmtbox.GetCurSel();
	V5B_LcdcSetGbufFormat((UINT8)this->m_option.bfmt);
}

void CV568LcdcDlg::OnCbnSelchangeDitherselCombo()
{
	this->m_option.dithersel = this->m_ditherselbox.GetCurSel();
	V5B_LcdcSetDitherSel((UINT8)this->m_option.dithersel);
}

void CV568LcdcDlg::OnCbnSelchangeB0overlaymodeCombo()
{
	this->m_option.b0olymode = this->m_b0olybox.GetCurSel();
	V5B_LcdcSetB0OverlayMode((UINT8)this->m_option.b0olymode);
}

void CV568LcdcDlg::OnCbnSelchangeB1overlaymodeCombo()
{
	this->m_option.b1olymode = this->m_b1olybox.GetCurSel();
	V5B_LcdcSetB1OverlayMode((UINT8)this->m_option.b1olymode);
}

/******************************************** ScrollBar Section ********************************************/

void CV568LcdcDlg::RefreshBar(void)
{
	this->m_option.blend[0]	  = V5B_LcdcGetB0BlendRatio();
	this->m_option.blend[0]	  = V5B_LcdcGetB1BlendRatio();

	this->m_option.dedly	  = V5B_LcdcGetDEdelay();
	this->m_option.deintv	  = V5B_LcdcGetDEinteval();
	this->m_option.lineintv	  = V5B_LcdcGetLineinteval();

	this->m_option.brightness = V5_DispGetBrightness();
	this->m_option.contrast   = V5_DispGetContrast();

	this->m_option.wsc = 0x001f & V5B_LcdifGetWrTiming();
	this->m_option.wrc = (0x03e0 & V5B_LcdifGetWrTiming()) >> 5;
	this->m_option.dhc = (0x7c00 & V5B_LcdifGetWrTiming()) >> 10;
}

void CV568LcdcDlg::GetBarValue(void)
{
	this->m_option.layerb[0].pos2panel.x = (UINT16)this->m_bx0bar.GetScrollPos();
	this->m_option.layerb[0].pos2panel.y = (UINT16)this->m_by0bar.GetScrollPos();
	this->m_option.layerb[0].win.size.cx = (UINT16)this->m_bw0bar.GetScrollPos();
	this->m_option.layerb[0].win.size.cy = (UINT16)this->m_bh0bar.GetScrollPos();
	this->m_option.layerb[0].win.ltp.x   = (UINT16)this->m_bdx0bar.GetScrollPos();
	this->m_option.layerb[0].win.ltp.y   = (UINT16)this->m_bdy0bar.GetScrollPos();
	this->m_option.layerb[0].memsize.cx  = (UINT16)this->m_bmw0bar.GetScrollPos();
	this->m_option.layerb[0].memsize.cy  = (UINT16)this->m_bmh0bar.GetScrollPos();

	this->m_option.layerb[0].refwin.ltp.x   = (UINT16)this->m_brx0bar.GetScrollPos();
	this->m_option.layerb[0].refwin.ltp.y   = (UINT16)this->m_bry0bar.GetScrollPos();
	this->m_option.layerb[0].refwin.size.cx = (UINT16)this->m_brw0bar.GetScrollPos();
	this->m_option.layerb[0].refwin.size.cy = (UINT16)this->m_brh0bar.GetScrollPos();

	this->m_option.layerb[1].pos2panel.x = (UINT16)this->m_bx1bar.GetScrollPos();
	this->m_option.layerb[1].pos2panel.y = (UINT16)this->m_by1bar.GetScrollPos();
	this->m_option.layerb[1].win.size.cx = (UINT16)this->m_bw1bar.GetScrollPos();
	this->m_option.layerb[1].win.size.cy = (UINT16)this->m_bh1bar.GetScrollPos();
	this->m_option.layerb[1].win.ltp.x   = (UINT16)this->m_bdx1bar.GetScrollPos();
	this->m_option.layerb[1].win.ltp.y   = (UINT16)this->m_bdy1bar.GetScrollPos();
	this->m_option.layerb[1].memsize.cx  = (UINT16)this->m_bmw1bar.GetScrollPos();
	this->m_option.layerb[1].memsize.cy  = (UINT16)this->m_bmh1bar.GetScrollPos();

	this->m_option.layerb[1].refwin.ltp.x   = (UINT16)this->m_brx1bar.GetScrollPos();
	this->m_option.layerb[1].refwin.ltp.y   = (UINT16)this->m_bry1bar.GetScrollPos();
	this->m_option.layerb[1].refwin.size.cx = (UINT16)this->m_brw1bar.GetScrollPos();
	this->m_option.layerb[1].refwin.size.cy = (UINT16)this->m_brh1bar.GetScrollPos();

	this->m_option.layera.pos2panel.x = (UINT16)this->m_axbar.GetScrollPos();
	this->m_option.layera.pos2panel.y = (UINT16)this->m_aybar.GetScrollPos();
	this->m_option.layera.win.size.cx = (UINT16)this->m_awbar.GetScrollPos();
	this->m_option.layera.win.size.cy = (UINT16)this->m_ahbar.GetScrollPos();
	this->m_option.layera.win.ltp.x	  = (UINT16)this->m_adxbar.GetScrollPos();
	this->m_option.layera.win.ltp.y   = (UINT16)this->m_adybar.GetScrollPos();
	this->m_option.layera.memsize.cx  = (UINT16)this->m_amwbar.GetScrollPos();
	this->m_option.layera.memsize.cy  = (UINT16)this->m_amhbar.GetScrollPos();

	this->m_option.blend[0]  = this->m_blend0bar.GetScrollPos();
	this->m_option.blend[1]  = this->m_blend1bar.GetScrollPos();

	this->m_option.bgpt.x	 = (UINT16)this->m_bgxbar.GetScrollPos();
	this->m_option.bgpt.y	 = (UINT16)this->m_bgybar.GetScrollPos();
	this->m_option.bgsize.cx = (UINT16)this->m_bgwbar.GetScrollPos();
	this->m_option.bgsize.cy = (UINT16)this->m_bghbar.GetScrollPos();

	this->m_option.dedly	 = this->m_dedlybar.GetScrollPos();
	this->m_option.deintv	 = this->m_deintvbar.GetScrollPos();
	this->m_option.lineintv  = this->m_lineintvbar.GetScrollPos();

	this->m_option.brightness = this->m_brightnessbar.GetScrollPos();
	this->m_option.contrast   = this->m_contrastbar.GetScrollPos();

	this->m_option.wsc = this->m_wscbar.GetScrollPos();
	this->m_option.wrc = this->m_wrcbar.GetScrollPos();
	this->m_option.dhc = this->m_dhcbar.GetScrollPos();
}

void CV568LcdcDlg::DispBarEditBox(void)
{
	char str[20];

	sprintf(str, "%x", this->m_option.layerb[0].pos2panel.x);
	this->m_bx0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[0].pos2panel.y);
	this->m_by0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[0].win.size.cx);
	this->m_bw0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[0].win.size.cy);
	this->m_bh0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[0].win.ltp.x);
	this->m_bdx0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[0].win.ltp.y);
	this->m_bdy0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[0].memsize.cx);
	this->m_bmw0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[0].memsize.cy);
	this->m_bmh0.SetWindowText(str);

	sprintf(str, "%x", this->m_option.layerb[0].refwin.ltp.x);
	this->m_brx0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[0].refwin.ltp.y);
	this->m_bry0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[0].refwin.size.cx);
	this->m_brw0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[0].refwin.size.cy);
	this->m_brh0.SetWindowText(str);

	sprintf(str, "%x", this->m_option.layerb[1].pos2panel.x);
	this->m_bx1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[1].pos2panel.y);
	this->m_by1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[1].win.size.cx);
	this->m_bw1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[1].win.size.cy);
	this->m_bh1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[1].win.ltp.x);
	this->m_bdx1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[1].win.ltp.y);
	this->m_bdy1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[1].memsize.cx);
	this->m_bmw1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[1].memsize.cy);
	this->m_bmh1.SetWindowText(str);

	sprintf(str, "%x", this->m_option.layerb[1].refwin.ltp.x);
	this->m_brx1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[1].refwin.ltp.y);
	this->m_bry1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[1].refwin.size.cx);
	this->m_brw1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layerb[1].refwin.size.cy);
	this->m_brh1.SetWindowText(str);

	sprintf(str, "%x", this->m_option.layera.pos2panel.x);
	this->m_ax.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layera.pos2panel.y);
	this->m_ay.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layera.win.size.cx);
	this->m_aw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layera.win.size.cy);
	this->m_ah.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layera.win.ltp.x);
	this->m_adx.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layera.win.ltp.y);
	this->m_ady.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layera.memsize.cx);
	this->m_amw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.layera.memsize.cy);
	this->m_amh.SetWindowText(str);

	sprintf(str, "%x", this->m_option.blend[0]);
	this->m_blend0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.blend[1]);
	this->m_blend1.SetWindowText(str);

	sprintf(str, "%x", this->m_option.bgpt.x);
	this->m_bgx.SetWindowText(str);
	sprintf(str, "%x", this->m_option.bgpt.y);
	this->m_bgy.SetWindowText(str);
	sprintf(str, "%x", this->m_option.bgsize.cx);
	this->m_bgw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.bgsize.cy);
	this->m_bgh.SetWindowText(str);

	sprintf(str, "%x", this->m_option.dedly);
	this->m_dedly.SetWindowText(str);
	sprintf(str, "%x", this->m_option.deintv);
	this->m_deintv.SetWindowText(str);
	sprintf(str, "%x", this->m_option.lineintv);
	this->m_lineintv.SetWindowText(str);

	sprintf(str, "%x", this->m_option.brightness);
	this->m_brightness.SetWindowText(str);
	sprintf(str, "%x", this->m_option.contrast);
	this->m_contrast.SetWindowText(str);

	sprintf(str, "%x", this->m_option.wsc);
	this->m_wsc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.wrc);
	this->m_wrc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dhc);
	this->m_dhc.SetWindowText(str);
}

void CV568LcdcDlg::AdjustScrollBar(void)
{
	int width = 0, height = 0;

	width = (this->m_option.ippdispsize.cx > 510) ? 510 : this->m_option.ippdispsize.cx;
	height = (this->m_option.ippdispsize.cy > 512) ? 512 : this->m_option.ippdispsize.cy;

	this->m_amwbar.SetScrollRange(0, width);
	this->m_amwbar.SetScrollPos(width);
	this->m_amhbar.SetScrollRange(0, height);
	this->m_amhbar.SetScrollPos(height);
	if(this->m_option.arotmode % 0x02)
	{
		this->m_axbar.SetScrollRange(0, (this->m_option.panelsize.cy - this->m_option.layera.win.size.cx));
		this->m_axbar.SetScrollPos(this->m_option.layera.pos2panel.x);
		this->m_aybar.SetScrollRange(0, (this->m_option.panelsize.cx - this->m_option.layera.win.size.cy));
		this->m_aybar.SetScrollPos(this->m_option.layera.pos2panel.y);

		height = this->m_option.panelsize.cx > height ? height : this->m_option.panelsize.cx;
		this->m_awbar.SetScrollRange(0, height);
		this->m_option.layera.win.size.cx = (UINT16)height;
		this->m_awbar.SetScrollPos(this->m_option.layera.win.size.cx);
		width = this->m_option.panelsize.cy > width ? width : this->m_option.panelsize.cy;
		this->m_ahbar.SetScrollRange(0, width);
		this->m_option.layera.win.size.cy = (UINT16)width;
		this->m_ahbar.SetScrollPos(this->m_option.layera.win.size.cy);

		this->m_adxbar.SetScrollRange(0, (this->m_option.layera.memsize.cy - this->m_option.layera.win.size.cx));
		this->m_adxbar.SetScrollPos(this->m_option.layera.win.ltp.x);
		this->m_adybar.SetScrollRange(0, (this->m_option.panelsize.cx - this->m_option.layera.win.size.cy));
		this->m_adybar.SetScrollPos(this->m_option.layera.win.ltp.y);
	}
	else
	{
		this->m_axbar.SetScrollRange(0, (this->m_option.panelsize.cx - this->m_option.layera.win.size.cx));
		this->m_axbar.SetScrollPos(this->m_option.layera.pos2panel.x);
		this->m_aybar.SetScrollRange(0, (this->m_option.panelsize.cy - this->m_option.layera.win.size.cy));
		this->m_aybar.SetScrollPos(this->m_option.layera.pos2panel.y);

		this->m_awbar.SetScrollRange(0, width);
		this->m_awbar.SetScrollPos((this->m_option.layera.win.size.cx > width ? width : this->m_option.layera.win.size.cx));
		this->m_ahbar.SetScrollRange(0, height);
		this->m_ahbar.SetScrollPos((this->m_option.layera.win.size.cy > height ? height : this->m_option.layera.win.size.cy));

		this->m_adxbar.SetScrollRange(0, (this->m_option.layera.memsize.cx - this->m_option.layera.win.size.cx));
		this->m_adxbar.SetScrollPos(this->m_option.layera.win.ltp.x);
		this->m_adybar.SetScrollRange(0, (this->m_option.layera.memsize.cy - this->m_option.layera.win.size.cy));
		this->m_adybar.SetScrollPos(this->m_option.layera.win.ltp.y);
	}

	this->m_bmw0bar.SetScrollRange(0, 511);
	this->m_bmw0bar.SetScrollPos(this->m_option.layerb[0].memsize.cx);
	this->m_bmh0bar.SetScrollRange(0, 511);
	this->m_bmh0bar.SetScrollPos(this->m_option.layerb[0].memsize.cy);

	width = (this->m_option.layerb[0].memsize.cx > this->m_option.panelsize.cx) ? 
				this->m_option.panelsize.cx : this->m_option.layerb[0].memsize.cx;
	height = (this->m_option.layerb[0].memsize.cy > this->m_option.panelsize.cy) ? 
				this->m_option.panelsize.cy : this->m_option.layerb[0].memsize.cy;

	this->m_bx0bar.SetScrollRange(0, (this->m_option.panelsize.cx - this->m_option.layerb[0].win.size.cx));
	this->m_bx0bar.SetScrollPos(this->m_option.layerb[0].pos2panel.x);
	this->m_by0bar.SetScrollRange(0, (this->m_option.panelsize.cy - this->m_option.layerb[0].win.size.cy));
	this->m_by0bar.SetScrollPos(this->m_option.layerb[0].pos2panel.y);

	this->m_bw0bar.SetScrollRange(0, width);
	this->m_bw0bar.SetScrollPos(this->m_option.layerb[0].win.size.cx);
	this->m_bh0bar.SetScrollRange(0, height);
	this->m_bh0bar.SetScrollPos(this->m_option.layerb[0].win.size.cy);

	this->m_bdx0bar.SetScrollRange(0, (this->m_option.layerb[0].memsize.cx - this->m_option.layerb[0].win.size.cx));
	this->m_bdx0bar.SetScrollPos(this->m_option.layerb[0].win.ltp.x);
	this->m_bdy0bar.SetScrollRange(0, (this->m_option.layerb[0].memsize.cy - this->m_option.layerb[0].win.size.cy));
	this->m_bdy0bar.SetScrollPos(this->m_option.layerb[0].win.ltp.y);

	this->m_brx0bar.SetScrollRange(0, (this->m_option.layerb[0].win.size.cx - this->m_option.layerb[0].refwin.size.cx));
	this->m_brx0bar.SetScrollPos(this->m_option.layerb[0].refwin.ltp.x);
	this->m_bry0bar.SetScrollRange(0, (this->m_option.layerb[0].win.size.cy - this->m_option.layerb[0].refwin.size.cy));
	this->m_bry0bar.SetScrollPos(this->m_option.layerb[0].refwin.ltp.y);

	this->m_brw0bar.SetScrollRange(0, this->m_option.layerb[0].win.size.cx);
	this->m_brw0bar.SetScrollPos(this->m_option.layerb[0].refwin.size.cx);
	this->m_brh0bar.SetScrollRange(0, this->m_option.layerb[0].win.size.cy);
	this->m_brh0bar.SetScrollPos(this->m_option.layerb[0].refwin.size.cy);

	this->m_bmw1bar.SetScrollRange(0, 511);
	this->m_bmw1bar.SetScrollPos(this->m_option.layerb[1].memsize.cx);
	this->m_bmh1bar.SetScrollRange(0, 511);
	this->m_bmh1bar.SetScrollPos(this->m_option.layerb[1].memsize.cy);

	width = (this->m_option.layerb[1].memsize.cx > this->m_option.panelsize.cx) ? 
				this->m_option.panelsize.cx : this->m_option.layerb[1].memsize.cx;
	height = (this->m_option.layerb[1].memsize.cy > this->m_option.panelsize.cy) ? 
				this->m_option.panelsize.cy : this->m_option.layerb[1].memsize.cy;

	this->m_bx1bar.SetScrollRange(0, (this->m_option.panelsize.cx - this->m_option.layerb[1].win.size.cx));
	this->m_bx1bar.SetScrollPos(this->m_option.layerb[1].pos2panel.x);
	this->m_by1bar.SetScrollRange(0, (this->m_option.panelsize.cy - this->m_option.layerb[1].win.size.cy));
	this->m_by1bar.SetScrollPos(this->m_option.layerb[1].pos2panel.y);

	this->m_bw1bar.SetScrollRange(0, width);
	this->m_bw1bar.SetScrollPos(this->m_option.layerb[1].win.size.cx);
	this->m_bh1bar.SetScrollRange(0, height);
	this->m_bh1bar.SetScrollPos(this->m_option.layerb[1].win.size.cy);

	this->m_bdx1bar.SetScrollRange(0, (this->m_option.layerb[1].memsize.cx - this->m_option.layerb[1].win.size.cx));
	this->m_bdx1bar.SetScrollPos(this->m_option.layerb[1].win.ltp.x);
	this->m_bdy1bar.SetScrollRange(0, (this->m_option.layerb[1].memsize.cy - this->m_option.layerb[1].win.size.cy));
	this->m_bdy1bar.SetScrollPos(this->m_option.layerb[1].win.ltp.y);

	this->m_brx1bar.SetScrollRange(0, (this->m_option.layerb[1].win.size.cx - this->m_option.layerb[1].refwin.size.cx));
	this->m_brx1bar.SetScrollPos(this->m_option.layerb[1].refwin.ltp.x);
	this->m_bry1bar.SetScrollRange(0, (this->m_option.layerb[1].win.size.cy - this->m_option.layerb[1].refwin.size.cy));
	this->m_bry1bar.SetScrollPos(this->m_option.layerb[1].refwin.ltp.y);

	this->m_brw1bar.SetScrollRange(0, this->m_option.layerb[1].win.size.cx);
	this->m_brw1bar.SetScrollPos(this->m_option.layerb[1].refwin.size.cx);
	this->m_brh1bar.SetScrollRange(0, this->m_option.layerb[1].win.size.cy);
	this->m_brh1bar.SetScrollPos(this->m_option.layerb[1].refwin.size.cy);

	this->m_blend0bar.SetScrollRange(0, 255);
	this->m_blend0bar.SetScrollPos(this->m_option.blend[0]);
	this->m_blend1bar.SetScrollRange(0, 255);
	this->m_blend1bar.SetScrollPos(this->m_option.blend[1]);

	this->m_bgxbar.SetScrollRange(0, this->m_option.panelsize.cx - 1);
	this->m_bgxbar.SetScrollPos(this->m_option.bgpt.x);
	this->m_bgybar.SetScrollRange(0, this->m_option.panelsize.cy - 1);
	this->m_bgybar.SetScrollPos(this->m_option.bgpt.y);

	this->m_bgwbar.SetScrollRange(0, this->m_option.panelsize.cx);
	this->m_bgwbar.SetScrollPos(this->m_option.bgsize.cx);
	this->m_bghbar.SetScrollRange(0, this->m_option.panelsize.cy - 1);
	this->m_bghbar.SetScrollPos(this->m_option.bgsize.cy);

	this->m_dedlybar.SetScrollRange(14, 255);
	this->m_dedlybar.SetScrollPos(this->m_option.dedly);
	this->m_deintvbar.SetScrollRange(2, 63);
	this->m_deintvbar.SetScrollPos(this->m_option.deintv);
	this->m_lineintvbar.SetScrollRange(14, 255);
	this->m_lineintvbar.SetScrollPos(this->m_option.lineintv);

	this->m_brightnessbar.SetScrollRange(0, 64);
	this->m_brightnessbar.SetScrollPos(this->m_option.brightness);
	this->m_contrastbar.SetScrollRange(0, 64);
	this->m_contrastbar.SetScrollPos(this->m_option.contrast);

	this->m_wscbar.SetScrollRange(0,31);
	this->m_wscbar.SetScrollPos(this->m_option.wsc);
	this->m_wrcbar.SetScrollRange(1,31);
	this->m_wrcbar.SetScrollPos(this->m_option.wrc);
	this->m_dhcbar.SetScrollRange(1,31);
	this->m_dhcbar.SetScrollPos(this->m_option.dhc);

	this->GetBarValue();
	this->DispBarEditBox();
}

void CV568LcdcDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			nPos++;
			pScrollBar->SetScrollPos(nPos);
			break;
		default:
			break;
		}
		this->GetBarValue();
		this->AdjustScrollBar();
	}

	if(pScrollBar == &(this->m_blend0bar))
		V5B_LcdcSetB0BlendRatio((UINT8)this->m_option.blend[0]);
	else if(pScrollBar == &(this->m_blend1bar))
		V5B_LcdcSetB1BlendRatio((UINT8)this->m_option.blend[1]);
	else if(pScrollBar == &(this->m_brightnessbar))
		V5_DispSetBrightness((UINT8)this->m_option.brightness);
	else if(pScrollBar == &(this->m_contrastbar))
		V5_DispSetContrast((UINT8)this->m_option.contrast);

	CBaseCtrlDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

/********************************************* EditBox Section *********************************************/

void CV568LcdcDlg::GetEditboxValue(void)
{
	char str[20];

	this->m_kcolor0.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.kcolor[0]));
	this->m_kcolor1.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.kcolor[1]));
	this->m_bgcolor.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.bgcolor));

	this->m_gammaval00.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[0]));
	this->m_gammaval01.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[1]));
	this->m_gammaval02.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[2]));
	this->m_gammaval03.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[3]));
	this->m_gammaval04.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[4]));
	this->m_gammaval05.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[5]));
	this->m_gammaval06.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[6]));
	this->m_gammaval07.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[7]));
	this->m_gammaval08.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[8]));
	this->m_gammaval09.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[9]));
	this->m_gammaval10.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[10]));
	this->m_gammaval11.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[11]));
	this->m_gammaval12.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[12]));
	this->m_gammaval13.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[13]));
	this->m_gammaval14.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[14]));
	this->m_gammaval15.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[15]));
	this->m_gammaval16.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.gamma[16]));
}

void CV568LcdcDlg::DispEditBox(void)
{
	char str[20];

	sprintf(str, "%x", this->m_option.kcolor[0]);
	this->m_kcolor0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.kcolor[1]);
	this->m_kcolor1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.bgcolor);
	this->m_bgcolor.SetWindowText(str);

	sprintf(str, "%x", this->m_option.gamma[0]);
	this->m_gammaval00.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[1]);
	this->m_gammaval01.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[2]);
	this->m_gammaval02.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[3]);
	this->m_gammaval03.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[4]);
	this->m_gammaval04.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[5]);
	this->m_gammaval05.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[6]);
	this->m_gammaval06.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[7]);
	this->m_gammaval07.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[8]);
	this->m_gammaval08.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[9]);
	this->m_gammaval09.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[10]);
	this->m_gammaval10.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[11]);
	this->m_gammaval11.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[12]);
	this->m_gammaval12.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[13]);
	this->m_gammaval13.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[14]);
	this->m_gammaval14.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[15]);
	this->m_gammaval15.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gamma[16]);
	this->m_gammaval16.SetWindowText(str);
}

void CV568LcdcDlg::RefreshEditBox(void)
{
	UINT8 tmp = 0;

	this->m_option.kcolor[0] = (int)V5B_LcdcGetB0OverKeyColor();
	this->m_option.kcolor[1] = (int)V5B_LcdcGetB1OverKeyColor();
	this->m_option.bgcolor	 = (int)V5B_LcdcGetBgColor();

	if(this->m_option.gamma_sel == R_TABLE)
	{
		for(tmp = 0; tmp < 17; tmp++)
			this->m_option.gamma[tmp] = V5B_LcdcGetGammaRUnit(tmp);
	}
	else if(this->m_option.gamma_sel == G_TABLE)
	{
		for(tmp = 0; tmp < 17; tmp++)
			this->m_option.gamma[tmp] = V5B_LcdcGetGammaGUnit(tmp);
	}
	else
	{
		for(tmp = 0; tmp < 17; tmp++)
			this->m_option.gamma[tmp] = V5B_LcdcGetGammaBUnit(tmp);
	}
}

/********************************************** Button Section *********************************************/

void CV568LcdcDlg::OnBnClickedRefreshButton()
{
	V5_DispGetPanelSize(&(this->m_option.panelsize));
	V5B_IppGetDispSize(&(this->m_option.ippdispsize));
	V5_DispGetAmemSize(&(this->m_option.layera));
	V5_DispGetB0Proty(&(this->m_option.layerb[0]));
	V5_DispGetB1Proty(&(this->m_option.layerb[1]));
	V5B_LcdcGetBgStartPt(&(this->m_option.bgpt));
	V5B_LcdcGetBgSize(&(this->m_option.bgsize));

	this->m_option.laybfmt[0] = (int)V5_DispGetB0format();
	this->m_option.laybfmt[1] = (int)V5_DispGetB1format();

	this->RefreshBar();
	this->RefreshCombo();
	this->RefreshEditBox();
	this->RefreshCheckBox();

	this->DispOption();
	this->DispSpeedInfo();
}

void CV568LcdcDlg::OnBnClickedSetButton()
{
	UINT8 i = 0;

	this->GetUserOption();

	V5B_LcdcSetDEdelay((UINT8)this->m_option.dedly);
	V5B_LcdcSetDEinteval((UINT8)this->m_option.deintv);
	V5B_LcdcSetLineinteval((UINT8)this->m_option.lineintv);

	if(this->m_option.gamma_sel == R_TABLE)
	{
		for(i = 0; i < 17; i++)
			V5B_LcdcSetGammaRUnit(i, (UINT8)this->m_option.gamma[i]);
	}
	else if(this->m_option.gamma_sel == G_TABLE)
	{
		for(i = 0; i < 17; i++)
			V5B_LcdcSetGammaGUnit(i, (UINT8)this->m_option.gamma[i]);
	}
	else
	{
		for(i = 0; i < 17; i++)
			V5B_LcdcSetGammaBUnit(i, (UINT8)this->m_option.gamma[i]);
	}
}

void CV568LcdcDlg::OnBnClickedLcdcrstButton()
{
	V5B_LcdcResetState();
}

void CV568LcdcDlg::OnBnClickedSetAButton()
{
	V5_DispSetAmemSize(&(this->m_option.layera));
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
}

void CV568LcdcDlg::OnBnClickedSetB0Button()
{
	V5_DispSetB0Proty(&(this->m_option.layerb[0]));
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
}

void CV568LcdcDlg::OnBnClickedUpdateB0Button()
{
	TWinPos winpos;

	winpos.ps = this->m_option.layerb[0].refwin.ltp;
	winpos.size = this->m_option.layerb[0].refwin.size;
	V5_DispUpdateB0Win(&winpos);
}

void CV568LcdcDlg::OnBnClickedSetB1Button()
{
	V5_DispSetB1Proty(&(this->m_option.layerb[1]));
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
}

void CV568LcdcDlg::OnBnClickedUpdateB1Button()
{
	TWinPos winpos;

	winpos.ps = this->m_option.layerb[1].refwin.ltp;
	winpos.size = this->m_option.layerb[1].refwin.size;
	V5_DispUpdateB1Win(&winpos);
}

void CV568LcdcDlg::OnBnClickedSetBgButton()
{
	V5B_LcdcSetBgStartPt(this->m_option.bgpt);
	V5B_LcdcSetBgSize(this->m_option.bgsize);
}

void CV568LcdcDlg::OnBnClickedUpdateBgButton()
{
	V5B_LcdcBgUpdate();
}

void CV568LcdcDlg::OnBnClickedKcolor0Button()
{
	char str[20];

	this->m_kcolor0.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.kcolor[0]));
	V5B_LcdcSetB0OverKeyColor((UINT32)this->m_option.kcolor[0]);
}

void CV568LcdcDlg::OnBnClickedKcolor1Button()
{
	char str[20];

	this->m_kcolor1.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.kcolor[1]));
	V5B_LcdcSetB1OverKeyColor((UINT32)this->m_option.kcolor[1]);
}

void CV568LcdcDlg::OnBnClickedBgColorButton()
{
	char str[20];

	this->m_bgcolor.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.bgcolor));
	V5B_LcdcSetBgColor((UINT32)this->m_option.bgcolor);
}

/***********************************************************************************************************/

void CV568LcdcDlg::OnBnClickedTestButton()
{
	UINT16 time = 0;

	this->GetUserOption();

	time = (UINT16)this->m_option.dhc & 0x001f;
	time = time << 5;
	time |= this->m_option.wrc & 0x001f;
	time = time << 5;
	time |= this->m_option.wsc & 0x001f;
	V5_DispSetWrTiming(time);

	this->m_option.dedly = V5B_LcdcGetDEdelay();
	this->m_option.deintv = V5B_LcdcGetDEinteval();
	this->m_option.lineintv = V5B_LcdcGetLineinteval();
	this->DispOption();
	this->DispSpeedInfo();
}

void CV568LcdcDlg::DispSpeedInfo(void)
{
	int tframe = 0, tline = 0;
	UINT32 mclk = 0;
	char str[20];
	TSize size;

	V5_DispGetPanelSize(&size);
    mclk = V5_CtrlGetMclk();
	tline = (this->m_option.dedly + 2 + (this->m_option.deintv + 2) * (size.cx - 1) + this->m_option.lineintv + 2) / (mclk / 1000);	// us
	tframe = mclk * 1000 / ((this->m_option.dedly + 2 + (this->m_option.deintv + 2) * (size.cx - 1) + this->m_option.lineintv + 2) * size.cy);

	sprintf(str, "%d", tframe);
	GetDlgItem(IDC_LCDCFR_STATIC)->SetWindowText(str);
	sprintf(str, "%d", tline);
	GetDlgItem(IDC_LCDCLO_STATIC)->SetWindowText(str);
}

#endif //V578CMD_DISABLE
