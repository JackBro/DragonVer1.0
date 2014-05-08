#ifndef _CMODEL_IMAGEBOX_PROPDLG_H_
#define _CMODEL_IMAGEBOX_PROPDLG_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/BaseBoxPropDlg.h"
#include "BaseClass/MDecEdit.h"

class CMODELBASE_DLL CImageBoxPropDlg : public CBaseBoxPropDlg
{
	DECLARE_DYNAMIC(CImageBoxPropDlg)

public:
	CImageBoxPropDlg(UINT Idd, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageBoxPropDlg();

public:
	virtual void UpdateOption(void);

private:
	void	DispImagePropCtrl(void);

protected:
	CMDecEdit m_DummyEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
