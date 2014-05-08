#ifndef _CMODEL_YUVDIV_PROPDLG_H_
#define _CMODEL_YUVDIV_PROPDLG_H_



#ifndef CMODELSTPROP_DLL
#define CMODELSTPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

class CMODELSTPROP_DLL CYuvDivPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CYuvDivPropDlg)

public:
	CYuvDivPropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CYuvDivPropDlg();

// �Ի�������
	enum { IDD = IDD_YUVDIV_BOXPROP_DLG };

public:
	void DispUserOption(void);

protected:
	void	GetUserOption(void);
	virtual void UpdateOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
