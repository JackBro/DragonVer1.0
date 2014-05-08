// V568LcdifDlg.cpp : 实现文件
//
#include "internal.h"
#include "..\include\v568plane\v568lcdifdlg.h"


#ifndef	V578CMD_DISABLE




// CV568LcdifDlg 对话框

IMPLEMENT_DYNAMIC(CV568LcdifDlg, CBaseCtrlDlg)
CV568LcdifDlg::CV568LcdifDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568LcdifDlg::IDD, pParent)
{
}

CV568LcdifDlg::~CV568LcdifDlg()
{
}

void CV568LcdifDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);

	// Check Box
	DDX_Control(pDX, IDC_HEADEN_CHECK,			m_headenbtn);
	DDX_Control(pDX, IDC_RSHEN_CHECK,			m_rshighbtn);
	DDX_Control(pDX, IDC_DMAPCONV_CHECK,		m_dmapconvbtn);
	DDX_Control(pDX, IDC_BIASDIVX_CHECK,		m_biasdivxbtn);
	DDX_Control(pDX, IDC_BIASDIVY_CHECK,		m_biasdivybtn);
	DDX_Control(pDX, IDC_COMM_CODE_EN_CHECK,	m_commcode);
	DDX_Control(pDX, IDC_HEAD_RPT_EN_CHECK,		m_headrpt);

	// Combo Box
	DDX_Control(pDX, IDC_CSMODE_COMBO,			m_csmodebox);
	DDX_Control(pDX, IDC_HEADNUM_COMBO,			m_headnumbox);
	DDX_Control(pDX, IDC_DATAFMT_COMBO,			m_datafmt);
	DDX_Control(pDX, IDC_MAPPTN_COMBO,			m_mapptn);
	DDX_Control(pDX, IDC_CSSEL_COMBO,			m_csselbox);
	DDX_Control(pDX, IDC_BUSTYPE_COMBO,			m_bustypebox);
	DDX_Control(pDX, IDC_PANELSEL_COMBO,		m_panelselbox);

	// ScrollBar
	DDX_Control(pDX, IDC_WSC_SCROLLBAR,			m_wscbar);
	DDX_Control(pDX, IDC_WRC_SCROLLBAR,			m_wrcbar);
	DDX_Control(pDX, IDC_DHC_SCROLLBAR,			m_dhcbar);
	DDX_Control(pDX, IDC_RSC_SCROLLBAR,			m_rscbar);
	DDX_Control(pDX, IDC_RAC_SCROLLBAR,			m_racbar);
	DDX_Control(pDX, IDC_ROC_SCROLLBAR,			m_rocbar);
	DDX_Control(pDX, IDC_BIASX_SCROLLBAR,		m_biasxbar);
	DDX_Control(pDX, IDC_BIASY_SCROLLBAR,		m_biasybar);
	DDX_Control(pDX, IDC_SX_SCROLLBAR,			m_linesxbar);
	DDX_Control(pDX, IDC_SY_SCROLLBAR,			m_linesybar);
	DDX_Control(pDX, IDC_EX_SCROLLBAR,			m_lineexbar);

	// ScrollBar EditBox
	DDX_Control(pDX, IDC_WSC_EDIT,				m_wsc);
	DDX_Control(pDX, IDC_WRC_EDIT,				m_wrc);
	DDX_Control(pDX, IDC_DHC_EDIT,				m_dhc);
	DDX_Control(pDX, IDC_RSC_EDIT,				m_rsc);
	DDX_Control(pDX, IDC_RAC_EDIT,				m_rac);
	DDX_Control(pDX, IDC_ROC_EDIT,				m_roc);
	DDX_Control(pDX, IDC_BIASX_EDIT,			m_biasx);
	DDX_Control(pDX, IDC_BIASY_EDIT,			m_biasy);
	DDX_Control(pDX, IDC_SX_EDIT,				m_linesx);
	DDX_Control(pDX, IDC_SY_EDIT,				m_linesy);
	DDX_Control(pDX, IDC_EX_EDIT,				m_lineex);

	// EditBox
	DDX_Control(pDX, IDC_CSFLAG_EDIT,			m_csflag);
	DDX_Control(pDX, IDC_RS_EDIT,				m_rsflag);
	DDX_Control(pDX, IDC_STARTXPOS_EDIT,		m_startx_pos);
	DDX_Control(pDX, IDC_STARTYPOS_EDIT,		m_starty_pos);
	DDX_Control(pDX, IDC_ENDXPOS_EDIT,			m_endx_pos);
	DDX_Control(pDX, IDC_ENDYPOS_EDIT,			m_endy_pos);
	DDX_Control(pDX, IDC_DATA0_EDIT,			m_headdata0);
	DDX_Control(pDX, IDC_DATA1_EDIT,			m_headdata1);
	DDX_Control(pDX, IDC_DATA2_EDIT,			m_headdata2);
	DDX_Control(pDX, IDC_DATA3_EDIT,			m_headdata3);
	DDX_Control(pDX, IDC_DATA4_EDIT,			m_headdata4);
	DDX_Control(pDX, IDC_DATA5_EDIT,			m_headdata5);
	DDX_Control(pDX, IDC_DATA6_EDIT,			m_headdata6);
	DDX_Control(pDX, IDC_DATA7_EDIT,			m_headdata7);
	DDX_Control(pDX, IDC_DATA8_EDIT,			m_headdata8);
	DDX_Control(pDX, IDC_DATA9_EDIT,			m_headdata9);
	DDX_Control(pDX, IDC_DATA10_EDIT,			m_headdata10);
	DDX_Control(pDX, IDC_DATA11_EDIT,			m_headdata11);
	DDX_Control(pDX, IDC_DATA12_EDIT,			m_headdata12);
	DDX_Control(pDX, IDC_DATA13_EDIT,			m_headdata13);
	DDX_Control(pDX, IDC_DATA14_EDIT,			m_headdata14);
	DDX_Control(pDX, IDC_DATA15_EDIT,			m_headdata15);
	DDX_Control(pDX, IDC_DATA_EDIT,				m_linedata);

	DDX_Control(pDX, IDC_CMD_SX_EDIT,			m_cmdsx);
	DDX_Control(pDX, IDC_CMD_EX_EDIT,			m_cmdex);
	DDX_Control(pDX, IDC_CMD_SY_EDIT,			m_cmdsy);
	DDX_Control(pDX, IDC_CMD_EY_EDIT,			m_cmdey);
	DDX_Control(pDX, IDC_RPT_SX_EDIT,			m_rptsx);
	DDX_Control(pDX, IDC_RPT_EX_EDIT,			m_rptex);
	DDX_Control(pDX, IDC_RPT_SY_EDIT,			m_rptsy);
	DDX_Control(pDX, IDC_RPT_EY_EDIT,			m_rptey);
}


