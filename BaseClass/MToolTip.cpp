// MHexEdit.cpp : implementation file
//
#include "internal.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMToolTip

CMToolTip::CMToolTip()
{
}

CMToolTip::~CMToolTip()
{
}

BEGIN_MESSAGE_MAP(CMToolTip, CToolTipCtrl)
	//{{AFX_MSG_MAP(CMToolTip)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CMToolTip::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CToolTipCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->SetMaxTipWidth(800);
	this->SetDelayTime(10000);
	return 0;
}
