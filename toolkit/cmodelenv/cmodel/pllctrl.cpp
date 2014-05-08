

#include "pllctrl.h"

//==================== class CPllCtrlModule ===================

CPllCtrlModule::CPllCtrlModule() : CChipBase(gChipPara[IDD_PLLCTRL])
{
	this->InitProcess();
}

int	CPllCtrlModule::Process(int clk)
{
	if(this->m_pchip == NULL)
		return clk;
	if(clk != CMODEL_CLKHIGH)
		return clk;
	this->m_pchip->Run0(clk);
	return clk;
}
