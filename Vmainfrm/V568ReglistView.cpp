// ReglistView.cpp : 实现文件
//

#include "internal.h"

// CV568ReglistView

IMPLEMENT_DYNCREATE(CV568ReglistView, CBaseReglistView)

CV568ReglistView::CV568ReglistView()
{
}

CV568ReglistView::~CV568ReglistView()
{
}

void CV568ReglistView::DoDataExchange(CDataExchange* pDX)
{
	CBaseReglistView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CV568ReglistView, CBaseReglistView)
END_MESSAGE_MAP()


// CV568ReglistView 诊断

#ifdef _DEBUG
void CV568ReglistView::AssertValid() const
{
	CBaseReglistView::AssertValid();
}

void CV568ReglistView::Dump(CDumpContext& dc) const
{
	CBaseReglistView::Dump(dc);
}
#endif //_DEBUG


// CV568ReglistView 消息处理程序
void CV568ReglistView::OnInitialUpdate()
{
	CBaseReglistView::SetRegIndex(V568_REGGROUP);
	CBaseReglistView::OnInitialUpdate();
}

