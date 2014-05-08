// V568JpegDlg.cpp : 实现文件
//
#include "internal.h"
#include "..\include\v568plane\v568jpegdlg.h"

#ifndef	V578CMD_DISABLE





// CV568JpegDlg 对话框

IMPLEMENT_DYNAMIC(CV568JpegDlg, CBaseCtrlDlg)
CV568JpegDlg::CV568JpegDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568JpegDlg::IDD, pParent)
{
}

CV568JpegDlg::~CV568JpegDlg()
{
}

void CV568JpegDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRAMECOUNT_EDIT, m_framecountbox);
	DDX_Control(pDX, IDC_JPGIMGWIDTH_EDIT, m_jpgimgwidthbox);
	DDX_Control(pDX, IDC_JPGIMGHEIGHT_EDIT, m_jpgimgheightbox);
	DDX_Control(pDX, IDC_JPGWIDTH_EDIT, m_jpgjpgwidth);
	DDX_Control(pDX, IDC_JPGHEIGHT_EDIT, m_jpgjpgheight);
	DDX_Control(pDX, IDC_JPGSOP_EDIT, m_jpgsopbox);
	DDX_Control(pDX, IDC_JPGDATAPOS_EDIT, m_jpgoffsetbox);
	DDX_Control(pDX, IDC_JPEGTYPE_COMBO, m_jpgtypebox);
	DDX_Control(pDX, IDC_STARTWIDTH_EDIT, m_startwidthbox);
	DDX_Control(pDX, IDC_STARTHEIGHT_EDIT, m_startheightbox);
	DDX_Control(pDX, IDC_ENDWIDTH_EDIT, m_endwidthbox);
	DDX_Control(pDX, IDC_ENDHEIGHT_EDIT, m_endheightbox);
	DDX_Control(pDX, IDC_STEP_EDIT, m_stepbox);
	DDX_Control(pDX, IDC_DELAY_EDIT, m_delaybox);
	DDX_Control(pDX, IDC_TOTAL_TIME, m_totaltimebox);
	DDX_Control(pDX, IDC_QF_SMP_TIME, m_qftimebox);
	DDX_Control(pDX, IDC_QF, m_qfbox);
	DDX_Control(pDX, IDC_QT_TEST, m_testqfbt);
	DDX_Control(pDX, IDC_QT_SET, m_testqfsetbt);
	DDX_Control(pDX, IDC_QT_AUTO, m_QtAutoBtn);
	DDX_Control(pDX, IDC_EDIT_TCR, m_ctlTcr);
	DDX_Control(pDX, IDC_RUDE_THD_R1, m_edThdR1);
	DDX_Control(pDX, IDC_RUDE_THD_R2, m_edThdR2);
	DDX_Control(pDX, IDC_RUDE_THD_L1, m_edThdL1);
	DDX_Control(pDX, IDC_RUDE_THD_L2, m_edThdL2);
	DDX_Control(pDX, IDC_RUDE_SHF_R1, m_edShfR1);
	DDX_Control(pDX, IDC_RUDE_SHF_R2, m_edShfR2);
	DDX_Control(pDX, IDC_RUDE_SHF_L1, m_edShfL1);
	DDX_Control(pDX, IDC_RUDE_SHF_L2, m_edShfL2);
	DDX_Control(pDX, IDC_RUDE_COMP0, m_edCompLevel0);
	DDX_Control(pDX, IDC_RUDE_COMP1, m_edCompLevel1);
	DDX_Control(pDX, IDC_RUDE_COMP2, m_edCompLevel2);
	DDX_Control(pDX, IDC_RUDE_COMP3, m_edCompLevel3);
	DDX_Control(pDX, IDC_RUDE_COMP4, m_edCompLevel4);
	DDX_Control(pDX, IDC_RUDE_COMP5, m_edCompLevel5);
	DDX_Control(pDX, IDC_RUDE_AC, m_edRudeAc);
	DDX_Control(pDX, IDC_RUDE_LEVEL, m_edRudeLevel);
	DDX_Control(pDX, IDC_RUDE_THD_WC, m_edThdWc);
	DDX_Control(pDX, IDC_RUDE_EN, m_rudebrcen);
	DDX_Control(pDX, IDC_RUDE_AC_EN, m_rudeacen);
	DDX_Control(pDX, IDC_ENH_EN, m_enhbrcen);
	DDX_Control(pDX, IDC_CAP_DLY_EN, m_capdlyen);
	DDX_Control(pDX, IDC_CAP_DLY_FEED, m_capdlyfeed);

}


BEGIN_MESSAGE_MAP(CV568JpegDlg, CBaseCtrlDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PARSERJPG_BUTTON, OnBnClickedParserjpgButton)
	ON_BN_CLICKED(IDC_SPLITJPG_BUTTON, OnBnClickedSplitjpgButton)
	ON_BN_CLICKED(IDC_CAPSTILL_BUTTON, OnBnClickedCapstillButton)
	ON_BN_CLICKED(IDC_DISPSTILL_BUTTON, OnBnClickedDispstillButton)
	ON_BN_CLICKED(IDC_BUTTON_DEMO_IN, OnBnClickedButtonDemoIn)
	ON_BN_CLICKED(IDC_BUTTON_DEMO_CAP_STILL, OnBnClickedButtonDemoCapStill)
	ON_BN_CLICKED(IDC_BUTTON_DEMO_DISP_STILL, OnBnClickedButtonDemoDispStill)
	ON_BN_CLICKED(IDC_BUTTON_DEMO_CAP_VIDEO, OnBnClickedButtonDemoCapVideo)
	ON_BN_CLICKED(IDC_BUTTON_DEMO_DISP_VIDEO, OnBnClickedButtonDemoDispVideo)
	ON_BN_CLICKED(IDC_BUTTON_DEMO_CAP_FRAME, OnBnClickedButtonDemoCapFrame)
	ON_BN_CLICKED(IDC_BUTTON_DEMO_DISP_FRAME, OnBnClickedButtonDemoDispFrame)
	ON_BN_CLICKED(_BUTTON_DEMO_ENC, OnBnClickedButtonDemoEnc)
	ON_BN_CLICKED(_BUTTON_DEMO_DEC, OnBnClickedButtonDemoDec)
	ON_BN_CLICKED(IDC_BUTTON_DEMO_CAP_MUL, OnBnClickedButtonDemoCapMul)
	ON_BN_CLICKED(IDC_BUTTON_DEMO_RETVIEW, OnBnClickedButtonDemoRetview)
	ON_BN_CLICKED(IDC_BUTTON_DEMO_MUL_RETVIEW, OnBnClickedButtonDemoMulRetview)
	ON_BN_CLICKED(IDC_SPLITJPG_ALL_BUTTON, OnBnClickedSplitjpgAllButton)
	ON_BN_CLICKED(IDC_BTN_JFIF_SPLIT, OnBnClickedBtnJfifSplit)
	ON_BN_CLICKED(IDC_CAPSTILL_ERROR_BUTTON, OnBnClickedCapstillErrorButton)
	ON_BN_CLICKED(IDC_BTN_EXIF_SPLIT, OnBnClickedBtnExifSplit)
	ON_BN_CLICKED(IDC_QT_TEST, OnBnClickedQtTest)
	ON_BN_CLICKED(IDC_QT_SET, OnBnClickedQtSet)
	ON_BN_CLICKED(IDC_QT_AUTO, OnBnClickedQtAuto)
	ON_BN_CLICKED(IDC_FRAME_RATE_TEST, OnBnClickedFrameRateTest)
	ON_BN_CLICKED(IDC_BUTTON_TCR, OnBnClickedButtonTcr)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_RUDE_SET, OnBnClickedButtonRudeSet)
	ON_BN_CLICKED(IDC_BUTTON_ENH_SET, OnBnClickedButtonEnhSet)
	ON_BN_CLICKED(IDC_DEC_BUTTON, OnBnClickedDecButton)
	ON_BN_CLICKED(IDC_DECIPP_BUTTON, OnBnClickedDecippButton)
	ON_BN_CLICKED(IDC_DECLCD_BUTTON, OnBnClickedDeclcdButton)
	ON_BN_CLICKED(IDC_DEC_ALL_BUTTON, OnBnClickedDecAllButton)
	ON_BN_CLICKED(IDC_BUTTON_CAP_DLY, OnBnClickedButtonCapDly)
	ON_BN_CLICKED(IDC_CAP_DLY_EN, OnBnClickedCapDlyEn)
