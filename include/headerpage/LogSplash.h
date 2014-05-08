#ifndef __HEADERPAGE_LOGSPLASH_H__
#define __HEADERPAGE_LOGSPLASH_H__


#ifndef HEADERPAGE_DLL
	#define HEADERPAGE_DLL __declspec(dllimport)
#endif

// CLogSplash

class HEADERPAGE_DLL CLogSplash : public CWnd
{
	DECLARE_DYNAMIC(CLogSplash)

public:
	CLogSplash();
	virtual ~CLogSplash();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(char *filename = NULL);

private:
	CBitmap m_bmp;
public:
	afx_msg void OnPaint();
};



#endif
