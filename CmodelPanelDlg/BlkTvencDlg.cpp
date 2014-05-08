// BlkAudadcDlg.cpp : 实现文件
//
#include "internal.h"
#include "..\include\cmodelpaneldlg\blktvencdlg.h"


// CBlkTvencDlg 对话框

IMPLEMENT_DYNAMIC(CBlkTvencDlg, CBasePanelDlg)
CBlkTvencDlg::CBlkTvencDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CBlkTvencDlg::IDD, pParent)
{
}

CBlkTvencDlg::~CBlkTvencDlg()
{
}

void CBlkTvencDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TVINPWIDTH_EDIT, m_widthbox);
	DDX_Control(pDX, IDC_TVINPHEIGHT_EDIT, m_heightbox);
	DDX_Control(pDX, IDC_QFACTOR_EDIT, m_qfactorbox);
	DDX_Control(pDX, IDC_VIDEOFMT_COMBO, m_palbox);
	DDX_Control(pDX, IDC_OUTTYPE_COMBO, m_typebox);
	DDX_Control(pDX, IDC_702TVOPCODE_COMBO, m_tvopcodebox);
	DDX_Control(pDX, IDC_702IPPCOLOR_COMBO, m_colorbox);
	DDX_Control(pDX, IDC_CHECK_702BYPASS, m_bypass);
	DDX_Control(pDX, IDC_CHECK_702AVERAGE, m_average);
	DDX_Control(pDX, IDC_COMPON_COMBO, m_componbox);

	DDX_Control(pDX, IDC_CHECK_RUDEBRC, m_rudebrc);
	DDX_Control(pDX, IDC_BRCMODE_COMBO, m_brcmodebox);
	DDX_Control(pDX, IDC_CHECK_ONLY_UV, m_onlyuv);
	DDX_Control(pDX, IDC_ACNUM_EDIT, m_acnumbox);
	DDX_Control(pDX, IDC_RUDETHD_EDIT, m_rudethdbox);
	DDX_Control(pDX, IDC_TCR_EDIT, m_tcrbox);

	DDX_Control(pDX, IDC_QFCOMP0_EDIT, m_qfcomp0box);
	DDX_Control(pDX, IDC_QFCOMP1_EDIT, m_qfcomp1box);
	DDX_Control(pDX, IDC_QFCOMP2_EDIT, m_qfcomp2box);
	DDX_Control(pDX, IDC_QFCOMP3_EDIT, m_qfcomp3box);
	DDX_Control(pDX, IDC_QFCOMP4_EDIT, m_qfcomp4box);
	DDX_Control(pDX, IDC_QFCOMP5_EDIT, m_qfcomp5box);
	DDX_Control(pDX, IDC__THDL1_EDIT, m_thdl1box);
	DDX_Control(pDX, IDC__THDL2_EDIT, m_thdl2box);
	DDX_Control(pDX, IDC__THDR1_EDIT, m_thdr1box);
	DDX_Control(pDX, IDC__THDR2_EDIT, m_thdr2box);
	DDX_Control(pDX, IDC__SHIFTL1_EDIT, m_shiftl1box);
	DDX_Control(pDX, IDC__SHIFTL2_EDIT, m_shiftl2box);
	DDX_Control(pDX, IDC__SHIFTR1_EDIT, m_shiftr1box);
	DDX_Control(pDX, IDC__SHIFTR2_EDIT, m_shiftr2box);

	DDX_Control(pDX, IDC_CFGFILE_COMBO, m_cfgfilebox);
	DDX_Control(pDX, IDC_CHECK_AUTOBRC, m_autobrc);

}

	//ON_BN_CLICKED(IDC_702CONVERT_BUTTON, OnBnClicked702convertButton)

BEGIN_MESSAGE_MAP(CBlkTvencDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_702JPEGENC_BUTTON, OnBnClicked702jpegencButton)
	ON_BN_CLICKED(IDC_702JPEGDEC_BUTTON, OnBnClicked702jpegdecButton)
	ON_BN_CLICKED(IDC_702IPPSIZER_BUTTON, OnBnClicked702ippsizerButton)
	ON_BN_CLICKED(IDC_702IPPRGB_BUTTON, OnBnClicked702ipprgbButton)
	ON_BN_CLICKED(IDC_702TVCONVERT_BUTTON, OnBnClicked702tvconvertButton)
	ON_BN_CLICKED(IDC_CHECK_RUDEBRC, OnBnClickedCheckRudebrc)

	ON_BN_CLICKED(IDC_OPENCFGFILE_BUTTON, OnBnClickedOpencfgfileButton)
	ON_BN_CLICKED(IDC_LOADCFG_BUTTON, OnBnClickedLoadcfgButton)
	ON_BN_CLICKED(IDC_SAVECFG_BUTTON, OnBnClickedSavecfgButton)
END_MESSAGE_MAP()



BOOL CBlkTvencDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	this->InitUserOption();
	this->InitRudeOption();
	this->DispOption();
	this->DispRudeOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CBlkTvencDlg::GetRudeOption(void)
{
	char str[20];	

	if ((this->m_rudebrc.GetCheck() == BST_CHECKED) && this->m_rudebrc.IsWindowEnabled())
		this->m_rudeoption.rude_en = 1;
	else
		this->m_rudeoption.rude_en = 0;
	if (this->m_rudeoption.rude_en)
	{
		if ((this->m_onlyuv.GetCheck() == BST_CHECKED) && this->m_onlyuv.IsWindowEnabled())
			this->m_rudeoption.only_uv = 1;
		else
			this->m_rudeoption.only_uv = 0;

		this->m_rudeoption.rude_level = this->m_brcmodebox.GetCurSel();
		this->m_acnumbox.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.rude_ac));
		/*this->m_tcrbox.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.tcr));*/
		this->m_rudethdbox.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.rude_thd));
		this->m_qfcomp0box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.qfcomp0));
		this->m_qfcomp1box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.qfcomp1));
		this->m_qfcomp2box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.qfcomp2));
		this->m_qfcomp3box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.qfcomp3));
		this->m_qfcomp4box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.qfcomp4));
		this->m_qfcomp5box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.qfcomp5));
		this->m_thdl1box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.thdl1));
		this->m_thdl2box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.thdl2));
		this->m_thdr1box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.thdr1));
		this->m_thdr2box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.thdr2));
		this->m_shiftl1box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.shiftl1));
		this->m_shiftl2box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.shiftl2));
		this->m_shiftr1box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.shiftr1));
		this->m_shiftr2box.GetWindowText(str, 20);
		sscanf(str, "%d", &(this->m_rudeoption.shiftr2));
	}
}

