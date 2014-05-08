

#include "sto.h"

//==================== class CStoModule ===================

CStoModule::CStoModule() : CChipBase(gChipPara[IDD_STO])
{
	this->InitProcess();
}

