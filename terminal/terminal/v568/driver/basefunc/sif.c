#include "basefunc.h"
#ifndef	V578CMD_DISABLE

/********************************************************************************
  Description:
	congfig sensor mode in SIF modul
  Parameters:
	sel:	sensor mode value
  Note:
			<TABLE>
			value		meaning
			-----		-------
			BIT0		pull up sensor reset pin

			BIT1		Sensor enable
			BIT2		power on sensor
			BIT3		Enable output signal to sensor
			BIT4		Pull up output pin to sensor when BIT3 = 1
			BIT5		Enable sync-generator
			BIT6		1:YUV, 0:Bayer
			BIT7		reserve
			</TABLE>
  Remarks:
*********************************************************************************/
void V5B_SifSetSensorMode(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_SNR_MODE, sel, BIT_8);
}

/********************************************************************************
  Description:
	get sensor mode setting
  Returns:
	return sensor mode setting
  Note:
			<TABLE>
			value		meaning
			-----		-------
			BIT0		pull up sensor reset pin
			BIT1		Sensor enable
			BIT2		power on sensor
			BIT3		Enable output signal to sensor
			BIT4		Pull up output pin to sensor when BIT3 = 1
			BIT5		Enable sync-generator
			BIT6		1:YUV, 0:Bayer
			BIT7		reserve
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetSensorMode(void)
{
	UINT8 x = 0;
	
	x = (UINT8)V5_GetReg(V5_REG_SIF_SNR_MODE, BIT_8);
	return x;
}

/********************************************************************************
Description:
Enable sensor sync singal
Returns:
return Last setting for sensor mode.
Note:
Remarks:
*********************************************************************************/
UINT8 V5B_SifEnableSnrSyncGen(void)
{
	UINT8 uReg = 0;

	uReg = V5B_SifGetSensorMode();
	V5B_SifSetSensorMode(uReg | 0x20);
	return uReg;
}

/********************************************************************************
Description:
Disable sensor sync singal
Returns:
return Last setting for sensor mode.
Note:
Remarks:
*********************************************************************************/
UINT8 V5B_SifDisableSnrSyncGen(void)
{
	UINT8 uReg = 0;

	uReg = V5B_SifGetSensorMode();
	V5B_SifSetSensorMode(uReg & 0xdf);
	return uReg;
}

/********************************************************************************
Description:
Set sensor setting format
Returns:
return Last setting for sensor mode.
Note:
Remarks:
*********************************************************************************/
UINT8 V5B_SifSetDataFormat(UINT8 uFormat)
{
	UINT8 uReg = 0;

	uFormat &= 0x1;
	uReg = V5B_SifGetSensorMode();
	V5B_SifSetSensorMode(uReg | (uFormat << 6));
	return uReg;
}

/********************************************************************************
  Description:
	config sensor opration mode
  Parameters:
	sel:	sensor operation mode value
  Note:
			  <TABLE>
			  value		meaning
			  -----		-------
			  BIT0		sync signal is from sensor
			  BIT1		obreg enable, it is used when TASC sensor is used
			  BIT2		ov i2c used
			  BIT3		sensor clk invert
			  BIT4		serial bus clock reverse
			  BIT5		1:serial bus selected	0:I2c bus selected
			  BIT6		sync signal output to ISP
			  BIT7		start capture
			  </TABLE>
  Remarks:
	state: valid
*********************************************************************************/
void V5B_SifSetSensorOpMode(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_SNROP_MODE, sel, BIT_8);
}

/********************************************************************************
  Description:
	get sensor opration mode setting in SIF module
  Returns:
	return sensor operation mode setting
  Note:
			<TABLE>
			value		meaning
			-----		-------
			BIT0		sync signal is from sensor
			BIT1		obreg enable, it is used when TASC sensor is used
			BIT2		ov i2c used
			BIT3		sensor clk invert
			BIT4		serial bus clock reverse
			BIT5		1:serial bus selected	0:I2c bus selected
			BIT6		sync signal output to ISP
			BIT7		start capture
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetSensorOpMode(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_SIF_SNROP_MODE, BIT_8);
	return x;
}

/********************************************************************************
Description:
Start caption
Parameters:
Return:	setting before modify
Remarks:
*********************************************************************************/
void V5B_SifStartCapture(void)
{
	UINT8 x = 0;

	x = V5B_SifGetSensorOpMode();
	V5B_SifSetSensorOpMode(x | 0xc0);
}

/********************************************************************************
Description:
stop capture
Returns:
setting before modify
Note:
Remarks:
*********************************************************************************/
void V5B_SifStopCapture(void)
{
	UINT8 x = 0;

	x = V5B_SifGetSensorOpMode();
	V5B_SifSetSensorOpMode(x & 0x7f);
}

/********************************************************************************
Description:
enable sync signal output to ISP
Note:
the ISP and IPP won't work if no sync signal input
Remarks:
*********************************************************************************/
UINT8 V5B_SifEnableSyncGen(void)
{
	UINT8 sel = 0;

	sel = V5B_SifGetSensorOpMode();
	V5B_SifSetSensorOpMode((sel | 0x40) & 0x7f);
	return (sel & 0x7f);
}

/********************************************************************************
Description:
Disable sync signal output to ISP
Note:
the ISP and IPP won't work if no sync signal input
Remarks:
*********************************************************************************/
UINT8 V5B_SifDisableSyncGen(void)
{
	UINT8 sel = 0;

	sel = V5B_SifGetSensorOpMode();
	if(sel & 0x40)
		V5B_SifSetSensorOpMode(sel & 0x3f);
	return (sel & 0x7f);
}

