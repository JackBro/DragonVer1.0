// Tools4PanelDlg.cpp : 实现文件
//

#include "internal.h"

// CTools4PanelDlg 对话框

IMPLEMENT_DYNAMIC(CTools4PanelDlg, CBasePanelDlg)
CTools4PanelDlg::CTools4PanelDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CTools4PanelDlg::IDD, pParent)
{
}

CTools4PanelDlg::~CTools4PanelDlg()
{
}

void CTools4PanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WIDTH_EDIT, m_imgwidthbox);
	DDX_Control(pDX, IDC_HEIGHT_EDIT, m_imgheightbox);
}


BEGIN_MESSAGE_MAP(CTools4PanelDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_TOOLS4MISC_BUTTON, OnBnClickedTools4miscButton)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CTools4PanelDlg 消息处理程序
#define IDC_PICTURE_PIC		6000
#define IDC_IMAGEBOX_PIC	6010

BOOL CTools4PanelDlg::OnInitDialog()
{
	RECT rect;
	CBasePanelDlg::OnInitDialog();

	rect.left = 0;
	rect.top = 0;
	rect.right = 0;
	rect.bottom = 0;
	this->m_pic.Create(rect, this, IDC_PICTURE_PIC);
	this->m_pic.ShowWindow(SW_SHOW);

	this->m_imagebox.SetDiffFileName("c:/temp/alldiffpixels.txt");
	this->m_imagebox.SetScaleRatio(3);
	rect.left = 100;
	rect.top =80;
	rect.right = 740;
	rect.bottom = 560;
	this->m_imagebox.Create(rect, this, IDC_IMAGEBOX_PIC);

	this->InitUserOption();
	this->DispOption();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTools4PanelDlg::InitUserOption(void)
{
	memset(this->m_option.src, 0, DEFAULT_FILENAME_LEN);
	memset(this->m_option.dest, 0, DEFAULT_FILENAME_LEN);
	this->m_option.imgheight = 0;
	this->m_option.imgwidth = 0;
	//memset( &(this->m_option), 0, sizeof(TUserT4Option) );
}
	
void CTools4PanelDlg::GetUserOption(char *tail)
{
	this->GetEditBoxValue();
	CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, tail);
}
	
void CTools4PanelDlg::AutoSetSrcfile(void)
{
	CBasePanelDlg::AutoSetFileBox(this->m_option.dest);
}
	
void CTools4PanelDlg::DispOption(void)
{
	char str[20];

	sprintf(str, "%d", this->m_option.imgwidth);
	this->m_imgwidthbox.SetWindowText(str);
	sprintf(str, "%d", this->m_option.imgheight);
	this->m_imgheightbox.SetWindowText(str);
}

void CTools4PanelDlg::GetEditBoxValue(void)
{
	char str[20];
	int x;

	this->m_imgwidthbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.imgwidth = (UINT16)x;

	this->m_imgheightbox.GetWindowText(str, 20);
	sscanf(str, "%d", &x);
	this->m_option.imgheight = (UINT16)x;

}

#include <GL/gl.h>
#include <GL/glaux.h>

void CTools4PanelDlg::OnBnClickedTools4miscButton()
{
	CWaitCursor cursor;


}

void CTools4PanelDlg::OnBnClickedButton2()
{
	CWaitCursor cursor;
	int len;
	CFileBin fb;

	//CBufferBase pool1(0x200000);
	//CBufferBase pool2(0x200000);

	this->GetUserOption(".jpg");
	fb.Open(m_option.src);
	len = fb.GetFileLength();
	m_option.pool1.SetSize(len);
	fb.Read(m_option.pool1.m_buf, len);
	fb.Close();
	if(!len)
		return;

	fb.Open(m_option.dest);
	len = fb.GetFileLength();
	m_option.pool2.SetSize(len);
	fb.Read(m_option.pool2.m_buf, len);
	fb.Close();
	if(!len)
		return;

	m_imagebox.SetImgParm(m_option.imgwidth, m_option.imgheight);//, RGB24);
	//m_imagebox.ShowWindow(); 
	//imagebox.BringWindowToTop();
	m_imagebox.ShowImageDiff((UINT8 *) m_option.pool1.m_buf, (UINT8 *) m_option.pool2.m_buf, len);
}