BEGIN_MESSAGE_MAP(CV568LcdifDlg, CBaseCtrlDlg)
	ON_WM_HSCROLL()
	// Check Box
	ON_BN_CLICKED(IDC_HEADEN_CHECK,			OnBnClickedHeadenCheck)
	ON_BN_CLICKED(IDC_RSHEN_CHECK,			OnBnClickedRshenCheck)
	ON_BN_CLICKED(IDC_DMAPCONV_CHECK,		OnBnClickedDmapconvCheck)
	ON_BN_CLICKED(IDC_BIASDIVX_CHECK,		OnBnClickedBiasdivxCheck)
	ON_BN_CLICKED(IDC_BIASDIVY_CHECK,		OnBnClickedBiasdivyCheck)
	ON_BN_CLICKED(IDC_COMM_CODE_EN_CHECK,	OnBnClickedCommCodeEnCheck)
	ON_BN_CLICKED(IDC_HEAD_RPT_EN_CHECK,	OnBnClickedHeadRptEnCheck)

	// Combo Box
	ON_CBN_SELCHANGE(IDC_HEADNUM_COMBO,		OnCbnSelchangeHeadnumCombo)
	ON_CBN_SELCHANGE(IDC_DATAFMT_COMBO,		OnCbnSelchangeDatafmtCombo)
	ON_CBN_SELCHANGE(IDC_MAPPTN_COMBO,		OnCbnSelchangeMapptnCombo)
	ON_CBN_SELCHANGE(IDC_CSSEL_COMBO,		OnCbnSelchangeCsselCombo)
	ON_CBN_SELCHANGE(IDC_BUSTYPE_COMBO,		OnCbnSelchangeBustypeCombo)
	ON_CBN_SELCHANGE(IDC_PANELSEL_COMBO,	OnCbnSelchangePanelselCombo)
	ON_CBN_SELCHANGE(IDC_CSMODE_COMBO,		OnCbnSelchangeCsmodeCombo)

	// Button
	ON_BN_CLICKED(IDC_INITPANEL_BUTTON,		OnBnClickedInitpanelButton)
	ON_BN_CLICKED(IDC_DRAWIMAGE_BUTTON,		OnBnClickedDrawimageButton)
	ON_BN_CLICKED(IDC_READPANEL_BUTTON,		OnBnClickedReadpanelButton)
	ON_BN_CLICKED(IDC_UPDATE_BUTTON,		OnBnClickedUpdateButton)
	ON_BN_CLICKED(IDC_SETCFG_BUTTON,		OnBnClickedSetcfgButton)
	ON_BN_CLICKED(IDC_RESTPANEL_BUTTON,		OnBnClickedRestpanelButton)
	ON_BN_CLICKED(IDC_LINESTART_BUTTON,		OnBnClickedLinestartButton)
