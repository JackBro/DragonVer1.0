// CSummonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SummonDlg.h"
#include "CSummonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSummonDlg 对话框


CSummonDlg::CSummonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSummonDlg::IDD, pParent)
	, m_IN_FileName(_T(""))
	, m_OUT_FileName(_T(""))
	, m_SummonMode(0)
	, m_SummonOutput(0)
	, m_Summon_Lame_ChannelMode(0)
	, m_Summon_Lame_BitRate(0)
	, m_Summon_Lame_Channel_Left(FALSE)
	, m_Summon_Lame_Channel_Right(FALSE)
	, m_Summon_MP3_LeftGain(1)
	, m_Summon_MP3_RightGain(1)
	, m_SUMMON_Lame_ABR(8)
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CSummonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SUMMON_IN, m_IN_FileName);
	DDX_Text(pDX, IDC_SUMMON_OUT, m_OUT_FileName);
	DDX_Radio(pDX, IDC_RADIO_MP3_DE, m_SummonMode);
	DDX_Control(pDX, IDC_SUMMON_LAME_PROGRESS, m_SummonLame_Progress);
	DDX_Radio(pDX, IDC_SUMMON_MODE, m_Summon_Lame_ChannelMode);
	DDX_Radio(pDX, IDC_RADIO_SUMMON_BITRATE, m_Summon_Lame_BitRate);
	DDX_Check(pDX, IDC_CHECK_SUMMON_CHANNEL_LEFT, m_Summon_Lame_Channel_Left);
	DDX_Check(pDX, IDC_CHECK_SUMMON_CHANNEL_RIGHT, m_Summon_Lame_Channel_Right);
	DDX_Text(pDX, IDC_EDIT_SUMMON_MP3_LEFTGAIN, m_Summon_MP3_LeftGain);
	DDV_MinMaxFloat(pDX, m_Summon_MP3_LeftGain, 0.1, 9.9);
	DDX_Text(pDX, IDC_EDIT_SUMMON_MP3_RIGHTGAIN, m_Summon_MP3_RightGain);
	DDV_MinMaxFloat(pDX, m_Summon_MP3_RightGain, 0.1, 9.9);
	DDX_Text(pDX, IDC_SUMMON_ABR, m_SUMMON_Lame_ABR);
	DDV_MinMaxInt(pDX, m_SUMMON_Lame_ABR, 8, 310);
	DDX_Control(pDX, IDC_SCROLLBAR_SUMMON_ABR, m_ScrollBar_Summon_Mp3_ABR);
}

BEGIN_MESSAGE_MAP(CSummonDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SUMMON_APPLY, OnBnClickedSummonApply)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SUMMON_IN, OnBnClickedButtonSummonIn)
	ON_BN_CLICKED(IDC_BUTTON_SUMMON_OUT, OnBnClickedButtonSummonOut)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_SUMMON_CHANNEL_LEFT, OnBnClickedCheckSummonChannelLeft)
	ON_BN_CLICKED(IDC_CHECK_SUMMON_CHANNEL_RIGHT, OnBnClickedCheckSummonChannelRight)
	ON_BN_CLICKED(IDC_RADIO_AAC_ENCODE, OnBnClickedAACEncode)
	ON_BN_CLICKED(IDC_RADIO_MP3_EN, OnBnClickedRadioMp3En)
	
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CSummonDlg 消息处理程序

BOOL CSummonDlg::OnInitDialog()
{
	
	
	CDialog::OnInitDialog();

	
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_SummonLame_Progress.SetRange( 0, 100 );
	m_SummonLame_Progress.SetPos(0);

	CComboBox *cBox;
	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_OUTPUT_MPEG_VERSION);
	//init AAC
	cBox->AddString((LPCTSTR)"MPEG4");
	cBox->AddString((LPCTSTR)"MPEG2");
	cBox->SetCurSel(1);
	
	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_OUTPUT_AAC_VERSION);
	cBox->AddString((LPCTSTR)"Main");
	cBox->AddString((LPCTSTR)"Low Complexity");
	cBox->AddString((LPCTSTR)"LTP");
	cBox->SetCurSel(1);
	m_ScrollBar_Summon_Mp3_ABR.EnableScrollBar(ESB_ENABLE_BOTH );
	m_ScrollBar_Summon_Mp3_ABR.ShowScrollBar(TRUE);
	m_ScrollBar_Summon_Mp3_ABR.SetScrollRange(8,310,TRUE);
	
	
	return TRUE;  
}

