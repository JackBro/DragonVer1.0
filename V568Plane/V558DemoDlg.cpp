// V558DemoDlg.cpp : 实现文件
//
#include "internal.h"
#include "..\include\v568plane\v558demodlg.h"

#ifndef	V578CMD_DISABLE




// CV558DemoDlg 对话框

IMPLEMENT_DYNAMIC(CV558DemoDlg, CBaseCtrlDlg)
CV558DemoDlg::CV558DemoDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV558DemoDlg::IDD, pParent)
{
}

CV558DemoDlg::~CV558DemoDlg()
{
}

void CV558DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SNRSIZE_COMBO,			m_sensoridxbox);
	DDX_Control(pDX, IDC_ENCJPEGFORMAT_COMBO,	m_encfmtbox);
	DDX_Control(pDX, IDC_DEMOWORKMODE_COMBO,	m_workmodebox);
	DDX_Control(pDX, IDC_DEMOCAPYUVMODE_COMBO,	m_yuvmodebox);
	DDX_Control(pDX, IDC_JPEGTYPE_COMBO,		m_jpgtypebox);
	DDX_Control(pDX, IDC_DECTYPE_COMBO,			m_dectypebox);
	DDX_Control(pDX, IDC_PANELSEL_COMBO,		m_curpanelbox);

	DDX_Control(pDX, IDC_SNRWIDTH_EDIT,			m_srcimgwidbox);
	DDX_Control(pDX, IDC_SNRHEIGHT_EDIT,		m_srcimgheightbox);
	DDX_Control(pDX, IDC_PANELWIDTH_EDIT,		m_panelwidthbox);
	DDX_Control(pDX, IDC_PANELHEIGHT_EDIT,		m_panelheightbox);
	DDX_Control(pDX, IDC_DEMOFRAMECOUNT_EDIT,	m_frmcntbox);
	DDX_Control(pDX, IDC_ENCIMAGEWIDTH_EDIT,	m_encwidthbox);
	DDX_Control(pDX, IDC_ENCIMAGEHEIGHT_EDIT,	m_encheightbox);
	DDX_Control(pDX, IDC_DEMOJPGIMGWIDTH_EDIT,	m_jpgimgwidthbox);
	DDX_Control(pDX, IDC_DEMOJPGWIDTH_EDIT,		m_jpgwidthbox);
	DDX_Control(pDX, IDC_DEMOJPGIMGHEIGHT_EDIT, m_jpgimgheightbox);
	DDX_Control(pDX, IDC_DEMOJPGHEIGHT_EDIT,	m_jpgheightbox);
	DDX_Control(pDX, IDC_DEMOJPGSOP_EDIT,		m_sopbox);
	DDX_Control(pDX, IDC_DEMOJPGDATAPOS_EDIT,	m_jpgposbox);

	DDX_Control(pDX, IDC_BRIGHTNESS_SCROLLBAR,	m_brightbar);
	DDX_Control(pDX, IDC_CONTRAST_SCROLLBAR,	m_contrastbar);
	DDX_Control(pDX, IDC_IPP_WINX_SCROLLBAR,	m_winxbar);
	DDX_Control(pDX, IDC_IPP_WINY_SCROLLBAR,	m_winybar);
	DDX_Control(pDX, IDC_IPP_WINW_SCROLLBAR,	m_winwbar);
	DDX_Control(pDX, IDC_IPP_WINH_SCROLLBAR,	m_winhbar);
	DDX_Control(pDX, IDC_IPP_DSW_SCROLLBAR,		m_dpswbar);
	DDX_Control(pDX, IDC_IPP_DSH_SCROLLBAR,		m_dpshbar);
	DDX_Control(pDX, IDC_IPP_CSW_SCROLLBAR,		m_cpswbar);
	DDX_Control(pDX, IDC_IPP_CSH_SCROLLBAR,		m_cpshbar);
	DDX_Control(pDX, IDC_IPP_TSW_SCROLLBAR,		m_tmswbar);
	DDX_Control(pDX, IDC_IPP_TSH_SCROLLBAR,		m_tmshbar);
	DDX_Control(pDX, IDC_LAX_SCROLLBAR,			m_axbar);
	DDX_Control(pDX, IDC_LAY_SCROLLBAR,			m_aybar);
	DDX_Control(pDX, IDC_LAW_SCROLLBAR,			m_awbar);
	DDX_Control(pDX, IDC_LAH_SCROLLBAR,			m_ahbar);
	DDX_Control(pDX, IDC_LADX_SCROLLBAR,		m_adxbar);
	DDX_Control(pDX, IDC_LADY_SCROLLBAR,		m_adybar);
	DDX_Control(pDX, IDC_LAMW_SCROLLBAR,		m_amwbar);
	DDX_Control(pDX, IDC_LAMH_SCROLLBAR,		m_amhbar);

	DDX_Control(pDX, IDC_BRIGHTNESS_EDIT,		m_brightbox);
	DDX_Control(pDX, IDC_CONTRAST_EDIT,			m_contrastbox);
	DDX_Control(pDX, IDC_IPP_WINX_EDIT,			m_winx);
	DDX_Control(pDX, IDC_IPP_WINY_EDIT,			m_winy);
	DDX_Control(pDX, IDC_IPP_WINW_EDIT,			m_winw);
	DDX_Control(pDX, IDC_IPP_WINH_EDIT,			m_winh);
	DDX_Control(pDX, IDC_IPP_DSW_EDIT,			m_dpsw);
	DDX_Control(pDX, IDC_IPP_DSH_EDIT,			m_dpsh);
	DDX_Control(pDX, IDC_IPP_CSW_EDIT,			m_cpsw);
	DDX_Control(pDX, IDC_IPP_CSH_EDIT,			m_cpsh);
	DDX_Control(pDX, IDC_IPP_TSW_EDIT,			m_tmsw);
	DDX_Control(pDX, IDC_IPP_TSH_EDIT,			m_tmsh);
	DDX_Control(pDX, IDC_LAX_EDIT,				m_ax);
	DDX_Control(pDX, IDC_LAY_EDIT,				m_ay);
	DDX_Control(pDX, IDC_LAW_EDIT,				m_aw);
	DDX_Control(pDX, IDC_LAH_EDIT,				m_ah);
	DDX_Control(pDX, IDC_LADX_EDIT,				m_adx);
	DDX_Control(pDX, IDC_LADY_EDIT,				m_ady);
	DDX_Control(pDX, IDC_LAMW_EDIT,				m_amw);
	DDX_Control(pDX, IDC_LAMH_EDIT,				m_amh);

	DDX_Control(pDX, IDC_DISPLAY_BUTTON,		m_displaybtn);
	DDX_Control(pDX, IDC_ENCODE_BUTTON,			m_encodebtn);
	DDX_Control(pDX, IDC_PREVIEW_BUTTON,		m_previewbtn);
	DDX_Control(pDX, IDC_DECODE_BUTTON,			m_decodebtn);
	DDX_Control(pDX, IDC_MULTISHOT_BUTTON,		m_multishotbtn);
	DDX_Control(pDX, IDC_CAPTURE_BUTTON,		m_capturebtn);

	DDX_Control(pDX, IDC_DIRDISP_SIZE_EDIT,		m_ctlDispSize);
	DDX_Control(pDX, IDC_SKIP_DISP_NEXT,		m_dirdispnextbtn);

	DDX_Control(pDX, IDC_CHECK_SIFCAP, 			m_sifCapBtn);
	DDX_Control(pDX, IDC_CHECK_CAP_RD_DATA, 	m_rdCapBtn);
	DDX_Control(pDX, IDC_RADIO_DEC_NORM, 		m_decNormBtn);
	DDX_Control(pDX, IDC_RADIO_DEC_AUTO, 		m_decAutoBtn);

	DDX_Control(pDX, IDC_HUE_COS_SCROLLBAR, 	m_huecosbar);
	DDX_Control(pDX, IDC_HUE_SIN_SCROLLBAR, 	m_huesinbar);
	DDX_Control(pDX, IDC_ISP_SAT_SCROLLBAR,		m_saturationbar);
	DDX_Control(pDX, IDC_HUE_Y_SCROLLBAR, 		m_hueyoffbar);
	DDX_Control(pDX, IDC_HUE_U_SCROLLBAR, 		m_hueuoffbar);
	DDX_Control(pDX, IDC_HUE_V_SCROLLBAR, 		m_huevoffbar);

	DDX_Control(pDX, IDC_HUE_COS_EDIT,			m_huecos);
	DDX_Control(pDX, IDC_HUE_SIN_EDIT, 			m_huesin);
	DDX_Control(pDX, IDC_ISP_SAT_EDIT,			m_saturation);
	DDX_Control(pDX, IDC_HUE_Y_EDIT, 			m_hueyoff);
	DDX_Control(pDX, IDC_HUE_U_EDIT, 			m_hueuoff);
	DDX_Control(pDX, IDC_HUE_V_EDIT, 			m_huevoff);

	DDX_Control(pDX, IDC_H_STEP_SCROLLBAR, 		m_hstepbar);
	DDX_Control(pDX, IDC_V_STEP_SCROLLBAR, 		m_vstepbar);

	DDX_Control(pDX, IDC_H_STEP_EDIT, 			m_hstep);
	DDX_Control(pDX, IDC_V_STEP_EDIT, 			m_vstep);
	DDX_Control(pDX, IDC_VID_TIMER, 			m_vidtimer);

	DDX_Control(pDX, IDC_RPT_CNT_EDIT, 			m_rptcnt);

	DDX_Control(pDX, IDC_CHECK_RECAP, 			m_recap);
	DDX_Control(pDX, IDC_RECAP_CNT, 			m_recapcnt);
	DDX_Control(pDX, IDC_RECAP_ACT_CNT, 		m_recapactcnt);
	DDX_Control(pDX, IDC_VID_STOP_TIMECNT, 		m_vidstoptimecnt);
	DDX_Control(pDX, IDC_RADIO_VID_STOP_HOST, 	m_vidstophost);
	DDX_Control(pDX, IDC_RADIO_VID_STOP_TIME, 	m_vidstoptime);

	DDX_Control(pDX, IDC_CHECK_SIMP_HEAD, 		m_simphead);
	DDX_Control(pDX, IDC_CHECK_AVI, 			m_avi);
	DDX_Control(pDX, IDC_CHECK_TEST_RECAP, 		m_tstrecap);

	DDX_Control(pDX, IDC_CHECK_DEC_WR_DATA, 	m_decwrdata);
}


