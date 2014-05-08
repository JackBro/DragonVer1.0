// BlkIspDlg.cpp : 实现文件
//
#include "internal.h"



// CBlkIspDlg 对话框

IMPLEMENT_DYNAMIC(CBlkIspDlg, CBasePanelDlg)
CBlkIspDlg::CBlkIspDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CBlkIspDlg::IDD, pParent)
{
}

CBlkIspDlg::~CBlkIspDlg()
{
}

void CBlkIspDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_BLACKLEVEL, m_blacklevel);
	DDX_Control(pDX, IDC_CHECK_DPD, m_dpddpc);
	DDX_Control(pDX, IDC_CHECK_LENSFALLOFF, m_lensfalloff);
	DDX_Control(pDX, IDC_CHECK_DIGITALGAIN, m_digitalgain);
	DDX_Control(pDX, IDC_CHECK_CFA, m_cfa);
	DDX_Control(pDX, IDC_CHECK_COLORCORRECT, m_colorcorrect);
	DDX_Control(pDX, IDC_CHECK_GAMMACORRECT, m_gammacorrect);
	DDX_Control(pDX, IDC_CHECK_EDGEENHANCE, m_edgeenhance);
	DDX_Control(pDX, IDC_CHECK_RAWDATA, m_rawdata);
	DDX_Control(pDX, IDC_CHECK_JPEG_HEADER, m_jpegheader);
	DDX_Control(pDX, IDC_CHECK_DITHER, m_dither);
	DDX_Control(pDX, IDC_CHECK_DPDDPC_NEW, m_newdpd);
	DDX_Control(pDX, IDC_CHECK_NEW_EDGE, m_newedge);
	DDX_Control(pDX, IDC_CHECK_COLORCONVERT, m_colorconvert);
	DDX_Control(pDX, IDC_CHECK_OUTPUTFILE, m_outputfile);
	DDX_Control(pDX, IDC_CHECK_BMP, m_outputbmp);
	DDX_Control(pDX, IDC_CHECK_AUTOFOCUS, m_autofocus);
	DDX_Control(pDX, IDC_EDIT_BGLINE, m_bgline);
	DDX_Control(pDX, IDC_EDIT_GPIXEL, m_gfirst);
	DDX_Control(pDX, IDC_EDIT_UNIT, m_unit);	
	DDX_Control(pDX, IDC_EDIT_GAIN, m_gain);	
	DDX_Control(pDX, IDC_EDIT_FILE_COUNT, m_filecount);	
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_width);	
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_height);	
	DDX_Control(pDX, IDC_EDIT_DPDTHD, m_dpdthd);	

	DDX_Control(pDX, IDC_EDIT_EDGE00, m_edgepara[0]);	
	DDX_Control(pDX, IDC_EDIT_EDGE01, m_edgepara[1]);	
	DDX_Control(pDX, IDC_EDIT_EDGE02, m_edgepara[2]);	
	DDX_Control(pDX, IDC_EDIT_EDGE03, m_edgepara[3]);	
	DDX_Control(pDX, IDC_EDIT_EDGE04, m_edgepara[4]);	
	DDX_Control(pDX, IDC_EDIT_EDGE10, m_edgepara[5]);	
	DDX_Control(pDX, IDC_EDIT_EDGE11, m_edgepara[6]);	
	DDX_Control(pDX, IDC_EDIT_EDGE12, m_edgepara[7]);	
	DDX_Control(pDX, IDC_EDIT_EDGE13, m_edgepara[8]);	
	DDX_Control(pDX, IDC_EDIT_EDGE14, m_edgepara[9]);	
	DDX_Control(pDX, IDC_EDIT_EDGE20, m_edgepara[10]);	
	DDX_Control(pDX, IDC_EDIT_EDGE21, m_edgepara[11]);	
	DDX_Control(pDX, IDC_EDIT_EDGE22, m_edgepara[12]);	
	DDX_Control(pDX, IDC_EDIT_EDGE23, m_edgepara[13]);	
	DDX_Control(pDX, IDC_EDIT_EDGE24, m_edgepara[14]);	
	DDX_Control(pDX, IDC_EDIT_EDGE30, m_edgepara[15]);	
	DDX_Control(pDX, IDC_EDIT_EDGE31, m_edgepara[16]);	
	DDX_Control(pDX, IDC_EDIT_EDGE32, m_edgepara[17]);	
	DDX_Control(pDX, IDC_EDIT_EDGE33, m_edgepara[18]);	
	DDX_Control(pDX, IDC_EDIT_EDGE34, m_edgepara[19]);	
	DDX_Control(pDX, IDC_EDIT_EDGE40, m_edgepara[20]);	
	DDX_Control(pDX, IDC_EDIT_EDGE41, m_edgepara[21]);	
	DDX_Control(pDX, IDC_EDIT_EDGE42, m_edgepara[22]);	
	DDX_Control(pDX, IDC_EDIT_EDGE43, m_edgepara[23]);	
	DDX_Control(pDX, IDC_EDIT_EDGE44, m_edgepara[24]);	
	DDX_Control(pDX, IDC_EDIT_EDGE_DIV, m_edgediv);	
	DDX_Control(pDX, IDC_EDIT_EDGE_MAX, m_edgemax);	
	DDX_Control(pDX, IDC_EDIT_EDGE_SUM, m_edgesum);	
	DDX_Control(pDX, IDC_EDIT_AH0, m_afh[0]);	
	DDX_Control(pDX, IDC_EDIT_AH1, m_afh[1]);	
	DDX_Control(pDX, IDC_EDIT_AH2, m_afh[2]);	
	DDX_Control(pDX, IDC_EDIT_AH3, m_afh[3]);	
	DDX_Control(pDX, IDC_EDIT_AH4, m_afh[4]);	
	DDX_Control(pDX, IDC_EDIT_AV0, m_afv[0]);	
	DDX_Control(pDX, IDC_EDIT_AV1, m_afv[1]);	
	DDX_Control(pDX, IDC_EDIT_AV2, m_afv[2]);	
	DDX_Control(pDX, IDC_EDIT_AV3, m_afv[3]);	
	DDX_Control(pDX, IDC_EDIT_AV4, m_afv[4]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT00, m_afw[0]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT01, m_afw[1]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT02, m_afw[2]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT03, m_afw[3]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT10, m_afw[4]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT11, m_afw[5]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT12, m_afw[6]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT13, m_afw[7]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT20, m_afw[8]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT21, m_afw[9]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT22, m_afw[10]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT23, m_afw[11]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT30, m_afw[12]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT31, m_afw[13]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT32, m_afw[14]);	
	DDX_Control(pDX, IDC_EDIT_WEIGHT33, m_afw[15]);	
	DDX_Control(pDX, IDC_EDIT_FRAME_NUM, m_motionframecount);	
	DDX_Control(pDX, IDC_EDIT_H_THRESHOLD, m_motionhthd);	
	DDX_Control(pDX, IDC_EDIT_MOTION_THRESHOLD, m_motionmthd);	
	DDX_Control(pDX, IDC_EDIT_MOTION_DETECT, m_motiondetect);
}