END_MESSAGE_MAP()


// CV568JpegDlg 消息处理程序

BOOL CV568JpegDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispOption();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CV568JpegDlg::InitUserOption(void)
{
	int i;

	memset(&(this->m_option), 0, sizeof(this->m_option));
	for(i = 0; i < gTotalJpegFmt; i++)
		this->m_jpgtypebox.InsertString(i, gJpegFmt[i]);
	this->m_option.qftime = 10;
	this->m_option.qf = 41;
}

void CV568JpegDlg::DispOption(void)
{
	char str[20];

	sprintf(str, "%d", this->m_option.framecount);
	this->m_framecountbox.SetWindowText(str);

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

	sprintf(str, "%d", this->m_option.startsize.cx);
	this->m_startwidthbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.startsize.cy);
	this->m_startheightbox.SetWindowText(str);

	sprintf(str, "%d", this->m_option.endsize.cx);
	this->m_endwidthbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.endsize.cy);
	this->m_endheightbox.SetWindowText(str);

	sprintf(str, "%d", this->m_option.step);
	this->m_stepbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.delay);
	this->m_delaybox.SetWindowText(str);

	sprintf(str, "%d", this->m_option.totaltime);
	this->m_totaltimebox.SetWindowText(str);

	sprintf(str, "%d", this->m_option.qftime);
	this->m_qftimebox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.qf);
	this->m_qfbox.SetWindowText(str);

	sprintf(str, "%x", this->m_option.tcr);
	this->m_ctlTcr.SetWindowText(str);

	sprintf(str, "%x", this->m_option.enhbrc.ThdR1);
	this->m_edThdR1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.enhbrc.ThdR2);
	this->m_edThdR2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.enhbrc.ThdL1);
	this->m_edThdL1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.enhbrc.ThdL2);
	this->m_edThdL2.SetWindowText(str);

	sprintf(str, "%x", this->m_option.enhbrc.ShfR1);
	this->m_edShfR1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.enhbrc.ShfR2);
	this->m_edShfR2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.enhbrc.ShfL1);
	this->m_edShfL1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.enhbrc.ShfL2);
	this->m_edShfL2.SetWindowText(str);

	sprintf(str, "%x", this->m_option.rudebrc.CompLevel[0]);
	this->m_edCompLevel0.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rudebrc.CompLevel[1]);
	this->m_edCompLevel1.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rudebrc.CompLevel[2]);
	this->m_edCompLevel2.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rudebrc.CompLevel[3]);
	this->m_edCompLevel3.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rudebrc.CompLevel[4]);
	this->m_edCompLevel4.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rudebrc.CompLevel[5]);
	this->m_edCompLevel5.SetWindowText(str);

	sprintf(str, "%x", this->m_option.rudebrc.RudeAc);
	this->m_edRudeAc.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rudebrc.RudeLevel);
	this->m_edRudeLevel.SetWindowText(str);
	sprintf(str, "%x", this->m_option.rudebrc.ThdWc);
	this->m_edThdWc.SetWindowText(str);
	sprintf(str, "%d", this->m_option.capdlyfeed);
	this->m_capdlyfeed.SetWindowText(str);

	if(this->m_option.rudeyuonlyen)
		m_rudeacen.SetCheck(1);
	else
		m_rudeacen.SetCheck(0);

	if(this->m_option.rudebrcen)
		m_rudebrcen.SetCheck(1);
	else
		m_rudebrcen.SetCheck(0);

	if(this->m_option.enhbrcen)
		m_enhbrcen.SetCheck(1);
	else
		m_enhbrcen.SetCheck(0);

	if(this->m_option.capdlyen)
		m_capdlyen.SetCheck(1);
	else
		m_capdlyen.SetCheck(0);


}

