#include "internal.h"
//#include "..\..\..\..\include\cmpropgates\addgatepropdlg.h"


IMPLEMENT_DYNAMIC(CAddGatePropDlg, CImageBoxPropDlg)
CAddGatePropDlg::CAddGatePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CAddGatePropDlg::IDD, pParent)
{
	/*this->SetFont();*/
}

CAddGatePropDlg::~CAddGatePropDlg(void)
{
}

void CAddGatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_CLIP_EDIT, m_clipEdit);
	DDX_Control(pDX, IDC_ADD_UNIT_EDIT, m_unitEdit);
}


BEGIN_MESSAGE_MAP(CAddGatePropDlg, CImageBoxPropDlg)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void CAddGatePropDlg::DispUserOption(void)
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

void CAddGatePropDlg::GetUserOption(void)
{
	char str[20];
	CStringTools tools;
//	CAddGate *punit = (CAddGate *)this->m_pUnit;

	this->m_clipEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.clip);
	this->m_unitEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.unit);
}

void CAddGatePropDlg::UpdateOption(void)
{
	CAddGate *punit = (CAddGate *)this->m_pUnit;

	this->GetUserOption();
	CImageBoxPropDlg::UpdateOption();
	punit->SetClip(this->m_option.clip);
	punit->SetUnit(this->m_option.unit);
}

BOOL CAddGatePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->DispUserOption();

	return TRUE;
}


void CAddGatePropDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
}
