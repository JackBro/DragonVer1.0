// V568GeDlg.cpp : 实现文件
//

#include "internal.h"
#include "..\include\v568plane\v568gedlg.h"

#ifndef	V578CMD_DISABLE




// CV568GeDlg 对话框

IMPLEMENT_DYNAMIC(CV568GeDlg, CBaseCtrlDlg)
CV568GeDlg::CV568GeDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568GeDlg::IDD, pParent)
{
}

CV568GeDlg::~CV568GeDlg()
{
}

void CV568GeDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	// Check Box
	DDX_Control(pDX, IDC_MONOPTN_CHECK,			m_monoptnbtn);
	DDX_Control(pDX, IDC_MONOSRC_CHECK,			m_monosrcbtn);
	DDX_Control(pDX, IDC_SRCWRMASK_CHECK,		m_monosrcmaskbtn);
	DDX_Control(pDX, IDC_PTNWRMASK_CHECK,		m_monoptnmaskbtn);
	DDX_Control(pDX, IDC_TRANSPARENT_CHECK,		m_transpbtn);
	DDX_Control(pDX, IDC_SRCEXTEND_CHECK,		m_srcexpbtn);
	// Combo Box
	DDX_Control(pDX, IDC_DSTBLSEL_COMBO,		m_destbselbox);
	DDX_Control(pDX, IDC_TRANSEL_COMBO,			m_transelbox);
	DDX_Control(pDX, IDC_SRCBSEL_COMBO,			m_srcbselbox);
	DDX_Control(pDX, IDC_FONTOPTION_COMBO,		m_fontoptionbox);
	// ScrollBar
	DDX_Control(pDX, IDC_GELINE_SPX_SCROLLBAR,	m_linesxbar);
	DDX_Control(pDX, IDC_GELINE_SPY_SCROLLBAR,	m_linesybar);
	DDX_Control(pDX, IDC_GELINE_EPX_SCROLLBAR,	m_lineexbar);
	DDX_Control(pDX, IDC_GELINE_EPY_SCROLLBAR,	m_lineeybar);
	DDX_Control(pDX, IDC_DST_SPX_SCROLLBAR,		m_dstsxbar);
	DDX_Control(pDX, IDC_DST_SPY_SCROLLBAR,		m_dstsybar);
	DDX_Control(pDX, IDC_DST_WIDTH_SCROLLBAR,	m_dstwidthbar);
	DDX_Control(pDX, IDC_DST_HEIGHT_SCROLLBAR,	m_dstheightbar);
	DDX_Control(pDX, IDC_SRC_SPX_SCROLLBAR,		m_srcsxbar);
	DDX_Control(pDX, IDC_SRC_SPY_SCROLLBAR,		m_srcsybar);
	DDX_Control(pDX, IDC_CLIP_SPX_SCROLLBAR,	m_clipsxbar);
	DDX_Control(pDX, IDC_CLIP_SPY_SCROLLBAR,	m_clipsybar);
	DDX_Control(pDX, IDC_CLIP_EPX_SCROLLBAR,	m_clipexbar);
	DDX_Control(pDX, IDC_CLIP_EPY_SCROLLBAR,	m_clipeybar);
	DDX_Control(pDX, IDC_FONTSX_SCROLLBAR,		m_fontsxbar);
	DDX_Control(pDX, IDC_FONTSY_SCROLLBAR,		m_fontsybar);
	// ScrollBar EditBox
	DDX_Control(pDX, IDC_GELINE_SPX_EDIT,		m_linesxbox);
	DDX_Control(pDX, IDC_GELINE_SPY_EDIT,		m_linesybox);
	DDX_Control(pDX, IDC_GELINE_EPX_EDIT,		m_lineexbox);
	DDX_Control(pDX, IDC_GELINE_EPY_EDIT,		m_lineeybox);
	DDX_Control(pDX, IDC_DST_SPX_EDIT,			m_dstsxbox);
	DDX_Control(pDX, IDC_DST_SPY_EDIT,			m_dstsybox);
	DDX_Control(pDX, IDC_DST_WIDTH_EDIT,		m_dstwidthbox);
	DDX_Control(pDX, IDC_DST_HEIGHT_EDIT,		m_dstheightbox);
	DDX_Control(pDX, IDC_SRC_SPX_EDIT,			m_srcsxbox);
	DDX_Control(pDX, IDC_SRC_SPY_EDIT,			m_srcsybox);
	DDX_Control(pDX, IDC_CLIP_SPX_EDIT,			m_clipsxbox);
	DDX_Control(pDX, IDC_CLIP_SPY_EDIT,			m_clipsybox);
	DDX_Control(pDX, IDC_CLIP_EPX_EDIT,			m_clipexbox);
	DDX_Control(pDX, IDC_CLIP_EPY_EDIT,			m_clipeybox);
	DDX_Control(pDX, IDC_FONTX_EDIT,			m_fontsxbox);
	DDX_Control(pDX, IDC_FONTY_EDIT,			m_fontsybox);
	// EditBox
	DDX_Control(pDX, IDC_BITWISE_EDIT,			m_bitwisebox);
	DDX_Control(pDX, IDC_DSTSPAN_EDIT,			m_dstspanbox);
	DDX_Control(pDX, IDC_LINECOLOR_EDIT,		m_linecolorbox);
	DDX_Control(pDX, IDC_PTNSRCBGCOLOR_EDIT,	m_ptnsrcbgbox);
	DDX_Control(pDX, IDC_PTNSRCFGCOLOR_EDIT,	m_ptnsrcfgbox);
	DDX_Control(pDX, IDC_SRCEXPBGCOLOR_EDIT,	m_srcexpbgbox);
	DDX_Control(pDX, IDC_SRCEXPFGCOLOR_EDIT,	m_srcexpfgbox);
	DDX_Control(pDX, IDC_SRCSPAN_EDIT,			m_srcspanbox);
	DDX_Control(pDX, IDC_DISPCHAR_EDIT,			m_charbox);
	DDX_Control(pDX, IDC_DISPSTR_EDIT,			m_strbox);
}


