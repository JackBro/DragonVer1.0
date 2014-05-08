#ifndef _WAVEBASE_H_
#define _WAVEBASE_H_


#ifndef TOOLWAVE_DLL
	#define TOOLWAVE_DLL  __declspec(dllimport)
#endif

#include "tools/global.h"

class TOOLWAVE_DLL CWaveBase  
{
public:
	CWaveBase();
	virtual ~CWaveBase();

public:
	void	SetFormat(WAVEFORMATEX *fmt);
	void	GetFormat(WAVEFORMATEX *fmt);
	int		GetFormat();
	int		GetNumberOfChannels();
	void	SetNumberOfChannels(int nchan);
	int		GetSamplesPerSecond();
	void	SetSamplesPerSecond(int sps);
	int		GetBitsPerSample();
	void	SetBitsPerSample(int bps);
	void	Update(void);

private:
	virtual void	Init(void);

protected:
	WAVEFORMATEX	m_Format;
};

#endif

