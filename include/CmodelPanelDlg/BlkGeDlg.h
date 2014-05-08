#ifndef __CMODELPANEL_BLKGEDLG_H__
#define __CMODELPANEL_BLKGEDLG_H__



#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif

#include "BaseClass/BasePanelDlg.h"
#include "toolv326alg/ge.h"
#include "BaseClass/MHexEdit.h"
#include "BaseClass/MComboBox.h"


// CBlkGeDlg 对话框

typedef struct tag_TBlkGeOption {
	char			srcfile[DEFAULT_FILENAME_LEN];
	char			dstfile[DEFAULT_FILENAME_LEN];
	char			srcfile2[DEFAULT_FILENAME_LEN];
	int		dstcolordepth;
	int		srccolordepth;
	int		patterncolordepth;
	int		width;
	int		height;
	int		patternbg;
	int		patternfg;
	int		srcbg;
	int		srcfg;
	int		patternmask;
	int		srcmask;
	int		bitwise;
	int		colortrnasen;
	int		colortranssel;
	int		srcextend;
	int		startpoint;
	int		dststart;
	int		srcstart;
	int		span;
	int		linecolor;
	TSize	start;
	TSize	end;
	TSize	clipleft;
	TSize	clipright;
} TBlkGeOption, *PTBlkGeOption;

typedef struct tag_TBlkGeColorDepth {
	char	desc[20];
	int		depth;
} TBlkGeColorDepth, *PTBlkGeColorDepth;

typedef struct tag_TBlkGeStartPoint {
	char	desc[20];
	int		startpoint;
} TBlkGeStartPoint, *PTBlkGeStartPoint;

class CMODELPANEL_DLL CBlkGeDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkGeDlg)

public:
	CBlkGeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkGeDlg();

// 对话框数据
	enum { IDD = IDD_BLK_GE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	TBlkGeOption	m_option;
	CGe ge;

private:
	void InitOption(void);
	void DispOption(void);
	void GetUserOption(char *tail = FILE_TAIL_DAT);
	void GetCombo(char *tail = FILE_TAIL_DAT);
	void GetEditValue(void);
	void GetCheckValue(void);

public:
	virtual BOOL OnInitDialog();
	CMHexEdit	m_edit_width;
	CMHexEdit	m_edit_height;
	CMHexEdit	m_edit_bitwise;
	CMHexEdit	m_edit_patternbg;
	CMHexEdit	m_edit_patternfg;
	CMHexEdit	m_edit_srcbg;
	CMHexEdit	m_edit_srcfg;
	CMHexEdit	m_edit_colortranssel;
	CMHexEdit	m_edit_dst_start;
	CMHexEdit	m_edit_src_start;
	CMHexEdit	m_edit_span;
	CMHexEdit	m_edit_linecolor;
	CMHexEdit	m_edit_start_x;
	CMHexEdit	m_edit_start_y;
	CMHexEdit	m_edit_end_x;
	CMHexEdit	m_edit_end_y;
	CMHexEdit	m_edit_clip_left_x;
	CMHexEdit	m_edit_clip_left_y;
	CMHexEdit	m_edit_clip_right_x;
	CMHexEdit	m_edit_clip_right_y;
	CButton		m_pattern_mask;
	CButton		m_src_mask;
	CButton		m_color_trans_en;
	CButton		m_src_extend;
	CComboBox	m_cbo_dst_colordepth;
	CComboBox	m_cbo_src_colordepth;
	CComboBox	m_cbo_pattern_colordepth;
	CComboBox	m_cbo_start_point;
	CMComboBox	m_cbo_src_file2;

	afx_msg void OnBnClickedBtnOpenfile();
	afx_msg void OnBnClickedBtnBitblt();
	afx_msg void OnBnClickedBtnInitGe();
	afx_msg void OnBnClickedBtnLineDraw();
};

#endif
