#include "internal.h"
//#include "..\..\..\..\include\cmpropgates\add2gatepropdlg.h"

#define MaxOutUnit 4

IMPLEMENT_DYNAMIC(CArithmGatePropDlg, CImageBoxPropDlg)
CArithmGatePropDlg::CArithmGatePropDlg(CWnd* pParent /*=NULL*/)
	: CImageBoxPropDlg(CArithmGatePropDlg::IDD, pParent)
{
	/*this->SetFont();*/
}

CArithmGatePropDlg::~CArithmGatePropDlg(void)
{
}

void CArithmGatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CImageBoxPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPCLIP_EDIT, m_UpClipEdit);
	DDX_Control(pDX, IDC_LOWCLIP_EDIT, m_LowClipEdit);
	DDX_Control(pDX, IDC_OPDATA_EDIT, m_opdataEdit);
	DDX_Control(pDX, IDC_INUNIT_COMBO, m_InUnitBox);
	DDX_Control(pDX, IDC_OUTUNIT_COMBO, m_OutUnitBox);
	DDX_Control(pDX, IDC_OPERATION_COMBO, m_operationBox);
}


BEGIN_MESSAGE_MAP(CArithmGatePropDlg, CImageBoxPropDlg)
	//ON_EN_CHANGE(IDC_ADD2_UNIT_EDIT, OnEnChangeAddUnitEdit)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)

END_MESSAGE_MAP()

void CArithmGatePropDlg::InitUserOption(void)
{
	int i;
	char str[20];
	const char *operationstr[] = {"Add","Sub", "Mul", "Div"};

	for(i = 0; i < MaxOutUnit; i++)
	{	
		sprintf(str, "%d", i+1);
		this->m_InUnitBox.InsertString(i, str);
		this->m_OutUnitBox.InsertString(i, str);
	}

	for(i = 0; i< sizeof(operationstr) / sizeof(char *); i++)
		this->m_operationBox.InsertString(i, operationstr[i]);
}

void CArithmGatePropDlg::DispUserOption(void)
{
	int val;
	char str[20];
	CArithmGate *punit = (CArithmGate *) (this->m_pUnit);

	val = punit->GetUpClip();
	sprintf(str, "%d", val);
	this->m_UpClipEdit.SetWindowText(str);
	val = punit->GetLowClip();
	sprintf(str, "%d", val);
	this->m_LowClipEdit.SetWindowText(str);
	val = punit->GetOpdata();
	sprintf(str, "%d", val);
	this->m_opdataEdit.SetWindowText(str);
	val = punit->GetInUnit();
	sprintf(str, "%d", val);
	this->m_InUnitBox.SetWindowText(str);
	val = punit->GetOutUnit();
	sprintf(str, "%d", val);
	this->m_OutUnitBox.SetWindowText(str);
	
	this->m_operationBox.SetCurSel(punit->GetOperator());
}

void CArithmGatePropDlg::GetUserOption(void)
{
	char str[20];
	CStringTools tools;
//	CArithmGate *punit = (CArithmGate *)this->m_pUnit;

	this->m_UpClipEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.UpClip);
	this->m_LowClipEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.LowClip);
	this->m_opdataEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.opdata);

	this->m_InUnitBox.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.InUnit);
	this->m_OutUnitBox.GetWindowText(str, 20);
	tools.Str2Dec(str, &this->m_option.OutUnit);

	this->m_option.operation = this->m_operationBox.GetCurSel();
}


void CArithmGatePropDlg::UpdateOption(void)
{
	CArithmGate *punit = (CArithmGate *)this->m_pUnit;

	this->GetUserOption();
	CImageBoxPropDlg::UpdateOption();
	punit->SetUpClip(this->m_option.UpClip);
	punit->SetLowClip(this->m_option.LowClip);
	punit->SetInUnit(this->m_option.InUnit);
	punit->SetOutUnit(this->m_option.OutUnit);
	punit->SetOpdata(this->m_option.opdata);
	punit->SetOperator(this->m_option.operation);
}

BOOL CArithmGatePropDlg::OnInitDialog()
{
	CImageBoxPropDlg::OnInitDialog();

	this->InitUserOption();
	this->DispUserOption();

	return TRUE;
}

void CArithmGatePropDlg::OnBnClickedOk()
{
	this->UpdateOption();
}


