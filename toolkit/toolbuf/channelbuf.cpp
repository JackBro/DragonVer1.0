
#include "internal.h"

CChannelBuf::CChannelBuf(int size, int unit) : CSramBuf(size, unit)
{
	this->ClearPoint();
}

CChannelBuf::~CChannelBuf()
{
}

void CChannelBuf::ClearPoint(void)
{
	this->m_point = 0;
}

void CChannelBuf::SetSize(int size, int unit)
{
	CSramBuf::SetSize(size, unit);
	this->ClearPoint();
}

void CChannelBuf::SetSize(char *buf, int size, int unit)
{
	CSramBuf::SetSize(buf, size, unit);
	this->ClearPoint();
}

int	CChannelBuf::Write(char *buf, int size)
{
	int len, offset;
	char *pbuf = buf;

	offset = size > this->m_totalsize ? size - this->m_totalsize : 0;
	size -= offset;
	pbuf += offset;
	len = CSramBuf::Write(pbuf, size, this->m_point);
	this->m_point += len;
	this->m_point %= this->m_totalsize;
	size -= len;
	pbuf += len;
	if(size)
	{
		len = CSramBuf::Write(pbuf, size, this->m_point);
		this->m_point += len;
		this->m_point %= this->m_totalsize;
	}
	return size;
}

int	CChannelBuf::Read(char *buf, int size)
{
	int point, i;
	char *pbuf = buf;

	point = (this->m_point + (this->m_totalsize - this->m_unit)) % this->m_totalsize;
	for(i = 0; i < size; i += this->m_unit, pbuf += this->m_unit)
		CSramBuf::Read(pbuf, this->m_unit, point);
	return size;
}