/********************************************************************************
  Description:
	config SIF module's yuv data format
  Parameters:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			-----			-----------
			BIT7-4			reserve
			BIT3-2			internale delay count(sif_clk) when handle YUV data
			BIT1-0			YUV data format	00:UYVY, 01:VYUY, 10:YUYV, 11:YVYU
			</TABLE>
  Remarks:
*********************************************************************************/
void V5B_SifSetYuvFormat(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_YUV_FORMAT, sel, BIT_8);
}

/********************************************************************************
  Description:
	get SIF module's YUV data format setting
  Returns:
	return YUV data format setting
  Note:
			<TABLE>
			value			description
			-----			-----------
			BIT7-4			reserve
			BIT3-2			internale delay count(sif_clk) when handle YUV data
			BIT1-0			YUV data format	00:UYVY, 01:VYUY, 10:YUYV, 11:YVYU
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetYuvFormat(void)
{
	UINT8 x = 0;
	
	x = (UINT8)V5_GetReg(V5_REG_SIF_YUV_FORMAT, BIT_8);
	return x;
}

/********************************************************************************
  Description:
	config SIF module's Vsync signal control
  Parameters:
	sel:	setting value
  Note:
			  <TABLE>
			  value			description
			  -----			-----------
			  BIT7-6			reserve
			  BIT5			Vref is full
			  BIT4			Vsync send to sensor less than 1 row
			  BIT3			Vsync send to SIF less than 1 row
			  BIT2			input V signal is bypass to ISP
			  BIT1			input V signal is active high
			  BIT0			input V signal is Vref
			  </TABLE>
  Remarks:
*********************************************************************************/
void V5B_SifSetVsyncCtrl(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_V_CONTROL, sel, BIT_8);
}

/********************************************************************************
  Description:
	get SIF module's Vsync signal control setting
  Returns:
	return setting value
  Note:
			<TABLE>
			value			description
			-----			-----------
			BIT7-6			reserve
			BIT5			Vref is full
			BIT4			Vsync send to sensor less than 1 row
			BIT3			Vsync send to SIF less than 1 row
			BIT2			input V signal is bypass to ISP
			BIT1			input V signal is active high
			BIT0			input V signal is Vref
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetVsyncCtrl(void)
{
	UINT8 x = 0;
	
	x = (UINT8)V5_GetReg(V5_REG_SIF_V_CONTROL, BIT_8);
	return x;
}

/********************************************************************************
  Description:
	config SIF module's Hsync signal control
  Parameters:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			-----			-----------
			BIT7-4			reserve
			BIT3			the input h signal is bypass to ISP
			BIT2			input H signal is active high
			BIT1-0			input H sync flag, 00:Hsync  01:Href  10:drdy
			</TABLE>
  Remarks:
*********************************************************************************/
void V5B_SifSetHsyncCtrl(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_H_CONTROL, sel, BIT_8);
}

/********************************************************************************
  Description:
	get SIF module's Hsync signal control setting
  Returns:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			-----			-----------
			BIT7-4			reserve
			BIT3			the input h signal is bypass to ISP
			BIT2			input H signal is active high
			BIT1-0			input H sync flag, 00:Hsync  01:Href  10:drdy
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetHsyncCtrl(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_SIF_H_CONTROL, BIT_8);
	return x;
}

/********************************************************************************
  Description:
	config sensor's mclk ratio
  Parameters:
	sel:	setting value
  Note:
	sensor's clk = SIF's clk / (sel+1)
  Remarks:
*********************************************************************************/
void V5B_SifSetSnrClk(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_SNRCLK_CNT, sel, BIT_8);
}

/********************************************************************************
  Description:
	get sensor's mclk setting
  Returns:
	return setting value
  Note:
	sensor's mclk = SIF's clk / (sel+1)
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetSnrClk(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_SIF_SNRCLK_CNT, BIT_8);
	return	x;
}

/********************************************************************************
  Description:
	config sensor's output pixel clk ratio
  Parameters:
	sel: setting value
  Note:
	pixel's clk = SIF's clk / (sel+1)
  Remarks:
*********************************************************************************/
void V5B_SifSetPixelRate(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_PIXRATE_SEL, sel, BIT_8);
}

/********************************************************************************
  Description:
	get sensor's output pixel clk ratio setting
  Returns:
	return setting value
  Note:
	pixel's clk = SIF's clk / (sel+1)
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetPixelRate(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_PIXRATE_SEL, BIT_8);
	return (sel & 0x3f);
}

/********************************************************************************
  Description:
	config serial bus clk ratio
  Parameters:
	sel: setting value
  Note:
	serialbus' clk = SIF's clk / sel
  Remarks:
*********************************************************************************/
void V5B_SifSetSerialclk(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_SICLK_CNT, sel, BIT_8);
}

/********************************************************************************
  Description:
	get serial bus clk ratio setting
  Returns:
	return setting value
  Note:
	serialbus' clk = SIF's clk / sel
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetSerialclk(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_SICLK_CNT, BIT_8);
	return sel;
}

/********************************************************************************
Description:
config I2C bus clk ratio
Parameters:
sel:	setting value
Note:
I2C' clk = SIF's clk / sel
Remarks:
*********************************************************************************/
void V5B_SifSetI2cclk(UINT16 sel)
{
	V5_SetReg(V5_REG_SIF_I2C_CLKCNTH, sel, BIT_16_HTOL);
}

