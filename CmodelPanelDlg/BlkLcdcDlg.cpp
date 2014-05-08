// BlkLcdcDlg.cpp : 实现文件
//
#include "internal.h"
#include "..\include\cmodelpaneldlg\blklcdcdlg.h"


// CBlkLcdcDlg 对话框

IMPLEMENT_DYNAMIC(CBlkLcdcDlg, CBasePanelDlg)
CBlkLcdcDlg::CBlkLcdcDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CBlkLcdcDlg::IDD, pParent)
{
}

CBlkLcdcDlg::~CBlkLcdcDlg()
{
}

void CBlkLcdcDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROTATION, m_cboRotation);
	DDX_Control(pDX, IDC_EDIT_LCDC_WIDTH, m_edit_lcd_width);
	DDX_Control(pDX, IDC_EDIT_LCDC_HEIGHT, m_edit_lcd_height);
	DDX_Control(pDX, IDC_EDIT_AL_WIDTH, m_edit_al_width);
	DDX_Control(pDX, IDC_EDIT_AL_HEIGHT, m_edit_al_height);
	DDX_Control(pDX, IDC_EDIT_MEM_WIDTH, m_edit_mem_width);
	DDX_Control(pDX, IDC_EDIT_MEM_HEIGHT, m_edit_mem_height);
	DDX_Control(pDX, IDC_EDIT_LCD_POINT_X, m_edit_lcd_pointx);
	DDX_Control(pDX, IDC_EDIT_LCD_POINT_Y, m_edit_lcd_pointy);
	DDX_Control(pDX, IDC_EDIT_MEM_POINT_X, m_edit_mem_pointx);
	DDX_Control(pDX, IDC_EDIT_MEM_POINT_Y, m_edit_mem_pointy);
	DDX_Control(pDX, IDC_EDIT_BLAYER_WIDTH, m_edit_bl_width);
	DDX_Control(pDX, IDC_EDIT_BLAYER_HEIGHT, m_edit_bl_height);
	DDX_Control(pDX, IDC_EDIT_MEM_WIDTH_B, m_edit_mem_width_b);
	DDX_Control(pDX, IDC_EDIT_LCD_POINT_X_B, m_edit_lcd_pointx_b);
	DDX_Control(pDX, IDC_EDIT_LCD_POINT_Y_B, m_edit_lcd_pointy_b);
	DDX_Control(pDX, IDC_EDIT_MEM_POINT_X_B, m_edit_mem_pointx_b);
	DDX_Control(pDX, IDC_EDIT_MEM_POINT_Y_B, m_edit_mem_pointy_b);
	DDX_Control(pDX, IDC_EDIT_B_ADDR, m_edit_baddr);
	DDX_Control(pDX, IDC_COMBO_ROTATION_B, m_cboRotationB);
	DDX_Control(pDX, IDC_COMBO_FORMAT_B, m_cboFormatB);

	DDX_Control(pDX, IDC_COMBO_OVERLAY_MODE, m_cboOverlayMode);
	DDX_Control(pDX, IDC_COMBO_LAYER_SELECT, m_cboLayerSel);
	DDX_Control(pDX, IDC_EDIT_KEY_COLOR, m_edit_keycolor);
	DDX_Control(pDX, IDC_EDIT_BLEND_RATIO, m_edit_blendratio);
	DDX_Control(pDX, IDC_CHECK_ALPHA_BLENDING, m_alphablendingen);
	DDX_Control(pDX, IDC_CHECK_OVERLAY, m_overlayen);
	DDX_Control(pDX, IDC_COMBO_LAYER_PRI, m_cboLayerPri);
}


