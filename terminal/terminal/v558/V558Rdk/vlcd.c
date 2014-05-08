#include <string.h>
#include "v558api.h"
#include "vlcd.h"
#include "vregdef.h"





V558_LCD_CONTEXT	gLcdContext;

/********************************************************/
//														//
//	internal function of Lcd module						//
//														//
/********************************************************/
#define AL_UPDATEREG()		V558_SetReg(V558_REG_LCD_LCDTEST, 0X80)

/********************************************************************************

	Description:
		delay "delay" ms

	Parameters:
		delay:	the delay time

	Remarks:
		state: ok

*******************************************************************************/
#define LcdDelay(x)		V558_Delay(x*70)

/********************************************************************************

	Description:
		get color index

	Parameters:
		colorDepth:	input color depth

	Note:
		mono return 6

	Return Value List:
	   0xff	:  if error
	   index:  otherwise

	Remarks:
		state: checking

*******************************************************************************/
UINT8	GetColorIndex(UINT8 colorDepth)
{
	if(colorDepth == 4)
		return 0;
	if(colorDepth == 8)	
		return 1;
	if(colorDepth == 16)	
		return 2;
	if(colorDepth == 15)
		return 3;
	if(colorDepth == 24)
		return 4;
	if(colorDepth == 32)
		return 5;
	if(colorDepth == 1)		//mono color
		return 6;
	return 0xff;
}

/********************************************************************************

	Description:
		get color depth

	Parameters:
		colorIndex:	input color index

	Note:
		mono return 1

	Return Value List:
	   0			:  if error
	   color depth	:  otherwise

	Remarks:
		state: checking

*******************************************************************************/
UINT8	GetColordepth(UINT8 colorIndex)
{
	colorIndex &= 0xf;

	if(colorIndex == 0)
		return 4;
	if(colorIndex == 1)	
		return 8;
	if(colorIndex == 2)	
		return 16;
	if(colorIndex == 3)
		return 15;
	if(colorIndex == 4)
		return 24;
	if(colorIndex == 5)
		return 32;
	if(colorIndex == 6)		//mono color
		return 1;
	return 0;
}

/********************************************************************************

	Description:
		get memory span from color depth

	Parameters:
		width:	image width
		colordepth:	image clolor depth

	Returns
	   the memory span of input setting

	Remarks:
		state: checking

*******************************************************************************/
UINT16	GetSpanFromDepth(UINT16 width, UINT8 colordepth)
{
	if(colordepth != 15)
		return (width * colordepth) >> 3;
	return width << 1;
}

/********************************************************************************

	Description:
		get memory span from color index

	Parameters:
		width:	image width
		colorIndex:	image clolor index

	Returns
	   the memory span of input setting

	Remarks:
		state: checking

*******************************************************************************/
UINT16	GetSpanFromIndex(UINT16 width, UINT8 colorIndex)
{
	if(0 == colorIndex)
		return width >> 1;
	if(1 == colorIndex)
		return width;
	if(2 == colorIndex)
		return width << 1;
	if(3 == colorIndex)
		return width << 1;
	if(4 == colorIndex)
		return width + (width << 1);
	if(5 == colorIndex)
		return width << 2;
	return width >> 3;
}









/********************************************************/
//														//
//	global function of Lcd module						//
//														//
/********************************************************/




/********************************************************************************

	Description:
		set lcd panel write function

	Parameters:
		func:	lcd panel write function's point

	Remarks:
		state: ok

	Example:
	void V558_LcdPanelSetReg(UINT16 adr, UINT16 val)
	{
		PanelWrRsClr(adr);
		PanelWrRsSet(val);
	}

	void test(void)
	{
		V558_PANELPARM parm;
	
		parm.len = sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE);
		parm.regval = Panel_powon;
		V558_SelectPanel(0);		//select current panel as main panel
		V558_LcdSetLcdIFParm(Panel_LcdIF);
		V558_LcdSetRWpanelFunc(V558_LcdPanelSetReg);	//set panel r/w function
		V558_SetPanelRegList(parm);					//power on the panel
	}

*******************************************************************************/
void	V558_LcdSetRWpanelFunc(PPanelSetRegFunc func)
{
	gLcdContext.pPanelSetReg = func;
}

/********************************************************************************

	Description:
		write panel's register function

	Parameters:
		parm:	register list point

	Note:
		The v558 chip must work in through mode

	Remark:
		state: checking

	Example:
	void V558_LcdPanelSetReg(UINT16 adr, UINT16 val)
	{
		PanelWrRsClr(adr);
		PanelWrRsSet(val);
	}

	void test(void)
	{
		V558_PANELPARM parm;
	
		parm.len = sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE);
		parm.regval = Panel_powon;
		V558_SelectPanel(0);		//select current panel as main panel
		V558_LcdSetLcdIFParm(Panel_LcdIF);
		V558_LcdSetRWpanelFunc(V558_LcdPanelSetReg);	//set panel r/w function
		V558_SetPanelRegList(parm);					//power on the panel
	}
*******************************************************************************/
void V558_SetPanelRegList(V558_PANELPARM parm)
{
	UINT16 i;

	if(parm.regval == NULL)
		return;

	if(gLcdContext.pPanelSetReg == NULL)
		return;
	for(i = 0; i < parm.len; i++)
	{
		gLcdContext.pPanelSetReg( (parm.regval)[i].adr, (parm.regval)[i].val );
		LcdDelay( (parm.regval)[i].wait );
	}
}

/********************************************************************************

	Description:
		Initialize Lcd module's context

	Note:
		This function must be called when system is power on. Indict the Lcd 
		module's context is not valid.

	Remarks:
		state: checking

	Example:
	Power on process
	void power(void)
	{
		V558_LcdInitContext();
		V558_LcdSetPanelInfo(Panel_Info);
		V558_LcdSetSlavePanelInfo(SlavePanel_Info);
	}
*******************************************************************************/
void V558_LcdInitContext(void)
{
	memset(&gLcdContext, 0, sizeof(V558_LCD_CONTEXT));
	gLcdContext.panelcur = gLcdContext.panelInfo;
//	gLcdContext.panelValid = FALSE;
}

/********************************************************************************

	Description:
		Application layer send initialize data to main Lcd module, 
		the driver store the panel info in module.

	Parameters:
		info:	point to the start address of lcd module's initialize data

	Note:
		This function must be called before use other function of this RDK,
		or the lcd module will be abnormal.

	Return Value List:
	   SUCCEED	:  if succesful
	   LCDFAILED:  otherwise

	Remarks:
		state: valid

	Example:
	Power on process
	void power(void)
	{
		V558_LcdInitContext();
		V558_LcdSetPanelInfo(Panel_Info);
		V558_LcdSetSlavePanelInfo(SlavePanel_Info);
	}
*******************************************************************************/
UINT16 V558_LcdSetPanelInfo(PV558_PANELINFO info)
{
#if 0
#define SYSTEM_HAS_MALLOC		0

#if	SYSTEM_HAS_MALLOC
	//if the system support malloc(), this can be changed
#else
	static V558_PANELREGVALUE	PanelPowon[120];
	static V558_PANELREGVALUE	PanelPowoff[10];
	static V558_PANELREGVALUE	PanelSleep[10];
	static V558_PANELREGVALUE	PanelWakefs[120];
	static V558_PANELREGVALUE	PanelStdby[10];
	static V558_PANELREGVALUE	PanelWakefstd[120];
#endif

	V558_PANELINFO *pPif = &(gLcdContext.panelInfo);

	memcpy( pPif, info, sizeof(V558_PANELINFO) );
	pPif->lcdif->lc &= 0xffef;
	memcpy( PanelPowon, pPif->powon.regval, pPif->powon.len * sizeof(V558_PANELREGVALUE) );
	pPif->powon.regval = PanelPowon;
	memcpy( PanelPowoff, pPif->powoff.regval, pPif->powoff.len * sizeof(V558_PANELREGVALUE) );
	pPif->powoff.regval = PanelPowoff;
	memcpy( PanelSleep, pPif->sleep.regval, pPif->sleep.len * sizeof(V558_PANELREGVALUE) );
	pPif->sleep.regval = PanelSleep;
	memcpy( PanelWakefs, pPif->wakefs.regval, pPif->wakefs.len * sizeof(V558_PANELREGVALUE) );
	pPif->wakefs.regval = PanelWakefs;
	memcpy( PanelStdby, pPif->stdby.regval, pPif->stdby.len * sizeof(V558_PANELREGVALUE) );
	pPif->stdby.regval = PanelStdby;
	memcpy( PanelWakefstd, pPif->wakefstd.regval, pPif->wakefstd.len * sizeof(V558_PANELREGVALUE) );
	pPif->wakefstd.regval = PanelWakefstd;
#endif

	gLcdContext.panelInfo = info;
	gLcdContext.panelValid |= 1;

	return SUCCEED;
}

/********************************************************************************

	Description:
		Application layer send initialize data to sub-Lcd module, 
		the driver store the panel info in module.

	Parameters:
		info:	point to the start address of lcd module's initialize data

	Note:
		This function must be called before use other function of this RDK,
		or the lcd module will be abnormal.

	Return Value List:
	   SUCCEED	:  if succesful
	   LCDFAILED:  otherwise

	Remarks:
		state: valid

	Example:
	Power on process
	void power(void)
	{
		V558_LcdInitContext();
		V558_LcdSetPanelInfo(Panel_Info);
		V558_LcdSetSlavePanelInfo(SlavePanel_Info);
	}
*******************************************************************************/
UINT16 V558_LcdSetSlavePanelInfo(PV558_PANELINFO info)
{
#if 0
#if	SYSTEM_HAS_MALLOC
	//if the system support malloc(), this can be changed
#else
	static V558_PANELREGVALUE	PanelPowon[120];
	static V558_PANELREGVALUE	PanelPowoff[10];
	static V558_PANELREGVALUE	PanelSleep[10];
	static V558_PANELREGVALUE	PanelWakefs[120];
	static V558_PANELREGVALUE	PanelStdby[10];
	static V558_PANELREGVALUE	PanelWakefstd[120];
#endif

	V558_PANELINFO *pPif = &(gLcdContext.slavepanelInfo);

	memcpy( pPif, info, sizeof(V558_PANELINFO) );
	pPif->lcdif->lc |= 0x10;
	memcpy( PanelPowon, pPif->powon.regval, pPif->powon.len * sizeof(V558_PANELREGVALUE) );
	pPif->powon.regval = PanelPowon;
	memcpy( PanelPowoff, pPif->powoff.regval, pPif->powoff.len * sizeof(V558_PANELREGVALUE) );
	pPif->powoff.regval = PanelPowoff;
	memcpy( PanelSleep, pPif->sleep.regval, pPif->sleep.len * sizeof(V558_PANELREGVALUE) );
	pPif->sleep.regval = PanelSleep;
	memcpy( PanelWakefs, pPif->wakefs.regval, pPif->wakefs.len * sizeof(V558_PANELREGVALUE) );
	pPif->wakefs.regval = PanelWakefs;
	memcpy( PanelStdby, pPif->stdby.regval, pPif->stdby.len * sizeof(V558_PANELREGVALUE) );
	pPif->stdby.regval = PanelStdby;
	memcpy( PanelWakefstd, pPif->wakefstd.regval, pPif->wakefstd.len * sizeof(V558_PANELREGVALUE) );
	pPif->wakefstd.regval = PanelWakefstd;
#endif

	gLcdContext.slavepanelInfo = info;
	gLcdContext.panelValid |= 2;

	return SUCCEED;
}

/********************************************************************************

	Description:
		Initialize the register of lcd module, include the register about
		lcd panel, and the other default setting of internal register of 
		lcd module, such as gamma table.

	Note:
		if we reset lcd module, but wont change the content of lcd panel, 
		we should call this function to make our lcd module to it's default
		setting.

	Remarks:
		state: checking

*******************************************************************************/
void V558_LcdOpenModule(void)
{
	V558_LcdSetLcdIFParm(gLcdContext.panelcur->lcdif);
	V558_LcdSetIFControl(gLcdContext.lc);
}

