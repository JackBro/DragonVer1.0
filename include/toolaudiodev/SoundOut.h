#ifndef __TOOLAUDIODEV_SOUNDOUT_H__
#define __TOOLAUDIODEV_SOUNDOUT_H__


#ifndef TOOLAUDIODEV_DLL
	#define TOOLAUDIODEV_DLL  __declspec(dllimport)
#endif

#include <mmsystem.h>
#include "toolbuf/bufferbase.h"
#include "toolwave/wavebase.h"


enum SOUNDOUTSTATEenum {
	SOUNDOUT_NOTSTART,
	SOUNDOUT_PLAYING,
	SOUNDOUT_PAUSE,
	SOUNDOUT_STOP
};

#define TOTAL_WAVEHEADER	2

class TOOLAUDIODEV_DLL CSoundOut : public CWinThread, public CWaveBase
{
//	DECLARE_DYNCREATE(CSoundOut)

public:
	CSoundOut();
	virtual ~CSoundOut();

public:
	void Stop(void);
	void Start(WAVEFORMATEX* format = NULL);
	void Pause(void);
	void Restart(void);

public:
	virtual int  GetSoundData(char *buf, int len) = 0;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnMM_WOM_DONE(UINT parm1, LONG parm2);

private:
	void WriteWaveQueue(WAVEHDR *hdr);

private:
	int				m_status;
	WAVEHDR			m_wavhdr[TOTAL_WAVEHEADER];
	CBufferBase		m_wavbuf[TOTAL_WAVEHEADER];
	int				m_QueueCount;
	HWAVEOUT		m_hPlay;
//	DWORD			m_ThreadID;

public:
	virtual BOOL InitInstance();
};



#endif
