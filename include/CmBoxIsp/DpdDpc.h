#ifndef _CMODEL_DPDDPC_H_
#define _CMODEL_DPDDPC_H_


#ifndef CMODELISP_DLL
	#define CMODELISP_DLL  __declspec(dllimport)
#endif


#include "CmodelBase/imageunit.h"
#include "CmAlgIsp/326DpdDpcAlg.h"
#include "CmAlgIsp/568DpdDpcAlg.h"

class CMODELISP_DLL CDpdDpc : public CImageUnit
{
public:
	CDpdDpc(void);
	virtual ~CDpdDpc(void);

public:
	virtual int	 GetID(void);
	virtual void ChoiceAlg(int sel);

public:
	void SetDpdThd(int thd);
	int  GetDpdThd() const;
	void SetNoiseTable(int *val);
	int  *GetNoiseTable();
protected:	
	virtual void UpdateConfig(void);

private:
	int m_dpdthd;
	int m_noisetable[gTotalNoiseTable];

	C326DpdDpcAlg	m_326DpdDpcAlg;
	C568DpdDpcAlg	m_568DpdDpcAlg;
	
};

#endif
