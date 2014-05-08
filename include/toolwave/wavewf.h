#ifndef _TOOLS_WAVEWF_H_
#define _TOOLS_WAVEWF_H_



#ifndef TOOLWAVE_DLL
	#define TOOLWAVE_DLL  __declspec(dllimport)
#endif


#include "toolfile/filebin.h"
#include "wavebase.h"



typedef struct tag_TChunkHeader {
	int hdr, filesize;
	int type, fmt, fmtsize;
	int dataid, datasize;
} TChunkHeader, *PTChunkHeader;

class TOOLWAVE_DLL CWaveWF : public CFileBin, public CWaveBase
{
public:
	CWaveWF(char *wavf = NULL);
	virtual ~CWaveWF();

public:
	void Open(char *filename = NULL);
	virtual int	 Write(char *buf, int len);
	virtual void Close(void);

private:
	void SetWaveDefaultValue(void);
	void UpdateSize(void);

private:
	TChunkHeader	m_header;
	int				m_pos;
};

#endif

