#ifndef __CMODELPANEL_BLKSIFDLG_H__
#define __CMODELPANEL_BLKSIFDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CBlkSifDlg 对话框

class CMODELPANEL_DLL CBlkSifDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkSifDlg)

public:
	CBlkSifDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkSifDlg();

// 对话框数据
	enum { IDD = IDD_BLK_SIF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	

};
#endif
