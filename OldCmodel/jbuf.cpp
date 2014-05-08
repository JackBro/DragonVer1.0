

#include "dbugPrint.h"

#include "jbuf.h"

CJbuf::CJbuf(void)
{
	this->RegInit();
}

CJbuf::~CJbuf(void)
{
}

void CJbuf::RegInit(void)
{
	CChipBase1::RegInit();
}

void CJbuf::SetReg(int adr, int val)
{
	CChipBase1::SetReg(adr, val);
}




