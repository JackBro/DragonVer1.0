#include "v558api.h"
#include "vregdef.h"

#include "vctrl.h"

//============================================================================//
/*                           chip version                                    */
//============================================================================//
UINT8 const DRIV_VERSION[5]={04,9,1,1,0};
//============================================================================//
/*                           Interrupt deal                                   */
//============================================================================//
#define MINPRI	4
static UINT8 uIntPri[] = {
			0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,
			0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,
			0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,
			0x2,0x1,0x1,0x1,0x1,0x1,0x1,0x1,
			0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x1,
			0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,
			0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,
			0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1
			};
void (*ChipIntHandlers[V558_INT_MAX_NUMBER])(void);
static void ChipIntDummy() { }

static UINT16 AdjustFactor(UINT16 m, UINT16 n, UINT16 p);
static void ChipClkCreate(UINT32 clkin, UINT32 clkout, UINT8 factorh, UINT8 factorl, PCLK_OUT_PARM clkoutparm);

//============================================================================//
/*                           Ctrl context                                     */
//============================================================================//
V558_CTRL_CONTEXT g_CtrlContext;

//==============================================================================//
//	global function of CTRL                                                //
//==============================================================================//

/********************************************************************************
  Description:
	Open 558 chip module.
  Note:
	This function must be called when system is power on.
*********************************************************************************/
void V558_Open()
{
	V558_CtrlOpen();
	V558_LcdOpen();
	V558_SifOpen();
	V558_IppOpen();
	
	V558_LbufOpen();
	V558_JbufOpen();
	V558_JpegOpen();

	//jbuf port acess type
	V558_CtrlJbutPortEnable();

}

/********************************************************************************
  Description:
	Initialize CTRL module's context
  Note:
	This function must be called when system is power on.
  Remarks:
*********************************************************************************/
void V558_CtrlInitContext(void)
{
	g_CtrlContext.Valid = FALSE;
	g_CtrlContext.State = V558_CTRL_NOT_INIT;
}

/********************************************************************************
  Description:
	Application level send initialize data to CTRL module
  Parameters:
  info:  point to the start address of CTRL's initialize data
  Note:
	This function must be called before use other function of this RDK,
		or the CTRL will be abnormal.
  Return Value List:
	SUCCEED	:  if succesful
	CTRLFAILED:  otherwise
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlSetInfo(V558_CTRL_INFO *info)
{
	g_CtrlContext.pInfo = info;
	
	g_CtrlContext.Valid = TRUE;
	g_CtrlContext.State = V558_CTRL_POWOFF;
	
	
	return SUCCEED;
}

/********************************************************************************
  Description:
	Open host control module
  Parameters:
  Note:
	This function must be used before 558 chip work
  Returns:
	SUCCEED:  if succesful
	CTRLFAILED:  otherwise
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlOpen()
{
	PV558_CTRL_INFO pInf = g_CtrlContext.pInfo;

#if 0
	//V558 chip property
	CtrlSetChipSel(1); //558
	CtrlSetInterfaceAsynOrSyc(0); //asyn
	CtrlSetWeDrive(0);
	CtrlSetCpuPinCtrl(0);
	CtrlSetLcdPinCtrl(0);
	CtrlSetSnrPinCtrl(0);
	CtrlSetUmCtrl(0xff);
	CtrlSetMiscCtrl(0);
#endif

	//Int control
	V558_CtrlIntInit(pInf->Int.LeverOrEdge, pInf->Int.HighOrLow, pInf->Int.Width);

	g_CtrlContext.Valid = TRUE;
	g_CtrlContext.State = V558_CTRL_POWON;
	
	
	return SUCCEED;
}

/********************************************************************************
  Description:
	Get current bus type
  Parameters:
  Note:
  Returns:
	return bustype: 1-seperate,2-multi8,3-multi16;
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetBusType(void)
{
	return g_CtrlContext.pInfo->BusType;
}

//============================================================================//
/*                           Interrupt deal                                   */
//============================================================================//

/********************************************************************************
Description:
	set Interrupt parameter
Parameters:
	LeverOrEdge: Trigger type, 0 for edge trigger and 1 for level trigger
	HighOrLow:  Edge active fall or rise, Level active low or high
	Width: hexadecimal number.how many clk width of interrupt trigger.
Note:
	This function must be used before use 558 function related to interrupt
Remarks:
*********************************************************************************/
void V558_CtrlIntInit(UINT8 LeverOrEdge,UINT8 HighOrLow, UINT8 Width)
{
	UINT16 i;

	V558_CtrlIntEnable(V558_INT_ALL, 0);

	V558_CtrlSetIntCtrl(LeverOrEdge, HighOrLow, Width);
	for (i = 0; i < V558_INT_MAX_NUMBER; i++)
	{
		ChipIntHandlers[i] = ChipIntDummy;

	}
}