void CBlkTvencDlg::DispRudeOption(void)
{		
	char str[20];
	
	sprintf(str, "%d", this->m_rudeoption.rude_ac);
	this->m_acnumbox.SetWindowText(str);
	/*sprintf(str, "%d", this->m_rudeoption.tcr);
	this->m_tcrbox.SetWindowText(str);*/
	sprintf(str, "%d", this->m_rudeoption.rude_thd);
	this->m_rudethdbox.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.qfcomp0);
	this->m_qfcomp0box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.qfcomp1);
	this->m_qfcomp1box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.qfcomp2);
	this->m_qfcomp2box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.qfcomp3);
	this->m_qfcomp3box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.qfcomp4);
	this->m_qfcomp4box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.qfcomp5);
	this->m_qfcomp5box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.thdl1);
	this->m_thdl1box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.thdl2);
	this->m_thdl2box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.thdr1);
	this->m_thdr1box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.thdr2);
	this->m_thdr2box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.shiftl1);
	this->m_shiftl1box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.shiftl2);
	this->m_shiftl2box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.shiftr1);
	this->m_shiftr1box.SetWindowText(str);
	sprintf(str, "%d", this->m_rudeoption.shiftr2);
	this->m_shiftr2box.SetWindowText(str);

	this->m_brcmodebox.SetCurSel(this->m_rudeoption.rude_level);
	this->m_onlyuv.SetCheck(this->m_rudeoption.only_uv);

	if (this->m_rudeoption.rude_en)
	{
		this->m_rudebrc.SetCheck(BST_CHECKED);
		this->m_brcmodebox.EnableWindow(TRUE);
		this->m_onlyuv.EnableWindow(TRUE);
		this->m_acnumbox.EnableWindow(TRUE);
		this->m_rudethdbox.EnableWindow(TRUE);
		//this->m_tcrbox.EnableWindow(TRUE);

		this->m_qfcomp0box.EnableWindow(TRUE);
		this->m_qfcomp1box.EnableWindow(TRUE);
		this->m_qfcomp2box.EnableWindow(TRUE);
		this->m_qfcomp3box.EnableWindow(TRUE);
		this->m_qfcomp4box.EnableWindow(TRUE);
		this->m_qfcomp5box.EnableWindow(TRUE);
		this->m_thdl1box.EnableWindow(TRUE);
		this->m_thdl2box.EnableWindow(TRUE);
		this->m_thdr1box.EnableWindow(TRUE);
		this->m_thdr2box.EnableWindow(TRUE);
		this->m_shiftl1box.EnableWindow(TRUE);
		this->m_shiftl2box.EnableWindow(TRUE);
		this->m_shiftr1box.EnableWindow(TRUE);
		this->m_shiftr2box.EnableWindow(TRUE);
	}
	else
	{
		this->m_rudebrc.SetCheck(BST_UNCHECKED);
		this->m_brcmodebox.EnableWindow(FALSE);
		this->m_onlyuv.EnableWindow(FALSE);
		this->m_acnumbox.EnableWindow(FALSE);
		this->m_rudethdbox.EnableWindow(FALSE);
		//this->m_tcrbox.EnableWindow(FALSE);

		this->m_qfcomp0box.EnableWindow(FALSE);
		this->m_qfcomp1box.EnableWindow(FALSE);
		this->m_qfcomp2box.EnableWindow(FALSE);
		this->m_qfcomp3box.EnableWindow(FALSE);
		this->m_qfcomp4box.EnableWindow(FALSE);
		this->m_qfcomp5box.EnableWindow(FALSE);
		this->m_thdl1box.EnableWindow(FALSE);
		this->m_thdl2box.EnableWindow(FALSE);
		this->m_thdr1box.EnableWindow(FALSE);
		this->m_thdr2box.EnableWindow(FALSE);
		this->m_shiftl1box.EnableWindow(FALSE);
		this->m_shiftl2box.EnableWindow(FALSE);
		this->m_shiftr1box.EnableWindow(FALSE);
		this->m_shiftr2box.EnableWindow(FALSE);
	}
}

void CBlkTvencDlg::InitRudeOption(void)
{
	const char *brcmode[] = { "0-pred", "1/4-pred", "1/2-pred", "3/4-pred" };
	int i;

	memset(&this->m_rudeoption, 0, sizeof(TRudebrcOption));
	this->m_rudeoption.rude_ac = 10;
	this->m_rudeoption.rude_thd = 0;
	//this->m_rudeoption.tcr = 41;
	this->m_rudeoption.qfcomp0 = 0;
	this->m_rudeoption.qfcomp1 = 0;
	this->m_rudeoption.qfcomp2 = 0;
	this->m_rudeoption.qfcomp3 = 0;
	this->m_rudeoption.qfcomp4 = 0;
	this->m_rudeoption.qfcomp5 = 0;
	this->m_rudeoption.thdl1 = 0;
	this->m_rudeoption.thdl2 = 0;
	this->m_rudeoption.thdr1 = 0;
	this->m_rudeoption.thdr2 = 0;
	this->m_rudeoption.shiftl1 = 0;
	this->m_rudeoption.shiftl2 = 0;
	this->m_rudeoption.shiftr1 = 0;
	this->m_rudeoption.shiftr2 = 0;

	for (i = 0; i < sizeof(brcmode) / sizeof(char *); i++)
		this->m_brcmodebox.InsertString(i, brcmode[i]);
}

