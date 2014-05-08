 
#include "driver938.h"

#ifndef	V938CMD_DISABLE

#ifdef WIN32
#include "ComChannel/simhost.h"
#endif


static TCtrlContext938 g_TCtrlContext938;
/* temperorily located here */
extern TV9TransBufContext       gV9TransBufContext;

static const UINT32 gClkCtrl[][4] = 
{
    /* mclk            clkCtrl1  clkCtrl2  clkCtrl3 */
	{  3000000,        0x10,     0x1e,     0x11},
	{  3500000,        0x10,     0x1a,     0x11},
	{  4000000,        0x10,     0x16,     0x11},
	{  5000000,        0x10,     0x12,     0x11},
	{  6000000,        0x10,     0x0e,     0x11},
	{  7000000,        0x10,     0x0c,     0x11},
	{  8000000,        0x10,     0x0a,     0x11},
	{  9000000,        0x10,     0x09,     0x11},
	{  10000000,       0x10,     0x08,     0x11},
	{  11000000,       0x10,     0x07,     0x11},
	{  12000000,       0x41,     0x0e,     0x11},
	{  13000000,       0x41,     0x0d,     0x11},
	{  14000000,       0x42,     0x0c,     0x11},
	{  15000000,       0x43,     0x0b,     0x11},
	{  16000000,       0x21,     0x10,     0x11},
	{  17000000,       0x21,     0x0f,     0x11},
	{  18000000,       0x21,     0x0e,     0x11},
	{  19000000,       0x21,     0x0d,     0x11},
	{  20000000,       0x22,     0x12,     0x11},
	{  21000000,       0x22,     0x11,     0x11},
	{  22000000,       0x21,     0x0b,     0x11},
	{  23000000,       0x21,     0x0b,     0x11},
	{  24000000,       0x21,     0x0a,     0x11},
	{  25000000,       0x23,     0x12,     0x11},
	{  26000000,       0x23,     0x11,     0x11},
	{  27000000,       0x23,     0x10,     0x11},
	{  28000000,       0x23,     0x0f,     0x11},
	{  29000000,       0x23,     0x0f,     0x11},
	{  30000000,       0x21,     0x08,     0x11},
	{  31000000,       0x23,     0x0e,     0x11},
	{  32000000,       0x23,     0x0d,     0x11},
	{  33000000,       0x23,     0x0d,     0x11},
	{  34000000,       0x23,     0x0c,     0x11},
	{  35000000,       0x23,     0x0c,     0x11},
	{  36000000,       0x21,     0x06,     0x11}
};

static void DummyIsr(void){}

void V9_CtrlInitContext(void)
{
	memset(&g_TCtrlContext938, 0, sizeof(g_TCtrlContext938));

	g_TCtrlContext938.eFifoLen = 512;
	g_TCtrlContext938.sFifoLen = 1888;
	g_TCtrlContext938.cFifoLen = 64;
	g_TCtrlContext938.state = CTRL_INIT_CONTEXT938;
}

void V9_CtrlSetInfo(TCtrlInfo938 *pinfo)
{
	g_TCtrlContext938.pinputinfo = pinfo;

	g_TCtrlContext938.state = CTRL_INIT_INFO938;
}

TCtrlContext938* V9_CtrlGetContext(void)
{
	return &g_TCtrlContext938;
}


void V9_CtrlOpen(void)
{
	if(g_TCtrlContext938.pinputinfo == NULL)
		return;	

    /******************** start 938 clock ******************/
	V9_CtrlSetPll(g_TCtrlContext938.pinputinfo->mclk);
    V9_CtrlStartClk(CTRL_PLL_WORK_MODE);

	/********* set 938 interrupt triger as low level *******/
	V9B_CpmSetIntCtrl(g_TCtrlContext938.pinputinfo->intprop);

	/************* Set evnet buffer water mark *************/
	V9B_AudioSetEWMark(0x1);

	/************* Set interrupt process function *************/
	V9_CtrlIntSetCallback(CTRL_INT_EFIFOEMPTY, V9_CtrlSetEventFifo);
	V9_CtrlIntSetCallback(CTRL_INT_SFIFO0EMPTY, V9_CtrlSetStreamFifo);
	V9_CtrlIntSetCallback(CTRL_INT_SFIFO1EMPTY, V9_CtrlSetStreamFifo);
	V9_CtrlIntSetCallback(CTRL_INT_SFIFO2EMPTY, V9_CtrlSetStreamFifo);
	V9_CtrlIntSetCallback(CTRL_INT_SFIFO3EMPTY, V9_CtrlSetStreamFifo);

	/*************** set 938 to normal mode ****************/
	V9_CtrlStandby2Idle();
	V9_CtrlIdle2Normal();

	g_TCtrlContext938.state = CTRL_OPEN938;
}

