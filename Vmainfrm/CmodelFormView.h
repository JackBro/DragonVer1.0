#pragma once

#include "BaseClass/BaseTabCtrlView.h"

#include "CmodelPanelDlg/OldCmodelDlg.h"
#include "CmodelPanelDlg/CmodelDlg.h"

#include "CmodelPanelDlg/BlkAudadcDlg.h"
#include "CmodelPanelDlg/BlkBiuDlg.h"
#include "CmodelPanelDlg/BlkCpmDlg.h"
#include "CmodelPanelDlg/BlkIppDlg.h"
#include "CmodelPanelDlg/BlkIspDlg.h"
#include "CmodelPanelDlg/BlkJpegDlg.h"
#include "CmodelPanelDlg/BlkLbufDlg.h"
#include "CmodelPanelDlg/BlkLcdcDlg.h"
#include "CmodelPanelDlg/BlkLcdifDlg.h"
#include "CmodelPanelDlg/BlkMarbDlg.h"
#include "CmodelPanelDlg/BlkSdcardDlg.h"
#include "CmodelPanelDlg/BlkSdifDlg.h"
#include "CmodelPanelDlg/BlkSensorDlg.h"
#include "CmodelPanelDlg/BlkSifDlg.h"
#include "CmodelPanelDlg/BlkUsbcDlg.h"
#include "CmodelPanelDlg/BlkTvencDlg.h"
#include "CmodelPanelDlg/BlkGeDlg.h"

// CCmodelFormView 窗体视图

class CCmodelFormView : public CBaseTabCtrlView
{
	DECLARE_DYNCREATE(CCmodelFormView)

protected:
	CCmodelFormView();           // 动态创建所使用的受保护的构造函数
	virtual ~CCmodelFormView();

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

private:
	COldCmodelDlg	m_OldCmodelDlg;
	CCmodelDlg		m_CmodelDlg;
	CBlkAudadcDlg	m_AudadcDlg;
	CBlkBiuDlg		m_BiuDlg;
	CBlkCpmDlg		m_CpmDlg;
	CBlkIppDlg		m_IppDlg;
	CBlkIspDlg		m_IspDlg;
	CBlkJpegDlg		m_JpegDlg;
	CBlkLbufDlg		m_LbufDlg;
	CBlkLcdcDlg		m_LcdcDlg;
	CBlkLcdifDlg	m_LcdifDlg;
	CBlkMarbDlg		m_MarbDlg;
	CBlkSdcardDlg	m_SdcardDlg;
	CBlkSdifDlg		m_SdifDlg;
	CBlkSensorDlg	m_SensorDlg;
	CBlkSifDlg		m_SifDlg;
	CBlkUsbcDlg		m_UsbcDlg;
	CBlkTvencDlg	m_TvencDlg;
	CBlkGeDlg		m_GeDlg;
};