void CBlkTvencDlg::InitUserOption(void)
{
	const char *pal[] = { "NTSC", "PAL" };
	const char *type[] = { "Progressive", "oddfield", "evenfield" };
	const char *tvopr[] = { "Filter", "Upsample", "CVBS/Y/C" };
	const char *color[] = { "color", "B/W" };	
	const char *compon[] = { "0", "1"};
	int i;

	memset(&this->m_option, 0, sizeof(TTvencOption));
	this->m_option.width = 640;
	this->m_option.height = 480;
	this->m_option.qfactor = 120;
	this->m_option.tcr = 41;
	for(i = 0; i < sizeof(pal) / sizeof(char *); i++)
		this->m_palbox.InsertString(i, pal[i]);
	for(i = 0; i < sizeof(type) / sizeof(char *); i++)
		this->m_typebox.InsertString(i, type[i]);
	for(i = 0; i < sizeof(tvopr) / sizeof(char *); i++)
		this->m_tvopcodebox.InsertString(i, tvopr[i]);
	for(i = 0; i < sizeof(color) / sizeof(char *); i++)
		this->m_colorbox.InsertString(i, color[i]);	
	for (i = 0; i < sizeof(compon) / sizeof(char *); i++)
		this->m_componbox.InsertString(i, compon[i]);
}

void CBlkTvencDlg::DispOption(void)
{
	char str[20];

	sprintf(str, "%d", this->m_option.width);
	this->m_widthbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.height);
	this->m_heightbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.qfactor);
	this->m_qfactorbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.tcr);
	this->m_tcrbox.SetWindowText(str);
	
	this->m_palbox.SetCurSel(this->m_option.pal);
	this->m_typebox.SetCurSel(this->m_option.interlace);
	this->m_tvopcodebox.SetCurSel(this->m_option.tvopcode);
	this->m_colorbox.SetCurSel(this->m_option.colormode);		
	this->m_componbox.SetCurSel(this->m_option.compon);

	if (this->m_option.bypass)
		this->m_bypass.SetCheck(BST_CHECKED);
	else
		this->m_bypass.SetCheck(BST_UNCHECKED);
	if (this->m_option.average)
		this->m_average.SetCheck(BST_CHECKED);
	else
		this->m_average.SetCheck(BST_UNCHECKED);	
	if (this->m_option.autobrc)
		this->m_autobrc.SetCheck(BST_CHECKED);
	else
		this->m_autobrc.SetCheck(BST_UNCHECKED);
}

void CBlkTvencDlg::GetEditboxValue(void)
{
	char str[20];
	
	this->m_widthbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.width));
	this->m_heightbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.height));
	this->m_qfactorbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.qfactor));	
	this->m_tcrbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.tcr));
}