BEGIN_MESSAGE_MAP(CV558DemoDlg, CBaseCtrlDlg)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_DEMOWORKMODE_COMBO,	OnCbnSelchangeDemoworkmodeCombo)
	ON_CBN_SELCHANGE(IDC_DEMOCAPYUVMODE_COMBO,	OnCbnSelchangeDemocapyuvmodeCombo)
	ON_CBN_SELCHANGE(IDC_PANELSEL_COMBO,		OnCbnSelchangePanelselCombo)
	ON_CBN_SELCHANGE(IDC_DECTYPE_COMBO,			OnCbnSelchangeDecTypeCombo)
	ON_CBN_SELCHANGE(IDC_SNRSIZE_COMBO,			OnCbnSelchangeSnrsizeCombo)
	ON_BN_CLICKED(IDC_GETCONTENT_BUTTON,		OnBnClickedGetcontentButton)
	ON_BN_CLICKED(IDC_V5OPEN_BUTTON,			OnBnClickedV5openButton)
	ON_BN_CLICKED(IDC_PREVIEW_BUTTON,			OnBnClickedPreviewButton)
	ON_BN_CLICKED(IDC_B1DISPBMP_BUTTON,			OnBnClickedB1dispbmpButton)
	ON_BN_CLICKED(IDC_B0DISPBMP_BUTTON,			OnBnClickedB0dispbmpButton)
	ON_BN_CLICKED(IDC_CAPSIZER_BUTTON,			OnBnClickedCapsizerButton)
	ON_BN_CLICKED(IDC_DISPSIZER_BUTTON,			OnBnClickedDispsizerButton)
	ON_BN_CLICKED(IDC_DEMOPARSERJPG_BUTTON,		OnBnClickedDemoparserjpgButton)
	ON_BN_CLICKED(IDC_DEMOSPLITJPG_BUTTON,		OnBnClickedDemosplitjpgButton)
	ON_BN_CLICKED(IDC_ENCODE_BUTTON,			OnBnClickedEncodeButton)
	ON_BN_CLICKED(IDC_DISPLAY_BUTTON,			OnBnClickedDisplayButton)
	ON_BN_CLICKED(IDC_CAPTURE_BUTTON,			OnBnClickedCaptureButton)
	ON_BN_CLICKED(IDC_MULTISHOT_BUTTON,			OnBnClickedMultishotButton)
	ON_BN_CLICKED(IDC_DECODE_BUTTON,			OnBnClickedDecodeButton)
	ON_BN_CLICKED(IDC_SKIP_DISP_NEXT,			OnBnClickedSkipDispNext)
	ON_BN_CLICKED(IDC_SET_LAYER_A_BUTTON,		OnBnClickedSetLayerAButton)
	ON_BN_CLICKED(IDC_IPP_WIN_BUTTON,			OnBnClickedIppWinButton)
	ON_BN_CLICKED(IDC_THUMB_SIZER_BUTTON,		OnBnClickedThumbSizerButton)
	ON_BN_CLICKED(IDC_RADIO_DEC_NORM,			OnBnClickedDecNorm)
	ON_BN_CLICKED(IDC_RADIO_DEC_AUTO,			OnBnClickedDecAuto)
	ON_BN_CLICKED(IDC_CHECK_SIFCAP,				OnBnClickedCheckSifcap)
	ON_BN_CLICKED(IDC_C_SIZER_TEST_BUTTON,		OnBnClickedCSizerTestButton)
	ON_BN_CLICKED(IDC_CHECK_RECAP, OnBnClickedCheckRecap)
	ON_BN_CLICKED(IDC_RADIO_VID_STOP_HOST, OnBnClickedRadioVidStopHost)
	ON_BN_CLICKED(IDC_RADIO_VID_STOP_TIME, OnBnClickedRadioVidStopTime)
	ON_BN_CLICKED(IDC_CHECK_SIMP_HEAD, OnBnClickedCheckSimpHead)
	ON_BN_CLICKED(IDC_CHECK_AVI, OnBnClickedCheckAvi)
	ON_BN_CLICKED(IDC_CHECK_DEC_WR_DATA, OnBnClickedCheckDecWrData)
END_MESSAGE_MAP()


// CV558DemoDlg 消息处理程序

BOOL CV558DemoDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CV558DemoDlg::InitUserOption(void)
{
	int i;
	char str[20];
	const char *dectype[] = { "rgb24", "565" };
	const char *paneltype[] = { "LCD_NULL_PANEL		",
								"LCD_LTS180S3_HF1	",
								"LCD_LTS180SH_H1I_X	",
								"LCD_TRULY_BJ2432E_8",
								"LCD_MGG1256DTSW	",
								"LCD_PLT2001		",
								"LCD_AMPIRE128160	" };

	memset(&(this->m_option), 0, sizeof(TDemoOption));
	this->m_option.snrindex   = 0;
	this->m_option.snrsize.cx = 640;
	this->m_option.snrsize.cy = 480;
	this->m_option.curpanel = V5_LCD_TYPE;
	this->m_option.panelsize.cx = 128;
	this->m_option.panelsize.cy = 160;
	this->m_option.bright = (V5_ISP_MAX_BRIGHT_CONTRAST + V5_ISP_MIN_BRIGHT_CONTRAST) >> 1;
	this->m_option.contrast = (V5_ISP_MAX_BRIGHT_CONTRAST + V5_ISP_MIN_BRIGHT_CONTRAST) >> 1;

	V5_SetCurPanel((UINT8)this->m_option.curpanel);
	for(i = 0; i < sizeof(paneltype) / sizeof(char *); i++)
		this->m_curpanelbox.InsertString(i, paneltype[i]);

	for(i = 0; i < 8; i++)
	{
		sprintf(str, "%d", i);
		this->m_sensoridxbox.InsertString(i, str);
	}

	for(i = 0; i < gV568WmodeCount; i++)
		this->m_workmodebox.InsertString(i, gV568WorkModeDesc[i]);
	for(i = 0; i < gV568PrevYuvCount; i++)
		this->m_yuvmodebox.InsertString(i, gV568PrevYuvmodeDesc[i]);
	for(i = 0; i < gTotalJpegFmt; i++)
		this->m_jpgtypebox.InsertString(i, gJpegFmt[i]);
	for(i = 0; i < gTotalJpegFmt-2; i++)
		this->m_encfmtbox.InsertString(i, gJpegFmt[i]);

	for(i = 0; i < sizeof(dectype) / sizeof(char *); i++)
		this->m_dectypebox.InsertString(i, dectype[i]);

	this->m_previewbtn.EnableWindow(FALSE);
	this->m_displaybtn.EnableWindow(FALSE);
	this->m_encodebtn.EnableWindow(FALSE);
	this->m_decodebtn.EnableWindow(FALSE);
	this->m_capturebtn.EnableWindow(FALSE);
	this->m_multishotbtn.EnableWindow(FALSE);
	this->m_dirdispnextbtn.EnableWindow(FALSE);

	this->m_option.decmode = DEC_MODE_NORM;
	this->m_ctlDispSize.EnableWindow(FALSE);
	this->m_sifCapBtn.EnableWindow(FALSE);
	this->m_rdCapBtn.EnableWindow(FALSE);
	this->m_decNormBtn.EnableWindow(FALSE);
	this->m_decAutoBtn.EnableWindow(FALSE);

	this->m_option.caprddata = 1;
	this->m_rdCapBtn.SetCheck(1);
	this->m_option.vidtimer = 100;
	this->m_decwrdata.SetCheck(1);
	this->m_option.decwrdata = 1;

	this->m_option.recap = 0;
	this->m_option.vidstop = VID_STOP_HOST;
	m_recapcnt.EnableWindow(FALSE);
	m_recapactcnt.EnableWindow(FALSE);
	m_vidstoptimecnt.EnableWindow(FALSE);



}

void CV558DemoDlg::GetUserOption(char *tail)
{
	this->GetEditBoxValue();
	this->GetComboValue(tail);

	if(this->m_rdCapBtn.GetCheck())
		this->m_option.caprddata = 1;
	else
		this->m_option.caprddata = 0;

	if(this->m_decNormBtn.GetCheck())
		this->m_option.decmode = DEC_MODE_NORM;
	else if (this->m_decAutoBtn.GetCheck())
		this->m_option.decmode = DEC_MODE_AUTO;


	if(this->m_recap.GetCheck())
		this->m_option.recap = 1;
	else
		this->m_option.recap = 0;

	if(this->m_vidstophost.GetCheck())
		this->m_option.vidstop = VID_STOP_HOST;

	this->m_option.simphead = this->m_simphead.GetCheck();
	this->m_option.avi = this->m_avi.GetCheck();
	this->m_option.tstrecap = this->m_tstrecap.GetCheck();

	this->m_option.decwrdata = this->m_decwrdata.GetCheck();

}