void CV568JpegDlg::GetUserOption(char *tail)
{
	int x;
	char str[20];
//	CStringTools tools;

	this->m_framecountbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.framecount));
	this->m_stepbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.step));
	this->m_option.step = (this->m_option.step >> 1) << 1;
	this->m_option.step = (this->m_option.step > 0) ? this->m_option.step : 2;
	this->m_delaybox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.delay));

	this->m_startwidthbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	x = (x > 2048) ? 2048 : x;
	x = (x >> 1) << 1;
	this->m_option.startsize.cx = (UINT16)x;
	this->m_startheightbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	x = (x > 1536) ? 1536 : x;
	x = (x >> 1) << 1;
	this->m_option.startsize.cy = (UINT16)x;
	this->m_framecountbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.framecount));

	this->m_endwidthbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	x = (x > 2048) ? 2048 : x;
	x = (x >> 1) << 1;
	this->m_option.endsize.cx = (UINT16)x;
	this->m_endheightbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	x = (x > 1536) ? 1536 : x;
	x = (x >> 1) << 1;
	this->m_option.endsize.cy = (UINT16)x;

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);

	this->m_totaltimebox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.totaltime));

	this->m_qftimebox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.qftime));

	this->m_qfbox.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.qf));

	this->m_ctlTcr.GetWindowText(str, 20);
	sscanf(str, "%x", &(this->m_option.tcr));

	this->m_edThdR1.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.enhbrc.ThdR1 = (UINT8)x;
	this->m_edThdR2.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.enhbrc.ThdR2 = (UINT8)x;
	this->m_edThdL1.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.enhbrc.ThdL1 = (UINT8)x;
	this->m_edThdL2.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.enhbrc.ThdL2 = (UINT8)x;
	this->m_edShfR1.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.enhbrc.ShfR1 = (UINT8)x;
	this->m_edShfR2.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.enhbrc.ShfR2 = (UINT8)x;
	this->m_edShfL1.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.enhbrc.ShfL1 = (UINT8)x;
	this->m_edShfL2.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.enhbrc.ShfL2 = (UINT8)x;
	this->m_edCompLevel0.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.rudebrc.CompLevel[0] = (UINT8)x;
	this->m_edCompLevel1.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.rudebrc.CompLevel[1] = (UINT8)x;
	this->m_edCompLevel2.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.rudebrc.CompLevel[2] = (UINT8)x;
	this->m_edCompLevel3.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.rudebrc.CompLevel[3] = (UINT8)x;
	this->m_edCompLevel4.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.rudebrc.CompLevel[4] = (UINT8)x;
	this->m_edCompLevel5.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.rudebrc.CompLevel[5] = (UINT8)x;
	this->m_edRudeAc.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.rudebrc.RudeAc = (UINT8)x;
	this->m_edRudeLevel.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.rudebrc.RudeLevel = (UINT8)x;
	this->m_edThdWc.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.rudebrc.ThdWc = (UINT8)x;
	this->m_capdlyfeed.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.capdlyfeed = (UINT8)x;
	if(this->m_rudebrcen.GetCheck())
		this->m_option.rudebrcen = 1;
	else
		this->m_option.rudebrcen = 0;
	if(this->m_rudeacen.GetCheck())
		this->m_option.rudeyuonlyen = 1;
	else
		this->m_option.rudeyuonlyen = 0;
	if(this->m_enhbrcen.GetCheck())
		this->m_option.enhbrcen = 1;
	else
		this->m_option.enhbrcen = 0;

	if(this->m_capdlyen.GetCheck())
		this->m_option.capdlyen = 1;
	else
		this->m_option.capdlyen = 0;


}

void CV568JpegDlg::OnBnClickedParserjpgButton()
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	int size;

	this->GetUserOption("info");
	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	pool.SetSize(size);
	fb.Read(pool.m_buf, size);

	JpegParse((UINT8 *)pool.m_buf, size, &(this->m_option.info));

	this->DispOption();
}

void CV568JpegDlg::OnBnClickedSplitjpgButton()
{
	CWaitCursor cursor;
	CJpegF tools;

	this->GetUserOption(FILE_TAIL_JPG);
	tools.SplitJpeg(this->m_option.srcfile, JPEGF_LIST, this->m_option.dstfile);
}

void CV568JpegDlg::OnBnClickedCapstillButton()
{
	// TODO: 在此添加控件通知处理程序代码

	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	CStringTools tools;
	TDataBuf dbuf;
	int len, flag = 1, count, totaltime;
	TSize size, size1;
	TJpegCapParm capparm;
	UINT8 bSifCap;

	this->GetUserOption(FILE_TAIL_AVI);
	pool.SetSize(0x200000);
	if(pool.GetSize() == 0)
		return;

	bSifCap = (UINT8)(V5B_SifGetSensorOpMode() & BIT7);

	totaltime = this->m_option.totaltime;
	while(totaltime--)
	{
		flag = 1;

		size = this->m_option.startsize;
		V5B_IppGetWinSize(&size1);
		size1.cx <<= 1;
		size1.cy <<= 1;
		this->m_option.endsize.cx = (this->m_option.endsize.cx > size1.cx) ? size1.cx : this->m_option.endsize.cx;
		this->m_option.endsize.cy = (this->m_option.endsize.cy > size1.cy) ? size1.cy : this->m_option.endsize.cy;
		this->m_option.endsize.cx = (UINT16)(this->m_option.endsize.cx > 2048 ? 2048 : this->m_option.endsize.cx);
		this->m_option.endsize.cy = (UINT16)(this->m_option.endsize.cy > 1536 ? 1536 : this->m_option.endsize.cy);
		size.cx = size.cx > this->m_option.endsize.cx ? this->m_option.endsize.cx : size.cx;
		size.cy = size.cy > this->m_option.endsize.cy ? this->m_option.endsize.cy : size.cy;
		len = pool.GetTotalSize();

		while(flag)
		{
			V5_IppSetCapSizer(size);
			V5_JpegAutoSetImage();
			V5_MarbAutoSetMem();

			count = this->m_option.framecount;
			while(count--)
			{
				dbuf.pData = (UINT8 *)pool.m_buf;
				dbuf.Len = (UINT32)len;
				dbuf.Pos = 0;

				if(bSifCap)
					TestSifCap(&dbuf, &capparm);
				else
					TestCap(&dbuf, &capparm);


				if(dbuf.Pos <= 0)
					break;
				fb.Open(this->m_option.dstfile, "wb");
				fb.Write(pool.m_buf, dbuf.Pos);

				tools.AutoIndex2Str(this->m_option.dstfile, FILE_TAIL_JPG, this->m_option.dstfile);
			}
			if((size.cx >= this->m_option.endsize.cx) && (size.cy >= this->m_option.endsize.cy) )
			{
				flag = 0;
				break;
			}
			size.cx = (UINT16)(size.cx + this->m_option.step);
			if(size.cx > this->m_option.endsize.cx)
			{
				size.cx = this->m_option.startsize.cx;
				size.cy = (UINT16)(size.cy + this->m_option.step);
				if(size.cy > this->m_option.endsize.cy)
					size.cy = this->m_option.endsize.cy;
			}
			Sleep(this->m_option.delay);
		}
	}

}

