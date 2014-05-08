#ifndef __V568PLANE_V568LCDCDLG_H__
#define __V568PLANE_V568LCDCDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "tools/global.h"
#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MHexEdit.h"

// CV568LcdcDlg 对话框

enum LCDCCFGenum
{
	CFG_LA_OP_EN	= 0x0001,
	CFG_LA_DISP_EN	= 0x0002,
	CFG_BG_EN		= 0x0004,
	CFG_LB0_EN		= 0x0008,
	CFG_LB0_PRI		= 0x0010,
	CFG_LB1_EN		= 0x0020,
	CFG_LB1_PRI		= 0x0040,
	CFG_BLEND_EN	= 0x0080,
	CFG_OVERLAY_EN	= 0x0100,
	CFG_DITHER_EN	= 0x0200,
	CFG_GAMMA_EN	= 0x0400,
	CFG_LB0_OKCM	= 0x0800,
	CFG_LB1_OKCM	= 0x1000
};

enum LCDCGAMMATABLE
{
	R_TABLE,
	G_TABLE,
	B_TABLE
};

typedef struct tag_TLcdcOption
{
	char srcfile[DEFAULT_FILENAME_LEN];
	char dstfile[DEFAULT_FILENAME_LEN];
	TSize			panelsize;
	TSize			ippdispsize;
	int				laybfmt[2];
	//////////////////////////////////
	unsigned int	config;
	int				gamma_sel;
	int				overlap;
	int				arotmode;
	int				brotmode;
	int				bfmt;
	int				dithersel;
	int				b0olymode;
	int				b1olymode;
	//////////////////////////////////
	TBLayerProty	layerb[2];
	TALayerProty	layera;
	TPoint			bgpt;
	TSize			bgsize;
	int				blend[2];
	int				kcolor[2];
	int				bgcolor;

	int				dedly;
	int				deintv;
	int				lineintv;

	int				brightness;
	int				contrast;

	int				wsc;
	int				wrc;
	int				dhc;
	//////////////////////////////////
	int		gamma[17];
}TLcdcOption, *PTLcdcOption;

class V568PLANE_DLL CV568LcdcDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568LcdcDlg)

public:
	CV568LcdcDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568LcdcDlg();

// 对话框数据
	enum { IDD = IDD_V568LCDC_DLG };

protected:
	void InitUserOption(void);
	void DispBarEditBox(void);
	void DispOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void AdjustScrollBar(void);
	void GetBarValue(void);
	void GetEditboxValue(void);
	void GetComboValue(char *tail = FILE_TAIL_BIN);

