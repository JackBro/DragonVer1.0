#include "internal.h"


CComGate::CComGate(void) : CImageUnit(1, 4, "Com Gate", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_com1alg;
	this->SetInportFormat(0, g_GatesFmtCount, (int *)g_GateFormats);
}

CComGate::~CComGate(void)
{
}

int	CComGate::GetID(void)
{
	return LOGICALCOMGATE_BOX | (LOGICCLASS << CLASSSHIFT);
}

void CComGate::UpdateConfig(void)
{
	//body config
}

void CComGate::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_com1alg;
	CBaseUnit::ChoiceAlg(sel);
}

