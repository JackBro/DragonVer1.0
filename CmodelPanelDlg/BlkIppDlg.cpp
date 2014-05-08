// BlkIppDlg.cpp : 实现文件
//

#include "internal.h"

// CBlkIppDlg 对话框

IMPLEMENT_DYNAMIC(CBlkIppDlg, CBasePanelDlg)
CBlkIppDlg::CBlkIppDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CBlkIppDlg::IDD, pParent)
{
}

CBlkIppDlg::~CBlkIppDlg()
{
}

void CBlkIppDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SPECIAL_EFFECT, m_cboSpeEffect);
	DDX_Control(pDX, IDC_SCRBAR_U_OFFSET, m_bar_u_offset);
	DDX_Control(pDX, IDC_SCRBAR_V_OFFSET, m_bar_v_offset);
	DDX_Control(pDX, IDC_SCRBAR_U_DOWN, m_bar_u_down);
	DDX_Control(pDX, IDC_SCRBAR_U_UP, m_bar_u_up);
	DDX_Control(pDX, IDC_SCRBAR_V_DOWN, m_bar_v_down);
	DDX_Control(pDX, IDC_SCRBAR_V_UP, m_bar_v_up);
	DDX_Control(pDX, IDC_EDIT_U_OFFSET, m_edit_u_offset);
	DDX_Control(pDX, IDC_EDIT_V_OFFSET, m_edit_v_offset);
	DDX_Control(pDX, IDC_EDIT_U_DOWN, m_edit_u_down);
	DDX_Control(pDX, IDC_EDIT_U_UP, m_edit_u_up);
	DDX_Control(pDX, IDC_EDIT_V_DOWN, m_edit_v_down);
	DDX_Control(pDX, IDC_EDIT_V_UP, m_edit_v_up);
	DDX_Control(pDX, IDC_EDIT_PATTERN_WIDTH, m_edit_patternwidth);
	DDX_Control(pDX, IDC_EDIT_PATTERN_HEIGHT, m_edit_patternheight);
	DDX_Control(pDX, IDC_CBO_SRCFILE2, m_cboSrcFile2);
	DDX_Control(pDX, IDC_EDIT_IMAGE_WIDTH, m_edit_width);
	DDX_Control(pDX, IDC_EDIT_IMAGE_HEIGHT, m_edit_height);	
	DDX_Control(pDX, IDC_EDIT_UNIT, m_edit_unit);	
	DDX_Control(pDX, IDC_CHECK_RAWDATA, m_rawdata);
	DDX_Control(pDX, IDC_CHECK_OUTPUTBMP, m_outputbmp);
	DDX_Control(pDX, IDC_EDIT_SHIFT_MAXMOVE, m_edit_shift_maxmove);
	DDX_Control(pDX, IDC_EDIT_TV_DEG, m_edit_tv_deg);
	DDX_Control(pDX, IDC_EDIT_SRC_WIDTH, m_edit_src_width);
	DDX_Control(pDX, IDC_EDIT_SRC_HEIGHT, m_edit_src_height);
	DDX_Control(pDX, IDC_EDIT_IPP_DST_WIDTH, m_edit_dst_width);
	DDX_Control(pDX, IDC_EDIT_IPP_DST_HEIGHT, m_edit_dst_height);
}


BEGIN_MESSAGE_MAP(CBlkIppDlg, CBasePanelDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SPECIAL_EFFECT, OnCbnSelchangeComboSpecialEffect)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_DOCMODULE, OnBnClickedBtnDocmodule)
	ON_BN_CLICKED(IDC_BTN_OPENFILE, OnBnClickedBtnOpenfile)
	ON_BN_CLICKED(IDC_BTN_NEW_CMODULE, OnBnClickedBtnNewCmodule)
	ON_EN_CHANGE(IDC_EDIT_IMAGE_WIDTH, OnEnChangeEditImageWidth)
	ON_BN_CLICKED(IDC_BTN_SIZER, OnBnClickedBtnSizer)
