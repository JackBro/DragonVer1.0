
#ifndef _CMODEL_HSIZER_PROPDLG_H_
#define _CMODEL_HSIZER_PROPDLG_H_


#ifndef CMODELIPPPROP_DLL
	#define CMODELIPPPROP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/ImageBoxPropDlg.h"


class CMODELIPPPROP_DLL CHsizerBoxPropDlg : public CImageBoxPropDlg
{
	DECLARE_DYNAMIC(CHsizerBoxPropDlg)

public:
	CHsizerBoxPropDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHsizerBoxPropDlg();

	// �Ի������� 
	enum { IDD = IDD_HSIZERPROP_DLG };

private:
	//void InitUserOption(void);
	//void GetUserOption(void);
	void DispUserOption(void);

//protected:
//	virtual void UpdateOption(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnInitDialog();
};












#endif