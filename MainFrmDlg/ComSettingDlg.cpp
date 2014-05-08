// ComSettingDlg.cpp : 实现文件
//
#include "internal.h"

// CComSettingDlg 对话框

IMPLEMENT_DYNAMIC(CComSettingDlg, CDialog)
CComSettingDlg::CComSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComSettingDlg::IDD, pParent)
{
}

CComSettingDlg::~CComSettingDlg()
{
}

void CComSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BAUD_COMBO, m_Baudbox);
	DDX_Control(pDX, IDC_PORT_COMBO, m_Portbox);
}


BEGIN_MESSAGE_MAP(CComSettingDlg, CDialog)
END_MESSAGE_MAP()


// CComSettingDlg 消息处理程序
static const int gBaudRate[] =
{
	CBR_9600, CBR_38400, CBR_57600, CBR_115200
};

void CComSettingDlg::GetUserOption(void)
{
	int sel;

	sel = this->m_Baudbox.GetCurSel();
	this->m_Baud = gBaudRate[sel];
	this->m_Port = this->m_Portbox.GetCurSel();
	this->m_Port++;
}

void CComSettingDlg::DispOption(void)
{
	int i;

	this->m_Portbox.SetCurSel(this->m_Port-1);
	for(i = 0; i < sizeof(gBaudRate) / sizeof(int); i++)
	{
		if(this->m_Baud == gBaudRate[i])
		{
			this->m_Baudbox.SetCurSel(i);
			return;
		}
	}
	this->m_Baudbox.SetCurSel(3);
}

BOOL CComSettingDlg::OnInitDialog()
{
	int i;
	char str[10];
	const char *gBaudDesc[] =
	{
		"CBR_9600", "CBR_38400", "CBR_57600", "CBR_115200"
	};

	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化

	for(i = 0; i < sizeof(gBaudRate) / sizeof(int); i++)
		this->m_Baudbox.InsertString(i, (char *)gBaudDesc[i]);
	for(i = 0; i < MAX_COMPORT; i++)
	{
		sprintf(str, "%d", i+1);
		this->m_Portbox.InsertString(i, str);
	}
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CComSettingDlg::OnOK()
{
	this->GetUserOption();
	this->GetParent()->SendMessage(WM_COMSETTING);
	Sleep(1);
	CDialog::OnOK();
}

