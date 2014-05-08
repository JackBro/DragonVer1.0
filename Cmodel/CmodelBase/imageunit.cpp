#include "internal.h"



CImageUnit::CImageUnit(int inp, int oup, char *desc, int dummyline, int size) : 
	CBaseUnit(inp, oup, desc, size)
{
	this->SetDummyline(dummyline);
}

CImageUnit::~CImageUnit(void)
{
}

void CImageUnit::SetDummyline(int dummyline)
{
	this->m_dummyline = dummyline;
	this->m_dummyline = 0;
	this->m_dummcount = 0;
}

int	CImageUnit::GetDummyline(void)
{
	return this->m_dummyline;
}

int	CImageUnit::DummyProcess(void)
{
	int i, flag = 0, count;
	CInportProp *port;
	CImageAlgBase *palg;

	ASSERT(this->m_pAlg);
	palg = (CImageAlgBase *)(this->m_pAlg);
	count = this->m_inpchain.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		port = this->m_inpchain[i];
		if(port->GetPin() != NULL)
			flag = 1;
	}
	if( palg->IsFrameDone() )
	{
		if((flag == 0) && this->m_dummcount)
		{
			this->m_dummcount--;
			count = this->m_oupchain.GetUnitCount();
			for(i = 0; i < count; i++)
				this->m_oupchain[i]->Write((char *)&i, 0);
			return UNIT_DONE;
		}
		palg->ResetState();
		palg->m_outline = 0;
		this->UpdateInputCfg();
		this->UpdateConfig();
		if(this->UpdateOutputCfg())
			return UNIT_FAILED;
		this->m_dummcount = this->m_dummyline;
	}
	return UNIT_IDLE;
}

int	CImageUnit::CheckInpins(void)
{
	int i, state, count, width = 0, height = 0;
	int format;
	CInportProp *port;
	CPinPortBase *ptr;
	CTypeProp	tp;

	state = CBaseUnit::CheckInpins();
	if(state != UNIT_DONE)
		return state;

	count = this->m_inpchain.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		port = this->m_inpchain[i];
		ptr = port->GetPin();
		if(ptr == NULL)
			ptr = port;
		format = ptr->GetFormat();
		if( (format >> CLASSSHIFT) == VIDEOCLASS )
		{
			width = ptr->GetWidth();
			height = ptr->GetHeight();
			i++;
			state = tp.getLinewidth(width, format, ptr->GetUnit());
			this->m_pool.SetSize(state);
			break;
		}
	}
	if( (width % 4) || (height % 2) )
	{
		this->m_dbg.PrintMsg("Image width 4X, height 2X\n");
		return UNIT_FAILED;
	}
	for(; i < count; i++)
	{
		port = this->m_inpchain[i];
		ptr = port->GetPin();
		if(ptr == NULL)
			ptr = port;
		format = ptr->GetFormat();
		if( (format >> CLASSSHIFT) == VIDEOCLASS )
		{
			if( (width != ptr->GetWidth()) || (height != ptr->GetHeight()) )
				return UNIT_FAILED;
			state = tp.getLinewidth(width, format, ptr->GetUnit());
			this->m_pool.SetSize(state);
		}
	}
	return UNIT_DONE;
}

void CImageUnit::StoreParm(CKeyTree *ptree)
{
	CBaseUnit::StoreParm(ptree);
	ptree->AddLeaf("Dummyline", this->GetDummyline());
}

void CImageUnit::LoadParm(CKeyTree *ptree)
{
	CCntLeaf *pl;

	CBaseUnit::LoadParm(ptree);
	pl = ptree->GetLeaf("Algorithm");
	if(pl)
		this->SetDummyline(pl->GetKeyVal());
}





