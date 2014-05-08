#ifndef __CMODELPANEL_BLKBIUDLG_H__
#define __CMODELPANEL_BLKBIUDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif

#include "BaseClass/BasePanelDlg.h"

// CBlkBiuDlg 对话框

class CMODELPANEL_DLL CBlkBiuDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkBiuDlg)

public:
	CBlkBiuDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkBiuDlg();

// 对话框数据
	enum { IDD = IDD_BLK_BIU_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
