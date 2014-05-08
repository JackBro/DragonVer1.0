#include	"../../s3c44box/s3c44box.h"
#include "nandenv.h"
#include "../driver/nanddrv.h"



///////////////////////////////////////////////////////////////////////////////////
//                              Initial Env for Nand                             //
///////////////////////////////////////////////////////////////////////////////////

void NandInitEnv(void)
{
	BusInit(BUS_IDX_1, BUS8);
//	NandInit();
}