/********************************************************************************

	Description:
		Initialize v558's lcd I/F register about lcd panel

	Parameters:
		pPif:	point to the start address of lcd I/F module's initialize data

	Remarks:
		state: valid

	Example:
	void V558_LcdPanelSetReg(UINT16 adr, UINT16 val)
	{
		PanelWrRsClr(adr);
		PanelWrRsSet(val);
	}

	void test(void)
	{
		V558_PANELPARM parm;
	
		parm.len = sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE);
		parm.regval = Panel_powon;
		V558_SelectPanel(0);		//select current panel as main panel
		V558_LcdSetLcdIFParm(Panel_LcdIF);
		V558_LcdSetRWpanelFunc(V558_LcdPanelSetReg);	//set panel r/w function
		V558_SetPanelRegList(parm);					//power on the panel
	}
*******************************************************************************/
void V558_LcdSetLcdIFParm(V558_LCDIFPARM *pPif)
{
	V558_LAYER_PROPERTY proty;

	V558_SetReg(V558_REG_LCD_WCONT		,  (UINT8)(pPif->wrcycle) );
	V558_SetReg(V558_REG_LCD_WCONT+1	,  (UINT8)(pPif->wrcycle >> 8) );
	V558_SetReg(V558_REG_LCD_RCONT		,  (UINT8)(pPif->rdcycle) );
	V558_SetReg(V558_REG_LCD_RCONT+1	,  (UINT8)(pPif->rdcycle >> 8) );
	V558_SetReg(V558_REG_LCD_DRS		,  pPif->rsPolarity );

	V558_SetReg( V558_REG_LCD_HSA	, (UINT8)(pPif->hsa) );
	V558_SetReg( V558_REG_LCD_HSA+1	, (UINT8)(pPif->hsa >> 8) );
	V558_SetReg( V558_REG_LCD_HEA	, (UINT8)(pPif->hea) );
	V558_SetReg( V558_REG_LCD_HEA+1	, (UINT8)(pPif->hea >> 8) );
	V558_SetReg( V558_REG_LCD_HSXA	, (UINT8)(pPif->hsxa) );
	V558_SetReg( V558_REG_LCD_HSXA+1, (UINT8)(pPif->hsxa >> 8) );
	V558_SetReg( V558_REG_LCD_HSYA	, (UINT8)(pPif->hsya) );
	V558_SetReg( V558_REG_LCD_HSYA+1, (UINT8)(pPif->hsya >> 8) );
	V558_SetReg( V558_REG_LCD_HEXA	, (UINT8)(pPif->hexa) );
	V558_SetReg( V558_REG_LCD_HEXA+1, (UINT8)(pPif->hexa >> 8) );
	V558_SetReg( V558_REG_LCD_HEYA	, (UINT8)(pPif->heya) );
	V558_SetReg( V558_REG_LCD_HEYA+1, (UINT8)(pPif->heya >> 8) );
	V558_SetReg( V558_REG_LCD_WRI	, (UINT8)(pPif->dispadr) );
	V558_SetReg( V558_REG_LCD_WRI+1	, 0 );
	V558_SetReg( V558_REG_LCD_LC	, (UINT8)(pPif->lc) );
	V558_SetReg( V558_REG_LCD_LC+1	, (UINT8)(pPif->lc >> 8) );
	V558_SetReg( V558_REG_LCD_DMAP	, pPif->dmap );
	V558_SetReg( V558_REG_LCD_DPAD	, (UINT8)(pPif->dpad) );
	V558_SetReg( V558_REG_LCD_DPAD+1, (UINT8)(pPif->dpad >> 8) );
	V558_SetReg( V558_REG_LCD_INDEXSEL, pPif->idsel );

	V558_LcdSetDisPeriod(pPif->size);
	V558_LcdSetDispMode(10);
//	V558_SetReg(V558_REG_LCD_LCDRST		,  0x1);

	proty.Size.cx = pPif->size.cx;
	proty.Size.cy = pPif->size.cy;
	proty.DisPoint.x = 0;
	proty.DisPoint.y = 0;
	proty.DisRect.top = 0;
	proty.DisRect.left = 0;
	proty.DisRect.width = pPif->size.cx;
	proty.DisRect.height = pPif->size.cy;
	proty.Format = DEFAULT_BLFORMAT;	//565 format, combine two plane;
	V558_LcdSetBLProperty(&proty);
	V558_LcdALDisable();
	V558_LcdSetGammaTable(	pPif->gammar,
		pPif->gammag,
		pPif->gammab);
}

/********************************************************************************

	Description:
		Open lcd module

	Note:
		\1. Initialize Lcd panel's register.

		\2. Initialize v558's internal register about lcd panel.

		\3.	Set some register to its default value.

		\4. Set lcd work state.

	Return Value List:
	   SUCCEED	:  if succesful
	   LCDFAILED:  otherwise

	Remarks:
		state: checking

*******************************************************************************/
UINT16 V558_LcdOpen(void)
{
	if( gLcdContext.panelValid )
	{
		V558_LcdPanelReset();

		if(gLcdContext.panelValid & 2)
		{
			gLcdContext.panelcur = gLcdContext.slavepanelInfo;
			gLcdContext.pPanelSetReg = gLcdContext.slavepanelInfo->pPanelSetReg;
			gLcdContext.lc = gLcdContext.slavepanelInfo->lcdif->lc | 0x10;
			V558_LcdOpenModule();
			V558_LcdSetPanelState(V558_LCDPANEL_POWON);
			V558_LcdSetBGColor(gLcdContext.slavepanelInfo->lcdif->bkcolor);
			V558_LcdBgEnable();
			V558_LcdUpdate(4);
			V558_LcdBgDisable();
		}
		if(gLcdContext.panelValid & 1)
		{
			gLcdContext.panelcur = gLcdContext.panelInfo;
			gLcdContext.pPanelSetReg = gLcdContext.panelInfo->pPanelSetReg;
			gLcdContext.lc = gLcdContext.panelInfo->lcdif->lc & 0xffef;
			V558_LcdOpenModule();
			V558_LcdSetPanelState(V558_LCDPANEL_POWON);
			V558_LcdSetBGColor(gLcdContext.panelInfo->lcdif->bkcolor);
			V558_LcdBgEnable();
			V558_LcdUpdate(4);
			V558_LcdBgDisable();
		}

		return SUCCEED;
	}
	return LCDFAILED;
}

/********************************************************************************

	Description:
		Close lcd module

	Remarks:
		state: checking

*******************************************************************************/
void V558_LcdClose(void)
{
}

/********************************************************************************

	Description:
		Get lcd module's internal context

	Returns:
		return the address of lcd module's internal context

	Remarks:
		state: checking

*******************************************************************************/
V558_LCD_CONTEXT * V558_LcdGetContext(void)
{
	return &gLcdContext;
}

/********************************************************************************

	Description:
		select current lcd panel, and switch the lcd I/F register setting, Panel R/W function

	Parameters:
		sel:	the panel index

	Note:
		<TABLE>
		value	meaning
		-----	-------
		0		main lcdpanel
		1		sub lcdpanel
		</TABLE>

	Remarks:
		state: checking

	Example:
	void test(void)
	{
		V558_PANELPARM parm;
	
		parm.len = sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE);
		parm.regval = Panel_powon;
		V558_LcdSwitchPanel(0);		//select current panel as main panel
	}
*******************************************************************************/
void V558_LcdSwitchPanel(UINT8 sel)
{
	if(sel == V558_MAIN_LCDPANEL)
	{
		if( (gLcdContext.panelValid & 1) == 0 )
			return;
		gLcdContext.panelcur = gLcdContext.panelInfo;
		gLcdContext.pPanelSetReg = gLcdContext.panelInfo->pPanelSetReg;
		gLcdContext.lc = gLcdContext.panelInfo->lcdif->lc & 0xffef;
	}
	if(sel == V558_SLAVE_LCDPANEL)
	{
		if( (gLcdContext.panelValid & 2) == 0 )
			return;
		gLcdContext.panelcur = gLcdContext.slavepanelInfo;
		gLcdContext.pPanelSetReg = gLcdContext.slavepanelInfo->pPanelSetReg;
		gLcdContext.lc = gLcdContext.slavepanelInfo->lcdif->lc | 0x10;
	}
	V558_LcdOpenModule();
}

/*********************************************************/
//														//
//			through mode, panel r/w						//
//														//
/*********************************************************/

/********************************************************************************

	Description:
		write data to external lcd panel

	Parameters:
		val:	value sending out

	Note:
		when writing, RS = low, WR = low

	Remarks:
		state: ok

*******************************************************************************/
void PanelWrRsClr(UINT16 val)
{
		V558_SetReg(V558_REG_LCD_EXW0	,  (UINT8)val );
		V558_SetReg(V558_REG_LCD_EXW0+1	,  (UINT8)(val >> 8) );
		V558_SetReg(V558_REG_LCD_EXWEN, 0x1);
}

/********************************************************************************

	Description:
		send out data to external lcd panel

	Parameters:
		val:	value sending out

	Note:
		when writing, RS = high, WR = low

	Remarks:
		state: ok

	Example:
	General panel R/W function
	void V558_LcdPanelSetReg(UINT16 adr, UINT16 val)
	{
		PanelWrRsClr(adr);
		PanelWrRsSet(val);
	}
*******************************************************************************/
void PanelWrRsSet(UINT16 val)
{
		V558_SetReg(V558_REG_LCD_EXW1	,  (UINT8)val );
		V558_SetReg(V558_REG_LCD_EXW1+1	,  (UINT8)(val >> 8) );
		V558_SetReg(V558_REG_LCD_EXWEN, 0x2);
}

/********************************************************************************

	Description:
		read data from external lcd panel

	Return Value List:
		return the data read

	Note:
		when reading, RS = low, WR = low

	Remarks:
		state: ok

*******************************************************************************/
UINT16 PanelRdRsClr(void)
{
	UINT16	ret = 0;
	UINT8	x1, x2;

	V558_SetReg(V558_REG_LCD_EXRS, 0x2);
	V558_GetReg(V558_REG_LCD_EXR0   ,  &x1);	//set display gram address
	V558_GetReg(V558_REG_LCD_EXR0+1	,  &x2);
	ret = ((UINT16)x2 << 8) | (UINT16)x1;
	return ret;
}

/********************************************************************************

	Description:
		read data from external lcd panel

	Return Value List:
		return the value read

	Note:
		when reading, RS = high, WR = low

	Remarks:
		state: ok

*******************************************************************************/
UINT16 PanelRdRsSet(void)
{
	UINT16	ret = 0;
	UINT8	x1, x2;

	V558_SetReg(V558_REG_LCD_EXRS, 0x1);
	V558_GetReg(V558_REG_LCD_EXR0   ,  &x1);	//set display gram address
	V558_GetReg(V558_REG_LCD_EXR0+1	,  &x2);
	ret = ((UINT16)x2 << 8) | (UINT16)x1;
	return ret;
}














/********************************************************/
//														//
//	global function of lcd panel with chip				//
//														//
/********************************************************/



/********************************************************************************

	Description:
		set lcd panel to desired work state

	Parameters:
		state:	desired state

	Note:
		This function must be in though mode.
		<TABLE>
		value					meaning
		-----					-------
		V558_LCDPANEL_POWOFF	power off the panel
		V558_LCDPANEL_SLEEP		sleep the panel
		V558_LCDPANEL_STANDBY	stand by the panel
		V558_LCDPANEL_POWON		power on the panel
		default					power on the panel
		</TABLE>

	Return Value List:
	   SUCCEED	:  if succesful
	   LCDFAILED:  otherwise

	Remarks:
		state: valid

	Example:
	void test(void)
	{
		V558_LcdSetPanelState(V558_LCDPANEL_SLEEP);	//panel enter sleep state
		Sleep(1000);
		V558_LcdSetPanelState(V558_LCDPANEL_POWON);	//panel wakeup from sleep state
	}

*******************************************************************************/
UINT16 V558_LcdSetPanelState(UINT8 state)
{
	UINT8 opmode;
	V558_PANELINFO *pPif = gLcdContext.panelcur;

	if( !pPif )
		return LCDFAILED;

	opmode = V558_CtrlGetWorkMode();
	V558_CtrlSetWorkMode(V558_MODE_THROUGH);
	
	switch(state)
	{
	case V558_LCDPANEL_POWOFF:
		V558_SetPanelRegList(pPif->powoff);
		break;
	case V558_LCDPANEL_SLEEP:
		V558_SetPanelRegList(pPif->sleep);
		break;
	case V558_LCDPANEL_STANDBY:
		V558_SetPanelRegList(pPif->stdby);
		break;
	case V558_LCDPANEL_WAKEUP:
		V558_SetPanelRegList(pPif->wakefs);
		break;
	case V558_LCDPANEL_WAKEFSTY:
		V558_SetPanelRegList(pPif->wakefstd);
		break;
	default:
//		V558_LcdPanelReset();
		V558_SetPanelRegList(pPif->powon);
		if(pPif->pPanelInit != NULL)
			pPif->pPanelInit();
		break;
	}
	V558_CtrlSetWorkMode(opmode);

	if(pPif == gLcdContext.panelInfo)
		gLcdContext.panelState = state;
	if(pPif == gLcdContext.slavepanelInfo)
		gLcdContext.SlavepanelState = state;
	return SUCCEED;
}

/********************************************************************************

	Description:
		get current lcd panel's work state

	Returns :
		<TABLE>
		value					meaning
		-----					-------
		V558_LCDPANEL_POWOFF	power off state
		V558_LCDPANEL_SLEEP		sleep the state
		V558_LCDPANEL_STANDBY	stand by state
		V558_LCDPANEL_POWON		power on state
		V558_LCDPANEL_WAKEUP	wakeup from sleep
		V558_LCDPANEL_WAKEFSTY	wakeup from standby
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
UINT8  V558_LcdGetPanelState(void)
{
	if(gLcdContext.panelcur == gLcdContext.panelInfo)
		return gLcdContext.panelState;
	if(gLcdContext.panelcur == gLcdContext.slavepanelInfo)
		return gLcdContext.SlavepanelState;
	return V558_LCDPANEL_NOT_INIT;
}

/********************************************************************************

	Description:
		set the panel's display dimension

	Parameters:
		size:	panel's display dimension

	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdSetDisPeriod(V558_SIZE size)
{
	V558_SetReg( V558_REG_LCD_HDP	, (UINT8)(size.cx) );
	V558_SetReg( V558_REG_LCD_HDP+1	, (UINT8)(size.cx >> 8) );
	V558_SetReg( V558_REG_LCD_VDP	, (UINT8)(size.cy) );
	V558_SetReg( V558_REG_LCD_VDP+1	, (UINT8)(size.cy >> 8) );
}

/********************************************************************************

	Description:
		get the setting of panel's display dimension

	Parameters:
		size:	return the setting of panel's display dimension.

	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdGetDisPeriod(V558_SIZE *size)
{
	UINT8 val1, val2;

	V558_GetReg( V558_REG_LCD_HDP	,  &val1 );
	V558_GetReg( V558_REG_LCD_HDP+1	,  &val2 );
	size->cx = ((UINT16)val2 << 8) | ((UINT16)val1);
	V558_GetReg( V558_REG_LCD_VDP	,  &val1 );
	V558_GetReg( V558_REG_LCD_VDP+1	,  &val2 );
	size->cy = ((UINT16)val2 << 8) | ((UINT16)val1);
}

/********************************************************************************

	Description:
		reset the lcd panel, all of LCD panel's internal register will set to its default setting

	Note:
		Hardware must design right to support this function

	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdPanelReset(void)
{
	V558_SetReg(V558_REG_LCD_LCDRST		,  0x0);
	LcdDelay(1);
	V558_SetReg(V558_REG_LCD_LCDRST		,  0x1);
	LcdDelay(1);
	gLcdContext.panelState = V558_LCDPANEL_NOT_INIT;
	gLcdContext.SlavepanelState = V558_LCDPANEL_NOT_INIT;
}



/*********************************************************/
//														//
//			setup or modify B layer						//
//														//
/*********************************************************/






