#include "internal.h"


CLogWnd::CLogWnd()
{
}

CLogWnd::~CLogWnd()
{
}

BEGIN_MESSAGE_MAP(CLogWnd, CWnd)
	//{{AFX_MSG_MAP(CLogWin)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CLogWnd::Create(LPCSTR dibfilename)
{
	int cx, cy;

	if(!m_dib.Load((unsigned char*) dibfilename))
		return FALSE;

	HDC hDC = ::GetDC(NULL);

	cx = ::GetDeviceCaps(hDC, HORZRES);
	cy = ::GetDeviceCaps(hDC, VERTRES);

	::ReleaseDC( NULL, hDC ); 

	m_pWnd = AfxGetMainWnd();
	
	return CreateEx(0,AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), "Vmonitor1.0", WS_VISIBLE, (cx/8), (cy/8), (cx*3/4), (cy*3/4), NULL, NULL);
}

BOOL CLogWnd::Create(WORD nID)
{
	int cx, cy;

	if(!m_dib.Load(nID))
		return FALSE;

	HDC hDC = ::GetDC(NULL);

	cx = ::GetDeviceCaps(hDC, HORZRES);
	cy = ::GetDeviceCaps(hDC, VERTRES);

	::ReleaseDC( NULL, hDC ); 

	m_pWnd = AfxGetMainWnd();
	
	return CreateEx(0,AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), "Vmonitor1.0", WS_VISIBLE, (cx/8), (cy/8), (cx*3/4), (cy*3/4), NULL, NULL);
}

int CLogWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	AfxGetMainWnd()->ShowWindow(SW_HIDE);
	// Center the window.
	AfxGetApp()->m_pMainWnd = this;;
	CenterWindow();

	// Set a timer to destroy the splash screen.
	SetTimer(1, 6000, NULL);

	return 0;
}

void CLogWnd::OnPaint()
{
	RECT rect;
	CPaintDC dc(this);
	TRect Dst;

	this->GetWindowRect(&rect);

	Dst.ltp.x = 0;
	Dst.ltp.y = 0;
	Dst.size.cx = (UINT16)(rect.right - rect.left);
	Dst.size.cy = (UINT16)(rect.bottom - rect.top);

	m_dib.StretchDisplay(&dc, Dst, SRCCOPY);
}

void CLogWnd::OnTimer(UINT nIDEvent)
{
	nIDEvent = nIDEvent;
	HideSplashScreen();
}

void CLogWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	AfxGetApp()->m_pMainWnd = m_pWnd;;
	DestroyWindow();
}

void CLogWnd::PostNcDestroy()
{
	// Free the C++ class.

	AfxGetMainWnd()->SetActiveWindow();
	AfxGetMainWnd()->ShowWindow(SW_SHOW);

	delete this;
}


void CLogWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	nFlags = nFlags;
	point.x = point.x;
	
	HideSplashScreen();

//	CWnd::OnLButtonDown(nFlags, point);
}
