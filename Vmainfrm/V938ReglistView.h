#pragma once

#include "BasicFunc/BaseReglistView.h"

// CV938ReglistView ������ͼ

class CV938ReglistView : public CBaseReglistView
{
	DECLARE_DYNCREATE(CV938ReglistView)

protected:
	CV938ReglistView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CV938ReglistView();

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


