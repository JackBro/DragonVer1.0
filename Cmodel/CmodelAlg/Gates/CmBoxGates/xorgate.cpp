#include "internal.h"


CXorGate::CXorGate(void) : CImageUnit(2, 1, "Xor Gate", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_xor1alg;
	this->SetInportFormat(0, g_GatesFmtCount, (int *)g_GateFormats);
	this->SetInportFormat(1, g_GatesFmtCount, (int *)g_GateFormats);
}

CXorGate::~CXorGate(void)
{
}

int	CXorGate::GetID(void)
{
	return LOGICALXORGATE_BOX | (LOGICCLASS << CLASSSHIFT);;
}

void CXorGate::UpdateConfig(void)
{
	//body config
}

void CXorGate::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_xor1alg;
	CBaseUnit::ChoiceAlg(sel);
}

