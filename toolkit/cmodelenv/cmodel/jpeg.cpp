

#include "jpeg.h"

//==================== class CJpegModule ===================

CJpegModule::CJpegModule() : CChipBase(gChipPara[IDD_JPEG])
{
	this->InitProcess();
}

