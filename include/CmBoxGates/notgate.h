#ifndef _CMODEL_NOTGATE_H_
#define _CMODEL_NOTGATE_H_


#ifndef CMBOXGATES_DLL
	#define CMBOXGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/not1Alg.h"


class CMBOXGATES_DLL CNotGate : public CImageUnit
{
public:
	CNotGate(void);
	virtual ~CNotGate(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	CNot1Alg		m_not1alg;
};

#endif
