
#include "internal.h"

CNodeBase::CNodeBase(void)
{
}

CNodeBase::~CNodeBase(void)
{
}

int CNodeBase::GetNodeProp(void)
{
	return this->m_prop;
}

void CNodeBase::SetNodeProp(int sel)
{
	this->m_prop = sel;
}

