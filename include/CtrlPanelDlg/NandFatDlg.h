#ifndef __CTRLPLANE_NANDFATDLG_H__
#define __CTRLPLANE_NANDFATDLG_H__


#ifndef CTRLPLANE_DLL
	#define CTRLPLANE_DLL _declspec(dllimport)
#endif

#include "BasicFunc/BaseCtrlDlg.h"

#define	FILE_TAIL_WDK	"wdk"

enum NANDINITenum {
		NAND_INITED,
		NAND_UNINIT
	};

typedef struct tag_TNandFatOption
{
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	int		initflag;
	int		flashtype;
	int		makerid;
	int		deviceid;
	int		nblock;
	int		npage;
	int		start;
	int		len;
} TNandFatOption, *PTNandFatOption;


// CNandFatDlg 对话框

class CTRLPLANE_DLL CNandFatDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CNandFatDlg)

public:
	CNandFatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNandFatDlg();

// 对话框数据
	enum { IDD = IDD_NANDFAT_DLG };

protected:
	void InitUserOption(void);
	void GetEditboxValue(void);
	void GetUserOption(char *tail = FILE_TAIL_DAT);
	void DispOption(void);
	void GetComboValue(char *tail = FILE_TAIL_DAT);
	void GetCheckBtnValue(void);
	void DispBarEditBox(void);
	void AdjustScrollBar(void);
	void GetBarValue(void);

private:
	TNandFatOption	m_option;

	CMHexEdit		m_makerid;
	CMHexEdit		m_devid;
	CMHexEdit		m_ndblock;
	CMHexEdit		m_ndpage;
	CMHexEdit		m_ndstart;
	CMHexEdit		m_ndlen;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRefreshButton();
	afx_msg void OnBnClickedInitnandButton();
	afx_msg void OnBnClickedDatareadButton();
	afx_msg void OnBnClickedDatawriteButton();
	afx_msg void OnBnClickedSparereadButton();
	afx_msg void OnBnClickedSparewriteButton();
	afx_msg void OnBnClickedEraseButton();
	bool CheckInitFlg(void);
	afx_msg void OnBnClickedBlkupdateButton();
	afx_msg void OnBnClickedOpenEmuButton();
	afx_msg void OnBnClickedCloseEmuButton();
	afx_msg void OnBnClickedFsMountButton();
};

#endif
