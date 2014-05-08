// MainFrm.cpp : CMainFrame 类的实现
//

#include "internal.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define WM_CHANNELRCV_DATA	(WM_USER+2)

CMainFrame *gFrame = NULL;


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_COMSETTING, OnComSet)
	ON_MESSAGE(WM_USBSETTING, OnUsbSet)
	ON_MESSAGE(WM_CHANNELRCV_DATA, OnComData)
	ON_COMMAND(ID_COM_SETTING, OnComSetting)
	ON_COMMAND(ID_USB_SETTING, OnUsbSetting)
	ON_COMMAND(ID_SIMCMD_CONNECT, OnSimcmdConnect)
	ON_COMMAND(ID_UART_CONNECT, OnUartConnect)
	ON_COMMAND(ID_SIMREG_CONNECT, OnSimregConnect)
	ON_COMMAND(ID_SAVEINSTRUCTION, OnSaveinstruction)
	ON_COMMAND(ID_SAVEHIGHINST, OnSavehighinst)
	ON_UPDATE_COMMAND_UI(ID_UART_CONNECT, OnUpdateUartConnect)
	ON_UPDATE_COMMAND_UI(ID_USB_CONNECT, OnUpdateUsbConnect)
	ON_UPDATE_COMMAND_UI(ID_SIMREG_CONNECT, OnUpdateSimregConnect)
	ON_COMMAND(ID_USB_CONNECT, OnUsbConnect)
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static void FrameCallBack(void)
{
	if(gFrame)
		gFrame->SendMessage(WM_CHANNELRCV_DATA);
	//	gFrame->OnRecvData();
}

static void	dispCont(char *buf, int size)
{
	if(gFrame)
		gFrame->DisplayContent(buf, size);
}

static void	dispProg(int rest, int total)
{
	if(gFrame)
		gFrame->SetProgress(rest, total);
}

// CMainFrame 构造/销毁


CMainFrame::CMainFrame()
{
	this->m_bAutoMenuEnable = FALSE;
	gFrame = this;
	GetCurrentDirectory(0x100, gCurDirectory);
	CopyCurPath(gCurDirectory);
	gComChnl.SetRecordPath(gCurDirectory);
	gComChnl.SetCallBack(FrameCallBack);
	gComChnl.SetDispCall(dispCont);
	gComChnl.SetProgCall(dispProg);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

//	LoadBarState(VMONITOR_BAR);

	//this->m_toolbar.Create(this, IDD_CMTOOLBAR_DLGBAR, CBRS_TOP, 65535);
	//this->m_toolbar.UpdateData(FALSE);
	//this->m_toolbar.EnableDocking(CBRS_ORIENT_HORZ);
	//this->DockControlBar(&this->m_toolbar);
	
	//this->OnUartConnect();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(gComChnl.IsOpened())
		gComChnl.Close();
	return CMDIFrameWnd::DestroyWindow();
}