BEGIN_MESSAGE_MAP(CBlkLcdcDlg, CBasePanelDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_ROTATION, OnCbnSelchangeComboRotation)
	ON_BN_CLICKED(IDC_BTN_PROCESS, OnBnClickedBtnProcess)
	ON_CBN_SELCHANGE(IDC_COMBO_ROTATION_B, OnCbnSelchangeComboRotationB)
	ON_CBN_SELCHANGE(IDC_COMBO_FORMAT_B, OnCbnSelchangeComboFormatB)
	ON_BN_CLICKED(IDC_BTN_PROCESS_B, OnBnClickedBtnProcessB)
	ON_CBN_SELCHANGE(IDC_COMBO_OVERLAY_MODE, OnCbnSelchangeComboOverlayMode)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER_SELECT, OnCbnSelchangeComboLayerSelect)
	ON_BN_CLICKED(IDC_BTN_INIT_GBUF, OnBnClickedBtnInitGbuf)
	ON_BN_CLICKED(IDC_BTN_LCDC_PROCESS, OnBnClickedBtnLcdcProcess)
	ON_CBN_SELCHANGE(IDC_COMBO_LAYER_PRI, OnCbnSelchangeComboLayerPri)
	ON_BN_CLICKED(IDC_BTN_UPDATE_B, OnBnClickedBtnUpdateB)
	ON_BN_CLICKED(IDC_BTN_OUTPUT_BMP, OnBnClickedBtnOutputBmp)
END_MESSAGE_MAP()


// CBlkLcdcDlg 消息处理程序

const static  TBlkLcdcRotation gRot[] = 
{
	{"ROTATION_0",		ROTATION_0},
	{"ROTATION_90",		ROTATION_90},
	{"ROTATION_180",	ROTATION_180},
	{"ROTATION_270",	ROTATION_270},
	{"ROTATION_MIRROR",	ROTATION_MIRROR},
	{"ROTATION_MIRROR_90",	ROTATION_MIRROR_90},
	{"ROTATION_MIRROR_180",	ROTATION_MIRROR_180},
	{"ROTATION_MIRROR_270",	ROTATION_MIRROR_270}
};

const static TBlkLcdcFormat gFormat[] = 
{
	{"INDIRECT_RGB1", INDIRECT_RGB1},
	{"INDIRECT_RGB2", INDIRECT_RGB2},
	{"INDIRECT_RGB4", INDIRECT_RGB4},
	{"INDIRECT_RGB8", INDIRECT_RGB8},
	{"DIRECT_RGB444", DIRECT_RGB444},
	{"DIRECT_RGB555_ALPHA", DIRECT_RGB555_ALPHA},
	{"DIRECT_RGB565", DIRECT_RGB565},
	{"DIRECT_RGB666", DIRECT_RGB666},
	{"DIRECT_RGB888", DIRECT_RGB888},
	{"DIRECT_RGB888_ALPHA", DIRECT_RGB888_ALPHA}
};

const static TBlkLcdcOverlayMode gMode[] = 
{
	{"TRANSPARENT",	OVERLAY_TRANSPARENT},
	{"AND",			OVERLAY_AND},
	{"OR",			OVERLAY_OR},
	{"INVERT",		OVERLAY_INVERT}
};

BOOL CBlkLcdcDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	this->InitOption();
	this->DispOption();

	return TRUE;
}

void CBlkLcdcDlg::InitOption()
{
	int i;

	memset(&(this->m_option), 0, sizeof(TBlkLcdcOption));

	for(i = 0; i < sizeof(gRot)/sizeof(TBlkLcdcRotation); i++)
		this->m_cboRotation.InsertString(i, gRot[i].desc);
	for(i = 0; i < sizeof(gRot)/sizeof(TBlkLcdcRotation); i++)
		this->m_cboRotationB.InsertString(i, gRot[i].desc);
	for(i = 0; i < sizeof(gFormat)/sizeof(TBlkLcdcFormat); i++)
		this->m_cboFormatB.InsertString(i, gFormat[i].desc);
	for(i = 0; i < sizeof(gMode)/sizeof(TBlkLcdcOverlayMode); i++)
		this->m_cboOverlayMode.InsertString(i, gMode[i].desc);
	this->m_cboLayerSel.InsertString(0, "Layer B0");
	this->m_cboLayerSel.InsertString(1, "Layer B1");
	this->m_cboLayerPri.InsertString(0, "A Priority");
	this->m_cboLayerPri.InsertString(1, "B Priority");

	this->m_option.rotation = ROTATION_0;
	this->m_option.memwidth = 128;
	this->m_option.memheight = 160;
	this->m_option.alwidth = 120;
	this->m_option.alheight = 160;
	this->m_option.lcdwidth = 120;
	this->m_option.lcdheight = 160;
	this->m_option.mempointx = 0;
	this->m_option.mempointy = 0;
	this->m_option.lcdpointx = 0;
	this->m_option.lcdpointy = 0;
	this->m_option.baddr = 0;
	this->m_option.bformat = DIRECT_RGB888;
	this->m_option.blcdpointx = 0;
	this->m_option.blcdpointy = 0;
	this->m_option.blwidth = 120;
	this->m_option.blheight = 160;
	this->m_option.bmempointx = 0;
	this->m_option.bmempointy = 0;
	this->m_option.bmemwidth = 128;
	this->m_option.brotation = ROTATION_0;

	this->m_option.alphablendingen = 0;
	this->m_option.overlayen = 0;
	this->m_option.keycolor = 0xffffff;
	this->m_option.blendratio = 1;
	this->m_option.layerpri = 1;
	this->m_lcdbuf.SetSize(0xe100);
}

