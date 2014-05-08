#include "basefunc938.h"

#ifndef	V938CMD_DISABLE


void V9B_ConvEnSelConv(void)
{
	V9_SetReg(V9_REG_PAGE1_SEL_CONV, 0x1, BIT_8);
}

void V9B_ConvDisSelConv(void)
{
	V9_SetReg(V9_REG_PAGE1_SEL_CONV, 0x0, BIT_8);
}

/*               Bufer1 water mark                   */
/* 0x0: 1/2 empty    0x1: 3/4 empty   0x2: 1/4 empty */
void V9B_ConvSetBuf1WMark(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE1_BUF1_W_MARK, val, BIT_8);
}

UINT8 V9B_ConvGetBuf1WMark(void)
{
	UINT8 val;

    val = (UINT8)V9_GetReg(V9_REG_PAGE1_BUF1_W_MARK, BIT_8);
	
	return val;
}

void V9B_ConvSetBuf1Addr(UINT16 val)
{
    V9_SetReg(V9_REG_PAGE1_BUF1_ADDR_L, val, BIT_16_LTOH);
}

UINT16 V9B_ConvGetBuf1Addr(void)
{
	UINT16 val;

    val = (UINT16)V9_GetReg(V9_REG_PAGE1_BUF1_ADDR_L, BIT_16_LTOH);
	
	return val;
}

void V9B_ConvSetBuf2Addr(UINT16 val)
{
    V9_SetReg(V9_REG_PAGE1_BUF2_ADDR_L, val, BIT_16_LTOH);
}

UINT16 V9B_ConvGetBuf2Addr(void)
{
	UINT16 val;

    val = (UINT16)V9_GetReg(V9_REG_PAGE1_BUF2_ADDR_L, BIT_16_LTOH);
	
	return val;
}

void V9B_ConvSetEEvtTyp(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE1_E_EVENT_TYPE, val, BIT_8);
}

UINT8 V9B_ConvGetEEvtTyp(void)
{
	UINT8 val;

    val = (UINT8)V9_GetReg(V9_REG_PAGE1_E_EVENT_TYPE, BIT_8);

	return val;
}

void V9B_ConvSetCEvtTyp(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE1_C_EVENT_TYPE, val, BIT_8);
}

UINT8 V9B_ConvGetCEvtTyp(void)
{
	UINT8 val;

    val = (UINT8)V9_GetReg(V9_REG_PAGE1_C_EVENT_TYPE, BIT_8);

	return val;
}

UINT16 V9B_ConvGetBufFlg(void)
{
	UINT16 val;

    val = (UINT16)V9_GetReg(V9_REG_PAGE1_BUFFLAG0, BIT_16_LTOH);

	return val;
}

void V9B_ConvSetDlyTim(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE1_DLYTIME, val, BIT_8);
}

UINT8 V9B_ConvGetDlyTim(void)
{
	UINT8 val;

    val = (UINT8)V9_GetReg(V9_REG_PAGE1_DLYTIME, BIT_8);

	return val;
}

void V9B_ConvEnSBack(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE1_CHNCTRL, (UINT8)BIT0, OR_OPCODE);
}

void V9B_ConvDisSBack(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE1_CHNCTRL, (UINT8)~BIT0, AND_OPCODE);
}

void V9B_ConvEnStream(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE1_CHNCTRL, (UINT8)BIT1, OR_OPCODE);
}

void V9B_ConvDisStream(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE1_CHNCTRL, (UINT8)~BIT1, AND_OPCODE);
}

void V9B_ConvSetBuf2CmdAddr(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE1_BUF2_CMD_ADDR_L, val, BIT_16_LTOH);
}

UINT16 V9B_ConvGetBuf2CmdAddr(void)
{
	UINT16 val;

    val = (UINT16)V9_GetReg(V9_REG_PAGE1_BUF2_CMD_ADDR_L, BIT_16_LTOH);

	return val;
}

void V9B_ConvSetV8051Addr(UINT32 val)
{
    V9_SetReg(V9_REG_PAGE1_V8051_ADDR_L, (UINT16)(val & 0xffff), BIT_16_LTOH);
	V9_SetReg(V9_REG_PAGE1_V8051_ADDR_HH, (UINT8)((val & 0xff0000) >> 16), BIT_8);
}

UINT32 V9B_ConvGetV8051Addr(void)
{
	UINT8 adrHH;
	UINT16 adrLH;

    adrLH = (UINT16)V9_GetReg(V9_REG_PAGE1_V8051_ADDR_L, BIT_16_LTOH);
	adrHH = (UINT8) V9_GetReg(V9_REG_PAGE1_V8051_ADDR_L, BIT_8);

	return ((UINT32)adrLH & 0xffff) | (((UINT32)adrLH & 0xff) << 16);
}

/* Bit 0: Command Finished            Bit 1: Command failed */
UINT8 V9B_ConvGetCmdSta(void)
{
	UINT8 val;

	val = (UINT8) V9_GetReg(V9_REG_PAGE1_CMDSTATUS, BIT_8);

	return val;
}

UINT32 V9B_ConvGetCmdDat(void)
{
	UINT32 val;

	val = V9_GetReg(V9_REG_PAGE1_CMDATA0, BIT_32_LTOH);

	return val;
}

void V9B_ConvEnStartLoad(void)
{
    V9_SetReg(V9_REG_PAGE1_STARTLOAD, 0x1, BIT_8);
}

void V9B_ConvDisStartLoad(void)
{
    V9_SetReg(V9_REG_PAGE1_STARTLOAD, 0x0, BIT_8);
}

UINT8 V9B_ConvGetLoadDone(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_LOADDONE, BIT_8);

	return val;
}

UINT16 V9B_ConvGetEFifoRmn(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE1_EFIFO_RMN_L, BIT_16_LTOH);

	return val;
}

void V9B_ConvSetEfifoEptTT(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_EFIFO_EPT_TT, val, BIT_8);
}

UINT8 V9B_ConvGetEfifoEptTT(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_EFIFO_EPT_TT, BIT_8);

	return val;
}

void V9B_ConvSetVolGainL(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_VOL_GAIN_L, val, BIT_8);
}

UINT8 V9B_ConvGetVolGainL(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_VOL_GAIN_L, BIT_8);

	return val;
}

void V9B_ConvSetVolGainR(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_VOL_GAIN_R, val, BIT_8);
}

UINT8 V9B_ConvGetVolGainR(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_VOL_GAIN_R, BIT_8);

	return val;
}


#endif

