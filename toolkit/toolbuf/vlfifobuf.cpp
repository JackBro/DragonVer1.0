
#include "internal.h"

CVLfifoBuf::CVLfifoBuf(int size, int unit) : CUfifoBuf(size, unit)
{
}

CVLfifoBuf::~CVLfifoBuf()
{
	
}

void CVLfifoBuf::Write(int val)
{
	int temp = 0, cnt = 1;

	temp = val & 0x7f;
	val >>= 7;
	while(val)
	{
		temp <<= 8;
		temp |= (val & 0x7f) | 0x80;
		val >>= 7;
		cnt++;
	}
	CUfifoBuf::Write(temp, cnt);
}

int	CVLfifoBuf::Read()
{
	int val = 0, temp;

	do {
		temp = CUfifoBuf::Read(1);
		val <<= 7;
		val |= temp & 0x7f;
	} while(temp & 0x80);

	return val;
}

