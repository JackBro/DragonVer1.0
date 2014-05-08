#ifndef _RDK_CPM938_H_
#define _RDK_CPM938_H_

#ifdef __cplusplus
extern "C" {
#endif


	//software reset
	enum CPM_SWRESETenum
	{
		//SWRST1 register
		SW_RESET_STMR	    		= 0x0,
		SW_RESET_TMR,
		SW_RESET_EQ,
		SW_RESET_SEQ,
		SW_RESET_WS,
		SW_RESET_SFIFOCLR,
		SW_RESET_EFIFOCLR,
		SW_RESET_GLOBAL,
		
		//SWRST2 register
		SW_RESET_MP3,
		SW_RESET_VIBRATOR,
		SW_RESET_CMDFIFO,
		SW_RESET_BIUMCLK,
		SW_RESET_INT,
		SW_RESET_SYS,
		SW_RESET_I2S,
		SW_RESET_LED,

		//SWRST3 register
		SW_RESET_SFIFO0,
		SW_RESET_SFIFO1,
		SW_RESET_SFIFO2,
		SW_RESET_SFIFO3
	};

	//Interrupt serve 
	enum CPM_INTSERVenum
	{
		INTSERV_EFIFO  	    		    = BIT1,
		INTSERV_MP3_EMPTY	    		= BIT2,
		INTSERV_MP3_INT  	    		= BIT3,
		INTSERV_SFIFO0_INT 	    		= BIT4,
		INTSERV_SFIFO1_INT 	    		= BIT5,
		INTSERV_SFIFO2_INT 	    		= BIT6,
		INTSERV_SFIFO3_INT 	    		= BIT7
	};


void   V9B_CpmSetPmcCtrl(UINT8 val);
UINT8  V9B_CpmGetPmcCtrl(void);

void   V9B_CpmSetPaCfg(UINT8 val);
UINT8  V9B_CpmGetPaCfg(void);
void  V9B_CpmEnOverTemProt(void);
void  V9B_CpmDisOverTemProt(void);
void  V9B_CpmEnPopCancel(void);
void  V9B_CpmDisPopCancel(void);
void  V9B_CpmEnCurProt(void);
void  V9B_CpmDisCurProt(void);
void  V9B_CpmEnFaston(void);
void  V9B_CpmDisFaston(void);
void  V9B_CpmEnSlowOff(void);
void  V9B_CpmDisSlowOff(void);

void   V9B_CpmSetDacCfg(UINT8 val);
UINT8  V9B_CpmGetDacCfg(void);
void   V9B_CpmSetDacDataSrcOnChip(void);
void   V9B_CpmSetDacDataSrcOffChip(void);
void   V9B_CpmSetOnChipDacMclkFre(UINT8 fre);

void   V9B_CpmSetSpVolCtl(UINT8 val);
UINT8  V9B_CpmGetSpVolCtl(void);
void V9B_CpmEnUpdateIns(void);
void V9B_CpmDisUpdateIns(void);

void   V9B_CpmSetDacCtrl(UINT8 val);
UINT8  V9B_CpmGetDacCtrl(void);
void   V9B_CpmSetDacToTestMod(void);
void   V9B_CpmSetDacToWorkMod(void);
void   V9B_CpmEnDeepth(void);
void   V9B_CpmDisDeepth(void);
void   V9B_CpmSetIOWl(UINT8 iowl);

void   V9B_CpmSetSpVol(UINT8 val);
UINT8  V9B_CpmGetSpVol(void);
void   V9B_CpmSetTestO(UINT8 val);
UINT8  V9B_CpmGetTestO(void);
UINT8  V9B_CpmGetStrap(void);

void   V9B_CpmSetCInfoCtrl(UINT8 val);
UINT8  V9B_CpmGetCInfoCtrl(void);
void   V9B_CpmEnPullUpResi(void);
void   V9B_CpmDisPullUpResi(void);

UINT8  V9B_CpmGetCInfo(void);

void   V9B_CpmSetDummyReg(UINT32 val);
UINT32 V9B_CpmGetDummyReg(void);

void   V9B_CpmSetMiscCon(UINT8 val);
UINT8  V9B_CpmGetMiscCon(void);
void  V9B_CpmEnSleep(void);
void  V9B_CpmDisSleep(void);
void  V9B_CpmEnIiSDat(void);
void  V9B_CpmDisIiSDat(void);

void   V9B_CpmSetIisCfg(UINT8 val);
UINT8  V9B_CpmGetIiscCfg(void);
void V9B_CpmSetOnChipMclkDiv(UINT8 divider2);
void V9B_CpmSetOffChipMclkDiv(UINT8 divider3);
void V9B_CpmSetIiSMod(UINT8 i2sMod);
void V9B_CpmSetIiSMclkDir(UINT8 mclkDir);
void V9B_CpmSetIiSSclkPoly(UINT8 sclkPoly);

void   V9B_CpmSetGpioFuncSel(UINT8 val);
UINT8  V9B_CpmGetGpioFuncSel(void);
void V9B_CpmSetP0FuncMod(UINT8 funcMod);

UINT8  V9B_CpmGetSrd(void);
void   V9B_CpmSetUmCtrl(UINT8 val);
UINT8  V9B_CpmGetUmCtrl(void);

void   V9B_CpmSetGpioCfg(UINT8 val);
UINT8  V9B_CpmGetGpioCfg(void);
void   V9B_CpmSetP0Mod(UINT8 val);
UINT8  V9B_CpmGetP0Mod(void);
void   V9B_CpmSetP0Dir(UINT8 val);
UINT8  V9B_CpmGetP0Dir(void);
void   V9B_CpmSetPort0(UINT8 val);
UINT8  V9B_CpmGetPort0(void);
void   V9B_CpmSetDatBusCtrl(UINT8 val);
UINT8  V9B_CpmGetDatBusCtrl(void);
void   V9B_CpmSetGpioCtrl(UINT8 val);
UINT8  V9B_CpmGetGpioCtrl(void);
void   V9B_CpmSetMiscTest(UINT8 val);
UINT8  V9B_CpmGetMiscTest(void);

void   V9B_CpmSetClkCtrl0(UINT8 val);
UINT8  V9B_CpmGetClkCtrl0(void);
void   V9B_CpmSetClkCtrl1(UINT8 val);
UINT8  V9B_CpmGetClkCtrl1(void);
void   V9B_CpmSetClkCtrl2(UINT8 val);
UINT8  V9B_CpmGetClkCtrl2(void);
void   V9B_CpmSetClkCtrl3(UINT8 val);
UINT8  V9B_CpmGetClkCtrl3(void);
void   V9B_CpmSetClkCtrl4(UINT8 val);
UINT8  V9B_CpmGetClkCtrl4(void);

UINT16 V9B_CpmGetChip(void);

void   V9B_CpmSetGlbCtrl(UINT8 val);
UINT8  V9B_CpmGetGlbCtrl(void);
void   V9B_CpmStartSeq(void);
void   V9B_CpmStopSeq(void);
void   V9B_CpmStartTrmTimer(void);
void   V9B_CpmStopTrmTimer(void);
void   V9B_CpmPauWavSyn(void);
void   V9B_CpmResuWavSyn(void);
void   V9B_CpmSetMonoOut(void);
void   V9B_CpmSetStereoOut(void);
void   V9B_CpmEnSamRatConv(void);
void   V9B_CpmDisSamRatConv(void);
void   V9B_CpmEnAllChanFM(void);
void   V9B_CpmDisAllChanFM(void);
void   V9B_CpmEnAllChanAM(void);
void   V9B_CpmDisAllChanAM(void);
void   V9B_CpmStartSTRMTimer(void);
void   V9B_CpmStopSTRMTimer(void);

UINT8  V9B_CpmGetStopAll(void);

void   V9B_CpmSetWsMisc(UINT8 val);
UINT8  V9B_CpmGetWsMisc(void);
void   V9B_CpmEnSyncOut(void);
void   V9B_CpmDisSyncOut(void);

void   V9B_CpmSetGlbCfg(UINT8 val);
UINT8  V9B_CpmGetGlbCfg(void);
void   V9B_CpmEnVolUpdateTst(void);
void   V9B_CpmDisVolUpdateTst(void);
void   V9B_CpmEnVolUpdateInst(void);
void   V9B_CpmDisVolUpdateInst(void);
void   V9B_CpmSetWsCntSel(UINT8 sel);

/********** some functions maybe define in other c file ***********/

UINT32 V9B_CpmGetTmr(void);
void   V9B_CpmSetTmrTgt(UINT32 val);
UINT32 V9B_CpmGetTmrTgt(void);

UINT8  V9B_CpmGetSTmr(void);
void   V9B_CpmSetSTmrTgt(UINT8 val);
UINT8  V9B_CpmGetSTmrTgt(void);

void   V9B_CpmSetTmrBase(UINT16 val);
UINT16 V9B_CpmGetTmrBase(void);

void   V9B_CpmSetFRatioDlt(UINT32 val);
UINT32 V9B_CpmGetFRatioDlt(void);

void   V9B_CpmSetAnaErrFlg(UINT8 val);
UINT8  V9B_CpmGetAnaErrFlg(void);

void   V9B_CpmSetAnaErrMsk(UINT8 val);
UINT8  V9B_CpmGetAnaErrMsk(void);

void   V9B_CpmSetSwReset(UINT8 uType);

UINT16 V9B_CpmGetVer(void);

void   V9B_CpmSetIntCtrl(UINT8 val);
UINT8  V9B_CpmGetIntCtrl(void);
void   V9B_CpmSetIntEn0(UINT8 val);
UINT8  V9B_CpmGetIntEn0(void);
void   V9B_CpmSetIntFlg0(UINT8 val);
UINT8  V9B_CpmGetIntFlg0(void);
void   V9B_CpmSetIntEn1(UINT8 val);
UINT8  V9B_CpmGetIntEn1(void);
void   V9B_CpmSetIntFlg1(UINT8 val);
UINT8  V9B_CpmGetIntFlg1(void);
void   V9B_CpmSetIntEn2(UINT8 val);
UINT8  V9B_CpmGetIntEn2(void);
void   V9B_CpmSetIntFlg2(UINT8 val);
UINT8  V9B_CpmGetIntFlg2(void);
void   V9B_CpmSetIntLock(UINT8 val);
UINT8  V9B_CpmGetIntLock(void);
void   V9B_CpmSetIntServ(UINT8 val);
UINT8  V9B_CpmGetIntServ(void);

void   V9B_CpmSetIntDlyCnt(UINT16 val);
UINT16 V9B_CpmGetIntDlyCnt(void);

void   V9B_CpmSetIntDlySel(UINT8 val);
UINT8  V9B_CpmGetIntDlySel(void);
void V9B_CpmEnIntDly(void);
void V9B_CpmDisIntDly(void);

void   V9B_CpmSetSwInt(UINT8 val);
UINT8  V9B_CpmGetSwInt(void);

void   V9B_CpmSetAutoStopEn(UINT8 val);
UINT8  V9B_CpmGetAutoStopEn(void);
void   V9B_CpmEnAutoStopEn(UINT8 seqNum);
void   V9B_CpmDisAutoStopEn(UINT8 seqNum);

void   V9B_CpmSetRepeatIntCnt(UINT16 val);
UINT16 V9B_CpmGetRepeatIntCnt(void);

void   V9B_CpmSetGlbModSel(UINT8 val);
UINT8  V9B_CpmGetGlbModSel(void);
void   V9B_CpmSetIntFlgClrType(UINT8 clrType);
void   V9B_CpmEnIntRepeat(void);
void   V9B_CpmDisIntRepeat(void);
void   V9B_CpmSetCmdStatusClrType(UINT8 clrType);
void   V9B_CpmSetStopAllClrType(UINT8 clrType);


#ifdef __cplusplus
}
#endif

#endif //_RDK_CPM938_H_


