#ifndef _TRM_NAND_OPTION_H_
#define _TRM_NAND_OPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

	// Nandflash type list
	#define	NAND_K9F2808U0C		0


	// Current nandflash type
	#define	NAND_TYPE			NAND_K9F2808U0C



#if	NAND_TYPE == NAND_K9F2808U0C
	#define	BYTES_PER_PAGE		528		// 512 + 16 = 528 bytes
	#define BLOCK_MAP_SIZE		128		// 1024 / 8-bit = 128 bytes
#else
	#error Please select nandflash type
#endif


	extern const TNandInfo		gNandInfo;
	extern const char *gNandTypeList[];


#ifdef __cplusplus
}
#endif

#endif
