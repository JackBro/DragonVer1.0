// BlkLbufDlg.cpp : 实现文件
//

#include "internal.h"
#include "..\include\cmodelpaneldlg\blklbufdlg.h"



// CBlkLbufDlg 对话框

IMPLEMENT_DYNAMIC(CBlkLbufDlg, CBasePanelDlg)
CBlkLbufDlg::CBlkLbufDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CBlkLbufDlg::IDD, pParent)
{
}

CBlkLbufDlg::~CBlkLbufDlg()
{
}

void CBlkLbufDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LBUFWIDTH_EDIT, m_widthbox);
	DDX_Control(pDX, IDC_LBUFHEIGHT_EDIT, m_heightbox);
}


BEGIN_MESSAGE_MAP(CBlkLbufDlg, CBasePanelDlg)

	ON_BN_CLICKED(IDC_LBUFENC_BUTTON, OnBnClickedLbufencButton)
	ON_BN_CLICKED(IDC_LBUFDEC_BUTTON, OnBnClickedLbufdecButton)
END_MESSAGE_MAP()

BOOL CBlkLbufDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	this->InitUserOption();
	this->DispOption();

	return TRUE;
}

void CBlkLbufDlg::InitUserOption(void)
{
	memset(&this->m_option, 0, sizeof(TLbufOption));
	this->m_option.width = 640;
	this->m_option.height = 480;	
}

void CBlkLbufDlg::DispOption(void)
{
	char str[20];

	sprintf(str, "%d", this->m_option.width);
	this->m_widthbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.height);
	this->m_heightbox.SetWindowText(str);
}

void CBlkLbufDlg::GetUserOption(char *tail)
{
	char str[20];

	this->m_widthbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.width));
	this->m_heightbox.GetWindowText(str, 20);
	sscanf(str, "%d", &(this->m_option.height));

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CBlkLbufDlg::decbin2txt(char *filename, char *buf, int len)
{
	CFileBin fb;
	int i;

	fb.Open(filename, "wt");
	for (i = 0; i < len; i += 2)
	{
		fb.Printf("%02x%02x\n", buf[i] & 0xff, buf[i + 1] & 0xff);
		//fb.Printf("%02x\n", buf[i +1] & 0xff);
	}
}

void CBlkLbufDlg::encbin2txt(char *filename, char *buf, int len)
{
	CFileBin fb;
	int i;

	fb.Open(filename, "wt");
	for (i = 0; i < len; i++)
	{
		fb.Printf("%02x\n", buf[i] & 0xff);		
	}
}

void CBlkLbufDlg::OnBnClickedLbufdecButton()
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int len, val, index = 0;
	CYuv422F tools;
	char *tail = {"lbuf"};

	this->GetUserOption(tail);
	fsrc.Open(this->m_option.srcfile);
	len = fsrc.GetFileLength();
	if (!len)
		return;
	
	srcbuf.SetSize(len);
	fsrc.Read(srcbuf.m_buf, len);

	while (index < len)
	{
		val = srcbuf.m_buf[index] + 128;
		srcbuf.m_buf[index] = (char)val;
		index++;
	}	
	fdst.Open(this->m_option.dstfile, "wb");
	fdst.Write(srcbuf.m_buf, len);
	fdst.Close();
	this->AutoSetFileBox(this->m_option.dstfile);

	this->GetUserOption();
	tools.Blk2Yuyv(this->m_option.srcfile, this->m_option.dstfile, this->m_option.width, this->m_option.height);
	this->AutoSetFileBox(this->m_option.dstfile);

	fdst.Open(this->m_option.dstfile);
	fdst.Read(srcbuf.m_buf, len);
	this->decbin2txt(this->m_option.srcfile, srcbuf.m_buf, len);
}

void CBlkLbufDlg::OnBnClickedLbufencButton()
{
	CFileBin fsrc, fdst;
	CBufferBase srcbuf, dstbuf;
	int len, val, index = 0;
	CYuv422F tools;
	char *tail = {"lbuf"};

	this->GetUserOption(tail);
	fsrc.Open(this->m_option.srcfile);
	len = fsrc.GetFileLength();
	if (!len)
		return;

	srcbuf.SetSize(len);
	fsrc.Read(srcbuf.m_buf, len);

	while (index < len)
	{
		val = srcbuf.m_buf[index] - 128;
		srcbuf.m_buf[index] = (char)val;
		index++;
	}
	fdst.Open(this->m_option.dstfile, "wb");
	fdst.Write(srcbuf.m_buf, len);
	fdst.Close();
	this->AutoSetFileBox(this->m_option.dstfile);

	this->GetUserOption();
	tools.Yuyv2Blk(this->m_option.srcfile, this->m_option.dstfile, this->m_option.width, this->m_option.height);
	this->AutoSetFileBox(this->m_option.dstfile);

	fdst.Open(this->m_option.dstfile);
	fdst.Read(srcbuf.m_buf, len);
	this->encbin2txt(this->m_option.srcfile, srcbuf.m_buf, len);
}
