#ifndef _CMODEL_NOTGATE_PROPDLG_H_
#define _CMODEL_NOTGATE_PROPDLG_H_


#ifndef CMPROPGATES_DLL
	#define CMPROPGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

class CMPROPGATES_DLL CNotGatePropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CNotGatePropDlg)

public:
	CNotGatePropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNotGatePropDlg();

// �Ի�������
	enum { IDD = IDD_NOTGATE_BOXPROP_DLG };

private:
	void DispUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
