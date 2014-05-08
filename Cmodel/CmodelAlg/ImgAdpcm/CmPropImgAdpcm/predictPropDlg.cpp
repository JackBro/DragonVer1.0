#include "internal.h"


IMPLEMENT_DYNAMIC(CPredictPropDlg, CImageBoxPropDlg)
CPredictPropDlg::CPredictPropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CPredictPropDlg::IDD, pParent)
{
	/*this->SetFont();*/
}

CPredictPropDlg::~CPredictPropDlg(void)
{
}

void CPredictPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_CLIP_EDIT, m_clipEdit);
	DDX_Control(pDX, IDC_ADD_UNIT_EDIT, m_unitEdit);
}


BEGIN_MESSAGE_MAP(CPredictPropDlg, CImageBoxPropDlg)
END_MESSAGE_MAP()

void CPredictPropDlg::DispUserOption(void)
{
	int val;
	char str[20];
	CAddGate *punit = (CAddGate *)this->m_pUnit;

	val = punit->GetClip();
	sprintf(str, "%d", val);
	this->m_clipEdit.SetWindowText(str);
	val = punit->GetUnit();
	sprintf(str, "%d", val);
	this->m_unitEdit.SetWindowText(str);
}

void CPredictPropDlg::GetUserOption(void)
{
	char str[20];
	CStringTools tools;
//	CAddGate *punit = (CAddGate *)this->m_pUnit;

	this->m_clipEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.clip);
	this->m_unitEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.unit);
}

void CPredictPropDlg::UpdateOption(void)
{
	CAddGate *punit = (CAddGate *)this->m_pUnit;

	this->GetUserOption();
	CImageBoxPropDlg::UpdateOption();
	punit->SetClip(this->m_option.clip);
	punit->SetUnit(this->m_option.unit);
}

BOOL CPredictPropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}