END_MESSAGE_MAP()


// CV568LcdifDlg 消息处理程序

BOOL CV568LcdifDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CV568LcdifDlg::InitUserOption(void)
{
	memset(&(this->m_option), 0, sizeof(TLcdifOption));
	this->InitCombo();
}

void CV568LcdifDlg::GetUserOption(char *tail)
{
	this->GetEditboxValue();
	this->GetComboValue(tail);
}

void CV568LcdifDlg::DispOption(void)
{
	this->DispCombo();
	this->DispEditBox();
	this->DispCheckBox();
	this->AdjustScrollBar();
}

/******************************************** Check Box Section ********************************************/

void CV568LcdifDlg::OnBnClickedHeadenCheck()
{
	if(this->m_headenbtn.GetCheck())
	{
		this->m_option.config |= HEAD_EN;
		V5B_LcdifEnableLineHead();
	}
	else
	{
		this->m_option.config &= ~HEAD_EN;
		V5B_LcdifDisableLineHead();
	}
}

void CV568LcdifDlg::OnBnClickedRshenCheck()
{
	if(this->m_rshighbtn.GetCheck())
	{
		this->m_option.config |= RSHIGH_EN;
		V5B_LcdifEnableRsHigh();
	}
	else
	{
		this->m_option.config &= ~RSHIGH_EN;
		V5B_LcdifDisableRsHigh();
	}
}

void CV568LcdifDlg::OnBnClickedDmapconvCheck()
{
	UINT8 tmp;

	tmp = V5B_LcdifGetDmapfmt();
	if(this->m_dmapconvbtn.GetCheck())
	{
		this->m_option.config |= DMAP_CONV;
		V5B_LcdifSetDmapfmt(0x80 | tmp);
	}
	else
	{
		this->m_option.config &= ~DMAP_CONV;
		V5B_LcdifSetDmapfmt(0x7f & tmp);
	}
}

void CV568LcdifDlg::OnBnClickedBiasdivxCheck()
{
	if(this->m_biasdivxbtn.GetCheck())
	{
		this->m_option.config |= BIAS_DIVX;
		V5B_LcdifDivxEnable();
	}
	else
	{
		this->m_option.config &= ~BIAS_DIVX;
		V5B_LcdifDivxDisalbe();
	}
}

void CV568LcdifDlg::OnBnClickedBiasdivyCheck()
{
	if(this->m_biasdivybtn.GetCheck())
	{
		this->m_option.config |= BIAS_DIVY;
		V5B_LcdifDivyEnable();
	}
	else
	{
		this->m_option.config &= ~BIAS_DIVY;
		V5B_LcdifDivyDisalbe();
	}
}

void CV568LcdifDlg::OnBnClickedCommCodeEnCheck()
{
	if(this->m_commcode.GetCheck())
	{
		this->m_option.procflg |= 0x01;
		V5B_LcdifCommCodeEnable();
	}
	else
	{
		this->m_option.procflg &= 0xfe;
		V5B_LcdifCommCodeDisalbe();
	}
}

void CV568LcdifDlg::OnBnClickedHeadRptEnCheck()
{
	if(this->m_headrpt.GetCheck())
	{
		this->m_option.procflg |= 0x02;
		V5B_LcdifHeadRptEnable();
	}
	else
	{
		this->m_option.procflg &= 0xfd;
		V5B_LcdifHeadRptDisalbe();
	}
}

void CV568LcdifDlg::RefreshCheckBox(void)
{
	if(0x01 & V5B_LcdifGetHeadCfg())
		this->m_option.config |= HEAD_EN;
	else
		this->m_option.config &= ~HEAD_EN;

	if(0x08 & V5B_LcdifGetCfg())
		this->m_option.config |= RSHIGH_EN;
	else
		this->m_option.config &= ~RSHIGH_EN;

	if(0x80 & V5B_LcdifGetDmapfmt())
		this->m_option.config |= DMAP_CONV;
	else
		this->m_option.config &= ~DMAP_CONV;

	if(0x80 & V5B_LcdifGetBiasX())
		this->m_option.config |= BIAS_DIVX;
	else
		this->m_option.config &= ~BIAS_DIVX;

	if(0x80 & V5B_LcdifGetBiasY())
		this->m_option.config |= BIAS_DIVY;
	else
		this->m_option.config &= ~BIAS_DIVY;

	this->m_option.procflg = V5B_LcdifGetHeadProcFlag();
}

