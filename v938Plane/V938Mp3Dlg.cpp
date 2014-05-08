// V568UsbcDlg.cpp : 实现文件
//
#include "internal.h"

#ifndef	V938CMD_DISABLE


// CV938CpmDlg 对话框

IMPLEMENT_DYNAMIC(CV938Mp3Dlg, CBaseCtrlDlg)
CV938Mp3Dlg::CV938Mp3Dlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV938Mp3Dlg::IDD, pParent)
{
}

CV938Mp3Dlg::~CV938Mp3Dlg()
{
}

void CV938Mp3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CV938Mp3Dlg, CBaseCtrlDlg)

END_MESSAGE_MAP()


// CV938CpmDlg 消息处理程序

void CV938Mp3Dlg::InitUserOption(void)
{
}

void CV938Mp3Dlg::DispUserOption(void)
{	
}

void CV938Mp3Dlg::GetUserOption(void)
{
}

#else

void null938Mp3Dlg(void)
{
}

#endif //V938CMD_DISABLE


