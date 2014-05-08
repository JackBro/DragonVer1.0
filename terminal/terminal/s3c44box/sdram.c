#include "internal.h"

#ifndef	WIN32
extern char Image$$RO$$Base[];
extern char Image$$RO$$Limit[];
extern char Image$$RW$$Base[];
extern char Image$$RW$$Limit[];
extern char Image$$ZI$$Base[];
extern char Image$$ZI$$Limit[];
#endif

TSdramInfo gSdramInfo;

#ifdef	WIN32

void GetFreeSdramInfo(void)
{
	gSdramInfo.flag = FALSE;
	gSdramInfo.start_addr = NULL;
	gSdramInfo.len = 0;
}

void PrintSdramInfo(void)
{
}

#else

void GetFreeSdramInfo(void)
{
	int saddr, length, i, addr;
	const val = 0x5a5a5a5a;

	gSdramInfo.flag = FALSE;
	gSdramInfo.start_addr = NULL;
	gSdramInfo.len = 0;

	saddr = (((int)Image$$RW$$Limit + 3) >> 2) << 2;		// 32bit aligned
	length = MALLOC_MAX_SIZE;
	if(saddr + length > SDRAM_FREE_LIMIT)
		length = SDRAM_FREE_LIMIT - saddr;
	length >>= 2;

	addr = saddr;
	for(i = 0; i < length; i++)
	{
		WriteMemory(addr, val);
		if(val != ReadMemory(addr))
			return;

		addr += 4;
	}

	gSdramInfo.flag = TRUE;
	gSdramInfo.start_addr = (char *)saddr;
	gSdramInfo.len = length << 2;
}

void PrintSdramInfo(void)
{
	char *p;

	p = Image$$RO$$Base;
	tmPrintf("Image$$RO$$Base		= 0x%x\n", p);
	p = Image$$RO$$Limit;
	tmPrintf("Image$$RO$$Limit		= 0x%x\n\n", p);
	p = Image$$RW$$Base;
	tmPrintf("Image$$RW$$Base	= 0x%x\n", p);
	p = Image$$RW$$Limit;
	tmPrintf("Image$$RW$$Limit	= 0x%x\n\n", p);
	p = Image$$ZI$$Base;
	tmPrintf("Image$$ZI$$Base		= 0x%x\n", p);
	p = Image$$ZI$$Limit;
	tmPrintf("Image$$ZI$$Limit		= 0x%x\n\n", p);

	tmPrintf("info.flag			= %d\n", gSdramInfo.flag);
	tmPrintf("info.start_addr		= 0x%x\n", gSdramInfo.start_addr);
	tmPrintf("info.len			= 0x%x\n\n", gSdramInfo.len);
}

#endif
