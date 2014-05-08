

#include "sensor.h"


//==================== class CSensorModule ===================

CSensorModule::CSensorModule() : CChipBase(gChipPara[IDD_SENSOR])
{
	this->InitProcess();
}