BEGIN_MESSAGE_MAP(CV568GeDlg, CBaseCtrlDlg)
	ON_WM_HSCROLL()
	// Check Box
	ON_BN_CLICKED(IDC_SRCWRMASK_CHECK,			OnBnClickedSrcwrmaskCheck)
	ON_BN_CLICKED(IDC_SRCEXTEND_CHECK,			OnBnClickedSrcextendCheck)
	ON_BN_CLICKED(IDC_MONOPTN_CHECK,			OnBnClickedMonoptnCheck)
	ON_BN_CLICKED(IDC_PTNWRMASK_CHECK,			OnBnClickedPtnwrmaskCheck)
	ON_BN_CLICKED(IDC_TRANSPARENT_CHECK,		OnBnClickedTransparentCheck)

	// Combo Box
	ON_CBN_SELCHANGE(IDC_DSTBLSEL_COMBO,		OnCbnSelchangeDstblselCombo)
	ON_CBN_SELCHANGE(IDC_SRCBSEL_COMBO,			OnCbnSelchangeSrcbselCombo)
	ON_CBN_SELCHANGE(IDC_TRANSEL_COMBO,			OnCbnSelchangeTranselCombo)
	ON_CBN_SELCHANGE(IDC_FONTOPTION_COMBO,		OnCbnSelchangeFontoptionCombo)

	// Button
	ON_BN_CLICKED(IDC_REFRESH_BUTTON,			OnBnClickedRefreshButton)
	ON_BN_CLICKED(IDC_DRAWLINE_BUTTON,			OnBnClickedDrawlineButton)
	ON_BN_CLICKED(IDC_BITBLT_BUTTON,			OnBnClickedBitbltButton)
	ON_BN_CLICKED(IDC_CLIP_BUTTON,				OnBnClickedClipButton)
	ON_BN_CLICKED(IDC_WRITEPTN_BUTTON,			OnBnClickedWriteptnButton)
	ON_BN_CLICKED(IDC_SETCOLOR_BUTTON,			OnBnClickedSetcolorButton)
	ON_BN_CLICKED(IDC_BTN_LOADFONT,				OnBnClickedBtnLoadfont)
	ON_BN_CLICKED(IDC_PREDISP_BUTTON,			OnBnClickedPredispButton)
	ON_BN_CLICKED(IDC_DISP_BUTTON,				OnBnClickedDispButton)
	ON_BN_CLICKED(IDC_DISP_STR_BUTTON, OnBnClickedDispStrButton)
