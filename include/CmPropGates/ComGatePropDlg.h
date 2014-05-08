#ifndef _CMODEL_COMGATE_PROPDLG_H_
#define _CMODEL_COMGATE_PROPDLG_H_


#ifndef CMPROPGATES_DLL
	#define CMPROPGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"

class CMPROPGATES_DLL CComGatePropDlg :	public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CComGatePropDlg)

public:
	CComGatePropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CComGatePropDlg();

// �Ի�������
	enum { IDD = IDD_COMGATE_BOXPROP_DLG };

private:
	void DispUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