void CSummonDlg::SelectAAC(BOOL check)
{
	CComboBox *cBox;
	CButton *cBut;
	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_OUTPUT_MPEG_VERSION);
	cBox->EnableWindow(check);
	cBox->ResetContent();
	cBox->AddString((LPCTSTR)"MPEG4");
	cBox->AddString((LPCTSTR)"MPEG2");
	cBox->SetCurSel(1);
	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_OUTPUT_AAC_VERSION);
	cBox->EnableWindow(check);
	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_AAC_BANDWIDTH);
	cBox->EnableWindow(check);
	cBut=(CButton *)GetDlgItem( IDC_RADIO_SUMMON_AAC_BANDWIDTH);
	cBut->EnableWindow(check);
	
	
}
void CSummonDlg::SelectMP3(BOOL check)
{
	CButton *cBut;
	CComboBox *cBox;
	char i;

	cBut=(CButton *)GetDlgItem( IDC_RADIO_SUMMON_BITRATE);
	cBut->EnableWindow(check);
	cBut=(CButton *)GetDlgItem( IDC_RADIO1);
	cBut->EnableWindow(check);
	cBut=(CButton *)GetDlgItem( IDC_RADIO6);
	cBut->EnableWindow(check);

	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_CBR);
	cBox->EnableWindow(check);
	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_VBRMIN);
	cBox->EnableWindow(check);
	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_VBRMAX);
	cBox->EnableWindow(check);
	
	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_VBRQUALITY);
	cBox->EnableWindow(check);
	cBox->ResetContent();
	for(i='0';i<='9';i++)
	{	
		cBox->AddString((LPCTSTR)i);
	}
	cBox->SetCurSel(4);

	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_SAMPLERATE);
	cBox->EnableWindow(check);
	cBox=(CComboBox *)GetDlgItem( IDC_COMBO_SUMMON_OUTPUT_MPEG_VERSION);
	cBox->EnableWindow(check);
	cBox->ResetContent();
	cBox->AddString((LPCTSTR)"MPEG2");
	cBox->AddString((LPCTSTR)"MPEG1");
	cBox->SetCurSel(1);

}
void CSummonDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CSummonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSummonDlg::OnBnClickedSummonApply()
{
	
	UpdateData(TRUE);
	switch (m_SummonMode)
	{
	case MP3DECODE:
		SummonMp3Decoder();
		break;
	case MP3ENCODE:
		SummonMp3Encoder();
		break;
	case ADPCMENCODE:
		break;
	case ADPCMDECODE:
		break;
	}
	
}