void CV558DemoDlg::DispOption(void)
{
	int tmp = 0;
	char str[20];

	this->AdjustScrollBar();

	sprintf(str, "%x", this->m_option.bright);
	this->m_brightbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.contrast);
	this->m_contrastbox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.winpt.x);
	this->m_winx.SetWindowText(str);
	sprintf(str, "%x", this->m_option.winpt.y);
	this->m_winy.SetWindowText(str);
	sprintf(str, "%x", this->m_option.winsz.cx);
	this->m_winw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.winsz.cy);
	this->m_winh.SetWindowText(str);

	sprintf(str, "%x", this->m_option.thmsz.cx);
	this->m_tmsw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.thmsz.cy);
	this->m_tmsh.SetWindowText(str);
	sprintf(str, "%x", this->m_option.capsz.cx);
	this->m_cpsw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.capsz.cy);
	this->m_cpsh.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dspsz.cx);
	this->m_dpsw.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dspsz.cy);
	this->m_dpsh.SetWindowText(str);

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

	sprintf(str, "%x", this->m_option.snrsize.cx);
	this->m_srcimgwidbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.snrsize.cy);
	this->m_srcimgheightbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.panelsize.cx);
	this->m_panelwidthbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.panelsize.cy);
	this->m_panelheightbox.SetWindowText(str);

	this->m_sensoridxbox.SetCurSel(this->m_option.snrindex);
	this->m_curpanelbox.SetCurSel(this->m_option.curpanel);

	this->m_encfmtbox.SetCurSel(this->m_option.encfmt);
	this->m_workmodebox.SetCurSel(this->m_option.workmode);
	this->m_yuvmodebox.SetCurSel(this->m_option.yuvmode);
	this->m_jpgtypebox.SetCurSel(this->m_option.info.YUVType);
	this->m_dectypebox.SetCurSel(this->m_option.dectype);

	sprintf(str, "%d", this->m_option.info.ImageSize.cx);
	this->m_jpgimgwidthbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.info.ImageSize.cy);
	this->m_jpgimgheightbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.info.JpgSize.cx);
	this->m_jpgwidthbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.info.JpgSize.cy);
	this->m_jpgheightbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.info.sop);
	this->m_sopbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.info.offset);
	this->m_jpgposbox.SetWindowText(str);

	sprintf(str, "%d", this->m_option.encsize.cx);
	this->m_encwidthbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.encsize.cy);
	this->m_encheightbox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.dispsize);
	this->m_ctlDispSize.SetWindowText(str);

	if(this->m_option.sifcap)
		this->m_sifCapBtn.SetCheck(1);
	else
		this->m_sifCapBtn.SetCheck(0);

	if(this->m_option.caprddata)
		this->m_rdCapBtn.SetCheck(1);
	else
		this->m_rdCapBtn.SetCheck(0);

	switch(this->m_option.decmode)
	{
	case DEC_MODE_NORM:
		this->m_decNormBtn.SetCheck(1);
		this->m_decAutoBtn.SetCheck(0);
		break;
	case DEC_MODE_AUTO:
		this->m_decNormBtn.SetCheck(0);
		this->m_decAutoBtn.SetCheck(1);
		break;
	}

	sprintf(str, "%d", this->m_option.framecount);
	this->m_frmcntbox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.saturation);
	this->m_saturation.SetWindowText(str);
	tmp = 0;
	if(this->m_option.huecos < 0)
	{
		tmp ++;
		str[0] = '-';
	}
	sprintf(&(str[tmp]), "%x", abs(this->m_option.huecos));

	this->m_huecos.SetWindowText(str);
	tmp = 0;
	if(this->m_option.huesin < 0)
	{
		tmp ++;
		str[0] = '-';
	}
	sprintf(&(str[tmp]), "%x", abs(this->m_option.huesin));
	this->m_huesin.SetWindowText(str);

	tmp = 0;
	if(this->m_option.hueyoff < 0)
	{
		tmp ++;
		str[0] = '-';
	}
	sprintf(&(str[tmp]), "%x", abs(this->m_option.hueyoff));
	this->m_hueyoff.SetWindowText(str);

	tmp = 0;
	if(this->m_option.hueuoff < 0)
	{
		tmp ++;
		str[0] = '-';
	}
	sprintf(str, "%d", this->m_option.framecount);
	this->m_frmcntbox.SetWindowText(str);
	sprintf(&(str[tmp]), "%x", abs(this->m_option.hueuoff));
	this->m_hueuoff.SetWindowText(str);

	tmp = 0;
	if(this->m_option.huevoff < 0)
	{
		tmp ++;
		str[0] = '-';
	}
	sprintf(&(str[tmp]), "%x", abs(this->m_option.huevoff));
	this->m_huevoff.SetWindowText(str);

	sprintf(str, "%x", this->m_option.hstep);
	this->m_hstep.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vstep);
	this->m_vstep.SetWindowText(str);
	sprintf(str, "%d", this->m_option.vidtimer);
	this->m_vidtimer.SetWindowText(str);

	sprintf(str, "%d", this->m_option.rptcnt);
	this->m_rptcnt.SetWindowText(str);

	sprintf(str, "%d", this->m_option.recapcnt);
	this->m_recapcnt.SetWindowText(str);
	sprintf(str, "%d", this->m_option.recapactcnt);
	this->m_recapactcnt.SetWindowText(str);
	sprintf(str, "%d", this->m_option.vidstoptimecnt);
	this->m_vidstoptimecnt.SetWindowText(str);

	if(this->m_option.recap)
		this->m_recap.SetCheck(1);
	else
		this->m_recap.SetCheck(0);

	switch(this->m_option.vidstop)
	{
	case VID_STOP_HOST:
		this->m_vidstophost.SetCheck(1);
		this->m_vidstoptime.SetCheck(0);
		break;
	case VID_STOP_TIME:
		this->m_vidstophost.SetCheck(0);
		this->m_vidstoptime.SetCheck(1);
		break;
	}

	this->m_simphead.SetCheck(this->m_option.simphead);
	this->m_avi.SetCheck(this->m_option.avi);
	this->m_tstrecap.SetCheck(this->m_option.tstrecap);
	this->m_decwrdata.SetCheck(this->m_option.decwrdata);




}

void CV558DemoDlg::AdjustScrollBar(void)
{
	TSize size;

	this->m_brightbar.SetScrollRange(V5_ISP_MIN_BRIGHT_CONTRAST, V5_ISP_MAX_BRIGHT_CONTRAST);
	this->m_brightbar.SetScrollPos(this->m_option.bright);
	this->m_contrastbar.SetScrollRange(V5_ISP_MIN_BRIGHT_CONTRAST, V5_ISP_MAX_BRIGHT_CONTRAST);
	this->m_contrastbar.SetScrollPos(this->m_option.contrast);

	this->m_winxbar.SetScrollRange(0, (this->m_option.imgsz.cx - this->m_option.winsz.cx));
	this->m_winxbar.SetScrollPos(this->m_option.winpt.x);
	this->m_winybar.SetScrollRange(0, (this->m_option.imgsz.cy - this->m_option.winsz.cy));
	this->m_winybar.SetScrollPos(this->m_option.winpt.y);

	this->m_winwbar.SetScrollRange(0, this->m_option.imgsz.cx);
	this->m_winwbar.SetScrollPos(this->m_option.winsz.cx);
	this->m_winhbar.SetScrollRange(0, this->m_option.imgsz.cy);
	this->m_winhbar.SetScrollPos(this->m_option.winsz.cy);

	size.cx = (UINT16)((this->m_option.winsz.cx > 510) ? 510 : this->m_option.winsz.cx);
	size.cy = (UINT16)((this->m_option.winsz.cy > 512) ? 512 : this->m_option.winsz.cy);
	if(this->m_option.workmode == IPP_MODE_DEC)
		size = this->m_option.winsz;
	this->m_dpswbar.SetScrollRange(0, size.cx);
	this->m_option.dspsz.cx = (this->m_option.dspsz.cx > size.cx) ? size.cx : this->m_option.dspsz.cx;
	this->m_dpswbar.SetScrollPos(this->m_option.dspsz.cx);
	this->m_option.dspsz.cy = (this->m_option.dspsz.cy > size.cy) ? size.cy : this->m_option.dspsz.cy;
	this->m_dpshbar.SetScrollRange(0, size.cy);
	this->m_dpshbar.SetScrollPos(this->m_option.dspsz.cy);

	size.cx = (UINT16)((this->m_option.winsz.cx > IPP_CSIZER_HIN_MAX) ? this->m_option.winsz.cx : (this->m_option.winsz.cx << 1));
	size.cy = (UINT16)(((this->m_option.winsz.cy << 1) > IPP_CSIZER_VOUT_MAX) ? IPP_CSIZER_VOUT_MAX : (this->m_option.winsz.cy << 1));
	this->m_cpswbar.SetScrollRange(0, size.cx);
	this->m_cpswbar.SetScrollPos(this->m_option.capsz.cx);
	this->m_cpshbar.SetScrollRange(0, size.cy);
	this->m_cpshbar.SetScrollPos(this->m_option.capsz.cy);

	switch(this->m_option.thumbsrc)
	{
	case IPP_TMB_SPECIAL :
		size = this->m_option.winsz;
		break;
	case IPP_TMB_DSIZER :
		size = this->m_option.dspsz;
		break;
	default :
		size = this->m_option.capsz;
		break;
	}
	this->m_tmswbar.SetScrollRange(IPP_TSIZER_HOUT_MIN, ((size.cx > IPP_TSIZER_HOUT_MAX) ? IPP_TSIZER_HOUT_MAX : size.cx));
	this->m_tmswbar.SetScrollPos(((this->m_option.thmsz.cx > IPP_TSIZER_HOUT_MAX) ? IPP_TSIZER_HOUT_MAX : this->m_option.thmsz.cx));
	this->m_tmshbar.SetScrollRange(IPP_TSIZER_VOUT_MIN, ((size.cy > IPP_TSIZER_VOUT_MAX) ? IPP_TSIZER_VOUT_MAX : size.cy));
	this->m_tmshbar.SetScrollPos(((this->m_option.thmsz.cy > IPP_TSIZER_VOUT_MAX) ? IPP_TSIZER_VOUT_MAX : this->m_option.thmsz.cy));

	size.cx = (UINT16)((this->m_option.dspsz.cx > 510) ? 510 : this->m_option.dspsz.cx);
	size.cy = (UINT16)((this->m_option.dspsz.cy > 512) ? 512 : this->m_option.dspsz.cy);
	this->m_amwbar.SetScrollRange(0, size.cx);
	this->m_amwbar.SetScrollPos(size.cx);
	this->m_amhbar.SetScrollRange(0, size.cy);
	this->m_amhbar.SetScrollPos(size.cy);
	if(this->m_option.arot % 0x02)
	{
		this->m_axbar.SetScrollRange(0, (this->m_option.panelsize.cy - this->m_option.layera.win.size.cx));
		this->m_axbar.SetScrollPos(this->m_option.layera.pos2panel.x);
		this->m_aybar.SetScrollRange(0, (this->m_option.panelsize.cx - this->m_option.layera.win.size.cy));
		this->m_aybar.SetScrollPos(this->m_option.layera.pos2panel.y);

		size.cy = this->m_option.panelsize.cx > size.cy ? size.cy : this->m_option.panelsize.cx;
		this->m_awbar.SetScrollRange(0, size.cy);
		this->m_option.layera.win.size.cx = (UINT16)size.cy;
		this->m_awbar.SetScrollPos(this->m_option.layera.win.size.cx);
		size.cx = this->m_option.panelsize.cy > size.cx ? size.cx : this->m_option.panelsize.cy;
		this->m_ahbar.SetScrollRange(0, size.cx);
		this->m_option.layera.win.size.cy = (UINT16)size.cx;
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

		this->m_awbar.SetScrollRange(0, size.cx);
		this->m_awbar.SetScrollPos((this->m_option.layera.win.size.cx > size.cx ? size.cx : this->m_option.layera.win.size.cx));
		this->m_ahbar.SetScrollRange(0, size.cy);
		this->m_ahbar.SetScrollPos((this->m_option.layera.win.size.cy > size.cy ? size.cy : this->m_option.layera.win.size.cy));

		this->m_adxbar.SetScrollRange(0, (this->m_option.layera.memsize.cx - this->m_option.layera.win.size.cx));
		this->m_adxbar.SetScrollPos(this->m_option.layera.win.ltp.x);
		this->m_adybar.SetScrollRange(0, (this->m_option.layera.memsize.cy - this->m_option.layera.win.size.cy));
		this->m_adybar.SetScrollPos(this->m_option.layera.win.ltp.y);
	}

	this->m_saturationbar.SetScrollRange(0, 0x80);
	this->m_saturationbar.SetScrollPos(this->m_option.saturation);
	this->m_huecosbar.SetScrollRange(-0x100, 0x100);
	this->m_huecosbar.SetScrollPos(this->m_option.huecos);
	this->m_huesinbar.SetScrollRange(-0x100, 0x100);
	this->m_huesinbar.SetScrollPos(this->m_option.huesin);
	this->m_hueyoffbar.SetScrollRange(-0xff, 0xff);
	this->m_hueyoffbar.SetScrollPos(this->m_option.hueyoff);
	this->m_hueuoffbar.SetScrollRange(-0xff, 0xff);
	this->m_hueuoffbar.SetScrollPos(this->m_option.hueuoff);
	this->m_huevoffbar.SetScrollRange(-0xff, 0xff);
	this->m_huevoffbar.SetScrollPos(this->m_option.huevoff);

	this->m_hstepbar.SetScrollRange(0, 1024);
	this->m_hstepbar.SetScrollPos(this->m_option.hstep);
	this->m_vstepbar.SetScrollRange(0, 768);
	this->m_vstepbar.SetScrollPos(this->m_option.vstep);

	this->GetBarValue();
}

