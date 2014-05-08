		

#include "internal.h"

IMPLEMENT_DYNAMIC(CRgb2YuvPropDlg, CImageBoxPropDlg)
CRgb2YuvPropDlg::CRgb2YuvPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CRgb2YuvPropDlg::IDD, pParent)
{
}

CRgb2YuvPropDlg::~CRgb2YuvPropDlg(void)
{
}

void CRgb2YuvPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRgb2YuvPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()


// CComGatePropDlg 消息处理程序
void CRgb2YuvPropDlg::DispUserOption(void)
{
}

BOOL CRgb2YuvPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

