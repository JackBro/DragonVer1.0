#pragma once

#include "BasicFunc/BaseReglistView.h"

// CV568ReglistView 窗体视图

class CV568ReglistView : public CBaseReglistView
{
	DECLARE_DYNCREATE(CV568ReglistView)

protected:
	CV568ReglistView();           // 动态创建所使用的受保护的构造函数
	virtual ~CV568ReglistView();

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


