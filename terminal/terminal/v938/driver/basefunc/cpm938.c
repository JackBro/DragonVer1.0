#include "basefunc938.h"

#ifndef	V938CMD_DISABLE


void   V9B_CpmSetPmcCtrl(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_PMCTRL, val, BIT_8);
}

UINT8  V9B_CpmGetPmcCtrl(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_PMCTRL, BIT_8);

	return val;
}

void   V9B_CpmSetPaCfg(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_PACFG, val, BIT_8);
}

UINT8  V9B_CpmGetPaCfg(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_PACFG, BIT_8);

	return val;
}

void  V9B_CpmEnOverTemProt(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE0_PACFG, (UINT8)BIT4, OR_OPCODE);
}

void  V9B_CpmDisOverTemProt(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE0_PACFG, (UINT8)~BIT4, AND_OPCODE);
}

void  V9B_CpmEnPopCancel(void)
{
   V9B_SetRegMaskBit8(V9_REG_PAGE0_PACFG, (UINT8)BIT3, OR_OPCODE);
}

void  V9B_CpmDisPopCancel(void)
{
   V9B_SetRegMaskBit8(V9_REG_PAGE0_PACFG, (UINT8)~BIT3, AND_OPCODE);
}

void  V9B_CpmEnCurProt(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE0_PACFG, (UINT8)BIT2, OR_OPCODE);
}

void  V9B_CpmDisCurProt(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE0_PACFG, (UINT8)~BIT2, AND_OPCODE);
}

void  V9B_CpmEnFaston(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE0_PACFG, (UINT8)BIT1, OR_OPCODE);
}

void  V9B_CpmDisFaston(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE0_PACFG, (UINT8)~BIT1, AND_OPCODE);
}

void  V9B_CpmEnSlowOff(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE0_PACFG, (UINT8)BIT0, OR_OPCODE);
}

void  V9B_CpmDisSlowOff(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE0_PACFG, (UINT8)~BIT0, AND_OPCODE);
}

void   V9B_CpmSetDacCfg(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_DACCFG, val, BIT_8);
}

UINT8  V9B_CpmGetDacCfg(void)
{
	UINT8 val;

	val  = (UINT8)V9_GetReg(V9_REG_PAGE0_DACCFG, BIT_8);

	return val;
}

void   V9B_CpmSetDacDataSrcOnChip(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE0_DACCFG, (UINT8)~BIT6, AND_OPCODE);
}

void   V9B_CpmSetDacDataSrcOffChip(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE0_DACCFG, (UINT8)BIT6, OR_OPCODE);
}

/****************************************************/
/*           Set On-chip DAC_MCLK_FRQ               */
/*  0x0:Fs*128   0x1:Fs*256	0x2: Fs*384  0x3:Fs*512 */
/****************************************************/
void   V9B_CpmSetOnChipDacMclkFre(UINT8 fre)
{
    UINT8 val;

	val  = V9B_CpmGetDacCfg();
	val  &= 0xcf;
	val  |= fre << 0x4;
    V9B_CpmSetDacCfg(val);
}

void   V9B_CpmSetSpVolCtl(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_SPVOL_CTL, val, BIT_8);
}

UINT8  V9B_CpmGetSpVolCtl(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_SPVOL_CTL, BIT_8);

	return val;
}

void V9B_CpmEnUpdateIns(void)
{
    V9_SetReg(V9_REG_PAGE0_SPVOL_CTL, 0x1, BIT_8);
}

void V9B_CpmDisUpdateIns(void)
{
    V9_SetReg(V9_REG_PAGE0_SPVOL_CTL, 0x0, BIT_8);
}

void   V9B_CpmSetDacCtrl(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_DACCTRL, val, BIT_8);
}

UINT8  V9B_CpmGetDacCtrl(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_DACCTRL, BIT_8);

	return val;
}

void   V9B_CpmSetDacToTestMod(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE0_DACCTRL, (UINT8)BIT7, OR_OPCODE);
}

void   V9B_CpmSetDacToWorkMod(void)
{
   V9B_SetRegMaskBit8(V9_REG_PAGE0_DACCTRL, (UINT8)~BIT7, AND_OPCODE);
}

void   V9B_CpmEnDeepth(void)
{
    V9B_SetRegMaskBit8(V9_REG_PAGE0_DACCTRL, (UINT8)BIT6, OR_OPCODE);
}