void CBlkLcdcDlg::DispOption()
{
	char str[10];

	this->m_cboRotation.SetCurSel(this->m_option.rotation);
	this->m_cboRotationB.SetCurSel(this->m_option.brotation);
	this->m_cboFormatB.SetCurSel(this->m_option.bformat);
	this->m_cboOverlayMode.SetCurSel(this->m_option.overlaymode);
	this->m_cboLayerSel.SetCurSel(0);
	this->m_cboLayerPri.SetCurSel(this->m_option.layerpri);

	sprintf(str, "%d", this->m_option.memwidth);
	this->m_edit_mem_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.memheight);
	this->m_edit_mem_height.SetWindowText(str);
	sprintf(str, "%d", this->m_option.lcdwidth);
	this->m_edit_lcd_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.lcdheight);
	this->m_edit_lcd_height.SetWindowText(str);
	sprintf(str, "%d", this->m_option.alwidth);
	this->m_edit_al_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.alheight);
	this->m_edit_al_height.SetWindowText(str);
	sprintf(str, "%d", this->m_option.mempointx);
	this->m_edit_mem_pointx.SetWindowText(str);
	sprintf(str, "%d", this->m_option.mempointy);
	this->m_edit_mem_pointy.SetWindowText(str);
	sprintf(str, "%d", this->m_option.lcdpointx);
	this->m_edit_lcd_pointx.SetWindowText(str);
	sprintf(str, "%d", this->m_option.lcdpointy);
	this->m_edit_lcd_pointy.SetWindowText(str);

	sprintf(str, "%d", this->m_option.bmemwidth);
	this->m_edit_mem_width_b.SetWindowText(str);
	sprintf(str, "%d", this->m_option.blwidth);
	this->m_edit_bl_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.blheight);
	this->m_edit_bl_height.SetWindowText(str);
	sprintf(str, "%d", this->m_option.bmempointx);
	this->m_edit_mem_pointx_b.SetWindowText(str);
	sprintf(str, "%d", this->m_option.bmempointy);
	this->m_edit_mem_pointy_b.SetWindowText(str);
	sprintf(str, "%d", this->m_option.blcdpointx);
	this->m_edit_lcd_pointx_b.SetWindowText(str);
	sprintf(str, "%d", this->m_option.blcdpointy);
	this->m_edit_lcd_pointy_b.SetWindowText(str);
	sprintf(str, "%d", this->m_option.baddr);
	this->m_edit_baddr.SetWindowText(str);

	sprintf(str, "%x", this->m_option.keycolor);
	this->m_edit_keycolor.SetWindowText(str);
	sprintf(str, "%d", this->m_option.blendratio);
	this->m_edit_blendratio.SetWindowText(str);
	if(this->m_option.alphablendingen)
		this->m_alphablendingen.SetCheck(BST_CHECKED);
	else
		this->m_alphablendingen.SetCheck(BST_UNCHECKED);
	if(this->m_option.overlayen)
		this->m_overlayen.SetCheck(BST_CHECKED);
	else
		this->m_overlayen.SetCheck(BST_UNCHECKED);
}

void CBlkLcdcDlg::OnCbnSelchangeComboRotation()
{
	this->m_option.rotation = gRot[this->m_cboRotation.GetCurSel()].rot;
}

void CBlkLcdcDlg::OnCbnSelchangeComboRotationB()
{
	this->m_option.brotation = gRot[this->m_cboRotationB.GetCurSel()].rot;
}

