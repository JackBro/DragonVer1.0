#ifndef __CMODELPANEL_BLKSDIFDLG_H__
#define __CMODELPANEL_BLKSDIFDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"


// CBlkSdifDlg 对话框

class CMODELPANEL_DLL CBlkSdifDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkSdifDlg)

public:
	CBlkSdifDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkSdifDlg();

// 对话框数据
	enum { IDD = IDD_BLK_SDIF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
