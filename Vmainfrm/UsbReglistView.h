#pragma once

#include "BasicFunc/BaseReglistView.h"

// CUsbReglistView 窗体视图

class CUsbReglistView : public CBaseReglistView
{
	DECLARE_DYNCREATE(CUsbReglistView)

protected:
	CUsbReglistView();           // 动态创建所使用的受保护的构造函数
	virtual ~CUsbReglistView();

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