void CBlkLcdcDlg::OnCbnSelchangeComboFormatB()
{
	this->m_option.bformat = gFormat[this->m_cboFormatB.GetCurSel()].format;
}

void CBlkLcdcDlg::OnCbnSelchangeComboOverlayMode()
{
	this->m_option.overlaymode = gMode[this->m_cboOverlayMode.GetCurSel()].mode;
}

void CBlkLcdcDlg::OnCbnSelchangeComboLayerSelect()
{
	this->m_option.layersel = this->m_cboLayerSel.GetCurSel();
}

void CBlkLcdcDlg::OnCbnSelchangeComboLayerPri()
{
	this->m_option.layerpri = this->m_cboLayerPri.GetCurSel();
}


void CBlkLcdcDlg::GetCombo(char *tail)
{
	this->m_option.rotation = gRot[this->m_cboRotation.GetCurSel()].rot;
	this->m_option.brotation = gRot[this->m_cboRotationB.GetCurSel()].rot;
	this->m_option.bformat = gFormat[this->m_cboFormatB.GetCurSel()].format;
	this->m_option.overlaymode = gMode[this->m_cboOverlayMode.GetCurSel()].mode;
	this->m_option.layersel = this->m_cboLayerSel.GetCurSel();
	this->m_option.layerpri = this->m_cboLayerPri.GetCurSel();
	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CBlkLcdcDlg::GetEditValue(void)
{
	char str[100];

	this->m_edit_mem_width.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.memwidth);
	this->m_edit_mem_height.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.memheight);
	this->m_edit_lcd_width.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.lcdwidth);
	this->m_edit_lcd_height.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.lcdheight);
	this->m_edit_al_width.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.alwidth);
	this->m_edit_al_height.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.alheight);
	this->m_edit_mem_pointx.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.mempointx);
	this->m_edit_mem_pointy.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.mempointy);
	this->m_edit_lcd_pointx.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.lcdpointx);
	this->m_edit_lcd_pointy.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.lcdpointy);

	this->m_edit_mem_width_b.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.bmemwidth);
	this->m_edit_bl_width.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.blwidth);
	this->m_edit_bl_height.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.blheight);
	this->m_edit_mem_pointx_b.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.bmempointx);
	this->m_edit_mem_pointy_b.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.bmempointy);
	this->m_edit_lcd_pointx_b.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.blcdpointx);
	this->m_edit_lcd_pointy_b.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.blcdpointy);
	
	this->m_edit_keycolor.GetWindowText(str, 100);
	sscanf(str, "%x", &this->m_option.keycolor);
	this->m_edit_blendratio.GetWindowText(str, 100);
	sscanf(str, "%d", &this->m_option.blendratio);
}

void CBlkLcdcDlg::GetCheckValue(void)
{
	if((this->m_overlayen.GetCheck() == BST_CHECKED) && this->m_overlayen.IsWindowEnabled())
		this->m_option.overlayen = 1;
	else
		this->m_option.overlayen = 0;
	if((this->m_alphablendingen.GetCheck() == BST_CHECKED) && this->m_alphablendingen.IsWindowEnabled())
		this->m_option.alphablendingen = 1;
	else
		this->m_option.alphablendingen = 0;
}

void CBlkLcdcDlg::GetUserOption(char *tail)
{
	this->GetCombo(tail);
	this->GetEditValue();
	this->GetCheckValue();
}

