#include "internal.h"


//==========================================
//
//		class CRegBase 
//
//===================================
CRegBase::CRegBase(int count)
{
	this->m_reg = new TRegister[count];
	if(this->m_reg)
		memset(this->m_reg, 0, sizeof(TRegister) * count);
}

CRegBase::~CRegBase()
{
	if(this->m_reg)
		delete [](this->m_reg);
}

void CRegBase::SetReg(int adr, int val)
{
	if(this->m_reg == NULL)
		return;
	this->m_reg[adr].val &= this->m_reg[adr].mask;
	val &= ~(this->m_reg[adr].mask);
	this->m_reg[adr].val |= val;
}

int	CRegBase::GetReg(int adr)
{
	if(this->m_reg == NULL)
		return 0;
	return this->m_reg[adr].val;
}

void CRegBase::SetRegMask(int adr, int mask)
{
	if(this->m_reg == NULL)
		return;
	this->m_reg[adr].mask = mask;
}