void V9_CtrlClose(void)
{
	/************** set 938 to standby mode ***************/
	V9_CtrlNormal2Idle();
	V9_CtrlIdle2Standby();

	g_TCtrlContext938.state = CTRL_NOT_INIT938;
}

/* Event and stream fifo interrupt process */
void    V9_CtrlSetEventFifo(void)
{
	UINT16 len = g_TCtrlContext938 .eFifoLen;
	UINT8  *pBuf = gV9TransBufContext.pBuf + gV9TransBufContext.curRdPos;

	if(gV9TransBufContext.curRdPos >= gV9TransBufContext.realEndPos)
		return;

    if(gV9TransBufContext.curRdPos + len > gV9TransBufContext.realEndPos)
		len = (UINT16)(gV9TransBufContext.realEndPos - gV9TransBufContext.curRdPos);

    V9B_FifoWrEFifo(pBuf, len);
	gV9TransBufContext.curRdPos += len; 
}

void    V9_CtrlEventInt(void)
{
	V9B_CpmSetIntServ(INTSERV_EFIFO);
    V9_CtrlSetEventFifo();
	V9B_CpmSetIntServ(0x0);
}

void    V9_CtrlSetStreamFifo(void)
{
	UINT16 len = g_TCtrlContext938 .sFifoLen;
	UINT8  *pBuf = gV9TransBufContext.pBuf + gV9TransBufContext.curRdPos;

	if(gV9TransBufContext.curRdPos >= gV9TransBufContext.realEndPos)
		return;

    if(gV9TransBufContext.curRdPos + len > gV9TransBufContext.realEndPos)
		len = (UINT16)(gV9TransBufContext.realEndPos - gV9TransBufContext.curRdPos);

    V9B_FifoWrSFifo(pBuf, len);
	gV9TransBufContext.curRdPos += len; 
}

void    V9_CtrlStreamInt(void)
{
	V9B_CpmSetIntServ(INTSERV_EFIFO);
    V9_CtrlSetStreamFifo();
	V9B_CpmSetIntServ(0x0);
}

void    V9_CtrlSetCommandFifo(void)
{
	UINT16 len = g_TCtrlContext938 .cFifoLen;
	UINT8  *pBuf = gV9TransBufContext.pBuf + gV9TransBufContext.curRdPos;

	if(gV9TransBufContext.curRdPos >= gV9TransBufContext.realEndPos)
		return;

    if(gV9TransBufContext.curRdPos + len > gV9TransBufContext.realEndPos)
		len = (UINT16)(gV9TransBufContext.realEndPos - gV9TransBufContext.curRdPos);

    V9B_FifoWrCFifo(pBuf, len);
	gV9TransBufContext.curRdPos += len; 
}

void    V9_CtrlSetTestMod(void)
{
	V9B_CpmSetDacToTestMod();
}

void    V9_CtrlSetNormalMod(void)
{
	V9B_CpmSetDacToWorkMod();
}

void    V9_CtrlSetPmcCtrl(UINT8 reg_pwdb, UINT8 iref_pwdb, UINT8 vref_pwdb)
{
	V9B_CpmSetPmcCtrl((reg_pwdb & 0x1) | ((iref_pwdb & 0x1) << 0x1) | ((vref_pwdb & 0x1) << 0x2));
}

void   V9_CtrlSetUmCtrl(UINT8 modSel, UINT8 modGroupSel)
{
	V9B_CpmSetUmCtrl((UINT8)(0x1 << 0x7) | ((modSel & 0x7) << 0x4) | (modGroupSel & 0xf));
}

/*Set reset to specific or all module*/
void V9_CtrlSwReset(UINT8 uType)
{
	V9B_CpmSetSwReset(uType);
}

void    V9_CtrlResetClk(void)
{
	V9B_CpmSetClkCtrl0(0x1E);
	V9B_CpmSetClkCtrl0(0x0E);
	V9B_CpmSetClkCtrl0(0x00);

	V9_Delay(1);
}

