#ifndef __V568PLANE_V568LCDIFDLG_H__
#define __V568PLANE_V568LCDIFDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "tools/global.h"
#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MDecEdit.h"
#include "BaseClass/MHexEdit.h"


// CV568LcdifDlg 对话框
#define LCDIF_MAX_HEAD_NUM		0x10


enum LCDIFCFGenum {
	HEAD_EN = 0x01,
	RSHIGH_EN = 0x02,
	DMAP_CONV = 0x04,
	BIAS_DIVX = 0x08,
	BIAS_DIVY = 0x10
};

typedef struct tag_TLcdifOption
{
	char srcfile[DEFAULT_FILENAME_LEN];
	char dstfile[DEFAULT_FILENAME_LEN];
	// EditBox
	int  csflag;
	int  rsflag;
	int  startxpos;
	int  startypos;
	int  endxpos;
	int  endypos;
	int  headdata[LCDIF_MAX_HEAD_NUM];
	int  line_data;
	int  procflg;
	int  cmdsx;
	int  cmdex;
	int  cmdsy;
	int  cmdey;
	int  rptsx;
	int  rptex;
	int  rptsy;
	int  rptey;
	// Check Box
	int  config;
	// Combo Box
	int  csmode;
	int  headnum;
	int  datafmt;
	int  mapptn;
	int  cssel;
	int  bustype;
	int  panelsel;
	// ScrollBar
	int  wsc;
	int  wrc;
	int  dhc;
	int  rsc;
	int  rac;
	int  roc;
	TPoint  biaspt;
	TPoint	line_sp;
	int  line_ex;
} TLcdifOption, *PTLcdifOption;

class V568PLANE_DLL CV568LcdifDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568LcdifDlg)

public:
	CV568LcdifDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568LcdifDlg();

// 对话框数据
	enum { IDD = IDD_V568LCDIF_DLG };

protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
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
	TLcdifOption m_option;

	// Check Box
	CButton		 m_headenbtn;
	CButton		 m_rshighbtn;
	CButton		 m_dmapconvbtn;
	CButton		 m_biasdivxbtn;
	CButton		 m_biasdivybtn;
	CButton		 m_commcode;
	CButton		 m_headrpt;

	// Combo Box
	CMComboBox	 m_csmodebox;
	CMComboBox	 m_headnumbox;
	CMComboBox	 m_datafmt;
	CMComboBox	 m_mapptn;
	CMComboBox	 m_csselbox;
	CMComboBox	 m_bustypebox;
	CMComboBox	 m_panelselbox;

	// ScrollBar
	CScrollBar	 m_wscbar;
	CScrollBar	 m_wrcbar;
	CScrollBar	 m_dhcbar;
	CScrollBar	 m_rscbar;
	CScrollBar	 m_racbar;
	CScrollBar	 m_rocbar;
	CScrollBar	 m_biasxbar;
	CScrollBar	 m_biasybar;
	CScrollBar	 m_linesxbar;
	CScrollBar	 m_linesybar;
	CScrollBar	 m_lineexbar;

	// ScrollBar EditBox
	CMHexEdit	 m_wsc;
	CMHexEdit	 m_wrc;
	CMHexEdit	 m_dhc;
	CMHexEdit	 m_rsc;
	CMHexEdit	 m_rac;
	CMHexEdit	 m_roc;
	CMHexEdit	 m_biasx;
	CMHexEdit	 m_biasy;
	CMHexEdit	 m_linesx;
	CMHexEdit	 m_linesy;
	CMHexEdit	 m_lineex;

	// EditBox
	CMHexEdit	 m_csflag;
	CMHexEdit	 m_rsflag;
	CMHexEdit	 m_startx_pos;
	CMHexEdit	 m_starty_pos;
	CMHexEdit	 m_endx_pos;
	CMHexEdit	 m_endy_pos;

	CMHexEdit	 m_headdata0;
	CMHexEdit	 m_headdata1;
	CMHexEdit	 m_headdata2;
	CMHexEdit	 m_headdata3;
	CMHexEdit	 m_headdata4;
	CMHexEdit	 m_headdata5;
	CMHexEdit	 m_headdata6;
	CMHexEdit	 m_headdata7;
	CMHexEdit	 m_headdata8;
	CMHexEdit	 m_headdata9;
	CMHexEdit	 m_headdata10;
	CMHexEdit	 m_headdata11;
	CMHexEdit	 m_headdata12;
	CMHexEdit	 m_headdata13;
	CMHexEdit	 m_headdata14;
	CMHexEdit	 m_headdata15;

	CMHexEdit	 m_linedata;

	CMHexEdit	 m_cmdsx;
	CMHexEdit	 m_cmdex;
	CMHexEdit	 m_cmdsy;
	CMHexEdit	 m_cmdey;
	CMHexEdit	 m_rptsx;
	CMHexEdit	 m_rptex;
	CMHexEdit	 m_rptsy;
	CMHexEdit	 m_rptey;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// Check Box
	afx_msg void OnBnClickedHeadenCheck();
	afx_msg void OnBnClickedRshenCheck();
	afx_msg void OnBnClickedDmapconvCheck();
	afx_msg void OnBnClickedBiasdivxCheck();
	afx_msg void OnBnClickedBiasdivyCheck();
	afx_msg void OnBnClickedCommCodeEnCheck();
	afx_msg void OnBnClickedHeadRptEnCheck();

	// Combo Box
	afx_msg void OnCbnSelchangeHeadnumCombo();
	afx_msg void OnCbnSelchangeDatafmtCombo();
	afx_msg void OnCbnSelchangeMapptnCombo();
	afx_msg void OnCbnSelchangeCsselCombo();
	afx_msg void OnCbnSelchangeBustypeCombo();
	afx_msg void OnCbnSelchangePanelselCombo();
	afx_msg void OnCbnSelchangeCsmodeCombo();

	// Button
	afx_msg void OnBnClickedInitpanelButton();
	afx_msg void OnBnClickedDrawimageButton();
	afx_msg void OnBnClickedReadpanelButton();
	afx_msg void OnBnClickedUpdateButton();
	afx_msg void OnBnClickedSetcfgButton();
	afx_msg void OnBnClickedRestpanelButton();
	afx_msg void OnBnClickedLinestartButton();
};

#endif