END_MESSAGE_MAP()


// CV568GeDlg 消息处理程序

BOOL CV568GeDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CV568GeDlg::InitUserOption(void)
{
	memset(&(this->m_option), 0, sizeof(TGeOption));
	this->InitCombo();
}

void CV568GeDlg::GetUserOption(char *tail)
{
	this->GetEditboxValue();
	this->GetComboValue(tail);
}

void CV568GeDlg::DispOption(void)
{
	this->DispCombo();
	this->DispEditBox();
	this->DispCheckBox();
	this->AdjustScrollBar();
}

void CV568GeDlg::RefreshData(void)
{
	this->RefreshBar();
	this->RefreshCombo();
	this->RefreshEditBox();
	this->RefreshCheckBox();
	this->DispOption();
}

/******************************************** Check Box Section ********************************************/

void CV568GeDlg::OnBnClickedSrcwrmaskCheck()
{
	if(this->m_monosrcmaskbtn.GetCheck())
	{
		this->m_option.opmode0 |= SRC_MONO_WRMASK;
		V5B_GeEnableSrcMask();
	}
	else
	{
		this->m_option.opmode0 &= ~SRC_MONO_WRMASK;
		V5B_GeDisableSrcMask();
	}
}

void CV568GeDlg::OnBnClickedSrcextendCheck()
{
	if(this->m_srcexpbtn.GetCheck())
	{
		this->m_option.opmode0 |= SRC_EXTEND_EN;
		V5B_GeMonoSrcUseSrc();
	}
	else
	{
		this->m_option.opmode0 &= ~SRC_EXTEND_EN;
		V5B_GeMonoSrcUsePtnSrc();
	}
}

void CV568GeDlg::OnBnClickedMonoptnCheck()
{
	if(this->m_monoptnbtn.GetCheck())
	{
		this->m_option.opmode1 |= PTN_MONO_EN;
		V5B_GeMonoPtn();
	}
	else
	{
		this->m_option.opmode1 &= ~PTN_MONO_EN;
		V5B_GeColorPtn();
	}
}

void CV568GeDlg::OnBnClickedPtnwrmaskCheck()
{
	if(this->m_monoptnmaskbtn.GetCheck())
	{
		this->m_option.opmode1 |= PTN_MONO_WRMASK;
		V5B_GeEnablePtnMask();
	}
	else
	{
		this->m_option.opmode1 &= ~PTN_MONO_WRMASK;
		V5B_GeDisablePtnMask();
	}
}

void CV568GeDlg::OnBnClickedTransparentCheck()
{
	if(this->m_transpbtn.GetCheck())
	{
		this->m_option.opmode0 |= TRANSPARENT_EN;
		V5B_GeEnableTransparent();
	}
	else
	{
		this->m_option.opmode0 &= ~TRANSPARENT_EN;
		V5B_GeDisableTransparent();
	}
}

void CV568GeDlg::RefreshCheckBox(void)
{
	this->m_option.opmode0 = V5B_GeGetOpmode();
	this->m_option.opmode1 = V5B_GeGetOpmode1();
}

