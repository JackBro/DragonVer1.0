// V558TestFpgaDlg.cpp : 实现文件
//
#include "internal.h"

// CBaseCtrlDlg 对话框

IMPLEMENT_DYNAMIC(CBaseCtrlDlg, CBasePanelDlg)
CBaseCtrlDlg::CBaseCtrlDlg(UINT Idd, CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(Idd, pParent)
{
}

CBaseCtrlDlg::~CBaseCtrlDlg()
{
}

void CBaseCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBaseCtrlDlg, CBasePanelDlg)
END_MESSAGE_MAP()