BOOL CBlkIspDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();
	
	this->InitUserOption();
	this->DispUserOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CBlkIspDlg::InitUserOption(void)
{
	memset( &(this->m_option), 0, sizeof(TIspBlkOption) );
	this->m_option.bgline = 1;
	this->m_option.gfirst = 1;
	this->m_option.unit = 2;
	this->m_option.rawdata = 1;
	this->m_option.jpegheader = 1;
	this->m_option.gain = 0x40;
	this->m_option.filecount = 0xff;
	this->m_option.width = 640;
	this->m_option.height = 480;
	this->m_option.dither = 0;
	this->m_option.newdpd = 0;
	this->m_option.newedge = 0;
	this->m_option.dpdthd = 0xff;
	this->m_option.outputfile = 1;
	this->m_option.outputbmp = 1;
	this->m_colorcorrect.EnableWindow(FALSE);
	this->m_gammacorrect.EnableWindow(FALSE);
	this->m_colorconvert.EnableWindow(FALSE);
	this->m_edgeenhance.EnableWindow(FALSE);
	this->m_newedge.EnableWindow(FALSE);
	this->m_autofocus.EnableWindow(FALSE);
	this->m_option.edgepara[0] = 0;
	this->m_option.edgepara[1] = 0;
	this->m_option.edgepara[2] = 0;
	this->m_option.edgepara[3] = 0;
	this->m_option.edgepara[4] = 0;
	this->m_option.edgepara[5] = 0;
	this->m_option.edgepara[6] = -1;
	this->m_option.edgepara[7] = -1;
	this->m_option.edgepara[8] = -1;
	this->m_option.edgepara[9] = 0;
	this->m_option.edgepara[10] = 0;
	this->m_option.edgepara[11] = -1;
	this->m_option.edgepara[12] = 8;
	this->m_option.edgepara[13] = -1;
	this->m_option.edgepara[14] = 0;
	this->m_option.edgepara[15] = 0;
	this->m_option.edgepara[16] = -1;
	this->m_option.edgepara[17] = -1;
	this->m_option.edgepara[18] = -1;
	this->m_option.edgepara[19] = 0;
	this->m_option.edgepara[20] = 0;
	this->m_option.edgepara[21] = 0;
	this->m_option.edgepara[22] = 0;
	this->m_option.edgepara[23] = 0;
	this->m_option.edgepara[24] = 0;
	this->m_option.edgediv = 8;
	this->m_option.edgemax = 255;
	this->m_option.edgesum = 0;

	this->m_option.afh[0] = 0;
	this->m_option.afh[1] = 160;
	this->m_option.afh[2] = 160;
	this->m_option.afh[3] = 160;
	this->m_option.afh[4] = 160;

	this->m_option.afv[0] = 0;
	this->m_option.afv[1] = 120;
	this->m_option.afv[2] = 120;
	this->m_option.afv[3] = 120;
	this->m_option.afv[4] = 120;
	
	this->m_option.afw[0] = 1;
	this->m_option.afw[1] = 1;
	this->m_option.afw[2] = 1;
	this->m_option.afw[3] = 1;
	this->m_option.afw[4] = 1;
	this->m_option.afw[5] = 1;
	this->m_option.afw[6] = 1;
	this->m_option.afw[7] = 1;
	this->m_option.afw[8] = 1;
	this->m_option.afw[9] = 1;
	this->m_option.afw[10] = 1;
	this->m_option.afw[11] = 1;
	this->m_option.afw[12] = 1;
	this->m_option.afw[13] = 1;
	this->m_option.afw[14] = 1;
	this->m_option.afw[15] = 1;
	//motion para
	this->m_option.motionframecount = 2;
	this->m_option.motionhthd = 0x68;
	this->m_option.motionmthd = 0x10;
	this->m_option.motiondetect = 0;
}