void CV568GeDlg::DispCheckBox(void)
{
	if(this->m_option.opmode0 & SRC_MONO_WRMASK)
		this->m_monosrcmaskbtn.SetCheck(BST_CHECKED);
	else
		this->m_monosrcmaskbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.opmode0 & SRC_EXTEND_EN)
		this->m_srcexpbtn.SetCheck(BST_CHECKED);
	else
		this->m_srcexpbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.opmode1 & PTN_MONO_EN)
		this->m_monoptnbtn.SetCheck(BST_CHECKED);
	else
		this->m_monoptnbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.opmode1 & PTN_MONO_WRMASK)
		this->m_monoptnmaskbtn.SetCheck(BST_CHECKED);
	else
		this->m_monoptnmaskbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.opmode0 & TRANSPARENT_EN)
		this->m_transpbtn.SetCheck(BST_CHECKED);
	else
		this->m_transpbtn.SetCheck(BST_UNCHECKED);
}

/******************************************** Combo Section ********************************************/

void CV568GeDlg::InitCombo(void)
{
	int i = 0;
	const char *dstsel[]     = {"B0", "B1"};
	const char *srcsel[]     = {"B0", "B1", "Font"};
	const char *trans[]      = {"Mode0", "Mode1", "Mode2", "Mode3"};
	const char *fontoption[] = {"Normal", "High Light"};

	for(i = 0; i < sizeof(dstsel) / sizeof(char *); i++)
		this->m_destbselbox.InsertString(i, dstsel[i]);
	for(i = 0; i < sizeof(srcsel) / sizeof(char *); i++)
		this->m_srcbselbox.InsertString(i, srcsel[i]);
	for(i = 0; i < sizeof(trans) / sizeof(char *); i++)
		this->m_transelbox.InsertString(i, trans[i]);
	for(i = 0; i < sizeof(fontoption) / sizeof(char *); i++)
		this->m_fontoptionbox.InsertString(i, fontoption[i]);
}

void CV568GeDlg::GetComboValue(char *tail)
{
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CV568GeDlg::RefreshCombo(void)
{
	this->m_option.dstbsel = V5_GeGetDst();
	this->m_option.srcbsel = V5_GeGetSrc();
	this->m_option.transel = V5B_GeGetTransparent();
}

void CV568GeDlg::DispCombo(void)
{
	this->m_destbselbox.SetCurSel(this->m_option.dstbsel);
	this->m_srcbselbox.SetCurSel(this->m_option.srcbsel);
	this->m_transelbox.SetCurSel(this->m_option.transel);
	this->m_fontoptionbox.SetCurSel(this->m_option.font_option);
}

void CV568GeDlg::OnCbnSelchangeDstblselCombo()
{
	this->m_option.dstbsel = this->m_destbselbox.GetCurSel();
	V5_GeSetDst((UINT8)this->m_option.dstbsel);
}

void CV568GeDlg::OnCbnSelchangeSrcbselCombo()
{
	this->m_option.srcbsel = this->m_srcbselbox.GetCurSel();
	V5_GeSetSrc((UINT8)this->m_option.srcbsel);
}

void CV568GeDlg::OnCbnSelchangeTranselCombo()
{
	this->m_option.transel = this->m_transelbox.GetCurSel();
	V5B_GeSetTransparent((UINT8)this->m_option.transel);
}

void CV568GeDlg::OnCbnSelchangeFontoptionCombo()
{
	this->m_option.font_option = this->m_fontoptionbox.GetCurSel();
}

/******************************************** ScrollBar Section ********************************************/

void CV568GeDlg::GetBarValue(void)
{
	this->m_option.linestart.x = (UINT16)this->m_linesxbar.GetScrollPos();
	this->m_option.linestart.y = (UINT16)this->m_linesybar.GetScrollPos();
	this->m_option.lineend.x   = (UINT16)this->m_lineexbar.GetScrollPos();
	this->m_option.lineend.y   = (UINT16)this->m_lineeybar.GetScrollPos();

	this->m_option.srcstart.x  = (UINT16)this->m_srcsxbar.GetScrollPos();
	this->m_option.srcstart.y  = (UINT16)this->m_srcsybar.GetScrollPos();

	this->m_option.dststart.x  = (UINT16)this->m_dstsxbar.GetScrollPos();
	this->m_option.dststart.y  = (UINT16)this->m_dstsybar.GetScrollPos();
	this->m_option.dstsize.cx  = (UINT16)this->m_dstwidthbar.GetScrollPos();
	this->m_option.dstsize.cy  = (UINT16)this->m_dstheightbar.GetScrollPos();

	this->m_option.clip.ltp.x  = (UINT16)this->m_clipsxbar.GetScrollPos();
	this->m_option.clip.ltp.y  = (UINT16)this->m_clipsybar.GetScrollPos();
	this->m_option.clip.rbp.x  = (UINT16)this->m_clipexbar.GetScrollPos();
	this->m_option.clip.rbp.y  = (UINT16)this->m_clipeybar.GetScrollPos();

	this->m_option.font_st.x   = (UINT16)this->m_fontsxbar.GetScrollPos();
	this->m_option.font_st.y   = (UINT16)this->m_fontsybar.GetScrollPos();
}

void CV568GeDlg::RefreshBar(void)
{
	V5B_GetDstSize(&(this->m_option.dstsize));
	V5B_GeGetLineStartPt(&(this->m_option.linestart));
	V5B_GeGetLineEndPt(&(this->m_option.lineend));
	V5B_GeGetClipping(&(this->m_option.clip));
}

void CV568GeDlg::DispBarEditBox(void)
{
	char str[20];

	sprintf(str, "%x", this->m_option.clip.rbp.x);
	this->m_clipexbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clip.rbp.y);
	this->m_clipeybox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clip.ltp.x);
	this->m_clipsxbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.clip.ltp.y);
	this->m_clipsybox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dstsize.cx);
	this->m_dstwidthbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dstsize.cy);
	this->m_dstheightbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dststart.x);
	this->m_dstsxbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dststart.y);
	this->m_dstsybox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.lineend.x);
	this->m_lineexbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.lineend.y);
	this->m_lineeybox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.linestart.x);
	this->m_linesxbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.linestart.y);
	this->m_linesybox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.srcstart.x);
	this->m_srcsxbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.srcstart.y);
	this->m_srcsybox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.font_st.x);
	this->m_fontsxbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.font_st.y);
	this->m_fontsybox.SetWindowText(str);
}

