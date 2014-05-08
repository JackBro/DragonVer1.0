#ifndef __CMODELVIEW_OUTPORTPROPDLG_H__
#define __CMODELVIEW_OUTPORTPROPDLG_H__

#ifndef CMODELVIEW_DLL
	#define CMODELVIEW_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/BasePinPropDlg.h"

// COutportPropDlg �Ի���
typedef struct tag_TOutportOption {
	char srcf[DEFAULT_FILENAME_LEN];
	int save;
} TOutportOption, *PTOutportOption;

class CMODELVIEW_DLL COutportPropDlg : public CBasePortPropDlg
{
	DECLARE_DYNAMIC(COutportPropDlg)

public:
	COutportPropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COutportPropDlg();

// �Ի�������
	enum { IDD = IDD_OUPORTPROP_DLG };

private:
	void InitUserOption(void);
	void GetUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();

private:
	TOutportOption	m_option;
	CButton			m_saveBtn;
	CButton			m_openBtn;
	CStatic			m_descStatic;

public:
	afx_msg void OnBnClickedOpenfileButton();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedSaveCheck();
};

#endif
