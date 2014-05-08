#include "internal.h"


IMPLEMENT_DYNAMIC(CAndGatePropDlg, CImageBoxPropDlg)
CAndGatePropDlg::CAndGatePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CAndGatePropDlg::IDD, pParent)
{
	/*this->SetFont();*/
}

CAndGatePropDlg::~CAndGatePropDlg(void)
{
}

void CAndGatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAndGatePropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CAndGatePropDlg::DispUserOption(void)
{

}

BOOL CAndGatePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

