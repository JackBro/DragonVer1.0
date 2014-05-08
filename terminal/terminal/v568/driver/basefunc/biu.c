#include "basefunc.h"

#ifndef	V578CMD_DISABLE

#ifdef WIN32
#include <malloc.h>
#include "ComChannel/simhost.h"
#endif
#include "../../userapi/userbaserw.h"

#ifndef WIN32
static void V5_By16SetRSLowCs1Fun(UINT16 val);
static void V5_By16SetRSHighCs1Fun(UINT16 val);
static UINT16 V5_By16GetRSLowCs1Fun(void);
static UINT16 V5_By16GetRSHighCs1Fun(void);

static void V5_By8SetRSLowCs1Fun(UINT16 val);
static void V5_By8SetRSHighCs1Fun(UINT16 val);
static UINT16 V5_By8GetRSLowCs1Fun(void);
static UINT16 V5_By8GetRSHighCs1Fun(void);

static void V5_By8SetRSLowCs2Fun(UINT16 val);
static void V5_By8SetRSHighCs2Fun(UINT16 val);
static UINT16 V5_By8GetRSLowCs2Fun(void);
static UINT16 V5_By8GetRSHighCs2Fun(void);

static void V5_By16SetRSLowCs2Fun(UINT16 val);
static void V5_By16SetRSHighCs2Fun(UINT16 val);
static UINT16 V5_By16GetRSLowCs2Fun(void);
static UINT16 V5_By16GetRSHighCs2Fun(void);

static void V5_SelRsOpeFunc(void);
#endif

/*biu context initial setting*/
TBiuContext gBiuContext = {
	V5_BUS_MULTI8,   // Bus type
	0x1,             // bypass select type; 1:by register, 0:by GPIO
	0x90,			 //bypass register address
	0,               //bypass cs select config
	0,               //panel selected in bypass mode
	V5_Mul8SetRegByWType,   // Set register function
	V5_Mul8GetRegByWType,   // Get register function
	V5_Mul8ReadSram,        // Read sram function
	V5_Mul8WriteSram,       //Write sram function
	V5_Mul8WriteSramRev,    //Write sram reverse two bytes function
	V5_Mul8WriteSramOnWord, //Write sram on word function

#ifndef WIN32
	V5_By16SetRSLowCs1Fun,      //set rs low function in bypass mode
    V5_By16GetRSLowCs1Fun,      //get rs low function in bypass mode
	V5_By16SetRSHighCs1Fun,     //set rs high function in bypass mode 
    V5_By16GetRSHighCs1Fun,     //get rs high function in bypass mode
#else
	NULL,
	NULL,
	NULL,
	NULL
#endif
};

#ifndef WIN32
/*Set/Get bypass low/high operation function in multi8/16 bus type*/
static void V5_By8SetRSLowCs1Fun(UINT16 val)
{
	 V5_By8SetRSLowCs1(val); 
}

static void V5_By8SetRSHighCs1Fun(UINT16 val)
{
	V5_By8SetRSHighCs1(val);  
}

static void V5_By16SetRSLowCs1Fun(UINT16 val)
{
	V5_By16SetRSLowCs1(val);
}

static void V5_By16SetRSHighCs1Fun(UINT16 val)
{
	V5_By16SetRSHighCs1(val);
}

static void V5_By8SetRSLowCs2Fun(UINT16 val)
{
	V5_By8SetRSLowCs2(val);
}

static void V5_By8SetRSHighCs2Fun(UINT16 val)
{
	V5_By8SetRSHighCs2(val);
}

static void V5_By16SetRSLowCs2Fun(UINT16 val) 
{
	V5_By16SetRSLowCs2(val);
}

static void V5_By16SetRSHighCs2Fun(UINT16 val)
{
	V5_By16SetRSHighCs2(val);
}

static UINT16 V5_By8GetRSLowCs1Fun(void)
{
	return V5_By8GetRSLowCs1();
}

static UINT16 V5_By8GetRSHighCs1Fun(void)
{
	return V5_By8GetRSHighCs1();
}

static UINT16 V5_By16GetRSLowCs1Fun(void)
{
	return V5_By16GetRSLowCs1();
}

