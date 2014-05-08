#ifndef __BASECLASS_REGLISTCTRL_H__
#define __BASECLASS_REGLISTCTRL_H__



#include "basectrl.h"
#include "toolparse/batchparse.h"
#include "BaseClass/MHexEdit.h"
#include "BaseClass/MToolTip.h"


enum REGLISTINDEXenum
{
	REGLIST_REGTYPE,
	REGLIST_REGNAME,
	REGLIST_REGADDR,
	REGLIST_VALHEX,
	REGLIST_VALDEC,
	REGLIST_VALBINERY,
	REGLIST_DESC,
	REGILST_NOTES,
};

// CRegListCtrl

class CRegListCtrl : public CListCtrl, protected CBaseCtrl
{
	DECLARE_DYNAMIC(CRegListCtrl)

public:
	CRegListCtrl();
	virtual ~CRegListCtrl();
	
public:
	void	InitRegList(void);
	void	SetContent(PTRegUnit pContent, int count);
	void	SetUnitItem(int index, int val);
	void	SetUnitItem(int index, PTRegUnit pContent);
	void	GetUnitItem(int index, PTRegUnit pContent);

protected:
	void	ModifyRegVal(void);
	void	ClearContext(void);

private:
	int		m_item;
	int		m_subitem;
//	int		m_tipindex;
	PTRegUnit	m_pRegUnit;
	CMHexEdit m_unit;
	CMToolTip m_tip;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};



#endif
