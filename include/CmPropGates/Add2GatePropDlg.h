#ifndef _CMODEL_ADD2GATE_PROPDLG_H_
#define _CMODEL_ADD2GATE_PROPDLG_H_


#ifndef CMPROPGATES_DLL
	#define CMPROPGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

typedef struct tag_TAdd2PropOption {
	int clip;
	int InUnit;
	int OutUnit;
	int opdata;
} TAdd2PropOption, *PTAdd2PropOption;

class CMPROPGATES_DLL CAdd2GatePropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CAdd2GatePropDlg)

public:
	CAdd2GatePropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdd2GatePropDlg();

// 对话框数据
	enum { IDD = IDD_ADD2GATE_BOXPROP_DLG };

protected:
	virtual void UpdateOption(void);

private:
	void DispUserOption(void);
	void GetUserOption(void);
	void InitUserOption(void);

private:
	TAdd2PropOption	m_option;

	CMDecEdit m_clipEdit;
	CMDecEdit m_opdataEdit;
	CMComboBox m_InUnitBox;
	CMComboBox m_OutUnitBox;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
public:
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeAddClipEdit();
	afx_msg void OnCbnSelchangeInunitCombo();
	afx_msg void OnCbnSelchangeAdd2InunitCombo();
};


#endif
