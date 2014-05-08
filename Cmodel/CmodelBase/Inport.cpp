#include "internal.h"


CPortBase::CPortBase(void)
{
	this->m_filename[0] = 0;
	this->m_pin = NULL;
}

CPortBase::~CPortBase(void)
{
}

void CPortBase::SetPin(CPinBase *pin)
{
	this->m_pin = pin;
}

CPinBase *CPortBase::GetPin(void)
{
	return this->m_pin;
}

void CPortBase::SetFilename(char *filename)
{
	strcpy(this->m_filename, filename);
}

char *CPortBase::GetFilename(void)
{
	return this->m_filename;
}

void CPortBase::Close(void)
{
	this->m_fb.Close();
}

//====================================

CInportProp::CInportProp(void) : m_fmtpool(sizeof(int))
{
	this->m_fmtcnt = 1;
	this->m_fmts = (int *)this->m_fmtpool.m_buf;
	this->m_fmts[0] = gDefaultFmt;
	this->m_pos = 0;
	this->m_len = 0;
}

CInportProp::~CInportProp(void)
{
}

void CInportProp::Open(void)
{
	if(this->m_pin == NULL)
		this->m_fb.Open(this->m_filename);
}

int	CInportProp::HasUnknowType(void)
{
	int i;

	for(i = 0; i < this->m_fmtcnt; i++)
	{
		if(this->m_fmts[i] == gDefaultFmt)
		{
			this->m_fmtcnt = 1;
			this->m_fmts[0] = gDefaultFmt;
			return 1;
		}
	}
	return 0;
}

int	CInportProp::IsCtrlPort(void)
{
	int i;

	for(i = 0; i < this->m_fmtcnt; i++)
	{
		if( ((this->m_fmts[i] & CLASSMASK) >> CLASSSHIFT) == CTRLCLASS )
			return 1;
	}
	return 0;
}

void CInportProp::SetPortFormats(int count, int *format)
{
	ASSERT(count > 0);
	this->m_fmtcnt = count;
	this->m_fmtpool.SetSize(count, sizeof(int));
	this->m_fmts = (int *)this->m_fmtpool.m_buf;
	memcpy(this->m_fmtpool.m_buf, format, count * sizeof(int));
	this->HasUnknowType();
	this->SetFormat(this->m_fmts[0]);
}

int	CInportProp::Read(char *buf, int len)
{
	ASSERT(len > 0);
	if(this->m_len == 0)
	{
		if(this->m_pin)
		{
			this->m_len = this->m_pin->GetDataSize();
			this->m_pool.SetSize(this->m_len);
			this->m_len = this->m_pin->Read(this->m_pool.m_buf, this->m_len);
		}
		else
		{
			if( ((this->GetFormat() & CLASSMASK) >> CLASSSHIFT) == CTRLCLASS )
				return 0;
			this->m_len = len;
			this->m_pool.SetSize(this->m_len);
			this->m_len = this->m_fb.Read(this->m_pool.m_buf, this->m_len);
			if(this->m_len == 0)
				return -1;
		}
		this->m_pos = 0;
	}
	len = len > (this->m_len - this->m_pos) ? (this->m_len - this->m_pos) : len;
	memcpy(buf, this->m_pool.m_buf + this->m_pos, len);
	return len;
}

void CInportProp::AddRpoint(int rp)
{
	this->m_pos += rp;
	if(this->m_pos >= this->m_len)
		this->m_len = 0;
}

int CInportProp::GetFmtCount(void)
{
	return this->m_fmtcnt;
}

int *CInportProp::GetFormats(void)
{
	return this->m_fmts;
}

void CInportProp::StoreParm(CKeyTree *ptree)
{
	if(this->GetPin())
		return;
	if((int)strlen(this->GetFilename()) > 0)
		ptree->AddLeaf("InputFile", this->GetFilename());
	ptree->AddLeaf("Streamformat", this->GetFormat());
	ptree->AddLeaf("Unit", this->GetUnit());
	ptree->AddLeaf("Width", this->GetWidth());
	ptree->AddLeaf("Height", this->GetHeight());
}

void CInportProp::LoadParm(CKeyTree *ptree)
{
	CCntLeaf *pl;

	pl = ptree->GetLeaf("InputFile");
	if(pl)
		this->SetFilename(pl->GetKeyStr());
	pl = ptree->GetLeaf("Streamformat");
	if(pl)
		this->SetFormat(pl->GetKeyVal());
	pl = ptree->GetLeaf("Unit");
	if(pl)
		this->SetUnit(pl->GetKeyVal());
	pl = ptree->GetLeaf("Width");
	if(pl)
		this->SetWidth(pl->GetKeyVal());
	pl = ptree->GetLeaf("Height");
	if(pl)
		this->SetHeight(pl->GetKeyVal());
}

//==========================

CInport::CInport(void)
{
}

CInport::~CInport(void)
{
}

void CInport::StoreParm(CKeyTree *ptree)
{
	int i, count;
	CKeyTree *pk;

	count = this->GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pk = ptree->AddFold("Inport", i);
		this->GetUnit(i)->StoreParm(pk);
	}
}

void CInport::LoadParm(CKeyTree *ptree)
{
	int i, count;
	CKeyTree *pk;

	count = this->GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pk = ptree->GetFold("Inport", i);
		if(pk)
			this->GetUnit(i)->LoadParm(pk);
	}
}
