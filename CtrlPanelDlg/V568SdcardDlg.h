#ifndef __CTRLPLANE_V568SDCARDDLG_H__
#define __CTRLPLANE_V568SDCARDDLG_H__


#ifndef _WINDLL
#define CTRLPLANE_DLL
#endif

#ifndef CTRLPLANE_DLL
#define CTRLPLANE_DLL _declspec(dllimport)
#endif


#include "../BaseClass/BaseCtrlDlg.h"

// CV568SdcardDlg 对话框

class CTRLPLANE_DLL CV568SdcardDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568SdcardDlg)

public:
	CV568SdcardDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568SdcardDlg();

// 对话框数据
	enum { IDD = IDD_V568SDCARD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
