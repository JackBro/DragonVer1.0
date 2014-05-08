#ifndef _CMODEL_ADD2GATE_H_
#define _CMODEL_ADD2GATE_H_


#ifndef CMBOXGATES_DLL
	#define CMBOXGATES_DLL  __declspec(dllimport)
#endif

#include "CmodelBase/imageunit.h"
#include "CmAlgGates/add2Alg.h"


class CMBOXGATES_DLL CAdd2Gate : public CImageUnit
{
public:
	CAdd2Gate(void);
	virtual ~CAdd2Gate(void);

protected:	
	virtual void UpdateConfig(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
	void	SetClip(int val);
	int		GetClip(void);
	void	SetInUnit(int val);
	int		GetInUnit(void);
	void	SetOutUnit(int val);
	int		GetOutUnit(void);
	void	SetOpdata(int val);
	int		GetOpdata(void);

private:
	int		m_clip;
	int		m_InUnit;
	int		m_OutUnit;
	int		m_opdata;

private:

	CAdd2Alg	m_alg2;
	
};

extern CMBOXGATES_DLL const int g_GatesFmtCount;
extern CMBOXGATES_DLL const int g_GateFormats[];


#endif
