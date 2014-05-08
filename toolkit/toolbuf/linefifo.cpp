
#include "internal.h"

CLineFifo::CLineFifo(int width, int unit) : CUfifoBuf(width, unit)
{
	CLineFifo::SetSize(width, unit);
}

CLineFifo::~CLineFifo()
{
}

void CLineFifo::SetSize(int width, int unit)
{
	this->release();
	CUfifoBuf::SetSize(width, unit);
	this->m_width = width;
	this->m_linewidth = this->m_width * this->m_unit;
}

void CLineFifo::SetSize(char *buf, int width, int unit)
{
	CUfifoBuf::SetSize(buf, width, unit);
	this->m_width = width;
	this->m_linewidth = this->m_width * this->m_unit;
}

int	CLineFifo::Write(char *buf, int size)
{
	if(this->IsFifoValid())
		return 0;
	return CUfifoBuf::Write(buf, size);
}

int	CLineFifo::IsFifoValid(void)
{
	if( (this->GetWritePoint() == this->m_linewidth) ||
		((CUfifoBuf::IsFifoEmpty() == 0) && (CUfifoBuf::GetWritePoint() == 0)) )
		return 1;
	return 0;
}

int	CLineFifo::Read(char *buf, int size)
{
	if(this->IsFifoValid())
		return CUfifoBuf::Read(buf, size);
	return 0;
}

int	CLineFifo::Write(int val)
{
	return CLineFifo::Write((char *)&val, this->m_unit);
}

int	CLineFifo::Read(void)
{
	return CUfifoBuf::Read(this->m_unit);
}

