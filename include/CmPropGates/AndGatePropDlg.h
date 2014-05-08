#ifndef _CMODEL_ANDGATE_PROPDLG_H_
#define _CMODEL_ANDGATE_PROPDLG_H_


#ifndef CMPROPGATES_DLL
	#define CMPROPGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

class CMPROPGATES_DLL CAndGatePropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CAndGatePropDlg)

public:
	CAndGatePropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAndGatePropDlg();

// �Ի�������
	enum { IDD = IDD_ANDGATE_BOXPROP_DLG };

private:
	void DispUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};


#endif
