// BaseStatusBar.cpp : 实现文件
//
#include "internal.h"




// CBaseStatusBar

IMPLEMENT_DYNAMIC(CBaseStatusBar, CStatusBar)
CBaseStatusBar::CBaseStatusBar()
{
}

CBaseStatusBar::~CBaseStatusBar()
{
}


BEGIN_MESSAGE_MAP(CBaseStatusBar, CStatusBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CBaseStatusBar 消息处理程序



int CBaseStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= WS_CLIPCHILDREN;
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	this->m_wndProgBar.Create(WS_CHILD, CRect(), this, 1);
	this->m_wndProgBar.SetRange(MIN_PROGRESS, MAX_PROGRESS);

	return 0;
}

void CBaseStatusBar::OnSize(UINT nType, int cx, int cy)
{
	CRect rc;								  // rectangle 

	CStatusBar::OnSize(nType, cx, cy); // call base class
	GetItemRect(0, &rc);					  // item 0 = first pane, "ready" message
	m_wndProgBar.MoveWindow(&rc,FALSE);// move progress bar
}

void CBaseStatusBar::SetProgress(int status)
{
	DWORD oldstyle = this->m_wndProgBar.GetStyle();
	DWORD style = oldstyle;

	if(status > MIN_PROGRESS)
		style |= WS_VISIBLE;
	else
		style &= ~WS_VISIBLE;

	if (style != oldstyle) 
	{
		SetWindowText(NULL);											// clear old text
		SetWindowLong(this->m_wndProgBar.m_hWnd, GWL_STYLE, style);	// change style
	}

	this->m_wndProgBar.SetPos(status);
	if(status == MIN_PROGRESS)
		GetParent()->PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
}

