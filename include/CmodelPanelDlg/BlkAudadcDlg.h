#ifndef __CMODELPANEL_BLKAUDADCDLG_H__
#define __CMODELPANEL_BLKAUDADCDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"
// CBlkAudadcDlg �Ի���

class CMODELPANEL_DLL CBlkAudadcDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkAudadcDlg)

public:
	CBlkAudadcDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkAudadcDlg();

// �Ի�������
	enum { IDD = IDD_BLK_AUDADC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
