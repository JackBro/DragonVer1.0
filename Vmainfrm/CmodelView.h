#pragma once

#include "BaseClass/BaseTabCtrlView.h"
#include "CmodelView/BaseCmViewDlg.h"

// CCmodelView ������ͼ

class CCmodelView : public CBaseTabCtrlView
{
	DECLARE_DYNCREATE(CCmodelView)

protected:
	CCmodelView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CCmodelView();

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

private:
	CBaseCmViewDlg	m_V568Dlg;
	CBaseCmViewDlg	m_IspDlg;
	CBaseCmViewDlg	m_IppDlg;
	CBaseCmViewDlg	m_LbufDlg;
	CBaseCmViewDlg	m_LcdcDlg;
	CBaseCmViewDlg	m_JpegEncDlg;
	CBaseCmViewDlg	m_JpegDecDlg;
	CBaseCmViewDlg	m_CpmDlg;
	CBaseCmViewDlg	m_LcdifDlg;
	CBaseCmViewDlg	m_Mpeg2EncDlg;
	CBaseCmViewDlg	m_Mpeg2DecDlg;
	CBaseCmViewDlg	m_Mpeg4EncDlg;
	CBaseCmViewDlg	m_Mpeg4DecDlg;
	CBaseCmViewDlg	m_H264EncDlg;
	CBaseCmViewDlg	m_H264DecDlg;
};


