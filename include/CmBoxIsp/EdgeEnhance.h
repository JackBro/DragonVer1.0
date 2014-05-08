#ifndef _CMODEL_EDGEENHANCE_H_
#define _CMODEL_EDGEENHANCE_H_



#ifndef CMODELISP_DLL
	#define CMODELISP_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgIsp/568EdgeEnhanceAlg.h"

class CMODELISP_DLL CEdgeEnhance : public CImageUnit
{
public:
	CEdgeEnhance(void);
	virtual ~CEdgeEnhance(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	C568EdgeEnhanceAlg m_568EdgeEnhanceAlg;
};

#endif