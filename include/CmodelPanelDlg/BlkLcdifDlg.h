#ifndef __CMODELPANEL_BLKLCDIFDLG_H__
#define __CMODELPANEL_BLKLCDIFDLG_H__



#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"


// CBlkLcdifDlg �Ի���

class CMODELPANEL_DLL CBlkLcdifDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkLcdifDlg)

public:
	CBlkLcdifDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkLcdifDlg();

// �Ի�������
	enum { IDD = IDD_BLK_LCDIF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
