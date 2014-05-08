
#include "internal.h"

CSramBuf::CSramBuf(int size, int unit) : CBufferBase(size, unit)
{
	this->SetOption(0, 0);
}

void CSramBuf::SetSize(int size, int unit)
{
	CBufferBase::SetSize(size, unit);
	this->SetOption(0, 0);
}

void CSramBuf::SetSize(char *buf, int size, int unit)
{
	CBufferBase::SetSize(buf, size, unit);
	this->SetOption(0, 0);
}

int	CSramBuf::Read(char *buf, int size)
{
	if(this->m_start >= this->m_end)
		return 0;
	size = (size > (this->m_end - this->m_start)) ? (this->m_end - this->m_start) : size;
	memcpy(buf, this->m_buf+this->m_start, size);
	return size;
}

int	CSramBuf::Write(char *buf, int size)
{
	if(this->m_start >= this->m_end)
		return 0;
	size = (size > (this->m_end - this->m_start)) ? (this->m_end - this->m_start) : size;
	memcpy(this->m_buf+this->m_start, buf, size);
	return size;
}

int	CSramBuf::Write(char *buf, int size, int start, int end)
{
	this->SetOption(start, end);
	if( (size <= 0) || (buf == NULL) )
		return 0;
	return CSramBuf::Write(buf, size);
}

int	CSramBuf::Read(char *buf, int size, int start, int end)
{
	this->SetOption(start, end);
	if( (size <= 0) || (buf == NULL) )
		return 0;
	return CSramBuf::Read(buf, size);
}

void CSramBuf::SetOption(int start, int end)
{
	this->m_start = (start < 0) ? 0 : start;
	this->m_end = ( (end < 0) || (end > this->m_totalsize) ) ? this->m_totalsize : end;
}