void CV568LcdifDlg::DispCheckBox(void)
{
	if(this->m_option.config & HEAD_EN)
		this->m_headenbtn.SetCheck(BST_CHECKED);
	else
		this->m_headenbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & RSHIGH_EN)
		this->m_rshighbtn.SetCheck(BST_CHECKED);
	else
		this->m_rshighbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & DMAP_CONV)
		this->m_dmapconvbtn.SetCheck(BST_CHECKED);
	else
		this->m_dmapconvbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & BIAS_DIVX)
		this->m_biasdivxbtn.SetCheck(BST_CHECKED);
	else
		this->m_biasdivxbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.config & BIAS_DIVY)
		this->m_biasdivybtn.SetCheck(BST_CHECKED);
	else
		this->m_biasdivybtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.procflg & 0x01)
		this->m_commcode.SetCheck(BST_CHECKED);
	else
		this->m_commcode.SetCheck(BST_UNCHECKED);

	if(this->m_option.procflg & 0x02)
		this->m_headrpt.SetCheck(BST_CHECKED);
	else
		this->m_headrpt.SetCheck(BST_UNCHECKED);
}

/********************************************   Combo Section   ********************************************/

void CV568LcdifDlg::InitCombo(void)
{
	int i = 0;
	const char *headnum[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
	const char *datafmt[] = {	"R:G:B=5:6:5 (16bit)",
								"R:G:B=5:5:5 (16bit)",
								"R:G:B=4:4:4 (16bit)",
								"R:G:B=6:6:6 (16bit)",
								"R:G:B=5:6:5 (8bit, 2byte/pixel)",
								"R:G:B=6:6:6 (18bit, 2byte/pixel, little endian)",
								"R:G:B=6:6:6 (18bit, 2byte/pixel, big endian)",
								"R:G:B=4:4:4 (8bit, 2byte/pixel, big endian)",
								"R:G:B=4:4:4 (8bit, 2byte/pixe, little endian)",
								"R:G:B=each 6, 8bit (16bit, 2hw/pixel, big endian)",
								"R:G:B=each 6, 8bit (16bit, 2hw/pixel, little endian)",
								"R:G:B=4:4:4 (8bit, 3byte/2pixel, big endian)",
								"R:G:B=4:4:4 (8bit, 3byte/2pixel, little endian)",
								"R:G:B=each 6, 8bit (16bit, 3hw/2pixel, big endian)",
								"R:G:B=each 6, 8bit (16bit, 3hw/2pixel, little endian)" };
	const char *mapptn[]   = { "R-G-B", "R-B-G", "G-R-B", "G-B-R", "B-R-G", "B-G-R" };
	const char *cssel[]    = { "CS0", "CS1" };
	const char *bustype[]  = { "80 Bus", "68K Bus" };
	const char *panelsel[] = { "Main Panel", "Sub Panel" };
	const char *csmode[]   = { "Disable", "Enable" };

	for(i = 0; i < sizeof(headnum) / sizeof(char *); i++)
		this->m_headnumbox.InsertString(i, headnum[i]);
	for(i = 0; i < sizeof(datafmt) / sizeof(char *); i++)
		this->m_datafmt.InsertString(i, datafmt[i]);
	for(i = 0; i < sizeof(mapptn) / sizeof(char *); i++)
		this->m_mapptn.InsertString(i, mapptn[i]);
	for(i = 0; i < sizeof(cssel) / sizeof(char *); i++)
		this->m_csselbox.InsertString(i, cssel[i]);
	for(i = 0; i < sizeof(bustype) / sizeof(char *); i++)
		this->m_bustypebox.InsertString(i, bustype[i]);
	for(i = 0; i < sizeof(panelsel) / sizeof(char *); i++)
		this->m_panelselbox.InsertString(i, panelsel[i]);
	for(i = 0; i < sizeof(csmode) / sizeof(char *); i++)
		this->m_csmodebox.InsertString(i, csmode[i]);
}

void CV568LcdifDlg::GetComboValue(char *tail)
{
	this->m_option.csmode = this->m_csmodebox.GetCurSel();
	this->m_option.headnum = this->m_headnumbox.GetCurSel();
	this->m_option.datafmt = this->m_datafmt.GetCurSel();
	this->m_option.mapptn = this->m_mapptn.GetCurSel();
	this->m_option.cssel = this->m_csselbox.GetCurSel();
	this->m_option.bustype = this->m_bustypebox.GetCurSel();
	this->m_option.panelsel = this->m_panelselbox.GetCurSel();

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CV568LcdifDlg::RefreshCombo(void)
{
	this->m_option.headnum = V5B_LcdifGetHeadNum();
	this->m_option.datafmt = 0x0f & V5B_LcdifGetDmapfmt();
	if(this->m_option.datafmt > 6)
		this->m_option.datafmt -= 1;
	this->m_option.mapptn  = (0x70 & V5B_LcdifGetDmapfmt()) >> 4;
	this->m_option.cssel   = 0x10 & V5B_LcdifGetCfg();
	this->m_option.bustype = 0x20 & V5B_LcdifGetCfg();
	this->m_option.csmode  = V5B_LcdifGetCsMode();
}

void CV568LcdifDlg::DispCombo(void)
{
	this->m_headnumbox.SetCurSel(this->m_option.headnum - 1);
	this->m_datafmt.SetCurSel(this->m_option.datafmt);
	this->m_mapptn.SetCurSel(this->m_option.mapptn);
	this->m_csselbox.SetCurSel(this->m_option.cssel);
	this->m_bustypebox.SetCurSel(this->m_option.bustype);
	this->m_panelselbox.SetCurSel(this->m_option.panelsel);
	this->m_csmodebox.SetCurSel(this->m_option.csmode);
}

void CV568LcdifDlg::OnCbnSelchangeHeadnumCombo()
{
	this->m_option.headnum = this->m_headnumbox.GetCurSel() + 1;
	V5B_LcdifSetHeadNum((UINT16)this->m_option.headnum);
}

void CV568LcdifDlg::OnCbnSelchangeDatafmtCombo()
{
	UINT8 tmp, fmt;

	this->m_option.datafmt = this->m_datafmt.GetCurSel();

	tmp = 0xf0 & V5B_LcdifGetDmapfmt();
	if(this->m_option.datafmt > 6)
		fmt = (UINT8)this->m_option.datafmt + 1;
	else
		fmt = (UINT8)this->m_option.datafmt;
	tmp |= fmt & 0x0f;
	V5B_LcdifSetDmapfmt(tmp);
}

void CV568LcdifDlg::OnCbnSelchangeMapptnCombo()
{
	UINT8 tmp;

	this->m_option.mapptn = this->m_mapptn.GetCurSel();

	tmp = 0x8f & V5B_LcdifGetDmapfmt();
	tmp |= (UINT8)(this->m_option.mapptn & 0x07) << 4;
	V5B_LcdifSetDmapfmt(tmp);
}

void CV568LcdifDlg::OnCbnSelchangeCsselCombo()
{
	UINT8 tmp;

	this->m_option.cssel = this->m_csselbox.GetCurSel();

	tmp = V5B_LcdifGetCfg();
	if(this->m_option.cssel == 0)
		V5B_LcdifEnableCs0();
	else
		V5B_LcdifEnableCs1();
}

void CV568LcdifDlg::OnCbnSelchangeBustypeCombo()
{
	UINT8 tmp;

	this->m_option.bustype = this->m_bustypebox.GetCurSel();

	tmp = V5B_LcdifGetCfg();
	if(this->m_option.bustype == 0)
		V5B_LcdifEnable80Bus();
	else
		V5B_LcdifEnable68kBus();
}

void CV568LcdifDlg::OnCbnSelchangeCsmodeCombo()
{
	this->m_option.csmode = this->m_csmodebox.GetCurSel();
	V5B_LcdifSetCsMode((UINT16)this->m_option.csmode);
}

void CV568LcdifDlg::OnCbnSelchangePanelselCombo()
{
	TSize size;
	TBLayerProty protyb;
	TALayerProty protya;

	V5B_LcdcDisableAlayer();

	this->m_option.panelsel = this->m_panelselbox.GetCurSel();

	if(this->m_option.panelsel == 0)
		V5_DispSwitchPanel(PANEL_MASTER);
	else
		V5_DispSwitchPanel(PANEL_SLAVE);
	V5_DispSetPanelState(PANEL_POWERON, 0);

	V5_DispGetPanelSize(&size);
	V5_IppSetDispSizer(size);

	protyb.memsize = size;
	protyb.pos2panel.x = protyb.pos2panel.y = 0;
	protyb.win.ltp.x = protyb.win.ltp.y = 0;
	protyb.win.size = size;
	protyb.refwin.ltp.x = protyb.refwin.ltp.y = 0;
	protyb.refwin.size = size;
	V5_DispSetB0format(BLAYER_RGB565);
	V5_DispSetB1format(BLAYER_RGB565);
	V5_DispSetB0Proty(&protyb);
	memset(&protyb, 0, sizeof(protyb));
	V5_DispSetB1Proty(&protyb);

	protya.memsize = size;
	protya.pos2panel.x = protya.pos2panel.y = 0;
	protya.win.ltp.x = protya.win.ltp.y = 0;
	protya.win.size = size;
	V5_DispSetAmemSize(&protya);

	V5_MarbAutoSetMem();

	V5B_LcdcEnableAlayer();
}

/******************************************** ScrollBar Section ********************************************/

void CV568LcdifDlg::GetBarValue(void)
{
	this->m_option.line_sp.x = (UINT16)this->m_linesxbar.GetScrollPos();
	this->m_option.line_sp.y = (UINT16)this->m_linesybar.GetScrollPos();
	this->m_option.line_ex = this->m_lineexbar.GetScrollPos();

	this->m_option.wsc = this->m_wscbar.GetScrollPos();
	this->m_option.wrc = this->m_wrcbar.GetScrollPos();
	this->m_option.dhc = this->m_dhcbar.GetScrollPos();
	this->m_option.rsc = this->m_rscbar.GetScrollPos();
	this->m_option.rac = this->m_racbar.GetScrollPos();
	this->m_option.roc = this->m_rocbar.GetScrollPos();

	this->m_option.biaspt.x = (UINT16)this->m_biasxbar.GetScrollPos();
	this->m_option.biaspt.y = (UINT16)this->m_biasybar.GetScrollPos();
}

void CV568LcdifDlg::RefreshBar(void)
{
	this->m_option.wsc = 0x001f & V5B_LcdifGetWrTiming();
	this->m_option.wrc = (0x03e0 & V5B_LcdifGetWrTiming()) >> 5;
	this->m_option.dhc = (0x7c00 & V5B_LcdifGetWrTiming()) >> 10;
	this->m_option.rsc = 0x001f & V5B_LcdifGetRdTiming();
	this->m_option.rac = (0x03e0 & V5B_LcdifGetRdTiming()) >> 5;
	this->m_option.roc = (0x7c00 & V5B_LcdifGetRdTiming()) >> 10;

	this->m_option.biaspt.x = V5B_LcdifGetBiasX();
	this->m_option.biaspt.y = V5B_LcdifGetBiasY();
}

void CV568LcdifDlg::DispBarEditBox(void)
{
	char str[20];

	sprintf(str, "%x", this->m_option.line_sp.x);
	this->m_linesx.SetWindowText(str);
	sprintf(str, "%x", this->m_option.line_sp.y);
	this->m_linesy.SetWindowText(str);
	sprintf(str, "%x", this->m_option.line_ex);
	this->m_lineex.SetWindowText(str);

	sprintf(str, "%x", this->m_option.wsc);
	this->m_wsc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.wrc);
	this->m_wrc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dhc);
	this->m_dhc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rsc);
	this->m_rsc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rac);
	this->m_rac.SetWindowText(str);
	sprintf(str, "%x", this->m_option.roc);
	this->m_roc.SetWindowText(str);

	sprintf(str, "%x", this->m_option.biaspt.x);
	this->m_biasx.SetWindowText(str);
	sprintf(str, "%x", this->m_option.biaspt.y);
	this->m_biasy.SetWindowText(str);
}

