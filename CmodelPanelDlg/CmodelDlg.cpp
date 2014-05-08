// CmodelDlg.cpp : 实现文件
//
#include "internal.h"


#pragma comment (lib,"gdiplus")
// CCmodelDlg 对话框

IMPLEMENT_DYNAMIC(CCmodelDlg, CBasePanelDlg)
CCmodelDlg::CCmodelDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CCmodelDlg::IDD, pParent)
{
}

CCmodelDlg::~CCmodelDlg()
{
}

void CCmodelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCmodelDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_STARTCMODEL_BTN, OnBnClickedStartcmodelBtn)
	ON_BN_CLICKED(IDC_PAUSECMODEL_BTN, OnBnClickedPausecmodelBtn)
	ON_BN_CLICKED(IDC_STOPCMODEL_BTN, OnBnClickedStopcmodelBtn)
	ON_BN_CLICKED(IDC_CMODELMISC_BTN, OnBnClickedCmodelmiscBtn)
END_MESSAGE_MAP()


// CCmodelDlg 消息处理程序

void CCmodelDlg::OnBnClickedStartcmodelBtn()
{
//	StartCmodel();
}

void CCmodelDlg::OnBnClickedPausecmodelBtn()
{
//	PauseCmodel();
}

void CCmodelDlg::OnBnClickedStopcmodelBtn()
{
//	StopCmodel();
}

void CCmodelDlg::OnBnClickedCmodelmiscBtn()
{
//	this->RedrawWindow();
	int x = 100, y = 100;
	CDC *pDC = GetDC();
	Graphics graph(pDC->m_hDC);
	Pen greenPen((DWORD)Color::Green, 3);
	Point point1(x, y);
	Point point2(x+300, y+300);

	graph.DrawLine(&greenPen, point1, point2);
//	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
}

BOOL CCmodelDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
}