void CBlkTvencDlg::GetComboValue(char *tail)
{
	this->m_option.pal = this->m_palbox.GetCurSel();
	this->m_option.interlace = this->m_typebox.GetCurSel();
	this->m_option.colormode = this->m_colorbox.GetCurSel();	
	this->m_option.compon = this->m_componbox.GetCurSel();
	//this->m_option.opcode = this->m_operatebox.GetCurSel();
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CBlkTvencDlg::GetCheckValue(void)
{
	if ((this->m_bypass.GetCheck() == BST_CHECKED) && this->m_bypass.IsWindowEnabled())
		this->m_option.bypass = 1;
	else
		this->m_option.bypass = 0;
	if ((this->m_average.GetCheck() == BST_CHECKED) && this->m_average.IsWindowEnabled())
		this->m_option.average = 1;
	else
		this->m_option.average = 0;	
	if ((this->m_autobrc.GetCheck() == BST_CHECKED) && this->m_autobrc.IsWindowEnabled())
		this->m_option.autobrc = 1;
	else
		this->m_option.autobrc = 0;
}

void CBlkTvencDlg::GetUserOption(char *tail)
{
	//tail = NULL;
	this->GetEditboxValue();
	this->GetComboValue(tail);
	this->GetCheckValue();
}


void CBlkTvencDlg::UpdateParam(int *width, int *height)
{
	if (this->m_option.pal == 1)
	{
		*width = PAL_WIDTH;
		*height = (this->m_option.height == 480) ? PAL_HEIGHT_EXT : PAL_HEIGHT;
		*height >>= ((this->m_option.interlace == 0) ? 0 : 1);
	}
	else
	{
		*width = NTSC_WIDTH;
		*height = NTSC_HEIGHT >> ((this->m_option.interlace == 0) ? 0 : 1);
	}
	//if (this->m_option.interlace)
	//	*width <<= 1;
}

int CBlkTvencDlg::versizerbin2txt(char *filename, char *buf, int len)
{
	int i;
	CFileBin fb;

    fb.Open(filename, "wt");
	for (i = 0; i < len; i += 4)
	{
		fb.Printf("%02x ", buf[i] & 0xff);
		fb.Printf("%02x ", buf[i + 1] & 0xff);
		fb.Printf("%02x\n", buf[i + 3] & 0xff);

		fb.Printf("%02x ", buf[i + 2] & 0xff);
		fb.Printf("%02x ", buf[i + 1] & 0xff);
		fb.Printf("%02x\n", buf[i + 3] & 0xff);
	}
	fb.Close();

	return len;
}

int CBlkTvencDlg::sizerbin2txt(char *filename, char *buf, int len)
{
	int i;
	CFileBin fb;

	fb.Open(filename, "wt");
	for (i = 0; i < len; i += 3)
	{
		fb.Printf("%02x ", buf[i] & 0xff);
		fb.Printf("%02x ", buf[i + 1] & 0xff);
		fb.Printf("%02x\n", buf[i + 2] & 0xff);
		if (this->m_option.interlace)
		{
			fb.Printf("%02x ", buf[i] & 0xff);
			fb.Printf("%02x ", buf[i + 1] & 0xff);
			fb.Printf("%02x\n", buf[i + 2] & 0xff);
		}
	}
	fb.Close();

	return len;
}

int CBlkTvencDlg::decbin2txt(char *filename, char *buf, int len)
{
	int i, size;
	CFileBin fb;

	size = len >> 2;
	fb.Open(filename, "wt");
	for (i = 0; i < size; i++)
	{
		fb.Printf("%02x ", buf[i << 2] & 0xff);
		fb.Printf("%02x ", buf[(i << 2) + 1] & 0xff);
		fb.Printf("%02x ", buf[(i << 2) + 2] & 0xff);
		fb.Printf("%02x\n", buf[(i << 2) + 3] & 0xff);
	}
	fb.Close();

	return len;
}

int CBlkTvencDlg::bin2txt(char *filename, char *buf, int len, int unit)
{
	int i, tmp;
	CFileBin fb;

	fb.Open(filename, "wt");
	if (unit == 1)
	{
		for (i = 0; i < len; i++)
			fb.Printf("%02x\n", buf[i] & 0xff);
	}
	else
	{
		for (i = 0; i < len; i += unit)
		{
			tmp = (buf[i] & 0xff) + (buf[i + 1] << 8);
			fb.Printf("%03x\n", tmp & 0xfff);
		}
	}	
	fb.Close();
	return len;
}

int CBlkTvencDlg::filterbin2txt(char *filename, char *buf, int len, int yuv)
{
	int i, tmp;
	CFileBin fb;

	fb.Open(filename, "wt");
	switch(yuv) 
	{
	case YUV_Y:
		for (i = 0; i < len; i += 4)
		{
			tmp = (buf[i] & 0xff) + (buf[i + 1] << 8);
			fb.Printf("%03x\n", tmp & 0xfff);
		}
		break;
	case YUV_U:
		for (i = 2; i < len; i += 8)
		{
			tmp = (buf[i] & 0xff) + (buf[i + 1] << 8);
			fb.Printf("%03x\n", tmp & 0xfff);
		}
		break;
	case YUV_V:
		for (i = 6; i < len; i += 8)
		{
			tmp = (buf[i] & 0xff) + (buf[i + 1] << 8);
			fb.Printf("%03x\n", tmp & 0xfff);
		}
		break;
	default:
		break;
	}
	return len;
}

int CBlkTvencDlg::upsampbin2txt(char *filename, char *buf, int len, int yuv)
{
	int i, tmp;
	int pos[3] = {0, 2, 4};
	CFileBin fb;

	fb.Open(filename, "wt");
	for (i = pos[yuv]; i < len; i += 6)
	{
		tmp = (buf[i] & 0xff) + (buf[i + 1] << 8);
		fb.Printf("%03x\n", tmp & 0xfff);
	}
	fb.Close();
	return len;
}

void CBlkTvencDlg::OnBnClicked702jpegencButton()
{
	CWaitCursor cursor;
	CFileBin fsrc;
	CBufferBase srcbuf;
	CYuv422F tools;
	int len, unit = 1;
	char *tail[] = { "tmp", "jpg", "JPGTXT" };

	this->GetUserOption(tail[0]);
	fsrc.Open(this->m_option.srcfile, "rb");
	len = fsrc.GetFileLength();
	fsrc.Close();
	if (!len)
	{
		return;
	}	
	if ((this->m_option.qfactor > 127) || (this->m_option.qfactor <= 0))
	{
		MessageBox("qfactor value is wrong!\n");
		return;
	}

	tools.Yuyv2Blk(this->m_option.srcfile, this->m_option.dstfile, 
		this->m_option.width, this->m_option.height, unit);
	this->AutoSetFileBox(this->m_option.dstfile);

	this->GetUserOption(tail[1]);
	this->GetRudeOption();
	if ((!this->m_option.autobrc) && (!this->m_rudeoption.rude_en))
        this->m_option.qfactor = tools.Blk2Jpeg(this->m_option.srcfile, this->m_option.dstfile, 
										this->m_option.width, this->m_option.height, this->m_option.qfactor);
	else
	{
		this->m_option.qfactor = 
			this->JpegEncOperation(this->m_option.srcfile, this->m_option.dstfile,	this->m_option.width,
							this->m_option.height, this->m_option.qfactor);
	}
	/*this->m_option.qfactor = 
			this->JpegEncOperation(this->m_option.srcfile, this->m_option.dstfile,	this->m_option.width, this->m_option.height,
				this->m_option.qfactor, this->m_option.rudebrc, this->m_option.brcmode, this->m_option.onlyuv, this->m_option.acnum);*/
	this->AutoSetFileBox(this->m_option.dstfile);
	this->DispOption();

	this->GetUserOption(tail[2]);
	fsrc.Open(this->m_option.srcfile, "rb");
	len = fsrc.GetFileLength();
	srcbuf.SetSize(len);
	fsrc.Read(srcbuf.m_buf, len);
	this->bin2txt(this->m_option.dstfile, srcbuf.m_buf, len);
	fsrc.Close();
}

int CBlkTvencDlg::JpegEncOperation(char *srcfile, char *dstfile, int width, int height, int qf)
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	CYuv422 tools;
	int size, q;
	PTRudebrcOption pinfo = NULL;

	if (this->m_rudeoption.rude_en)
		pinfo = &(this->m_rudeoption);
	size = tools.getVwc(width, height);
	srcbuf.SetSize(size);
	dstbuf.SetSize(size);
	
	fsrc.Open(srcfile);
	fsrc.Read(srcbuf.m_buf, size);
	size = tools.Blk2Jpeg1(srcbuf.m_buf, dstbuf.m_buf, width, height, &q, qf, this->m_option.tcr, pinfo);
	fdst.Open(dstfile, "wb");
	fdst.Write(dstbuf.m_buf, size);

	return q;
}

