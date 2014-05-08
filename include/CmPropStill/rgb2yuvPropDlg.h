#ifndef _CMODEL_RGB2YUV_PROPDLG_H_
#define _CMODEL_RGB2YUV_PROPDLG_H_



#ifndef CMODELSTPROP_DLL
#define CMODELSTPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"

class CMODELSTPROP_DLL CRgb2YuvPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CRgb2YuvPropDlg)

public:
	CRgb2YuvPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRgb2YuvPropDlg();

// 对话框数据
	enum { IDD = IDD_RGB2YUV_BOXPROP_DLG };

public:
	void DispUserOption(void);

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
