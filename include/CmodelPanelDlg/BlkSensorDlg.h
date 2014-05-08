#ifndef __CMODELPANEL_BLKSENSORDLG_H__
#define __CMODELPANEL_BLKSENSORDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CBlkSensorDlg �Ի���

class CMODELPANEL_DLL CBlkSensorDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkSensorDlg)

public:
	CBlkSensorDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkSensorDlg();

// �Ի�������
	enum { IDD = IDD_BLK_SENSOR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
