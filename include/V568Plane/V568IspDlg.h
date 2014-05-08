#ifndef __V568PLANE_V568ISPDLG_H__
#define __V568PLANE_V568ISPDLG_H__

#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "tools/global.h"
#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MDecEdit.h"
#include "BaseClass/MHexEdit.h"

// CV568IspDlg 对话框
typedef struct tag_TIspOption
{
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	int		ispctrl;
	int		aflnctrl;
	int		glgainctrl;
	int		afctrl;
	int		ctrl1;
	// EditBox
	TSize	barsz;
	TSize	imgsz;
	TV5IspEdgeParm			edge;
	TV5IspLensFalloff		lfo;
	TV5IspColorCvt			clrcvt;
	TV5IspGrayImg			grayimg;
	TV5IspColorMatrix		clrmat;
	TV5IspAwbClrRange		awblimit;
	TV5IspAwbGainLmt		awbgain;
	int		afstat;
	int		aflnstep;
	int		blank;
	int		rgbgain[3];
	int		glgain;
	UINT8	gmatbl[18];
	UINT8	hstart[5];
	UINT8	vstart[5];
	int		awbstep;
	int		awbthd1;
	int		awbthd2;
	int		mean[4];
	int		afstatus;
	int		edgesum;
	int		histgain;
	int		blroff;
	int		blboff;
	int		blg1off;
	int		blg2off;
	int		dpdthd;
	int		dpduvthd;
	int		ymeantgt;
	int		ymeanthd;
	// Check Box
	int		intflag;
	int		histctrl;
	int		vsyncrst;
	int		hue_en;
	// Combo Box
	int		gmasel;
	// ScrollBar

	int		env;
	int		mode;
} TIspOption, *PTIspOption;

typedef struct tag_TPixelRate
{
	int  idx;
	char str[20];
} TPixelRate, *PTPixelRate;

class V568PLANE_DLL CV568IspDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568IspDlg)

public:
	CV568IspDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568IspDlg();

// 对话框数据
	enum { IDD = IDD_V568ISP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_DAT);
	void DispOption(void);
	void InitCombo(void);
	void GetBarValue(void);
	void GetEditboxValue(void);
	void GetComboValue(char *tail = FILE_TAIL_DAT);
	void RefreshCheckBox(void);
	void RefreshCombo(void);
	void RefreshBar(void);
	void RefreshEditBox(void);
	void DispCheckBox(void);
	void DispCombo(void);
	void DispEditBox(void);
	void AdjustScrollBar(void);
	void DispBarEditBox(void);

	DECLARE_MESSAGE_MAP()

private:
	TIspOption m_option;

	// Check Box
	CButton		m_gpfirst;
	CButton		m_edge_en;
	CButton		m_lfo_en;
	CButton		m_dpdc_en;
	CButton		m_gamma_en;
	CButton		m_matrix_en;
	CButton		m_ygamma_en;
	CButton		m_rgbgain_en;
	CButton		m_aflnmode;
	CButton		m_glgain_en;
	CButton		m_aflk_en;
	CButton		m_awb_en;
	CButton		m_ynoise_en;
	CButton		m_uvnoise_en;
	CButton		m_afocus_en;
	CButton		m_bar_en;
	CButton		m_blkimg_en;
	CButton		m_fcsint_en;
	CButton		m_flkint_en;
	CButton		m_ymnint_en;
	CButton		m_hist_eft_en;
	CButton		m_hist_mnl_en;
	CButton		m_vsync_rst;
	CButton		m_hue_en;

	// Combo Box
	CMComboBox	m_pratebox;
	CMComboBox	m_fstlinebox;
	CMComboBox	m_siffmtbox;
	CMComboBox	m_filterbox;
	CMComboBox	m_imgmodebox;
	CMComboBox	m_glstepbox;
	CMComboBox	m_gldlybox;
	CMComboBox	m_gmaselbox;
	CMComboBox	m_intfreqbox;
	CMComboBox	m_aeweightbox;
	CMComboBox	m_winweightbox;
	CMComboBox	m_awbmodebox;
	CMComboBox	m_awbwinbox;
	CMComboBox	m_kernelbox;
	CMComboBox	m_blfilterbox;
	CMComboBox	m_barmodebox;

	// ScrollBar

	// ScrollBar EditBox

	// EditBox
	CMHexEdit	m_barw;
	CMHexEdit	m_barh;
	CMHexEdit	m_imgw;
	CMHexEdit	m_imgh;
	CMHexEdit	m_femm;
	CMHexEdit	m_femp;
	CMHexEdit	m_femx1;
	CMHexEdit	m_femx2;
	CMHexEdit	m_femmin;
	CMHexEdit	m_femmax;
	CMHexEdit	m_cenx;
	CMHexEdit	m_ceny;
	CMHexEdit	m_f2;
	CMHexEdit	m_aflncnt;
	CMHexEdit	m_aflnst;
	CMHexEdit	m_afstat;
	CMHexEdit	m_aflnstep;
	CMHexEdit	m_blank;
	CMHexEdit	m_rgain;
	CMHexEdit	m_ggain;
	CMHexEdit	m_bgain;
	CMHexEdit	m_glgain;
	CMHexEdit	m_cradj;
	CMHexEdit	m_cbadj;
	CMHexEdit	m_croft;
	CMHexEdit	m_cboft;
	CMHexEdit	m_mat11;
	CMHexEdit	m_mat12;
	CMHexEdit	m_mat13;
	CMHexEdit	m_mat21;
	CMHexEdit	m_mat22;
	CMHexEdit	m_mat23;
	CMHexEdit	m_mat31;
	CMHexEdit	m_mat32;
	CMHexEdit	m_mat33;
	CMHexEdit	m_roft;
	CMHexEdit	m_goft;
	CMHexEdit	m_boft;
	CMHexEdit	m_gmatbl0;
	CMHexEdit	m_gmatbl1;
	CMHexEdit	m_gmatbl2;
	CMHexEdit	m_gmatbl3;
	CMHexEdit	m_gmatbl4;
	CMHexEdit	m_gmatbl5;
	CMHexEdit	m_gmatbl6;
	CMHexEdit	m_gmatbl7;
	CMHexEdit	m_gmatbl8;
	CMHexEdit	m_gmatbl9;
	CMHexEdit	m_gmatbl10;
	CMHexEdit	m_gmatbl11;
	CMHexEdit	m_gmatbl12;
	CMHexEdit	m_gmatbl13;
	CMHexEdit	m_gmatbl14;
	CMHexEdit	m_gmatbl15;
	CMHexEdit	m_gmatbl16;
	CMHexEdit	m_gmatbl17;
	CMHexEdit	m_hstarta;
	CMHexEdit	m_hstartb;
	CMHexEdit	m_hstartc;
	CMHexEdit	m_hstartd;
	CMHexEdit	m_hstarte;
	CMHexEdit	m_vstarta;
	CMHexEdit	m_vstartb;
	CMHexEdit	m_vstartc;
	CMHexEdit	m_vstartd;
	CMHexEdit	m_vstarte;
	CMHexEdit	m_ybot;
	CMHexEdit	m_ytop;
	CMHexEdit	m_qtop;
	CMHexEdit	m_itop;
	CMHexEdit	m_glow;
	CMHexEdit	m_gtop;
	CMHexEdit	m_awbstep;
	CMHexEdit	m_awbthd1;
	CMHexEdit	m_awbthd2;
	CMHexEdit	m_rmean;
	CMHexEdit	m_gmean;
	CMHexEdit	m_bmean;
	CMHexEdit	m_ymean;
	CMHexEdit	m_afstatus;
	CMHexEdit	m_edgesum;
	CMHexEdit	m_histgain;
	CMHexEdit	m_blroff;
	CMHexEdit	m_blboff;
	CMHexEdit	m_blg1off;
	CMHexEdit	m_blg2off;
	CMHexEdit	m_dpdthd;
	CMHexEdit	m_dpduvthd;
	CMHexEdit	m_ymeantgt;
	CMHexEdit	m_ymeanthd;

public:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// Check Box
	afx_msg void OnBnClickedGpFirstCheck();
	afx_msg void OnBnClickedEdgeEnCheck();
	afx_msg void OnBnClickedLfoEnCheck();
	afx_msg void OnBnClickedDpdcEnCheck();
	afx_msg void OnBnClickedRgbGammaEnCheck();
	afx_msg void OnBnClickedMatrixEnCheck();
	afx_msg void OnBnClickedYGammaEnCheck();
	afx_msg void OnBnClickedRgbGainEnCheck();
	afx_msg void OnBnClickedAfLineCheck();
	afx_msg void OnBnClickedGlobalGainEnCheck();
	afx_msg void OnBnClickedAntiFlickerEnCheck();
	afx_msg void OnBnClickedAwbEnCheck();
	afx_msg void OnBnClickedNoiseYEnCheck();
	afx_msg void OnBnClickedNoiseUvEnCheck();
	afx_msg void OnBnClickedAutoFocusEnCheck();
	afx_msg void OnBnClickedTestBarEnCheck();
	afx_msg void OnBnClickedBlackImageEnCheck();
	afx_msg void OnBnClickedFocusIntEnCheck();
	afx_msg void OnBnClickedFlickerIntEnCheck();
	afx_msg void OnBnClickedYMeanIntEnCheck();
	afx_msg void OnBnClickedHistEffectEnCheck();
	afx_msg void OnBnClickedHistManualEnCheck();
	afx_msg void OnBnClickedVsyncRstCheck();
	afx_msg void OnBnClickedHueEnCheck();

	// Combo Box
	afx_msg void OnCbnSelchangePixelRateCombo();
	afx_msg void OnCbnSelchangeFirstLineCombo();
	afx_msg void OnCbnSelchangeSifFmtCombo();
	afx_msg void OnCbnSelchangeFilterCombo();
	afx_msg void OnCbnSelchangeImageModeCombo();
	afx_msg void OnCbnSelchangeGlobalGainStepCombo();
	afx_msg void OnCbnSelchangeGlobalGainDlyCombo();
	afx_msg void OnCbnSelchangeGammaSelCombo();
	afx_msg void OnCbnSelchangeAfIntFreqCombo();
	afx_msg void OnCbnSelchangeAeWeightCombo();
	afx_msg void OnCbnSelchangeWinWeightCombo();
	afx_msg void OnCbnSelchangeAwbModeCombo();
	afx_msg void OnCbnSelchangeAwbWinCombo();
	afx_msg void OnCbnSelchangeEdgeKernelCombo();
	afx_msg void OnCbnSelchangeFilterModeCombo();
	afx_msg void OnCbnSelchangeTestBarCombo();

	// Button
	afx_msg void OnBnClickedButtonIspApply();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedGammaGetButton();
	afx_msg void OnBnClickedGammaSetButton();
	afx_msg void OnBnClickedGetBlankButton();
	afx_msg void OnBnClickedSetBlankButton();
};

#endif
