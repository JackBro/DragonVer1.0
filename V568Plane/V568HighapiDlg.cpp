// V568UsbcDlg.cpp : 实现文件
//

#include "internal.h"



#ifndef	V578CMD_DISABLE


// CV568HighapiDlg 对话框

IMPLEMENT_DYNAMIC(CV568HighapiDlg, CBaseCtrlDlg)
CV568HighapiDlg::CV568HighapiDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568HighapiDlg::IDD, pParent)
{
}

CV568HighapiDlg::~CV568HighapiDlg()
{
	 
}

void CV568HighapiDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SNRSIZE_COMBO, m_sensorresbox);
	DDX_Control(pDX, IDC_ADJUST_COMBO, m_adjustprtybox);
	DDX_Control(pDX, IDC_FRAME_CHECK, m_withframe1);
	DDX_Control(pDX, IDC_FRAME_CHECK2, m_withframe2);
	DDX_Control(pDX, IDC_WITHTHUMB_CHECK, m_withthumb);
	DDX_Control(pDX, IDC_AF_CHECK, m_af);
	DDX_Control(pDX, IDC_MSHOTCOUNT_EDIT, m_mshotcountbox);
	DDX_Control(pDX, IDC_MSHOTINTERVAL_EDIT, m_mshotintervalbox);
	//DDX_Control(pDX, IDC_JFILESIZE_EDIT, m_jpegfilesizebox);
	//DDX_Control(pDX, IDC_JCOMPRATIO_EDIT, m_jpegcompratiobox);
	DDX_Control(pDX, IDC_VIDEOLENGTH_EDIT,m_initvideolengthbox);
	DDX_Control(pDX, IDC_AE_COMBO, m_aebox);
	DDX_Control(pDX, IDC_FLASH_COMBO, m_flashbox);
	DDX_Control(pDX, IDC_SPECIALEFFECT_COMBO, m_specialeffectbox);
	DDX_Control(pDX, IDC_SPECIALEFFECT_COMBO2, m_specialeffectbox2);
	DDX_Control(pDX, IDC_ROTATION_COMBO, m_rotationbox);
	DDX_Control(pDX, IDC_AWB_COMBO, m_awbbox);
	DDX_Control(pDX, IDC_SCENCE_COMBO, m_scenebox); 
	DDX_Control(pDX, IDC_CAPSIZE_COMBO, m_capsizebox);
	//DDX_Control(pDX, IDC_JCOMPRESS_COMBO, m_jcompresstypebox);
	DDX_Control(pDX, IDC_JCOMPRESS_COMBO2,m_jcompresstypebox);
	DDX_Control(pDX, IDC_JCOMPRESS_EDIT, m_jcompressvaluebox);
	DDX_Control(pDX, IDC_FRAMEITEM_COMBO, m_frameitembox);
	DDX_Control(pDX, IDC_SIZER_SCROLLBAR, m_sizerbar);
	DDX_Control(pDX, IDC_ARMDEMO_CHECK, m_pcdemobtn);
}


