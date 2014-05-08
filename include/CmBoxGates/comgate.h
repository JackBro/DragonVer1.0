#ifndef _CMODEL_COMGATE_H_
#define _CMODEL_COMGATE_H_


#ifndef CMBOXGATES_DLL
	#define CMBOXGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/com1Alg.h"


class CMBOXGATES_DLL CComGate : public CImageUnit
{
public:
	CComGate(void);
	virtual ~CComGate(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

protected:	
	virtual void UpdateConfig(void);

private:
	CCom1Alg		m_com1alg;
};

#endif
