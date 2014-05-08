#ifndef _HOSTAPI_H_
#define _HOSTAPI_H_

#include "../v568/tools/type.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef WIN32

int __inline ReadMemory4ByteAlign(int adr)
{
	return *(volatile int *)adr;
}

void __inline WriteMemory4ByteAlign(int adr, int val)
{
	*(volatile int *)adr = val;
}

#else

int __inline ReadMemory4ByteAlign(int adr)
{
    return adr;
}

void __inline WriteMemory4ByteAlign(int adr, int val)
{
    adr = val;
}

#endif  

    int ReadMemory(int adr);
	void WriteMemory(int adr, int val);

	void testDelay(int count);

	int WriteBuf(int adr, char *buf, int len);
	int ReadBuf(int adr, char *buf, int len);
	void AdjustHostMclk(int mclk);
	void AdjustI2sClk(int clk);

	void PrintMsg(char *str);

	int SetMemory(int adr, int mask, int op);

	extern char *gTransferBuf;


#ifdef __cplusplus
}
#endif

#endif