void  V9_CtrlSetPll(UINT32 mclk)
{
	UINT8 i;
     
    for(i = 0; i < 35; i++)
	{
		if(gClkCtrl[i][0] == mclk)
		{
			V9B_CpmSetClkCtrl1((UINT8)gClkCtrl[i][1]);
			V9B_CpmSetClkCtrl2((UINT8)gClkCtrl[i][2]);
			V9B_CpmSetClkCtrl3((UINT8)gClkCtrl[i][3]);

			g_TCtrlContext938.mclk = mclk;

			break;
		}
	}
}

void	V9_CtrlStartClk(UINT8 pllMode)
{
	if(pllMode == CTRL_PLL_WORK_MODE)			//PLL working mode
	{
		V9B_CpmSetClkCtrl0(0xFE);
		V9B_CpmSetClkCtrl0(0x0E);
		V9_Delay(2);
		V9_CtrlResetClk();
	} 
	else if(pllMode == CTRL_PLL_BYPASS_MODE)	//PLL bypass mode 
	{
		V9B_CpmSetClkCtrl3(0xFF);
		V9B_CpmSetClkCtrl0(0xFE);
		V9B_CpmSetClkCtrl0(0xFE);  // why need two times?
		V9B_CpmSetClkCtrl0(0x1E);
	    V9B_CpmSetClkCtrl0(0x0E);
		V9B_CpmSetClkCtrl0(0xC0);
	}
}

void  V9_CtrlStopClk(void)
{
	V9B_CpmSetClkCtrl0(0xFE);
	V9B_CpmSetClkCtrl0(0xFF);
}

UINT32 V9_CtrlGetMclk(void)
{
	return g_TCtrlContext938.mclk;
}

/*Set interrupt control*/
void V9_CtrlSetIntCtrl(UINT8 triggerType, UINT8 levelType, UINT8 edgeType, UINT8 sigWidth)
{
	UINT8 ctrl;
 
	ctrl = (triggerType & 0x1) | ((levelType & 0x1) << 1) | ((edgeType & 0x1) << 2) | ((sigWidth & 0xf) << 4);
	V9B_CpmSetIntCtrl(ctrl);
}

/*Get interrupt control*/
void V9_CtrlGetIntCtrl(UINT8* triggerType, UINT8* levelType, UINT8* edgeType, UINT8* sigWidth)
{
	UINT8 ctrl;

	ctrl = V9B_CpmGetIntCtrl();

	*triggerType = ctrl & 0x1;
	*levelType   = (ctrl & 0x2) >> 1;
	*edgeType    = (ctrl & 0x4) >> 2;
	*sigWidth    = (ctrl & 0xf8) >> 3;
}

/*Set isr to corresponding to module interrupt*/
void V9_CtrlIntSetCallback(UINT8 index, PV9IsrCallback pcall)
{
	UINT8 i;

	if(V9_INT_NUM <= index)
	{
		for(i = 0; i < V9_INT_NUM; i++)
		{
			g_TCtrlContext938.intisr[i] = pcall;
		}
	}
	else
	{
		g_TCtrlContext938.intisr[index] = pcall;
	}
}

/*Remove isr corresponding to module interrupt*/
void V9_CtrlIntRemoveCallback(UINT8 index)
{
	UINT8 i;

	if(V9_INT_NUM <= index)
	{
		for(i = 0; i < V9_INT_NUM; i++)
		{
			g_TCtrlContext938.intisr[i] = DummyIsr;
		}
	}
	else
	{
		g_TCtrlContext938.intisr[index] = DummyIsr;
	}
}

/*Enable interrupt of specific or all module*/
void V9_CtrlIntEnable(UINT8 intType)
{
	UINT8 bIntEn0 = 0;
	UINT8 bIntEn1 = 0;
	UINT8 bIntEn2 = 0;
	
	if (intType == CTRL_INT_ALL)
	{
		V9B_CpmSetIntEn0(0xff);
		V9B_CpmSetIntEn1(0xff);
		V9B_CpmSetIntEn2(0xff);
	}
	else if (intType <= CTRL_INT_SFIFO3EMPTY)
	{
		bIntEn0  = V9B_CpmGetIntEn0();
		bIntEn0 |= (0x01 << intType);
		V9B_CpmSetIntEn0(bIntEn0);
	}
	else if (intType <= CTRL_INT_SWI7)
	{
		bIntEn1 =  V9B_CpmGetIntEn1();
		bIntEn1 |= (0x01 << (intType - CTRL_INT_MFIFOEMPTY));
		V9B_CpmSetIntEn1(bIntEn1);
	}
	else if (intType <= CTRL_INT_MP3_END)
	{
		bIntEn2 =  V9B_CpmGetIntEn2();
		bIntEn2 |= (0x01 << (intType - CTRL_INT_STM0_END));
		V9B_CpmSetIntEn2(bIntEn2);
	}	
}

