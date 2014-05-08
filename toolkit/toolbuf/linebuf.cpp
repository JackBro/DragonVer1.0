
#include "internal.h"

CLineBuf::CLineBuf(int width, int edge, int unit) : CSramBuf(width, unit)
{
	CLineBuf::SetSize(width, edge, unit);
}

CLineBuf::~CLineBuf()
{
}

void CLineBuf::ClearPoint(void)
{
	this->m_wpoint = 0;
}

int	CLineBuf::IsFull(void)
{
	return (this->m_wpoint == this->m_width);
}

void CLineBuf::SetSize(int width, int edge, int unit)
{
	CSramBuf::SetSize(width, unit);
	if(this->GetSize())
	{
		this->m_width = width;
		this->m_edge = edge;
	}
	this->ClearPoint();
}

void CLineBuf::SetSize(char *buf, int width, int edge, int unit)
{
	CSramBuf::SetSize(buf, width, unit);
	if(this->GetSize())
	{
		this->m_width = width;
		this->m_edge = edge;
	}
	this->ClearPoint();
}

int	CLineBuf::Write(char *buf, int size)
{
	size -= size % this->m_unit;
	size = CSramBuf::Write(buf, size, this->m_wpoint * this->m_unit, this->m_width * this->m_unit);
	this->m_wpoint += size / this->m_unit;
	return size;
}

int CLineBuf::Write(int value)
{
	int pos;

	pos = CSramBuf::Write((char*)&value, this->m_unit, this->m_wpoint * this->m_unit, this->m_width * this->m_unit);
	this->m_wpoint += pos / this->m_unit;
	
	return pos;
}

int	CLineBuf::Read(char *buf, int size)
{
	if(this->m_wpoint != this->m_width)
		return 0;
	size = CSramBuf::Read(buf, size, 0, this->m_wpoint * this->m_unit);
	return size;
}

int	CLineBuf::Read(int pos)
{
	int val = 0, adr;

	if(this->m_wpoint != this->m_width)
		return 0;
	adr = this->m_width + (this->m_edge << 1);
	pos = (pos < 0) ? 0 : (pos > adr) ? adr : pos;
	if(pos < this->m_edge)
		adr = 0;
	else if(pos >= (this->m_width + this->m_edge))
		adr = (this->m_width - 1) * this->m_unit;
	else
		adr = (pos - this->m_edge) * this->m_unit;
	CSramBuf::Read((char *)&val, this->m_unit, adr, adr + this->m_unit);

	return val;
}

void CLineBuf::operator = (CLineBuf &a)
{
	CLineBuf::SetSize(a.m_width, a.m_edge, a.m_unit);
	memcpy(this->m_buf, a.m_buf, a.m_width * a.m_unit);
	this->m_wpoint = a.m_wpoint;
}

