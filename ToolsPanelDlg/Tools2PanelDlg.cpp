// Tools2PanelDlg.cpp : 实现文件
//

#include "internal.h"




// CTools2PanelDlg 对话框

IMPLEMENT_DYNAMIC(CTools2PanelDlg, CBasePanelDlg)
CTools2PanelDlg::CTools2PanelDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CTools2PanelDlg::IDD, pParent)
{
}

CTools2PanelDlg::~CTools2PanelDlg()
{
}

void CTools2PanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WIDTH2_EDIT, m_width);
	DDX_Control(pDX, IDC_HEIGHT2_EDIT, m_height);
	DDX_Control(pDX, IDC_DESTFORMAT2_COMBO, m_dstfmt);
	DDX_Control(pDX, IDC_BYTEPERDATA2_COMBO, m_bytepdata);
	DDX_Control(pDX, IDC_SRCFORMAT2_COMBO, m_srcfmt);
	DDX_Control(pDX, IDC_DSTWIDTH2_EDIT, m_dstwidth);
	DDX_Control(pDX, IDC_DSTHEIGHT2_EDIT, m_dstheight);
	DDX_Control(pDX, IDC_DITHER2_CHECK, m_dither);
	DDX_Control(pDX, IDC_BMPFMT2_EDIT, m_Bmpfmt);
	DDX_Control(pDX, IDC_BMPWIDTH2_EDIT, m_BmpWidth);
	DDX_Control(pDX, IDC_BMPHEIGHT2_EDIT, m_BmpHeight);
	DDX_Control(pDX, IDC_FACTOREN2_CHECK, m_factoren);
	DDX_Control(pDX, IDC_SRCVAL_EDIT, m_srcucvalBox);
	DDX_Control(pDX, IDC_DSTVAL_EDIT, m_dstucvalBox);
	DDX_Control(pDX, IDC_QF_EDIT, m_qfactorBox);

	DDX_Control(pDX, IDC_HFACTOR2_EDIT, m_hfactorBox);
	DDX_Control(pDX, IDC_VFACTOR2_EDIT, m_vfactorBox);
	DDX_Control(pDX, IDC_FILTER02_EDIT, m_filter0Box);
	DDX_Control(pDX, IDC_FILTER02_EDIT2, m_filter1Box);
	DDX_Control(pDX, IDC_FILTER02_EDIT3, m_filter2Box);
	DDX_Control(pDX, IDC_FILTER02_EDIT4, m_filter3Box);
	DDX_Control(pDX, IDC_POINTX2_EDIT, m_pointxBox);
	DDX_Control(pDX, IDC_POINTY2_EDIT, m_pointyBox);
	DDX_Control(pDX, IDC_ROTATEMODE2_COMBO, m_rotmodebox);
	DDX_Control(pDX, IDC_SRCFILE2_COMBO2, m_srcfile2box);
	DDX_Control(pDX, IDC_FRAMERATE2_COMBO, m_frmrateBox);
	DDX_Control(pDX, IDC_STARTFRAME2_EDIT, m_startfrmBox);
	DDX_Control(pDX, IDC_ENDFRAME2_EDIT, m_endfrmBox);
	DDX_Control(pDX, IDC_IMGOPEN2_BUTTON, m_imgopenbtn);
	DDX_Control(pDX, IDC_IMGPLAY2_BUTTON, m_imgplaybtn);

	DDX_Control(pDX, IDC_MAXYR_EDIT, m_maxYR);
	DDX_Control(pDX, IDC_MINYR_EDIT, m_minYR);
	DDX_Control(pDX, IDC_AVGYR_EDIT, m_avgYR);
	DDX_Control(pDX, IDC_MAXUG_EDIT, m_maxUG);
	DDX_Control(pDX, IDC_MINUG_EDIT, m_minUG);
	DDX_Control(pDX, IDC_AVGUG_EDIT, m_avgUG);
	DDX_Control(pDX, IDC_MAXVB_EDIT, m_maxVB);
	DDX_Control(pDX, IDC_MINVB_EDIT, m_minVB);
	DDX_Control(pDX, IDC_AVGVB_EDIT, m_avgVB);
}