BEGIN_MESSAGE_MAP(CV568HighapiDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_V5OPEN_BUTTON, OnBnClickedV5openButton)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_UP_BUTTON, OnBnClickedUpButton)
	ON_BN_CLICKED(IDC_DOWN_BUTTON, OnBnClickedDownButton)
	ON_BN_CLICKED(IDC_CAP_PHOTO_BUTTON, OnBnClickedCapPhotoButton)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_SNRSIZE_COMBO, OnCbnSelchangeSnrsizeCombo)
	ON_CBN_SELCHANGE(IDC_ADJUST_COMBO, OnCbnSelchangeAdjustCombo)
	ON_BN_CLICKED(IDC_FRAME_CHECK, OnBnClickedFrameCheck)
	ON_BN_CLICKED(IDC_WITHTHUMB_CHECK, OnBnClickedWiththumbCheck)
	ON_BN_CLICKED(IDC_AF_CHECK, OnBnClickedAfCheck)
	ON_EN_CHANGE(IDC_MSHOTINTERVAL_EDIT, OnEnChangeMshotintervalEdit)
	ON_BN_CLICKED(IDC_CAP_SHOT_BUTTON, OnBnClickedCapShotButton)
	ON_BN_CLICKED(IDC_CAP_VIDEO_BUTTON, OnBnClickedCapVideoButton)
	ON_CBN_SELCHANGE(IDC_AE_COMBO, OnCbnSelchangeAeCombo)
	ON_EN_CHANGE(IDC_VIDEOLENGTH_EDIT, OnEnChangeVideolengthEdit)
	ON_CBN_SELCHANGE(IDC_SPECIALEFFECT_COMBO, OnCbnSelchangeSpecialeffectCombo)
	ON_CBN_SELCHANGE(IDC_ROTATION_COMBO, OnCbnSelchangeRotationCombo)
	ON_CBN_SELCHANGE(IDC_FLASH_COMBO, OnCbnSelchangeFlashCombo)
	ON_CBN_SELCHANGE(IDC_CAPSIZE_COMBO, OnCbnSelchangeCapsizeCombo)
	//ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_JCOMPCONFIRM_BUTTON, OnBnClickedJcompconfirmButton)
	//ON_BN_CLICKED(IDC_DISPLAY_BUTTON, OnBnClickedDisplayButton)
	//ON_CBN_SELCHANGE(IDC_JCOMPRESS_COMBO, OnCbnSelchangeJcompressCombo) 
	ON_BN_CLICKED(IDC_HAPIREFRESH_BUTTON, OnBnClickedHapirefreshButton)
	ON_BN_CLICKED(IDC_DISPLAYSTILL_BUTTON, OnBnClickedDisplaystillButton)
	ON_BN_CLICKED(IDC_DISPLAYVIDEO_BUTTON, OnBnClickedDisplayvideoButton)
	ON_BN_CLICKED(IDC_PPPCONFIRM_BUTTON, OnBnClickedPppconfirmButton)
	ON_BN_CLICKED(IDC_FRAME_CHECK2, OnBnClickedFrameCheck2)
	ON_CBN_SELCHANGE(IDC_SPECIALEFFECT_COMBO2, OnCbnSelchangeSpecialeffectCombo2)
	ON_BN_CLICKED(IDC_DECODEIPPSAVE_BUTTON, OnBnClickedDecodeippsaveButton)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_FRAMEITEM_COMBO, OnCbnSelchangeFrameitemCombo)
	ON_CBN_SELCHANGE(IDC_JCOMPRESS_COMBO2, OnCbnSelchangeJcompressCombo2)
	ON_BN_CLICKED(IDC_ARMDEMO_CHECK, OnBnClickedArmdemoCheck)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_PREVIEW_BUTTON, OnBnClickedPreviewButton)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
END_MESSAGE_MAP()


// CV568HighapiDlg 消息处理程序


void CV568HighapiDlg::OnBnClickedCapstillButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CV568HighapiDlg::OnBnClickedCapvideoButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CV568HighapiDlg::OnBnClickedV5openButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CWaitCursor cursor;

	int i = 0; 
	char snrinfo[10],snrsizecount;
	char strwidth[10],strheighth[10],strsnrsize[20];
	//char str[0x100];
	

	//V5_SendCommand(V568_OPEN_DEFAULT, NULL, 0);


		/*sprintf(str, ""V5_SendCommand %d %x %d", V568_OPEN_WITH_CONTEXT, 0, 0 ):
		SendCmd(str);*/
	V5_SendCommand_Auto(V568_OPEN_WITH_CONTEXT, NULL, 0);

	/*get sensor resolution info */
	V5_SendCommand_Auto(V5M_GETCFGSNRINFO,snrinfo, 10);
	snrsizecount = *snrinfo;
	this->m_sensorresbox.ResetContent();
	for(i = 1; i<=snrsizecount; i++)
	{
		TgetSnrSizeByIndexParam param;

		strwidth [0]= '\0';
		strheighth[0] = '\0';
		strsnrsize[0] = '\0';
		param.index = snrinfo[i];
		snrres[i-1] = param.index;
		V5_SendCommand_Auto(V5M_GETCFGSNRSIZEBYINDEX, &param,sizeof(TgetSnrSizeByIndexParam)); 

		sprintf(strwidth,"%d", param.size.cx);
		sprintf(strheighth,"%d", param.size.cy);
		strcpy(strsnrsize, strwidth);
		strcat(strsnrsize, " * ");
		strcat(strsnrsize, strheighth);
		this->m_sensorresbox.InsertString(i-1,strsnrsize);
		
	}
	//this->m_sensorresbox.set
		
	this->OnBnClickedHapirefreshButton();
}

