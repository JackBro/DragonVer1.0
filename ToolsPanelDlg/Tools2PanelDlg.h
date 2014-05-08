#ifndef __TOOLSPANEL_TOOLS2PANELDLG_H__
#define __TOOLSPANEL_TOOLS2PANELDLG_H__

#ifndef _WINDLL
#define TOOLSPANEL_DLL
#endif

#ifndef TOOLSPANEL_DLL
#define TOOLSPANEL_DLL _declspec(dllimport)
#endif


#include "../BaseClass/BasePanelDlg.h"
#include "../BaseClass/RawVideoFrm.h"

// CTools2PanelDlg 对话框

#define TOOLS2_STRING_LENGTH	20
#include "ex_ToolsPanel.h"


enum AUTOFILESELECT2enum
{
	SELECT2_NORMAL,
	SELECT2_CONVERT_FMT,
	SELECT2_RESIZE
};

typedef struct tag_TUser2Option
{
	char src[DEFAULT_FILENAME_LEN];
	char dest[DEFAULT_FILENAME_LEN];
	char src2[DEFAULT_FILENAME_LEN];
	int	 bytepdata;
	int  srcfmt;
	int  dstfmt;
	int	 width;
	int  height;
	int	 dstwidth;
	int  dstheight;
	int  pointx;
	int  pointy;
	int	 dither;
	int  src1perbyte;
	int  src2perbyte;
	int  srcbcnt;
	int  dstbcnt;
	int  opercode;
	int  srcucval;
	int  dstucval;
	int  hfactor;
	int  vfactor;
	int  filter[4];
	int  factoren;
	int  rotmode;
	int	 startfrm;
	int  endfrm;
	int  frmrate;
	int  imgopen;
	int  imgplay;
	int  imgStat[9];
	int  qfactor;
} TUser2Option, *PTUser2Option;

class TOOLSPANEL_DLL CTools2PanelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CTools2PanelDlg)

public:
	CTools2PanelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTools2PanelDlg();

// 对话框数据
	enum { IDD = IDD_TOOLS2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	CMComboBox m_dstfmt;
	CMComboBox m_bytepdata;
	CMComboBox m_srcfmt;
	CMComboBox m_srcfile2box;
	CMComboBox m_rotmodebox;

	TUser2Option m_option;
	CButton m_dither;
	CButton m_imgopenbtn;
	CButton m_imgplaybtn;

	CMDecEdit m_width;
	CMDecEdit m_height;
	CMDecEdit m_dstwidth;
	CMDecEdit m_dstheight;
	CMDecEdit m_Bmpfmt;
	CMDecEdit m_BmpWidth;
	CMDecEdit m_BmpHeight;
	CMDecEdit m_pointxBox;
	CMDecEdit m_pointyBox;

	CButton m_factoren;

	CMHexEdit m_srcucvalBox;
	CMHexEdit m_dstucvalBox;
	CMDecEdit m_qfactorBox;

	CMHexEdit m_hfactorBox;
	CMHexEdit m_vfactorBox;
	CMHexEdit m_filter0Box;
	CMHexEdit m_filter1Box;
	CMHexEdit m_filter2Box;
	CMHexEdit m_filter3Box;

	CMDecEdit m_startfrmBox;
	CMDecEdit m_endfrmBox;
	CMComboBox m_frmrateBox;

	CMDecEdit m_maxYR;
	CMDecEdit m_minYR;
	CMDecEdit m_avgYR;
	
	CMDecEdit m_maxUG;
	CMDecEdit m_minUG;
	CMDecEdit m_avgUG;

	CMDecEdit m_maxVB;
	CMDecEdit m_minVB;
	CMDecEdit m_avgVB;

	DECLARE_MESSAGE_MAP()

private:
	CRawVideoFrm m_Imgfrm;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTool2miscButton();
	afx_msg void OnBnClickedPrintbmpinfoButton();
	afx_msg void OnBnClickedCompareimgButton();
	afx_msg void OnBnClickedImgsnrButton();
	afx_msg void OnBnClickedSplitjpgButton();
	afx_msg void OnBnClickedChkjpglistButton();
	afx_msg void OnBnClickedDataconvertButton();
	afx_msg void OnBnClickedDataresizeButton();
	afx_msg void OnBnClickedDatasizerButton();
	afx_msg void OnBnClickedUnitcvtButton();
	afx_msg void OnBnClickedPrefilterButton();
	afx_msg void OnBnClickedRotateButton();
	afx_msg void OnBnClickedOpensrcfile2Btn();
	afx_msg void OnBnClickedImgplayButton();
	afx_msg void OnBnClickedImgopenButton();
	afx_msg void OnBnClickedImgstatButton();

protected:
	void InitUserOption(void);
	void GetUserOption(char * tail = FILE_TAIL_BIN);
	void AutoSetSrcfile(int select = SELECT2_NORMAL);
	void DispOption(void);
};

#endif
