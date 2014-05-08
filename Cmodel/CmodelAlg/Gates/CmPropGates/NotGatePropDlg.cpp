#include "internal.h"

IMPLEMENT_DYNAMIC(CNotGatePropDlg, CImageBoxPropDlg)
CNotGatePropDlg::CNotGatePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CNotGatePropDlg::IDD, pParent)
{
}

CNotGatePropDlg::~CNotGatePropDlg()
{
}

void CNotGatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNotGatePropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CNotGatePropDlg::DispUserOption(void)
{
}

BOOL CNotGatePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

