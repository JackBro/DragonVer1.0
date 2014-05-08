// V558TestFpgaDlg.cpp : 实现文件
//
#include "internal.h"

// CBasePortPropDlg 对话框


IMPLEMENT_DYNAMIC(CBasePortPropDlg, CBaseDlg)
CBasePortPropDlg::CBasePortPropDlg(UINT Idd, CWnd* pParent /*=NULL*/)
	: CBaseDlg(Idd, pParent)
{
	this->m_port = NULL;
}

CBasePortPropDlg::~CBasePortPropDlg()
{
}

void CBasePortPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBasePortPropDlg, CBaseDlg)
END_MESSAGE_MAP()

void CBasePortPropDlg::SetPort(CPortBase *port)
{
	this->m_port = port;
}

