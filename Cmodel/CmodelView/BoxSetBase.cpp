// BoxSetBase.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "../../toolkit/toolchar/stringtools.h"
#include "internal.h"

// CBoxSetBase 对话框

IMPLEMENT_DYNAMIC(CBoxSetBase, CDialog)
CBoxSetBase::CBoxSetBase(CWnd* pParent /*=NULL*/)
	: CDialog(CBoxSetBase::IDD, pParent)
{
	this->m_pbox = NULL;
}

CBoxSetBase::~CBoxSetBase()
{
}

void CBoxSetBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPINCNT_EDIT, m_InpcntEdit);
	DDX_Control(pDX, IDC_OUPINCNT_EDIT, m_OupcntEdit);
}


BEGIN_MESSAGE_MAP(CBoxSetBase, CDialog)
END_MESSAGE_MAP()

//	ON_EN_CHANGE(IDC_INPINCNT_EDIT, OnEnChangeDgainInpincntEdit)

// CBoxSetBase 消息处理程序

void CBoxSetBase::SetBoxCwnd(CWnd *pbox)
{
	this->m_pbox = pbox;
}

void CBoxSetBase::DispOption(void)
{
	char str[20];

	sprintf(str, "%d", this->m_option->GetInPinCount());
	this->m_InpcntEdit.SetWindowText(str);
	sprintf(str, "%d", this->m_option->GetOuPinCount());
	this->m_OupcntEdit.SetWindowText(str);
}

BOOL CBoxSetBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}
void CBoxSetBase::OnOK()
{
	char str[20];
	CStringTools tools;
	int i = 0;

	this->m_InpcntEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &i);
	this->m_option->SetInPinCount(i);
	this->m_OupcntEdit.GetWindowText(str, 20);
	tools.Str2Dec(str, &i);
	this->m_option->SetOuPinCount(i);

	this->m_pbox->SendMessage(WM_SETDLG_ONOK);

	CDialog::OnOK();
}