/*Disable interrupt of specific or all module*/
void V9_CtrlIntDisable(UINT8 intType)
{
	UINT8 bIntEn0 = 0;
	UINT8 bIntEn1 = 0;
	UINT8 bIntEn2 = 0;
	
	if (intType == CTRL_INT_ALL)
	{
		V9B_CpmSetIntEn0(0x00);
		V9B_CpmSetIntEn1(0x00);
		V9B_CpmSetIntEn2(0x00);
	}
	else if (intType <= CTRL_INT_SFIFO3EMPTY)
	{
		bIntEn0 = V9B_CpmGetIntEn0();
		bIntEn0 &= ~(0x01 << intType);
		V9B_CpmSetIntEn0(bIntEn0);
	}
	else if (intType <= CTRL_INT_SWI7)
	{
		bIntEn1 = V9B_CpmGetIntEn1();
		bIntEn1 &= ~(0x01 << (intType - CTRL_INT_MFIFOEMPTY));
		V9B_CpmSetIntEn1(bIntEn1);
	}
	else if (intType <= CTRL_INT_MP3_END)
	{
		bIntEn2 = V9B_CpmGetIntEn2();
		bIntEn2 &= ~(0x01 << (intType - CTRL_INT_STM0_END));
		V9B_CpmSetIntEn2(bIntEn2);
	}	
}

static void V9_CtrlClearFlag(UINT8 intType)
{
	if (intType == CTRL_INT_ALL)
	{
		V9B_CpmSetIntFlg0(0xff);
		V9B_CpmSetIntFlg1(0xff);
		V9B_CpmSetIntFlg2(0x1f);
	}
	else if (intType <= CTRL_INT_SFIFO3EMPTY)
	{
		V9B_CpmSetIntFlg0((UINT8)(0x01 << intType));
	}
	else if (intType <= CTRL_INT_SWI7)
	{
		V9B_CpmSetIntFlg1((UINT8)(0x01 << (intType-CTRL_INT_MFIFOEMPTY)));
	}
	else if(intType <= CTRL_INT_MP3_END)
	{
		V9B_CpmSetIntFlg2((UINT8)(0x01 << (intType-CTRL_INT_STM0_END)));
	}
}

/*Set interrupt lock*/
void    V9_CtrlSetIntLock(void)
{
	V9B_CpmSetIntLock(0x1);
}

void    V9_CtrlClearIntLock(void)
{
	V9B_CpmSetIntLock(0x0);
}

void    V9_CtrlSetIntServe(UINT8 intBit)
{
    V9B_CpmSetIntServ(0x1 << intBit);
}

void    V9_CtrlClearIntServe(void)
{
    V9B_CpmSetIntServ(0x0);
}

void    V9_CtrlClearSwi(UINT8 intType)
{
	UINT8 val;

	if(intType >= CTRL_INT_SWI1 && intType <= CTRL_INT_SWI7)
	{
		val = V9B_CpmGetSwInt();
		val &= ~(0x1 << (intType - CTRL_INT_SWI1));
		V9B_CpmSetSwInt(val);
	}
	else if(CTRL_INT_ALL == intType)
	{
        V9B_CpmSetSwInt(0x0);
	}
}

/*Interrupt process*/
static void _ISR_V9Int0Handle(void)
{
	UINT8 bIntEn0 = 0;
	UINT8 bIntFlag0 = 0;
	UINT8 i = 0;

	bIntEn0 = V9B_CpmGetIntEn0();
	if (bIntEn0)
	{
		bIntFlag0 = V9B_CpmGetIntFlg0();;
		if (bIntFlag0)
		{
			for (i = 0; i < 0x8; i++)
			{
				if ((bIntEn0 & bIntFlag0) & (0x01 << i))
				{
					V9_CtrlClearFlag(i);
					g_TCtrlContext938.intisr[i]();
				}
			}
		}
	}
}

