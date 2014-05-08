// V568UsbcDlg.cpp : ʵ���ļ�
//
#include "internal.h"

#ifndef	V578CMD_DISABLE

// CV568Test1TDlg �Ի���

IMPLEMENT_DYNAMIC(CV568Test1TDlg, CBaseCtrlDlg)
CV568Test1TDlg::CV568Test1TDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568Test1TDlg::IDD, pParent)
{
}

CV568Test1TDlg::~CV568Test1TDlg()
{
}

void CV568Test1TDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CV568Test1TDlg, CBaseCtrlDlg)
	ON_BN_CLICKED(IDC_1TTEST_BUTTON, OnBnClicked1ttestButton)
END_MESSAGE_MAP()


// CV568Test1TDlg ��Ϣ�������

void CV568Test1TDlg::OnBnClicked1ttestButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned char flag = 0;

	this->GetDlgItem(IDC_1TTEST_STATIC)->SetWindowText("");
	TestChip1TStart();
	Sleep(2000);
	flag = TestChip1TChk();
	if(flag)
		this->GetDlgItem(IDC_1TTEST_STATIC)->SetWindowText("OK !");
	else
	{
		this->GetDlgItem(IDC_1TTEST_STATIC)->SetWindowText("Failed !");
		MessageBox("1T Test Failed !", "Fail", MB_OK | MB_ICONERROR);
	}
}

#endif //V578CMD_DISABLE

