#pragma once

#include "BasicFunc/BaseReglistView.h"

// CV578ReglistView 窗体视图

class CV578ReglistView : public CBaseReglistView
{
	DECLARE_DYNCREATE(CV578ReglistView)

protected:
	CV578ReglistView();           // 动态创建所使用的受保护的构造函数
	virtual ~CV578ReglistView();

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
};


