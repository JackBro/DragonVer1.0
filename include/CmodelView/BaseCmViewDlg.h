#ifndef __CMODELVIEW_BASECMVIEWDLG_H__
#define __CMODELVIEW_BASECMVIEWDLG_H__


#ifndef CMODELVIEW_DLL
	#define CMODELVIEW_DLL  __declspec(dllimport)
#endif

#include "CmodelContainer/mapgov.h"
#include "BaseClass/BaseDlg.h"
#include "BaseClass/MComboBox.h"
#include "CmodelContainer/mapgov.h"

#include "InportPropDlg.h"
#include "OutportPropDlg.h"
#include "PinPropDlg.h"



typedef struct tag_TCmViewOption {
	char srcf[DEFAULT_FILENAME_LEN];
	int	index;
	int	inpin, oupin;
} TCmViewOption, *PTCmViewOption;



class CMODELVIEW_DLL CBaseCmViewDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CBaseCmViewDlg)

public:
	CBaseCmViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBaseCmViewDlg();

// 对话框数据
	enum { IDD = IDD_BASECMVIEW_DLG };

private:
	void InitUserOption(void);
	void DispOption(void);
	void GetUserOption(void);

private:
	CMapGov			m_map;
	CKeyTree		m_parmtree;

	CInportPropDlg	m_InpDlg;
	COutportPropDlg	m_OupDlg;
	CPinPropDlg		m_PinDlg;

private:
	TCmViewOption m_option;

private:
	CMComboBox	m_srcfbox;
	CMComboBox	m_indexbox;
	CMComboBox	m_inpinbox;
	CMComboBox	m_oupinbox;
	CMComboBox  m_classtype;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedInsertButton();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBoxrmenuProperty();
	afx_msg void OnBoxrmenuSet();
	afx_msg void OnBoxrmenuDelete();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedModulerunButton();
	afx_msg void OnCbnSelchangeClasstypeCombo();
	afx_msg void OnBnClickedSavemoduleButton();
	afx_msg void OnBnClickedLoadmoduleButton();
	afx_msg void OnBnClickedOpenmoduleButton();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};

#endif
