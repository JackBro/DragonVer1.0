#include "basefunc938.h"

#ifndef	V938CMD_DISABLE

void V9B_Mp3SetClrA(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_CLR_A, val, BIT_8);
}

UINT16 V9B_Mp3GetRdPtr(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE2_MP3_RD_PTR_L, BIT_16_LTOH);

	return val;
}

UINT16 V9B_Mp3GetWrPtr(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE2_MP3_WR_PTR_L, BIT_16_LTOH);

	return val;
}

/** 0x0: 1/4      0x1: 1/2      0x2: 3/4 **/ 
void V9B_Mp3SetWMark(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE2_W_MARK, val, BIT_8);
}

UINT8 V9B_Mp3GetWMark(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_W_MARK, BIT_8);

	return val;
}

void  V9B_Mp3SetDatDone(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_MP3_DONE, val, BIT_8);
}

UINT8 V9B_Mp3GetDatDone(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_MP3_DONE, BIT_8);

	return val;
}

UINT32 V9B_Mp3GetPara(void)
{
	UINT32 val;

	val = (UINT32)V9_GetReg(V9_REG_PAGE2_MP3_PARA0, BIT_24_LTOH);

	return val;
}

void  V9B_Mp3SetEqBand(UINT8 *pVal)
{
	UINT8 i;
	UINT8 *pTmpVal = pVal; 

    for(i = 0; i < 10; i++)
	    V9_SetReg(V9_REG_PAGE2_MP3_EQBAND0 + i, *pTmpVal++, BIT_8);
}

void V9B_Mp3GetEqBand(UINT8 *pVal)
{
	UINT8 i;
	UINT8 *pTmpVal = pVal; 

	for(i = 0; i < 10; i++)
	    *pTmpVal++ = (UINT8)V9_GetReg(V9_REG_PAGE2_MP3_EQBAND0 + i, BIT_8);
}

void  V9B_Mp3SetVolStep(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_MP3_VOLSTEP, val, BIT_8);
}

UINT8 V9B_Mp3GetVolStep(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_MP3_VOLSTEP, BIT_8);

	return val;
}

void  V9B_Mp3WrMFifo(UINT8 *pDat, UINT16 wSize)
{
	UINT16 wTmpSize;

	wTmpSize = wSize >> 2;
	wTmpSize <<= 2;

	if(pDat != NULL)
	    V9_WriteSram(V9_REG_PAGE2_MFIFO, pDat, wTmpSize);
}


#endif


