#ifndef __V568PLANE_V568IPPDLG_H__
#define __V568PLANE_V568IPPDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "tools/global.h"
#include "BasicFunc/BaseCtrlDlg.h"


// CV568IppDlg 对话框

#define	FILTER_BUF_SIZE			120


typedef struct tag_TIppOption
{
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	// EditBox
	int		ddrop;
	int		cdrop;
	int		hist_q;
	TSize	imgsz;
	TV5BIppSzDnFct	tmhsfct;
	TV5BIppSzDnFct	tmvsfct;
	TV5BIppSzUpFct	cpuhsfct;
	TV5BIppSzUpFct	cpuvsfct;
	TV5BIppSzDnFct	cpdhsfct;
	TV5BIppSzDnFct	cpdvsfct;
	TV5BIppSzDnFct	dphsfct;
	TV5BIppSzDnFct	dpvsfct;
	THistStat		histstat;
	// Check Box
	int		spe_en;
	int		ctrl0;
	int		ctrl1;
	// Combo Box
	int		workmode;
	int		ippsrc;
	int		tmbsrc;
	int		spetype;
	int		histsel;
	int		thumfmt;
	// ScrollBar
	int		uoff;
	int		voff;
	int		utop;
	int		ubtm;
	int		vtop;
	int		vbtm;
	TPoint	winpt;
	TSize	winsz;
	TSize	thmsz;
	TSize	capsz;
	TSize	dspsz;
} TIppOption, *PTIppOption;


class V568PLANE_DLL CV568IppDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568IppDlg)

public:
	CV568IppDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568IppDlg();

// 对话框数据
	enum { IDD = IDD_V568IPP_DLG };

