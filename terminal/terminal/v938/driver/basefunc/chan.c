#include "basefunc938.h"

#ifndef	V938CMD_DISABLE


void V9B_ChanSetChnStart(UINT8 *pVal)
{
	UINT8 i;

	for(i = 0; i < 8; i++)
        V9_SetReg(V9_REG_PAGE1_CHNSTART0, *pVal++, BIT_8);
}

void V9B_ChanGetChnStart(UINT8 *pVal)
{
	UINT8 i;
	UINT8 *pTmpVal = pVal;

	for(i = 0; i < 8; i++)
        *pTmpVal++ = (UINT8)V9_GetReg(V9_REG_PAGE1_CHNSTART0, BIT_8);
}

void V9B_ChanSetChnStop(UINT8 *pVal)
{
	UINT8 i;

	for(i = 0; i < 8; i++)
        V9_SetReg(V9_REG_PAGE1_CHNSTOP0, *pVal++, BIT_8);
}

void V9B_ChanGetChnStop(UINT8 *pVal)
{
	UINT8 i;
	UINT8 *pTmpVal = pVal;

	for(i = 0; i < 8; i++)
        *pTmpVal++ = (UINT8)V9_GetReg(V9_REG_PAGE1_CHNSTOP0, BIT_8);
}

void V9B_ChanGetChnDly(UINT8 *pVal)
{
	UINT8 i;
	UINT8 *pTmpVal = pVal;

	for(i = 0; i < 8; i++)
        *pTmpVal++ = (UINT8)V9_GetReg(V9_REG_PAGE1_CHNDLY0, BIT_8);
}

void V9B_ChanSetDlyEnd(UINT8 *pVal)
{
	UINT8 i;

	for(i = 0; i < 8; i++)
        V9_SetReg(V9_REG_PAGE1_DLYEND0, *pVal++, BIT_8);
}

void V9B_ChanSetOpMod(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE1_OP_MODE, val, BIT_8);
}

UINT8 V9B_ChanGetOpMod(void)
{
	UINT8 val;

    val = (UINT8)V9_GetReg(V9_REG_PAGE1_OP_MODE, BIT_8);

	return val;
}

#endif