void CV558DemoDlg::GetBarValue(void)
{
	this->m_option.bright = this->m_brightbar.GetScrollPos();
	this->m_option.contrast = this->m_contrastbar.GetScrollPos();

	this->m_option.winpt.x  = (UINT16)this->m_winxbar.GetScrollPos();
	this->m_option.winpt.y  = (UINT16)this->m_winybar.GetScrollPos();
	this->m_option.winsz.cx = (UINT16)this->m_winwbar.GetScrollPos();
	this->m_option.winsz.cy = (UINT16)this->m_winhbar.GetScrollPos();

	this->m_option.thmsz.cx = (UINT16)this->m_tmswbar.GetScrollPos();
	this->m_option.thmsz.cy = (UINT16)this->m_tmshbar.GetScrollPos();
	this->m_option.capsz.cx = (UINT16)this->m_cpswbar.GetScrollPos();
	this->m_option.capsz.cy = (UINT16)this->m_cpshbar.GetScrollPos();
	this->m_option.dspsz.cx = (UINT16)this->m_dpswbar.GetScrollPos();
	this->m_option.dspsz.cy = (UINT16)this->m_dpshbar.GetScrollPos();

	this->m_option.layera.pos2panel.x = (UINT16)this->m_axbar.GetScrollPos();
	this->m_option.layera.pos2panel.y = (UINT16)this->m_aybar.GetScrollPos();
	this->m_option.layera.win.size.cx = (UINT16)this->m_awbar.GetScrollPos();
	this->m_option.layera.win.size.cy = (UINT16)this->m_ahbar.GetScrollPos();
	this->m_option.layera.win.ltp.x	  = (UINT16)this->m_adxbar.GetScrollPos();
	this->m_option.layera.win.ltp.y   = (UINT16)this->m_adybar.GetScrollPos();
	this->m_option.layera.memsize.cx  = (UINT16)this->m_amwbar.GetScrollPos();
	this->m_option.layera.memsize.cy  = (UINT16)this->m_amhbar.GetScrollPos();

	this->m_option.saturation = this->m_saturationbar.GetScrollPos();
	this->m_option.huecos	  = this->m_huecosbar.GetScrollPos();
	this->m_option.huesin	  = this->m_huesinbar.GetScrollPos();
	this->m_option.hueyoff	  = this->m_hueyoffbar.GetScrollPos();
	this->m_option.hueuoff	  = this->m_hueuoffbar.GetScrollPos();
	this->m_option.huevoff	  = this->m_huevoffbar.GetScrollPos();

	this->m_option.hstep = this->m_hstepbar.GetScrollPos();
	this->m_option.vstep = this->m_vstepbar.GetScrollPos();
}

