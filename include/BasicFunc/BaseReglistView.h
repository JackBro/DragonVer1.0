#ifndef __BASECLASS_BASEREGLISTVIEW_H__
#define __BASECLASS_BASEREGLISTVIEW_H__



#include "BaseClass/BaseTabCtrlView.h"
#include "RegListDlg.h"

class CBaseReglistView : public CBaseTabCtrlView
{
	DECLARE_DYNCREATE(CBaseReglistView)

protected:
	CBaseReglistView();           // 动态创建所使用的受保护的构造函数
	virtual ~CBaseReglistView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetRegIndex(int index);
	virtual void OnInitialUpdate();

private:
	int			m_index;
	CRegListDlg	*m_pdlg;
};



#endif