void CV568JpegDlg::OnBnClickedDispstillButton()
{
	CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	int len, flag = 1;
	TDataBuf	dbuf;
	TJpegIndex info;
	TSize size = {0x80, 0x60};
	TPoint pt = {0, 0};
	TALayerProty proty;
	CFileFind cf;
	BOOL bfind;
	CString fn;

	proty.memsize = size;
	proty.pos2panel = pt;
	proty.win.ltp = pt;
	proty.win.size = size;
	this->GetUserOption(FILE_TAIL_DAT);

	bfind = cf.FindFile("*.jpg");
	while(flag)
	{
		bfind = cf.FindNextFile();
		if(bfind == FALSE)
			return;
		fn = cf.GetFileName();
		fb.Open(fn.GetBuffer());
		len = fb.GetFileLength();
		if(len == 0)
			return;
		pool.SetSize(len << 1);
		fb.Read(pool.m_buf, len);

	//=========== test code =========
		JpegParse((UINT8 *)pool.m_buf, len, &info);
		if(info.YUVType == JPEG_UNKNOWN)
			continue;
		dbuf.pData = (UINT8 *)pool.m_buf;
		dbuf.Len = (UINT32)len;
		dbuf.Pos = 0;
		V5_IppSetDispSizerEx2(info.JpgSize, pt, info.ImageSize, size);
		V5_DispSetAmemSize(&proty);
		V5_HJpegDispFrm(&dbuf);
	//=================================

		Sleep(this->m_option.delay);
	}
}

void CV568JpegDlg::OnBnClickedButtonDemoIn()
{
	// TODO: 在此添加控件通知处理程序代码
	SmpCameraOpen();
	SmJpegOpen();
}


void CV568JpegDlg::OnBnClickedButtonDemoCapStill()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegCapStill();
}

void CV568JpegDlg::OnBnClickedButtonDemoDispStill()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegDispStill();
}

void CV568JpegDlg::OnBnClickedButtonDemoCapVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegCapVideo();
}

void CV568JpegDlg::OnBnClickedButtonDemoDispVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegDispVideo();
}

void CV568JpegDlg::OnBnClickedButtonDemoCapFrame()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegCapFrame();
}

void CV568JpegDlg::OnBnClickedButtonDemoDispFrame()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegMulSelDisp();
}

void CV568JpegDlg::OnBnClickedButtonDemoEnc()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegEncode();
}

void CV568JpegDlg::OnBnClickedButtonDemoDec()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegDecode();
}

void CV568JpegDlg::OnBnClickedButtonDemoCapMul()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegMultishot();
}

void CV568JpegDlg::OnBnClickedButtonDemoRetview()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegRetView();
}

void CV568JpegDlg::OnBnClickedButtonDemoMulRetview()
{
	// TODO: 在此添加控件通知处理程序代码
	smJpegClearMenuRetView();
}

void CV568JpegDlg::OnBnClickedSplitjpgAllButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CWaitCursor cursor;
	CJpegF tools;
	CFileFind find;
	BOOL bfind;
	char srcpath[0x100];
	char dstpath[0x100];
	CString strTemp,strTemp1;
	int site;

	this->GetUserOption(FILE_TAIL_JPG);

	strTemp = this->m_option.srcfile;
	site = strTemp.ReverseFind('\\');
	if(site == -1)
		return;
	strTemp1 = strTemp.Left(site);
	strcpy(dstpath, strTemp1);

	site = strTemp.ReverseFind('.');
	if(site == -1)
		return;

	strTemp1 += "\\*";
	strTemp1 += strTemp.Right(strTemp.GetLength() - site);
	strcpy(srcpath, strTemp1);

	strcat(dstpath, "\\paser");

	bfind = find.FindFile(dstpath);
	if(!bfind)
	{
		if(!CreateDirectory(dstpath, NULL))
			return;
	}

	strcat(dstpath, "\\");

	bfind = find.FindFile(srcpath);

	if(bfind)
		bfind = find.FindNextFile();

	while(bfind)
	{
		strcpy(this->m_option.srcfile, find.GetFilePath());
	
		strcpy(this->m_option.dstfile, dstpath);

		strcat(this->m_option.dstfile, find.GetFileTitle());
		strcat(this->m_option.dstfile, "_.jpg");

		tools.SplitJpeg(this->m_option.srcfile, JPEGF_LIST, this->m_option.dstfile);

		bfind = find.FindNextFile();
	}
}

void CV568JpegDlg::OnBnClickedCapstillErrorButton()
{
	// TODO: 在此添加控件通知处理程序代码

	CWaitCursor cursor;
	CBufferBase pool;
	TDataBuf dbuf;
	TJpegCapParm capparm;

	this->GetUserOption(FILE_TAIL_AVI);
	pool.SetSize(0x200000);
	if(pool.GetSize() == 0)
		return;

	dbuf.pData = (UINT8 *)pool.m_buf;
	dbuf.Len = 0;
	dbuf.Pos = 0;

	TestCap(&dbuf, &capparm);

}

