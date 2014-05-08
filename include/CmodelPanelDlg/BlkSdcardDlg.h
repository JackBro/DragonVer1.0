#ifndef __CMODELPANEL_BLKSDCARDDLG_H__
#define __CMODELPANEL_BLKSDCARDDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"


// CBlkSdcardDlg 对话框

class CMODELPANEL_DLL CBlkSdcardDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkSdcardDlg)

public:
	CBlkSdcardDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkSdcardDlg();

// 对话框数据
	enum { IDD = IDD_BLK_SDCARD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
