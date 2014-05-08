// PortPropDlg.cpp : 实现文件
//

#include "internal.h"

// CPinPropDlg 对话框

IMPLEMENT_DYNAMIC(CPinPropDlg, CBaseDlg)
CPinPropDlg::CPinPropDlg(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CPinPropDlg::IDD, pParent)
{
	this->m_pin = NULL;
}

CPinPropDlg::~CPinPropDlg()
{
}

void CPinPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PINDESC_STATIC, m_descStatic);
}

BEGIN_MESSAGE_MAP(CPinPropDlg, CBaseDlg)
END_MESSAGE_MAP()


// CPinPropDlg 消息处理程序
void CPinPropDlg::SetPin(CPinBase *pin)
{
	this->m_pin = pin;
}

void CPinPropDlg::InitUserOption(void)
{
	char str[256];
	CTypeProp tp;

	ASSERT(this->m_pin);
	sprintf(str, "%s", tp.GetFormatDesc(this->m_pin->GetFormat()));	
	sprintf(str, "%s\nWidth = %d\nHeight = %d\nUnit = %d", str,
		this->m_pin->GetWidth(), this->m_pin->GetHeight(), this->m_pin->GetUnit());
	this->m_descStatic.SetWindowText(str);
}

BOOL CPinPropDlg::OnInitDialog()
{
	CBaseDlg::OnInitDialog();

	this->InitUserOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