void CV568GeDlg::AdjustScrollBar(void)
{
	TSize *psize = NULL, size, fontsize;

	if(this->m_option.dstbsel == GE_B0)
		psize = &(this->m_option.b0size);
	else
		psize = &(this->m_option.b1size);

	this->m_clipsxbar.SetScrollRange(0, psize->cx - this->m_option.clip.rbp.x - 1);
	this->m_clipsxbar.SetScrollPos(this->m_option.clip.ltp.x);
	this->m_clipsybar.SetScrollRange(0, psize->cy - this->m_option.clip.rbp.y - 1);
	this->m_clipsybar.SetScrollPos(this->m_option.clip.ltp.y);
	this->m_clipexbar.SetScrollRange(this->m_option.clip.ltp.x, psize->cx - 1);
	this->m_clipexbar.SetScrollPos(this->m_option.clip.rbp.x);
	this->m_clipeybar.SetScrollRange(this->m_option.clip.ltp.y, psize->cy - 1);
	this->m_clipeybar.SetScrollPos(this->m_option.clip.rbp.y);

	this->m_linesxbar.SetScrollRange(0, psize->cx - 1);
	this->m_linesxbar.SetScrollPos(this->m_option.linestart.x);
	this->m_linesybar.SetScrollRange(0, psize->cy - 1);
	this->m_linesybar.SetScrollPos(this->m_option.linestart.y);
	this->m_lineexbar.SetScrollRange(0, psize->cx - 1);
	this->m_lineexbar.SetScrollPos(this->m_option.lineend.x);
	this->m_lineeybar.SetScrollRange(0, psize->cy - 1);
	this->m_lineeybar.SetScrollPos(this->m_option.lineend.y);


	this->m_dstsxbar.SetScrollRange(0, psize->cx - this->m_option.dstsize.cx);
	this->m_dstsxbar.SetScrollPos(this->m_option.dststart.x);
	this->m_dstsybar.SetScrollRange(0, psize->cy - this->m_option.dstsize.cy);
	this->m_dstsybar.SetScrollPos(this->m_option.dststart.y);
	if(this->m_option.srcbsel == this->m_option.dstbsel)
	{
		this->m_srcsxbar.SetScrollRange(0, psize->cx - this->m_option.dstsize.cx);
		this->m_srcsybar.SetScrollRange(0, psize->cy - this->m_option.dstsize.cy);
		this->m_dstwidthbar.SetScrollRange(0, psize->cx);
		this->m_dstwidthbar.SetScrollPos(this->m_option.dstsize.cx);
		this->m_dstheightbar.SetScrollRange(0, psize->cy);
		this->m_dstheightbar.SetScrollPos(this->m_option.dstsize.cy);
	}
	else if(this->m_option.srcbsel == GE_FONT)
	{
		this->m_srcsxbar.SetScrollRange(0, 0);
		this->m_srcsybar.SetScrollRange(0x200, 0x7f0);
		this->m_dstwidthbar.SetScrollRange(0, psize->cx);
		this->m_dstwidthbar.SetScrollPos(this->m_option.dstsize.cx);
		this->m_dstheightbar.SetScrollRange(0, psize->cy);
		this->m_dstheightbar.SetScrollPos(this->m_option.dstsize.cy);
	}
	else
	{
		size.cx = (this->m_option.b0size.cx > this->m_option.b1size.cx) ? 
			this->m_option.b1size.cx : this->m_option.b0size.cx;
		size.cy = (this->m_option.b0size.cy > this->m_option.b1size.cy) ? 
			this->m_option.b1size.cy : this->m_option.b0size.cy;
		this->m_dstwidthbar.SetScrollRange(0, size.cx);
		this->m_dstwidthbar.SetScrollPos(this->m_option.dstsize.cx);
		this->m_dstheightbar.SetScrollRange(0, size.cy);
		this->m_dstheightbar.SetScrollPos(this->m_option.dstsize.cy);
		this->m_srcsxbar.SetScrollRange(0, psize->cx - this->m_option.dstsize.cx);
		this->m_srcsybar.SetScrollRange(0, psize->cy - this->m_option.dstsize.cy);

	}
	this->m_srcsxbar.SetScrollPos(this->m_option.srcstart.x);
	this->m_srcsybar.SetScrollPos(this->m_option.srcstart.y);

	V5_DispGetCurFontSize(&fontsize);
	this->m_fontsxbar.SetScrollRange(0, psize->cx - fontsize.cx);
	this->m_fontsxbar.SetScrollPos(this->m_option.font_st.x);
	this->m_fontsybar.SetScrollRange(0, psize->cy - fontsize.cy);
	this->m_fontsybar.SetScrollPos(this->m_option.font_st.y);

	this->GetBarValue();
	this->DispBarEditBox();
}

