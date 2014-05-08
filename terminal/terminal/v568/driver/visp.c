#include "driver.h"
#ifndef	V578CMD_DISABLE

#ifdef WIN32
TVispContext	gVispContext;
#else
static TVispContext	gVispContext;
#endif

static UINT8 gTmflk_50[MAX_FLK_ET] = { 2, 2, 3, 3, 3, 7, 7, 8, 8, 8, 12, 12, 13, 13, 13, 17, 17, 18, 18, 18, 22, 22, 23, 23, 23 };
static UINT8 gTmflk_60[MAX_FLK_ET] = { 3, 3, 3, 3, 3, 9, 9, 9, 9, 9, 15, 15, 15, 15, 15, 21, 21, 21, 21, 21, 27, 27, 27, 27, 27 };

static UINT8 gNTableDefault[17] = { 0x04, 0x04, 0x04, 0x04, 0x04, 0x06, 0x06, 0x06, 0x06, 0x08, 0x08, 0x0a, 0x0a, 0x0a, 0x0a, 0x08, 0x04 };

/*********************************************************************
*                            SIF Module                              *
*********************************************************************/

static void _ISR_VispSifVSync(void)
{
}

static void _ISR_VispSifFrameEnd(void)
{
}

void V5_VispSifIntSetCallback(UINT8 index, PVispIsrCallback pcall)
{
	gVispContext.sifisr[index] = pcall;
}

static void V5_VispInitI2cClk(void)
{
	UINT16 div = 0x010c;
	UINT32 mclk = 0;;

	if(gVispContext.pinfo == NULL)
		return;
	mclk = V5_CtrlGetMclk();
	if((gVispContext.pinfo->bustype == 0) && (gVispContext.pinfo->i2crate != 0))
	{
		div = (UINT16)(((mclk / (UINT32)gVispContext.pinfo->i2crate) + 3) >> 2) << 2;
		if(div < 0x40)
			div = 0x40;
		V5B_SifSetI2cclk(div);
	}
}

static void V5_InitSifParm(void)
{
	UINT8 mode = 0;
	TSnrInfo *pinfo = gVispContext.pinfo;

	if(pinfo == NULL)
		return;

	V5_VispInitI2cClk();
	mode = (((pinfo->snrtype & 0x01) << 6) | (0x01 << 0x05) | (pinfo->pwrcfg & 0x01) | (((~pinfo->pwrcfg) & 0x06)));
	V5B_SifSetSensorMode(mode);
	V5_Delay(1);

	mode = V5B_SifGetSensorOpMode();
	if(pinfo->bustype)
		mode |= 0x20;
	else
	{
		mode &= 0xdb;
		if(pinfo->i2ctype)
			mode |= 0x04;
	}
	V5B_SifSetSensorOpMode(mode);

	if(pinfo->pclk)
	{
		V5B_SifEnableSnrPClk();
		if(pinfo->clkcfg & 0x04)
			V5B_SifSetPClkNegEdge();
		if(pinfo->clkcfg & 0x02)
			V5B_SifEnableFifoVSyncDly();
	}

	if(pinfo->clkcfg & 0x01)
		V5B_SifEnableVSyncDly();

	V5_VispSifIntSetCallback(INT_SIF_VSYNC,		_ISR_VispSifVSync);
	V5_VispSifIntSetCallback(INT_SIF_FRAMEEND,	_ISR_VispSifFrameEnd);
}

void _ISR_VispSifIntHandle(UINT16 flag)
{
	//adjust according to interrupt prior
	flag = 0;
}

void V5_VispSifIntEnable(UINT8 index)
{
	UINT16 flag = 0;

	if(index >= INT_SIF_ALL)
		V5B_CpmSetIntEnSec(INT_SIF, 0xff);
	else
	{
		flag = V5B_CpmGetIntEnSec(INT_SIF);
		flag |= (0x01 << index);
		V5B_CpmSetIntEnSec(INT_SIF, flag);
	}
}

void V5_VispSifIntDisable(UINT8 index)
{
	UINT16 flag = 0;

	if(index >= INT_SIF_ALL)
		V5B_CpmSetIntEnSec(INT_SIF, 0);
	else
	{
		flag = V5B_CpmGetIntEnSec(INT_SIF);
		flag &= (~(0x01 << index));
		V5B_CpmSetIntEnSec(INT_SIF, flag);
	}
}

static void V5_VispSetSifReg(PTRegGroup parm)
{
	UINT16 i = 0;

	if(parm == NULL)
		return;
	for(i = 0; i < parm->len; i++)
	{
		V5_SetReg((UINT32)(V5_REG_SIF_BASE | parm->regval[i].adr), (UINT8)parm->regval[i].val, BIT_8);
		V5_Delay(parm->regval[i].wait);
	}
}

void V5_VispSetSensorRegList(PTRegGroup parm)
{
	UINT16 i = 0;

	if(gVispContext.pinfo == NULL)
		return;
	V5B_SifSetI2cDevAddr(gVispContext.pinfo->i2caddress);
	for(i = 0; i < parm->len; i++)
	{
		gVispContext.pinfo->snrSetRegCall((UINT8)parm->regval[i].adr, (UINT16)parm->regval[i].val);
		V5_Delay(parm->regval[i].wait);
	}
}

void V5_VispResetSensor(void)
{
	UINT8 mode = 0;

	mode = V5B_SifGetSensorMode();
	if(mode & 0x01)
		V5B_SifSetSensorMode(mode & 0xfe);
	else
		V5B_SifSetSensorMode(mode | 0x01);
	V5_Delay(1);
	V5B_SifSetSensorMode(mode);
	V5_Delay(1);
}

void V5_VispPoweronSensor(void)
{
	UINT8 mode = 0;
	TSnrInfo *pinfo = gVispContext.pinfo;

	if(gVispContext.pinfo == NULL)
		return;
	mode = V5B_SifGetSensorMode();
	mode = mode & 0xf9;
	V5B_SifSetSensorMode(mode | ((pinfo->pwrcfg) & 0x06));
	V5_Delay(1);
}

void V5_VispPoweroffSensor(void)
{
	UINT8 mode = 0;
	TSnrInfo *pinfo = gVispContext.pinfo;

	if(gVispContext.pinfo == NULL)
		return;
	mode = V5B_SifGetSensorMode();
	mode = (mode & 0xe1) | gVispContext.i2cmode;
	V5B_SifSetSensorMode(mode | ((~pinfo->pwrcfg) & 0x06));
	V5_Delay(1);
}

void V5_VispGetSnrReg(UINT8 uAddr, UINT16 *uVal)
{
	if(gVispContext.pinfo == NULL)
		return;
	if(gVispContext.pinfo->snrGetRegCall)
		gVispContext.pinfo->snrGetRegCall(uAddr, uVal);
}

void V5_VispSetSnrMirrorFlip(UINT8 val)
{
	if(gVispContext.pinfo == NULL)
		return;
	if(gVispContext.pinfo->snrMirrorFlipCall)
		gVispContext.pinfo->snrMirrorFlipCall(val);
}

/********************************************************************************
Description:
Set sensor power state
Parameters:
state: power state param SENSORSTATEenum
Returns:
SUCCEED	: if succesful
FAILED: failed
*********************************************************************************/
UINT16 V5_VispSetSnrState(UINT8 state)
{
	if(gVispContext.pinfo == NULL)
		return FAILED;
	if(gVispContext.snrstate == state)
		return FAILED;

	switch(state)
	{
	case SENSOR_POWERON:
		V5_VispSetSifReg(&gVispContext.pinfo->sifpwronseq);
		V5_VispSetSensorRegList(&gVispContext.pinfo->snrSizeCfg.pSizeCfg[gVispContext.index].snr);
		V5_VispSetEt();
		break;
	case SENSOR_STANDBY:
		V5_VispSetSifReg(&gVispContext.pinfo->sifstdbyseq);
		V5_VispSetSensorRegList(&gVispContext.pinfo->standby);
		break;
	case SENSOR_SLEEP:
		V5_VispSetSensorRegList(&gVispContext.pinfo->sleep);
		break;
	case SENSOR_POWEROFF:
		V5_VispSetSensorRegList(&gVispContext.pinfo->poweroff);
		V5_VispPoweroffSensor();
		break;
	default:
		break;
	}
	gVispContext.snrstate = state;
	return SUCCEED;
}

UINT8 V5_VispGetSnrState(void)
{
	return gVispContext.snrstate;
}

void V5_VispSetI2cPin(UINT8 mode)
{
	if((mode != I2C_NORMAL) && (mode != I2C_PUP) && (mode != I2C_PDOWN))
		mode = I2C_NORMAL;
	gVispContext.i2cmode = mode;
}


