#include "internal.h"

IMPLEMENT_DYNAMIC(CDelayGatePropDlg, CImageBoxPropDlg)
CDelayGatePropDlg::CDelayGatePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CDelayGatePropDlg::IDD, pParent)
{
}

CDelayGatePropDlg::~CDelayGatePropDlg(void)
{
}

void CDelayGatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DELAYTIME_EDIT, m_delaytimeEdit);
}


BEGIN_MESSAGE_MAP(CDelayGatePropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()


void CDelayGatePropDlg::GetUserOption(void)
{
	char str[20];
	CStringTools tools;

	this->m_delaytimeEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.delay);
}

void CDelayGatePropDlg::DispUserOption(void)
{
	CDelayGate *punit = (CDelayGate *)this->m_pUnit;
	char str[20];

	sprintf(str, "%d", punit->GetDelay());
	this->m_delaytimeEdit.SetWindowText(str);
}

BOOL CDelayGatePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

void CDelayGatePropDlg::UpdateOption(void)
{
	CDelayGate *punit = (CDelayGate *)this->m_pUnit;

	this->GetUserOption();
	CImageBoxPropDlg::UpdateOption();
	punit->SetDelay(this->m_option.delay);
}
