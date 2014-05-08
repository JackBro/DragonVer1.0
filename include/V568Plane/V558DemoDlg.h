#ifndef __V568PLANE_V558DEMODLG_H__
#define __V568PLANE_V558DEMODLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MDecEdit.h"
#include "BaseClass/MHexEdit.h"

enum DECODE_MODEenum{
	DEC_MODE_NORM,
	DEC_MODE_AUTO
};
enum VID_STOPenum{
	VID_STOP_HOST,
	VID_STOP_TIME
};


// CV558DemoDlg 对话框
typedef struct tag_TDemoOption
{
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	int		snrindex;
	TSize	snrsize;
	TSize	panelsize;
	int		curpanel;
	int		dispcfg;
	int		vispcfg;
	int		bright;
	int		contrast;
	int		encfmt;
	int		workmode;
	int		yuvmode;
	int		framecount;
	int		dectype;
	TSize	encsize;
	TJpegIndex info;
	int		dispsize;
	TALayerProty	layera;
	TSize	imgsz;
	TPoint	winpt;
	TSize	winsz;
	TSize	dspsz;
	TSize	capsz;
	TSize	thmsz;
	int		arot;
	int		thumbsrc;
	int		sifcap;
	int		caprddata;
	int		decmode;

	int		huecos;
	int		huesin;
	int		saturation;
	int		hueyoff;
	int		hueuoff;
	int		huevoff;

	int		hstep;
	int		vstep;
	int		vidtimer;

	int		rptcnt;

	int		recapcnt;
	int		recapactcnt;
	int		vidstoptimecnt;
	int		recap;
	int		vidstop;

	int		simphead;
	int		avi;
	int		tstrecap;

	int		decwrdata;


} TDemoOption, *PTDemoOption;

class V568PLANE_DLL CV558DemoDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV558DemoDlg)

public:
	CV558DemoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV558DemoDlg();

// 对话框数据
	enum { IDD = IDD_V558DEMO_DLG };

protected:
	void InitUserOption(void);
	void DispOption(void);
	void GetUserOption(char *tail = FILE_TAIL_JPG);
	void AdjustScrollBar(void);
	void GetBarValue(void);
	void GetEditBoxValue(void);
	void GetComboValue(char *tail = FILE_TAIL_JPG);

