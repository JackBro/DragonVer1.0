#include "basefunc938.h"

#ifndef	V938CMD_DISABLE


void V9B_FifoWrEFifo(UINT8 *pDat, UINT16 wSize)
{
	UINT16 wTmpSize;

	wTmpSize = wSize >> 2;
	wTmpSize <<= 2;

	if(pDat != NULL)
	    V9_WriteSram(V9_REG_PAGE1_EFIFO, pDat, wTmpSize);
}

void V9B_FifoWrSFifo(UINT8 *pDat, UINT16 wSize)
{
	UINT16 wTmpSize;

	wTmpSize = wSize >> 2;
	wTmpSize <<= 2;

	if(pDat != NULL)
	    V9_WriteSram(V9_REG_PAGE1_SFIFO, pDat, wTmpSize);
}

void V9B_FifoWrCFifo(UINT8 *pDat, UINT16 wSize)
{
	UINT16 wTmpSize;

	wTmpSize = wSize >> 2;
	wTmpSize <<= 2;

	if(pDat != NULL)
	    V9_WriteSram(V9_REG_PAGE1_CFIFO, pDat, wTmpSize);
}

/* Bit3: Stream channel 3 buffer0 full     Bit2: Stream channel 1 buffer0 full */
/* Bit1: Stream channel 2 buffer0 full     Bit0: Stream channel 0 buffer0 full */
void V9B_FifoSetSBuf0Ful(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE1_SBUF0_FULL, val, BIT_8);
}

UINT8 V9B_FifoGetSBuf0Ful(void)
{
	UINT8 val;

    val = (UINT8)V9_GetReg(V9_REG_PAGE1_SBUF0_FULL, BIT_8);

	return val;
}

/* Bit3: Stream channel 3 buffer1 full     Bit2: Stream channel 1 buffer1 full */
/* Bit1: Stream channel 2 buffer1 full     Bit0: Stream channel 0 buffer1 full */
void V9B_FifoSetSBuf1Ful(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE1_SBUF1_FULL, val, BIT_8);
}

UINT8 V9B_FifoGetSBuf1Ful(void)
{
	UINT8 val;

    val = (UINT8)V9_GetReg(V9_REG_PAGE1_SBUF1_FULL, BIT_8);

	return val;
}


#endif