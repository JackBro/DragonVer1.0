// ReglistView.cpp : 实现文件
//

#include "internal.h"


// CV938ReglistView

IMPLEMENT_DYNCREATE(CV938ReglistView, CBaseReglistView)

CV938ReglistView::CV938ReglistView()
{
}

CV938ReglistView::~CV938ReglistView()
{
}

void CV938ReglistView::DoDataExchange(CDataExchange* pDX)
{
	CBaseReglistView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CV938ReglistView, CBaseReglistView)
END_MESSAGE_MAP()


// CV938ReglistView 诊断

#ifdef _DEBUG
void CV938ReglistView::AssertValid() const
{
	CBaseReglistView::AssertValid();
}

void CV938ReglistView::Dump(CDumpContext& dc) const
{
	CBaseReglistView::Dump(dc);
}
#endif //_DEBUG


// CV938ReglistView 消息处理程序
void CV938ReglistView::OnInitialUpdate()
{
	CBaseReglistView::SetRegIndex(V938_REGGROUP);
	CBaseReglistView::OnInitialUpdate();
}

