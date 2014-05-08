#ifndef __CMODELPANEL_BLKTVENCDLG_H__
#define __CMODELPANEL_BLKTVENCDLG_H__




#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MDecEdit.h"
#include "tooljpg/jpegencbase.h"



enum OPERATECODEenum {	
	FILTER		=	0,
	UPSAMPLE	=	1,
	CVBS_Y_C	=	2	
};
enum YUVCOMPenum {
	YUV_Y,
	YUV_U,
	YUV_V
};

//typedef struct tag_TRudebrcOption {
//	int		rude_en;
//	int		rude_level;
//	int		only_uv;
//	int		rude_ac;
//	int		rude_thd;
//	int		tcr;
//	
//	int		qfcomp0;
//	int		qfcomp1;
//	int		qfcomp2;
//	int		qfcomp3;
//	int		qfcomp4;
//	int		qfcomp5;
//
//	int		thdl1;
//	int		thdl2;
//	int		thdr1;
//	int		thdr2;
//
//	int		shiftl1;
//	int		shiftl2;
//	int		shiftr1;
//	int		shiftr2;
//}TRudebrcOption, *PTRudebrcOption;

typedef struct tag_TTvencOption {
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	char	cfgfile[DEFAULT_FILENAME_LEN];
	int		width;
	int		height;
	int		qfactor;
	int		tcr;
	int		autobrc;
	int		pal;
	int		interlace;
	int		tvopcode;
	int		colormode;
	int		bypass;
	int		average;	
	int		compon;
} TTvencOption, *PTTvencOption;

// CBlkTvencDlg 对话框

class CMODELPANEL_DLL CBlkTvencDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkTvencDlg)

public:
	CBlkTvencDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkTvencDlg();

// 对话框数据
	enum { IDD = IDD_BLK_TVENC_DLG };

private:
	TTvencOption	m_option;
	TRudebrcOption	m_rudeoption;

	CMDecEdit		m_widthbox;
	CMDecEdit		m_heightbox;
	CMDecEdit		m_qfactorbox;	
	CMComboBox		m_palbox;
	CMComboBox		m_typebox;
	CMComboBox		m_colorbox;
	CMComboBox		m_tvopcodebox;	
	CMComboBox		m_componbox;
	CButton			m_bypass;
	CButton			m_average;

	CButton			m_rudebrc;
	CButton			m_onlyuv;
	CMDecEdit		m_acnumbox;
	CMDecEdit		m_rudethdbox;
	CMDecEdit		m_tcrbox;
	CMComboBox		m_brcmodebox;
	CMDecEdit		m_qfcomp0box;
	CMDecEdit		m_qfcomp1box;
	CMDecEdit		m_qfcomp2box;
	CMDecEdit		m_qfcomp3box;
	CMDecEdit		m_qfcomp4box;
	CMDecEdit		m_qfcomp5box;
	CMDecEdit		m_thdl1box;
	CMDecEdit		m_thdl2box;
	CMDecEdit		m_thdr1box;
	CMDecEdit		m_thdr2box;
	CMDecEdit		m_shiftl1box;
	CMDecEdit		m_shiftl2box;
	CMDecEdit		m_shiftr1box;
	CMDecEdit		m_shiftr2box;

	CMComboBox		m_cfgfilebox;
	CButton			m_autobrc;


private:
	void InitUserOption(void);
	void DispOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void UpdateParam(int *width, int *height);
	void GetRudeOption(void);
	void DispRudeOption(void);
	void InitRudeOption(void);

private:
	void GetEditboxValue(void);
	void GetComboValue(char *tail);
	void GetCheckValue(void);

	void ClearUV(char *buf, int len);

	int JpegEncOperation(char *srcfile, char *dstfile, int width, int height, int qf = 40);
	int SizerOperation(void);
	int FilterOperation(void);
	int	UpsampleOperation(void);
	int CVBSOperation(void);

	int versizerbin2txt(char *filename, char *buf, int len);
	int sizerbin2txt(char *filename, char *buf, int len);
	int decbin2txt(char *filename, char *buf, int len);
	int bin2txt(char *filename, char *buf, int len, int unit = 1);
	int filterbin2txt(char *filename, char *buf, int len, int yuv);
	int	upsampbin2txt(char *filename, char *buf, int len, int yuv);

	void ParserCfg(char *str, int val);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();	
	afx_msg void OnBnClicked702convertButton();
	afx_msg void OnBnClicked702jpegencButton();
	afx_msg void OnBnClicked702jpegdecButton();
	afx_msg void OnBnClicked702ippsizerButton();
	afx_msg void OnBnClicked702ipprgbButton();
	afx_msg void OnBnClicked702tvconvertButton();
	afx_msg void OnBnClickedCheckRudebrc();

	afx_msg void OnBnClickedOpencfgfileButton();
	afx_msg void OnBnClickedLoadcfgButton();
	afx_msg void OnBnClickedSavecfgButton();
};

#endif
