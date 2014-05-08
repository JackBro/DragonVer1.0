#include "internal.h"


static const int g_CfaFormats[] =
{
	RAW_BG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GB | (VIDEOCLASS << CLASSSHIFT),
	RAW_RG | (VIDEOCLASS << CLASSSHIFT),
	RAW_GR | (VIDEOCLASS << CLASSSHIFT)
};

static const char *gCfaExtractAlgType[] = { "V568CfaExtract"};
static const int  gTotalCfaExtractAlgType = sizeof(gCfaExtractAlgType) / sizeof(char *);

CCfaExtract::CCfaExtract(void) : CImageUnit(1, 2, "CFA/EDGE", 10)
{
	this->SetAlgStr(gTotalCfaExtractAlgType, (char **)gCfaExtractAlgType);
	this->m_pAlg = &this->m_CfaExtract568Alg;

	this->SetInportFormat(0, sizeof(g_CfaFormats) / sizeof(int), (int *)g_CfaFormats);

	this->m_mode = m_CfaExtract568Alg.GetMode();
	this->m_femm = m_CfaExtract568Alg.GetFemm();
	this->m_femp = m_CfaExtract568Alg.GetFemp();
	this->m_femmin = m_CfaExtract568Alg.GetFemmin();
	this->m_femmax = m_CfaExtract568Alg.GetFemmax();
	this->m_femx1 = m_CfaExtract568Alg.GetFemx1();
	this->m_femx2 = m_CfaExtract568Alg.GetFemx2();
	this->m_divedge = m_CfaExtract568Alg.GetDivEdge();
	memcpy(&this->m_coefedge, m_CfaExtract568Alg.GetCoefEdge(), sizeof(this->m_coefedge));
}

CCfaExtract::~CCfaExtract(void)
{
}

int	CCfaExtract::GetID(void)
{
	return CFAEXTRACT_BOX | (ISPCLASS << CLASSSHIFT);;
}

void CCfaExtract::UpdateConfig(void)
{
	switch(this->GetCurAlg()) 
	{
	case 0:
		this->m_CfaExtract568Alg.SetMode(this->m_mode);
		this->m_CfaExtract568Alg.SetFemm(this->m_femm);
		this->m_CfaExtract568Alg.SetFemp(this->m_femp);
		this->m_CfaExtract568Alg.SetFemmin(this->m_femmin);
		this->m_CfaExtract568Alg.SetFemmax(this->m_femmax);
		this->m_CfaExtract568Alg.SetFemx1(this->m_femx1);
		this->m_CfaExtract568Alg.SetFemx2(this->m_femx2);
		this->m_CfaExtract568Alg.SetDivEdge(this->m_divedge);
		this->m_CfaExtract568Alg.SetCoefEdge(&this->m_coefedge[0][0]);
		break;
	default:
		this->m_CfaExtract568Alg.SetMode(this->m_mode);
		this->m_CfaExtract568Alg.SetFemm(this->m_femm);
		this->m_CfaExtract568Alg.SetFemp(this->m_femp);
		this->m_CfaExtract568Alg.SetFemmin(this->m_femmin);
		this->m_CfaExtract568Alg.SetFemmax(this->m_femmax);
		this->m_CfaExtract568Alg.SetFemx1(this->m_femx1);
		this->m_CfaExtract568Alg.SetFemx2(this->m_femx2);
		this->m_CfaExtract568Alg.SetDivEdge(this->m_divedge);
		this->m_CfaExtract568Alg.SetCoefEdge(&this->m_coefedge[0][0]);
		break;
	}
}

void CCfaExtract::ChoiceAlg(int sel)
{
	switch(sel) 
	{
	case 0:
		this->m_pAlg = &this->m_CfaExtract568Alg;
		break;
	default:
		this->m_pAlg = &this->m_CfaExtract568Alg;
		break;
	}
	CBaseUnit::ChoiceAlg(sel);
}

//edge extract
void CCfaExtract::SetMode(int mode)
{
	this->m_mode = mode&0x3;
}

int CCfaExtract::GetMode(void)
{
	return this->m_mode;
}

void CCfaExtract::SetFemm(int femm)
{
	this->m_femm = femm&0xff;
}

int CCfaExtract::GetFemm(void)
{
	return this->m_femm;
}

void CCfaExtract::SetFemp(int femp)
{
	this->m_femp = femp&0xff;
}

int CCfaExtract::GetFemp(void)
{
	return this->m_femp;
}

void CCfaExtract::SetFemmin(int femmin)
{
	this->m_femmin = femmin&0xff;
}

int CCfaExtract::GetFemmin(void)
{
	return this->m_femmin;
}

void CCfaExtract::SetFemmax(int femmax)
{
	this->m_femmax = femmax&0xff;
}

int CCfaExtract::GetFemmax(void)
{
	return this->m_femmax;
}

void CCfaExtract::SetFemx1(int femx1)
{
	this->m_femx1 = femx1&0xff;
}

int CCfaExtract::GetFemx1(void)
{
	return this->m_femx1;
}

void CCfaExtract::SetFemx2(int femx2)
{
	this->m_femx2 = femx2&0xff;
}

int CCfaExtract::GetFemx2(void)
{
	return this->m_femx2;
}

void CCfaExtract::SetCoefEdge(char *coef)
{
	memcpy(this->m_coefedge, coef, sizeof(this->m_coefedge));
}
char* CCfaExtract::GetCoefEdge()
{
	return this->m_coefedge[0];
}

void CCfaExtract::SetDivEdge(int div)
{
	this->m_divedge = div;
}

int CCfaExtract::GetDivEdge()
{
	return this->m_divedge;
}
