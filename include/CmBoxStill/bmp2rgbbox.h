#ifndef _CMODEL_BMP2RGBBOX_H_
#define _CMODEL_BMP2RGBBOX_H_



#ifndef CMODELSTBOX_DLL
#define CMODELSTBOX_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgStill/bmp2rgbAlg.h"

class CMODELSTBOX_DLL CBmp2rgbBox :	public CImageUnit
{
public:
	CBmp2rgbBox(void);
	virtual ~CBmp2rgbBox(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	CBmp2rgbAlg	m_alg1;
};

#endif
