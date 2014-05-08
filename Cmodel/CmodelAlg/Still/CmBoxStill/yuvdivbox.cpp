
#include "internal.h"

static const int g_YuvDivFormats[] =
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

	YUV444 | (VIDEOCLASS << CLASSSHIFT),

	RAW_RGB | (VIDEOCLASS << CLASSSHIFT),
	RAW_RBG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GBR | (VIDEOCLASS << CLASSSHIFT),
	RAW_GRB | (VIDEOCLASS << CLASSSHIFT),
	RAW_BGR | (VIDEOCLASS << CLASSSHIFT),
	RAW_BRG | (VIDEOCLASS << CLASSSHIFT),
};
static const int g_YuvDivFmtCount = sizeof(g_YuvDivFormats) / sizeof(int);

CYuvdivBox::CYuvdivBox(void) : CImageUnit(1, 3, "Yuv Divider", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_alg1;
	this->SetInportFormat(0, g_YuvDivFmtCount, (int *)g_YuvDivFormats);
}

CYuvdivBox::~CYuvdivBox(void)
{
}

int	CYuvdivBox::GetID(void)
{
	return YUVDIV_BOX | (STILLCLASS << CLASSSHIFT);;
}

void CYuvdivBox::UpdateConfig(void)
{
}

void CYuvdivBox::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_alg1;
	CBaseUnit::ChoiceAlg(sel);
}