/********************************************************************************

	Description:
		set B layer memory size

	Parameters:
		size:	B layer memory's size
		
	Note:
		\1. the unit is pixel

		\2. must set B layer memory before access to B layer memory

	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdSetBLSize(V558_SIZE Size)
{
	V558_SetReg( V558_REG_LCD_BMW	,  (UINT8)(Size.cx) );
	V558_SetReg( V558_REG_LCD_BMW+1	,  (UINT8)(Size.cx >> 8) );
	V558_SetReg( V558_REG_LCD_BMH	,  (UINT8)(Size.cy) );
	V558_SetReg( V558_REG_LCD_BMH+1	,  (UINT8)(Size.cy >> 8) );

	gLcdContext.blSize = Size;
	gLcdContext.blSpan = GetSpanFromDepth(gLcdContext.blSize.cx, gLcdContext.blDepth);
}

/********************************************************************************

	Description:
		get the setting of B layer memory

	Parameters:
		Size:	return B layer setting

	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdGetBLSize(V558_SIZE *Size)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_LCD_BMW	,  &x1 );
	V558_GetReg( V558_REG_LCD_BMW+1	,  &x2 );
	Size->cx = ((UINT16)x2 << 8) + ((UINT16)x1);
	V558_GetReg( V558_REG_LCD_BMH	,  &x1 );
	V558_GetReg( V558_REG_LCD_BMH+1	,  &x2 );
	Size->cy = ((UINT16)x2 << 8) + ((UINT16)x1);
}

/********************************************************************************

	Description:
		set B layer display region

	Parameters:
		Rect:	the coordinate is according to B layer memory
		
	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdSetBLDisRect(V558_RECT Rect)
{
	V558_SetReg( V558_REG_LCD_BDX0	,  (UINT8)(Rect.left) );
	V558_SetReg( V558_REG_LCD_BDX0+1,  (UINT8)(Rect.left >> 8) );
	V558_SetReg( V558_REG_LCD_BDY0	,  (UINT8)(Rect.top) );
	V558_SetReg( V558_REG_LCD_BDY0+1,  (UINT8)(Rect.top >> 8) );
	V558_SetReg( V558_REG_LCD_BW	,  (UINT8)(Rect.width) );
	V558_SetReg( V558_REG_LCD_BW+1	,  (UINT8)(Rect.width >> 8) );
	V558_SetReg( V558_REG_LCD_BH	,  (UINT8)(Rect.height) );
	V558_SetReg( V558_REG_LCD_BH+1	,  (UINT8)(Rect.height >> 8) );
}

/********************************************************************************

	Description:
		get the setting of B layer display region

	Parameters:
		Rect:	return B layer display region setting

	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdGetBLDisRect(V558_RECT *Rect)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_LCD_BDX0		,  &x1 );
	V558_GetReg( V558_REG_LCD_BDX0+1	,  &x2 );
	Rect->left = ((UINT16)x2 << 8) | ((UINT16)x1);
	V558_GetReg( V558_REG_LCD_BDY0		,  &x1 );
	V558_GetReg( V558_REG_LCD_BDY0+1	,  &x2 );
	Rect->top = ((UINT16)x2 << 8) | ((UINT16)x1);
	V558_GetReg( V558_REG_LCD_BW	,  &x1 );
	V558_GetReg( V558_REG_LCD_BW+1	,  &x2 );
	Rect->width = ((UINT16)x2 << 8) | ((UINT16)x1);
	V558_GetReg( V558_REG_LCD_BH	,  &x1 );
	V558_GetReg( V558_REG_LCD_BH+1	,  &x2 );
	Rect->height = ((UINT16)x2 << 8) | ((UINT16)x1);
}

/********************************************************************************

	Description:
		set B layer display point

	Parameters:
		DisPoint:	the coordinate is according to panel
		
	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdSetBLDisPoint(V558_POINT DisPoint)
{
	DisPoint.x += gLcdContext.panelcur->lcdif->mp.x;
	DisPoint.y += gLcdContext.panelcur->lcdif->mp.y;
	V558_SetReg( V558_REG_LCD_BX	,  (UINT8)(DisPoint.x) );
	V558_SetReg( V558_REG_LCD_BX+1	,  (UINT8)(DisPoint.x >> 8) );
	V558_SetReg( V558_REG_LCD_BY	,  (UINT8)(DisPoint.y) );
	V558_SetReg( V558_REG_LCD_BY+1	,  (UINT8)(DisPoint.y >> 8) );
}

/********************************************************************************

	Description:
		get B layer display point

	Parameters:
		DisPoint:	return B layer display point setting
		
	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdGetBLDisPoint(V558_POINT *DisPoint)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_LCD_BX	,  &x1 );
	V558_GetReg( V558_REG_LCD_BX+1	,  &x2 );
	DisPoint->x = ((UINT16)x2 << 8) | ((UINT16)x1);
	DisPoint->x -= gLcdContext.panelcur->lcdif->mp.x;
	V558_GetReg( V558_REG_LCD_BY	,  &x1 );
	V558_GetReg( V558_REG_LCD_BY+1	,  &x2 );
	DisPoint->y = ((UINT16)x2 << 8) | ((UINT16)x1);
	DisPoint->y -= gLcdContext.panelcur->lcdif->mp.y;
}

/********************************************************************************

	Description:
		move up B layer

	Parameters:
		step:	move distance
		
	Remarks:
		state: valid

*******************************************************************************/
void	V558_LcdUpBLOnPanel(UINT16 step)
{
	V558_POINT point;

	V558_LcdGetBLDisPoint(&point);
	if(point.y < step)
		return;
	step = step > point.y ? step : point.y;
	point.y -= step;
	V558_LcdSetBLDisPoint(point);
	V558_LcdUpdate(8);
}







/********************************************************************************

	Description:
		setup B layer data format

	Parameters:
		format:	data format
		
	Note:
		the onchip sram is 40kB, if the color depth is 16bit, the B layer
		max size can achieve 128X160.
		<TABLE>
		value	meaning
		-----	-------
		0		4bit, indirect color
		1		8bit, indirect color
		2		565, direct color
		3		555+alpha, direct color
		4		24bit, direct color
		5		32bit, direct color
		</TABLE>

	Remarks:
		state: checking

*******************************************************************************/
void V558_LcdSetBLFormat(UINT8 format)
{
	V558_SetReg( V558_REG_LCD_BM	,  format | 0x30 );	//combin two panel as one
	gLcdContext.blDepth = GetColordepth(format);
	gLcdContext.blSpan = GetSpanFromDepth(gLcdContext.blSize.cx, gLcdContext.blDepth);
}

/********************************************************************************

	Description:
		get the setting of B layer data format

	Returns:
		the format setting of the B layer

	Note:
		<TABLE>
		the lower 4bit	meaning
		--------------	-------
		0				4bit, indirect color
		1				8bit, indirect color
		2				565, direct color
		3				555+alpha, direct color
		4				24bit, direct color
		5				32bit, direct color
		</TABLE>

	Remarks:
		state: valid

*******************************************************************************/
UINT8	V558_LcdGetBLFormat(void)
{
	UINT8 format;

	V558_GetReg( V558_REG_LCD_BM	,  &format );
	return format;
}

/********************************************************************************

	Description:
		setup B layer

	Parameters:
		proty:	B layer's parameter
		
	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdSetBLProperty(PV558_LAYER_PROPERTY proty)
{
	V558_LcdSetBLDisPoint(proty->DisPoint);
	V558_LcdSetBLDisRect(proty->DisRect);
	V558_LcdSetBLSize(proty->Size);
	V558_LcdSetBLFormat(proty->Format);
}

/********************************************************************************

	Description:
		get B layer's setting

	Parameters:
		proty:	return the B layer setting
		
	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdGetBLProperty(V558_LAYER_PROPERTY *proty)
{
	proty->Format = V558_LcdGetBLFormat();
	V558_LcdGetBLSize( &(proty->Size) );
	V558_LcdGetBLDisRect( &(proty->DisRect) );
	V558_LcdGetBLDisPoint( &(proty->DisPoint) );
}

/********************************************************************************

	Description:
		setup B layer palette entry

	Parameters:
		palette:	color palette
		index:		entry index
		
	Note:
		it is used in indirect color mode, 4bit and 8bit color depth

	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdSetBLPaletteEntry(V558_COLOR palette, UINT8 index)
{
//	V558_SetReg( V558_REG_LCD_IW	,  0);
	V558_SetReg( V558_REG_LCD_IDR	,  palette.r);
	V558_SetReg( V558_REG_LCD_IDG	,  palette.g);
	V558_SetReg( V558_REG_LCD_IDB	,  palette.b);
	V558_SetReg( V558_REG_LCD_IA	,  index);
	V558_SetReg( V558_REG_LCD_IW	,  1);
}

/********************************************************************************

	Description:
		get the setting of B layer palette entry

	Parameters:
		palette:	return color position
		index:		entry index
		
	Note:
		it is used in indirect color mode, 4bit and 8bit color depth

	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdGetBLPaletteEntry(V558_COLOR *palette, UINT8 index)
{
	V558_SetReg( V558_REG_LCD_IA	,  index );
	V558_SetReg( V558_REG_LCD_IR	,  1 );
	V558_GetReg( V558_REG_LCD_IRDR	,  &(palette->r) );
	V558_GetReg( V558_REG_LCD_IRDG	,  &(palette->g) );
	V558_GetReg( V558_REG_LCD_IRDB	,  &(palette->b) );
}

/********************************************************************************

	Description:
		setup B layer palettes

	Parameters:
		palette:	color palette buffer start address
		total:		palette size
		
	Note:
		it is used in indirect color mode, 4bit and 8bit color depth

	Remarks:
		state: valid

*******************************************************************************/
void V558_LcdSetBLPalette(PV558_COLOR palette, UINT16 total)
{
	UINT16 i;

	for(i = 0; i < total; i++)
		V558_LcdSetBLPaletteEntry(palette[i], (UINT8)i);
}

/********************************************************************************

	Description:
		config B layer overlay control

	Parameters:
		x:		setting value

	Note:
		BIT1:	0, enable key color active

	Remarks:
		state: valid

*******************************************************************************/
void	V558_LcdSetBLOverlayCtrl(UINT8 x)
{
	V558_SetReg( V558_REG_LCD_BTCF	,  x );
}

/********************************************************************************

	Description:
		get B layer overlay control setting

	Returns:
		return setting value

	Note:
		BIT1:	0, enable key color active

	Remarks:
		state: valid

*******************************************************************************/
UINT8	V558_LcdGetBLOverlayCtrl(void)
{
	UINT8	x;

	V558_GetReg( V558_REG_LCD_BTCF	,  &x );
	return x;
}

/********************************************************************************

	Description:
		enable B layer ovelay function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetBLOverlayCtrl();

	Remarks:
		state: valid

*******************************************************************************/
UINT8	V558_LcdEnableBLOverlay(void)
{
	UINT8	x;

	x = V558_LcdGetBLOverlayCtrl();
	V558_LcdSetBLOverlayCtrl(x & 0xfd);
	return x;
}

/********************************************************************************

	Description:
		disable B layer ovelay function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetBLOverlayCtrl();

	Remarks:
		state: valid

*******************************************************************************/
UINT8	V558_LcdDisableBLOverlay(void)
{
	UINT8	x;

	x = V558_LcdGetBLOverlayCtrl();
	V558_LcdSetBLOverlayCtrl(x | 2);
	return x;
}