void CV568JpegDlg::OnBnClickedBtnJfifSplit()
{
	CFileBin fb;
	CBufferBase poolJfif, poolThumb;
	CGeneralTools tools;
	int size;
	unsigned char thumbx, thumby;
	int len;
	char thumbRgb[100], thumbBmp[100];
	TJpegIndex jpgInfo;
	int i;

	this->GetUserOption(FILE_TAIL_JPG);

	fb.Open(this->m_option.srcfile, "rb");
	size = fb.GetFileLength();
	if(size == 0)
		return;
	poolJfif.SetSize(size);
	poolThumb.SetSize(size);
	if((poolJfif.GetSize() == 0) || (poolThumb.GetSize() == 0))
		return;
	//readf jfif file and get thumbnail info
	fb.Read(poolJfif.m_buf, size);

	JpegParse((unsigned char*)poolJfif.m_buf, (unsigned long)size, &jpgInfo);
	if ((jpgInfo.thumbinfo.thumbtype != THUMBNAIL_JFIF) || (jpgInfo.thumbinfo.thumbtype != THUMBNAIL_JFXX))
		return;

	thumbx = (unsigned char)jpgInfo.thumbinfo.thumbWidth;
	thumby = (unsigned char)jpgInfo.thumbinfo.thumbHeight;
	len = (int)jpgInfo.thumbinfo.thumblen;
	for(i = 0; i < len; i++)
		poolThumb.m_buf[i] = jpgInfo.thumbinfo.pthumbbuf[i];	
	
	fb.Close();

	switch(jpgInfo.thumbinfo.thumbfmt)
	{
	case JPEG422:
		strcpy(thumbRgb, this->m_option.srcfile);
		strcat(thumbRgb, ".thumb.jpg");
		fb.Open (thumbRgb, "wb");
		fb.Write(poolThumb.m_buf, len);
		fb.Close();
		break;
	case RGB8:
		strcpy(thumbRgb, this->m_option.srcfile);
		strcat(thumbRgb, ".rgb");
		strcpy(thumbBmp, this->m_option.srcfile);
		strcat(thumbBmp, ".bmp");
		fb.Open(thumbRgb, "wb");
		fb.Write(poolThumb.m_buf, len);
		fb.Close();
		tools.DataConvert(thumbRgb, thumbBmp, thumbx, thumby, RGB8, BMP, 1, 0);
		break;
	case RGB24:
		strcpy(thumbRgb, this->m_option.srcfile);
		strcat(thumbRgb, ".rgb");
		strcpy(thumbBmp, this->m_option.srcfile);
		strcat(thumbBmp, ".bmp");
		fb.Open(thumbRgb, "wb");
		fb.Write(poolThumb.m_buf, len);
		fb.Close();
		tools.DataConvert(thumbRgb, thumbBmp, thumbx, thumby, RGB24, BMP, 1, 0);
		break;
	default:
		break;
	}
}
/*
void CV568JpegDlg::OnBnClickedBtnExifSplit()
{
	CFileBin fb;
	CBufferBase poolExif, poolThumb;
	CGeneralTools tools;
	int size;
	unsigned char fmt, thumbx, thumby;
	int len;
	char thumbYuv[100], thumbRgb[100], thumbBmp[100];
	int srcfmt, desfmt;

	this->GetUserOption(FILE_TAIL_JPG);

	fb.Open(this->m_option.srcfile, "rb");
	size = fb.GetFileLength();
	if(size == 0)
		return;
	poolExif.SetSize(size);
	poolThumb.SetSize(size);
	if((poolExif.GetSize() == 0) || (poolThumb.GetSize() == 0))
		return;
	//read exif file and get thumbnail info
	fb.Read(poolExif.m_buf, size);
	len = (int)GetThumbFromExif((unsigned char*)poolExif.m_buf, (unsigned long)size, (unsigned char*)poolThumb.m_buf, &fmt, &thumbx, & thumby);
	fb.Close();

	switch(fmt)
	{
	case THUMB_FORMAT_JPEG:
		strcpy(thumbRgb, this->m_option.srcfile);
		strcat(thumbRgb, ".thumb.jpg");
		fb.Open (thumbRgb, "wb");
		fb.Write(poolThumb.m_buf, len);
		fb.Close();
		break;
	case THUMB_FORMAT_RGB24:
		strcpy(thumbRgb, this->m_option.srcfile);
		strcat(thumbRgb, ".rgb");
		strcpy(thumbBmp, this->m_option.srcfile);
		strcat(thumbBmp, ".bmp");
		fb.Open(thumbRgb, "wb");
		fb.Write(poolThumb.m_buf, len);
		fb.Close();
		tools.DataConvert(thumbRgb, thumbBmp, thumbx, thumby, RGB24, BMP, 1, 0);
		break;
	case THUMB_FORMAT_YCBCR:
		strcpy(thumbYuv, this->m_option.srcfile);
		strcat(thumbYuv, ".yuv");
		strcpy(thumbRgb, this->m_option.srcfile);
		strcat(thumbRgb, ".rgb");
		strcpy(thumbBmp, this->m_option.srcfile);
		strcat(thumbBmp, ".bmp");
		fb.Open(thumbYuv, "wb");
		fb.Write(poolThumb.m_buf, len);
		fb.Close();
		srcfmt = YUV422_UYVY;
		desfmt = RGB24;
		tools.DataConvert(thumbYuv, thumbRgb, thumbx, thumby, srcfmt, desfmt, 1, 0);
		srcfmt = RGB24;
		desfmt = BMP;
		tools.DataConvert(thumbRgb, thumbBmp, thumbx, thumby, srcfmt, desfmt, 1, 0);
	default:
		break;
	}
}
*/

void CV568JpegDlg::OnBnClickedBtnExifSplit()
{
	CFileBin fb;
	CBufferBase poolExif, poolThumb;
	CGeneralTools tools;
	int size;
	unsigned char thumbx, thumby;
	int len;
	char thumbYuv[100], thumbRgb[100], thumbBmp[100];
	int srcfmt, desfmt;
	TJpegIndex jpgInfo;
	int i;

	this->GetUserOption(FILE_TAIL_JPG);

	fb.Open(this->m_option.srcfile, "rb");
	size = fb.GetFileLength();
	if(size == 0)
		return;
	poolExif.SetSize(size);
	poolThumb.SetSize(size);
	if((poolExif.GetSize() == 0) || (poolThumb.GetSize() == 0))
		return;
	//read exif file and get thumbnail info
	fb.Read(poolExif.m_buf, size);

	memset((void*)&jpgInfo, 0, sizeof(TJpegIndex));
	JpegParse((unsigned char*)poolExif.m_buf, (unsigned long)size, &jpgInfo);
	if (jpgInfo.thumbinfo.thumbtype != THUMBNAIL_EXIF)
		return;

	thumbx = (unsigned char)jpgInfo.thumbinfo.thumbWidth;
	thumby = (unsigned char)jpgInfo.thumbinfo.thumbHeight;
	len = (int)jpgInfo.thumbinfo.thumblen;
	for(i = 0; i < len; i++)
		poolThumb.m_buf[i] = jpgInfo.thumbinfo.pthumbbuf[i];	
	
	fb.Close();

	switch(jpgInfo.thumbinfo.thumbfmt)
	{
	case JPEG422:
		strcpy(thumbRgb, this->m_option.srcfile);
		strcat(thumbRgb, ".thumb.jpg");
		fb.Open (thumbRgb, "wb");
		fb.Write(poolThumb.m_buf, len);
		fb.Close();
		break;
	case RAW_RGB:
		strcpy(thumbRgb, this->m_option.srcfile);
		strcat(thumbRgb, ".rgb");
		strcpy(thumbBmp, this->m_option.srcfile);
		strcat(thumbBmp, ".bmp");
		fb.Open(thumbRgb, "wb");
		fb.Write(poolThumb.m_buf, len);
		fb.Close();
		tools.DataConvert(thumbRgb, thumbBmp, thumbx, thumby, RAW_RGB, BMP, 1, 0);
		break;	
	case YCbCr422_YYCbCr:
		strcpy(thumbYuv, this->m_option.srcfile);
		strcat(thumbYuv, ".yuv");
		strcpy(thumbRgb, this->m_option.srcfile);
		strcat(thumbRgb, ".rgb");
		strcpy(thumbBmp, this->m_option.srcfile);
		strcat(thumbBmp, ".bmp");
		fb.Open(thumbYuv, "wb");
		fb.Write(poolThumb.m_buf, len);
		fb.Close();
		srcfmt = YUV422_YYUV;
		desfmt = RGB24;
		tools.DataConvert(thumbYuv, thumbRgb, thumbx, thumby, srcfmt, desfmt, 1, 0);
		srcfmt = RGB24;
		desfmt = BMP;
		tools.DataConvert(thumbRgb, thumbBmp, thumbx, thumby, srcfmt, desfmt, 1, 0);

	case YCbCr422_YpCbpCrp:
	case YCbCr420_YYYYCbCr:
	case YCbCr420_YpCbpCrp:
	case RAW_RGB_RpGpBp:
		break;
	default:
		break;
	}
}

