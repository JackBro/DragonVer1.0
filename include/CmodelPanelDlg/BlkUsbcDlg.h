#ifndef __CMODELPANEL_BLKUSBCDLG_H__
#define __CMODELPANEL_BLKUSBCDLG_H__



#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CBlkUsbcDlg �Ի���

class CMODELPANEL_DLL CBlkUsbcDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkUsbcDlg)

public:
	CBlkUsbcDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkUsbcDlg();

// �Ի�������
	enum { IDD = IDD_BLK_USBC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