/********************************************************************************
Description:
get I2C clk ratio setting
Returns:
return setting value
Note:
I2C' clk = SIF's clk / sel
Remarks:
*********************************************************************************/
UINT16 V5B_SifGetI2cclk(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_SIF_I2C_CLKCNTH, BIT_16_HTOL);
	return x;
}

/********************************************************************************
Description:
config I2C Interval
Parameters:
sel:	setting value
Note:
Remarks:
*********************************************************************************/
void V5B_SifSetI2cInterval(UINT16 sel)
{
	V5_SetReg(V5_REG_SIF_I2C_INTER_TRANSH, sel, BIT_16_HTOL);
}

/********************************************************************************
Description:
get I2C interval
Returns:
return setting value
Note:
Remarks:
*********************************************************************************/
UINT16 V5B_SifGetI2cInterval(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_SIF_I2C_INTER_TRANSH, BIT_16_HTOL);
	return x;
}

/********************************************************************************
  Description:
	config Href signal start position
  Parameters:
	startx:	setting value
  Note:
	SIF module will output Href sync signal to ISP and IPP module
	according to this setting
  Remarks:
*********************************************************************************/
void V5B_SifSetHrefStart(UINT16 startx)
{
	V5_SetReg(V5_REG_SIF_HREF_STARTH, startx , BIT_16_HTOL);
}

/********************************************************************************
  Description:
	get Href signal start position setting
  Returns:
	return setting value
  Note:
	SIF module will output Href sync signal to ISP and IPP module
	according to this setting
  Remarks:
*********************************************************************************/
UINT16 V5B_SifGetHrefStart(void)
{
	UINT16 x = 0;
	
	x = (UINT16)V5_GetReg(V5_REG_SIF_HREF_STARTH, BIT_16_HTOL);
	return x;
}

/********************************************************************************
  Description:
	config Href signal length
  Parameters:
	length:	setting value
  Note:
	SIF module will output Href sync signal to ISP and IPP module
	according to this setting
  Remarks:
*********************************************************************************/
void V5B_SifSetHrefLength(UINT16 length)
{
	V5_SetReg(V5_REG_SIF_HREF_LENGTHH, length, BIT_16_HTOL);
}

/********************************************************************************
  Description:
	get Href signal length setting
  Returns:
	return setting value
  Note:
	SIF module will output Href sync signal to ISP and IPP module
	according to setting
  Remarks:
*********************************************************************************/
UINT16 V5B_SifGetHrefLength(void)
{
	UINT16 x = 0;
	
	x = (UINT16)V5_GetReg(V5_REG_SIF_HREF_LENGTHH, BIT_16_HTOL);
	return x;
}

/********************************************************************************
  Description:
	config Vref signal start position
  Parameters:
	startx:	setting value
  Note:
	SIF module will output Vref sync signal to ISP and IPP module
	according to this setting
  Remarks:
*********************************************************************************/
void V5B_SifSetVrefStart(UINT16 startx)
{
	V5_SetReg(V5_REG_SIF_VREF_STARTH, startx, BIT_16_HTOL);
}

/********************************************************************************
  Description:
	get Vref signal start setting
  Returns:
	return setting value
  Note:
	SIF module will output Vref sync signal to ISP and IPP module
	according to this setting
  Remarks:
*********************************************************************************/
UINT16 V5B_SifGetVrefStart(void)
{
	UINT16 x = 0;
	
	x = (UINT16)V5_GetReg(V5_REG_SIF_VREF_STARTH, BIT_16_HTOL);
	return x;
}

/********************************************************************************
  Description:
	config Vref signal stop position
  Parameters:
	stop:	setting value
  Note:
	SIF module will output Vref sync signal to ISP and IPP module
	according to this setting
  Remarks:
*********************************************************************************/
void V5B_SifSetVrefStop(UINT16 stop)
{
	V5_SetReg(V5_REG_SIF_VREF_STOPH, stop, BIT_16_HTOL);
}

/********************************************************************************
  Description:
	get Vref signal stop setting
  Returns:
	return setting value
  Note:
	SIF module will output Vref sync signal to ISP and IPP module
	according to this setting
  Remarks:
*********************************************************************************/
UINT16 V5B_SifGetVrefStop(void)
{
	UINT16 x = 0;
	
	x = (UINT16)V5_GetReg(V5_REG_SIF_VREF_STOPH, BIT_16_HTOL);
	return x;
}

void V5B_SifSetOB(UINT8 x)
{
	V5_SetReg(V5_REG_SIF_OBREG, x, BIT_8);
}

UINT8 V5B_SifGetOB(void)
{
	UINT8 x = 0;

	x = (UINT8)V5_GetReg(V5_REG_SIF_OBREG, BIT_8);
	return x;
}

/********************************************************************************
  Description:
	config AE windows start
  Parameters:
	start: setting value
  Note:
	the SIF module will batch update sensor's register according to 
	this register
  Remarks:
*********************************************************************************/
void V5B_SifSetAeWinStart(UINT16 start)
{
	V5_SetReg(V5_REG_SIF_AEWIN_STARTH, start, BIT_16_HTOL);
}