void CBlkIspDlg::GetUserOption(char *tail)
{
	CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, tail);
	this->GetEditValue();
	this->GetCheckValue();
}

void CBlkIspDlg::GetCheckValue(void)
{
	this->m_option.ispctrl = 0;

	if((this->m_blacklevel.GetCheck() == BST_CHECKED) && this->m_blacklevel.IsWindowEnabled())
		this->m_option.ispctrl |= BLACKLEVEL;
	else
		this->m_option.ispctrl &= (~(BLACKLEVEL));
	
	if((this->m_lensfalloff.GetCheck() == BST_CHECKED) && this->m_lensfalloff.IsWindowEnabled())
		this->m_option.ispctrl |= LENSFALLOFF;
	else
		this->m_option.ispctrl &= (~(LENSFALLOFF));
	
	if((this->m_dpddpc.GetCheck() == BST_CHECKED) && this->m_dpddpc.IsWindowEnabled())
		this->m_option.ispctrl |= DPDDPC;
	else
		this->m_option.ispctrl &= (~(DPDDPC));
	
	if((this->m_digitalgain.GetCheck() == BST_CHECKED) && this->m_digitalgain.IsWindowEnabled())
		this->m_option.ispctrl |= DIGITALGAIN;
	else
		this->m_option.ispctrl &= (~(DIGITALGAIN));
	
	if((this->m_cfa.GetCheck() == BST_CHECKED) && this->m_cfa.IsWindowEnabled())
		this->m_option.ispctrl |= CFAEXTRACT;
	else
		this->m_option.ispctrl &= (~(CFAEXTRACT));
	
	if((this->m_colorcorrect.GetCheck() == BST_CHECKED) && this->m_colorcorrect.IsWindowEnabled())
		this->m_option.ispctrl |= COLORCORRECT;
	else
		this->m_option.ispctrl &= (~(COLORCORRECT));
	
	if((this->m_gammacorrect.GetCheck() == BST_CHECKED) && this->m_gammacorrect.IsWindowEnabled())
		this->m_option.ispctrl |= GAMMACORRECT;
	else
		this->m_option.ispctrl &= (~(GAMMACORRECT));
	
	if((this->m_edgeenhance.GetCheck() == BST_CHECKED) && this->m_edgeenhance.IsWindowEnabled())
		this->m_option.ispctrl |= EDGEENHANCE;
	else
		this->m_option.ispctrl &= (~(EDGEENHANCE));
	
	if((this->m_colorconvert.GetCheck() == BST_CHECKED) && this->m_colorconvert.IsWindowEnabled())
		this->m_option.ispctrl |= COLORCONVERT;
	else
		this->m_option.ispctrl &= (~(COLORCONVERT));
	
	if((this->m_autofocus.GetCheck() == BST_CHECKED) && this->m_autofocus.IsWindowEnabled())
		this->m_option.ispctrl |= AUTOFOCUS;
	else
		this->m_option.ispctrl &= (~(AUTOFOCUS));
	
	if(this->m_rawdata.GetCheck() == BST_CHECKED)
		this->m_option.rawdata = 1;
	else
		this->m_option.rawdata = 0;

	if(this->m_jpegheader.GetCheck() == BST_CHECKED)
		this->m_option.jpegheader = 1;
	else
		this->m_option.jpegheader = 0;

	if((this->m_dither.GetCheck() == BST_CHECKED) && this->m_dither.IsWindowEnabled())
		this->m_option.dither = 1;
	else
		this->m_option.dither = 0;
	if(this->m_newdpd.GetCheck() == BST_CHECKED)
		this->m_option.newdpd = 1;
	else
		this->m_option.newdpd = 0;
	if((this->m_newedge.GetCheck() == BST_CHECKED) && this->m_newedge.IsWindowEnabled())
		this->m_option.newedge = 1;
	else
		this->m_option.newedge = 0;
	if(this->m_outputfile.GetCheck() == BST_CHECKED)
		this->m_option.outputfile = 1;
	else
		this->m_option.outputfile = 0;
	if(this->m_outputbmp.GetCheck() == BST_CHECKED)
		this->m_option.outputbmp = 1;
	else
		this->m_option.outputbmp = 0;
}

