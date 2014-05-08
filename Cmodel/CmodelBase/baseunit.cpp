#include "internal.h"


CBaseUnit::CBaseUnit(int inp, int oup, char *desc, int size)
{
	this->SetInPinCount(inp);
	this->SetOuPinCount(oup);
	this->SetDesc(desc);
	this->m_AlgCount = 1;
	this->m_pAlgstr = (char **)gDefaultAlgType;
	this->m_number = 0;
	this->m_algsel = 0;
	this->m_pAlg = NULL;
	this->m_pool.SetSize(size);
}

CBaseUnit::~CBaseUnit()
{
}

void CBaseUnit::SetInPinCount(int inpcnt)
{
	this->m_inpchain.SetUnitCount(inpcnt);
}

int CBaseUnit::GetInPinCount()
{
	return this->m_inpchain.GetUnitCount();
}

void CBaseUnit::SetOuPinCount(int oupcnt)
{
	this->m_oupchain.SetUnitCount(oupcnt);
}

int CBaseUnit::GetOuPinCount()
{
	return this->m_oupchain.GetUnitCount();
}

void CBaseUnit::SetAlgStr(int count, char **str)
{
	this->m_AlgCount = count;
	this->m_pAlgstr = str;
}

void CBaseUnit::SetInportFormat(int index, int count, int* format)
{
	ASSERT((index >= 0) && (index < this->GetInPinCount()));
	this->m_inpchain[index]->SetPortFormats(count, format);
}

void CBaseUnit::SetDesc(char *str)
{
	if(str)
		strcpy(this->m_desc, str);
	else
		strcpy(this->m_desc, DEFAULT_BLOCK_DESC);
}

char * CBaseUnit::GetDesc(void)
{
	return this->m_desc;
}

void CBaseUnit::SetNum(int num)
{
	this->m_number = num;
}

int CBaseUnit::GetNum()
{
	return this->m_number;
}

void CBaseUnit::SetInputPin(CPinBase *pin, int index)
{
	ASSERT((index >= 0) && (index < this->GetInPinCount()));
	this->m_inpchain[index]->SetPin(pin);
}

CPinBase * CBaseUnit::GetInputPin(int index)
{
	ASSERT((index >= 0) && (index < this->GetInPinCount()));
	return this->m_inpchain[index]->GetPin();
}

void CBaseUnit::SetOutputPin(CPinBase *pin, int index)
{
	ASSERT((index >= 0) && (index < this->GetOuPinCount()));
	this->m_oupchain[index]->SetPin(pin);
}

CPinBase * CBaseUnit::GetOutputPin(int index)
{
	ASSERT((index >= 0) && (index < this->GetOuPinCount()));
	return this->m_oupchain[index]->GetPin();
}

//===================================


void CBaseUnit::PostProcess(void)
{
	int i;

	for(i = 0; i < this->m_inpchain.GetUnitCount(); i++)
		this->m_inpchain[i]->Close();
	for(i = 0; i < this->m_oupchain.GetUnitCount(); i++)
		this->m_oupchain[i]->Close();
}

int CBaseUnit::CheckInpins(void)
{
	int i, format, count, unit = 0, fmtcnt, *fmts;
	CInportProp *port;
	CPinBase *pin;

	count = this->m_inpchain.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		port = this->m_inpchain[i];
		pin = port->GetPin();
		if(pin == NULL)
			continue;
		format = pin->GetFormat();
		if(format == gDefaultFmt)
			return UNIT_IDLE;

		fmtcnt = port->GetFmtCount();
		fmts = port->GetFormats();
		for(i = 0; i < fmtcnt; i++)
		{
			if( (fmts[i] == gDefaultFmt) || (fmts[i] == format) )
			{
				i = fmtcnt + 10;
				break;
			}
		}
		if(i != fmtcnt + 10)
			return UNIT_FAILED;
		if(unit == 0)
			unit = pin->GetUnit();
		else if( (unit < 0) || (unit != pin->GetUnit()) )
			return UNIT_FAILED;
		unit = pin->GetUnit();
	}
	return UNIT_DONE;
}