void CBlkTvencDlg::OnBnClicked702jpegdecButton()
{
	CFileBin fsrc;
	CBufferBase srcbuf;
	CJpegF tools;
	CYuv422F blktlools;
	int len, unit = 1;
	char *tail[] = { "tmp", "yuv422", "YUYV" };

	this->GetUserOption(tail[0]);
	fsrc.Open(this->m_option.srcfile, "rb");
	len = fsrc.GetFileLength();
	fsrc.Close();
	if (!len)
	{
		return;
	}

	tools.Jpeg2Blk(this->m_option.srcfile, this->m_option.dstfile);
	this->AutoSetFileBox(this->m_option.dstfile);

	this->GetUserOption(tail[1]);
	blktlools.Blk2Yuyv(this->m_option.srcfile, this->m_option.dstfile, 
		this->m_option.width, this->m_option.height, unit);
	this->AutoSetFileBox(this->m_option.dstfile);

	this->GetUserOption(tail[2]);
	fsrc.Open(this->m_option.srcfile, "rb");
	len = fsrc.GetFileLength();
	srcbuf.SetSize(len);
	fsrc.Read(srcbuf.m_buf, len);
	this->decbin2txt(this->m_option.dstfile, srcbuf.m_buf, len);
	fsrc.Close();
}

void CBlkTvencDlg::OnBnClicked702ippsizerButton()
{
	CWaitCursor cursor;
	CFileBin fsrc, fdst, fvdst;
	CBufferBase srcbuf, dstbuf, verbuf;
	C702Sizer sizerModule;
	CYuv422	tools;
	int i, len, size, verheight, versize;
	char *tail[] = { "versizer", "sizer" };
	char *bytail[] = {"verYUV", "YUV"};
	char filename[256];
//	int tmp0;

	this->GetUserOption(tail[0]);	
	if ( ((this->m_option.width != 352) || (this->m_option.height != 288)) &&
		((this->m_option.width != 640) || (this->m_option.height != 480)) )
	{
		MessageBox("now just support CIF or VGA format!\n");
		return;
	}
	if (this->m_option.bypass)
		return;
	fsrc.Open(this->m_option.srcfile, "rb");
	len = fsrc.GetFileLength();
	if (!len)
	{
		fsrc.Close();
		return;
	}
	size = PAL_WIDTH * PAL_HEIGHT_EXT << 1;
	verheight = (this->m_option.pal == 0) ? NTSC_HEIGHT : (this->m_option.height == 480 ? PAL_HEIGHT_EXT : PAL_HEIGHT);
	verheight = (this->m_option.interlace == 0) ? verheight : (verheight >> 1);
	versize = verheight * this->m_option.width << 1;
	srcbuf.SetSize(size);
	dstbuf.SetSize((size * 3) >> 1);
	verbuf.SetSize(size);
	fsrc.Read(srcbuf.m_buf, len);

	sizerModule.Init(this->m_option.width, this->m_option.height, this->m_option.pal, this->m_option.interlace, this->m_option.average, this->m_option.compon);
	len = sizerModule.Process(srcbuf.m_buf, dstbuf.m_buf, verbuf.m_buf);

	fvdst.Open(this->m_option.dstfile, "wb");
	fvdst.Write(verbuf.m_buf, versize);
	fvdst.Close();

	this->GetUserOption(bytail[0]);
	this->versizerbin2txt(this->m_option.dstfile, verbuf.m_buf, versize);

	this->AutoSetFileBox(this->m_option.dstfile);
	this->GetUserOption(tail[1]);
	for (i = 0; i < 256; i++)
		filename[i] = this->m_option.dstfile[i];

	/*if (this->m_option.interlace && (this->m_option.compon == 0))
	{
		for (i = 0; i < len; i += 6)
		{
			tmp0 = ((dstbuf.m_buf[i + 1] & 0xff) + (dstbuf.m_buf[i + 4] & 0xff)) >> 1;
			dstbuf.m_buf[i + 1] = (char)tmp0;
			dstbuf.m_buf[i + 4] = (char)tmp0;
			tmp0 = ((dstbuf.m_buf[i + 2] & 0xff) + (dstbuf.m_buf[i + 5] & 0xff)) >> 1;
			dstbuf.m_buf[i + 2] = (char)tmp0;
			dstbuf.m_buf[i + 5] = (char)tmp0;			
		}
	}*/
	for (size = 0, i = 0; i < len; i += 6)
	{
		size += tools.yuv444_422order(dstbuf.m_buf + i, srcbuf.m_buf + size, YUV422_YUYV);
	}
	fdst.Open(this->m_option.dstfile, "wb");
	fdst.Write(srcbuf.m_buf, size);
	fdst.Close();
	fsrc.Close();

	this->GetUserOption(bytail[1]);
	this->sizerbin2txt(this->m_option.dstfile, dstbuf.m_buf, len);

	this->UpdateParam(&(this->m_option.width), &(this->m_option.height));
	this->AutoSetFileBox(filename);	
	this->DispOption();
}

void CBlkTvencDlg::OnBnClicked702ipprgbButton()
{
	int len;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf;
	CYuv422F tools;
	char *tail[] = { "tmp", "rgb", "RGBTXT" };
	char filename[256];

	this->GetUserOption(tail[0]);
	if (this->m_option.bypass)
		return;
	fsrc.Open(this->m_option.srcfile, "rb");
	len = fsrc.GetFileLength();	
	if (!len)
	{
		fsrc.Close();
		return;
	}

	if (this->m_option.colormode)
	{
		srcbuf.SetSize(len);
		fsrc.Read(srcbuf.m_buf, len);
		this->ClearUV(srcbuf.m_buf, len);
		fdst.Open(this->m_option.dstfile, "wb");
		fdst.Write(srcbuf.m_buf, len);
		fdst.Close();
		fsrc.Close();
		this->AutoSetFileBox(this->m_option.dstfile);
	}		
	this->GetUserOption(tail[1]);
	tools.Yuyv2Rgb(this->m_option.srcfile, this->m_option.dstfile, 
						this->m_option.width, this->m_option.height);

	for (len = 0; len < 256; len++)
		filename[len] = this->m_option.dstfile[len];
	this->GetUserOption(tail[2]);
	fsrc.Open(filename, "rb");
	len = fsrc.GetFileLength();
	srcbuf.SetSize(len);
	fsrc.Read(srcbuf.m_buf, len);
	this->bin2txt(this->m_option.dstfile, srcbuf.m_buf, len);
}

