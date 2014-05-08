#ifndef _LOGWND_H_
#define _LOGWND_H_



#ifndef HEADERPAGE_DLL
	#define HEADERPAGE_DLL __declspec(dllimport)
#endif

#include "dib.h"

/////////////////////////////////////////////////////////////////////////////

//   LogWnd	class
class HEADERPAGE_DLL CDib;

class HEADERPAGE_DLL CLogWnd : public CWnd
{
public:
// Construction
	CLogWnd();

	virtual ~CLogWnd();

public:
	BOOL Create(LPCSTR dibfilename = "");
	BOOL Create(WORD nID);
	void HideSplashScreen();

public:
	virtual void PostNcDestroy();

protected:
	//{{AFX_MSG(CLogWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

public:
	CDib m_dib;
	CWnd *m_pWnd;

private:

	CRect m_Rect;

	DECLARE_MESSAGE_MAP()

};


#endif
