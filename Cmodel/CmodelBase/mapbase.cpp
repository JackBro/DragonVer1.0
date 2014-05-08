#include "internal.h"


//==========================================
//
//		class CMapBase 
//
//===================================
CMapBase::CMapBase()
{
}

CMapBase::~CMapBase()
{
}

int CMapBase::PreProcess(void)
{
	int state, count, flag = 1;
	CBaseUnit *punit = NULL;

	count = this->GetUnitCount();
	count *= count+1;
	count >>= 1;
	while(flag && count)
	{
		flag = 0;
		punit = this->GetFirstUnit();
		while(punit != NULL)
		{
			state = punit->CheckConnect();
			if( (state == UNIT_FAILED) || (state == UNIT_IDLE) )
				flag = 1;
			punit = this->GetNextUnit(punit);
		}
		count--;
	}

	return flag;
}

void CMapBase::PostProcess(void)
{
	CBaseUnit *punit = NULL;

	punit = this->GetFirstUnit();
	while(punit != NULL)
	{
		punit->PostProcess();
		punit = this->GetNextUnit(punit);
	}
}

void CMapBase::Open(void)
{
	CBaseUnit *punit = NULL;

	punit = this->GetFirstUnit();
	while(punit != NULL)
	{
		punit->Open();
		punit = this->GetNextUnit(punit);
	}
}

void CMapBase::Process(void)
{
	int ret, flag = 1, ret1 = 1;
	CBaseUnit *punit = NULL;

	if(this->PreProcess())
	{
		this->m_dbg.PrintMsg("Unknow stream in map\n");
		return;
	}
	this->Open();
	while(flag == 1)
	{
		flag = 0;
		ret1 = 0;
		punit = this->GetFirstUnit();
		while(punit != NULL)
		{
			ret = punit->Process();
			if(ret == UNIT_DONE)
				flag = 1;
			if(ret == UNIT_FAILED)
				ret1 = 1;
			punit = this->GetNextUnit(punit);
		}
	}
	this->PostProcess();
	if(ret1 == 0)
	{
		this->m_dbg.PrintMsg("Map is blocked!\n");
	}
}