static UINT16 V5_By16GetRSHighCs1Fun(void)
{
	return V5_By16GetRSHighCs1(); 
}

static UINT16 V5_By8GetRSLowCs2Fun(void)
{
	return V5_By8GetRSLowCs2();
}

static UINT16 V5_By8GetRSHighCs2Fun(void)
{
	return V5_By8GetRSHighCs2();
}

static UINT16 V5_By16GetRSLowCs2Fun(void)
{
	return V5_By16GetRSLowCs2();
}

static UINT16 V5_By16GetRSHighCs2Fun(void)
{
	return V5_By16GetRSHighCs2();
}

/*Select rs operation functions in bypass mode according to current configuration*/
static void V5_SelRsOpeFunc(void)
{
	if(gBiuContext.bypassCsSelCon == 0x0 && gBiuContext.bypassPanelSel == SLAVE_PANEL)//use two cs, and select cs2
	{
		if(gBiuContext.busType == V5_BUS_MULTI16)
		{
			gBiuContext.pBySetRSLowFunc  =  V5_By16SetRSLowCs2Fun;
			gBiuContext.pBySetRSHighFunc =  V5_By16SetRSHighCs2Fun;
			gBiuContext.pByGetRSLowFunc  =  V5_By16GetRSLowCs2Fun;
			gBiuContext.pByGetRSHighFunc =  V5_By16GetRSHighCs2Fun;
		}
		else
		{
		    gBiuContext.pBySetRSLowFunc  =  V5_By8SetRSLowCs2Fun;
			gBiuContext.pBySetRSHighFunc =  V5_By8SetRSHighCs2Fun;
			gBiuContext.pByGetRSLowFunc  =  V5_By8GetRSLowCs2Fun;
			gBiuContext.pByGetRSHighFunc =  V5_By8GetRSHighCs2Fun;
		}	
	}
	else//use cs1 only or use two cs, and select cs1
	{
		if(gBiuContext.busType == V5_BUS_MULTI16)
		{
			gBiuContext.pBySetRSLowFunc  =  V5_By16SetRSLowCs1Fun;
			gBiuContext.pBySetRSHighFunc =  V5_By16SetRSHighCs1Fun;
			gBiuContext.pByGetRSLowFunc  =  V5_By16GetRSLowCs1Fun;
			gBiuContext.pByGetRSHighFunc =  V5_By16GetRSHighCs1Fun;
		}
		else
		{
		    gBiuContext.pBySetRSLowFunc  =  V5_By8SetRSLowCs1Fun;
			gBiuContext.pBySetRSHighFunc =  V5_By8SetRSHighCs1Fun;
			gBiuContext.pByGetRSLowFunc  =  V5_By8GetRSLowCs1Fun;
			gBiuContext.pByGetRSHighFunc =  V5_By8GetRSHighCs1Fun;
		}		
	}
}

#endif

/*Set value to command test register*/
void V5B_BiuSetTestReg(UINT16 uVal)
{
	V5_SetReg(V5_REG_BIU_CMD, uVal, BIT_16_LTOH);
}

/*Get value from command test register*/
UINT16 V5B_BiuGetTestReg(void)
{
	UINT16 uReg;

	uReg = (UINT16)V5_GetReg(V5_REG_BIU_CMD, BIT_16_LTOH);
	
	return uReg;
}

/*Set biu to write mode*/
void V5B_BiuSelWrite(void)
{
	V5_SetReg(V5_REG_BIU_SEL_WRITE_READ, 0x1, BIT_8);
}

/*Set biu to read mode*/
void V5B_BiuSelRead(void)
{
	V5_SetReg(V5_REG_BIU_SEL_WRITE_READ, 0x0, BIT_8);
}

/*Get write/read mode of biu*/
UINT8 V5B_BiuGetRdWr(void)
{
	UINT8 uReg;

	uReg = (UINT8)V5_GetReg(V5_REG_BIU_SEL_WRITE_READ, BIT_8);
	
	return uReg;
}