/********************************************************************************

	Description:
		setup B layer overlay key color

	Parameters:
		keycolor:	overlay key color

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdSetBLKeyColor(V558_COLOR keycolor)
{
	UINT8	format;

	format = V558_LcdGetBLFormat();
	switch(format & 0xf)
	{
	case 2:
		keycolor.r <<= 3;
		keycolor.g <<= 2;
		keycolor.b <<= 3;
		break;
	case 3:
		keycolor.r <<= 3;
		keycolor.g <<= 3;
		keycolor.b <<= 3;
		break;
	default:
		break;
	}
	V558_SetReg( V558_REG_LCD_BTCR	,  keycolor.r );
	V558_SetReg( V558_REG_LCD_BTCG	,  keycolor.g );
	V558_SetReg( V558_REG_LCD_BTCB	,  keycolor.b );
}

/********************************************************************************

	Description:
		get B layer overlay key color

	Parameter:
		keycolor:	return overlay key color

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdGetBLKeyColor(V558_COLOR *keycolor)
{
	V558_GetReg( V558_REG_LCD_BTCR	,  &(keycolor->r) );
	V558_GetReg( V558_REG_LCD_BTCR	,  &(keycolor->g) );
	V558_GetReg( V558_REG_LCD_BTCB	,  &(keycolor->b) );
}

/********************************************************************************

	Description:
		enable B layer display function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetDispEn();

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdBLEnable(void)
{
	UINT8	flag;

	flag = V558_LcdGetDispEn();
	V558_LcdSetDispEn(flag | 5);
	return flag;
}

/********************************************************************************

	Description:
		disable B layer display function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetDispEn();

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdBLDisable(void)
{
	UINT8	flag;

	flag = V558_LcdGetDispEn();
	V558_LcdSetDispEn(flag & 0xfb);
	return flag;
}





/*********************************************************/
//														//
//			setup or modify Background					//
//														//
/*********************************************************/






/********************************************************************************

	Description:
		setup background color

	Parameters:
		color:	background color

	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdSetBGColor(V558_COLOR color)
{
	V558_SetReg( V558_REG_LCD_BGDR	,  color.r);
	V558_SetReg( V558_REG_LCD_BGDG	,  color.g);
	V558_SetReg( V558_REG_LCD_BGDB	,  color.b);
}

/********************************************************************************

	Description:
		get background color setting

	Parameters:
		color:	return background color

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdGetBGColor(V558_COLOR *color)
{
	V558_GetReg( V558_REG_LCD_BGDR	,  &(color->r) );
	V558_GetReg( V558_REG_LCD_BGDG	,  &(color->g) );
	V558_GetReg( V558_REG_LCD_BGDB	,  &(color->b) );
}

/********************************************************************************

	Description:
		enable background layer display function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetDispEn();

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdBgEnable(void)
{
	UINT8	flag;

	flag = V558_LcdGetDispEn();
	V558_LcdSetDispEn(flag | 9);
	return flag;
}

/********************************************************************************

	Description:
		Disable background layer display function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetDispEn();

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdBgDisable(void)
{
	UINT8	flag;

	flag = V558_LcdGetDispEn();
	V558_LcdSetDispEn(flag & 0xf7);
	return flag;
}

/********************************************************************************

	Description:
		write B layer memory 

	Parameters:
		pData:	input data buffer, write it to B layer memory
		length: the data buffer size
		beginp:	the start position of B layer memory, pData will be writen from it
		span:	increase step of each write process
		skip:	skip size of each times

	Note:
		\Description as:

		<IMAGE V558_LcdWriteBL>

	Remarks:
		state: valid

	Example:
	#include <stdio.h>
	#include "v558api.h"
	void test(void)
	{
		FILE *fp;
		char buf[0xa000];
		int len;

		fp = fopen("raw565.dat", rb);	//image size = 0x30*0x40
		if(NULL == fp)
			return;
		len = fread(buf, 1, 0xa000, fp);	//len = 0x30*0x40*2 = 0x1800
		fclose(fp);
		//start point(0, 0), Blayer size = 0x80*0xa0, 565 format
		//skip = (0x80-0x30) * 2
		//span = 0x30 * 2
		V558_LcdWriteBL(buf, 0x1800, 0, 0x60, 0xa0);	
	}

*********************************************************************************/
void	V558_LcdWriteBL(UINT8 *pData, UINT16 length, 
						UINT16 beginp, UINT16 span, UINT16 skip)
{
	UINT32 startAddr = 0x10000 + beginp;
	UINT16 len = 0;
	UINT8 *pbuf = pData;

	if(span == 0)
		return;

	if(skip == 0)
	{
		V558_WriteSram(startAddr, pbuf, length);
		return;
	}

	while( len < length )
	{
		V558_WriteSram(startAddr, pbuf+len, span);
		startAddr += span+skip;
		len += span;
	}
}

/********************************************************************************

	Description:
		transfer bitmap to B layer memory 

	Parameters:
		point:	the bmp display position
		pData:	bmp data buffer

	Note:
		\1. the input data must be V558_BMP format

		\2. if colordep <=8, it will change the chip's palatte

		\3. if current B layer's color depth isnt equal input BMP, 
			then the B layer will change to fit input format

		\4. otherwise, the coordinate position's B layer memory will be replace
			by new input data

		\5. the span of input bmp must align to byte

	Remarks:
		state: checking

*********************************************************************************/

UINT16	GetOffset(UINT16 x, UINT8 colordep)
{
	switch(colordep)
	{
	case 4:
		x >>= 1;
		break;
	case 8:
		break;
	case 24:
		x  *= 3;
		break;
	case 32:
		x <<= 2;
	case 15:
	case 16:
	default:
		x <<= 1;
		break;
	}
	return x;
}











/*********************************************************/
//														//
//			setup or modify panel layer API				//
//														//
/*********************************************************/




void	V558_PanelSetSize(V558_SIZE Size)
{
	V558_RECT Rect;
	
	Rect.left = 0;
	Rect.top = 0;
	Rect.width = Size.cx;
	Rect.height = Size.cy;
	V558_LcdSetBLDisRect(Rect);
	V558_LcdSetBLSize(Size);
	V558_LcdSetBLFormat(DEFAULT_BLFORMAT);
}

void	V558_PanelGetSize(V558_SIZE *Size)
{
	V558_LcdGetBLSize(Size);
}



/********************************************************/
//														//
//			setup or modify A layer						//
//														//
/********************************************************/


/********************************************************************************

	Description:
		set A layer memory size

	Parameters:
		size:	A layer memory's size
		
	Note:
		the unit is pixel

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdSetALSize(V558_SIZE Size)
{
	if(Size.cx % 2)
		Size.cx--;
	V558_SetReg( V558_REG_LCD_AMW	,  (UINT8)(Size.cx) );
	V558_SetReg( V558_REG_LCD_AMW+1	,  (UINT8)(Size.cx >> 8) );
	V558_SetReg( V558_REG_LCD_AMH	,  (UINT8)(Size.cy) );
	V558_SetReg( V558_REG_LCD_AMH+1	,  (UINT8)(Size.cy >> 8) );
	AL_UPDATEREG();	//ymq0618
}

/********************************************************************************

	Description:
		get the setting of B layer memory

	Parameters:
		Size:	return B layer setting

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdGetALSize(V558_SIZE *Size)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_LCD_AMW	,  &x1 );
	V558_GetReg( V558_REG_LCD_AMW+1	,  &x2 );
	Size->cx = ((UINT16)x2 << 8) | ((UINT16)x1);
	V558_GetReg( V558_REG_LCD_AMH	,  &x1 );
	V558_GetReg( V558_REG_LCD_AMH+1	,  &x2 );
	Size->cy = ((UINT16)x2 << 8) | ((UINT16)x1);
}

/********************************************************************************

	Description:
		set A layer display region

	Parameters:
		Rect:	the coordinate is according to A layer memory
		
	Note:
		Need adjust when lcd panel is different

	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdSetALDisRect(V558_RECT Rect)
{
	if(Rect.left % 2)
		Rect.left --;
	V558_SetReg( V558_REG_LCD_ADX0	,  (UINT8)(Rect.left) );
	V558_SetReg( V558_REG_LCD_ADX0+1,  (UINT8)(Rect.left >> 8) );
	V558_SetReg( V558_REG_LCD_ADY0	,  (UINT8)(Rect.top) );
	V558_SetReg( V558_REG_LCD_ADY0+1,  (UINT8)(Rect.top >> 8) );
	V558_SetReg( V558_REG_LCD_AW	,  (UINT8)(Rect.width) );
	V558_SetReg( V558_REG_LCD_AW+1	,  (UINT8)(Rect.width >> 8) );
	V558_SetReg( V558_REG_LCD_AH	,  (UINT8)(Rect.height) );
	V558_SetReg( V558_REG_LCD_AH+1	,  (UINT8)(Rect.height >> 8) );
	AL_UPDATEREG();	//ymq0618
}

/********************************************************************************

	Description:
		get the setting of A layer display region

	Parameters:
		Rect:	return A layer display region setting

	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdGetALDisRect(V558_RECT *Rect)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_LCD_ADX0		,  &x1 );
	V558_GetReg( V558_REG_LCD_ADX0+1	,  &x2 );
	Rect->left = ((UINT16)x2 << 8) | ((UINT16)x1);
	V558_GetReg( V558_REG_LCD_ADY0		,  &x1 );
	V558_GetReg( V558_REG_LCD_ADY0+1	,  &x2 );
	Rect->top = ((UINT16)x2 << 8) | ((UINT16)x1);
	V558_GetReg( V558_REG_LCD_AW		,  &x1 );
	V558_GetReg( V558_REG_LCD_AW+1		,  &x2 );
	Rect->width = ((UINT16)x2 << 8) | ((UINT16)x1);
	V558_GetReg( V558_REG_LCD_AH		,  &x1 );
	V558_GetReg( V558_REG_LCD_AH+1		,  &x2 );
	Rect->height = ((UINT16)x2 << 8) | ((UINT16)x1);
}

/********************************************************************************

	Description:
		set A layer display point

	Parameters:
		DisPoint:	the coordinate is according to panel
		
	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdSetALDisPoint(V558_POINT DisPoint)
{
	DisPoint.x += gLcdContext.panelcur->lcdif->mp.x;
	DisPoint.y += gLcdContext.panelcur->lcdif->mp.y;

	V558_SetReg( V558_REG_LCD_AX	,  (UINT8)(DisPoint.x) );
	V558_SetReg( V558_REG_LCD_AX+1	,  (UINT8)(DisPoint.x >> 8) );
	V558_SetReg( V558_REG_LCD_AY	,  (UINT8)(DisPoint.y) );
	V558_SetReg( V558_REG_LCD_AY+1	,  (UINT8)(DisPoint.y >> 8) );

	AL_UPDATEREG();	//ymq0618
}

/********************************************************************************

	Description:
		get A layer display point

	Parameters:
		DisPoint:	return A layer display point setting
		
	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdGetALDisPoint(V558_POINT *DisPoint)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_LCD_AX	,  &x1 );
	V558_GetReg( V558_REG_LCD_AX+1	,  &x2 );
	DisPoint->x = ((UINT16)x2 << 8) | ((UINT16)x1);
	DisPoint->x -= gLcdContext.panelcur->lcdif->mp.x;
	V558_GetReg( V558_REG_LCD_AY	,  &x1 );
	V558_GetReg( V558_REG_LCD_AY+1	,  &x2 );
	DisPoint->y = ((UINT16)x2 << 8) | ((UINT16)x1);
	DisPoint->y -= gLcdContext.panelcur->lcdif->mp.y;
}

/********************************************************************************

	Description:
		set A layer display point on sub lcd panel

	Parameters:
		DisPoint:	the coordinate is according to panel
		
	Remarks:
		state: valid

*********************************************************************************/
void V558_SubLcdSetALDisPoint(V558_POINT DisPoint)
{
	DisPoint.x += gLcdContext.panelcur->lcdif->mp.x + gLcdContext.panelcur->lcdif->sp.x;
	DisPoint.y += gLcdContext.panelcur->lcdif->mp.y + gLcdContext.panelcur->lcdif->sp.y;

	V558_SetReg( V558_REG_LCD_AX	,  (UINT8)(DisPoint.x) );
	V558_SetReg( V558_REG_LCD_AX+1	,  (UINT8)(DisPoint.x >> 8) );
	V558_SetReg( V558_REG_LCD_AY	,  (UINT8)(DisPoint.y) );
	V558_SetReg( V558_REG_LCD_AY+1	,  (UINT8)(DisPoint.y >> 8) );
	
	AL_UPDATEREG();	//ymq0618
}

/********************************************************************************

	Description:
		get A layer display point on sub lcd panel

	Parameters:
		DisPoint:	return A layer display point setting
		
	Remarks:
		state: valid

*********************************************************************************/
void V558_SubLcdGetALDisPoint(V558_POINT *DisPoint)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_LCD_AX	,  &x1 );
	V558_GetReg( V558_REG_LCD_AX+1	,  &x2 );
	DisPoint->x = ((UINT16)x2 << 8) | ((UINT16)x1);
	DisPoint->x -= gLcdContext.panelcur->lcdif->mp.x + gLcdContext.panelcur->lcdif->sp.x;
	V558_GetReg( V558_REG_LCD_AY	,  &x1 );
	V558_GetReg( V558_REG_LCD_AY+1	,  &x2 );
	DisPoint->y = ((UINT16)x2 << 8) | ((UINT16)x1);
	DisPoint->y -= gLcdContext.panelcur->lcdif->mp.y + gLcdContext.panelcur->lcdif->sp.y;
}

/********************************************************************************

	Description:
		setup A layer

	Parameters:
		proty:	A layer's parameter
		
	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdSetALProperty(PV558_LAYER_PROPERTY proty)
{
	V558_LcdSetALSize(proty->Size);
	V558_LcdSetALDisRect(proty->DisRect);
	V558_LcdSetALDisPoint(proty->DisPoint);
}

/********************************************************************************

	Description:
		get A layer's setting

	Parameters:
		proty:	return the A layer setting
		
	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdGetALProperty(V558_LAYER_PROPERTY *proty)
{
	V558_LcdGetALSize( &(proty->Size) );
	V558_LcdGetALDisRect( &(proty->DisRect) );
	V558_LcdGetALDisPoint( &(proty->DisPoint) );
}

/********************************************************************************

	Description:
		setup A layer

	Parameters:
		proty:	A layer's parameter
		
	Remarks:
		state: valid

*********************************************************************************/
void V558_SubLcdSetALProperty(PV558_LAYER_PROPERTY proty)
{
	V558_LcdSetALSize(proty->Size);
	V558_LcdSetALDisRect(proty->DisRect);
	proty->DisPoint.x += gLcdContext.panelcur->lcdif->sp.x;
	proty->DisPoint.y += gLcdContext.panelcur->lcdif->sp.y;
	V558_LcdSetALDisPoint(proty->DisPoint);
}

