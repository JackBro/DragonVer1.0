// V568SensorDlg.cpp : ʵ���ļ�
//

#include "internal.h"

#ifndef	DEVICECMD_DISABLE

// CV568SensorDlg �Ի���

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


// CV568SensorDlg ��Ϣ�������
#else
void nullV568Sensor(void)
{
}
#endif //DEVICECMD_DISABLE
