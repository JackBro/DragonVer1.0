// PortPropDlg.cpp : 实现文件
//

#include "internal.h"

// COutportPropDlg 对话框

IMPLEMENT_DYNAMIC(COutportPropDlg, CBasePortPropDlg)
COutportPropDlg::COutportPropDlg(CWnd* pParent /*=NULL*/)
	: CBasePortPropDlg(COutportPropDlg::IDD, pParent)
{
}

COutportPropDlg::~COutportPropDlg()
{
}

void COutportPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePortPropDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILESEL_COMBO, m_SrcfCombo);
	DDX_Control(pDX, IDC_SAVE_CHECK, m_saveBtn);
	DDX_Control(pDX, IDC_OPENFILE_BUTTON, m_openBtn);
	DDX_Control(pDX, IDC_OUTPORTDESC_STATIC, m_descStatic);
}

BEGIN_MESSAGE_MAP(COutportPropDlg, CBasePortPropDlg)
	ON_BN_CLICKED(IDC_OPENFILE_BUTTON, OnBnClickedOpenfileButton)
	ON_BN_CLICKED(IDC_SAVE_CHECK, OnBnClickedSaveCheck)
END_MESSAGE_MAP()


// COutportPropDlg 消息处理程序

void COutportPropDlg::InitUserOption(void)
{
	char str[256];
	CTypeProp tp;
	COuportProp *port = ((COuportProp *)this->m_port);

	ASSERT(this->m_port);
	if(port->IsSaveFlag())
		this->m_saveBtn.SetCheck(BST_CHECKED);
	else
		this->m_saveBtn.SetCheck(BST_UNCHECKED);
	this->OnBnClickedSaveCheck();
	this->m_SrcfCombo.SetWindowText(port->GetFilename());

	sprintf(str, "%s", tp.GetFormatDesc(port->GetFormat()));	
	sprintf(str, "%s\nWidth = %d\nHeight = %d\nUnit = %d", str,
		port->GetWidth(), port->GetHeight(), port->GetUnit());
	this->m_descStatic.SetWindowText(str);
}

BOOL COutportPropDlg::OnInitDialog()
{
	CBasePortPropDlg::OnInitDialog();

	this->InitUserOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void COutportPropDlg::GetUserOption(void)
{
	this->m_SrcfCombo.GetWindowText(this->m_option.srcf, DEFAULT_FILENAME_LEN);
	this->m_option.save = this->m_saveBtn.GetCheck();
}

void COutportPropDlg::OnBnClickedOpenfileButton()
{
	CString str;

	if(CBasePortPropDlg::OpenFileDlg(&str))
		this->m_SrcfCombo.addString(str.GetBuffer());
}

void COutportPropDlg::OnOK()
{
	COuportProp *port = ((COuportProp *)this->m_port);
	
	this->GetUserOption();
	port->EnableSave(this->m_option.save);
	port->SetFilename(this->m_option.srcf);
	CBasePortPropDlg::OnOK();
}

void COutportPropDlg::OnBnClickedSaveCheck()
{
	if(this->m_saveBtn.GetCheck() == BST_CHECKED)
	{
		this->m_SrcfCombo.EnableWindow();
		this->m_openBtn.EnableWindow();
	}
	else
	{
		this->m_SrcfCombo.EnableWindow(FALSE);
		this->m_openBtn.EnableWindow(FALSE);
	}
}