static void _ISR_V9Int1Handle(void)
{
    UINT8 bIntEn1 = 0;
	UINT8 bIntFlag1 = 0;
	UINT8 i = 0;

	bIntEn1 = V9B_CpmGetIntEn1();
	if (bIntEn1)
	{
		bIntFlag1 = V9B_CpmGetIntFlg1();;
		if (bIntFlag1)
		{
			for (i = 0; i < 0x8; i++)
			{
				if ((bIntEn1 & bIntFlag1) & (0x01 << i))
				{
					V9_CtrlClearFlag((UINT8)(CTRL_INT_MFIFOEMPTY + i));
					g_TCtrlContext938.intisr[CTRL_INT_MFIFOEMPTY + i]();
				}
			}
		}
	}
}

static void _ISR_V9Int2Handle(void)
{
	UINT8 bIntEn2 = 0;
	UINT8 bIntFlag2 = 0;
	UINT8 i = 0;

    bIntEn2 = V9B_CpmGetIntEn2();
	if(bIntEn2)
	{
		bIntFlag2 = V9B_CpmGetIntFlg2();

		if (bIntFlag2)
		{
			for (i = 0; i < 0x5; i++)
			{
				if ((bIntEn2 & bIntFlag2) & (0x01 << i))
				{
					if (i == 4)
						V9_CtrlSetIntServe(2);
					else
						V9_CtrlSetIntServe((i+4));
						
					V9_CtrlClearFlag((UINT8)(CTRL_INT_STM0_END + i));
					g_TCtrlContext938.intisr[CTRL_INT_STM0_END + i]();

					V9_CtrlClearIntServe();
				}
			}
		}
	}
}

void _ISR_V9IntHandle(void)
{
	TAcceContext938 locAcceCon; 

	V9_CtrlSetIntLock();

	V9_StoreReg2SramCont(&locAcceCon);

    _ISR_V9Int2Handle();
	_ISR_V9Int1Handle();
    _ISR_V9Int0Handle();
	
	V9_RestoreReg2SramCont(&locAcceCon);

	V9_CtrlClearIntServe();

	V9_CtrlClearIntLock();
}

void    V9_CtrlSetTmrBase(UINT16 val)
{
    V9B_CpmSetTmrBase(val);    
}

UINT16  V9_CtrlGetTmrBase(void)
{
	UINT16  val;

	val = V9B_CpmGetTmrBase();

	return val;
}

void	V9_CtrlSetTmrTimer(UINT32 val)
{
    V9B_CpmSetTmrTgt(val);
}

UINT32	V9_CtrlGetTmrTimer(void)
{
	UINT32  val;

	val = V9B_CpmGetTmrTgt();

	return val;
}

void    V9_CtrlRstTmrTimer(void)
{
	V9B_CpmSetSwReset(SW_RESET_TMR);
}

void    V9_CtrlStartTmrTimer(void)
{
	V9B_CpmStartTrmTimer();
}

void    V9_CtrlStopTmrTimer(void)
{
	V9B_CpmStopTrmTimer();
}

void	V9_CtrlSetSTmrTimer(UINT8 val)
{
	V9B_CpmSetSTmrTgt(val);
}

UINT8	V9_CtrlGetSTmrTimer(void)
{
	UINT8 val;

    val = V9B_CpmGetSTmr();

	return val;
}

void    V9_CtrlRstSTmrTimer(void)
{
	V9B_CpmSetSwReset(SW_RESET_STMR);
}

void    V9_CtrlStartSTmrTimer(void)
{
	V9B_CpmStartSTRMTimer();
}

void    V9_CtrlStopSTmrTimer(void)
{
	V9B_CpmStopSTRMTimer();
}

void    V9_CtrlEnIIS(void)
{
    V9B_CpmSetSwReset(SW_RESET_I2S);
	V9B_CpmEnIiSDat();
	V9B_CpmSetPaCfg(0x9);
}

void    V9_CtrlDisIIS(void)
{
	V9B_CpmSetPaCfg(0x9);
    V9B_CpmDisDeepth();
	V9B_CpmDisIiSDat();
}