void CV568GeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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

/********************************************* EditBox Section *********************************************/

void CV568GeDlg::GetEditboxValue(void)
{
	char str[20];

	this->m_bitwisebox.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.bitwise));
	this->m_linecolorbox.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.linecolor));
	this->m_srcexpbgbox.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.srcexpbg));
	this->m_srcexpfgbox.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.srcexpfg));
	this->m_ptnsrcbgbox.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.ptnsrcbg));
	this->m_ptnsrcfgbox.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.ptnsrcfg));
}

void CV568GeDlg::RefreshEditBox(void)
{
	this->m_option.bitwise   = V5B_GeGetBitWise();
	this->m_option.linecolor = V5B_GeGetSrcLineColor();

	this->m_option.dstspan   = V5B_GeGetDstSpan();
	this->m_option.srcspan   = V5B_GeGetSrcSpan();

	this->m_option.ptnsrcbg  = V5B_GeGetPtnBkColor();
	this->m_option.ptnsrcfg  = V5B_GeGetPtnForeColor();
	this->m_option.srcexpbg  = V5B_GeGetSrcBkColor();
	this->m_option.srcexpfg  = V5B_GeGetSrcForeColor();
}

void CV568GeDlg::DispEditBox(void)
{
	char str[20];

	sprintf(str, "%x", this->m_option.bitwise);
	this->m_bitwisebox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dstspan);
	this->m_dstspanbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.linecolor);
	this->m_linecolorbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.ptnsrcbg);
	this->m_ptnsrcbgbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.ptnsrcfg);
	this->m_ptnsrcfgbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.srcexpbg);
	this->m_srcexpbgbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.srcexpfg);
	this->m_srcexpfgbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.srcspan);
	this->m_srcspanbox.SetWindowText(str);
}

