// Tools6PanelDlg.cpp : 实现文件
//

#include "internal.h"


// CTools6PanelDlg 对话框

IMPLEMENT_DYNAMIC(CTools6PanelDlg, CBasePanelDlg)
CTools6PanelDlg::CTools6PanelDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CTools6PanelDlg::IDD, pParent)
{
}

CTools6PanelDlg::~CTools6PanelDlg()
{
}

void CTools6PanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTools6PanelDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_GAMA, OnBnClickedGama)
	ON_MESSAGE(WM_GAMMADLG_ONAPPLY, OnGammaApply)
END_MESSAGE_MAP()


// CTools6PanelDlg 消息处理程序

BOOL CTools6PanelDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CTools6PanelDlg::OnBnClickedGama()
{
	CGammaDlg dlg;
	dlg.SetTargetCWnd(this);
	dlg.DoModal();
}

LRESULT CTools6PanelDlg::OnGammaApply(WPARAM wParam , LPARAM lParam)
{
	CGammaDlg *pdlg;
	TGammaOption myOpt;
	int *myXPoints , *myYPoints;
	CSramBuf bufXPoints,bufYPoints;


	if( lParam != NULL)
	{
		pdlg= (CGammaDlg *)lParam;
		pdlg->GetGammaOpt(&myOpt);
		//pdlg->SetGammaOpt(&myOpt);
		bufXPoints.SetSize(myOpt.PNum,sizeof(int));
		bufYPoints.SetSize(myOpt.PNum,sizeof(int));
		myXPoints=(int *)bufXPoints.m_buf;
		myYPoints=(int *)bufYPoints.m_buf;
		pdlg->GetPoints(myXPoints,myYPoints,myOpt.PNum);
		//pdlg->SetPoints(myXPoints,myYPoints,myOpt.PNum);
	}
	wParam = NULL;
	return LRESULT();
}
