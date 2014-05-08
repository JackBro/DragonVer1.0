#ifndef __CMODELPANEL_BLKCPMDLG_H__
#define __CMODELPANEL_BLKCPMDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"


// CBlkCpmDlg 对话框

class CMODELPANEL_DLL CBlkCpmDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkCpmDlg)

public:
	CBlkCpmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkCpmDlg();

// 对话框数据
	enum { IDD = IDD_BLK_CPM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
