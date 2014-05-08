#ifndef _CMODEL_PREDICTPROPDLG_H_
#define _CMODEL_PREDICTPROPDLG_H_


#ifndef CMBOXIMGADPCM_DLL
	#define CMBOXIMGADPCM_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

typedef struct tag_TPredictPropOption {
	int clip;
	int unit;
} TPredictPropOption, *PTPredictPropOption;

class CMBOXIMGADPCM_DLL CPredictPropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CPredictPropDlg)

public:
	CPredictPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPredictPropDlg();

// 对话框数据
	enum { IDD = IDD_ADDGATE_BOXPROP_DLG };

protected:
	virtual void UpdateOption(void);

private:
	void DispUserOption(void);
	void GetUserOption(void);

private:
	TPredictPropOption	m_option;

	CMDecEdit m_clipEdit;
	CMDecEdit m_unitEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};


#endif
