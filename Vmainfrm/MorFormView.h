#pragma once

#include "BaseClass/BaseTabCtrlView.h"
		
#include "CtrlPanelDlg/HostPanelDlg.h"
#include "CtrlPanelDlg/TestFpgaDlg.h"
#include "CtrlPanelDlg/V568SdcardDlg.h"
#include "CtrlPanelDlg/V568SensorDlg.h"
#include "CtrlPanelDlg/NandFatDlg.h"
		
#include "CtrlPanelDlg/UsbChipDlg.h"
		
#include "V568Plane/V568BoardDlg.h"
#include "V568Plane/V558HostIFDlg.h"
#include "V568Plane/V558DemoDlg.h"
#include "V568Plane/V568HighapiDlg.h"
		
#include "V568Plane/V568AudioAdcDlg.h"
#include "V568Plane/V568CpmDlg.h"
#include "V568Plane/V568IppDlg.h"
#include "V568Plane/V568IspDlg.h"

#include "V568Plane/V568JpegDlg.h"
#include "V568Plane/V568LbufDlg.h"
#include "V568Plane/V568LcdcDlg.h"
#include "V568Plane/V568LcdifDlg.h"
		
#include "V568Plane/V568MarbDlg.h"
#include "V568Plane/V568SifDlg.h"
#include "V568Plane/V568UsbcDlg.h"
		
#include "V568Plane/V568SdifDlg.h"
#include "V568Plane/V568GeDlg.h"
#include "V568Plane/V568PhoneDlg.h"
		
#include "V568Plane/V568TestChipDlg.h"
#include "V568Plane/V568Test1TDlg.h"
#include "V568Plane/V568Demo1Dlg.h"
			 
// CMorFormView 窗体视图

class CMorFormView : public CBaseTabCtrlView
{
	DECLARE_DYNCREATE(CMorFormView)

protected:
	CMorFormView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMorFormView();

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
#ifndef HOSTCMD_DISABLE
	CHostPanelDlg	m_HostDlg;
	CTestFpgaDlg	m_TestDlg;
	CNandFatDlg		m_NandFatDlg;
#endif

#ifndef DEVICECMD_DISABLE
	CV568SensorDlg	m_SensorDlg;
	CV568SdcardDlg	m_SdcardDlg;
#endif

#ifndef USBCMD_DISABLE
	CUsbChipDlg		m_UsbDlg;
#endif

#ifndef V868CMD_DISABLE
	CV568SdifDlg	m_SdifDlg;
#endif

#ifndef V578CMD_DISABLE
	CV568BoardDlg	m_BoardDlg;
	CV558HostIFDlg	m_HostIFDlg;
	CV558DemoDlg	m_DemoDlg;
	CV568HighapiDlg m_HighapiDlg;
	CV568AudioAdcDlg m_AudioDlg;

	CV568CpmDlg		m_CpmDlg;
	CV568IppDlg		m_IppDlg;
	CV568IspDlg		m_IspDlg;
	CV568JpegDlg	m_JpegDlg;
	CV568LbufDlg	m_LbufDlg;
	CV568Demo1Dlg	m_Demo1Dlg;

	CV568LcdcDlg	m_LcdcDlg;
	CV568LcdifDlg	m_LcdifDlg;
	CV568MarbDlg	m_MarbDlg;
	CV568PhoneDlg	m_PhoneDlg;

	CV568SifDlg		m_SifDlg;
	CV568UsbcDlg	m_UsbcDlg;
	CV568GeDlg		m_GeDlg;
	CV568TestChipDlg m_TestChipDlg;
	CV568Test1TDlg	m_Test1TDlg;
#endif
};


