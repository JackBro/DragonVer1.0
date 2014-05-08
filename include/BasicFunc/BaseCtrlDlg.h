#ifndef __BASECLASS_BASECTRLDLG_H__
#define __BASECLASS_BASECTRLDLG_H__


// CBaseCtrlDlg 对话框


#include "BaseClass/BasePanelDlg.h"
#include "basectrl.h"

class CBaseCtrlDlg : public CBasePanelDlg, protected CBaseCtrl
{
	DECLARE_DYNAMIC(CBaseCtrlDlg)

public:
	CBaseCtrlDlg(UINT Idd, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaseCtrlDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
