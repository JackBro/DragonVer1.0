// BlkGeDlg.cpp : 实现文件
//
#include "internal.h"


// CBlkGeDlg 对话框

IMPLEMENT_DYNAMIC(CBlkGeDlg, CBasePanelDlg)
CBlkGeDlg::CBlkGeDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CBlkGeDlg::IDD, pParent)
{
}

CBlkGeDlg::~CBlkGeDlg()
{
}

void CBlkGeDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DST_WIDTH, m_edit_width);
	DDX_Control(pDX, IDC_EDIT_DST_HEIGHT, m_edit_height);
	DDX_Control(pDX, IDC_EDIT_BIT_WISE, m_edit_bitwise);
	DDX_Control(pDX, IDC_EDIT_PATTERN_BG, m_edit_patternbg);
	DDX_Control(pDX, IDC_EDIT_PATTERN_FG, m_edit_patternfg);
	DDX_Control(pDX, IDC_EDIT_SRC_BG, m_edit_srcbg);
	DDX_Control(pDX, IDC_EDIT_SRC_FG, m_edit_srcfg);
	DDX_Control(pDX, IDC_CHECK_PATTERN_MASK, m_pattern_mask);
	DDX_Control(pDX, IDC_CHECK_SRC_MASK, m_src_mask);
	DDX_Control(pDX, IDC_CBO_DST_COLORDEPTH, m_cbo_dst_colordepth);
	DDX_Control(pDX, IDC_CBO_SRC_COLORDEPTH, m_cbo_src_colordepth);
	DDX_Control(pDX, IDC_CBO_PATTERN_COLORDEPTH, m_cbo_pattern_colordepth);
	DDX_Control(pDX, IDC_CBO_SRCFILE2, m_cbo_src_file2);
	DDX_Control(pDX, IDC_EDIT_COLOR_TRANS_SEL, m_edit_colortranssel);
	DDX_Control(pDX, IDC_CHECK_COLOR_TRANS, m_color_trans_en);
	DDX_Control(pDX, IDC_CHECK_SRC_EXTEND, m_src_extend);
	DDX_Control(pDX, IDC_CBO_START_POINT, m_cbo_start_point);
	DDX_Control(pDX, IDC_EDIT_DST_START, m_edit_dst_start);
	DDX_Control(pDX, IDC_EDIT_SRC_START, m_edit_src_start);
	DDX_Control(pDX, IDC_EDIT_SPAN, m_edit_span);
	DDX_Control(pDX, IDC_EDIT_LINE_COLOR, m_edit_linecolor);
	DDX_Control(pDX, IDC_EDIT_START_X, m_edit_start_x);
	DDX_Control(pDX, IDC_EDIT_START_Y, m_edit_start_y);
	DDX_Control(pDX, IDC_EDIT_END_X, m_edit_end_x);
	DDX_Control(pDX, IDC_EDIT_END_Y, m_edit_end_y);
	DDX_Control(pDX, IDC_EDIT_CLIP_LEFT_X, m_edit_clip_left_x);
	DDX_Control(pDX, IDC_EDIT_CLIP_LEFT_Y, m_edit_clip_left_y);
	DDX_Control(pDX, IDC_EDIT_CLIP_RIGHT_X, m_edit_clip_right_x);
	DDX_Control(pDX, IDC_EDIT_CLIP_RIGHT_Y, m_edit_clip_right_y);
}

BEGIN_MESSAGE_MAP(CBlkGeDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_BTN_OPENFILE, OnBnClickedBtnOpenfile)
	ON_BN_CLICKED(IDC_BTN_BITBLT, OnBnClickedBtnBitblt)
	ON_BN_CLICKED(IDC_BTN_INIT_GE, OnBnClickedBtnInitGe)
	ON_BN_CLICKED(IDC_BTN_LINE_DRAW, OnBnClickedBtnLineDraw)
END_MESSAGE_MAP()


// CBlkGeDlg 消息处理程序

const static  TBlkGeColorDepth gDepth[] = 
{
	{"MONO",	GE_MONOCOLOR},
	{"RGB16",	GE_RGB16},
	{"RGB24",	GE_RGB24}
};

const static TBlkGeStartPoint gStartPoint[] = 
{
	{"TopLeft",		GE_TOPLEFT},
	{"TopRight",	GE_TOPRIGHT},
	{"BottomLeft",	GE_BOTTOMLEFT},
	{"BottomRight",	GE_BOTTOMRIGHT}
};

BOOL CBlkGeDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	this->InitOption();
	this->DispOption();

	return TRUE;
}

