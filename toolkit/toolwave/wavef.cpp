
#include "internal.h"

CWaveF::CWaveF(char *wavf)
{
	this->Open(wavf);
}

void CWaveF::Open(char *wavf)
{
	memset(&this->m_chunk, 0, sizeof(TChunkList));
	CFileBin::Open(wavf);
	if(this->GetWaveFInfo() != 0)
		CFileBin::Close();
}

int CWaveF::FindChunk(int flag, int *size, int *pos)
{
	int fcc, len, len1;

	this->Read((char *)&fcc, 4);
	while(fcc != flag)
	{
		len1 = this->Read((char *)&len, 4);
		if(len1 != 4)
			return 1;
		*pos += 8+len;
		this->Fseek(*pos, SEEK_SET);
		len1 = this->Read((char *)&fcc, 4);
		if(len1 != 4)
			return 1;
	}
	this->Read((char *)size, 4);
	*pos += 8;
	return 0;
}

int CWaveF::GetWaveFInfo(void)
{
	int fcc, pos = 0;
	CBufferBase pool;

	this->Fseek(0, SEEK_SET);
	if( this->FindChunk(RIFF_FLAG, &this->m_chunk.hdrsize, &pos) )
		return 1;
	this->m_chunk.hdr = pos;

	this->Read((char *)&fcc, 4);
	if(fcc != WAVE_FLAG)
		return 1;
	pos += 4;

	if( this->FindChunk(FMT_FLAG, &this->m_chunk.fmtsize, &pos) )
		return 1;
	this->m_chunk.fmt = pos;

	pool.SetSize(this->m_chunk.fmtsize);
	this->Read(pool.m_buf, this->m_chunk.fmtsize);
	pos += this->m_chunk.fmtsize;
	CWaveBase::SetFormat((WAVEFORMATEX *)pool.m_buf);
	if(this->GetFormat() != WAVE_FORMAT_PCM)
		return 1;

	if( this->FindChunk(DATA_FLAG, &this->m_chunk.datsize, &pos) )
		return 1;
	this->m_chunk.dat = pos;

	return 0;
}

int CWaveF::GetHeaderSize() const
{
	return this->m_chunk.dat;
}

int CWaveF::GetHeaderData(char* buf, int nLength)
{
	this->Fseek(0, SEEK_SET);
	if (nLength >= this->m_chunk.dat) 
	{
		return this->Read(buf, m_chunk.dat);
	}
	else
	{
		return this->Read(buf, nLength);
	}
}

int CWaveF::GetDataSize() const
{
	return this->m_chunk.datsize;
}

int CWaveF::GetData(char* buf, int nLength)
{
	this->SeekChunk(SEEK_WAVEDATA);
	if (nLength >= this->m_chunk.datsize) 
	{
		return this->Read(buf, this->m_chunk.datsize);
	}
	else
	{
		return this->Read(buf, nLength);
	}
}

void CWaveF::SeekChunk(int index)
{
	if(index == SEEK_WAVEHDR)
		this->Fseek(this->m_chunk.hdr, SEEK_SET);
	else if(index == SEEK_WAVEFMT)
		this->Fseek(this->m_chunk.fmt, SEEK_SET);
	else
		this->Fseek(this->m_chunk.dat, SEEK_SET);
}

