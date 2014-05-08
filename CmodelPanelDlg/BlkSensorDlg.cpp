// BlkSensorDlg.cpp : 实现文件
//

#include "internal.h"




// CBlkSensorDlg 对话框

IMPLEMENT_DYNAMIC(CBlkSensorDlg, CBasePanelDlg)
CBlkSensorDlg::CBlkSensorDlg(CWnd* pParent /*=NULL*/)
	: CBasePanelDlg(CBlkSensorDlg::IDD, pParent)
{
}

CBlkSensorDlg::~CBlkSensorDlg()
{
}

void CBlkSensorDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBlkSensorDlg, CBasePanelDlg)
END_MESSAGE_MAP()


// CBlkSensorDlg 消息处理程序
