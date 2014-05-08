#ifndef _CMBASEEDIT_H_
#define _CMBASEEDIT_H_

#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif



#define MEDIT_MAXCOUNT	8

class BASECLASS_DLL CMBaseEdit : public CEdit
{
// Construction
public:
	CMBaseEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMBaseEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMBaseEdit();

private:
	int m_maxcount;

public:
	void SetMaxCount(int count);
	// Generated message map functions
protected:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

/////////////////////////////////////////////////////////////////////////////

#endif