BEGIN_MESSAGE_MAP(CTools2PanelDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_TOOL2MISC_BUTTON, OnBnClickedTool2miscButton)
	ON_BN_CLICKED(IDC_PRINTBMPINFO2_BUTTON, OnBnClickedPrintbmpinfoButton)
	ON_BN_CLICKED(IDC_COMPAREIMG2_BUTTON, OnBnClickedCompareimgButton)
	ON_BN_CLICKED(IDC_IMGSNR2_BUTTON, OnBnClickedImgsnrButton)
	ON_BN_CLICKED(IDC_SPLITJPG2_BUTTON, OnBnClickedSplitjpgButton)
	ON_BN_CLICKED(IDC_CHKJPGLIST2_BUTTON, OnBnClickedChkjpglistButton)
	ON_BN_CLICKED(IDC_DATACONVERT2_BUTTON, OnBnClickedDataconvertButton)
	ON_BN_CLICKED(IDC_DATARESIZE2_BUTTON, OnBnClickedDataresizeButton)
	ON_BN_CLICKED(IDC_DATASIZER2_BUTTON, OnBnClickedDatasizerButton)
	ON_BN_CLICKED(IDC_UNITCVT2_BUTTON2, OnBnClickedUnitcvtButton)
	ON_BN_CLICKED(IDC_PREFILTER2_BUTTON, OnBnClickedPrefilterButton)
	ON_BN_CLICKED(IDC_ROTATE2_BUTTON, OnBnClickedRotateButton)
	ON_BN_CLICKED(IDC_OPENSRCFILE2_BTN2, OnBnClickedOpensrcfile2Btn)
	ON_BN_CLICKED(IDC_IMGPLAY2_BUTTON, OnBnClickedImgplayButton)
	ON_BN_CLICKED(IDC_IMGOPEN2_BUTTON, OnBnClickedImgopenButton)
	ON_BN_CLICKED(IDC_IMGSTAT_BUTTON, OnBnClickedImgstatButton)
END_MESSAGE_MAP()


// CTools2PanelDlg 消息处理程序

BOOL CTools2PanelDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	this->InitUserOption();
	this->m_Imgfrm.Create(this);

	return TRUE;  // return TRUE unless you set the focus to a control
}

//To be modified.
void CTools2PanelDlg::OnBnClickedTool2miscButton()
{
	//CDirTools tools;
	//char path[100] = "aaaa";

	//tools.DeleteDir(path);
}


void CTools2PanelDlg::OnBnClickedPrintbmpinfoButton()
{
	CWaitCursor cursor;
	CGeneralTools tools;
	//CJpegF tjpg;
	//CMpeg2IF tmpg;
	TBitMap bmpinfo;
	TJpegIndex info;
	TMpeg2Option mpginfo;
	TMpeg4Option mpg4info;
	char str[20];
	const char *rgbfmt[] = { "RGB1", "RGB2", "RGB4", "RGB8", "RGB444", "RGB555", 
				"RGB565", "RGB666", "RGB24", "RGB32", "Unknown" };
	const int mpgfmt[] = { YUV420BLK, YUV422M2BLK, YUV444M2BLK };

	this->GetUserOption(FILE_TAIL_TXT);
	tools.PrintBmpFileInfo(this->m_option.src, this->m_option.dest, &bmpinfo);
	if(bmpinfo.index > RGB32)
	{
		bmpinfo.width = 0;
		bmpinfo.height = 0;
		tools.JpegParse(this->m_option.src, &info);
		if(info.YUVType != JPEG_UNKNOWN)
		{
			this->m_Bmpfmt.SetWindowText(gJpegFmt[info.YUVType]);
			bmpinfo.width = info.JpgSize.cx;
			bmpinfo.height = info.JpgSize.cy;
			this->m_srcfmt.SetCurSel(JPEG422 + info.YUVType);
			this->m_dstfmt.SetCurSel(YUV422BLK + info.YUVType);
		}
		else
		{
			tools.Mpeg2Parse(this->m_option.src, &mpginfo);
			if( (mpginfo.chroma >= CHROMT_420) && (mpginfo.chroma <= CHROMT_444) )
			{
				this->m_Bmpfmt.SetWindowText(gImageStreamType[mpginfo.chroma + MPEG2IFRM420 - 1]);
				bmpinfo.width = mpginfo.srcwidth;
				bmpinfo.height = mpginfo.srcheight;
				this->m_srcfmt.SetCurSel(mpginfo.chroma + MPEG2IFRM420 - 1);
				this->m_dstfmt.SetCurSel(mpgfmt[mpginfo.chroma-1]);
			}
			else
			{
				tools.Mpeg4Parse(this->m_option.src, &mpg4info);
				if (mpg4info.chroma == CHROMT_420)
				{
					this->m_Bmpfmt.SetWindowText(gImageStreamType[MPEG4_FRM]);
					bmpinfo.width = mpg4info.srcwidth;
					bmpinfo.height = mpg4info.srcheight;
					this->m_srcfmt.SetCurSel(MPEG4_FRM);
					this->m_dstfmt.SetCurSel(YUV420BLK);
				}
				else
					this->m_Bmpfmt.SetWindowText(rgbfmt[RGB32 - RGB1 + 1]);
			}
				
		}
	}
	else
	{
		this->m_Bmpfmt.SetWindowText(rgbfmt[bmpinfo.index - RGB1]);
		this->m_srcfmt.SetCurSel(BMP);
		this->m_dstfmt.SetCurSel(bmpinfo.index);
	}
	sprintf(str, "%d", bmpinfo.width);
	this->m_BmpWidth.SetWindowText(str);
	this->m_width.SetWindowText(str);
	sprintf(str, "%d", bmpinfo.height);
	this->m_BmpHeight.SetWindowText(str);
	this->m_height.SetWindowText(str);
}

