
#include "internal.h"

static const int g_GammaColorFormats[] =
{
	RAW_RGB | (VIDEOCLASS << CLASSSHIFT),
};

static const char *gGammaCorrectAlgType[] = { "V568GammaCorrcet"};
static const int  gTotalGammaConvertAlgType = sizeof(gGammaCorrectAlgType) / sizeof(char *);



CGammaCorrect::CGammaCorrect(void) : CImageUnit(1, 1, "Gamma correct", 10)
{
	this->SetAlgStr(gTotalGammaConvertAlgType, (char **)gGammaCorrectAlgType);

	this->m_pAlg = &this->m_568GammaCorrectAlg;
	this->SetInportFormat(0, sizeof(g_GammaColorFormats) / sizeof(int), (int *)g_GammaColorFormats);

}


CGammaCorrect::~CGammaCorrect(void)
{
}

int	CGammaCorrect::GetID(void)
{
	return GAMMACORRECT_BOX | (ISPCLASS << CLASSSHIFT);;
}

void CGammaCorrect::UpdateConfig(void)
{

}

void CGammaCorrect::ChoiceAlg(int sel)
{
	switch(sel)
	{
	case 0:
		this->m_pAlg = &this->m_568GammaCorrectAlg;
		break;
	default:
		this->m_pAlg = &this->m_568GammaCorrectAlg;
	}

	CBaseUnit::ChoiceAlg(sel);
}

