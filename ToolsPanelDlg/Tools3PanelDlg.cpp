// Tools2PanelDlg.cpp : 实现文件
//

#include "internal.h"




// CTools3PanelDlg 对话框

IMPLEMENT_DYNAMIC(CTools3PanelDlg, CBasePanelDlg)
CTools3PanelDlg::CTools3PanelDlg(CWnd* pParent /*=NULL*/)
: CBasePanelDlg(CTools3PanelDlg::IDD, pParent)
{
}

CTools3PanelDlg::~CTools3PanelDlg()
{
}

void CTools3PanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CBasePanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AUDFINFO_STATIC, m_info);
	DDX_Control(pDX, IDC_DESTFORMAT_COMBO, m_dstfmt);
	DDX_Control(pDX, IDC_SRCFORMAT_COMBO, m_srcfmt);
	DDX_Control(pDX, IDC_HRTFBUF_3D_POSITION_COMBO, m_cmb3DPosition);
	DDX_Control(pDX, IDC_FORMAT_TYPE_COMBO, m_cmbFormatType);
	DDX_Control(pDX, IDC_CHANNEL_NUMBER_COMBO, m_cmbChannels);
	DDX_Control(pDX, IDC_SAMPLES_PER_SEC_COMBO, m_cmbSamplesPerSec);
	DDX_Control(pDX, IDC_BIT_PER_SAMPLE_COMBO, m_cmbBitsPerSample);	
}


BEGIN_MESSAGE_MAP(CTools3PanelDlg, CBasePanelDlg)
	ON_BN_CLICKED(IDC_TOOL3MISC_BUTTON, OnBnClickedTool3miscButton)
	ON_BN_CLICKED(IDC_TOOL3PLAY_BUTTON, OnBnClickedTool3playButton)
	ON_BN_CLICKED(IDC_TOOL3STOP_BUTTON, OnBnClickedTool3stopButton)
	ON_BN_CLICKED(IDC_TOOL3PRINT_BUTTON, OnBnClickedTool3printButton)
	ON_BN_CLICKED(IDC_DATACONVERT_BUTTON, OnBnClickedDataconvertButton)
	ON_BN_CLICKED(IDC_BTN_WRITE_WAVE, OnBnClickedBtnWriteWave)
	ON_BN_CLICKED(IDC_LOAD_3D_POSITION_BUTTON, OnBnClickedLoad3dPositionButton)
	ON_CBN_SELCHANGE(IDC_HRTFBUF_3D_POSITION_COMBO, OnCbnSelchangeHrtfbuf3dPositionCombo)
END_MESSAGE_MAP()


// CTools3PanelDlg 消息处理程序