void CBlkTvencDlg::ClearUV(char *buf, int len)
{
	int i, size;

	size = len >> 1;
	for (i = 0; i < size; i++)	
		buf[(i << 1) + 1] = -128;
}


void CBlkTvencDlg::OnBnClicked702tvconvertButton()
{
	int ret = 0;

	this->m_option.tvopcode = this->m_tvopcodebox.GetCurSel();
	switch(this->m_option.tvopcode) 
	{
	case FILTER:
		ret = this->FilterOperation();
		break;
	case UPSAMPLE:
		ret = this->UpsampleOperation();
		break;
	case CVBS_Y_C:
		ret = this->CVBSOperation();
		break;
	default:
		break;
	}
	if (ret == 1)
	{
		this->m_option.tvopcode++;
		this->m_option.tvopcode = (this->m_option.tvopcode > CVBS_Y_C) ? CVBS_Y_C : 
		(this->m_option.tvopcode < FILTER ? FILTER : this->m_option.tvopcode);
		this->DispOption();
	}	
}

int CBlkTvencDlg::FilterOperation(void)
{
	CWaitCursor cursor;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	C702Filter tools;
	int len;
	char *tail[] = { "filter", "FIL_Y", "FIL_U", "FIL_V" };

	this->GetUserOption(tail[0]);

	tools.Init(this->m_option.width, this->m_option.height, this->m_option.pal);
	fsrc.Open(this->m_option.srcfile);
	len = fsrc.GetFileLength();
	if (!len)
	{
		fsrc.Close();
		return 0;
	}
	srcbuf.SetSize(len);
	dstbuf.SetSize(len << 1);
	fsrc.Read(srcbuf.m_buf, len);

	len = tools.Process(srcbuf.m_buf, dstbuf.m_buf);
	fdst.Open(this->m_option.dstfile, "wb");
	fdst.Write(dstbuf.m_buf, len);
	fdst.Close();
	fsrc.Close();
	this->AutoSetFileBox(this->m_option.dstfile);

	this->GetUserOption(tail[1]);	
	this->filterbin2txt(this->m_option.dstfile, dstbuf.m_buf, len, YUV_Y);
	this->GetUserOption(tail[2]);
	this->filterbin2txt(this->m_option.dstfile, dstbuf.m_buf, len, YUV_U);
	this->GetUserOption(tail[3]);
	this->filterbin2txt(this->m_option.dstfile, dstbuf.m_buf, len, YUV_V);

	return 1;
}

int CBlkTvencDlg::UpsampleOperation(void)
{
	CWaitCursor cursor;
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	C702Upsample tools;
	int len;
	char *tail[] = { "upsample", "UPSAMP_Y", "UPSAMP_U", "UPSAMP_V"};

	this->GetUserOption(tail[0]);	
	fsrc.Open(this->m_option.srcfile);
	len = fsrc.GetFileLength();
	if (!len)
	{
		fsrc.Close();
		return 0;
	}
	srcbuf.SetSize(len);
	dstbuf.SetSize(len * 3);
	fsrc.Read(srcbuf.m_buf, len);
	tools.Init(this->m_option.width, this->m_option.height, 2);
	len = tools.Process(srcbuf.m_buf, dstbuf.m_buf);
	fdst.Open(this->m_option.dstfile, "wb");
	fdst.Write(dstbuf.m_buf, len);
	fdst.Close();
	fsrc.Close();
	this->AutoSetFileBox(this->m_option.dstfile);
	this->m_option.width <<= 1;
	this->DispOption();	

	this->GetUserOption(tail[1]);	
	this->upsampbin2txt(this->m_option.dstfile, dstbuf.m_buf, len, YUV_Y);
	this->GetUserOption(tail[2]);
	this->upsampbin2txt(this->m_option.dstfile, dstbuf.m_buf, len, YUV_U);
	this->GetUserOption(tail[3]);
	this->upsampbin2txt(this->m_option.dstfile, dstbuf.m_buf, len, YUV_V);
	return 1;
}

