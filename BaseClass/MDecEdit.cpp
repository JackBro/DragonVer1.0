// MHexEdit.cpp : implementation file
//
#include "internal.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDecEdit

CMDecEdit::CMDecEdit()
{
}

CMDecEdit::~CMDecEdit()
{
}


BEGIN_MESSAGE_MAP(CMDecEdit, CMBaseEdit)
	//{{AFX_MSG_MAP(CMDecEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDecEdit message handlers

void CMDecEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( ((nChar >= '0') && (nChar <= '9')) || (nChar == VK_BACK))
		CMBaseEdit::OnChar(nChar, nRepCnt, nFlags);
}