void CTools2PanelDlg::OnBnClickedCompareimgButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption();
	tools.CompareImg(this->m_option.src, this->m_option.src2, this->m_option.dest);
	this->AutoSetSrcfile();
}

void CTools2PanelDlg::OnBnClickedImgsnrButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;
	double db;
	char str[100];

	this->GetUserOption();
	db = tools.Snr(this->m_option.src, this->m_option.src2, this->m_option.bytepdata);
	sprintf(str, "db = %lf", db);
	this->MessageBox(str);
}

void CTools2PanelDlg::OnBnClickedSplitjpgButton()
{
	CWaitCursor cursor;
	CJpegF tools;

	this->GetUserOption();
	tools.SplitJpeg(this->m_option.src, JPEGF_LIST, this->m_option.dest);
}

void CTools2PanelDlg::OnBnClickedChkjpglistButton()
{
	CWaitCursor cursor;
	CJpegF tools;

	this->GetUserOption();
	tools.CheckJpegList(this->m_option.src);
}

void CTools2PanelDlg::OnBnClickedDataconvertButton()
{
	CGeneralTools tools;
	CBaseConvert tc;
	CWaitCursor cursor;

	this->GetUserOption();
	//Set output file's extension type.
	switch(this->m_option.dstfmt)
	{
	case RAW_BG:
	case RAW_GB:
	case RAW_RG:
	case RAW_GR:
	case RAW_RGB:
	case RGB1:
	case RGB2:
	case RGB4:
	case RGB8:
	case RGB444:
	case RGB666:
	case RGB32:
	case RGB24:
	case RGB565:
	case RGB555:
		CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, FILE_TAIL_RGB);
		break;
	case BMP:
		CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, FILE_TAIL_BMP);
		break;
	case JPEG422:
	case JPEG420:
	case JPEG411:
	case JPEG400:
	case JPEG444:
		CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, FILE_TAIL_JPG);
		break;
	default:
		CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, FILE_TAIL_DAT);
		break;
	} 

//	if(this->m_bytepdata.GetItemData(this->m_bytepdata.GetCurSel()) == 0xff)
//		tools.DataConvert(this->m_option.src, this->m_option.dest, this->m_option.width, this->m_option.height, 
//			this->m_option.srcfmt, this->m_option.dstfmt, 0xff, this->m_option.dither);
//	else
		tools.DataConvert(this->m_option.src, this->m_option.dest, this->m_option.width, this->m_option.height, 
			this->m_option.srcfmt, this->m_option.dstfmt, this->m_option.bytepdata, this->m_option.dither);
	this->AutoSetSrcfile(SELECT2_CONVERT_FMT);
//	tc.GetImgSize(this->m_option.dstfmt, this->m_option.width, this->m_option.height,
//			&(this->m_option.width), &(this->m_option.height));
	tc.AdjustImgparm(&(this->m_option.width), &(this->m_option.height), this->m_option.dstfmt);
	this->DispOption();
}

