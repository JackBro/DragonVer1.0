// ReglistView.cpp : ʵ���ļ�
//

#include "internal.h"

// CV578ReglistView

IMPLEMENT_DYNCREATE(CV578ReglistView, CBaseReglistView)

CV578ReglistView::CV578ReglistView()
{
}

CV578ReglistView::~CV578ReglistView()
{
}

void CV578ReglistView::DoDataExchange(CDataExchange* pDX)
{
	CBaseReglistView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CV578ReglistView, CBaseReglistView)
END_MESSAGE_MAP()


// CV578ReglistView ���

#ifdef _DEBUG
void CV578ReglistView::AssertValid() const
{
	CBaseReglistView::AssertValid();
}

void CV578ReglistView::Dump(CDumpContext& dc) const
{
	CBaseReglistView::Dump(dc);
}
#endif //_DEBUG


// CV578ReglistView ��Ϣ�������
void CV578ReglistView::OnInitialUpdate()
{
	CBaseReglistView::SetRegIndex(V578_REGGROUP);
	CBaseReglistView::OnInitialUpdate();
}