/*Set port access enable*/
void V5B_BiuPortEnable(void)
{
	V5B_SetRegMaskBit8(V5_REG_BIU_SEL_PORT, (UINT8)BIT0, OR_OPCODE);
}

/*Set port access disable*/
void V5B_BiuPortDisable(void)
{
	V5B_SetRegMaskBit8(V5_REG_BIU_SEL_PORT, (UINT8)~BIT0, AND_OPCODE);
}

/*Init multiplex 16 bus type*/
void V5B_BiuMul16Init(void)
{
	V5_SetReg(V5_REG_BIU_REG_8_HIGH_WORD, (UINT8)((((UINT16)V5_REG_BIU_SEL_8_16)&0xff00)>>8), BIT_8);
	V5_SetReg(V5_REG_BIU_REG_8_LOW_WORD, (UINT8)(((UINT16)V5_REG_BIU_SEL_8_16)&0xff), BIT_8);
	V5_SetReg(V5_REG_BIU_REG_8_FLG, 0x1, BIT_8);
}

/*Set biu transfer size, 8/16*/
void V5B_BiuSetMul_8_16(UINT8 val)
{
	V5_SetReg(V5_REG_BIU_SEL_8_16, val, BIT_8);
}

/*Set address of bypass register*/
void V5B_BiuSetBypassRegAddr(UINT8 val)
{
	gBiuContext.bypassRegAddr = val;

    V5_SetReg(V5_REG_BIU_BYPASS_ADDR_L, val, BIT_8);
	V5_SetReg(V5_REG_BIU_BYPASS_ADDR_M, 0x18, BIT_8);
	V5_SetReg(V5_REG_BIU_BYPASS_ADDR_H, 0x0, BIT_8);
}

/*Get address of bypass register*/
UINT32 V5B_BiuGetBypassRegAddr(void)
{
	UINT8 addrL, addrM;

	addrL = (UINT8)V5_GetReg(V5_REG_BIU_BYPASS_ADDR_L, BIT_8);
	addrM = (UINT8)V5_GetReg(V5_REG_BIU_BYPASS_ADDR_M, BIT_8);

	return ( ((UINT32)addrM << 8) & 0xff00 ) | ( ((UINT32)addrL) & 0xff ); 
}

/*Get sram address of accessed currently*/
UINT32 V5B_BiuGetCurMemAddr(void)
{
	UINT32 val;

	val = V5_GetReg(V5_REG_BIU_ADDR_MEM_LOW_L, BIT_24_LTOH);
	
	return val;
}

/*Set m of pll*/
void V5B_BiuSetPllM(UINT8 val)
{
	V5_SetReg(V5_REG_BIU_PLL_M, val, BIT_8);
}

/*Get m of pll*/
UINT8 V5B_BiuGetPllM(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_PLL_M, BIT_8);

	return val;
}

/*Set n of pll*/
void V5B_BiuSetPllN(UINT8 val)
{
	V5_SetReg(V5_REG_BIU_PLL_N, val, BIT_8);
}

/*Get n of pll*/
UINT8 V5B_BiuGetPllN(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_PLL_N, BIT_8);

	return val;
}

/*Set od of pll*/
void V5B_BiuSetPllOD(UINT8 od0, UINT8 od1)
{
	V5_SetReg(V5_REG_BIU_PLL_OD, (od0&0x1)|((od1&0x1)<<1), BIT_8);
}

/*Get od of pll*/
void V5B_BiuGetPllOD(UINT8* od0, UINT8* od1)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_PLL_OD, BIT_8);

	*od0 = val&0x1;
	*od1 = (val>>1)&0x1;
}

/*Set clock div of pll*/
void V5B_BiuSetClkDiv(UINT8 val)
{
	V5_SetReg(V5_REG_BIU_PLL_CLKCTRL, val, BIT_8);
}

/*Get clock div of pll*/
UINT8 V5B_BiuGetClkDiv(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_PLL_CLKCTRL, BIT_8);

	return val;
}

/*Enable PLL bypass*/
void V5B_BiuPllBypassEnable(void)
{
	V5B_SetRegMaskBit8(V5_REG_BIU_PLL_BP,(UINT8) BIT0, OR_OPCODE);
}

