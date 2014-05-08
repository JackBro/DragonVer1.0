
#include "internal.h"

IMPLEMENT_DYNAMIC(COrGatePropDlg, CImageBoxPropDlg)
COrGatePropDlg::COrGatePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(COrGatePropDlg::IDD, pParent)
{
}

COrGatePropDlg::~COrGatePropDlg(void)
{
}

void COrGatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COrGatePropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void COrGatePropDlg::DispUserOption(void)
{
}

BOOL COrGatePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