/********************************************************************************
  Description:
	Add an interrupt handler function
  Parameters:
	uIntType: interrupt type V558_INT_TYPE
	func: interrupt handler function
  Note:
	This function is used for interrupt
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlIntAdd(UINT8 uIntType, void (*func)() )
{
	ChipIntHandlers[uIntType] = func;
}

/********************************************************************************

  Description:
	remove an interrupt handler function
  Parameters:
	uIntType: interrupt type V558_INT_TYPE
  Note:
	This function is used for interrupt
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlIntRemove(UINT8 uIntType)
{
	ChipIntHandlers[uIntType] = ChipIntDummy;
}

/********************************************************************************
  Description:
	enable or disable an interrupt handler
  Parameters:
	uIntType: interrupt type V558_INT_TYPE
	uEnable: 0 disable 1 enable
  Note:
	This function is used for interrupt
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlIntEnable(UINT16 uIntType,UINT8 uEnable)
{
	UINT8 i,uIntTypeSec,uIntTypeFst,uIntEnaFst,uIntEnaSec;

	if(V558_INT_ALL == uIntType)
	{
		if(uEnable == 0)
		{
			V558_SetReg(V558_REG_HCTRL_INTEN, (UINT8)0x0);

			for(i=0; i<8; i++)
			{

				V558_SetReg(V558_REG_HCTRL_INTEN0+2*i, (UINT8)0x0);
			}
		}
		else
		{
			V558_SetReg(V558_REG_HCTRL_INTEN, (UINT8)0xff);
			for(i=0; i<8; i++)
			{
				V558_SetReg(V558_REG_HCTRL_INTEN0+2*i, (UINT8)0xff);
			}
		}
	}
	else
	{
		uIntTypeFst = (UINT8)(uIntType/8);
		uIntTypeSec = (UINT8)(uIntType%8);

		V558_GetReg(V558_REG_HCTRL_INTEN, &uIntEnaFst);
		V558_GetReg(V558_REG_HCTRL_INTEN0+uIntTypeFst*4, &uIntEnaSec);
		if(uEnable == 0)
		{
			
			uIntEnaSec &= ~(0x1<<uIntTypeSec);
			V558_SetReg(V558_REG_HCTRL_INTEN0+uIntTypeFst*4, uIntEnaSec);
			if(uIntEnaSec == 0)
			{
				uIntEnaFst &= ~(0x1<<uIntTypeFst);
				V558_SetReg(V558_REG_HCTRL_INTEN, uIntEnaFst);
			}
		}
		else
		{
			uIntEnaSec |= (0x1<<uIntTypeSec);
			V558_SetReg(V558_REG_HCTRL_INTEN0+uIntTypeFst*4, uIntEnaSec);

			uIntEnaFst |= (0x1<<uIntTypeFst);
			V558_SetReg(V558_REG_HCTRL_INTEN, uIntEnaFst);
		}
	}
}

/********************************************************************************
  Description:
	interrupt drive function.
  Parameters:
  Note:
	This function is used for interrupt cpu to call 558 interrupt
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlIntHandle(void)
{
	UINT8 int_Ena;
	UINT8 uIntIndex[64];
	UINT8 uIntOcurrNum = 0;

	V558_GetReg(V558_REG_HCTRL_INTEN, &int_Ena);
	if(int_Ena != 0)
	{
		UINT8 i,k,iPri,int_Flag,int_Type;
		int j;

		V558_GetReg(V558_REG_HCTRL_INTFLAG, &int_Flag);
		if(int_Flag !=0)
		{
			for(i=0; i<8; i++)
			{
				if(int_Flag&((UINT8)(0x1<<i)))
				{
					V558_SetReg(V558_REG_HCTRL_INTSERV, (UINT8)(0x1<<i));
					V558_GetReg((V558_REG_HCTRL_INTFLAG0 + i*4), &int_Type);
					if(int_Type != 0)
					{
						for(j=0; j<8; j++)
						{
							if(int_Type&((UINT8)(0x1<<j)))
							{
								uIntIndex[uIntOcurrNum] = i*8 + j;
								uIntOcurrNum++;
							}
						}

					}
					V558_SetReg(V558_REG_HCTRL_INTSERV, 0x0);
				}
			}
			for(iPri=0;iPri<=MINPRI;iPri++)
			{
				for(k=0; k<uIntOcurrNum; k++)
				{
					if(uIntPri[uIntIndex[k]] == iPri)
					{
						ChipIntHandlers[uIntIndex[k]]();
					}
				}
			}
		}	
	}

}

//============================================================================//
/*                                                                            */
/*                 Base function of host control module                       */
/*                                                                            */
//============================================================================//

/********************************************************************************
  Description:
	Set or change 558 chip work mode
  Parameters:
	uMode: work mode V558_WORKMODE
  Note:
  Returns:
  Remarks:
*********************************************************************************/

void V558_CtrlSetWorkMode(UINT8 uMode) 
{ 
	if(uMode == V558_MODE_BYPASS) 
	{ 
		UINT8 uTempMode = 0; 
		V558_GetReg(V558_REG_BIU_OP_MODE, &uTempMode ); 
		uTempMode &= 0x18; 
		uTempMode |= V558_MODE_THROUGH; 
		V558_SetReg(V558_REG_BIU_OP_MODE, uTempMode); 
		
		V558_SetReg(V558_REG_BIU_BYPASS_SEL ,0x1); 
	//	*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = 0x1802;
	} 
	else 
	{ 
		UINT8 uTempMode = 0; 
		UINT8 uBypassSel = 0; 
		V558_GetReg(V558_REG_BIU_BYPASS_SEL ,&uBypassSel); 
		if((uBypassSel & 0x1) == 0x1) 
		{ 
			V558_SetReg(V558_REG_BIU_BYPASS_SEL ,0x0); 
		} 
		
		V558_GetReg(V558_REG_BIU_OP_MODE, &uTempMode ); 
		if(uTempMode & 0xe7 == uMode & 0xe7)
			return;
		uTempMode &= 0x18; 
		uTempMode |= uMode; 
		switch(uMode) 
		{ 
		case V558_MODE_DISPLAY:	
		case V558_MODE_ENCODER:
		case V558_MODE_DECODER:
		case V558_MODE_DECODER_FRAME:
			V558_SifDisableSyncGen();
			break;

		case V558_MODE_VIEW: 
		default: 
			break; 
		} 
		V558_SetReg(V558_REG_BIU_OP_MODE, uTempMode); 
		switch(uMode) 
		{ 
		case V558_MODE_VIEW: 
			{ 
				V558_SIZE size; 

				V558_JpegSetWorkMode(0x0);
				V558_SifGetSnrInfoImgSize(&size); 
				V558_IppSetImageSize(size); 
//				V558_SifEnableSyncGen();
				V558_CtrlEnableBRC();
			} 
			break; 
#if 0			
		case V558_MODE_DISPLAY:	
		case V558_MODE_ENCODER:
		case V558_MODE_DECODER:
		case V558_MODE_DECODER_FRAME:
			V558_SifDisableSyncGen();
			break;
#endif
		default: 
			break; 
		} 
		
	} 
} 

