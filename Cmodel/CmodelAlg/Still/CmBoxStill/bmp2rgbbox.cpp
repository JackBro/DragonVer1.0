
#include "internal.h"

static const int g_BoxFormats[] =
{
	BMP | (VIDEOCLASS << CLASSSHIFT)
};

CBmp2rgbBox::CBmp2rgbBox(void) : CImageUnit(1, 1, "Bmp2Rgb", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_alg1;
	this->SetInportFormat(0, sizeof(g_BoxFormats) / sizeof(int), (int *)g_BoxFormats);
}

CBmp2rgbBox::~CBmp2rgbBox(void)
{
}

int	CBmp2rgbBox::GetID(void)
{
	return BMP2RGB_BOX | (STILLCLASS << CLASSSHIFT);;
}

void CBmp2rgbBox::UpdateConfig(void)
{
	//body config
}

void CBmp2rgbBox::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_alg1;
	CBaseUnit::ChoiceAlg(sel);
}

