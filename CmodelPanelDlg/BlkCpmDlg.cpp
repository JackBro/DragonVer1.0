// BlkCpmDlg.cpp : 实现文件
//
#include "internal.h"



// CBlkCpmDlg 对话框

IMPLEMENT_DYNAMIC(CBlkCpmDlg, CBasePanelDlg)
CBlkCpmDlg::CBlkCpmDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CBlkCpmDlg::IDD, pParent)
{
}

CBlkCpmDlg::~CBlkCpmDlg()
{
}

void CBlkCpmDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBlkCpmDlg, CBasePanelDlg)
END_MESSAGE_MAP()


// CBlkCpmDlg 消息处理程序
