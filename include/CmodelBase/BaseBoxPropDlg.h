#ifndef _CMODEL_BASEBOX_PROPDLG_H_
#define _CMODEL_BASEBOX_PROPDLG_H_



#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "baseunit.h"
#include "BaseClass/BaseDlg.h"
#include "BaseClass/MComboBox.h"

class CMODELBASE_DLL CBaseBoxPropDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CBaseBoxPropDlg)

public:
	CBaseBoxPropDlg(UINT Idd, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaseBoxPropDlg();

public:
	void SetUnit(void *unit);

protected:
	virtual void UpdateOption(void);

private:
	void	DispBasePropCtrl(void);

protected:
	CBaseUnit	*m_pUnit;
	CMComboBox	m_AlgTypeCombo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

public:
	virtual BOOL OnInitDialog();
};

#endif


