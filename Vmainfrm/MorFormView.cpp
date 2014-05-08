// MorFormView.cpp : 实现文件
//

#include "internal.h"


// CMorFormView

IMPLEMENT_DYNCREATE(CMorFormView, CBaseTabCtrlView)

CMorFormView::CMorFormView()
{
}

CMorFormView::~CMorFormView()
{
}

void CMorFormView::DoDataExchange(CDataExchange* pDX)
{
	CBaseTabCtrlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMorFormView, CBaseTabCtrlView)
END_MESSAGE_MAP()


// CMorFormView 诊断

#ifdef _DEBUG
void CMorFormView::AssertValid() const
{
	CBaseTabCtrlView::AssertValid();
}

void CMorFormView::Dump(CDumpContext& dc) const
{
	CBaseTabCtrlView::Dump(dc);
}

#endif //_DEBUG


// CMorFormView 消息处理程序

void CMorFormView::OnInitialUpdate()
{
#ifndef HOSTCMD_DISABLE
	this->AddDlg(&(this->m_HostDlg),	IDD_HOSTPANEL_DLG,		"Host");
	this->AddDlg(&(this->m_TestDlg),	IDD_TESTFPGA_DLG,		"FPGA");
	this->AddDlg(&(this->m_NandFatDlg), IDD_NANDFAT_DLG,		"Nand_Fat");
#endif

#ifndef V578CMD_DISABLE
	this->AddDlg(&(this->m_BoardDlg),	IDD_V568BOARD_DLG,		"Board");
	this->AddDlg(&(this->m_DemoDlg),	IDD_V558DEMO_DLG,		"Demo");
	this->AddDlg(&(this->m_SifDlg),		IDD_V568SIF_DLG,		"Sif");
	this->AddDlg(&(this->m_IspDlg),		IDD_V568ISP_DLG,		"Isp");
	this->AddDlg(&(this->m_IppDlg),		IDD_V568IPP_DLG,		"Ipp");

	this->AddDlg(&(this->m_MarbDlg),	IDD_V568MARB_DLG,		"Marb");
	this->AddDlg(&(this->m_JpegDlg),	IDD_V568JPEG_DLG,		"Jpeg");
	this->AddDlg(&(this->m_LbufDlg),	IDD_V568LBUF_DLG,		"Lbuf");
	this->AddDlg(&(this->m_CpmDlg),		IDD_V568CPM_DLG,		"Cpm");
	this->AddDlg(&(this->m_HostIFDlg),	IDD_V558HOSTIF_DLG,		"Host I/F");

	this->AddDlg(&(this->m_LcdcDlg),	IDD_V568LCDC_DLG,		"Lcdc");
	this->AddDlg(&(this->m_LcdifDlg),	IDD_V568LCDIF_DLG,		"Lcd I/F");
	this->AddDlg(&(this->m_GeDlg),		IDD_V568GE_DLG,			"GE");

	//this->AddDlg(&(this->m_Demo1Dlg),	IDD_V568DEMO1_DLG,		"Demo1");
	//this->AddDlg(&(this->m_HighapiDlg), IDD_V568HIGHAPI_DLG,	"Highapi");

	this->AddDlg(&(this->m_UsbcDlg),	IDD_V568USBC_DLG,		"USB ctrl");
	//this->AddDlg(&(this->m_AudioDlg),	IDD_V568AUDIOADC_DLG,	"Audio Adc");
	//this->AddDlg(&(this->m_PhoneDlg),	IDD_V568PHONE_DLG,		"Phone Demo");
	this->AddDlg(&(this->m_TestChipDlg),IDD_V568TESTCHIP_DLG,	"Test Chip");
	//this->AddDlg(&(this->m_Test1TDlg),	IDD_V568TEST1T_DLG,		"1T Sram");
#endif

//#ifndef V868CMD_DISABLE
//	this->AddDlg(&(this->m_SdifDlg),	IDD_V568SDIF_DLG,		"SD I/F");
//#endif
//
//#ifndef USBCMD_DISABLE
//	this->AddDlg(&(this->m_UsbDlg),		IDD_USBCHIP_DLG,		"Usb9604");
//#endif
//
//#ifndef DEVICECMD_DISABLE
//	this->AddDlg(&(this->m_SensorDlg),	IDD_V568SENSOR_DLG,		"Sensor");
//	this->AddDlg(&(this->m_SdcardDlg),	IDD_V568SDCARD_DLG,		"SD card");
//#endif

	CBaseTabCtrlView::OnInitialUpdate();
}

