#ifndef __CMODELPANEL_BLKAUDADCDLG_H__
#define __CMODELPANEL_BLKAUDADCDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"
// CBlkAudadcDlg 对话框

class CMODELPANEL_DLL CBlkAudadcDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkAudadcDlg)

public:
	CBlkAudadcDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkAudadcDlg();

// 对话框数据
	enum { IDD = IDD_BLK_AUDADC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