void CBaseUnit::SetInputCfg(void)
{
	int i, count;
	CInportProp *port;
	CPinBase *pin;
	TStreamCfg *pcfg;

	ASSERT(this->m_pAlg);
	count = this->m_inpchain.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		port = this->m_inpchain[i];
		pin = port->GetPin();
		if(pin == NULL)
			pcfg = port->GetCfg();
		else
		{
			pcfg = pin->GetCfg();
			port->SetCfg(pcfg);
		}
		this->m_pAlg->SetInputCfg(i, pcfg);
	}
}

int CBaseUnit::SetOutputCfg(void)
{
	int i, count;
	COuportProp *port;
	CPinBase *pin;
	TStreamCfg cfg;

	ASSERT(this->m_pAlg);
	count = this->m_oupchain.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		port = this->m_oupchain[i];
		pin = port->GetPin();
		if(this->m_pAlg->GetOutputCfg(i, &cfg))
			return 1;
		port->SetCfg(&cfg);
		if(pin)
			pin->SetCfg(&cfg);
	}
	return 0;
}

int CBaseUnit::CheckConnect(void)
{
	int state;

	state = this->CheckInpins();
	if(state != UNIT_DONE)
		return state;
	this->SetInputCfg();
	this->UpdateConfig();
	if(this->SetOutputCfg())
		return UNIT_FAILED;
	return UNIT_DONE;
}

void CBaseUnit::Open(void)
{
	int i, count;

	ASSERT(this->m_pAlg);
	count = this->m_oupchain.GetUnitCount();
	for(i = 0; i < count; i++)
		this->m_oupchain[i]->Open();
	count = this->m_inpchain.GetUnitCount();
	for(i = 0; i < count; i++)
		this->m_inpchain[i]->Open();
	this->m_pAlg->ResetState();
}

//==========================

void CBaseUnit::CtrlInPinProcess(int index, char *buf, int len)
{
	index = len;
	buf = NULL;
}

int CBaseUnit::CheckInpinStatus(void)
{
	int i, count, size, flag = 0;
	CInportProp *port;
	CPinBase *pin;

	count = this->m_inpchain.GetUnitCount();
	for(i = 0; i < count; i++)	//check inpurt pin status
	{
		port = this->m_inpchain[i];
		pin = port->GetPin();
		if(((port->GetFormat() & CLASSMASK) >> CLASSSHIFT) == CTRLCLASS)
		{
			if(pin)
			{
				if(pin->IsFifoEmpty())
					continue;
				size = pin->GetDataSize();
				this->m_pool.SetSize(size);
				size = port->Read(this->m_pool.m_buf, size);
				this->CtrlInPinProcess(i, this->m_pool.m_buf, size);
			}
		}
		else
		{
			if(pin)
			{
				if(pin->IsFifoEmpty())
					flag = 1;
			}
		}
	}
	return flag;
}

int CBaseUnit::CheckPinStatus(void)
{
	int i, count;
	CPinBase *pin;

	if(this->CheckInpinStatus())
		return 1;
	count = this->m_oupchain.GetUnitCount();
	for(i = 0; i < count; i++)		//check output pin status
	{
		pin = this->m_oupchain[i]->GetPin();
		if(pin)
		{
			if(pin->IsFifoEmpty() == 0)
				return 1;
		}
	}

	return 0;
}

int CBaseUnit::WriteAlg(void)
{
	int i, len, len1, count, size, flag = UNIT_IDLE;
	CInportProp *port;

	ASSERT(this->m_pAlg);
	count = this->m_inpchain.GetUnitCount();
	size = this->m_pool.GetTotalSize();
	for(i = 0; i < count; i++)
	{
		port = this->m_inpchain[i];
		do {
			len = port->Read(this->m_pool.m_buf, size);
			if( (len == -1) && (flag == UNIT_IDLE) )
			{
				flag = UNIT_FAILED;
				len = 0;
			}
			len1 = this->m_pAlg->Write(i, this->m_pool.m_buf, len);
			if(len1)
				flag = UNIT_DONE;
			port->AddRpoint(len1);
			if(len1 < len)
				len1 = 0;
		} while(len1);
	}

	return flag;
}

