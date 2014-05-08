// V568UsbcDlg.cpp : 实现文件
//
#include "internal.h"


#ifndef	V578CMD_DISABLE





// CV568Demo1Dlg 对话框

IMPLEMENT_DYNAMIC(CV568Demo1Dlg, CBaseCtrlDlg)
CV568Demo1Dlg::CV568Demo1Dlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568Demo1Dlg::IDD, pParent)
{
}

CV568Demo1Dlg::~CV568Demo1Dlg()
{
}

void CV568Demo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_JPGIMGWIDTH_EDIT, m_jpgimgwidthbox);
	DDX_Control(pDX, IDC_JPGIMGHEIGHT_EDIT, m_jpgimgheightbox);
	DDX_Control(pDX, IDC_JPGWIDTH_EDIT, m_jpgjpgwidth);
	DDX_Control(pDX, IDC_JPGHEIGHT_EDIT, m_jpgjpgheight);
	DDX_Control(pDX, IDC_JPGSOP_EDIT, m_jpgsopbox);
	DDX_Control(pDX, IDC_JPGDATAPOS_EDIT, m_jpgoffsetbox);
	DDX_Control(pDX, IDC_JPEGTYPE_COMBO, m_jpgtypebox);
	DDX_Control(pDX, IDC_DSTWIDTH_EDIT, m_dstwidthBox);
	DDX_Control(pDX, IDC_DSTHEIGHT_EDIT, m_dstheightBox);
	DDX_Control(pDX, IDC_IMGWIDTH_EDIT, m_imgwidthBox);
	DDX_Control(pDX, IDC_IMGHEIGHT_EDIT, m_imgheightBox);
	DDX_Control(pDX, IDC_SLEEPCNT_EDIT, m_sleepBox);
	DDX_Control(pDX, IDC_STEPCNT_EDIT, m_stepBox);
	DDX_Control(pDX, IDC_COMBO_FILT_FUNC, m_filfuncbox);
	DDX_Control(pDX, IDC_STATIC_SRC, m_sticsrc);
	DDX_Control(pDX, IDC_STATIC_DST, m_sticdst);


}

BEGIN_MESSAGE_MAP(CV568Demo1Dlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_DEMO1DISPLAYBUTTON, OnBnClickedDemo1displaybutton)
	ON_CBN_SELCHANGE(IDC_COMBO_FILT_FUNC, OnCbnSelchangeComboFiltFunc)
	ON_BN_CLICKED(IDC_PARSERJPG_BUTTON, OnBnClickedParserjpgButton)
END_MESSAGE_MAP()

void CV568Demo1Dlg::InitOption()
{
	int i;
	const char *func[] = { "Disp (Img size changed)", "Disp (Disp size changed)", "Cap (Img size changed)", "Cap (Cap size changed)"};

	memset( &this->m_option, 0, sizeof(TDemo1Option) );
	this->m_option.dstsize.cx = 128;
	this->m_option.dstsize.cy = 160;
	this->m_option.srcsize.cx = 128;
	this->m_option.srcsize.cy = 160;
	this->m_option.sleepcnt = 1000;
	this->m_option.step = 5;
	this->m_option.filterfunc = 0;
	for(i = 0; i < 4; i++)
	{
		this->m_filfuncbox.InsertString(i, func[i]);
	}
	this->m_filfuncbox.SetCurSel(0);
	for(i = 0; i < gTotalJpegFmt; i++)
		this->m_jpgtypebox.InsertString(i, gJpegFmt[i]);

}

void CV568Demo1Dlg::DispOption()
{
	char str[100];

	sprintf(str, "%d", this->m_option.info.ImageSize.cx);
	this->m_jpgimgwidthbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.info.ImageSize.cy);
	this->m_jpgimgheightbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.info.JpgSize.cx);
	this->m_jpgjpgwidth.SetWindowText(str);
	sprintf(str, "%d", this->m_option.info.JpgSize.cy);
	this->m_jpgjpgheight.SetWindowText(str);
	sprintf(str, "%d", this->m_option.info.sop);
	this->m_jpgsopbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.info.offset);
	this->m_jpgoffsetbox.SetWindowText(str);

	this->m_jpgtypebox.SetCurSel(this->m_option.info.YUVType);

	sprintf(str, "%x", this->m_option.dstsize.cy);
	this->m_dstheightBox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dstsize.cx);
	this->m_dstwidthBox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.srcsize.cy);
	this->m_imgheightBox.SetWindowText(str);
	sprintf(str, "%x", this->m_option.srcsize.cx);
	this->m_imgwidthBox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.sleepcnt);
	this->m_sleepBox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.step);
	this->m_stepBox.SetWindowText(str);
	this->m_option.filterfunc = 0;
	this->m_filfuncbox.SetCurSel(this->m_option.filterfunc);

}

void CV568Demo1Dlg::GetUserOption()
{
	char str[100];
	CStringTools tools;

	this->m_dstwidthBox.GetWindowText(str, 100);
	sscanf(str, "%hx", &(this->m_option.dstsize.cx));
	this->m_dstheightBox.GetWindowText(str, 100);
	sscanf(str, "%hx", &(this->m_option.dstsize.cy));
	this->m_imgwidthBox.GetWindowText(str, 100);
	sscanf(str, "%hx", &(this->m_option.srcsize.cx));
	this->m_imgheightBox.GetWindowText(str, 100);
	sscanf(str, "%hx", &(this->m_option.srcsize.cy));
	this->m_sleepBox.GetWindowText(str, 100);
	tools.Str2Dec(str, &(this->m_option.sleepcnt));
	this->m_stepBox.GetWindowText(str, 100);
	tools.Str2Dec(str, &(this->m_option.step));
	this->m_option.filterfunc = this->m_filfuncbox.GetCurSel();

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile);
}