#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnClose()
{
	//WINDOWPLACEMENT wp;

	//SaveBarState(VMONITOR_BAR);
	//GetWindowPlacement(&wp);
	//AfxGetApp()->WriteProfileBinary(SETTINGS_KEY, WINDOWPLACEMENT_KEY, 
	//	(LPBYTE)&wp, sizeof(WINDOWPLACEMENT));
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnComSetting()
{
	this->m_ComSetDlg.m_Baud = gComChnl.GetBaud();
	this->m_ComSetDlg.m_Port = gComChnl.GetPort();
	this->m_ComSetDlg.DoModal();
}

void CMainFrame::OnUsbSetting()
{
	this->m_UsbSetDlg.m_Pipe = gComChnl.GetPipe();
	this->m_UsbSetDlg.DoModal();
}

LRESULT CMainFrame::OnComSet(WPARAM wParam, LPARAM lParam)
{
	int baud, port;

	wParam = lParam;
	baud = this->m_ComSetDlg.m_Baud;
	port = this->m_ComSetDlg.m_Port;
	gComChnl.SetupBaud(baud);
	gComChnl.SetPort(port);
	return 0;
}

LRESULT CMainFrame::OnUsbSet(WPARAM wParam, LPARAM lParam)
{
	int pipe;

	wParam = lParam;

	pipe = this->m_UsbSetDlg.m_Pipe;
	gComChnl.SetPipe(pipe);

	return 0;
}

void CMainFrame::SetProgress(int rest, int total)
{
	if(rest == 0)
	{
		this->m_wndStatusBar.SetProgress(MAX_PROGRESS);
		this->m_wndStatusBar.SetProgress(MIN_PROGRESS);
		return;
	}
	rest = total - rest;
	rest *= MAX_PROGRESS;
	rest /= total;
	this->m_wndStatusBar.SetProgress(rest);
}

void CMainFrame::DisplayContent(char *buf, int size)
{
	CVmonitorApp *pApp = NULL;

	if(size == 0)
		return;
	pApp = (CVmonitorApp *)AfxGetApp();
	if(!pApp)
		return;
	if(pApp->m_pDoc)
		((CVmonitorDoc *)pApp->m_pDoc)->DisplayContent(buf, size);
}

void CMainFrame::OnRecvData(void)
{
	int size = this->m_pool.GetTotalSize();

	if(gComChnl.GetRWmode() != CALLBACK_MODE)
		return;
	size = gComChnl.Read(this->m_pool.m_buf, size);
	this->DisplayContent(this->m_pool.m_buf, size);
}

LRESULT CMainFrame::OnComData(WPARAM wParam, LPARAM lParam)
{
	wParam = lParam;

	this->OnRecvData();
	return 0;
}

void CMainFrame::ChangeMenuItem(int chnl)
{
	int i;
	const int map[] =
	{
		ID_SIMCMD_CONNECT, ID_UART_CONNECT, ID_SIMREG_CONNECT,ID_USB_CONNECT
	};
	CMenu *pMainMenu = NULL;

	pMainMenu = this->GetMenu();
	if(!pMainMenu)
		return;
	if(chnl > CHANNEL_USB)
		chnl = CHANNEL_UART;

	if(MF_CHECKED == pMainMenu->GetMenuState(map[chnl], MF_BYCOMMAND) )
	{
		gComChnl.Close();
		pMainMenu->CheckMenuItem(map[chnl], MF_UNCHECKED);
	}
	else
	{
		gComChnl.ChannelSelect(chnl);
		gComChnl.Open();
		for(i = 0; i < sizeof(map) / sizeof(int); i++)
		{
			if(i == chnl)
			{
				if(gComChnl.IsOpened())
				{
					pMainMenu->CheckMenuItem(map[chnl], MF_CHECKED);
				}
			}
			else
			{
				pMainMenu->CheckMenuItem(map[i], MF_UNCHECKED);
			}
		}
	}
}

void CMainFrame::OnSimcmdConnect()
{
//	this->ChangeMenuItem(CHANNEL_CMODEL);
}

void CMainFrame::OnUartConnect()
{
	this->ChangeMenuItem(CHANNEL_UART);
}

void CMainFrame::OnSimregConnect()
{
	this->ChangeMenuItem(CHANNEL_SIMCMD);
}

void CMainFrame::OnSaveinstruction()
{
	CMenu *pMainMenu = NULL;

	pMainMenu = this->GetMenu();
	if(!pMainMenu)
		return;
	if(MF_CHECKED == pMainMenu->GetMenuState(ID_SAVEINSTRUCTION, MF_BYCOMMAND) )
	{
		gComChnl.StopRecordRegCmd();
		pMainMenu->CheckMenuItem(ID_SAVEINSTRUCTION, MF_UNCHECKED);
	}
	else
	{
		gComChnl.StartRecordRegCmd();
		pMainMenu->CheckMenuItem(ID_SAVEINSTRUCTION, MF_CHECKED);
	}
}

void CMainFrame::OnSavehighinst()
{
	CMenu *pMainMenu = NULL;

	pMainMenu = this->GetMenu();
	if(!pMainMenu)
		return;
	if(MF_CHECKED == pMainMenu->GetMenuState(ID_SAVEHIGHINST, MF_BYCOMMAND) )
	{
		gComChnl.StopRecordCmd();
		pMainMenu->CheckMenuItem(ID_SAVEHIGHINST, MF_UNCHECKED);
	}
	else
	{
		gComChnl.StartRecordCmd();
		pMainMenu->CheckMenuItem(ID_SAVEHIGHINST, MF_CHECKED);
	}
}

void CMainFrame::OnUpdateUartConnect(CCmdUI *pCmdUI)
{
	if(gComChnl.IsOpened() && (gComChnl.GetChannelSel() == CHANNEL_UART) )
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnUpdateSimregConnect(CCmdUI *pCmdUI)
{
	if(gComChnl.IsOpened() && (gComChnl.GetChannelSel() == CHANNEL_SIMCMD) )
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnUpdateUsbConnect(CCmdUI *pCmdUI)
{
	if(gComChnl.IsOpened() && (gComChnl.GetChannelSel() == CHANNEL_USB) )
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnUsbConnect()
{
	this->ChangeMenuItem(CHANNEL_USB);
}
