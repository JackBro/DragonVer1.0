#pragma once

#include "BasicFunc/BaseReglistView.h"

// CV578ReglistView ������ͼ

class CV578ReglistView : public CBaseReglistView
{
	DECLARE_DYNCREATE(CV578ReglistView)

protected:
	CV578ReglistView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CV578ReglistView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


