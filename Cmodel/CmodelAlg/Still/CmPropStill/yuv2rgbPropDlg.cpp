		

#include "internal.h"

IMPLEMENT_DYNAMIC(CYuv2RgbPropDlg, CImageBoxPropDlg)
CYuv2RgbPropDlg::CYuv2RgbPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CYuv2RgbPropDlg::IDD, pParent)
{
}

CYuv2RgbPropDlg::~CYuv2RgbPropDlg(void)
{
}

void CYuv2RgbPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CYuv2RgbPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()


// CComGatePropDlg 消息处理程序
void CYuv2RgbPropDlg::DispUserOption(void)
{
}

BOOL CYuv2RgbPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

