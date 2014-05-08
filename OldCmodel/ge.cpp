

#include "dbugPrint.h"

#include "ge.h"


CGE::CGE(void)
{
	this->RegInit();
}

CGE::~CGE(void)
{
}

void CGE::RegInit(void)
{
	CChipBase1::RegInit();
}

void CGE::SetReg(int adr, int val)
{
	CChipBase1::SetReg(adr, val);
}