void    V9_CtrlSetIISMod(UINT8 mod, UINT8 freq)
{
	UINT8 dacCtrl, iisCfg;

    iisCfg = V9B_CpmGetIiscCfg();
	dacCtrl =  V9B_CpmGetDacCtrl();

	if(mod == CTRL_IIS_MASTER)
	{
        dacCtrl &= 0xdf;
		iisCfg |= 0x6;
		iisCfg &= 0xf;
	}
	else if(mod == CTRL_IIS_SLAVE1)
	{
        dacCtrl |= 0x40;
		iisCfg |= 0xe;
		iisCfg &= 0xf9;
	}
	else if(mod == CTRL_IIS_SLAVE2)
	{
		iisCfg |= 0x2;
		iisCfg &= 0xb;
	}

	if(freq == CTRL_IIS_FREQ1)
	{
		iisCfg &= 0x3f;
	}
	else if(freq == CTRL_IIS_FREQ2)
	{
        iisCfg &= 0x3f;
		iisCfg |= 0x1 << 6;
	}
	else if(freq == CTRL_IIS_FREQ3)
	{
        iisCfg &= 0x3f;
		iisCfg |= 0x1 << 7;
	}
    else if(freq == CTRL_IIS_FREQ4)
	{
        iisCfg |= 0xc0;
	}

	V9B_CpmSetDacCtrl(dacCtrl);
	V9B_CpmSetIisCfg(iisCfg);
}

/* Three mode transform:   standby <------> idle <-------> normal */

void  V9_CtrlStandby2Idle(void)
{
	/* Reset LED */
    V9_CtrlSwReset(SW_RESET_LED);

	/* Start chip Mclk */
    V9_CtrlStartClk(CTRL_PLL_WORK_MODE);

	/* Config GPIO, I2S, DAC */
	/* Set GPIO 0 - 3 as iis function mode, GPIO 4 -7 as GPIO mode */
	V9B_CpmSetGpioCfg(0xf0);
	V9B_CpmSetGpioFuncSel(0x05);	
	V9B_CpmSetIisCfg(0x46);
	V9B_CpmSetDacCfg(0x3e);

	V9B_CpmSetDacCtrl(0x09);
	V9_Delay(0x5);
	V9B_CpmSetPaCfg(0xcd);
	V9_Delay(0x5);

	/* Config interrupt setting, enable wave engine */
	V9B_CpmSetGlbModSel(0x02);
	V9_CtrlSetIntCtrl(0x0, 0x0, 0x1, 0x3);
	V9B_CpmSetGlbCfg(0x02);
	V9B_CpmEnSyncOut();

	/* Stop chip Mclk*/
	V9_CtrlStopClk();
} 

void V9_CtrlIdle2Normal(void)
{
	/* Start chip Mclk */
	V9_CtrlStartClk(CTRL_PLL_WORK_MODE);

	/* Config PA, audio DAC, audio data path */
	V9B_CpmSetPmcCtrl(0xf);
	V9B_CpmEnIiSDat();

	V9B_CpmSetPaCfg(0x4d);
    V9_Delay(20);
	V9B_CpmSetDacCtrl(0x08);
    V9_Delay(10);
    V9B_CpmSetPaCfg(0x09);

	V9B_CpmSetSpVol(0x0f);

	/* Set mp3 and volume step, maybe needed to move to mp3Eq module! */
	V9B_Mp3SetVolStep(0x1f);
	V9B_EqSetVolStep(0x09);

	V9_CtrlSetTmrBase(0x5dc0);

	/* Set audio and channel settings, maybe needed to move to sequencer module! */
	/************************************************************/

	/************************************************************/

    V9_Delay(10);
}

void V9_CtrlNormal2Idle(void)
{
    /* PA and DAC power down */
	V9B_CpmSetDacCtrl(0x09);

	V9_Delay(20);

	V9B_CpmSetGpioFuncSel(0x00);
	V9B_CpmSetDacCtrl(0x09);// Reset
	V9B_CpmSetPmcCtrl(0x00);

	V9_Delay(5);

    /* Stop chip Mclk*/
	V9_CtrlStopClk();
}

void V9_CtrlIdle2Standby(void)
{
    /* Start chip Mclk */
	V9_CtrlStartClk(CTRL_PLL_WORK_MODE);

	/* Set SP power down */
	V9B_CpmSetPaCfg(0x8d);

	V9_Delay(5);

	V9B_CpmSetDacCtrl(0x07);
	V9B_CpmSetDacCtrl(0x07);// Need two times?
	V9B_CpmSetPaCfg(0x89);

	/* Reset LED and Mclk */
    V9_CtrlSwReset(SW_RESET_LED);
	V9_CtrlSwReset(SW_RESET_BIUMCLK);

	/* Stop chip Mclk*/
	V9_CtrlStopClk();
}


#endif //V938CMD_DISABLE