void CBlkGeDlg::InitOption()
{
	int i;

	memset(&(this->m_option), 0, sizeof(TBlkGeOption));

	for(i = 0; i < sizeof(gDepth)/sizeof(TBlkGeColorDepth); i++)
	{
		this->m_cbo_dst_colordepth.InsertString(i, gDepth[i].desc);
		this->m_cbo_src_colordepth.InsertString(i, gDepth[i].desc);
		this->m_cbo_pattern_colordepth.InsertString(i, gDepth[i].desc);
	}
	for(i = 0; i < sizeof(gStartPoint)/sizeof(TBlkGeStartPoint); i++)
		this->m_cbo_start_point.InsertString(i, gStartPoint[i].desc);
	this->m_option.bitwise = 0xcc;
	this->m_option.dstcolordepth = GE_RGB24;
	this->m_option.patterncolordepth = GE_MONOCOLOR;
	this->m_option.srccolordepth = GE_RGB24;
	this->m_option.startpoint = GE_TOPLEFT;
	this->m_option.width = 128;
	this->m_option.height = 160;
	this->m_option.patternbg = 0xffffff;
	this->m_option.patternfg = 0;
	this->m_option.patternmask = 0;
	this->m_option.srcbg = 0xffffff;
	this->m_option.srcfg = 0;
	this->m_option.srcmask = 0;
	this->m_option.colortrnasen = 0;
	this->m_option.colortranssel = 0;
	this->m_option.srcextend = 0;
	this->m_option.dststart = 0;
	this->m_option.srcstart = 62400;
	this->m_option.span = 130;
	this->m_option.linecolor = 0x0;
	this->m_option.start.cx = 0;
	this->m_option.start.cy = 0;
	this->m_option.end.cx = 20;
	this->m_option.end.cy = 10;
	this->m_option.clipleft.cx = 0;
	this->m_option.clipleft.cy = 0;
	this->m_option.clipright.cx = 0;
	this->m_option.clipright.cy = 0;
}

void CBlkGeDlg::DispOption()
{
	char str[10];

	this->m_cbo_dst_colordepth.SetCurSel(this->m_option.dstcolordepth);
	this->m_cbo_pattern_colordepth.SetCurSel(this->m_option.patterncolordepth);
	this->m_cbo_src_colordepth.SetCurSel(this->m_option.srccolordepth);
	this->m_cbo_start_point.SetCurSel(this->m_option.startpoint);

	sprintf(str, "%d", this->m_option.width);
	this->m_edit_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.height);
	this->m_edit_height.SetWindowText(str);
	sprintf(str, "%x", this->m_option.bitwise);
	this->m_edit_bitwise.SetWindowText(str);
	sprintf(str, "%x", this->m_option.patternbg);
	this->m_edit_patternbg.SetWindowText(str);
	sprintf(str, "%x", this->m_option.patternfg);
	this->m_edit_patternfg.SetWindowText(str);
	sprintf(str, "%x", this->m_option.srcbg);
	this->m_edit_srcbg.SetWindowText(str);
	sprintf(str, "%x", this->m_option.srcfg);
	this->m_edit_srcfg.SetWindowText(str);
	sprintf(str, "%x", this->m_option.colortranssel);
	this->m_edit_colortranssel.SetWindowText(str);
	sprintf(str, "%d", this->m_option.dststart);
	this->m_edit_dst_start.SetWindowText(str);
	sprintf(str, "%d", this->m_option.srcstart);
	this->m_edit_src_start.SetWindowText(str);
	sprintf(str, "%d", this->m_option.span);
	this->m_edit_span.SetWindowText(str);
	sprintf(str, "%d", this->m_option.linecolor);
	this->m_edit_linecolor.SetWindowText(str);
	sprintf(str, "%d", this->m_option.start.cx);
	this->m_edit_start_x.SetWindowText(str);
	sprintf(str, "%d", this->m_option.start.cy);
	this->m_edit_start_y.SetWindowText(str);
	sprintf(str, "%d", this->m_option.end.cx);
	this->m_edit_end_x.SetWindowText(str);
	sprintf(str, "%d", this->m_option.end.cy);
	this->m_edit_end_y.SetWindowText(str);
	sprintf(str, "%d", this->m_option.clipleft.cx);
	this->m_edit_clip_left_x.SetWindowText(str);
	sprintf(str, "%d", this->m_option.clipleft.cy);
	this->m_edit_clip_left_y.SetWindowText(str);
	sprintf(str, "%d", this->m_option.clipright.cx);
	this->m_edit_clip_right_x.SetWindowText(str);
	sprintf(str, "%d", this->m_option.clipright.cy);
	this->m_edit_clip_right_y.SetWindowText(str);
	
	if(this->m_option.patternmask)
		this->m_pattern_mask.SetCheck(BST_CHECKED);
	else
		this->m_pattern_mask.SetCheck(BST_UNCHECKED);
	if(this->m_option.srcmask)
		this->m_src_mask.SetCheck(BST_CHECKED);
	else
		this->m_src_mask.SetCheck(BST_UNCHECKED);
	if(this->m_option.colortrnasen)
		this->m_color_trans_en.SetCheck(BST_CHECKED);
	else
		this->m_color_trans_en.SetCheck(BST_UNCHECKED);
	if(this->m_option.srcextend)
		this->m_src_extend.SetCheck(BST_CHECKED);
	else
		this->m_src_extend.SetCheck(BST_UNCHECKED);

	this->m_cbo_src_file2.ShowWindow(SW_SHOW);
}

