#ifndef _CMODEL_IMAGEUNIT_H_
#define _CMODEL_IMAGEUNIT_H_


#ifndef CMODELBASE_DLL
	#define CMODELBASE_DLL  __declspec(dllimport)
#endif

#include "baseunit.h"

class CMODELBASE_DLL CImageUnit : public CBaseUnit
{
public:
	CImageUnit(int inp = 1, int oup = 1, char *desc = NULL, int dummyline = 0, int size = BUFFER_DEFAULT_SIZE);
	virtual ~CImageUnit(void);

public:
	void	SetDummyline(int dummyline);
	int		GetDummyline(void);

protected:	
	virtual int	 DummyProcess(void);

protected:
	virtual int	CheckInpins(void);

public:
	virtual void StoreParm(CKeyTree *ptree);
	virtual void LoadParm(CKeyTree *ptree);

private:
	int		m_dummcount;
	int		m_dummyline;
};

#endif

