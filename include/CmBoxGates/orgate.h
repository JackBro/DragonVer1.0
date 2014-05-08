#ifndef _CMODEL_ORGATE_H_
#define _CMODEL_ORGATE_H_



#ifndef CMBOXGATES_DLL
#define CMBOXGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/or1Alg.h"

class CMBOXGATES_DLL COrGate :	public CImageUnit
{
public:
	COrGate(void);
	virtual ~COrGate(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	COr1Alg		m_or1alg;
};

#endif
