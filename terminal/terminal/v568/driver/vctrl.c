 
#include "driver.h"

#ifndef	V578CMD_DISABLE

#ifdef WIN32
#include "ComChannel/simhost.h"
#endif


#define V578_PLL_BUG_NOTFIX

static const UINT8 gIntPriority[V5_INT_NUM]
	= { INT_SIF, INT_ISP, INT_LBUF, INT_JPEG, INT_GE, INT_MARB1, INT_MARB0, INT_LCDC};

static TCtrlContext g_TCtrlContext;

void V5_CtrlInitContext(void)
{
	memset(&g_TCtrlContext, 0, sizeof(g_TCtrlContext));

	g_TCtrlContext.state = CTRL_INIT_CONTEXT;
}

void V5_CtrlSetInfo(TCtrlInfo *pinfo)
{
	g_TCtrlContext.pinputinfo = pinfo;

	g_TCtrlContext.state = CTRL_INIT_INFO;
}

TCtrlContext* V5_CtrlGetContext(void)
{
	return &g_TCtrlContext;
}


void V5_CtrlOpen(void)
{
	/*Judge chip version*/
	V5B_CpmGetChipID();

	if(g_TCtrlContext.pinputinfo == NULL)
		return;

	/*Set delay value*/
	V5B_BiuSetAddrDelaySel(g_TCtrlContext.pinputinfo->dlycell[0]);
	V5B_BiuSetDataOutDelaySel(g_TCtrlContext.pinputinfo->dlycell[1]);
	V5B_BiuSetDataInDelaySel(g_TCtrlContext.pinputinfo->dlycell[2]);
	V5B_BiuSetDataOenDelaySel(g_TCtrlContext.pinputinfo->dlycell[3]);

	/*Set clock*/
	if(g_TCtrlContext.pinputinfo->clkcfgtype == 0)
	{
		V5_CtrlSetChipClk(g_TCtrlContext.pinputinfo->clkin, g_TCtrlContext.pinputinfo->mclk);
	}
	else
	{
		V5_CtrlSetChipClkDirect(g_TCtrlContext.pinputinfo->clkin, g_TCtrlContext.pinputinfo->pll_m,
								g_TCtrlContext.pinputinfo->pll_n,	g_TCtrlContext.pinputinfo->pll_od0,
								g_TCtrlContext.pinputinfo->pll_od1,g_TCtrlContext.pinputinfo->clkoutdiv);
	}
#ifdef V578_PLL_BUG_NOTFIX
	V5B_BiuPllBypassEnable();
#endif
	/*Disable all interrupt and clear isp regist*/
	V5_CtrlIntDisable(INT_ALL);
	V5_CtrlIntSetCallback(INT_ALL, NULL);

	/*Set interrupt control*/
	V5B_CpmSetIntCtl(g_TCtrlContext.pinputinfo->intprop);

	/*Open clock of all module*/
	V5_CtrlSetModClkOn(MOD_CLK_ALL);

	/*Set byppss select type and register address*/
	V5B_BiuSetBypassSelType(BY_REG);
	V5B_BiuSetBypassRegAddr(g_TCtrlContext.pinputinfo->bypassRegAddr);

	g_TCtrlContext.state = CTRL_OPEN;
}

void V5_CtrlClose(void)
{
}

/*Set reset to specific or all module*/
void V5_CtrlSwReset(UINT16 uType)
{
	if(uType == SW_RESET_GLB)
	{
		V5B_CpmModGlbReset();
	}
	else
	{
		V5B_CpmModReset(uType);
	}
}

/*On clock of specific or all module*/
UINT16	V5_CtrlSetModClkOn(UINT16 uType)
{
	UINT16 val = 0;

	val = V5B_CpmGetModClkVal();
	V5B_CpmSetModClkVal(val&(~uType));

	V5_Delay(10);

	return val;
}

/*Off clock of specific or all module*/
UINT16	V5_CtrlSetModClkOff(UINT16 uType)
{
	UINT16 val = 0;

	val = V5B_CpmGetModClkVal();
	V5B_CpmSetModClkVal(val|uType);

	return val;
}

/*Set interrupt control*/
void V5_CtrlSetIntCtrl(UINT8 triggerType, UINT8 levelType, UINT8 edgeType, UINT8 sigWidth)
{
	UINT8 ctrl;

	ctrl = (triggerType&0x1)|((levelType&0x1)<<1)|((edgeType&0x1)<<2)|(sigWidth<<3);
	V5B_CpmSetIntCtl(ctrl);
}

