// ReglistView.cpp : 实现文件
//

#include "internal.h"

// CUsbReglistView

IMPLEMENT_DYNCREATE(CUsbReglistView, CBaseReglistView)

CUsbReglistView::CUsbReglistView()
{
}

CUsbReglistView::~CUsbReglistView()
{
}

void CUsbReglistView::DoDataExchange(CDataExchange* pDX)
{
	CBaseReglistView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUsbReglistView, CBaseReglistView)
END_MESSAGE_MAP()


// CUsbReglistView 诊断

#ifdef _DEBUG
void CUsbReglistView::AssertValid() const
{
	CBaseReglistView::AssertValid();
}

void CUsbReglistView::Dump(CDumpContext& dc) const
{
	CBaseReglistView::Dump(dc);
}
#endif //_DEBUG


// CUsbReglistView 消息处理程序
void CUsbReglistView::OnInitialUpdate()
{
	CBaseReglistView::SetRegIndex(USB9604_REGGROUP);
	CBaseReglistView::OnInitialUpdate();
}

