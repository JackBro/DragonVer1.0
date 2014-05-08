


#ifndef _RDK_CHIP_H_
#define _RDK_CHIP_H_

//Define chip type
#define V5_CHIP_558		0
#define V5_CHIP_558L	1
#define V5_CHIP_568		2
#define V5_CHIP_568L	3
#define V5_CHIP_578		4

#define V5_RELEASE_FINAL 0
#define V5_RELEASE_BElTA 1
#define V5_RELEASE_ALPHA 2
#define V5_RELEASE_TEST	 3 /* IF > 3, ALL IS TEST */


//Define CHIP version
#define V5_CHIP_TYPE		V5_CHIP_578
//Define MAIN version
#define V5_VER_MAIN			1
//Define BRANCH version
#define V5_VER_BRANCH		0
//Define RELEASE type
#define V5_RELEASE_TYPE		3

//Compatible with old ASIC(Mark it when use FPGA) 
//#define ADAPT_OLD_ASIC


#if V5_CHIP_TYPE == V5_CHIP_568

#include "vregdef.h"

#elif V5_CHIP_TYPE == V5_CHIP_578

#include "v578regdef.h"

#endif

#endif

