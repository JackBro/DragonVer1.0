
#include "internal.h"

const int g_Yuv2YuvFormats[] =
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

	//YUV420_UYVY | (VIDEOCLASS << CLASSSHIFT),
	//YUV420_VYUY | (VIDEOCLASS << CLASSSHIFT),
	//YUV420_YUYV | (VIDEOCLASS << CLASSSHIFT),
	//YUV420_YVYU | (VIDEOCLASS << CLASSSHIFT),
	//YUV420_YUVY | (VIDEOCLASS << CLASSSHIFT),
	//
	//YUV420_YVUY | (VIDEOCLASS << CLASSSHIFT),
	//YUV420_UVYY | (VIDEOCLASS << CLASSSHIFT),
	//YUV420_VUYY | (VIDEOCLASS << CLASSSHIFT),
	//YUV420_YYVU | (VIDEOCLASS << CLASSSHIFT),
	//YUV420_YYUV | (VIDEOCLASS << CLASSSHIFT),

	YUV411_YYYYUV | (VIDEOCLASS << CLASSSHIFT),
	YUV411_YYYYVU | (VIDEOCLASS << CLASSSHIFT),
	YUV411_YUYVYY | (VIDEOCLASS << CLASSSHIFT),
	YUV411_YVYUYY | (VIDEOCLASS << CLASSSHIFT),
	YUV411_UVYYYY | (VIDEOCLASS << CLASSSHIFT),
	YUV411_VUYYYY | (VIDEOCLASS << CLASSSHIFT),

	RAW_Y | (VIDEOCLASS << CLASSSHIFT),
	YUV400 | (VIDEOCLASS << CLASSSHIFT),
	EDGE_MAP | (VIDEOCLASS << CLASSSHIFT),

	YUV444 | (VIDEOCLASS << CLASSSHIFT)
};
const int g_Yuv2YuvFmtCount = sizeof(g_Yuv2YuvFormats) / sizeof(int);

CYuv2yuvBox::CYuv2yuvBox(void) : CImageUnit(1, 1, "Yuv2Yuv", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_alg1;
	this->SetInportFormat(0, g_Yuv2YuvFmtCount, (int *)g_Yuv2YuvFormats);

	this->m_outfmt = YUV422_UYVY | (VIDEOCLASS << CLASSSHIFT);
	this->m_alg1.SetOutFmt(this->m_outfmt);
}

CYuv2yuvBox::~CYuv2yuvBox(void)
{
}

int	CYuv2yuvBox::GetID(void)
{
	return YUV2YUV_BOX | (STILLCLASS << CLASSSHIFT);;
}

void CYuv2yuvBox::UpdateConfig(void)
{
	switch(this->GetCurAlg())
	{
	case 0:
		this->m_alg1.SetOutFmt(this->m_outfmt);
		break;
	default:
		break;
	}
}

int	CYuv2yuvBox::GetOutFmt(void)
{
	return this->m_outfmt;
}

void CYuv2yuvBox::SetOutFmt(int fmt)
{
	this->m_outfmt = fmt;
}

void CYuv2yuvBox::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_alg1;
	CBaseUnit::ChoiceAlg(sel);
}