void CV568LcdifDlg::AdjustScrollBar(void)
{
	TSize size;
	V5_DispGetPanelSize(&size);

	this->m_linesxbar.SetScrollRange(0, this->m_option.line_ex);
	this->m_linesxbar.SetScrollPos(this->m_option.line_sp.x);
	this->m_linesybar.SetScrollRange(0, (int)size.cy - 1);
	this->m_linesybar.SetScrollPos(this->m_option.line_sp.y);
	this->m_lineexbar.SetScrollRange(this->m_option.line_sp.x, (int)size.cx - 1);
	this->m_lineexbar.SetScrollPos(this->m_option.line_ex);

	this->m_wscbar.SetScrollRange(0, 31);
	this->m_wscbar.SetScrollPos(this->m_option.wsc);
	this->m_wrcbar.SetScrollRange(0, 31);
	this->m_wrcbar.SetScrollPos(this->m_option.wrc);
	this->m_dhcbar.SetScrollRange(0, 31);
	this->m_dhcbar.SetScrollPos(this->m_option.dhc);
	this->m_rscbar.SetScrollRange(0, 31);
	this->m_rscbar.SetScrollPos(this->m_option.rsc);
	this->m_racbar.SetScrollRange(0, 31);
	this->m_racbar.SetScrollPos(this->m_option.rac);
	this->m_rocbar.SetScrollRange(0, 31);
	this->m_rocbar.SetScrollPos(this->m_option.roc);

	this->m_biasxbar.SetScrollRange(0, size.cx - 1);
	this->m_biasxbar.SetScrollPos(this->m_option.biaspt.x);
	this->m_biasybar.SetScrollRange(0, size.cy - 1);
	this->m_biasybar.SetScrollPos(this->m_option.biaspt.y);

	this->GetBarValue();
	this->DispBarEditBox();
}

