// V568SensorDlg.cpp : 实现文件
//

#include "internal.h"

#ifndef	DEVICECMD_DISABLE

// CV568SensorDlg 对话框

IMPLEMENT_DYNAMIC(CV568SensorDlg, CBaseCtrlDlg)
CV568SensorDlg::CV568SensorDlg(CWnd* pParent /*=NULL*/)
	: CBaseCtrlDlg(CV568SensorDlg::IDD, pParent)
{
}

CV568SensorDlg::~CV568SensorDlg()
{
}

void CV568SensorDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseCtrlDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CV568SensorDlg, CBaseCtrlDlg)
END_MESSAGE_MAP()


// CV568SensorDlg 消息处理程序
#else
void nullV568Sensor(void)
{
}
#endif //DEVICECMD_DISABLE
