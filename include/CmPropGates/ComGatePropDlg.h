#ifndef _CMODEL_COMGATE_PROPDLG_H_
#define _CMODEL_COMGATE_PROPDLG_H_


#ifndef CMPROPGATES_DLL
	#define CMPROPGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

class CMPROPGATES_DLL CComGatePropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CComGatePropDlg)

public:
	CComGatePropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComGatePropDlg();

// 对话框数据
	enum { IDD = IDD_COMGATE_BOXPROP_DLG };

private:
	void DispUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