/*Disable PLL bypass*/
void V5B_BiuPllBypassDisable(void)
{
	V5B_SetRegMaskBit8(V5_REG_BIU_PLL_BP, (UINT8)~BIT0, AND_OPCODE);
}

/*Enable PLL power down*/
void V5B_BiuPllPowerDownEnable(void)
{
	V5B_SetRegMaskBit8(V5_REG_BIU_PLL_PD, (UINT8)BIT0, OR_OPCODE);
}

/*Disable PLL power down*/
void V5B_BiuPllPowerDownDisable(void)
{
	V5B_SetRegMaskBit8(V5_REG_BIU_PLL_PD, (UINT8)~BIT0, AND_OPCODE);
}

/*Set value to lcd interface configuration register*/
void V5B_BiuSetLcdIfCon(UINT8 val)
{
    V5_SetReg(V5_REG_BIU_LCDIF_CONFIG, val, BIT_8);
}

/*Get value from lcd interface configuration register*/
UINT8 V5B_BiuGetLcdIfCon(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_LCDIF_CONFIG, BIT_8);

	return val;
}

void   V5B_BiuXclkEanble(void)
{
    V5_SetReg(V5_REG_BIU_XCLK_DISABLE, 0x0, BIT_8);
}

void   V5B_BiuXclkDisable(void)
{
    V5_SetReg(V5_REG_BIU_XCLK_DISABLE, 0x1, BIT_8);
}



/*Set address delay value*/
void V5B_BiuSetAddrDelaySel(UINT8 val)
{
	V5_SetReg(V5_REG_BIU_ADDR_DELAY_SEL, val, BIT_8);
}

/*Get address delay value*/
UINT8 V5B_BiuGetAddrDelaySel(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_ADDR_DELAY_SEL ,BIT_8);

	return val;
}

/*Set data out delay value*/
void V5B_BiuSetDataOutDelaySel(UINT8 val)
{
	V5_SetReg(V5_REG_BIU_DATA_OUT_DELAY_SEL, val, BIT_8);
}

/*Get data out delay value*/
UINT8 V5B_BiuGetDataOutDelaySel(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_DATA_OUT_DELAY_SEL, BIT_8);

	return val;
}

/*Set data in delay value*/
void V5B_BiuSetDataInDelaySel(UINT8 val)
{
	V5_SetReg(V5_REG_BIU_DATA_IN_DELAY_SEL, val, BIT_8);
}

/*Get data in delay value*/
UINT8 V5B_BiuGetDataInDelaySel(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_DATA_IN_DELAY_SEL, BIT_8);

	return val;
}

/*Set data oen delay value*/
void V5B_BiuSetDataOenDelaySel(UINT8 val)
{
	V5_SetReg(V5_REG_BIU_DATA_OEN_SEL, val, BIT_8);
}

/*Get data oen delay value*/
UINT8 V5B_BiuGetDataOenDelaySel(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_DATA_OEN_SEL, BIT_8);

	return val;
}

/*Set cs delay value*/
void V5B_BiuSetCsDelaySel(UINT8 val)
{
	V5_SetReg(V5_REG_BIU_CS_DELAY_SEL, val, BIT_8);
}

/*Get cs delay value*/
UINT8 V5B_BiuGetCsDelaySel(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_CS_DELAY_SEL, BIT_8);

	return val;
}

/*Set wr thru delay value*/
void V5B_BiuSetWrThruDelaySel(UINT8 val)
{
	V5_SetReg(V5_REG_BIU_WR_THRU_DELAY_SEL, val, BIT_8);
}

/*Get wr thru delay value*/
UINT8 V5B_BiuGetWrThruDelaySel(void)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(V5_REG_BIU_WR_THRU_DELAY_SEL, BIT_8);

	return val;
}

UINT8 V5B_SetRegMaskBit8(UINT32 adr, UINT8 mask, UINT8 op)
{
	UINT8 val;

	val = (UINT8)V5_GetReg(adr, BIT_8);
	switch(op)
	{
	case AND_OPCODE:
		val &= mask;
		break;
	case OR_OPCODE:
		val |= mask;
		break;
	default:
		break;;
	}
	V5_SetReg(adr, val, BIT_8);

	return val;
}

