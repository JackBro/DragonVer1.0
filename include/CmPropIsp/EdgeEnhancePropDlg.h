#ifndef _CMODEL_EDGEENHANCE_PROPDLG_H_
#define _CMODEL_EDGEENHANCE_PROPDLG_H_


#ifndef CMODELISPBOXPROP_DLL
	#define CMODELISPBOXPROP_DLL  __declspec(dllimport)
#endif


#include "CmodelBase/ImageBoxPropDlg.h"


class CMODELISPBOXPROP_DLL CEdgeEnhancePropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CEdgeEnhancePropDlg)

public:
	CEdgeEnhancePropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEdgeEnhancePropDlg();

	// 对话框数据
	enum { IDD = IDD_EDGEENHANCEPROP_DLG };

private:
	void DispUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
public:
	//afx_msg void OnBnClickedOk();
};

#endif