void CV558DemoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWaitCursor cursor;
	int pos;

	if(pScrollBar != NULL)
	{
		switch(nSBCode)
		{
		case SB_LINELEFT:
			pos = pScrollBar->GetScrollPos();
			pScrollBar->SetScrollPos(pos - 2);
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
			//nPos = ((nPos + 1) >> 1) << 1;
			pScrollBar->SetScrollPos(nPos);
			break;
		default:
			break;
		}
		this->GetBarValue();
		this->DispOption();
	}
	if(pScrollBar == &(this->m_saturationbar))
		V5B_IspSetSat((UINT8)this->m_option.saturation);
	if(pScrollBar == &(this->m_huecosbar))
		V5B_IspSetHueCos(this->m_option.huecos);
	if(pScrollBar == &(this->m_huesinbar))
		V5B_IspSetHueSin(this->m_option.huesin);
	if(pScrollBar == &(this->m_hueyoffbar))
		V5B_IspSetHueYOff(this->m_option.hueyoff);
	if(pScrollBar == &(this->m_hueuoffbar))
		V5B_IspSetHueUOff(this->m_option.hueuoff);
	if(pScrollBar == &(this->m_huevoffbar))
		V5B_IspSetHueVOff(this->m_option.huevoff);

	CBaseCtrlDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CV558DemoDlg::GetComboValue(char *tail)
{
	this->m_SrcfCombo.GetWindowText(this->m_option.srcfile, DEFAULT_FILENAME_LEN);
	this->m_option.snrindex = this->m_sensoridxbox.GetCurSel();
	this->m_option.workmode = this->m_workmodebox.GetCurSel();
	this->m_option.yuvmode = this->m_yuvmodebox.GetCurSel();
	this->m_option.encfmt = this->m_encfmtbox.GetCurSel();
	this->m_option.dectype = this->m_dectypebox.GetCurSel();
	this->m_option.curpanel = this->m_curpanelbox.GetCurSel();

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CV558DemoDlg::OnCbnSelchangeDemoworkmodeCombo()
{
	CWaitCursor cursor;

	this->GetUserOption();
	this->m_option.imgsz = this->m_option.snrsize;

	V5B_LcdcDisableAlayer();
	V5B_SifDisableSyncGen();
	V5_SetMode((UINT8)(this->m_option.workmode));
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_SifEnableSyncGen();
	Delay(10);
	V5B_LcdcEnableAlayer();

	switch(this->m_option.workmode)
	{
	case PREVIEW_MODE:
	case PREVIEWBRC_MODE:
	case PREVIEWFRAME_MODE:
		this->m_previewbtn.EnableWindow();
		this->m_displaybtn.EnableWindow(FALSE);
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow(FALSE);
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow();
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(FALSE);
		this->m_decNormBtn.EnableWindow(FALSE);
		this->m_decAutoBtn.EnableWindow(FALSE);

		break;
	case ENCODE_MODE:
		this->m_previewbtn.EnableWindow();
		this->m_displaybtn.EnableWindow(FALSE);
		this->m_encodebtn.EnableWindow();
		this->m_decodebtn.EnableWindow(FALSE);
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow(FALSE);
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(FALSE);
		this->m_decNormBtn.EnableWindow(FALSE);
		this->m_decAutoBtn.EnableWindow(FALSE);
		break;

	case CAPTURESTILL_MODE:
	case CAPTURESTILLTHUMB_MODE:
	case CAPTUREFRAME_MODE:
	case CAPTURETHUMB_MODE:
	case CAPTUREFRAMETHUMB_MODE:
		this->m_previewbtn.EnableWindow();
		this->m_displaybtn.EnableWindow(FALSE);
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow(FALSE);
		this->m_capturebtn.EnableWindow();
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow();
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(TRUE);
		this->m_rdCapBtn.EnableWindow(TRUE);
		this->m_decNormBtn.EnableWindow(FALSE);
		this->m_decAutoBtn.EnableWindow(FALSE);
		break;
	case CAPTUREVIDEO_MODE:
	case CAPTUREVIDEOTHUMB_MODE:
	case CAPTUREAVI_MODE:
	case CAPTUREAVITHUMB_MODE:
	case CAPTUREFRAMEVIDEO_MODE:
	case CAPTUREFRAMEAVI_MODE:
	case CAPTUREFRAMEVIDEOTHUMB_MODE:
	case CAPTUREFRAMEAVITHUMB_MODE:

		this->m_previewbtn.EnableWindow();
		this->m_displaybtn.EnableWindow(FALSE);
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow(FALSE);
		this->m_capturebtn.EnableWindow();
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow();
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(TRUE);
		this->m_rdCapBtn.EnableWindow(TRUE);
		this->m_decNormBtn.EnableWindow(FALSE);
		this->m_decAutoBtn.EnableWindow(FALSE);
		break;
	case MULTISHOT_MODE:
	case MULTISHOTTHUMB_MODE:
	case MULTISHOTFRAME_MODE:
		this->m_previewbtn.EnableWindow();
		this->m_displaybtn.EnableWindow(FALSE);
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow(FALSE);
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow();
		this->m_yuvmodebox.EnableWindow();
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(TRUE);
		this->m_decNormBtn.EnableWindow(FALSE);
		this->m_decAutoBtn.EnableWindow(FALSE);
		break;
	case DISPLAYSTILL_MODE:
		this->m_previewbtn.EnableWindow(FALSE);
		this->m_displaybtn.EnableWindow();
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow(FALSE);
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow(FALSE);
		this->m_dirdispnextbtn.EnableWindow();
		if(this->m_option.decmode == DEC_MODE_AUTO)
			this->m_ctlDispSize.EnableWindow(TRUE);
		else
			this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(FALSE);
		this->m_decNormBtn.EnableWindow(TRUE);
		this->m_decAutoBtn.EnableWindow(TRUE);
		break;

	case DISPLAYVIDEO_MODE:
	case DISPLAYAVI_MODE:

		this->m_previewbtn.EnableWindow(FALSE);
		this->m_displaybtn.EnableWindow();
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow(FALSE);
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow(FALSE);
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(FALSE);
		this->m_decNormBtn.EnableWindow(TRUE);
		this->m_decAutoBtn.EnableWindow(TRUE);
		break;
	case DECODEIPPBYPASS_MODE:
		this->m_previewbtn.EnableWindow(FALSE);
		this->m_displaybtn.EnableWindow(FALSE);
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow();
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow(FALSE);
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(FALSE);
		this->m_decNormBtn.EnableWindow(FALSE);
		this->m_decAutoBtn.EnableWindow(FALSE);
		break;
	case DECODEIPP_MODE:
		this->m_previewbtn.EnableWindow(FALSE);
		this->m_displaybtn.EnableWindow(FALSE);
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow();
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow(FALSE);
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(FALSE);
		this->m_decNormBtn.EnableWindow(TRUE);
		this->m_decAutoBtn.EnableWindow(TRUE);
		break;
	case DECODEFRAMEBYPASS_MODE:
		this->m_previewbtn.EnableWindow(FALSE);
		this->m_displaybtn.EnableWindow(FALSE);
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow();
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow(FALSE);
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(FALSE);
		this->m_decNormBtn.EnableWindow(FALSE);
		this->m_decAutoBtn.EnableWindow(FALSE);
		break;
	case DECODEFRAME_MODE:
		this->m_previewbtn.EnableWindow(FALSE);
		this->m_displaybtn.EnableWindow(FALSE);
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow();
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow(FALSE);
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(FALSE);
		this->m_decNormBtn.EnableWindow(TRUE);
		this->m_decAutoBtn.EnableWindow(TRUE);
		break;
	case DECODE_MODE:
		this->m_previewbtn.EnableWindow();
		this->m_displaybtn.EnableWindow(FALSE);
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow();
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow(FALSE);
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(FALSE);
		this->m_decNormBtn.EnableWindow(TRUE);
		this->m_decAutoBtn.EnableWindow(TRUE);
		break;
	case DISPLAYBYPASS_MODE:
		this->m_previewbtn.EnableWindow(FALSE);
		this->m_displaybtn.EnableWindow();
		this->m_encodebtn.EnableWindow(FALSE);
		this->m_decodebtn.EnableWindow(FALSE);
		this->m_capturebtn.EnableWindow(FALSE);
		this->m_multishotbtn.EnableWindow(FALSE);
		this->m_yuvmodebox.EnableWindow(FALSE);
		this->m_dirdispnextbtn.EnableWindow(FALSE);
		this->m_ctlDispSize.EnableWindow(FALSE);
		this->m_sifCapBtn.EnableWindow(FALSE);
		this->m_rdCapBtn.EnableWindow(FALSE);
		this->m_decNormBtn.EnableWindow(FALSE);
		this->m_decAutoBtn.EnableWindow(FALSE);
		break;
	default:
		break;
	}
}

void CV558DemoDlg::OnCbnSelchangeDemocapyuvmodeCombo()
{
	CWaitCursor cursor;

	this->GetUserOption();
	V5B_LcdcDisableAlayer();
	V5B_SifDisableSyncGen();
	V5B_LbufSetYuvMode((UINT8)(this->m_option.yuvmode));
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_SifEnableSyncGen();
	V5B_LcdcEnableAlayer();
}

void CV558DemoDlg::GetEditBoxValue(void)
{
	char str[20];
	int x;

	this->m_encwidthbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.encsize.cx = (UINT16)x;
	this->m_encheightbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.encsize.cy = (UINT16)x;
	this->m_frmcntbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.framecount));

	this->m_ctlDispSize.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.dispsize));
	this->m_vidtimer.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.vidtimer));

	this->m_rptcnt.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.rptcnt));


	this->m_recapcnt.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.recapcnt));
	this->m_recapactcnt.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.recapactcnt));
	this->m_vidstoptimecnt.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.vidstoptimecnt));



}

void CV558DemoDlg::OnBnClickedGetcontentButton()
{
	CWaitCursor cursor;

	V5_VispGetCurrentSize(&(this->m_option.snrsize));

	this->m_option.bright = (int)V5_VispGetBrightness();
	this->m_option.contrast = (int)V5_VispGetContrast();

	this->m_option.thumbsrc = V5B_IppGetThumbInput();
	this->m_option.arot = V5B_LcdcGetAlayerRM();

	V5_DispGetPanelSize(&(this->m_option.panelsize));
	V5_DispGetAmemSize(&(this->m_option.layera));

	V5B_IppGetImageSize(&(this->m_option.imgsz));
	V5B_IppGetWinPoint(&(this->m_option.winpt));
	V5B_IppGetWinSize(&(this->m_option.winsz));
	V5B_IppGetDispSize(&(this->m_option.dspsz));
	V5B_IppGetCapSize(&(this->m_option.capsz));
	V5B_IppGetThumbSize(&(this->m_option.thmsz));

	this->m_option.dispcfg = (int)V5B_LcdcGetDc();
	this->m_option.vispcfg = (int)V5B_IppGetSizerCtrl0();

	this->m_option.workmode = V5_GetMode();
	this->m_option.yuvmode = V5B_LbufGetYuvMode();

	this->m_option.saturation = V5B_IspGetSat();
	this->m_option.huecos	  = V5B_IspGetHueCos();
	this->m_option.huesin	  = V5B_IspGetHueSin();
	this->m_option.hueyoff	  = V5B_IspGetHueYOff();
	this->m_option.hueuoff	  = V5B_IspGetHueUOff();
	this->m_option.huevoff	  = V5B_IspGetHueVOff();

	this->DispOption();
}

void CV558DemoDlg::OnBnClickedV5openButton()
{
	CWaitCursor cursor;

	V5_Open();
	V5_CameraOn(0);
	this->m_option.snrindex = V5_VispGetImageIndex();
	this->OnCbnSelchangeDemoworkmodeCombo();
	this->OnBnClickedGetcontentButton();
}

void CV558DemoDlg::OnBnClickedPreviewButton()
{
	CWaitCursor cursor;

	this->GetUserOption();
	V5_VispSetDispSizer((UINT8)this->m_option.snrindex, this->m_option.winpt, this->m_option.winsz, this->m_option.dspsz);
	V5_VispSetBrightness((UINT8)this->m_option.bright);
	V5_VispSetContrast((UINT8)this->m_option.contrast);
}

void CV558DemoDlg::OnBnClickedB1dispbmpButton()
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	int size;

	this->GetUserOption();
	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	if(size == 0)
		return;
	pool.SetSize(size);
	fb.Read(pool.m_buf, size);
	V5_DisplayB1Bmp((UINT8 *)pool.m_buf);
}

void CV558DemoDlg::OnBnClickedB0dispbmpButton()
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	int size;

	this->GetUserOption();
	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	if(size == 0)
		return;
	pool.SetSize(size);
	fb.Read(pool.m_buf, size);
	V5_DisplayB0Bmp((UINT8 *)pool.m_buf);
}

void CV558DemoDlg::OnBnClickedDispsizerButton()
{
	CWaitCursor cursor;

	this->GetUserOption();
	if(this->m_option.workmode == DECODEIPPBYPASS_MODE
		|| this->m_option.workmode == DECODEFRAMEBYPASS_MODE
		|| this->m_option.workmode == DISPLAYBYPASS_MODE)
	{
		this->m_option.imgsz = this->m_option.encsize;
		this->m_option.winsz = this->m_option.encsize;
		this->m_option.winsz.cx >>= 1;
		this->m_option.winsz.cx <<= 1;
		this->m_option.winsz.cy >>= 1;
		this->m_option.winsz.cy <<= 1;
		this->DispOption();
	}
	V5B_LcdcDisableAlayer();
	V5_IppSetDispSizerEx2(this->m_option.imgsz, this->m_option.winpt, this->m_option.winsz, this->m_option.dspsz);

	//l_yong add this to reduce operation of the user.
	V5B_LbufSetYuvMode((UINT8)(this->m_option.yuvmode));
	V5_DispSetAmemSize(&(this->m_option.layera));

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_LcdcEnableAlayer();
}