BOOL CTools3PanelDlg::OnInitDialog()
{
	CBasePanelDlg::OnInitDialog();
	this->InitUserOption();
	this->DispOption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTools3PanelDlg::InitUserOption(void)
{
	int i;

	memset( &(this->m_option), 0, sizeof(TTools3Option) );
	for(i = 0; i < gTotalAudStreamType; i++)
	{
		this->m_srcfmt.InsertString(i, gAudStreamType[i]);
		this->m_dstfmt.InsertString(i, gAudStreamType[i]);
	}
	this->m_cmbFormatType.InsertString(0, "WAVE_FORMAT_PCM");
	this->m_cmbChannels.InsertString(0, "1");
	this->m_cmbChannels.InsertString(1, "2");
	this->m_cmbSamplesPerSec.InsertString(0, "22050");
	this->m_cmbSamplesPerSec.InsertString(1, "44100");
	this->m_cmbSamplesPerSec.InsertString(2, "44200");
	this->m_cmbSamplesPerSec.InsertString(3, "48000");
	this->m_cmbBitsPerSample.InsertString(0, "8");
	this->m_cmbBitsPerSample.InsertString(1, "16");
	this->m_dstfmt.SetCurSel(0);
	this->m_srcfmt.SetCurSel(0);
	this->m_cmbFormatType.SetCurSel(0);
	this->m_cmbChannels.SetCurSel(1);
	this->m_cmbSamplesPerSec.SetCurSel(0);
	this->m_cmbBitsPerSample.SetCurSel(1);
}

void CTools3PanelDlg::DispOption(void)
{

}

void CTools3PanelDlg::GetUserOption(char *tail)
{
	this->m_option.srcfmt = m_srcfmt.GetCurSel();
	this->m_option.dstfmt = m_dstfmt.GetCurSel();

	int nCurSel = m_cmbFormatType.GetCurSel();
	if (nCurSel == 0)
	{
		this->m_option.formatType = WAVE_FORMAT_PCM;
	}
	CString str;
	m_cmbChannels.GetWindowText(str);
	this->m_option.channels = atoi(str);
	m_cmbSamplesPerSec.GetWindowText(str);
	this->m_option.samplesPerSec = atoi(str);
	m_cmbBitsPerSample.GetWindowText(str);
	this->m_option.bitsPerSample = atoi(str);

	this->m_cmb3DPosition.GetWindowText(this->m_option.srcPosition, DEFAULT_FILENAME_LEN);
	CBasePanelDlg::GetFileBoxOption(this->m_option.src, this->m_option.dest, tail);
}

void CTools3PanelDlg::WavePcmToOther()
{
	CWaveF wf;
	WAVEFORMATEX fmt;

	this->GetUserOption();
	wf.Open(this->m_option.src);
	wf.GetFormat(&fmt);

	if(fmt.wFormatTag != WAVE_FORMAT_PCM)  
	{
		AfxMessageBox("Unknown format");
		return ;
	}
	int nAdpcmSize = (wf.GetDataSize()>>2)+4;
	int nDataSize = 0;
	int nReadLength = 0;
	CFileBin destFile;
	CBufferBase bufPcm;
	CBufferBase bufAdpcm;

	switch(this->m_option.dstfmt) 
	{
	case WAVE_PCM_MS:
		AfxMessageBox("Cannot Convert wavepcm to wavepcm");
		break;
	case WAVE_ADPCM_MS:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case WAVE_ADPCM_VIMICRO:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case WAVE_ADPCM_YAMAYA:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case AUD_PCM:
		{
			nDataSize = wf.GetDataSize();
			bufPcm.SetSize(nDataSize);
			wf.GetData(bufPcm.m_buf, nDataSize);
			destFile.Open(this->m_option.dest, "wb");
			destFile.Write(bufPcm.m_buf, nDataSize);
			destFile.Close();
		}
		break;
	case AUD_ADPCM_MS:
		AfxMessageBox("Cannot Support");
		break;
	case AUD_ADPCM_VIMICRO:
		{
			C968VimicroEncoderAdpcm vimicroEncoderAdpcm(nAdpcmSize);
			nDataSize = wf.GetDataSize();
			bufPcm.SetSize(nDataSize);
			wf.GetData(bufPcm.m_buf, nDataSize);
			vimicroEncoderAdpcm.Write(bufPcm.m_buf, nDataSize);
			bufAdpcm.SetSize(vimicroEncoderAdpcm.GetDataSize());
			nReadLength = vimicroEncoderAdpcm.Read(bufAdpcm.m_buf, vimicroEncoderAdpcm.GetDataSize());
			destFile.Open(this->m_option.dest, "wb");
			destFile.Write(bufAdpcm.m_buf, nReadLength);
			destFile.Close();
		}
		break;
	case AUD_ADPCM_YAMAYA:
		{
			C968YamahaEncoderAdpcm  yamahaEncoderAdpcm(nAdpcmSize);
			nDataSize = wf.GetDataSize();
			bufPcm.SetSize(nDataSize);
			wf.GetData(bufPcm.m_buf, nDataSize);
			yamahaEncoderAdpcm.Write(bufPcm.m_buf, nDataSize);
			bufAdpcm.SetSize(yamahaEncoderAdpcm.GetDataSize());
			nReadLength = yamahaEncoderAdpcm.Read(bufAdpcm.m_buf, yamahaEncoderAdpcm.GetDataSize());
			destFile.Open(this->m_option.dest, "wb");
			destFile.Write(bufAdpcm.m_buf, nReadLength);
			destFile.Close();
		}
		break;
	case AUD_PCM_3D_EXPANSION:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_3D_POSITION:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_3D_SURROUNDING:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_AGC:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_ADC:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_128_48KHZ:
		AfxMessageBox("Cannot Support!");
		break;
	case MIDI:
		AfxMessageBox("Cannot Support");
		break;
	default:
		break;
	}
}


void CTools3PanelDlg::AudPcmToOther()
{
	CFileBin pcmFile;
	pcmFile.Open(this->m_option.src);
	int nPcmLength = pcmFile.GetFileLength();
	if (nPcmLength < 1) return ;
	CBufferBase bufPcm;
	bufPcm.SetSize(nPcmLength);
	pcmFile.Read(bufPcm.m_buf, nPcmLength);

	CFileBin destFile;
	switch(this->m_option.dstfmt) 
	{
	case WAVE_PCM_MS:
		AfxMessageBox("Cannot Convert wavepcm to wavepcm");
		break;
	case WAVE_ADPCM_MS:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case WAVE_ADPCM_VIMICRO:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case WAVE_ADPCM_YAMAYA:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case AUD_PCM:
		AfxMessageBox("Cannot Convert audpcm to audpcm");
		break;
	case AUD_ADPCM_MS:
		break;
	case AUD_ADPCM_VIMICRO:
		{
			C968VimicroEncoderAdpcm vimicroEncoderAdpcm(nPcmLength+4);
			vimicroEncoderAdpcm.Write(bufPcm.m_buf, nPcmLength);
			CBufferBase bufAdpcm;
			bufAdpcm.SetSize(vimicroEncoderAdpcm.GetDataSize());
			int nAdpcmLength = vimicroEncoderAdpcm.Read(bufAdpcm.m_buf, vimicroEncoderAdpcm.GetDataSize());
			destFile.Open(this->m_option.dest, "wb");
			destFile.Write(bufAdpcm.m_buf, nAdpcmLength);
			destFile.Close();
		}
		break;
	case AUD_ADPCM_YAMAYA:
		{
			C968YamahaEncoderAdpcm  yamahaEncoderAdpcm(nPcmLength+4);
			yamahaEncoderAdpcm.Write(bufPcm.m_buf, nPcmLength);
			CBufferBase bufAdpcm;
			bufAdpcm.SetSize(yamahaEncoderAdpcm.GetDataSize());
			int nAdpcmLength = yamahaEncoderAdpcm.Read(bufAdpcm.m_buf, yamahaEncoderAdpcm.GetDataSize());
			destFile.Open(this->m_option.dest, "wb");
			destFile.Write(bufAdpcm.m_buf, nAdpcmLength);
			destFile.Close();
		}
		break;
	case AUD_PCM_3D_EXPANSION:
		{
			C9683StereoDipole stereoDipole(nPcmLength+4);
			stereoDipole.Write(bufPcm.m_buf, nPcmLength);
			CBufferBase buf3DAudio;
			buf3DAudio.SetSize(stereoDipole.GetDataSize());
			int n3DpcmLength = stereoDipole.Read(buf3DAudio.m_buf, stereoDipole.GetDataSize());
			destFile.Open(this->m_option.dest, "wb");
			destFile.Write(buf3DAudio.m_buf, n3DpcmLength);
			destFile.Close();
		}
		break;
	case AUD_PCM_3D_POSITION:
		{
			C968ThreeDPosition threeDPosition(nPcmLength+4);
			if (strlen(this->m_option.srcPosition) > 0)
			{
				LoadHrtf(threeDPosition);
			}
			threeDPosition.Write(bufPcm.m_buf, nPcmLength);
			CBufferBase buf3DAudio;
			buf3DAudio.SetSize(threeDPosition.GetDataSize());
			int n3DpcmLength = threeDPosition.Read(buf3DAudio.m_buf, threeDPosition.GetDataSize());
			destFile.Open(this->m_option.dest, "wb");
			destFile.Write(buf3DAudio.m_buf, n3DpcmLength);
			destFile.Close();
		}
		break;
	case AUD_PCM_3D_SURROUNDING:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_AGC:
		{
			C968Agc agc(nPcmLength+4);
			agc.Write(bufPcm.m_buf, nPcmLength);
			CBufferBase bufAgc;
			bufAgc.SetSize(agc.GetDataSize());
			int nAgcLength = agc.Read(bufAgc.m_buf, agc.GetDataSize());
			destFile.Open(this->m_option.dest, "wb");
			destFile.Write(bufAgc.m_buf, nAgcLength);
			destFile.Close();
		}
		break;
	case AUD_PCM_ADC:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_128_48KHZ:
		AfxMessageBox("Cannot Support!");
		break;
	case MIDI:
		AfxMessageBox("Cannot Support");
		break;
	default:
		break;
	}
}


void CTools3PanelDlg::AudVimicroAdpcmToOther()
{
	CFileBin adpcmFile;
	adpcmFile.Open(this->m_option.src);
	int nAdpcmLength = adpcmFile.GetFileLength();
	int nPcmLength = 0;
	CBufferBase bufPcm;
	CBufferBase bufAdpcm;
	bufAdpcm.SetSize(nAdpcmLength);
	adpcmFile.Read(bufAdpcm.m_buf, nAdpcmLength);
	CFileBin destFile;
	C968VimicroDecoderAdpcm vimicroDecoderAdpcm((nAdpcmLength << 2)+4);
	switch(this->m_option.dstfmt) 
	{
	case WAVE_PCM_MS:
		AfxMessageBox("Cannot Convert wavepcm to wavepcm");
		break;
	case WAVE_ADPCM_MS:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case WAVE_ADPCM_VIMICRO:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case WAVE_ADPCM_YAMAYA:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case AUD_PCM:
		vimicroDecoderAdpcm.Write(bufAdpcm.m_buf, nAdpcmLength);
		bufPcm.SetSize(vimicroDecoderAdpcm.GetDataSize());
		nPcmLength =vimicroDecoderAdpcm.Read(bufPcm.m_buf, vimicroDecoderAdpcm.GetDataSize());
		destFile.Open(this->m_option.dest, "wb");
		destFile.Write(bufPcm.m_buf, nPcmLength);
		destFile.Close();
		break;
	case AUD_ADPCM_MS:
		AfxMessageBox("Cannot Support");
		break;
	case AUD_ADPCM_VIMICRO:
		AfxMessageBox("Cannot Support");
		break;
	case AUD_ADPCM_YAMAYA:
		AfxMessageBox("Cannot Support");
		break;
	case AUD_PCM_3D_EXPANSION:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_3D_POSITION:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_3D_SURROUNDING:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_AGC:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_ADC:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_128_48KHZ:
		AfxMessageBox("Cannot Support!");
		break;
	case MIDI:
		AfxMessageBox("Cannot Support");
		break;
	default:
		break;
	}
}


void CTools3PanelDlg::AudYamahaAdpcmToOther()
{
	CFileBin adpcmFile;
	adpcmFile.Open(this->m_option.src);
	int nAdpcmLength = adpcmFile.GetFileLength();
	int nPcmLength = 0;
	CBufferBase bufPcm;
	CBufferBase bufAdpcm;
	bufAdpcm.SetSize(nAdpcmLength);
	adpcmFile.Read(bufAdpcm.m_buf, nAdpcmLength);
	CFileBin destFile;
	C968YamahaDecoderAdpcm yamahaDecoderAdpcm((nAdpcmLength << 2)+4);
	switch(this->m_option.dstfmt) 
	{
	case WAVE_PCM_MS:
		AfxMessageBox("Cannot Convert wavepcm to wavepcm");
		break;
	case WAVE_ADPCM_MS:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case WAVE_ADPCM_VIMICRO:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case WAVE_ADPCM_YAMAYA:
		AfxMessageBox("Cannot Convert wavepcm to waveadpcm");
		break;
	case AUD_PCM:
		yamahaDecoderAdpcm.Write(bufAdpcm.m_buf, nAdpcmLength);
		bufPcm.SetSize(yamahaDecoderAdpcm.GetDataSize());
		nPcmLength =yamahaDecoderAdpcm.Read(bufPcm.m_buf, yamahaDecoderAdpcm.GetDataSize());
		destFile.Open(this->m_option.dest, "wb");
		destFile.Write(bufPcm.m_buf, nPcmLength);
		destFile.Close();
		break;
	case AUD_ADPCM_MS:
		AfxMessageBox("Cannot Support");
		break;
	case AUD_ADPCM_VIMICRO:
		AfxMessageBox("Cannot Support");
		break;
	case AUD_ADPCM_YAMAYA:
		AfxMessageBox("Cannot Support");
		break;
	case AUD_PCM_3D_EXPANSION:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_3D_POSITION:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_3D_SURROUNDING:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_AGC:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_ADC:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_128_48KHZ:
		AfxMessageBox("Cannot Support!");
		break;
	case MIDI:
		AfxMessageBox("Cannot Support");
		break;
	default:
		break;
	}
}



void CTools3PanelDlg::LoadHrtf(C968ThreeDPosition& threeDPosition)
{
	CWaveF rf;
	rf.Open(this->m_option.srcPosition);
	int nDataSize = rf.GetDataSize();
	CBufferBase buf(nDataSize);
	rf.GetData(buf.m_buf, nDataSize);
	threeDPosition.SetHrtfData(buf.m_buf, nDataSize);
	rf.Close();
}

void CTools3PanelDlg::AutoSetSrcfile(void)
{
	CBasePanelDlg::AutoSetFileBox(this->m_option.dest);
}



void CTools3PanelDlg::OnBnClickedTool3miscButton()
{
	//	CVLfifoBuf pool;

	//	pool.Write(20);
}

void CTools3PanelDlg::OnBnClickedTool3playButton()
{
	CWaitCursor cursor;

	this->GetUserOption();
	this->m_audio.Open(this->m_option.src);
	this->m_audio.Start();
}


void CTools3PanelDlg::OnBnClickedTool3stopButton()
{
	CWaitCursor cursor;

	this->m_audio.Stop();
}

void CTools3PanelDlg::OnBnClickedTool3printButton()
{
	CWaitCursor cursor;
	CWaveF wf;
	WAVEFORMATEX fmt;
	char str[0x100] = "Unknown format";

	this->GetUserOption();
	wf.Open(this->m_option.src);
	wf.GetFormat(&fmt);
	if(fmt.wFormatTag == WAVE_FORMAT_PCM)
	{
		sprintf(str, "fmt:\t%s \nsamplerate:\t %d \nchannel:\t %d", 
			gAudStreamType[WAVE_PCM_MS], fmt.nSamplesPerSec, fmt.nChannels);
		this->m_srcfmt.SetCurSel(WAVE_PCM_MS);
	}
	this->m_info.SetWindowText(str);
}


void CTools3PanelDlg::OnBnClickedDataconvertButton()
{
	CWaitCursor cursor;
	this->GetUserOption();

	switch(this->m_option.srcfmt) 
	{
	case WAVE_PCM_MS:
		WavePcmToOther();
		break;
	case WAVE_ADPCM_MS:
		AfxMessageBox("Cannot Support!");
		break;
	case WAVE_ADPCM_VIMICRO:
		AfxMessageBox("Cannot Support!");
		break;
	case WAVE_ADPCM_YAMAYA:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM:
		AudPcmToOther();
		break;
	case AUD_ADPCM_MS:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_ADPCM_VIMICRO:
		AudVimicroAdpcmToOther();
		break;
	case AUD_ADPCM_YAMAYA:
		AudYamahaAdpcmToOther();
		break;
	case AUD_PCM_3D_EXPANSION:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_3D_POSITION:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_3D_SURROUNDING:
		AfxMessageBox("Cannot Support!");
		break;
	case MIDI:
		AfxMessageBox("Cannot Support!");
	case AUD_PCM_AGC:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_ADC:
		AfxMessageBox("Cannot Support!");
		break;
	case AUD_PCM_128_48KHZ:
		AfxMessageBox("Cannot Support!");
		break;
		break;
	default:
		break;
	}
}

void CTools3PanelDlg::OnBnClickedBtnWriteWave()
{
	CWaitCursor cursor;
	this->GetUserOption();

	CFileBin pcmFile;
	pcmFile.Open(this->m_option.src);
	int nPcmLength = pcmFile.GetFileLength();
	CBufferBase bufPcm;
	bufPcm.SetSize(nPcmLength);
	pcmFile.Read(bufPcm.m_buf, nPcmLength);
	pcmFile.Close();

	CWaveWF wf;
	WAVEFORMATEX fmt;
	fmt.wFormatTag = (WORD)this->m_option.formatType;
	fmt.nChannels =  (WORD)this->m_option.channels;
	fmt.nSamplesPerSec = this->m_option.samplesPerSec;
	fmt.wBitsPerSample = (WORD)this->m_option.bitsPerSample;
	wf.Open(this->m_option.dest);
	wf.SetFormat(&fmt);
	wf.Write(bufPcm.m_buf, nPcmLength);
	wf.Close();
}

void CTools3PanelDlg::OnBnClickedLoad3dPositionButton()
{
	CString str;
	if(CBasePanelDlg::OpenFileDlg(&str))
	{
		this->m_cmb3DPosition.addString(str.GetBuffer());
	}
}

void CTools3PanelDlg::OnCbnSelchangeHrtfbuf3dPositionCombo()
{

}