/********************************************************************************
  Description:
	Get current 558 chip work mode
  Parameters:
  Note:
  Returns:
	Return work mode V558_WORKMODE
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetWorkMode(void)
{
	UINT8 mode;
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_BIU_BYPASS_SEL, &uReg);
	if((uReg & 0x1) == 1)
	{
		return V558_MODE_BYPASS;
	}
	else
	{
		V558_GetReg(V558_REG_BIU_OP_MODE, &uReg);
		uReg &= 0xe7;
		mode = uReg;
		return mode;
	}
}


/********************************************************************************
  Description:
	Set jpeg bit rate contorl
  Parameters:
	enable: 0 disable 1 enable
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSetBitRateCtrl(UINT8 enable)
{
	UINT8 uMode = 0;
	V558_GetReg(V558_REG_BIU_OP_MODE, &uMode);
	if(enable)
	{
		uMode |= 0x10;
		V558_SetReg(V558_REG_BIU_OP_MODE, uMode);
	}
	else
	{
		uMode &= 0xef;
		V558_SetReg(V558_REG_BIU_OP_MODE, uMode);
	}
}

/********************************************************************************
  Description:
	enable jbuf port acess type
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlJbutPortEnable(void)
{
	V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT, 0x1);
}

/********************************************************************************
  Description:
	disable jbuf port acess type
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlJbutPortDisable(void)
{
	V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT, 0x0);
}

/********************************************************************************
  Description:
	Enable auto clear jbuf address in multi mode
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlMultiClrAutoEnable(void)
{
	V558_SetReg((UINT16)V558_REG_BIU_SEL_ASYN_SYN, 0x1);
}

/********************************************************************************
  Description:
	disable auto clear jbuf address in multi mode
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlMultiClrAutoDisable(void)
{
	V558_SetReg((UINT16)V558_REG_BIU_SEL_ASYN_SYN, 0x0);
}

/********************************************************************************
  Description:
	Software reset 558 chip modules.
  Parameters:
	uType: 558 chip module type V558_SW_RESET
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSwReset(UINT16 uType)
{
	V558_SetReg(V558_REG_HCTRL_RSTCTRL2,(UINT8)((uType&0xff00)>>8));
	V558_SetReg(V558_REG_HCTRL_RSTCTRL1,(UINT8)(uType&0xff));
	switch(uType)
	{
	case V558_SW_RESET_SIF:
		break;	
	case V558_SW_RESET_ISP:
		break;	
	case V558_SW_RESET_IPP:
		V558_IppOpen();
		break;	
	case V558_SW_RESET_LB:
		V558_LbufOpen();
		break;	
	case V558_SW_RESET_JPG:
		V558_JpegOpen();
		break;	
	case V558_SW_RESET_GE:
		break;	
	case V558_SW_RESET_LCD:
		V558_LcdOpenModule();
		break;	
	case V558_SW_RESET_GBL:
		V558_LcdOpenModule();
		break;
	default:
		break;	
	}
}

/********************************************************************************
  Description:
	Software set 558 chip modules clock on.
  Parameters:
	uType: 558 chip module type V558_MOD_CLK
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlSetModClkOn(UINT8 uType)
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKOFF, &uReg);
	V558_SetReg(V558_REG_HCTRL_CLKOFF, uReg & (~uType));
	return uReg;
}

/********************************************************************************
  Description:
	Software set 558 chip modules clock off.
  Parameters:
	uType: 558 chip module type V558_MOD_CLK
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlSetModClkOff(UINT8 uType)
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKOFF, &uReg);
	V558_SetReg(V558_REG_HCTRL_CLKOFF, uReg|uType);
	return uReg;
}

/********************************************************************************
  Description:
	Get 558 chip version
  Parameters:
	uType: Pointer to chip version context.
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlGetChipVision(UINT16* uType)
{
	UINT8 uTemp1, uTemp2;
	V558_GetReg(V558_REG_HCTRL_CHIPCTRL, &uTemp1);
	V558_GetReg(V558_REG_HCTRL_CHIPCTRL+1, &uTemp2);
	*uType = uTemp1+(uTemp2<<8);
}

/********************************************************************************
  Description:
	Set 558 chip clock input division
  Parameters:
	idiv: the division.
  Note:
	pllin = clkin/idiv
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSetChipClkIDiv(UINT8 idiv)
{

	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKCTRL1, &uReg);
	uReg &= 0xE0;
	uReg |= (idiv&0x1f);
	V558_SetReg(V558_REG_HCTRL_CLKCTRL1, uReg);

}

/********************************************************************************
  Description:
	Enable 558 chip clock input division function
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlChipClkIDivEnable()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKCTRL2, &uReg);
	uReg |= 0x80;
	V558_SetReg(V558_REG_HCTRL_CLKCTRL2, uReg);


}

/********************************************************************************
  Description:
	Disable 558 chip clock input division function
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlChipClkIDivDisable()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKCTRL2, &uReg);
	uReg &= 0x7f;
	V558_SetReg(V558_REG_HCTRL_CLKCTRL2, uReg);
}

/********************************************************************************
  Description:
  Parameters:
	fbdiv: feedback div
	plldivh: plldiv high byte
	plldivl: plldiv low byte
  Note:
	mclk = (pllin*fbdiv)/(plldivh+plldivl+2);
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSetChipClkPlldiv(UINT8 fbdiv, UINT8 plldivh, UINT8 plldivl)
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKCTRL2, &uReg);
	uReg &= 0xC0;
	uReg |= (fbdiv&0x3f);
	V558_SetReg(V558_REG_HCTRL_CLKCTRL2, uReg);

	uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKCTRL3, &uReg);
	uReg = (((plldivh&0xf)<<4)|(plldivl&0xf));
	V558_SetReg(V558_REG_HCTRL_CLKCTRL3, uReg);
}

/********************************************************************************
  Description:
	Get 558 chip clock pll division
  Parameters:
	fbdiv: feedback div
	plldivh: plldiv high byte
	plldivl: plldiv low byte
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlGetChipClkPlldiv(UINT8* fbdiv, UINT8* plldivh, UINT8* plldivl)
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKCTRL2, &uReg);
	*fbdiv = uReg & 0x3f;

	V558_GetReg(V558_REG_HCTRL_CLKCTRL3, &uReg);
	*plldivh = (uReg&0xf0)>>4;
	*plldivl = uReg&0xf;
}

/********************************************************************************
  Description:
	Enable chip clock pll division
  Parameters:
	fbdiv: feedback div
	plldivh: plldiv high byte
	plldivl: plldiv low byte
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlChipClkPlldivEnable()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKCTRL2, &uReg);
	uReg &= 0xBF;
	V558_SetReg(V558_REG_HCTRL_CLKCTRL2, uReg);

}

/********************************************************************************
  Description:
	Disable chip clock pll division
  Parameters:
	fbdiv: feedback div
	plldivh: plldiv high byte
	plldivl: plldiv low byte
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlChipClkPlldivDisable()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKCTRL2, &uReg);
	uReg |= 0x40;
	V558_SetReg(V558_REG_HCTRL_CLKCTRL2, uReg);
}

/********************************************************************************
  Description:
	Set chip clock on
  Parameters:
		clkin: input clock(KHZ)
		clkout: ouput clock(KHZ)
		factorh: duty factor
		factorl: duty factor
  Note:
		<TABLE>
		This function can make sure such conditions:
		clk----->/-----------[idiv]-------->[pll]------/----------mclk(out)--->
		Pll clock output clk mclk calculate as:
		pllin = clkin/idiv;
		mclk = (pllin*fbdiv)/(plldivh+plldivl+2);
		(pllin*fbdiv must >65 and < 130)
		</TABLE>
  Returns:
	Return actual clock(KHZ)
  Remarks:
  Example:
		<TABLE>
		if clkin = 12 requieted mclk 24, can call function as:
		{
		V558_CtrlSetChipClk(12000, 24000, 1, 1);
		}
		</TABLE>
*********************************************************************************/
UINT16 V558_CtrlSetChipClk(UINT32 clkin, UINT32 clkout, UINT8 factorh, UINT8 factorl)
{
	CLK_OUT_PARM clkoutparm;
	ChipClkCreate(clkin, clkout, factorh, factorl, &clkoutparm);
	if(clkoutparm.clkoutput==0)
	{
		return clkoutparm.clkoutput;
	}

	g_CtrlContext.pInfo->Clk.clkin = clkin;
	g_CtrlContext.pInfo->Clk.clkout = clkout;
	g_CtrlContext.pInfo->Clk.factorh = factorh;
	g_CtrlContext.pInfo->Clk.factorl = factorl;

	V558_CtrlSetChipClkOff();
	if(clkoutparm.idiv)
	{
		V558_CtrlChipClkIDivEnable();
	}
	else
	{
		V558_CtrlChipClkIDivDisable();
	}
	V558_CtrlSetChipClkIDiv(clkoutparm.idiv);
	V558_CtrlSetChipClkPlldiv(clkoutparm.fdiv, clkoutparm.plldivh, clkoutparm.plldivl);
	V558_CtrlChipClkPlldivEnable();
	V558_CtrlSetChipClkOn();

	return clkoutparm.clkoutput;
}

