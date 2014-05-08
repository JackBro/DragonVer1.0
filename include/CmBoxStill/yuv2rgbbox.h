#ifndef _CMODEL_YUV2RGB_BOX_H_
#define _CMODEL_YUV2RGB_BOX_H_



#ifndef CMODELSTBOX_DLL
#define CMODELSTBOX_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgStill/yuv2rgbAlg.h"

class CMODELSTBOX_DLL CYuv2rgbBox :	public CImageUnit
{
public:
	CYuv2rgbBox(void);
	virtual ~CYuv2rgbBox(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	CYuv2rgbAlg	m_alg1;
};

#endif
