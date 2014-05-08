
#ifndef _CMODEL_COLORCORRECT_PROPDLG_H_
#define _CMODEL_COLORCORRECT_PROPDLG_H_


#ifndef CMODELISPBOXPROP_DLL
	#define CMODELISPBOXPROP_DLL  __declspec(dllimport)
#endif


#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"


typedef struct tag_TColorCorrectOption
{
	int colormatrix[3][3];
	int offset[3];
} TColorCorrectOption, *PTColorCorrectOption;


class CMODELISPBOXPROP_DLL CColorCorrectPropDlg : public CImageBoxPropDlg
{
   DECLARE_DYNAMIC(CColorCorrectPropDlg)

public:
	CColorCorrectPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CColorCorrectPropDlg();

// 对话框数据
	enum { IDD = IDD_COLORCORRECTPROP_DLG };

private:
	void InitUserOption(void);
	void GetUserOption(void);
	void DispUserOption(void);

private:
	TColorCorrectOption m_option;

	CMHexEdit m_colormatrixEdit[3][3];
	CMHexEdit m_offsetEdit[3];

protected:
	virtual void UpdateOption(void);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