/*********************************************************************
*                            ISP Module                              *
*********************************************************************/

static void _ISR_VispIspAutoFunInt(void)
{
	static UINT8 valid = 0;

	if(gVispContext.snrstate != SENSOR_POWERON)
		return;

	// if the position that setting et is not in ae window,the value should be set at next frame beginning,
	// so gain delay should add 1 to assure gain and et effect in same frame
	if(valid < (gVispContext.pinfo->aeparm.gdelay + 1))
	{
		valid ++;
		return;
	}
	valid = 0;
	V5_VispAutoExpose();
}

static void _ISR_VispIspAntiFlicker(void)
{
	static UINT8 valid = 0;

	if(gVispContext.snrstate != SENSOR_POWERON)
		return;

	// if the position that setting et is not in ae window,the value should be set at next frame beginning,
	// so gain delay should add 1 to assure gain and et effect in same frame
	if(valid < (gVispContext.pinfo->aeparm.gdelay + 1))
	{
		valid ++;
		return;
	}
	valid = 0;
	if(gVispContext.enfliker)
		V5_VispDoFlk();
}

static void _ISR_VispIspAutoFocus(void)
{
}

void V5_VispIspIntSetCallback(UINT8 index, PVispIsrCallback pcall)
{
	gVispContext.vispisr[index] = pcall;
}

static void V5_InitIspParm(void)
{
	UINT8 i = 0;
	TSnrInfo *pinfo = gVispContext.pinfo;

	if(pinfo == NULL)
		return;
	V5B_IspSetNoiseTable(gNTableDefault);
	if(pinfo->snrtype)
	{
		V5B_SifSetI2cDevAddr(pinfo->i2cispaddress);
		for(i = 0; i < pinfo->snrisp.len; i++)
		{
			pinfo->snrSetRegCall((UINT8)pinfo->snrisp.regval[i].adr, (UINT16)pinfo->snrisp.regval[i].val);
			V5_Delay(pinfo->snrisp.regval[i].wait);
		}
		V5B_SifSetI2cDevAddr(pinfo->i2caddress);
		V5B_IspEnableYuvFmt();
	}
	else
	{
		V5B_IspDisableYuvFmt();

		V5_VispSetBrightness(gVispContext.brightness);
		V5_VispSetContrast(gVispContext.contrast);
		V5_VispSetSaturation(gVispContext.saturation);

		V5B_IspSetYMeanTarget(pinfo->aeparm.ytarget);
		V5B_IspSetYMeanThd(pinfo->aeparm.ythreshold);
		V5B_IspSetGlbGainDelay(pinfo->aeparm.gdelay);
		V5B_IspSetGlbGain(pinfo->aeparm.gain);

		V5_VispIspIntSetCallback(INT_ISP_Y_MEAN,		_ISR_VispIspAutoFunInt);
		V5_VispIspIntSetCallback(INT_ISP_ANTIF,			_ISR_VispIspAntiFlicker);
		V5_VispIspIntSetCallback(INT_ISP_AUTO_FOC,		_ISR_VispIspAutoFocus);

		VComputeGainThd(pinfo->aeparm.gtarget, pinfo->aeparm.gthreshold);
	}

	for(i = 0; i < pinfo->disp.len; i++)
	{
		V5_SetReg((UINT32)(V5_REG_ISP_BASE | pinfo->disp.regval[i].adr), (UINT8)pinfo->disp.regval[i].val, BIT_8);
		V5_Delay(pinfo->disp.regval[i].wait);
	}
}

void _ISR_VispIspIntHandle(UINT16 flag)
{
	if(flag & (0x01 << INT_ISP_Y_MEAN))
	{
		if(gVispContext.vispisr[INT_ISP_Y_MEAN])
			gVispContext.vispisr[INT_ISP_Y_MEAN]();
	}

	if(flag & (0x01 << INT_ISP_ANTIF))
	{
		if(gVispContext.vispisr[INT_ISP_ANTIF])
			gVispContext.vispisr[INT_ISP_ANTIF]();
	}
}

void V5_VispIspIntEnable(UINT8 index)
{
	UINT16 flag = 0;

	if(index >= INT_ISP_ALL)
		V5B_CpmSetIntEnSec(INT_ISP, 0xff);			//if index>maxintnum,set all int enable
	else
	{
		flag = V5B_CpmGetIntEnSec(INT_ISP);
		flag |= (0x01 << index);
		V5B_CpmSetIntEnSec(INT_ISP, flag);
	}
}

void V5_VispIspIntDisable(UINT8 index)
{
	UINT16 flag = 0;

	if(index >= INT_ISP_ALL)
		V5B_CpmSetIntEnSec(INT_ISP, 0);
	else
	{
		flag = V5B_CpmGetIntEnSec(INT_ISP);
		flag &= (~(0x01 << index));
		V5B_CpmSetIntEnSec(INT_ISP, flag);
	}
}


/*********************************************************************
*                            IPP Module                              *
*********************************************************************/

static void V5_CapSizerChk(void)
{
	UINT8 tmp = 0, tmp1 = 0, cratio = 0, pratio = 0, div = 0;
	UINT16 i2cclk = 0;
	TSize size;
	static UINT8 capflg = 0;

	if(gVispContext.pinfo == NULL)
		return;
	cratio = gVispContext.pinfo->snrSizeCfg.pSizeCfg[ gVispContext.index].snrclkratio;
	pratio = gVispContext.pinfo->snrSizeCfg.pSizeCfg[ gVispContext.index].snrpixelratio;
	tmp = V5B_IppGetSizerCtrl0();
	if((capflg == 0) && (tmp & 0x02))	// Capture Up Sizer Enable
	{
		V5B_SifDisableSyncGen();

		div = (gVispContext.pinfo->snrtype || (cratio == pratio)) ? 1 : 2;
		tmp = ((cratio + 1) << div) - 1;
		V5B_SifSetSnrClk(tmp);
		if(gVispContext.pinfo->pclk == 0)
		{
			tmp1 = ((pratio + 1) << div) - 1;
			V5B_SifSetPixelRate(tmp1);
		}
		i2cclk = V5B_SifGetI2cclk();
		V5B_SifSetI2cclk((i2cclk << div));

		V5_VispGetSize(gVispContext.index, &size);
		if(gVispContext.pinfo->snrSetETCall)
		{
			if(cratio)
				tmp = tmp / cratio;
			gVispContext.pinfo->snrSetETCall(gVispContext.env, (gVispContext.ae.snrclock / gVispContext.ae.freq) * gVispContext.ae.et / tmp, size, gVispContext.ae.snrclock);
		}
		V5B_IspSetTailBlank((UINT32)(size.cx << div));

		V5B_IppReset();
		V5B_SifEnableSyncGen();
		capflg = 1;
		return;
	}
	if((capflg != 0) && (tmp & 0x04))	// Capture Down Sizer Enable
	{
		V5B_SifDisableSyncGen();

		div = (gVispContext.pinfo->snrtype || (cratio == pratio)) ? 1 : 2;
		V5B_SifSetSnrClk(cratio);
		if(gVispContext.pinfo->pclk == 0)
			V5B_SifSetPixelRate(pratio);
		i2cclk = V5B_SifGetI2cclk();
		V5B_SifSetI2cclk((i2cclk >> div));

		V5_VispGetSize(gVispContext.index, &size);
		if(gVispContext.pinfo->snrSetETCall)
			gVispContext.pinfo->snrSetETCall(gVispContext.env, (gVispContext.ae.snrclock / gVispContext.ae.freq) * gVispContext.ae.et, size, gVispContext.ae.snrclock);
		V5B_IspSetTailBlank(0x20);

		V5B_IppReset();
		V5B_SifEnableSyncGen();
		capflg = 0;
		return;
	}
}

static void V5_InitIppParm(void)
{
}

void V5_IppSetDispSizerByWidth(UINT16 width)
{
	TSize wsize, dsize;
	TV5BIppSzDnFct fct;

	if(width == 0)
		return;
	width = (width >> 1) << 1;
	V5B_IppGetWinSize(&wsize);
	VCalcSizerDownHPara(wsize, width, &dsize, &fct);
	if((wsize.cx == width) || (fct.w1 == fct.w2))
	{
		V5B_IppEnableDPHSBP();
		V5B_IppEnableDPVSBP();
		dsize.cx = width;
		dsize.cy = wsize.cy;
	}
	else
	{
		V5B_IppSetDispHFactor(fct);
		V5B_IppSetDispVFactor(fct);
		V5B_IppDisableDPHSBP();
		V5B_IppDisableDPVSBP();
	}
	V5B_IppSetDispSize(dsize);
	V5_IppThumbSizerUpdateByWidth();
}

