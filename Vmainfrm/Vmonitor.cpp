// Vmonitor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "internal.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVmonitorApp

BEGIN_MESSAGE_MAP(CVmonitorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// ��׼��ӡ��������
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


// CVmonitorApp ����

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
// Ψһ��һ�� CVmonitorApp ����

CVmonitorApp theApp;

void CVmonitorApp::InitDocTemplate(void)
{
	this->m_pCmdDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_CMDWIN_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CVmonitorView));
	AddDocTemplate(this->m_pCmdDocTemplate);

	this->m_pTxtDocTemplate = new CMultiDocTemplate(IDR_TXTFILE_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CEditView));
	AddDocTemplate(this->m_pTxtDocTemplate);

	this->m_pCmodelPrjTemplate = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CCmodelView));
	AddDocTemplate(this->m_pCmodelPrjTemplate);

	this->m_pControlDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_CTRLPANEL_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CMorFormView));
	AddDocTemplate(this->m_pControlDocTemplate);

	this->m_pRegListDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_REGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CV568ReglistView));
	AddDocTemplate(this->m_pRegListDocTemplate);

	this->m_pToolsDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_TOOLSPANEL_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CToolsFormView));
	AddDocTemplate(this->m_pToolsDocTemplate);

	this->m_pCmodelDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_CMODELPANEL_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CCmodelFormView));
	AddDocTemplate(this->m_pCmodelDocTemplate);

	this->m_pV558RegTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_V558REGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CV558ReglistView));
	AddDocTemplate(this->m_pV558RegTemplate);

	this->m_pArmRegTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_ARMREGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CArmReglistView));
	AddDocTemplate(this->m_pArmRegTemplate);

	this->m_pUsbRegTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_USBREGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CUsbReglistView));
	AddDocTemplate(this->m_pUsbRegTemplate);

	this->m_pV578RegTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_USBREGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CV578ReglistView));
	AddDocTemplate(this->m_pV578RegTemplate);

	this->m_pV938RegTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_V938REGLIST_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CV938ReglistView));
	AddDocTemplate(this->m_pV938RegTemplate);

	this->m_pV938PanelTemplate = new CMultiDocTemplate(IDR_MAINFRAME, //IDR_V938PANEL_TYPE,
		RUNTIME_CLASS(CVmonitorDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CV938FormView));
	AddDocTemplate(this->m_pV938PanelTemplate);

}


BOOL CVmonitorApp::InitInstance()
{
	GdiplusStartupInput gdiplus;

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	GdiplusStartup(&this->m_GdiplusToken, &gdiplus, NULL);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
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
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(COMPANY_KEY);
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ����� MRU��

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	this->InitDocTemplate();

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������
	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���
	
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



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CVmonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CVmonitorApp ��Ϣ�������

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