/********************************************************************************

	Description:
		get A layer's setting

	Parameters:
		proty:	return the A layer setting
		
	Remarks:
		state: valid

*********************************************************************************/
void V558_SubLcdGetALProperty(V558_LAYER_PROPERTY *proty)
{
	V558_LcdGetALSize( &(proty->Size) );
	V558_LcdGetALDisRect( &(proty->DisRect) );
	V558_LcdGetALDisPoint( &(proty->DisPoint) );
	proty->DisPoint.x -= gLcdContext.panelcur->lcdif->sp.x;
	proty->DisPoint.y -= gLcdContext.panelcur->lcdif->sp.y;
}

/********************************************************************************

	Description:
		config A layer overlay control

	Parameters:
		x:		setting value

	Note:
		BIT1:	0, enable key color active

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdSetALOverlayCtrl(UINT8 x)
{
	V558_SetReg( V558_REG_LCD_ATCF	,  x );
	AL_UPDATEREG();
}

/********************************************************************************

	Description:
		get A layer overlay control setting

	Returns:
		return setting value

	Note:
		BIT1:	0, enable key color active

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdGetALOverlayCtrl(void)
{
	UINT8	x;

	V558_GetReg( V558_REG_LCD_ATCF	,  &x );
	return x;
}

/********************************************************************************

	Description:
		enable A layer ovelay function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetALOverlayCtrl();

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdEnableALOverlay(void)
{
	UINT8 x;

	x = V558_LcdGetALOverlayCtrl();
	V558_LcdSetALOverlayCtrl(x & 0xfd);
//	V558_GetReg( V558_REG_LCD_ATCF	,  &x );
//	V558_SetReg( V558_REG_LCD_ATCF	,  x & 0xfd );
	return x;
}

/********************************************************************************

	Description:
		Disable A layer ovelay function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetALOverlayCtrl();

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdDisableALOverlay(void)
{
	UINT8 x;

	x = V558_LcdGetALOverlayCtrl();
	V558_LcdSetALOverlayCtrl(x | 2);
//	V558_GetReg( V558_REG_LCD_ATCF	,  &x );
//	V558_SetReg( V558_REG_LCD_ATCF	,  x | 2 );
	return x;
}

/********************************************************************************

	Description:
		setup A layer overlay key color

	Parameters:
		keycolor:	overlay key color

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdSetALKeyColor(V558_COLOR keycolor)
{
	V558_SetReg( V558_REG_LCD_ATCR	,  keycolor.r );
	V558_SetReg( V558_REG_LCD_ATCG	,  keycolor.g );
	V558_SetReg( V558_REG_LCD_ATCB	,  keycolor.b );
	AL_UPDATEREG();
}

/********************************************************************************

	Description:
		get A layer overlay key color

	Parameter:
		keycolor:	return overlay key color

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdGetALKeyColor(V558_COLOR *keycolor)
{
	V558_GetReg( V558_REG_LCD_ATCR	,  &(keycolor->r) );
	V558_GetReg( V558_REG_LCD_ATCR	,  &(keycolor->g) );
	V558_GetReg( V558_REG_LCD_ATCB	,  &(keycolor->b) );
}

/********************************************************************************

	Description:
		set layer priority of A and B

	Parameter:
		flag:	priority setting

	Note:
		<TABLE>
		value		priority
		-----		--------
		0			A is prioritized
		1			B is prioritized
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdSetALPriority(UINT8 flag)
{
	V558_SetReg( V558_REG_LCD_LPM,  (UINT8)(flag & 1));
	AL_UPDATEREG();
}

/********************************************************************************

	Description:
		set layer priority of A and B

	Return value list:
		0	:		A is prioritized
		1	:		otherwise

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdGetALPriority(void)
{
	UINT8 flag;

	V558_GetReg( V558_REG_LCD_LPM,  &flag);
	return flag;
}

/********************************************************************************

	Description:
		enable A layer display function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetDispEn();

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdALEnable(void)
{
	UINT8	flag;

	flag = V558_LcdGetDispEn();
	V558_LcdSetDispEn(flag | 3);
	//V558_GetReg( V558_REG_LCD_DCE	,  &flag );
	//V558_SetReg( V558_REG_LCD_DCE	,  flag | 3 );
	//AL_UPDATEREG();	//ymq0626
	return flag;
}

/********************************************************************************

	Description:
		disable A layer display function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetDispEn();

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdALDisable(void)
{
	UINT8	x;

	x = V558_LcdGetDispEn();
	V558_LcdSetDispEn(x & 0xc);
	V558_LcdSetDispEn(x & 0xd); 

	return x;
}





/********************************************************/
//														//
//			config other function of lcd module			//
//														//
/********************************************************/






/********************************************************************************

	Description:
		set display enable register

	Parameters:
		x:	display enable setting value

	Note:
		<TABLE>
		value	meaning
		-----	-------
		BIT0	Display enable
		BIT1	A layer display enable
		BIT2	B layer display enable
		BIT3	Background display enable
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdSetDispEn(UINT8 x)
{
//	UINT16 i = 1000;
	V558_SetReg( V558_REG_LCD_DCE	,  x & 0xf );
	AL_UPDATEREG();	//ymq0618
//	while(i--);
}

/********************************************************************************

	Description:
		set display enable register

	Returns:
		return display register setting

	Note:
		<TABLE>
		value	meaning
		-----	-------
		BIT0	Display enable
		BIT1	A layer display enable
		BIT2	B layer display enable
		BIT3	Background display enable
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdGetDispEn(void)
{
	UINT8	flag;

	V558_GetReg( V558_REG_LCD_DCE	,  &flag );
	return flag;
}

/********************************************************************************

	Description:
		enable display function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetDispEn();

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdDispEnable(void)
{
	UINT8	flag;

	flag = V558_LcdGetDispEn();
	V558_LcdSetDispEn(flag | 1);
	return flag;
}

/********************************************************************************

	Description:
		disable display function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetDispEn();

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdDispDisable(void)
{
	UINT8	flag;
		UINT16 i = 0xff;

	flag = V558_LcdGetDispEn();
	V558_LcdSetDispEn(flag & 0xfe);
	while(i--);	//??
	return flag;
}

/********************************************************************************

	Description:
		set display mode

	Parameters:
		mode:	display mode

	Note:
		<TABLE>
		value		meaning
		-----		-------
		0			90 degree rotation
		1			270 degree rotation
		2			180 degree rotation
		3			vertical mirror
		4			normal
		5			horizontal mirror
		default		normal
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdSetDispMode(UINT8 mode)
{
	if(mode > 0x5)
		mode = 0x4;

	V558_SetReg( V558_REG_LCD_DM ,  mode );
	AL_UPDATEREG();
}

/********************************************************************************

	Description:
		get display mode

	Returns:
		return display mode setting

	Note:
		<TABLE>
		value		meaning
		-----		-------
		0			90 degree rotation
		1			270 degree rotation
		2			180 degree rotation
		3			vertical mirror
		4			normal
		5			horizontal mirror
		default		normal
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdGetDispMode(void)
{
	UINT8 mode;

	V558_GetReg( V558_REG_LCD_DM ,  &mode );
	mode &= 0xf;
	return mode;
}

/********************************************************************************

	Description:
		set overlay mode

	Parameters:
		mode:	overlay mode

	Note:
		<TABLE>
		value		meaning
		-----		-------
		0			transparent overlay
		1			AND overlay
		2			OR overlay
		3			INVERT overlay
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdSetOverlayMode(UINT8 mode)
{
	V558_SetReg( V558_REG_LCD_OM	,  mode & 3);
	AL_UPDATEREG();
}

/********************************************************************************

	Description:
		get overlay mode

	Returns:
		return overlay mode setting

	Note:
		<TABLE>
		value		meaning
		-----		-------
		0			transparent overlay
		1			AND overlay
		2			OR overlay
		3			INVERT overlay
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdGetOverlayMode(void)
{
	UINT8 mode;

	V558_GetReg( V558_REG_LCD_OM	,  &mode);
	return mode;
}

/********************************************************************************

	Description:
		set blending mode

	Parameters:
		mode:	blending mode

	Note:
		<TABLE>
		value			meaning
		-----			-------
		BIT0			Blending enable
		BIT1			Blending ratio select
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdSetBlendMode(UINT8 mode)
{
	V558_SetReg( V558_REG_LCD_BMODE+1	,  mode);
	AL_UPDATEREG();
}

/********************************************************************************

	Description:
		get blending mode

	Returns:
		return blending mode setting

	Note:
		<TABLE>
		value			meaning
		-----			-------
		BIT0			Blending enable
		BIT1			Blending ratio select
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdGetBlendMode(void)
{
	UINT8 mode;

	V558_GetReg( V558_REG_LCD_BMODE+1	,  &mode);
	return mode;
}

/********************************************************************************

	Description:
		enable display blending function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetBlendMode();

	Note:
		the display blending function affect A layer and B layer

	Remarks:
		state: valid

*********************************************************************************/
UINT8 V558_LcdEnableBlend(void)
{
	UINT8	x;

	x = V558_LcdGetBlendMode();
	V558_LcdSetBlendMode(x | 1);
	return x;
}

/********************************************************************************

	Description:
		disable display blending function

	Returns:
		return setting value befor change, you can restore the setting by
		using function V558_LcdSetBlendMode();

	Note:
		the display blending function affect A layer and B layer

	Remarks:
		state: valid

*********************************************************************************/
UINT8 V558_LcdDisableBlend(void)
{
	UINT8	x;

	x = V558_LcdGetBlendMode();
	V558_LcdSetBlendMode(x & 0XFE);
	return x;
}

/********************************************************************************

	Description:
		set blending ratio

	Parameters:
		ratio:	blending ration value

	Note:
		\formula:	(COLORX * ratio + COLORY * (256-ratio)) / 256

		\COLORX:	color of the prioritied layer

		\COLORY:	color of not prioritied layer

	Remarks:
		state: valid

*********************************************************************************/
void V558_LcdSetBlendRatio(UINT8 ratio)
{
	V558_SetReg( V558_REG_LCD_BMODE,  ratio);
	AL_UPDATEREG();
}

/********************************************************************************

	Description:
		set blending ratio

	Returns:
		return blending ration value

	Note:
		\formula:	(COLORX * ratio + COLORY * (256-ratio)) / 256

		\COLORX:	color of the prioritied layer

		\COLORY:	color of not prioritied layer

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdGetBlendRatio(void)
{
	UINT8 ratio;

	V558_GetReg( V558_REG_LCD_BMODE,  &ratio );
	return ratio;
}

/********************************************************************************

	Description:
		add blending ratio

	Parameters:
		step:	add step to blending ratio value

	Returns:
		return blend ratio value before change

	Note:
		\formula:	(COLORX * ratio + COLORY * (256-ratio)) / 256

		\COLORX:	color of the prioritied layer

		\COLORY:	color of not prioritied layer

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdUpBlendRatio(UINT8 step)
{
	UINT8 ratio;

	ratio = V558_LcdGetBlendRatio();
	V558_LcdSetBlendRatio(ratio + step);
	return ratio;
}

/********************************************************************************

	Description:
		decrease blending ratio

	Parameters:
		step:	decrease step to blending ratio value

	Returns:
		return blend ratio value before change

	Note:
		\formula:	(COLORX * ratio + COLORY * (256-ratio)) / 256

		\COLORX:	color of the prioritied layer

		\COLORY:	color of not prioritied layer

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdDownBlendRatio(UINT8 step)
{
	UINT8 ratio;

	ratio = V558_LcdGetBlendRatio();
	V558_LcdSetBlendRatio(ratio - step);
	return ratio;
}


/********************************************************************************

	Description:
		get LCD module interrupt status

	Returns:
		return interrupt status value

	Note:
		\the explanation of return value is

		<TABLE>
		value		meaning
		-----		-------
		BIT0		A layer update done pulse signal
		BIT1		B layer update done pulse signal
		BIT2		BG layer update done pulse signal
		BIT3		Stop video interrupt signal
		BIT4		A layer line buffer warning pulse signal
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
UINT8 V558_LcdGetInterrupt(void)
{
	UINT8 status;

	V558_GetReg( V558_REG_LCD_IST,  &status );

	return status;
}

/********************************************************************************

	Description:
		update image to lcd panel

	Parameters:
		flag:	update method

	Note:
		<TABLE>
		value		meaning
		-----		-------
		BIT0		stop video
		BIT1		B layer update
		BIT2		BG layer update
		BIT3		update B layer outside A
		</TABLE>
		this function is different with various lcd panel

	Remarks:
		state: checking

*********************************************************************************/
void V558_LcdUpdate(UINT8 flag)
{
	UINT8	opmode;
	UINT16	temp;
//	UINT8 opcam;
//	UINT8 dce;
	V558_SIZE	size;

	opmode = V558_CtrlGetWorkMode();
	if(opmode & 7 > 2)
		V558_CtrlSetWorkMode(V558_MODE_VIEW);

	//opcam = V558_SifGetSensorOpmode();
	//V558_SifDisableSyncGen();
	//dce = V558_LcdALDisable();

	V558_SetReg(V558_REG_LCD_IST, 0);
	
	if(flag & 4)
	{
		size.cx = gLcdContext.panelcur->lcdif->size.cx + gLcdContext.panelcur->lcdif->mp.x;
		size.cy = gLcdContext.panelcur->lcdif->size.cy + gLcdContext.panelcur->lcdif->mp.y;
		V558_LcdSetDisPeriod(size);
	}
	V558_SetReg( V558_REG_LCD_UPDATE,  (UINT8)(flag & 0xf) );
	if(flag & 2)
	{
		temp = 0x5000;
		while( ((V558_LcdGetInterrupt() & 0x3) == 0) && (temp-- != 0) );
	}
	else if(flag & 4)
	{
		temp = 0x5000;
		while( ((V558_LcdGetInterrupt() & 0x4) == 0) && (temp-- != 0) );
	}
	if(flag & 4)
	{
		V558_LcdSetDisPeriod(gLcdContext.panelcur->lcdif->size);
	}

	//V558_SifSetSensorOpmode(opcam);
	//V558_LcdSetDispEn(dce);
//	V558_CtrlSetWorkMode(opmode);
}

