
#include "internal.h"


CLine444Buf::CLine444Buf(int width, int edge, int unit)
{
	CLine444Buf::SetSize(width, edge, unit);
}

CLine444Buf::~CLine444Buf()
{
}

void CLine444Buf::SetSize(int width, int edge, int unit)
{
	CLineBuf::SetSize(width * 3, edge, unit);
	if(this->GetSize())
		this->m_width = width;
}

void CLine444Buf::SetSize(char *buf, int width, int edge, int unit)
{
	CLineBuf::SetSize(buf, width * 3, edge, unit);
	if(this->GetSize())
		this->m_width = width;
}

int	CLine444Buf::Write(char *buf, int size)
{
	int end, start;

	size -= size % (this->m_unit * 3);
	start = this->m_wpoint * this->m_unit * 3;
	end = this->m_width * this->m_unit * 3;
	size = CSramBuf::Write(buf, size, start, end);
	this->m_wpoint += size / (this->m_unit * 3);
	return size;
}

void CLine444Buf::Read(int pos, int *yuv)
{
	CYuv2Rgb tools;

	pos = (pos < this->m_edge) ? 0 : ((pos >= (this->m_width + this->m_edge)) ? 
		(this->m_width - 1) : (pos - this->m_edge));
	pos *= 3 * this->m_unit;
	tools.getyuv444(this->m_buf + pos, yuv, this->m_unit);
}

void CLine444Buf::operator = (CLine444Buf &a)
{
	int size;

	size = a.m_width * a.m_unit * 3;
	CLine444Buf::SetSize(a.m_width, a.m_edge, a.m_unit);
	memcpy(this->m_buf, a.m_buf, size);
	this->m_wpoint = a.m_wpoint;
}