void   V9B_CpmDisDeepth(void)
{
     V9B_SetRegMaskBit8(V9_REG_PAGE0_DACCTRL, (UINT8)~BIT6, AND_OPCODE);
}

/********************************************************/
/*                   Set IO word length                 */
/*  0b00:16 bits  0b01:18bits  0b10:20bits	0b11:24bits */
/********************************************************/
void   V9B_CpmSetIOWl(UINT8 iowl)
{
    UINT8 val;

	val  = V9B_CpmGetDacCtrl();
	val  &= 0xcf;
	val  |= iowl << 0x4;
    V9B_CpmSetDacCtrl(val);
}

void   V9B_CpmSetSpVol(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_SPVOL, val, BIT_8);
}

UINT8  V9B_CpmGetSpVol(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_SPVOL, BIT_8);

	return val;
}

void   V9B_CpmSetTestO(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_TEST_O, val, BIT_8);
}

UINT8  V9B_CpmGetTestO(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_TEST_O, BIT_8);

	return val;
}

UINT8  V9B_CpmGetStrap(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_STRAP, BIT_8);

	return val;
}

void   V9B_CpmSetCInfoCtrl(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE0_CINFO_REN_ADD, val, BIT_8);
}

UINT8  V9B_CpmGetCInfoCtrl(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_CINFO_REN_ADD, BIT_8);

	return val;
}

void   V9B_CpmEnPullUpResi(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE0_CINFO_REN_ADD, (UINT8)~BIT0, AND_OPCODE);
}

void   V9B_CpmDisPullUpResi(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE0_CINFO_REN_ADD, (UINT8)BIT0, OR_OPCODE);
}

UINT8  V9B_CpmGetCInfo(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_CINFO_ADD, BIT_8);

	return val;
}

void   V9B_CpmSetDummyReg(UINT32 val)
{
    V9_SetReg(V9_REG_PAGE0_DUMMY_REG0, val, BIT_32_LTOH);
}

UINT32  V9B_CpmGetDummyReg(void)
{
	UINT32 val;

	val = V9_GetReg(V9_REG_PAGE0_DUMMY_REG0, BIT_32_LTOH);

	return val;
}

void   V9B_CpmSetMiscCon(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_MISC_CON, val, BIT_8);
}

UINT8  V9B_CpmGetMiscCon(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_MISC_CON, BIT_8);

	return val;
}

void  V9B_CpmEnSleep(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE0_MISC_CON, (UINT8)~BIT2, AND_OPCODE);
}

void  V9B_CpmDisSleep(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE0_MISC_CON, (UINT8)BIT2, OR_OPCODE);
}

void  V9B_CpmEnIiSDat(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE0_MISC_CON, (UINT8)~BIT1, AND_OPCODE);
}

void  V9B_CpmDisIiSDat(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE0_MISC_CON, (UINT8)BIT1, OR_OPCODE);
}

void   V9B_CpmSetIisCfg(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_I2SCFG, val, BIT_8);
}

UINT8  V9B_CpmGetIiscCfg(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_I2SCFG, BIT_8);

	return val;
}

/****************************************************/
/*           dac_mclk is divided from MCLK          */
/* 0x1: 1/1     0x1: 1/2     0x2: 1/3      0x3: 1/4 */
/****************************************************/
void V9B_CpmSetOnChipMclkDiv(UINT8 divider2)
{
	UINT8 val;

	val = V9B_CpmGetIiscCfg();
	val &= 0x3f;
	val |= divider2 << 0x6;
	V9B_CpmSetIisCfg(val);
}

/****************************************************/
/*  dac_mclk is divided from off-chip I2S MCK input */
/* 0x1: 1/1     0x1: 1/2     0x2: 1/3      0x3: 1/4 */
/****************************************************/
void V9B_CpmSetOffChipMclkDiv(UINT8 divider3)
{
	UINT8 val;

	val = V9B_CpmGetIiscCfg();
	val &= 0xcf;
	val |= divider3 << 0x4;
	V9B_CpmSetIisCfg(val);
}

/**************************************************/
/* 0x0: slave mode, VC0938 is an I2S receiver     */
/* 0x1: master mode, VC0938 is an I2S transmitter */
/**************************************************/
void V9B_CpmSetIiSMod(UINT8 iisMod)
{
	UINT8 val;

	val = V9B_CpmGetIiscCfg();
	val &= 0xfb;
	val |= iisMod << 0x2;
	V9B_CpmSetIisCfg(val);
}

