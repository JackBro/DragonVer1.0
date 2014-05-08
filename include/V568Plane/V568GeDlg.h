#ifndef __V568PLANE_V568GEDLG_H__
#define __V568PLANE_V568GEDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MHexEdit.h"

// CV558DemoDlg 对话框
typedef struct tag_TGeOption
{
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	TSize	b0size;
	TSize	b1size;
	// Check Box
	int		opmode0;
	int		opmode1;
	// Combo Box
	int		srcbsel;
	int		transel;
	int		dstbsel;
	int		font_option;
	// ScrollBar
	TPoint	linestart;
	TPoint	lineend;
	TPoint	dststart;
	TSize	dstsize;
	TPoint	srcstart;
	TClip	clip;
	TPoint	font_st;
	// EditBox
	int		bitwise;
	int		dstspan;
	int		linecolor;
	int		ptnsrcbg;
	int		ptnsrcfg;
	int		srcexpbg;
	int		srcexpfg;
	int		srcspan;
} TGeOption, *PTGeOption;

// CV568GeDlg 对话框

class V568PLANE_DLL CV568GeDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568GeDlg)

public:
	CV568GeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568GeDlg();

// 对话框数据
	enum { IDD = IDD_V568GE_DLG };

protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void RefreshData(void);
	void DispOption(void);
	void InitCombo(void);
	void GetBarValue(void);
	void GetEditboxValue(void);
	void GetComboValue(char *tail = FILE_TAIL_BIN);
	void RefreshCheckBox(void);
	void RefreshCombo(void);
	void RefreshBar(void);
	void RefreshEditBox(void);
	void DispCheckBox(void);
	void DispCombo(void);
	void DispEditBox(void);
	void AdjustScrollBar(void);
	void DispBarEditBox(void);

private:
	TGeOption	m_option;

	// Check Box
	CButton		m_monoptnbtn;
	CButton		m_monosrcbtn;
	CButton		m_monosrcmaskbtn;
	CButton		m_monoptnmaskbtn;
	CButton		m_transpbtn;
	CButton		m_srcexpbtn;

	// Combo Box
	CMComboBox	m_destbselbox;
	CMComboBox	m_transelbox;
	CMComboBox	m_srcbselbox;
	CMComboBox	m_fontoptionbox;

	// ScrollBar
	CScrollBar	m_linesxbar;
	CScrollBar	m_linesybar;
	CScrollBar	m_lineexbar;
	CScrollBar	m_lineeybar;
	CScrollBar	m_dstsxbar;
	CScrollBar	m_dstsybar;
	CScrollBar	m_dstwidthbar;
	CScrollBar	m_dstheightbar;
	CScrollBar	m_srcsxbar;
	CScrollBar	m_srcsybar;
	CScrollBar	m_clipsxbar;
	CScrollBar	m_clipsybar;
	CScrollBar	m_clipexbar;
	CScrollBar	m_clipeybar;
	CScrollBar	m_fontsxbar;
	CScrollBar	m_fontsybar;

	// ScrollBar EditBox
	CMHexEdit	m_linesxbox;
	CMHexEdit	m_linesybox;
	CMHexEdit	m_lineexbox;
	CMHexEdit	m_lineeybox;
	CMHexEdit	m_dstsxbox;
	CMHexEdit	m_dstsybox;
	CMHexEdit	m_dstwidthbox;
	CMHexEdit	m_dstheightbox;
	CMHexEdit	m_srcexpbgbox;
	CMHexEdit	m_srcexpfgbox;
	CMHexEdit	m_clipsxbox;
	CMHexEdit	m_clipsybox;
	CMHexEdit	m_clipexbox;
	CMHexEdit	m_clipeybox;
	CMHexEdit	m_fontsxbox;
	CMHexEdit	m_fontsybox;

	// EditBox
	CEdit		m_charbox;
	CEdit		m_strbox;
	CMHexEdit	m_dstspanbox;
	CMHexEdit	m_bitwisebox;
	CMHexEdit	m_linecolorbox;
	CMHexEdit	m_srcsxbox;
	CMHexEdit	m_srcsybox;
	CMHexEdit	m_ptnsrcbgbox;
	CMHexEdit	m_ptnsrcfgbox;
	CMHexEdit	m_srcspanbox;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// Check Box
	afx_msg void OnBnClickedSrcwrmaskCheck();
	afx_msg void OnBnClickedSrcextendCheck();
	afx_msg void OnBnClickedMonoptnCheck();
	afx_msg void OnBnClickedPtnwrmaskCheck();
	afx_msg void OnBnClickedTransparentCheck();

	// Combo Box
	afx_msg void OnCbnSelchangeDstblselCombo();
	afx_msg void OnCbnSelchangeSrcbselCombo();
	afx_msg void OnCbnSelchangeTranselCombo();
	afx_msg void OnCbnSelchangeFontoptionCombo();

	// Button
	afx_msg void OnBnClickedRefreshButton();
	afx_msg void OnBnClickedDrawlineButton();
	afx_msg void OnBnClickedBitbltButton();
	afx_msg void OnBnClickedClipButton();
	afx_msg void OnBnClickedWriteptnButton();
	afx_msg void OnBnClickedSetcolorButton();
	afx_msg void OnBnClickedBtnLoadfont();
	afx_msg void OnBnClickedPredispButton();
	afx_msg void OnBnClickedDispButton();
	afx_msg void OnBnClickedDispStrButton();
};

#endif
