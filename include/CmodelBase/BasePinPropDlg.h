

#ifndef _CMODEL_BASEPIN_PROPDLG_H_
#define _CMODEL_BASEPIN_PROPDLG_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "Inport.h"
#include "BaseClass/BaseDlg.h"
#include "BaseClass/MComboBox.h"

class CMODELBASE_DLL CBasePortPropDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CBasePortPropDlg)

public:
	CBasePortPropDlg(UINT Idd, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBasePortPropDlg();

public:
	void SetPort(CPortBase *port);

protected:
	CMComboBox	m_SrcfCombo;
	CPortBase	*m_port;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
};

#endif
