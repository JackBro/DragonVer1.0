// V568UsbcDlg.cpp : ʵ���ļ�
//
#include "internal.h"

#ifndef	V938CMD_DISABLE




// CV938HostIfDlg �Ի���

IMPLEMENT_DYNAMIC(CV938HostIfDlg, CBaseCtrlDlg)
CV938HostIfDlg::CV938HostIfDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV938HostIfDlg::IDD, pParent)
{
}

CV938HostIfDlg::~CV938HostIfDlg()
{
}

void CV938HostIfDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_V938HOSTIFLENGTH_EDIT, m_sizeBox);
	DDX_Control(pDX, IDC_V938HOSTIFADDRESS_EDIT, m_startAddrBox);
}


BEGIN_MESSAGE_MAP(CV938HostIfDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_V938HOSTIFWRITESRAM_BUTTON, OnBnClickedV938hostifwritesramButton)
	ON_BN_CLICKED(IDC_V938HOSTIFREADSRAM_BUTTON, OnBnClickedV938hostifreadsramButton)
END_MESSAGE_MAP()


// CV938HostIfDlg ��Ϣ�������


void CV938HostIfDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CV938HostIfDlg::InitUserOption(void)
{
	memset(&(this->m_option), 0, sizeof(this->m_option));
}

void CV938HostIfDlg::DispUserOption(void)
{
	char str[20] = {0};
	unsigned int val;

	/* sram write/read */
	val = this->m_option.size;
	sprintf(str, "%x", val);
	this->m_sizeBox.SetWindowText(str);

	val = this->m_option.startAddr;
	sprintf(str, "%x", val);
	this->m_startAddrBox.SetWindowText(str);
}

void CV938HostIfDlg::GetUserOption(char *tail)
{
	int x;
	char str[20] = {0};

	/* sram write/read */
	this->m_sizeBox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.size = (unsigned short)x;

	this->m_startAddrBox.GetWindowText(str, 20);
	sscanf(str, "%x", &x);
	this->m_option.startAddr = (unsigned int)x;

	CBasePanelDlg::GetFileBoxOption(this->m_option.srcfile, this->m_option.dstfile, tail);
}

BOOL CV938HostIfDlg::OnInitDialog()
{
	CBaseCtrlDlg::OnInitDialog();

	this->InitUserOption();
	this->DispUserOption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}

void CV938HostIfDlg::OnBnClickedV938hostifwritesramButton()
{
	CBufferBase pool;
	CFileBin fb;

	this->GetUserOption();

	pool.SetSize(this->m_option.size);
	fb.Open(this->m_option.srcfile, "rb");
	fb.Read(pool.m_buf, this->m_option.size);
	V9_WriteSram(this->m_option.startAddr, (UINT8 *)pool.m_buf, this->m_option.size);
}

void CV938HostIfDlg::OnBnClickedV938hostifreadsramButton()
{
	CBufferBase pool;
	CFileBin fb;

	this->GetUserOption();

	pool.SetSize(this->m_option.size);
	fb.Open(this->m_option.srcfile, "wb");
	V9_ReadSram(this->m_option.startAddr, (UINT8 *)pool.m_buf, this->m_option.size);
	fb.Write(pool.m_buf, this->m_option.size);
}

#else
void null938HostifDlg(void)
{
}
#endif //V938CMD_DISABLE

