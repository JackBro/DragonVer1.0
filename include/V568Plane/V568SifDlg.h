#ifndef __V568PLANE_V568SIFDLG_H__
#define __V568PLANE_V568SIFDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"

// CV568SifDlg 对话框
typedef struct tag_TSifOption
{
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	int		snrmode;
	int		snropmode;
	// EditBox
	int		snrclk;
	int		pixelrate;
	int		siclk;
	int		i2cclk;
	int		i2cinter;
	int		hrefst;
	int		hreflen;
	int		vrefst;
	int		vrefsp;
	int		obreg;
	int		capdelay;
	int		vsyncdly_val;
	// Check Box
	int		snrpclk;
	int		vsyncdly_en;
	int		fifo_vsync_en;
	int		pclk_neg;
	int		snrshift;
	// Combo Box
	int		yuvfmt;
	int		edgechos;
	int		aemode;
	int		envsel;
	// ScrollBar
	int		aewinst;
	int		aewinsp;

	int		et;
	int		addr[7];
	int		count;
	int		valuecount[7];
	int		value[7][3];
	int		enflash;
	int		yflashthd;
	int		flashstep;
} TSifOption, *PTSifOption;

class V568PLANE_DLL CV568SifDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568SifDlg)

public:
	CV568SifDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568SifDlg();

// 对话框数据
	enum { IDD = IDD_V568SIF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

	DECLARE_MESSAGE_MAP()

private:
	TSifOption m_option;

	// Check Box
	CButton		m_snr_en;
	CButton		m_polarity_en;
	CButton		m_syncgen_en;
	CButton		m_obreg_en;
	CButton		m_snrclk_inv;
	CButton		m_siclk_rvs;
	CButton		m_syncout_en;
	CButton		m_snr_pclk_en;
	CButton		m_vsyncdly_en;
	CButton		m_fifodly_en;
	CButton		m_pclk_neg;
	CButton		m_snrshift_en;

	// Combo Box
	CMComboBox	m_plmode_box;
	CMComboBox	m_snrdata_box;
	CMComboBox	m_syncmode_box;
	CMComboBox	m_syncdir_box;
	CMComboBox	m_bussel_box;
	CMComboBox	m_i2csel_box;
	CMComboBox	m_yuvfmt_box;
	CMComboBox	m_edgechos_box;
	CMComboBox	m_aemode_box;
	CMComboBox	m_envsel_box;

	// ScrollBar
	CScrollBar	m_aewinst_bar;
	CScrollBar	m_aewinsp_bar;

	// ScrollBar EditBox
	CMHexEdit	m_aewinst;
	CMHexEdit	m_aewinsp;

	// EditBox
	CMHexEdit	m_snrclk;
	CMHexEdit	m_pixelrate;
	CMHexEdit	m_siclk;
	CMHexEdit	m_i2cclk;
	CMHexEdit	m_i2cinter;
	CMHexEdit	m_hrefst;
	CMHexEdit	m_hreflen;
	CMHexEdit	m_vrefst;
	CMHexEdit	m_vrefsp;
	CMHexEdit	m_obreg;
	CMHexEdit	m_capdelay;
	CMHexEdit	m_vsyncdly_val;

	CMHexEdit	m_edit_addr[7];
	CMHexEdit	m_edit_value[7][3];
	CButton		m_btnFlashEn;
	CMHexEdit	m_edit_flash_y;
	CMHexEdit	m_edit_flash_et;
	CMHexEdit	m_edit_et;

public:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// Check Box
	afx_msg void OnBnClickedSensorEnCheck();
	afx_msg void OnBnClickedPolarityEnCheck();
	afx_msg void OnBnClickedSyncGenEnCheck();
	afx_msg void OnBnClickedObRegEnCheck();
	afx_msg void OnBnClickedSnrClkInvCheck();
	afx_msg void OnBnClickedSiClkRvsCheck();
	afx_msg void OnBnClickedSyncOutEnCheck();
	afx_msg void OnBnClickedSnrPclkEnCheck();
	afx_msg void OnBnClickedFifoVsyncEnCheck();
	afx_msg void OnBnClickedVsyncDelayEnCheck();
	afx_msg void OnBnClickedPclkNegCheck();
	afx_msg void OnBnClickedSnrShiftCheck();

	// Combo Box
	afx_msg void OnCbnSelchangePolarityModeCombo();
	afx_msg void OnCbnSelchangeSensorDataCombo();
	afx_msg void OnCbnSelchangeSyncModeCombo();
	afx_msg void OnCbnSelchangeSyncDirCombo();
	afx_msg void OnCbnSelchangeBusSelCombo();
	afx_msg void OnCbnSelchangeI2cSelCombo();
	afx_msg void OnCbnSelchangeYuvFmtCombo();
	afx_msg void OnCbnSelchangeEdgeChosCombo();
	afx_msg void OnCbnSelchangeAeModeCombo();
	afx_msg void OnCbnSelchangeEnvSelCombo();

	// Button
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedSensorResetButton();
	afx_msg void OnBnClickedButtonPoweron();
	afx_msg void OnBnClickedButtonPoweroff();
	afx_msg void OnBnClickedButtonSensorWrite();
	afx_msg void OnBnClickedButtonFlashCharge();
	afx_msg void OnBnClickedButtonStopFlashCharge();
	afx_msg void OnBnClickedButtonStartFlash();
	afx_msg void OnBnClickedButtonStopFlash();
	afx_msg void OnBnClickedCheckFlashEn();
	afx_msg void OnBnClickedBtnSetflash();
	afx_msg void OnBnClickedBtnGetFlash();
	afx_msg void OnBnClickedButtonSetet();
	afx_msg void OnBnClickedGetEnvButton();
	afx_msg void OnBnClickedJudgeEnvButton();
	afx_msg void OnBnClickedStartAeButton();
	afx_msg void OnBnClickedSimAeButton();
};

#endif