void CTools2PanelDlg::OnBnClickedDataresizeButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption(FILE_TAIL_DAT);
	if(this->m_option.factoren == 0)
	{
		tools.ImageResize(this->m_option.src, this->m_option.dest, this->m_option.width, this->m_option.height, 
			this->m_option.dstwidth, this->m_option.dstheight, this->m_option.dstfmt, this->m_option.bytepdata);
	}
	else
	{
		tools.ImageResize(this->m_option.src, this->m_option.dest, this->m_option.width, this->m_option.height, 
			this->m_option.dstwidth, this->m_option.dstheight, this->m_option.dstfmt, this->m_option.bytepdata);
	}
	this->AutoSetSrcfile(SELECT2_RESIZE);
}

void CTools2PanelDlg::OnBnClickedDatasizerButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption(FILE_TAIL_DAT);
	tools.ImageSizer(this->m_option.src, this->m_option.dest, this->m_option.width, this->m_option.height, 
			this->m_option.dstwidth, this->m_option.dstheight, 
			this->m_option.pointx, this->m_option.pointy, this->m_option.dstfmt, this->m_option.bytepdata);
	this->AutoSetSrcfile(SELECT2_RESIZE);
}

void CTools2PanelDlg::OnBnClickedUnitcvtButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption();
	this->m_option.dstucval = tools.UnitConvert(this->m_option.srcucval, this->m_option.srcfmt, this->m_option.dstfmt);
	this->DispOption();
}

void CTools2PanelDlg::OnBnClickedPrefilterButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;

	this->GetUserOption();
	tools.ImagePrefilter(this->m_option.src, this->m_option.dest, this->m_option.width, this->m_option.height, 
			4, this->m_option.filter, this->m_option.dstfmt, this->m_option.bytepdata);
	this->AutoSetSrcfile(SELECT2_CONVERT_FMT);
}

void CTools2PanelDlg::OnBnClickedRotateButton()
{
	CGeneralTools tools;
	CWaitCursor cursor;
	int temp;

	this->GetUserOption();
	tools.ImageRotate(this->m_option.src, this->m_option.dest, this->m_option.width, this->m_option.height,
		this->m_option.srcfmt, this->m_option.rotmode, this->m_option.bytepdata);
	switch(this->m_option.rotmode)
	{
	case ROT_DEGREE90:
	case ROT_DEGREE270:
	case ROT_MIRROR90:
	case ROT_MIRROR270:
		temp = this->m_option.width;
		this->m_option.width = this->m_option.height;
		this->m_option.height = temp;
		break;
	case ROT_MIRROR0:
	case ROT_DEGREE180:
	case ROT_MIRROR180:
	case ROT_DEGREE0:
	default:
		break;
	}
	this->AutoSetSrcfile();
	this->DispOption();
}

void CTools2PanelDlg::InitUserOption(void)
{
	int i;
	char str[TOOLS2_STRING_LENGTH];
	const int frmrate[] = { 1, 5, 10, 15, 20, 25, 30 };
	memset( &(this->m_option), 0, sizeof(TUser2Option) );

	for(i = 0; i < gTotalRotateMode; i++)
		this->m_rotmodebox.InsertString(i, gRotateMode[i]);

	for(i = 0; i < 2; i++)
	{
		sprintf(str, "%d", i+1);
		this->m_bytepdata.InsertString(i, str);
	}
	
	for(i = 0; i < gTotalImageStreamType; i++)
	{
		this->m_dstfmt.InsertString(i, gImageStreamType[i]);
		this->m_srcfmt.InsertString(i, gImageStreamType[i]);
	}

	for(i = 0; i < sizeof(frmrate) / sizeof(int); i++)
	{
		sprintf(str, "%d", frmrate[i]);
		this->m_frmrateBox.InsertString(i, str);
	}
	this->m_frmrateBox.SetCurSel(6);

	this->m_rotmodebox.SetCurSel(0);
	this->m_bytepdata.SetCurSel(0);
	this->m_dstfmt.SetCurSel(0);
	this->m_srcfmt.SetCurSel(0);

	this->m_width.SetWindowText("640");
	this->m_height.SetWindowText("480");
	this->m_dstwidth.SetWindowText("320");
	this->m_dstheight.SetWindowText("240");
	this->m_pointxBox.SetWindowText("0");
	this->m_pointyBox.SetWindowText("0");
	this->m_startfrmBox.SetWindowText("0");
	this->m_endfrmBox.SetWindowText("0");

	this->m_srcucvalBox.SetWindowText("800");
	this->m_dstucvalBox.EnableWindow(FALSE);
	this->m_qfactorBox.SetWindowText("40");

	this->m_hfactorBox.SetWindowText("800");
	this->m_vfactorBox.SetWindowText("800");
	this->m_filter0Box.SetWindowText("40");
	this->m_filter1Box.SetWindowText("20");
	this->m_filter2Box.SetWindowText("20");
	this->m_filter3Box.SetWindowText("20");

	this->m_imgplaybtn.EnableWindow(FALSE);

	this->m_maxYR.EnableWindow(FALSE);
	this->m_minYR.EnableWindow(FALSE);
	this->m_avgYR.EnableWindow(FALSE);
	this->m_maxUG.EnableWindow(FALSE);
	this->m_minUG.EnableWindow(FALSE);
	this->m_avgUG.EnableWindow(FALSE);
	this->m_maxVB.EnableWindow(FALSE);
	this->m_minVB.EnableWindow(FALSE);
	this->m_avgVB.EnableWindow(FALSE);
}

