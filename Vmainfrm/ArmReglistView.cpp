// ReglistView.cpp : 实现文件
//

#include "internal.h"

// CArmReglistView

IMPLEMENT_DYNCREATE(CArmReglistView, CBaseReglistView)

CArmReglistView::CArmReglistView()
{
}

CArmReglistView::~CArmReglistView()
{
}

void CArmReglistView::DoDataExchange(CDataExchange* pDX)
{
	CBaseReglistView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CArmReglistView, CBaseReglistView)
END_MESSAGE_MAP()


// CArmReglistView 诊断

#ifdef _DEBUG
void CArmReglistView::AssertValid() const
{
	CBaseReglistView::AssertValid();
}

void CArmReglistView::Dump(CDumpContext& dc) const
{
	CBaseReglistView::Dump(dc);
}
#endif //_DEBUG


// CArmReglistView 消息处理程序
void CArmReglistView::OnInitialUpdate()
{
	CBaseReglistView::SetRegIndex(ARM7_REGGROUP);
	CBaseReglistView::OnInitialUpdate();
}


