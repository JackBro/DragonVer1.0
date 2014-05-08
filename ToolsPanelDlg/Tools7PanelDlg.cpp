// Tools4PanelDlg.cpp : 实现文件
//
#include "internal.h"


// CTools7PanelDlg 对话框

IMPLEMENT_DYNAMIC(CTools7PanelDlg, CBasePanelDlg)
CTools7PanelDlg::CTools7PanelDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CTools7PanelDlg::IDD, pParent)
{
}

CTools7PanelDlg::~CTools7PanelDlg()
{
}

void CTools7PanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTools7PanelDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_TOOLS7MISC_BUTTON, OnBnClickedTools7miscButton)
END_MESSAGE_MAP()


BOOL CTools7PanelDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();
	this->InitUserOption();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTools7PanelDlg::InitUserOption(void)
{
	memset( &(this->m_option), 0, sizeof(TUserT7Option) );
}
	
void CTools7PanelDlg::GetUserOption(char *tail)
{
	CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, tail);
}
	
void CTools7PanelDlg::AutoSetSrcfile(void)
{
	CBasePanelDlg::AutoSetFileBox(this->m_option.dest);
}
	
void CTools7PanelDlg::DispOption(void)
{
}

#include "toolchar/charfifo.h"

void CTools7PanelDlg::OnBnClickedTools7miscButton()
{
	CWaitCursor cursor;
	CCharFifo cf;
	CFileBin  fb;
	CBufferBase	pool;
	int len, len1;

	this->GetUserOption();
	fb.Open(this->m_option.src);
	len = fb.GetFileLength();
	pool.SetSize(len);
	cf.SetSize(len);
	fb.Read(pool.m_buf, len);
	cf.Write(pool.m_buf, len);
	fb.Open(this->m_option.dest, "wb");
	len1 = pool.GetSize();
	do {
		len = cf.Read(pool.m_buf, len1);
		pool.m_buf[len] = 0;
	//	fb.Write("new line ---\n", (int)strlen("new line ---\n"));
		sprintf(pool.m_buf, "%s\n", pool.m_buf);
		fb.Write(pool.m_buf, (int)strlen(pool.m_buf));
	} while(len);
}