/********************************************************************************
  Description:
	Get chip clock current setting
  Parameters:
  Note:
  Returns:
		clkin: current chip input clock setting(KHZ)
		clkout: current chip mclk(KHZ)
		factorh: current duty factor setting
		factorl: current duty factor setting
  Remarks:
  Example:
*********************************************************************************/
void V558_CtrlGetChipClk(UINT32* clkin, UINT32* clkout, UINT8* factorh, UINT8* factorl)
{
	*clkin = g_CtrlContext.pInfo->Clk.clkin;
	*clkout = g_CtrlContext.pInfo->Clk.clkout;
	*factorh = g_CtrlContext.pInfo->Clk.factorh;
	*factorl = g_CtrlContext.pInfo->Clk.factorl;
}

/********************************************************************************
  Description:
	Set chip clock on
  Parameters:
		clkout: ouput clock(KHZ)
  Note:
		<TABLE>
		This function can make sure such conditions:
		clk----->/-----------[idiv]-------->[pll]------/----------mclk(out)--->
		Pll clock output clk mclk calculate as:
		pllin = clkin/idiv;
		mclk = (pllin*fbdiv)/(plldivh+plldivl+2);
		(pllin*fbdiv must >65 and < 130)
		</TABLE>
  Returns:
	Return actual clock(KHZ)
  Remarks:
  Example:
		<TABLE>
		if clkin(set before) = 12 requieted mclk 24, can call function as:
		{
		V558_CtrlChangeChipClk(24000);
		}
		</TABLE>
*********************************************************************************/
UINT16 V558_CtrlChangeChipClk(UINT32 clkout)
{
	CLK_OUT_PARM clkoutparm;
	ChipClkCreate(g_CtrlContext.pInfo->Clk.clkin, clkout, g_CtrlContext.pInfo->Clk.factorh, g_CtrlContext.pInfo->Clk.factorl, &clkoutparm);
	if(clkoutparm.clkoutput==0)
	{
		return clkoutparm.clkoutput;
	}

	g_CtrlContext.pInfo->Clk.clkout = clkout;

	V558_CtrlSetChipClkOff();
	if(clkoutparm.idiv)
	{
		V558_CtrlChipClkIDivEnable();
	}
	else
	{
		V558_CtrlChipClkIDivDisable();
	}
	V558_CtrlSetChipClkIDiv(clkoutparm.idiv);
	V558_CtrlSetChipClkPlldiv(clkoutparm.fdiv, clkoutparm.plldivh, clkoutparm.plldivl);
	V558_CtrlChipClkPlldivEnable();
	V558_CtrlSetChipClkOn();

	return clkoutparm.clkoutput;
}

