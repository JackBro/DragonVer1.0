
#include "internal.h"


static const int g_EdgeEnhanceFormats1[] =
{
	YUV422_YUYV | (VIDEOCLASS << CLASSSHIFT),
};

static const int g_EdgeEnhanceFormats2[] =
{
	EDGE_MAP | (VIDEOCLASS << CLASSSHIFT),
};

static const char *gEdgeEnhanceAlgType[] = { "V568EdgeEnhance"};
static const int  gTotalEdgeEnhanceAlgType = sizeof(gEdgeEnhanceAlgType) / sizeof(char *);



CEdgeEnhance::CEdgeEnhance(void) : CImageUnit(2, 1, "Edge enhance", 10)
{
	this->SetAlgStr(gTotalEdgeEnhanceAlgType, (char **)gEdgeEnhanceAlgType);
	this->m_pAlg = &this->m_568EdgeEnhanceAlg;

	this->SetInportFormat(0, sizeof(g_EdgeEnhanceFormats1) / sizeof(int), (int *)g_EdgeEnhanceFormats1);
	this->SetInportFormat(1, sizeof(g_EdgeEnhanceFormats2) / sizeof(int), (int *)g_EdgeEnhanceFormats2);
}


CEdgeEnhance::~CEdgeEnhance(void)
{
}

int	CEdgeEnhance::GetID(void)
{
	return EDGEENHANCE_BOX | (ISPCLASS << CLASSSHIFT);;
}

void CEdgeEnhance::UpdateConfig(void)
{
}

void CEdgeEnhance::ChoiceAlg(int sel)
{
	switch(sel)
	{
	case 0:
		this->m_pAlg = &this->m_568EdgeEnhanceAlg;
		break;
	default:
		this->m_pAlg = &this->m_568EdgeEnhanceAlg;
	}

	CBaseUnit::ChoiceAlg(sel);
}

