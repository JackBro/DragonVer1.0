#ifndef __CMODELPANEL_BLKSDIFDLG_H__
#define __CMODELPANEL_BLKSDIFDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"


// CBlkSdifDlg �Ի���

class CMODELPANEL_DLL CBlkSdifDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkSdifDlg)

public:
	CBlkSdifDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkSdifDlg();

// �Ի�������
	enum { IDD = IDD_BLK_SDIF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
