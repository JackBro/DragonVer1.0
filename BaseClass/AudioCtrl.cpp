// MHexEdit.cpp : implementation file
//
#include "internal.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




/////////////////////////////////////////////////////////////////////////////
// CAudioCtrl

IMPLEMENT_DYNAMIC(CAudioCtrl, CWnd)
CAudioCtrl::CAudioCtrl()
{
}

CAudioCtrl::~CAudioCtrl()
{
}

BEGIN_MESSAGE_MAP(CAudioCtrl, CWnd)
	//{{AFX_MSG_MAP(CAudioCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAudioCtrl::Create(CWnd* pParentWnd, UINT nID)
{
	RECT rect = { 10, 10, 20, 20 };

	return CWnd::Create(NULL, NULL, WS_CHILD, rect, pParentWnd, nID, NULL);
}
