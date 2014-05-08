

#include "marb.h"

//==================== class CMarbModule ===================

CMarbModule::CMarbModule() : CChipBase(gChipPara[IDD_MARB])
{
	this->InitProcess();
}

