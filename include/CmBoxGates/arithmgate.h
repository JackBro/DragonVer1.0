#ifndef _CMODEL_ARITHMGATE_H_
#define _CMODEL_ARITHMGATE_H_


#ifndef CMBOXGATES_DLL
	#define CMBOXGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/arithm1Alg.h"


class CMBOXGATES_DLL CArithmGate : public CImageUnit
{
public:
	CArithmGate(void);
	virtual ~CArithmGate(void);

protected:	
	virtual void UpdateConfig(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
	void	SetUpClip(int val);
	int		GetUpClip(void);
	void	SetLowClip(int val);
	int		GetLowClip(void);
	void	SetInUnit(int val);
	int		GetInUnit(void);
	void	SetOutUnit(int val);
	int		GetOutUnit(void);
	void	SetOpdata(int val);
	int		GetOpdata(void);
	void	SetOperator(int val);
	int		GetOperator(void);

private:
	int		m_UpClip;
	int		m_LowClip;
	int		m_InUnit;
	int		m_OutUnit;
	int		m_opdata;
	int		m_operator;

private:

	CArithm1Alg	m_alg1;
	
};

extern CMBOXGATES_DLL const int g_GatesFmtCount;
extern CMBOXGATES_DLL const int g_GateFormats[];


#endif