void CSummonDlg::SummonMp3Encoder(void)
{
	
	DWORD dwThreadId; 
    HANDLE hThread; 
	LPVOID threadpara;
	BOOL ChannelFlag;
	if(m_IN_FileName.IsEmpty() ) 
	{
		this->OnBnClickedButtonSummonIn();
		if(!m_OUT_FileName.Find(".wav",0))
		{
			MessageBox("Please Input a wav file","Type Error",MB_OK);
			return;
		}
	}
	if(m_OUT_FileName.IsEmpty())
	{
		m_OUT_FileName=m_IN_FileName;
		if(!m_OUT_FileName.Replace(".wav",".mp3"))
		{
			MessageBox("Input filename doesn't have the .wav extension name");
			return;
		}
	}
	switch(m_Summon_Lame_ChannelMode)
	{
	case	MP3STD_STEREO:
		m_Para.argv[2]="-ms";
		break;
	case	MP3JOINT_STEREO:
		m_Para.argv[2]="-mj";
		break;
	case	MP3DUAL_CHANNEL:
		m_Para.argv[2]="-mf";
		break;
	case	MP3MONO:
        
		ChannelFlag=m_Summon_Lame_Channel_Right^m_Summon_Lame_Channel_Left;
		switch (ChannelFlag)
		{
			case FALSE:
				m_Para.argv[2]="-mm";
				break;
			case TRUE:
				m_Para.argv[2]= m_Summon_Lame_Channel_Right	== TRUE ? "-mmr":"-mml";
				break;
		}
		
		
		break;
	}
	//int frames;
	threadpara	=	(LPVOID)&m_Para;
	m_Para.argc	=	5;
	m_Para.argv[1]	=	"--silent";
    m_Para.argv[3]	=	m_IN_FileName.GetBuffer();
	m_Para.argv[4]	=	m_OUT_FileName.GetBuffer();
	m_Para.pUserframeNum	=	&UserframeNum;
	m_Para.pUserTotalframeNum	=	&UserTotalframeNum;

	UserframeNum	=	0;
	UserTotalframeNum	=	0;
	CWnd::SetTimer(0,100,0);
	
  
    hThread = CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        LameThread,                  // thread function 
        threadpara,                // argument to thread function 
         0 ,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
 
	
}

void CSummonDlg::SummonMp3Decoder(void)
{
	DWORD dwThreadId; 
    HANDLE hThread; 
	LPVOID threadpara;

	if(m_IN_FileName.IsEmpty() ) 
	{
		this->OnBnClickedButtonSummonIn();
		if(!m_OUT_FileName.Find(".mp3",0))
		{
			MessageBox("Please Input a mp3 file","Type Error",MB_OK);
			return;
		}
	}
	if(m_OUT_FileName.IsEmpty())
	{
		m_OUT_FileName=m_IN_FileName;
		if(!m_OUT_FileName.Replace(".mp3",".wav"))
		{
			MessageBox("Input filename doesn't have the .mp3 extension name");
			return;
		}
	}
	//int frames;
	threadpara	=	(LPVOID)&m_Para;
	m_Para.argc	=	5;
	m_Para.argv[1]	=	"--silent";
	m_Para.argv[2]	=	"--decode";
	m_Para.argv[3]	=	m_IN_FileName.GetBuffer();
	m_Para.argv[4]	=	m_OUT_FileName.GetBuffer();
	m_Para.pUserframeNum	=	&UserframeNum;
	m_Para.pUserTotalframeNum	=	&UserTotalframeNum;

	UserframeNum	=	0;
	UserTotalframeNum	=	0;
	CWnd::SetTimer(0,100,0);
	
  
    hThread = CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        LameThread,                  // thread function 
        threadpara,                // argument to thread function 
         0 ,                           // use default creation flags 
        &dwThreadId);                // returns the thread identifier 
 

}

DWORD CSummonDlg::LameThread(LPVOID  threadpara)
{ 
    int i=0;
	PT_Summon_Parameter para;
	para = (PT_Summon_Parameter)threadpara;
	
	lame_entry(para->argc,(char **)para->argv,
		(para->pUserframeNum),(para->pUserTotalframeNum));
	ExitThread(0);
    return 0; 
} 



void CSummonDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==0)
	{
		if(UserTotalframeNum!=0)
		{
			m_SummonOutput=(UserframeNum+2)*100.0/UserTotalframeNum+0.5;
			m_SummonLame_Progress.SetPos(m_SummonOutput);
			m_SummonLame_Progress.UpdateData(FALSE);

		}
	
	}

	CDialog::OnTimer(nIDEvent);
}

