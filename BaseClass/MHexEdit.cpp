// MHexEdit.cpp : implementation file
//
#include "internal.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMHexEdit

CMHexEdit::CMHexEdit()
{
}

CMHexEdit::~CMHexEdit()
{
}


BEGIN_MESSAGE_MAP(CMHexEdit, CMBaseEdit)
	//{{AFX_MSG_MAP(CMHexEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMHexEdit message handlers

void CMHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( ((nChar >= '0') && (nChar <= '9')) ||
		((nChar >= 'a') && (nChar <= 'f')) ||
		((nChar >= 'A') && (nChar <= 'F')) ||
		(nChar == VK_BACK) )
		CMBaseEdit::OnChar(nChar, nRepCnt, nFlags);
}