void CBlkGeDlg::GetCombo(char *tail)
{
	this->m_option.dstcolordepth = this->m_cbo_dst_colordepth.GetCurSel();
	this->m_option.srccolordepth = this->m_cbo_src_colordepth.GetCurSel();
	this->m_option.patterncolordepth = this->m_cbo_pattern_colordepth.GetCurSel();
	this->m_option.startpoint = this->m_cbo_start_point.GetCurSel();
	this->m_cbo_src_file2.GetWindowText(this->m_option.srcfile2, DEFAULT_FILENAME_LEN);
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CBlkGeDlg::GetEditValue(void)
{
	char str[100];

	this->m_edit_bitwise.GetWindowText(str, 100);
	sscanf(str, "%x", &this->m_option.bitwise);
	this->m_edit_width.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.width);
	this->m_edit_height.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.height);
	this->m_edit_patternbg.GetWindowText(str, 100);
	sscanf(str, "%x", &this->m_option.patternbg);
	this->m_edit_patternfg.GetWindowText(str, 100);
	sscanf(str, "%x", &this->m_option.patternfg);
	this->m_edit_srcbg.GetWindowText(str, 100);
	sscanf(str, "%x", &this->m_option.srcbg);
	this->m_edit_srcfg.GetWindowText(str, 100);
	sscanf(str, "%x", &this->m_option.srcfg);
	this->m_edit_colortranssel.GetWindowText(str, 100);
	sscanf(str, "%x", &this->m_option.colortranssel);
	this->m_edit_dst_start.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.dststart);
	this->m_edit_src_start.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.srcstart);
	this->m_edit_span.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.span);
	this->m_edit_linecolor.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.linecolor);
	this->m_edit_start_x.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.start.cx);
	this->m_edit_start_y.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.start.cy);
	this->m_edit_end_x.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.end.cx);
	this->m_edit_end_y.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.end.cy);
	this->m_edit_clip_left_x.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.clipleft.cx);
	this->m_edit_clip_left_y.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.clipleft.cy);
	this->m_edit_clip_right_x.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.clipright.cx);
	this->m_edit_clip_right_y.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.clipright.cy);
}

void CBlkGeDlg::GetCheckValue(void)
{
	if((this->m_pattern_mask.GetCheck() == BST_CHECKED) && this->m_pattern_mask.IsWindowEnabled())
		this->m_option.patternmask = 1;
	else
		this->m_option.patternmask = 0;
	if((this->m_src_mask.GetCheck() == BST_CHECKED) && this->m_src_mask.IsWindowEnabled())
		this->m_option.srcmask = 1;
	else
		this->m_option.srcmask = 0;
	if((this->m_color_trans_en.GetCheck() == BST_CHECKED) && this->m_color_trans_en.IsWindowEnabled())
		this->m_option.colortrnasen = 1;
	else
		this->m_option.colortrnasen = 0;
	if((this->m_src_extend.GetCheck() == BST_CHECKED) && this->m_src_extend.IsWindowEnabled())
		this->m_option.srcextend = 1;
	else
		this->m_option.srcextend = 0;
}

void CBlkGeDlg::GetUserOption(char *tail)
{
	this->GetCombo(tail);
	this->GetEditValue();
	this->GetCheckValue();
}

void CBlkGeDlg::OnBnClickedBtnOpenfile()
{
	CString str;

	if(CBaseDlg::OpenFileDlg(&str))
	{
		this->m_cbo_src_file2.addString(str.GetBuffer());
	}
}

