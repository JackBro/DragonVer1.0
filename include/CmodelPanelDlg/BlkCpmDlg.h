#ifndef __CMODELPANEL_BLKCPMDLG_H__
#define __CMODELPANEL_BLKCPMDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"


// CBlkCpmDlg �Ի���

class CMODELPANEL_DLL CBlkCpmDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkCpmDlg)

public:
	CBlkCpmDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkCpmDlg();

// �Ի�������
	enum { IDD = IDD_BLK_CPM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