void CV568HighapiDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CV568HighapiDlg::OnBnClickedUpButton()
{
	switch (this->m_option.adjustprty)
	{
	case V5M_DLG_DIGITALZOOM:
		V5_SendCommand_Auto(V5M_CAMERAZOOMOUT, NULL, 0);
		break;
	case V5M_DLG_BRIGHTNESS:
		V5_SendCommand_Auto(V5M_BRIGHTNESSUP, NULL, 0);
		break;
	case V5M_DLG_CONTRAST:
		V5_SendCommand_Auto(V5M_CONTRASTUP, NULL, 0);
		break;
	case V5M_DLG_BLENDRATIO:
		V5_SendCommand_Auto(V5M_BLENDRATIOUP, NULL, 0);
	default:
		break;
	}
	
}



void CV568HighapiDlg::OnBnClickedDownButton()
{
	switch (this->m_option.adjustprty)
	{
	case V5M_DLG_DIGITALZOOM:
		V5_SendCommand_Auto(V5M_CAMERAZOOMIN, NULL, 0);
		break;
	case V5M_DLG_BRIGHTNESS:
		V5_SendCommand_Auto(V5M_BRIGHTNESSDOWN, NULL, 0);
		break;
	case V5M_DLG_CONTRAST:
		V5_SendCommand_Auto(V5M_CONTRASTDOWN, NULL, 0);
		break;
	case V5M_DLG_BLENDRATIO:	
		V5_SendCommand_Auto(V5M_BLENDRATIODOWN, NULL, 0);
	default:
		break;
	}

}

void CV568HighapiDlg::GetUserOption(char *tail)
{
	this->GetComboValue(tail);
	//this->GetRadioValue();
	//this->GetCheckBtnValue();
	//this->GetBarValue();
	this->GetEditBoxValue();
	this->GetBarValue();
	//CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile);

}


void CV568HighapiDlg::GetEditBoxValue(void)
{
	char str[20];
	int x;

	this->m_mshotcountbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.mshotcount =(UINT8)x;

	this->m_mshotintervalbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.mshotinterval =(UINT8)x;

	//this->m_jpegcompratiobox.GetWindowText(str, 20);
	//sscanf(str, "%d", &x);
	//this->m_option.mshotcount =(UINT8)x;

	//this->m_jpegfilesizebox.GetWindowText(str, 20);
	//sscanf(str, "%d", &x);
	//this->m_option.jpegfilesize =(UINT8)x;

	this->m_initvideolengthbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.initvideolength =(UINT8)x;

	this->m_jcompressvaluebox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	switch (this->m_option.compresssizetype)
	{
	case 1:
		this->m_option.jpegcompratio =(UINT8)x;
		break;
	case 2:
		this->m_option.jpegfilesize =(UINT16)x;
		break;
	default:
		break;
	}
	/*if (this->m_option.choosecomprate)
		this->m_option.jpegcompratio =(UINT8)x;
	else
		this->m_option.jpegfilesize =(UINT16)x;*/
}


void CV568HighapiDlg::GetBarValue(void)
{
	this->m_option.sizerratio = (UINT8) this->m_sizerbar.GetScrollPos();
}