/**************************************************/ 
/*          0x0: from eternal I2S                 */
/*          0x1:output MCK to external I2S        */
/**************************************************/
void V9B_CpmSetIiSMclkDir(UINT8 mclkDir)
{
	UINT8 val;

	val = V9B_CpmGetIiscCfg();
	val &= 0xfd;
	val |= mclkDir << 0x1;
	V9B_CpmSetIisCfg(val);
}

/**************************************************/ 
/*         0x0: normal;		0x1: inverted         */
/**************************************************/
void V9B_CpmSetIiSSclkPoly(UINT8 sclkPoly)
{
	UINT8 val;

	val = V9B_CpmGetIiscCfg();
	val &= 0xfe;
	val |= sclkPoly;
	V9B_CpmSetIisCfg(val);
}

void   V9B_CpmSetGpioFuncSel(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_FUNC_SEL, val, BIT_8);
}

UINT8  V9B_CpmGetGpioFuncSel(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_FUNC_SEL, BIT_8);

	return val;
}

/**************************************************/ 
/*  0x0: P0[3]-P0[0] in PIO mode                  */
/*	0x1: P0[3]-P0[0] server as Led and MTR output */
/*	0x2: P0[3]-P0[0] serve as I2S interface       */
/*	0x8: P0[3]-P0[0] serve as PA test output      */
/*	Others:  reserve                              */
/**************************************************/
void V9B_CpmSetP0FuncMod(UINT8 funcMod)
{
	UINT8 val;

	val = V9B_CpmGetGpioFuncSel();
	val &= 0xf0;
	val |= funcMod;
	V9B_CpmSetGpioFuncSel(val);
}

UINT8  V9B_CpmGetSrd(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_SRD, BIT_8);

	return val;
}

void   V9B_CpmSetUmCtrl(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE0_UM_CTRL, val, BIT_8);
}

UINT8  V9B_CpmGetUmCtrl(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_UM_CTRL, BIT_8);

	return val;
}

void   V9B_CpmSetGpioCfg(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_GPIO_CFG, val, BIT_8);
}

UINT8  V9B_CpmGetGpioCfg(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_GPIO_CFG, BIT_8);

	return val;
}

void   V9B_CpmSetP0Mod(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_P0_MODE, val, BIT_8);
}

UINT8  V9B_CpmGetP0Mod(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_P0_MODE, BIT_8);

	return val;
}

void   V9B_CpmSetP0Dir(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_P0_DIR, val, BIT_8);
}

UINT8  V9B_CpmGetP0Dir(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_P0_DIR, BIT_8);

	return val;
}

void   V9B_CpmSetPort0(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_PORT0, val, BIT_8);
}

UINT8  V9B_CpmGetPort0(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_PORT0, BIT_8);

	return val;
}

void   V9B_CpmSetDatBusCtrl(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE0_D_REN, val, BIT_8);
}

UINT8  V9B_CpmGetDatBusCtrl(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_D_REN, BIT_8);

	return val;
}

void   V9B_CpmSetGpioCtrl(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_GPIO_REN, val, BIT_8);
}

UINT8  V9B_CpmGetGpioCtrl(void)
{
    UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_GPIO_REN, BIT_8);

	return val;
}

void   V9B_CpmSetMiscTest(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE0_MISC_TEST, val, BIT_8);
}

UINT8  V9B_CpmGetMiscTest(void)
{
    UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_MISC_TEST, BIT_8);

	return val;
}

void   V9B_CpmSetClkCtrl0(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_CLKCTRL0, val, BIT_8);
}

UINT8  V9B_CpmGetClkCtrl0(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_CLKCTRL0, BIT_8);

	return val;
}

void   V9B_CpmSetClkCtrl1(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_CLKCTRL1, val, BIT_8);
}

UINT8  V9B_CpmGetClkCtrl1(void)
{
    UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_CLKCTRL1, BIT_8);

	return val;
}

void   V9B_CpmSetClkCtrl2(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_CLKCTRL2, val, BIT_8);
}

UINT8  V9B_CpmGetClkCtrl2(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_CLKCTRL2, BIT_8);

	return val;
}

void   V9B_CpmSetClkCtrl3(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_CLKCTRL3, val, BIT_8);
}

