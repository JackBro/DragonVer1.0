#ifndef __BASECLASS_BASETABCTRLVIEW_H__
#define __BASECLASS_BASETABCTRLVIEW_H__



#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif



#define TABHEADERLEN	20

typedef struct tag_TabContext
{
	CDialog		*pDlg;
	UINT		DlgId;
	char		Header[TABHEADERLEN];
} TabContext, *pTabContext;

#define MAXTABCOUNT	40

enum DIALOGSTATEenum {
	DIALOG_NOTCREATED	=	0,
	DIALOG_CREATED		=	1
};

class BASECLASS_DLL CBaseTabCtrlView : public CCtrlView
{
	DECLARE_DYNCREATE(CBaseTabCtrlView)

protected:
	CBaseTabCtrlView(int count = MAXTABCOUNT);
	virtual ~CBaseTabCtrlView();

protected:
	void	InitMtab(void);
	void	SetTabPos(void);

	// Attributes
public:
	CTabCtrl& GetTabCtrl() const;
	void	AddDlg(CDialog *dlg, UINT dlgId, char *header);
	int		SetCurSel(int nItem);

	//{{AFX_MSG(CBaseTabCtrlView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	TabContext	*m_pTab;
	int			m_CurDlg;
	int			m_TotalDlg;
	int			m_ValidDlg;
	int			m_state;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

#endif
