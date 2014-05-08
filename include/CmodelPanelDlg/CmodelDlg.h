#ifndef __CMODELPANEL_CMODELDLG_H__
#define __CMODELPANEL_CMODELDLG_H__



#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CCmodelDlg �Ի���

class CMODELPANEL_DLL CCmodelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CCmodelDlg)

public:
	CCmodelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCmodelDlg();

// �Ի�������
	enum { IDD = IDD_CMODEL_DLG };

private:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartcmodelBtn();
	afx_msg void OnBnClickedPausecmodelBtn();
	afx_msg void OnBnClickedStopcmodelBtn();
	afx_msg void OnBnClickedCmodelmiscBtn();
	virtual BOOL OnInitDialog();
};

#endif
