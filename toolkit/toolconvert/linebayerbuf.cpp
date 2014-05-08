
#include "internal.h"


CLineBayerBuf::CLineBayerBuf(int width, int edge, int unit) : CLineBuf(width, edge, unit)
{
}

CLineBayerBuf::~CLineBayerBuf()
{
}

int	CLineBayerBuf::Read(int pos)
{
	int val = 0, adr;

	adr = this->m_width + (this->m_edge << 1);
	pos = (pos < 0) ? 0 : (pos > adr) ? adr : pos;
	if(pos < this->m_edge)
		adr = ((this->m_edge - pos) % 2) * this->m_unit;
	else if(pos >= (this->m_width + this->m_edge))
	{
		pos -= this->m_width + this->m_edge - 1;
		adr = this->m_width - 1 - (pos % 2);
		adr *= this->m_unit;
	}
	else
		adr = (pos - this->m_edge) * this->m_unit;
	CSramBuf::Read((char *)&val, this->m_unit, adr, adr + this->m_unit);

	return val;
}

void CLineBayerBuf::operator = (CLineBayerBuf &a)
{
	CLineBayerBuf::SetSize(a.m_width, a.m_edge, a.m_unit);
	memcpy(this->m_buf, a.m_buf, a.GetTotalSize());
	this->m_wpoint = a.m_wpoint;
}

