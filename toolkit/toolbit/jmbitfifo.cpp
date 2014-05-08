
#include "internal.h"

CJmBitFifo::CJmBitFifo(int size, int order) : CBitFifo(size, order)
{
}

int	CJmBitFifo::Write(char *buf, int len)
{
	int size;

	size = this->GetFreeSize();
	size = (size >> 3) << 3;
	len = len > size ? size : len;
	return CBitFifo::Write(buf, len << 3);
}

int	CJmBitFifo::Read(char *buf, int len)
{
	int size;

	size = this->GetDataSize();
	size = (size >> 3) << 3;
	len = len > size ? size : len;
	return CBitFifo::Read(buf, len);
}