UINT16 V5B_SetRegMaskBit16(UINT32 adr, UINT16 mask, UINT8 op)
{
	UINT16 val;

	val = (UINT16)V5_GetReg(adr, BIT_16_LTOH);
	switch(op)
	{
	case AND_OPCODE:
		val &= mask;
		break;
	case OR_OPCODE:
		val |= mask;
		break;
	default:
		break;;
	}
	V5_SetReg(adr, val, BIT_16_LTOH);

	return val;
}

void V5B_Mul16SetBypassOrNormalDir(UINT8 byOrNormal)
{
	V5_Mul16SetRegByWType((UINT16)(V5_REG_BIU_BASE + gBiuContext.bypassRegAddr), byOrNormal, BIT_8);	  
}

void V5B_Mul16SetBypassOrNormal(UINT8 byOrNormal)
{
    if(byOrNormal)
    {
	    V5_Mul16SetRegByWType(V5_REG_BIU_SEL_8_16, 0x0, BIT_8);

		V5_HOSTSETADDR8(V5_MULTI16_ADDR_ADDR, 0xb4);
        V5_HOSTSETVAL8(V5_MULTI16_ADDR_DATA, 0x30);

        V5_HOSTSETADDR8(V5_MULTI16_ADDR_ADDR, 0xb2);
        V5_HOSTSETVAL8(V5_MULTI16_ADDR_DATA, 0x0);

	    V5_HOSTSETADDR8(V5_MULTI16_ADDR_ADDR, gBiuContext.bypassRegAddr);
        V5_HOSTSETVAL8(V5_MULTI16_ADDR_DATA, 0x1);
    }
    else
    {         
	    V5_HOSTSETADDR8(V5_MULTI16_ADDR_ADDR, gBiuContext.bypassRegAddr);
        V5_HOSTSETVAL8(V5_MULTI16_ADDR_DATA, 0x0);

	    V5_Mul8SetRegByWType(V5_REG_BIU_SEL_8_16, 0x1, BIT_8);
    }
}

void V5B_Mul8SetBypassOrNormal(UINT8 byOrNormal)
{
    if(byOrNormal)
    {
        V5_HOSTSETADDR8(V5_MULTI8_ADDR_ADDR, 0xb4);
        V5_HOSTSETVAL8(V5_MULTI8_ADDR_DATA, 0x30);

        V5_HOSTSETADDR8(V5_MULTI8_ADDR_ADDR, 0xb2);
        V5_HOSTSETVAL8(V5_MULTI8_ADDR_DATA, 0x0);

        V5_HOSTSETADDR8(V5_MULTI8_ADDR_ADDR, gBiuContext.bypassRegAddr);
        V5_HOSTSETVAL8(V5_MULTI8_ADDR_DATA, 0x1);
    }
    else
    {
	    V5_HOSTSETADDR8(V5_MULTI8_ADDR_ADDR, gBiuContext.bypassRegAddr);
        V5_HOSTSETVAL8(V5_MULTI8_ADDR_DATA, 0x0);
    }
}

#ifdef WIN32

void V5B_SetBusType(UINT8 type)
{
    char str[0x100];
	
	sprintf(str, "V5B_SetBusType %x\n", type);
	SendCmd(str);
}

void V5B_BiuSetBypassSelType(UINT8 selType)
{
    char str[0x100];
	
	sprintf(str, "V5B_BiuSetBypassSelType %x\n", selType);
	SendCmd(str);
}

void V5B_SetBypassOrNormal(UINT8 byOrNormal)
{
	char str[0x100];
	
	sprintf(str, "V5B_SetBypassOrNormal %x\n", byOrNormal);
	SendCmd(str);
}

#else