void V5_IppSetDispSizerByWidthEx1(TPoint winpt, TSize winsize, UINT16 width)
{
	winpt.x = (winpt.x >> 1) << 1;
	winsize.cx = (winsize.cx >> 1) << 1;
	width = (width >> 1) << 1;
	V5B_IppSetWinPoint(winpt);
	V5B_IppSetWinSize(winsize);
	V5_IppSetDispSizerByWidth(width);
}

void V5_IppSetDispSizerByWidthEx2(TSize imgsize, TPoint winpt, TSize winsize, UINT16 width)
{
	imgsize.cx = (imgsize.cx >> 1) << 1;
	V5B_IppSetImageSize(imgsize);
	V5_IppSetDispSizerByWidthEx1(winpt, winsize, width);
}

void V5_IppSetDispSizer(TSize size)
{
	TSize wsize, dsize;
	TV5BIppSzDnFct hfct, vfct;

	size.cx = (size.cx >> 1) << 1;
	V5B_IppGetWinSize(&wsize);
	VCalcSizerDownHPara(wsize, size.cx, &dsize, &hfct);
	if((wsize.cx == size.cx) || (hfct.w1 == hfct.w2))
		V5B_IppEnableDPHSBP();
	else
	{
		V5B_IppSetDispHFactor(hfct);
		V5B_IppDisableDPHSBP();
	}
	VCalcSizerDownVPara(wsize, size.cy, &dsize, &vfct);
	if((wsize.cy == size.cy) || (vfct.w1 == vfct.w2))
		V5B_IppEnableDPVSBP();
	else
	{
		V5B_IppSetDispVFactor(vfct);
		V5B_IppDisableDPVSBP();
	}
	V5B_IppSetDispSize(size);
	V5_IppThumbSizerUpdate();
}

void V5_IppSetDispSizerEx1(TPoint winpt, TSize winsize, TSize dsize)
{
	winpt.x = (winpt.x >> 1) << 1;
	winsize.cx = (winsize.cx >> 1) << 1;
	dsize.cx = (dsize.cx >> 1) << 1;
	V5B_IppSetWinPoint(winpt);
	V5B_IppSetWinSize(winsize);
	V5_IppSetDispSizer(dsize);
}

void V5_IppSetDispSizerEx2(TSize imgsize, TPoint winpt, TSize winsize, TSize dsize)
{
	imgsize.cx = (imgsize.cx >> 1) << 1;
	V5B_IppSetImageSize(imgsize);
	V5_IppSetDispSizerEx1(winpt, winsize, dsize);
}

void V5_IppSetCapSizerByWidth(UINT16 width)
{
	UINT16 maxval = 0;
	TSize wsize, csize, size;
	TV5BIppSzUpFct ufct;
	TV5BIppSzDnFct dfct;

	if(width == 0)
		return;
	width = (width >> 1) << 1;
	V5B_IppGetWinSize(&wsize);
	if(width <= wsize.cx)
	{
		V5B_IppDisableCapUpSizer();
		V5B_IppDisableCPUHSBP();
		V5B_IppDisableCPUVSBP();
		VCalcSizerDownHPara(wsize, width, &csize, &dfct);
		if((wsize.cx == width) || (dfct.w1 == dfct.w2))
		{
			V5B_IppEnableCPDHSBP();
			V5B_IppEnableCPDVSBP();
			csize.cx = width;
			csize.cy = wsize.cy;
		}
		else
		{
			V5B_IppSetCapDnHFactor(dfct);
			V5B_IppSetCapDnVFactor(dfct);
			V5B_IppDisableCPDHSBP();
			V5B_IppDisableCPDVSBP();
		}
		V5B_IppSetCapSize(csize);
		V5B_IppEnableCapDnSizer();
	}
	else
	{
		if((wsize.cx > IPP_CSIZER_HIN_MAX) || (wsize.cy > IPP_CSIZER_VIN_MAX))
			return;
		V5B_IppDisableCapDnSizer();
		V5B_IppDisableCPDHSBP();
		V5B_IppDisableCPDVSBP();

		maxval = wsize.cx << 1;
		maxval = (maxval > IPP_CSIZER_HOUT_MAX) ? IPP_CSIZER_HOUT_MAX : maxval;
		width = (width > maxval) ? maxval : width;
		VCalcSizerUpHPara(wsize, width, &csize, &ufct);
		V5B_IppSetCapUpHFactor(ufct);
		V5B_IppDisableCPUHSBP();

		maxval = wsize.cy << 1;
		maxval = (maxval > IPP_CSIZER_VOUT_MAX) ? IPP_CSIZER_VOUT_MAX : maxval;
		if(wsize.cy == maxval)
		{
			V5B_IppEnableCPUVSBP();
			csize.cy = maxval;
		}
		else
		{
			if(csize.cy > maxval)
			{
				csize.cy = maxval;
				VCalcSizerUpVPara(wsize, maxval, &size, &ufct);
			}
			V5B_IppSetCapUpVFactor(ufct);
			V5B_IppDisableCPUVSBP();
		}
		V5B_IppSetCapSize(csize);
		V5B_IppEnableCapUpSizer();
	}
	V5_IppThumbSizerUpdateByWidth();
	V5_CapSizerChk();
}

void V5_IppSetCapSizerByWidthEx1(TPoint winpt, TSize winsize, UINT16 width)
{
	UINT16 height = 0;

	winpt.x = (winpt.x >> 1) << 1;
	winsize.cx = (winsize.cx >> 1) << 1;
	width = (width >> 1) << 1;
	V5B_IppSetWinPoint(winpt);
	V5B_IppSetWinSize(winsize);
	V5_IppSetCapSizerByWidth(width);
	height = V5B_IppGetDispHeight();
	V5B_IppSetDispHeight(height);
}

void V5_IppSetCapSizerByWidthEx2(TSize imgsize, TPoint winpt, TSize winsize, UINT16 width)
{
	imgsize.cx = (imgsize.cx >> 1) << 1;
	V5B_IppSetImageSize(imgsize);
	V5_IppSetCapSizerByWidthEx1(winpt, winsize, width);
}

void V5_IppSetCapSizer(TSize size)
{
	UINT16 maxval = 0;
	TSize wsize, csize;
	TV5BIppSzUpFct uhfct, uvfct;
	TV5BIppSzDnFct dhfct, dvfct;

	size.cx = (size.cx >> 1) << 1;
	V5B_IppGetWinSize(&wsize);
	if(((size.cx < wsize.cx) && (size.cy > wsize.cy)) || ((size.cx > wsize.cx) && (size.cy < wsize.cy)))
		return;
	if((wsize.cx == size.cx) && (wsize.cy == size.cy))
	{
		V5B_IppDisableCPUHSBP();
		V5B_IppDisableCPUVSBP();
		V5B_IppDisableCapUpSizer();

		V5B_IppEnableCPDHSBP();
		V5B_IppEnableCPDVSBP();
		V5B_IppSetCapSize(size);
		V5B_IppEnableCapDnSizer();
	}
	else if((wsize.cx > size.cx) || (wsize.cy > size.cy))
	{
		V5B_IppDisableCPUHSBP();
		V5B_IppDisableCPUVSBP();
		V5B_IppDisableCapUpSizer();

		VCalcSizerDownHPara(wsize, size.cx, &csize, &dhfct);
		if((wsize.cx == size.cx) || (dhfct.w1 == dhfct.w2))
			V5B_IppEnableCPDHSBP();
		else
		{
			V5B_IppSetCapDnHFactor(dhfct);
			V5B_IppDisableCPDHSBP();
		}
		VCalcSizerDownVPara(wsize, size.cy, &csize, &dvfct);
		if((wsize.cy == size.cy) || (dvfct.w1 == dvfct.w2))
			V5B_IppEnableCPDVSBP();
		else
		{
			V5B_IppSetCapDnVFactor(dvfct);
			V5B_IppDisableCPDVSBP();
		}
		V5B_IppSetCapSize(size);
		V5B_IppEnableCapDnSizer();
	}
	else if((wsize.cx < size.cx) || (wsize.cy < size.cy))
	{
		if((wsize.cx > IPP_CSIZER_HIN_MAX) || (wsize.cy > IPP_CSIZER_VIN_MAX))
			return;
		V5B_IppDisableCPDHSBP();
		V5B_IppDisableCPDVSBP();
		V5B_IppDisableCapDnSizer();

		if(wsize.cx == size.cx)
			V5B_IppEnableCPUHSBP();
		else
		{
			maxval = wsize.cx << 1;
			maxval = (maxval > IPP_CSIZER_HOUT_MAX) ? IPP_CSIZER_HOUT_MAX : maxval;
			size.cx = (size.cx > maxval) ? maxval : size.cx;
			VCalcSizerUpHPara(wsize, size.cx, &csize, &uhfct);
			V5B_IppSetCapUpHFactor(uhfct);
			V5B_IppDisableCPUHSBP();
		}
		if(wsize.cy == size.cy)
			V5B_IppEnableCPUVSBP();
		else
		{
			maxval = wsize.cy << 1;
			maxval = (maxval > IPP_CSIZER_VOUT_MAX) ? IPP_CSIZER_VOUT_MAX : maxval;
			size.cy = (size.cy > maxval) ? maxval : size.cy;
			VCalcSizerUpVPara(wsize, size.cy, &csize, &uvfct);
			V5B_IppSetCapUpVFactor(uvfct);
			V5B_IppDisableCPUVSBP();
		}
		V5B_IppSetCapSize(size);
		V5B_IppEnableCapUpSizer();
	}
	else
		return;
	V5_IppThumbSizerUpdate();
	V5_CapSizerChk();
}

