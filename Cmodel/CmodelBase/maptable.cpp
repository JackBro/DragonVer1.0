#include "internal.h"


//==========================================
//
//		class CMapTable
//
//===================================

CMapTable::CMapTable(void)
{
}

CMapTable::~CMapTable(void)
{
}

void CMapTable::Connect(CBaseUnit *ps, CBaseUnit *pe, int spin, int epin)
{
	TMap     *map;

	if( (ps == pe) || (ps == NULL) || (pe == NULL) ||
		(spin & PINPOS_MASK) == (epin & PINPOS_MASK) )
		return;
//		return NULL;
	map = this->Add();
	if(spin & INPUT_PIN_FLAG)
	{
		map->psrc = pe;
		map->pdst = ps;
		map->portout = epin & PIN_MASK;
		map->portin = spin & PIN_MASK;
	}
	else
	{
		map->psrc = ps;
		map->pdst = pe;
		map->portout = spin & PIN_MASK;
		map->portin = epin & PIN_MASK;
	}
	map->psrc->SetOutputPin(&(map->pin), map->portout);
	map->pdst->SetInputPin(&(map->pin), map->portin);
	map->state = 0;
//	return &(map->pin);
}

TMap* CMapTable::FindMap(CPinBase* pin)
{
	TMap *map;

	if(pin == NULL)
		return NULL;
	map = this->GetFirstUnit();	
	while(map != NULL)
	{
		if(&(map->pin) == pin)
			return map;
		map = this->GetNextUnit(map);
	}

	return NULL;
}

void CMapTable::DeletePin(CPinBase *pin)
{
	TMap *map;

	map = this->FindMap(pin);
	this->DeleteUnit(map);
}

void CMapTable::DeleteUnit(TMap *pmap)
{
	if(pmap)
	{
		pmap->psrc->SetOutputPin(NULL, pmap->portout);
		pmap->pdst->SetInputPin(NULL, pmap->portin);
		__super::DeleteUnit(pmap);
	}
}

void CMapTable::DeleteItem(CBaseUnit * punit)
{
	int		inpcnt = punit->GetInPinCount();
	int		oupcnt = punit->GetOuPinCount();
	TMap*	map;
	
	for(int i = 0; i < inpcnt; i++)
	{
		map = this->FindMap(punit->GetInputPin(i));
		this->DeleteUnit(map);
	}
	for(i = 0; i < oupcnt; i++)
	{
		map = this->FindMap(punit->GetOutputPin(i));
		this->DeleteUnit(map);
	}
}

TMap *CMapTable::GetCurrentPin(void)
{
	TMap *map;

	map = this->GetFirstUnit();	
	while(map != NULL)
	{
		if(map->state)
			return map;
		map = this->GetNextUnit(map);
	}

	return NULL;
}

void CMapTable::StoreParm(CKeyTree *pb)
{
	TMap *pmap;
	int i, count;

	count = this->GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pmap = this->GetUnit(i);
		pb->AddLeaf(pmap->psrc->GetNum(), pmap->portout, 
			pmap->pdst->GetNum(), pmap->portin);
	}
}

void CMapTable::LoadParm(CMapBase *pmap, CKeyTree *pb)
{
	int i, count;
	int src, oupin, dst, inpin;
	CCntLeaf *pl;

	count = pb->GetLeafCount();
	for(i = 0; i < count; i++)
	{
		pl = pb->GetLeaf(i);
		pl->GetConnect(&src, &oupin, &dst, &inpin);
		oupin |= OUTPUT_PIN_FLAG;
		inpin |= INPUT_PIN_FLAG;
		this->Connect(pmap->GetUnit(src), pmap->GetUnit(dst), oupin, inpin);
	}
}