void CBlkIspDlg::DispUserOption(void)
{
	char str[100];
	int i;

	sprintf(str, "%d", this->m_option.bgline);
	this->m_bgline.SetWindowText(str);
	sprintf(str, "%d", this->m_option.gfirst);
	this->m_gfirst.SetWindowText(str);
	sprintf(str, "%d", this->m_option.unit);
	this->m_unit.SetWindowText(str);
	sprintf(str, "%x", this->m_option.gain);
	this->m_gain.SetWindowText(str);
	sprintf(str, "%x", this->m_option.filecount);
	this->m_filecount.SetWindowText(str);
	sprintf(str, "%d", this->m_option.width);
	this->m_width.SetWindowText(str);
	sprintf(str, "%d", this->m_option.height);
	this->m_height.SetWindowText(str);
	sprintf(str, "%x", this->m_option.dpdthd);
	this->m_dpdthd.SetWindowText(str);
	//motion para
	sprintf(str, "%d", this->m_option.motionframecount);
	this->m_motionframecount.SetWindowText(str);
	sprintf(str, "%d", this->m_option.motionhthd);
	this->m_motionhthd.SetWindowText(str);
	sprintf(str, "%d", this->m_option.motionmthd);
	this->m_motionmthd.SetWindowText(str);
	sprintf(str, "%d", this->m_option.motiondetect);
	this->m_motiondetect.SetWindowText(str);
	for(i = 0; i < 25; i++)
	{
		sprintf(str, "%d", this->m_option.edgepara[i]);
		this->m_edgepara[i].SetWindowText(str);
	}

	for(i = 0; i < 5; i++)
	{
		sprintf(str, "%d", this->m_option.afh[i]);
		this->m_afh[i].SetWindowText(str);
	}

	for(i = 0; i < 5; i++)
	{
		sprintf(str, "%d", this->m_option.afv[i]);
		this->m_afv[i].SetWindowText(str);
	}

	for(i = 0; i < 16; i++)
	{
		sprintf(str, "%d", this->m_option.afw[i]);
		this->m_afw[i].SetWindowText(str);
	}
	sprintf(str, "%d", this->m_option.edgediv);
	this->m_edgediv.SetWindowText(str);
	sprintf(str, "%d", this->m_option.edgemax);
	this->m_edgemax.SetWindowText(str);

	sprintf(str, "%x", this->m_option.edgesum);
	this->m_edgesum.SetWindowText(str);

	if(this->m_option.rawdata)
		this->m_rawdata.SetCheck(BST_CHECKED);
	else
		this->m_rawdata.SetCheck(BST_UNCHECKED);
	if(this->m_option.jpegheader)
		this->m_jpegheader.SetCheck(BST_CHECKED);
	else
		this->m_jpegheader.SetCheck(BST_UNCHECKED);

	if(this->m_option.dither)
		this->m_dither.SetCheck(BST_CHECKED);
	else
		this->m_dither.SetCheck(BST_UNCHECKED);

	if(this->m_option.newdpd)
		this->m_newdpd.SetCheck(BST_CHECKED);
	else
		this->m_newdpd.SetCheck(BST_UNCHECKED);

	if(this->m_option.newedge)
		this->m_newedge.SetCheck(BST_CHECKED);
	else
		this->m_newedge.SetCheck(BST_UNCHECKED);
	if(this->m_option.outputfile)
		this->m_outputfile.SetCheck(BST_CHECKED);
	else
		this->m_outputfile.SetCheck(BST_UNCHECKED);
	if(this->m_option.outputbmp)
		this->m_outputbmp.SetCheck(BST_CHECKED);
	else
		this->m_outputbmp.SetCheck(BST_UNCHECKED);
}

BEGIN_MESSAGE_MAP(CBlkIspDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_BTN_ISP_PROCESS, OnBnClickedBtnIspProcess)
	ON_BN_CLICKED(IDC_BTN_SEL_ALL, OnBnClickedBtnSelAll)
	ON_BN_CLICKED(IDC_BTN_CMODULE_TEST, OnBnClickedBtnCmoduleTest)
	ON_BN_CLICKED(IDC_BTN_CREATE_RAW_DATA, OnBnClickedBtnCreateRawData)
	ON_BN_CLICKED(IDC_BTN_COMPARE_Y, OnBnClickedBtnCompareY)
	ON_BN_CLICKED(IDC_BTN_10TO8, OnBnClickedBtn10to8)
	ON_BN_CLICKED(IDC_CHECK_COLORCONVERT, OnBnClickedCheckColorconvert)
	ON_BN_CLICKED(IDC_CHECK_CFA, OnBnClickedCheckCfa)
	ON_BN_CLICKED(IDC_CHECK_EDGEENHANCE, OnBnClickedCheckEdgeenhance)
	ON_BN_CLICKED(IDC_CHECK_NEW_EDGE, OnBnClickedCheckNewEdge)
	ON_BN_CLICKED(IDC_BTN_MOTION, OnBnClickedBtnMotion)
END_MESSAGE_MAP()

// CBlkIspDlg 消息处理程序

//======================================