private:
	TLcdcOption	m_option;

	// Check Box
	CButton		m_adisp_en;
	CButton		m_aoperation_en;
	CButton		m_b0_en;
	CButton		m_b0_pri;
	CButton		m_b1_en;
	CButton		m_b1_pri;
	CButton		m_blend_en;
	CButton		m_overlay_en;
	CButton		m_dither_en;
	CButton		m_gamma_en;
	CButton		m_b0okcm;
	CButton		m_b1okcm;

	// Combo Box
	CMComboBox	 m_gammaselbox;
	CMComboBox	 m_overlapbox;
	CMComboBox	 m_arotbox;
	CMComboBox	 m_brotbox;
	CMComboBox	 m_bfmtbox;
	CMComboBox	 m_ditherselbox;
	CMComboBox	 m_b0olybox;
	CMComboBox	 m_b1olybox;

	// ScrollBar
	CScrollBar	 m_bx0bar;
	CScrollBar	 m_by0bar;
	CScrollBar	 m_bw0bar;
	CScrollBar	 m_bh0bar;
	CScrollBar	 m_bdx0bar;
	CScrollBar	 m_bdy0bar;
	CScrollBar	 m_bmw0bar;
	CScrollBar	 m_bmh0bar;

	CScrollBar	 m_brx0bar;
	CScrollBar	 m_bry0bar;
	CScrollBar	 m_brw0bar;
	CScrollBar	 m_brh0bar;

	CScrollBar	 m_bx1bar;
	CScrollBar	 m_by1bar;
	CScrollBar	 m_bw1bar;
	CScrollBar	 m_bh1bar;
	CScrollBar	 m_bdx1bar;
	CScrollBar	 m_bdy1bar;
	CScrollBar	 m_bmw1bar;
	CScrollBar	 m_bmh1bar;

	CScrollBar	 m_brx1bar;
	CScrollBar	 m_bry1bar;
	CScrollBar	 m_brw1bar;
	CScrollBar	 m_brh1bar;

	CScrollBar	 m_axbar;
	CScrollBar	 m_aybar;
	CScrollBar	 m_awbar;
	CScrollBar	 m_ahbar;
	CScrollBar	 m_adxbar;
	CScrollBar	 m_adybar;
	CScrollBar	 m_amwbar;
	CScrollBar	 m_amhbar;

	CScrollBar	 m_blend0bar;
	CScrollBar	 m_blend1bar;

	CScrollBar	 m_bgxbar;
	CScrollBar	 m_bgybar;
	CScrollBar	 m_bgwbar;
	CScrollBar	 m_bghbar;

	CScrollBar	 m_dedlybar;
	CScrollBar	 m_deintvbar;
	CScrollBar	 m_lineintvbar;

	CScrollBar	 m_brightnessbar;
	CScrollBar	 m_contrastbar;

	CScrollBar	 m_wscbar;
	CScrollBar	 m_wrcbar;
	CScrollBar	 m_dhcbar;

	// ScrollBar EditBox
	CMHexEdit	 m_bx0;
	CMHexEdit	 m_by0;
	CMHexEdit	 m_bw0;
	CMHexEdit	 m_bh0;
	CMHexEdit	 m_bdx0;
	CMHexEdit	 m_bdy0;
	CMHexEdit	 m_bmw0;
	CMHexEdit	 m_bmh0;

	CMHexEdit	 m_brx0;
	CMHexEdit	 m_bry0;
	CMHexEdit	 m_brw0;
	CMHexEdit	 m_brh0;

	CMHexEdit	 m_bx1;
	CMHexEdit	 m_by1;
	CMHexEdit	 m_bw1;
	CMHexEdit	 m_bh1;
	CMHexEdit	 m_bdx1;
	CMHexEdit	 m_bdy1;
	CMHexEdit	 m_bmw1;
	CMHexEdit	 m_bmh1;

	CMHexEdit	 m_brx1;
	CMHexEdit	 m_bry1;
	CMHexEdit	 m_brw1;
	CMHexEdit	 m_brh1;

	CMHexEdit	 m_ax;
	CMHexEdit	 m_ay;
	CMHexEdit	 m_aw;
	CMHexEdit	 m_ah;
	CMHexEdit	 m_adx;
	CMHexEdit	 m_ady;
	CMHexEdit	 m_amw;
	CMHexEdit	 m_amh;

	CMHexEdit	 m_blend0;
	CMHexEdit	 m_blend1;

	CMHexEdit	 m_bgx;
	CMHexEdit	 m_bgy;
	CMHexEdit	 m_bgw;
	CMHexEdit	 m_bgh;

	CMHexEdit	 m_dedly;
	CMHexEdit	 m_deintv;
	CMHexEdit	 m_lineintv;

	CMHexEdit	 m_brightness;
	CMHexEdit	 m_contrast;

	CMHexEdit	 m_wsc;
	CMHexEdit	 m_wrc;
	CMHexEdit	 m_dhc;

	// EditBox
	CMHexEdit	 m_kcolor0;
	CMHexEdit	 m_kcolor1;
	CMHexEdit	 m_bgcolor;

	CMHexEdit	 m_gammaval00;
	CMHexEdit	 m_gammaval01;
	CMHexEdit	 m_gammaval02;
	CMHexEdit	 m_gammaval03;
	CMHexEdit	 m_gammaval04;
	CMHexEdit	 m_gammaval05;
	CMHexEdit	 m_gammaval06;
	CMHexEdit	 m_gammaval07;
	CMHexEdit	 m_gammaval08;
	CMHexEdit	 m_gammaval09;
	CMHexEdit	 m_gammaval10;
	CMHexEdit	 m_gammaval11;
	CMHexEdit	 m_gammaval12;
	CMHexEdit	 m_gammaval13;
	CMHexEdit	 m_gammaval14;
	CMHexEdit	 m_gammaval15;
	CMHexEdit	 m_gammaval16;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

protected:
	void DispSpeedInfo(void);

protected:
	void InitCombo(void);
	void RefreshCheckBox(void);
	void RefreshCombo(void);
	void RefreshBar(void);
	void RefreshEditBox(void);
	void DispCheckBox(void);
	void DispCombo(void);
	void DispEditBox(void);

public:
	// Check Box
	afx_msg void OnBnClickedLayeraopCheck();
	afx_msg void OnBnClickedLayeradispCheck();
	afx_msg void OnBnClickedB0enableCheck();
	afx_msg void OnBnClickedB0firstCheck();
	afx_msg void OnBnClickedB1enableCheck();
	afx_msg void OnBnClickedB1firstCheck();
	afx_msg void OnBnClickedBlendenCheck();
	afx_msg void OnBnClickedOverlayenCheck();
	afx_msg void OnBnClickedDitherenCheck();
	afx_msg void OnBnClickedGammaenCheck();
	afx_msg void OnBnClickedB0okcmCheck();
	afx_msg void OnBnClickedB1okcmCheck();

	// Combo Box
	afx_msg void OnCbnSelchangeGammaselCombo();
	afx_msg void OnCbnSelchangeOverlapCombo();
	afx_msg void OnCbnSelchangeArotationCombo();
	afx_msg void OnCbnSelchangeBrotationCombo();
	afx_msg void OnCbnSelchangeLbfmtCombo();
	afx_msg void OnCbnSelchangeDitherselCombo();
	afx_msg void OnCbnSelchangeB0overlaymodeCombo();
	afx_msg void OnCbnSelchangeB1overlaymodeCombo();

	// Button
	afx_msg void OnBnClickedRefreshButton();
	afx_msg void OnBnClickedSetButton();
	afx_msg void OnBnClickedLcdcrstButton();
	afx_msg void OnBnClickedTestButton();
	afx_msg void OnBnClickedSetAButton();
	afx_msg void OnBnClickedSetB0Button();
	afx_msg void OnBnClickedUpdateB0Button();
	afx_msg void OnBnClickedSetB1Button();
	afx_msg void OnBnClickedUpdateB1Button();
	afx_msg void OnBnClickedSetBgButton();
	afx_msg void OnBnClickedUpdateBgButton();
	afx_msg void OnBnClickedKcolor0Button();
	afx_msg void OnBnClickedKcolor1Button();
	afx_msg void OnBnClickedBgColorButton();
};

#endif
