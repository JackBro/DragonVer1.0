#include "internal.h"


IMPLEMENT_DYNAMIC(CSubGatePropDlg, CImageBoxPropDlg)
CSubGatePropDlg::CSubGatePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CSubGatePropDlg::IDD, pParent)
{
	/*this->SetFont();*/
}

CSubGatePropDlg::~CSubGatePropDlg(void)
{
}

void CSubGatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSubGatePropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CSubGatePropDlg::DispUserOption(void)
{

}

BOOL CSubGatePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