void CV568JpegDlg::OnBnClickedQtTest()
{
	// TODO: 在此添加控件通知处理程序代码
	int qftime;

	this->GetUserOption(FILE_TAIL_AVI);
	qftime = this->m_option.qftime;

	// Set a timer to destroy the splash screen.
	if(!this->m_option.testqt)
	{
		this->m_option.testqt = 1;
		m_testqfbt.SetWindowText("Stop");
		m_qffb.Open(this->m_option.dstfile, "wb");
        SetTimer(TIMER_QF, qftime, NULL);
	}
	else
	{
		KillTimer(TIMER_QF);
		m_qffb.Close();
		this->m_option.testqt = 0;
		m_testqfbt.SetWindowText("Test");
	}

}

void CV568JpegDlg::OnTimer(UINT nIDEvent)
{
	CFileBin fb;
	UINT8 ft;
	char str[100];
	SYSTEMTIME time;
	UINT16 frmcnt;
	int frmrate;
	int minute, sec;
	CWnd *pWnd;

	if(TIMER_QF == nIDEvent)
	{
		ft = V5B_JpegGetQfactor();
		sprintf(str, "		Qf		=		0x%2x\n", ft);
		m_qffb.Write(str, (int)strlen(str));
	}

	if(TIMER_FR == nIDEvent)
	{
		GetSystemTime(&time);
		frmcnt = V5B_JpegGetFrameCnt();

		minute = (time.wMinute + 60 - this->m_option.frmtime.wMinute) % 60;
		sec = (time.wSecond + 60 - this->m_option.frmtime.wSecond) % 60;
		sec += minute;

		frmrate = this->m_option.frmcnt > frmcnt ? (0xffff+frmcnt-this->m_option.frmcnt) : (frmcnt-this->m_option.frmcnt);
		frmrate /= sec;

		pWnd = this->GetDlgItem(IDC_STATIC_FRAME_RATE);
		sprintf(str, "%d f/s", frmrate);
		pWnd->SetWindowText(str);

		this->m_option.frmcnt = frmcnt;
		this->m_option.frmtime = time;
	}

}



void CV568JpegDlg::OnBnClickedQtSet()
{
	// TODO: 在此添加控件通知处理程序代码
	char str[100];

	this->GetUserOption(FILE_TAIL_AVI);
	sprintf(str, "========begin to wr Qf=========\n");
	if(this->m_option.testqt)
		m_qffb.Write(str, (int)strlen(str));
	V5_JpegSetQFactor((UINT8)this->m_option.qf);

/*
	qftime = this->m_option.qftime;

	// Set a timer to destroy the splash screen.
	if(!btimer)
	{
		qf = this->m_option.qf;
		btimer = 1;
		m_testqfbt.SetWindowText("Stop");
		m_testqfsetbt.SetWindowText("Stop");
		m_qffb.Open(this->m_option.dstfile, "wb");
        SetTimer(1, qftime, NULL);
		V5_JpegSetQFactor((UINT8)qf);
	}
	else
	{
		KillTimer(1);
		m_qffb.Close();
		btimer = 0;
		m_testqfbt.SetWindowText("Test");
		m_testqfsetbt.SetWindowText("Test");
	}
	*/
}

void CV568JpegDlg::OnBnClickedQtAuto()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_QtAutoBtn.GetCheck())
		V5B_JpegEnAbrc();
	else
        V5B_JpegDisAbrc();

}

#define TEST_FRM_RATE_TIME 5

void CV568JpegDlg::OnBnClickedFrameRateTest()
{
	// TODO: 在此添加控件通知处理程序代码
/*
	CWnd *pWnd;

	pWnd = this->GetDlgItem(IDC_FRAME_RATE_TEST);
	this->m_option.frmcnt = 0;
	GetSystemTime(&this->m_option.frmtime);
	// Set a timer to destroy the splash screen.
	if(!this->m_option.testfrmrate)
	{
		this->m_option.testfrmrate = 1;
		pWnd->SetWindowText("Stop");
        SetTimer(TIMER_FR, 2000, NULL);
	}
	else
	{
		KillTimer(TIMER_FR);
		this->m_option.testfrmrate = 0;
		pWnd->SetWindowText("Test");
	}
*/

    CWaitCursor cursor;
	CWnd *pWnd, *pWnd1;
	UINT32 msec0, frmrate;
	SYSTEMTIME Time;
	int i = 0;
	char str[100];
	UINT32 msec[TEST_FRM_RATE_TIME], count[TEST_FRM_RATE_TIME];

	pWnd = this->GetDlgItem(IDC_STATIC_FRAME_RATE);
	pWnd1 = this->GetDlgItem(IDC_FRAME_RATE_TEST);
	pWnd1->EnableWindow(FALSE);
	for(i=0; i<TEST_FRM_RATE_TIME; i++)
	{
		GetSystemTime(&Time);
		msec[i] = ((Time.wHour*60 + Time.wMinute)*60 + Time.wSecond)*1000 + Time.wMilliseconds;
		count[i] = V5B_JpegGetFrameCnt();
		Sleep(2000);
	}
	frmrate = 0;
	for(i=0; i<TEST_FRM_RATE_TIME-1; i++)
	{
		msec0 = msec[i+1]-msec[i];
		msec0 = msec0 ? msec0 : 1;
		frmrate += (count[i+1]-count[i])*10000/msec0;
	}
	frmrate /= (TEST_FRM_RATE_TIME-1);
	sprintf(str, "rate = %d.%d f/s", frmrate/10, frmrate%10);
	pWnd->SetWindowText(str);
	pWnd1->EnableWindow();

}

