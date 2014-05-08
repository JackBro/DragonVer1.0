#ifndef __CMODELPANEL_BLKLCDCDLG_H__
#define __CMODELPANEL_BLKLCDCDLG_H__


#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"
#include "toolv326alg/lcdc.h"
#include "toolbuf/spanbuf.h"

// CBlkLcdcDlg 对话框

typedef struct tag_TBlkLcdcOption {
	char		srcfile[DEFAULT_FILENAME_LEN];
	char		dstfile[DEFAULT_FILENAME_LEN];
	int			lcdwidth;
	int			lcdheight;
	int			lcdpointx;
	int			lcdpointy;
	int			memwidth;
	int			memheight;
	int			mempointx;
	int			mempointy;
	int			alwidth;
	int			alheight;
	int			rotation;

	int			blcdpointx;
	int			blcdpointy;
	int			bmemwidth;
	int			bmempointx;
	int			bmempointy;
	int			blwidth;
	int			blheight;
	int			brotation;
	int			baddr;
	int			bformat;

	int			alphablendingen;
	int			overlayen;
	int			overlaymode;
	int			keycolor;
	int			blendratio;
	int			layersel;
	int			layerpri;
} TBlkLcdcOption, *PTBlkLcdcOption;

typedef struct tag_TBlkLcdcRotation {
	char desc[20];
	int	rot;
} TBlkLcdcRotation, *PTBlkLcdcRotation;

typedef struct tag_TBlkLcdcFormat {
	char	desc[20];
	int		format;
} TBlkLcdcFormat, *PTBlkLcdcFormat;

typedef struct tag_TBlkLcdcOverlayMode {
	char	desc[20];
	int		mode;
} TBlkLcdcOverlayMode, *PTBlkLcdcOverlayMode;

class CMODELPANEL_DLL CBlkLcdcDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkLcdcDlg)

public:
	CBlkLcdcDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkLcdcDlg();

// 对话框数据
	enum { IDD = IDD_BLK_LCDC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	TBlkLcdcOption	m_option;
	CLcdc			m_lcdc;
	CSpanBuf		m_lcdbuf;

private:
	void InitOption(void);
	void DispOption(void);
	void GetUserOption(char *tail = FILE_TAIL_DAT);
	void GetCombo(char *tail = FILE_TAIL_DAT);
	void GetEditValue(void);
	void GetCheckValue(void);
	void WriteLcdBuf(char *buf);

public:
	virtual BOOL OnInitDialog();
	CComboBox		m_cboRotation;
	CMHexEdit		m_edit_al_width;
	CMHexEdit		m_edit_al_height;
	CMHexEdit		m_edit_lcd_width;
	CMHexEdit		m_edit_lcd_height;
	CMHexEdit		m_edit_mem_width;
	CMHexEdit		m_edit_mem_height;
	CMHexEdit		m_edit_lcd_pointx;
	CMHexEdit		m_edit_lcd_pointy;
	CMHexEdit		m_edit_mem_pointx;
	CMHexEdit		m_edit_mem_pointy;

	CComboBox		m_cboRotationB;
	CComboBox		m_cboFormatB;
	CMHexEdit		m_edit_bl_width;
	CMHexEdit		m_edit_bl_height;
	CMHexEdit		m_edit_mem_width_b;
	CMHexEdit		m_edit_lcd_pointx_b;
	CMHexEdit		m_edit_lcd_pointy_b;
	CMHexEdit		m_edit_mem_pointx_b;
	CMHexEdit		m_edit_mem_pointy_b;
	CMHexEdit		m_edit_baddr;

	CMHexEdit		m_edit_keycolor;
	CMHexEdit		m_edit_blendratio;
	CComboBox		m_cboOverlayMode;
	CComboBox		m_cboLayerSel;
	CComboBox		m_cboLayerPri;
	CButton			m_overlayen;
	CButton			m_alphablendingen;

	afx_msg void OnCbnSelchangeComboRotation();
	afx_msg void OnBnClickedBtnProcess();
	afx_msg void OnCbnSelchangeComboRotationB();
	afx_msg void OnCbnSelchangeComboFormatB();
	afx_msg void OnBnClickedBtnProcessB();
	afx_msg void OnCbnSelchangeComboOverlayMode();
	afx_msg void OnCbnSelchangeComboLayerSelect();
	afx_msg void OnBnClickedBtnInitGbuf();
	afx_msg void OnBnClickedBtnLcdcProcess();
	afx_msg void OnCbnSelchangeComboLayerPri();
	afx_msg void OnBnClickedBtnUpdateB();
	afx_msg void OnBnClickedBtnOutputBmp();
};

#endif