void CV568HighapiDlg::GetComboValue(char *tail)
{
	this->m_SrcfCombo.GetWindowText(this->m_option.srcfile, DEFAULT_FILENAME_LEN);
	this->m_option.adjustprty = (UINT8) this->m_adjustprtybox.GetCurSel();
	this->m_option.scenemode = (UINT8) this->m_scenebox.GetCurSel();
	this->m_option.awbmode = (UINT8) this->m_awbbox.GetCurSel();
	this->m_option.aemode = (UINT8) this->m_aebox.GetCurSel();
	this->m_option.flashmode = (UINT8) this->m_flashbox.GetCurSel();
	this->m_option.specEffect = (UINT8) this->m_specialeffectbox.GetCurSel();
	
	this->m_option.rotation = (UINT8) this->m_rotationbox.GetCurSel();
	this->m_option.capsize = (UINT8) this->m_capsizebox.GetCurSel();
	this->m_option.compresssizetype = (UINT8) this->m_jcompresstypebox.GetCurSel();
	/*if (this->m_jcompresstypebox.GetCurSel())
		this->m_option.choosecomprate = 0;
	else
		this->m_option.choosecomprate = 1;*/
	this->m_option.frameindex = (UINT8) this->m_frameitembox.GetCurSel();
	
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CV568HighapiDlg::InitUserOption(void)
{
	int i = 0; 
	const char *adjustprtystr[] = {"digital zoom" ,"brightness" ,"contrast" ,"blending"};
	const char *scenemodestr[] = {"NORMAL","NIGHT","PORTRAIT","NIGHT_PORTRAIT","LANDSCAPE","SNOW","SAND","BEACH","MOTION","FIREWORKS	"};
	const char *speceffectstr[] = {"NOEFFECT","SEPHIA", "SPECIALCOLOR","NEGATIV","SKETCH","RELIEF","MONOCHROME"};
	const char *rotationstr[] = {"0","90","180","270"};
	const char *awbstr[] = {"AUTO","SUNLIGHT","INCANDESCENT","FLUORESCENT","INDOOR"};
	const char *aestr[] = {"AUTO","OUTDOOR","INDOOR_50HZ","INDOOR_60HZ","NIGHT"};
	const char *flashstr[] = {"AUTO","ALWAYSOPEN","ALWAYSCLOSE"};
	//const char *capsizestr[] = {"AUTO","SXGA (1280 x 960)", "XGA (1024 x 768)", "SVGA (800 x 600)",
	//									"VGA (640 x 480)", "CIF( 352 x 288)", "SIF (320 x 240)",
	//									"QCIF (176 x 144)", "QSIF(160 x 120)","LCDPANELSIZE" };
	const char *capsizestr[] = {"Auto (change after digital zoom)", "LCD PANEL SIZE"};
	const char *compresstypestr[] = {"Default","compress ratio", "Jpeg filesize"};
	const char *frameitemstr[] ={"blue rect" ,"butterfly"};


	memset( &this->m_option, 0, sizeof(THapiOption) );

	this->m_option.adjustprty = 0;
	this->m_option.frameindex = 0;
	this->m_option.sizerratio = 10;
	strcpy(&m_option.frame[0][0], "F:/currentwork/VmonitorVer1.0/flash/15b_frame.bmp" );
	strcpy(&m_option.frame[1][0], "F:/currentwork/VmonitorVer1.0/flash/15b_butterfly.bmp" );
	strcpy(&m_option.frame_arm[0][0], "/flash/15b_frame_1.bmp" );
	strcpy(&m_option.frame_arm[1][0], "/flash/15b_frame_1.bmp" );
	//strcpy(this->m_option.dstfile, "c:\\temp\\dst");


	//const char *adjustprtystr[] = {"digital zoom" ,"brightness" ,"contrast"};
	
	for(i = 0; i < sizeof(adjustprtystr) / sizeof(char *); i++)
		this->m_adjustprtybox.InsertString(i, adjustprtystr[i]);

	for(i = 0; i < sizeof(scenemodestr) / sizeof(char *); i++)
		this->m_scenebox.InsertString(i, scenemodestr[i]);

	for(i = 0; i < sizeof(speceffectstr) / sizeof(char *); i++)
	{
		this->m_specialeffectbox.InsertString(i, speceffectstr[i]);
		this->m_specialeffectbox2.InsertString(i, speceffectstr[i]);
	}

	for(i = 0; i < sizeof(rotationstr) / sizeof(char *); i++)
		this->m_rotationbox.InsertString(i, rotationstr[i]);

	for(i = 0; i < sizeof(awbstr) / sizeof(char *); i++)
		this->m_awbbox.InsertString(i, awbstr[i]);

	for(i = 0; i < sizeof(aestr) / sizeof(char *); i++)
		this->m_aebox.InsertString(i, aestr[i]);

	for(i = 0; i < sizeof(flashstr) / sizeof(char *); i++)
		this->m_flashbox.InsertString(i, flashstr[i]);

	for(i = 0; i < sizeof(capsizestr) / sizeof(char *); i++)
		this->m_capsizebox.InsertString(i, capsizestr[i]);

	for(i = 0; i < sizeof(compresstypestr) / sizeof(char *); i++)
		this->m_jcompresstypebox.InsertString(i, compresstypestr[i]);

	for(i = 0; i < sizeof(frameitemstr) / sizeof(char *); i++)
		this->m_frameitembox.InsertString(i, frameitemstr[i]);
	
	this->m_sizerbar.SetScrollRange(1, 10);
	this->m_sizerbar.SetScrollPos(m_option.sizerratio);
}

void CV568HighapiDlg::DispOption()
{
	char str[20];

	this->m_capsizebox.SetCurSel(this->m_option.capsize);
	this->m_scenebox.SetCurSel(this->m_option.scenemode);
	//if ( V5M_NOEFFECT == this->m_option.specEffect)
	//	this->m_specialeffectbox.SetCurSel(0);
	//else 
		this->m_specialeffectbox.SetCurSel(this->m_option.specEffect );
		this->m_specialeffectbox2.SetCurSel(this->m_option.specEffect );
	this->m_rotationbox.SetCurSel(this->m_option.rotation);
	this->m_awbbox.SetCurSel(this->m_option.awbmode);
	this->m_aebox.SetCurSel(this->m_option.aemode);
	this->m_flashbox.SetCurSel(this->m_option.flashmode);
	/*if (this->m_option.choosecomprate )
		this->m_jcompresstypebox.SetCurSel(0);
	else 
		this->m_jcompresstypebox.SetCurSel(1);*/
	//this->m_jcompresstypebox.SetCurSel(this->m_option.choosecomprate );
	this->m_jcompresstypebox.SetCurSel(this->m_option.compresssizetype);
	this->m_adjustprtybox.SetCurSel(this->m_option.adjustprty);
	this->m_sensorresbox.SetCurSel(this->m_option.sensorres);
	this->m_frameitembox.SetCurSel(this->m_option.frameindex);
	//this->m_option.thumbres
	switch (this->m_option.compresssizetype)
	{
	case 0:
		this->m_jcompressvaluebox.EnableWindow(FALSE);
		this->m_jcompressvaluebox.SetWindowText("");
		break;
	case 1:
		this->m_jcompressvaluebox.EnableWindow(TRUE);
		sprintf(str, "%d", this->m_option.jpegcompratio);
		this->m_jcompressvaluebox.SetWindowText(str);
		break;
	case 2:
		this->m_jcompressvaluebox.EnableWindow(TRUE);
		sprintf(str, "%d", this->m_option.jpegfilesize);
		this->m_jcompressvaluebox.SetWindowText(str);
		break;
	default:
		break;
	}
	/*if (this->m_option.choosecomprate)
		sprintf(str, "%d", this->m_option.jpegcompratio);
	else
		sprintf(str, "%d", this->m_option.jpegfilesize);
	this->m_jcompressvaluebox.SetWindowText(str);*/
	
	sprintf(str, "%d", this->m_option.mshotcount);
	this->m_mshotcountbox.SetWindowText(str);

	sprintf(str, "%d", this->m_option.mshotinterval);
	this->m_mshotintervalbox.SetWindowText(str);

	sprintf(str, "%d", this->m_option.initvideolength);
	this->m_initvideolengthbox.SetWindowText(str);

	this->m_sizerbar.SetScrollPos(m_option.sizerratio);
}


BOOL CV568HighapiDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	//this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}


void CV568HighapiDlg::OnBnClickedCapPhotoButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CWaitCursor cursor;
	TgetCaptureInfoToFileParam param;
	char armfile[200];
		
	this->GetUserOption(".jpg");
	if (this->m_option.armdemoen)  
	{
		sprintf( armfile, "/flash/%s", this->m_option.dstfile);
		param.filename = armfile;
	}
	else
		param.filename = this->m_option.dstfile;
	param.bufsizeOnetime = 0x10000;

	V5_SendCommand_Auto(V5M_CAPSTILL_START, NULL, 0);
	V5_SendCommand_Auto(V5M_GETCAPTUREINFO_TOFILE,&param,sizeof(TgetCaptureInfoToFileParam));
}

