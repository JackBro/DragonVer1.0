#if !defined(AFX_MHEXEDIT_H__4A095310_920E_4ACB_8392_7A76A9AFE11B__INCLUDED_)
#define AFX_MHEXEDIT_H__4A095310_920E_4ACB_8392_7A76A9AFE11B__INCLUDED_


#if _MSC_VER > 1000
#ifndef __BASECLASS_MHEXEDIT_H__
#define __BASECLASS_MHEXEDIT_H__

#endif // _MSC_VER > 1000
// MHexEdit.h : header file


#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif


#include "MBaseEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CMHexEdit window

class BASECLASS_DLL CMHexEdit : public CMBaseEdit
{
// Construction
public:
	CMHexEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMHexEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMHexEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMHexEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MHEXEDIT_H__4A095310_920E_4ACB_8392_7A76A9AFE11B__INCLUDED_)

#endif
