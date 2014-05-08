#ifndef __V568PLANE_V568HIGHAPIDLG_H__
#define __V568PLANE_V568HIGHAPIDLG_H__


#ifndef V568PLANE_DLL
	#define V568PLANE_DLL _declspec(dllimport)
#endif

#include "tools/global.h"
#include "BasicFunc/BaseCtrlDlg.h"
#include "BaseClass/MComboBox.h"
#include "BaseClass/MHexEdit.h"

typedef struct tag_THapiOption{
	char	srcfile[DEFAULT_FILENAME_LEN];
	char	dstfile[DEFAULT_FILENAME_LEN];

	UINT8	sensorres;			/* sensor resolution */
	//Tsize	LCDdisplayres;				/* display resolution */
	TSize	thumbres;
	UINT8	capsize;
	UINT8	scenemode;
	UINT8	specEffect; 		/* Special Effect */
	UINT8	rotation;			/* Rotation */
	UINT8	awbmode;			/* white balance mode */
	UINT8	aemode;				/* Auto Exposure */
	UINT8	flashmode;			/* Flash */
	//int	isaf;				/* Auto Focus */
	UINT8  adjustprty;
	//UINT8	brightness;			/* brightness  */
	//UINT8	contrast;			/* Contrast */
	//UINT8	digitalzoom;
	//UINT8	brightstep;			/* Brightness step value */
	//UINT8	contraststep;		/* Contrast step value */
	//UINT8	digitalzoomstep;	/* Digital Zoom step value, between(1-9) */
	UINT8   compresssizetype;	//choosecomprate;		/* if true ,choose JPEG compression rate , else choose JPEG File size */
	UINT8	jpegcompratio;		/* JPEG compression rate, value range(1-100),0 is invalid */
	UINT16	jpegfilesize;		/* JPEG File size ,0 is invalid , kbyte*/ 
	
								

	/*-------Setup only for capture multi-shot-----*/
	UINT8	mshotcount;			/* multi-shot count */
	UINT16	mshotinterval;		/* capture interval */
	/*-------Setup/control only for capture video-----*/
	UINT8	videores;
	UINT8	initvideolength;		/* real videolength can be smaller than this value when user stops capturing manually */
	UINT8	frameindex;
	char	frame[10][100];
	char	frame_arm[10][100];
	UINT8	sizerratio;
	UINT8	armdemoen;
	/*  */
	//TFileOperations fileop;
}THapiOption, PTHapiOption;

enum CHOOSEPROPTenum{
	V5M_DLG_DIGITALZOOM,
	V5M_DLG_BRIGHTNESS,
	V5M_DLG_CONTRAST,
	V5M_DLG_BLENDRATIO
};


// CV568HighapiDlg 对话框

class V568PLANE_DLL CV568HighapiDlg : public CBaseCtrlDlg
{
	DECLARE_DYNAMIC(CV568HighapiDlg)

public:
	CV568HighapiDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CV568HighapiDlg();

// 对话框数据
	enum { IDD = IDD_V568HIGHAPI_DLG };

protected:

	 void InitUserOption(void);
	 void DispOption(void);
	 void GetUserOption(char *tail);
	 void GetComboValue(char *tail);
	 void GetEditBoxValue(void);
	 void GetBarValue(void);

private:
	THapiOption 	m_option;
	UINT8			snrres[10];
	CMComboBox		m_sensorresbox;
	CMComboBox		m_adjustprtybox;
	CButton			m_withframe1;
	CButton			m_withframe2;
	CButton			m_withthumb;
	CButton			m_af;
	CMHexEdit		m_mshotcountbox;
	CMHexEdit		m_mshotintervalbox;
	//CMHexEdit		m_jpegcompratiobox;
	//CMHexEdit		m_jpegfilesizebox;
	CMHexEdit		m_initvideolengthbox;
	CMComboBox		m_aebox;
	CMComboBox		m_flashbox;
	CMComboBox		m_specialeffectbox;
	CMComboBox		m_specialeffectbox2;
	CMComboBox		m_rotationbox;
	CMComboBox		m_awbbox;
	CMComboBox		m_scenebox;
	CMComboBox		m_capsizebox;
	CMComboBox		m_jcompresstypebox;
	CMHexEdit		m_jcompressvaluebox;
	CMComboBox		m_frameitembox;
	CScrollBar		m_sizerbar;
	CButton			m_pcdemobtn;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	UINT32 V5_SendCommand_Auto(UINT32 msg, void *param, UINT32 paramsize);
	afx_msg void OnBnClickedCapstillButton();
	afx_msg void OnBnClickedCapvideoButton();
	afx_msg void OnBnClickedV5openButton();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedUpButton();
	afx_msg void OnBnClickedDownButton();
	afx_msg void OnBnClickedCapPhotoButton();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeSnrsizeCombo();
	afx_msg void OnCbnSelchangeAdjustCombo();
	afx_msg void OnBnClickedFrameCheck();
	afx_msg void OnBnClickedWiththumbCheck();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedAfCheck();
	afx_msg void OnEnChangeMshotintervalEdit();
	afx_msg void OnBnClickedCapShotButton();
	afx_msg void OnBnClickedCapVideoButton();
	afx_msg void OnCbnSelchangeAeCombo();
	afx_msg void OnEnChangeVideolengthEdit();
	afx_msg void OnCbnSelchangeSpecialeffectCombo();
	afx_msg void OnCbnSelchangeRotationCombo();
	afx_msg void OnCbnSelchangeFlashCombo();
	afx_msg void OnCbnSelchangeCapsizeCombo();
	afx_msg void OnBnClickedJcompconfirmButton();
	//afx_msg void OnCbnSelchangeJcompressCombo();
	afx_msg void OnBnClickedHapirefreshButton();
	afx_msg void OnBnClickedDisplaystillButton();
	afx_msg void OnBnClickedDisplayvideoButton();
	afx_msg void OnBnClickedPppconfirmButton();
	afx_msg void OnBnClickedFrameCheck2();
	afx_msg void OnBnClickedViewButton();
	afx_msg void OnCbnSelchangeSpecialeffectCombo2();
	afx_msg void OnBnClickedDecodeippsaveButton();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	afx_msg void OnCbnSelchangeFrameitemCombo();
	afx_msg void OnCbnSelchangeJcompressCombo2();
	afx_msg void OnBnClickedPcdemoCheck();
	afx_msg void OnBnClickedArmdemoCheck();
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedPreviewButton();
	afx_msg void OnBnClickedButton4();
};

#endif
