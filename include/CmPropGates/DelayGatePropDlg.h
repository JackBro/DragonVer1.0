#ifndef _CMODEL_DELAYGATE_PROPDLG_H_
#define _CMODEL_DELAYGATE_PROPDLG_H_



#ifndef CMPROPGATES_DLL
#define CMPROPGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MDecEdit.h"


typedef struct tag_TDelayOption {
	int delay;
} TDelayOption, *PTDelayOption;

class CMPROPGATES_DLL CDelayGatePropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CDelayGatePropDlg)

public:
	CDelayGatePropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDelayGatePropDlg();

// 对话框数据
	enum { IDD = IDD_DELAYGATE_BOXPROP_DLG };

protected:
	virtual void UpdateOption(void);

private:
	void DispUserOption(void);
	void GetUserOption(void);

private:
	TDelayOption	m_option;
	CMDecEdit m_delaytimeEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
