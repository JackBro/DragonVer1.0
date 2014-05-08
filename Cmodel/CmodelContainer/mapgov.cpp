#include "internal.h"


CMapGov::CMapGov(void)
{
}

CMapGov::~CMapGov(void)
{
}

void CMapGov::SetNum(void)
{
	int i, count;

	count = this->m_map.GetUnitCount();
	for(i = 0; i < count; i++)
		this->m_map[i]->SetNum(i);
}

CUnitRect * CMapGov::Add(int sel)
{
	CBaseUnit *punit = NULL;
	CUnitRect *pos = NULL;

	punit = this->m_units.Add(sel);
	if (punit) 
	{
		this->m_map.Add(punit);
		pos = CMapInfo::Add();
		pos->SetUnit(punit);
		this->SetNum();
	}
	return pos;
}

void CMapGov::DelCurrent(void)
{
	CUnitRect *prect = NULL;
	CBaseUnit *punit = NULL;

	prect = this->GetCurrentUnit();
	if(prect)
		punit = prect->GetUnit();
	CMapInfo::DelCurrent();
	if(punit)
	{
		this->m_map.DeleteUnit(punit);
		this->m_units.Del(punit);
	}
	this->SetNum();
}

void CMapGov::Process(void)
{
	this->m_map.Process();
}

int	CMapGov::PreProcess(void)
{
	return this->m_map.PreProcess();
}

void CMapGov::LoadParm(CKeyTree *ptree)
{
	CKeyTree *pk;
	CMapBase tmap;
	CUnitRect *punit;
	CBaseUnit *pu;
	CCntLeaf *pl;
	int i = 0, count;

	count = ptree->GetFoldCount();
	for(i = 0; i < count; i++)
	{
		pk = ptree->GetFold("Unit", i);
		if(pk == NULL)
			break;
		pl = pk->GetLeaf("UnitID");
		if(pl == NULL)
			break;
		punit = this->Add(pl->GetKeyVal());
		if(punit == NULL)
			continue;
		pu = punit->GetUnit();
		punit->LoadParm(pk);
		pu->LoadParm(pk);
		tmap.Add(pu);
	}
	this->m_table.LoadParm(&tmap, ptree->GetFold("Map", 0));
}

