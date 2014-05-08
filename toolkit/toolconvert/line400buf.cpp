
#include "internal.h"

CLine400Buf::CLine400Buf(int width, int edge, int unit)
{
	CLine400Buf::SetSize(width, edge, unit);
}

CLine400Buf::~CLine400Buf()
{
}

void CLine400Buf::SetSize(int width, int edge, int unit)
{
	CLineBuf::SetSize(width, edge, unit);
	if(this->GetSize())
		this->m_width = width;
}

void CLine400Buf::SetSize(char *buf, int width, int edge, int unit)
{
	CLineBuf::SetSize(buf, width, edge, unit);
	if(this->GetSize())
		this->m_width = width;
}

int	CLine400Buf::Write(char *buf, int size)
{
	int end = this->m_width * this->m_unit;
	int start = this->m_wpoint * this->m_unit;

	size -= size % this->m_unit;
	size = CSramBuf::Write(buf, size, start, end);
	this->m_wpoint += size / this->m_unit;
	return size;
}

void CLine400Buf::Read(int pos, int *yuv)
{
	CYuv2Rgb tools;

	pos = (pos < this->m_edge) ? 0 : ((pos >= (this->m_width + this->m_edge)) ? 
		(this->m_width - 1) : (pos - this->m_edge));
	pos *= this->m_unit;
	tools.getyuv400(this->m_buf + pos, yuv, this->m_unit);
	yuv[1] = 0x80;
	yuv[2] = 0x80;
}

void CLine400Buf::operator = (CLine400Buf &a)
{
	CLine400Buf::SetSize(a.m_width, a.m_edge, a.m_unit);
	memcpy(this->m_buf, a.m_buf, a.m_width * a.m_unit);
	this->m_wpoint = a.m_wpoint;
}

