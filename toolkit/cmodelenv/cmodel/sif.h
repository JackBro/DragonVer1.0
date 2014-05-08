

#ifndef _CMODEL_SENSORIF_H_
#define _CMODEL_SENSORIF_H_

#include "./baseclass/chipbase.h"
#include "./algorism/sifalg.h"

#if		SENSOR_8BIT
#define		SENSOR_BUSWIDTH		TRAW8_TYPE
#else
#define		SENSOR_BUSWIDTH		TRAW10_TYPE
#endif
//
//	use "COM" conception later to modify cmodel
//


//=============== class CSifModule =================

class CSifModule : public CChipBase, public CSifAlg
{
public:
	CSifModule();
	virtual ~CSifModule();

protected:
	void	UpdateRegister(void);

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


