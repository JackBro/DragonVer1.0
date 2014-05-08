#include "basefunc938.h"

#ifndef	V938CMD_DISABLE


void V9B_AudioSetAdCtrl(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_ADCTRL, val, BIT_8);
}

UINT8 V9B_AudioGetAdCtrl(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_ADCTRL, BIT_8);

	return val;
}

void V9B_AudioSetAdpcmCmpRat(UINT8 cmpRat)
{
	UINT8 val;

    val =  (UINT8)V9_GetReg(V9_REG_PAGE1_ADCTRL, BIT_8);
	val &= 0xf3;
	val |= (cmpRat & 0x3) << 2;

	V9_SetReg(V9_REG_PAGE1_ADCTRL, val, BIT_8);
}

void V9B_AudioSetLaw(UINT8 law)
{
	UINT8 val;

    val =  (UINT8)V9_GetReg(V9_REG_PAGE1_ADCTRL, BIT_8);
	val &= 0xfc;
	val |= law & 0x3;

	V9_SetReg(V9_REG_PAGE1_ADCTRL, val, BIT_8);
}

void V9B_AudioSetVolMidL(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_VOLMID_L, val, BIT_8);
}

UINT8 V9B_AudioGetVolMidL(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_VOLMID_L, BIT_8);

	return val;
}

void V9B_AudioSetVolMidR(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_VOLMID_R, val, BIT_8);
}

UINT8 V9B_AudioGetVolMidR(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_VOLMID_R, BIT_8);

	return val;
}

void V9B_AudioSetVolWavL(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_VOLWAV_L, val, BIT_8);
}

UINT8 V9B_AudioGetVolWavL(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_VOLWAV_L, BIT_8);

	return val;
}

void V9B_AudioSetVolWavR(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_VOLWAV_R, val, BIT_8);
}

UINT8 V9B_AudioGetVolWavR(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_VOLWAV_R, BIT_8);

	return val;
}

void V9B_AudioSetHpnVolL(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_HPNVOL_L, val, BIT_8);
}

UINT8 V9B_AudioGetHpnVolL(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_HPNVOL_L, BIT_8);

	return val;
}

void V9B_AudioSetHpnVolR(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_HPNVOL_R, val, BIT_8);
}

UINT8 V9B_AudioGetHpnVolR(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_HPNVOL_R, BIT_8);

	return val;
}

void V9B_AudioSetDacPgg(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_DACPGG, val, BIT_8);
}

UINT8 V9B_AudioGetDacPgg(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_DACPGG, BIT_8);

	return val;
}

void V9B_AudioSetMutCtrl(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_MUTECTRL, val, BIT_8);
}

UINT8 V9B_AudioGetMutCtrl(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_MUTECTRL, BIT_8);

	return val;
}

void V9B_AudioSetEWMark(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_E_W_MARK, val, BIT_8);
}

UINT8 V9B_AudioGetEWMark(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_E_W_MARK, BIT_8);

	return val;
}

void V9B_AudioSetDTRetri(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_DT_RETRI, val, BIT_8);
}

UINT8 V9B_AudioGetDTRetri(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_DT_RETRI, BIT_8);

	return val;
}

void V9B_AudioSetReg000(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_REG000, val, BIT_8);
}

UINT8 V9B_AudioGetReg000(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_REG000, BIT_8);

	return val;
}

void V9B_AudioSetReg010(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_REG010, val, BIT_8);
}

UINT8 V9B_AudioGetReg010(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_REG010, BIT_8);

	return val;
}

void V9B_AudioSetIntpCoef(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE1_INTPCOEF0, val, BIT_16_LTOH);
}

UINT16 V9B_AudioGetIntpCoef(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE1_INTPCOEF0, BIT_16_LTOH);

	return val;
}

void V9B_AudioSetReg020(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_REG020, val, BIT_8);
}

UINT8 V9B_AudioGetReg020(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_REG020, BIT_8);

	return val;
}

void V9B_AudioSetECntInit2(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_ECNTINIT2, val, BIT_8);
}

UINT8 V9B_AudioGetECntInit2(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_ECNTINIT2, BIT_8);

	return val;
}

void V9B_AudioSetPreVal(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE1_PREVAL0, val, BIT_16_LTOH);
}

UINT16 V9B_AudioGetPreVal(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE1_PREVAL0, BIT_16_LTOH);

	return val;
}

void V9B_AudioSetReg111(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_REG111, val, BIT_8);
}

UINT8 V9B_AudioGetReg111(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_REG111, BIT_8);

	return val;
}

void V9B_AudioSetEvlpChg20(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_EVLPCHG20, val, BIT_8);
}

UINT8 V9B_AudioGetEvlpChg20(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_EVLPCHG20, BIT_8);

	return val;
}

void V9B_AudioSetLoadAddr(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_LOADADDR, val, BIT_8);
}

UINT8 V9B_AudioGetLoadAddr(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_LOADADDR, BIT_8);

	return val;
}

void V9B_AudioSetSFifoWm(UINT8 sFifoIdx, UINT8 val)
{
    V9_SetReg(V9_REG_PAGE1_SFIFO0_WM + sFifoIdx, val, BIT_8);
}

UINT8 V9B_AudioGetSFifoWm(UINT8 sFifoIdx)
{
	UINT8 val;

    val = (UINT8)V9_GetReg(V9_REG_PAGE1_SFIFO0_WM + sFifoIdx, BIT_8);

	return val;
}

void V9B_AudioSetSFifoAWB(UINT8 sFifoIdx, UINT16 val)
{
	V9_SetReg(V9_REG_PAGE1_SFIFO0_AW_BL + 4 * sFifoIdx, val, BIT_16_LTOH);
}

UINT16 V9B_AudioGetSFifoAWB(UINT8 sFifoIdx)
{
	UINT16 val;
    
	val = (UINT16)V9_GetReg(V9_REG_PAGE1_SFIFO0_AW_BL + 4 * sFifoIdx, BIT_16_LTOH);

	return val;
}

void V9B_AudioSetSFifoAWE(UINT8 sFifoIdx, UINT16 val)
{
    V9_SetReg(V9_REG_PAGE1_SFIFO0_AW_EL + 4 * sFifoIdx, val, BIT_16_LTOH);
}

UINT16 V9B_AudioGetSFifoAWE(UINT8 sFifoIdx)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE1_SFIFO0_AW_EL + 4 * sFifoIdx, BIT_16_LTOH);

	return val;
}

UINT16 V9B_AudioGetSFifoARB(UINT8 sFifoIdx)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE1_SFIFO0_AR_BL + 4 * sFifoIdx, BIT_16_LTOH);

	return val;
}

UINT16 V9B_AudioGetSFifoARE(UINT8 sFifoIdx)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE1_SFIFO0_AR_EL + 4 * sFifoIdx, BIT_16_LTOH);

	return val;
}


#endif