// .\CtrlPanelDlg\V568AudioAdc.cpp : 实现文件
//

#include "internal.h"


#ifndef	V578CMD_DISABLE

// CV568AudioAdcDlg 对话框

IMPLEMENT_DYNAMIC(CV568AudioAdcDlg, CBaseCtrlDlg)
CV568AudioAdcDlg::CV568AudioAdcDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568AudioAdcDlg::IDD, pParent)
{
}

CV568AudioAdcDlg::~CV568AudioAdcDlg()
{
}

void CV568AudioAdcDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CV568AudioAdcDlg, CBaseCtrlDlg)
END_MESSAGE_MAP()


// CV568AudioAdcDlg 消息处理程序

#endif //V578CMD_DISABLE
