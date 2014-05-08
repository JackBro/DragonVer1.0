// ComSettingDlg.cpp : 实现文件
//
#include "internal.h"


char gCurDirectory[0x100];


// CUsbSettingDlg 对话框

IMPLEMENT_DYNAMIC(CUsbSettingDlg, CDialog)
CUsbSettingDlg::CUsbSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbSettingDlg::IDD, pParent)
{
}

CUsbSettingDlg::~CUsbSettingDlg()
{
}

void CUsbSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIPE_COMBO, m_Pipebox);
}


BEGIN_MESSAGE_MAP(CUsbSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_INST_USB, OnBnClickedButtonInstUsb)
END_MESSAGE_MAP()


// CUsbSettingDlg 消息处理程序

void CUsbSettingDlg::GetUserOption(void)
{
	this->m_Pipe = this->m_Pipebox.GetCurSel();
}

void CUsbSettingDlg::DispOption(void)
{
	this->m_Pipebox.SetCurSel(this->m_Pipe);
}

BOOL CUsbSettingDlg::OnInitDialog()
{
	int i;
	char str[10];

	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化

	for(i = 0; i < MAX_USBPIPE; i++)
	{
		sprintf(str, "%d", i);
		this->m_Pipebox.InsertString(i, str);
	}
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CUsbSettingDlg::OnOK()
{
	this->GetUserOption();
	this->GetParent()->SendMessage(WM_USBSETTING);
	Sleep(1);
	CDialog::OnOK();
}

void CUsbSettingDlg::OnBnClickedButtonInstUsb()
{
	BOOL bOk;
	char drvdir[0x100];


	sprintf(drvdir, "%s/Vmainfrm/lib/vcmobi.inf", gCurDirectory);
	
	bOk = SetupCopyOEMInf(drvdir, NULL, SPOST_PATH, 0, NULL, 0, NULL, NULL);

	if(!bOk)
		::AfxMessageBox("Error!");

}
