// ToolsFormView.cpp : 实现文件
//

#include "internal.h"


// CCmodelFormView

IMPLEMENT_DYNCREATE(CCmodelFormView, CBaseTabCtrlView)

CCmodelFormView::CCmodelFormView()
{
}

CCmodelFormView::~CCmodelFormView()
{
}

void CCmodelFormView::DoDataExchange(CDataExchange* pDX)
{
	CBaseTabCtrlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCmodelFormView, CBaseTabCtrlView)
END_MESSAGE_MAP()


// CCmodelFormView 诊断

#ifdef _DEBUG
void CCmodelFormView::AssertValid() const
{
	CBaseTabCtrlView::AssertValid();
}

void CCmodelFormView::Dump(CDumpContext& dc) const
{
	CBaseTabCtrlView::Dump(dc);
}

#endif //_DEBUG


// CCmodelFormView 消息处理程序

void CCmodelFormView::OnInitialUpdate()
{
	this->AddDlg(&(this->m_OldCmodelDlg), IDD_OLDCMODEL_DLG, "Old Cmodel");
	this->AddDlg(&(this->m_CmodelDlg), IDD_CMODEL_DLG, "New Cmodel");
	this->AddDlg(&(this->m_LcdcDlg), IDD_BLK_LCDC_DLG, "Lcdc");
	this->AddDlg(&(this->m_GeDlg), IDD_BLK_GE_DLG, "Ge");
	this->AddDlg(&(this->m_JpegDlg), IDD_BLK_JPEG_DLG, "Jpeg");
	this->AddDlg(&(this->m_IspDlg), IDD_BLK_ISP_DLG, "Isp");
	this->AddDlg(&(this->m_IppDlg), IDD_BLK_IPP_DLG, "Ipp");
	this->AddDlg(&(this->m_LbufDlg), IDD_BLK_LBUF_DLG, "Lbuf");
	this->AddDlg(&(this->m_TvencDlg), IDD_BLK_TVENC_DLG, "Tv enc");

	this->AddDlg(&(this->m_SifDlg), IDD_BLK_SIF_DLG, "Sensor I/F");
	this->AddDlg(&(this->m_AudadcDlg), IDD_BLK_AUDADC_DLG, "Audio ADC");
	this->AddDlg(&(this->m_BiuDlg), IDD_BLK_BIU_DLG, "Biu");
	this->AddDlg(&(this->m_CpmDlg), IDD_BLK_CPM_DLG, "Cpm");
	this->AddDlg(&(this->m_LcdifDlg), IDD_BLK_LCDIF_DLG, "Lcd I/F");
	this->AddDlg(&(this->m_MarbDlg), IDD_BLK_MARB_DLG, "Marb");
	this->AddDlg(&(this->m_SdcardDlg), IDD_BLK_SDCARD_DLG, "SD card");
	this->AddDlg(&(this->m_SdifDlg), IDD_BLK_SDIF_DLG, "SD I/F");
	this->AddDlg(&(this->m_SensorDlg), IDD_BLK_SENSOR_DLG, "Sensor");
	this->AddDlg(&(this->m_UsbcDlg), IDD_BLK_USBC_DLG, "Usbc");

	CBaseTabCtrlView::OnInitialUpdate();
}