void CBlkGeDlg::OnBnClickedBtnBitblt()
{
	CFileBin fpSrc, fpPattern, fpDst;
	CBufferBase srcBuf, dstBuf, patternBuf;
	int dstSize, srcSize, patternSize;
	char bmpf[200];
	CRaw2RgbF rgb2bmp;

	this->GetUserOption();
	
	fpSrc.Open(this->m_option.srcfile);
	fpPattern.Open(this->m_option.srcfile2);
	fpDst.Open(this->m_option.dstfile);
	srcSize = fpSrc.GetFileLength();
	dstSize = fpDst.GetFileLength();
	if(!srcSize || !dstSize )
		return;
	patternSize = fpPattern.GetFileLength();
	dstBuf.SetSize(dstSize);
	srcBuf.SetSize(srcSize);
	fpSrc.Read(srcBuf.m_buf, srcSize);
	fpDst.Read(dstBuf.m_buf, dstSize);
	ge.SetDestSize(this->m_option.width, this->m_option.height);
	ge.SetBitbltColorDepth(this->m_option.dstcolordepth);
	ge.SetPatternColorDepth(this->m_option.patterncolordepth);
	ge.SetSrcColorDepth(this->m_option.srccolordepth);
	ge.SetBitWise(this->m_option.bitwise);
	ge.SetPatternBg(this->m_option.patternbg);
	ge.SetPatternFg(this->m_option.patternfg);
	ge.SetSrcBg(this->m_option.srcbg);
	ge.SetSrcFg(this->m_option.srcfg);
	ge.SetPatternMask(this->m_option.patternmask);
	ge.SetSrcMask(this->m_option.srcmask);
	ge.SetColorTransSel(this->m_option.colortranssel);
	ge.SetColorTrnasEn(this->m_option.colortrnasen);
	ge.SetSrcExtendSel(this->m_option.srcextend);
	ge.SetStartPointSel(this->m_option.startpoint);
	ge.SetSpan(this->m_option.span);
	ge.SetDestAddrOffset(this->m_option.dststart);
	ge.SetSrcAddrOffset(this->m_option.srcstart);
	ge.SetDestBuf(dstBuf.m_buf, dstSize);
	ge.SetSrcBuf(srcBuf.m_buf, srcSize);
	if(patternSize)
	{
		patternBuf.SetSize(patternSize);
		fpPattern.Read(patternBuf.m_buf, patternSize);
		ge.SetPattern(patternBuf.m_buf, patternSize);
	}
	ge.StartBitblt();
	ge.GetDestBuf(dstBuf.m_buf, dstSize);
//	ge.Process(srcBuf.m_buf, dstBuf.m_buf);
	fpSrc.Close();
	fpPattern.Close();
	fpDst.Close();
	strcat(this->m_option.dstfile, ".new");
	fpDst.Open(this->m_option.dstfile, "wb");
	fpDst.Write(dstBuf.m_buf, dstSize);
	fpDst.Close();
	//	output bmp file
	strcpy(bmpf, this->m_option.dstfile);
	strcat(bmpf, ".bmp");
	if(this->m_option.dstcolordepth == GE_RGB24)
		rgb2bmp.Rgb24_Bmp(this->m_option.dstfile, bmpf, this->m_option.width, this->m_option.height);
	else
		rgb2bmp.Rgb555_Bmp(this->m_option.dstfile, bmpf, this->m_option.width, this->m_option.height);
}

void CBlkGeDlg::OnBnClickedBtnInitGe()
{
	
}

void CBlkGeDlg::OnBnClickedBtnLineDraw()
{
	CFileBin fp;
	CBufferBase buf;
	int size;
	char bmpf[200];
	CRaw2RgbF rgb2bmp;

	this->GetUserOption();
	
	fp.Open(this->m_option.srcfile);
	size = fp.GetFileLength();
	if(!size)
		return;
	buf.SetSize(size);
	fp.Read(buf.m_buf, size);
	ge.SetDestSize(this->m_option.width, this->m_option.height);
	ge.SetBitbltColorDepth(this->m_option.dstcolordepth);
	ge.SetSpan(this->m_option.span);
	ge.SetDestAddrOffset(this->m_option.dststart);
	ge.SetDestBuf(buf.m_buf, size);
	ge.SetLinecolor(this->m_option.linecolor);
	ge.SetLineDrawStart(this->m_option.start.cx, this->m_option.start.cy);
	ge.SetLineDrawEnd(this->m_option.end.cx, this->m_option.end.cy);
	ge.SetClipLeftTop(this->m_option.clipleft.cx, this->m_option.clipleft.cy);
	ge.SetClipRightBottom(this->m_option.clipright.cx, this->m_option.clipright.cy);
	ge.StartLineDraw();
	ge.GetDestBuf(buf.m_buf, size);
	fp.Close();
	fp.Open(this->m_option.dstfile, "wb");
	fp.Write(buf.m_buf, size);
	fp.Close();
	//	output bmp file
	strcpy(bmpf, this->m_option.dstfile);
	strcat(bmpf, ".bmp");
	if(this->m_option.dstcolordepth == GE_RGB24)
		rgb2bmp.Rgb24_Bmp(this->m_option.dstfile, bmpf, this->m_option.width, this->m_option.height);
	else
		rgb2bmp.Rgb555_Bmp(this->m_option.dstfile, bmpf, this->m_option.width, this->m_option.height);
}
