
#include "stream.h"

CStream::CStream(int count)
{
	this->SetSize(count);
}

void CStream::SetSize(int size)
{
	CSramBuf::SetSize(size, sizeof(TStream));
	this->Clear();
}

void CStream::Clear(void)
{
	int i;
	TStream dat;

	memset(&dat, 0, sizeof(TStream));

	for(i = 0; i < this->m_size; i++)
		this->Write(&dat, i);
}

int CStream::Write(TStream *stream, int index)
{
	return CSramBuf::Write((char *)stream, sizeof(TStream), index * sizeof(TStream));
}

int CStream::Read(TStream *stream, int index)
{
	return CSramBuf::Read((char *)stream, sizeof(TStream), index * sizeof(TStream));
}