UINT8  V9B_CpmGetClkCtrl3(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_CLKCTRL3, BIT_8);

	return val;
}

void   V9B_CpmSetClkCtrl4(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE0_CLKCTRL4, val, BIT_8);
}

UINT8  V9B_CpmGetClkCtrl4(void)
{
    UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE0_CLKCTRL4, BIT_8);

	return val;
}

UINT16  V9B_CpmGetChip(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE1_CHIP_L, BIT_16_LTOH);

	return val;
}

void   V9B_CpmSetGlbCtrl(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE1_GLBCTRL, val, BIT_8);
}

UINT8  V9B_CpmGetGlbCtrl(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_GLBCTRL, BIT_8);

	return val;
}

void   V9B_CpmStartSeq(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)BIT7, OR_OPCODE);
}

void   V9B_CpmStopSeq(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)~BIT7, AND_OPCODE);
}

void   V9B_CpmStartTrmTimer(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)BIT6, OR_OPCODE);
}

void   V9B_CpmStopTrmTimer(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)~BIT6, AND_OPCODE);
}

void   V9B_CpmPauWavSyn(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)BIT5, OR_OPCODE);
}

void   V9B_CpmResuWavSyn(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)~BIT5, AND_OPCODE);
}

void   V9B_CpmSetMonoOut(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)~BIT4, AND_OPCODE);
}

void   V9B_CpmSetStereoOut(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)BIT4, OR_OPCODE);
}

void   V9B_CpmEnSamRatConv(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)~BIT3, AND_OPCODE);
}

void   V9B_CpmDisSamRatConv(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)BIT3, OR_OPCODE);
}

void   V9B_CpmEnAllChanFM(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)~BIT2, AND_OPCODE);
}

void   V9B_CpmDisAllChanFM(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)BIT2, OR_OPCODE);
}

void   V9B_CpmEnAllChanAM(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)~BIT1, AND_OPCODE);
}

void   V9B_CpmDisAllChanAM(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)BIT1, OR_OPCODE);
}

void   V9B_CpmStartSTRMTimer(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)BIT0, OR_OPCODE);
}

void   V9B_CpmStopSTRMTimer(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCTRL, (UINT8)~BIT0, AND_OPCODE);
}

/* Read to clear */
UINT8  V9B_CpmGetStopAll(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_STOPALL, BIT_8);

	return val;
}

void   V9B_CpmSetWsMisc(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_WS_MISC, val, BIT_8);
}

UINT8  V9B_CpmGetWsMisc(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_WS_MISC, BIT_8);

	return val;
}

void   V9B_CpmEnSyncOut(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_WS_MISC, (UINT8)BIT0, OR_OPCODE);
}

void   V9B_CpmDisSyncOut(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_WS_MISC, (UINT8)~BIT0, AND_OPCODE);
}

void   V9B_CpmSetGlbCfg(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_GLBCFG, val, BIT_8);
}

UINT8  V9B_CpmGetGlbCfg(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_GLBCFG, BIT_8);

	return val;
}

void   V9B_CpmEnVolUpdateTst(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCFG, (UINT8)BIT4, OR_OPCODE);
}

void   V9B_CpmDisVolUpdateTst(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCFG, (UINT8)~BIT4, AND_OPCODE);
}

void   V9B_CpmEnVolUpdateInst(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCFG, (UINT8)BIT1, OR_OPCODE);
}

void   V9B_CpmDisVolUpdateInst(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLBCFG, (UINT8)~BIT1, AND_OPCODE);
}

/*  0x0: Fs*512 	0x1: Fs*768 */
void   V9B_CpmSetWsCntSel(UINT8 sel)
{
    UINT8 val;

	val = V9B_CpmGetGlbCfg();
	val &= ~BIT0;
	val |= sel;
    V9B_CpmSetGlbCfg(val);
}

/********** some functions maybe define in other c file ***********/

UINT32  V9B_CpmGetTmr(void)
{
	UINT32 val;

	val = V9_GetReg(V9_REG_PAGE1_TMR_L, BIT_24_LTOH);

	return val;
}

void   V9B_CpmSetTmrTgt(UINT32 val)
{
	V9_SetReg(V9_REG_PAGE1_TMRTGT_L, val, BIT_24_LTOH);
}

