#include "internal.h"



static const int g_ColorCorrectFormats[] =
{
	RAW_RGB | (VIDEOCLASS << CLASSSHIFT),
};

static const char *gColorCorrectAlgType[] = { "V568ColorCorrect"};
static const int  gTotalColorCorrectAlgType = sizeof(gColorCorrectAlgType) / sizeof(char *);


CColorCorrect::CColorCorrect(void) : CImageUnit(1, 1, "Color Correct", 10)
{
	this->SetAlgStr(gTotalColorCorrectAlgType, (char **)gColorCorrectAlgType);
	this->m_pAlg = &this->m_568ColorCorrectAlg;

	this->SetInportFormat(0, sizeof(g_ColorCorrectFormats) / sizeof(int), (int *)g_ColorCorrectFormats);

	int i, j;
	int *p = m_568ColorCorrectAlg.GetColorMatrix();
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			this->m_colormatrix[i][j] = p[i*3+j]&0xff;

	p = m_568ColorCorrectAlg.GetColorOffset();
	for(i = 0; i < 3; i++)
		this->m_offset[i] = p[i]&0xff;
}

CColorCorrect::~CColorCorrect(void)
{
}

int	CColorCorrect::GetID(void)
{
	return COLORCORRECT_BOX  | (ISPCLASS << CLASSSHIFT);;
}



void CColorCorrect::ChoiceAlg(int sel)
{
	switch(sel)
	{
	case 0:
		this->m_pAlg = &this->m_568ColorCorrectAlg;
		break;
	default:
		this->m_pAlg = &this->m_568ColorCorrectAlg;
	}
	
	CBaseUnit::ChoiceAlg(sel);
}

void CColorCorrect::SetColorMatrix(int *p)
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			this->m_colormatrix[i][j] = p[i*3+j]&0xff;
}

int* CColorCorrect::GetColorMatrix()
{
	return &this->m_colormatrix[0][0];
}


void CColorCorrect::SetColorOffset(int *p)
{
	int i;

	for(i = 0; i < 3; i++)
		this->m_offset[i] = p[i]&0xff;
}

int* CColorCorrect::GetColorOffset()
{
	return &this->m_offset[0];
}


void CColorCorrect::UpdateConfig(void)
{
	switch(this->GetCurAlg()) 
	{
	case 0:
		this->m_568ColorCorrectAlg.SetColorMatrix(&this->m_colormatrix[0][0]);
		this->m_568ColorCorrectAlg.SetColorOffset(&this->m_offset[0]);
		break;
	default:
		this->m_568ColorCorrectAlg.SetColorMatrix(&this->m_colormatrix[0][0]);
		this->m_568ColorCorrectAlg.SetColorOffset(&this->m_offset[0]);
	}
}