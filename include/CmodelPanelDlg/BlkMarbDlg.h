#ifndef __CMODELPANEL_BLKMARBDLG_H__
#define __CMODELPANEL_BLKMARBDLG_H__

#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"



// CBlkMarbDlg 对话框

class CMODELPANEL_DLL CBlkMarbDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkMarbDlg)

public:
	CBlkMarbDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkMarbDlg();

// 对话框数据
	enum { IDD = IDD_BLK_MARB_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
