#pragma once

#include "BaseClass/BaseTabCtrlView.h"
#include "ToolsPanelDlg/Tools1PanelDlg.h"
#include "ToolsPanelDlg/Tools2PanelDlg.h"
#include "ToolsPanelDlg/Tools3PanelDlg.h"
#include "ToolsPanelDlg/Tools4PanelDlg.h"
#include "ToolsPanelDlg/Tools5PanelDlg.h"
#include "ToolsPanelDlg/Tools6PanelDlg.h"
#include "ToolsPanelDlg/Tools7PanelDlg.h"
#include "ToolsPanelDlg/Tools8PanelDlg.h"
#include "ToolsPanelDlg/Tools9PanelDlg.h"
			 
// CToolsFormView 窗体视图
			 
class CToolsFormView : public CBaseTabCtrlView
{			 
	DECLARE_DYNCREATE(CToolsFormView)
			 
protected:
	CToolsFormView();           // 动态创建所使用的受保护的构造函数
	virtual ~CToolsFormView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

private:
	CTools1PanelDlg		m_Tools1Dlg;
	CTools2PanelDlg		m_Tools2Dlg;
	CTools3PanelDlg		m_Tools3Dlg;
	CTools4PanelDlg		m_Tools4Dlg;
	CTools5PanelDlg		m_Tools5Dlg;
	CTools6PanelDlg		m_Tools6Dlg;
	CTools7PanelDlg		m_Tools7Dlg;
	CTools8PanelDlg		m_Tools8Dlg;
	CTools9PanelDlg		m_Tools9Dlg;
};


