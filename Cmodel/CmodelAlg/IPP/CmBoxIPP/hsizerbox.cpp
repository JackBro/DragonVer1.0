
#include "internal.h"


static const char *gHsizerAlgType[] = { "702Hsizer" };
static const int  gTotalHsizerAlgType = sizeof(gHsizerAlgType) / sizeof(char *);
static const int g_HsizerFormats[] =
{
	YUV422_YUYV | (VIDEOCLASS << CLASSSHIFT)		
};

CHSizerBox::CHSizerBox(void) : CImageUnit(1, 1, "Horizontal Sizer", 0)
{
	this->SetAlgStr(gTotalHsizerAlgType, (char **)gHsizerAlgType);
	this->m_pAlg = &this->m_702HsizerAlg;
	this->SetInportFormat(0, sizeof(g_HsizerFormats) / sizeof(int), (int *)g_HsizerFormats);
}

CHSizerBox::~CHSizerBox(void)
{
}

int CHSizerBox::GetID(void)
{
	return HORSIZER_BOX  | (IPPCLASS << CLASSSHIFT);
}

void CHSizerBox::UpdateConfig(void)
{

}

void CHSizerBox::ChoiceAlg(int sel)
{
	switch(sel) 
	{
	case 0:
		this->m_pAlg = &this->m_702HsizerAlg;
		break;	
	default:
		this->m_pAlg = &this->m_702HsizerAlg;
		break;
	}
	CBaseUnit::ChoiceAlg(sel);
}