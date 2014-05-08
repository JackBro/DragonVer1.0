// MBaseEdit.cpp : implementation file
//
#include "internal.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMBaseEdit

CMBaseEdit::CMBaseEdit()
{
	this->m_maxcount = MEDIT_MAXCOUNT;
}

CMBaseEdit::~CMBaseEdit()
{
}


BEGIN_MESSAGE_MAP(CMBaseEdit, CEdit)
	//{{AFX_MSG_MAP(CMBaseEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMBaseEdit message handlers

void CMBaseEdit::SetMaxCount(int count)
{
	this->m_maxcount = count;
}

void CMBaseEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_BACK)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	if(this->GetWindowTextLength() == this->m_maxcount)
		return;

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