/********************************************************************************
  Description:
	get AE windows start setting
  Returns:
	return setting value
  Note:
	the SIF module will batch update sensor's register according to 
	this register
  Remarks:
*********************************************************************************/
UINT16 V5B_SifGetAeWinStart(void)
{
	UINT16 x = 0;
	
	x = (UINT16)V5_GetReg(V5_REG_SIF_AEWIN_STARTH, BIT_16_HTOL);
	return x;
}

/********************************************************************************
  Description:
	config AE windows stop
  Parameters:
	x: setting value
  Note:
	the SIF module will batch update sensor's register according to 
	this register
  Remarks:
*********************************************************************************/
void V5B_SifSetAeWinStop(UINT16 x)
{
	V5_SetReg(V5_REG_SIF_AEWIN_STOPH, x, BIT_16_HTOL);
}

/********************************************************************************
  Description:
	get AE windows stop setting
  Returns:
	return setting value
  Note:
	the SIF module will batch update sensor's register according to 
	this register
  Remarks:
*********************************************************************************/
UINT16 V5B_SifGetAeWinStop(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_SIF_AEWIN_STOPH, BIT_16_HTOL);
	return x;
}

/********************************************************************************
  Description:
	Choose posedge or negedge sample sensor inputs
  Parameters:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			BIT7-1			reserve
			BIT0				0/1
			</TABLE>
  Remarks:
*********************************************************************************/
void V5B_SifSetEdgeChos(UINT8 sel)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	val &= 0x01;
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, (val | sel), BIT_8);
}

/********************************************************************************
  Description:
	get choiced posedge or negedge sample sensor inputs
  Returns:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			BIT7-1			reserve
			BIT0				0/1
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetEdgeChos(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	return (sel & 0x01);
}

void V5B_SifEnableSnrPClk(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, (val | 0x02), BIT_8);
}

void V5B_SifDisableSnrPClk(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, (val & 0xfd), BIT_8);
}

UINT8 V5B_SifIsSnrPClkEn(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	return ((val >> 1) & 0x01);
}

void V5B_SifEnableVSyncDly(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, (val | 0x04), BIT_8);
}

void V5B_SifDisableVSyncDly(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, (val & 0xfb), BIT_8);
}

UINT8 V5B_SifIsVSyncDlyEn(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	return ((val >> 2) & 0x01);
}

void V5B_SifEnableFifoVSyncDly(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, (val | 0x08), BIT_8);
}

void V5B_SifDisableFifoVSyncDly(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, (val & 0xf7), BIT_8);
}

UINT8 V5B_SifIsFifoVSyncDlyEn(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	return ((val >> 3) & 0x01);
}

void V5B_SifSetPClkPosEdge(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	val = val & 0xef;
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, val, BIT_8);
}

void V5B_SifSetPClkNegEdge(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	val = val | 0x10;
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, val, BIT_8);
}

UINT8 V5B_SifGetPClkEdge(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	return ((val >> 4) & 0x01);
}

void V5B_SifEnableSnrDataShift(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, (val | 0x20), BIT_8);
}

void V5B_SifDisableSnrDataShift(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	V5_SetReg(V5_REG_SIF_EDGE_CHOS, (val & 0xdf), BIT_8);
}

UINT8 V5B_SifIsSnrDataShiftEn(void)
{
	UINT8 val = 0;

	val = (UINT8)V5_GetReg(V5_REG_SIF_EDGE_CHOS, BIT_8);
	return ((val >> 5) & 0x01);
}

/********************************************************************************
  Description:
	reset SIF internal counter, but not reset SIF registers setting. Set by biu and reset by SIF self
  Parameters:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			BIT7-1			reserve
			BIT0				1: reset counter;0: normal operation
			</TABLE>
  Remarks:
*********************************************************************************/
void V5B_SifSetCounterReset(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_COUNTER_RESET, sel, BIT_8);
}

/********************************************************************************
  Description:
	reset SIF internal counter, but not reset SIF registers setting. Set by biu and reset by SIF self
  Returns:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			BIT7-1			reserve
			BIT0			1: reset counter;0: normal operation
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetCounterReset(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_COUNTER_RESET, BIT_8);
	return sel;
}
		
void V5B_SifSetCaptureDelay(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_CAPTURE_DELAY, sel, BIT_8);
}

UINT8 V5B_SifGetCaptureDelay(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_CAPTURE_DELAY, BIT_8);
	return (sel & 0x03);
}

/********************************************************************************
  Description:
	set Flash power charge enable/disable
  Parameters:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			BIT7-1			reserve
			BIT0				0/1
			</TABLE>
  Remarks:
*********************************************************************************/
void V5B_SifSetFlashPowerEn (UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_FL_PWM_EN, sel, BIT_8);
}

/********************************************************************************
  Description:
	get Flash power charge enable/disable
  Returns:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			BIT7-1			reserve
			BIT0				0/1
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetFlashPowerEn(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_FL_PWM_EN, BIT_8);
	return sel;
}

/********************************************************************************
	Description:
		Disable flash power
	Returns:
		return flash power setting before modify it
	Note:
		You can restore flash power setting by void V5B_SifSetFlashPowerEn(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V5B_SifDisableFlashPower(void)
{
	UINT8 x = 0;

	x = V5B_SifGetFlashPowerEn();
	V5B_SifSetFlashPowerEn(x & 0xfe);
	return x;
}

/********************************************************************************
	Description:
		Enable flash power
	Returns:
		return flash power setting before modify it
	Note:
		You can restore special control setting by void V5B_SifSetFlashPowerEn(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V5B_SifEnableFlashPower(void)
{
	UINT8 x = 0;

	x = V5B_SifGetFlashPowerEn();
	V5B_SifSetFlashPowerEn(x | 0x01);
	return x;
}

/********************************************************************************
  Description:
	Pwm pulse polarity;
  Parameters:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			BIT7-2			reserve
			BIT1			Flash light charge type
				0: fix level (high or low) active;
				1 : programable frequence and duty plus.
			BIT0			Flash light charge signal polarity;
				0: low active;
				1: high active;
			</TABLE>
  Remarks:
*********************************************************************************/
void V5B_SifSetFlashPolarity(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_FL_POLARITY, (sel & 0x03), BIT_8);
}

