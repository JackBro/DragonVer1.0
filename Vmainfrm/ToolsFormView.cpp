// ToolsFormView.cpp : 实现文件
//

#include "internal.h"


// CToolsFormView

IMPLEMENT_DYNCREATE(CToolsFormView, CBaseTabCtrlView)

CToolsFormView::CToolsFormView()
{
}

CToolsFormView::~CToolsFormView()
{
}

void CToolsFormView::DoDataExchange(CDataExchange* pDX)
{
	CBaseTabCtrlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CToolsFormView, CBaseTabCtrlView)
END_MESSAGE_MAP()


// CToolsFormView 诊断

#ifdef _DEBUG
void CToolsFormView::AssertValid() const
{
	CBaseTabCtrlView::AssertValid();
}

void CToolsFormView::Dump(CDumpContext& dc) const
{
	CBaseTabCtrlView::Dump(dc);
}

#endif //_DEBUG


// CToolsFormView 消息处理程序

void CToolsFormView::OnInitialUpdate()
{
	this->AddDlg(&(this->m_Tools1Dlg), IDD_TOOLS1_DLG, "Normal");
	this->AddDlg(&(this->m_Tools2Dlg), IDD_TOOLS2_DLG, "Image");
	this->AddDlg(&(this->m_Tools3Dlg), IDD_TOOLS3_DLG, "Audio");
	this->AddDlg(&(this->m_Tools4Dlg), IDD_TOOLS4_DLG, "Tools4");
	this->AddDlg(&(this->m_Tools5Dlg), IDD_TOOLS5_DLG, "Tools5");
	this->AddDlg(&(this->m_Tools6Dlg), IDD_TOOLS6_DLG, "Summon");
	this->AddDlg(&(this->m_Tools7Dlg), IDD_TOOLS7_DLG, "Tools7");
	this->AddDlg(&(this->m_Tools8Dlg), IDD_TOOLS8_DLG, "Scripts");
	this->AddDlg(&(this->m_Tools9Dlg), IDD_TOOLS9_DLG, "Tools9");
	CBaseTabCtrlView::OnInitialUpdate();
}

