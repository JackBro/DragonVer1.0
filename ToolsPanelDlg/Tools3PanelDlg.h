#ifndef __TOOLSPANEL_TOOLS3PANELDLG_H__
#define __TOOLSPANEL_TOOLS3PANELDLG_H__


#ifndef TOOLSPANEL_DLL
	#define TOOLSPANEL_DLL _declspec(dllimport)
#endif


#include "BaseClass/BasePanelDlg.h"
#include "BaseClass/AudioCtrl.h"
// CTools3PanelDlg 对话框

typedef struct tag_TTools3Option
{
	char src[DEFAULT_FILENAME_LEN];
	char dest[DEFAULT_FILENAME_LEN];
	char srcPosition[DEFAULT_FILENAME_LEN];
	int formatType;
	int channels;
	int samplesPerSec;
	int bitsPerSample;
	int srcfmt;
	int dstfmt;
} TTools3Option, *PTTools3Option;


class TOOLSPANEL_DLL C968ThreeDPosition;
class TOOLSPANEL_DLL CTools3PanelDlg : public CBasePanelDlg
{
	DECLARE_DYNAMIC(CTools3PanelDlg)

public:
	CTools3PanelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTools3PanelDlg();

// 对话框数据
	enum { IDD = IDD_TOOLS3_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	void InitUserOption(void);
	void GetUserOption(char *tail = FILE_TAIL_BIN);
	void AutoSetSrcfile(void);
	void DispOption(void);

	//{{ function Dataconvert
	void WavePcmToOther();
	void AudPcmToOther();
	void AudVimicroAdpcmToOther();
	void AudYamahaAdpcmToOther();

	void LoadHrtf(C968ThreeDPosition& threeDPosition);
	//}} function Dataconvert

private:
	TTools3Option m_option;

private:
	CPlayAudio	m_audio;
	CStatic		m_info;
	CMComboBox  m_dstfmt;
	CMComboBox  m_srcfmt;
	CMComboBox  m_cmb3DPosition;

	CMComboBox  m_cmbFormatType;
	CMComboBox  m_cmbChannels;
	CMComboBox  m_cmbSamplesPerSec;
	CMComboBox  m_cmbBitsPerSample;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTool3miscButton();
	afx_msg void OnBnClickedTool3playButton();

public:
	afx_msg void OnBnClickedTool3stopButton();
	afx_msg void OnBnClickedTool3printButton();
	afx_msg void OnBnClickedDataconvertButton();
	afx_msg void OnBnClickedBtnWriteWave();
	afx_msg void OnCbnSelchangeSrcformatCombo();
	afx_msg void OnCbnSelchangeDestformatCombo();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeSrcAdpcmCombo();
	afx_msg void OnBnClickedOpenSrcPcmButton();
	afx_msg void OnBnClickedOpenDstAdpcmButton();
	afx_msg void OnBnClickedOpenSrcAdpcmButton();
	afx_msg void OnBnClickedOpenDstPcmButton();
	afx_msg void OnBnClickedCodestreamConvet();
	afx_msg void OnBnClickedCodestreamConvetToAdpcm();
	afx_msg void OnBnClickedCodestreamConvetToPcm();
	afx_msg void OnBnClickedLoadPcmButton();
	afx_msg void OnBnClickedLoadPcmHeaderButton();
	afx_msg void OnBnClickedLoadPcmCodeButton();
	afx_msg void OnBnClickedDstWaveButton();


	afx_msg void OnBnClickedLoad3dPositionButton();
	afx_msg void OnCbnSelchangeHrtfbuf3dPositionCombo();
};

#endif
