// MainFrm.h : CMainFrame 类的接口
//

#pragma once

#include "BaseClass/BasePanelDlg.h"
#include "MainFrmDlg/ComSettingDlg.h"
#include "MainFrmDlg/UsbSettingDlg.h"
#include "ComChannel/ComChannel.h"
#include "BaseClass/BaseToolBar.h"
#include "BaseClass/BaseStatusBar.h"


class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:

// 操作
public:
	void	DisplayContent(char *buf, int size);
	void	SetProgress(int rest, int total);

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void	ChangeMenuItem(int chnl);

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public: 
	afx_msg void OnClose();
	afx_msg LRESULT OnComSet(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUsbSet(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnComSetting();
	afx_msg void OnUsbSetting();
	afx_msg void OnSimcmdConnect();
	afx_msg void OnUartConnect();

public:
	void OnRecvData(void);

private:
	CComSettingDlg  m_ComSetDlg;
	CUsbSettingDlg  m_UsbSetDlg;
	CBaseStatusBar  m_wndStatusBar;
	CBaseToolBar	m_wndToolBar;
	CBufferBase		m_pool;

public:
	afx_msg void OnSimregConnect();
	afx_msg void OnSaveinstruction();
	afx_msg void OnSavehighinst();
	afx_msg void OnUpdateUartConnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUsbConnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSimregConnect(CCmdUI *pCmdUI);
	afx_msg void OnUsbConnect();
};

extern CMainFrame *gFrame;


