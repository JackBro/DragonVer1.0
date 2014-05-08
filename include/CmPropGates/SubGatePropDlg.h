#ifndef _CMODEL_SUBGATE_PROPDLG_H_
#define _CMODEL_SUBGATE_PROPDLG_H_


#ifndef CMPROPGATES_DLL
	#define CMPROPGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

class CMPROPGATES_DLL CSubGatePropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CSubGatePropDlg)

public:
	CSubGatePropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSubGatePropDlg();

// 对话框数据
	enum { IDD = IDD_SUBGATE_BOXPROP_DLG };

private:
	void DispUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};


#endif
