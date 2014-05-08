#ifndef __CMODELPANEL_BLKLCDIFDLG_H__
#define __CMODELPANEL_BLKLCDIFDLG_H__



#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"


// CBlkLcdifDlg 对话框

class CMODELPANEL_DLL CBlkLcdifDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkLcdifDlg)

public:
	CBlkLcdifDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkLcdifDlg();

// 对话框数据
	enum { IDD = IDD_BLK_LCDIF_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

#endif
