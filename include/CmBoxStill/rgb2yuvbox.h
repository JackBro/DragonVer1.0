#ifndef _CMODEL_RGB2YUV_BOX_H_
#define _CMODEL_RGB2YUV_BOX_H_



#ifndef CMODELSTBOX_DLL
#define CMODELSTBOX_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgStill/rgb2yuvAlg.h"

class CMODELSTBOX_DLL CRgb2yuvBox :	public CImageUnit
{
public:
	CRgb2yuvBox(void);
	virtual ~CRgb2yuvBox(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	CRgb2yuvAlg	m_alg1;
};

#endif
