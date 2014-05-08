

#include "hostif.h"

//==================== class CHostIFModule ===================

CHostIFModule::CHostIFModule() : CChipBase(gChipPara[IDD_HOSTIF])
{
	this->InitProcess();
}


