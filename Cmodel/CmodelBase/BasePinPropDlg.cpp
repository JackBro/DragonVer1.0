// V558TestFpgaDlg.cpp : ʵ���ļ�
//
#include "internal.h"

// CBasePortPropDlg �Ի���


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

