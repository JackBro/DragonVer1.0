// MorFormView.cpp : 实现文件
//

#include "internal.h"


// CV938FormView

IMPLEMENT_DYNCREATE(CV938FormView, CBaseTabCtrlView)

CV938FormView::CV938FormView()
{
}

CV938FormView::~CV938FormView()
{
}

void CV938FormView::DoDataExchange(CDataExchange* pDX)
{
	CBaseTabCtrlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CV938FormView, CBaseTabCtrlView)
END_MESSAGE_MAP()


// CV938FormView 诊断

#ifdef _DEBUG
void CV938FormView::AssertValid() const
{
	CBaseTabCtrlView::AssertValid();
}

void CV938FormView::Dump(CDumpContext& dc) const
{
	CBaseTabCtrlView::Dump(dc);
}

#endif //_DEBUG


// CV938FormView 消息处理程序

void CV938FormView::OnInitialUpdate()
{
#ifndef HOSTCMD_DISABLE
	this->AddDlg(&(this->m_HostDlg), IDD_HOSTPANEL_DLG, "Host");
	this->AddDlg(&(this->m_TestDlg), IDD_TESTFPGA_DLG, "Test FPGA");
	this->AddDlg(&(this->m_NandFatDlg), IDD_NANDFAT_DLG, "Nand & Fat");
#endif

#ifndef V938CMD_DISABLE
	this->AddDlg(&(this->m_HifDlg), IDD_V938HOSTIF_DLG, "Host IF");
	this->AddDlg(&(this->m_Cpm9Dlg), IDD_V938CPM_DLG, "CPM");
	this->AddDlg(&(this->m_SeqDlg), IDD_V938SEQ_DLG, "SEQ");
	this->AddDlg(&(this->m_WsDlg), IDD_V938WS_DLG, "WS");
	this->AddDlg(&(this->m_Mp3Dlg), IDD_V938MP3_DLG, "MP3");
	this->AddDlg(&(this->m_DemoDlg938), IDD_V938DEMO_DLG, "DEMO");
#endif

#ifndef USBCMD_DISABLE
	this->AddDlg(&(this->m_UsbDlg), IDD_USBCHIP_DLG, "Usb9604");
#endif

	CBaseTabCtrlView::OnInitialUpdate();
}

