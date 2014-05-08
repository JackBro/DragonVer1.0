
#include "internal.h"

CUfifoBuf::CUfifoBuf(int size, int unit) : CFifoBuf(size, unit)
{
}

CUfifoBuf::~CUfifoBuf()
{
}


int	CUfifoBuf::Write(char *buf, int size)
{
	return CFifoBuf::Write(buf, size);
}

int	CUfifoBuf::Read(char *buf, int size)
{
	return CFifoBuf::Read(buf, size);
}

int	CUfifoBuf::Query(int len)
{
	int val;

	CFifoBuf::PushState();
	val = this->Read(len);
	CFifoBuf::PopState();
	return val;
}

int CUfifoBuf::Write(int val, int vlen)
{
	return CFifoBuf::Write((char *)&val, vlen);
}

int	CUfifoBuf::Read(int len)
{
	int val = 0;

	len = (len > 4) ? 4 : len;
	CFifoBuf::Read((char *)&val, len);
	return val;
}

