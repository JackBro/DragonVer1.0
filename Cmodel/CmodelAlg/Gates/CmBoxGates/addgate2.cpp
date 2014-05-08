#include "internal.h"

CAdd2Gate::CAdd2Gate(void) : CImageUnit(1, 1, "Add2 Gate", 10)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->SetInportFormat(0, g_GatesFmtCount, (int *)g_GateFormats);
	this->m_pAlg = &this->m_alg2;
	this->m_clip = 255;
	this->m_InUnit = 1;
	this->m_OutUnit = 1;
	this->m_opdata = 0;
}

CAdd2Gate::~CAdd2Gate(void)
{
}

int	CAdd2Gate::GetID(void)
{
	return ADD2GATE_BOX | (LOGICCLASS << CLASSSHIFT);;
}

void CAdd2Gate::UpdateConfig(void)
{
	this->m_alg2.SetClip(this->m_clip);
	this->m_alg2.SetInUnit(this->m_InUnit);
	this->m_alg2.SetOutUnit(this->m_OutUnit);
	this->m_alg2.SetOpdata(this->m_opdata);
}

void CAdd2Gate::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_alg2;
	CBaseUnit::ChoiceAlg(sel);
}

void CAdd2Gate::SetClip(int val)
{
	this->m_clip = val;
}

int	CAdd2Gate::GetClip(void)
{
	return this->m_clip;
}

void CAdd2Gate::SetInUnit(int val)
{
	this->m_InUnit = val;
}

int	CAdd2Gate::GetInUnit(void)
{
	return this->m_InUnit;
}

void CAdd2Gate::SetOutUnit(int val)
{
	this->m_OutUnit = val;
}

int	CAdd2Gate::GetOutUnit(void)
{
	return this->m_OutUnit;
}

void CAdd2Gate::SetOpdata(int val)
{
	this->m_opdata = val;
}

int	CAdd2Gate::GetOpdata(void)
{
	return this->m_opdata;
}

