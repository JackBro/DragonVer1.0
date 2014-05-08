#pragma once

#include "BasicFunc/BaseReglistView.h"

// CArmReglistView 窗体视图

class CArmReglistView : public CBaseReglistView
{
	DECLARE_DYNCREATE(CArmReglistView)

protected:
	CArmReglistView();           // 动态创建所使用的受保护的构造函数
	virtual ~CArmReglistView();

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