void CBlkLcdcDlg::OnBnClickedBtnProcess()
{
	CFileBin fpSrc, fpDst;
	CBufferBase srcBuf, dstBuf;
	int srcSize, dstSize;
	CLcdcVBufControl vBuf;
	char bmpf[200];
	CRaw2RgbF rgb2bmp;
	CYuv422F yuv2rgb;

	this->GetUserOption();

	fpSrc.Open(this->m_option.srcfile);
	srcSize = fpSrc.GetFileLength();
	if(!srcSize)
		return;
	srcBuf.SetSize(srcSize);
	dstSize = srcSize;
	dstBuf.SetSize(dstSize);
	fpSrc.Read(srcBuf.m_buf, srcSize);

	vBuf.SetRotation(this->m_option.rotation);
	vBuf.SetSize(this->m_option.alwidth, this->m_option.alheight);
	vBuf.SetMemSize(this->m_option.memwidth, this->m_option.memheight);
	vBuf.SetPoint(this->m_option.mempointx, this->m_option.mempointy);
	vBuf.SetLcdcPoint(this->m_option.lcdpointx, this->m_option.lcdpointy);
	vBuf.Process(srcBuf.m_buf, dstBuf.m_buf);
//	frameBuf.SetSize(this->m_option.width, this->m_option.height, 2);
//	frameBuf.SetRotation(this->m_option.rotation);
//	frameBuf.Process(srcBuf.m_buf, dstBuf.m_buf);

	fpDst.Open(this->m_option.dstfile, "wb");
	fpDst.Write(dstBuf.m_buf, dstSize);
	fpDst.Close();
	//output bmp
//	dstWidth = frameBuf.GetRotWidth();
//	dstHeight = frameBuf.GetRotHeight();
	strcpy(bmpf, this->m_option.dstfile);
	strcat(bmpf, ".rgb");
	yuv2rgb.Yuyv2Rgb(this->m_option.dstfile, bmpf, this->m_option.alwidth, this->m_option.alheight);
	strcpy(this->m_option.dstfile, bmpf);
	strcat(bmpf, ".bmp");
	rgb2bmp.Rgb24_Bmp(this->m_option.dstfile, bmpf, this->m_option.alwidth, this->m_option.alheight);
}

void CBlkLcdcDlg::OnBnClickedBtnProcessB()
{
	CFileBin fpSrc, fpDst;
	CBufferBase srcBuf, dstBuf;
	int srcSize, dstSize;
	int	dstWidth, dstHeight;
	CLcdcGBufControl gBuf;
	char bmpf[200];
	CRaw2RgbF rgb2bmp;
	CYuv422F yuv2rgb;

	this->GetUserOption();

	fpSrc.Open(this->m_option.srcfile);
	srcSize = fpSrc.GetFileLength();
	if(!srcSize)
		return;
	srcBuf.SetSize(srcSize);
	dstSize = srcSize;
	dstBuf.SetSize(dstSize);
	fpSrc.Read(srcBuf.m_buf, srcSize);

	gBuf.m_gbuf[0].SetBAddr(this->m_option.baddr);
	gBuf.m_gbuf[0].SetFormat(this->m_option.bformat);
//	gBuf.SetGBuf(srcBuf.m_buf, srcSize);
	gBuf.m_gbuf[0].SetLcdcPoint(this->m_option.blcdpointx, this->m_option.blcdpointy);
	gBuf.m_gbuf[0].SetMemSize(srcSize);
	gBuf.m_gbuf[0].SetMemWidth(this->m_option.bmemwidth);
	gBuf.m_gbuf[0].SetPoint(this->m_option.bmempointx, this->m_option.bmempointy);
	gBuf.m_gbuf[0].SetRotation(this->m_option.brotation);
	gBuf.m_gbuf[0].SetSize(this->m_option.blwidth, this->m_option.blheight);
	dstSize = gBuf.m_gbuf[0].Process(srcBuf.m_buf, dstBuf.m_buf);

	fpDst.Open(this->m_option.dstfile, "wb");
	fpDst.Write(dstBuf.m_buf, dstSize);
	fpDst.Close();
	//output bmp
//	dstWidth = frameBuf.GetRotWidth();
//	dstHeight = frameBuf.GetRotHeight();
	dstWidth = gBuf.m_gbuf[0].GetWidth();
	dstHeight = gBuf.m_gbuf[0].GetHeight();
	strcpy(bmpf, this->m_option.dstfile);
	strcat(bmpf, ".bmp");
	rgb2bmp.Rgb24_Bmp(this->m_option.dstfile, bmpf, dstWidth, dstHeight);
}

