#include "internal.h"


CPlayAudio::CPlayAudio(char *filename)
{
	this->Open(filename);
}

int CPlayAudio::GetSoundData(char *buf, int len)
{
#if 0
	int len1;

	len1 = CWaveF::Read(buf, len);
	if(len1 == 0)
	{
		this->SeekChunk(SEEK_WAVEDATA);
		len1 =  CWaveF::Read(buf, len);
	}
	return len1;
#else
	return CWaveF::Read(buf, len);
#endif
}

void CPlayAudio::Open(char *filename)
{
	CSoundOut::Stop();
	CWaveF::Open(filename);
}

void CPlayAudio::Start(void)
{
	WAVEFORMATEX fmt;

	if(CFileBase::IsOpened())
	{
		CWaveF::GetFormat(&fmt);
		CSoundOut::Start(&fmt);
	}
}