void CBlkIspDlg::GetEditValue(void)
{
	char str[100];
	int i;

	for(i = 0; i < 25; i++)
	{
		this->m_edgepara[i].GetWindowText(str, 100);
		sscanf(str, "%d", &(this->m_option.edgepara[i]));
	}
	for(i = 0; i < 5; i++)
	{
		this->m_afh[i].GetWindowText(str, 100);
		sscanf(str, "%d", &(this->m_option.afh[i]));
	}
	for(i = 0; i < 5; i++)
	{
		this->m_afv[i].GetWindowText(str, 100);
		sscanf(str, "%d", &(this->m_option.afv[i]));
	}
	for(i = 0; i < 16; i++)
	{
		this->m_afw[i].GetWindowText(str, 100);
		sscanf(str, "%d", &(this->m_option.afw[i]));
	}
	this->m_edgediv.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.edgediv));
	this->m_edgemax.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.edgemax));
	this->m_bgline.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.bgline));
	this->m_gfirst.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.gfirst));
	this->m_unit.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.unit));
	this->m_gain.GetWindowText(str, 100);
	sscanf(str, "%x", &(this->m_option.gain));
	this->m_filecount.GetWindowText(str, 100);
	sscanf(str, "%x", &(this->m_option.filecount));
	this->m_width.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.width));
	this->m_height.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.height));
	this->m_dpdthd.GetWindowText(str, 100);
	sscanf(str, "%x", &(this->m_option.dpdthd));
	this->m_edgesum.GetWindowText(str, 100);
	sscanf(str, "%x", &(this->m_option.edgesum));
	this->m_motionframecount.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.motionframecount));
	this->m_motionhthd.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.motionhthd));
	this->m_motionmthd.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.motionmthd));
	this->m_motiondetect.GetWindowText(str, 100);
	sscanf(str, "%d", &(this->m_option.motiondetect));
}

void CBlkIspDlg::OnBnClickedBtnIspProcess()
{
	CFileBin fpIn, fpOut;
	CBufferBase srcBuf, dstBuf;
	int width = 640, height = 480 ;
	int size;
	char temp[100];

	//this->GetEditValue();
	//this->GetCheckValue();
	this->GetUserOption();
	width = this->m_option.width;
	height = this->m_option.height;

	CIspModule326	ispModule(width, height, this->m_option.unit);
	ispModule.SetRawData(this->m_option.rawdata);
	ispModule.SetGain(this->m_option.gain);
	ispModule.SetIspCtrl(this->m_option.ispctrl);
	ispModule.SetDither(this->m_option.dither);
	ispModule.SetDpdthd(this->m_option.dpdthd);
	ispModule.SetDpdVersion(this->m_option.newdpd);
	ispModule.SetNewEdge(this->m_option.newedge);
	ispModule.SetOutputFile(this->m_option.outputfile);
	ispModule.SetCoefEdge(this->m_option.edgepara);
	ispModule.SetDivEdge(this->m_option.edgediv);
	ispModule.SetMaxEdge(this->m_option.edgemax);
	ispModule.SetAFH(this->m_option.afh);
	ispModule.SetAFV(this->m_option.afv);
	ispModule.SetAFW(this->m_option.afw);

//	memset(srcf, 0, 100);
	//this->m_SrcfCombo.GetWindowText(srcf, 100);
	fpIn.Open(this->m_option.src, "rb");
//	memset(dstf, 0 ,100);
//	strcpy(dstf, srcf);
//	strcat(dstf, ".new");
	fpOut.Open(this->m_option.dest, "wb");
	
	srcBuf.SetSize(width * height * 3 * 2, 1);
	dstBuf.SetSize(width * height * 3 * 2, 1);
	
	// read width*heightbytes raw data
	size = fpIn.GetFileLength();
	if(this->m_option.jpegheader)
	{
		size -= 0x2e;
		fpIn.Read(temp, 0x2c);
	}
	if ( size != (signed)( fpIn.Read(srcBuf.m_buf,size) ))
		return;		
	ispModule.SetBGLine(this->m_option.bgline);
	ispModule.SetGpixel(this->m_option.gfirst);
	size = ispModule.Process(srcBuf.m_buf, dstBuf.m_buf);

	fpOut.Write(dstBuf.m_buf, size);
	fpOut.Close();
	if(this->m_option.outputbmp)
	{
		char bmpf[200];
		CRaw2RgbF rgb2bmp;

		strcpy(bmpf, this->m_option.dest);
		if(this->m_option.ispctrl&COLORCONVERT)	//yuv422_yuyv->rgb24
		{
			CYuv422F yuv2rgb;
			
			strcat(bmpf, ".rgb");
			yuv2rgb.Yuyv2Rgb(this->m_option.dest, bmpf, width, height);
			strcpy(this->m_option.dest, bmpf);
		}
		
		strcat(bmpf, ".bmp");
		if(this->m_option.ispctrl & CFAEXTRACT)
			rgb2bmp.Rgb24_Bmp(this->m_option.dest, bmpf, width, height);
		else
			rgb2bmp.Rgb8_Bmp(this->m_option.dest, bmpf, width, height);
	}
	this->m_option.edgesum = ispModule.GetEdgeSum();
	sprintf(temp, "%x", this->m_option.edgesum);
	this->m_edgesum.SetWindowText(temp);
	
}