void CV568HighapiDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CV568HighapiDlg::OnCbnSelchangeSnrsizeCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetUserOption(".jpg");
	V5_SendCommand_Auto(V5M_SETSNRRESOINDEX, &snrres[m_option.sensorres], sizeof(snrres[m_option.sensorres]));
}

void CV568HighapiDlg::OnCbnSelchangeAdjustCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetUserOption(".jpg");
}




void CV568HighapiDlg::OnBnClickedFrameCheck()
{
	CWaitCursor cursor;
	char *filename;

	this->GetUserOption(".jpg");

	if(this->m_withframe1.GetCheck())
	{
		if (m_option.armdemoen)
			filename = &(m_option.frame_arm[m_option.frameindex][0]);
		else
			filename = &(m_option.frame[m_option.frameindex][0]);
		V5_SendCommand_Auto( V5M_FRAMEON, filename, (UINT32) strlen(filename) + 1);
		this->m_withframe2.SetCheck(BST_CHECKED);
	}
	else
	{
		V5_SendCommand_Auto( V5M_FRAMEOFF, NULL, 0);
		this->m_withframe2.SetCheck(BST_UNCHECKED);
	}
}

void CV568HighapiDlg::OnBnClickedWiththumbCheck()
{
	if(this->m_withthumb.GetCheck())
		V5_SendCommand_Auto( V5M_THUMBON, NULL, 0);
	else
		V5_SendCommand_Auto( V5M_THUMBOFF, NULL, 0);
}

