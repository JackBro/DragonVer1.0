#ifndef __BASECLASS_BASECTRLDLG_H__
#define __BASECLASS_BASECTRLDLG_H__


// CBaseCtrlDlg �Ի���


#include "BaseClass/BasePanelDlg.h"
#include "basectrl.h"

class CBaseCtrlDlg : public CBasePanelDlg, protected CBaseCtrl
{
	DECLARE_DYNAMIC(CBaseCtrlDlg)

public:
	CBaseCtrlDlg(UINT Idd, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBaseCtrlDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
