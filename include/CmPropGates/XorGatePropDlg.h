#ifndef _CMODEL_XORGATE_PROPDLG_H_
#define _CMODEL_XORGATE_PROPDLG_H_


#ifndef CMPROPGATES_DLL
	#define CMPROPGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

class CMPROPGATES_DLL CXorGatePropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CXorGatePropDlg)

public:
	CXorGatePropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CXorGatePropDlg();

// 对话框数据
	enum { IDD = IDD_XORGATE_BOXPROP_DLG };

private:
	void DispUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};


#endif
