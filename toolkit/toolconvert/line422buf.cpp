
#include "internal.h"

CLine422Buf::CLine422Buf(int width, int edge, int unit)
{
	CLine422Buf::SetSize(width, edge, unit);
	this->m_format = YUV422_UYVY;
}

CLine422Buf::~CLine422Buf()
{
}

void CLine422Buf::SetSize(int width, int edge, int unit)
{
	CLineBuf::SetSize(width << 1, edge, unit);
	if(this->GetSize())
		this->m_width = width;
}

void CLine422Buf::SetSize(char *buf, int width, int edge, int unit)
{
	CLineBuf::SetSize(buf, width << 1, edge, unit);
	if(this->GetSize())
		this->m_width = width;
}

void CLine422Buf::SetFormat(int format)
{
	if( (format > YUV422_YYUV) || (format < YUV422_UYVY) )
		format = YUV422_UYVY;
	this->m_format = format;
	this->ClearPoint();
}

int	CLine422Buf::Write(char *buf, int size)
{
	int end = (this->m_width * this->m_unit) << 1;
	int start = (this->m_wpoint * this->m_unit) << 1;

	size -= size % (this->m_unit << 2);
	size = CSramBuf::Write(buf, size, start, end);
	this->m_wpoint += size / (this->m_unit << 1);
	return size;
}

void CLine422Buf::Read(int pos, int *yuv)
{
	int pos1, tem[6];
	CYuv2Rgb tools;

	pos = (pos < this->m_edge) ? 0 : 
		((pos >= (this->m_width + this->m_edge)) ? (this->m_width - 1) : (pos - this->m_edge));
	pos1 = pos % 2;
	pos -= pos1;
	pos = (pos * this->m_unit) << 1;
	tools.getyuv422(this->m_buf + pos, tem, this->m_format, this->m_unit);
	yuv[0] = tem[pos1*3];
	yuv[1] = tem[1];
	yuv[2] = tem[2];
}

void CLine422Buf::operator = (CLine422Buf &a)
{
	CLine422Buf::SetSize(a.m_width, a.m_edge, a.m_unit);
	memcpy(this->m_buf, a.m_buf, (a.m_width * a.m_unit) << 1);
	this->m_wpoint = a.m_wpoint;
}

