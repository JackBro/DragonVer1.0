#ifndef __CMODELPANEL_BLKSIFDLG_H__
#define __CMODELPANEL_BLKSIFDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CBlkSifDlg �Ի���

class CMODELPANEL_DLL CBlkSifDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkSifDlg)

public:
	CBlkSifDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkSifDlg();

// �Ի�������
	enum { IDD = IDD_BLK_SIF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	

};
#endif
