// SoundOut.cpp: implementation of the CSoundOut class.
//
//////////////////////////////////////////////////////////////////////

#include "internal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// IMPLEMENT_DYNCREATE(CSoundOut, CWinThread)
CSoundOut::CSoundOut()
{
	this->m_status = SOUNDOUT_NOTSTART;
	this->m_hPlay = NULL;
	CWinThread::m_bAutoDelete = true;
	this->m_QueueCount = 0;
	this->CreateThread();
}

CSoundOut::~CSoundOut()
{
	this->Stop();
	//::PostQuitMessage(0);
}

BEGIN_MESSAGE_MAP(CSoundOut, CWinThread)
	//{{AFX_MSG_MAP(CSoundOut)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(MM_WOM_DONE, OnMM_WOM_DONE)
END_MESSAGE_MAP()

void CSoundOut::Stop()
{
	int flag = 4;

	if( (this->m_status != SOUNDOUT_NOTSTART) && (this->m_status != SOUNDOUT_STOP) )
	{
		this->m_status = SOUNDOUT_STOP;
		::waveOutReset(this->m_hPlay);
	}
	while( (this->m_status != SOUNDOUT_NOTSTART) && (flag--) )
		Sleep(1);
}

void CSoundOut::Start(WAVEFORMATEX* format)
{
	MMRESULT mmReturn = 0;
	int i, size;

	this->Stop();
	if(format == NULL)
		return;

	this->SetFormat(format);
	mmReturn = ::waveOutOpen( &this->m_hPlay, WAVE_MAPPER, &this->m_Format, 
		this->m_nThreadID, NULL, CALLBACK_THREAD);
	if(mmReturn)
		return;
	
	this->m_status = SOUNDOUT_PLAYING;
	for(i = 0; i < TOTAL_WAVEHEADER; i++)
	{
		size = this->m_wavbuf[i].GetTotalSize();
		size = this->GetSoundData(this->m_wavbuf[i].m_buf, size);
		this->m_wavhdr[i].lpData = this->m_wavbuf[i].m_buf;
		this->m_wavhdr[i].dwBufferLength = size;
		this->m_wavhdr[i].dwFlags = 0;
		this->WriteWaveQueue(this->m_wavhdr + i);
	}
}

void CSoundOut::OnMM_WOM_DONE(UINT parm1, LONG parm2)
{
	MMRESULT mmReturn;
	LPWAVEHDR pHdr = (LPWAVEHDR) parm2;
	int len;

	mmReturn = ::waveOutUnprepareHeader(this->m_hPlay, pHdr, sizeof(WAVEHDR));
	if(mmReturn)
		return;
	this->m_QueueCount--;
	if(this->m_status == SOUNDOUT_PLAYING)
	{
		len = this->GetSoundData(pHdr->lpData, pHdr->dwBufferLength);
		if(len)
		{
			pHdr->dwBufferLength = len;
			this->WriteWaveQueue(pHdr);
			return;
		}
		this->m_status = SOUNDOUT_STOP;
		::waveOutReset(this->m_hPlay);
//		return;
	}
	if( (this->m_QueueCount == 0) || (this->m_status == SOUNDOUT_STOP) )
	{
		::waveOutClose(m_hPlay);
		this->m_hPlay = NULL;
		this->m_status = SOUNDOUT_NOTSTART;
	}
	parm1 = 0;
}

void CSoundOut::WriteWaveQueue(WAVEHDR *hdr)
{
	MMRESULT mmReturn;

	mmReturn = ::waveOutPrepareHeader(this->m_hPlay, hdr, sizeof(WAVEHDR));
	if(mmReturn)
		return;
	::waveOutWrite(this->m_hPlay, hdr, sizeof(WAVEHDR));
	this->m_QueueCount++;
}

void CSoundOut::Pause(void)
{
	if( (this->m_hPlay) && (this->m_status == SOUNDOUT_PLAYING) )
	{
		this->m_status = SOUNDOUT_PAUSE;
		::waveOutPause(this->m_hPlay);
	}
}

void CSoundOut::Restart(void)
{
	if( (this->m_hPlay) && (this->m_status == SOUNDOUT_PAUSE) )
	{
		this->m_status = SOUNDOUT_PLAYING;
		::waveOutRestart(this->m_hPlay);
	}
}

BOOL CSoundOut::InitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	this->m_nThreadID = ::GetCurrentThreadId();
	return TRUE;
}
