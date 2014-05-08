#ifndef _CMODEL_YUV2RGB_PROPDLG_H_
#define _CMODEL_YUV2RGB_PROPDLG_H_



#ifndef CMODELSTPROP_DLL
#define CMODELSTPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"
#include "BaseClass/MHexEdit.h"

class CMODELSTPROP_DLL CYuv2RgbPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CYuv2RgbPropDlg)

public:
	CYuv2RgbPropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CYuv2RgbPropDlg();

// �Ի�������
	enum { IDD = IDD_YUV2RGB_BOXPROP_DLG };

public:
	void DispUserOption(void);

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};

#endif