/********************************************************************************
  Description:
	Set chip bus access type
  Parameters:
    type : chip bus access type
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_SetBusType(UINT8 type)
{
	if(type == gBiuContext.busType)
	{
		return;
	}

	switch(type)
	{
	case V5_BUS_MULTI16:

        /*Need via Mul8 if bus type changed from Sep to Mul16*/
		if(gBiuContext.busType == V5_BUS_SEPERATE)
		{
			gBiuContext.busType = V5_BUS_MULTI8;
            gBiuContext.pSetRegFunc = V5_Mul8SetRegByWType;
		}

		V5B_BiuSetMul_8_16(1); //chage to bus 16
		gBiuContext.busType = V5_BUS_MULTI16;
		gBiuContext.pSetRegFunc = V5_Mul16SetRegByWType;
		gBiuContext.pGetRegFunc = V5_Mul16GetRegByWType;
		gBiuContext.pReadSramFunc = V5_Mul16ReadSram;
		gBiuContext.pWriteSramFunc = V5_Mul16WriteSram;
		gBiuContext.pWriteSramRevFunc = V5_Mul16WriteSramRev;
		gBiuContext.pWriteSramOnWordFunc = V5_Mul16WriteSramOnWord;
		break;

	case V5_BUS_MULTI8:
		V5B_BiuSetMul_8_16(0); //chage to bus 8
		gBiuContext.busType = V5_BUS_MULTI8;
		gBiuContext.pSetRegFunc = V5_Mul8SetRegByWType;
		gBiuContext.pGetRegFunc = V5_Mul8GetRegByWType;
		gBiuContext.pReadSramFunc = V5_Mul8ReadSram;
		gBiuContext.pWriteSramFunc = V5_Mul8WriteSram;
		gBiuContext.pWriteSramRevFunc = V5_Mul8WriteSramRev;
		gBiuContext.pWriteSramOnWordFunc = V5_Mul8WriteSramOnWord;
		break;

	case V5_BUS_SEPERATE:
		V5B_BiuSetMul_8_16(0); //chage to bus 8
		gBiuContext.busType = V5_BUS_SEPERATE;
		gBiuContext.pSetRegFunc = V5_SepSetRegByWType;
		gBiuContext.pGetRegFunc = V5_SepGetRegByWType;
		gBiuContext.pReadSramFunc = V5_SepReadSram;
		gBiuContext.pWriteSramFunc = V5_SepWriteSram;
		gBiuContext.pWriteSramRevFunc = V5_SepWriteSramRev;
		gBiuContext.pWriteSramOnWordFunc = V5_SepWriteSramOnWord;
		break;
	
	default:
		break;
	}
}

void V5B_BiuSetBypassSelType(UINT8 selType)
{
	gBiuContext.bypassSelType = selType;
    V5_SetReg(V5_REG_BIU_BYPASS_FROM_SEL, selType, BIT_8);
}

void V5B_SetBypassOrNormal(UINT8 byOrNormal)
{
	if(gBiuContext.bypassSelType == BY_REG)
	{
		if(byOrNormal)
		{
			V5B_BiuPllPowerDownEnable();
		}
		if(gBiuContext.busType == V5_BUS_MULTI16)
			V5B_Mul16SetBypassOrNormalDir(byOrNormal);
		else
            V5B_Mul8SetBypassOrNormal(byOrNormal);
	}
	V5_SelRsOpeFunc();

	if((byOrNormal == 0) && (gBiuContext.bypassSelType == BY_REG))
	{
		V5B_BiuPllPowerDownDisable();
	}
}

#endif

/********************************************************************************
  Description:
	Initial set chip bus access type
  Parameters:
    type : chip bus access type
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5_InitBusType(UINT8 type)
{
	V5B_SetBusType(type);

	if(type == V5_BUS_MULTI16)
	{
		V5B_BiuMul16Init();
	}
}

#ifdef WIN32

/********************************************************************************
  Description:
	Delay function
  Parameters:
	count : time(ms)
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5_Delay(UINT32 count)
{
	SimHost_Delay(count);
}

/********************************************************************************
  Description:
	Read chip sram(marb control)
  Parameters:
	adr  : initial read address
	buf  : the pointer to data buffer
	size : data buffer size
  Note:
  Returns:
	return read sram actual size
  Remarks:
*********************************************************************************/
UINT32 V5_ReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	return SimHost_ReadSram(adr, buf, size);
}

/********************************************************************************
  Description:
	Write chip sram(marb control)
  Parameters:
	adr  : initial write address
	buf  : the pointer to data
	size : data buf size
  Note:
  Returns:
	return wroten sram actual size
  Remarks:
*********************************************************************************/
UINT32 V5_WriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	return SimHost_WriteSram(adr, buf, size);
}

/********************************************************************************
  Description:
	Write chip sram(marb control), in reverse sequence between two bytes
  Parameters:
	adr  : initial write address
	buf  : the pointer to data
	size : data buf size
  Note:
  Returns:
	return wroten sram actual size
  Remarks:
*********************************************************************************/
UINT32 V5_WriteSramRev(UINT32 adr, UINT8 *buf, UINT32 size)
{
	return SimHost_WriteSramRev(adr, buf, size);
}

/********************************************************************************
  Description:
	Write chip sram(marb control)
  Parameters:
	adr  : initial write address
	buf  : data
	size : data size
  Note:
  Returns:
	return wroten sram actual size
  Remarks:
*********************************************************************************/
UINT32 V5_WriteSramOnWord(UINT32 adr, UINT32 dat, UINT32 size)
{
	char cmd[0x100];
	int val;

	sprintf(cmd, "V5_WriteSramOnWord %x, %x, %x\n", adr, dat, size);
	val = SendRetCmd(cmd);

	return (UINT32)val;
}

#else //ARM Environment

/********************************************************************************
  Description:
	Delay function
  Parameters:
	count : time(ms)
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5_Delay(UINT32 count)
{
	V5_HostDelay(count);
}

/********************************************************************************
  Description:
	Read chip sram(marb control)
  Parameters:
	adr  : intial read address
	buf  : the pointer to data buffer
	size : data buffer size
  Note:
  Returns:
	return read sram actual size
  Remarks:
*********************************************************************************/
UINT32 V5_ReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 cnt;

	cnt = gBiuContext.pReadSramFunc(adr, buf, size);

	return cnt;
}

/********************************************************************************
  Description:
	Write chip sram(marb control)
  Parameters:
	adr  : initial write address
	buf  : the pointer to data data buffer
	size : data size
  Note:
  Returns:
	return wroten sram actual size
  Remarks:
*********************************************************************************/
UINT32 V5_WriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 cnt;

	cnt = gBiuContext.pWriteSramFunc(adr, buf, size);

	return cnt;
}

/********************************************************************************
  Description:
	Write chip sram(marb control), in reverse sequence between two bytes
  Parameters:
	adr  : initial write address
	buf  : the pointer to data
	size : data buf size
  Note:
  Returns:
	return wroten sram actual size
  Remarks:
*********************************************************************************/
UINT32 V5_WriteSramRev(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 cnt;

	cnt = gBiuContext.pWriteSramRevFunc(adr, buf, size);

	return cnt;
}

/********************************************************************************
  Description:
	Write chip sram(marb control)
  Parameters:
	adr  : initial write address
	dat  : the data 
	size : data buffer size
  Note:
  Returns:
	return wroten sram actual size
  Remarks:
*********************************************************************************/
UINT32 V5_WriteSramOnWord(UINT32 adr, UINT32 dat, UINT32 size)
{
	UINT32 cnt;

	cnt = gBiuContext.pWriteSramOnWordFunc(adr, dat, size);

	return cnt;
}

#endif //WIN32



#ifdef WIN32

#ifndef REGOPER_INWIN32LEVEL //for register debug

