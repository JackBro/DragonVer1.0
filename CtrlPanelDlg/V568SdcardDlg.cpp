// V568SdcardDlg.cpp : ʵ���ļ�
//

#include "internal.h"


#ifndef	DEVICECMD_DISABLE

// CV568SdcardDlg �Ի���

IMPLEMENT_DYNAMIC(CV568SdcardDlg, CBaseCtrlDlg)
CV568SdcardDlg::CV568SdcardDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568SdcardDlg::IDD, pParent)
{
}

CV568SdcardDlg::~CV568SdcardDlg()
{
}

void CV568SdcardDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CV568SdcardDlg, CBaseCtrlDlg)
END_MESSAGE_MAP()


// CV568SdcardDlg ��Ϣ�������
#else
void nullSdcardDlg(void)
{
}

#endif //DEVICECMD_DISABLE
