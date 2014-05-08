#ifndef _SDRAM_H_
#define _SDRAM_H_

#ifdef __cplusplus
extern "C" {
#endif


	#define MAX_TRANSBUF_SIZE	0x200000	/* 2M */
	#define	SDRAM_FREE_LIMIT	0xc7fdfff
	#define	MALLOC_START_OFFSET	0x8000		/* 32K */
	#define	MALLOC_MAX_SIZE		0x200000	/* 2M */


	typedef struct tag_TSdramInfo
	{
		UINT8			flag;
		char			*start_addr;
		unsigned int	len;
	} TSdramInfo, *PTSdramInfo;

	extern TSdramInfo gSdramInfo;

	void GetFreeSdramInfo(void);
	void PrintSdramInfo(void);



#ifdef __cplusplus
}
#endif

#endif