void CTools2PanelDlg::GetUserOption(char * tail)
{
	char str[TOOLS2_STRING_LENGTH];
	CStringTools tools;

	this->m_width.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.width));
	this->m_height.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.height));

	this->m_startfrmBox.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.startfrm));
	this->m_endfrmBox.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.endfrm));
	this->m_frmrateBox.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.frmrate));

	this->m_dstwidth.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.dstwidth));
	this->m_dstheight.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.dstheight));
	this->m_pointxBox.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.pointx));
	this->m_pointyBox.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.pointy));

	this->m_srcucvalBox.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Hex(str, &(this->m_option.srcucval));
	
	this->m_qfactorBox.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Dec(str, &(this->m_option.qfactor));

	this->m_hfactorBox.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Hex(str, &(this->m_option.hfactor));
	this->m_vfactorBox.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Hex(str, &(this->m_option.vfactor));

	this->m_filter0Box.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Hex(str, this->m_option.filter);
	this->m_filter1Box.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Hex(str, this->m_option.filter+1);
	this->m_filter2Box.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Hex(str, this->m_option.filter+2);
	this->m_filter3Box.GetWindowText(str, TOOLS2_STRING_LENGTH);
	tools.Str2Hex(str, this->m_option.filter+3);

	this->m_option.rotmode = this->m_rotmodebox.GetCurSel();
	this->m_option.dstfmt = this->m_dstfmt.GetCurSel();
	this->m_option.srcfmt = this->m_srcfmt.GetCurSel();
	this->m_option.bytepdata = this->m_bytepdata.GetCurSel() + 1;
	this->m_option.dither = this->m_dither.GetCheck();
	this->m_option.factoren = this->m_factoren.GetCheck();

	this->m_srcfile2box.GetWindowText(this->m_option.src2, DEFAULT_FILENAME_LEN);
	CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, tail);
}

void CTools2PanelDlg::AutoSetSrcfile(int select)
{
	char str[100];

	CBasePanelDlg::AutoSetFileBox(this->m_option.dest);
	switch(select)
	{
	case SELECT2_NORMAL:
		break;
	case SELECT2_CONVERT_FMT:
		if ((this->m_option.dstfmt == CCIR601_656NTSC) || (this->m_option.dstfmt == CCIR601_656PAL))
			this->m_srcfmt.SetCurSel(this->m_option.srcfmt);											//modified by mfbao
		else
			this->m_srcfmt.SetCurSel(this->m_option.dstfmt);
		break;
	case SELECT2_RESIZE:
		this->m_srcfmt.SetCurSel(this->m_option.dstfmt);
		sprintf(str, "%d", this->m_option.dstwidth);
		this->m_width.SetWindowText(str);
		sprintf(str, "%d", this->m_option.dstheight);
		this->m_height.SetWindowText(str);
		break;
	default:
		break;
	}
}