void CV568HighapiDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CV568HighapiDlg::OnBnClickedAfCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->m_af.GetCheck())
		V5_SendCommand_Auto( V5M_THUMBON, NULL, 0);
	else
		V5_SendCommand_Auto( V5M_THUMBOFF, NULL, 0);

}

void CV568HighapiDlg::OnEnChangeMshotintervalEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CBaseCtrlDlg::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CV568HighapiDlg::OnBnClickedCapShotButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CWaitCursor cursor;
	TgetCaptureInfoToFileParam param;
	char armfile[200];
		
	this->GetUserOption(".jpg");
	if (this->m_option.armdemoen)  
	{
		sprintf( armfile, "/flash/%s", this->m_option.dstfile);
		param.filename = armfile;
	}
	else
		param.filename = this->m_option.dstfile;
	param.bufsizeOnetime = 0x80000;

	//V5_SendCommand_Auto(V5M_SETFRAMEINTERVAL, &m_option.mshotinterval, sizeof(m_option.mshotinterval));
	V5_SendCommand_Auto(V5M_CAPMULTISHOT_START, &m_option.mshotcount, sizeof(m_option.mshotcount));
	V5_SendCommand_Auto(V5M_GETCAPTUREINFO_TOFILE, &param,sizeof(TgetCaptureInfoToFileParam));
}

void CV568HighapiDlg::OnBnClickedCapVideoButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CWaitCursor cursor;
	TgetCaptureInfoToFileParam param;
	char armfile[200];
		
	this->GetUserOption(".jpg");
	if (this->m_option.armdemoen)  
	{
		sprintf( armfile, "/flash/%s", this->m_option.dstfile);
		param.filename = armfile;
	}
	else
		param.filename = this->m_option.dstfile;
	param.bufsizeOnetime = 0x80000;

	//V5_SendCommand_Auto(V5M_SETFRAMEINTERVAL, &m_option.mshotinterval, sizeof(m_option.mshotinterval));
	V5_SendCommand_Auto(V5M_SETVIDEOFILESIZE, &m_option.initvideolength, sizeof(m_option.initvideolength)); //set filelength
	V5_SendCommand_Auto(V5M_CAPVIDEO_START, NULL, 0);
	V5_SendCommand_Auto(V5M_GETCAPTUREINFO_TOFILE, &param,sizeof(TgetCaptureInfoToFileParam));
}

void CV568HighapiDlg::OnCbnSelchangeAeCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetUserOption(".jpg");

	V5_SendCommand_Auto(V5M_SETEXPOSEMODE, &m_option.aemode, sizeof(m_option.aemode));
}

void CV568HighapiDlg::OnEnChangeVideolengthEdit()
{

}

void CV568HighapiDlg::OnCbnSelchangeSpecialeffectCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT8 effect;
	this->GetUserOption(".jpg");

	if (m_option.specEffect)
	{
		effect = m_option.specEffect - 1;
		V5_SendCommand_Auto(V5M_SETSPECIALEFFECT, &effect, sizeof(effect));
	}
	else
		V5_SendCommand_Auto(V5M_SPECIALEFFECTOFF, NULL, 0);
}

void CV568HighapiDlg::OnCbnSelchangeRotationCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetUserOption(".jpg");

	V5_SendCommand_Auto(V5M_SETROTATEMODE, &m_option.rotation, sizeof(m_option.rotation));
}

