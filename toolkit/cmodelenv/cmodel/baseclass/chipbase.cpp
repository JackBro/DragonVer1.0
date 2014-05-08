

#include "chipbase.h"

//==================== class CChipBase ===================

CChipBase::CChipBase(TModulePara para) : CModuleBase(para)
{
}

CChipBase::~CChipBase()
{
}

void CChipBase::InitModuleIndex(void)
{
}

void CChipBase::OnWriteReg(int adr, int val)
{
	adr = val;
}

void CChipBase::InitRegProp(void)
{
}

int CChipBase::CanRun(int clk)
{
	clk = 0;
	return 1;
}

void CChipBase::doProcess(TStream *input, TStream *output)
{
	input = output;
}

void CChipBase::SelfDriver(TStream *input, int index)
{
	input = input;
	index = 0;
}

void CChipBase::ResetCounter(void)
{
}
