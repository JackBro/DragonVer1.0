// BlackBox.cpp : 实现文件
//
#include "internal.h"


CUnitRect::CUnitRect(void)
{
	this->m_body.x = 100;
	this->m_body.y = 100;
	this->m_body.width = DEFAULT_BOX_WIDTH;
	this->m_body.height = DEFAULT_BOX_HEIGHT;
	this->m_punit = NULL;
	this->m_state = 0;
}

CUnitRect::~CUnitRect(void)
{
}

void CUnitRect::SetUnit(CBaseUnit *punit)
{
	this->m_punit = punit;
	this->UpdateInfo();
}

CBaseUnit * CUnitRect::GetUnit(void)
{
	return this->m_punit;
}

void CUnitRect::SetStartPoint(int x, int y)
{
	this->m_body.x = x;
	this->m_body.y = y;
	this->update();
}

void CUnitRect::Move(int x, int y)
{
	this->m_body.x += x;
	this->m_body.y += y;
	this->update();
}

void CUnitRect::update(void)
{
	int i, count;
	TUnitRect *ptr;

	count = this->m_inpin.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		ptr = this->m_inpin[i];
		ptr->y = this->m_body.y + DEFAULT_PORT_STEP + PORT_GAP * i;
		ptr->x = this->m_body.x - (DEFAULT_PORT_SIZE >> 1);
		ptr->width = DEFAULT_PORT_SIZE;
		ptr->height = DEFAULT_PORT_SIZE;
	}

	count = this->m_oupin.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		ptr = this->m_oupin[i];
		ptr->y = this->m_body.y + DEFAULT_PORT_STEP + PORT_GAP * i;
		ptr->x = this->m_body.x + this->m_body.width - (DEFAULT_PORT_SIZE >> 1);
		ptr->width = DEFAULT_PORT_SIZE;
		ptr->height = DEFAULT_PORT_SIZE;
	}
}

void CUnitRect::UpdateInfo(void)
{
	int inp, oup;

	if(this->m_punit == NULL)
		return;
	inp = this->m_punit->GetInPinCount();
	oup = this->m_punit->GetOuPinCount();
	this->m_inpin.SetUnitCount(inp);
	this->m_oupin.SetUnitCount(oup);
	this->update();
	inp = (inp < oup) ? oup : inp;
	this->m_body.height = DEFAULT_PORT_STEP + PORT_GAP * inp;
}

int CUnitRect::LocatePoint(int x, int y)
{
	int i, count;

	count = this->m_inpin.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		if(this->IsInside(x, y, this->m_inpin[i]))
			return i | INPUT_PIN_FLAG;
	}
	count = this->m_oupin.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		if(this->IsInside(x, y, this->m_oupin[i]))
			return i | OUTPUT_PIN_FLAG;
	}
	if(this->IsInside(x, y, &this->m_body))
		return INSIDE_BOX_FLAG;
	return 0;
}

int CUnitRect::IsInside(int x, int y, TUnitRect *rect)
{
	if( (rect->x <= x) && ((rect->x + rect->width) >= x) &&
		(rect->y <= y) && ((rect->y + rect->height) >= y) )
		return 1;
	return 0;
}

void CUnitRect::SetState(int set)
{
	this->m_state = set;
}

int CUnitRect::GetState(void)
{
	return this->m_state;
}

int CUnitRect::GetInpinPos(int index, int *px, int *py)
{
	int count;

	count = this->m_inpin.GetUnitCount();
	if(index >= count)
		return 1;
	*px = this->m_body.x;
	*py = this->m_body.y + PORT_GAP * (index+1) - (DEFAULT_PORT_SIZE >> 1);
	return 0;
}

int CUnitRect::GetOupinPos(int index, int *px, int *py)
{
	int count;

	count = this->m_oupin.GetUnitCount();
	if(index >= count)
		return 1;
	*px = this->m_body.x + this->m_body.width;
	*py = this->m_body.y + PORT_GAP * (index+1) - (DEFAULT_PORT_SIZE >> 1);
	return 0;
}

void CUnitRect::StoreParm(CKeyTree *ptree)
{
	ptree->AddLeaf("Xpos", this->m_body.x);
	ptree->AddLeaf("Ypos", this->m_body.y);
}

void CUnitRect::LoadParm(CKeyTree *ptree)
{
	CCntLeaf *pl;
	int x = 100, y = 100;

	pl = ptree->GetLeaf("Xpos");
	if(pl)
		x = pl->GetKeyVal();
	pl = ptree->GetLeaf("Ypos");
	if(pl)
		y = pl->GetKeyVal();
	this->SetStartPoint(x, y);
}
