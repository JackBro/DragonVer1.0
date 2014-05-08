		

#include "internal.h"

IMPLEMENT_DYNAMIC(CBmpboxPropDlg, CImageBoxPropDlg)
CBmpboxPropDlg::CBmpboxPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CBmpboxPropDlg::IDD, pParent)
{
}

CBmpboxPropDlg::~CBmpboxPropDlg(void)
{
}

void CBmpboxPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBmpboxPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()


// CComGatePropDlg 消息处理程序
void CBmpboxPropDlg::DispUserOption(void)
{
}

BOOL CBmpboxPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