void CV568LcdifDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWaitCursor cursor;
	int pos = 0;

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
	CBaseCtrlDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

/********************************************  EditBox Section  ********************************************/

void CV568LcdifDlg::GetEditboxValue(void)
{
	char str[20];

	this->m_linedata.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.line_data));

	this->m_csflag.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.csflag));
	this->m_rsflag.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.rsflag));
	this->m_startx_pos.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.startxpos));
	this->m_starty_pos.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.startypos));
	this->m_endx_pos.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.endxpos));
	this->m_endy_pos.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.endypos));

	this->m_headdata0.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[0]));
	this->m_headdata1.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[1]));
	this->m_headdata2.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[2]));
	this->m_headdata3.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[3]));
	this->m_headdata4.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[4]));
	this->m_headdata5.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[5]));
	this->m_headdata6.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[6]));
	this->m_headdata7.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[7]));
	this->m_headdata8.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[8]));
	this->m_headdata9.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[9]));
	this->m_headdata10.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[10]));
	this->m_headdata11.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[11]));
	this->m_headdata12.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[12]));
	this->m_headdata13.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[13]));
	this->m_headdata14.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[14]));
	this->m_headdata15.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.headdata[15]));

	this->m_cmdsx.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.cmdsx));
	this->m_cmdex.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.cmdex));
	this->m_cmdsy.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.cmdsy));
	this->m_cmdey.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.cmdey));
	this->m_rptsx.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.rptsx));
	this->m_rptex.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.rptex));
	this->m_rptsy.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.rptsy));
	this->m_rptey.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.rptey));
}

