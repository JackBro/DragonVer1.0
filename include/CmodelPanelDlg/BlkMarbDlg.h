#ifndef __CMODELPANEL_BLKMARBDLG_H__
#define __CMODELPANEL_BLKMARBDLG_H__

#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"



// CBlkMarbDlg �Ի���

class CMODELPANEL_DLL CBlkMarbDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkMarbDlg)

public:
	CBlkMarbDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkMarbDlg();

// �Ի�������
	enum { IDD = IDD_BLK_MARB_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
