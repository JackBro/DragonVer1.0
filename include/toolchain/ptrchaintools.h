#ifndef _PTRCHAIN_TOOLS_H_
#define _PTRCHAIN_TOOLS_H_


#include "chaintools.h"


#define TEMPLATE_PCHAIN		template <class T1>
#define CPCHAINTOOL			CPChainTool <T1>

TEMPLATE_PCHAIN
class  CPChainTool : public CChainTool<T1 *>
{
public:
	CPChainTool(void);
	virtual ~CPChainTool();

public:
	virtual void Add(T1 * punit = NULL);
	virtual void DeleteUnit(T1 * punit);
	T1* operator[](int index);

public:
	T1* GetLastUnit(void);
	T1* GetFirstUnit(void);
	T1* GetUnit(int index);
	T1* GetNextUnit(T1* unit);
};

TEMPLATE_PCHAIN
CPCHAINTOOL::CPChainTool(void)
{
}

TEMPLATE_PCHAIN
CPCHAINTOOL::~CPChainTool(void)
{
}

TEMPLATE_PCHAIN
void CPCHAINTOOL::Add(T1 * punit)
{
	T1 **pu;

	if(punit == NULL)
		return;
	//pu = CChainTool<T1 *>::Add();
	pu = __super::Add();
	*pu = punit;
}

TEMPLATE_PCHAIN
void CPCHAINTOOL::DeleteUnit(T1 * punit)
{
	T1 **pu = NULL, **pu1 = NULL;
	
	if(punit == NULL)
		return;
	pu = __super::GetFirstUnit();
	while(pu != NULL)
	{
		if(*pu == punit)
		{
			pu1 = __super::GetNextUnit(pu);
			//CChainTool<CUnitBase *>::DeleteUnit(pu);
			__super::DeleteUnit(pu);
			pu = pu1;
		}
		pu = __super::GetNextUnit(pu);
	}
}

TEMPLATE_PCHAIN
T1* CPCHAINTOOL::GetLastUnit(void)
{
	T1 **punit;

	punit = __super::GetLastUnit();
	if(punit != NULL)
		return *punit;
	return NULL;
}

TEMPLATE_PCHAIN
T1* CPCHAINTOOL::GetFirstUnit(void)
{
	T1 **punit;

	punit = __super::GetFirstUnit();
	if(punit != NULL)
		return *punit;
	return NULL;
}

TEMPLATE_PCHAIN
T1* CPCHAINTOOL::GetNextUnit(T1 * punit)
{
	T1 **pu = NULL;
	
	if(punit == NULL)
		return NULL;
	pu = __super::GetFirstUnit();
	while(pu != NULL)
	{
		if(*pu == punit)
		{
			pu = __super::GetNextUnit(pu);
			if(pu == NULL)
				return NULL;
			return *pu;
		}
		pu = __super::GetNextUnit(pu);
	}
	return NULL;
}

TEMPLATE_PCHAIN
T1* CPCHAINTOOL::GetUnit(int index)
{
	T1 **pu = NULL;

	pu = __super::GetUnit(index);
	if(pu == NULL)
		return NULL;
	return *pu;
}

TEMPLATE_PCHAIN
T1* CPCHAINTOOL::operator[](int index)
{
	return this->GetUnit(index);
}

#endif


