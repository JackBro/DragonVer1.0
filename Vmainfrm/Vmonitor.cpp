// Vmonitor.cpp : 定义应用程序的类行为。
//

#include "internal.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVmonitorApp

BEGIN_MESSAGE_MAP(CVmonitorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 标准打印设置命令
//	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_CMDWIN, OnNewCmdWin)
	ON_COMMAND(ID_CONTROLPANEL, OnNewcontrolPanel)
	ON_COMMAND(ID_REGLISTWIN, OnNewRegList)
	ON_COMMAND(ID_TOOLSPANEL, OnNewToolsPanel)
	ON_COMMAND(ID_CMODELPANEL, OnNewCmodelPanel)
	ON_COMMAND(ID_V558REGLIST, OnV558reglist)
	ON_COMMAND(ID__ARM7REGLIST, OnNewArmReglist)
	ON_COMMAND(ID__USB9604REGLIST, OnNewUsbReglist)
	ON_COMMAND(ID__V938REGLIST, OnNewV938Reglist)
	ON_COMMAND(ID__V938PANEL, OnNewV938Panel)
	ON_COMMAND(ID__TXTFILE, OnNewTxtFile)
	ON_COMMAND(ID__CMODELGRAPH, OnNewCmodelPrj)
	ON_COMMAND(ID_NEW_V578REGLIST, OnNewV578reglist)
END_MESSAGE_MAP()


// CVmonitorApp 构造

CVmonitorApp::CVmonitorApp()
{
	this->m_pTxtDocTemplate = NULL;
	this->m_pCmdDocTemplate = NULL;
	this->m_pControlDocTemplate = NULL;
	this->m_pRegListDocTemplate = NULL;
	this->m_pToolsDocTemplate = NULL;
	this->m_pCmodelDocTemplate = NULL;
	this->m_pV558RegTemplate = NULL;
	this->m_pArmRegTemplate = NULL;
	this->m_pUsbRegTemplate = NULL;
	this->m_pV938RegTemplate = NULL;
	this->m_pV938PanelTemplate = NULL;
	this->m_pCmodelPrjTemplate = NULL;

	this->m_pDoc = NULL;
}

CVmonitorApp::~CVmonitorApp()
{
	GdiplusShutdown(this->m_GdiplusToken);
}
// 唯一的一个 CVmonitorApp 对象

CVmonitorApp theApp;

void CVmonitorApp::InitDocTemplate(void)
{
	this->m_pCmdDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_CMDWIN_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CVmonitorView));
	AddDocTemplate(this->m_pCmdDocTemplate);

	this->m_pTxtDocTemplate = new CMultiDocTemplate(IDR_TXTFILE_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CEditView));
	AddDocTemplate(this->m_pTxtDocTemplate);

	this->m_pCmodelPrjTemplate = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CCmodelView));
	AddDocTemplate(this->m_pCmodelPrjTemplate);

	this->m_pControlDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_CTRLPANEL_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CMorFormView));
	AddDocTemplate(this->m_pControlDocTemplate);

	this->m_pRegListDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_REGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CV568ReglistView));
	AddDocTemplate(this->m_pRegListDocTemplate);

	this->m_pToolsDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_TOOLSPANEL_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CToolsFormView));
	AddDocTemplate(this->m_pToolsDocTemplate);

	this->m_pCmodelDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_CMODELPANEL_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CCmodelFormView));
	AddDocTemplate(this->m_pCmodelDocTemplate);

	this->m_pV558RegTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_V558REGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CV558ReglistView));
	AddDocTemplate(this->m_pV558RegTemplate);

	this->m_pArmRegTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_ARMREGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CArmReglistView));
	AddDocTemplate(this->m_pArmRegTemplate);

	this->m_pUsbRegTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_USBREGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CUsbReglistView));
	AddDocTemplate(this->m_pUsbRegTemplate);

	this->m_pV578RegTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_USBREGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CV578ReglistView));
	AddDocTemplate(this->m_pV578RegTemplate);

	this->m_pV938RegTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_V938REGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CV938ReglistView));
	AddDocTemplate(this->m_pV938RegTemplate);

	this->m_pV938PanelTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_V938PANEL_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CV938FormView));
	AddDocTemplate(this->m_pV938PanelTemplate);

}


BOOL CVmonitorApp::InitInstance()
{
	GdiplusStartupInput gdiplus;

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	GdiplusStartup(&this->m_GdiplusToken, &gdiplus, NULL);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(COMPANY_KEY);
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项（包括 MRU）

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	this->InitDocTemplate();

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	
	UINT size;
	WINDOWPLACEMENT *pWP;
	if(GetProfileBinary(SETTINGS_KEY, WINDOWPLACEMENT_KEY, (LPBYTE *)&pWP, &size))
	{
		pMainFrame->SetWindowPlacement(pWP);
		delete []pWP;
	}
	else
		pMainFrame->ShowWindow(m_nCmdShow);

	//Display logo
//	CLogWnd* pWnd = new CLogWnd;
//	if(pWnd != NULL)
//	{
////		if(pWnd->Create(IDB_BITMAP_LOGO))
//		if(pWnd->Create(IDR_DAT_LOGO))
//		{
//			pWnd->ShowWindow(SW_SHOW);
//			pWnd->UpdateWindow();
//		}
//		else
//		{
//			delete pWnd;
//			pWnd = NULL;
//		}
//	}

	//CLogSplash log;
	//log.Create();
	//log.UpdateWindow();
	//Sleep(2000);
	pMainFrame->UpdateWindow();
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CVmonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CVmonitorApp 消息处理程序

void CVmonitorApp::OnNewCmdWin()
{
	CFrameWnd* pNewFrame;

	if(this->m_pCmdDocTemplate == NULL)
		return;
	if(this->m_pDoc)
	{
		pNewFrame = this->m_pCmdDocTemplate->CreateNewFrame(this->m_pDoc, NULL);
		this->m_pCmdDocTemplate->InitialUpdateFrame(pNewFrame, this->m_pDoc);
		return;
	}
	this->m_pDoc = this->m_pCmdDocTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewcontrolPanel()
{
	this->m_pControlDocTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewRegList()
{
	this->m_pRegListDocTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewToolsPanel()
{
	this->m_pToolsDocTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewCmodelPanel()
{
	this->m_pCmodelDocTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnV558reglist()
{
	this->m_pV558RegTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewArmReglist()
{
	this->m_pArmRegTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewUsbReglist()
{
	this->m_pUsbRegTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewV938Reglist()
{
	this->m_pV938RegTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewV938Panel()
{
	this->m_pV938PanelTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewTxtFile()
{
	this->m_pTxtDocTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewCmodelPrj()
{
	this->m_pCmodelPrjTemplate->OpenDocumentFile(NULL);
}

void CVmonitorApp::OnNewV578reglist()
{
	this->m_pV578RegTemplate->OpenDocumentFile(NULL);
}
