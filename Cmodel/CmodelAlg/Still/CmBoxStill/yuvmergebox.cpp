
#include "internal.h"

static const int g_Fmt0Formats[] =
{
	YUV400 | (VIDEOCLASS << CLASSSHIFT),
	EDGE_MAP | (VIDEOCLASS << CLASSSHIFT),
	RAW_Y | (VIDEOCLASS << CLASSSHIFT)
};
static const int g_Fmt0Count = sizeof(g_Fmt0Formats) / sizeof(int);

static const int g_Fmt1Formats[] =
{
	RAW_U444 | (VIDEOCLASS << CLASSSHIFT),
	RAW_U422 | (VIDEOCLASS << CLASSSHIFT),
	RAW_U411 | (VIDEOCLASS << CLASSSHIFT)
};
static const int g_Fmt1Count = sizeof(g_Fmt1Formats) / sizeof(int);

static const int g_Fmt2Formats[] =
{
	RAW_V444 | (VIDEOCLASS << CLASSSHIFT),
	RAW_V422 | (VIDEOCLASS << CLASSSHIFT),
	RAW_V411 | (VIDEOCLASS << CLASSSHIFT)
};
static const int g_Fmt2Count = sizeof(g_Fmt2Formats) / sizeof(int);

CYuvMergeBox::CYuvMergeBox(void) : CImageUnit(3, 1, "Yuv merge", 0)
{
	this->SetAlgStr(1, (char **)gDefaultAlgType);
	this->m_pAlg = &this->m_alg1;
	this->SetInportFormat(0, g_Fmt0Count, (int *)g_Fmt0Formats);
	this->SetInportFormat(1, g_Fmt1Count, (int *)g_Fmt1Formats);
	this->SetInportFormat(2, g_Fmt2Count, (int *)g_Fmt2Formats);
}

CYuvMergeBox::~CYuvMergeBox(void)
{
}

int	CYuvMergeBox::GetID(void)
{
	return YUVMERGE_BOX | (STILLCLASS << CLASSSHIFT);;
}

void CYuvMergeBox::UpdateConfig(void)
{
}

void CYuvMergeBox::ChoiceAlg(int sel)
{
	sel = 0;
	this->m_pAlg = &this->m_alg1;
	CBaseUnit::ChoiceAlg(sel);
}

