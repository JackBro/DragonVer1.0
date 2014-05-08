#include "internal.h"


CNotGate::CNotGate(void) : CImageUnit(1, 4, "Not Gate", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_not1alg;
	this->SetInportFormat(0, g_GatesFmtCount, (int *)g_GateFormats);
}

CNotGate::~CNotGate(void)
{
}

int	CNotGate::GetID(void)
{
	return LOGICALNOTGATE_BOX | (LOGICCLASS << CLASSSHIFT);;
}

void CNotGate::UpdateConfig(void)
{
	//body config
}

void CNotGate::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_not1alg;
	CBaseUnit::ChoiceAlg(sel);
}