/********************************************************************************
  Description:
	Set chip clock on
  Parameters:
  Note:
		<TABLE>
		clk----->/-----------[idiv]-------->[pll]------/----------mclk(out)--->
		Pll clock output clk mclk calculate as:
		pllin = clkin/idiv;
		mclk = (pllin*fbdiv)/(plldivh+plldivl+2);
		(pllin*fbdiv must >65 and < 130)
		</TABLE>
  Returns:
  Remarks:
  Example:
		<TABLE>
		if clkin = 12 requieted mclk 24, can call function as:
		{
		V558_CtrlSetChipClkIDiv(0x2)
		V558_CtrlChipClkIDivEnable();
		V558_CtrlSetChipClkPlldiv(0x10,0x1,0x1);
		V558_CtrlChipClkPlldivEnable();
		V558_CtrlSetChipClkOn();
		}
		pllin = clkin/2 = 12/2 =6;
		pllin*fbdiv = pllin*0x10 = 96; (>60 && <130)
		mclk = 96/(1+1+2) = 96/4=24;
		</TABLE>
*********************************************************************************/
void V558_CtrlSetChipClkOn()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_CLKOFF, &uReg);
	V558_SetReg(V558_REG_HCTRL_CLKOFF, 0xff);	//close module clk

	V558_SetReg(V558_REG_HCTRL_CLKOFF, uReg);
	V558_SetReg(V558_REG_HCTRL_CLKCTRL0,0xfe);	//power down;pll bypass;close pllin;close pllout
	V558_SetReg(V558_REG_HCTRL_CLKCTRL0,0x02);	//Open pll out(mclk)

	V558_Delay(100);
	V558_SetReg(V558_REG_HCTRL_CLKCTRL0,0x00);	//power on;open pllin;pll work;

	V558_Delay(50);
	V558_SetReg(V558_REG_HCTRL_CLKOFF, uReg);
	V558_Delay(50);
}

/********************************************************************************
  Description:
	Set chip clock off
  Parameters:
  Note:
	When chip'clock is off, the chip cann't work
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSetChipClkOff()
{
	V558_SetReg(V558_REG_HCTRL_CLKCTRL0,0xc3);	//power down;pll bypass;close pllin;close pllout
}


/********************************************************************************
  Description:
	Change 558 chip clock pll division
  Parameters:
	fbdiv: feedback div
	plldivh: plldiv high byte
	plldivl: plldiv low byte
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlChangeChipClkPlldiv(UINT8 fbdiv, UINT8 plldivh, UINT8 plldivl)
{
	V558_CtrlSetChipClkOff();
	V558_CtrlSetChipClkPlldiv(fbdiv, plldivh, plldivl);
	V558_CtrlSetChipClkOn();
}


/********************************************************************************
  Description:
	Set 558 gpio config
  Parameters:
	cfg: gpio config,bit active '1'=Gpio mode
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSetGpioCfg(UINT8 cfg)
{
	V558_SetReg(V558_REG_HCTRL_GPIO_CFG, cfg);
}

/********************************************************************************
  Description:
	Get current 558 gpio config
  Parameters:
  Note:
  Returns:
	gpio config
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetGpioCfg()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_GPIO_CFG, &uReg);
	return uReg;
}

/********************************************************************************
  Description:
	Set 558 gpio mode
  Parameters:
	mode: gpio mode,bit active '1'=Open drain mode
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSetGpioMode(UINT8 mode)
{
	V558_SetReg(V558_REG_HCTRL_GPIO_MODE, mode);
}

/********************************************************************************
  Description:
	Get current 558 gpio mode
  Parameters:
  Note:
  Returns:
	gpio mode
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetGpioMode()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_GPIO_MODE, &uReg);


	return uReg;
}

/********************************************************************************
  Description:
	Set 558 gpio direction
  Parameters:
	dir-- gpio direction,bit active '1'=output '0'=input
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSetGpioDirection(UINT8 dir)
{
	V558_SetReg(V558_REG_HCTRL_GPIO_DIR, dir);
}

/********************************************************************************
  Description:
	Get current 558 gpio direction
  Parameters:
  Note:
  Returns:
	gpio direction
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetGpioDirection()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_GPIO_DIR, &uReg);
	return uReg;
}

/********************************************************************************
  Description:
	output 558 gpio data
  Parameters:
  Note:
  Returns:
	data: gpio data(4bit)
  Remarks:
*********************************************************************************/
void V558_CtrlSetGpioData(UINT8 data)
{
	V558_SetReg(V558_REG_HCTRL_GPIO_P0, data);
}

