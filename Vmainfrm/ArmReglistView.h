#pragma once

#include "BasicFunc/BaseReglistView.h"

// CArmReglistView ������ͼ

class CArmReglistView : public CBaseReglistView
{
	DECLARE_DYNCREATE(CArmReglistView)

protected:
	CArmReglistView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CArmReglistView();

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