void V5_IppSetCapSizerEx1(TPoint winpt, TSize winsize, TSize csizer)
{
	UINT16 height = 0;

	winpt.x = (winpt.x >> 1) << 1;
	winsize.cx = (winsize.cx >> 1) << 1;
	csizer.cx = (csizer.cx >> 1) << 1;
	V5B_IppSetWinPoint(winpt);
	V5B_IppSetWinSize(winsize);
	V5_IppSetCapSizer(csizer);
	height = V5B_IppGetDispHeight();
	V5B_IppSetDispHeight(height);
}

void V5_IppSetCapSizerEx2(TSize imgsize, TPoint winpt, TSize winsize, TSize csize)
{
	imgsize.cx = (imgsize.cx >> 1) << 1;
	V5B_IppSetImageSize(imgsize);
	V5_IppSetCapSizerEx1(winpt, winsize, csize);
}

void V5_IppSetThumbSizerByWidth(UINT8 width)
{
	UINT8 input = 0;
	TSize isize, tsize;
	TV5BIppSzDnFct fct;

	input = V5B_IppGetThumbInput();
	switch(input)
	{
	case IPP_TMB_SPECIAL :
		V5B_IppGetWinSize(&isize);
		break;
	case IPP_TMB_DSIZER :
		V5B_IppGetDispSize(&isize);
		break;
	case IPP_TMB_CSIZER :
		V5B_IppGetCapSize(&isize);
		break;
	default :
		return;
	}

	if(isize.cy < IPP_TSIZER_VOUT_MIN)
		return;
	width = (width >> 1) << 1;
	width = (width < IPP_TSIZER_HOUT_MIN) ? IPP_TSIZER_HOUT_MIN : width;
	width = (width > IPP_TSIZER_HOUT_MAX) ? IPP_TSIZER_HOUT_MAX : width;
	VCalcSizerDownHPara(isize, width, &tsize, &fct);
	if((width == isize.cx) || (fct.w1 == fct.w2))
	{
		V5B_IppEnableTMHSBP();
		V5B_IppEnableTMVSBP();
		tsize.cx = width;
		tsize.cy = isize.cy;
	}
	else
	{
		V5B_IppSetThumbHFactor(fct);
		V5B_IppSetThumbVFactor(fct);
		V5B_IppDisableTMHSBP();
		V5B_IppDisableTMVSBP();
	}
	V5B_IppSetThumbSize(tsize);
}

void V5_IppThumbSizerUpdateByWidth(void)
{
	UINT8 tmpw = 0;

	if(IPP_TMB_SIZER_EN & V5B_IppGetSizerCtrl0())
	{
		tmpw = (UINT8)V5B_IppGetThumbWidth();
		V5_IppSetThumbSizerByWidth(tmpw);
	}
}

void V5_IppSetThumbSizer(TSize size)
{
	UINT8 input = 0;
	TSize isize, tsize;
	TV5BIppSzDnFct fct;

	input = V5B_IppGetThumbInput();
	switch(input)
	{
	case IPP_TMB_SPECIAL :
		V5B_IppGetWinSize(&isize);
		break;
	case IPP_TMB_DSIZER :
		V5B_IppGetDispSize(&isize);
		break;
	case IPP_TMB_CSIZER :
		V5B_IppGetCapSize(&isize);
		break;
	default :
		return;
	}

	if((isize.cx < IPP_TSIZER_HOUT_MIN) || (isize.cy < IPP_TSIZER_VOUT_MIN))
		return;
	size.cx = (size.cx >> 1) << 1;
	size.cx = (size.cx < IPP_TSIZER_HOUT_MIN) ? IPP_TSIZER_HOUT_MIN : size.cx;
	size.cx = (size.cx > IPP_TSIZER_HOUT_MAX) ? IPP_TSIZER_HOUT_MAX : size.cx;
	size.cy = (size.cy < IPP_TSIZER_VOUT_MIN) ? IPP_TSIZER_VOUT_MIN : size.cy;
	size.cy = (size.cy > IPP_TSIZER_VOUT_MAX) ? IPP_TSIZER_VOUT_MAX : size.cy;
	VCalcSizerDownHPara(isize, size.cx, &tsize, &fct);
	if((size.cx == isize.cx) || (fct.w1 == fct.w2))
		V5B_IppEnableTMHSBP();
	else
	{
		V5B_IppSetThumbHFactor(fct);
		V5B_IppDisableTMHSBP();
	}
	VCalcSizerDownVPara(isize, size.cy, &tsize, &fct);
	if((size.cy == isize.cy) || (fct.w1 == fct.w2))
		V5B_IppEnableTMVSBP();
	else
	{
		V5B_IppSetThumbVFactor(fct);
		V5B_IppDisableTMVSBP();
	}
	V5B_IppSetThumbSize(size);
}

void V5_IppThumbSizerUpdate(void)
{
	TSize size;

	if(IPP_TMB_SIZER_EN & V5B_IppGetSizerCtrl0())
	{
		V5B_IppGetThumbSize(&size);
		V5_IppSetThumbSizer(size);
	}
}

void V5_IppSetDataSrc(UINT8 sel)
{
	UINT8 presel = 0;
	TSize size;

	presel = V5B_IppGetDataSelect();
	V5B_IppSetDataSelect(sel);
	if((presel == IPP_DATA_LBUF) && (sel == IPP_DATA_ISP))
	{
		V5B_IspGetImageSize(&size);
		V5B_IppSetImageSize(size);
	}
}

/*********************************************************************
*                            VISP Module                             *
*********************************************************************/

/********************************************************************************
  Description:
	Initialize VISP module's context
  Note:
	This function must be called when system is power on. Indict the 
	SIF (sensor interface), ISP (image signal processing) module and 
	external sensor's context is not valid.
  Remarks:
*********************************************************************************/
void V5_VispInitContext(void)
{
	gVispContext.valid    = FALSE;
	gVispContext.snrstate = SENSOR_NOINIT;
	gVispContext.i2cmode  = I2C_NORMAL;
}

PTVispContext V5_VispGetContext(void)
{
	return &gVispContext;
}

static void V5_VispSnrInfo2Context(void)
{
	if(gVispContext.pinfo == NULL)
		return;

	gVispContext.brightness = gVispContext.pinfo->brightness ;
	gVispContext.contrast   = gVispContext.pinfo->contrast ;
	gVispContext.saturation	= gVispContext.pinfo->saturation;

	gVispContext.ae.ytarget = gVispContext.pinfo->aeparm.ytarget;
	gVispContext.ae.ythd    = gVispContext.pinfo->aeparm.ythreshold;
	gVispContext.ae.ytop    = gVispContext.pinfo->aeparm.ytop;
	gVispContext.ae.gain    = gVispContext.pinfo->aeparm.gain;
	gVispContext.ae.gtarget = gVispContext.pinfo->aeparm.gtarget;
	gVispContext.ae.gthd    = gVispContext.pinfo->aeparm.gthreshold;
	gVispContext.ae.et      = ET_ONE_STEP * 3;
	gVispContext.ae.freq	= (gVispContext.env == FREQ_60HZ) ? EPFREQ_60HZ : EPFREQ_50HZ;
	gVispContext.ae.maxet   = gVispContext.pinfo->snrSizeCfg.pSizeCfg[0].maxet;
	gVispContext.ae.minet   = gVispContext.pinfo->aeparm.minet;
	gVispContext.ae.maxgain = gVispContext.pinfo->aeparm.maxgain;
	gVispContext.ae.mingain = gVispContext.pinfo->aeparm.mingain;
	gVispContext.ae.speed   = (gVispContext.pinfo->aeparm.speed & 0x03);

	gVispContext.enflash = 0;
	gVispContext.flashparm.flashstep = gVispContext.pinfo->flashparm.flashstep;
	gVispContext.flashparm.yflashthd = gVispContext.pinfo->flashparm.yflashthd;
}