/********************************************************************************
  Description:
	Get 558 gpio input data
  Parameters:
  Note:
  Returns:
	gpio input data(4bit)
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetGpioData()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_GPIO_P0, &uReg);
	return uReg;
}

/********************************************************************************
  Description:
	Set 558 interrupt control
  Parameters:
	LeverOrEdge: Trigger type '1' level '0' edge
	HighOrLow: Level low or high; Trigger rise or fall
	Width: Level active width
  Note:
  Returns:
	gpio input data(4bit)
  Remarks:
*********************************************************************************/
void V558_CtrlSetIntCtrl(UINT8 LeverOrEdge,UINT8 HighOrLow, UINT8 Width)
{
	UINT16 Temp;

	if(LeverOrEdge)		//lever
	{
		if(HighOrLow)		//high
		{
			Temp = 0x1;
		}
		else
		{
			Temp = 0x3;
		}
	}
	else
	{
		Temp = 0x0;
		if(HighOrLow)		//high
		{
			Temp = 0x0;
		}
		else
		{
			Temp = 0x4;
		}
	}
	if(Width)
	{
		Width--;
	}
	Width<<=3;
	Temp|=Width;
	V558_SetReg((UINT16)V558_REG_HCTRL_INTCTRL,Temp);

}

/********************************************************************************
  Description:
	Set 558 first level interrupt for each modules
  Parameters:
	uVal: enable interrupt to first level, corresponding bit '1' enable '0' disable
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSetIntEnFirst(UINT8 uVal)
{
	V558_SetReg(V558_REG_HCTRL_INTEN, uVal);
}

/********************************************************************************
  Description:
	Get 558 first level interrupt for each modules
  Parameters:
  Note:
  Returns:
	first level interrupt is enabled or disabled, corresponding bit '1' enable '0' disable
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetIntEnFirst()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_INTEN, &uReg);
	return uReg;
}

/********************************************************************************
  Description:
	Set 558 interrupt serve flag
  Parameters:
	uVal sever flag corresponding bit '1' enable '0'
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSetIntServe(UINT8 uVal)
{
	V558_SetReg(V558_REG_HCTRL_INTSERV, uVal);
}

/********************************************************************************
  Description:
	Get 558 interrupt serve flag
  Parameters:
  Note:
  Returns:
	sever flag corresponding bit '1' enable '0'
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetIntServe()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_INTSERV, &uReg);
	return uReg;
}

/********************************************************************************
  Description:
	Get 558 interrupt source status for each chip module
  Parameters:
  Note:
  Returns:
	sever interrupt status corresponding bit '1' enable '0'
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetIntFlagFirst()
{
	UINT8 uReg = 0;
	V558_GetReg(V558_REG_HCTRL_INTFLAG, &uReg);
	return uReg;
}

/********************************************************************************
  Description:
	Set 558 first level interrupt  for one module by type
  Parameters:
	nFirstType: module type

	uVal: module interrupt is enabled or disabled, corresponding bit '1' enable '0' disable
  Note:

  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlSetIntEnSecend(UINT8 nFirstType, UINT8 uVal)
{
	V558_SetReg((V558_REG_HCTRL_INTEN0 + nFirstType*4), uVal);
}

/********************************************************************************
  Description:
	Get 558 first level interrupt  for one module by type
  Parameters:
	nFirstType: module type
  Note:
  Returns:
	module interrupt is enabled or disabled, corresponding bit '1' enable '0' disable
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetIntEnSecend(UINT8 nFirstType)
{
	UINT8 uReg = 0;
	V558_GetReg((V558_REG_HCTRL_INTEN0 + nFirstType*4), &uReg);
	return uReg;
}

/********************************************************************************
  Description:
	Get 558 interrupt source status for one module by type
  Parameters:
	nFirstType: module type
  Note:
  Returns:
	sever interrupt status corresponding bit '1' enable '0'
  Remarks:
*********************************************************************************/
UINT8 V558_CtrlGetIntFlagSecend(UINT8 nFirstType)
{
	UINT8 uReg = 0;
	V558_GetReg((V558_REG_HCTRL_INTFLAG0 + nFirstType*4), &uReg);
	return uReg;
}

/********************************************************************************
  Description:
	Enable jpeg bit rate control
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlEnableBRC(void) 
{ 
	UINT8 uMode = 0; 
	
    V558_GetReg(V558_REG_BIU_OP_MODE, &uMode); 
	uMode |= 0x10; 
    V558_SetReg(V558_REG_BIU_OP_MODE, uMode); 
}

/********************************************************************************
  Description:
	Disable jpeg bit rate control
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V558_CtrlDisableBRC(void) 
{ 
	UINT8 uMode = 0; 

	V558_GetReg(V558_REG_BIU_OP_MODE, &uMode); 
	uMode &= 0xef; 
	V558_SetReg(V558_REG_BIU_OP_MODE, uMode); 

	V558_CtrlSwReset(V558_SW_RESET_LB);
} 





//==============================================================================//
//  private function of CTRL:config v558 chip or some figure no need for user    //
//==============================================================================//

/********************************************************************************
  Description:
	Set Chip configuration
  Parameters:
	uSel:	1--558,0--559
  Returns:
  Remarks:
*********************************************************************************/
void CtrlSetChipSel(UINT8 uSel)
{
	UINT8 uMode = 0;
	V558_GetReg(V558_REG_BIU_OP_MODE, &uMode);
	if(uSel&0x1)
	{
		uMode |= 0x8;
		V558_SetReg(V558_REG_BIU_OP_MODE, uMode);
	}
	else
	{
		uMode &= 0xf7;
		V558_SetReg(V558_REG_BIU_OP_MODE, uMode);
	}
}

