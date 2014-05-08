#ifndef _CMODEL_YUV2YUV_PROPDLG_H_
#define _CMODEL_YUV2YUV_PROPDLG_H_



#ifndef CMODELSTPROP_DLL
#define CMODELSTPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"

typedef struct tag_TYuv2YuvPropOption {
	int outfmt;
} TYuv2YuvPropOption, *PTYuv2YuvPropOption;

class CMODELSTPROP_DLL CYuv2YuvPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CYuv2YuvPropDlg)

public:
	CYuv2YuvPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CYuv2YuvPropDlg();

// 对话框数据
	enum { IDD = IDD_YUV2YUV_BOXPROP_DLG };

public:
	void DispUserOption(void);

protected:
	void	GetUserOption(void);
	virtual void UpdateOption(void);

private:
	TYuv2YuvPropOption m_option;

private:
	CMComboBox	m_OutFmtCombo;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
