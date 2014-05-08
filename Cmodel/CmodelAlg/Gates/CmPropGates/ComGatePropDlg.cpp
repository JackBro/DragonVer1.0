#include "internal.h"

IMPLEMENT_DYNAMIC(CComGatePropDlg, CImageBoxPropDlg)
CComGatePropDlg::CComGatePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CComGatePropDlg::IDD, pParent)
{
}

CComGatePropDlg::~CComGatePropDlg()
{
}

void CComGatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CComGatePropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CComGatePropDlg::DispUserOption(void)
{
}

BOOL CComGatePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

