
#include "internal.h"

COrGate::COrGate(void) : CImageUnit(2, 1, "OR Gate", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_or1alg;
	this->SetInportFormat(0, g_GatesFmtCount, (int *)g_GateFormats);
	this->SetInportFormat(1, g_GatesFmtCount, (int *)g_GateFormats);
}

COrGate::~COrGate(void)
{
}

int	COrGate::GetID(void)
{
	return LOGICALORGATE_BOX | (LOGICCLASS << CLASSSHIFT);;
}

void COrGate::UpdateConfig(void)
{
	//body config
}

void COrGate::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_or1alg;
	CBaseUnit::ChoiceAlg(sel);
}

