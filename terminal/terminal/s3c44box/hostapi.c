
#include "internal.h"

//char gTransferBuf[MAX_TRANSBUF_SIZE];
#ifndef WIN32

extern char Image$$RW$$Limit[];
char *gTransferBuf = Image$$RW$$Limit + MALLOC_MAX_SIZE;

#else

#ifndef NCSC
char Image$$RW$$Limit[MAX_TRANSBUF_SIZE];
char *gTransferBuf = Image$$RW$$Limit;
#endif

#endif

int SetMemory(int adr, int mask, int op)
{
	int val;

	val = ReadMemory(adr);
	switch(op)
	{
	case AND_OPCODE:
		val &= mask;
		break;
	case OR_OPCODE:
		val |= mask;
		break;
	default:
		return val;
	}
	WriteMemory(adr, val);

	return val;
}


#ifdef WIN32

void PrintMsg(char *str)
{
//	SimHost_PrintMsg(str);
	SmPrintf("%s", str);
}

int WriteBuf(int adr, char *buf, int len)
{
	return SimHost_WriteBuf(adr, buf, len);
}

int ReadBuf(int adr, char *buf, int len)
{
	return SimHost_ReadBuf(adr, buf, len);
}

int ReadMemory(int adr)
{
	return SimHost_ReadMemory(adr);
}

void WriteMemory(int adr, int val)
{
	SimHost_WriteMemory(adr, val);
}

#else

void PrintMsg(char *str)
{
}

int WriteBuf(int adr, char *buf, int len)
{
	int size, actsize;

	actsize = len;
	buf = gTransferBuf + adr;
	while(len)
	{
		size = tmReadBuf(buf, len);
		buf += size;
		len -= size;
	}
	return actsize;
}

int ReadBuf(int adr, char *buf, int len)
{
	int size, i, leng;

	i = len / (MAX_TRANSBUF_SIZE - adr);
	while(i--)
	{
		buf = gTransferBuf + adr;
		leng = MAX_TRANSBUF_SIZE - adr;
		len -= leng;
		while(leng)
		{
			size = tmWriteBuf(buf, leng);
			buf += size;
			leng -= size;
		}
	}

	leng = len;
	buf = gTransferBuf + adr;
	while(len)
	{
		size = tmWriteBuf(buf, len);
		buf += size;
		len -= size;
	}
	return leng;
}

#ifndef NCSC

int ReadMemory(int adr)
{
	int locAdr, outOrderByte, val, val1;


	locAdr = (adr >> 2) << 2;
    outOrderByte = adr - locAdr;

	val  = ReadMemory4ByteAlign(locAdr);
	val1 = ReadMemory4ByteAlign(locAdr + 4);

	if(outOrderByte == 0)
	{
        val = val;
	}
	else if(outOrderByte == 1)
	{
        val = ((val >> 8) & 0xffffff) | ((val1 << 24) & 0xff000000);
		//val = ((val >> (outOrderByte * 8)) & ((0x1 << ((4 - outOrderByte) * 8)) - 1)) | 
		//	  ((val1 << ((4 - outOrderByte) * 8)) & (((0x1 << (outOrderByte * 8)) - 1) << ((4 - outOrderByte) * 8)) );
	}
	else if(outOrderByte == 2)
	{
        val = ((val >> 16) & 0xffff) | ((val1 << 16) & 0xffff0000);
	}
	else if(outOrderByte == 3)
	{
        val = ((val >> 24) & 0xff) | ((val1 << 8) & 0xffffff00);
	}

	return val;
}

void WriteMemory(int adr, int val)
{	
	int locAdr, outOrderByte, val1;


	locAdr = (adr >> 2) << 2;
    outOrderByte = adr - locAdr;

	if(outOrderByte == 0)
	{
		WriteMemory4ByteAlign(adr, val);
	}
	else if(outOrderByte == 1)
	{
		val1 = ReadMemory4ByteAlign(locAdr);	    
        val1 = ((val << 8) & 0xffffff00) | (val1 & 0xff);
		WriteMemory4ByteAlign(locAdr, val1);

		val1 = ReadMemory4ByteAlign(locAdr + 4);	    
        val1 = ((val >> 24) & 0xff) | (val1 & 0xffffff00);
		WriteMemory4ByteAlign(locAdr + 4, val1);
	}
	else if(outOrderByte == 2)
	{
        val1 = ReadMemory4ByteAlign(locAdr);	    
        val1 = ((val << 16) & 0xffff0000) | (val1 & 0xffff);
		WriteMemory4ByteAlign(locAdr, val1);

		val1 = ReadMemory4ByteAlign(locAdr + 4);	    
        val1 = ((val >> 16) & 0xffff) | (val1 & 0xffff0000);
		WriteMemory4ByteAlign(locAdr + 4, val1);
	}
	else if(outOrderByte == 3)
	{
        val1 = ReadMemory4ByteAlign(locAdr);	    
        val1 = ((val << 24) & 0xff000000) | (val1 & 0xffffff);
		WriteMemory4ByteAlign(locAdr, val1);

		val1 = ReadMemory4ByteAlign(locAdr + 4);	    
        val1 = ((val >> 8) & 0xffffff) | (val1 & 0xff000000);
		WriteMemory4ByteAlign(locAdr + 4, val1);
	}	
}

#endif  // NCSC

#endif  //WIN32

void AdjustHostMclk(int mclk)
{
	int baud0, baud1;

	baud0 = GetUartBaud(0);
	baud1 = GetUartBaud(1);
	SetHostMclk(mclk);
	SetUartBaud(0, baud0);
	SetUartBaud(1, baud1);
	mclk = GetI2sClk();
	SetI2sClk(mclk);
	WDTInit();
}

void AdjustI2sClk(int clk)
{
	//int mclk;

	if(clk < MIN_I2SCLK)
		clk = I2S_DEFAULT_CLK;
	//mclk = GetHostMclk();
	//mclk += clk - 1;
	//mclk /= clk;
	//mclk *= clk;
	//AdjustHostMclk(mclk);
	SetI2sClk(clk);
}

void testDelay(int count)
{
	while(count--)
	{
		Delay(1000);
		tmPrintf("%x\n", count);
	}
}