/********************************************************************************

	Description:
		config lcd module interface control

	Parameters:
		x:	setting value

	Note:
		<TABLE>
		value		meaning
		-----		-------
		BIT0		reserve
		BIT1-2		dither select: 00--444, 01--565, 10--666
		BIT3		dither enable
		BIT4		CS select: 0--CS1, 1--CS2
		BIT5		bus interface select: 0--80 bus, 1--68 bus
		BIT6		gamma correction enable for layer A
		BIT7		each line head function enable
		BIT8		head start or end address sending out to lcd use one word
		BIT9		the Y address will send out first the the x address
		BIT10-11	head separate mode		

		BIT12		send out start address first then send out end address
		BIT13		dont send out the end address
		BIT14		type select, default is 1
		BIT15		X and Y address send out in succession
		</TABLE>
		the setting value will be different with various lcd panel

	Remarks:
		state: checking

*********************************************************************************/
void	V558_LcdSetIFControl(UINT16 x)
{
	V558_SetReg( V558_REG_LCD_LC	,  (UINT8)x);
	V558_SetReg( V558_REG_LCD_LC+1	,  (UINT8)(x >> 8));
}

/********************************************************************************

	Description:
		get lcd module interface control setting

	Returns:
		return setting value

	Note:
		<TABLE>
		value		meaning
		-----		-------
		BIT0		reserve
		BIT1-2		dither select: 00--444, 01--565, 10--666
		BIT3		dither enable
		BIT4		CS select: 0--CS1, 1--CS2
		BIT5		bus interface select: 0--80 bus, 1--68 bus
		BIT6		gamma correction enable for layer A
		BIT7		each line head function enable
		BIT8		head start or end address sending out to lcd use one word
		BIT9		the Y address will send out first the the x address
		BIT10-11	head separate mode		
		BIT12		send out start address first then send out end address
		BIT13		dont send out the end address
		BIT14		type select, default is 1
		BIT15		X and Y address send out in succession
		</TABLE>
		the setting value will be different with various lcd panel

	Remarks:
		state: checking

*********************************************************************************/
UINT16	V558_LcdGetIFControl(void)
{
	UINT8 x1, x2;

	V558_GetReg( V558_REG_LCD_LC	,  &x1 );
	V558_GetReg( V558_REG_LCD_LC+1	,  &x2 );

	return ((UINT16)x2 << 8) | ((UINT16)x1);
}

/********************************************************************************

	Description:
		select current lcd panel

	Parameters:
		sel:	lcd panel index

	Returns:
		return LC(0x40e) register setting

	Example:
	void V558_LcdPanelSetReg(UINT16 adr, UINT16 val)
	{
		PanelWrRsClr(adr);
		PanelWrRsSet(val);
	}

	void test(void)
	{
		V558_PANELPARM parm;
	
		parm.len = sizeof(Panel_powon) / sizeof(V558_PANELREGVALUE);
		parm.regval = Panel_powon;
		V558_SelectPanel(0);		//select current panel as main panel
		V558_LcdSetLcdIFParm(Panel_LcdIF);
		V558_LcdSetRWpanelFunc(V558_LcdPanelSetReg);	//set panel r/w function
		V558_SetPanelRegList(parm);					//power on the panel
	}

*******************************************************************************/
UINT16	V558_SelectPanel(UINT8 sel)
{
	UINT16 x, y;

	x = V558_LcdGetIFControl();
	y = x & 0xffef;
	if(sel)
		y |= 0x10;
	V558_LcdSetIFControl(y);
	return x;
}

/********************************************************************************

	Description:
		Enable display gamma correction for A layer

	Returns:
		return LC(0x40e) register setting

	Remarks:
		state: valid

*********************************************************************************/
UINT16 V558_LcdEnableGamma(void)
{
	UINT16 x;

	x = V558_LcdGetIFControl();
	V558_LcdSetIFControl(x | 0x40);
	return x;
}

/********************************************************************************

	Description:
		disable display gamma correction for A layer

	Returns:
		return LC(0x40e) register setting

	Remarks:
		state: valid

*********************************************************************************/
UINT16 V558_LcdDisableGamma(void)
{
	UINT16 x;

	x = V558_LcdGetIFControl();
	V558_LcdSetIFControl(x & 0xffbf);
	return x;
}

/********************************************************************************

	Description:
		enable display dither function

	Returns:
		return LC(0x40e) register setting

	Remarks:
		state: valid

*********************************************************************************/
UINT16	V558_LcdEnableDither(void)
{
	UINT16 x;

	x = V558_LcdGetIFControl();
	V558_LcdSetIFControl(x | 8);
	return x;
}

/********************************************************************************

	Description:
		disable display dither function

	Returns:
		return LC(0x40e) register setting

	Remarks:
		state: valid

*********************************************************************************/
UINT16	V558_LcdDisableDither(void)
{
	UINT16 x;

	x = V558_LcdGetIFControl();
	V558_LcdSetIFControl(x & 0XffF7);
	return x;
}

/********************************************************************************

	Description:
		select dither mode

	Parameters:
		sel:	dither mode setting

	Note:
		<TABLE>
		value			meaning
		-----			-------
		00				4:4:4
		01				5:6:5
		10				6:6:6
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdSetDitherMode(UINT8 sel)
{
	UINT8 x1;

	V558_GetReg( V558_REG_LCD_LC	,  &x1 );
	V558_SetReg( V558_REG_LCD_LC	,  (x1 & 0xf9) | ((sel & 0x3) << 1) );
}

/********************************************************************************

	Description:
		select dither mode

	Parameters:
		sel:	dither mode setting

	Note:
		<TABLE>
		value			meaning
		-----			-------
		00				4:4:4
		01				5:6:5
		10				6:6:6
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_LcdGetDitherMode(void)
{
	UINT8 x1;

	V558_GetReg( V558_REG_LCD_LC	,  &x1 );
	return (x1 & 0x6) >> 1;
}

/********************************************************************************

	Description:
		set gamma R table for A layer

	Parameters:
		x:	table

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdSetGammaR(UINT8 *x)
{
	UINT16 i;

	for(i = 0; i < 17; i++)
		V558_SetReg(V558_REG_LCD_GLTR0 + i, x[i]);
}

/********************************************************************************

	Description:
		Get gamma R table for A layer

	Parameters:
		x:	table

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdGetGammaR(UINT8 *x)
{
	UINT16 i;

	for(i = 0; i < 17; i++)
		V558_GetReg(V558_REG_LCD_GLTR0 + i, x+i);
}

/********************************************************************************

	Description:
		set gamma G table for A layer

	Parameters:
		x:	table

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdSetGammaG(UINT8 *x)
{
	UINT16 i;

	for(i = 0; i < 17; i++)
		V558_SetReg(V558_REG_LCD_GLTG0 + i, x[i]);
}

/********************************************************************************

	Description:
		Get gamma G table for A layer

	Parameters:
		x:	table

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdGetGammaG(UINT8 *x)
{
	UINT16 i;

	for(i = 0; i < 17; i++)
		V558_GetReg(V558_REG_LCD_GLTG0 + i, x+i);
}

/********************************************************************************

	Description:
		set gamma B table for A layer

	Parameters:
		x:	table

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdSetGammaB(UINT8 *x)
{
	UINT16 i;

	for(i = 0; i < 17; i++)
		V558_SetReg(V558_REG_LCD_GLTB0 + i, x[i]);
}

/********************************************************************************

	Description:
		Get gamma B table for A layer

	Parameters:
		x:	table

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdGetGammaB(UINT8 *x)
{
	UINT16 i;

	for(i = 0; i < 17; i++)
		V558_GetReg(V558_REG_LCD_GLTB0 + i, x+i);
}

/********************************************************************************

	Description:
		set gamma table for A layer

	Parameters:
		x:	table

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdSetGammaTable(UINT8 *r, UINT8 *g, UINT8 *b)
{
	V558_LcdSetGammaR(r);
	V558_LcdSetGammaG(g);
	V558_LcdSetGammaB(b);
}

/********************************************************************************

	Description:
		Get gamma table for A layer

	Parameters:
		x:	table

	Remarks:
		state: valid

*********************************************************************************/
void	V558_LcdGetGammaTable(UINT8 *r, UINT8 *g, UINT8 *b)
{
	V558_LcdGetGammaR(r);
	V558_LcdGetGammaG(g);
	V558_LcdGetGammaB(b);
}



/********************************************************/
//														//
//			graphics engine part						//
//														//
/********************************************************/
#define V558_GeReset()	V558_CtrlSwReset(V558_SW_RESET_GE)



/********************************************************/
//														//
//	global base function of GE module part				//
//														//
/********************************************************/






/********************************************************************************

	Description:
		get graphics engine status

	Return value list:
		1:	if busy
		0:	otherwise

	Note:
		the busy status will be clear when hardware has wroten image to B layer

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_GeGetStatus(void)
{
	UINT8 status;

	V558_GetReg(V558_REG_GE_STATUS, &status);
	return status & 0x80;
}

/********************************************************************************

	Description:
		start graphics engine working

	Remarks:
		state: valid

*********************************************************************************/
void	V558_GeStartup(void)
{
	UINT16	temp;
	UINT8 x;
	UINT8 opmode;

	opmode = V558_CtrlGetWorkMode();
	if(opmode & 7 > 2)
		V558_CtrlSetWorkMode(V558_MODE_VIEW);
	//x = V558_LcdDispDisable();
	x = V558_LcdALDisable();
	temp = 0x1000;
	V558_SetReg(V558_REG_LCD_IST, 0);
	V558_SetReg(V558_REG_GE_STARTUP, 1);
	while( (V558_GeGetStatus() != 0) && (temp--) );
	while( ((V558_LcdGetInterrupt() & 0x3) == 0) && (temp-- != 0) );
	V558_LcdSetDispEn(x);
//	V558_CtrlSetWorkMode(opmode);
}

/********************************************************************************

	Description:
		set GE module operation mode

	Parameters:
		sel:	GE operation mode

	Note:
		\opmode description:

		<TABLE>
		value		meaning
		-----		-------
		1			BITBLT
		0			drawline
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void	V558_GeSetOpmode(UINT8 sel)
{
	UINT8 val;

	V558_GetReg(V558_REG_GE_CTRL1, &val);
	val &= ~V558_GE_BITBLT_OP;
	if( sel )
		val |= V558_GE_BITBLT_OP;
	V558_SetReg(V558_REG_GE_CTRL1, val);
}

/********************************************************************************

	Description:
		set GE module operation mode

	Return value list:
		0:		if drawline
		!=0:	BITBLT

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_GeGetOpmode(void)
{
	UINT8 val;

	V558_GetReg(V558_REG_GE_CTRL1, &val);
	return (val & V558_GE_BITBLT_OP);
}

/********************************************************************************

	Description:
		set bitwise for BITBLT operation
	
	Parameters:
		bitwise:	BITBLT operation code

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetBitwise(UINT8 bitwise)
{
	V558_SetReg(V558_REG_GE_BITWISE, bitwise);
}

/********************************************************************************

	Description:
		get bitwise for BITBLT operation
	
	Returns:
		return BITBLT operation setting

	Remarks:
		state: checking

*********************************************************************************/
UINT8	V558_GeGetBitwise(void)
{
	UINT8 val;

	V558_GetReg(V558_REG_GE_CTRL1, &val);
	return val;
}

/********************************************************************************

	Description:
		setup GE operate color depth

	Parameters:
		colordep:	GE operate color depth
		
	Note:
		<TABLE>
		value	meaning
		-----	-------
		0		4bit, indirect color
		1		8bit, indirect color
		2		565, direct color
		3		555+alpha, direct color
		4		24bit, direct color
		5		32bit, direct color
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void	V558_GeSetColorDepth(UINT8 colordep)
{
	V558_SetReg(V558_REG_GE_COLORDEPTH, colordep & 7);
}

/********************************************************************************

	Description:
		setup GE operate color depth

	Returns:
		return GE operate color depth setting
		
	Note:
		<TABLE>
		value	meaning
		-----	-------
		0		4bit, indirect color
		1		8bit, indirect color
		2		565, direct color
		3		555+alpha, direct color
		4		24bit, direct color
		5		32bit, direct color
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
UINT8	V558_GeGetColorDepth(void)
{
	UINT8 val = 0;

	V558_GetReg(V558_REG_GE_COLORDEPTH, &val);
	return val;
}






/********************************************************/
//														//
//			pattern funtion								//
//														//
/********************************************************/






