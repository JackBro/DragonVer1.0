// V568UsbcDlg.cpp : ʵ���ļ�
//

#include "internal.h"


#ifndef	V938CMD_DISABLE

// CV938CpmDlg �Ի���

IMPLEMENT_DYNAMIC(CV938WsDlg, CBaseCtrlDlg)
CV938WsDlg::CV938WsDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV938WsDlg::IDD, pParent)
{
}

CV938WsDlg::~CV938WsDlg()
{
}

void CV938WsDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CV938WsDlg, CBaseCtrlDlg)

END_MESSAGE_MAP()


// CV938CpmDlg ��Ϣ�������

void CV938WsDlg::InitUserOption(void)
{
}

void CV938WsDlg::DispUserOption(void)
{	
}

void CV938WsDlg::GetUserOption(void)
{
}

#else

void null938WsDlg(void)
{
}

#endif //V938CMD_DISABLE


