// ReglistView.cpp : ʵ���ļ�
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


// CUsbReglistView ���

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


// CUsbReglistView ��Ϣ�������
void CUsbReglistView::OnInitialUpdate()
{
	CBaseReglistView::SetRegIndex(USB9604_REGGROUP);
	CBaseReglistView::OnInitialUpdate();
}

