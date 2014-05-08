		

#include "internal.h"

IMPLEMENT_DYNAMIC(CYuvMergePropDlg, CImageBoxPropDlg)
CYuvMergePropDlg::CYuvMergePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CYuvMergePropDlg::IDD, pParent)
{
}

CYuvMergePropDlg::~CYuvMergePropDlg(void)
{
}

void CYuvMergePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CYuvMergePropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()


// CComGatePropDlg 消息处理程序
void CYuvMergePropDlg::DispUserOption(void)
{
}

void CYuvMergePropDlg::GetUserOption(void)
{
}

void CYuvMergePropDlg::UpdateOption(void)
{
	this->GetUserOption();
	CImageBoxPropDlg::UpdateOption();
}

BOOL CYuvMergePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

