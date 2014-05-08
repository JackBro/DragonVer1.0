

#include "usbc.h"

//==================== class CUsbcModule ===================

CUsbcModule::CUsbcModule() : CChipBase(gChipPara[IDD_USBC])
{
	this->InitProcess();
}