/********************************************************************************
  Description:
	Get Chip configuration
  Parameters:
  Returns:
	uSel:	Pointer to the value. 1--558,0--559
  Remarks:
*********************************************************************************/
void CtrlGetChipSel(UINT8* uSel)
{
	UINT8 uMode = 0;
	V558_GetReg(V558_REG_BIU_OP_MODE, &uMode);
	if(uMode & 0x8)
	{
		*uSel = 1;
	}
	else
	{
		*uSel = 0;
	}
}

/********************************************************************************
  Description:
	Set asynchronous and synchronous interface
  Parameters:
	uSel: value:  0: asynchronous 1: synchronous
  Returns:
  Remarks:
*********************************************************************************/
void CtrlSetInterfaceAsynOrSyc(UINT8 uVal)
{
	V558_SetReg(V558_REG_BIU_SEL_ASYN_SYN, uVal);
}

/********************************************************************************
  Description:
	Get asynchronous and synchronous interface setting
  Parameters:
  Returns:
	uSel:	Pointer to the value, 0: asynchronous 1: synchronous
  Remarks:
*********************************************************************************/
UINT8 CtrlGetInterfaceAsynOrSyc(void)
{
	UINT8 Reg = 0;
	V558_GetReg(V558_REG_BIU_SEL_ASYN_SYN, &Reg);
	Reg &= 0x1;
	return Reg;
}

/********************************************************************************
  Description:
	Set host write signal drive
  Parameters:
	uSel: value
  Returns:
  Remarks:
*********************************************************************************/
void CtrlSetWeDrive(UINT8 uVal)
{
	V558_SetReg(V558_REG_BIU_WE_DRIVE, uVal);
}

/********************************************************************************
  Description:
	Get host write signal drive
  Parameters:
  Returns:
	uSel:	Pointer to the value
  Remarks:
*********************************************************************************/
UINT8 CtrlGetWeDrive(void)
{
	UINT8 Reg = 0;
	V558_GetReg(V558_REG_BIU_WE_DRIVE, &Reg);
	return Reg;
}

/********************************************************************************
  Description:
	Set host I/F control
  Parameters:
	uSel: value
  Returns:
  Remarks:
*********************************************************************************/
void CtrlSetCpuPinCtrl(UINT8 uVal)
{
	V558_SetReg(V558_REG_HCTRL_CPU_PC, uVal);
}

/********************************************************************************
  Description:
	Get host I/F pin control
  Parameters:
  Returns:
	uSel:	Pointer to the value
  Remarks:
*********************************************************************************/
UINT8 CtrlGetCpuPinCtrl(void)
{
	UINT8 Reg = 0;
	V558_GetReg(V558_REG_HCTRL_CPU_PC, &Reg);
	return Reg;
}

/********************************************************************************
  Description:
	Set lcd pin control
  Parameters:
	uSel: value
  Returns:
  Remarks:


*********************************************************************************/
void CtrlSetLcdPinCtrl(UINT8 uVal)
{
	V558_SetReg(V558_REG_HCTRL_LCD_PC, uVal);
}

/********************************************************************************
  Description:
	Get lcd pin control
  Parameters:
  Returns:
	uSel:	Pointer to the value
  Remarks:
*********************************************************************************/
UINT8 CtrlGetLcdPinCtrl(void)
{
	UINT8 Reg = 0;
	V558_GetReg(V558_REG_HCTRL_LCD_PC, &Reg);
	return Reg;
}

/********************************************************************************
  Description:
	Set sif pin control
  Parameters:
	uSel: value
  Returns:
  Remarks:
*********************************************************************************/
void CtrlSetSnrPinCtrl(UINT8 uVal)
{
	V558_SetReg(V558_REG_HCTRL_CS_PC, uVal);
}

/********************************************************************************
  Description:
	Get sif pin control
  Parameters:
  Returns:
	uSel:	Pointer to the value
  Remarks:
*********************************************************************************/
UINT8 CtrlGetSnrPinCtrl(void)
{
	UINT8 Reg = 0;
	V558_GetReg(V558_REG_HCTRL_CS_PC, &Reg);
	return Reg;
}

/********************************************************************************
  Description:
	Set umonitor control(for debug)
  Parameters:
	uSel: value
  Returns:
  Remarks:
*********************************************************************************/
void CtrlSetUmCtrl(UINT8 uVal)
{
	V558_SetReg(V558_REG_HCTRL_UMCTRL, uVal);
}

/********************************************************************************
  Description:
	Get umonitor control(for debug)
  Parameters:
  Returns:
	uSel:	Pointer to the value
  Remarks:
*********************************************************************************/
UINT8 CtrlGetUmCtrl(void)
{
	UINT8 Reg = 0;
	V558_GetReg(V558_REG_HCTRL_UMCTRL, &Reg);
	return Reg;
}

/********************************************************************************
  Description:
	Set miscellaneous control(for debug)
  Parameters:
	uSel: value
  Returns:
  Remarks:
*********************************************************************************/
void CtrlSetMiscCtrl(UINT8 uVal)
{
	V558_SetReg(V558_REG_HCTRL_MISC_CON, uVal);
}