UINT32  V9B_CpmGetTmrTgt(void)
{
	UINT32 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_TMRTGT_L, BIT_24_LTOH);

	return val;
}

UINT8  V9B_CpmGetSTmr(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_STMR, BIT_8);

	return val;
}

void   V9B_CpmSetSTmrTgt(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_STMRTGT, val, BIT_8);
}

UINT8  V9B_CpmGetSTmrTgt(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_STMRTGT, BIT_8);

	return val;
}

void   V9B_CpmSetTmrBase(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE1_TMRBASE_L, val, BIT_16_LTOH);
}

UINT16  V9B_CpmGetTmrBase(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE1_TMRBASE_L, BIT_16_LTOH);

	return val;
}

void   V9B_CpmSetFRatioDlt(UINT32 val)
{
	V9_SetReg(V9_REG_PAGE1_FRATIO_DLT0, val, BIT_32_LTOH);
}

UINT32  V9B_CpmGetFRatioDlt(void)
{
	UINT32 val;

	val = V9_GetReg(V9_REG_PAGE1_FRATIO_DLT0, BIT_32_LTOH);

	return val;
}

/* Analog error control */
/* Write 1 to clear */
void   V9B_CpmSetAnaErrFlg(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_ANAERRFLAG, val, BIT_8);
}

UINT8  V9B_CpmGetAnaErrFlg(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_ANAERRFLAG, BIT_8);

	return val;
}

void   V9B_CpmSetAnaErrMsk(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_ANAERRMASK, val, BIT_8);
}

UINT8  V9B_CpmGetAnaErrMsk(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_ANAERRMASK, BIT_8);

	return val;
}

/* Write 1 to clear */
void   V9B_CpmSetErrFlg(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_ERRORFLAG, val, BIT_8);
}

UINT8  V9B_CpmGetErrFlg(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_ERRORFLAG, BIT_8);

	return val;
}

void   V9B_CpmSetErrMsk(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_ERRORMASK, val, BIT_8);
}

UINT8  V9B_CpmGetErrMsk(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_ERRORMASK, BIT_8);

	return val;
}

void   V9B_CpmSetSwReset(UINT8 uType)
{
    if(uType <= SW_RESET_GLOBAL)
        V9_SetReg(V9_REG_PAGE1_SWRST1, (0x1 << uType), BIT_8);
	else if(uType <= SW_RESET_LED)
		V9_SetReg(V9_REG_PAGE1_SWRST2, (0x1 << (uType - SW_RESET_MP3)), BIT_8);
	else if(uType <= SW_RESET_SFIFO3)
		V9_SetReg(V9_REG_PAGE1_SWRST3, (0x1 << (uType - SW_RESET_SFIFO0)), BIT_8);
}

UINT16  V9B_CpmGetVer(void)
{
    UINT16 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_VERSION_L, BIT_16_LTOH);

	return val;
}

void   V9B_CpmSetIntCtrl(UINT8 val)
{
    V9_SetReg(V9_REG_PAGE1_INTCTRL, val, BIT_8);
}

UINT8  V9B_CpmGetIntCtrl(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_INTCTRL, BIT_8);

	return val;
}

void   V9B_CpmSetIntEn0(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_INTEN0, val, BIT_8);
}

UINT8  V9B_CpmGetIntEn0(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_INTEN0, BIT_8);

	return val;
}

void   V9B_CpmSetIntFlg0(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_INTFLAG0, val, BIT_8);
}

UINT8  V9B_CpmGetIntFlg0(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_INTFLAG0, BIT_8);

	return val;
}

void   V9B_CpmSetIntEn1(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_INTEN1, val, BIT_8);
}

UINT8  V9B_CpmGetIntEn1(void)
{
    UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_INTEN1, BIT_8);

	return val;
}

void   V9B_CpmSetIntFlg1(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_INTFLAG1, val, BIT_8);
}

UINT8  V9B_CpmGetIntFlg1(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_INTFLAG1, BIT_8);

	return val;
}

void   V9B_CpmSetIntEn2(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_INTEN2, val, BIT_8);
}

UINT8  V9B_CpmGetIntEn2(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_INTEN2, BIT_8);

	return val;
}

void   V9B_CpmSetIntFlg2(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_INTFLAG2, val, BIT_8);
}

UINT8  V9B_CpmGetIntFlg2(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_INTFLAG2, BIT_8);

	return val;
}

