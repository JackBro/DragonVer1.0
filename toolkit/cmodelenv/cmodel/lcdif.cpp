

#include "lcdif.h"


//==================== class CLcdIFModule ===================

CLcdIFModule::CLcdIFModule() : CChipBase(gChipPara[IDD_LCDIF])
{
	this->InitProcess();
}