/********************************************************************************
  Description:
	Pwm pulse polarity;
  Returns:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			BIT7-1			reserve
			BIT0				"1" is positive active"0" is negative active
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetFlashPolarity(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_FL_POLARITY, BIT_8);
	return (sel & 0x03);
}
		
/********************************************************************************
	Description:
		config Flash clock number in SIF module
	Parameters:
		x:		Flash clock number  to SIF module
	Note:
		Flash clock number based on bus clock
	Remarks:
*********************************************************************************/
void V5B_SifSetFlashPowerCntTgt(UINT16 x)
{
	V5_SetReg(V5_REG_SIF_FL_PWM_CNT_TGT_H, x, BIT_16_HTOL);
}

/********************************************************************************
	Description:
		get config Flash clock number in SIF module
	Returns:
		return flash clock number setting to SIF module
	Note:
		Flash clock number based on bus clock
	Remarks:
*********************************************************************************/
UINT16 V5B_SifGetFlashPowerCntTgt(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_SIF_FL_PWM_CNT_TGT_H, BIT_16_HTOL);
	return x;
}

/********************************************************************************
	Description:
		Duty count based on duty and the fl_pwm_cnt_tgt
	Parameters:
		x:		Duty Flash clock number  to SIF module
	Note:
	Remarks:
*********************************************************************************/
void V5B_SifSetFlashPowerCntDuty(UINT16 x)
{
	V5_SetReg(V5_REG_SIF_FL_PWM_CNT_DUTY_H, x, BIT_16_HTOL);
}

/********************************************************************************
	Description:
		Duty count based on duty and the fl_pwm_cnt_tgt
	Returns:
		return duty flash clock number setting to SIF module
	Note:
	Remarks:
*********************************************************************************/
UINT16 V5B_SifGetFlashPowerCntDuty(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_SIF_FL_PWM_CNT_DUTY_H, BIT_16_HTOL);
	return x;
}

/********************************************************************************
  Description:
	Flash Duty
  Parameters:
	sel:	setting value
  Note:
	Pwm duty from 1 to 16
  Remarks:
*********************************************************************************/
void V5B_SifSetFlashDuty(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_FL_DUTY, sel, BIT_8);
}

/********************************************************************************
  Description:
	Flash Duty
  Returns:
	sel:	setting value
  Note:
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetFlashDuty(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_FL_DUTY, BIT_8);
	return sel;
}

/********************************************************************************
  Description:
	set m10USCNT enable/disable
  Parameters:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			BIT7-1			reserve
			BIT0				0/1
			</TABLE>
  Remarks:
*********************************************************************************/
void V5B_SifSetM10usCntEn (UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_M10US_CNT_EN, sel, BIT_8);
}

/********************************************************************************
  Description:
	get m10USCNT enable/disable
  Returns:
	sel:	setting value
  Note:
			<TABLE>
			value			description
			BIT7-1			reserve
			BIT0				0/1
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetM10usCntEn (void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_M10US_CNT_EN, BIT_8);
	return sel;
}

/********************************************************************************
	Description:
		Disable M10USCNT
	Returns:
		return M10USCNT setting before modify it
	Note:
		You can restore flash power setting by void V5B_SifSetM10usCntEn(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V5B_SifDisableM10usCnt(void)
{
	UINT8 x = 0;

	x = V5B_SifGetM10usCntEn();
	V5B_SifSetM10usCntEn(x & 0xfe);
	return x;
}

/********************************************************************************
	Description:
		Enable M10USCNT
	Returns:
		return M10USCNT setting before modify it
	Note:
		You can restore special control setting by void V5B_SifSetM10usCntEn(UINT8 sel);
	Remarks:
*********************************************************************************/
UINT8 V5B_SifEnableM10usCnt(void)
{
	UINT8 x = 0;

	x = V5B_SifGetM10usCntEn();
	V5B_SifSetM10usCntEn(x | 0x01);
	return x;
}

/********************************************************************************
  Description:
	M10US Target
  Parameters:
	sel:	setting value
  Note:
  Remarks:
*********************************************************************************/
void V5B_SifSetM10usTgt(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_M10US_TGT	, sel, BIT_8);
}

/********************************************************************************
  Description:
	M10US Target
  Returns:
	sel:	setting value
  Note:
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetM10usTgt(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_M10US_TGT, BIT_8);
	return sel;
}

/********************************************************************************
	Description:
		Delay time counter target based on bus clock
	Parameters:
		x:		Delay time counter to SIF module
	Note:
	Remarks:
*********************************************************************************/
void V5B_SifSetDelayFlashTgt(UINT32 x)
{
	V5_SetReg(V5_REG_SIF_DELAY_FLASH_TGT_H, x, BIT_24_HTOL);
}

