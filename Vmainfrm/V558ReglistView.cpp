// ReglistView.cpp : ʵ���ļ�
//

#include "internal.h"

// CV558ReglistView

IMPLEMENT_DYNCREATE(CV558ReglistView, CBaseReglistView)

CV558ReglistView::CV558ReglistView()
{
}

CV558ReglistView::~CV558ReglistView()
{
}

void CV558ReglistView::DoDataExchange(CDataExchange* pDX)
{
	CBaseReglistView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CV558ReglistView, CBaseReglistView)
END_MESSAGE_MAP()


// CV558ReglistView ���

#ifdef _DEBUG
void CV558ReglistView::AssertValid() const
{
	CBaseReglistView::AssertValid();
}

void CV558ReglistView::Dump(CDumpContext& dc) const
{
	CBaseReglistView::Dump(dc);
}
#endif //_DEBUG


// CV558ReglistView ��Ϣ�������
void CV558ReglistView::OnInitialUpdate()
{
	CBaseReglistView::SetRegIndex(V558_REGGROUP);
	CBaseReglistView::OnInitialUpdate();
}


