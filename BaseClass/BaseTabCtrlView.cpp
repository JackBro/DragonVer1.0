// TabCtrlView.cpp : 实现文件
//
#include "internal.h"


IMPLEMENT_DYNCREATE(CBaseTabCtrlView, CCtrlView)

CBaseTabCtrlView::CBaseTabCtrlView(int count) : CCtrlView(WC_TABCONTROL, AFX_WS_DEFAULT_VIEW) 
{ 
	int i;

	this->m_pTab = NULL;
	this->m_TotalDlg = 0;
	this->m_CurDlg = 0;
	this->m_ValidDlg = 0;
	this->m_pTab = new TabContext[count];
	if(!this->m_pTab)
		return;
	this->m_TotalDlg = count;
	for(i = 0; i < this->m_TotalDlg; i++)
		memset(&(this->m_pTab[i]), 0, sizeof(TabContext));
	this->m_state = DIALOG_NOTCREATED;
}

CBaseTabCtrlView::~CBaseTabCtrlView()
{
	if(this->m_pTab)
		delete [](this->m_pTab);
}

CTabCtrl& CBaseTabCtrlView::GetTabCtrl() const 
{ return *(CTabCtrl*)this; }

BEGIN_MESSAGE_MAP(CBaseTabCtrlView, CCtrlView)
	//{{AFX_MSG_MAP(CBaseTabCtrlView)
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CBaseTabCtrlView::AddDlg(CDialog *dlg, UINT dlgId, char *header)
{
	if(!this->m_TotalDlg)
		return;
	this->m_pTab[this->m_ValidDlg].pDlg = dlg;
	this->m_pTab[this->m_ValidDlg].DlgId = dlgId;
	strcpy(this->m_pTab[this->m_ValidDlg].Header, header);
	this->m_ValidDlg++;
}

void CBaseTabCtrlView::SetTabPos(void)
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;
	int i;
	CTabCtrl &tab = this->GetTabCtrl();

	if(this->m_state == DIALOG_NOTCREATED)
		return;

	tab.GetClientRect(&tabRect);
	tab.GetItemRect(0, &itemRect);
	nX  = 2;//itemRect.left;
	nY  = itemRect.bottom + 4;
	nXc = tabRect.right - tabRect.left + nX;
	nYc = tabRect.bottom - nY - 2;

	for(i = 0; i < this->m_ValidDlg; i++)
	{
		if(i == this->m_CurDlg)
			this->m_pTab[i].pDlg->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
		else
			this->m_pTab[i].pDlg->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
}

void CBaseTabCtrlView::InitMtab(void)
{
	int i;
	CTabCtrl &tab = this->GetTabCtrl();

	for(i = 0; i < this->m_ValidDlg; i++)
	{
		this->m_pTab[i].pDlg->Create(this->m_pTab[i].DlgId, this);
		tab.InsertItem(i, this->m_pTab[i].Header);
	}
	this->m_state = DIALOG_CREATED;
	this->SetTabPos();
	this->SetCurSel(this->m_CurDlg);
}

int CBaseTabCtrlView::SetCurSel(int nItem)
{
	CTabCtrl &tab = this->GetTabCtrl();

	if(this->m_CurDlg != nItem)
	{
		this->m_pTab[this->m_CurDlg].pDlg->ShowWindow(SW_HIDE);
		this->m_pTab[nItem].pDlg->ShowWindow(SW_SHOW);
	}
	this->m_CurDlg = nItem;
	return tab.SetCurSel(nItem);
}

void CBaseTabCtrlView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTabCtrl &tab = this->GetTabCtrl();

	CCtrlView::OnLButtonDown(nFlags, point);
	this->SetCurSel(tab.GetCurSel());
}

void CBaseTabCtrlView::OnInitialUpdate()
{
	this->InitMtab();
	CCtrlView::OnInitialUpdate();
}

void CBaseTabCtrlView::OnSize(UINT nType, int cx, int cy)
{
	CCtrlView::OnSize(nType, cx, cy);
	this->SetTabPos();
	// TODO: 在此添加消息处理程序代码
}

BOOL CBaseTabCtrlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

//	this->m_dwDefaultStyle |= WS_HSCROLL | WS_VSCROLL;// |	ES_AUTOHSCROLL | ES_AUTOVSCROLL;
//	cs.style |= WS_MAXIMIZE;
	return CCtrlView::PreCreateWindow(cs);
}
