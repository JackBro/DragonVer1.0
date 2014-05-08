#ifndef _CMODEL_ARITHMGATE_PROPDLG_H_
#define _CMODEL_ARITHMGATE_PROPDLG_H_


#ifndef CMPROPGATES_DLL
	#define CMPROPGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

typedef struct tag_TArithmPropOption {
	int UpClip;
	int LowClip;
	int InUnit;
	int OutUnit;
	int opdata;
	int operation;
} TArithmPropOption, *PTArithmPropOption;


class CMPROPGATES_DLL CArithmGatePropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CArithmGatePropDlg)

public:
	CArithmGatePropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CArithmGatePropDlg();

// 对话框数据
	enum { IDD = IDD_ARITHMGATE_BOXPROP_DLG };

protected:
	virtual void UpdateOption(void);

private:
	void DispUserOption(void);
	void GetUserOption(void);
	void InitUserOption(void);

private:
	TArithmPropOption	m_option;

	CMDecEdit m_UpClipEdit;
	CMDecEdit m_LowClipEdit;
	CMDecEdit m_opdataEdit;
	CMComboBox m_InUnitBox;
	CMComboBox m_OutUnitBox;
	CMComboBox m_operationBox;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
public:
	
	afx_msg void OnBnClickedOk();

};


#endif