void CV558DemoDlg::OnBnClickedCapsizerButton()
{
	CWaitCursor cursor;

	this->GetUserOption();

	V5B_LcdcDisableAlayer();
	V5B_IppSetThumbSize(this->m_option.thmsz);
	V5_IppSetCapSizerEx2(this->m_option.imgsz, this->m_option.winpt, this->m_option.winsz, this->m_option.capsz);
	//l_yong add this to reduce the operation of the user.
	V5B_LbufSetYuvMode((UINT8)(this->m_option.yuvmode));

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_LcdcEnableAlayer();
}

void CV558DemoDlg::OnBnClickedSetLayerAButton()
{
	V5B_LcdcDisableAlayer();
	V5_DispSetAmemSize(&(this->m_option.layera));
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_LcdcEnableAlayer();
}

void CV558DemoDlg::OnBnClickedIppWinButton()
{
	V5B_LcdcDisableAlayer();
	V5_IppSetDispSizerEx2(this->m_option.imgsz, this->m_option.winpt, this->m_option.winsz, this->m_option.dspsz);
	if(IPP_MODE_CAP == V5B_IppGetWorkMode())
		V5_IppSetCapSizerEx2(this->m_option.imgsz, this->m_option.winpt, this->m_option.winsz, this->m_option.capsz);
	V5_DispSetAmemSize(&(this->m_option.layera));
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	V5B_LcdcEnableAlayer();
}

void CV558DemoDlg::OnBnClickedThumbSizerButton()
{
	this->GetUserOption();

	V5B_IppSetThumbSize(this->m_option.thmsz);
	V5_IppThumbSizerUpdate();

	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
}

void CV558DemoDlg::OnBnClickedDemoparserjpgButton()
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	int size;
	//CJpegDecoder jpgDec;

	this->GetUserOption("info");
	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	pool.SetSize(size);
	fb.Read(pool.m_buf, size);

	//jpgDec.JpegParse((UINT8 *)pool.m_buf, size, &(this->m_option.info));
	JpegParse((UINT8 *)pool.m_buf, size, &(this->m_option.info));

	switch(this->m_option.workmode)
	{
	case DISPLAYSTILL_MODE:
	case DECODE_MODE:
	case DECODEIPP_MODE:
	case DECODEFRAME_MODE:
	case DISPLAYVIDEO_MODE:
	case DISPLAYAVI_MODE:
		this->m_option.imgsz = this->m_option.info.JpgSize;
		this->m_option.winsz = this->m_option.imgsz;
		break;
	default:
		break;
	}
	this->DispOption();
}

void CV558DemoDlg::OnBnClickedDemosplitjpgButton()
{
	CWaitCursor cursor;
	CJpegF tools;

	this->GetUserOption(FILE_TAIL_JPG);
	tools.SplitJpeg(this->m_option.srcfile, JPEGF_LIST, this->m_option.dstfile);
}

void CV558DemoDlg::OnBnClickedEncodeButton()
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool, pool1;
	TDataBuf dbuf, dsrc;
	int size;

	this->GetUserOption();
	pool.SetSize(0x100000);
	dbuf.pData = (UINT8 *)pool.m_buf;
	dbuf.Pos = 0;
	size = pool.GetTotalSize();
	dbuf.Len = (UINT32)size;

	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	pool1.SetSize(size);
	if(pool1.GetSize() == 0)
		return;
	fb.Read(pool1.m_buf, size);
	dsrc.pData = (UINT8 *)pool1.m_buf;
	dsrc.Len = (UINT32)size;
	dsrc.Pos = 0;

	V5_HJpegEnc(&dsrc, &dbuf, (UINT8)this->m_option.encfmt, this->m_option.encsize);

	if(dbuf.Pos <= 0)
		return;
	fb.Open(this->m_option.dstfile, "wb");
	fb.Write(pool.m_buf, dbuf.Pos);
}

void CV558DemoDlg::OnBnClickedDisplayButton()
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	int size;
	TDataBuf	dbuf;
	TJpegIndex info;
	UINT8* pbuf;

	//this->OnBnClickedDemoparserjpgButton();
	this->GetUserOption(FILE_TAIL_DAT);
	if(this->m_option.info.YUVType == JPEG_UNKNOWN)
		return;

	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	if(size == 0)
		return;
	pool.SetSize(size << 1);
	fb.Read(pool.m_buf, size);

	pbuf = (UINT8 *)pool.m_buf;

	JpegParse((UINT8 *)pbuf, size, &info);

	//this->m_option.imgsz = info.JpgSize;
	//this->m_option.ippsizer = info.ImageSize;

	//this->DispOption();

	//V5_IppSetDispSizerEx2(this->m_option.imgsz, this->m_option.ipppt, 
	//		this->m_option.ippsizer, this->m_option.ippdisp);
	//V5_DispSetAmemSize(&this->m_option.layera);

	dbuf.Len = (UINT32)size;
	dbuf.Pos = 0;
	dbuf.pData = pbuf;
	
	switch(this->m_option.workmode)
	{
	case DISPLAYBYPASS_MODE:
		V5_HJpegYuvDisp(&dbuf, this->m_option.encsize);
		break;
	case DISPLAYVIDEO_MODE:
		if(this->m_option.decmode == DEC_MODE_AUTO)
			V5_HJpegDispVidAuto(&dbuf, this->m_option.vidtimer);
		else
			V5_HJpegDispVidNorm(&dbuf);

		break;
	case DISPLAYAVI_MODE:
		if(this->m_option.decmode == DEC_MODE_AUTO)
			V5_HJpegDispAviAuto(&dbuf, this->m_option.vidtimer);
		else
			V5_HJpegDispVidNorm(&dbuf);
		if(!this->m_avi)
			V5B_JpegAviFmtDisable();

		break;

	case DISPLAYSTILL_MODE:
		if(this->m_option.decmode == DEC_MODE_AUTO)
		{
			if(this->m_option.decwrdata)
				V5_HJpegDispFrmAuto(&dbuf);
			else
				V5_HJpegDispFrmDirAuto((UINT8)(this->m_option.info.YUVType), info.JpgSize, this->m_option.dispsize);
		}
		else
			V5_HJpegDispFrm(&dbuf);

		break;
	default:
		break;
	}

}


void CV558DemoDlg::Capture(int bSifCap)
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool, pool1, poolThumb;
	TDataBuf dbuf;
	int size;
	TJpegCapParm capparm;
	UINT8 captype = 0;
	char tmpbuf[100];

	memset((void*)&capparm, 0, sizeof(TJpegCapParm));

	OnBnClickedGetcontentButton();

	pool.SetSize(0x150000);
	//pool.SetSize(0x80000);
	if(pool.GetSize() == 0)
		return;
	size = pool.GetTotalSize();
	dbuf.pData = (UINT8 *)pool.m_buf;
	dbuf.Len = (UINT32)size;
	dbuf.Pos = 0;

	switch(this->m_option.workmode)
	{
	case CAPTURESTILL_MODE:
	case CAPTURESTILLTHUMB_MODE:
		if(bSifCap)
		{
			captype = 1;
		}
		else
		{
			if(this->m_option.recap)
				captype = 2;
		}
		break;
	case CAPTUREVIDEO_MODE:
	case CAPTUREVIDEOTHUMB_MODE:
	case CAPTUREAVI_MODE:
	case CAPTUREAVITHUMB_MODE:
	case CAPTUREFRAMEVIDEO_MODE:
	case CAPTUREFRAMEAVI_MODE:
	case CAPTUREFRAMEVIDEOTHUMB_MODE:
	case CAPTUREFRAMEAVITHUMB_MODE:
		if(this->m_option.vidstop == VID_STOP_TIME)
			captype = 4;
		break;
	}

	switch(captype)
	{
	case 0:
		if(this->m_option.caprddata)
			TestCap(&dbuf, &capparm);
		else
		{
			TestCap(NULL, NULL);
			this->m_option.dispsize = V5B_MarbGetJbufSize()-8;
			this->DispOption();
			return;
		}
		break;
	case 1:
		if(this->m_option.caprddata)
			TestSifCap(&dbuf, &capparm);
		else
		{
			TestSifCap(NULL, NULL);
			this->m_option.dispsize = V5B_MarbGetJbufSize()-8;
			this->DispOption();
			return;
		}
		break;
	case 2:
		if(this->m_option.tstrecap)
			V5B_IspDisableTestBar();
		if(this->m_option.caprddata)
		{
			TestReCap(&dbuf, &capparm, (UINT8*)&this->m_option.recapcnt);
			this->m_option.recapactcnt = (UINT8)this->m_option.recapcnt;
			sprintf(tmpbuf, "%x", this->m_option.recapactcnt);
			this->m_recapactcnt.SetWindowText(tmpbuf);
		}
		else
		{
			TestReCap(NULL, NULL, (UINT8*)&this->m_option.recapcnt);
			this->m_option.dispsize = V5B_MarbGetJbufSize()-8;
			this->m_option.recapactcnt = (UINT8)this->m_option.recapcnt;
			this->DispOption();
			return;
		}
		break;
	case 4:
		if(this->m_option.caprddata)
			TestCapVidTimer(&dbuf, this->m_option.vidstoptimecnt, &capparm);
		else
		{
			TestCapVidTimer(NULL, this->m_option.vidstoptimecnt,NULL);
			this->m_option.dispsize = V5B_MarbGetJbufSize()-8;
			this->DispOption();
			return;
		}
		break;
	}

	if(dbuf.Pos <= 0)
		return;

	if(capparm.ilen != 0)
	{
		char thumb[100] = "";

		strcpy(thumb, this->m_option.dstfile);
		strcat(thumb, ".idx");

		fb.Open(thumb, "wb");
		fb.Write(pool.m_buf+capparm.ilen, capparm.tlen);
		fb.Close();
	}

	if(capparm.tlen != 0)
	{
		CGeneralTools tools;
		char thumb[100] = "";
		char thumb1[100] = "";
		char thumb2[100] = "";
		int srcfmt, desfmt;
		UINT32 len;

		UINT8 thmfmt = V5B_IppGetThumOutFmt();

		if(!thmfmt)
		{
			strcpy(thumb, this->m_option.dstfile);
			strcat(thumb, ".yuv");

			strcpy(thumb1, this->m_option.dstfile);
			strcat(thumb1, ".rgb");

			/* add jfif format */
			strcpy(thumb2, this->m_option.dstfile);
			strcat(thumb2, ".jfxx");
			//strcat(thumb2, ".jfif");

			pool1.SetSize(0x180000);
			poolThumb.SetSize(0x80000);
			len = MYuv422_Rgborder(pool.m_buf+capparm.ilen, poolThumb.m_buf, this->m_option.thmsz.cx, this->m_option.thmsz.cy, YUV422_UYVY);
			//len = Jpeg2Jfif((unsigned char*)pool.m_buf+capparm.ilen+capparm.tlen, (unsigned char*)poolThumb.m_buf, (unsigned char*)pool1.m_buf, capparm.jlen, len,this->m_option.thumb.cx, this->m_option.thumb.cy);
			len = Jpeg2Jfxx((unsigned char*)pool.m_buf+capparm.ilen+capparm.tlen, (unsigned char*)poolThumb.m_buf, (unsigned char*)pool1.m_buf, 
							capparm.jlen, (unsigned short)len, (unsigned char)this->m_option.thmsz.cx, (unsigned char)this->m_option.thmsz.cy, THUMB_FORMAT_RGB24);
			fb.Open(thumb2, "wb");
			fb.Write(pool1.m_buf, len);
			fb.Close();
		
			fb.Open(thumb, "wb");
			fb.Write(pool.m_buf+capparm.ilen, capparm.tlen);
			fb.Close();

			//srcfmt = YuvmodeToYuvfmt(this->m_option.yuvmode);
			srcfmt = YuvmodeToYuvfmt(V5B_LBUF_YUV_422);
			desfmt = RGB24;
			tools.DataConvert(thumb, thumb1, this->m_option.thmsz.cx, this->m_option.thmsz.cy, srcfmt, desfmt, 1, 0);

			strcpy(thumb, this->m_option.dstfile);
			strcat(thumb, ".bmp");

			srcfmt = RGB24;
			desfmt = BMP;
			tools.DataConvert(thumb1, thumb, this->m_option.thmsz.cx, this->m_option.thmsz.cy, srcfmt, desfmt, 1, 0);
		}
		else
		{
			strcpy(thumb, this->m_option.dstfile);
			strcat(thumb, ".rgb565");

			strcpy(thumb1, this->m_option.dstfile);
			strcat(thumb1, ".rgb");

			fb.Open(thumb, "wb");
			fb.Write(pool.m_buf+capparm.ilen, capparm.tlen);
			fb.Close();

			//srcfmt = YuvmodeToYuvfmt(this->m_option.yuvmode);
			srcfmt = RGB565;
			desfmt = RGB24;
			tools.DataConvert(thumb, thumb1, this->m_option.thmsz.cx, this->m_option.thmsz.cy, srcfmt, desfmt, 1, 0);

			strcpy(thumb, this->m_option.dstfile);
			strcat(thumb, ".bmp");

			srcfmt = RGB24;
			desfmt = BMP;
			tools.DataConvert(thumb1, thumb, this->m_option.thmsz.cx, this->m_option.thmsz.cy, srcfmt, desfmt, 1, 0);
		}
	}

	fb.Open(this->m_option.dstfile, "wb");
	fb.Write(pool.m_buf+capparm.ilen+capparm.tlen, capparm.jlen);
