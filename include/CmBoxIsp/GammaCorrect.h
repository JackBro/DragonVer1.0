#ifndef _CMODEL_GAMMACORRECT_H_
#define _CMODEL_GAMMACORRECT_H_


#ifndef CMODELISP_DLL
	#define CMODELISP_DLL  __declspec(dllimport)
#endif


#include "CmodelBase/imageunit.h"
#include "CmAlgIsp/568GammaCorrectAlg.h"

class CMODELISP_DLL CGammaCorrect : public CImageUnit
{
public:
	CGammaCorrect(void);
	virtual ~CGammaCorrect(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	C568GammaCorrectAlg m_568GammaCorrectAlg;

	int m_rgamma[17];
	int m_ggamma[17];
	int m_bgamma[17];
	int m_enable;		//1: enable gamma  0:disable gamma,just cut lower 2 bit
	int m_dither;
	int m_dither_val;
};

#endif
