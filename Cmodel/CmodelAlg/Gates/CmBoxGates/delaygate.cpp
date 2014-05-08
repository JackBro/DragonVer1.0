
#include "internal.h"

CDelayGate::CDelayGate(void) : CImageUnit(1, 1, "DelayCell", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_delay1alg;
	this->SetInportFormat(0, g_GatesFmtCount, (int *)g_GateFormats);
	this->m_delay = 0;
}

CDelayGate::~CDelayGate(void)
{
}

int	CDelayGate::GetID(void)
{
	return DELAYCELL_BOX | (LOGICCLASS << CLASSSHIFT);;
}

void CDelayGate::UpdateConfig(void)
{
	this->m_delay1alg.SetDelay(this->m_delay);
}

void CDelayGate::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_delay1alg;
	CBaseUnit::ChoiceAlg(sel);
}

void CDelayGate::SetDelay(int delay)
{
	this->m_delay = delay;
}

int	CDelayGate::GetDelay(void)
{
	return this->m_delay;
}

