// V568PhoneDlg.cpp : 实现文件
//
#include "internal.h"


#ifndef	V578CMD_DISABLE




// CV568PhoneDlg 对话框

IMPLEMENT_DYNAMIC(CV568PhoneDlg, CBaseCtrlDlg)
CV568PhoneDlg::CV568PhoneDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568PhoneDlg::IDD, pParent)
{
}

CV568PhoneDlg::~CV568PhoneDlg()
{
}

void CV568PhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPEN_CHECK, m_openbtn);
	DDX_Control(pDX, IDC_POWERON_CHECK, m_poweronbtn);
	DDX_Control(pDX, IDC_PCDEMO_CHECK, m_pcdemobtn);
}


BEGIN_MESSAGE_MAP(CV568PhoneDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_POWERON_CHECK, OnBnClickedPoweronCheck)
	ON_BN_CLICKED(IDC_OPEN_CHECK, OnBnClickedOpenCheck)
	ON_BN_CLICKED(IDC_DEMOBUTTON_UP, OnBnClickedDemobuttonUp)
	ON_BN_CLICKED(IDC_DEMOBUTTON_DOWN, OnBnClickedDemobuttonDown)
	ON_BN_CLICKED(IDC_DEMOBUTTON_LEFT, OnBnClickedDemobuttonLeft)
	ON_BN_CLICKED(IDC_DEMOBUTTON_RIGHT, OnBnClickedDemobuttonRight)
	ON_BN_CLICKED(IDC_DEMOBUTTON_MENU, OnBnClickedDemobuttonMenu)
	ON_BN_CLICKED(IDC_PCDEMO_CHECK, OnBnClickedPcdemoCheck)
	ON_BN_CLICKED(IDC_DISPPEARMENU_BTN, OnBnClickedDisppearmenuBtn)
	ON_BN_CLICKED(IDC_CLEARDEMO_BTN, OnBnClickedCleardemoBtn)
END_MESSAGE_MAP()


// CV568PhoneDlg 消息处理程序

BOOL CV568PhoneDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->InitUserOption();
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CV568PhoneDlg::InitUserOption(void)
{
	memset(&(this->m_option), 0, sizeof(TPhoneOption));
	this->m_option.openstate = 1;
}

void CV568PhoneDlg::DispOption(void)
{
	if(this->m_option.openstate)
		this->m_openbtn.SetCheck(BST_CHECKED);
	else
		this->m_openbtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.pcdemoen)
		this->m_pcdemobtn.SetCheck(BST_CHECKED);
	else
		this->m_pcdemobtn.SetCheck(BST_UNCHECKED);

	if(this->m_option.poweronstate)
		this->m_poweronbtn.SetCheck(BST_CHECKED);
	else
		this->m_poweronbtn.SetCheck(BST_UNCHECKED);
}

void CV568PhoneDlg::GetUserOption(void)
{
	this->m_option.pcdemoen = this->m_pcdemobtn.GetCheck();
	this->m_option.poweronstate = this->m_poweronbtn.GetCheck();
	this->m_option.openstate = this->m_openbtn.GetCheck();
}

void CV568PhoneDlg::OnBnClickedPoweronCheck()
{
	this->GetUserOption();
	if(this->m_option.poweronstate)
		this->m_pcdemobtn.EnableWindow(FALSE);
	else
		this->m_pcdemobtn.EnableWindow();
}

void CV568PhoneDlg::OnBnClickedOpenCheck()
{
	this->GetUserOption();
	if(this->m_option.openstate)
		this->m_poweronbtn.EnableWindow();
	else
		this->m_poweronbtn.EnableWindow(FALSE);
}

void CV568PhoneDlg::OnBnClickedPcdemoCheck()
{
	if(this->m_pcdemobtn.GetCheck())	//if checked, cmd will be sent to arm
	{
		this->m_option.pcdemoen = 1;
		EnableExInt(EXINT1);
	}
	else
	{
		this->m_option.pcdemoen = 0;
		DisableExInt(EXINT1);
	}
}




//================== simple demo ================

void CV568PhoneDlg::OnBnClickedDemobuttonUp()
{
	this->HSendKey(MENU_KEY_UP);
}

void CV568PhoneDlg::OnBnClickedDemobuttonDown()
{
	this->HSendKey(MENU_KEY_DOWN);
}

void CV568PhoneDlg::OnBnClickedDemobuttonLeft()
{
	this->HSendKey(MENU_KEY_LEFT);
}

void CV568PhoneDlg::OnBnClickedDemobuttonRight()
{
	this->HSendKey(MENU_KEY_RIGHT);
}

void CV568PhoneDlg::OnBnClickedDemobuttonMenu()
{
	this->HSendKey(MENU_KEY_SELECT);
}

void CV568PhoneDlg::OnBnClickedDisppearmenuBtn()
{
	this->HSendKey(MENU_KEY_DISAPPEAR);
}

void CV568PhoneDlg::OnBnClickedCleardemoBtn()
{
	this->HSendKey(MENU_KEY_RESET);
}

void CV568PhoneDlg::HSendKey(int key)
{
	CWaitCursor cursor;
	char str[0x100];

	if(this->m_option.pcdemoen)
	{
		sprintf(str, "SmpSendKey %x\n", key);
		SendCmd(str);
	}
	else
		SmpSendKey((UINT8)key);
	sprintf(str, "SmpSendKey(%d);\n", key);
	SaveCmd(str);
}

//================== end simple demo ================

#endif //V578CMD_DISABLE
