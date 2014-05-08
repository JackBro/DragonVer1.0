// OldCmodelDlg.cpp : ʵ���ļ�
//

#include "internal.h"



// COldCmodelDlg �Ի���

IMPLEMENT_DYNAMIC(COldCmodelDlg, CBasePanelDlg)
COldCmodelDlg::COldCmodelDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(COldCmodelDlg::IDD, pParent)
{
}

COldCmodelDlg::~COldCmodelDlg()
{
}

void COldCmodelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SRCFILE_COMBO, m_cfgfilebox);
}


BEGIN_MESSAGE_MAP(COldCmodelDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_RUNCMODEL_BUTTON, OnBnClickedRuncmodelButton)
	ON_BN_CLICKED(IDC_OPENSRCFILE_BTN, OnBnClickedOpensrcfileBtn)
END_MESSAGE_MAP()


BOOL COldCmodelDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	this->InitUserOption();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void COldCmodelDlg::InitUserOption(void)
{
	memset(&(this->m_option), 0, sizeof(TOldCmodelOption));
}

void COldCmodelDlg::OnBnClickedOpensrcfileBtn()
{
	CString str;

	if(this->OpenFileDlg(&str))
		this->m_cfgfilebox.addString(str.GetBuffer());
}

void COldCmodelDlg::GetUserOption(void)
{
	this->m_cfgfilebox.GetWindowText(this->m_option.cfgfile, OLDCMODEL_FILENAME_LENGTH);
}

void COldCmodelDlg::OnBnClickedRuncmodelButton()
{
	CWaitCursor cursor;
//	char *argv[2];

	this->GetUserOption();
	if(this->m_option.cfgfile[0] == '\0')
		return;
//	argv[1] = this->m_option.cfgfile;
//	main1(2, argv);
}