void CV568LcdifDlg::RefreshEditBox(void)
{
	UINT8 i = 0;

	this->m_option.csflag = V5B_LcdifGetCsFlag();
	this->m_option.rsflag = V5B_LcdifGetHeadRsFlag();
	this->m_option.startxpos = V5B_LcdifGetHeadStxPos();
	this->m_option.startypos = V5B_LcdifGetHeadStyPos();
	this->m_option.endxpos = V5B_LcdifGetHeadEdxPos();
	this->m_option.endypos = V5B_LcdifGetHeadEdyPos();
	for(i = 0; i < LCDIF_MAX_HEAD_NUM; i++)
		this->m_option.headdata[i] = V5B_LcdifGetHeadData(i);

	this->m_option.cmdsx = V5B_LcdifGetCommStartX();
	this->m_option.cmdex = V5B_LcdifGetCommEndX();
	this->m_option.cmdsy = V5B_LcdifGetCommStartY();
	this->m_option.cmdey = V5B_LcdifGetCommEndY();
	this->m_option.rptsx = V5B_LcdifGetRptStartX();
	this->m_option.rptex = V5B_LcdifGetRptStartY();
	this->m_option.rptsy = V5B_LcdifGetRptEndX();
	this->m_option.rptey = V5B_LcdifGetRptEndY();
}

void CV568LcdifDlg::DispEditBox(void)
{
	char str[20];

	sprintf(str, "%x", this->m_option.line_data);
	this->m_linedata.SetWindowText(str);

	sprintf(str, "%x", this->m_option.csflag);
	this->m_csflag.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rsflag);
	this->m_rsflag.SetWindowText(str);
	sprintf(str, "%x", this->m_option.startxpos);
	this->m_startx_pos.SetWindowText(str);
	sprintf(str, "%x", this->m_option.startypos);
	this->m_starty_pos.SetWindowText(str);
	sprintf(str, "%x", this->m_option.endxpos);
	this->m_endx_pos.SetWindowText(str);
	sprintf(str, "%x", this->m_option.endypos);
	this->m_endy_pos.SetWindowText(str);

	sprintf(str, "%x", this->m_option.headdata[0]);
	this->m_headdata0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[1]);
	this->m_headdata1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[2]);
	this->m_headdata2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[3]);
	this->m_headdata3.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[4]);
	this->m_headdata4.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[5]);
	this->m_headdata5.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[6]);
	this->m_headdata6.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[7]);
	this->m_headdata7.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[8]);
	this->m_headdata8.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[9]);
	this->m_headdata9.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[10]);
	this->m_headdata10.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[11]);
	this->m_headdata11.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[12]);
	this->m_headdata12.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[13]);
	this->m_headdata13.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[14]);
	this->m_headdata14.SetWindowText(str);
	sprintf(str, "%x", this->m_option.headdata[15]);
	this->m_headdata15.SetWindowText(str);

	sprintf(str, "%x", this->m_option.cmdsx);
	this->m_cmdsx.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cmdex);
	this->m_cmdex.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cmdsy);
	this->m_cmdsy.SetWindowText(str);
	sprintf(str, "%x", this->m_option.cmdey);
	this->m_cmdey.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rptsx);
	this->m_rptsx.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rptex);
	this->m_rptex.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rptsy);
	this->m_rptsy.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rptey);
	this->m_rptey.SetWindowText(str);
}