int CBlkTvencDlg::CVBSOperation(void)
{
	CWaitCursor cursor;
	CFileBin fsrc, fcvbs;
	CBufferBase srcbuf, cvbsbuf;
	C702Composite tools;
	int inter, len;
	char *tail[] = { "cvbs", "y", "c" };
	char *bytail[] = { "CVBSTXT", "YTXT", "CTXT" };
	
	this->GetUserOption(tail[0]);
	/*if (this->m_option.interlace == 0)
	{
		MessageBox("now just support interlace type!\n");
		return 0;
	}*/
	fsrc.Open(this->m_option.srcfile, "rb");
	len = fsrc.GetFileLength();
	if (!len)
	{
		fsrc.Close();
		return 0;
	}
	inter = this->m_option.interlace ? 1 : 0;
	srcbuf.SetSize(len);
	cvbsbuf.SetSize(len >> 1);
	fsrc.Read(srcbuf.m_buf, len);
	tools.Init(this->m_option.width, this->m_option.height, this->m_option.pal, inter, 2);	
	len = tools.Process(srcbuf.m_buf, cvbsbuf.m_buf);
	fcvbs.Open(this->m_option.dstfile, "wb");
	fcvbs.Write(cvbsbuf.m_buf, len);
	fcvbs.Close();
	fsrc.Close();
	this->AutoSetFileBox(this->m_option.dstfile);

	this->GetUserOption(bytail[0]);
	this->bin2txt(this->m_option.dstfile, cvbsbuf.m_buf, len, 2);

	this->GetUserOption(tail[2]);

	return 1;
}
void CBlkTvencDlg::OnBnClickedCheckRudebrc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (this->m_rudebrc.GetCheck())
	{
		this->m_brcmodebox.EnableWindow(TRUE);
		this->m_onlyuv.EnableWindow(TRUE);
		this->m_acnumbox.EnableWindow(TRUE);
		this->m_rudethdbox.EnableWindow(TRUE);
		//this->m_tcrbox.EnableWindow(TRUE);

		this->m_qfcomp0box.EnableWindow(TRUE);
		this->m_qfcomp1box.EnableWindow(TRUE);
		this->m_qfcomp2box.EnableWindow(TRUE);
		this->m_qfcomp3box.EnableWindow(TRUE);
		this->m_qfcomp4box.EnableWindow(TRUE);
		this->m_qfcomp5box.EnableWindow(TRUE);
		this->m_thdl1box.EnableWindow(TRUE);
		this->m_thdl2box.EnableWindow(TRUE);
		this->m_thdr1box.EnableWindow(TRUE);
		this->m_thdr2box.EnableWindow(TRUE);
		this->m_shiftl1box.EnableWindow(TRUE);
		this->m_shiftl2box.EnableWindow(TRUE);
		this->m_shiftr1box.EnableWindow(TRUE);
		this->m_shiftr2box.EnableWindow(TRUE);
	}
	else
	{
		this->m_brcmodebox.EnableWindow(FALSE);
		this->m_onlyuv.EnableWindow(FALSE);
		this->m_acnumbox.EnableWindow(FALSE);
		this->m_rudethdbox.EnableWindow(FALSE);
		//this->m_tcrbox.EnableWindow(FALSE);

		this->m_qfcomp0box.EnableWindow(FALSE);
		this->m_qfcomp1box.EnableWindow(FALSE);
		this->m_qfcomp2box.EnableWindow(FALSE);
		this->m_qfcomp3box.EnableWindow(FALSE);
		this->m_qfcomp4box.EnableWindow(FALSE);
		this->m_qfcomp5box.EnableWindow(FALSE);
		this->m_thdl1box.EnableWindow(FALSE);
		this->m_thdl2box.EnableWindow(FALSE);
		this->m_thdr1box.EnableWindow(FALSE);
		this->m_thdr2box.EnableWindow(FALSE);
		this->m_shiftl1box.EnableWindow(FALSE);
		this->m_shiftl2box.EnableWindow(FALSE);
		this->m_shiftr1box.EnableWindow(FALSE);
		this->m_shiftr2box.EnableWindow(FALSE);
	}
}


void CBlkTvencDlg::OnBnClickedOpencfgfileButton()
{
	CString str;

	if(CBaseDlg::OpenFileDlg(&str))
	{
		this->m_cfgfilebox.addString(str.GetBuffer());		
	}
}



void CBlkTvencDlg::OnBnClickedLoadcfgButton()
{
	//CFileBin fb;
	//char line[256];

	////CBasePanelDlg::GetFileBoxOption(this->m_option.cfgfile, this->m_option.dstfile, tail);
	//this->m_cfgfilebox.GetWindowText(this->m_option.cfgfile, DEFAULT_FILENAME_LEN);
	//fb.Open(this->m_option.cfgfile, "rt");
	//if (!fb.IsOpened())
	//{
	//	return;
	//}
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_option.width));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_option.height));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_option.qfactor));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.rude_en));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.rude_level));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.only_uv));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.rude_thd));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.rude_ac));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_option.tcr));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.qfcomp0));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.qfcomp1));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.qfcomp2));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.qfcomp3));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.qfcomp4));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.qfcomp5));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.thdl1));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.thdl2));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.thdr1));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.thdr2));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.shiftl1));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.shiftl2));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.shiftr1));
	//fb.Gets(line, 254);
	//sscanf(line, "%d", &(this->m_rudeoption.shiftr2));

	//this->DispRudeOption();
	//sprintf(line, "%d", this->m_option.width);
	//this->m_widthbox.SetWindowText(line);
	//sprintf(line, "%d", this->m_option.height);
	//this->m_heightbox.SetWindowText(line);
	//sprintf(line, "%d", this->m_option.qfactor);
	//this->m_qfactorbox.SetWindowText(line);	
	//sprintf(line, "%d", this->m_option.tcr);
	//this->m_tcrbox.SetWindowText(line);
	
	CFileBin fb;
	CCharFifo cf;
	CBufferBase	pool;
	int len, len1, val;
	char namestr[20];

	this->m_cfgfilebox.GetWindowText(this->m_option.cfgfile, DEFAULT_FILENAME_LEN);
	fb.Open(this->m_option.cfgfile);
	len = fb.GetFileLength();
	pool.SetSize(len);
	cf.SetSize(len);
	fb.Read(pool.m_buf, len);
	cf.Write(pool.m_buf, len);
	len1 = pool.GetSize();
	do {
		len = cf.Read(pool.m_buf, len1);
		pool.m_buf[len] = 0;		
		sprintf(namestr, "%s\n", pool.m_buf);
		namestr[len] = 0;
		len = cf.Read(pool.m_buf, len1);
		len = cf.Read(pool.m_buf, len1);
		pool.m_buf[len] = 0;
		sscanf(pool.m_buf, "%d\n", &val);
		
		this->ParserCfg(namestr, val);
		
	} while (len);

	this->DispOption();
	this->DispRudeOption();
}

