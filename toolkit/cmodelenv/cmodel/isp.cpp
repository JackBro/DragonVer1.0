

#include "isp.h"


//==================== class CIspModule ===================

CIspModule::CIspModule() : CChipBase(gChipPara[IDD_ISP])
{
	this->InitProcess();
}

