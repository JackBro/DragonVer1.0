

#include "internal.h"

IMPLEMENT_DYNAMIC(CHsizerBoxPropDlg, CImageBoxPropDlg)
CHsizerBoxPropDlg::CHsizerBoxPropDlg(CWnd* pParent /*=NULL*/)
: CImageBoxPropDlg(CHsizerBoxPropDlg::IDD, pParent)
{

}

CHsizerBoxPropDlg::~CHsizerBoxPropDlg()
{
}

void CHsizerBoxPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CHsizerBoxPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CHsizerBoxPropDlg::DispUserOption(void)
{
}

BOOL CHsizerBoxPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}