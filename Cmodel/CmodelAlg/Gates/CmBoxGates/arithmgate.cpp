#include "internal.h"

#define TOTAL_ARITHM_GATES 4

static char *Arithm_BoxDesc[]=
{
	"Add fixdata Gate",
	"Sub fixdata Gate",
	"Mul fixdata Gate",
	"Div fixdata Gate"
};

CArithmGate::CArithmGate(void) : CImageUnit(1, 1, "Arithmetic Gate", 10)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->SetInportFormat(0, g_GatesFmtCount, (int *)g_GateFormats);
	this->m_pAlg = &this->m_alg1;
	this->m_UpClip = 255;
	this->m_LowClip = -256;
	this->m_operator = OP_ADD;
	this->m_InUnit = 1;
	this->m_OutUnit = 1;
	this->m_opdata = 0;
}

CArithmGate::~CArithmGate(void)
{
}

int	CArithmGate::GetID(void)
{
	return ARITHMGATE_BOX | (LOGICCLASS << CLASSSHIFT);;
}

void CArithmGate::UpdateConfig(void)
{
	this->m_alg1.SetUpClip(this->m_UpClip);
	this->m_alg1.SetLowClip(this->m_LowClip);
	this->m_alg1.SetInUnit(this->m_InUnit);
	this->m_alg1.SetOutUnit(this->m_OutUnit);
	this->m_alg1.SetOpdata(this->m_opdata);
	this->m_alg1.SetOperator(this->m_operator);
}

void CArithmGate::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_alg1;
	CBaseUnit::ChoiceAlg(sel);
}

void CArithmGate::SetUpClip(int val)
{
	this->m_UpClip = val;
}

int	CArithmGate::GetUpClip(void)
{
	return this->m_UpClip;
}

void CArithmGate::SetLowClip(int val)
{
	this->m_LowClip = val;
}

int	CArithmGate::GetLowClip(void)
{
	return this->m_LowClip;
}

void CArithmGate::SetInUnit(int val)
{
	this->m_InUnit = val;
}

int	CArithmGate::GetInUnit(void)
{
	return this->m_InUnit;
}

void CArithmGate::SetOutUnit(int val)
{
	this->m_OutUnit = val;
}

int	CArithmGate::GetOutUnit(void)
{
	return this->m_OutUnit;
}

void CArithmGate::SetOpdata(int val)
{
	this->m_opdata = val;
}

int	CArithmGate::GetOpdata(void)
{
	return this->m_opdata;
}

void CArithmGate::SetOperator(int val)
{
	if (val >= TOTAL_ARITHM_GATES)
		return;

	this->SetDesc(Arithm_BoxDesc[val]);
	this->m_operator = val;
}

int CArithmGate::GetOperator(void)
{
	return this->m_operator;
}