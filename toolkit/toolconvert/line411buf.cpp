
#include "internal.h"

CLine411Buf::CLine411Buf(int width, int edge, int unit)
{
	CLine411Buf::SetSize(width, edge, unit);
	this->m_format = YUV411_YYYYUV;
}

CLine411Buf::~CLine411Buf()
{
}

void CLine411Buf::SetSize(int width, int edge, int unit)
{
	CLineBuf::SetSize((width * 3) >> 1, edge, unit);
	if(this->GetSize())
		this->m_width = width;
}

void CLine411Buf::SetSize(char *buf, int width, int edge, int unit)
{
	CLineBuf::SetSize(buf, (width * 3) >> 1, edge, unit);
	if(this->GetSize())
		this->m_width = width;
}

void CLine411Buf::SetFormat(int format)
{
	if( (format > YUV411_VUYYYY) || (format < YUV411_YYYYUV) )
		format = YUV411_YYYYUV;
	this->m_format = format;
	this->ClearPoint();
}

int	CLine411Buf::Write(char *buf, int size)
{
	int end = (this->m_width * 3 * this->m_unit) >> 1;
	int start = (this->m_wpoint * 3 * this->m_unit) >> 1;

	size -= size % (this->m_unit * 6);
	size = CSramBuf::Write(buf, size, start, end);
	this->m_wpoint += size / ((this->m_unit * 3) >> 1);
	return size;
}

void CLine411Buf::Read(int pos, int *yuv)
{
	int pos1, tem[12];
	CYuv2Rgb tools;

	pos = (pos < this->m_edge) ? 0 : 
		((pos >= (this->m_width + this->m_edge)) ? (this->m_width - 1) : (pos - this->m_edge));
	pos1 = pos % 4;
	pos -= pos1;
	pos = (pos * this->m_unit * 3) >> 1;
	tools.getyuv411(this->m_buf + pos, tem, this->m_format, this->m_unit);
	yuv[0] = tem[pos1*3];
	yuv[1] = tem[1];
	yuv[2] = tem[2];
}

void CLine411Buf::operator = (CLine411Buf &a)
{
	CLine411Buf::SetSize(a.m_width, a.m_edge, a.m_unit);
	memcpy(this->m_buf, a.m_buf, (a.m_width * a.m_unit * 3) >> 1);
	this->m_wpoint = a.m_wpoint;
}