/********************************************************************************
	Description:
		Delay time counter target based on bus clock
	Returns:
		Delay time counter to SIF module
	Note:
	Remarks:
*********************************************************************************/
UINT32 V5B_SifGetDelayFlashTgt(void)
{
	UINT32 x = 0;

	x = (UINT32)V5_GetReg(V5_REG_SIF_DELAY_FLASH_TGT_H, BIT_24_HTOL);
	return x;
}

/********************************************************************************
	Description:
		Trigger pulse width based on 10 us
	Parameters:
		x:		Trigger pulse width to SIF module
	Note:
	Remarks:
*********************************************************************************/
void V5B_SifSetTriggerTgt(UINT16 x)
{
	V5_SetReg(V5_REG_SIF_TRIGGER_TGT_H, x, BIT_16_HTOL);
}

/********************************************************************************
	Description:
		Trigger pulse width based on 10 us
	Returns:
		return trigger pulse width setting to SIF module
	Note:
	Remarks:
*********************************************************************************/
UINT16 V5B_SifGetTriggerTgt(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_SIF_TRIGGER_TGT_H, BIT_16_HTOL);
	return x;
}

/********************************************************************************
  Description:
	trigger_style_sel
  Parameters:
	sel:	setting value
  Note:
	Bit0 : select the way to start trigger
			0 : use snrop_mode[7] to start fl trigger;
			1: use start_trigger to generate trigger , snrop_mode[7] will not able to startr fl trigger.
	Bit1:  select the way of locating fl trigger postion.
			0 : use frame_end position as trigger postion;
			1 : use trigger_start_row/col regs to locate trigger position;
  Remarks:
*********************************************************************************/
void V5B_SifSetTriggerStyle(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_TRIGGER_STYLE_SEL, (sel & 0x03), BIT_8);
}

/********************************************************************************
  Description:
	trigger_style_sel
  Returns:
	sel:	setting value
  Note:
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetTriggerStyle(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_TRIGGER_STYLE_SEL, BIT_8);
	return (sel & 0x03);
}

/********************************************************************************
  Description:
	start_trigger
  Parameters:
	sel:	setting value
  Note:
  Remarks:
*********************************************************************************/
void V5B_SifSetTriggerStart(UINT8 sel)
{
	V5_SetReg(V5_REG_SIF_START_TRIGGER, (sel & 0x01), BIT_8);
}

/********************************************************************************
  Description:
	start_trigger
  Returns:
	sel:	setting value
  Note:
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetTriggerStart(void)
{
	UINT8 sel = 0;
	
	sel = (UINT8)V5_GetReg(V5_REG_SIF_START_TRIGGER, BIT_8);
	return (sel & 0x01);
}

void V5B_SifStartTrigger(void)
{
	V5_SetReg(V5_REG_SIF_START_TRIGGER, 0x01, BIT_8);
}

void V5B_SifStopTrigger(void)
{
	V5_SetReg(V5_REG_SIF_START_TRIGGER, 0, BIT_8);
}

/********************************************************************************
	Description:
		trigger_start_row
	Parameters:
		x:		trigger_start_row
	Note:
	Remarks:
*********************************************************************************/
void V5B_SifSetTriggerStartRow(UINT16 x)
{
	V5_SetReg(V5_REG_SIF_TRIGGER_START_ROW_H, x, BIT_16_HTOL);
}

/********************************************************************************
	Description:
		trigger_start_row
	Returns:
		return trigger_start_row
	Note:
	Remarks:
*********************************************************************************/
UINT16 V5B_SifGetTriggerStartRow(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_SIF_TRIGGER_START_ROW_H, BIT_16_HTOL);
	return x;
}

/********************************************************************************
	Description:
		trigger_start_col
	Parameters:
		x:		trigger_start_col
	Note:
	Remarks:
*********************************************************************************/
void V5B_SifSetTriggerStartCol(UINT16 x)
{
	V5_SetReg(V5_REG_SIF_TRIGGER_START_COL_H, x, BIT_16_HTOL);
}

/********************************************************************************
	Description:
		trigger_start_col
	Returns:
		return trigger_start_col
	Note:
	Remarks:
*********************************************************************************/
UINT16 V5B_SifGetTriggerStartCol(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_SIF_TRIGGER_START_COL_H, BIT_16_HTOL);
	return x;
}

void V5B_SifSetVSyncDelay(UINT16 x)
{
	V5_SetReg(V5_REG_SIF_VSYNC_DELAY_H, x, BIT_16_HTOL);
}

UINT16 V5B_SifGetVSyncDelay(void)
{
	UINT16 x = 0;

	x = (UINT16)V5_GetReg(V5_REG_SIF_VSYNC_DELAY_H, BIT_16_HTOL);
	return x;
}

/********************************************************************************
		I2C bus operation
*********************************************************************************/

/********************************************************************************
  Description:
	config I2C bus device address
  Parameters:
	adr: I2C bus device address
  Remarks:
*********************************************************************************/
void V5B_SifSetI2cDevAddr(UINT8 adr)
{
	adr >>= 1;
	adr |= 0x80;
	V5_SetReg(V5_REG_SIF_IIC_DEVADDR, adr, BIT_8);
}

/********************************************************************************
  Description:
	get I2C bus device address setting
  Returns:
	return I2C bus device address
  Remarks:
*********************************************************************************/
UINT8 V5B_SifGetI2cDevAddr(void)
{
	UINT8 x = 0;
	
	x = (UINT8)V5_GetReg(V5_REG_SIF_IIC_DEVADDR, BIT_8);
	return (x << 1);
}

