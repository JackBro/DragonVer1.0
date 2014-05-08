#pragma once

#include "BaseClass/BaseTabCtrlView.h"

#include "CtrlPanelDlg/HostPanelDlg.h"
#include "CtrlPanelDlg/TestFpgaDlg.h"
#include "CtrlPanelDlg/NandFatDlg.h"

#include "CtrlPanelDlg/UsbChipDlg.h"

#include "V938Plane/V938HostIFDlg.h"
#include "V938Plane/V938CpmDlg.h"
#include "V938Plane/V938SeqDlg.h"
#include "V938Plane/V938WsDlg.h"
#include "V938Plane/V938Mp3Dlg.h"
#include "V938Plane/V938DemoDlg.h"

// CV938FormView 窗体视图

class CV938FormView : public CBaseTabCtrlView
{
	DECLARE_DYNCREATE(CV938FormView)

protected:
	CV938FormView();           // 动态创建所使用的受保护的构造函数
	virtual ~CV938FormView();

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

#ifndef V938CMD_DISABLE
	CV938HostIfDlg	m_HifDlg;
	CV938CpmDlg		m_Cpm9Dlg;
	CV938SeqDlg		m_SeqDlg;
	CV938WsDlg		m_WsDlg;
	CV938Mp3Dlg		m_Mp3Dlg;
	CV938DemoDlg	m_DemoDlg938;
#endif

#ifndef USBCMD_DISABLE
	CUsbChipDlg		m_UsbDlg;
#endif
};


