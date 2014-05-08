// CSummonDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "../lame/lame.h"
#include "afxcmn.h"
#include "../lame/summonmp3.h"

enum Summonenum {
	MP3DECODE=0,
	MP3ENCODE=1,
	ADPCMENCODE=2,
	ADPCMDECODE=3
};
enum SummonLameModeenum {
	MP3STD_STEREO=0,
	MP3JOINT_STEREO=1,
	MP3DUAL_CHANNEL=2,
	MP3MONO=3
};

typedef struct tag_Summon_Parameter
	{
		int argc;
		char *argv[8];
		int *pUserframeNum;
		int *pUserTotalframeNum;
	}T_Summon_Parameter,*PT_Summon_Parameter;
// CSummonDlg 对话框
class CSummonDlg : public CDialog
{

public:
	CSummonDlg(CWnd* pParent = NULL);	
	enum { IDD = IDD_SUMMONDLG_DIALOG };
	int UserframeNum,UserTotalframeNum;
	static DWORD LameThread(LPVOID  threadpara);
protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedSummonApply();
	DECLARE_MESSAGE_MAP()
	
private:
	CString m_IN_FileName;
	CString m_OUT_FileName;
	int m_SummonMode;
	T_Summon_Parameter m_Para;
	CProgressCtrl m_SummonLame_Progress;
	int m_Summon_Lame_ChannelMode;
	int m_Summon_Lame_BitRate;
	BOOL m_Summon_Lame_Channel_Left;
	BOOL m_Summon_Lame_Channel_Right;
	int m_SummonOutput;


	void SummonMp3Encoder(void);
	void SummonMp3Decoder(void);
	void SelectAAC(BOOL check);
	void SelectMP3(BOOL check);
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonSummonIn();
	afx_msg void OnBnClickedButtonSummonOut();
	afx_msg void OnBnClickedOk();



	
private:
	float m_Summon_MP3_LeftGain;
	float m_Summon_MP3_RightGain;
public:
	afx_msg void OnBnClickedCheckSummonChannelLeft();
	afx_msg void OnBnClickedCheckSummonChannelRight();

	afx_msg void OnBnClickedAACEncode();
	afx_msg void OnBnClickedRadioMp3En();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
private:
	int m_SUMMON_Lame_ABR;
		
	
	CScrollBar m_ScrollBar_Summon_Mp3_ABR;
};


