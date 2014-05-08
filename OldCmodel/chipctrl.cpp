

#include "dbugPrint.h"

#include "chipctrl.h"


CChipCtrl::CChipCtrl(void)
{
	this->gpBusIF = NULL;
	this->gpGE = NULL;
	this->gpIpp = NULL;
	this->gpIsp = NULL;
	this->gpJbuf = NULL;
	this->gpJpeg = NULL;
	this->gpLbuf = NULL;
	this->gpLcdIF = NULL;
	this->gpSif = NULL;

	this->RegInit();
}

CChipCtrl::~CChipCtrl(void)
{
}

void CChipCtrl::RegInit(void)
{
	CChipBase1::RegInit();
}

void CChipCtrl::SetReg(int adr, int val)
{
	CChipBase1::SetReg(adr, val);
}




