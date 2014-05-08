// CmodelView.cpp : 实现文件
//

#include "internal.h"

// CCmodelView

IMPLEMENT_DYNCREATE(CCmodelView, CBaseTabCtrlView)

CCmodelView::CCmodelView()
{
}

CCmodelView::~CCmodelView()
{
}

void CCmodelView::DoDataExchange(CDataExchange* pDX)
{
	CBaseTabCtrlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCmodelView, CBaseTabCtrlView)
END_MESSAGE_MAP()


// CCmodelView 诊断

#ifdef _DEBUG
void CCmodelView::AssertValid() const
{
	CBaseTabCtrlView::AssertValid();
}

void CCmodelView::Dump(CDumpContext& dc) const
{
	CBaseTabCtrlView::Dump(dc);
}

#endif //_DEBUG


// CCmodelView 消息处理程序

void CCmodelView::OnInitialUpdate()
{
	this->AddDlg(&(this->m_V568Dlg), IDD_BASECMVIEW_DLG, "V568");
	//this->AddDlg(&(this->m_IspDlg), IDD_BASECMVIEW_DLG, "Isp");
	//this->AddDlg(&(this->m_IppDlg), IDD_BASECMVIEW_DLG, "Ipp");
	//this->AddDlg(&(this->m_LbufDlg), IDD_BASECMVIEW_DLG, "Lbuf");
	//this->AddDlg(&(this->m_LcdcDlg), IDD_BASECMVIEW_DLG, "Lcdc");
	//this->AddDlg(&(this->m_JpegEncDlg), IDD_BASECMVIEW_DLG, "Jpeg Enc");
	//this->AddDlg(&(this->m_JpegDecDlg), IDD_BASECMVIEW_DLG, "Jpeg Dec");
	//this->AddDlg(&(this->m_CpmDlg), IDD_BASECMVIEW_DLG, "Cpm");
	//this->AddDlg(&(this->m_LcdifDlg), IDD_BASECMVIEW_DLG, "Lcd I/F");
	//this->AddDlg(&(this->m_Mpeg2EncDlg), IDD_BASECMVIEW_DLG, "Mpeg2 Enc");
	//this->AddDlg(&(this->m_Mpeg2DecDlg), IDD_BASECMVIEW_DLG, "Mpeg2 Dec");
	//this->AddDlg(&(this->m_Mpeg4EncDlg), IDD_BASECMVIEW_DLG, "Mpeg4 Enc");
	//this->AddDlg(&(this->m_Mpeg4DecDlg), IDD_BASECMVIEW_DLG, "Mpeg4 Dec");
	//this->AddDlg(&(this->m_H264EncDlg), IDD_BASECMVIEW_DLG, "H264 Enc");
	//this->AddDlg(&(this->m_H264DecDlg), IDD_BASECMVIEW_DLG, "H264 Dec");

	CBaseTabCtrlView::OnInitialUpdate();
}


