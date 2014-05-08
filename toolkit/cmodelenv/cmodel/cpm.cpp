

#include "cpm.h"

//==================== class CCpmModule ===================

CCpmModule::CCpmModule() : CChipBase(gChipPara[IDD_CPM])
{
	this->InitProcess();
}

