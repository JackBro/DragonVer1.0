

#include "sdcard.h"


//==================== class CSdcardModule ===================

CSdcardModule::CSdcardModule() : CChipBase(gChipPara[IDD_SDCARD])
{
	this->InitProcess();
}