/********************************************************************************
  Description:
	Set vaule to chip register, if adr > V5_SRAM_ADDR_MIN, acess sram
  Parameters:
	adr  : register address
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5_SetReg(UINT32 adr, UINT32 val, UINT8 regWType)
{
	UINT8 x[4];

	if(adr < V5_SRAM_BASE)
	{
		SimHost_SetReg(adr, val, regWType);
	}
	else {//For debug
		adr -= V5_SRAM_BASE;
		SimHost_ReadSram((adr>>2)<<2, x, 4);
		x[adr&0x3] = (UINT8)val;
		SimHost_WriteSram((adr>>2)<<2, x, 4);
	}
}

/********************************************************************************
  Description:
	Get value from chip register, if adr > V5_SRAM_ADDR_MIN, acess sram
  Parameters:
	adr : register address
  Note:
  Returns:register value 
  Remarks:
*********************************************************************************/
UINT32 V5_GetReg(UINT32 adr, UINT8 regWType)
{
	UINT8 x[4];

	if(adr < V5_SRAM_BASE)
	{
		return SimHost_GetReg(adr, regWType);
	}
	else { //For debug
		adr -= V5_SRAM_BASE;
		SimHost_ReadSram((adr>>2)<<2, x, 4);

		return x[adr&0x3];
	}
}

#endif

#else //ARM Environment

/********************************************************************************
  Description:
	Set value to chip register, if adr > V5_SRAM_ADDR_MIN, acess sram
  Parameters:
	adr  : register address
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5_SetReg(UINT32 adr, UINT32 val, UINT8 regWType)
{
	gBiuContext.pSetRegFunc(adr, val, regWType);
}

/********************************************************************************
  Description:
	Get value from chip register, if adr > V5_SRAM_ADDR_MIN, acess sram
  Parameters:
	adr  : address
  Note:
  Returns: register value
  Remarks:
*********************************************************************************/
UINT32 V5_GetReg(UINT32 adr, UINT8 regWType)
{
	return gBiuContext.pGetRegFunc(adr, regWType);
}

#endif //WIN32


#ifdef WIN32

void V5_BySetPanelSel(UINT8 panelSel)
{
    char str[0x100];
	
	sprintf(str, "V5_BySetPanelSel %x \n", panelSel);
	SendCmd(str);
}

void V5_BySetRSLow(UINT16 val)
{
    char str[0x100];
	
	sprintf(str, "V5_BySetRSLow %x \n", val);
	SendCmd(str);
}

void V5_BySetRSHigh(UINT16 val)
{
    char str[0x100];
	
	sprintf(str, "V5_BySetRSHigh %x \n", val);
	SendCmd(str);
}

UINT16 V5_ByGetRSLow(void)
{
	char cmd[0x100];
	int val;

	sprintf(cmd, "V5_ByGetRSLow\n");
	val = SendRetCmd(cmd);

	return (UINT16)val;
}

UINT16 V5_ByGetRSHigh(void)
{
	char cmd[0x100];
	int val;

	sprintf(cmd, "V5_ByGetRSHigh\n");
	val = SendRetCmd(cmd);

	return (UINT16)val;
}

#else  //WIN32

void V5_BySetPanelSel(UINT8 panelSel)
{
	gBiuContext.bypassPanelSel = panelSel;
		
	if(gBiuContext.bypassCsSelCon == 0x0)
	{
        V5B_BiuSetLcdIfCon(0x0);
	}
	else//use two cs
	{
		V5B_BiuSetLcdIfCon(0x2 | (panelSel & 0x1));
	}
}

/********************************************************************************
  Description:
	Set control to LCD directly in bypass mode
  Parameters:
	ctrl : control 
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5_BySetRSLow(UINT16 val)
{
	gBiuContext.pBySetRSLowFunc(val);
}

/********************************************************************************
  Description:
	Set value to LCD directly in bypass mode
  Parameters:
	val :  value 
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5_BySetRSHigh(UINT16 val)
{
	gBiuContext.pBySetRSHighFunc(val);
}

/********************************************************************************
  Description:
	Get control from LCD directly in bypass mode
  Parameters:
  Note:
  Returns: Control
  Remarks:
*********************************************************************************/
UINT16 V5_ByGetRSLow(void)
{
    UINT16 val;

	val = gBiuContext.pByGetRSLowFunc();

    return val;
}

/********************************************************************************
  Description:
	Get value from LCD directly in bypass mode
  Parameters:
  Note:
  Returns: value
  Remarks:
*********************************************************************************/
UINT16 V5_ByGetRSHigh(void)
{
    UINT16 val;

	val = gBiuContext.pByGetRSHighFunc();

    return val;
}

#endif

#endif //V578CMD_DISABLE