static UINT8 V5_VispSnrQuerry(void)
{
	UINT8 flag = FAILED, mode = 0;
	UINT16 i = 0, div = 0x010c;
	UINT32 mclk = 0;;

	for(i = 0; i < gSensorNum; i++)
	{
		if(gPSensorInfo[i] == NULL)
			continue;

		mclk = V5_CtrlGetMclk();
		if((gPSensorInfo[i]->bustype == 0) && (gPSensorInfo[i]->i2crate != 0))
		{
			div = (UINT16)(((mclk / (UINT32)gPSensorInfo[i]->i2crate) + 3) >> 2) << 2;
			if(div < 0x40)
				div = 0x40;
			V5B_SifSetI2cclk(div);
		}

		mode = (((gPSensorInfo[i]->snrtype & 0x01) << 6) | (0x01 << 0x05) | (gPSensorInfo[i]->pwrcfg & 0x07));
		V5B_SifSetSensorMode(mode);
		V5_Delay(1);

		mode = V5B_SifGetSensorOpMode();
		if(gPSensorInfo[i]->bustype)
			mode |= 0x20;
		else
		{
			mode &= 0xdb;
			if(gPSensorInfo[i]->i2ctype)
				mode |= 0x04;
		}
		V5B_SifSetSensorOpMode(mode);
		V5B_SifSetSnrClk(gPSensorInfo[i]->snrSizeCfg.pSizeCfg[0].snrclkratio);
		V5B_SifSetI2cDevAddr(gPSensorInfo[i]->i2caddress);

		if(gPSensorInfo[i]->snrIdChkCall)
			flag = gPSensorInfo[i]->snrIdChkCall();
#if 1
		if(flag == SUCCEED)
		{
			gVispContext.pinfo = gPSensorInfo[i];
			V5_VispSnrInfo2Context();
			for(i = 0; i < gVispContext.pinfo->dsif.len ; i++)
			{
				V5_SetReg((UINT32)(gVispContext.pinfo->dsif.regval[i].adr | V5_REG_SIF_BASE), (UINT8)gVispContext.pinfo->dsif.regval[i].val, BIT_8);
				V5_Delay(gVispContext.pinfo->dsif.regval[i].wait);
			}
			return flag;
		}
		else
		{
			if(i == (gSensorNum - 1))
				return flag;
			continue;
		}
	}
#else
//		if(flag == SUCCEED)
		{
			gVispContext.pinfo = gPSensorInfo[i];
			V5_VispSnrInfo2Context();
			for(i = 0; i < gVispContext.pinfo->dsif.len ; i++)
			{
				V5_SetReg((UINT32)(gVispContext.pinfo->dsif.regval[i].adr | V5_REG_SIF_BASE), (UINT8)gVispContext.pinfo->dsif.regval[i].val, BIT_8);
				V5_Delay(gVispContext.pinfo->dsif.regval[i].wait);
			}
			return SUCCEED;
		}
		//else
		//{
		//	if(i == (gSensorNum - 1))
		//		return flag;
		//	continue;
		//}
	}
#endif
	return flag;
}

/********************************************************************************
  Description:
	Application level send initialize data to SIF, ISP, sensor module
  Parameters:
	info: point to the start address of SIF's initialize data PTSnrInfo
  Note:
	This function must be called before use other function of this RDK,
	or the SIF will be abnormal.
  Returns:
	SUCCEED	:  if succesful
	FAILED:  otherwise
  Remarks:
*********************************************************************************/
void V5_VispSetSensorInfo(void)
{
	gVispContext.index    = 0xff;
	gVispContext.enfliker = 0;
	gVispContext.env      = FREQ_50HZ;

	gVispContext.valid    = TRUE;
	gVispContext.snrstate = SENSOR_POWEROFF;
}

/********************************************************************************
Description:
Ipp module work mode setting
Parameters:
mode:	PREVIEW_MODE,
PREVIEWBRC_MODE,
PREVIEWFRAME_MODE,
CAPTURESTILL_MODE,
CAPTUREFRAME_MODE,
PCCAMERA_MODE,
PCCAMERA_FRAME_MODE,
DISPLAYSTILL_MODE,
ENCODE_MODE,	
DECODE_MODE,
DECODEIPP_MODE,
DECODEFRAME_MODE,
Remarks:
*********************************************************************************/
void V5_VispSetMode(UINT8 mode)
{
	TSize size;

	if(gVispContext.mode != mode)
		V5B_IppReset();
	gVispContext.mode = mode;
	V5B_SifStopCapture();
	switch(mode)
	{
	case VISP_PREVIEWFRAME_MODE :
		V5B_IppSetWorkMode(IPP_MODE_VIEW);
		V5_IppSetDataSrc(IPP_DATA_ISP);
		V5B_IppEnableDispSizer();
		V5B_IppDisableCapDnSizer();
		V5B_IppDisableCapUpSizer();
		V5B_IppDisableThumbSizer();
		V5B_IppEnableFrame();
		break;
	case VISP_PREVIEW_MODE :
		V5B_IppSetWorkMode(IPP_MODE_VIEW);
		V5_IppSetDataSrc(IPP_DATA_ISP);
		V5B_IppEnableDispSizer();
		V5B_IppDisableCapDnSizer();
		V5B_IppDisableCapUpSizer();
		V5B_IppDisableThumbSizer();
		V5B_IppDisableFrame();
		break;
	case VISP_PREVIEWBRC_MODE :
		V5B_IppSetWorkMode(IPP_MODE_VIEW);
		V5_IppSetDataSrc(IPP_DATA_ISP);
		V5B_IppEnableDispSizer();
		V5B_IppEnableCapDnSizer();
		V5B_IppEnableCapUpSizer();
		V5B_IppDisableThumbSizer();
		V5B_IppDisableFrame();
		break;
	case VISP_CAPTURE_MODE :
		V5B_IppSetWorkMode(IPP_MODE_CAP);
		V5_IppSetDataSrc(IPP_DATA_ISP);
		V5B_IppEnableDispSizer();
		V5B_IppDisableThumbSizer();
		V5B_IppGetCapSize(&size);
		V5_IppSetCapSizer(size);
		V5B_IppDisableFrame();
		break;
	case VISP_CAPTURETHUMB_MODE :
		V5B_IppSetWorkMode(IPP_MODE_CAP);
		V5_IppSetDataSrc(IPP_DATA_ISP);
		V5B_IppEnableDispSizer();
		V5B_IppEnableThumbSizer();
		V5B_IppGetCapSize(&size);
		V5_IppSetCapSizer(size);
		V5B_IppDisableFrame();
		break;
	case VISP_CAPTUREFRAME_MODE :	
		V5B_IppSetWorkMode(IPP_MODE_CAP);
		V5_IppSetDataSrc(IPP_DATA_ISP);
		V5B_IppEnableDispSizer();
		V5B_IppDisableCapDnSizer();
		V5B_IppDisableCapUpSizer();
		V5B_IppDisableThumbSizer();
		V5B_IppEnableFrame();
		break;
	case VISP_DISPLAY_MODE :
		V5B_IppSetWorkMode(IPP_MODE_DISP);
		V5_IppSetDataSrc(IPP_DATA_LBUF);
		V5B_IppEnableDispSizer();
		V5B_IppDisableCapDnSizer();
		V5B_IppDisableCapUpSizer();
		V5B_IppDisableThumbSizer();
		V5B_IppDisableFrame();
		break;
	case VISP_ENCODE_DECODE_MODE :
		V5B_IppSetWorkMode(IPP_MODE_VIEW);
		V5_IppSetDataSrc(IPP_DATA_ISP);
		V5B_IppEnableDispSizer();
		V5B_IppDisableCapDnSizer();
		V5B_IppDisableCapUpSizer();
		V5B_IppDisableThumbSizer();
		V5B_IppDisableFrame();
		break;
	case VISP_DECODEIPP_MODE :
		V5B_IppSetWorkMode(IPP_MODE_DEC);
		V5_IppSetDataSrc(IPP_DATA_LBUF);
		V5B_IppEnableDispSizer();
		V5B_IppDisableCapDnSizer();
		V5B_IppDisableCapUpSizer();
		V5B_IppDisableThumbSizer();
		V5B_IppDisableFrame();
		break;
	case VISP_DECODEFRAME_MODE :
		V5B_IppEnableFrame();
		V5B_IppSetWorkMode(IPP_MODE_DEC);
		V5_IppSetDataSrc(IPP_DATA_LBUF);
		V5B_IppEnableDispSizer();
		V5B_IppDisableCapDnSizer();
		V5B_IppDisableCapUpSizer();
		V5B_IppDisableThumbSizer();
		break;
	case VISP_NOSTREAM_MODE :
		break;
	}
}

