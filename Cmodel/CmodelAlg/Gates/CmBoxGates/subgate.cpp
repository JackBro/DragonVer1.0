#include "internal.h"


CSubGate::CSubGate(void) : CImageUnit(2, 1, "Sub Gate", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_alg1;
	this->SetInportFormat(0, g_GatesFmtCount, (int *)g_GateFormats);
	this->SetInportFormat(1, g_GatesFmtCount, (int *)g_GateFormats);
}

CSubGate::~CSubGate(void)
{
}

int	CSubGate::GetID(void)
{
	return SUBGATE_BOX | (LOGICCLASS << CLASSSHIFT);;
}

void CSubGate::UpdateConfig(void)
{
	//body config
}

void CSubGate::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_alg1;
	CBaseUnit::ChoiceAlg(sel);
}