void CV568JpegDlg::OnBnClickedButtonTcr()
{
	// TODO: 在此添加控件通知处理程序代码
	this->GetUserOption();
	V5B_JpegSetTCR((UINT8)this->m_option.tcr);
}

void CV568JpegDlg::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	CWaitCursor cursor;
	UINT8 i, x;

	this->m_option.qf = V5B_JpegGetBrcQF();
	this->m_option.tcr = V5B_JpegGetTCR();

	if( V5B_JpegGetBrc() & 0x80 )
		this->m_QtAutoBtn.SetCheck(1);
	else
		this->m_QtAutoBtn.SetCheck(0);

	this->m_option.enhbrc.ThdR1 = V5B_JpegGetEnhThdR1();
	this->m_option.enhbrc.ThdR2 = V5B_JpegGetEnhThdR2();
	this->m_option.enhbrc.ThdL1 = V5B_JpegGetEnhThdL1();
	this->m_option.enhbrc.ThdL2 = V5B_JpegGetEnhThdL2();
	this->m_option.enhbrc.ShfR1 = V5B_JpegGetEnhShfR1();
	this->m_option.enhbrc.ShfR2 = V5B_JpegGetEnhShfR2();
	this->m_option.enhbrc.ShfL1 = V5B_JpegGetEnhShfL1();
	this->m_option.enhbrc.ShfL2 = V5B_JpegGetEnhShfL2();
	for(i=0; i<6; i++)
		this->m_option.rudebrc.CompLevel[i] = V5B_JpegGetRudeCompLevel(i);
	this->m_option.rudebrc.RudeAc = V5B_JpegGetRudeAc();
	this->m_option.rudebrc.RudeLevel = V5B_JpegGetRudeLevel();
	this->m_option.rudebrc.ThdWc = V5B_JpegGetRudeThdWc();

	x = V5B_JpegGetRudeBrc();
	if(x & BIT7)
		this->m_option.rudebrcen = 1;
	else
		this->m_option.rudebrcen = 0;
	if(x & BIT6)
		this->m_option.rudeyuonlyen = 1;
	else
		this->m_option.rudeyuonlyen = 0;

	x = V5B_JpegGetMod();
	if(x & BIT7)
		this->m_option.enhbrcen = 1;
	else
		this->m_option.enhbrcen = 0;

	this->m_option.capdlyen = V5_JpegGetContext()->capdly.en;
	this->m_option.capdlyfeed = V5_JpegGetContext()->capdly.feedval;

	this->DispOption();
}

void CV568JpegDlg::OnBnClickedButtonRudeSet()
{
	// TODO: 在此添加控件通知处理程序代码

	this->GetUserOption();
	V5_JpegSetRudeBrcPara(&this->m_option.rudebrc);

	if(this->m_option.rudeyuonlyen)
		V5B_JpegRudeBrcUvOnlyEnable();
	else
		V5B_JpegRudeBrcUvOnlyEnable();

	if(this->m_option.rudebrcen)
		V5B_JpegRudeBrcEnable();
	else
		V5B_JpegRudeBrcDisable();

}

void CV568JpegDlg::OnBnClickedButtonEnhSet()
{
	this->GetUserOption();
	V5_JpegSetEnhBrcPara(&this->m_option.enhbrc);
	if(this->m_option.enhbrcen)
	{
		V5B_JpegEnhBrcEnable();
	}
	else
	{
		V5B_JpegEnhBrcDisable();
	}
}

void CV568JpegDlg::DecChgFile(char* filename, TSize size, UINT8 mode)
{
	CGeneralTools tools;
	UINT8 yuvmode;
	UINT8 srcfmt, desfmt;
	char src[0x100], dst[0x100];

	if(mode == DECODE_MODE)
	{
		strcpy(src, filename);
		strcat(src, ".dat");
		strcpy(dst, filename);
		strcat(dst, ".yuv");

		yuvmode = V5B_LbufGetYuvMode();
		if(V5B_LBUF_YUV_444 == yuvmode)
			yuvmode = V5B_LBUF_YUV_422;

		srcfmt = YuvmodeToYuvLbuf(yuvmode);
		desfmt = YuvmodeToYuvfmt(yuvmode);
		tools.DataConvert(src, dst, size.cx, size.cy, 
								srcfmt, desfmt, 1, 0);


		strcpy(src, filename);
		strcat(src, ".rgb");

		srcfmt = YuvmodeToYuvfmt(yuvmode);
		desfmt = RGB24;
		tools.DataConvert(dst, src, size.cx, size.cy, 
								srcfmt, desfmt, 1, 0);

		strcpy(dst, filename);
		strcat(dst, ".bmp");

		srcfmt = RGB24;
		desfmt = BMP;
		tools.DataConvert(src, dst, size.cx, size.cy, srcfmt, desfmt, 1, 0);
	}
	else
	{
		strcpy(src, filename);
		strcat(src, ".dat");
		strcpy(dst, filename);
		strcat(dst, ".rgb");

		if( mode == DECODEFRAME_MODE || mode == DECODEFRAMEBYPASS_MODE )
		{
			if(V5B_LbufGetInterCtrl() & V5B_LBUF_INTERCTRL_RGBSEL)
			{
				srcfmt = RGB565;
			}
			else
				srcfmt = YuvmodeToYuvfmt(V5B_LBUF_YUV_422);
		}
		else
			srcfmt = YuvmodeToYuvfmt(V5B_LBUF_YUV_422);
		desfmt = RGB24;

		tools.DataConvert(src, dst, size.cx, size.cy, srcfmt, desfmt, 1, 0);

		strcpy(src, filename);
		strcat(src, ".bmp");

		srcfmt = RGB24;
		desfmt = BMP;
		tools.DataConvert(dst, src, size.cx, size.cy, srcfmt, desfmt, 1, 0);

	}
}

