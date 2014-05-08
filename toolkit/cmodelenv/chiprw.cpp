#include "chiprw.h"
#include "main.h"

void Cmodel_SetReg(int adr, int val)
{
	if(gpchip)
		gpchip->SetRegister(adr, val);
}

int Cmodel_GetReg(int adr)
{
	if(gpchip)
		return gpchip->GetRegister(adr);
	return 0;
}





