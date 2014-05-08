#ifndef _CMODEL_SUBGATE_H_
#define _CMODEL_SUBGATE_H_


#ifndef CMBOXGATES_DLL
	#define CMBOXGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/sub1Alg.h"

class CMBOXGATES_DLL CSubGate : public CImageUnit
{
public:
	CSubGate(void);
	virtual ~CSubGate(void);

protected:	
	virtual void UpdateConfig(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

private:
	CSub1Alg	m_alg1;
};

#endif