int CBaseUnit::ReadAlg(void)
{
	int i, len, count, size;
	int state = 1;

	ASSERT(this->m_pAlg);
	count = this->m_oupchain.GetUnitCount();
	size = this->m_pool.GetTotalSize();
	for(i = 0; i < count; i++)
	{
		len = this->m_pAlg->Read(i, this->m_pool.m_buf, size);
		if(len)
			state = 0;
		this->m_oupchain[i]->Write(this->m_pool.m_buf, len);
	}
	return state;
}

int	CBaseUnit::DummyProcess(void)
{
	return UNIT_IDLE;
}

int CBaseUnit::Process(void)
{
	int wstate, rstate;

	if(this->CheckPinStatus() == 1)
		return UNIT_IDLE;
	wstate = this->DummyProcess();
	if(wstate != UNIT_IDLE)
		return wstate;
	wstate = this->WriteAlg();
	rstate = this->ReadAlg();
	if( (wstate == UNIT_DONE) || (rstate == 0) )
		return UNIT_DONE;
	if(wstate == UNIT_FAILED)
		return UNIT_FAILED;
	return UNIT_IDLE;
}

CInportProp *CBaseUnit::GetInport(int index)
{
	ASSERT((index >= 0) && (index < this->GetInPinCount()));
	return this->m_inpchain[index];
}

COuportProp *CBaseUnit::GetOuport(int index)
{
	ASSERT((index >= 0) && (index < this->GetOuPinCount()));
	return this->m_oupchain[index];
}

int	CBaseUnit::GetTotalAlg(void)
{
	return this->m_AlgCount;
}

char *CBaseUnit::GetAlgDesc(int index)
{
	ASSERT((index >= 0) && (index < this->m_AlgCount));
	return this->m_pAlgstr[index];
}

void CBaseUnit::ChoiceAlg(int sel)
{
	ASSERT(this->m_pAlg);
	this->m_algsel = sel;
	this->m_pAlg->ResetState();
}

int	CBaseUnit::GetCurAlg(void)
{
	return this->m_algsel;
}

void CBaseUnit::UpdateInputCfg(void)
{
	int i, count;
	TStreamCfg *pcfg;

	ASSERT(this->m_pAlg);
	count = this->m_inpchain.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		pcfg = this->m_inpchain[0]->GetCfg();
		this->m_pAlg->SetInputCfg(0, pcfg);
	}
}

int CBaseUnit::UpdateOutputCfg(void)
{
	int i, count;
	TStreamCfg cfg;

	count = this->m_oupchain.GetUnitCount();
	for(i = 0; i < count; i++)
	{
		if(this->m_pAlg->GetOutputCfg(i, &cfg))
			return 1;
		this->m_oupchain[i]->SetCfg(&cfg);
	}
	return 0;
}

void CBaseUnit::UpdateConfig(void)
{
}

void CBaseUnit::StoreParm(CKeyTree *ptree)
{
	this->m_inpchain.StoreParm(ptree);
	this->m_oupchain.StoreParm(ptree);
	ptree->AddLeaf("Description", this->GetDesc());
	ptree->AddLeaf("UnitID", this->GetID());
	ptree->AddLeaf("Algorithm", this->GetCurAlg());
}

void CBaseUnit::LoadParm(CKeyTree *ptree)
{
	CCntLeaf *pl;

	this->m_inpchain.LoadParm(ptree);
	this->m_oupchain.LoadParm(ptree);
	pl = ptree->GetLeaf("Algorithm");
	if(pl)
		this->ChoiceAlg(pl->GetKeyVal());
	pl = ptree->GetLeaf("Description");
	if(pl)
		this->SetDesc(pl->GetKeyStr());
}