UINT8 V5_VispGetMode(void)
{
	return gVispContext.mode;
}

/********************************************************************************
  Description:
		Open SIF, ISP, sensor module
  Note:
  Returns:
	SUCCEED	:  if succesful
	FAILED:  otherwise
  Remarks:
*********************************************************************************/
UINT8 V5_VispOpen(void)
{
	if(gVispContext.valid)
	{
		V5_VispSnrQuerry();

		V5_InitSifParm();							//init sif
		V5_InitIspParm();							//init isp
		V5_InitIppParm();
		V5_VispSetMode(VISP_NOSTREAM_MODE);
		V5_CtrlIntSetCallback(INT_ISP, _ISR_VispIspIntHandle);		//isp callback handle
		return SUCCEED;
	}
	return FAILED;
}

void V5_VispClose(void)
{
}

void V5_VispSetDefaultParm(void)
{
	TSize size;
 
	V5_InitIspParm();
	
	V5_VispGetSize(gVispContext.index, &size);
	V5B_IspSetImageSize(size);
	V5B_IspSetPixelRate((gVispContext.pinfo->snrSizeCfg.pSizeCfg[ gVispContext.index].snrpixelratio + 1));
	gVispContext.brightness = gVispContext.pinfo->brightness ;
	gVispContext.contrast   = gVispContext.pinfo->contrast ;
	gVispContext.saturation	= gVispContext.pinfo->saturation;
	V5_VispSetBrightness(gVispContext.brightness);
	V5_VispSetContrast(gVispContext.contrast);
	V5_VispSetSaturation(gVispContext.saturation);
	gVispContext.ae.gain    = gVispContext.pinfo->aeparm.gain;
	gVispContext.ae.ytarget = gVispContext.pinfo->aeparm.ytarget;
	gVispContext.ae.ythd    = gVispContext.pinfo->aeparm.ythreshold;
}

static void V5_VispSetHistQVal(void)
{
	UINT32 qval = 0;
	TSize size;

	V5B_IppGetDispSize(&size);
	qval = (UINT32)(size.cx * size.cy * gVispContext.pinfo->histparm.ratio) / 100;
	V5B_IppSetHistQ(qval);
}

void V5_VispGetHistStat(PTHistStat pdata)
{
	V5B_IppEnableHist();
	pdata->x_cur = V5B_IppGetHistXCur();
	pdata->x_avg = V5B_IppGetHistXAvg();
	V5B_IppGetHistResult(pdata->buf);
}

/*********************************************************************
*                 Brightness & Contrast & Saturation                 *
*********************************************************************/

void V5_VispSetBrightness(UINT8 val)
{
	UINT8 gamma[17];
	SINT16 bright = 0;
	SINT16 contrast = 0;

	if(val > V5_ISP_MAX_BRIGHT_CONTRAST)
		val = V5_ISP_MAX_BRIGHT_CONTRAST;
	if(val < V5_ISP_MIN_BRIGHT_CONTRAST)
		val =V5_ISP_MIN_BRIGHT_CONTRAST;
	gVispContext.brightness = val;

	bright = (SINT16)gVispContext.brightness - 32;
	contrast = (SINT16)gVispContext.contrast - 32;
	CalculateNewGammaCurve(gVispContext.pinfo->gammaRGB, contrast, bright, gamma);

	V5B_IspSetRGammaStart(gamma);
	V5B_IspSetGGammaStart(gamma);
	V5B_IspSetBGammaStart(gamma);
}

UINT8 V5_VispGetBrightness(void)
{
	return gVispContext.brightness;
}

void V5_VispSetContrast(UINT8 val)
{
	UINT8 gamma[17];
	SINT16 bright = 0;
	SINT16 contrast = 0;

	if(val > V5_ISP_MAX_BRIGHT_CONTRAST)
		val = V5_ISP_MAX_BRIGHT_CONTRAST;
	if(val < V5_ISP_MIN_BRIGHT_CONTRAST)
		val =V5_ISP_MIN_BRIGHT_CONTRAST;
	gVispContext.contrast = val;

	bright = (SINT16)gVispContext.brightness - 32;
	contrast = (SINT16)gVispContext.contrast - 32;
	CalculateNewGammaCurve(gVispContext.pinfo->gammaRGB, contrast, bright, gamma);

	V5B_IspSetRGammaStart(gamma);
	V5B_IspSetGGammaStart(gamma);
	V5B_IspSetBGammaStart(gamma);
}

UINT8 V5_VispGetContrast(void)
{
	return gVispContext.contrast;
}

void V5_VispSetSaturation(UINT8 val)
{
	if(val > V5_ISP_MAX_SATURATION)
		val = V5_ISP_MAX_SATURATION;
	gVispContext.saturation = val;

	val <<= 1;
	V5B_IspSetSat(val);
}

UINT8 V5_VispGetSaturation(void)
{
	return gVispContext.saturation;
}

/*********************************************************************
*                        Global Size Adjust                          *
*********************************************************************/

void V5_VispSetDispSizerByWidth(UINT8 index, TPoint winpt, TSize winsize, UINT16 width)
{
	UINT8 temp = 0;
	TSize imgsize;

	V5_VispGetSize(index, &imgsize);
	V5_IppSetDispSizerByWidthEx2(imgsize, winpt, winsize, width);

	temp = V5B_SifDisableSyncGen();
	V5_VispSetImageIndex(index);
	V5B_SifSetSensorOpMode(temp);
}

void V5_VispSetDispSizer(UINT8 index, TPoint winpt, TSize winsize, TSize dsize)
{
	UINT8 temp = 0;
	TSize imgsize;

	V5_VispGetSize(index, &imgsize);
	V5_IppSetDispSizerEx2(imgsize, winpt, winsize, dsize);

	temp = V5B_SifDisableSyncGen();
	V5_VispSetImageIndex(index);
	V5B_SifSetSensorOpMode(temp);
}

void V5_VispSetCapSizerByWidth(UINT8 index, TPoint winpt, TSize winsize, UINT16 width)
{
	UINT8 temp = 0;
	TSize imgsize;

	V5_VispGetSize(index, &imgsize);
	V5_IppSetCapSizerByWidthEx2(imgsize, winpt, winsize, width);

	temp = V5B_SifDisableSyncGen();
	V5_VispSetImageIndex(index);
	V5B_SifSetSensorOpMode(temp);
}

void V5_VispSetCapSizer(UINT8 index, TPoint winpt, TSize winsize, TSize csize)
{
	UINT8 temp = 0;
	TSize imgsize;

	V5_VispGetSize(index, &imgsize);
	V5_IppSetCapSizerEx2(imgsize, winpt, winsize, csize);

	temp = V5B_SifDisableSyncGen();
	V5_VispSetImageIndex(index);
	V5B_SifSetSensorOpMode(temp);
}

/*********************************************************************
*                            Image Size                              *
*********************************************************************/

void V5_VispSetImageIndex(UINT8 index)
{	
	UINT8 i = 0;
	UINT16 start = 0;
	TSize size;

	if(gVispContext.pinfo == NULL)
		return;

	if(index >= gVispContext.pinfo->snrSizeCfg.len )
		index = gVispContext.pinfo->snrSizeCfg.len -1;
	if(index == gVispContext.index)
		return;

	V5_VispGetSize(index, &size);
	size.cx = (size.cx >> 1) << 1;

	V5B_SifSetHrefLength(size.cx - 1);
	start = V5B_SifGetVrefStart();
	V5B_SifSetVrefStop(size.cy + start);
	V5B_SifSetSnrClk(gVispContext.pinfo->snrSizeCfg.pSizeCfg[index].snrclkratio);
	if(gVispContext.pinfo->pclk == 0)
		V5B_SifSetPixelRate(gVispContext.pinfo->snrSizeCfg.pSizeCfg[index].snrpixelratio);
	for(i = 0; i < gVispContext.pinfo->snrSizeCfg.pSizeCfg[index].visp.len ; i++)
	{
		V5_SetReg((UINT32)(gVispContext.pinfo->snrSizeCfg.pSizeCfg[index].visp.regval[i].adr),
					(UINT8)gVispContext.pinfo->snrSizeCfg.pSizeCfg[index].visp.regval[i].val, BIT_8);
		V5_Delay(gVispContext.pinfo->snrSizeCfg.pSizeCfg[index].visp.regval[i].wait);
	}

	V5B_IspSetImageSize(size);
	V5B_IspSetPixelRate((gVispContext.pinfo->snrSizeCfg.pSizeCfg[index].snrpixelratio + 1));
	V5_VispSetHistQVal();

	if(gVispContext.snrstate == SENSOR_POWERON)
	{
		if(gVispContext.pinfo->snrrst)
			V5_VispResetSensor();
		V5_VispSetSensorRegList(&gVispContext.pinfo->snrSizeCfg.pSizeCfg[index].snr);
		gVispContext.index = index;
		gVispContext.ae.maxet = gVispContext.pinfo->snrSizeCfg.pSizeCfg[gVispContext.index].maxet;
		V5_VispSetEt();
	}
	gVispContext.index = index;
}

