
#ifndef _CMODEL_COLORCONVERT_PROPDLG_H_
#define _CMODEL_COLORCONVERT_PROPDLG_H_


#ifndef CMODELISPBOXPROP_DLL
	#define CMODELISPBOXPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"


class CMODELISPBOXPROP_DLL CColorConvertPropDlg : public CImageBoxPropDlg
{
   DECLARE_DYNAMIC(CColorConvertPropDlg)

public:
	CColorConvertPropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CColorConvertPropDlg();

// �Ի�������
	enum { IDD = IDD_COLORCONVERTPROP_DLG };

private:
	void DispUserOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif

