

#include "lcdc.h"

//==================== class CLcdcModule ===================

CLcdcModule::CLcdcModule() : CChipBase(gChipPara[IDD_LCDC])
{
	this->InitProcess();
}