void CBlkIspDlg::OnBnClickedBtnSelAll()
{
	this->m_blacklevel.SetCheck(BST_CHECKED);
	this->m_lensfalloff.SetCheck(BST_CHECKED);
	this->m_dpddpc.SetCheck(BST_CHECKED);
	this->m_digitalgain.SetCheck(BST_CHECKED);
	this->m_cfa.SetCheck(BST_CHECKED);
	this->m_colorcorrect.SetCheck(BST_CHECKED);
	this->m_gammacorrect.SetCheck(BST_CHECKED);
	this->m_edgeenhance.SetCheck(BST_CHECKED);
	this->m_colorconvert.SetCheck(BST_CHECKED);

	this->m_colorcorrect.EnableWindow(TRUE);
	this->m_gammacorrect.EnableWindow(TRUE);
	this->m_colorconvert.EnableWindow(TRUE);
	this->m_edgeenhance.EnableWindow (TRUE);
	this->m_newedge.EnableWindow(TRUE);
}

int CBlkIspDlg::ParseFile(char *dirf, char *dstf)
{
	char srcf[100];
	char *p;
	int result, i;

	this->m_SrcfCombo.GetWindowText(srcf, 100);
	p = strrchr(srcf, '\\');
	if(p == NULL)
		return -1;
	result = (int)(p - srcf + 1);
	strncpy(dirf, srcf, result);
	dirf[result] = '\0';
	strcpy(dstf, srcf+result);
	p = strchr(dstf, '.');
	if(p == NULL)
		return -1;
	result = (int)(p - dstf);
	dstf[result] = '\0';
	sscanf(dstf, "%x", &i);
	
	return i;
}

#define _OUTPUT_ONE_DATA

void CBlkIspDlg::OnBnClickedBtnCmoduleTest()
{
	char dirf[100], strf[100], tmpf[100], dstf[100];
	CFileBin fpIn, fpOut;
	CBufferBase srcBuf, dstBuf;
	int width = 640, height = 480 ;
	int size, i, fCount;
//	CYuv422 yuv422;
//	CYuv420 yuv420;

	this->GetEditValue();
	this->GetCheckValue();
	width = this->m_option.width;
	height = this->m_option.height;

	CIspModule326	ispModule(width, height, this->m_option.unit);
	ispModule.SetRawData(this->m_option.rawdata);
	ispModule.SetGain(this->m_option.gain);
	ispModule.SetBGLine(this->m_option.bgline);
	ispModule.SetGpixel(this->m_option.gfirst);
	ispModule.SetIspCtrl(this->m_option.ispctrl);
	ispModule.SetDither(this->m_option.dither);
		
	memset(strf, 0, 100);
	memset(dstf, 0, 100);
	fCount = this->ParseFile(dirf, tmpf);
#ifdef _OUTPUT_ONE_DATA
	//fpOut.Open("yuvout.dat", "wb");
	fpOut.Open("rgb24out.dat", "wb");
#endif
	for(i = 0; i < this->m_option.filecount; i++, fCount++)
	{
		sprintf(tmpf, "%.8X", (int)fCount);
		strcpy(strf, dirf);
		strcat(strf, tmpf);
		strcat(strf, ".raw");
        fpIn.Open(strf, "rb");
#ifndef _OUTPUT_ONE_DATA
		strcpy(dstf, dirf);
		strcat(dstf, "sw_");
		strcat(dstf, tmpf);
		strcat(dstf, ".yuv");
		fpOut.Open(dstf, "wb");
#endif
	
		srcBuf.SetSize(width * height * 3 * 2, 1);
		dstBuf.SetSize(width * height * 3 * 2, 1);
	
		// read width*heightbytes raw data
		size = fpIn.GetFileLength();
		if(this->m_option.jpegheader)
			size -= 0x2e;
		if(size <= 0)
		{
			fpIn.Close();
#ifndef _OUTPUT_ONE_DATA
			fpOut.Close();
#endif
			memset(strf, 0, 100);
			memset(dstf, 0 ,100);
			continue;
		}
		if(this->m_option.jpegheader)
			fpIn.Read(strf, 0x2c);
		if ( size != (signed)( fpIn.Read(srcBuf.m_buf,size) ))
		{
			fpIn.Close();
#ifndef _OUTPUT_ONE_DATA
			fpOut.Close();
#endif
			memset(strf, 0, 100);
			memset(dstf, 0 ,100);
			continue;	
		}
		size = ispModule.Process(srcBuf.m_buf, dstBuf.m_buf);
#ifdef _OUTPUT_ONE_DATA
#if 0
		int j, pos
		//convert to yuv420frame
		memcpy(srcBuf.m_buf, dstBuf.m_buf, size);
		pos = 0;
		for(j = 0; j < size; j += 4)
			pos += yuv422.yuv422_444order(srcBuf.m_buf+j, dstBuf.m_buf+pos, YUV422_YUYV, 1);
		size = size*6/4;
		memcpy(srcBuf.m_buf, dstBuf.m_buf, size);
		pos = 0;
		for(j = 0; j < height; j+=2)
			pos += yuv420.Yuv444_Yuyv(srcBuf.m_buf+j*width*3, dstBuf.m_buf+pos, width, 1);
		size /= 2;
		memcpy(srcBuf.m_buf, dstBuf.m_buf, size);
		pos = 0;
		size = yuv420.MYuv420_FrmOrder(srcBuf.m_buf, dstBuf.m_buf, width, height, YUV420_YUYV, 1);
#endif
#endif
		fpOut.Write(dstBuf.m_buf, size);
		fpIn.Close();
#ifndef _OUTPUT_ONE_DATA
		fpOut.Close();
#endif
		memset(strf, 0, 100);
		memset(dstf, 0 ,100);
	}
#ifdef _OUTPUT_ONE_DATA
	fpOut.Close();
#endif
}