void CBlkTvencDlg::ParserCfg(char *str, int val)
{	
	
	if (strcmp("width", str) == 0)
		this->m_option.width = val;
	else if (strcmp("height", str) == 0)
		this->m_option.height = val;

	else if (strcmp("pal", str) == 0)
		this->m_option.pal = val;
	else if (strcmp("interlace", str) == 0)
		this->m_option.interlace = val;
	else if (strcmp("line_avg", str) == 0)
		this->m_option.average = val;
	else if (strcmp("color_mode", str) == 0)
		this->m_option.colormode = val;
	else if (strcmp("component", str) == 0)
		this->m_option.compon = val;
	else if (strcmp("bypass", str) == 0)
		this->m_option.bypass = val;

	else if (strcmp("qfactor", str) == 0)
		this->m_option.qfactor = val;
	else if (strcmp("tcr", str) == 0)
		this->m_option.tcr = val;
	else if (strcmp("autobrc", str) == 0)
		this->m_option.autobrc = val;

	else if (strcmp("rudebrc",str) == 0)
		this->m_rudeoption.rude_en = val;
	else if (strcmp("only_uv", str) == 0)
		this->m_rudeoption.only_uv = val;
	else if (strcmp("rude_level", str) == 0)
		this->m_rudeoption.rude_level = val;
	else if (strcmp("rude_ac", str) == 0)
		this->m_rudeoption.rude_ac = val;
	else if (strcmp("rude_thd", str) == 0)
		this->m_rudeoption.rude_thd = val;

	else if (strcmp("qfcomp0", str) == 0)
		this->m_rudeoption.qfcomp0 = val;
	else if (strcmp("qfcomp1", str) == 0)
		this->m_rudeoption.qfcomp1 = val;
	else if (strcmp("qfcomp2", str) == 0)
		this->m_rudeoption.qfcomp2 = val;
	else if (strcmp("qfcomp3", str) == 0)
		this->m_rudeoption.qfcomp3 = val;
	else if (strcmp("qfcomp4", str) == 0)
		this->m_rudeoption.qfcomp4 = val;
	else if (strcmp("qfcomp5", str) == 0)
		this->m_rudeoption.qfcomp5 = val;

	else if (strcmp("thd_l1", str) == 0)
		this->m_rudeoption.thdl1 = val;
	else if (strcmp("thd_l2", str) == 0)
		this->m_rudeoption.thdl2 = val;
	else if (strcmp("thd_r1", str) == 0)
		this->m_rudeoption.thdr1 = val;
	else if (strcmp("thd_r2", str) == 0)
		this->m_rudeoption.thdr2 = val;

	else if (strcmp("shift_l1", str) == 0)
		this->m_rudeoption.shiftl1 = val;
	else if (strcmp("shift__l2", str) == 0)
		this->m_rudeoption.shiftl2 = val;
	else if (strcmp("shift_r1", str) == 0)
		this->m_rudeoption.shiftr1 = val;
	else if (strcmp("shift_r2", str) == 0)
		this->m_rudeoption.shiftr2 = val;


}

void CBlkTvencDlg::OnBnClickedSavecfgButton()
{
	CFileBin fb;	
	
	this->m_cfgfilebox.GetWindowText(this->m_option.cfgfile, DEFAULT_FILENAME_LEN);
	fb.Open(this->m_option.cfgfile, "wt");
	if (!fb.IsOpened())
	{
		return;
	}
	this->GetRudeOption();
	this->GetUserOption();
	fb.Printf("width		= %d		// image width 			\n", this->m_option.width);
	fb.Printf("height		= %d		// image height 		\n", this->m_option.height);
	fb.Printf("interlace	= %d		// 0: prg, 1: oddfield, 2: evenfield 		\n", this->m_option.interlace);
	fb.Printf("line_avg	= %d		// 0: disable, 1: enable 					\n", this->m_option.average);
	fb.Printf("color_mode	= %d		// 0: color, 1: B/W 						\n", this->m_option.colormode);
	fb.Printf("component	= %d		// 0: disable, 1: enable 					\n", this->m_option.compon);
	fb.Printf("bypass		= %d		// IPP module, 0: disable, 1: enable 		\n", this->m_option.bypass);
	fb.Printf("qfactor		= %d		// Q factor 			\n", this->m_option.qfactor);
	fb.Printf("tcr		= %d		// target cr			\n", this->m_option.tcr);
	fb.Printf("autobrc		= %d		// auto brc, 0: disable, 1: enable			\n", this->m_option.autobrc);
	fb.Printf("rudebrc		= %d		// rude_brc, 0: disable, 1: enable			\n", this->m_rudeoption.rude_en);
	fb.Printf("only_uv		= %d		// only_uv,  0: disable, 1: enable			\n", this->m_rudeoption.only_uv);
	fb.Printf("rude_level	= %d		// rude_level		\n", this->m_rudeoption.rude_level);
	fb.Printf("rude_ac		= %d		// rude_ac			\n", this->m_rudeoption.rude_ac);
	fb.Printf("rude_thd	= %d		// rude_thd_wc 		\n", this->m_rudeoption.rude_thd);	
	
	fb.Printf("qfcomp0		= %d		// qf_level0_comp	\n", this->m_rudeoption.qfcomp0);
	fb.Printf("qfcomp1		= %d		// qf_level1_comp	\n", this->m_rudeoption.qfcomp1);
	fb.Printf("qfcomp2		= %d		// qf_level2_comp	\n", this->m_rudeoption.qfcomp2);
	fb.Printf("qfcomp3		= %d		// qf_level3_comp	\n", this->m_rudeoption.qfcomp3);
	fb.Printf("qfcomp4		= %d		// qf_level4_comp	\n", this->m_rudeoption.qfcomp4);
	fb.Printf("qfcomp5		= %d		// qf_level5_comp	\n", this->m_rudeoption.qfcomp5);
	fb.Printf("thd_l1		= %d		// thd_l1			\n", this->m_rudeoption.thdl1);
	fb.Printf("thd_l2		= %d		// thd_l2			\n", this->m_rudeoption.thdl2);
	fb.Printf("thd_r1		= %d		// thd_r1			\n", this->m_rudeoption.thdr1);
	fb.Printf("thd_r2		= %d		// thd_r2			\n", this->m_rudeoption.thdr2);
	fb.Printf("shift_l1	= %d		// shift_nbit_l1	\n", this->m_rudeoption.shiftl1);
	fb.Printf("shift_l2	= %d		// shift_nbit_l2 	\n", this->m_rudeoption.shiftl2);
	fb.Printf("shift_r1	= %d		// shift_nbit_r1 	\n", this->m_rudeoption.shiftr1);
	fb.Printf("shift_r2	= %d		// shift_nbit_r2 	\n", this->m_rudeoption.shiftr2);
	
	MessageBox("The paramenters are saved Ok!\n");
}
