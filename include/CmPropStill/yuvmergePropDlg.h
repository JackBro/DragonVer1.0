#ifndef _CMODEL_YUVMERGE_PROPDLG_H_
#define _CMODEL_YUVMERGE_PROPDLG_H_



#ifndef CMODELSTPROP_DLL
#define CMODELSTPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

class CMODELSTPROP_DLL CYuvMergePropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CYuvMergePropDlg)

public:
	CYuvMergePropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CYuvMergePropDlg();

// �Ի�������
	enum { IDD = IDD_YUVMERGE_BOXPROP_DLG };

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