/********************************************************************************

	Description:
		Set GE pattern aligh interval

	Parameters:
		align:	align interval
		
	Note:
		0 <= aligh < 8

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetPatternAlign(UINT8 align)
{
	UINT8	val = 0;

	V558_GetReg(V558_REG_GE_CTRL0, &val);
	val &= ~0x70;		
	val |= (align & 0x7) << 4;
	V558_SetReg( V558_REG_GE_CTRL0, val );
}

/********************************************************************************

	Description:
		get GE pattern align interval

	Returns:
		return GE pattern aligh interval
		
	Remarks:
		state: checking

*********************************************************************************/
UINT8	V558_GeGetPatternAlign(void)
{
	UINT8	val = 0;

	V558_GetReg(V558_REG_GE_CTRL0, &val);

	return (val & 0x70) >> 4;
}

/********************************************************************************

	Description:
		set GE pattern start address

	Parameters:
		addr:	pattern start address in the B layer memory

	Note:
		it won't active when pattern data is from host
		
	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetPatternAddr(UINT16 addr)
{
	V558_SetReg( V558_REG_GE_PTN_START_L, (UINT8)addr );
	V558_SetReg( V558_REG_GE_PTN_START_H, (UINT8)(addr >> 8) );
}

/********************************************************************************

	Description:
		get GE pattern start address

	Returns:
		return	pattern start address in the B layer memory

	Remarks:
		state: checking

*********************************************************************************/
UINT16	V558_GeGetPatternAddr(void)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_GE_PTN_START_L, &x1 );
	V558_GetReg( V558_REG_GE_PTN_START_H, &x2 );
	return ((UINT16)x2 << 8) | ((UINT16)x1);
}

/********************************************************************************

	Description:
		Set the data source of patter data 

	Parameters:
		bsel:	setting value

	Note:
		<TABLE>
		value		meaning
		-----		-------
		1			from host
		0			from B layer memory
		</TABLE>

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetHostPattern(UINT8 bsel)
{
	UINT8	val = 0;

	V558_GetReg(V558_REG_GE_CTRL0, &val);
	val &= ~0x80;
	if(bsel)
		val |= 0x80;
	V558_SetReg( V558_REG_GE_CTRL0, val );
}

/********************************************************************************

	Description:
		Set pattern format

	Parameters:
		bsel:	pattern format

	Note:
		<TABLE>
		value		meaning
		-----		-------
		1			mono pattern
		0			color pattern
		</TABLE>

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetMonoPattern(UINT8 bsel)
{
	UINT8	val = 0;

	V558_GetReg(V558_REG_GE_CTRL0, &val);
	val &= ~0x4;
	if(bsel)
		val |= 0x4;
	V558_SetReg( V558_REG_GE_CTRL0, val );
}

/********************************************************************************

	Description:
		set mono pattern background color

	Parameters:
		color:	mono pattern background color

	Note:
		when mono pattern, 0 will set to this background color 
		
	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetPatternBgcolor(V558_COLOR color)
{
	V558_SetReg( V558_REG_GE_PTN_BG_R, color.r );
	V558_SetReg( V558_REG_GE_PTN_BG_G, color.g );
	V558_SetReg( V558_REG_GE_PTN_BG_B, color.b );
}

/********************************************************************************

	Description:
		get mono pattern background color setting

	Parameters:
		color:	return mono pattern background color setting

	Note:
		when mono pattern, 0 will set to this background color 
		
	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeGetPatternBgcolor(V558_COLOR *color)
{
	V558_GetReg( V558_REG_GE_PTN_BG_R, &(color->r) );
	V558_GetReg( V558_REG_GE_PTN_BG_G, &(color->g) );
	V558_GetReg( V558_REG_GE_PTN_BG_B, &(color->b) );
}

/********************************************************************************

	Description:
		set mono pattern foreground color

	Parameters:
		color:	mono pattern foreground color

	Note:
		when mono pattern, 1 will set to this foreground color 
		
	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetPatternFgcolor(V558_COLOR color)
{
	V558_SetReg( V558_REG_GE_PTN_FG_R, color.r );
	V558_SetReg( V558_REG_GE_PTN_FG_G, color.g );
	V558_SetReg( V558_REG_GE_PTN_FG_B, color.b );
}

/********************************************************************************

	Description:
		get mono pattern foreground color setting

	Parameters:
		color:	return mono pattern foreground color setting

	Note:
		when mono pattern, 1 will set to this foreground color 
		
	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeGetPatternFgcolor(V558_COLOR *color)
{
	V558_GetReg( V558_REG_GE_PTN_FG_R, &(color->r) );
	V558_GetReg( V558_REG_GE_PTN_FG_G, &(color->g) );
	V558_GetReg( V558_REG_GE_PTN_FG_B, &(color->b) );
}




/********************************************************/
//														//
//			source funtion								//
//														//
/********************************************************/







/********************************************************************************

	Description:
		set GE source start address

	Parameters:
		addr:	source start address in the B layer memory

	Note:
		it won't active when source data is from host
		
	Remarks:
		state: valid

*********************************************************************************/
void	V558_GeSetSourceAddr(UINT16 addr)
{
	V558_SetReg( V558_REG_GE_SRC_START_L, (UINT8)addr );
	V558_SetReg( V558_REG_GE_SRC_START_H, (UINT8)(addr >> 8) );
}

/********************************************************************************

	Description:
		get GE source block start address

	Returns:
		return	source block start address in the B layer memory

	Remarks:
		state: valid

*********************************************************************************/
UINT16	V558_GeGetSourceAddr(void)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_GE_SRC_START_L, &x1 );
	V558_GetReg( V558_REG_GE_SRC_START_H, &x2 );
	return ((UINT16)x2 << 8) + ((UINT16)x1);
}

/********************************************************************************

	Description:
		set GE source span

	Parameters:
		span:	source span setting value


	Remarks:
		state: valid

*********************************************************************************/
void	V558_GeSetSourceSpan(UINT16 span)
{
	V558_SetReg( V558_REG_GE_SRC_SPAN_L, (UINT8)span );
	V558_SetReg( V558_REG_GE_SRC_SPAN_H, (UINT8)(span >> 8) );
}

/********************************************************************************

	Description:
		get GE source span setting

	Returns:
		return source span setting

	Remarks:
		state: valid

*********************************************************************************/
UINT16	V558_GeGetSourceSpan(void)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_GE_SRC_SPAN_L, &x1 );
	V558_GetReg( V558_REG_GE_SRC_SPAN_H, &x2 );
	return ((UINT16)x2 << 8) + ((UINT16)x1 & 0Xff);
}

/********************************************************************************

	Description:
		Set the data source of source data 

	Parameters:
		bsel:	setting value

	Note:
		<TABLE>
		value		meaning
		-----		-------
		1			from host
		0			from B layer memory
		</TABLE>

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetHostSource(UINT8 bsel)
{
	UINT8	val = 0;

	V558_GetReg(V558_REG_GE_CTRL1, &val);
	val &= ~0x4;
	if(bsel)
		val |= 0x4;
	V558_SetReg( V558_REG_GE_CTRL1, val );
}

/********************************************************************************

	Description:
		Set source format

	Parameters:
		bsel:	source format

	Note:
		<TABLE>
		value		meaning
		-----		-------
		1			mono source
		0			color source
		</TABLE>

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetMonoSource(UINT8 bsel)
{
	UINT8	val = 0;

	V558_GetReg(V558_REG_GE_CTRL1, &val);
	val &= ~0x10;
	if(bsel)
		val |= 0x10;
	V558_SetReg( V558_REG_GE_CTRL1, val );
}

/********************************************************************************

	Description:
		set mono source background color

	Parameters:
		color:	mono source background color

	Note:
		when mono source, 0 will set to this background color 
		
	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetSourceBgcolor(V558_COLOR color)
{
	V558_SetReg( V558_REG_GE_SRC_BG_R, color.r );
	V558_SetReg( V558_REG_GE_SRC_BG_G, color.g );
	V558_SetReg( V558_REG_GE_SRC_BG_B, color.b );
}

/********************************************************************************

	Description:
		get mono source background color setting

	Parameters:
		color:	return mono source background color setting

	Note:
		when mono source, 0 will set to this background color 
		
	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeGetSourceBgcolor(V558_COLOR *color)
{
	V558_GetReg( V558_REG_GE_SRC_BG_R, &(color->r) );
	V558_GetReg( V558_REG_GE_SRC_BG_G, &(color->g) );
	V558_GetReg( V558_REG_GE_SRC_BG_B, &(color->b) );
}

/********************************************************************************

	Description:
		set mono source foreground color

	Parameters:
		color:	mono source foreground color

	Note:
		when mono source, 1 will set to this foreground color 
		
	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetSourceFgcolor(V558_COLOR color)
{
	V558_SetReg( V558_REG_GE_SRC_BG_R, color.r );
	V558_SetReg( V558_REG_GE_SRC_BG_G, color.g );
	V558_SetReg( V558_REG_GE_SRC_BG_B, color.b );
}

/********************************************************************************

	Description:
		get mono source foreground color setting

	Parameters:
		color:	return mono source foreground color setting

	Note:
		when mono source, 0 will set to this foreground color 
		
	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeGetSourceFgcolor(V558_COLOR *color)
{
	V558_GetReg( V558_REG_GE_SRC_BG_R, &(color->r) );
	V558_GetReg( V558_REG_GE_SRC_BG_G, &(color->g) );
	V558_GetReg( V558_REG_GE_SRC_BG_B, &(color->b) );
}




/********************************************************/
//														//
//			destination funtion							//
//														//
/********************************************************/





/********************************************************************************

	Description:
		set GE destination start address

	Parameters:
		addr:	destination start address in the B layer memory

	Remarks:
		state: valid

*********************************************************************************/
void	V558_GeSetDestAddr(UINT16 addr)
{
	V558_SetReg( V558_REG_GE_DEST_START_L, (UINT8)addr );
	V558_SetReg( V558_REG_GE_DEST_START_H, (UINT8)(addr >> 8) );
}

/********************************************************************************

	Description:
		get GE destination block start address

	Returns:
		return	destination block start address in the B layer memory

	Remarks:
		state: valid

*********************************************************************************/
UINT16	V558_GeGetDestAddr(void)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_GE_DEST_START_L, &x1 );
	V558_GetReg( V558_REG_GE_DEST_START_H, &x2 );
	return ((UINT16)x2 << 8) | ((UINT16)x1);
}

/********************************************************************************

	Description:
		set GE destination span

	Parameters:
		span:	destination span setting value

	Remarks:
		state: valid

*********************************************************************************/
void	V558_GeSetDestSpan(UINT16 span)
{
	V558_SetReg( V558_REG_GE_DEST_SPAN_L, (UINT8)span );
	V558_SetReg( V558_REG_GE_DEST_SPAN_H, (UINT8)(span >> 8) );
}

/********************************************************************************

	Description:
		get GE destination span setting

	Returns:
		return destination span setting

	Remarks:
		state: valid

*********************************************************************************/
UINT16	V558_GeGetDestSpan(void)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_GE_DEST_SPAN_L, &x1 );
	V558_GetReg( V558_REG_GE_DEST_SPAN_H, &x2 );
	return ((UINT16)x2 << 8) | ((UINT16)x1);
}

/********************************************************************************

	Description:
		set GE BITBLT region

	Parameters:
		size:	destination region setting

	Note:
		\size.cx = block.width*cw

		\block.width:	destination region's width, the unit is dot

		\cw:			bytes of per dot

	Remarks:
		state: valid

*********************************************************************************/
void	V558_GeSetBitbltSize(V558_SIZE size)
{
	V558_SetReg( V558_REG_GE_DEST_WIDTH_L, (UINT8)size.cx );
	V558_SetReg( V558_REG_GE_DEST_WIDTH_H, (UINT8)(size.cx >> 8) );
	V558_SetReg( V558_REG_GE_DEST_HEIGHT_L, (UINT8)(size.cy) );
	V558_SetReg( V558_REG_GE_DEST_HEIGHT_H, (UINT8)(size.cy >> 8) );
}

/********************************************************************************

	Description:
		get GE BITBLT region

	Parameters:
		size:	return destination region setting

	Note:
		\size.cx = block.width*cw

		\block.width:	destination region's width, the unit is dot

		\cw:			bytes of per dot

	Remarks:
		state: valid

*********************************************************************************/
void	V558_GeGetBitbltSize(V558_SIZE *size)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_GE_DEST_WIDTH_L, &x1 );
	V558_GetReg( V558_REG_GE_DEST_WIDTH_H, &x2 );
	size->cx = ((UINT16)x2 << 8) | ((UINT16)x1);
	V558_GetReg( V558_REG_GE_DEST_HEIGHT_L, &x1 );
	V558_GetReg( V558_REG_GE_DEST_HEIGHT_H, &x2 );
	size->cy = ((UINT16)x2 << 8) | ((UINT16)x1);
}



/********************************************************/
//														//
//			drawline funtion							//
//														//
/********************************************************/





