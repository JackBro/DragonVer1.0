#include "internal.h"


CAndGate::CAndGate(void) : CImageUnit(2, 1, "And Gate", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_and1alg;
	this->SetInportFormat(0, g_GatesFmtCount, (int *)g_GateFormats);
	this->SetInportFormat(1, g_GatesFmtCount, (int *)g_GateFormats);
}

CAndGate::~CAndGate(void)
{
}

int	CAndGate::GetID(void)
{
	return LOGICALANDGATE_BOX | (LOGICCLASS << CLASSSHIFT);
}

void CAndGate::UpdateConfig(void)
{
	//body config
}

void CAndGate::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_and1alg;
	CBaseUnit::ChoiceAlg(sel);
}