void CBlkLcdcDlg::OnBnClickedBtnInitGbuf()
{
	CFileBin fp;
	CBufferBase buf;
	int size;

	this->GetUserOption();
	fp.Open(this->m_option.srcfile);
	size = fp.GetFileLength();
	if(!size)
		return;
	buf.SetSize(size);
	fp.Read(buf.m_buf, size);
	this->m_lcdc.SetBAddr(this->m_option.baddr, this->m_option.layersel);
	this->m_lcdc.SetBFormat(this->m_option.bformat, this->m_option.layersel);
	this->m_lcdc.SetBLcdcPoint(this->m_option.blcdpointx, this->m_option.blcdpointy, this->m_option.layersel);
	this->m_lcdc.SetBMemSize(size, this->m_option.layersel);
	this->m_lcdc.SetBMemWidth(this->m_option.bmemwidth, this->m_option.layersel);
	this->m_lcdc.SetBPoint(this->m_option.bmempointx, this->m_option.bmempointy, this->m_option.layersel);
	this->m_lcdc.SetBRotation(this->m_option.brotation, this->m_option.layersel);
	this->m_lcdc.SetBSize(this->m_option.blwidth, this->m_option.blheight, this->m_option.layersel);
	this->m_lcdc.SetAlphaBlendingEn(this->m_option.alphablendingen, this->m_option.layersel);
	this->m_lcdc.SetOverlayEn(this->m_option.overlayen, this->m_option.layersel);
	this->m_lcdc.SetOverlayMode(this->m_option.overlaymode, this->m_option.layersel);
	this->m_lcdc.SetKeyColor(this->m_option.keycolor, this->m_option.layersel);
	this->m_lcdc.SetBlendRatio(this->m_option.blendratio, this->m_option.layersel);
	this->m_lcdc.SetPriority(this->m_option.layerpri, this->m_option.layersel); 
	this->m_lcdc.WriteGBuf(buf.m_buf, size, this->m_option.layersel);
}

void CBlkLcdcDlg::OnBnClickedBtnLcdcProcess()
{
	CFileBin fpSrc, fpDst;
	CBufferBase srcBuf, dstBuf;
	int srcSize, dstSize;
	//char bmpf[200];
	CRaw2RgbF rgb2bmp;

	this->GetUserOption();

	fpSrc.Open(this->m_option.srcfile);
	srcSize = fpSrc.GetFileLength();
	if(!srcSize)
		return;
	srcBuf.SetSize(srcSize);
	fpSrc.Read(srcBuf.m_buf, srcSize);

	this->m_lcdc.SetARotation(this->m_option.rotation);
	this->m_lcdc.SetASize(this->m_option.alwidth, this->m_option.alheight);
	this->m_lcdc.SetAMemSize(this->m_option.memwidth, this->m_option.memheight);
	this->m_lcdc.SetAPoint(this->m_option.mempointx, this->m_option.mempointy);
	this->m_lcdc.SetALcdcPoint(this->m_option.lcdpointx, this->m_option.lcdpointy);
	dstSize = this->m_lcdc.GetALayerSize() * 3 / 2;
	dstBuf.SetSize(dstSize);
	dstSize = this->m_lcdc.Process(srcBuf.m_buf, dstBuf.m_buf);	
	this->WriteLcdBuf(dstBuf.m_buf);

	//fpDst.Open(this->m_option.dstfile, "wb");
	//fpDst.Write(dstBuf.m_buf, dstSize);
	//fpDst.Close();
	//output bmp
	//strcpy(bmpf, this->m_option.dstfile);
	//strcat(bmpf, ".bmp");
	//rgb2bmp.Rgb24_Bmp(this->m_option.dstfile, bmpf, this->m_option.alwidth, this->m_option.alheight);
}

void CBlkLcdcDlg::WriteLcdBuf(char *buf)
{
	int start, span, skip;

	start = this->m_option.lcdpointy*this->m_option.lcdwidth*3 + this->m_option.lcdpointx*3;
	span = this->m_option.alwidth;
	skip = this->m_option.lcdwidth - this->m_option.alwidth;
	this->m_lcdbuf.SetOption(start, span, skip);
	this->m_lcdbuf.Write(buf, this->m_option.alwidth*this->m_option.alheight*3);
}

void CBlkLcdcDlg::OnBnClickedBtnUpdateB()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CBlkLcdcDlg::OnBnClickedBtnOutputBmp()
{
	CFileBin	fp;
	CRaw2RgbF	rgb2bmp;

	fp.Open("temp.dat", "wb", 0xe100);
	fp.Write(this->m_lcdbuf.m_buf, 0xe100);
	fp.Close();
	rgb2bmp.Rgb24_Bmp("temp.dat", "LcdScreen.bmp", 120, 160);
}