void   V9B_CpmSetIntLock(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_INTLOCK, val, BIT_8);
}

UINT8  V9B_CpmGetIntLock(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_INTLOCK, BIT_8);

	return val;
}

void   V9B_CpmSetIntServ(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_INTSERV, val, BIT_8);
}

UINT8  V9B_CpmGetIntServ(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_INTSERV, BIT_8);

	return val;
}

void   V9B_CpmSetIntDlyCnt(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE1_INT_DLY_CNT0, val, BIT_16_LTOH);
}

UINT16  V9B_CpmGetIntDlyCnt(void)
{
    UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE1_INT_DLY_CNT0, BIT_16_LTOH);

	return val;
}

void   V9B_CpmSetIntDlySel(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_DLY_SEL, val, BIT_8);
}

UINT8  V9B_CpmGetIntDlySel(void)
{
    UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_DLY_SEL, BIT_8);

	return val;
}

void V9B_CpmEnIntDly(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_DLY_SEL, (UINT8)~BIT0, AND_OPCODE);
}

void V9B_CpmDisIntDly(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_DLY_SEL, (UINT8)BIT0, OR_OPCODE);
}
void   V9B_CpmSetSwInt(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_SW_INT, val, BIT_8);
}

UINT8  V9B_CpmGetSwInt(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_SW_INT, BIT_8);

	return val;
}

void   V9B_CpmSetAutoStopEn(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_AUTOSTOPEN, val, BIT_8);
}

UINT8  V9B_CpmGetAutoStopEn(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_AUTOSTOPEN, BIT_8);

	return val;
}

/* input: bit sequence number, from 0 to 3 */
void   V9B_CpmEnAutoStopEn(UINT8 seqNum)
{
	UINT8 val;

	val = V9B_CpmGetAutoStopEn();
	val |= 0x1 << seqNum; 
	V9B_CpmSetAutoStopEn(val);
}

/* input: bit sequence number, from 0 to 3 */
void   V9B_CpmDisAutoStopEn(UINT8 seqNum)
{
	UINT8 val;

	val = V9B_CpmGetAutoStopEn();
	val &= ~(0x1 << seqNum); 
	V9B_CpmSetAutoStopEn(val);
}

void   V9B_CpmSetRepeatIntCnt(UINT16 val)
{
	V9_SetReg(V9_REG_PAGE1_REPEAT_INT_CNT0, val, BIT_16_LTOH);
}

UINT16  V9B_CpmGetRepeatIntCnt(void)
{
	UINT16 val;

	val = (UINT16)V9_GetReg(V9_REG_PAGE1_REPEAT_INT_CNT0, BIT_16_LTOH);

	return val;
}

void   V9B_CpmSetGlbModSel(UINT8 val)
{
	V9_SetReg(V9_REG_PAGE1_GLB_MOD_SEL, val, BIT_8);
}

UINT8  V9B_CpmGetGlbModSel(void)
{
	UINT8 val;

	val = (UINT8)V9_GetReg(V9_REG_PAGE1_GLB_MOD_SEL, BIT_8);

	return val;
}

/* 0 is write2clear        1 is read2clear */
void   V9B_CpmSetIntFlgClrType(UINT8 clrType)
{
	UINT8 val;

	val = V9B_CpmGetGlbModSel();
	val &= ~BIT0;
	val |= clrType;
    V9B_CpmSetGlbModSel(val);
}

void   V9B_CpmEnIntRepeat(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLB_MOD_SEL, (UINT8)~BIT1, AND_OPCODE);
}

void   V9B_CpmDisIntRepeat(void)
{
	V9B_SetRegMaskBit8(V9_REG_PAGE1_GLB_MOD_SEL, (UINT8)BIT1, OR_OPCODE);
}

/* 0 is write2clear        1 is read2clear */
void   V9B_CpmSetCmdStatusClrType(UINT8 clrType)
{
	UINT8 val;

	val = V9B_CpmGetGlbModSel();
	val &= ~BIT2;
	val |= clrType;
    V9B_CpmSetGlbModSel(val);
}

/* 0 is write2clear        1 is read2clear */
void   V9B_CpmSetStopAllClrType(UINT8 clrType)
{
	UINT8 val;

	val = V9B_CpmGetGlbModSel();
	val &= ~BIT3;
	val |= clrType;
    V9B_CpmSetGlbModSel(val);
}


#endif

