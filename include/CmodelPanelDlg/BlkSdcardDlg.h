#ifndef __CMODELPANEL_BLKSDCARDDLG_H__
#define __CMODELPANEL_BLKSDCARDDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"


// CBlkSdcardDlg �Ի���

class CMODELPANEL_DLL CBlkSdcardDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkSdcardDlg)

public:
	CBlkSdcardDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkSdcardDlg();

// �Ի�������
	enum { IDD = IDD_BLK_SDCARD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
