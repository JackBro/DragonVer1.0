#include "internal.h"

static const int g_BoxFormats[] =
{
	YUV444 | (VIDEOCLASS << CLASSSHIFT)
};



CYuv2rgbBox::CYuv2rgbBox(void) : CImageUnit(1, 1, "Yuv2Rgb", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_alg1;
	this->SetInportFormat(0, sizeof(g_BoxFormats) / sizeof(int), (int *)g_BoxFormats);
}

CYuv2rgbBox::~CYuv2rgbBox(void)
{
}

int	CYuv2rgbBox::GetID(void)
{
	return YUV2RGB_BOX | (STILLCLASS << CLASSSHIFT);;
}

void CYuv2rgbBox::UpdateConfig(void)
{
	//body config
}

void CYuv2rgbBox::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_alg1;
	CBaseUnit::ChoiceAlg(sel);
}
