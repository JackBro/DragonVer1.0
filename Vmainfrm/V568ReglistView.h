#pragma once

#include "BasicFunc/BaseReglistView.h"

// CV568ReglistView ������ͼ

class CV568ReglistView : public CBaseReglistView
{
	DECLARE_DYNCREATE(CV568ReglistView)

protected:
	CV568ReglistView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CV568ReglistView();

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