/********************************************************************************

	Description:
		set GE line start point coordinate

	Parameters:
		ps:	start point coordinate

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetLineStartp(V558_POINT ps)
{
	V558_SetReg( V558_REG_GE_LINE_SABS_L, (UINT8)ps.x );
	V558_SetReg( V558_REG_GE_LINE_SABS_H, (UINT8)(ps.x >> 8) );
	V558_SetReg( V558_REG_GE_LINE_SORD_L, (UINT8)ps.y );
	V558_SetReg( V558_REG_GE_LINE_SORD_H, (UINT8)(ps.y >> 8) );
}

/********************************************************************************

	Description:
		get GE line start point coordinate

	Parameters:
		ps:	return line start point coordinate

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeGetLineStartp(V558_POINT *ps)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_GE_LINE_SABS_L, &x1 );
	V558_GetReg( V558_REG_GE_LINE_SABS_H, &x2 );
	ps->x = ((UINT16)x2 << 8) | ((UINT16)x1);
	V558_GetReg( V558_REG_GE_LINE_SORD_L, &x1 );
	V558_GetReg( V558_REG_GE_LINE_SORD_H, &x2 );
	ps->y = ((UINT16)x2 << 8) | ((UINT16)x1);
}

/********************************************************************************

	Description:
		set GE line end point coordinate

	Parameters:
		pe:	end point coordinate

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetLineEndp(V558_POINT pe)
{
	V558_SetReg( V558_REG_GE_LINE_EABS_L, (UINT8)pe.x );
	V558_SetReg( V558_REG_GE_LINE_EABS_H, (UINT8)(pe.x >> 8) );
	V558_SetReg( V558_REG_GE_LINE_EORD_L, (UINT8)pe.y );
	V558_SetReg( V558_REG_GE_LINE_EORD_H, (UINT8)(pe.y >> 8) );
}

/********************************************************************************

	Description:
		get GE line end point coordinate

	Parameters:

		pe:	return line end point coordinate

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeGetLineEndp(V558_POINT *pe)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_GE_LINE_EABS_L, &x1 );
	V558_GetReg( V558_REG_GE_LINE_EABS_H, &x2 );
	pe->x = ((UINT16)x2 << 8) | ((UINT16)x1);
	V558_GetReg( V558_REG_GE_LINE_EORD_L, &x1 );
	V558_GetReg( V558_REG_GE_LINE_EORD_H, &x2 );
	pe->y = ((UINT16)x2 << 8) | ((UINT16)x1);
}

/********************************************************************************

	Description:
		set GE line color

	Parameters:
		color:	line color

	Note:
		the line color must according to GE color depth

	Remarks:
		state: checking

*********************************************************************************/
void V558_GeSetLineColor(V558_COLOR color)
{
	V558_SetReg( V558_REG_GE_LINE_R	,  color.r);
	V558_SetReg( V558_REG_GE_LINE_G	,  color.g);
	V558_SetReg( V558_REG_GE_LINE_B	,  color.b);
}

/********************************************************************************

	Description:
		get GE line color

	Parameters:
		color:	return line color setting

	Note:
		the line color must according to GE color depth

	Remarks:
		state: checking

*********************************************************************************/
void V558_GeGetLineColor(V558_COLOR *color)
{
	V558_GetReg( V558_REG_GE_LINE_R	,  &(color->r) );
	V558_GetReg( V558_REG_GE_LINE_G	,  &(color->g) );
	V558_GetReg( V558_REG_GE_LINE_B	,  &(color->b) );
}

/********************************************************************************

	Description:
		set GE line properties

	Parameters:
		ps:	line start point
		pe: line end point
		color: line color

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeSetLineProty(V558_POINT ps, V558_POINT pe, V558_COLOR color)
{
	V558_GeSetLineStartp(ps);
	V558_GeSetLineEndp(pe);
	V558_GeSetLineColor(color);
}

/********************************************************************************

	Description:
		get GE line properties

	Parameters:
		ps:	return line start point setting
		pe: return line end point setting
		color: return line color setting

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeGetLineProty(V558_POINT *ps, V558_POINT *pe, V558_COLOR *color)
{
	V558_GeGetLineStartp(ps);
	V558_GeGetLineEndp(pe);
	V558_GeGetLineColor(color);
}







/********************************************************/
//														//
//			top funtion									//
//														//
/********************************************************/




void	V558_GeOpen(void)
{
	V558_JbufGeInit();
}

/********************************************************************************

	Description:
		draw line function

	Parameters:
		ps:	line start point
		pe: line end point
		color: line color

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeDrawLine(V558_POINT ps, V558_POINT pe, V558_COLOR color)
{
	UINT8 colorIndex;
	UINT16 span;
	V558_SIZE size;
	//UINT8 opmode;

	//opmode = V558_CtrlGetWorkMode();
	//V558_CtrlSetWorkMode(V558_MODE_VIEW);

	V558_GeReset();
	V558_JbufGeInit();

	colorIndex = V558_LcdGetBLFormat();
	colorIndex &= 0x7;
	V558_GeSetColorDepth(colorIndex);
	V558_LcdGetBLSize(&size);
	span = GetSpanFromIndex(size.cx, colorIndex);
	V558_GeSetDestSpan(span);

	V558_GeSetOpmode(0);		//drawline
	V558_GeSetLineProty( ps, pe, color);
	
	V558_GeStartup();

	//V558_CtrlSetWorkMode(opmode);
}

/********************************************************************************

	Description:
		BITBLT function

	Parameters:
		src:	source block position
		ptn:	pattern position
		dst:	destionation block position
		bitwise:	BITBLT oprate code

	Remarks:
		state: checking

*********************************************************************************/
void	V558_GeBitblt(V558_POINT src, V558_POINT ptn, V558_RECT dst, UINT8 bitwise)
{
	UINT8 colorIndex;
	V558_SIZE size;
	UINT16 span;
	//UINT8 opmode;

	//opmode = V558_CtrlGetWorkMode();
	//V558_CtrlSetWorkMode(V558_MODE_VIEW);

	V558_GeReset();
	V558_JbufGeInit();
	V558_GeSetOpmode(1);		//bitblt
	V558_GeSetBitwise(bitwise);

	colorIndex = V558_LcdGetBLFormat();
	colorIndex &= 0x7;
	V558_GeSetColorDepth(colorIndex);
	V558_LcdGetBLSize(&size);
	span = GetSpanFromIndex(size.cx, colorIndex);

	V558_GeSetSourceSpan(span);
	V558_GeSetSourceAddr( (src.y * span) + 
					GetSpanFromIndex(src.x, colorIndex) );
	V558_GeSetHostSource(0);
	V558_GeSetMonoSource(0);

	V558_GeSetPatternAlign(0);
	V558_GeSetPatternAddr( (ptn.y * span) +
					GetSpanFromIndex(ptn.x, colorIndex) );
	V558_GeSetHostPattern(0);
	V558_GeSetMonoPattern(0);

	V558_GeSetDestSpan(span);
	V558_GeSetDestAddr( (dst.top * span) +
					GetSpanFromIndex(dst.left, colorIndex) );
	size.cx = GetSpanFromIndex(dst.width, colorIndex);
	size.cy = dst.height;
	V558_GeSetBitbltSize(size);

	V558_GeStartup();

//	V558_CtrlSetWorkMode(opmode);
}

//=========================
//============ add part
//===============================


/********************************************************************************

	Description:
		write bmp data to Blayer

	Parameters:
		pData:	source bmp buffer, only data
		length:	buffer length
		beginp: Blayer start address
		span:	modify data per line
		skip:	skip data per line

	Remarks:
		state: ok

*********************************************************************************/
void WriteBmp2BL(UINT8 *pData, UINT16 length, 
						UINT16 beginp, UINT16 span, UINT16 skip)
{
//	UINT32 startAddr = 0x10000 + beginp;
	UINT16 len = 0;
//	UINT8 *pbuf = pData;
	UINT16	height;
	UINT32	endAddr;
	
	if(span == 0)
		return;

	height = length / span;
	endAddr = 0x10000 + beginp + (span + skip) * (height - 1);

	while( len < length )
	{
		V558_WriteSram(endAddr, pData+len, span);
		endAddr -= span+skip;
		len += (((span + 3) >> 2) << 2);
	}
}

/********************************************************************************

	Description:
		swap data, high byte swap with low byte

	Parameters:
		BmpBuf:	source data buffer
		length:	buffer length

	Remarks:
		state: ok

*********************************************************************************/
void V558HighLowConverse(UINT8 *BmpBuf, UINT16 length)
{
	UINT8 high;
	UINT16 i;

	for(i = 0; i < length; i += 2)
	{
		high = BmpBuf[i];
		BmpBuf[i] = BmpBuf[i+1];
		BmpBuf[i+1] = high;
	}
}

/********************************************************************************

	Description:
		write data to Blayer

	Parameters:
		startx:	image data start point x-coordinate
		starty:	image data start point y-coordinate
		endx:	image data end point x-coordinate
		endy:	image data end point y-coordinate
		uspImage: image data buffer

	Remarks:
		state: ok

*********************************************************************************/
void V558_LCDDrawBL(UINT16 startx, UINT16 starty, UINT16 endx, UINT16 endy, UINT8 *uspImage)
{
	UINT32 y, address, width;
	V558_SIZE		size;
	UINT8	colordep, OneColor;
	UINT16 span;
	UINT8 *ptr = uspImage;

	if(startx >= endx) return;
	if(starty >= endy) return;	

	V558_LcdGetBLSize(&size);
	colordep = V558_LcdGetBLFormat();
	colordep = GetColordepth(colordep);
	span = GetSpanFromDepth(size.cx, colordep);
	//OneColor = colordep / 8;
	OneColor = colordep >> 3;
	if(colordep % 8)
		OneColor++;

	address = 0x10000+starty * span + startx*OneColor;
	width = endx - startx;
	width *= OneColor;

	for(y = starty; y < endy; y++)
	{
		V558_WriteSram(address, ptr, width);
		address += span;
		ptr += width;
		//V558_WriteSram(address+span*x, uspImage+x*(endx-startx)*OneColor, (endx-startx)*OneColor);
		//x++;		///write one line
	}
}

/********************************************************************************

	Description:
		write bmp buffer to Blayer

	Parameters:
		point:	image data start point
		pData:	image data buffer, include BMP file header
		Converse:	swap data or not

	Note:
		if the original Blayer format is not same as bmp format, then nothing to do

	Remarks:
		state: ok

*********************************************************************************/
void	V558_LcdDrawBmp(V558_POINT point, UINT8 *pData, UINT8 Converse)
{
	UINT8 colordep;
	UINT16 begin, width, height, x0, x1;
	UINT32 size;
	V558_BITMAPINFOHEADER *pHead;
	UINT16			bspan;

	pHead = (PV558_BITMAPINFOHEADER)(pData + sizeof(V558_BITMAPFILEHEADER));
	x0 = (SINT16)(pHead->biWidth[0]) & 0Xff;
	x1 = (SINT16)(pHead->biWidth[1]) & 0Xff;
	width = (x1 << 8) | x0;
	x0 = (SINT16)(pHead->biHeight[0]) & 0Xff;
	x1 = (SINT16)(pHead->biHeight[1]) & 0Xff;
	height = (x1 << 8) | x0;

	size = width * height;
	colordep = pHead->biBitCount[0];
	if(colordep == gLcdContext.blDepth + 1)
		colordep = gLcdContext.blDepth;
	begin = sizeof(V558_BITMAPFILEHEADER) + sizeof(V558_BITMAPINFOHEADER);
	switch(colordep)
	{
	case 4:
		begin += 4 << 4;
		size >>= 1;
		break;
	case 8:
		begin += 4 << 8;
		break;
	case 15:
	case 16:
		size <<= 1;
		break;
	case 24:
		size *= 3;
		break;
	case 32:
		size <<= 2;
		break;
	default:
		return;
	}

	bspan = GetSpanFromDepth(width, colordep);
		
	if( (colordep == gLcdContext.blDepth) 
		&& (gLcdContext.blSpan >= bspan))
		//&& (gLcdContext.blSize.cy >= height) )
	{
		if(Converse)
			V558HighLowConverse(pData+begin,(UINT16)size);
		WriteBmp2BL(pData+begin, (UINT16)size, 
						point.y * gLcdContext.blSpan + GetOffset(point.x, colordep),
						bspan, gLcdContext.blSpan - bspan);
	}
}

/********************************************************************************

	Description:
		get panel wcy(write cycle)

	Returns:
		return panel wcy setting

	Remarks:
		state: ok

*********************************************************************************/
UINT16 V558_GetPanelWcy(void)
{
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg( V558_REG_LCD_WCONT, &x1 );
	V558_GetReg( V558_REG_LCD_WCONT+1, &x2 );
	return ((UINT16)x2 << 8) + ((UINT16)x1);
}

/********************************************************************************

	Description:
		set panel's wcy(write cycle)

	Parameters:
		x:	wcy setting value

	Remarks:
		state: ok

*********************************************************************************/
void V558_SetPanelWcy(UINT16 x)
{
	V558_SetReg(V558_REG_LCD_WCONT		,  (UINT8)x );
	V558_SetReg(V558_REG_LCD_WCONT+1	,  (UINT8)(x >> 8) );
}

/********************************************************************************

	Description:
		Initialize the register of lcd gamma table

	Note:
		NULL
	Remarks:
		state: checking

*******************************************************************************/
void V558_LcdInitGammaTable(void)
{
	V558_LcdSetGammaTable(gLcdContext.panelcur->lcdif->gammar,
	   gLcdContext.panelcur->lcdif->gammag, gLcdContext.panelcur->lcdif->gammab);
}
