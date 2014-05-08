#include "basefunc938.h"

#ifndef	V938CMD_DISABLE


void  V9B_EqSetBand0(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_EQBAND0, val, BIT_8);
}

UINT8 V9B_EqGetBand0(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_EQBAND0, BIT_8);

	return val;
}

void  V9B_EqSetBand1(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_EQBAND1, val, BIT_8);
}

UINT8 V9B_EqGetBand1(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_EQBAND1, BIT_8);

	return val;
}

void  V9B_EqSetBand2(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_EQBAND2, val, BIT_8);
}

UINT8 V9B_EqGetBand2(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_EQBAND2, BIT_8);

	return val;
}

void  V9B_EqSetBand3(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_EQBAND3, val, BIT_8);
}

UINT8 V9B_EqGetBand3(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_EQBAND3, BIT_8);

	return val;
}

void  V9B_EqSetBand4(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_EQBAND4, val, BIT_8);
}

UINT8 V9B_EqGetBand4(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_EQBAND4, BIT_8);

	return val;
}

void  V9B_EqSetBand5(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_EQBAND5, val, BIT_8);
}

UINT8 V9B_EqGetBand5(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_EQBAND5, BIT_8);

	return val;
}

/** 0x1: 48K HZ       0x0: 44.1K HZ**/
void  V9B_EqSetSampRat(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_SAMP_RATE, val, BIT_8);
}

UINT8 V9B_EqGetSampRat(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_SAMP_RATE, BIT_8);

	return val;
}

void  V9B_EqSetVolStep(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_VOLSTEP, val, BIT_8);
}

UINT8 V9B_EqGetVolStep(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_VOLSTEP, BIT_8);

	return val;
}

void  V9B_EqSetOutSel(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE2_OUTSEL, val, BIT_8);
}

UINT8 V9B_EqGetOutSel(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE2_OUTSEL, BIT_8);

	return val;
}

void  V9B_EqSetEqMod(void)
{
	V9_SetReg(V9_REG_PAGE2_OUTSEL, 0x0, BIT_8);
}

void  V9B_EqSetBypassMod(void)
{
	V9_SetReg(V9_REG_PAGE2_OUTSEL, 0x1, BIT_8);
}


#endif

