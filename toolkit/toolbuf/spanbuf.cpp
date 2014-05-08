
#include "internal.h"

CSpanBuf::CSpanBuf(int size, int unit) : CSramBuf(size, unit)
{
	this->SetOption(0, 1, 0);
}

CSpanBuf::~CSpanBuf()
{
}

void CSpanBuf::SetSize(int size, int unit)
{
	CSramBuf::SetSize(size, unit);
	this->SetOption(0, 1, 0);
}

void CSpanBuf::SetSize(char *buf, int size, int unit)
{
	CSramBuf::SetSize(buf, size, unit);
	this->SetOption(0, 1, 0);
}

void CSpanBuf::SetOption(int start, int span, int skip)
{
	this->m_span = (span <= 0) ? 1 : span;
	this->m_skip = (skip < 0) ? 0 : skip;
	this->m_start = (start < 0) ? 0 : start;
}

int	CSpanBuf::Read(char *buf, int size)
{
	int len = 0, flag = 1, leng, span, start;

	if(this->m_skip == 0)
		return CSramBuf::Read(buf, size, this->m_start);

	start = this->m_start;
	while(flag)
	{
		span = (size > this->m_span) ? this->m_span : size;
		leng = CSramBuf::Read(buf+len, span, start);
		len += leng;
		size -= leng;
		start += leng + this->m_skip;
		if( (leng == 0) || (size == 0) )
			flag = 0;
	}
	return len;
}

int	CSpanBuf::Write(char *buf, int size)
{
	int len = 0, flag = 1, leng, span, start;

	if(this->m_skip == 0)
		return CSramBuf::Write(buf, size, this->m_start);

	start = this->m_start;
	while(flag)
	{
		span = (size > this->m_span) ? this->m_span : size;
		leng = CSramBuf::Write(buf+len, span, start);
		len += leng;
		size -= leng;
		start += leng + this->m_skip;
		if( (leng == 0) || (size == 0) )
			flag = 0;
	}
	return len;
}

int	CSpanBuf::Write(char *buf, int size, int start, int span, int skip)
{
	this->SetOption(start, span, skip);
	return CSpanBuf::Write(buf, size);
}

int	CSpanBuf::Read(char *buf, int size, int start, int span, int skip)
{
	this->SetOption(start, span, skip);
	return CSpanBuf::Read(buf, size);
}

int CSpanBuf::ReadOne(int pos)
{
	int i, j, k, index;
	int val = 0;

	i = pos % (this->m_span/this->m_unit);
	j = pos / (this->m_span/this->m_unit);
	index = this->m_start + (this->m_span + this->m_skip)*j + i*this->m_unit;
	for(k = 0; k < this->m_unit; k++)
		val |= (((int)(*(this->m_buf + index + k))&0xff) << (8*k));

	return val;
}

int CSpanBuf::WriteOne(int value, int pos)
{
	int i, j, k, index;
	
	i = pos % (this->m_span/this->m_unit);
	j = pos / (this->m_span/this->m_unit);
	index = this->m_start + (this->m_span + this->m_skip)*j + i*this->m_unit;
	for(k = 0; k < this->m_unit; k++)
		*(this->m_buf+index+k) = (char)((value >> (8*k))&0xff);
	return k;
}