#ifndef _CMODEL_XORGATE_H_
#define _CMODEL_XORGATE_H_


#ifndef CMBOXGATES_DLL
	#define CMBOXGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/xor1Alg.h"

class CMBOXGATES_DLL CXorGate : public CImageUnit
{
public:
	CXorGate(void);
	virtual ~CXorGate(void);

protected:	
	virtual void UpdateConfig(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

private:
	CXor1Alg	m_xor1alg;
};

#endif
