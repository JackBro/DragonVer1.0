#ifndef _CMODEL_FMTCONVERT_PROPDLG_H_
#define _CMODEL_FMTCONVERT_PROPDLG_H_



#ifndef CMODELSTPROP_DLL
#define CMODELSTPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"

typedef struct tag_TFmtcvtPropOption {
	int outfmt;
} TFmtcvtPropOption, *PTFmtcvtPropOption;

class CMODELSTPROP_DLL CFmtCvtPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CFmtCvtPropDlg)

public:
	CFmtCvtPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFmtCvtPropDlg();

// 对话框数据
	enum { IDD = IDD_FMTCVT_BOXPROP_DLG };

public:
	void DispUserOption(void);

protected:
	void	GetUserOption(void);
	virtual void UpdateOption(void);

private:
	TFmtcvtPropOption m_option;

private:
	CMComboBox	m_OutFmtCombo;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
