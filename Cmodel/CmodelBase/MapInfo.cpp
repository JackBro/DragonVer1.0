// BlackBox.cpp : 实现文件
//
#include "internal.h"


CMapInfo::CMapInfo(void)
{
}

CMapInfo::~CMapInfo(void)
{
}

int	CMapInfo::CheckUnitLdown(int x, int y)
{
	int i, count = this->GetUnitCount();
	CUnitRect *prect;
	int sel = 0, flag = NOUP_SELECTED;

	for(i = 0; i < count; i++)
	{
		prect = this->GetUnit(i);
		sel = prect->LocatePoint(x, y);
		if(sel == 0)
		{
			if(prect->GetState())
				flag |= MODIFIED_SELECT;
			prect->SetState(0);
		}
		else
		{
			if(prect->GetState() == 0)
				flag |= MODIFIED_SELECT;
			flag |= UNIT_SELECTED;
			prect->SetState(sel);
		}
	}
	return flag;
}

#define PINRANGE	2

int CMapInfo::CheckPinLdown(int x, int y)
{
	int i, count, flag = NOUP_SELECTED;
	int sx, sy, ex, ey, tmp;
	TMap *pmap;

	count = this->m_table.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pmap = this->m_table[i];
		this->GetPinPos(i, &sx, &sy, &ex, &ey);
		if(sx > ex)
		{
			tmp = sx;
			sx = ex;
			ex = tmp;
		}
		if(sy > ey)
		{
			tmp = sy;
			sy = ey;
			ey = tmp;
		}
		if(sx == ex)
		{
			if((x >= (sx-PINRANGE)) && (x <= (sx+PINRANGE)) && (y > sy) && (y < ey) )
			{
				if(pmap->state == 0)
				{
					pmap->state = 1;
					flag |= MODIFIED_SELECT;
				}
				flag = PIN_SELECTED;
				continue;
			}
		}
		else if(sy == ey)
		{
			if((y >= (sy-PINRANGE)) && (y <= (sy+PINRANGE)) && (x > sx) && (x < ex) )
			{
				if(pmap->state == 0)
				{
					pmap->state = 1;
					flag |= MODIFIED_SELECT;
				}
				flag |= PIN_SELECTED;
				continue;
			}
		}
		else
		{
			tmp = (ey - sy) * (x - sx) / (ex - sx) + sy;
			if( (tmp >= (y-PINRANGE)) && (tmp <= (y+PINRANGE)) )
			{
				if(pmap->state == 0)
				{
					pmap->state = 1;
					flag |= MODIFIED_SELECT;
				}
				flag |= PIN_SELECTED;
				continue;
			}
		}
		if(pmap->state == 1)
		{
			pmap->state = 0;
			flag |= MODIFIED_SELECT;
		}
	}
	return flag;
}

int CMapInfo::CheckLdown(int x, int y)
{
	int flag0, flag1;

	flag0 = this->CheckUnitLdown(x, y);
	flag1 = this->CheckPinLdown(x, y);
	this->m_posx = x;
	this->m_posy = y;
	return flag0 | flag1;
}

int CMapInfo::CheckLup(int x, int y, int *start, int *end)
{
	int i, count = this->GetUnitCount();
	CUnitRect *psrc;
	int state;
	
	*start = -1;
	*end = -1;
	for(i = 0; i < count; i++)
	{
		psrc = this->GetUnit(i);
		state = psrc->GetState();
		if(state == INSIDE_BOX_FLAG)
		{
			psrc->Move(x - this->m_posx, y - this->m_posy);
			return 1;
		}
		else if(state != 0)
		{
			*start = i;
			if(*end != -1)
				return 0;
		}
		state = psrc->LocatePoint(x, y);
		if( (state != 0) && (state != INSIDE_BOX_FLAG) )
		{
			*end = i;
			psrc->SetState(state);
			if(*start != -1)
				return 0;
		}
	}
	return 0;
}

CUnitRect * CMapInfo::GetCurrentUnit(void)
{
	int i, count = this->GetUnitCount();
	CUnitRect *ptr;

	for(i = 0; i < count; i++)
	{
		ptr = this->GetUnit(i);
		if(ptr->GetState())
			return ptr;
	}
	return NULL;
}

int CMapInfo::GetPinPos(int index, int *sx, int *sy, int *ex, int *ey)
{
	TMap *pmap;
	CUnitRect *punit;

	pmap = this->m_table[index];
	punit = this->GetUnitRect(pmap->psrc);
	if(punit == NULL)
		return 1;
	punit->GetOupinPos(pmap->portout, sx, sy);
	punit = this->GetUnitRect(pmap->pdst);
	if(punit == NULL)
		return 1;
	punit->GetInpinPos(pmap->portin, ex, ey);
	return 0;
}

CUnitRect * CMapInfo::GetUnitRect(CBaseUnit *punit)
{
	int i, count = this->GetUnitCount();
	CUnitRect *ptr;

	for(i = 0; i < count; i++)
	{
		ptr = this->GetUnit(i);
		if(punit == ptr->GetUnit())
			return ptr;
	}
	return NULL;
}

int	CMapInfo::GetPinCount(void)
{
	return this->m_table.GetUnitCount();
}

TMap *CMapInfo::GetPinMap(int index)
{
	return this->m_table[index];
}

TMap *CMapInfo::GetCurrentPinMap(void)
{
	return this->m_table.GetCurrentPin();
}

int CMapInfo::Connect(int start, int end)
{
	CUnitRect *psrc, *pdst;

	if( (start < 0) || (end < 0) )
		return 0;
	psrc = this->GetUnit(start);
	pdst = this->GetUnit(end);
	this->m_table.Connect(psrc->GetUnit(), pdst->GetUnit(),
		psrc->GetState(), pdst->GetState());
	return 1;
}

void CMapInfo::DeleteUnit(CUnitRect* punit)
{
	if(punit)
	{
		this->m_table.DeleteItem(punit->GetUnit());
		__super::DeleteUnit(punit);
	}
}

void CMapInfo::DelCurrent(void)
{
	CUnitRect *prect = NULL;
	TMap *pmap = NULL;

	prect = this->GetCurrentUnit();
	this->DeleteUnit(prect);
	pmap = this->GetCurrentPinMap();
	if(pmap)
		this->m_table.DeleteUnit(pmap);
}

void CMapInfo::StoreParm(CKeyTree *ptree)
{
	CKeyTree *pk;
	CUnitRect *pr;
	CBaseUnit *punit;
	int i, count;

	count = this->GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pr = this->GetUnit(i);
		punit = pr->GetUnit();
		pk = ptree->AddFold("Unit", punit->GetNum());
		pr->StoreParm(pk);
		punit->StoreParm(pk);
	}
	pk = ptree->AddFold("Map", 0);
	this->m_table.StoreParm(pk);
}

