

#include "dbugPrint.h"
#include "lcdif.h"

CLcdIF::CLcdIF(void)
{
	this->RegInit();
}

CLcdIF::~CLcdIF(void)
{
}

void CLcdIF::RegInit(void)
{
	CChipBase1::RegInit();
}

void CLcdIF::SetReg(int adr, int val)
{
	CChipBase1::SetReg(adr, val);
}