void CV568JpegDlg::TestDec(UINT8 mode, UINT8 zoom)
{
	CStringArray array;
	CBufferBase pool, temppool;
	CFileBin fb;
	TDataBuf dbuf;
	TDataBuf tempbuf;
	TJpegIndex info;
	CString strPathDst;
	int len;
	char root[0x100];
	CString strDstfile;
	CString strSrcfile;
	CString strTemp;
	TSize panelSize, ippDispSize;
	TPoint pt;
	TALayerProty proty;


	this->GetUserOption(FILE_TAIL_DAT);
	//===get root
	strPathDst = this->m_option.srcfile;
	if(strPathDst.IsEmpty())
		return;
	if(strPathDst.Find('.'))
	{
		strPathDst = strPathDst.Left(strPathDst.ReverseFind('\\'));
	}
	strcpy(root, strPathDst);

	strPathDst = this->m_option.dstfile;
	if(strPathDst.IsEmpty())
		return;
	if(strPathDst.Find('.'))
	{
		strPathDst = strPathDst.Left(strPathDst.ReverseFind('\\'));
	}

	SetCurrentDirectory(root);

	EnumFile(&array, root, "*.jpg", 1);

	SetCurrentDirectory(strPathDst);

	strTemp.Format("_m%x_z%x", mode, zoom);
	strPathDst += strTemp;
	V5_DispGetPanelSize(&panelSize);
	pt.x = 0;
	pt.y = 0;

	for(int i = 0; i< array.GetCount(); i++)
	{
		strSrcfile = root;
		strSrcfile += array.GetAt(i);
		fb.Open(strSrcfile.GetBuffer());
		len = fb.GetFileLength();
		if(len == 0)
			return;
		pool.SetSize(len << 1);
		fb.Read(pool.m_buf, len);
		fb.Close();
		//=========== test code =========
		JpegParse((UINT8 *)pool.m_buf, len, &info);
		if(info.YUVType == JPEG_UNKNOWN)
			continue;
		dbuf.pData = (UINT8 *)pool.m_buf;
		dbuf.Len = (UINT32)len;
		dbuf.Pos = 0;
		len = info.JpgSize.cx * info.JpgSize.cy * 3;
		temppool.SetSize(len);
		tempbuf.pData = (UINT8*)temppool.m_buf;
		tempbuf.Len = len;
		tempbuf.Pos = 0;

		if(mode == DECODEIPP_MODE || mode == DECODEFRAME_MODE)
		{
			// change ipp sizer
			ippDispSize.cx = (info.ImageSize.cx < panelSize.cx) ? info.ImageSize.cx : panelSize.cx;
			ippDispSize.cy = (info.ImageSize.cy < panelSize.cy) ? info.ImageSize.cy : panelSize.cy;
			ippDispSize.cx >>= zoom;
			ippDispSize.cy >>= zoom;
			V5_IppSetDispSizerEx2(info.JpgSize, pt, info.ImageSize, ippDispSize);
			if(mode == DECODEFRAME_MODE)
			{
				proty.memsize = ippDispSize;
				proty.pos2panel = pt;
				proty.win.ltp = pt;
				proty.win.size = ippDispSize;
				V5_DispSetAmemSize(&proty);
			}
			V5B_LbufSetYuvMode(info.YUVType);
			V5_JpegAutoSetImage();
			V5_MarbAutoSetMem();
		}
		
		switch(mode)
		{
		case DECODE_MODE:
			V5_HJpegDec(&dbuf, &tempbuf);
			break;
		case DECODEIPP_MODE:
			V5_HJpegDecIpp(&dbuf, &tempbuf);
			break;
		case DECODEFRAME_MODE:
			V5_HJpegDecFrm(&dbuf, &tempbuf);
			break;
		}

		if(tempbuf.Pos == 0)
			continue;

		strDstfile = strPathDst + array.GetAt(i);
		strTemp = strDstfile + ".dat";
		fb.Open(strTemp.GetBuffer(), "wb");
		fb.Write((char*)tempbuf.pData, tempbuf.Pos);
		fb.Close();

		DecChgFile(strDstfile.GetBuffer(), info.JpgSize, mode);

		Sleep(this->m_option.delay);

	}
	array.RemoveAll();
}

void CV568JpegDlg::OnBnClickedDecButton()
{
	TestDec(DECODE_MODE , 0);
}

void CV568JpegDlg::OnBnClickedDecippButton()
{
	TestDec(DECODEIPP_MODE , 0);
}


void CV568JpegDlg::OnBnClickedDeclcdButton()
{
	TestDec(DECODEFRAME_MODE , 0);
}

void CV568JpegDlg::EnumFile(CStringArray * arry, const char* root, const char* filetype, char type)
/* type 0 only curent directory, 1 inclue the deep directory */
/* filetype such as "*.jpeg, *.*"*/
{
	BOOL bAllFile = FALSE;
	BOOL bWorking;
	CFileFind* cf;
	CString str, strTemp;

	if(arry == NULL)
		return;
	cf = new CFileFind;
	_ASSERT(cf != NULL);

	str = filetype;
	if(str.IsEmpty() || str == "*.*" || str == "*")
		bAllFile = TRUE;
	else
	{
		str = str.Mid(str.ReverseFind('.'));
	}

	bWorking = cf->FindFile();
	if( !bWorking )
	{
		delete cf;
		return;
	}


	while(bWorking)
	{
		bWorking = cf->FindNextFile();
		if(cf->IsDots())
			continue;

		if(type && cf->IsDirectory())
		{
			SetCurrentDirectory(cf->GetFilePath());
			EnumFile(arry, root, filetype, type);
			SetCurrentDirectory(cf->GetRoot());
		}
		else
		{
			if(!bAllFile)
			{
				strTemp = cf->GetFileName();
				strTemp = strTemp.Mid(strTemp.ReverseFind('.'));
				if(str != strTemp)
					continue;
			}
			strTemp = cf->GetFilePath();
			strTemp = strTemp.Mid((int)(strTemp.Find(root) + strlen(root)));
			arry->Add(strTemp);
		}
	}

	delete cf;
}


void CV568JpegDlg::OnBnClickedDecAllButton()
{
	for(unsigned char i = 0; i < 2; i++)
	{
		TestDec(DECODE_MODE , i);
		TestDec(DECODEIPP_MODE , i);
		TestDec(DECODEFRAME_MODE , i);
	}

}

void CV568JpegDlg::OnBnClickedButtonCapDly()
{
	this->GetUserOption(FILE_TAIL_DAT);

	V5_JpegGetContext()->capdly.feedval = (UINT8)this->m_option.capdlyfeed;
}

void CV568JpegDlg::OnBnClickedCapDlyEn()
{
	if(this->m_capdlyen.GetCheck())
	{
		this->m_option.capdlyen = 1;
		V5B_LbufCapDlyEnable();
	}
	else
	{
		this->m_option.capdlyen = 0;
		V5B_LbufCapDlyDisable();
	}
	V5_JpegGetContext()->capdly.en = (UINT8)this->m_option.capdlyen;
}



#endif //V578CMD_DISABLE