UINT8 V5_VispGetImageIndex(void)
{
	return gVispContext.index;
}

UINT8 V5_VispGetImageIndexBySize(TSize size)
{
	UINT8 i = 0;

	for(i = 0; i < gVispContext.pinfo->snrSizeCfg.len; i++)
	{
		if((size.cx == gVispContext.pinfo->snrSizeCfg.pSizeCfg[i].size.cx) && (size.cy == gVispContext.pinfo->snrSizeCfg.pSizeCfg[i].size.cy))
			return i;
	}
	return 0xff;
}

UINT8 V5_VispGetImageIndexByWidth(UINT16 width)
{
	UINT8 i = 0;

	for(i = 0; i < gVispContext.pinfo->snrSizeCfg.len; i++)
	{
		if(width == gVispContext.pinfo->snrSizeCfg.pSizeCfg[i].size.cx)
			return i;
	}
	return 0xff;
}

void V5_VispGetSize(UINT8 index, TSize *size)
{
	if(gVispContext.pinfo == NULL)
		return;
	if(index >= gVispContext.pinfo->snrSizeCfg.len )
		index = gVispContext.pinfo->snrSizeCfg.len - 1;
	*size = gVispContext.pinfo->snrSizeCfg.pSizeCfg[index].size;
}

UINT8 V5_VispGetCurrentSize(TSize *size)
{
	if(gVispContext.pinfo == NULL)
		return FAILED;
	if((gVispContext.index == 0xff) | (gVispContext.index >= gVispContext.pinfo->snrSizeCfg.len))
		return FAILED;
	*size = gVispContext.pinfo->snrSizeCfg.pSizeCfg[gVispContext.index].size;
	return SUCCEED;
}

/*********************************************************************
*                            Auto Focus                              *
*********************************************************************/

void V5_VispStartAutoFocus(void)
{
	/*	disable AE */
	//to be add
	/* disable AWB */
	V5B_IspDisableAwb();
	/* enable auto focus */
	V5B_IspEnableAutoFocus();
}

/* stop auto focus */
void V5_VispStopAutoFocus(void)
{
	/* disable auto focus */
	V5B_IspDisableAutoFocus();
	/* enable AE */
	//to be add
	/* enable AWB */
	V5B_IspEnableAwb();
}

/*********************************************************************
*                                 AE                                 *
*********************************************************************/

void V5_VispSetYTarget(UINT8 val)
{
	V5B_IspSetYMeanTarget(val);
	gVispContext.ae.ytarget = val;
}

UINT8 V5_VispGetYTarget(void)
{
	return gVispContext.ae.ytarget;
}

void V5_VispSetYThd(UINT8 val)
{
	V5B_IspSetYMeanThd(val);
	gVispContext.ae.ythd = val;
}

UINT8 V5_VispGetYThd(void)
{
	return gVispContext.ae.ythd;
}

void V5_VispSetYTop(UINT8 val)
{
	gVispContext.ae.ytop = val;
}

UINT8 V5_VispGetYTop(void)
{
	return gVispContext.ae.ytop;
}

UINT8 V5_VispGetYTopDefault(void)
{
	return gVispContext.pinfo->aeparm.ytop;
}

void V5_VispSetMaxEt(UINT8 et)
{
	gVispContext.ae.maxet = et;
}

UINT8 V5_VispGetMaxEt(void)
{
	return gVispContext.ae.maxet;
}

UINT8 V5_VispGetMaxEtDefault(void)
{
	return gVispContext.pinfo->snrSizeCfg.pSizeCfg[gVispContext.index].maxet;
}

void V5_VispSetMinEt(UINT8 et)
{
	gVispContext.ae.minet = et;
}

UINT8 V5_VispGetMinEt(void)
{
	return gVispContext.ae.minet;
}

void V5_VispSetMaxGain(UINT8 gain)
{
	gVispContext.ae.maxgain = gain;
}

UINT8 V5_VispGetMaxGain(void)
{
	return gVispContext.ae.maxgain;
}

void V5_VispSetMinGain(UINT8 gain)
{
	gVispContext.ae.mingain = gain;
}

UINT8 V5_VispGetMinGain(void)
{
	return gVispContext.ae.mingain;
}

/**********************************************************************************
Description:
Set speed
Parameters:
speed:	0--slow
		1--normal
		2--fast
		3--fastest
Return:
Remarks:
*********************************************************************************/
void V5_VispSetAeSpeed(UINT8 speed)
{
	gVispContext.ae.speed = speed & 0x03;
}

/**********************************************************************************
Description:
Get speed
Parameters:
Return:
speed:	0--slow
		1--normal
		2--fast
		3--fastest
Remarks:
*********************************************************************************/
UINT8 V5_VispGetAeSpeed(void)
{
	return gVispContext.ae.speed;
}

void V5_VispSetCurEt(UINT32 et)
{
	gVispContext.ae.et = et;
}

UINT32 V5_VispGetCurEt(void)
{
	return gVispContext.ae.et;
}

UINT8 V5_VispGetCurGain(void)
{
	return gVispContext.ae.gain;
}

void V5_VispEnableAe(void)
{
	gVispContext.enae = 1;
	V5_CtrlIntEnable(INT_ISP);
	V5_VispIspIntEnable(INT_ISP_Y_MEAN);
	V5B_IspDisableAutoFocus();
}

void V5_VispDisableAe(void)
{
	gVispContext.enae = 0;
	V5_VispIspIntDisable(INT_ISP_Y_MEAN);
}

void V5_VispEnableAeQuery(void)
{
	V5_CtrlIntDisable(INT_ISP);
	V5_VispIspIntEnable(INT_ISP_Y_MEAN);
	V5B_IspDisableAutoFocus();
	gVispContext.enae = 1;
}

void V5_VispDisableAeQuery(void)
{
	gVispContext.enae = 0;
	V5_VispIspIntDisable(INT_ISP_Y_MEAN);
}

UINT8 V5_VispIsAe(void)
{
	return gVispContext.enae;
}

void V5_VispSetEt(void)
{
	UINT32 et = 0, tmp = 0;
	TSize size;
	PTAeAdjust ae = NULL;

	tmp = V5_CtrlGetMclk();
	ae = (&gVispContext.ae);
	if(gVispContext.pinfo->pclk)
		ae->snrclock = gVispContext.pinfo->pclkval;
	else
		ae->snrclock = tmp / (gVispContext.pinfo->snrSizeCfg.pSizeCfg[gVispContext.index].snrpixelratio + 1);
	ae->snrclock *= CLOCK_MUL;

	et = (UINT32)(ae->snrclock / ae->freq) * ae->et;
	V5_VispGetCurrentSize(&size);
	if(gVispContext.pinfo->snrSetETCall)
		gVispContext.pinfo->snrSetETCall(gVispContext.env, et, size, ae->snrclock);
}