/********************************************   Button Section  ********************************************/

void CV568LcdifDlg::OnBnClickedUpdateButton()
{
	this->RefreshBar();
	this->RefreshCombo();
	this->RefreshEditBox();
	this->RefreshCheckBox();
	this->DispOption();
}

void CV568LcdifDlg::OnBnClickedSetcfgButton()
{
	UINT8 tmp = 0;
	UINT16 time = 0;

	this->GetUserOption();

	V5B_LcdifSetCsFlag((UINT16)this->m_option.csflag);
	V5B_LcdifSetHeadRsFlag((UINT16)this->m_option.rsflag);
	V5B_LcdifSetHeadStxPos((UINT16)this->m_option.startxpos);
	V5B_LcdifSetHeadStyPos((UINT16)this->m_option.startypos);
	V5B_LcdifSetHeadEdxPos((UINT16)this->m_option.endxpos);
	V5B_LcdifSetHeadEdyPos((UINT16)this->m_option.endypos);
	V5B_LcdifSetBiasSpt(this->m_option.biaspt);

	time = (UINT16)this->m_option.dhc & 0x001f;
	time = time << 5;
	time |= this->m_option.wrc & 0x001f;
	time = time << 5;
	time |= this->m_option.wsc & 0x001f;
	V5B_LcdifSetWrTiming(time);

	time = (UINT16)this->m_option.roc & 0x001f;
	time = time << 5;
	time |= this->m_option.rac & 0x001f;
	time = time << 5;
	time |= this->m_option.rsc & 0x001f;
	V5B_LcdifSetRdTiming(time);

	for(tmp = 0; tmp < this->m_option.headnum; tmp++)
		V5B_LcdifSetHeadData((UINT8)tmp, (UINT16)this->m_option.headdata[tmp]);

	V5B_LcdifUpdateHead();
}

void CV568LcdifDlg::OnBnClickedRestpanelButton()
{
	V5B_LcdifResetPanel();
}

void CV568LcdifDlg::OnBnClickedInitpanelButton()
{
	CWaitCursor cursor;

	TestDisp();
	PanelDbgInitContext();
}

void CV568LcdifDlg::OnBnClickedDrawimageButton()
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	unsigned size = 0, bias = 0, datalen = 0, i = 0;

	this->GetUserOption();
	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	pool.SetSize(size);
	fb.Read(pool.m_buf, size);

	for(i = 5; i > 1; i--)
		datalen = (datalen << 8) | ((unsigned)pool.m_buf[i] & 0xff);
	for(i = 13; i > 9; i--)
		bias = (bias << 8) | ((unsigned)pool.m_buf[i] & 0xff);
	datalen -= bias;

	PanelWriteImage((unsigned short *)(&pool.m_buf[bias]), datalen >> 1);
}

void CV568LcdifDlg::OnBnClickedReadpanelButton()
{
	//CWaitCursor cursor;
	//CFileBin fb;
	//CBufferBase pool;

	//this->GetUserOption();
	//fb.Open(this->m_option.dstfile, "wb");
	//PanelReadImage((unsigned short *)pool.m_buf, this->m_option.datalen >> 1);
	//fb.Write(pool.m_buf, this->m_option.datalen);
}

void CV568LcdifDlg::OnBnClickedLinestartButton()
{
	TPoint sp;
	UINT16 len = 0;

	this->GetUserOption();
	sp = this->m_option.line_sp;
	len = (UINT16)(this->m_option.line_ex - this->m_option.line_sp.x + 1);
	PanelWriteGram(sp, (UINT16)this->m_option.line_data, len);
}

#endif //V578CMD_DISABLE