void CBlkIspDlg::OnBnClickedBtnCreateRawData()
{
	char dirf[100], dstf[100], srcf[100];
	CFileBin fpIn, fpOut;
	CBufferBase srcBuf, dstBuf;
	int fCount = 0x0, i, size;
	
	this->GetEditValue();
	this->GetCheckValue();
	this->m_SrcfCombo.GetWindowText(srcf, 100);
	this->ParseFile(dirf, dstf);

	fpIn.Open(srcf, "rb");
	size = fpIn.GetFileLength();
	srcBuf.SetSize(size, 1);
	if(!this->m_option.jpegheader)
	{
		dstBuf.SetSize(0x2e, 1);
		memset(dstBuf.m_buf, 0, 0x2e);
	}
	if ( size != (signed)( fpIn.Read(srcBuf.m_buf,size) ))
		return;
	memset(srcf, 0, 100);
	for(i = 0; i < this->m_option.filecount; i++, fCount++)
	{
		memset(dstf, 0, 100);
		sprintf(dstf, "%.8X", (int)fCount);
		strcpy(srcf, dirf);
		strcat(srcf, dstf);
		strcat(srcf, ".raw");
        fpOut.Open(srcf, "wb");
		if(!this->m_option.jpegheader)
			fpOut.Write(dstBuf.m_buf, 0x2c);
		fpOut.Write(srcBuf.m_buf, size);
		if(!this->m_option.jpegheader)
			fpOut.Write(dstBuf.m_buf, 0x2);
		fpOut.Close();
	}
	fpIn.Close();
}

void CBlkIspDlg::OnBnClickedBtnCompareY()
{
	char strf[100], dirf[100];
	CFileBin fp;
	CBufferBase srcBuf1, srcBuf2, dstBuf;
	int size;

	this->GetEditValue();
	this->GetCheckValue();
	this->ParseFile(dirf, strf);
	this->m_SrcfCombo.GetWindowText(strf, 100);	//yuv444 data
	fp.Open(strf, "rb");
	size = fp.GetFileLength();
	srcBuf1.SetSize(size, 1);
	if(size != (signed)(fp.Read(srcBuf1.m_buf, size)))
		return;
	fp.Close();
	this->m_DestfCombo.GetWindowText(strf, 100); //yuv422 data
	fp.Open(strf, "rb");
	size = fp.GetFileLength();
	srcBuf2.SetSize(size, 1);
	if(size != (signed)(fp.Read(srcBuf2.m_buf, size)))
		return;
	fp.Close();
	//compare
	size /= 2;
	dstBuf.SetSize(size, 1);
	this->CompareY(srcBuf1.m_buf, srcBuf2.m_buf, dstBuf.m_buf, size);

	strcat(dirf, "edge_Y.dat");
	fp.Open(dirf, "wb");
	fp.Write(dstBuf.m_buf, size);
	fp.Close();
}

void CBlkIspDlg::CompareY(char *src1, char *src2, char *dst, int size)
{
	int i;
	//char tmp;

	for(i = 0; i < size; i++)
	{
		dst[i] = src2[i*2] - src1[i*3];
		//dst[i] = (char)((tmp < 0) ? 0 : tmp);
	}
}

void CBlkIspDlg::Convert10To8(char *src, char *dst, int size)
{
	int i = 0, j = 0;
	
	for(; i < size; i+=2, j++)
		dst[j] = (char)(*((short*)(src+i))>>2);
}

