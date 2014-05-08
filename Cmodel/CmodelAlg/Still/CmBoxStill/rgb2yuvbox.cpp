
#include "internal.h"

static const int g_BoxFormats[] =
{
	RGB24 | (VIDEOCLASS << CLASSSHIFT)
};

CRgb2yuvBox::CRgb2yuvBox(void) : CImageUnit(1, 1, "Rgb2Yuv", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_alg1;
	this->SetInportFormat(0, sizeof(g_BoxFormats) / sizeof(int), (int *)g_BoxFormats);
}

CRgb2yuvBox::~CRgb2yuvBox(void)
{
}

int	CRgb2yuvBox::GetID(void)
{
	return RGB2YUV_BOX | (STILLCLASS << CLASSSHIFT);;
}

void CRgb2yuvBox::UpdateConfig(void)
{
	//body config
}

void CRgb2yuvBox::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_alg1;
	CBaseUnit::ChoiceAlg(sel);
}