END_MESSAGE_MAP()


// CBlkIppDlg 消息处理程序
const static  TBlkIppSpe gSpe[] = 
{
	{"SEPHIA",	IPPSEPHIA},
	{"SPECIALCOLOR",	IPPSPECIALCOLOR},
	{"NEGATIVE",	IPPNEGATIVE},
	{"SKETCH",	IPPSKETCH},
	{"RELIEF",	IPPRELIEF},
	{"CANVAS",	IPPCANVAS},
	{"TV",		IPPTV},
	{"SHIFT",	IPPSHIFT},
	{"WINDBLOW", IPPWINDBLOW},
	{"BINARY",	IPPBINARY},
	{"PAINT", IPPPAINT},
	{"PARTICLE", IPPPARTICLE}
};


BOOL CBlkIppDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	this->InitOption();
	this->DispOption();

	return TRUE;
}

void CBlkIppDlg::InitOption()
{
	int i;

	memset(&(this->m_option), 0, sizeof(TBlkIppOption));

	for(i = 0; i < sizeof(gSpe)/sizeof(TBlkIppSpe); i++)
		this->m_cboSpeEffect.InsertString(i, gSpe[i].desc);
	this->m_option.spe_effect = IPPSEPHIA;
	this->m_option.uoffset = 0x80;
	this->m_option.voffset = 0x80;
	this->m_option.uup = 0xff;
	this->m_option.udown = 0x0;
	this->m_option.vup = 0xff;
	this->m_option.vdown = 0x0;
	this->m_option.patternwidth = 32;
	this->m_option.patternheight = 32;
	this->m_option.width = 640;
	this->m_option.height = 480;
	this->m_option.rawdata = 0;
	this->m_option.unit = 1;
	this->m_option.outputbmp = 1;
	this->m_option.shift_maxmove = 16;
	this->m_option.tv_deg = 20;
	this->m_option.srcwidth = 640;
	this->m_option.srcheight = 480;
	this->m_option.dstwidth = 1280;
	this->m_option.dstheight = 960;
}

void CBlkIppDlg::DispOption()
{
	char str[10];

	this->AdjustBarValue();
	//this->GetUserOption();

	this->m_cboSpeEffect.SetCurSel(this->m_option.spe_effect);

	sprintf(str, "%x", this->m_option.uoffset);
	this->m_edit_u_offset.SetWindowText(str);
	sprintf(str, "%x", this->m_option.voffset);
	this->m_edit_v_offset.SetWindowText(str);
	sprintf(str, "%x", this->m_option.udown);
	this->m_edit_u_down.SetWindowText(str);
	sprintf(str, "%x", this->m_option.uup);
	this->m_edit_u_up.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vdown);
	this->m_edit_v_down.SetWindowText(str);
	sprintf(str, "%x", this->m_option.vup);
	this->m_edit_v_up.SetWindowText(str);
	sprintf(str, "%d", this->m_option.patternwidth);
	this->m_edit_patternwidth.SetWindowText(str);
	sprintf(str, "%d", this->m_option.patternheight);
	this->m_edit_patternheight.SetWindowText(str);
	sprintf(str, "%d", this->m_option.width);
	this->m_edit_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.height);
	this->m_edit_height.SetWindowText(str);
	sprintf(str, "%d", this->m_option.unit);
	this->m_edit_unit.SetWindowText(str);
	sprintf(str, "%d", this->m_option.shift_maxmove);
	this->m_edit_shift_maxmove.SetWindowText(str);
	sprintf(str, "%d", this->m_option.tv_deg);
	this->m_edit_tv_deg.SetWindowText(str);
	sprintf(str, "%d", this->m_option.srcwidth);
	this->m_edit_src_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.srcheight);
	this->m_edit_src_height.SetWindowText(str);
	sprintf(str, "%d", this->m_option.dstwidth);
	this->m_edit_dst_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.dstheight);
	this->m_edit_dst_height.SetWindowText(str);

	if(this->m_option.rawdata)
		this->m_rawdata.SetCheck(BST_CHECKED);
	else
		this->m_rawdata.SetCheck(BST_UNCHECKED);
	if(this->m_option.outputbmp)
		this->m_outputbmp.SetCheck(BST_CHECKED);
	else
		this->m_outputbmp.SetCheck(BST_UNCHECKED);
	this->m_cboSrcFile2.ShowWindow(SW_SHOW);
}

