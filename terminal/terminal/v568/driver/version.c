
#include "driver.h"
#ifndef	V578CMD_DISABLE


/********************************************************************************
  Description:
	Get rdk version
  Parameters:
  Note:
  Returns:
		sting: version string 
		<TABLE>
		Version format:
		V5_AA_BB_CC_DD
		V5	: chip serial
		AA	: chip type
		BB	: main version
		CC	: branch version
		DD	: charactor//alpha....
		</TABLE>
  Remarks:
  Example:
*********************************************************************************/
void V5_GetRdkVersion(SINT8 *sting)
{
	sprintf(sting,"V5_%02d_%02d_%02d_%02d", V5_CHIP_TYPE, V5_VER_MAIN, V5_VER_BRANCH, V5_RELEASE_TYPE);
}




#endif //V578CMD_DISABLE

