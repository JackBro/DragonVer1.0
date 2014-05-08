#ifndef _MDECEDIT_H_
#define _MDECEDIT_H_


#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif

#if _MSC_VER > 1000
#ifndef __BASECLASS_MDECEDIT_H__
#define __BASECLASS_MDECEDIT_H__


#endif // _MSC_VER > 1000

#include "MBaseEdit.h"


/////////////////////////////////////////////////////////////////////////////
// CMDecEdit window
#define MDEC_MAXCOUNT	8

class BASECLASS_DLL CMDecEdit : public CMBaseEdit
{
// Construction
public:
	CMDecEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDecEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMDecEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMDecEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MHEXEDIT_H__4A095310_920E_4ACB_8392_7A76A9AFE11B__INCLUDED_)

#endif
