#ifndef __CMODELPANEL_BLKISPDLG_H__
#define __CMODELPANEL_BLKISPDLG_H__

#ifndef CMODELPANEL_DLL
	#define CMODELPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"

// CBlkIspDlg 对话框
#define ISPBLK_FILENAME_LENGTH	100



typedef struct tag_TIspBlkOption
{
	char src[ISPBLK_FILENAME_LENGTH];
	char dest[ISPBLK_FILENAME_LENGTH];
	int ispctrl;
	int bgline;
	int gfirst;
	int unit;
	int gain;
	int rawdata;
	int jpegheader;
	int filecount;
	int width, height;
	int dither;
	int dpdthd;
	int newdpd;
	int newedge;
	int outputfile;
	int outputbmp;
	int edgepara[25];
	int edgediv;
	int edgemax;
	unsigned int edgesum;
	int afh[5];
	int afv[5];
	int afw[16];
	int motionframecount;
	int	motionhthd;
	int	motionmthd;
	int motiondetect;
} TIspBlkOption, *PTIspBlkOption;

class CMODELPANEL_DLL CBlkIspDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CBlkIspDlg)

public:
	CBlkIspDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlkIspDlg();

// 对话框数据
	enum { IDD = IDD_BLK_ISP_DLG };

protected:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void DispUserOption(void);
	void GetCheckValue(void);
	void GetEditValue(void);
	int ParseFile(char *dirf, char *dstf);
	void CompareY(char *src1, char *src2, char *dst, int size);
	void Convert10To8(char *src, char *dst, int size);

private:
	TIspBlkOption m_option;
	CButton		m_blacklevel;
	CButton		m_lensfalloff;
	CButton		m_dpddpc;
	CButton		m_digitalgain;
	CButton		m_cfa;
	CButton		m_colorcorrect;
	CButton		m_gammacorrect;
	CButton		m_edgeenhance;
	CButton		m_rawdata;
	CButton		m_jpegheader;
	CButton		m_dither;
	CButton		m_newdpd;
	CButton		m_newedge;
	CButton		m_colorconvert;
	CButton		m_outputfile;
	CButton		m_outputbmp;
	CButton		m_autofocus;
	CMHexEdit	m_bgline;
	CMHexEdit	m_gfirst;
	CMHexEdit	m_unit;
	CMHexEdit	m_gain;
	CMHexEdit	m_filecount;
	CMHexEdit	m_width;
	CMHexEdit	m_height;
	CMHexEdit	m_dpdthd;
	CMHexEdit	m_edgediv;
	CMHexEdit	m_edgemax;
	CMHexEdit	m_edgesum;
	CEdit		m_edgepara[25];
	CMHexEdit	m_afh[5];
	CMHexEdit	m_afv[5];
	CMHexEdit	m_afw[16];
	CMHexEdit	m_motionframecount;
	CMHexEdit	m_motionhthd;
	CMHexEdit	m_motionmthd;
	CMHexEdit	m_motiondetect;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnIspProcess();
	afx_msg void OnBnClickedBtnSelAll();
	afx_msg void OnBnClickedBtnCmoduleTest();
	afx_msg void OnBnClickedBtnCreateRawData();
	afx_msg void OnBnClickedBtnCompareY();
	afx_msg void OnBnClickedBtn10to8();
	afx_msg void OnBnClickedCheckColorconvert();
	afx_msg void OnBnClickedCheckCfa();
	afx_msg void OnBnClickedCheckEdgeenhance();
	afx_msg void OnBnClickedCheckNewEdge();
	afx_msg void OnBnClickedBtnMotion();
};

#endif
