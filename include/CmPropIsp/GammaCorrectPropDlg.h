#ifndef _CMODEL_GAMMACORRECT_PROPDLG_H_
#define _CMODEL_GAMMACORRECT_PROPDLG_H_


#ifndef CMODELISPBOXPROP_DLL
	#define CMODELISPBOXPROP_DLL  __declspec(dllimport)
#endif


#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"

class CMODELISPBOXPROP_DLL CGammaCorrectPropDlg : public CImageBoxPropDlg
{
   DECLARE_DYNAMIC(CGammaCorrectPropDlg)

public:
	CGammaCorrectPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGammaCorrectPropDlg();

// 对话框数据
	enum { IDD = IDD_GAMMACORRECTPROP_DLG };

private:
	void DispUserOption(void);

private:
	CMHexEdit m_ditherEdit;
	CMHexEdit m_dithervalEdit;
	CMHexEdit m_gammaEdit[17];
	CComboBox m_gammaselCombo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
