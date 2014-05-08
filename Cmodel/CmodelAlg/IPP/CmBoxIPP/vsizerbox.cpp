
#include "internal.h"


static const char *gVsizerAlgType[] = { "702Vsizer" };
static const int  gTotalVsizerAlgType = sizeof(gVsizerAlgType) / sizeof(char *);
static const int g_VsizerFormats[] =
{
		YUV422_YUYV | (VIDEOCLASS << CLASSSHIFT)		
};

CVSizerBox::CVSizerBox(void) : CImageUnit(1, 1, "Vertical Sizer", 0)
{
	this->SetAlgStr(gTotalVsizerAlgType, (char **)gVsizerAlgType);
	this->m_pAlg = &this->m_702VsizerAlg;
	this->m_pal = this->m_702VsizerAlg.GetOutMode();
	this->SetInportFormat(0, sizeof(g_VsizerFormats) / sizeof(int), (int *)g_VsizerFormats);
}

CVSizerBox::~CVSizerBox(void)
{
}

int CVSizerBox::GetID(void)
{
	return VERSIZER_BOX | (IPPCLASS << CLASSSHIFT);
}

void CVSizerBox::UpdateConfig(void)
{
	this->m_702VsizerAlg.SetOutMode(this->m_pal);
	this->m_702VsizerAlg.SetFactor();
}

void CVSizerBox::ChoiceAlg(int sel)
{
	switch(sel) 
	{
	case 0:
		this->m_pAlg = &this->m_702VsizerAlg;
		break;	
	default:
		this->m_pAlg = &this->m_702VsizerAlg;
		break;
	}
	CBaseUnit::ChoiceAlg(sel);
}

void CVSizerBox::SetOutMode(int pal)
{
	this->m_pal = pal;
}

int CVSizerBox::GetOutMode(void)
{
	return this->m_pal;
}

