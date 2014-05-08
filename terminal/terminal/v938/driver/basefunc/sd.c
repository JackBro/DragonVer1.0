#include "basefunc938.h"

#ifndef	V938CMD_DISABLE


void  V9B_SdSetCmd0Fifo(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE4_SD_CMD0_FIFO, val, BIT_8);
}

UINT8 V9B_SdGetCmd0Fifo(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE4_SD_CMD0_FIFO, BIT_8);

	return val;
}

void  V9B_SdSetCmd1Fifo(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE4_SD_CMD1_FIFO, val, BIT_8);
}

UINT8 V9B_SdGetCmd1Fifo(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE4_SD_CMD1_FIFO, BIT_8);

	return val;
}

void  V9B_SdSetDatFifo(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE4_SD_DATA_FIFO, val, BIT_8);
}

UINT8 V9B_SdGetDatFifo(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE4_SD_DATA_FIFO, BIT_8);

	return val;
}


#endif

