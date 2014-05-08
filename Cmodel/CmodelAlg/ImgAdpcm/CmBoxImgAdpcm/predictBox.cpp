#include "internal.h"

static const int g_PredictFormats[] =
{
	YUV422_UYVY | (VIDEOCLASS << CLASSSHIFT),
	YUV422_VYUY | (VIDEOCLASS << CLASSSHIFT),
	YUV422_YUYV | (VIDEOCLASS << CLASSSHIFT),
	YUV422_YVYU | (VIDEOCLASS << CLASSSHIFT),
	YUV422_YUVY | (VIDEOCLASS << CLASSSHIFT),

	YUV422_YVUY | (VIDEOCLASS << CLASSSHIFT),
	YUV422_UVYY | (VIDEOCLASS << CLASSSHIFT),
	YUV422_VUYY | (VIDEOCLASS << CLASSSHIFT),
	YUV422_YYVU | (VIDEOCLASS << CLASSSHIFT),
	YUV422_YYUV | (VIDEOCLASS << CLASSSHIFT),

	YUV411_YYYYUV | (VIDEOCLASS << CLASSSHIFT),
	YUV411_YYYYVU | (VIDEOCLASS << CLASSSHIFT),
	YUV411_YUYVYY | (VIDEOCLASS << CLASSSHIFT),
	YUV411_YVYUYY | (VIDEOCLASS << CLASSSHIFT),
	YUV411_UVYYYY | (VIDEOCLASS << CLASSSHIFT),
	YUV411_VUYYYY | (VIDEOCLASS << CLASSSHIFT),

	RAW_Y | (VIDEOCLASS << CLASSSHIFT),
	YUV400 | (VIDEOCLASS << CLASSSHIFT),
	EDGE_MAP | (VIDEOCLASS << CLASSSHIFT),

	YUV444 | (VIDEOCLASS << CLASSSHIFT),

	RAW_BG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GB | (VIDEOCLASS << CLASSSHIFT),
	RAW_RG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GR | (VIDEOCLASS << CLASSSHIFT),

	RAW_RGB | (VIDEOCLASS << CLASSSHIFT),
	RAW_RBG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GBR | (VIDEOCLASS << CLASSSHIFT),
	RAW_GRB | (VIDEOCLASS << CLASSSHIFT),
	RAW_BGR | (VIDEOCLASS << CLASSSHIFT),
	RAW_BRG | (VIDEOCLASS << CLASSSHIFT),

	RGB555 | (VIDEOCLASS << CLASSSHIFT),
	RGB565 | (VIDEOCLASS << CLASSSHIFT),
	RGB24 | (VIDEOCLASS << CLASSSHIFT),
	RGB32 | (VIDEOCLASS << CLASSSHIFT),

	RAW_R | (VIDEOCLASS << CLASSSHIFT),
	RAW_G | (VIDEOCLASS << CLASSSHIFT),
	RAW_B | (VIDEOCLASS << CLASSSHIFT),

	RAW_Y | (VIDEOCLASS << CLASSSHIFT),
	RAW_U444 | (VIDEOCLASS << CLASSSHIFT),
	RAW_U422 | (VIDEOCLASS << CLASSSHIFT),
	RAW_U420 | (VIDEOCLASS << CLASSSHIFT),
	RAW_U411 | (VIDEOCLASS << CLASSSHIFT),
	RAW_V444 | (VIDEOCLASS << CLASSSHIFT),
	RAW_V422 | (VIDEOCLASS << CLASSSHIFT),
	RAW_V420 | (VIDEOCLASS << CLASSSHIFT),
	RAW_V411 | (VIDEOCLASS << CLASSSHIFT),

	YIQ | (VIDEOCLASS << CLASSSHIFT),
	HSI | (VIDEOCLASS << CLASSSHIFT),
	RAW_YIQ_Y | (VIDEOCLASS << CLASSSHIFT),
	RAW_YIQ_I | (VIDEOCLASS << CLASSSHIFT),
	RAW_YIQ_Q | (VIDEOCLASS << CLASSSHIFT),

	RAW_HSI_I | (VIDEOCLASS << CLASSSHIFT),
	RAW_HSI_H | (VIDEOCLASS << CLASSSHIFT),
	RAW_HSI_S | (VIDEOCLASS << CLASSSHIFT)
};
static const int g_PredFmtCount = sizeof(g_PredictFormats) / sizeof(int);

CPredictBox::CPredictBox(void) : CImageUnit(2, 1, "Add Gate", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->SetInportFormat(0, g_PredFmtCount, (int *)g_PredictFormats);
	this->SetInportFormat(1, g_PredFmtCount, (int *)g_PredictFormats);
	this->m_pAlg = &this->m_alg1;
	this->m_clip = 255;
	this->m_unit = 1;
}

CPredictBox::~CPredictBox(void)
{
}

int	CPredictBox::GetID(void)
{
	return ADDGATE_BOX | (LOGICCLASS << CLASSSHIFT);;
}

void CPredictBox::UpdateConfig(void)
{
	this->m_alg1.SetClip(this->m_clip);
	this->m_alg1.SetUnit(this->m_unit);
}

void CPredictBox::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_alg1;
	CBaseUnit::ChoiceAlg(sel);
}

void CPredictBox::SetClip(int val)
{
	this->m_clip = val;
}

int	CPredictBox::GetClip(void)
{
	return this->m_clip;
}

void CPredictBox::SetUnit(int val)
{
	this->m_unit = val;
}

int	CPredictBox::GetUnit(void)
{
	return this->m_unit;
}

