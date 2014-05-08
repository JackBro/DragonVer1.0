
#include "internal.h"

//#define SIZEOF_PCMFMT	0x10

CWaveWF::CWaveWF(char *wavf)
{
	this->Open(wavf);
}

CWaveWF::~CWaveWF(void)
{
	this->Close();
}

void CWaveWF::Open(char *wavf)
{
	memset(&this->m_header, 0, sizeof(TChunkHeader));
	CFileBin::Open(wavf, "wb");
	this->SetWaveDefaultValue();	
	this->m_pos = this->m_header.filesize;
	this->Fseek(this->m_pos, SEEK_SET);
}

void CWaveWF::SetWaveDefaultValue(void)
{
	this->m_header.hdr = RIFF_FLAG;
	this->m_header.type = WAVE_FLAG;
	this->m_header.fmt = FMT_FLAG;
	this->m_header.dataid = DATA_FLAG;
	this->m_header.fmtsize = sizeof(WAVEFORMATEX);
	this->m_header.datasize = 0;
	this->m_header.filesize = this->m_header.fmtsize + this->m_header.datasize + sizeof(TChunkHeader);
}

void CWaveWF::Close(void)
{
	if(CFileBase::IsOpened() == 0)
		return;
	this->UpdateSize();
	this->Fseek(0, SEEK_SET);
	this->Write((char*)(&this->m_header), 20);
	this->Write((char*)(&this->m_Format), sizeof(WAVEFORMATEX));//SIZEOF_PCMFMT
	this->Write((char*)(&this->m_header.dataid), 8);
	CFileBin::Close();
}

void CWaveWF::UpdateSize(void)
{
	this->m_header.filesize = this->m_pos - 8;
	this->m_header.datasize = this->m_pos - this->m_header.fmtsize - sizeof(TChunkHeader);
}

int CWaveWF::Write(char *buf, int len)
{
	len = CFileBin::Write(buf, len);
	this->m_pos += len;

	return len;
}