void CBlkIppDlg::AdjustBarValue(void)
{
	this->m_bar_u_offset.SetScrollRange(0, 0xff);
	this->m_bar_u_offset.SetScrollPos(this->m_option.uoffset);
	this->m_bar_v_offset.SetScrollRange(0, 0xff);
	this->m_bar_v_offset.SetScrollPos(this->m_option.voffset);
	this->m_bar_u_down.SetScrollRange(0, 0xff);
	this->m_bar_u_down.SetScrollPos(this->m_option.udown);
	this->m_bar_u_up.SetScrollRange(0, 0xff);
	this->m_bar_u_up.SetScrollPos(this->m_option.uup);
	this->m_bar_v_down.SetScrollRange(0, 0xff);
	this->m_bar_v_down.SetScrollPos(this->m_option.vdown);
	this->m_bar_v_up.SetScrollRange(0, 0xff);
	this->m_bar_v_up.SetScrollPos(this->m_option.vup);	
}

void CBlkIppDlg::OnCbnSelchangeComboSpecialEffect()
{
	this->m_option.spe_effect = gSpe[this->m_cboSpeEffect.GetCurSel()].spe;
}

void CBlkIppDlg::GetCombo(char *tail)
{
	this->m_option.spe_effect = this->m_cboSpeEffect.GetCurSel();
	this->m_cboSrcFile2.GetWindowText(this->m_option.srcfile2, DEFAULT_FILENAME_LEN);
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CBlkIppDlg::GetEditValue(void)
{
	char str[100];

	this->m_edit_patternwidth.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.patternwidth);
	this->m_edit_patternheight.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.patternheight);
	this->m_edit_width.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.width);
	this->m_edit_height.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.height);
	this->m_edit_unit.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.unit);
	this->m_edit_shift_maxmove.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.shift_maxmove);
	this->m_edit_tv_deg.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.tv_deg);
	this->m_edit_src_width.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.srcwidth);
	this->m_edit_src_height.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.srcheight);
	this->m_edit_dst_width.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.dstwidth);
	this->m_edit_dst_height.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.dstheight);
}

void CBlkIppDlg::GetCheckValue(void)
{
	if((this->m_rawdata.GetCheck() == BST_CHECKED) && this->m_rawdata.IsWindowEnabled())
		this->m_option.rawdata = 1;
	else
		this->m_option.rawdata = 0;
	if((this->m_outputbmp.GetCheck() == BST_CHECKED) && this->m_outputbmp.IsWindowEnabled())
		this->m_option.outputbmp = 1;
	else
		this->m_option.outputbmp = 0;
}

void CBlkIppDlg::GetUserOption(char *tail)
{
	this->GetCombo(tail);
	this->GetBarValue();
	this->GetEditValue();
	this->GetCheckValue();
}

void CBlkIppDlg::GetBarValue(void)
{
	this->m_option.uoffset = (unsigned short)this->m_bar_u_offset.GetScrollPos();
	this->m_option.voffset = (unsigned short)this->m_bar_v_offset.GetScrollPos();
	this->m_option.uup = (unsigned short)this->m_bar_u_up.GetScrollPos();
	this->m_option.udown = (unsigned short)this->m_bar_u_down.GetScrollPos();
	this->m_option.vup = (unsigned short)this->m_bar_v_up.GetScrollPos();
	this->m_option.vdown = (unsigned short)this->m_bar_v_down.GetScrollPos();
	if(this->m_option.uup < this->m_option.udown)
		this->m_option.uup = this->m_option.udown;
	if(this->m_option.vup < this->m_option.vdown)
		this->m_option.vup = this->m_option.vdown;

}

void CBlkIppDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			pScrollBar->SetScrollPos(pos + 2);
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
			nPos = ((nPos + 1) >> 1) << 1;
			pScrollBar->SetScrollPos(nPos);
			break;
		default:
			break;
		}
		this->GetBarValue();
		this->DispOption();
	}

	CBasePanelDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBlkIppDlg::OnBnClickedBtnDocmodule()
{
	C326SpecialEffect ippModule(640, 480);
	CFileBin fpIn, fpOut, fpEdge;
	CBufferBase srcBuf, dstBuf, edgeBuf;
	int size,edgeSize;

	this->GetUserOption();
	ippModule.SetSize(this->m_option.width, this->m_option.height);
	ippModule.SetEffect(this->m_option.spe_effect);
	ippModule.SetUOffset(this->m_option.uoffset);
	ippModule.SetVOffset(this->m_option.voffset);
	ippModule.SetUUp(this->m_option.uup);
	ippModule.SetVUp(this->m_option.vup);
	ippModule.SetUDown(this->m_option.udown);
	ippModule.SetVDown(this->m_option.vdown);
	fpIn.Open(this->m_option.srcfile);
	size = fpIn.GetFileLength();
	if(!size)
	{
		fpIn.Close();
		return;
	}
	srcBuf.SetSize(size, 1);
	fpIn.Read(srcBuf.m_buf, size);
	dstBuf.SetSize(size, 1);
	if(this->m_option.spe_effect == IPPSKETCH)
	{
		//strcat(this->m_option.srcfile, ".edge");
		fpEdge.Open(this->m_option.srcfile2);
		edgeSize = fpEdge.GetFileLength();
		if(edgeSize)
		{
			edgeBuf.SetSize(edgeSize, 1);
			fpEdge.Read(edgeBuf.m_buf, edgeSize);
		}
	}
	ippModule.Process(srcBuf.m_buf, dstBuf.m_buf, edgeBuf.m_buf);
	fpOut.Open(this->m_option.dstfile, "wb");
	fpOut.Write(dstBuf.m_buf, size);
	fpOut.Close();
	if(this->m_option.outputbmp)
	{
		char bmpf[200];
		CRaw2RgbF rgb2bmp;
		CYuv422F yuv2rgb;

		strcpy(bmpf, this->m_option.dstfile);
		strcat(bmpf, ".rgb");
		yuv2rgb.Yuyv2Rgb(this->m_option.dstfile, bmpf, this->m_option.width, this->m_option.height);
		strcpy(this->m_option.dstfile, bmpf);
		strcat(bmpf, ".bmp");
		rgb2bmp.Rgb24_Bmp(this->m_option.dstfile, bmpf, this->m_option.width, this->m_option.height);
	}
	
}

void CBlkIppDlg::OnBnClickedBtnOpenfile()
{
	CString str;

	if(CBaseDlg::OpenFileDlg(&str))
	{
		this->m_cboSrcFile2.addString(str.GetBuffer());
		//len = this->m_cboSrcFile2.GetCurSel();
		//this->m_cboSrcFile2.SetCurSel(len);
	}
}

