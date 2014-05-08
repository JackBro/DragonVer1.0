#ifndef __TOOLSPANEL_TOOLS1PANELDLG_H__
#define __TOOLSPANEL_TOOLS1PANELDLG_H__

#ifndef _WINDLL
#define TOOLSPANEL_DLL
#endif

#ifndef TOOLSPANEL_DLL
#define TOOLSPANEL_DLL _declspec(dllimport)
#endif


#include "../BaseClass/BasePanelDlg.h"
#include "../BaseClass/RawVideoFrm.h"

// CTools1PanelDlg 对话框

#define TOOLS_STRING_LENGTH	20
#include "ex_ToolsPanel.h"


typedef struct tag_TUserOption
{
	char src[DEFAULT_FILENAME_LEN];
	char dest[DEFAULT_FILENAME_LEN];
	char src2[DEFAULT_FILENAME_LEN];
	int	 bytecount;
	int	 div;
	int	 width;
	int  height;
	int	 dstwidth;
	int  dstheight;
	int  pointx;
	int  pointy;
	int	 maxval;
	int  src1perbyte;
	int  src2perbyte;
	int  srcbcnt;
	int  dstbcnt;
	int  opercode;
	int  minval;
	int  capval;
} TUserOption, *PTUserOption;

class TOOLSPANEL_DLL CTools1PanelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CTools1PanelDlg)

public:
	CTools1PanelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTools1PanelDlg();

// 对话框数据
	enum { IDD = IDD_TOOLS1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void AutoSetSrcfile(void);
	void DispOption(void);

private:
	CMComboBox m_ByteCount;
	CMComboBox m_div;
	CMComboBox m_srcfile2box;
	CMComboBox m_srcperbyte;
	CMComboBox m_src2perbyte;
	CMComboBox m_srcbcnt;
	CMComboBox m_dstbcnt;
	CMComboBox m_operator;

	TUserOption m_option;

	CMDecEdit m_width;
	CMDecEdit m_height;
	CMDecEdit m_maxval;
	CMDecEdit m_dstwidth;
	CMDecEdit m_dstheight;
	CMDecEdit m_minval;
	CMDecEdit m_capval;
	CMDecEdit m_pointxBox;
	CMDecEdit m_pointyBox;

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedConvertButton();
	afx_msg void OnBnClickedHex2binButton();
	afx_msg void OnBnClickedCompareButton();
	afx_msg void OnBnClickedBin2hexButton();
	afx_msg void OnBnClickedDrophighButton();
	afx_msg void OnBnClickedDroplowButton();
	afx_msg void OnBnClickedCreateorderhexButton();
	afx_msg void OnBnClickedCopyButton();
	afx_msg void OnBnClickedOpensrcfile2Btn();
	afx_msg void OnBnClickedAppendFileButton();
	afx_msg void OnBnClickedMergefileButton();
	afx_msg void OnBnClickedMisctoolsButton();
	afx_msg void OnBnClickedBin2ascButton();
	afx_msg void OnBnClickedSpancvtButton();
	afx_msg void OnBnClickedSeqcvtButton();
};

#endif
