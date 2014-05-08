#ifndef __CMODELPANEL_BLKSENSORDLG_H__
#define __CMODELPANEL_BLKSENSORDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CBlkSensorDlg 对话框

class CMODELPANEL_DLL CBlkSensorDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkSensorDlg)

public:
	CBlkSensorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkSensorDlg();

// 对话框数据
	enum { IDD = IDD_BLK_SENSOR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