/********************************************** Button Section *********************************************/

void CV568GeDlg::OnBnClickedRefreshButton()
{
	CWaitCursor cursor;
	TBLayerProty proty;

	if(this->m_option.srcbsel == GE_FONT)
		V5_PreDispFont((UINT8)this->m_option.dstbsel);
	else
		V5_GeAutoSetProp();

	V5_DispGetB0Proty(&proty);
	this->m_option.b0size = proty.memsize;
	V5_DispGetB1Proty(&proty);
	this->m_option.b1size = proty.memsize;

	this->RefreshData();
}

void CV568GeDlg::OnBnClickedDrawlineButton()
{
	this->GetUserOption();
	V5_GeDrawLine(this->m_option.linestart, this->m_option.lineend, this->m_option.linecolor);
	V5_DispUpdateB0Win(NULL);
}

void CV568GeDlg::OnBnClickedBitbltButton()
{
	this->GetUserOption();

	if(this->m_option.srcbsel == GE_FONT)
		V5_PreDispFont((UINT8)this->m_option.dstbsel);
	else
		V5_GeAutoSetProp();
	V5_GeBitblt(this->m_option.srcstart, this->m_option.dststart, this->m_option.dstsize, (UINT8)this->m_option.bitwise);
	if(this->m_option.dstbsel == GE_B0)
		V5_DispUpdateB0Win(NULL);
	else
		V5_DispUpdateB1Win(NULL);
}

void CV568GeDlg::OnBnClickedClipButton()
{
	this->GetUserOption();
	V5_GeClip(this->m_option.clip);
	V5_DispUpdateB0Win(NULL);
}

void CV568GeDlg::OnBnClickedWriteptnButton()
{
	CFileBin fb;
	CBufferBase pool;
	int size = 0;

	this->GetUserOption();
	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	if(size == 0)
		return;
	size = (size > 256) ? 256 : size;

	fb.Read(pool.m_buf, size);
	V5B_GeSetPattern((UINT8)(size >> 2), (UINT32 *)pool.m_buf);
}

void CV568GeDlg::OnBnClickedSetcolorButton()
{
	this->GetUserOption();

	V5B_GeSetPtnBkColor(this->m_option.ptnsrcbg);
	V5B_GeSetPtnForeColor(this->m_option.ptnsrcfg);
	V5B_GeSetSrcBkColor(this->m_option.srcexpbg);
	V5B_GeSetSrcForeColor(this->m_option.srcexpfg);
}

void CV568GeDlg::OnBnClickedBtnLoadfont()
{
	V5_GraphInit();
	V5_FontInit(NULL);
}

void CV568GeDlg::OnBnClickedPredispButton()
{
	V5_PreDispFont(GE_B0);
}

void CV568GeDlg::OnBnClickedDispButton()
{
	char str[20];

	this->m_charbox.GetWindowText(str, 20);
	V5_DisplayChar(this->m_option.font_st, (UINT8)str[0], (UINT8)this->m_option.font_option);
}

void CV568GeDlg::OnBnClickedDispStrButton()
{
	char str[100];

	this->m_strbox.GetWindowText(str, 100);
	V5_DisplayStr(this->m_option.font_st, (UINT8 *)str, (UINT8)this->m_option.font_option);
}

#endif //V578CMD_DISABLE