BOOL CV568Demo1Dlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitOption();
	this->DispOption();
	OnCbnSelchangeComboFiltFunc();

	return TRUE;  // return TRUE unless you set the focus to a control
}

static void DispSet(UINT8 yuvfmt, TSize srcsize, TPoint ipppt, TSize ippsize, TSize dispsize)
{
	TALayerProty proty;

	proty.memsize = dispsize;
	proty.pos2panel = ipppt;
	proty.win.ltp = ipppt;
	proty.win.size = dispsize;

	V5_IppSetDispSizerEx2(srcsize, ipppt, ippsize, dispsize);
	V5B_LbufSetYuvMode(yuvfmt);
	V5_DispSetAmemSize(&proty);
	V5_JpegAutoSetImage();
	V5_MarbAutoSetMem();
}

void CV568Demo1Dlg::DoDisplay(int bSrc)
{
	CFileBin fb;
	CBufferBase pool;
	TJpegIndex info;
	TSize ippsize;
	TPoint	ipppt;
	TMarbMap map;
	UINT8* pbuf;
	int size;
	int btm, i;

	this->GetUserOption();
	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	if(size == 0)
		return;
	pool.SetSize(size << 1);
	fb.Read(pool.m_buf, size);
	pbuf = (UINT8 *)pool.m_buf;
	JpegParse(pbuf, size, &info);
	if(info.YUVType == JPEG_UNKNOWN)
		return;
	if( (info.JpgSize.cx < this->m_option.dstsize.cx) ||
		(info.JpgSize.cy < this->m_option.dstsize.cy) )
		return;
	ipppt.x = ipppt.y = 0;
	btm = (this->m_option.dstsize.cy * 100) / info.ImageSize.cy;
	i = (this->m_option.dstsize.cx * 100) / info.ImageSize.cx;
	if(btm < i)
		btm = i;
	V5_SetMode(DISPLAYSTILL_MODE);
	DispSet((UINT8)(info.YUVType), info.JpgSize, ipppt, info.ImageSize, this->m_option.dstsize);
	V5_MarbGetMap(&map);
	size = (size + map.jbufstart) > map.jbufend ? map.jbufend-map.jbufstart : size;
	V5_WriteSram(map.jbufstart, pbuf, size);
	for(i = 100; i >= btm; i -= this->m_option.step)
	{
		ippsize.cx = (UINT16)(info.ImageSize.cx * i / 100);
		ippsize.cy = (UINT16)(info.ImageSize.cy * i / 100);
		DispSet((UINT8)(info.YUVType), info.JpgSize, ipppt, ippsize, this->m_option.dstsize);
		V5_HJpegDispFrmDirAuto((UINT8)(info.YUVType), info.JpgSize, size);
		Sleep(this->m_option.sleepcnt);
	}

	bSrc = bSrc;
}

void CV568Demo1Dlg::DoCapture(int bSrc)
{
	bSrc = bSrc;
}

void CV568Demo1Dlg::OnBnClickedDemo1displaybutton()
{
	CWaitCursor cursor;

	this->GetUserOption();

	switch(this->m_option.filterfunc)
	{
	case FUNC_DISP_SRC:
		DoDisplay(1);
		break;
	case FUNC_DISP_DISP:
		DoDisplay(0);
		break;
	case FUNC_CAP_SRC:
		DoCapture(1);
		break;
	case FUNC_CAP_CAP:
		DoCapture(0);
		break;
	default:
		break;
	}

}

void CV568Demo1Dlg::OnCbnSelchangeComboFiltFunc()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetUserOption();

	switch(this->m_option.filterfunc)
	{
	case FUNC_DISP_SRC:
		m_imgwidthBox.EnableWindow();
		m_imgheightBox.EnableWindow();
		m_dstwidthBox.EnableWindow();
		m_dstheightBox.EnableWindow();
		m_sticsrc.SetWindowText("img(alt)");
		m_sticdst.SetWindowText("disp(fixed)");
		break;
	case FUNC_DISP_DISP:
		m_imgwidthBox.EnableWindow();
		m_imgheightBox.EnableWindow();
		m_dstwidthBox.EnableWindow();
		m_dstheightBox.EnableWindow();
		m_sticsrc.SetWindowText("img(fixed)");
		m_sticdst.SetWindowText("disp(alt)");
		break;
	case FUNC_CAP_SRC:
		m_imgwidthBox.EnableWindow();
		m_imgheightBox.EnableWindow();
		m_dstwidthBox.EnableWindow();
		m_dstheightBox.EnableWindow();
		m_sticsrc.SetWindowText("img(alt)");
		m_sticdst.SetWindowText("cap(fixed)");
		break;
	case FUNC_CAP_CAP:
		m_imgwidthBox.EnableWindow();
		m_imgheightBox.EnableWindow();
		m_dstwidthBox.EnableWindow();
		m_dstheightBox.EnableWindow();
		m_sticsrc.SetWindowText("img(fixed)");
		m_sticdst.SetWindowText("cap(alt)");
		break;
	default:
		break;
	}
}

void CV568Demo1Dlg::OnBnClickedParserjpgButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	int size;

	this->GetUserOption();
	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	pool.SetSize(size);
	fb.Read(pool.m_buf, size);

	JpegParse((UINT8 *)pool.m_buf, size, &(this->m_option.info));

	this->DispOption();

}

#else
void nullDemo1Dlg(void)
{
}
#endif //V578CMD_DISABLE