private:
	TDemoOption	m_option;

	CMComboBox	m_sensoridxbox;
	CMComboBox	m_bmpfilebox;
	CMComboBox	m_curpanelbox;

	CMHexEdit	m_srcimgwidbox;
	CMHexEdit	m_srcimgheightbox;
	CMHexEdit	m_panelwidthbox;
	CMHexEdit	m_panelheightbox;
	CMHexEdit	m_brightbox;
	CMHexEdit	m_contrastbox;
	CMHexEdit	m_saturbox;
	CMHexEdit	m_huebox;

	CScrollBar	m_brightbar;
	CScrollBar	m_contrastbar;

	CScrollBar	 m_axbar;
	CScrollBar	 m_aybar;
	CScrollBar	 m_awbar;
	CScrollBar	 m_ahbar;
	CScrollBar	 m_adxbar;
	CScrollBar	 m_adybar;
	CScrollBar	 m_amwbar;
	CScrollBar	 m_amhbar;

	CScrollBar	 m_winxbar;
	CScrollBar	 m_winybar;
	CScrollBar	 m_winwbar;
	CScrollBar	 m_winhbar;

	CScrollBar	 m_dpswbar;
	CScrollBar	 m_dpshbar;
	CScrollBar	 m_cpswbar;
	CScrollBar	 m_cpshbar;
	CScrollBar	 m_tmswbar;
	CScrollBar	 m_tmshbar;

	CMComboBox	m_encfmtbox;
	CMComboBox	m_workmodebox;
	CMComboBox	m_yuvmodebox;
	CMComboBox	m_jpgtypebox;
	CMComboBox	m_dectypebox;

	CMDecEdit	m_encwidthbox;
	CMDecEdit	m_encheightbox;
	CMDecEdit	m_jpgimgwidthbox;
	CMDecEdit	m_jpgwidthbox;
	CMDecEdit	m_jpgimgheightbox;
	CMDecEdit	m_jpgheightbox;

	CMHexEdit	m_sopbox;
	CMHexEdit	m_jpgposbox;

	CMHexEdit	 m_ax;
	CMHexEdit	 m_ay;
	CMHexEdit	 m_aw;
	CMHexEdit	 m_ah;
	CMHexEdit	 m_adx;
	CMHexEdit	 m_ady;
	CMHexEdit	 m_amw;
	CMHexEdit	 m_amh;

	CMHexEdit	 m_winx;
	CMHexEdit	 m_winy;
	CMHexEdit	 m_winw;
	CMHexEdit	 m_winh;

	CMDecEdit	m_frmcntbox;
	CMHexEdit	 m_dpsw;
	CMHexEdit	 m_dpsh;
	CMHexEdit	 m_cpsw;
	CMHexEdit	 m_cpsh;
	CMHexEdit	 m_tmsw;
	CMHexEdit	 m_tmsh;

	CButton		m_previewbtn;
	CButton		m_decodebtn;
	CButton		m_multishotbtn;
	CButton		m_capturebtn;
	CButton		m_displaybtn;
	CButton		m_encodebtn;
	CButton		m_dirdispnextbtn;

	CMHexEdit	m_ctlDispSize;

	CButton		m_sifCapBtn;
	CButton		m_rdCapBtn;
	CButton		m_decNormBtn;
	CButton		m_decAutoBtn;

	CScrollBar	 m_huecosbar;
	CScrollBar	 m_huesinbar;
	CScrollBar	 m_saturationbar;
	CScrollBar	 m_hueyoffbar;
	CScrollBar	 m_hueuoffbar;
	CScrollBar	 m_huevoffbar;

	CMHexEdit	 m_huecos;
	CMHexEdit	 m_huesin;
	CMHexEdit	 m_saturation;
	CMHexEdit	 m_hueyoff;
	CMHexEdit	 m_hueuoff;
	CMHexEdit	 m_huevoff;

	CScrollBar	 m_hstepbar;
	CScrollBar	 m_vstepbar;

	CMHexEdit	 m_hstep;
	CMHexEdit	 m_vstep;
	CMDecEdit	m_vidtimer;

	CMDecEdit	m_rptcnt;

	CButton		m_recap;
	CMDecEdit	m_recapcnt;
	CMDecEdit	m_recapactcnt;
	CMDecEdit	m_vidstoptimecnt;
	CButton		m_vidstophost;
	CButton		m_vidstoptime;

	CButton		m_simphead;
	CButton		m_avi;
	CButton		m_tstrecap;

	CButton		m_decwrdata;
protected:
	void	DecodeJpg(void);
	void	Capture(int bSifCap);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGetcontentButton();
	afx_msg void OnBnClickedV5openButton();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedPreviewButton();
	afx_msg void OnBnClickedB1dispbmpButton();
	afx_msg void OnBnClickedB0dispbmpButton();
	afx_msg void OnBnClickedCapsizerButton();
	afx_msg void OnBnClickedUpfilenameButton();
	afx_msg void OnCbnSelchangeDemoworkmodeCombo();
	afx_msg void OnBnClickedDispsizerButton();
	afx_msg void OnCbnSelchangeDemocapyuvmodeCombo();
	afx_msg void OnBnClickedDemoparserjpgButton();
	afx_msg void OnBnClickedDemosplitjpgButton();
	afx_msg void OnBnClickedEncodeButton();
	afx_msg void OnBnClickedDisplayButton();
	afx_msg void OnBnClickedCaptureButton();
	afx_msg void OnBnClickedMultishotButton();
	afx_msg void OnBnClickedDecodeButton();
	afx_msg void OnCbnSelchangeSnrsizeCombo();
	afx_msg void OnCbnSelchangeDecTypeCombo();
	afx_msg void OnBnClickedSkipDispNext();
	afx_msg void OnCbnSelchangePanelselCombo();
	afx_msg void OnBnClickedSetLayerAButton();
	afx_msg void OnBnClickedIppWinButton();
	afx_msg void OnBnClickedThumbSizerButton();
	afx_msg void OnBnClickedDecNorm();
	afx_msg void OnBnClickedDecAuto();
	afx_msg void OnBnClickedCheckSifcap();
	afx_msg void OnBnClickedCSizerTestButton();
	afx_msg void OnBnClickedCheckRecap();
	afx_msg void OnBnClickedRadioVidStopHost();
	afx_msg void OnBnClickedRadioVidStopTime();
	afx_msg void OnBnClickedCheckSimpHead();
	afx_msg void OnBnClickedCheckAvi();
	afx_msg void OnBnClickedCheckDecWrData();
};

#endif
