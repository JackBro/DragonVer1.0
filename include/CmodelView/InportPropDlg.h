#ifndef __CMODELVIEW_INPORTPROPDLG_H__
#define __CMODELVIEW_INPORTPROPDLG_H__

#ifndef CMODELVIEW_DLL
	#define CMODELVIEW_DLL  __declspec(dllimport)
#endif


#include "CmodelBase/BasePinPropDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MDecEdit.h"


// CInportPropDlg 对话框
typedef struct tag_TInportOption {
	char srcf[DEFAULT_FILENAME_LEN];
	int fmt;
	int width;
	int height;
	int unit;
	int sc;
} TInportOption, *PTInportOption;

class CMODELVIEW_DLL CInportPropDlg : public CBasePortPropDlg
{
	DECLARE_DYNAMIC(CInportPropDlg)

public:
	CInportPropDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInportPropDlg();

// 对话框数据
	enum { IDD = IDD_INPORTPROP_DLG };
private:
	void InitUserOption(void);
	void GetUserOption(void);
	void ClearCombo(void);
	void SetFmtCombo(int fmt);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();

private:
	TInportOption	m_option;
	CButton			m_openfileBtn;
	CMComboBox		m_fmtclassCombo;
	CMComboBox		m_unitCombo;
	CMDecEdit		m_widthEdit;
	CMDecEdit		m_heightEdit;
	CMComboBox		m_fmtCombo;

public:
	afx_msg void OnBnClickedOpenfileButton();
protected:
	virtual void OnOK();
public:
	afx_msg void OnCbnSelchangeStreamclassCombo();
};

#endif