void CBlkIspDlg::OnBnClickedBtn10to8()
{
	char dirf[100], dstf[100], srcf[100];
	CFileBin fpIn, fpOut;
	CBufferBase srcBuf, dstBuf;
	int fCount = 0x0, i, size;
	
	this->GetEditValue();
	this->GetCheckValue();
	this->m_SrcfCombo.GetWindowText(srcf, 100);
	fCount = this->ParseFile(dirf, dstf);

	for(i = 0; i < this->m_option.filecount; i++, fCount++)
	{
		memset(srcf, 0, 100);
		memset(dstf, 0, 100);
		sprintf(dstf, "%.8X", fCount);
		strcpy(srcf, dirf);
		strcat(srcf, dstf);
		strcat(srcf, ".raw");
		fpIn.Open(srcf, "rb");
		size = fpIn.GetFileLength();
		if(!size)
		{
			fpIn.Close();
			continue;
		}
		srcBuf.SetSize(size, 1);
		fpIn.Read(srcBuf.m_buf, size);
		fpIn.Close();
		if(this->m_option.jpegheader)
		{
			dstBuf.SetSize((size+0x2e)>>1, 1);
			memcpy(dstBuf.m_buf, srcBuf.m_buf, 0x2c);
			this->Convert10To8(srcBuf.m_buf+0x2c, dstBuf.m_buf+0x2c, size-0x2e);
			memset(dstBuf.m_buf+((size+0x2e-4)>>1), 0, 2);
			size = (size+0x2e)>>1;
		}
		else
		{
			dstBuf.SetSize(size>>1, 1);
			this->Convert10To8(srcBuf.m_buf, dstBuf.m_buf, size);
			size = size>>1;
		}
		fpOut.Open(srcf, "wb");
		fpOut.Write(dstBuf.m_buf, size);
		fpOut.Close();
	}
}


void CBlkIspDlg::OnBnClickedCheckColorconvert()
{
	if(this->m_colorconvert.GetCheck())
	{
		this->m_edgeenhance.EnableWindow(TRUE);
		this->m_newedge.EnableWindow(TRUE);
		if((this->m_edgeenhance.GetCheck() == BST_CHECKED) || (this->m_newedge.GetCheck() == BST_CHECKED))
			this->m_autofocus.EnableWindow(TRUE);
	}
	else
	{
		this->m_edgeenhance.EnableWindow(FALSE);
		this->m_newedge.EnableWindow(FALSE);
		this->m_autofocus.EnableWindow(FALSE);
	}
}

void CBlkIspDlg::OnBnClickedCheckCfa()
{
	if(this->m_cfa.GetCheck())
	{
		this->m_colorcorrect.EnableWindow(TRUE);
		this->m_gammacorrect.EnableWindow(TRUE);
		this->m_colorconvert.EnableWindow(TRUE);
		if(this->m_colorconvert.GetCheck())
		{
			this->m_edgeenhance.EnableWindow (TRUE);
			this->m_newedge.EnableWindow(TRUE);
			if((this->m_edgeenhance.GetCheck() == BST_CHECKED) || (this->m_newedge.GetCheck() == BST_CHECKED))
				this->m_autofocus.EnableWindow(TRUE);
		}
	}
	else
	{
		this->m_colorcorrect.EnableWindow(FALSE);
		this->m_gammacorrect.EnableWindow(FALSE);
		this->m_colorconvert.EnableWindow(FALSE);
		this->m_edgeenhance.EnableWindow(FALSE);
		this->m_newedge.EnableWindow(FALSE);
		this->m_autofocus.EnableWindow(FALSE);
	}
}

void CBlkIspDlg::OnBnClickedCheckEdgeenhance()
{
	if((this->m_edgeenhance.GetCheck() == BST_CHECKED) || (this->m_newedge.GetCheck() == BST_CHECKED))
		this->m_autofocus.EnableWindow(TRUE);
	else
		this->m_autofocus.EnableWindow(FALSE);
}

void CBlkIspDlg::OnBnClickedCheckNewEdge()
{
	if((this->m_edgeenhance.GetCheck() == BST_CHECKED) || (this->m_newedge.GetCheck() == BST_CHECKED))
		this->m_autofocus.EnableWindow(TRUE);
	else
		this->m_autofocus.EnableWindow(FALSE);
}

void CBlkIspDlg::OnBnClickedBtnMotion()
{
	char dirf[100], strf[100], tmpf[100], dstf[100];
	CFileBin fp;
	CBufferBase buf;
	int size, i, fCount;

	this->GetUserOption(); 
	
	C326Motion motion(this->m_option.width, this->m_option.height);
	motion.SetFrameCnt(this->m_option.motionframecount);
	motion.SetHThrld(this->m_option.motionhthd);
	motion.SetMThrld(this->m_option.motionmthd);
	buf.SetSize(this->m_option.width*this->m_option.height*2);	
	memset(strf, 0, 100);
	memset(dstf, 0, 100);
	fCount = this->ParseFile(dirf, tmpf);
	for(i = 0; i < this->m_option.motionframecount; i++, fCount++)
	{
		sprintf(tmpf, "%.2X", (int)fCount);
		strcpy(strf, dirf);
		strcat(strf, tmpf);
		strcat(strf, ".dat");
        fp.Open(strf, "rb");
		size = fp.GetFileLength();
		if(size <= 0)
		{
			fp.Close();
			memset(strf, 0, 100);
			memset(dstf, 0 ,100);
			continue;
		}
		if ( size != (signed)( fp.Read(buf.m_buf,size) ))
		{
			fp.Close();
			memset(strf, 0, 100);
			memset(dstf, 0 ,100);
			continue;	
		}
		this->m_option.motiondetect = motion.Process(buf.m_buf, NULL);
		fp.Close();
		memset(strf, 0, 100);
		memset(dstf, 0 ,100);
	}
	sprintf(strf, "%d", this->m_option.motiondetect);
	this->m_motiondetect.SetWindowText(strf);
}
