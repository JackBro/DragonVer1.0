#ifndef __BASECLASS_BASESTATUSBAR_H__
#define __BASECLASS_BASESTATUSBAR_H__



#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif


#define MIN_PROGRESS	0
#define MAX_PROGRESS	100

class BASECLASS_DLL CBaseStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CBaseStatusBar)

public:
	CBaseStatusBar();
	virtual ~CBaseStatusBar();

public:
	void SetProgress(int status);

protected:
	DECLARE_MESSAGE_MAP()

private:
	CProgressCtrl m_wndProgBar;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



#endif