void CTools2PanelDlg::DispOption(void)
{
	char str[20];

	sprintf(str, "%d", this->m_option.pointy);
	this->m_pointyBox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.dstucval);
	this->m_dstucvalBox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.vfactor);
	this->m_vfactorBox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.width);
	this->m_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.height);
	this->m_height.SetWindowText(str);

	sprintf(str, "%d", this->m_option.imgStat[0]);
	this->m_maxYR.SetWindowText(str);
	sprintf(str, "%d", this->m_option.imgStat[1]);
	this->m_minYR.SetWindowText(str);
	sprintf(str, "%d", this->m_option.imgStat[2]);
	this->m_avgYR.SetWindowText(str);

	sprintf(str, "%d", this->m_option.imgStat[3]);
	this->m_maxUG.SetWindowText(str);
	sprintf(str, "%d", this->m_option.imgStat[4]);
	this->m_minUG.SetWindowText(str);
	sprintf(str, "%d", this->m_option.imgStat[5]);
	this->m_avgUG.SetWindowText(str);

	sprintf(str, "%d", this->m_option.imgStat[6]);
	this->m_maxVB.SetWindowText(str);
	sprintf(str, "%d", this->m_option.imgStat[7]);
	this->m_minVB.SetWindowText(str);
	sprintf(str, "%d", this->m_option.imgStat[8]);
	this->m_avgVB.SetWindowText(str);
}

void CTools2PanelDlg::OnBnClickedOpensrcfile2Btn()
{
	CString str;

	if(CBasePanelDlg::OpenFileDlg(&str))
	{
		this->m_srcfile2box.addString(str.GetBuffer());
	}
}

void CTools2PanelDlg::OnBnClickedImgplayButton()
{
	CWaitCursor cursor;

	if(this->m_option.imgplay)
	{
		this->m_option.imgplay = 0;
		this->m_imgplaybtn.SetWindowText("Play");
		this->m_Imgfrm.Pause();
		return;
	}
	this->m_option.imgplay = 1;
	this->m_imgplaybtn.SetWindowText("Pause");
	this->m_Imgfrm.Play();
}

void CTools2PanelDlg::OnBnClickedImgopenButton()
{
	CWaitCursor cursor;

	this->GetUserOption();
	if( (this->m_option.src[0] == 0) || (this->m_option.imgopen == 1) )
	{
		this->m_imgplaybtn.EnableWindow(FALSE);
		this->m_imgopenbtn.SetWindowText("Open");
		this->m_option.imgopen = 0;
		this->m_frmrateBox.EnableWindow(TRUE);
		this->m_startfrmBox.EnableWindow(TRUE);
		this->m_endfrmBox.EnableWindow(TRUE);
		this->m_Imgfrm.Close();
		return;
	}
	this->m_frmrateBox.EnableWindow(FALSE);
	this->m_startfrmBox.EnableWindow(FALSE);
	this->m_endfrmBox.EnableWindow(FALSE);
	this->m_imgplaybtn.EnableWindow(TRUE);
	this->m_imgplaybtn.SetWindowText("Play");
	this->m_option.imgplay = 0;
	this->m_imgopenbtn.SetWindowText("Close");
	this->m_option.imgopen = 1;

	this->m_Imgfrm.SetImgParm(this->m_option.width, this->m_option.height, this->m_option.srcfmt);
	this->m_Imgfrm.SetFrameRate(this->m_option.frmrate);
	this->m_Imgfrm.Open(this->m_option.src);
}

void CTools2PanelDlg::OnBnClickedImgstatButton()
{
	//CWaitCursor cursor;  
	CDbugTools dtool;
	CYuvStatF statTool;
	int outputStat[9], i;

	for (i=0; i<9; i++)
		outputStat[i] = 0;

	this->GetUserOption();
	if( ( (this->m_option.width) * (this->m_option.height) ) >= (2048*2048))
		dtool.PrintMsg("Image Size(%d*%d) if too big.", this->m_option.width, this->m_option.height);
	if( NULL == this->m_option.src )
		dtool.PrintMsg("Please Input Image Source File!");
	statTool.printYuvfmtMsg(outputStat, this->m_option.src, this->m_option.srcfmt, this->m_option.width, 
		this->m_option.height, this->m_option.bytepdata);
	
	for(i=0; i<9; i++)
		this->m_option.imgStat[i] = outputStat[i];
	this->DispOption();
}


