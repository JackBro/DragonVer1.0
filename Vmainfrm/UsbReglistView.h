#pragma once

#include "BasicFunc/BaseReglistView.h"

// CUsbReglistView ������ͼ

class CUsbReglistView : public CBaseReglistView
{
	DECLARE_DYNCREATE(CUsbReglistView)

protected:
	CUsbReglistView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CUsbReglistView();

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