/********************************************************************************
  Description:
	I2C bus batch write 
  Parameters:
	uNum: AeData length, must in [0, 7]
	AeData: data
  Note:
	The write data number must same at every register's address
	The update time is according to Ae Window's setting
  Remarks:
*********************************************************************************/
void V5B_SifI2cAeBatch(UINT8 uNum, TI2cBatch *AeData)
{
	UINT8	i = 0, j = 0, reg_B_ctrl = 0, uRegAeCount = 0;
	UINT16	regCnt = 0, reg_B_cnt = 0;
	
	if((uNum > 0) && (uNum < 8))
	{
		V5B_SifSetI2cInterval(0x0190);		//set i2c interval normal
		for(i = 0; i < uNum; i++)
		{
			regCnt = AeData[i].RegBytes & 0x03;
			regCnt <<= (i << 1);
			reg_B_cnt |= regCnt;
			V5_SetReg((V5_REG_SIF_AE0_DATA + uRegAeCount), AeData[i].RegAddr, BIT_8);
			uRegAeCount++;
			for(j = 0; j < AeData[i].RegBytes; j++)
			{
				V5_SetReg((V5_REG_SIF_AE0_DATA + uRegAeCount), AeData[i].RegVal[j], BIT_8);
				uRegAeCount++;
			}
		}
		V5_SetReg(V5_REG_SIF_IIC_BYTE, (AeData[0].RegBytes & 0x03), BIT_8);
		V5_SetReg(V5_REG_SIF_B_CNTH, reg_B_cnt, BIT_16_HTOL);
		
		reg_B_ctrl = uNum;	//The number of register addresses
		reg_B_ctrl |= 0x08;	//AE data update done bit.
		V5_SetReg(V5_REG_SIF_AEBUS_CTRL, (UINT8)reg_B_ctrl, BIT_8);
	}
}

void V5B_SifI2cAeBatchNow(UINT8 uNum, TI2cBatch *AeData)
{
	UINT8	i = 0, j = 0, reg_B_ctrl = 0, uRegAeCount = 0;
	UINT16	regCnt = 0, reg_B_cnt = 0;
	
	if((uNum > 0) && (uNum < 8))
	{
		V5B_SifSetI2cInterval(0xff00);		//set i2c interval larger
		for(i = 0; i < uNum; i++)
		{
			regCnt = AeData[i].RegBytes & 0x03;
			regCnt <<= (i << 1);
			reg_B_cnt |= regCnt;
			V5_SetReg((V5_REG_SIF_AE0_DATA + uRegAeCount), AeData[i].RegAddr, BIT_8);
			uRegAeCount++;
			for(j = 0; j < AeData[i].RegBytes; j++)
			{
				V5_SetReg((V5_REG_SIF_AE0_DATA + uRegAeCount), AeData[i].RegVal[j], BIT_8);
				uRegAeCount++;
			}
		}
		V5_SetReg(V5_REG_SIF_IIC_BYTE, (AeData[0].RegBytes & 0x03), BIT_8);
		V5_SetReg(V5_REG_SIF_B_CNTH, reg_B_cnt, BIT_16_HTOL);
		
		reg_B_ctrl = uNum;	//The number of register addresses
		reg_B_ctrl |= 0x10;	//AE data update done bit.
		V5_SetReg(V5_REG_SIF_AEBUS_CTRL, (UINT8)reg_B_ctrl, BIT_8);
	}
}

/********************************************************************************
  Description:
	I2C write function 
  Parameters:
	uAddr: the address of I2C slave device's register
	uVal: value
  Remarks:
*********************************************************************************/
UINT8 V5B_SifI2cWriteByte(UINT8 uAddr, UINT8 uVal)
{
	UINT8 uState = 0;
	UINT32 count = V5_ISP_I2CWAITTIME;
	
	while(count--)
	{
		uState = (UINT8)V5_GetReg(V5_REG_SIF_BUS_STATE, BIT_8);
		if((uState & 0x02) == 0x02)
			break;
	}
	if(!count)
		return FAILED;

	V5_SetReg(V5_REG_SIF_SNRADDR, uAddr, BIT_8);
	V5_SetReg(V5_REG_SIF_SNRWRDATAH, uVal, BIT_8);
	V5_SetReg(V5_REG_SIF_IIC_BYTE, 0x01, BIT_8);
	V5_SetReg(V5_REG_SIF_SNRACSCTR, 0x01, BIT_8);
	return SUCCEED;
}

/********************************************************************************
  Description:
	I2C write function
  Parameters:
	uAddr: the address of I2C slave device's register 
	uVal: value
  Remarks:
*********************************************************************************/
UINT8 V5B_SifI2cWriteWord(UINT8 uAddr, UINT16 uVal)
{
	UINT8 uState = 0;
	UINT32	count = V5_ISP_I2CWAITTIME;
	
	while(count--)
	{
		uState = (UINT8)V5_GetReg(V5_REG_SIF_BUS_STATE, BIT_8);
		if((uState & 0x02) == 0x02)
			break;
	}
	if(!count)
		return FAILED;

	V5_SetReg(V5_REG_SIF_SNRADDR, uAddr, BIT_8);
	V5_SetReg(V5_REG_SIF_SNRWRDATAH, uVal, BIT_16_HTOL);
	V5_SetReg(V5_REG_SIF_IIC_BYTE, 0x02, BIT_8);
	V5_SetReg(V5_REG_SIF_SNRACSCTR, 0x01, BIT_8);
	return SUCCEED;
}

