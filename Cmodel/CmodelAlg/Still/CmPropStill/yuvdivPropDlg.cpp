		

#include "internal.h"

IMPLEMENT_DYNAMIC(CYuvDivPropDlg, CImageBoxPropDlg)
CYuvDivPropDlg::CYuvDivPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CYuvDivPropDlg::IDD, pParent)
{
}

CYuvDivPropDlg::~CYuvDivPropDlg(void)
{
}

void CYuvDivPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CYuvDivPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()


// CComGatePropDlg 消息处理程序
void CYuvDivPropDlg::DispUserOption(void)
{
}

void CYuvDivPropDlg::GetUserOption(void)
{
}

void CYuvDivPropDlg::UpdateOption(void)
{
	this->GetUserOption();
	CImageBoxPropDlg::UpdateOption();
}

BOOL CYuvDivPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