/*Get interrupt control*/
void V5_CtrlGetIntCtrl(UINT8* triggerType, UINT8* levelType, UINT8* edgeType, UINT8* sigWidth)
{
	UINT8 ctrl;

	ctrl = V5B_CpmGetIntCtl();

	*triggerType = ctrl&0x1;
	*levelType = (ctrl&0x2)>>1;
	*edgeType= (ctrl&0x4)>>2;
	*sigWidth = (ctrl&0xf8)>>3;
}

/*Set isp to corresponding to module interrupt*/
void V5_CtrlIntSetCallback(UINT8 index, PV5IsrCallback pcall)
{
	UINT8 i;

	if(INT_ALL <= index)
	{
		for(i=0; i<INT_ALL; i++)
		{
			g_TCtrlContext.intisr[i] = pcall;
		}
	}
	else
	{
		g_TCtrlContext.intisr[index] = pcall;
	}
}

/*Enable interrupt of specific or all module*/
void V5_CtrlIntEnable(UINT8 index)
{
	UINT8 inten;

	if(INT_ALL <= index)
	{
		V5B_CpmSetIntEn(0xff);
	}
	else
	{
		inten = V5B_CpmGetIntEn();
		inten |= (0x1<<index);
		V5B_CpmSetIntEn(inten);
	}
}

/*Disable interrupt of specific or all module*/
void V5_CtrlIntDisable(UINT8 index)
{
	UINT8 inten;

	if(INT_ALL <= index)
	{
		V5B_CpmSetIntEn(0x0);
	}
	else
	{
		inten = V5B_CpmGetIntEn();
		inten &= (~(0x1<<index));
		V5B_CpmSetIntEn(inten);
	}
}

/*Store sram context*/
void V5_CtrlStoreSramCont(UINT32 *pAddr, UINT8  *pDat, UINT8 *pBacknum, UINT8 *pRw)
{
	UINT32 addr, wData;
	UINT8  rw, backnum;

    addr = V5B_BiuGetCurMemAddr();
	 
	if(addr > V5_SRAM_BASE)
	{
		addr -= V5_SRAM_BASE;
	}
	else
	{
	    addr = 0;
	}

	backnum = (UINT8)(addr&0x3);
	addr &= 0xfffffffc;
	rw = V5B_BiuGetRdWr();

	*pAddr = addr;
	*pRw = rw;
	*pBacknum = backnum;

	if(backnum)
	{
	    if(rw)
		{
		    wData = V5B_MarbGetCurWData( );
			pDat[0] = (UINT8)(wData & 0xff);
			pDat[1] = (UINT8)((wData >> 8) & 0xff);
			pDat[2] = (UINT8)((wData >> 16) & 0xff);
			pDat[3] = (UINT8)((wData >> 24) & 0xff);
		} 
	}
}

/*Restore sram context*/
void V5_CtrlRestoreSramCont(UINT32 addr, UINT8  *pDat, UINT8 backnum, UINT8 rw)
{
	if(backnum)
	{
	    if(rw)
	    {
		    V5_WriteSram(addr, pDat, backnum);
	    }
	    else
	    {
		    V5_ReadSram(addr, pDat, backnum);
	    }
	}
}

/*Interrupt process*/
void _ISR_V5IntHandle(void)
{
	UINT32 addr = 0;
	UINT8  i, x, intEn, intFlg, rw = 0, backnum = 0;
	UINT8  dat[4];
	TAcceContext locAcceCon; 

	V5_StoreReg2SramCont(&locAcceCon);

	intEn = V5B_CpmGetIntEn();
	intFlg = V5B_CpmGetIntFlag();
	if(0 == intEn || 0 == intFlg)
	{
		V5_RestoreReg2SramCont(&locAcceCon);
		return;
	}	

	V5_CtrlStoreSramCont(&addr, dat, &backnum, &rw);

	g_TCtrlContext.intoccurnum = 0;

	for(i=0; i<V5_INT_NUM; i++)
	{
		x = (UINT8)(0x1<<gIntPriority[i]);
		if((intEn&x) && (intFlg&x))
		{
			V5B_CpmSetIntServ(x);

			g_TCtrlContext.intoccurtype[g_TCtrlContext.intoccurnum] = gIntPriority[i];
			g_TCtrlContext.intoccurflag[g_TCtrlContext.intoccurnum] = V5B_CpmGetIntFlagSec(gIntPriority[i]);
			g_TCtrlContext.intoccurnum++;

			V5B_CpmSetIntServ(0);
		}
	}

	for(i=0; i< g_TCtrlContext.intoccurnum; i++)
	{
		if(g_TCtrlContext.intisr[g_TCtrlContext.intoccurtype[i]])
			g_TCtrlContext.intisr[g_TCtrlContext.intoccurtype[i]](g_TCtrlContext.intoccurflag[i]);
	}

	V5_CtrlRestoreSramCont(addr, dat, backnum, rw);

	V5_RestoreReg2SramCont(&locAcceCon);
}

