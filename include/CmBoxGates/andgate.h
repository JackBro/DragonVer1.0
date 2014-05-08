#ifndef _CMODEL_ANDGATE_H_
#define _CMODEL_ANDGATE_H_


#ifndef CMBOXGATES_DLL
	#define CMBOXGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/and1Alg.h"

class CMBOXGATES_DLL CAndGate : public CImageUnit
{
public:
	CAndGate(void);
	virtual ~CAndGate(void);

protected:	
	virtual void UpdateConfig(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

private:
	CAnd1Alg	m_and1alg;
};

#endif
