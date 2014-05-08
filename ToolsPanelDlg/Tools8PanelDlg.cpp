// Tools4PanelDlg.cpp : 实现文件
//

#include "internal.h"


// CTools8PanelDlg 对话框

IMPLEMENT_DYNAMIC(CTools8PanelDlg, CBasePanelDlg)
CTools8PanelDlg::CTools8PanelDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CTools8PanelDlg::IDD, pParent)
{
}

CTools8PanelDlg::~CTools8PanelDlg()
{
}

void CTools8PanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTools8PanelDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_LEX_ANALYSIS, OnBnClickedLexAnalysis)
	ON_BN_CLICKED(IDC_SYN_PARSER, OnBnClickedSynParser)
	ON_BN_CLICKED(IDC_HEADFILE_LIST, OnBnClickedHeadfileList)
	ON_BN_CLICKED(IDC_MAPLEX, OnBnClickedMaplex)
	ON_BN_CLICKED(IDC_MAPSYN_PARSER, OnBnClickedMapsynParser)
END_MESSAGE_MAP()


BOOL CTools8PanelDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();
	this->InitUserOption();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTools8PanelDlg::InitUserOption(void)
{
	memset( &(this->m_option), 0, sizeof(TUserT8Option) );
}
	
void CTools8PanelDlg::GetUserOption(char *tail)
{
	CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, tail);
}
	
void CTools8PanelDlg::AutoSetSrcfile(void)
{
	CBasePanelDlg::AutoSetFileBox(this->m_option.dest);
}
	
void CTools8PanelDlg::DispOption(void)
{
}

void CTools8PanelDlg::OnBnClickedLexAnalysis(void)
{
}

void CTools8PanelDlg::OnBnClickedSynParser()
{
}

void CTools8PanelDlg::OnBnClickedHeadfileList()
{
}

void CTools8PanelDlg::OnBnClickedMaplex()
{
}

void CTools8PanelDlg::OnBnClickedMapsynParser()
{
}
