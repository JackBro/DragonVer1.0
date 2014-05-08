#ifndef __CTRLPLANE_V568SENSORDLG_H__
#define __CTRLPLANE_V568SENSORDLG_H__

#ifndef CTRLPLANE_DLL
	#define CTRLPLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"

// CV568SensorDlg 对话框

class CTRLPLANE_DLL CV568SensorDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568SensorDlg)

public:
	CV568SensorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568SensorDlg();

// 对话框数据
	enum { IDD = IDD_V568SENSOR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
