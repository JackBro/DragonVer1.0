#ifndef __CMODELPANEL_BLKJPEGDLG_H__
#define __CMODELPANEL_BLKJPEGDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CBlkJpegDlg �Ի���

class CMODELPANEL_DLL CBlkJpegDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkJpegDlg)

public:
	CBlkJpegDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlkJpegDlg();

// �Ի�������
	enum { IDD = IDD_BLK_JPEG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
