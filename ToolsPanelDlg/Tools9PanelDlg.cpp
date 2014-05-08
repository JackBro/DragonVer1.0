// Tools4PanelDlg.cpp : 实现文件
//

#include "internal.h"

// CTools9PanelDlg 对话框

IMPLEMENT_DYNAMIC(CTools9PanelDlg, CBasePanelDlg)
CTools9PanelDlg::CTools9PanelDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CTools9PanelDlg::IDD, pParent)
{
}

CTools9PanelDlg::~CTools9PanelDlg()
{
}

void CTools9PanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTools9PanelDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_TOOLS9MISC_BUTTON, OnBnClickedTools9miscButton)
END_MESSAGE_MAP()


BOOL CTools9PanelDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();
	this->InitUserOption();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTools9PanelDlg::InitUserOption(void)
{
	memset( &(this->m_option), 0, sizeof(TUserT9Option) );
}
	
void CTools9PanelDlg::GetUserOption(char *tail)
{
	CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, tail);
}
	
void CTools9PanelDlg::AutoSetSrcfile(void)
{
	CBasePanelDlg::AutoSetFileBox(this->m_option.dest);
}
	
void CTools9PanelDlg::DispOption(void)
{
}


//#include "../toolkit/tooltree/treebase.h"

void CTools9PanelDlg::OnBnClickedTools9miscButton()
{
	//CTreeBase	tree, *ptree, *p1;

	//ptree = tree.AddChild(0);
	//p1 = ptree->AddChild(1);
	//tree.AddChild(1);
	//tree.AddChild(2);

}