void CV568HighapiDlg::OnCbnSelchangeFlashCombo()
{
	this->GetUserOption(".jpg");

	//V5_SendCommand(V5M_SETROTATEMODE, &m_option.flashmode, sizeof(m_option.flashmode));
}

void CV568HighapiDlg::OnCbnSelchangeCapsizeCombo()
{
	if (this->m_capsizebox.GetCurSel())
		V5_SendCommand_Auto(V5M_CAPASWALLPICON, NULL , 0);
	else
		V5_SendCommand_Auto(V5M_CAPASWALLPICOFF, NULL , 0);

}



void CV568HighapiDlg::OnBnClickedJcompconfirmButton()
{
	CWaitCursor cursor;

	this->GetUserOption(".jpg");
	switch (m_option.compresssizetype)
	{
	case 0:
		//V5_SendCommand_Auto();
		break;
	case 1:
		V5_SendCommand_Auto(V5M_SETCOMPRATIO, &m_option.jpegcompratio, sizeof(m_option.jpegcompratio));
		break;
	case 2:
		V5_SendCommand_Auto(V5M_SETJFILESIZE, &m_option.jpegfilesize, sizeof(m_option.jpegfilesize));
		break;
	default:
		break;
	}

	/*if (m_option.choosecomprate)
		V5_SendCommand(V5M_SETCOMPRATIO, &m_option.jpegcompratio, sizeof(m_option.jpegcompratio));
	else
		V5_SendCommand(V5M_SETJFILESIZE, &m_option.jpegfilesize, sizeof(m_option.jpegfilesize));*/
}



//void CV568HighapiDlg::OnCbnSelchangeJcompressCombo()
//{
//	//this->m_option.compresssizetype = (UINT8) this->m_jcompresstypebox.GetCurSel(); 
//	this->GetUserOption(".jpg");
//	this->DispOption();
//}



void CV568HighapiDlg::OnBnClickedHapirefreshButton()
{
	// TODO: 在此添加控件通知处理程序代码
	THapiStdInfo info;

	V5_SendCommand_Auto(V5M_GETHAPISTDINFO, &info, sizeof(THapiStdInfo));
	this->m_option.thumbres = info.thumbres;
	this->m_option.capsize =  (UINT8) (info.featureflag & V5M_BIT_CAPASWALLPIC);
	this->m_option.scenemode = info.scenemode;
	//this->m_option.specEffect = info.specEffect; 		/* Special Effect */
	if ( V5M_NOEFFECT == info.specEffect)
		this->m_option.specEffect = 0;
	else 
		this->m_option.specEffect = info.specEffect + 1;

	this->m_option.rotation = info.rotation;			/* Rotation */
	this->m_option.awbmode = info.awbmode;			/* white balance mode */
	this->m_option.aemode = info.aemode;				/* Auto Exposure */
	this->m_option.flashmode = info.flashmode;	
	
	//this->m_option.choosecomprate = ((UINT8) (info.featureflag & V5M_BIT_CHOOSECOMPRATE)) >> 7;		/* if true ,choose JPEG compression rate , else choose JPEG File size */
	this->m_option.compresssizetype = info.compresssizetype;
	this->m_option.jpegcompratio = info.jpegcomprate;		/* JPEG compression rate, value range(1-100),0 is invalid */
	this->m_option.jpegfilesize = info.jpegfilesize;		/* JPEG File size ,0 is invalid*/
	
	/*-------Setup only for capture multi-shot-----*/
	this->m_option.mshotcount = info.mshotcount;			/* multi-shot count */
	this->m_option.mshotinterval = info.mshotinterval;		/* capture interval */
	/*-------Setup/control only for capture video-----*/
	this->m_option.videores = info.videores;
	this->m_option.initvideolength = info.initvideolength;		/* real videolength can be smaller than this value when user stops capturing manually */
	
	this->DispOption();
}


void CV568HighapiDlg::OnBnClickedDisplaystillButton()
{
	CWaitCursor cursor;
	this->GetUserOption(".jpg");

	V5_SendCommand_Auto(V5M_DISPLAYSTILL, &m_option.srcfile, (UINT32) strlen(m_option.srcfile) + 1);
}

void CV568HighapiDlg::OnBnClickedDisplayvideoButton()
{
	CWaitCursor cursor;
	this->GetUserOption(".jpg");

	V5_SendCommand_Auto(V5M_DISPLAYVIDEO, &m_option.srcfile, (UINT32) strlen(m_option.srcfile) + 1);
}



