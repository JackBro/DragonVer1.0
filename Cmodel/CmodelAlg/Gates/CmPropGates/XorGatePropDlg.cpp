#include "internal.h"


IMPLEMENT_DYNAMIC(CXorGatePropDlg, CImageBoxPropDlg)
CXorGatePropDlg::CXorGatePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CXorGatePropDlg::IDD, pParent)
{
	/*this->SetFont();*/
}

CXorGatePropDlg::~CXorGatePropDlg(void)
{
}

void CXorGatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CXorGatePropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CXorGatePropDlg::DispUserOption(void)
{

}

BOOL CXorGatePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

