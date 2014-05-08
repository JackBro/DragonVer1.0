#ifndef _CMODEL_ADDGATE_H_
#define _CMODEL_ADDGATE_H_


#ifndef CMBOXGATES_DLL
	#define CMBOXGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/add1Alg.h"

class CMBOXGATES_DLL CAddGate : public CImageUnit
{
public:
	CAddGate(void);
	virtual ~CAddGate(void);

protected:	
	virtual void UpdateConfig(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
	void	SetClip(int val);
	int		GetClip(void);
	void	SetUnit(int val);
	int		GetUnit(void);

private:
	int		m_clip;
	int		m_unit;

private:
	CAdd1Alg	m_alg1;
};

extern CMBOXGATES_DLL const int g_GatesFmtCount;
extern CMBOXGATES_DLL const int g_GateFormats[];


#endif