void CBlkIppDlg::OnBnClickedBtnNewCmodule()
{
	CFileBin fpIn, fpIn2, fpOut;
	CBufferBase srcBuf, dstBuf, srcBuf2;
	int size, size2;

	this->GetUserOption();
	
	fpIn.Open(this->m_option.srcfile);
	fpIn2.Open(this->m_option.srcfile2);
	size = fpIn.GetFileLength();
	if(!size)
		return;
	size2 = fpIn2.GetFileLength();
	if(this->m_option.rawdata)
	{
		srcBuf.SetSize(size*3*2);
		dstBuf.SetSize(size*3*2);
	}
	else
	{
		srcBuf.SetSize(size);
		dstBuf.SetSize(size);
	}
	fpIn.Read(srcBuf.m_buf, size);
	if(size2)
	{
		srcBuf2.SetSize(size2);
		fpIn2.Read(srcBuf2.m_buf, size2);
	}
	if(this->m_option.rawdata)	//isp process
	{
		CIspModule326	ispModule(this->m_option.width, this->m_option.height, this->m_option.unit);
		ispModule.SetRawData(this->m_option.rawdata);
		ispModule.SetBGLine(1);
		ispModule.SetGpixel(1);
		ispModule.SetIspCtrl(0x3f8);
		ispModule.SetNewEdge(1);
		size = ispModule.Process(srcBuf.m_buf, dstBuf.m_buf);
		memcpy(srcBuf.m_buf, dstBuf.m_buf, size);
	}
	if(this->m_option.spe_effect == IPPCANVAS)
	{
		C326Canvas canvas(this->m_option.width, this->m_option.height);
		canvas.SetPatternFormat(YUV422_YUYV);
		canvas.SetPatternSize(this->m_option.patternwidth, this->m_option.patternheight);
		canvas.Process(srcBuf.m_buf, srcBuf2.m_buf, dstBuf.m_buf);
	}
	if(this->m_option.spe_effect == IPPTV)
	{
		C326Tv tv(this->m_option.width, this->m_option.height);
		tv.SetDeg(this->m_option.tv_deg);
		tv.Process(srcBuf.m_buf, dstBuf.m_buf);
	}
	if(this->m_option.spe_effect == IPPSHIFT)
	{
		C326Shift shift(this->m_option.width, this->m_option.height);
		shift.SetMaxMove(this->m_option.shift_maxmove);
		shift.Process(srcBuf.m_buf, dstBuf.m_buf);
	}
	if(this->m_option.spe_effect == IPPWINDBLOW)
	{
		C326WindBlow wind(this->m_option.width, this->m_option.height);
		wind.SetThreshold(20);
		wind.Process(srcBuf.m_buf, dstBuf.m_buf);
	}
	if(this->m_option.spe_effect == IPPBINARY)
	{
		C326Binary binary(this->m_option.width, this->m_option.height);
		binary.SetThreshold(0xff);
		binary.Process(srcBuf.m_buf, dstBuf.m_buf);
	}
	if((this->m_option.spe_effect == IPPPAINT) || (this->m_option.spe_effect == IPPPARTICLE))
	{
		C326PaintParticle paintparticle(this->m_option.width, this->m_option.height);
		paintparticle.SetSpecial(this->m_option.spe_effect);
		paintparticle.Process(srcBuf.m_buf, dstBuf.m_buf);
	}
	fpOut.Open(this->m_option.dstfile, "wb");
	fpOut.Write(dstBuf.m_buf, size);
	fpOut.Close();
	if(this->m_option.outputbmp)
	{
		char bmpf[200];
		CRaw2RgbF rgb2bmp;
		CYuv422F yuv2rgb;

		strcpy(bmpf, this->m_option.dstfile);
		strcat(bmpf, ".rgb");
		yuv2rgb.Yuyv2Rgb(this->m_option.dstfile, bmpf, this->m_option.width, this->m_option.height);
		strcpy(this->m_option.dstfile, bmpf);
		strcat(bmpf, ".bmp");
		rgb2bmp.Rgb24_Bmp(this->m_option.dstfile, bmpf, this->m_option.width, this->m_option.height);
	}
}

