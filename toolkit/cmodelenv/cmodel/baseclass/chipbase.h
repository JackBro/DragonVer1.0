

#ifndef _CMODEL_CHIPBASE_H_
#define _CMODEL_CHIPBASE_H_


#include "modulebase.h"
//#include "./algorism/"
//
//	use "COM" conception later to modify cmodel
//



//=============== class CIspModule =================

class CChipBase : public CModuleBase
{
public:
	CChipBase(TModulePara para);
	virtual ~CChipBase();

protected:
	virtual void	InitModuleIndex(void);
	virtual void	OnWriteReg(int adr, int val);
	virtual void	InitRegProp(void);
	virtual int		CanRun(int clk);
	virtual void	doProcess(TStream *input, TStream *output);
	virtual void	SelfDriver(TStream *input, int index);
	virtual void	ResetCounter(void);
};


#endif


