#ifndef __TOOLAUDIODEV_PLAYAUDIO_H__
#define __TOOLAUDIODEV_PLAYAUDIO_H__


#ifndef TOOLAUDIODEV_DLL
	#define TOOLAUDIODEV_DLL  __declspec(dllimport)
#endif


#include "SoundOut.h"
#include "toolwave/wavef.h"
#include "toolaudiodev/PlayAudio.h"


class TOOLAUDIODEV_DLL CPlayAudio : public CSoundOut, public CWaveF
{
public:
	CPlayAudio(char *filename = NULL);

public:
	virtual void Open(char *filename = NULL);
	virtual int  GetSoundData(char *buf, int len);

public:
	void Start(void);

private:
};



#endif
