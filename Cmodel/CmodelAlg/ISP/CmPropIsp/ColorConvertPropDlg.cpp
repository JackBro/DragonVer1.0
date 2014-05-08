#include "internal.h"


IMPLEMENT_DYNAMIC(CColorConvertPropDlg, CImageBoxPropDlg)
CColorConvertPropDlg::CColorConvertPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CColorConvertPropDlg::IDD, pParent)
{
}

CColorConvertPropDlg::~CColorConvertPropDlg()
{
}

void CColorConvertPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CColorConvertPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CColorConvertPropDlg::DispUserOption(void)
{
}

BOOL CColorConvertPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

