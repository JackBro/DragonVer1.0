#ifndef __V568PLANE_V568JPEGDLG_H__
#define __V568PLANE_V568JPEGDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "tools/global.h"
#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MDecEdit.h"


// CV568JpegDlg 对话框
typedef struct tag_TJpegOption {
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];
	int		framecount;
	TJpegIndex info;

	TSize	startsize;
	TSize	endsize;
	int		delay;
	int		step;
	int		totaltime;

	int		qftime;
	int		qf;

	UINT8	testqt;
	UINT8	testfrmrate;
	SYSTEMTIME frmtime;
	UINT16	frmcnt;

	int		tcr;

	TJpegRudeBrc rudebrc;
	TJpegEnhBrc enhbrc;
	int		rudebrcen;
	int		rudeyuonlyen;
	int		enhbrcen;
	int		capdlyen;
	int		capdlyfeed;

} TJpegOption, *PTJpegOption;

typedef enum TIMERenum
{
	TIMER_QF,
	TIMER_FR
};

class V568PLANE_DLL CV568JpegDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568JpegDlg)

public:
	CV568JpegDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568JpegDlg();

// 对话框数据
	enum { IDD = IDD_V568JPEG_DLG };

private:
	TJpegOption	m_option;

private:
	CMComboBox m_jpgtypebox;

	CMDecEdit m_jpgimgwidthbox;
	CMDecEdit m_jpgimgheightbox;
	CMDecEdit m_jpgjpgwidth;
	CMDecEdit m_jpgjpgheight;
	CMDecEdit m_jpgsopbox;
	CMDecEdit m_jpgoffsetbox;

	CMDecEdit m_framecountbox;
	CMDecEdit m_startwidthbox;
	CMDecEdit m_startheightbox;
	CMDecEdit m_endwidthbox;
	CMDecEdit m_endheightbox;
	CMDecEdit m_stepbox;
	CMDecEdit m_delaybox;
	CMDecEdit m_totaltimebox;
	CMDecEdit m_qftimebox;
	CMDecEdit m_qfbox;

	CButton m_testqfbt;
	CButton m_testqfsetbt;
	CButton m_QtAutoBtn;

	CMHexEdit m_ctlTcr;
	CFileBin m_qffb;

	CMDecEdit m_edThdR1;
	CMDecEdit m_edThdR2;
	CMDecEdit m_edThdL1;
	CMDecEdit m_edThdL2;
	CMDecEdit m_edShfR1;
	CMDecEdit m_edShfR2;
	CMDecEdit m_edShfL1;
	CMDecEdit m_edShfL2;
	CMDecEdit m_edCompLevel0;
	CMDecEdit m_edCompLevel1;
	CMDecEdit m_edCompLevel2;
	CMDecEdit m_edCompLevel3;
	CMDecEdit m_edCompLevel4;
	CMDecEdit m_edCompLevel5;
	CMDecEdit m_edRudeAc;
	CMDecEdit m_edRudeLevel;
	CMDecEdit m_edThdWc;
	CMDecEdit m_capdlyfeed;
	CButton	  m_rudebrcen;
	CButton	  m_rudeacen;
	CButton	  m_enhbrcen;

	CButton	  m_capdlyen;


protected:
	void InitUserOption(void);
	void DispOption(void);

	void GetUserOption(char *tail = FILE_TAIL_JPG);
	void EnumFile(CStringArray * arry, const char* root = NULL, const char* filetype = NULL, char type = 0);
	void DecChgFile(char* filename, TSize size, UINT8 mode);
	void TestDec(UINT8 mode, UINT8 zoom);



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedParserjpgButton();
	afx_msg void OnBnClickedSplitjpgButton();
	afx_msg void OnBnClickedUpfilenameBtn();
	afx_msg void OnBnClickedCapstillButton();
	afx_msg void OnBnClickedDispstillButton();
	afx_msg void OnBnClickedButtonDemoIn();
	afx_msg void OnBnClickedButtonDemoCapStill();
	afx_msg void OnBnClickedButtonDemoDispStill();
	afx_msg void OnBnClickedButtonDemoCapVideo();
	afx_msg void OnBnClickedButtonDemoDispVideo();
	afx_msg void OnBnClickedButtonDemoCapFrame();
	afx_msg void OnBnClickedButtonDemoDispFrame();
	afx_msg void OnBnClickedButtonDemoEnc();
	afx_msg void OnBnClickedButtonDemoDec();
	afx_msg void OnBnClickedButtonDemoCapMul();
	afx_msg void OnBnClickedButtonDemoRetview();
	afx_msg void OnBnClickedButtonDemoMulRetview();
	afx_msg void OnBnClickedSplitjpgAllButton();
	afx_msg void OnBnClickedCapstillErrorButton();
	afx_msg void OnBnClickedBtnJfifSplit();
	afx_msg void OnBnClickedBtnExifSplit();
	afx_msg void OnBnClickedQtTest();
	afx_msg void OnBnClickedQtSet();
	afx_msg void OnBnClickedQtAuto();
	afx_msg void OnBnClickedFrameRateTest();
	afx_msg void OnBnClickedButtonTcr();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonRudeSet();
	afx_msg void OnBnClickedButtonEnhSet();
	afx_msg void OnBnClickedDecButton();
	afx_msg void OnBnClickedDecippButton();
	afx_msg void OnBnClickedDeclcdButton();
	afx_msg void OnBnClickedDecAllButton();
	afx_msg void OnBnClickedButtonCapDly();
	afx_msg void OnBnClickedCapDlyEn();
};

#endif
