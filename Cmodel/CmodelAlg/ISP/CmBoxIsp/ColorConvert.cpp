#include "internal.h"

static const int g_ColorConvertFormats[] =
{
	RAW_RGB | (VIDEOCLASS << CLASSSHIFT),
};

static const char *gColorCorrectAlgType[] = { "V568ColorConvert"};
static const int  gTotalColorConvertAlgType = sizeof(gColorCorrectAlgType) / sizeof(char *);


CColorConvert::CColorConvert(void) : CImageUnit(1, 1, "Color Convert", 10)
{
	this->SetAlgStr(gTotalColorConvertAlgType, (char **)gColorCorrectAlgType);
	this->m_pAlg = &this->m_568ColorConvertAlg;

	this->SetInportFormat(0, sizeof(g_ColorConvertFormats) / sizeof(int), (int *)g_ColorConvertFormats);
}

CColorConvert::~CColorConvert(void)
{
}

int	CColorConvert::GetID(void)
{
	return COLORCONVERT_BOX | (ISPCLASS << CLASSSHIFT);;
}

void CColorConvert::UpdateConfig(void)
{
}

void CColorConvert::ChoiceAlg(int sel)
{
	switch(sel)
	{
	case 0:
		this->m_pAlg = &this->m_568ColorConvertAlg;
		break;
	default:
		this->m_pAlg = &this->m_568ColorConvertAlg;
	}

	CBaseUnit::ChoiceAlg(sel);
}