void CV568HighapiDlg::OnBnClickedPppconfirmButton()
{
	CWaitCursor cursor;
	TDecodeFrameEncodeParam param;

	this->GetUserOption(".jpg");

	param.srcfile = m_option.srcfile;
	param.dstfile = m_option.dstfile;
	//V5_SendCommand(V5M_DECODEENCODE, &m_option.sizerratio, sizeof(m_option.sizerratio));

	V5_SendCommand_Auto(V5M_DECODEFRAMEENCODE, &param, sizeof(TDecodeFrameEncodeParam));
}



void CV568HighapiDlg::OnBnClickedFrameCheck2()
{
	CWaitCursor cursor;
	char *filename;

	this->GetUserOption(".jpg");

	if(this->m_withframe2.GetCheck())
	{
		if (m_option.armdemoen)
			filename = &(m_option.frame_arm[m_option.frameindex][0]);
		else
			filename = &(m_option.frame[m_option.frameindex][0]);

		V5_SendCommand_Auto( V5M_FRAMEON, filename, (UINT32) strlen(filename) + 1);
		this->m_withframe1.SetCheck(BST_CHECKED);
	}
	else
	{
		V5_SendCommand_Auto( V5M_FRAMEOFF, NULL, 0);
		this->m_withframe1.SetCheck(BST_UNCHECKED);
	}
}


void CV568HighapiDlg::OnBnClickedViewButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CV568HighapiDlg::OnCbnSelchangeSpecialeffectCombo2()
{
	UINT8 effect;
	//this->GetUserOption(".jpg");

	this->m_option.specEffect = (UINT8) this->m_specialeffectbox2.GetCurSel();
	if (m_option.specEffect)
	{
		effect = m_option.specEffect - 1;
		V5_SendCommand_Auto(V5M_SETSPECIALEFFECT, &effect, sizeof(effect));
	}
	else
		V5_SendCommand_Auto(V5M_SPECIALEFFECTOFF, NULL, 0);

	this->DispOption();
}


void CV568HighapiDlg::OnBnClickedDecodeippsaveButton()
{
	CWaitCursor cursor;
	TDecodeIPPEncodeParam param;

	this->GetUserOption(".jpg");

	param.srcfile = m_option.srcfile;
	param.dstfile = m_option.dstfile;
	param.ratio = m_option.sizerratio;
	//V5_SendCommand(V5M_DECODEENCODE, &m_option.sizerratio, sizeof(m_option.sizerratio));

	V5_SendCommand_Auto(V5M_DECODEIPPENCODE, &param, sizeof(TDecodeIPPEncodeParam));
}



void CV568HighapiDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWaitCursor cursor;
	int pos;

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
		case SB_THUMBPOSITION:						// operation.
			pScrollBar->SetScrollPos(nPos);
			break;
		case SB_THUMBTRACK:   
			pScrollBar->SetScrollPos(nPos);					
			break;
		default:
			break;
		}
		this->GetBarValue();
		this->DispOption();
	}

	CBaseCtrlDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CV568HighapiDlg::OnCbnSelchangeFrameitemCombo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CV568HighapiDlg::OnCbnSelchangeJcompressCombo2()
{
	this->m_option.compresssizetype = (UINT8) this->m_jcompresstypebox.GetCurSel();
	this->DispOption();
}


void CV568HighapiDlg::OnBnClickedPcdemoCheck()
{


}

void CV568HighapiDlg::OnBnClickedArmdemoCheck()
{
	if(this->m_pcdemobtn.GetCheck())	//if checked, cmd will be sent to arm
	{
		this->m_option.armdemoen = 1;
	}
	else
	{
		this->m_option.armdemoen = 0;
	}
}

UINT32 CV568HighapiDlg::V5_SendCommand_Auto(UINT32 msg, void *param, UINT32 paramsize)
{
	if (this->m_option.armdemoen )
		return V5_SendCommand_(msg, param, paramsize);
	else return V5_SendCommand(msg, param, paramsize);
}

void CV568HighapiDlg::OnBnClickedButton1()
{


}


void CV568HighapiDlg::OnBnClickedPreviewButton()
{
	V5_SendCommand_Auto(V5M_PREVIEWAUTO, NULL, 0 );
}

void CV568HighapiDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
}

#endif //V578CMD_DISABLE

