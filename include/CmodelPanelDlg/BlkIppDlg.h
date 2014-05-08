#ifndef __CMODELPANEL_BLKIPPDLG_H__
#define __CMODELPANEL_BLKIPPDLG_H__





#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif

#include "BaseClass/BasePanelDlg.h"



// CBlkIppDlg 对话框

typedef struct tag_TBlkIppOption {
	char			srcfile[DEFAULT_FILENAME_LEN];
	char			dstfile[DEFAULT_FILENAME_LEN];
	char			srcfile2[DEFAULT_FILENAME_LEN];
	int				spe_effect;
	int				uoffset;
	int				voffset;
	int				udown;
	int				uup;
	int				vdown;
	int				vup;
	int				width;
	int				height;
	int				patternwidth;
	int				patternheight;
	int				rawdata;
	int				unit;
	int				outputbmp;
	int				shift_maxmove;
	int				tv_deg;
	int				srcwidth;
	int				srcheight;
	int				dstwidth;
	int				dstheight;
} TBlkIppOption, *PTBlkIppOption;

typedef struct tag_TBlkIppSpe {
	char desc[20];
	int	spe;
} TBlkIppSpe, *PTBlkIppSpe;

class CMODELPANEL_DLL CBlkIppDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkIppDlg)

public:
	CBlkIppDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkIppDlg();

// 对话框数据
	enum { IDD = IDD_BLK_IPP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	TBlkIppOption m_option;
	CComboBox m_cboSpeEffect;
	CMComboBox m_cboSrcFile2;

private:
	void InitOption(void);
	void DispOption(void);
	void GetUserOption(char *tail = FILE_TAIL_DAT);
	void GetCombo(char *tail = FILE_TAIL_DAT);
	void GetBarValue(void);
	void GetEditValue(void);
	void GetCheckValue(void);
	void AdjustBarValue(void);
	int	 MaxDiv(int a, int b);
	int		GetCeil(int a, int b);
	int		GetFloor(int a, int b);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboSpecialEffect();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_bar_u_offset;
	CScrollBar m_bar_v_offset;
	CScrollBar m_bar_u_down;
	CScrollBar m_bar_u_up;
	CScrollBar m_bar_v_down;
	CScrollBar m_bar_v_up;
	CEdit m_edit_u_offset;
	CEdit m_edit_v_offset;
	CEdit m_edit_u_down;
	CEdit m_edit_u_up;
	CEdit m_edit_v_down;
	CEdit m_edit_v_up;
	CMHexEdit m_edit_patternwidth;
	CMHexEdit m_edit_patternheight;
	CMHexEdit m_edit_width;
	CMHexEdit m_edit_height;
	CMHexEdit m_edit_unit;
	CMHexEdit m_edit_shift_maxmove;
	CMHexEdit m_edit_tv_deg;
	CMHexEdit m_edit_src_width;
	CMHexEdit m_edit_src_height;
	CMHexEdit m_edit_dst_width;
	CMHexEdit m_edit_dst_height;
	CButton		m_rawdata;
	CButton		m_outputbmp;
	afx_msg void OnBnClickedButtonApplySpecial();

	afx_msg void OnBnClickedBtnDocmodule();
	afx_msg void OnBnClickedBtnOpenfile();
	afx_msg void OnBnClickedBtnNewCmodule();
	afx_msg void OnEnChangeEditImageWidth();
	afx_msg void OnBnClickedBtnSizer();
};

#endif
