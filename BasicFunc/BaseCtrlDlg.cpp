// V558TestFpgaDlg.cpp : ʵ���ļ�
//
#include "internal.h"

// CBaseCtrlDlg �Ի���

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



