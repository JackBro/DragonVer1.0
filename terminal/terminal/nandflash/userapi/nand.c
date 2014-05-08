#include	"../driver/nanddrv.h"
#include	"option.h"

#if	NAND_TYPE == NAND_K9F2808U0C
	
	const TNandInfo	gNandInfo = 
	{
		0xEC73,						// Maker ID = 0xEC, Device ID = 0x73
		1,							// unit is 1 byte
		1024,						// 1024 blocks
		32,							// 32 pages per block
		512,						// 512 data unit per page
		528							// Total unit per page
	};
	
#else

	#error Please select nandflash type

#endif

const char *gNandTypeList[] =
{
	"K9F2808U0C"
};
