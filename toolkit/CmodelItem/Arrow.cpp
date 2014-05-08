// Arrow.cpp : 实现文件
//

#include <afxpriv.h>
#include "Arrow.h"


// CArrow

IMPLEMENT_DYNAMIC(CArrow, CWnd)
CArrow::CArrow()
{
}

CArrow::~CArrow()
{
}


BEGIN_MESSAGE_MAP(CArrow, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CArrow 消息处理程序

BOOL CArrow::Create(const POINT& pt, CWnd* pParentWnd, UINT nID)
{
	RECT rect;

	rect.left = pt.x;
	rect.top = pt.y;
	rect.right = pt.x + 100;
	rect.bottom = pt.y + 100;
	return CWnd::Create(NULL, NULL, WS_CHILD, rect, pParentWnd, nID, NULL);
}

void CArrow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.Rectangle(10, 10, 200, 200);
}

void CArrow::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	this->MessageBox("clicked me!");
}