/********************************************************************************
  Description:
	Get miscellaneous control(for debug)
  Parameters:
  Returns:
	uSel:	Pointer to the value
  Remarks:
*********************************************************************************/
UINT8 CtrlGetMiscCtrl(void)
{
	UINT8 Reg = 0;
	V558_GetReg(V558_REG_HCTRL_MISC_CON, &Reg);
	return Reg;
}

UINT16 AdjustFactor(UINT16 m, UINT16 n, UINT16 p)
{
	UINT16 q = 1;
	if(m < p)
	{
		if(n < p)
		{
			if(p-m > p-n)
			{
				q = 0;
			}
		}
		else
		{
			if(p-m > n-p)
			{
				q = 0;
			}
		}
	}
	else
	{
		if(n < p)
		{
			if(m-p > p-n)
			{
				q = 0;
			}
		}
		else
		{
			if(m-p > n-p)
			{
				q = 0;
			}
		}
	}
	return q;
}

void ChipClkCreate(UINT32 clkin, UINT32 clkout, UINT8 factorh, UINT8 factorl, PCLK_OUT_PARM clkoutparm)
{
	UINT32 clkdiff, clkdifflast, clkouttemp;
	UINT8 plldivltemp, plldivhtemp;
	UINT32 pllin;

	UINT16 i, j, k;
	UINT16 m,n,p,q;

	clkoutparm->clkoutput = 0;
	clkdifflast = clkout;

	for(i=CHIP_IDIV_MAX; i>=CHIP_IDIV_MIN; i--)
	{
		for(j=CHIP_FDIV_MAX; j>=CHIP_FDIV_MIN; j--)
		{
			pllin = ((clkin*j)/i);
			k = (UINT8)(pllin/clkout);
			if((pllin < CHIP_PLLIN_MIN || pllin > CHIP_PLLIN_MAX)
				||(k<CHIP_PLLDIV_MIN || k>CHIP_PLLDIV_MAX))
			{
				continue;
			}
			clkouttemp = ((clkin*j)/(i*k));
			if(clkouttemp >= clkout)
			{
				clkdiff = clkouttemp - clkout;
			}
			else
			{
				clkdiff = clkout - clkouttemp;
			}
			if(clkdiff < clkdifflast)
			{
				clkdifflast = clkdiff;
				clkoutparm->idiv = i;
				clkoutparm->fdiv = j;
				clkoutparm->plldivh = (k*factorh)/(factorh+factorl);
				clkoutparm->plldivl = k - clkoutparm->plldivh;
				if(clkoutparm->plldivh > CHIP_PLLDIV_HL_MAX)
				{
					clkoutparm->plldivh = CHIP_PLLDIV_HL_MAX;
					clkoutparm->plldivl = k - clkoutparm->plldivh;
				}
				if(clkoutparm->plldivl > CHIP_PLLDIV_HL_MAX)
				{
					clkoutparm->plldivl = CHIP_PLLDIV_HL_MAX;
					clkoutparm->plldivh = k - clkoutparm->plldivl;
				}
				if(clkoutparm->plldivh < CHIP_PLLDIV_HL_MIN)
				{
					clkoutparm->plldivh = CHIP_PLLDIV_HL_MIN;
					clkoutparm->plldivl = k - clkoutparm->plldivh;
				}
				if(clkoutparm->plldivl < CHIP_PLLDIV_HL_MIN)
				{
					clkoutparm->plldivl = CHIP_PLLDIV_HL_MIN;
					clkoutparm->plldivh = k - clkoutparm->plldivl;
				}
				
				clkoutparm->clkoutput = clkouttemp;
			}
			else
			{
				if(clkdiff != clkdifflast)
				{
					continue;
				}

				plldivhtemp = (k*factorh)/(factorh+factorl);
				plldivltemp = k - plldivhtemp;
				if(plldivhtemp > CHIP_PLLDIV_HL_MAX)
				{
					plldivhtemp = CHIP_PLLDIV_HL_MAX;
					plldivltemp = k - plldivhtemp;
				}
				if(plldivltemp > CHIP_PLLDIV_HL_MAX)
				{
					plldivltemp = CHIP_PLLDIV_HL_MAX;
					plldivhtemp = k - plldivltemp;
				}
				if(plldivhtemp < CHIP_PLLDIV_HL_MIN)
				{
					plldivhtemp = CHIP_PLLDIV_HL_MIN;
					plldivltemp = k - plldivhtemp;
				}
				if(plldivltemp < CHIP_PLLDIV_HL_MIN)
				{
					plldivltemp = CHIP_PLLDIV_HL_MIN;
					plldivhtemp = k - plldivltemp;
				}
				
				m = (UINT16)((plldivhtemp+plldivltemp)*0x100)/plldivhtemp;
				n = (UINT16)((clkoutparm->plldivh+clkoutparm->plldivl)*0x100)/clkoutparm->plldivh;
				p = (UINT16)((factorh+factorl)*0x100)/factorh;
				
				q = AdjustFactor(m, n, p);
				if(q)
				{
					clkoutparm->plldivh = plldivhtemp;
					clkoutparm->plldivl = plldivltemp;
					
					clkoutparm->idiv = i;
					clkoutparm->fdiv = j;
					clkoutparm->clkoutput = clkouttemp;
				}
			}
		}
	}
	if(clkoutparm->idiv == 1)
	{
		clkoutparm->idiv = 0;
	}
	clkoutparm->plldivh -= 1;
	clkoutparm->plldivl -= 1;
}
