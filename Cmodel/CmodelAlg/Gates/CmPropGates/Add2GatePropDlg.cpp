#include "internal.h"
//#include "..\..\..\..\include\cmpropgates\add2gatepropdlg.h"

#define MaxOutUnit 4

IMPLEMENT_DYNAMIC(CAdd2GatePropDlg, CImageBoxPropDlg)
CAdd2GatePropDlg::CAdd2GatePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CAdd2GatePropDlg::IDD, pParent)
{
	/*this->SetFont();*/
}

CAdd2GatePropDlg::~CAdd2GatePropDlg(void)
{
}

void CAdd2GatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD2_CLIP_EDIT, m_clipEdit);
	DDX_Control(pDX, IDC_ADD2_OPDATA_EDIT, m_opdataEdit);
	DDX_Control(pDX, IDC_ADD2_INUNIT_COMBO, m_InUnitBox);
	DDX_Control(pDX, IDC_ADD2_OUTUNIT_COMBO, m_OutUnitBox);
}


BEGIN_MESSAGE_MAP(CAdd2GatePropDlg, CImageBoxPropDlg)
	//ON_EN_CHANGE(IDC_ADD2_UNIT_EDIT, OnEnChangeAddUnitEdit)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_CHANGE(IDC_ADD2_CLIP_EDIT, OnEnChangeAddClipEdit)
	ON_CBN_SELCHANGE(IDC_ADD2_INUNIT_COMBO, OnCbnSelchangeInunitCombo)
	ON_CBN_SELCHANGE(IDC_ADD2_INUNIT_COMBO, OnCbnSelchangeAdd2InunitCombo)
END_MESSAGE_MAP()

void CAdd2GatePropDlg::InitUserOption(void)
{
	int i;
	char str[20];

	for(i = 0; i < MaxOutUnit; i++)
	{	
		sprintf(str, "%d", i+1);
		this->m_InUnitBox.InsertString(i, str);
		this->m_OutUnitBox.InsertString(i, str);
	}
}

void CAdd2GatePropDlg::DispUserOption(void)
{
	int val;
	char str[20];
	CAdd2Gate *punit = (CAdd2Gate *) (this->m_pUnit);

	val = punit->GetClip();
	sprintf(str, "%d", val);
	this->m_clipEdit.SetWindowText(str);
	val = punit->GetOpdata();
	sprintf(str, "%d", val);
	this->m_opdataEdit.SetWindowText(str);
	val = punit->GetInUnit();
	sprintf(str, "%d", val);
	this->m_InUnitBox.SetWindowText(str);
	val = punit->GetOutUnit();
	sprintf(str, "%d", val);
	this->m_OutUnitBox.SetWindowText(str);
}

void CAdd2GatePropDlg::GetUserOption(void)
{
	char str[20];
	CStringTools tools;
//	CAdd2Gate *punit = (CAdd2Gate *)this->m_pUnit;

	this->m_clipEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.clip);
	this->m_opdataEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.opdata);

	this->m_InUnitBox.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.InUnit);
	this->m_OutUnitBox.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.OutUnit);
}


void CAdd2GatePropDlg::UpdateOption(void)
{
	CAdd2Gate *punit = (CAdd2Gate *)this->m_pUnit;

	this->GetUserOption();
	CImageBoxPropDlg::UpdateOption();
	punit->SetClip(this->m_option.clip);
	punit->SetInUnit(this->m_option.InUnit);
	punit->SetOutUnit(this->m_option.OutUnit);
	punit->SetOpdata(this->m_option.opdata);
}

BOOL CAdd2GatePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->InitUserOption();
	this->DispUserOption();

	return TRUE;
}

void CAdd2GatePropDlg::OnBnClickedOk()
{
	this->UpdateOption();
}

void CAdd2GatePropDlg::OnEnChangeAddClipEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CImageBoxPropDlg::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CAdd2GatePropDlg::OnCbnSelchangeInunitCombo()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CAdd2GatePropDlg::OnCbnSelchangeAdd2InunitCombo()
{
	// TODO: 在此添加控件通知处理程序代码
}
