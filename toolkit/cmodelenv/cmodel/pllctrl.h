

#ifndef _CMODEL_PLLCTRL_H_
#define _CMODEL_PLLCTRL_H_

#include "./baseclass/chipbase.h"

//
//	use "COM" conception later to modify cmodel
//


//=============== class CPllCtrlModule =================

class CPllCtrlModule : public CChipBase
{
public:
	CPllCtrlModule();

public:
	virtual int		Process(int clk);

};


#endif


