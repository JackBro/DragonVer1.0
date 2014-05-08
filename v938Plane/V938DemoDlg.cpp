// V568UsbcDlg.cpp : 实现文件
//
#include "internal.h"


#ifndef	V938CMD_DISABLE

// CV938CpmDlg 对话框

IMPLEMENT_DYNAMIC(CV938DemoDlg, CBaseCtrlDlg)
CV938DemoDlg::CV938DemoDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV938DemoDlg::IDD, pParent)
{
}

CV938DemoDlg::~CV938DemoDlg()
{
}

void CV938DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CV938DemoDlg, CBaseCtrlDlg)

	ON_BN_CLICKED(IDC_OPEN938_BUTTON, OnBnClickedOpen938Button)
END_MESSAGE_MAP()


// CV938CpmDlg 消息处理程序

void CV938DemoDlg::InitUserOption(void)
{
}

void CV938DemoDlg::DispUserOption(void)
{	
}

void CV938DemoDlg::GetUserOption(char *tail)
{
	this->m_SrcfCombo.GetWindowText(this->m_option.srcfile, DEFAULT_FILENAME_LEN);

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

void CV938DemoDlg::OnBnClickedOpen938Button()
{
    CWaitCursor cursor;
	CFileBin fb;
	CBufferBase pool;
	int size;

	this->GetUserOption();
	fb.Open(this->m_option.srcfile);
	size = fb.GetFileLength();
	if(size == 0)
		return;
	pool.SetSize(size);
	fb.Read(pool.m_buf, size);
	WriteBuf(0,pool.m_buf,size);
}

#else

void null938DemoDlg(void)
{
}

#endif //V938CMD_DISABLE