/*Switch to bypass mode*/
void V5_CtrlNormalToBypass(void)
{
	V5B_SetBypassOrNormal(0x01);
}

/*Switch to normal mode*/
void V5_CtrlBypassToNormal(void)
{
	V5B_SetBypassOrNormal(0);
}

/*Set clock according to parameters*/
static void SetClkByParm(UINT8 m, UINT8 n, UINT8 od0, UINT8 od1, UINT8 outdiv)
{
		V5_CtrlPllOff();

		V5B_BiuSetPllM(m);
		V5B_BiuSetPllN(n);
		V5B_BiuSetPllOD(od0, od1);
		V5B_BiuSetClkDiv(outdiv);

		V5_MarbInitMemClk();

		V5_CtrlPllOn();
}

/********************************************************************************
  Description:
	Set chip clock
  Parameters:
		clkin: input clock(KHZ)
		mclk: request ouput clock(KHZ)
  Note:
		<TABLE>
		This function can make sure such conditions:

		mclk = pllout/clkdiv;
		pllout = clkin*(m/n)*(1/NO);

		clkin must >= 2000(KHZ) and <= 375000(KHZ);
		clkin/n must >= 1000(KHZ) and <= 25000(KHZ);
		pllout*NO must >= 20000(KHZ) and <= 1000000(KHZ);

		</TABLE>
  Returns:
	Return actual clock(KHZ)
  Remarks:
  Example:
		<TABLE>
		if clkin = 12000 requieted mclk 24000, can call function as:
		{
			V558_CtrlSetChipClk(12000, 24000);
		}
		</TABLE>
*********************************************************************************/
UINT32 V5_CtrlSetChipClk(UINT32 clkin, UINT32 mclk)
{
	SMIC_PLL_PARM pllparm;

	//Set clk;
	if(SMIC_PllCalculate(clkin, mclk, &pllparm))
	{
		SetClkByParm(pllparm.m, pllparm.n, pllparm.od0, pllparm.od1, pllparm.outdiv);
		g_TCtrlContext.clkin = clkin;
		g_TCtrlContext.mclk = pllparm.clkout;
	
		return pllparm.clkout;
	}

	return 0;
}

/********************************************************************************
  Description:
	Set chip clock directly according to parameters
  Parameters:
		clkin: input clock(KHZ)
  Note:
		<TABLE>
		This function can make sure such conditions:

		mclk = pllout/clkdiv;
		pllout = clkin*(m/n)*(1/NO);

		clkin must >= 2000(KHZ) and <= 375000(KHZ);
		clkin/n must >= 1000(KHZ) and <= 25000(KHZ);
		pllout*NO must >= 20000(KHZ) and <= 1000000(KHZ);
		m must >= 2 and <= 255;
		n must >= 2 and <= 15;

		</TABLE>
  Returns:
	Return actual clock(KHZ)
  Remarks:
  Example:
*********************************************************************************/
UINT32	V5_CtrlSetChipClkDirect(UINT32 clkin, UINT8 m, UINT8 n, UINT8 od0, UINT8 od1, UINT8 outdiv)
{
	SMIC_PLL_PARM pllparm;

	pllparm.m = m;
	pllparm.n = n;
	pllparm.od0 = od0;
	pllparm.od1 = od1;
	pllparm.outdiv = outdiv;

	//Set clk;
	SMIC_PllCalCheck(clkin, &pllparm);

	if(pllparm.clkout != 0);
	{
		SetClkByParm(pllparm.m, pllparm.n, pllparm.od0, pllparm.od1, pllparm.outdiv);
		g_TCtrlContext.clkin = clkin;
		g_TCtrlContext.mclk = pllparm.clkout;
	}

	return pllparm.clkout;;
}

UINT32 V5_CtrlGetMclk(void)
{
	return g_TCtrlContext.mclk;
}

/*Pll on*/
void V5_CtrlPllOn(void)
{
	V5B_BiuXclkEanble();
	// Bypass disable, power down disable(power on)
	V5B_BiuPllBypassDisable();
	V5B_BiuPllPowerDownDisable();

	V5_Delay(10);
}

/*Pll off*/
void V5_CtrlPllOff(void)
{
	// Bypass disable, power down, set clkout = 0
	V5B_BiuPllBypassDisable();
	V5B_BiuPllPowerDownEnable();
	V5B_BiuXclkDisable();

	V5_Delay(10);
}

void V5_CtrlAllModReset(void)
{
	V5B_CpmSetModClkVal(0x0);
	V5B_CpmModGlbReset();
	V5_Delay(1);
}

#endif //V578CMD_DISABLE
