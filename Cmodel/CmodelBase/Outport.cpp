#include "internal.h"


COuportProp::COuportProp(void)
{
	this->m_bsave = 0;
}

COuportProp::~COuportProp(void)
{
}

void COuportProp::Open(void)
{
	if(this->m_bsave) 
		this->m_fb.Open(this->m_filename, "wb");
	if(this->m_pin)
		this->m_pin->ClearFlag();
}

void COuportProp::EnableSave(int sel)
{
	this->m_bsave = sel ? 1 : 0;
}

int	COuportProp::IsSaveFlag(void)
{
	return this->m_bsave;
}

int	COuportProp::Write(char *buf, int len)
{
	//if(len == 0)
	//{
	//	if(((this->GetFormat() & CLASSMASK) >> CLASSSHIFT) == CTRLCLASS)
	//		return 0;
	//}
	if(this->m_pin)
	{
		this->m_pin->SetSize(len);
		this->m_pin->Write(buf, len);
	}
	if( (this->m_bsave) && this->m_fb.IsOpened())
		this->m_fb.Write(buf, len);
	return len;
}

void COuportProp::StoreParm(CKeyTree *ptree)
{
	if((int)strlen(this->GetFilename()) > 0)
	{
		ptree->AddLeaf("OutputFile", this->GetFilename());
		ptree->AddLeaf("Save", this->IsSaveFlag());
	}
}

void COuportProp::LoadParm(CKeyTree *ptree)
{
	CCntLeaf *pl;

	pl = ptree->GetLeaf("OutputFile");
	if(pl)
		this->SetFilename(pl->GetKeyStr());
	pl = ptree->GetLeaf("Save");
	if(pl)
		this->EnableSave(pl->GetKeyVal());
}

//==========================

COutPort::COutPort(void)
{
}

COutPort::~COutPort(void)
{
}

void COutPort::StoreParm(CKeyTree *ptree)
{
	int i, count;
	CKeyTree *pk;

	count = this->GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pk = ptree->AddFold("Outport", i);
		this->GetUnit(i)->StoreParm(pk);
	}
}

void COutPort::LoadParm(CKeyTree *ptree)
{
	int i, count;
	CKeyTree *pk;

	count = this->GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pk = ptree->GetFold("Outport", i);
		if(pk)
			this->GetUnit(i)->LoadParm(pk);
	}
}
