
#include "internal.h"

static const int g_LensFormats[] =
{
	RAW_BG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GB | (VIDEOCLASS << CLASSSHIFT),
	RAW_RG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GR | (VIDEOCLASS << CLASSSHIFT)
};

static const char *gLansFallOffAlgType[] = { "V568LensOff" };
static const int  gTotalLansFallOffAlgType = sizeof(gLansFallOffAlgType) / sizeof(char *);


CLensFallOff::CLensFallOff(void) : CImageUnit(1, 1, "Lens falloff", 10)
{
	this->SetAlgStr(gTotalLansFallOffAlgType, (char **)gLansFallOffAlgType);
	this->m_pAlg = &this->m_568LensFallOffAlg;
	this->SetInportFormat(0, sizeof(g_LensFormats) / sizeof(int), (int *)g_LensFormats);

	this->m_568lensCx = m_568LensFallOffAlg.GetLensCx();
	this->m_568lensCy = m_568LensFallOffAlg.GetLensCy();
	this->m_568lensFocus = m_568LensFallOffAlg.GetLensFocus();
}


CLensFallOff::~CLensFallOff(void)
{
}

int	CLensFallOff::GetID(void)
{
	return LENSFALLOFF_BOX | (ISPCLASS << CLASSSHIFT);
}
int CLensFallOff::Get568LensCx(void) const
{
	return this->m_568lensCx;
}

int CLensFallOff::Get568LensCy(void) const
{
	return this->m_568lensCy;
}
int CLensFallOff::Get568LensFocus(void) const
{
	return this->m_568lensFocus;
}

void CLensFallOff::Set568LensCx(int lensCx)
{
	this->m_568lensCx = lensCx;
}

void CLensFallOff::Set568LensCy(int lensCy)
{
	this->m_568lensCy = lensCy;
}


void CLensFallOff::Set568LensFocus(int lensFocus)
{
	this->m_568lensFocus = lensFocus;
}


void CLensFallOff::UpdateConfig(void)
{
	switch(this->GetCurAlg()) 
	{
	case 0:
		this->m_568LensFallOffAlg.SetLensX(this->m_568lensCx);
		this->m_568LensFallOffAlg.SetLensY(this->m_568lensCy);
		this->m_568LensFallOffAlg.SetLensFocus(this->m_568lensFocus);
		break;
	default:
		this->m_568LensFallOffAlg.SetLensX(this->m_568lensCx);
		this->m_568LensFallOffAlg.SetLensY(this->m_568lensCy);
		this->m_568LensFallOffAlg.SetLensFocus(this->m_568lensFocus);
	}
}

void CLensFallOff::ChoiceAlg(int sel)
{
	switch(sel) 
	{
	case 0:
		this->m_pAlg = &this->m_568LensFallOffAlg;
		break;
	default:
		this->m_pAlg = &this->m_568LensFallOffAlg;
		break;
	}
	CBaseUnit::ChoiceAlg(sel);
}

