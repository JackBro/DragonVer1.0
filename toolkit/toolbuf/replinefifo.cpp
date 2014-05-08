
#include "internal.h"

CRepLineFifo::CRepLineFifo(int width, int unit) : CLineFifo(width, unit)
{
	CRepLineFifo::SetSize(width, unit);
}

CRepLineFifo::~CRepLineFifo()
{
}

int	CRepLineFifo::Write(char *buf, int size)
{
	if(this->m_flag == REPLINE_WRITE)
		return 0;
	if(this->m_flag == REPLINE_READ)
	{
		CFifoBuf::ClearPoint();
		this->m_flag = REPLINE_IDLE;
	}
	size = CLineFifo::Write(buf, size);
	if(CLineFifo::IsFifoValid())
		this->m_flag = REPLINE_WRITE;
	return size;
}

int	CRepLineFifo::Read(char *buf, int size)
{
	if(this->m_flag == REPLINE_IDLE)
		return 0;
	CFifoBuf::SetReadPoint(0);
	this->m_flag = REPLINE_READ;
	return CFifoBuf::Read(buf, size);
}

void CRepLineFifo::SetSize(int width, int unit)
{
	CLineFifo::SetSize(width, unit);
	this->m_flag = REPLINE_IDLE;
}

void CRepLineFifo::SetSize(char *buf, int width, int unit)
{
	CLineFifo::SetSize(buf, width, unit);
	this->m_flag = REPLINE_IDLE;
}

int	CRepLineFifo::GetFreeSize(void)
{
	if(this->m_flag == REPLINE_WRITE)
		return 0;
	if(this->m_flag == REPLINE_READ)
		return this->m_totalsize;
	return CFifoBuf::GetFreeSize();
}

