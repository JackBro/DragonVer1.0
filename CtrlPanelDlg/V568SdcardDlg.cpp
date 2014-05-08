// V568SdcardDlg.cpp : 实现文件
//

#include "internal.h"


#ifndef	DEVICECMD_DISABLE

// CV568SdcardDlg 对话框

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


// CV568SdcardDlg 消息处理程序
#else
void nullSdcardDlg(void)
{
}

#endif //DEVICECMD_DISABLE