/**********************************************************************************
Description:
Set Expose time or gain
Parameters:
Return:
GAIN_ET -- change et and gain		GAIN_ONLY -- just change gain
Remarks:
*********************************************************************************/
void V5_VispAutoExpose(void)
{
	UINT8 ret = 0, gstep = 0;
	UINT32 et = 0, tmp = 0;
	TSize size;
	PTAeAdjust ae = NULL;
#ifdef _DEBUG_ISP_AE
	char dbg[50];
#endif

	tmp = V5_CtrlGetMclk();
	ae = (&gVispContext.ae);
	ae->snrclock = tmp / (gVispContext.pinfo->snrSizeCfg.pSizeCfg[gVispContext.index].snrpixelratio + 1);
	ae->snrclock *= CLOCK_MUL;
	ae->ymean = V5B_IspGetYMean();
	ae->gtarget = gVispContext.pinfo->aeparm.gtarget;
	gstep = V5B_IspGetGlbGainStep();
	switch(gstep)
	{
	case 0:
		ae->gstep = 0x40;
		break;
	case 1:
		ae->gstep = 0x20;
		break;
	case 2:
		ae->gstep = 0x10;
		break;
	case 3:
		ae->gstep = 0x08;
		break;
	default:
		break;
	}

	ret = VComputeEt(ae);
	if(ret == FAILED)
		return;

	if(ret == GAIN_ONLY)
	{
		V5B_IspSetGlbGainDelay(0);
		if(ae->gtarget == 0)
			ae->gtarget = 1;
		V5B_IspSetGlbGain(ae->gtarget);
		ae->gain = ae->gtarget;
#ifdef _DEBUG_ISP_AE
		sprintf(dbg,"ymean=%x newet=%lx(%lx), newgain=%x \n",ae->ymean, (ae->snrclock/ae->freq)*ae->et, ae->et, ae->gtarget);
		if(fp!=NULL)
			fprintf(fp,dbg );
#endif
		return;
	}

	V5B_IspSetGlbGainDelay(gVispContext.pinfo->aeparm.gdelay);
	et = (UINT32)(ae->snrclock / ae->freq) * ae->newet;
	V5_VispGetCurrentSize(&size);
	if(gVispContext.pinfo->snrSetETCall)
		gVispContext.pinfo->snrSetETCall(gVispContext.env, et, size, ae->snrclock);
	if(ae->gtarget == 0)
		ae->gtarget = 1;
	V5B_IspSetGlbGain(ae->gtarget);

	ae->et = ae->newet;
	ae->gain = ae->gtarget;

#ifdef _DEBUG_ISP_AE
	sprintf(dbg,"ymean=%x newet=%lx(%lx), newgain=%x \n",ae->ymean, et, ae->newet, ae->gtarget);
	if(fp!=NULL)
		fprintf(fp,dbg );
#endif
}


/*********************************************************************
*                            Anti-Flicker                            *
*********************************************************************/

/**********************************************************************************
Description:
Set Environment
Parameters:
FREQ_50HZ =		0x0,
FREQ_60HZ =		0x1,
FREQ_NONE =		0x2,		//outdoor
FREQ_NOTSURE =	0x3			
Return:
Remarks:
*********************************************************************************/
void V5_VispSetEnv(UINT8 env)
{
	if(env == FREQ_60HZ)
	{
		gVispContext.env = FREQ_60HZ;
		gVispContext.ae.freq = EPFREQ_60HZ;
	}
	else
	{
		gVispContext.env = FREQ_50HZ;
		gVispContext.ae.freq = EPFREQ_50HZ;
	}
}

UINT8 V5_VispGetEnv(void)
{
	return gVispContext.env;
}

void V5_VispJudgeEnv(void)
{
	V5_VispDisableAe();
	V5_VispIspIntEnable(INT_ISP_ANTIF);
	V5_VispIspIntSetCallback(INT_ISP_ANTIF, _ISR_VispIspAntiFlicker);
	V5B_IspEnableAntiFlk();
	gVispContext.enfliker = 1;
}

static void V5_VispSwithFreq(void)
{
	if((gVispContext.ae.et / ET_ONE_STEP) > MAX_FLK_ET)
		gVispContext.ae.et = (MAX_FLK_ET - 1) * ET_ONE_STEP;

	if(gVispContext.env == FREQ_60HZ)
	{
		gVispContext.ae.et = ET_ONE_STEP * gTmflk_50[gVispContext.ae.et / ET_ONE_STEP - 1];
		gVispContext.env = FREQ_50HZ;
		gVispContext.ae.freq = EPFREQ_50HZ;
	}
	else
	{
		gVispContext.ae.et = ET_ONE_STEP * gTmflk_60[gVispContext.ae.et / ET_ONE_STEP - 1];
		gVispContext.env = FREQ_60HZ;
		gVispContext.ae.freq = EPFREQ_60HZ;
	}
}

/**********************************************************************************
Description:
Judge indoor or outdoor
Parameters:
Return:
FREQ_50HZ =		0x0,
FREQ_60HZ =		0x1,
FREQ_NONE =		0x2,		//outdoor
FREQ_NOTSURE =	0x3			//first judge,not sure
Remarks:
If return FREQ_NOTSURE,env should be judge at next interrupt
*********************************************************************************/
static UINT8 V5_VispDoJudgeEnv(void)
{
	static UINT8 flag = 0;
	static UINT32 thd[2];

	if(flag == 0)	//first frame
	{
		thd[0] = V5B_IspGetAfSta();
		V5_VispSwithFreq();
		V5_VispSetEt();
		flag = 1;
		return gVispContext.env;
	}
	else
	{
		thd[1] = V5B_IspGetAfSta();
		if((thd[0] < FLK_THRESHOLD) && (thd[1] < FLK_THRESHOLD))
		{
			thd[0] = 0;
			thd[1] = 0;
			flag = 0;
			return FREQ_NONE;
		}
		else if((thd[0] < FLK_THRESHOLD) && (thd[1] > FLK_THRESHOLD))
		{
			V5_VispSwithFreq();
			thd[0] = 0;
			thd[1] = 0;
			flag = 0;
			return gVispContext.env;
		}
		else if((thd[0] > FLK_THRESHOLD) && (thd[1] < FLK_THRESHOLD))
		{
			thd[0] = 0;
			thd[1] = 0;
			flag = 0;
			return gVispContext.env;
		}
		else
		{
			thd[0] = 0;
			thd[1] = 0;
			flag = 0;
			return FREQ_NOTSURE;
		}
	}
}

void V5_VispDoFlk(void)
{
	static UINT8 flag = 0;		// 0 -- set et,gain,	1 -- do flk
	static UINT8 flkstage = 0;	// 1 -- get first thd,	3 -- get second thd

	if(flag == 0)
	{
		if(gVispContext.env == FREQ_60HZ)
			gVispContext.ae.et = ET_ONE_STEP * gTmflk_60[gVispContext.ae.et / ET_ONE_STEP - 1];
		else if(gVispContext.env == FREQ_50HZ)
			gVispContext.ae.et = ET_ONE_STEP * gTmflk_50[gVispContext.ae.et / ET_ONE_STEP - 1];
		else
		{
			gVispContext.ae.et = ET_ONE_STEP * gTmflk_50[gVispContext.ae.et / ET_ONE_STEP - 1];
			gVispContext.env = FREQ_50HZ;
		}
		gVispContext.ae.freq = (gVispContext.env == FREQ_60HZ) ? EPFREQ_60HZ : EPFREQ_50HZ;
		V5_VispSetEt();		//change et setting
		flag = 1;
		flkstage = 0;
	}
	else
	{
		if(flkstage == 0 || flkstage == 2)		//do nothing
		{
			flkstage ++;
			return;
		}
		gVispContext.env = V5_VispDoJudgeEnv();
		flkstage ++;
		if(flkstage != 4)
			return;
		gVispContext.ae.freq = (gVispContext.env == FREQ_60HZ) ? EPFREQ_60HZ : EPFREQ_50HZ;
		flag = 0;
		flkstage = 0;
		gVispContext.enfliker = 0;
		V5B_IspDisableAntiFlk();
		V5_VispIspIntDisable(INT_ISP_ANTIF);
		V5_VispEnableAe();
	}
}


/*********************************************************************
*                            Flash Light                             *
*********************************************************************/

void V5_VispSetFlashYThd(UINT8 x)
{
	gVispContext.flashparm.yflashthd = x;
}

UINT8 V5_VispGetFlashYThd(void)
{
	return gVispContext.flashparm.yflashthd;
}

void V5_VispSetFlashEt(UINT8 x)
{
	gVispContext.flashparm.flashstep = x;
}

UINT8 V5_VispGetFlashEt(void)
{
	return gVispContext.flashparm.flashstep;
}

void V5_VispEnableFlash(void)
{
	gVispContext.enflash = 1;
}

void V5_VispDisableFlash(void)
{
	gVispContext.enflash = 0;
}

void V5_VispStartFlashCharge(void)
{
	V5B_CpmSetGpioDirection(0xff);
	V5B_CpmtFlashTriggerEnable();
	V5B_CpmtFlashChargeEnable();

	V5B_SifSetFlashPolarity(1);
	V5B_SifSetM10usTgt(0xef);
	V5B_SifSetTriggerStyle(0);
	V5B_SifEnableFlashPower();
}

void V5_VispStopFlashCharge(void)
{
	V5B_SifDisableFlashPower();
}

void V5_VispFlashLight(void)
{
	UINT8 ymean = 0;

	if(gVispContext.enflash)
	{
		ymean = V5B_IspGetYMean();
		if(ymean < gVispContext.flashparm.yflashthd)
		{
			gVispContext.ae.et = gVispContext.flashparm.flashstep * ET_ONE_STEP;
			V5B_SifEnableM10usCnt();
			V5_VispDisableAe();
		}
	}
}

void V5_VispStopFlashLight(void)
{
	V5B_SifDisableM10usCnt();
	V5B_SifStopCapture();
	V5_VispEnableAe();
}

#endif //V578CMD_DISABLE
