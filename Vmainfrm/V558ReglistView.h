#pragma once

#include "BasicFunc/BaseReglistView.h"

// CV558ReglistView ������ͼ

class CV558ReglistView : public CBaseReglistView
{
	DECLARE_DYNCREATE(CV558ReglistView)

protected:
	CV558ReglistView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CV558ReglistView();

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