/*
	if(capparm.jlen)
	{
		this->m_option.dispsart = V5B_MarbGetJbufStart();
		this->m_option.dispsize = V5B_MarbGetJbufSize()-8;
		this->m_option.dispsize = ((UINT32)this->m_option.dispsize > capparm.jlen) ? capparm.jlen : this->m_option.dispsize;

		this->DispOption();
	}
*/
}

void CV558DemoDlg::OnBnClickedCaptureButton()
{
	this->GetUserOption(FILE_TAIL_AVI);

	Capture(this->m_option.sifcap);
}

void CV558DemoDlg::OnBnClickedMultishotButton()
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	TDataBuf dbuf;
	int size;
	TJpegCapParm capparm;

	this->GetUserOption(FILE_TAIL_AVI);
	pool.SetSize(0x120000);
	if(pool.GetSize() == 0)
		return;
	size = pool.GetTotalSize();
	dbuf.pData = (UINT8 *)pool.m_buf;
	dbuf.Len = (UINT32)size;
	dbuf.Pos = 0;

	V5B_MarbSetMshotFcnt((UINT8)this->m_option.framecount);
	TestCap(&dbuf, &capparm);

	if(dbuf.Pos <= 0)
		return;
	if(capparm.tlen != 0)
	{
		CGeneralTools tools;
		char thumb[100] = "";
		char thumb1[100] = "";
		int srcfmt, desfmt;
		UINT8 i;
		UINT32 x; 
				
		x = V5B_IppGetThumbWidth();
		x *= V5B_IppGetThumbHeight();
		x <<= 1;

		for(i=0; i<this->m_option.framecount+1; i++)
		{
			strcpy(thumb1, this->m_option.dstfile);
			strcat(thumb, ".yuv");

			strcpy(thumb1, this->m_option.dstfile);
			strcat(thumb1, ".rgb");

			fb.Open(thumb, "wb");
			fb.Write(pool.m_buf+capparm.ilen+x*i, x);
			fb.Close();

			//srcfmt = YuvmodeToYuvfmt(this->m_option.yuvmode);
			srcfmt = YuvmodeToYuvfmt(V5B_LBUF_YUV_422);
			desfmt = RGB24;
			tools.DataConvert(thumb, thumb1, this->m_option.thmsz.cx, this->m_option.thmsz.cy, srcfmt, desfmt, 1, 0);

			sprintf(thumb, "%s%i", this->m_option.dstfile, i);
			strcat(thumb, ".bmp");

			srcfmt = RGB24;
			desfmt = BMP;
			tools.DataConvert(thumb1, thumb, this->m_option.thmsz.cx, this->m_option.thmsz.cy, srcfmt, desfmt, 1, 0);
		}
	}
	fb.Open(this->m_option.dstfile, "wb");
	fb.Write(pool.m_buf, dbuf.Pos);

	if(capparm.jlen)
	{
		this->m_option.dispsize = V5B_MarbGetJbufSize()-8;
		this->m_option.dispsize = ((UINT32)this->m_option.dispsize > capparm.jlen) ? capparm.jlen : this->m_option.dispsize;
		this->DispOption();
	}
}

void CV558DemoDlg::OnBnClickedDecodeButton()
{
	CWaitCursor cursor;
	CGeneralTools tools;
	char dec[100] = "";
	char dec1[100] = "";
	int srcfmt, desfmt;
	UINT8 yuvmode;

	//this->OnBnClickedDemoparserjpgButton();
	this->GetUserOption(FILE_TAIL_DAT);

	if(this->m_option.workmode != DECODEIPPBYPASS_MODE && this->m_option.workmode != DECODEFRAMEBYPASS_MODE)
	{
		if(this->m_option.info.YUVType == JPEG_UNKNOWN)
			return;
	}

	switch(this->m_option.workmode)
	{
	case DECODEIPP_MODE:
	case DECODEFRAME_MODE:
	case DECODEIPPBYPASS_MODE:
	case DECODEFRAMEBYPASS_MODE:

		this->DecodeJpg();

		strcpy(dec, this->m_option.dstfile);

		strcpy(dec1, this->m_option.dstfile);
		strcat(dec1, ".rgb");

		if(this->m_option.dectype
			&& (this->m_option.workmode == DECODEFRAME_MODE || this->m_option.workmode == DECODEFRAMEBYPASS_MODE))
		{
			srcfmt = RGB565;
		}
		else
		{
			srcfmt = YuvmodeToYuvfmt(V5B_LBUF_YUV_422);
		}
		desfmt = RGB24;
		tools.DataConvert(dec, dec1, this->m_option.dspsz.cx, this->m_option.dspsz.cy, srcfmt, desfmt, 1, 0);

		strcpy(dec, this->m_option.dstfile);
		strcat(dec, ".bmp");

		srcfmt = RGB24;
		desfmt = BMP;
		tools.DataConvert(dec1, dec, this->m_option.dspsz.cx, this->m_option.dspsz.cy, srcfmt, desfmt, 1, 0);

		break;
	case DECODE_MODE:

		this->DecodeJpg();

		strcpy(dec1, this->m_option.dstfile);

		strcpy(dec, this->m_option.dstfile);
		strcat(dec, ".yuv");

		if(V5B_LBUF_YUV_444 == this->m_option.info.YUVType)
		{
			yuvmode = V5B_LBUF_YUV_422;

			srcfmt = YUV444422LBUF;;
			desfmt = YuvmodeToYuvfmt(yuvmode);
		}
		else
		{
			yuvmode = this->m_option.info.YUVType;
			srcfmt = YuvmodeToYuvLbuf(yuvmode);
			desfmt = YuvmodeToYuvfmt(yuvmode);
		}


		tools.DataConvert(dec1, dec, this->m_option.info.JpgSize.cx, this->m_option.info.JpgSize.cy, 
								srcfmt, desfmt, 1, 0);


		strcpy(dec, this->m_option.dstfile);
		strcat(dec, ".yuv");

		strcpy(dec1, this->m_option.dstfile);
		strcat(dec1, ".rgb");

		srcfmt = YuvmodeToYuvfmt(yuvmode);
		desfmt = RGB24;
		tools.DataConvert(dec, dec1, this->m_option.info.JpgSize.cx, this->m_option.info.JpgSize.cy, 
								srcfmt, desfmt, 1, 0);

		strcpy(dec, this->m_option.dstfile);
		strcat(dec, ".bmp");

		srcfmt = RGB24;
		desfmt = BMP;
		tools.DataConvert(dec1, dec, this->m_option.info.JpgSize.cx, this->m_option.info.JpgSize.cy,
								srcfmt, desfmt, 1, 0);

		break;
	default:
		break;
	}
}

