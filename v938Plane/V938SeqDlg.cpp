// V568UsbcDlg.cpp : ʵ���ļ�
//

#include "internal.h"

#ifndef	V938CMD_DISABLE





// CV938CpmDlg �Ի���

IMPLEMENT_DYNAMIC(CV938SeqDlg, CBaseCtrlDlg)
CV938SeqDlg::CV938SeqDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV938SeqDlg::IDD, pParent)
{
}

CV938SeqDlg::~CV938SeqDlg()
{
}

void CV938SeqDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CV938SeqDlg, CBaseCtrlDlg)

END_MESSAGE_MAP()


// CV938CpmDlg ��Ϣ�������

void CV938SeqDlg::InitUserOption(void)
{
}

void CV938SeqDlg::DispUserOption(void)
{	
}

void CV938SeqDlg::GetUserOption(void)
{
}

#else

void null938SeqDlg(void)
{
}

#endif //V938CMD_DISABLE


