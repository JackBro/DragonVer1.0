
#include "internal.h"



static const int g_DpdDpcFormats[] =
{
	RAW_BG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GB | (VIDEOCLASS << CLASSSHIFT),
	RAW_RG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GR | (VIDEOCLASS << CLASSSHIFT)
};

static const char *gDpdDpcAlgType[] = { "V326DpdDpc", "V568DpdDpc" };
static const int  gTotalDpdDpcAlgType = sizeof(gDpdDpcAlgType) / sizeof(char *);


CDpdDpc::CDpdDpc(void) : CImageUnit(1, 1, "DPD/DPC", 10)
{
	this->SetAlgStr(gTotalDpdDpcAlgType , (char **)gDpdDpcAlgType);
	this->m_pAlg = &this->m_326DpdDpcAlg;
	this->SetInportFormat(0, sizeof(g_DpdDpcFormats) / sizeof(int), (int *)g_DpdDpcFormats);

	this->m_dpdthd = m_326DpdDpcAlg.GetDpdThd();
	memcpy(this->m_noisetable, m_326DpdDpcAlg.GetNoiseTable(), sizeof(int)*gTotalNoiseTable);

}

CDpdDpc::~CDpdDpc(void)
{
}

int	CDpdDpc::GetID(void)
{
	return DPDDPC_BOX | (ISPCLASS << CLASSSHIFT);;
}

void CDpdDpc::UpdateConfig(void)
{
	switch(this->GetCurAlg()) 
	{
	case 0:
		this->m_326DpdDpcAlg.SetDpdThd(this->m_dpdthd);
		this->m_326DpdDpcAlg.SetNoiseTable(this->m_noisetable);
		break;
	case 1:
		this->m_568DpdDpcAlg.SetDpdThd(this->m_dpdthd);
		this->m_568DpdDpcAlg.SetNoiseTable(this->m_noisetable);
		break;
	default:

		this->m_326DpdDpcAlg.SetDpdThd(this->m_dpdthd);
		this->m_326DpdDpcAlg.SetNoiseTable(this->m_noisetable);
	}
}


void CDpdDpc::ChoiceAlg(int sel)
{
	switch(sel) 
	{
	case 0:
		this->m_pAlg = &this->m_326DpdDpcAlg;
		break;
	case 1:
		this->m_pAlg = &this->m_568DpdDpcAlg;
		break;
	default:
		this->m_pAlg = &this->m_326DpdDpcAlg;
		break;
	}
	CBaseUnit::ChoiceAlg(sel);

}



void CDpdDpc::SetDpdThd(int thd)
{
	this->m_dpdthd = thd;
}

int CDpdDpc::GetDpdThd() const
{
	return this->m_dpdthd;
}

void CDpdDpc::SetNoiseTable(int *val)
{
	int i;
	for(i = 0; i < gTotalNoiseTable; i++)
	{
		this->m_noisetable[i] = val[i];
	}
}


int *CDpdDpc::GetNoiseTable()
{
	return &this->m_noisetable[0];
}

