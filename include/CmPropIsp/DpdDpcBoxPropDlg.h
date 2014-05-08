#ifndef _CMODEL_DPDDPC_PROPDLG_H_
#define _CMODEL_DPDDPC_PROPDLG_H_


#ifndef CMODELISPBOXPROP_DLL
	#define CMODELISPBOXPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"


typedef struct tag_TDpdDpcOption
{
	int dpdth;
	int noisetable[gTotalNoiseTable];
} TDpdDpcOption, *PTDpdDpcOption;

class CMODELISPBOXPROP_DLL CDpdDpcBoxPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CDpdDpcBoxPropDlg)

public:
	CDpdDpcBoxPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDpdDpcBoxPropDlg();

// 对话框数据
	enum { IDD = IDD_DPDDPCBOXPROP_DLG };

private:
	void InitUserOption(void);
	void GetUserOption(void);
	void DispUserOption(void);
private:
	TDpdDpcOption m_option;

	CMHexEdit m_dpdthdEdit;
	CMHexEdit m_noisetabEdit[17];

protected:
	virtual void UpdateOption(void);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
