#ifndef _TOOLS_WAVEF_H_
#define _TOOLS_WAVEF_H_


#ifndef TOOLWAVE_DLL
	#define TOOLWAVE_DLL  __declspec(dllimport)
#endif

#include "wavebase.h"
#include "toolfile/filebin.h"

typedef struct tag_TChunkList {
	int hdr, hdrsize;
	int fmt, fmtsize;
	int dat, datsize;
} TChunkList, *PTChunkList;

enum WAVECHUNKenum {
	SEEK_WAVEHDR,
	SEEK_WAVEFMT,
	SEEK_WAVEDATA
};

class TOOLWAVE_DLL CWaveF : public CFileBin, public CWaveBase
{
public:
	CWaveF(char *wavf = NULL);

public:
	void Open(char *filename = NULL);
	void SeekChunk(int index);

	int GetHeaderSize() const;
	int GetHeaderData(char* buf, int nLength);

	int  GetDataSize() const;
	int  GetData(char* buf, int nLength);
	
	
private:
	int  GetWaveFInfo(void);
	int  FindChunk(int flag, int *size, int *pos);

private:
	int			m_state;
	TChunkList	m_chunk;
};

#endif

