

#include "audioadc.h"

//==================== class CAudioAdcModule ===================

CAudioAdcModule::CAudioAdcModule() : CChipBase(gChipPara[IDD_AUDIOADC])
{
	this->InitProcess();
}