private:
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
	TIppOption	m_option;

	// Check Box
	CButton		m_spe_en;
	CButton		m_frame_en;
	CButton		m_tms_en;
	CButton		m_dps_en;
	CButton		m_cpus_en;
	CButton		m_cpds_en;
	CButton		m_tmhsbp_en;
	CButton		m_tmvsbp_en;
	CButton		m_dphsbp_en;
	CButton		m_dpvsbp_en;
	CButton		m_cpuhsbp_en;
	CButton		m_cpuvsbp_en;
	CButton		m_cpdhsbp_en;
	CButton		m_cpdvsbp_en;

	// Combo Box
	CMComboBox	 m_modebox;
	CMComboBox	 m_datasrcbox;
	CMComboBox	 m_tmbsrcbox;
	CMComboBox	 m_spebox;
	CMComboBox	 m_histselbox;
	CMComboBox	 m_thumfmtbox;

	// ScrollBar
	CScrollBar	 m_uoffbar;
	CScrollBar	 m_voffbar;
	CScrollBar	 m_utopbar;
	CScrollBar	 m_ubtmbar;
	CScrollBar	 m_vtopbar;
	CScrollBar	 m_vbtmbar;

	CScrollBar	 m_winxbar;
	CScrollBar	 m_winybar;
	CScrollBar	 m_winwbar;
	CScrollBar	 m_winhbar;

	CScrollBar	 m_tmswbar;
	CScrollBar	 m_tmshbar;
	CScrollBar	 m_cpswbar;
	CScrollBar	 m_cpshbar;
	CScrollBar	 m_dpswbar;
	CScrollBar	 m_dpshbar;

	// ScrollBar EditBox
	CMHexEdit	 m_uoff;
	CMHexEdit	 m_voff;
	CMHexEdit	 m_utop;
	CMHexEdit	 m_ubtm;
	CMHexEdit	 m_vtop;
	CMHexEdit	 m_vbtm;

	CMHexEdit	 m_winx;
	CMHexEdit	 m_winy;
	CMHexEdit	 m_winw;
	CMHexEdit	 m_winh;

	CMHexEdit	 m_tmsw;
	CMHexEdit	 m_tmsh;
	CMHexEdit	 m_cpsw;
	CMHexEdit	 m_cpsh;
	CMHexEdit	 m_dpsw;
	CMHexEdit	 m_dpsh;

	// EditBox
	CMHexEdit	 m_imgw;
	CMHexEdit	 m_imgh;

	CMHexEdit	 m_dispdrop;
	CMHexEdit	 m_captdrop;

	CMHexEdit	 m_tms_hw1;
	CMHexEdit	 m_tms_hw2;
	CMHexEdit	 m_tms_hs1;
	CMHexEdit	 m_tms_hs2;
	CMHexEdit	 m_tms_hc;
	CMHexEdit	 m_tms_vh1;
	CMHexEdit	 m_tms_vh2;
	CMHexEdit	 m_tms_vs1;
	CMHexEdit	 m_tms_vs2;
	CMHexEdit	 m_tms_vc;

	CMHexEdit	 m_cpds_hw1;
	CMHexEdit	 m_cpds_hw2;
	CMHexEdit	 m_cpds_hs1;
	CMHexEdit	 m_cpds_hs2;
	CMHexEdit	 m_cpds_hc;
	CMHexEdit	 m_cpds_vh1;
	CMHexEdit	 m_cpds_vh2;
	CMHexEdit	 m_cpds_vs1;
	CMHexEdit	 m_cpds_vs2;
	CMHexEdit	 m_cpds_vc;
	CMHexEdit	 m_cpus_hw1;
	CMHexEdit	 m_cpus_hw2;
	CMHexEdit	 m_cpus_hs;
	CMHexEdit	 m_cpus_vh1;
	CMHexEdit	 m_cpus_vh2;
	CMHexEdit	 m_cpus_vs;

	CMHexEdit	 m_dps_hw1;
	CMHexEdit	 m_dps_hw2;
	CMHexEdit	 m_dps_hs1;
	CMHexEdit	 m_dps_hs2;
	CMHexEdit	 m_dps_hc;
	CMHexEdit	 m_dps_vh1;
	CMHexEdit	 m_dps_vh2;
	CMHexEdit	 m_dps_vs1;
	CMHexEdit	 m_dps_vs2;
	CMHexEdit	 m_dps_vc;

	CMHexEdit	 m_hist_q;
	CMHexEdit	 m_hist_xcur;
	CMHexEdit	 m_hist_xavg;

	CMHexEdit	 m_hist_r00;
	CMHexEdit	 m_hist_r08;
	CMHexEdit	 m_hist_r10;
	CMHexEdit	 m_hist_r18;
	CMHexEdit	 m_hist_r20;
	CMHexEdit	 m_hist_r28;
	CMHexEdit	 m_hist_r30;
	CMHexEdit	 m_hist_r38;
	CMHexEdit	 m_hist_r40;
	CMHexEdit	 m_hist_r48;
	CMHexEdit	 m_hist_r50;
	CMHexEdit	 m_hist_r58;
	CMHexEdit	 m_hist_r60;
	CMHexEdit	 m_hist_r68;
	CMHexEdit	 m_hist_r70;
	CMHexEdit	 m_hist_r78;
	CMHexEdit	 m_hist_r80;
	CMHexEdit	 m_hist_r88;
	CMHexEdit	 m_hist_r90;
	CMHexEdit	 m_hist_r98;
	CMHexEdit	 m_hist_ra0;
	CMHexEdit	 m_hist_ra8;
	CMHexEdit	 m_hist_rb0;
	CMHexEdit	 m_hist_rb8;
	CMHexEdit	 m_hist_rc0;
	CMHexEdit	 m_hist_rc8;
	CMHexEdit	 m_hist_rd0;
	CMHexEdit	 m_hist_rd8;
	CMHexEdit	 m_hist_re0;
	CMHexEdit	 m_hist_re8;
	CMHexEdit	 m_hist_rf0;
	CMHexEdit	 m_hist_rf8;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// Check Box
	afx_msg void OnBnClickedSpecialEnCheck();
	afx_msg void OnBnClickedFrameEnCheck();
	afx_msg void OnBnClickedTmsEnCheck();
	afx_msg void OnBnClickedDpsEnCheck();
	afx_msg void OnBnClickedCpusEnCheck();
	afx_msg void OnBnClickedCpdsEnCheck();
	afx_msg void OnBnClickedTmhBpEnCheck();
	afx_msg void OnBnClickedTmvBpEnCheck();
	afx_msg void OnBnClickedDphBpEnCheck();
	afx_msg void OnBnClickedDpvBpEnCheck();
	afx_msg void OnBnClickedCpuhBpEnCheck();
	afx_msg void OnBnClickedCpuvBpEnCheck();
	afx_msg void OnBnClickedCpdhBpEnCheck();
	afx_msg void OnBnClickedCpdvBpEnCheck();

	// Combo Box
	afx_msg void OnCbnSelchangeWorkModeCombo();
	afx_msg void OnCbnSelchangeDataSrcCombo();
	afx_msg void OnCbnSelchangeTmbSrcCombo();
	afx_msg void OnCbnSelchangeSpeTypeCombo();
	afx_msg void OnCbnSelchangeHistSelCombo();
	afx_msg void OnCbnSelchangeThumbFmtCombo();

	// Button
	afx_msg void OnBnClickedRefreshButton();
	afx_msg void OnBnClickedApplyButton();
	afx_msg void OnBnClickedResetButton();
	afx_msg void OnBnClickedTSizerButton();
	afx_msg void OnBnClickedCSizerButton();
	afx_msg void OnBnClickedDSizerButton();
	afx_msg void OnBnClickedImageSizeButton();
	afx_msg void OnBnClickedWinSizeButton();
	afx_msg void OnBnClickedHistGetButton();
	afx_msg void OnBnClickedHistSetButton();
	afx_msg void OnBnClickedHistEnButton();
};

#endif
