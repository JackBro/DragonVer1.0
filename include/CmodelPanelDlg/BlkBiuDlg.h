#ifndef __CMODELPANEL_BLKBIUDLG_H__
#define __CMODELPANEL_BLKBIUDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif

#include "BaseClass/BasePanelDlg.h"

// CBlkBiuDlg �Ի���

class CMODELPANEL_DLL CBlkBiuDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkBiuDlg)

public:
	CBlkBiuDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkBiuDlg();

// �Ի�������
	enum { IDD = IDD_BLK_BIU_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
