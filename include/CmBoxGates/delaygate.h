#ifndef _CMODEL_DELAYGATE_H_
#define _CMODEL_DELAYGATE_H_



#ifndef CMBOXGATES_DLL
#define CMBOXGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/delay1Alg.h"

class CMBOXGATES_DLL CDelayGate :	public CImageUnit
{
public:
	CDelayGate(void);
	virtual ~CDelayGate(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
	void	SetDelay(int delay);
	int		GetDelay(void);

protected:	
	virtual void UpdateConfig(void);

private:
	int		m_delay;
	CDelay1Alg	m_delay1alg;
};

#endif