void CBlkIppDlg::OnEnChangeEditImageWidth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CBasePanelDlg::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CBlkIppDlg::OnBnClickedBtnSizer()
{
	CFileBin fpIn, fpOut;
	CBufferBase srcBuf, dstBuf;
	int size, dstsize;
	int srcw, srch, dstw, dsth;
	int	ceilw, ceilh, floorw, floorh;
	int	sepw, seph;
	int div;
	C578IppSizer sizer;
	C578IppZoomIn	sizerin;
	char bmpf[200];
	CRaw2RgbF rgb2bmp;
	CYuv422F yuv2rgb;

	this->GetUserOption();
	
	fpIn.Open(this->m_option.srcfile);
	size = fpIn.GetFileLength();
	if(!size)
		return;
	srcBuf.SetSize(size);
	fpIn.Read(srcBuf.m_buf, size);
	fpOut.Open(this->m_option.dstfile, "wb");
	dstsize = this->m_option.dstwidth * this->m_option.dstheight * 2;
	dstBuf.SetSize(dstsize);
	div = this->MaxDiv(this->m_option.srcwidth, this->m_option.dstwidth);
	srcw = this->m_option.srcwidth / div;
	dstw = this->m_option.dstwidth / div;
	if(srcw > 128)
	{
		dstw = (dstw * 128)/srcw + 1;
		srcw = 128;
	}
	div = this->MaxDiv(this->m_option.srcheight, this->m_option.dstheight);
	srch = this->m_option.srcheight / div;
	dsth = this->m_option.dstheight / div;
	if(srch > 128)
	{
		dsth = (dsth * 128)/srch + 1;
		srch = 128;
	}
	if(srcw >= dstw)	//zoom out
	{
		ceilw = this->GetCeil(srcw, dstw);
		ceilh = this->GetCeil(srch, dsth);
		floorw = this->GetFloor(srcw, dstw);
		floorh = this->GetFloor(srch, dsth);
		//ceilw = (int)ceil(srcw/dstw);
		//ceilh = (int)ceil(srch/dsth);
		//floorw = (int)floor(srcw/dstw);
		//floorh = (int)floor(srch/dsth);
		sepw = dstw - srcw%dstw;
		seph = dsth - srch%dsth;
		sizer.SetSrcSize(this->m_option.srcwidth, this->m_option.srcheight);
		sizer.SetDstSize(this->m_option.dstwidth, this->m_option.dstheight);
		sizer.SetSrcW(srcw);
		sizer.SetSrcH(srch);
		sizer.SetDstW(dstw);
		sizer.SetDstH(dsth);
		sizer.SetCeilW(ceilw);
		sizer.SetCeilH(ceilh);
		sizer.SetFloorW(floorw);
		sizer.SetFloorH(floorh);
		sizer.SetSepW(sepw);
		sizer.SetSepH(seph);
		size = sizer.Process(srcBuf.m_buf, dstBuf.m_buf);
	}
	else
	{
		sizerin.SetSrcSize(this->m_option.srcwidth, this->m_option.srcheight);
		sizerin.SetDstSize(this->m_option.dstwidth, this->m_option.dstheight);
		sizerin.SetSrcW(srcw);
		sizerin.SetSrcH(srch);
		sizerin.SetDstW(dstw);
		sizerin.SetDstH(dsth);
		sizerin.SetParaW(dstw);
		sizerin.SetParaH(dsth);
		size = sizerin.Process(srcBuf.m_buf, dstBuf.m_buf);
	}
	fpOut.Write(dstBuf.m_buf, size);
	fpOut.Close();
//	output bmp
	strcpy(bmpf, this->m_option.dstfile);
	strcat(bmpf, ".rgb");
	yuv2rgb.Yuyv2Rgb(this->m_option.dstfile, bmpf, this->m_option.dstwidth, this->m_option.dstheight);
	strcpy(this->m_option.dstfile, bmpf);
	strcat(bmpf, ".bmp");
	rgb2bmp.Rgb24_Bmp(this->m_option.dstfile, bmpf, this->m_option.dstwidth, this->m_option.dstheight);
}

int	CBlkIppDlg::MaxDiv(int a, int b)
{
	while(a && b)
	{
		if(a > b)
			a %= b;
		else
			b %= a;
	}
	if(a)
		return a;
	return b;
}

int CBlkIppDlg::GetCeil(int a, int b)
{
	return (a/b) + (a%b == 0 ? 0 : 1);
}

int CBlkIppDlg::GetFloor(int a, int b)
{
	return a/b;
}
