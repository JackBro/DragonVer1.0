#ifndef _CMODEL_BMPBOX_PROPDLG_H_
#define _CMODEL_BMPBOX_PROPDLG_H_



#ifndef CMODELSTPROP_DLL
#define CMODELSTPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"

class CMODELSTPROP_DLL CBmpboxPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CBmpboxPropDlg)

public:
	CBmpboxPropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBmpboxPropDlg();

// �Ի�������
	enum { IDD = IDD_BMP2RGB_BOXPROP_DLG };

public:
	void DispUserOption(void);

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
