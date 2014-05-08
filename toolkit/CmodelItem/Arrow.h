#ifndef __CMODELITEM_ARROW_H__
#define __CMODELITEM_ARROW_H__
#include "ex_CmodelItem.h"



// CArrow

class CMODELITEM_DLL CArrow : public CWnd
{
	DECLARE_DYNAMIC(CArrow)

public:
	CArrow();
	virtual ~CArrow();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(const POINT& pt, CWnd* pParentWnd, UINT nID);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};



#endif
