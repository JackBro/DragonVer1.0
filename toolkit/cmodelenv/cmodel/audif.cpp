

#include "audif.h"


//==================== class CAudioIFModule ===================

CAudioIFModule::CAudioIFModule() : CChipBase(gChipPara[IDD_AUDIF])
{
	this->InitProcess();
}

