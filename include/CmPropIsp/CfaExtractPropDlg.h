#ifndef _CMODEL_CFAEXTRACT_PROPDLG_H_
#define _CMODEL_CFAEXTRACT_PROPDLG_H_


#ifndef CMODELISPBOXPROP_DLL
	#define CMODELISPBOXPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"


typedef struct tag_TCfaExtractOption
{
	int  mode;
	int  femm;
	int  femp;
	int  femmin;
	int  femmax;
	int  femx1;
	int  femx2;
	int  divedge;
	char coefedge[5][5];
} TCfaExtractOption, *PTCfaExtractOption;


class CMODELISPBOXPROP_DLL CCfaExtractPropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CCfaExtractPropDlg)

public:
	CCfaExtractPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCfaExtractPropDlg();

	// 对话框数据
	enum { IDD = IDD_CFAEXTRACTPROP_DLG };

public:
	void InitUserOption(void);
	void GetUserOption(void);
	void DispUserOption(void);

protected:
	TCfaExtractOption m_option;

	CMHexEdit m_modeEdit;
	CMHexEdit m_femmEdit;
	CMHexEdit m_fempEdit;
	CMHexEdit m_femminEdit;
	CMHexEdit m_femmaxEdit;
	CMHexEdit m_femx1Edit;
	CMHexEdit m_femx2Edit;
	CMHexEdit m_divedgeEdit;
	CMHexEdit m_coefedgeEdit[5][5];


protected:
	virtual void UpdateOption(void);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnInitDialog();
};

#endif