/********************************************************************************
  Description:
	I2C write function
  Parameters:
	uAddr: the address of I2C slave device's register 
	uVal: value
  Remarks:
*********************************************************************************/
UINT8 V5B_SifI2cWriteTriWord(UINT8 uAddr, UINT32 uVal)
{
	UINT8 uState = 0;
	UINT32 count = V5_ISP_I2CWAITTIME;
	
	while(count--)
	{
		uState = (UINT8)V5_GetReg(V5_REG_SIF_BUS_STATE, BIT_8);
		if((uState & 0x02) == 0x02)
			break;
	}
	if(!count)
		return FAILED;

	V5_SetReg(V5_REG_SIF_SNRADDR, uAddr, BIT_8);
	V5_SetReg(V5_REG_SIF_SNRWRDATAH, uVal, BIT_24_HTOL);
	V5_SetReg(V5_REG_SIF_IIC_BYTE, 0x03, BIT_8);
	V5_SetReg(V5_REG_SIF_SNRACSCTR, 0x01, BIT_8);
	return SUCCEED;
}

/********************************************************************************
  Description:
	I2C read function
  Parameters:
	uAddr:		the address of I2C slave device's register 
	uVal:		return the value of this register
  Remarks:
*********************************************************************************/
UINT8 V5B_SifI2cReadByte(UINT8 uAddr, UINT8 *uVal)
{
	UINT8 uState = 0;
	UINT32 count = V5_ISP_I2CWAITTIME;
	
	while(count--)
	{
		uState = (UINT8)V5_GetReg(V5_REG_SIF_BUS_STATE, BIT_8);
		if((uState & 0x02) == 0x02)
			break;
	}
	if(!count)
		return FAILED;

	V5_SetReg(V5_REG_SIF_SNRADDR, uAddr, BIT_8);
	V5_SetReg(V5_REG_SIF_IIC_BYTE, 0x01, BIT_8);
	V5_SetReg(V5_REG_SIF_SNRACSCTR, 0x02, BIT_8);

	count = V5_ISP_I2CWAITTIME;
	while(count--)
	{
		uState = (UINT8)V5_GetReg(V5_REG_SIF_BUS_STATE, BIT_8);
		if((uState & 0x02) == 0x02)
			break;
	}
	if(!count)
		return FAILED;

	*uVal = (UINT8)V5_GetReg(V5_REG_SIF_SNRRDDATAH, BIT_8);
	return SUCCEED;
}

/********************************************************************************
  Description:
	I2C read function
  Parameters:
	uAddr:		the address of I2C slave device's register 
	uVal:		return the value of this register
  Remarks:
*********************************************************************************/
UINT8 V5B_SifI2cReadWord(UINT8 uAddr, UINT16 *uVal)
{
	UINT8 uState = 0;
	UINT32 count = V5_ISP_I2CWAITTIME;
	
	while(count--)
	{
		uState = (UINT8)V5_GetReg(V5_REG_SIF_BUS_STATE, BIT_8);
		if((uState & 0x02) == 0x02)
			break;
	}
	if(!count)
		return FAILED;

	V5_SetReg(V5_REG_SIF_SNRADDR, uAddr, BIT_8);
	V5_SetReg(V5_REG_SIF_IIC_BYTE, 0x02, BIT_8);
	V5_SetReg(V5_REG_SIF_SNRACSCTR, 0x02, BIT_8);

	count = V5_ISP_I2CWAITTIME;
	while(count--)
	{
		uState = (UINT8)V5_GetReg(V5_REG_SIF_BUS_STATE, BIT_8);
		if((uState & 0x02) == 0x02)
			break;
	}
	if(!count)
		return FAILED;

	*uVal = (UINT16)V5_GetReg(V5_REG_SIF_SNRRDDATAH, BIT_16_HTOL);
	return SUCCEED;
}

/********************************************************************************
  Description:
	I2C read function
  Parameters:
	uAddr:		the address of I2C slave device's register 
	uVal:		return the value of this register
  Remarks:
*********************************************************************************/
UINT8 V5B_SifI2cReadTriByte(UINT8 uAddr, UINT32 *uVal)
{
	UINT8 uState = 0;
	UINT32 count = V5_ISP_I2CWAITTIME;
	
	while(count--)
	{
		uState = (UINT8)V5_GetReg(V5_REG_SIF_BUS_STATE, BIT_8);
		if((uState & 0x02) == 0x02)
			break;
	}
	if(!count)
		return FAILED;

	V5_SetReg(V5_REG_SIF_SNRADDR, uAddr, BIT_8);
	V5_SetReg(V5_REG_SIF_IIC_BYTE, 0x03, BIT_8);
	V5_SetReg(V5_REG_SIF_SNRACSCTR, 0x02, BIT_8);

	count = V5_ISP_I2CWAITTIME;
	while(count--)
	{
		uState = (UINT8)V5_GetReg(V5_REG_SIF_BUS_STATE, BIT_8);
		if((uState & 0x02) == 0x02)
			break;
	}
	if(!count)
		return FAILED;

	*uVal = (UINT32)V5_GetReg(V5_REG_SIF_SNRRDDATAH, BIT_24_HTOL);
	return SUCCEED;
}

#endif //V578CMD_DISABLE