void CSummonDlg::OnBnClickedButtonSummonIn()
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_PATHMUSTEXIST|OFN_EXPLORER,_T("All Files (*.*)|*.*||"),this,0);
	CString str;
	dlg.m_ofn.lpstrTitle = _T("Open Input File");
	if(dlg.DoModal() ==  IDOK)
	{
		if(dlg.GetFileName().IsEmpty())
			return ;
			str=dlg.GetPathName();
			CWnd::UpdateData(TRUE);
			m_IN_FileName=str;
			CWnd::UpdateData(FALSE);
	}
}

void CSummonDlg::OnBnClickedButtonSummonOut()
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_PATHMUSTEXIST|OFN_EXPLORER,_T("All Files (*.*)|*.*||"),this,0);
	CString str;
	dlg.m_ofn.lpstrTitle = _T("Open Output File");
	if(dlg.DoModal() ==  IDOK)
	{
		if(dlg.GetFileName().IsEmpty())
			return ;
			str=dlg.GetPathName();
			CWnd::UpdateData(TRUE);
			m_OUT_FileName=str;
			CWnd::UpdateData(FALSE);
	}
}

void CSummonDlg::OnBnClickedOk()
{
	
	OnOK();
}

void CSummonDlg::OnBnClickedCheckSummonChannelLeft()
{
	CWnd *pWndEdit=CWnd::GetDlgItem(IDC_EDIT_SUMMON_MP3_LEFTGAIN);
	CWnd::UpdateData(TRUE);
	if(m_Summon_Lame_Channel_Left)
		pWndEdit->EnableWindow(TRUE);
	else
		pWndEdit->EnableWindow(FALSE);
}

void CSummonDlg::OnBnClickedCheckSummonChannelRight()
{
	CWnd *pWndEdit=CWnd::GetDlgItem(IDC_EDIT_SUMMON_MP3_RIGHTGAIN);
	CWnd::UpdateData(TRUE);
	if(m_Summon_Lame_Channel_Right)
		pWndEdit->EnableWindow(TRUE);
	else
		pWndEdit->EnableWindow(FALSE);
}



void CSummonDlg::OnBnClickedAACEncode()
{
	SelectMP3(FALSE);
	SelectAAC(TRUE);
}

void CSummonDlg::OnBnClickedRadioMp3En()
{
	SelectAAC(FALSE);
	SelectMP3(TRUE);
}






void CSummonDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(pScrollBar == &m_ScrollBar_Summon_Mp3_ABR)
	{
		if(nSBCode==SB_LINEDOWN )
		{
		
	//	m_ScrollBar_Summon_Mp3_ABR.SetScrollRange(8,310,FALSE);
	//	m_ScrollBar_Summon_Mp3_ABR.SetScrollPos(300,FALSE);
		m_SUMMON_Lame_ABR=m_ScrollBar_Summon_Mp3_ABR.GetScrollPos();
		UpdateData(TRUE);
		m_SUMMON_Lame_ABR++;
		m_SUMMON_Lame_ABR = (m_SUMMON_Lame_ABR>310)?310:m_SUMMON_Lame_ABR;
		m_SUMMON_Lame_ABR = (m_SUMMON_Lame_ABR<8)?8:m_SUMMON_Lame_ABR;
		m_ScrollBar_Summon_Mp3_ABR.SetScrollPos(m_SUMMON_Lame_ABR,TRUE);
		UpdateData(FALSE);
		}
		else if (nSBCode ==SB_LINEUP)
		{
		m_SUMMON_Lame_ABR=m_ScrollBar_Summon_Mp3_ABR.GetScrollPos();
		UpdateData(TRUE);
		m_SUMMON_Lame_ABR--;
		m_SUMMON_Lame_ABR = (m_SUMMON_Lame_ABR>310)?310:m_SUMMON_Lame_ABR;
		m_SUMMON_Lame_ABR = (m_SUMMON_Lame_ABR<8)?8:m_SUMMON_Lame_ABR;
		m_ScrollBar_Summon_Mp3_ABR.SetScrollPos(m_SUMMON_Lame_ABR,TRUE);
		UpdateData(FALSE);
		}
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
