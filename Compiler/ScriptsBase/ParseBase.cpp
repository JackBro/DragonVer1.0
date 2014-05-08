#include "internal.h"


CParseBase::CParseBase(void)
{
	this->m_pfather = NULL;
	this->m_psibing = NULL;
}

CParseBase::~CParseBase(void)
{
}

CParseBase *CParseBase::GetFather(void)
{
	return this->m_pfather;
}

void CParseBase::SetFather(CParseBase *pf)
{
	this->m_pfather = pf;
}

CParseBase *CParseBase::GetSibling(void)
{
	return this->m_psibing;
}

void CParseBase::SetSilbing(CParseBase *ps)
{
	this->m_psibing = ps;
}

