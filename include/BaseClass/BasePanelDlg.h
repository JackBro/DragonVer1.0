#ifndef __BASECLASS_BASEPANELDLG_H__
#define __BASECLASS_BASEPANELDLG_H__



#ifndef BASECLASS_DLL
	#define BASECLASS_DLL  __declspec(dllimport)
#endif

#include "BaseDlg.h"
#include "toolchar/stringtools.h"
#include "BaseClass/MComboBox.h"

class BASECLASS_DLL CBasePanelDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CBasePanelDlg)

public:
	CBasePanelDlg(UINT Idd, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBasePanelDlg();

protected:
	void	DispFileCtrl(void);
	void	GetFileBoxOption(char *src, char *dest, char *tail = FILE_TAIL_BIN);
	void	AutoSetFileBox(char *dest);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickOpensrcfileBtn();
	afx_msg void OnBnClickOpendestfileBtn();
	afx_msg void OnBnClickUpfileBtn();
	afx_msg void OnBnClickAutofileCheck();
	afx_msg void OnCbnChangeSrcfCombo();

protected:
	CMComboBox	m_SrcfCombo;
	CMComboBox	m_DestfCombo;
	CButton		m_AutoSelChkBtn;
	CButton		m_OpenSrcfBtn;
	CButton		m_OpenDstfBtn;
	CButton		m_UpfBtn;
	CStatic		m_SrcfStatic;
	CStatic		m_DstfStatic;
	CStatic		m_flenStatic;
	CStatic		m_filelenStatic;

public:
	virtual BOOL OnInitDialog();
};


#endif
