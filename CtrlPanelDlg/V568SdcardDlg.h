#ifndef __CTRLPLANE_V568SDCARDDLG_H__
#define __CTRLPLANE_V568SDCARDDLG_H__


#ifndef _WINDLL
#define CTRLPLANE_DLL
#endif

#ifndef CTRLPLANE_DLL
#define CTRLPLANE_DLL _declspec(dllimport)
#endif


#include "../BaseClass/BaseCtrlDlg.h"

// CV568SdcardDlg �Ի���

class CTRLPLANE_DLL CV568SdcardDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568SdcardDlg)

public:
	CV568SdcardDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CV568SdcardDlg();

// �Ի�������
	enum { IDD = IDD_V568SDCARD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#endif
