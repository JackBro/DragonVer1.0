#include "internal.h"


static const int g_DigitalFormats[] =
{
	RAW_BG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GB | (VIDEOCLASS << CLASSSHIFT),
	RAW_RG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GR | (VIDEOCLASS << CLASSSHIFT)
};

static const char *gAlgGainType[] = { "568DigitalGain" };
static const int  gTotalAlgGainType = sizeof(gAlgGainType) / sizeof(char *);


CDigitalGain::CDigitalGain() : CImageUnit(1, 1, "DigitalGain", 10)
{
	this->SetAlgStr(gTotalAlgGainType, (char **)gAlgGainType);
	this->m_pAlg = &this->m_568DigitalGainAlg;

	this->SetInportFormat(0, sizeof(g_DigitalFormats) / sizeof(int), (int *)g_DigitalFormats);

	this->m_globalEn = m_568DigitalGainAlg.GetGlobalEn();
	this->m_rgbEn = m_568DigitalGainAlg.GetRgbEn();
	this->m_globalGain = m_568DigitalGainAlg.GetGlobalGain();
	this->m_gainStep = m_568DigitalGainAlg.GetGainStep();
	this->m_rGain = m_568DigitalGainAlg.GetRGain();
	this->m_gGain = m_568DigitalGainAlg.GetGGain();
	this->m_bGain = m_568DigitalGainAlg.GetBGain();
}

CDigitalGain::~CDigitalGain(void)
{
}

int	CDigitalGain::GetID(void)
{
	return DIGITALGAIN_BOX | (ISPCLASS << CLASSSHIFT);;
}

void CDigitalGain::UpdateConfig(void)
{
	this->m_568DigitalGainAlg.SetGlobalEn(m_globalEn&0x1);
	if (this->m_globalEn&0x1) 
	{
		this->m_568DigitalGainAlg.SetGlobalGain(m_globalGain);
	}
	this->m_568DigitalGainAlg.SetRgbEn(m_rgbEn&0x1);
	if (this->m_rgbEn&0x1) 
	{
		this->m_568DigitalGainAlg.SetRGain(m_rGain);
		this->m_568DigitalGainAlg.SetGGain(m_gGain);
		this->m_568DigitalGainAlg.SetBGain(m_bGain);
	}
}

void CDigitalGain::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_568DigitalGainAlg;
	CBaseUnit::ChoiceAlg(sel);
}



void CDigitalGain::SetRGain(int gain)
{
	this->m_rGain = gain&0xff;
}

int CDigitalGain::GetRGain(void)
{
	return this->m_rGain;
}

void CDigitalGain::SetBGain(int gain)
{
	this->m_bGain = gain&0xff;
}

int CDigitalGain::GetBGain(void)
{
	return this->m_bGain;
}

void CDigitalGain::SetGGain(int gain)
{
	this->m_gGain = gain&0xff;
}

int CDigitalGain::GetGGain(void)
{
	return this->m_gGain;
}

void CDigitalGain::SetGlobalGain(int gain)
{
	this->m_globalGain = gain&0xff;
}

int CDigitalGain::GetGlobalGain(void)
{
	return this->m_globalGain;
}

void CDigitalGain::SetGlobalEn(int val)
{
	this->m_globalEn = val&0x1;
}

void CDigitalGain::SetRgbEn(int val)
{
	this->m_rgbEn = val&0x1;
}

void CDigitalGain::SetGainStep(int step)
{
	this->m_gainStep = step&0xff;
}

int CDigitalGain::GetGainStep(void)
{
	return this->m_gainStep;
}