void CV558DemoDlg::DecodeJpg(void)
{
	CFileBin fb;
	CBufferBase pool, pool1;
	TDataBuf dbuf, dsrc;
	int size;

	this->GetUserOption(FILE_TAIL_DAT);
	pool.SetSize(2048*1536*3);
	if(pool.GetSize() == 0)
		return;
	size = pool.GetTotalSize();
	dbuf.pData = (UINT8 *)pool.m_buf;
	dbuf.Len = (UINT32)size;
	dbuf.Pos = 0;

	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	pool1.SetSize(size << 1);
	if(pool1.GetSize() == 0)
		return;
	fb.Read(pool1.m_buf, size);
	dsrc.pData = (UINT8 *)pool1.m_buf;
	dsrc.Len = size;
	dsrc.Pos = 0;

	switch(this->m_option.workmode)
	{
	case DECODE_MODE:
		V5_HJpegDec(&dsrc, &dbuf);
		break;
	case DECODEIPP_MODE:
		V5_HJpegDecIpp(&dsrc, &dbuf);
		break;
	case DECODEFRAME_MODE:
		V5_HJpegDecFrm(&dsrc, &dbuf);
		break;
	case DECODEIPPBYPASS_MODE:
		V5_HJpegYuvDecIpp(&dsrc, &dbuf, this->m_option.encsize);
		break;
	case DECODEFRAMEBYPASS_MODE:
		V5_HJpegYuvDecFrm(&dsrc, &dbuf, this->m_option.encsize);
		break;
	}

	if(dbuf.Pos <= 0)
		return;
	fb.Open(this->m_option.dstfile, "wb");
	fb.Write(pool.m_buf, dbuf.Pos);
}

void CV558DemoDlg::OnCbnSelchangeSnrsizeCombo()
{
	this->m_option.snrindex = this->m_sensoridxbox.GetCurSel();

	V5B_LcdcDisableAlayer();
	V5_VispSetImageIndex((UINT8)this->m_option.snrindex);
	V5_VispGetSize((UINT8)this->m_option.snrindex, &this->m_option.snrsize);
	this->m_option.imgsz = this->m_option.snrsize;
	this->m_option.winpt.x = 0;
	this->m_option.winpt.y = 0;
	this->m_option.winsz = this->m_option.imgsz;
	this->m_option.capsz = this->m_option.winsz;
	this->OnBnClickedIppWinButton();
	V5B_LcdcEnableAlayer();

	this->DispOption();
}


void CV558DemoDlg::OnCbnSelchangeDecTypeCombo()
{
	CWaitCursor cursor;

	this->GetUserOption();

	if(this->m_option.dectype)
	{
		V5B_LbufDec2Rgb565();
	}
	else
	{
		V5B_LbufDec2Yuv422();
	}

}

void CV558DemoDlg::OnBnClickedSkipDispNext()
{
	// TODO: 在此添加控件通知处理程序代码
	V5_HJpegDispNextFrm();
}

void CV558DemoDlg::OnCbnSelchangePanelselCombo()
{
	this->m_option.curpanel = this->m_curpanelbox.GetCurSel();
	V5_SetCurPanel((UINT8)this->m_option.curpanel);
}

void CV558DemoDlg::OnBnClickedDecNorm()
{
	m_ctlDispSize.EnableWindow(FALSE);
}

void CV558DemoDlg::OnBnClickedDecAuto()
{
	m_ctlDispSize.EnableWindow(TRUE);
}

void CV558DemoDlg::OnBnClickedCheckSifcap()
{
	if(this->m_sifCapBtn.GetCheck())
	{
		this->m_recap.SetCheck(0);
		this->m_option.sifcap = 1;
	}
	else
	{
		this->m_option.sifcap = 0;
		V5B_SifEnableSyncGen();
	}
}

void CV558DemoDlg::OnBnClickedCSizerTestButton()
{
	char path[256], str[20];
	int loop = 1, count = 0, dir = 0, i = 0, step = 0, rpt = 0;
	TSize size = this->m_option.winsz, tsize = this->m_option.capsz;
	const int fnum[]        = { 10000, 1000, 100, 10 };
	const char *fname[3][5] = {	{ "VBps_", "VBps_0", "VBps_00", "VBps_000", "VBps_0000" },
								{ "HBps_", "HBps_0", "HBps_00", "HBps_000", "HBps_0000" },
								{ "SizerUp_", "SizerUp_0", "SizerUp_00", "SizerUp_000", "SizerUp_0000" } };

	this->GetUserOption(FILE_TAIL_JPG);
	strcpy(path, this->m_option.dstfile);
	if((tsize.cx > size.cx) || (tsize.cy > size.cy))
		dir = 1;

	rpt = this->m_option.rptcnt;
	while(rpt)
	{
		count ++;
		sprintf(str, "%d", count);
		strcpy(this->m_option.dstfile, path);
		strcat(this->m_option.dstfile, "UpBps_");
		strcat(this->m_option.dstfile, str);
		strcat(this->m_option.dstfile, ".jpg");

		this->Capture(this->m_option.sifcap);

		if(rpt == 1)
			return;
		rpt --;
	}

	V5_IppSetCapSizerEx1(this->m_option.winpt, this->m_option.winsz, size);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
	strcpy(this->m_option.dstfile, path);
	strcat(this->m_option.dstfile, "_Normal.jpg");
	this->Capture(this->m_option.sifcap);

	while(loop)
	{
		if(step == 0)
		{
			if(this->m_option.hstep == 0)
			{
				step  = 1;
				continue;
			}
			if(dir)
			{
				if(size.cx >= tsize.cx)
				{
					step  = 1;
					size = this->m_option.winsz;
					continue;
				}
				else
					size.cx = size.cx + (UINT16)this->m_option.hstep;
				if(size.cx > tsize.cx)
					size.cx = tsize.cx;
			}
			else
			{
				if(size.cx <= tsize.cx)
				{
					step  = 1;
					size = this->m_option.winsz;
					continue;
				}
				else
					size.cx = size.cx - (UINT16)this->m_option.hstep;
				if(size.cx < tsize.cx)
					size.cx = tsize.cx;
			}
		}

		if(step == 1)
		{
			if(this->m_option.vstep == 0)
			{
				step  = 2;
				continue;
			}
			if(dir)
			{
				if(size.cy >= tsize.cy)
				{
					step  = 2;
					size = this->m_option.winsz;
					continue;
				}
				else
					size.cy = size.cy + (UINT16)this->m_option.vstep;
				if(size.cy > tsize.cy)
					size.cy = tsize.cy;
			}
			else
			{
				if(size.cy <= tsize.cy)
				{
					step  = 2;
					size = this->m_option.winsz;
					continue;
				}
				else
					size.cy = size.cy - (UINT16)this->m_option.vstep;
				if(size.cy < tsize.cy)
					size.cy = tsize.cy;
			}
		}

		if(step == 2)
		{
			if((this->m_option.hstep == 0) && (this->m_option.vstep == 0))
				break;
			if(dir)
			{
				if((size.cx >= tsize.cx) && (size.cy >= tsize.cy))
					break;
				if(size.cx < tsize.cx)
					size.cx = size.cx + (UINT16)this->m_option.hstep;
				if(size.cx > tsize.cx)
					size.cx = tsize.cx;
				if(size.cy < tsize.cy)
					size.cy = size.cy + (UINT16)this->m_option.vstep;
				if(size.cy > tsize.cy)
					size.cy = tsize.cy;
			}
			else
			{
				if((size.cx <= tsize.cx) && (size.cy <= tsize.cy))
					break;
				if(size.cx > tsize.cx)
					size.cx = size.cx - (UINT16)this->m_option.hstep;
				if(size.cx < tsize.cx)
					size.cx = tsize.cx;
				if(size.cy > tsize.cy)
					size.cy = size.cy - (UINT16)this->m_option.vstep;
				if(size.cy < tsize.cy)
					size.cy = tsize.cy;
			}
		}

		V5_IppSetCapSizer(size);
		V5_JpegAutoSetImage();
		V5_MarbAutoSetMem();

		count ++;
		sprintf(str, "%d", count);
		strcpy(this->m_option.dstfile, path);
		for(i = 0; i < 4; i++)
		{
			if(count > fnum[i])
			{
				strcat(this->m_option.dstfile, fname[step][i]);
				break;
			}
			if(i == 3)
				strcat(this->m_option.dstfile, fname[step][4]);
		}
		strcat(this->m_option.dstfile, str);
		strcat(this->m_option.dstfile, ".jpg");

		this->Capture(this->m_option.sifcap);
	}
}

void CV558DemoDlg::OnBnClickedCheckRecap()
{
	if(m_recap.GetCheck())
	{
		if(this->m_sifCapBtn.GetCheck())
		{
			this->m_sifCapBtn.SetCheck(0);
			V5B_SifEnableSyncGen();
		}
		m_recapcnt.EnableWindow(TRUE);
	}
	else
	{
		m_recapcnt.EnableWindow(FALSE);
	}


}

void CV558DemoDlg::OnBnClickedRadioVidStopHost()
{
	m_vidstoptimecnt.EnableWindow(FALSE);
}

void CV558DemoDlg::OnBnClickedRadioVidStopTime()
{
	m_vidstoptimecnt.EnableWindow(TRUE);
}

void CV558DemoDlg::OnBnClickedCheckSimpHead()
{
	if(this->m_simphead.GetCheck())
	{
		V5B_JpegSimHeadEnable();
	}
	else
	{
		V5B_JpegSimHeadDisable();
	}
}

void CV558DemoDlg::OnBnClickedCheckAvi()
{
	if(this->m_avi.GetCheck())
	{
		V5B_JpegAviFmtEnable();
	}
	else
	{
		V5B_JpegAviFmtDisable();
	}
}

void CV558DemoDlg::OnBnClickedCheckDecWrData()
{
	if(this->m_decwrdata.GetCheck())
	{
		m_ctlDispSize.EnableWindow(FALSE);
	}
	else
	{
		m_ctlDispSize.EnableWindow(TRUE);
	}
}
#endif //V578CMD_DISABLE




