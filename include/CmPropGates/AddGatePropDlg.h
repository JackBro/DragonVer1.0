#ifndef _CMODEL_ADDGATE_PROPDLG_H_
#define _CMODEL_ADDGATE_PROPDLG_H_


#ifndef CMPROPGATES_DLL
	#define CMPROPGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

typedef struct tag_TAddPropOption {
	int clip;
	int unit;
} TAddPropOption, *PTAddPropOption;

class CMPROPGATES_DLL CAddGatePropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CAddGatePropDlg)

public:
	CAddGatePropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddGatePropDlg();

// 对话框数据
	enum { IDD = IDD_ADDGATE_BOXPROP_DLG };

protected:
	virtual void UpdateOption(void);

private:
	void DispUserOption(void);
	void GetUserOption(void);

private:
	TAddPropOption	m_option;

	CMDecEdit m_clipEdit;
	CMDecEdit m_unitEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
public:
	afx_msg void OnEnChangeAddUnitEdit();
	afx_msg void OnBnClickedOk();
};


#endif
