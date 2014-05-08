/**************************************************************
>
> The processing of adjust sensor is about SIF (sensor interface)
> ISP (image signal processing) and sensor part. For convience, the
> three parts is summaries to SIF.
>
>                COPYRIGHT (C) 2004, Vimicro Corporation
>                        ALL RIGHTS RESERVED
>
> This source code has been made available to you by VIMICRO on an
> AS-IS basis. Anyone receiving this source code is licensed under VIMICRO
> copyrights to use it in any way he or she deems fit, including copying it,
> modifying it, compiling it, and redistributing it either with or without
> modifications. Any person who transfers this source code or any derivative
> work must include the VIMICRO copyright notice and this paragraph in
> the transferred software.
>
***************************************************************/


#include "v558api.h"
#include "vregdef.h"
#include "vctrl.h"
#include <math.h>

#include "visp.h"

V558_SNR_CONTEXT	gSnrContext;

#define SPECIAL_FUJITSU_MBS04_BAYER 0
#define LIGHT_GUIDE_EPTIME 500

//==============================================================================//
//	static function of ISP init	                                   //
//==============================================================================//
static void InitAntiFlk(void);
static void InitIspParm(void);
static void InitSensor(void);
static void InitSifParm(void);
//==============================================================================//
//	static function of ISP auto image quality adjusting                    //
//==============================================================================//
static void _ISR_IspAutoFunInt(void);
static void AjustImageQulity(void);
static void AdjustUnlockAe(UINT8 ymean, UINT8 ygain);
static void DoFlk(PV558_ISP_AUTOCTRL pIsp);
static void SetNomalLightNoiseTable(void);
static void SetLowLightNoiseTable(void);
	
//==============================================================================//
//	static function of auto exposure adjusting                             //
//==============================================================================//
static void SetAeTime(UINT16 times);
static void SetGlbGain(UINT8 gain, UINT8 delay);
static void ExposureSuit(PV558_ISP_AUTOCTRL pIsp, UINT8 ymean);
static void I2cSensorWr(V558_SENSORREGVALUE regv);
static UINT8 AddExpTime(UINT16 step);
static UINT8 DecExpTime(UINT16 step);
static void GetGainRange(void);
static UINT8 AutoAddExpTime(UINT8 ymean, UINT8 *ygain);
static UINT8 AutoDecExpTime(UINT8 ymean, UINT8 *ygain);
//==============================================================================//
//	static function and structure of auto flicker adjusting                //
//==============================================================================//
static void FlkSetNextFreq(void);
static UINT8 Tmflk_50[MAX_FLK_ET] = {2,2,3,3,3,7,7,8,8,8,12,12,13,13,13,17,17,18,18,18,22,22,23,23,23};
static UINT8 Tmflk_60[MAX_FLK_ET] = {3,3,3,3,3,9,9,9,9,9,15,15,15,15,15,21,21,21,21,21,27,27,27,27,27};

/* static function of image adjusting math */
static void CalculateNewGammaCurve(UINT8 * OriginalGammaCurve, SINT8 DeltaContrast, SINT8 DeltaBrightness, UINT8 *NewGammaCurve);
static UINT32 PowTwo(UINT32 x, UINT16 y, UINT8 denominator, UINT8 numerator);



#ifdef __V558RDK_DEBUG
	UINT32 v558_thd[6];
#endif


static void SnrDelay(UINT16 time)
{
	time *= 6000;
	while(time--);
}


/********************************************************************************/
//								                                                //
//	global function of SensorInterface, ImageSignalProcess	                    //
//			and Sensor				                                            //
//								                                                //
/********************************************************************************/




/********************************************************************************
  Description:
	Initialize SIF module's context
  Note:
	This function must be called when system is power on. Indict the 
	SIF (sensor interface), ISP (image signal processing) module and 
	external sensor's context is not valid.
  Remarks:
*********************************************************************************/
void V558_SifInitContext(void)
{
	gSnrContext.valid = FALSE;
	gSnrContext.snrstate = V558_SNR_NOT_INIT;
}

/********************************************************************************
  Description:
	Application level send initialize data to SIF, ISP, sensor module
  Parameters:
	info: point to the start address of SIF's initialize data V558_SENSORINFO
  Note:
	This function must be called before use other function of this RDK,
	or the SIF will be abnormal.
  Returns:
	SUCCEED	:  if succesful
	SNRFAILED:  otherwise
  Remarks:
*********************************************************************************/
UINT8 V558_SifSetSensorInfo(V558_SENSORINFO *info)
{
#define SYSTEM_HAS_MALLOC		0

	UINT16 freq;

#if	SYSTEM_HAS_MALLOC
	//if the system support malloc(), this can be changed
#else
	static V558_SENSORREGVALUE		sifRegVal[0x100];
	static V558_SENSORREGVALUE		snrRegVal[0x100];
	static V558_SENSORREGVALUE		ispRegVal[0x100];
#endif
	V558_SENSORINFO *pSif = info;

	gSnrContext.pInfo = info;
	
	memcpy(sifRegVal, pSif->dsif.regval, (UINT16)(pSif->dsif.len)
		* sizeof(V558_SENSORREGVALUE));
	pSif->dsif.regval = sifRegVal;
	
	memcpy(snrRegVal, pSif->dsnr.regval, (UINT16)(pSif->dsnr.len)
		* sizeof(V558_SENSORREGVALUE));
	pSif->dsnr.regval = snrRegVal;
	
	memcpy(ispRegVal, pSif->disp.regval, (UINT16)(pSif->disp.len)
		* sizeof(V558_SENSORREGVALUE));
	pSif->disp.regval = ispRegVal;

	//calculate sensor pixel clock.


	gSnrContext.lock = 0;

	freq = gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype == V558_FREQ_50HZ ? EPFREQ_50HZ:EPFREQ_60HZ;

	if( gSnrContext.pInfo->disp.ctrl.maxexp == 0 )
		gSnrContext.pInfo->disp.ctrl.maxexp = 1000;
	
	if(gSnrContext.pInfo->disp.ctrl.enae)
	{
		gSnrContext.eptimes = ET_DEFAULT;
	}
	else
	{
		gSnrContext.pInfo->disp.ctrl.enflk = 0;
		gSnrContext.pInfo->disp.ctrl.enlight = 0;
		gSnrContext.eptimes = gSnrContext.pInfo->disp.ctrl.defexp;
		if(gSnrContext.eptimes < gSnrContext.pInfo->disp.ctrl.minexp)
		{
			gSnrContext.eptimes = gSnrContext.pInfo->disp.ctrl.minexp;
		}
		if(gSnrContext.eptimes > gSnrContext.pInfo->disp.ctrl.maxexp)
		{
			gSnrContext.eptimes = gSnrContext.pInfo->disp.ctrl.maxexp;
		}
	}

	
	gSnrContext.valid = TRUE;
	gSnrContext.snrstate = V558_SNR_POWOFF;
	
	
	return SUCCEED;
}


/********************************************************************************
  Description:
	Get sensor mode yuv or rgb
  Parameters:
	type: The pointer to the sensor type param 0-rgb,1-yuv
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_SifGetSnrInfoType(UINT8* type)
{
	if( gSnrContext.snrstate != V558_SNR_NOT_INIT )
	{
		*type = gSnrContext.pInfo->mode;
		return SUCCEED;
	}
	else
	{
		return SNRFAILED;
	}
}

/********************************************************************************
  Description:
	Get sensor mode pixrate
  Parameters:
	type: point to the sensor pixrate param
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_SifGetSnrInfoPixRate(UINT8* rate)
{
	if( gSnrContext.snrstate != V558_SNR_NOT_INIT )
	{
		*rate = gSnrContext.pInfo->pixRate;
		return SUCCEED;
	}
	else
	{
		return SNRFAILED;
	}
}

/********************************************************************************
  Description:
	Get Sif out image size
  Parameters:
	type: point to the sensor size param V558_SIZE
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_SifGetSnrInfoImgSize(V558_SIZE* size)
{
	if( gSnrContext.snrstate != V558_SNR_NOT_INIT )
	{
		size->cx = gSnrContext.pInfo->size.cx;
		size->cy = gSnrContext.pInfo->size.cy;
		return SUCCEED;
	}
	else
	{
		return SNRFAILED;
	}
	
}

/********************************************************************************
  Description:
		Open SIF, ISP, sensor module
  Note:
  Returns:
	SUCCEED	:  if succesful
	SNRFAILED:  otherwise
  Remarks:
*********************************************************************************/
UINT8 V558_SifOpen(void)
{
	if(gSnrContext.valid)
	{
		V558_LcdALDisable();

 		V558_CtrlSetWorkMode(V558_MODE_VIEW);

		InitSifParm();
		
		V558_SifDisableSyncGen();
		
		
		InitSensor();
		InitIspParm();
		
		V558_SifSetI2cDevAddr(gSnrContext.pInfo->dsnr.chipadr);	//set i2c address
		gSnrContext.snrstate = V558_SNR_POWON;
		
		V558_CtrlSwReset(V558_SW_RESET_IPP);


		return SUCCEED;
	}
	return SNRFAILED;
}


/********************************************************************************
  Description:
	Set sensor power state
  Parameters:
	state: power state param V558_SNR_STATE
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT16 V558_SifSetSnrState(UINT8 state)
{
	UINT8 uMode = 0;
	
	PV558_PWR_CFG pwrcfg = &gSnrContext.pInfo->pwrcfg;
	
	
	if(gSnrContext.snrstate == state)
	{
		return SNRFAILED;
	}
	switch(state)
	{

	case V558_SNR_POWON:

		uMode = V558_SifGetSensorMode();
		uMode &= 0xF9;
		uMode |= (((pwrcfg->SnrEn & 0x1)<<1)|((pwrcfg->PwrOn & 0x1)<<2));
		V558_SifSetSensorMode(uMode);
		V558_Delay(1);
		
		gSnrContext.snrstate = state;
		break;
	case V558_SNR_SLEEP:
	case V558_SNR_STANDBY:
	case V558_SNR_POWOFF:
		uMode = V558_SifGetSensorMode();
		uMode &= 0xF9;
		uMode |= ((((~pwrcfg->SnrEn) & 0x1)<<1)|(((~pwrcfg->PwrOn) & 0x1)<<2));
		V558_SifSetSensorMode(uMode);
		V558_Delay(1);
		gSnrContext.snrstate = state;
		break;

	default:
		break;
	}
	return SUCCEED;
}

/********************************************************************************
  Description:
	Get sensor power state
  Parameters:
	Null
  Returns:
	sensor power state V558_SNR_STATE
*********************************************************************************/
UINT8 V558_SifGetSnrState(void)
{
	return gSnrContext.snrstate;
}

/********************************************************************************
  Description:
	Set exposure time
  Parameters:
	time: exposure time :freq time/10000
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspSetExposureTime(UINT16 time)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.enae)
		{
			return SNRFAILED;
		}
		else
		{
			gSnrContext.eptimes = time;
			if(gSnrContext.eptimes < gSnrContext.pInfo->disp.ctrl.minexp)
			{
				gSnrContext.eptimes = gSnrContext.pInfo->disp.ctrl.minexp;
			}
			if(gSnrContext.eptimes > gSnrContext.pInfo->disp.ctrl.maxexp)
			{
				gSnrContext.eptimes = gSnrContext.pInfo->disp.ctrl.maxexp;
			}

			SetAeTime(gSnrContext.eptimes);
			return SUCCEED;
		}
	}
}

/********************************************************************************
  Description:
	Get exposure time
  Parameters:
	time: the pointer to exposure time :freq time/10000
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspGetExposureTime(UINT16* time)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.enae)
		{
			return SNRFAILED;
		}
		else
		{
			*time = gSnrContext.eptimes;
			return SUCCEED;
		}
	}
}
/********************************************************************************
  Description:
	Set gain
  Parameters:
	gain: gain value
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspSetGain(UINT8 gain)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.enae)
		{
			return SNRFAILED;
		}
		else
		{
			SetGlbGain(gain,0);
			return SUCCEED;
		}
	}
}

/********************************************************************************
  Description:
	Get gain
  Parameters:
	gain: the pointer to gain
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspGetGain(UINT8* gain)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.enae)
		{
			return SNRFAILED;
		}
		else
		{
			*gain= V558_IspGetGlbgain();
			return SUCCEED;
		}
	}
}

/********************************************************************************
  Description:
	Get lightguide
  Parameters:
	lightguide: the pointer to lightguide 0--light strong,1--light weak
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspGetLightGuide(UINT8* lightguide)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT)
	{
		return SNRFAILED;
	}
	else
	{
		if(gSnrContext.eptimes > LIGHT_GUIDE_EPTIME)
		{
			*lightguide = 1;
		}
		else
		{
			*lightguide = 0;
		}
		return SUCCEED;
	}
}


/********************************************************************************
  Description:
	Set Ae flag
  Parameters:
	uAeFlag: 0--disable ae, 1--enable ae
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspSetAeFlag(UINT8 uAeFlag)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		gSnrContext.pInfo->disp.ctrl.enae = uAeFlag;
		return SUCCEED;
	}
}

/********************************************************************************
  Description:
	Get Ae flag
  Parameters:
	uAeFlag: the pointer to ae flag,0--disable ae, 1--enable ae
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspGetAeFlag(UINT8* uAeFlag)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		*uAeFlag = gSnrContext.pInfo->disp.ctrl.enae;
		return SUCCEED;
	}
}

/********************************************************************************
  Description:
	Set aflk flag
  Parameters:
	uAFlkFlag: 0--disable aflk , 1--enable aflk
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspSetAFlkFlag(UINT8 uAFlkFlag)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.enae && gSnrContext.pInfo->disp.ctrl.enflk && gSnrContext.flkflag != FLK_END)
		{
			return SNRFAILED;
		}
		else
		{
			gSnrContext.pInfo->disp.ctrl.enflk = uAFlkFlag;
			if(uAFlkFlag)
			{
				gSnrContext.lock = 1;
				gSnrContext.flkflag = FLK_BEGIN;
			}
			return SUCCEED;
		}
	}
}

/********************************************************************************
  Description:
	Get aflk(auto detect light freq) flag
  Parameters:
	uAFlkFlag: the pointer to aflk flag,0--disable aflk , 1--enable aflk
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspGetAFlkFlag(UINT8* uAFlkFlag)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		*uAFlkFlag = gSnrContext.pInfo->disp.ctrl.enflk;
		return SUCCEED;
	}
}

/********************************************************************************
  Description:
	Set alight(auto detect indoor outdoor light) flag
  Parameters:
	uALightFlag: 0--disable alight, 1--enable alight
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspSetALightFlag(UINT8 uALightFlag)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.enae && gSnrContext.pInfo->disp.ctrl.enflk
			&&gSnrContext.pInfo->disp.ctrl.enlight && gSnrContext.flkflag != FLK_END)
		{
			return SNRFAILED;
		}
		else
		{
			gSnrContext.pInfo->disp.ctrl.enlight = uALightFlag;
			if(uALightFlag)
			{
				gSnrContext.lock = 1;
				gSnrContext.flkflag = FLK_BEGIN;
			}
			return SUCCEED;
		}

		return SUCCEED;
	}
}

/********************************************************************************
  Description:
	Get alight(auto detect indoor outdoor light) flag
  Parameters:
	uALightFlag: the pointer to alightflag 0--disable alight, 1--enable alight
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspGetALightFlag(UINT8* uALightFlag)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		*uALightFlag = gSnrContext.pInfo->disp.ctrl.enlight;
		return SUCCEED;
	}
}

/********************************************************************************
  Description:
	Set light freq
  Parameters:
	freqType: 0--50hz, 1--60hz
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspSetFreq(UINT8 freqType)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype == freqType)
		{
			return SUCCEED;
		}


		if(gSnrContext.pInfo->disp.ctrl.enae && gSnrContext.pInfo->disp.ctrl.enflk && gSnrContext.flkflag != FLK_END)
		{
			return SNRFAILED;
		}
		gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype = freqType;
		SetAeTime(gSnrContext.eptimes);
		return SUCCEED;
	}
	
}

/********************************************************************************
  Description:
	Get light freq
  Parameters:
	freqType: the pointer to light freq: 0--50hz, 1--60hz
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspGetFreq(UINT8* freqType)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.enae && gSnrContext.pInfo->disp.ctrl.enflk && gSnrContext.flkflag != FLK_END)
		{
			return SNRFAILED;
		}

		*freqType = gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype;
		return SUCCEED;
	}
}

/********************************************************************************
  Description:
	Set light type
  Parameters:
	lightType: 0--indoor, 1--outdoor
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspSetLight(UINT8 lightType)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		UINT8 ae = gSnrContext.pInfo->disp.ctrl.enae;
		if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == lightType)
		{
			return SUCCEED;
		}

		if(gSnrContext.pInfo->disp.ctrl.enae && gSnrContext.pInfo->disp.ctrl.enflk
			&&gSnrContext.pInfo->disp.ctrl.enlight && gSnrContext.flkflag != FLK_END)
		{
			return SNRFAILED;
		}
		gSnrContext.pInfo->disp.ctrl.enae = 0;

		gSnrContext.pInfo->disp.ctrl.aflk.lighttype = lightType;
		if(lightType)
		{
			if(gSnrContext.eptimes == 100)
			{
				GetGainRange();
			}
		}
		else
		{
			if(gSnrContext.eptimes < 100)
			{
				gSnrContext.eptimes = 100;
				SetAeTime(gSnrContext.eptimes);
			}
		}
		gSnrContext.lock = 0;
		gSnrContext.pInfo->disp.ctrl.enae = ae;
		return SUCCEED;
	}

}
/********************************************************************************
  Description:
	Get light type
  Parameters:
	lightType: the pointer to light freq: 0--indoor, 1--outdoor
  Returns:
	SUCCEED	: if succesful
	SNRFAILED: failed
*********************************************************************************/
UINT8 V558_IspGetLight(UINT8* lightType)
{
	if( gSnrContext.snrstate == V558_SNR_NOT_INIT )
	{
		return SNRFAILED;
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.enae && gSnrContext.pInfo->disp.ctrl.enflk
			&&gSnrContext.pInfo->disp.ctrl.enlight && gSnrContext.flkflag != FLK_END)
		{
			return SNRFAILED;
		}

		*lightType = gSnrContext.pInfo->disp.ctrl.aflk.lighttype ;
		return SUCCEED;
	}
}

/********************************************************************************/
//								                //
//	static function of Sensor Image Signal Process functions                //
//			               				                //
//								                //
/********************************************************************************/


//==============================================================================//
//	static function of ISP init	                                        //
//==============================================================================//
void InitAntiFlk(void)
{
	gSnrContext.flkflag = FLK_BEGIN;
	V558_IspSetAflcnt(gSnrContext.pInfo->disp.ctrl.aflk.flkcnt);
	if(gSnrContext.pInfo->disp.ctrl.aflk.flklinemode)
	{
		V558_IspEnableAflmode();
	}
	else
	{
		V558_IspDisableAflmode();
	}
	V558_IspSetAflstep(gSnrContext.pInfo->disp.ctrl.aflk.flkstep);
	V558_IspSetAflst(gSnrContext.pInfo->disp.ctrl.aflk.flklst);
}

void InitIspParm(void)
{
	UINT8	i;
	V558_SENSORINFO *pSif = gSnrContext.pInfo;
	const V558_SENSORREGVALUE	*pregv;
	
	if(pSif->mode & V558_SNR_YUVFORMAT)	//isp is in the sensor
	{
		if(pSif->comtype == V558_SNRC_SERIAL)	//serial communication mode
		{
			//
		}
		else		//i2c communication mode
		{
			V558_SifSetI2cDevAddr(pSif->disp.chipadr);	//set i2c address
			for(i = 0; i < pSif->disp.len; i++)
			{
				I2cSensorWr(pSif->disp.regval[i]);
			}
			V558_LcdDisableGamma();
		}
	}
	else
	{
		pregv = pSif->disp.regval;
		for(i = 0; i < pSif->disp.len; i++)
		{
			
			V558_SetReg(pregv[i].adr, (UINT8)pregv[i].val);
			SnrDelay(pregv[i].wait);
		}
		V558_IspSetImageSize(pSif->size);
		V558_IspSetPixelRate(pSif->pixRate);
		SetGlbGain(pSif->disp.ctrl.y.gain,0);

		V558_IspSetBrightness(pSif->disp.prp.brightness);
		
#if 0
		V558_IspSetRgain(pSif->disp.ctrl.r.gain);
		V558_IspSetGgain(pSif->disp.ctrl.g.gain);
		V558_IspSetBgain(pSif->disp.ctrl.b.gain);
#endif //add for future;
		if(gSnrContext.eptimes >= gSnrContext.pInfo->disp.ctrl.dpcThd)
		{
			SetLowLightNoiseTable();
		}
		else
		{
			SetNomalLightNoiseTable();
		}

		
		GetGainRange();
		
		V558_IspSetAfIntfrq(pSif->disp.ctrl.intfrq);
		if(gSnrContext.pInfo->disp.ctrl.enae)
		{
			V558_CtrlIntAdd(V558_INT_AUTO_FUNC, _ISR_IspAutoFunInt);

			V558_CtrlIntEnable(V558_INT_AUTO_FUNC, 1);
			
			if(pSif->disp.ctrl.enflk||gSnrContext.pInfo->disp.ctrl.enlight)
				InitAntiFlk();
		}
	}
}

void InitSensor(void)
{
	
	UINT8	i;
	V558_SENSORINFO *pSif = gSnrContext.pInfo;
	
	V558_SifSetI2cDevAddr(pSif->dsnr.chipadr);	//set i2c address
	for(i = 0; i < pSif->dsnr.len; i++)
	{
		I2cSensorWr(pSif->dsnr.regval[i]);
	}
	SetAeTime(gSnrContext.eptimes);
}

void InitSifParm(void)
{
	UINT8 i,uMode;
	V558_SENSORINFO *pSif = gSnrContext.pInfo;
	const V558_SENSORREGVALUE *pregv = pSif->dsif.regval;
	
	uMode = (((pSif->pwrcfg.PwrOn & 0x1) << 2) | 
		((pSif->mode & 0x1) <<6 ) |
		((pSif->pwrcfg.PltEn & 0x1) << 3) | 
		((pSif->pwrcfg.PltMd & 0x1) << 4)|
		(0x1 << 5));
	V558_SifSetSensorMode(uMode);
	SnrDelay(1);
	uMode |= (pSif->pwrcfg.SnrRe&0x1);
	V558_SifSetSensorMode(uMode);
	SnrDelay(1);
	
	V558_SifSetSnrclk(pSif->clkcnt);
	V558_SifSetPixelRate(pSif->pixRate);

	for(i = 0; i < pSif->dsif.len; i++)
	{
		V558_SetReg(pregv[i].adr, (UINT8)pregv[i].val);
		SnrDelay(pregv[i].wait);
	}
	V558_SifSetDataFormat(pSif->mode);
	
	uMode |= ((pSif->pwrcfg.SnrEn & 0x1) << 1);
	V558_SifSetSensorMode(uMode);
	SnrDelay(1);
}


//==============================================================================//
//	static function of ISP auto image quality adjusting                     //
//==============================================================================//
void _ISR_IspAutoFunInt(void)
{
	static valid = 0;
	if( gSnrContext.snrstate != V558_SNR_NOT_INIT )
	{
		if(gSnrContext.pInfo->disp.ctrl.enae)
		{
			if(valid < (gSnrContext.ygaindelay+1))
			{
				valid++;
				return;
			}
			valid = 0;
			gSnrContext.ygaindelay = 0;
			AjustImageQulity();
		}
	}
}


void AjustImageQulity()
{
	
	PV558_ISP_AUTOCTRL pIsp = & gSnrContext.pInfo->disp.ctrl;
	
	if((pIsp->enflk||gSnrContext.pInfo->disp.ctrl.enlight) && (gSnrContext.flkflag != FLK_END))
	{
		DoFlk(pIsp);
	}
	else //pIsp->enflk == 0
	{
		UINT8 ymean, ygain;
		
		ymean = V558_IspGetYMean();
		ygain = V558_IspGetGlbgain();
		
		ExposureSuit(pIsp, ymean);
		if(!gSnrContext.lock)
		{
			AdjustUnlockAe(ymean,ygain);
		}


	}
}


#if SPECIAL_FUJITSU_MBS04_BAYER == 0

void AdjustUnlockAe(UINT8 ymean, UINT8 ygain)
{
	UINT8	temp;
	UINT8	ytarget = gSnrContext.pInfo->disp.ctrl.y.target;
	UINT8	ytop = MAX_YMEAN - gSnrContext.pInfo->disp.ctrl.y.yfieldstep;
	UINT8	ybottom = gSnrContext.pInfo->disp.ctrl.y.yfieldstep;
	UINT8	ymaxtop = MAX_YMEAN - gSnrContext.pInfo->disp.ctrl.y.yfieldmulstep;
	UINT8	yminbottom = gSnrContext.pInfo->disp.ctrl.y.yfieldmulstep;
	UINT8	gaintop, gainbottom;
	UINT8	gainmin, gainmax;
	
	UINT32 uEtTemp;
	UINT8 uYgainTemp;

	uEtTemp = gSnrContext.eptimes;
	uYgainTemp = ygain;
	
	gaintop = gSnrContext.ygaintop;
	gainbottom = gSnrContext.ygainbottom;
	gainmin = gainbottom + gSnrContext.ygainmin;
	gainmax = gainbottom + gSnrContext.ygainmax;

#if 0
	if( (ymean > ybottom && ymean < ytop) )
#else
	if( (ymean > ybottom && ymean < ytop) || ((ymean > yminbottom && ymean < ymaxtop) &&
			(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)) )
#endif
	{
		if(ymean > ytarget)
		{
			if( ygain > gainmin )
			{
				gSnrContext.ygainmax = ygain - gainbottom;
				temp = ygain - gainmin;
				if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)
				{
					if(ymean > ytop)
					{
						ygain -= (temp + 7) >> 3;
					}
					else
					{
						ygain -= (temp + 3) >> 2;
					}
				}
				else
				{
					ygain -= (temp + 3) >> 2;
				}
			}
			else 
			{
				if( gainmin == gainbottom )
				{
					if( AutoDecExpTime(ymean, &ygain) )
						return;
				}
				else
				{
					//gainmin = gainbottom;
					gSnrContext.ygainmax = ygain - gainbottom;
					gSnrContext.ygainmin = 0;
					temp = ygain - gainbottom;
					if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)
					{
						if(ymean > ytop)
						{
							ygain -= (temp + 7) >> 3;
						}
						else
						{
							ygain -= (temp + 3) >> 2;
						}
					}
					else
					{
						ygain -= (temp + 3) >> 2;

					}
				}
			}
		}
		else // ymean < ytarget
		{
			if( ygain < gainmax )
			{
				gSnrContext.ygainmin = ygain - gainbottom;
				temp = gainmax - ygain;
				if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)
				{
					if(ymean < ybottom)
					{
						ygain += (temp + 3) >> 2;
					}
					else
					{
						ygain += (temp + 7) >> 3;
					}
				}
				else
				{
					ygain += (temp + 3) >> 2;
				}
			}
			else 
			{
				if( gainmax == gaintop )
				{
					if ( AutoAddExpTime(ymean, &ygain) )
						return;
				}
				else
				{
					gSnrContext.ygainmin = ygain - gainbottom;
					gSnrContext.ygainmax = gaintop - gainbottom;
					temp = gaintop - ygain;
					if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)
					{
						if(ymean < ybottom)
						{
							ygain += (temp + 3) >> 2;
						}
						else
						{
							ygain += (temp + 7) >> 3;
						}
					}
					else
					{
						ygain += (temp + 3) >> 2;
					}
				}
			}
		}
	}
	else
	{
		if(gSnrContext.eptimes >= gSnrContext.pInfo->disp.ctrl.maxexp)
		{
			if(ymean <= ybottom)
			{
				ygain = gaintop;

				gSnrContext.ygainmin = 0;
				gSnrContext.ygainmax = gaintop - gainbottom;
			}
			else
			{
				if( AutoDecExpTime(ymean, &ygain) )
				{
					if(ygain == gainbottom)

					{
						return;
					}
					else
					{
						ygain = gainbottom;
					}
				}
			}
		}
		else
		{
			if(ymean <= ybottom)
			{
				if( AutoAddExpTime(ymean, &ygain) )
					return;
			}
			else
			{
				if((gSnrContext.eptimes == ET_ONE_STEP) && (ygain != gainbottom))
				{
					ygain = gainbottom;
				}
				else
				{
					if( AutoDecExpTime(ymean, &ygain) )
					{
						if(ygain == gainbottom)
						{
							return;
						}
						else
						{
							ygain = gainbottom;
						}
					}
				}
			}
		}
	}
	
	if((uEtTemp != gSnrContext.eptimes))
	{
		if(uYgainTemp != ygain)
		{

			SetGlbGain(ygain, gSnrContext.pInfo->disp.ctrl.y.gaindelay);
		}
		gSnrContext.ygaindelay = gSnrContext.pInfo->disp.ctrl.y.gaindelay; //sure set ae time delay sum frame
		SetAeTime(gSnrContext.eptimes);
	}
	else
	{
		if(uYgainTemp != ygain)
		{
			SetGlbGain(ygain, 0);
		}
	}
}

#else

#define SNR_GAIN_DEF 7

#define SNR_GAIN_MAX 10


void AdjustUnlockAe(UINT8 ymean, UINT8 ygain)
{
	UINT8	temp;
	UINT8	ytarget = gSnrContext.pInfo->disp.ctrl.y.target;
	UINT8	ytop = MAX_YMEAN - gSnrContext.pInfo->disp.ctrl.y.yfieldstep;
	UINT8	ybottom = gSnrContext.pInfo->disp.ctrl.y.yfieldstep;
	UINT8	ymaxtop = MAX_YMEAN - gSnrContext.pInfo->disp.ctrl.y.yfieldmulstep;
	UINT8	yminbottom = gSnrContext.pInfo->disp.ctrl.y.yfieldmulstep;
	UINT8	gaintop, gainbottom;
	UINT8	gainmin, gainmax;
	
	UINT32 uEtTemp;
	UINT8 uYgainTemp;

	static SnrGain = SNR_GAIN_DEF;

	uEtTemp = gSnrContext.eptimes;
	uYgainTemp = ygain;
	
	gaintop = gSnrContext.ygaintop;
	gainbottom = gSnrContext.ygainbottom;
	gainmin = gainbottom + gSnrContext.ygainmin;
	gainmax = gainbottom + gSnrContext.ygainmax;

	if(ymean > ytarget) // for fujitsu
	{
		if(SnrGain > SNR_GAIN_DEF)
		{
#if 0
			if(ymean >= ytop)
			{
				SnrGain = SNR_GAIN_DEF;
			}
			else
			{
				SnrGain--;
			}
#endif
			SnrGain--;

			V558_SifSetSnrGain(SnrGain);
			return;
		}
	}
	else
	{
		if(gSnrContext.eptimes >= gSnrContext.pInfo->disp.ctrl.maxexp && ygain == gaintop && SnrGain < SNR_GAIN_MAX)
		{
#if 0
			if(ymean <= ybottom)
			{
				SnrGain = SNR_GAIN_MAX;
			}
			else
			{
				SnrGain++;
			}
#endif
			SnrGain++;

			V558_SifSetSnrGain(SnrGain);
			return;
		}
	}

#if 0
	if( (ymean > ybottom && ymean < ytop) )
#else
	if( (ymean > ybottom && ymean < ytop) || ((ymean > yminbottom && ymean < ymaxtop) &&
			(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)) )
#endif
	{
		if(ymean > ytarget)
		{
			if( ygain > gainmin )
			{
				gSnrContext.ygainmax = ygain - gainbottom;
				temp = ygain - gainmin;
				if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)
				{
					if(ymean > ytop)
					{
						ygain -= (temp + 7) >> 3;
					}
					else
					{
						ygain -= (temp + 3) >> 2;
					}
				}
				else
				{
					ygain -= (temp + 3) >> 2;
				}
			}
			else 
			{
				if( gainmin == gainbottom )
				{
					if( AutoDecExpTime(ymean, &ygain) )
						return;
				}
				else
				{
					//gainmin = gainbottom;
					gSnrContext.ygainmax = ygain - gainbottom;
					gSnrContext.ygainmin = 0;
					temp = ygain - gainbottom;
					if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)
					{
						if(ymean > ytop)
						{
							ygain -= (temp + 7) >> 3;
						}
						else
						{
							ygain -= (temp + 3) >> 2;
						}
					}
					else
					{
						ygain -= (temp + 3) >> 2;

					}
				}
			}
		}
		else // ymean < ytarget
		{
			if( ygain < gainmax )
			{
				gSnrContext.ygainmin = ygain - gainbottom;
				temp = gainmax - ygain;
				if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)
				{
					if(ymean < ybottom)
					{
						ygain += (temp + 3) >> 2;
					}
					else
					{
						ygain += (temp + 7) >> 3;
					}
				}
				else
				{
					ygain += (temp + 3) >> 2;
				}
			}
			else 
			{
				if( gainmax == gaintop )
				{
					if ( AutoAddExpTime(ymean, &ygain) )
						return;
				}
				else
				{
					gSnrContext.ygainmin = ygain - gainbottom;
					gSnrContext.ygainmax = gaintop - gainbottom;
					temp = gaintop - ygain;
					if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)
					{
						if(ymean < ybottom)
						{
							ygain += (temp + 3) >> 2;
						}
						else
						{
							ygain += (temp + 7) >> 3;
						}
					}
					else
					{
						ygain += (temp + 3) >> 2;
					}
				}
			}
		}
	}
	else
	{
		if(gSnrContext.eptimes >= gSnrContext.pInfo->disp.ctrl.maxexp)
		{
			if(ymean <= ybottom)
			{
				ygain = gaintop;

				gSnrContext.ygainmin = 0;
				gSnrContext.ygainmax = gaintop - gainbottom;
			}
			else
			{
				if( AutoDecExpTime(ymean, &ygain) )
				{
					if(ygain == gainbottom)

					{
						return;
					}
					else
					{
						ygain = gainbottom;
					}
				}
			}
		}
		else
		{
			if(ymean <= ybottom)
			{
				if( AutoAddExpTime(ymean, &ygain) )
					return;
			}
			else
			{
				if((gSnrContext.eptimes == ET_ONE_STEP) && (ygain != gainbottom))
				{
					ygain = gainbottom;
				}
				else
				{
					if( AutoDecExpTime(ymean, &ygain) )
					{
						if(ygain == gainbottom)
						{
							return;
						}
						else
						{
							ygain = gainbottom;
						}
					}
				}
			}
		}
	}
	
	if((uEtTemp != gSnrContext.eptimes))
	{
		if(uYgainTemp != ygain)
		{

			SetGlbGain(ygain, gSnrContext.pInfo->disp.ctrl.y.gaindelay);
		}
		gSnrContext.ygaindelay = gSnrContext.pInfo->disp.ctrl.y.gaindelay; //sure set ae time delay sum frame
		SetAeTime(gSnrContext.eptimes);
	}
	else
	{
		if(uYgainTemp != ygain)
		{
			SetGlbGain(ygain, 0);
		}
	}
}


#endif

//only active when indoor, so dont care outdoor condition
//and only when AE is lock
void DoFlk(PV558_ISP_AUTOCTRL pIsp)
{
	static valid = 0;
	static outvalid = 0;
	static UINT8 count = 0;
	static UINT8 outcount = 0;
	static UINT32 thd[6];
	static UINT8 templighttype = LIGHT_OUT;
	static UINT8 firstjust = 0;
	UINT8 i;
	
	if(gSnrContext.flkflag == FLK_BEGIN)
	{
		UINT8 ymean, ygain;
		if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype != LIGHT_OUT) // out door
		{
			gSnrContext.pInfo->disp.ctrl.aflk.lighttype = LIGHT_OUT;
		
			ymean = V558_IspGetYMean();
			ygain = V558_IspGetGlbgain();

			if(gSnrContext.eptimes == ET_ONE_STEP)
			{
				AutoAddExpTime(ymean, &ygain);
			}
			gSnrContext.lock = 0;
			templighttype = LIGHT_IN;
			firstjust = 1;
		}
		else
		{
			
			ymean = V558_IspGetYMean();
			ygain = V558_IspGetGlbgain();
			if(!firstjust)
			{
				templighttype = LIGHT_OUT;
				firstjust = 1;
			}
			ExposureSuit(pIsp, ymean);
			
			if(!gSnrContext.lock)
			{
				if((pIsp->enflk||gSnrContext.pInfo->disp.ctrl.enlight) && (gSnrContext.flkflag != FLK_END))
				{
					if((gSnrContext.eptimes >= gSnrContext.pInfo->disp.ctrl.maxexp)||
						(gSnrContext.eptimes <= gSnrContext.pInfo->disp.ctrl.minexp))
					{
						gSnrContext.lock = 1;
					}

					if(gSnrContext.eptimes/ET_ONE_STEP > MAX_FLK_ET)
					{
						gSnrContext.eptimes = (MAX_FLK_ET-1)*ET_ONE_STEP;
						gSnrContext.lock = 1;
					}
				}
			}

			if(!gSnrContext.lock)
			{
				AdjustUnlockAe(ymean, ygain);
			}
			else
			{
				firstjust = 0;
				count = 0;
				valid = 0;
				outcount = 0;
				outvalid = 0;

				for(i=0; i<6; i++)
				{
					thd[i] = 0;
				}

				if(gSnrContext.eptimes < ET_ONE_STEP)
				{
					if(gSnrContext.pInfo->disp.ctrl.enlight)
					{
						SetAeTime(gSnrContext.pInfo->disp.ctrl.aflk.timeinout);
						gSnrContext.flkflag = FLK_JUST_OUT;
					}
					else
					{
						gSnrContext.flkflag = FLK_END;
						
						if(templighttype == LIGHT_IN)
						{
							gSnrContext.eptimes = ET_ONE_STEP;
							gSnrContext.pInfo->disp.ctrl.aflk.lighttype = LIGHT_IN;

							SetAeTime(gSnrContext.eptimes);
						}
					}
				}
				else
				{
					FlkSetNextFreq();
					gSnrContext.pInfo->disp.ctrl.aflk.lighttype = LIGHT_IN;
					gSnrContext.flkflag = FLK_JUST_INDOOR;
				}

			}
			
		}
	}
	else
	{
		if(gSnrContext.flkflag == FLK_JUST_OUT)
		{
			if(outvalid < 2)
			{
				outvalid ++;
				return;
			}
			thd[outcount] = V558_IspGetAfThreshold();
#ifdef __V558RDK_DEBUG
			v558_thd[outcount] = thd[outcount];
#endif	//liuyong 2004.4.15,for debug
			outcount++;
			if(outcount == 2)
			{
				UINT32 thdtemp1;
				if(thd[0]>thd[1])
				{
					thdtemp1 = thd[1];
				}
				else
				{
					thdtemp1 = thd[0];
				}
				
				gSnrContext.flkflag = FLK_END;

				if(thdtemp1 <= FLK_THD_LT_STEP)
				{
					gSnrContext.pInfo->disp.ctrl.aflk.lighttype = LIGHT_OUT;
				}
				else
				{
					gSnrContext.eptimes = ET_ONE_STEP;
					gSnrContext.pInfo->disp.ctrl.aflk.lighttype = LIGHT_IN;

					SetAeTime(gSnrContext.eptimes);
				}
			}
		}
		else
		{
			if(gSnrContext.flkflag == FLK_JUST_INDOOR)
			{
				if(valid<2)
				{
					valid++;
					return;
				}
				thd[count] = V558_IspGetAfThreshold();
#ifdef __V558RDK_DEBUG
				v558_thd[count] = thd[count];
#endif	//liuyong 2004.4.15,for debug
				count++;
				if(count == 2)
				{
					valid = 0;
					FlkSetNextFreq();
				}
				if(count == 4)
				{
					UINT32 thdtemp1,thdtemp2;
					UINT8 bBig = 0;

					count =0;
					valid = 0;
					
					thdtemp1 = (thd[0]+thd[1])/2;
					thdtemp2 = (thd[2]+thd[3])/2;

					if(thdtemp1 > thdtemp2)
					{
						bBig = 1;
					}

					if((pIsp->enflk)&&(!bBig))
					{
						FlkSetNextFreq();
					}



					if( gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype == V558_FREQ_50HZ)
					{
						gSnrContext.eptimes = ET_ONE_STEP*Tmflk_50[gSnrContext.eptimes/ET_ONE_STEP - 1];
					}
					else
					{
						gSnrContext.eptimes = ET_ONE_STEP*Tmflk_60[gSnrContext.eptimes/ET_ONE_STEP - 1];
					}

					if(gSnrContext.pInfo->disp.ctrl.enlight)
					{
						if(bBig)
						{
							if(thdtemp1 < (thdtemp2*((UINT32)gSnrContext.pInfo->disp.ctrl.aflk.outthd)))
							{
								gSnrContext.pInfo->disp.ctrl.aflk.lighttype = LIGHT_OUT;
							}
						}
						else
						{
							if(thdtemp2 < (thdtemp1*((UINT32)gSnrContext.pInfo->disp.ctrl.aflk.outthd)))
							{
								gSnrContext.pInfo->disp.ctrl.aflk.lighttype = LIGHT_OUT;
							}
						}
					}
					else
					{
						if(templighttype == LIGHT_OUT)
						{

							gSnrContext.pInfo->disp.ctrl.aflk.lighttype = LIGHT_OUT;
						}
					}
					gSnrContext.flkflag = FLK_END;
				}
			}
			else
			{
				gSnrContext.flkflag = FLK_END;
			}

		}
		
	}
}



//==============================================================================//
//	static function of auto exposure adjusting                              //
//==============================================================================//

void SetAeTime(UINT16 times)
{
	UINT32 t, clk;
	UINT16 freq;

	
	static UINT8 lastfreqflag = 7; //only a define.


	if(lastfreqflag != gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype)
	{
		V558_SifSetFlkParam(gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype);
		lastfreqflag = gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype;
	}

	freq = lastfreqflag  == V558_FREQ_50HZ ? EPFREQ_50HZ:EPFREQ_60HZ;

	clk = gSnrContext.pInfo->snrclk;
	clk *= 1000;

	if(times < ET_ONE_STEP)
	{
		t = (((UINT32)(clk/freq)) * ET_ONE_STEP);
		t = PowTwo(t, ET_ONE_STEP-times, gSnrContext.pInfo->disp.ctrl.y.etdemout, gSnrContext.pInfo->disp.ctrl.y.etnumout);
	}
	else
	{
		t = ((UINT32)(clk/freq)) * times;
	}

	V558_SifSetSnrTime(lastfreqflag,t);
}
void SetGlbGain(UINT8 gain, UINT8 delay)
{
	gSnrContext.ygaindelay = delay;
	V558_IspSetGlbgainDelay(delay);
	V558_IspSetGlbgain(gain);
}


//adjusting whether current exposure time suit in a lock range.
void ExposureSuit(PV558_ISP_AUTOCTRL pIsp, UINT8 ymean)
{
	if(gSnrContext.lock)
	{
		UINT8 ulock;
		if(gSnrContext.eptimes < ET_ONE_STEP)
		{		
			ulock = pIsp->y.unlock + pIsp->y.inclockout;
		}
		else
		{
			ulock = pIsp->y.unlock;
		}

		if((ymean <= (pIsp->y.target - ulock)) || (ymean >= (pIsp->y.target + ulock)))
		{
			gSnrContext.lock = 0;
		}
	}
	else
	{
		UINT8 lock;
		if(gSnrContext.eptimes < ET_ONE_STEP)
		{		
			lock = pIsp->y.lock + pIsp->y.lock;
		}
		else
		{
			lock = pIsp->y.lock;
		}

		if((ymean <= (pIsp->y.target + lock)) && (ymean >= (pIsp->y.target - lock)))
		{
			gSnrContext.lock = 1;
		}
	}
}

void I2cSensorWr(V558_SENSORREGVALUE regv)
{
	if(gSnrContext.pInfo->dsnr.IicWordOrByte)
	{
		V558_SifI2cWriteWord((UINT8)regv.adr, regv.val);
	}
	else
	{
		V558_SifI2cWriteByte((UINT8)regv.adr, (UINT8)regv.val);
	}
	
	SnrDelay(regv.wait);
}

void GetGainRange(void)
{
	UINT8 gainrange;

	UINT16 uTempEt = gSnrContext.eptimes/ET_ONE_STEP;

	if(gSnrContext.eptimes < ET_ONE_STEP)
	{
		gainrange = gSnrContext.pInfo->disp.ctrl.y.gainrange[uTempEt]; //uTempEt == 0

		gSnrContext.ygaintop = gSnrContext.pInfo->disp.ctrl.y.gain - gainrange;
		gSnrContext.ygainbottom = gSnrContext.pInfo->disp.ctrl.y.gain - gainrange;
		gSnrContext.ygainmax = 0;
		gSnrContext.ygainmin = 0;
	}
	else
	{
		if(gSnrContext.eptimes < gSnrContext.pInfo->disp.ctrl.maxexp)
		{
			if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_IN && gSnrContext.eptimes == ET_ONE_STEP)
			{
				gSnrContext.ygaintop = gSnrContext.pInfo->disp.ctrl.y.gain + gSnrContext.pInfo->disp.ctrl.y.gainrange[0];
				gSnrContext.ygainbottom = gSnrContext.pInfo->disp.ctrl.y.gainminin;
				gSnrContext.ygainmax = gSnrContext.ygaintop - gSnrContext.ygainbottom;
				gSnrContext.ygainmin = 0;
			}
			else
			{
				if(uTempEt <= GAINRAGNE_TIME)
				{
					gainrange = gSnrContext.pInfo->disp.ctrl.y.gainrange[uTempEt-1];
				}
				else
				{
					gainrange = 0;
				}
				gSnrContext.ygaintop = gSnrContext.pInfo->disp.ctrl.y.gain + gainrange;
				gSnrContext.ygainbottom = gSnrContext.pInfo->disp.ctrl.y.gain - gainrange;
				gSnrContext.ygainmax = gainrange << 1;
				gSnrContext.ygainmin = 0;
			}
		}
		else
		{
			gSnrContext.ygaintop = gSnrContext.pInfo->disp.ctrl.y.gainmaxmaxtime;
			gSnrContext.ygainbottom = gSnrContext.pInfo->disp.ctrl.y.gain;
			gSnrContext.ygainmax = gSnrContext.pInfo->disp.ctrl.y.gainmaxmaxtime - gSnrContext.pInfo->disp.ctrl.y.gain;
			gSnrContext.ygainmin = 0;
		}
	}

}

void SetNomalLightNoiseTable(void)
{
	UINT8 table[17];
	UINT8 i;

	for(i=0; i<17; i++)
	{
		table[i] = 0xff;
	}
	V558_IspSetNoiseTable(table);
}

void SetLowLightNoiseTable(void)
{
	UINT8 table[17];
	UINT8 i;


	for(i=0; i<17; i++)
	{
		table[i] = 0x10;
	}

	V558_IspSetNoiseTable(table);
}

UINT8 AddExpTime(UINT16 step)
{
	UINT16	temp;

	temp = gSnrContext.eptimes;

	gSnrContext.eptimes += step;


	if(gSnrContext.eptimes > gSnrContext.pInfo->disp.ctrl.maxexp)
	{
		gSnrContext.eptimes = gSnrContext.pInfo->disp.ctrl.maxexp;
	}



	if(temp == gSnrContext.eptimes)
		return 1;

	if(temp < gSnrContext.pInfo->disp.ctrl.dpcThd)
	{

		if(gSnrContext.eptimes >= gSnrContext.pInfo->disp.ctrl.dpcThd)
		{
			SetLowLightNoiseTable();
		}
	}

	return 0;
}

UINT8 DecExpTime(UINT16 step)
{
	UINT16	temp;

	temp = gSnrContext.eptimes;

	if(gSnrContext.eptimes <= step)
	{
		gSnrContext.eptimes = 1;
	}
	else
	{
		gSnrContext.eptimes -= step;
	}

	if(gSnrContext.eptimes < gSnrContext.pInfo->disp.ctrl.minexp)
	{
		gSnrContext.eptimes = gSnrContext.pInfo->disp.ctrl.minexp;
	}

	if(temp == gSnrContext.eptimes)
		return 1;

	if(temp >= gSnrContext.pInfo->disp.ctrl.dpcThd)
	{
		if(gSnrContext.eptimes < gSnrContext.pInfo->disp.ctrl.dpcThd)
		{
			SetNomalLightNoiseTable();
		}
	}

	return 0;
}

UINT8	AutoAddExpTime(UINT8 ymean, UINT8 *ygain)
{

	UINT16 uTempEt = gSnrContext.eptimes;
	UINT16 step;
	if(gSnrContext.eptimes >= ET_ONE_STEP)
	{
		if(gSnrContext.eptimes < gSnrContext.pInfo->disp.ctrl.exppoint)
		{
			if(ymean < gSnrContext.pInfo->disp.ctrl.y.yfieldmulstep)
			{
				step = (gSnrContext.pInfo->disp.ctrl.exppoint/ET_ONE_STEP - gSnrContext.eptimes/ET_ONE_STEP + 1) >> 1;
				if( AddExpTime( step * ET_ONE_STEP) )
				{
					return 1;
				}
			}
			else
			{
				if( AddExpTime(ET_ONE_STEP) )
				{
					return 1;
				}
			}
		}
		else
		{
			step = (gSnrContext.pInfo->disp.ctrl.maxexp/ET_ONE_STEP - gSnrContext.eptimes/ET_ONE_STEP + 3) >> 2;
			if(step < gSnrContext.pInfo->disp.ctrl.y.stepgreattime)
			{
				step = gSnrContext.pInfo->disp.ctrl.y.stepgreattime;
			}
			if( AddExpTime( step * ET_ONE_STEP) )
			{
				return 1;
			}
		}
	}
	else
	{
#if 0
		if(ymean < gSnrContext.pInfo->disp.ctrl.y.yfieldstep)
		{
			if( AddExpTime((ET_ONE_STEP - gSnrContext.eptimes + 3) >> 2 ) )
			{
				return 1;
			}
		}
		else
		{
			if( AddExpTime(1) )
			{
				return 1;
			}

		}
#else
		AddExpTime(1);
#endif

		if(gSnrContext.eptimes > ET_ONE_STEP)
		{
			gSnrContext.eptimes = ET_ONE_STEP;
		}
	}
	if(uTempEt == gSnrContext.eptimes)
	{
		return 1;
	}


	GetGainRange();

	*ygain = gSnrContext.ygainbottom;

	return 0;
}

#if SPECIAL_FUJITSU_MBS04_BAYER == 0

UINT8	AutoDecExpTime(UINT8 ymean, UINT8 *ygain)

{
	UINT16 uTempEt = gSnrContext.eptimes;
	UINT16 step;

	if(gSnrContext.eptimes > ET_ONE_STEP)
	{
		if(gSnrContext.eptimes <= gSnrContext.pInfo->disp.ctrl.exppoint)
		{
			if(ymean > MAX_YMEAN - gSnrContext.pInfo->disp.ctrl.y.yfieldmulstep)
			{
				if( DecExpTime( ((gSnrContext.eptimes/ET_ONE_STEP + 1) >> 1 )*ET_ONE_STEP) )
				{
					return 1;
				}
			}

			else
			{
				if( DecExpTime(ET_ONE_STEP) )
				{
					return 1;
				}
			}
		}
		else
		{
			step = (gSnrContext.eptimes/ET_ONE_STEP - gSnrContext.pInfo->disp.ctrl.exppoint/ET_ONE_STEP + 3) >> 2;
			if(step < gSnrContext.pInfo->disp.ctrl.y.stepgreattime)
			{
				step = gSnrContext.pInfo->disp.ctrl.y.stepgreattime;
			}
			if( DecExpTime(step*ET_ONE_STEP) )
			{
				return 1;
			}
		}
		if(gSnrContext.eptimes < ET_ONE_STEP)
		{
			gSnrContext.eptimes = ET_ONE_STEP;
		}
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_OUT)
		{
#if 0
			if(ymean > MAX_YMEAN - gSnrContext.pInfo->disp.ctrl.y.yfieldstep)
			{
				if( DecExpTime( (gSnrContext.eptimes - gSnrContext.pInfo->disp.ctrl.minexp + 3) >> 2 ) )
				{
					return 1;
				}
			}
			else
			{
				if( DecExpTime(1) )
				{
					return 1;
				}
			}
#else
			DecExpTime(1);
#endif
		}
		else
		{
			gSnrContext.eptimes = ET_ONE_STEP;
		}
	}

	if(uTempEt == gSnrContext.eptimes)
	{
		return 1;
	}

	GetGainRange();
	*ygain = gSnrContext.ygaintop;

	return 0;
}

#else

UINT8	AutoDecExpTime(UINT8 ymean, UINT8 *ygain)

{
	UINT16 uTempEt = gSnrContext.eptimes;
	UINT16 step;

	if(gSnrContext.eptimes > ET_ONE_STEP)
	{
		if(gSnrContext.eptimes <= gSnrContext.pInfo->disp.ctrl.exppoint)
		{
			if(ymean > MAX_YMEAN - gSnrContext.pInfo->disp.ctrl.y.yfieldmulstep)
			{
				if( DecExpTime( ((gSnrContext.eptimes/ET_ONE_STEP + 3) >> 2 )*ET_ONE_STEP) )
				{
					return 1;
				}
			}
			else
			{
				if( DecExpTime(ET_ONE_STEP) )
				{
					return 1;
				}
			}
		}
		else
		{
			step = (gSnrContext.eptimes/ET_ONE_STEP - gSnrContext.pInfo->disp.ctrl.exppoint/ET_ONE_STEP + 3) >> 2;
			if(step < gSnrContext.pInfo->disp.ctrl.y.stepgreattime)
			{
				step = gSnrContext.pInfo->disp.ctrl.y.stepgreattime;
			}
			if( DecExpTime(step*ET_ONE_STEP) )
			{
				return 1;
			}
		}
		if(gSnrContext.eptimes < ET_ONE_STEP)
		{
			gSnrContext.eptimes = ET_ONE_STEP;
		}
	}
	else
	{
		if(gSnrContext.pInfo->disp.ctrl.aflk.lighttype == LIGHT_OUT)
		{
#if 0
			if(ymean > MAX_YMEAN - gSnrContext.pInfo->disp.ctrl.y.yfieldstep)
			{
				if( DecExpTime( (gSnrContext.eptimes - gSnrContext.pInfo->disp.ctrl.minexp + 3) >> 2 ) )
				{
					return 1;
				}
			}
			else
			{
				if( DecExpTime(1) )
				{
					return 1;
				}
			}
#else
			DecExpTime(1);
#endif
		}
		else
		{
			gSnrContext.eptimes = ET_ONE_STEP;
		}
	}

	if(uTempEt == gSnrContext.eptimes)
	{
		return 1;
	}

	GetGainRange();
	*ygain = gSnrContext.ygaintop;

	return 0;
}

#endif


//==============================================================================//
//	static function of auto flicker adjusting                               //
//==============================================================================//
void FlkSetNextFreq(void)
{
	UINT16 times, freq;

	if(gSnrContext.eptimes >= MAX_FLK_ET*ET_ONE_STEP)
	{
		gSnrContext.eptimes = MAX_FLK_ET*ET_ONE_STEP;
	}
	if(gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype == V558_FREQ_50HZ)
	{
		gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype = V558_FREQ_60HZ;
		times = ET_ONE_STEP*Tmflk_60[gSnrContext.eptimes/ET_ONE_STEP - 1];
	}
	else
	{
		gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype = V558_FREQ_50HZ;
		times = ET_ONE_STEP*Tmflk_50[gSnrContext.eptimes/ET_ONE_STEP - 1];
	}

	freq = gSnrContext.pInfo->disp.ctrl.aflk.indoorfreqtype == V558_FREQ_50HZ ? EPFREQ_50HZ:EPFREQ_60HZ;

	SetAeTime(times);

}

/****************************************************************/
//                                                              //
//	Base function of SensorInterface module                     //
//                                                              //
/****************************************************************/







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
void V558_SifSetSensorMode(UINT8 sel)
{
	V558_SetReg(V558_REG_SIF_SNR_MODE, sel);
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
UINT8 V558_SifGetSensorMode(void)
{
	UINT8 sel;

	
	V558_GetReg(V558_REG_SIF_SNR_MODE, &sel);
	return sel;
}

/********************************************************************************
  Description:
	Enable sensor sync singal
  Returns:
	return Last setting for sensor mode.
  Note:
  Remarks:
*********************************************************************************/
UINT8 V558_SifEnableSnrSyncGen(void)
{
	UINT8 uReg;
	
	uReg = V558_SifGetSensorMode();
	V558_SifSetSensorMode(uReg | 0x20);
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
UINT8 V558_SifDisableSnrSyncGen(void)
{
	UINT8 uReg;
	
	uReg = V558_SifGetSensorMode();
	V558_SifSetSensorMode(uReg & 0xdf);
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
UINT8 V558_SifSetDataFormat(UINT8 uFormat)
{
	UINT8 uReg;
	
	uFormat &= 0x1;
	uReg = V558_SifGetSensorMode();
	V558_SifSetSensorMode(uReg | (uFormat << 6));
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
void V558_SifSetSensorOpmode(UINT8 sel)
{
	V558_SetReg(V558_REG_SIF_SNROP_MODE, sel);
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
UINT8 V558_SifGetSensorOpmode(void)
{
	UINT8 sel;
	
	V558_GetReg(V558_REG_SIF_SNROP_MODE, &sel);
	return sel;
}

/********************************************************************************
  Description:
	enable sync signal output to ISP
  Note:
	the ISP and IPP won't work if no sync signal input
  Remarks:
*********************************************************************************/
UINT8 V558_SifEnableSyncGen(void)
{
	UINT8 sel;
	
	sel = V558_SifGetSensorOpmode();
	V558_SifSetSensorOpmode(sel | 0x40);
	return sel;
}

/********************************************************************************
  Description:
	Disable sync signal output to ISP
  Note:
	the ISP and IPP won't work if no sync signal input
  Remarks:
*********************************************************************************/
UINT8 V558_SifDisableSyncGen(void)
{
	UINT8 sel;//, temp;
	
	sel = V558_SifGetSensorOpmode();
	if(sel & 0x40)
	{
		V558_SifSetSensorOpmode(sel & 0xbf);
//		temp = V558_CtrlGetWorkMode();
//		if(temp & 7)
			V558_Delay(2000);
	}
	return sel;
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
void V558_SifSetYuvFormat(UINT8 sel)
{
	V558_SetReg(V558_REG_SIF_YUV_FORMAT, sel);
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
UINT8 V558_SifGetYuvFormat(void)
{
	UINT8 sel;
	
	V558_GetReg(V558_REG_SIF_YUV_FORMAT, &sel);
	
	return sel;
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
void V558_SifSetVsyncCtrl(UINT8 sel)
{
	V558_SetReg(V558_REG_SIF_V_CONTROL, sel);
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
UINT8 V558_SifGetVsyncCtrl(void)
{
	UINT8 sel;
	
	V558_GetReg(V558_REG_SIF_V_CONTROL, &sel);
	return sel;
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
void V558_SifSetHsyncCtrl(UINT8 sel)
{
	V558_SetReg(V558_REG_SIF_H_CONTROL, sel);
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
UINT8 V558_SifGetHsyncCtrl(void)
{
	UINT8 sel;
	
	V558_GetReg(V558_REG_SIF_H_CONTROL, &sel);
	return sel;
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
void V558_SifSetSnrclk(UINT8 sel)
{
	V558_SetReg(V558_REG_SIF_SNRCLK_CNT, sel);
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

UINT8 V558_SifGetSnrclk(void)

{
	UINT8 sel;
	
	V558_GetReg(V558_REG_SIF_SNRCLK_CNT, &sel);

	return sel;
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
void V558_SifSetPixelRate(UINT8 sel)
{
	V558_SetReg(V558_REG_SIF_PIXRATE_SEL, sel);
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
UINT8 V558_SifGetPixelRate(void)
{
	UINT8 sel;
	
	V558_GetReg(V558_REG_SIF_PIXRATE_SEL, &sel);
	sel &= 0x3f;
	return sel;
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
void V558_SifSetSerialclk(UINT8 sel)
{
	V558_SetReg(V558_REG_SIF_SICLK_FACTOR, sel);
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
UINT8 V558_SifGetSerialclk(void)
{
	
	UINT8 sel;
	
	V558_GetReg(V558_REG_SIF_SICLK_FACTOR, &sel);
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
void V558_SifSetI2cclk(UINT16 sel)
{
	V558_SetReg(V558_REG_SIF_I2C_CLKCNTH, (UINT8)(sel >> 8));
	V558_SetReg(V558_REG_SIF_I2C_CLKCNTL, (UINT8)sel);
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
UINT16 V558_SifGetI2cclk(void)
{

	UINT8 x1 = 0, x2 = 0;
	
	V558_GetReg(V558_REG_SIF_I2C_CLKCNTH, &x1);
	V558_GetReg(V558_REG_SIF_I2C_CLKCNTL, &x2);
	return ((UINT16)x1 << 8) | ((UINT16)x2);
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
void	V558_SifSetHrefStart(UINT16 startx)
{
	V558_SetReg(V558_REG_SIF_HREF_STARTH, (UINT8)(startx >> 8));
	V558_SetReg(V558_REG_SIF_HREF_STARTL, (UINT8)startx);
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
UINT16 V558_SifGetHrefStart(void)
{
	UINT8 x1 = 0, x2 = 0;
	
	V558_GetReg(V558_REG_SIF_HREF_STARTH, &x1);
	
	V558_GetReg(V558_REG_SIF_HREF_STARTL, &x2);
	return ((UINT16)x1 << 8) | ((UINT16)x2);
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
void V558_SifSetHrefLength(UINT16 length)
{
	V558_SetReg(V558_REG_SIF_HREF_LENGTHH, (UINT8)(length >> 8));
	V558_SetReg(V558_REG_SIF_HREF_LENGTHL, (UINT8)length);

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
UINT16 V558_SifGetHrefLength(void)
{
	UINT8 x1 = 0, x2 = 0;
	
	V558_GetReg(V558_REG_SIF_HREF_LENGTHH, &x1);
	V558_GetReg(V558_REG_SIF_HREF_LENGTHL, &x2);
	return ((UINT16)x1 << 8) | ((UINT16)x2);
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
void	V558_SifSetVrefStart(UINT16 startx)
{
	V558_SetReg(V558_REG_SIF_VREF_STARTH, (UINT8)(startx >> 8));
	V558_SetReg(V558_REG_SIF_VREF_STARTL, (UINT8)startx);
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
UINT16 V558_SifGetVrefStart(void)

{
	UINT8 x1 = 0, x2 = 0;
	
	V558_GetReg(V558_REG_SIF_VREF_STARTH, &x1);
	V558_GetReg(V558_REG_SIF_VREF_STARTL, &x2);
	return ((UINT16)x1 << 8) | ((UINT16)x2);

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
void V558_SifSetVrefStop(UINT16 stop)
{
	V558_SetReg(V558_REG_SIF_VREF_STOPH, (UINT8)(stop >> 8));
	V558_SetReg(V558_REG_SIF_VREF_STOPL, (UINT8)stop);

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
UINT16 V558_SifGetVrefStop(void)
{
	UINT8 x1 = 0, x2 = 0;
	
	V558_GetReg(V558_REG_SIF_VREF_STOPH, &x1);
	V558_GetReg(V558_REG_SIF_VREF_STOPL, &x2);
	return ((UINT16)x1 << 8) | ((UINT16)x2);
}

/********************************************************************************
  Description:
	config I2C bus device address
  Parameters:
	adr: I2C bus device address
  Remarks:
*********************************************************************************/
void V558_SifSetI2cDevAddr(UINT8 adr)
{
	adr >>= 1;
	adr |= 0x80;
	V558_SetReg(V558_REG_SIF_IIC_DEVADDR, adr);
}

/********************************************************************************
  Description:
	get I2C bus device address setting
  Returns:
	return I2C bus device address
  Remarks:
*********************************************************************************/
UINT8 V558_SifGetI2cDevAddr(void)
{
	UINT8 x;
	
	V558_GetReg(V558_REG_SIF_IIC_DEVADDR, &x);
	return x << 1;
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

void V558_SifSetAeWinStart(UINT16 start)
{
	V558_SetReg(V558_REG_SIF_AEWIN_STARTH, (UINT8)(start >> 8));
	V558_SetReg(V558_REG_SIF_AEWIN_STARTL, (UINT8)start);
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

UINT16 V558_SifGetAeWinStart(void)
{
	UINT8 x1 = 0, x2 = 0;
	
	V558_GetReg(V558_REG_SIF_AEWIN_STARTH, &x1);
	V558_GetReg(V558_REG_SIF_AEWIN_STARTL, &x2);
	return ((UINT16)x1 << 8) | x2;
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
void V558_SifSetAeWinStop(UINT16 x)
{
	V558_SetReg(V558_REG_SIF_AEWIN_STOPH, (UINT8)(x >> 8));
	V558_SetReg(V558_REG_SIF_AEWIN_STOPL, (UINT8)x);
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
UINT16 V558_SifGetAeWinStop(void)
{
	
	UINT8 x1 = 0, x2 = 0;

	V558_GetReg(V558_REG_SIF_AEWIN_STOPH, &x1);
	V558_GetReg(V558_REG_SIF_AEWIN_STOPL, &x2);
	return ((UINT16)x1 << 8) | (UINT16)x2;
}

/********************************************************************************
  Description:
	I2C bus batch write 
  Parameters:
	uNum: AeData length, must in [0, 8]
	AeData: data
  Note:
	The write data number must same at every register's address
	The update time is according to Ae Window's setting
  Remarks:
*********************************************************************************/
void V558_SifI2cAeBatch(UINT8 uNum, V558_I2C_BATCH *AeData)
{
	
	UINT8	i, j, reg_B_ctrl = 0;
	UINT8	uRegAeCount = 0;
	UINT16	regCnt, reg_B_cnt = 0;
	
	if((uNum > 0) && (uNum < 8))
	{
		for(i = 0; i < uNum; i++)
		{

			regCnt = AeData[i].RegBytes & 0x3;
			regCnt <<= i*2;
			reg_B_cnt |= regCnt;
			V558_SetReg(V558_REG_SIF_AE0_DATA+uRegAeCount, AeData[i].RegAddr);
			uRegAeCount++;
			for(j = 0; j < AeData[i].RegBytes; j++)
			{
				V558_SetReg(V558_REG_SIF_AE0_DATA+uRegAeCount, AeData[i].RegVal[j]);

				uRegAeCount++;
			}
		}
		V558_SetReg(V558_REG_SIF_IIC_BYTE, AeData[0].RegBytes & 0x3);
		V558_SetReg(V558_REG_SIF_B_CNTL, (UINT8)reg_B_cnt);

		V558_SetReg(V558_REG_SIF_B_CNTH, (UINT8)(reg_B_cnt >> 8));
		
		reg_B_ctrl = uNum;	//The number of register addresses
		reg_B_ctrl |= 0x8;	//AE data update done bit.

		V558_SetReg(V558_REG_SIF_AEBUS_CTRL, (UINT8)reg_B_ctrl);
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
UINT8 V558_SifI2cWriteByte(UINT8 uAddr, UINT8 uVal)
{
	UINT8 uState = 0;
	UINT32	count = I2CWAITTIME;
	
	while(count--)
	{
		V558_GetReg(V558_REG_SIF_BUS_STATE, &uState);
		if((uState&0x2) == 0x2)
		{
			break;
		}
	}
	
	if(!count)
		return SNRFAILED;
	V558_SetReg(V558_REG_SIF_SNRADDR, uAddr);
	V558_SetReg(V558_REG_SIF_SNRWRDATAH, uVal);
	V558_SetReg(V558_REG_SIF_IIC_BYTE, 1);
	V558_SetReg(V558_REG_SIF_SNRACSCTR, 0x1);

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
UINT8 V558_SifI2cWriteWord(UINT8 uAddr, UINT16 uVal)
{
	UINT8 uState = 0;
	UINT32	count = I2CWAITTIME;
	
	while(count--)
	{
		V558_GetReg(V558_REG_SIF_BUS_STATE, &uState);
		if((uState&0x2) == 0x2)
		{

			break;
		}
	}
	if(!count)
		return SNRFAILED;
	V558_SetReg(V558_REG_SIF_SNRADDR, uAddr);
	V558_SetReg(V558_REG_SIF_SNRWRDATAH, (UINT8)(uVal >> 8));
	V558_SetReg(V558_REG_SIF_SNRWRDATAM, (UINT8)uVal);
	V558_SetReg(V558_REG_SIF_IIC_BYTE, 2);
	V558_SetReg(V558_REG_SIF_SNRACSCTR, 0x1);
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
UINT8 V558_SifI2cWriteTriWord(UINT8 uAddr, UINT32 uVal)
{

	UINT8 uState = 0;
	UINT32	count = I2CWAITTIME;
	
	while(count--)
	{
		V558_GetReg(V558_REG_SIF_BUS_STATE, &uState);
		if((uState&0x2) == 0x2)
		{
			break;
		}
	}
	if(!count)
		return SNRFAILED;
	V558_SetReg(V558_REG_SIF_SNRADDR, uAddr);
	V558_SetReg(V558_REG_SIF_SNRWRDATAH, (UINT8)(uVal >> 16));
	V558_SetReg(V558_REG_SIF_SNRWRDATAM, (UINT8)(uVal >> 8));

	V558_SetReg(V558_REG_SIF_SNRWRDATAL, (UINT8)uVal);

	V558_SetReg(V558_REG_SIF_IIC_BYTE, 3);
	V558_SetReg(V558_REG_SIF_SNRACSCTR, 0x1);
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
UINT8 V558_SifI2cReadByte(UINT8 uAddr, UINT8 *uVal)
{
	UINT8 uState = 0;
	UINT32	count = I2CWAITTIME;
	
	while(count--)
	{
		V558_GetReg(V558_REG_SIF_BUS_STATE, &uState);
		if((uState&0x2) == 0x2)
		{
			break;
		}
	}
	if(!count)

		return SNRFAILED;
	V558_SetReg(V558_REG_SIF_SNRADDR, uAddr);
	V558_SetReg(V558_REG_SIF_IIC_BYTE, 1);
	V558_SetReg(V558_REG_SIF_SNRACSCTR, 0x2);
	count = I2CWAITTIME;
	while(count--)
	{

		V558_GetReg(V558_REG_SIF_BUS_STATE, &uState);
		if((uState&0x2) == 0x2)
		{
			
			
			break;
		}
	}
	if(!count)
		return SNRFAILED;
	V558_GetReg(V558_REG_SIF_SNRRDDATAH, uVal);
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
UINT8 V558_SifI2cReadWord(UINT8 uAddr, UINT16 *uVal)
{
	UINT8 uState = 0;

	UINT8 x1 = 0, x2 = 0;
	UINT32	count = I2CWAITTIME;
	
	while(count--)
	{
		
		V558_GetReg(V558_REG_SIF_BUS_STATE, &uState);
		if((uState&0x2) == 0x2)
		{
			break;
		}
	}
	if(!count)
		return SNRFAILED;
	V558_SetReg(V558_REG_SIF_SNRADDR, uAddr);
	V558_SetReg(V558_REG_SIF_IIC_BYTE, 2);
	V558_SetReg(V558_REG_SIF_SNRACSCTR, 0x2);
	count = I2CWAITTIME;
	while(count--)
	{
		V558_GetReg(V558_REG_SIF_BUS_STATE, &uState);
		if((uState&0x2) == 0x2)
		{
			break;
		}
	}

	if(!count)
		return SNRFAILED;
	V558_GetReg(V558_REG_SIF_SNRRDDATAH, &x1);
	V558_GetReg(V558_REG_SIF_SNRRDDATAM, &x2);


	*uVal = ((UINT16)x1 << 8) | ((UINT16)x2);
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
UINT8 V558_SifI2cReadTriByte(UINT8 uAddr, UINT32 *uVal)
{
	UINT8 uState = 0;
	UINT8 x1 = 0, x2 = 0, x3 = 0;
	UINT32	count = I2CWAITTIME;
	
	while(count--)
	{
		V558_GetReg(V558_REG_SIF_BUS_STATE, &uState);
		if((uState&0x2) == 0x2)
		{
			break;
		}
	}
	if(!count)
		return SNRFAILED;
	V558_SetReg(V558_REG_SIF_SNRADDR, uAddr);
	V558_SetReg(V558_REG_SIF_IIC_BYTE, 3);
	V558_SetReg(V558_REG_SIF_SNRACSCTR, 0x2);
	count = I2CWAITTIME;
	while(count--)
	{
		V558_GetReg(V558_REG_SIF_BUS_STATE, &uState);
		if((uState&0x2) == 0x2)
		{
			break;

		}
	}
	if(!count)
		return SNRFAILED;
	V558_GetReg(V558_REG_SIF_SNRRDDATAH, &x1);
	V558_GetReg(V558_REG_SIF_SNRRDDATAM, &x2);
	V558_GetReg(V558_REG_SIF_SNRRDDATAL, &x3);
	*uVal = ((UINT32)x1 << 16) | ((UINT32)x2 << 8) | (UINT32)x3;
	
	return SUCCEED;
}

/********************************************************************************
  Description:
	config internal ISP base function
  Parameters:
	x:		setting value
  Note:
			<TABLE>
			value		meaning
			-----		-------
			BIT0-3		pixel rate = (val+1)*clk
			BIT4		G pixel first
			BIT5		1:BG line first		0:RG line first
			BIT6		1:only testbar		0:video on center window
			BIT7		enable testbar function
			BIT8		enable edge enhancement function
			BIT9		enable lens fall off function 
			BIT10		dpd/dpc and noise removal function 
			BIT11		RGB gamma correction
			BIT12		color matrix correction
			BIT13		Y gamma correction
			BIT14		digital RGB gain
			BIT15		gray image
			</TABLE>
	Remarks:
*********************************************************************************/
void V558_IspSetBasctl(UINT16 x)
{
	UINT8 uSenMode = 0;
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_SetReg(V558_REG_ISP_BASCTL, (UINT8)x);
	

	
	V558_SetReg(V558_REG_ISP_BASCTL+1, (UINT8)(x >> 8));
	V558_SifSetSensorOpmode(uSenMode);

}

/********************************************************************************
  Description:
	get internal ISP base function setting
  Returns:
	return setting value
  Remarks:
*********************************************************************************/
UINT16 V558_IspGetBasctl(void)
{
	

	UINT8 x1 = 0, x2 = 0;
	
	V558_GetReg(V558_REG_ISP_BASCTL, &x1);
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x2);
	
	return ((UINT16)x2 << 8) | (UINT16)x1;
}

/********************************************************************************
  Description:
	enable digital RGB gain function
  Remarks:
*********************************************************************************/
void V558_IspEnableRgbGain(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x | 0x40);
	V558_SifSetSensorOpmode(uSenMode);
	
}

/********************************************************************************
  Description:
	disable digital RGB gain function
  Remarks:
*********************************************************************************/
void V558_IspDisableRgbGain(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x & 0xbf);
	V558_SifSetSensorOpmode(uSenMode);
	
}

/********************************************************************************
  Description:
	enable Y gamma correction function
  Remarks:
*********************************************************************************/
void V558_IspEnableYGamma(void)
{

	
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x | 0x20);
	V558_SifSetSensorOpmode(uSenMode);
	

}

/********************************************************************************
  Description:
	disable Y gamma correction function
  Remarks:
*********************************************************************************/
void V558_IspDisableYGamma(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x & 0xdf);
	V558_SifSetSensorOpmode(uSenMode);

	

}

/********************************************************************************
  Description:
	enable color matrix correction function
  Remarks:
*********************************************************************************/
void V558_IspEnableColorMatrix(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x | 0x10);
	V558_SifSetSensorOpmode(uSenMode);
	
}

/********************************************************************************
  Description:
	disable color matrix correction function
  Remarks:
*********************************************************************************/
void V558_IspDisableColorMatrix(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();

	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);

	V558_SetReg(V558_REG_ISP_BASCTL+1, x & 0xef);
	V558_SifSetSensorOpmode(uSenMode);
	
}

/********************************************************************************
  Description:
	enable RGB Gamma  correction function
  Remarks:
*********************************************************************************/
void V558_IspEnableRgbGamma(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x | 0x8);
	V558_SifSetSensorOpmode(uSenMode);
	
}

/********************************************************************************
  Description:
	disable RGB Gamma  correction function
  Remarks:
*********************************************************************************/
void V558_IspDisableRgbGamma(void)
{

	UINT8	x;
	UINT8 uSenMode = 0;

	
	uSenMode = V558_SifGetSensorOpmode();

	V558_SifDisableSyncGen();
	
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x & 0xf7);
	V558_SifSetSensorOpmode(uSenMode);
	
	
}


/********************************************************************************
  Description:
	enable DPC and noise removal function
  Remarks:
*********************************************************************************/
void V558_IspEnableDpc(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x | 0x4);
	V558_SifSetSensorOpmode(uSenMode);
	
}


/********************************************************************************
  Description:
	disable DPC and noise removal function
  Remarks:
*********************************************************************************/

void V558_IspDisableDpc(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x & 0xfb);
	V558_SifSetSensorOpmode(uSenMode);
	
}


/********************************************************************************
  Description:
	enable lens fall off function
  Remarks:
*********************************************************************************/
void V558_IspEnableLensoff(void)
{
	UINT8	x;

	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x | 2);
	V558_SifSetSensorOpmode(uSenMode);
	
}

/********************************************************************************
  Description:
	disable lens fall off function
  Remarks:
*********************************************************************************/
void V558_IspDisableLensoff(void)
{
	
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x & 0xfd);
	V558_SifSetSensorOpmode(uSenMode);
	
}

/********************************************************************************
  Description:
	enable edge enhancement function
  Remarks:
*********************************************************************************/
void V558_IspEnableEdgeenh(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x | 1);
	V558_SifSetSensorOpmode(uSenMode);
	
}


/********************************************************************************
  Description:
	disable edge enhancement function
  Remarks:
*********************************************************************************/
void V558_IspDisableEdgeenh(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL+1, &x);
	V558_SetReg(V558_REG_ISP_BASCTL+1, x & 0xfe);
	V558_SifSetSensorOpmode(uSenMode);
	
}

/********************************************************************************
  Description:
	enable testbar function
  Remarks:
*********************************************************************************/
void V558_IspEnableTestbar(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL, &x);
	V558_SetReg(V558_REG_ISP_BASCTL, x | 0x80);
	V558_SifSetSensorOpmode(uSenMode);
	
}

/********************************************************************************
  Description:
	disabel testbar function
  Remarks:
	state: valid
*********************************************************************************/
void V558_IspDisableTestbar(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL, &x);
	V558_SetReg(V558_REG_ISP_BASCTL, x & 0x7f);
	V558_SifSetSensorOpmode(uSenMode);

	
}


/********************************************************************************
  Description:
	enable bar video function
  Remarks:
*********************************************************************************/
void V558_IspEnableBarVideo(void)

{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();
	V558_GetReg(V558_REG_ISP_BASCTL, &x);
	V558_SetReg(V558_REG_ISP_BASCTL, x & 0xbf);
	V558_SifSetSensorOpmode(uSenMode);
	
}


/********************************************************************************
  Description:
	disable bar video function
  Remarks:
*********************************************************************************/
void V558_IspDisableBarVideo(void)
{
	UINT8	x;
	UINT8 uSenMode = 0;
	
	uSenMode = V558_SifGetSensorOpmode();
	V558_SifDisableSyncGen();

	V558_GetReg(V558_REG_ISP_BASCTL, &x);

	V558_SetReg(V558_REG_ISP_BASCTL, x | 0x40);

	V558_SifSetSensorOpmode(uSenMode);
	
}

/********************************************************************************
  Description:
	set isp's pixel rate
  Remarks:
	state: valid
*********************************************************************************/
void V558_IspSetPixelRate(UINT8 rate)

{
	UINT8 uReg;
	V558_GetReg(V558_REG_ISP_BASCTL, &uReg);
	uReg &= 0xf0;
	uReg |= (rate&0xf);
	V558_SetReg(V558_REG_ISP_BASCTL, uReg);
}

/********************************************************************************
  Description:
	set testbar window's size
  Parameters:
	size:	testbar windows's size
  Remarks:
*********************************************************************************/
void V558_IspSetTestBarWin(V558_SIZE size)
{
	V558_SetReg(V558_REG_ISP_WINWD, (UINT8)size.cx);
	V558_SetReg(V558_REG_ISP_WINWD+1, (UINT8)(size.cx >> 8));
	V558_SetReg(V558_REG_ISP_WINHT, (UINT8)size.cy);
	V558_SetReg(V558_REG_ISP_WINHT+1, (UINT8)(size.cy >> 8));
}

/********************************************************************************
  Description:
	get testbar window's size
  Parameters:
	size:	return testbar windows's size
  Remarks:
*********************************************************************************/
void V558_IspGetTestBarWin(V558_SIZE *size)
{
	UINT8 x1 = 0, x2 = 0;
	


	V558_GetReg(V558_REG_ISP_WINWD, &x1);
	V558_GetReg(V558_REG_ISP_WINWD+1, &x2);

	size->cx = ((UINT16)x2 << 8) | (UINT16)x1;
	V558_GetReg(V558_REG_ISP_WINHT, &x1);
	V558_GetReg(V558_REG_ISP_WINHT+1, &x2);
	size->cy = ((UINT16)x2 << 8) | (UINT16)x1;
}

/********************************************************************************
  Description:
	set image window's size
  Parameters:
	size:	image windows's size
  Remarks:
*********************************************************************************/
void V558_IspSetImageSize(V558_SIZE size)
{
	V558_SetReg(V558_REG_ISP_IMGWD, (UINT8)size.cx);
	V558_SetReg(V558_REG_ISP_IMGWD+1, (UINT8)(size.cx >> 8));
	V558_SetReg(V558_REG_ISP_IMGHT, (UINT8)size.cy);
	V558_SetReg(V558_REG_ISP_IMGHT+1, (UINT8)(size.cy >> 8));
}


/********************************************************************************
  Description:
	get image window's size
  Parameters:
	size:	return image windows's size
  Remarks:
*********************************************************************************/
void V558_IspGetImageSize(V558_SIZE *size)

{
	UINT8 x1 = 0, x2 = 0;
	
	V558_GetReg(V558_REG_ISP_IMGWD, &x1);
	V558_GetReg(V558_REG_ISP_IMGWD+1, &x2);
	size->cx = ((UINT16)x2 << 8) | (UINT16)x1;
	V558_GetReg(V558_REG_ISP_IMGHT, &x1);
	V558_GetReg(V558_REG_ISP_IMGHT+1, &x2);
	size->cy = ((UINT16)x2 << 8) | (UINT16)x1;
}

/********************************************************************************
  Description:
	set edge enhancement parameters
  Parameters:
	edge:	edge enhancement setting
  Remarks:
*********************************************************************************/
void V558_IspSetEdgeParm(PV558_EDGEPARM edge)
{
	V558_SetReg(V558_REG_ISP_FEM_M, edge->femm);
	V558_SetReg(V558_REG_ISP_FEM_P, edge->femp);
	V558_SetReg(V558_REG_ISP_FEM_X1, edge->femx1);
	V558_SetReg(V558_REG_ISP_FEM_X2, edge->femx2);

	V558_SetReg(V558_REG_ISP_FEM_MIN, edge->femmin);
	V558_SetReg(V558_REG_ISP_FEM_MAX, edge->femmax);
}

/********************************************************************************
  Description:
	get edge enhancement parameters setting
  Parameters:
	edge:	return edge enhancement setting
  Remarks:
*********************************************************************************/
void V558_IspGetEdgeParm(V558_EDGEPARM *edge)
{
	V558_GetReg(V558_REG_ISP_FEM_M, &(edge->femm));
	V558_GetReg(V558_REG_ISP_FEM_P, &(edge->femp));
	V558_GetReg(V558_REG_ISP_FEM_X1, &(edge->femx1));
	V558_GetReg(V558_REG_ISP_FEM_X2, &(edge->femx2));
	V558_GetReg(V558_REG_ISP_FEM_MIN, &(edge->femmin));
	V558_GetReg(V558_REG_ISP_FEM_MAX, &(edge->femmax));
}

/********************************************************************************
  Description:
	set lens fall off parameters
  Parameters:
	lens:	lens fall off setting
  Remarks:
*********************************************************************************/
void V558_IspSetLensParm(PV558_LENSFALLOFF lens)
{

	V558_SetReg(V558_REG_ISP_LFCENX, (UINT8)lens->lfcenx);
	V558_SetReg(V558_REG_ISP_LFCENX+1, (UINT8)(lens->lfcenx >> 8));
	V558_SetReg(V558_REG_ISP_LFCENY, (UINT8)lens->lfceny);
	V558_SetReg(V558_REG_ISP_LFCENY+1, (UINT8)(lens->lfceny >> 8));
	V558_SetReg(V558_REG_ISP_LFFREC, (UINT8)lens->lffrec);
	V558_SetReg(V558_REG_ISP_LFFREC+1, (UINT8)(lens->lffrec >> 8));
}

/********************************************************************************
  Description:
	get lens fall off parameters
  Parameters:
	lens:	return lens fall off setting
  Remarks:
*********************************************************************************/
void V558_IspGetLensParm(V558_LENSFALLOFF *lens)
{
	UINT8 x1, x2;
	
	V558_GetReg(V558_REG_ISP_LFCENX, &x1);
	V558_GetReg(V558_REG_ISP_LFCENX+1, &x2);
	lens->lfcenx = ((UINT16)x2 << 8) | x1;
	V558_GetReg(V558_REG_ISP_LFCENY, &x1);
	V558_GetReg(V558_REG_ISP_LFCENY+1, &x2);
	lens->lfceny = ((UINT16)x2 << 8) | x1;
	V558_GetReg(V558_REG_ISP_LFFREC, &x1);
	V558_GetReg(V558_REG_ISP_LFFREC+1, &x2);
	lens->lffrec = ((UINT16)x2 << 8) | x1;
}

/********************************************************************************
  Description:
	config auto flicker control
  Parameters:
	ctl:	setting value
  Note:
			  <TABLE>
			  value		meaning
			  -----		-------
			  BIT0-8		auto fliker line start
			  BIT9-14		auto fliker line count
			  BIT15		auto fliker line mode
			  </TABLE>
  Remarks:
*********************************************************************************/
void V558_IspSetAflnctl(UINT16 ctl)
{
	V558_SetReg(V558_REG_ISP_AFLNCTL, (UINT8)ctl);
	V558_SetReg(V558_REG_ISP_AFLNCTL+1, (UINT8)(ctl >> 8));
}

/********************************************************************************
  Description:
	get config auto flicker control setting
  Returns:
	return setting value
  Note:
		 <TABLE>
		 value		meaning
		 -----		-------
		 BIT0-8		auto fliker line start
		 BIT9-14		auto fliker line count
		 BIT15		auto fliker line mode
		 </TABLE>
  Remarks:
*********************************************************************************/
UINT16 V558_IspGetAflnctl(void)
{
	UINT8	x1, x2;
	
	V558_GetReg(V558_REG_ISP_AFLNCTL, &x1);
	V558_GetReg(V558_REG_ISP_AFLNCTL+1, &x2);
	return ((UINT16)x2 << 8) | x1;
}

/********************************************************************************
  Description:
	config auto flicker line start
  Parameters:
	aflst:	line start value
  Remarks:
*********************************************************************************/
void V558_IspSetAflst(UINT16 aflst)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFLNCTL+1, &x);
	V558_SetReg(V558_REG_ISP_AFLNCTL, (UINT8)aflst);
	V558_SetReg(V558_REG_ISP_AFLNCTL+1, (x & 0xfe) | ((UINT8)(aflst >> 8) & 1));
}

/********************************************************************************
  Description:
	get auto flicker line start setting
  Returns:
	return line start value
  Remarks:
*********************************************************************************/
UINT16 V558_IspGetAflst(void)
{
	UINT8	x1, x2;
	
	V558_GetReg(V558_REG_ISP_AFLNCTL, &x1);
	V558_GetReg(V558_REG_ISP_AFLNCTL+1, &x2);
	return ((UINT16)(x2 & 0xfe) << 8) | x1;
}

/********************************************************************************
  Description:
	config auto flicker line count
  Parameters:
	aflst:	line count
  Remarks:
*********************************************************************************/
void V558_IspSetAflcnt(UINT8 cnt)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFLNCTL+1, &x);
	V558_SetReg(V558_REG_ISP_AFLNCTL+1, (x & 0x81) | ((cnt & 0x3f) << 1));
}

/********************************************************************************
  Description:
	get auto flicker line count setting
  Returns:
	return line count
  Remarks:
	state: valid
*********************************************************************************/
UINT8 V558_IspGetAflcnt(void)
{
	UINT8 x;
	

	V558_GetReg(V558_REG_ISP_AFLNCTL+1, &x);
	return (x & 0x7e) >> 1;
}

/********************************************************************************
  Description:
	enable auto flicker function
  Remarks:
*********************************************************************************/
void V558_IspEnableAflmode(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFLNCTL+1, &x);
	V558_SetReg(V558_REG_ISP_AFLNCTL+1, x | 0x80);
}

/********************************************************************************
  Description:
	disable auto flicker function
  Remarks:
*********************************************************************************/
void V558_IspDisableAflmode(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFLNCTL+1, &x);
	V558_SetReg(V558_REG_ISP_AFLNCTL+1, x & 0x7f);
}

/********************************************************************************
  Description:
	config auto flicker line step
  Parameters:
	step:	line step
  Remarks:
*********************************************************************************/
void V558_IspSetAflstep(UINT8 step)
{
	V558_SetReg(V558_REG_ISP_AFLNSTEP, step & 0xf);
}

/********************************************************************************
  Description:
	get auto flicker line step setting
  Returns:
	return line step
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetAflstep(void)
{
	UINT8 x;
	
	V558_GetReg(V558_REG_ISP_AFLNSTEP, &x);
	
	return x & 0xf;
}

/********************************************************************************
  Description:
	read antiflicker threshold
  Returns:
	return threshold value
  Remarks:
*********************************************************************************/
UINT32 V558_IspGetAfThreshold(void)
{
	UINT8 x1, x2, x3;
	
	V558_GetReg(V558_REG_ISP_AFTHD, &x1);
	V558_GetReg(V558_REG_ISP_AFTHD+1, &x2);
	
	V558_GetReg(V558_REG_ISP_AFTHD+2, &x3);
	return ((UINT32)x3 << 16) | ((UINT32)x2 << 8) | (UINT32)x1;
}

/********************************************************************************
  Description:
	config digital red gain
  Parameters:
	x:	setting value
  Remarks:
*********************************************************************************/
void V558_IspSetRgain(UINT8 x)
{
	V558_SetReg(V558_REG_ISP_RGAIN, x);
}


/********************************************************************************
  Description:
	get digital red gain setting
  Returns:
	return digital red gain setting
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetRgain(void)
{
	UINT8 x;
	
	V558_GetReg(V558_REG_ISP_RGAIN, &x);
	return x;
	
}

/********************************************************************************
  Description:
	config digital green gain
  Parameters:
	x:	setting value
  Remarks:
*********************************************************************************/
void V558_IspSetGgain(UINT8 x)
{
	V558_SetReg(V558_REG_ISP_GGAIN, x);
}

/********************************************************************************
  Description:
	get digital green gain setting
  Returns:
	return digital green gain setting
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetGgain(void)
{
	UINT8 x;
	

	V558_GetReg(V558_REG_ISP_GGAIN, &x);

	return x;
}


/********************************************************************************
  Description:
	config digital blue gain
  Parameters:
	x:	setting value
  Remarks:
*********************************************************************************/
void V558_IspSetBgain(UINT8 x)
{
	V558_SetReg(V558_REG_ISP_BGAIN, x);
}

/********************************************************************************
  Description:
	get digital blue gain setting
  Returns:
	return digital blue gain setting
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetBgain(void)
{
	UINT8 x;
	
	V558_GetReg(V558_REG_ISP_BGAIN, &x);
	return x;
}

/********************************************************************************
  Description:
	config digital global gain
  Parameters:
	x:	setting value
  Remarks:
*********************************************************************************/
void V558_IspSetGlbgain(UINT8 x)
{
	
	V558_SetReg(V558_REG_ISP_GLBGAIN, x);
}


/********************************************************************************
  Description:
	get digital global gain setting
  Returns:
	return digital global gain setting
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetGlbgain(void)
{
	UINT8 x;
	
	V558_GetReg(V558_REG_ISP_GLBGAIN, &x);

	return x;
}

/********************************************************************************
  Description:
	enable global gain function
  Remarks:
*********************************************************************************/
void V558_IspEnableGlbgain(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_GLBGAIN+1, &x);


	V558_SetReg(V558_REG_ISP_GLBGAIN+1, x | 0x20);
}

/********************************************************************************
  Description:
	disable global gain function
  Remarks:
*********************************************************************************/
void V558_IspDisableGlbgain(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_GLBGAIN+1, &x);
	V558_SetReg(V558_REG_ISP_GLBGAIN+1, x & ~0x20);
}

/********************************************************************************
  Description:
	config digital global gain effect delay
  Parameters:
	val:	setting value
  Note:
	after val frames, the affect will active then
  Remarks:
*********************************************************************************/
void V558_IspSetGlbgainDelay(UINT8 val)
{
	UINT8 x;
	
	
	V558_GetReg(V558_REG_ISP_GLBGAIN+1, &x);
	V558_SetReg(V558_REG_ISP_GLBGAIN+1, (x & ~0x1c) | ((val & 0x7) << 2));

}


/********************************************************************************
  Description:
	get digital global gain effect delay
  Returns:
	return setting value
  Remarks:
*********************************************************************************/

UINT8 V558_IspGetGlbgainDelay(void)
{
	UINT8 x;
	
	V558_GetReg(V558_REG_ISP_GLBGAIN+1, &x);
	return (x & 0x1c) >> 2;
}

/********************************************************************************
  Description:
	config digital global gain effect fraction
  Parameters:
	val:	setting value
  Note:
			<TABLE>
			val			step
			---			----
			00			0x40
			01			0x20
			10			0x10
			11			0x8
			</TABLE>
  Remarks:
	this is gain unit, the real digita gain = 
	V558_IspGetGlbgain() / (step value);
*********************************************************************************/
void V558_IspSetGlbgainStep(UINT8 val)
{
	UINT8 x;
	
	V558_GetReg(V558_REG_ISP_GLBGAIN+1, &x);
	V558_SetReg(V558_REG_ISP_GLBGAIN+1, (x & ~3) | (val & 0x3));
	
}


/********************************************************************************
  Description:
	get digital global gain effect fraction setting
  Returns:
		  00		if step is 0x40
		  01		if step is 0x20
		  10		if step is 0x10
		  11		if step is 0x8
  Note:
	this is gain unit, the real digita gain = 
	V558_IspGetGlbgain() / (step value);
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetGlbgainStep(void)
{

	UINT8 x;
	
	V558_GetReg(V558_REG_ISP_GLBGAIN+1, &x);
	return x & 3;
}

/********************************************************************************
  Description:
	config color conversion parameters
  Parameters:
	val:	color conversion paramters
  Remarks:
*********************************************************************************/
void V558_IspSetColorCvt(PV558_COLORCVT val)
{
	V558_SetReg(V558_REG_ISP_CRADJ, val->cradj);
	V558_SetReg(V558_REG_ISP_CBADJ, val->cbadj);
	V558_SetReg(V558_REG_ISP_CROFS, val->croft);
	V558_SetReg(V558_REG_ISP_CBOFS, val->cboft);

}

/********************************************************************************
  Description:
	Get color conversion parameters
  Parameters:
	val:	return color conversion parameters
  Remarks:
*********************************************************************************/
void V558_IspGetColorCvt(V558_COLORCVT *val)
{
	V558_GetReg(V558_REG_ISP_CRADJ, &(val->cradj));
	V558_GetReg(V558_REG_ISP_CBADJ, &(val->cbadj));
	V558_GetReg(V558_REG_ISP_CROFS, &(val->croft));
	V558_GetReg(V558_REG_ISP_CBOFS, &(val->cboft));
}

/********************************************************************************
  Description:
	config color correction matrix
  Parameters:
	x:	color correction matrix
  Note:
	CMA11 -- 33 is between [-127, 127]
	\For positive values, CMAxx =Integer (Real Coefficient Value x 64);
	\For negative values, CMAxx = 2's Complement (Integer (Real Coefficient Value x 64));
	\Fundamental color matrix equation
	\R' = R*CMA11 + G*CMA12 + B*CMA13 + OFSR
	\G' = R*CMA21 + G*CMA22 + B*CMA23 + OFSG
	\B' = R*CMA31 + G*CMA32 + B*CMA33 + OFSB
  Remarks:
*********************************************************************************/
void V558_IspSetColorMatrix(PV558_COLORMATRIX x)
{
	UINT8 i;
	UINT8 *p = (UINT8 *)x;
	
	for(i = 0; i < 9; i++)
		V558_SetReg(V558_REG_ISP_CMAT11+i, p[i]);
}

/********************************************************************************
  Description:
	get color correction matrix
  Parameters:
	x:	return color correction matrix setting
  Remarks:
*********************************************************************************/
void V558_IspGetColorMatrix(V558_COLORMATRIX *x)
{
	UINT8 i;
	UINT8 *p = (UINT8 *)x;
	
	
	for(i = 0; i < 9; i++)
		V558_GetReg(V558_REG_ISP_CMAT11+i, p+i);
}


/********************************************************************************
  Description:
	config color correction matrix offset
  Parameters:
	x:	color correction matrix offset
  Remarks:
*********************************************************************************/
void V558_IspSetColorMatrixOff(PV558_COLORMATRIXOFF x)
{
	UINT8 i;
	UINT8 *p = (UINT8 *)x;
	
	for(i = 0; i < 3; i++)
		V558_SetReg(V558_REG_ISP_OFSR+i, p[i]);
}


/********************************************************************************
  Description:
	get color correction matrix offset
  Parameters:
	x:	return color correction matrix offset
  Remarks:
*********************************************************************************/
void V558_IspGetColorMatrixOff(V558_COLORMATRIXOFF *x)
{
	UINT8 i;
	UINT8 *p = (UINT8 *)x;
	
	for(i = 0; i < 3; i++)
		V558_GetReg(V558_REG_ISP_OFSR+i, p+i);
}


/********************************************************************************
  Description:
	config luminance gamma curve start
  Parameters:
	x:	luminance gamma cruve start value
  Remarks:
*********************************************************************************/
void V558_IspSetYGammaStart(UINT8 *x)
{
	UINT8 i;
	
	for(i = 0; i < 9; i++)
		V558_SetReg(V558_REG_ISP_YGMST0+i, x[i]);
}

/********************************************************************************
  Description:
	get luminance gamma curve start setting
  Parameters:
	x:	return luminance gamma cruve start setting
  Remarks:
*********************************************************************************/
void V558_IspGetYGammaStart(UINT8 *x)
{
	UINT8 i;
	
	for(i = 0; i < 9; i++)
		V558_GetReg(V558_REG_ISP_YGMST0+i, x+i);
}

/********************************************************************************
  Description:
	config luminance gamma curve slope
  Parameters:
	x:	luminance gamma cruve slope value
  Remarks:
*********************************************************************************/
void V558_IspSetYGammaSlope(UINT8 *x)
{
	UINT8 i;

	
	for(i = 0; i < 9; i++)
		V558_SetReg(V558_REG_ISP_YGASP0+i, x[i]);
}


/********************************************************************************
  Description:
	get luminance gamma curve slope
  Parameters:
	x:	return luminance gamma cruve slope value
  Remarks:
*********************************************************************************/
void V558_IspGetYGammaSlope(UINT8 *x)
{

	UINT8 i;

	
	for(i = 0; i < 9; i++)
		V558_GetReg(V558_REG_ISP_YGASP0+i, x+i);
}


/********************************************************************************
  Description:
	config R gamma curve start
  Parameters:
	x:	R gamma cruve start value
  Remarks:
*********************************************************************************/
void V558_IspSetRGammaStart(UINT8 *x)
{
	UINT8 i;
	
	for(i = 0; i < 17; i++)
		V558_SetReg(V558_REG_ISP_RGMST0+i, x[i]);
}

/********************************************************************************
  Description:
	get R gamma curve start setting
  Parameters:
	x:	return R gamma curve start setting
  Remarks:
*********************************************************************************/
void V558_IspGetRGammaStart(UINT8 *x)
{
	UINT8 i;
	
	for(i = 0; i < 17; i++)
		V558_GetReg(V558_REG_ISP_RGMST0+i, x+i);
}

/********************************************************************************
  Description:
	config G gamma curve start
  Parameters:
	x:	G gamma cruve start value
  Remarks:
*********************************************************************************/
void V558_IspSetGGammaStart(UINT8 *x)
{
	UINT8 i;
	
	for(i = 0; i < 17; i++)

		V558_SetReg(V558_REG_ISP_GGMST0+i, x[i]);
}

/********************************************************************************
  Description:
	get G gamma curve start setting
  Parameters:
	x:	return G gamma curve start setting
  Remarks:
*********************************************************************************/
void V558_IspGetGGammaStart(UINT8 *x)
{
	UINT8 i;

	
	for(i = 0; i < 17; i++)
		V558_GetReg(V558_REG_ISP_GGMST0+i, x+i);
}

/********************************************************************************
  Description:
	config B gamma curve start
  Parameters:
	x:	B gamma cruve start value
  Remarks:
*********************************************************************************/
void V558_IspSetBGammaStart(UINT8 *x)
{
	UINT16 i;
	
	for(i = 0; i < 17; i++)
		V558_SetReg(V558_REG_ISP_BGMST0+i, x[i]);
}

/********************************************************************************
  Description:
	get B gamma curve start setting
  Parameters:
	x:	return B gamma curve start setting
  Remarks:
*********************************************************************************/
void V558_IspGetBGammaStart(UINT8 *x)
{
	UINT16 i;
	
	for(i = 0; i < 17; i++)
		V558_GetReg(V558_REG_ISP_BGMST0+i, x+i);
}

/********************************************************************************
  Description:
	config auto function control
  Parameters:
	x:	auto function control setting
  Note:
			<TABLE>
			value		meaning
			-----		-------
			BIT0		Auto-flicker function on	
			BIT1-3		auto function interrupt frequency: 
			if 0 -> 1, else pow(2, val)+1
			BIT4		AE window weight mode, 1: Smaller Y range is used
			BIT5		AE block weight mode, 1: large center window
			BIT6-7		AE weight mode,00: Bweight(Block weight) mode only, 
			01: Wweight(Windows weight) mode only, 10: BWeight + Wweight
			11: weightless mode
			BIT8		AWB function enable
			BIT9		AWB reverse mode
			BIT10		AWB windows control, 1: 1 center window used only
			BIT11-15	reserve
			</TABLE>
  Remarks:
*********************************************************************************/
void V558_IspSetAfctrl(UINT16 x)
{
	V558_SetReg(V558_REG_ISP_AFCTRL, (UINT8)x);
	V558_SetReg(V558_REG_ISP_AFCTRL+1, (UINT8)(x >> 8));
}

/********************************************************************************
  Description:
	get auto function control
  Returns:
  return auto function control setting
  Note:
			<TABLE>
			value		meaning
			-----		-------
			BIT0		Auto-flicker function on	
			BIT1-3		auto function interrupt frequency: 
			if 0 -> 1, else pow(2, val)+1
			BIT4		AE window weight mode, 1: Smaller Y range is used
			BIT5		AE block weight mode, 1: large center window
			BIT6-7		AE weight mode,00: Bweight(Block weight) mode only, 
				01: Wweight(Windows weight) mode only, 10: BWeight + Wweight
				11: weightless mode
			BIT8		AWB function enable
			BIT9		AWB reverse mode
			BIT10		AWB windows control, 1: 1 center window used only
			BIT11-15	reserve
			</TABLE>
  Remarks:
*********************************************************************************/

UINT16 V558_IspGetAfctrl(void)
{
	UINT8 x1, x2;
	
	V558_GetReg(V558_REG_ISP_AFCTRL, &x1);
	V558_GetReg(V558_REG_ISP_AFCTRL+1, &x2);
	return ((UINT16)x2 << 8) | x1;
}


/********************************************************************************
  Description:
	enable auto function one window mode
  Remarks:
*********************************************************************************/
void V558_IspEnableAwbOnewin(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL+1, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL+1, x | 0x4);
}


/********************************************************************************
  Description:
	enable auto function nine window mode
  Remarks:
	state: valid
*********************************************************************************/
void V558_IspDisableAwbOnewin(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL+1, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL+1, x & 0xfb);
}

/********************************************************************************
  Description:
	enable AWB reverse mode
  Remarks:
*********************************************************************************/
void V558_IspEnableAwbReverse(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL+1, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL+1, x | 0x2);
}

/********************************************************************************
  Description:
	disable AWB reverse mode
  Remarks:
*********************************************************************************/
void V558_IspDisableAwbReverse(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL+1, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL+1, x & 0xfd);
}

/********************************************************************************
  Description:
	enable AWB function
  Remarks:
*********************************************************************************/
void V558_IspEnableAwb(void)
{
	UINT8	x;


	
	V558_GetReg(V558_REG_ISP_AFCTRL+1, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL+1, x | 0x1);
}

/********************************************************************************
  Description:
	disable AWB function
  Remarks:
*********************************************************************************/
void V558_IspDisableAwb(void)
{

	
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL+1, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL+1, x & 0xfe);
}


/********************************************************************************
  Description:
	set AE weight mode
  Parameters:
	mode:	AE weight mode setting
  Note:
			<TABLE>
			value		mode
			-----		----
			0			Bweight mode
			1			Wweight mode
			2			Bweight+Wweight mode
			3			Weightless mode
			</TABLE>
  Remarks:
*********************************************************************************/
void V558_IspSetAewmode(UINT8 mode)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL, (x & 0x3f) | ((mode << 6) & 0xc0));

}


/********************************************************************************
  Description:
	get AE weight mode setting
  Returns:
	return AE weight mode setting
  Note:
			  <TABLE>
			  value		mode
			  -----		----
			  0			Bweight mode
			  1			Wweight mode
			  2			Bweight+Wweight mode
			  3			Weightless mode
			  </TABLE>
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetAewmode(void)
{
	UINT8	x;
	

	V558_GetReg(V558_REG_ISP_AFCTRL, &x);
	return x >> 6;
}

/********************************************************************************
  Description:
	Enable AE large center window
  Remarks:
*********************************************************************************/
void V558_IspEnableAeLargeWin(void)

{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL, x | 0x20);
}

/********************************************************************************
  Description:
	disable AE large center window
  Remarks:
*********************************************************************************/
void V558_IspDisableAeLargeWin(void)
{
	UINT8	x;
	

	V558_GetReg(V558_REG_ISP_AFCTRL, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL, x & 0xdf);
}

/********************************************************************************
  Description:
	Enable AE smaller luminance range
  Remarks:
*********************************************************************************/
void V558_IspEnableAeSmallYrange(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL, x | 0x10);
}

/********************************************************************************
  Description:
	Enable AE larger luminance range
  Remarks:
*********************************************************************************/
void V558_IspDisableAeSmallYrange(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL, x & 0xef);
}

/********************************************************************************
  Description:
	set auto function interrupt frequency
  Parameters:
	frq:	auto function interrupt frequency
  Note:
				<TABLE>
				value		mode
				-----		----
				0			Interrupt per 1 frame
				1			Interrupt per 3 frame
				2			Interrupt per 5 frame
				3			Interrupt per 9 frame
				4			Interrupt per 17 frame
				5			Interrupt per 33 frame
				6			Interrupt per 65 frame
				7			Interrupt per 129 frame
				</TABLE>
  Remarks:
*********************************************************************************/
void V558_IspSetAfIntfrq(UINT8 frq)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL, (x & 0xf1) | ((frq << 1) & 0xe));
}

/********************************************************************************
  Description:
	get auto function interrupt frequency
  Returns:
	return auto function interrupt frequency
  Note:
			<TABLE>
			value		mode
			-----		----
			0			Interrupt per 1 frame
			1			Interrupt per 3 frame
			2			Interrupt per 5 frame
			3			Interrupt per 9 frame
			4			Interrupt per 17 frame
			5			Interrupt per 33 frame
			6			Interrupt per 65 frame
			7			Interrupt per 129 frame
			</TABLE>
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetAfIntfrq(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL, &x);
	return (x >> 1) & 0x7;
}

/********************************************************************************
  Description:
	Enable anti-flicker funtion
  Remarks:
*********************************************************************************/
void V558_IspEnableAntiFlk(void)


{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL, &x);
	V558_SetReg(V558_REG_ISP_AFCTRL, x | 0x1);
}

/********************************************************************************
  Description:
	disable anti-flicker funtion
  Remarks:
*********************************************************************************/
void V558_IspDisableAntiFlk(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AFCTRL, &x);

	V558_SetReg(V558_REG_ISP_AFCTRL, x & 0xfe);
}

/********************************************************************************
  Description:
	set auto function window's size
  Parameters:
	size:	auto function window's size
  Note:
	\the default setting is: Ha = size.cx / 16, Hb = (size.cx * 8) / 16,
	Hc = (size.cx * 3) / 16
  Remarks:
*********************************************************************************/
void V558_IspSetAfWin(V558_SIZE size)

{

	V558_SetReg(V558_REG_ISP_AFWHA, (UINT8)(size.cx/(16*8)));
	V558_SetReg(V558_REG_ISP_AFWHB, (UINT8)(size.cx*8/(16*8)));
	V558_SetReg(V558_REG_ISP_AFWHC, (UINT8)(size.cx*3/(16*8)));

	V558_SetReg(V558_REG_ISP_AFWVA, (UINT8)(size.cy/(12*8)));
	V558_SetReg(V558_REG_ISP_AFWVB, (UINT8)(size.cy*6/(12*8)));
	V558_SetReg(V558_REG_ISP_AFWVC, (UINT8)(size.cy*2/(12*8)));
}

/********************************************************************************
  Description:
	set AE weight
  Parameters:
	x:	AE weithg
  Remarks:
*********************************************************************************/
void V558_IspSetAeWeight(UINT16	x)
{
	V558_SetReg(V558_REG_ISP_AEWF, (UINT8)x);
	V558_SetReg(V558_REG_ISP_AEWF, (UINT8)(x >> 8));
}

/********************************************************************************
  Description:
	set AWB calculating color range
  Parameters:
	x:	range parameters
  Note:
			 When calculating the average value of red, green and blue, not all 
			 the pixels in frame can contribute to the sum.  Only those pixels 
			 which satisfy the following condition are added together:
			 Ybot < Y < Ytop, I < Itop, Q < Qtop, In which Y, I Q is the luminance, 
			 inphase, quadratire of a pixel in YIQ color model
  Remarks:
			   
*********************************************************************************/
void V558_IspSetColorRange(PV558_AWBCLRRANGE x)
{
	V558_SetReg(V558_REG_ISP_YBOT, x->ybot);
	V558_SetReg(V558_REG_ISP_YTOP, x->ytop);
	V558_SetReg(V558_REG_ISP_QTOP, x->qtop);
	V558_SetReg(V558_REG_ISP_ITOP, x->itop);
}

/********************************************************************************
  Description:
	get AWB calculating color range
  Parameters:
	x:	return range parameters
  Note:
			When calculating the average value of red, green and blue, not all 
			the pixels in frame can contribute to the sum.  Only those pixels 
			which satisfy the following condition are added together:
			Ybot < Y < Ytop, I < Itop, Q < Qtop, In which Y, I Q is the luminance, 
			inphase, quadratire of a pixel in YIQ color model
  Remarks:
*********************************************************************************/
void V558_IspGetColorRange(V558_AWBCLRRANGE *x)
{
	V558_GetReg(V558_REG_ISP_YBOT, &(x->ybot));
	V558_GetReg(V558_REG_ISP_YTOP, &(x->ytop));

	V558_GetReg(V558_REG_ISP_QTOP, &(x->qtop));
	V558_GetReg(V558_REG_ISP_ITOP, &(x->itop));
}


/********************************************************************************
  Description:
	set AWB gain limit range
  Parameters:
	x:	range parameters
  Note:
			These registers define the threshold of the gain value when auto 
			white balance is enabled. The produced gain value is 
			GLOW <= GAIN <= GTOP
  Remarks:
*********************************************************************************/
void V558_IspSetAwbGainLmt(V558_AWBGAINLMT x)
{
	V558_SetReg(V558_REG_ISP_GBOT, x.glow);
	V558_SetReg(V558_REG_ISP_GTOP, x.gtop);
}


/********************************************************************************
  Description:
	get AWB gain limit range
  Parameters:
	x:	return range parameters setting
  Note:
		These registers define the threshold of the gain value when auto 
		white balance is enabled. The produced gain value is 
		GLOW <= GAIN <= GTOP
  Remarks:
*********************************************************************************/
void V558_IspGetAwbGainLmt(V558_AWBGAINLMT *x)
{

	V558_GetReg(V558_REG_ISP_GBOT, &(x->glow));
	V558_GetReg(V558_REG_ISP_GTOP, &(x->gtop));
}

/********************************************************************************
  Description:
	set AWB threshold
  Parameters:
	x:	threshold value
  Note:
		\Rdiff = | Rmean - Gmean |,		Bdiff = | Bmean - Gmean |
		\The special R gain adjustment step length is determined by the 
		relationship among Rdiff and AWBTHD1 AWBTHD2 as below , B gain 
		adjustment step length is in the same way .
				<TABLE>
				value		meaning
				-----		-------
				BIT0-5		AWBSTEP
				BIT6-9		AWBTHD1
				BIT10-15	AWBTHD2
				</TABLE>
  Remarks:
*********************************************************************************/
void V558_IspSetAwbThreshold(UINT16 x)
{
	V558_SetReg(V558_REG_ISP_AWBCTRL, (UINT8)x);
	V558_SetReg(V558_REG_ISP_AWBCTRL+1, (UINT8)(x >> 8));
}

/********************************************************************************
  Description:
	set AWB threshold
  Returns:
	return AWB threshold setting
  Note:
			\Rdiff = | Rmean - Gmean |,		Bdiff = | Bmean - Gmean |
			\The special R gain adjustment step length is determined by the 
			relationship among Rdiff and AWBTHD1 AWBTHD2 as below , B gain 
			adjustment step length is in the same way .
				<TABLE>
				value		meaning
				-----		-------
				BIT0-5		AWBSTEP
				BIT6-9		AWBTHD1
				BIT10-15	AWBTHD2
				</TABLE>
  Remarks:
*********************************************************************************/
UINT16 V558_IspGetAwbThreshold(void)
{
	UINT8 x1, x2;
	
	V558_GetReg(V558_REG_ISP_AWBCTRL, &x1);
	V558_GetReg(V558_REG_ISP_AWBCTRL+1, &x2);

	
	return ((UINT16)x2 << 8) | x1;
	
}

/********************************************************************************
  Description:
	get Rmean
  Returns:
	return Rmean
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetRMean(void)
{
	UINT8	x;

	
	V558_GetReg(V558_REG_ISP_RMEAN, &x);
	return x;
}

/********************************************************************************
  Description:
	get Gmean
  Returns:
	return Gmean
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetGMean(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_GMEAN, &x);
	return x;

}

/********************************************************************************
  Description:
	get Bmean
  Returns:
	return Bmean
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetBMean(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_BMEAN, &x);
	return x;
}

/********************************************************************************
  Description:
	get Ymean
  Returns:
	return Ymean
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetYMean(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_YMEAN, &x);
	return x;
}

/********************************************************************************
  Description:
	get auto function's status
  Returns:
	return auto function's status
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetAfStatus(void)
{
	UINT8	x;
	
	V558_GetReg(V558_REG_ISP_AUTO_STATUS, &x);
	
	return x;
}

/********************************************************************************
  Description:
	set DPD threshold
  Parameters:
	x:	DPD threshold setting
  Remarks:
*********************************************************************************/
void V558_IspSetDpdThreshold(UINT8 x)
{
	V558_SetReg(V558_REG_ISP_DPCTHD, x);
}

/********************************************************************************
  Description:
	get DPD threshold setting
  Returns:
	return DPD threshold setting
  Remarks:
*********************************************************************************/
UINT8 V558_IspGetDpdThreshold(void)
{
	UINT8 x;
	
	V558_GetReg(V558_REG_ISP_DPCTHD, &x);
	return x;
}

/********************************************************************************
  Description:
	set noise table
  Parameters:
	x:	noise table
  Remarks:
*********************************************************************************/
void V558_IspSetNoiseTable(UINT8 *x)
{
	UINT16 i;
	
	for(i = 0; i < 17; i++)
		V558_SetReg(V558_REG_ISP_NT1+i, x[i]);
}

/********************************************************************************
  Description:
	get noise table
  Parameters:
	x:	noise table
  Remarks:
*********************************************************************************/
void V558_IspGetNoiseTable(UINT8 *x)
{
	UINT16 i;
	
	for(i = 0; i < 17; i++)
		V558_GetReg(V558_REG_ISP_NT1+i, x+i);
}





//================================================
//
// Isp property functions 
//
//=====================================

/********************************************************************************
  Description:
	Get current setting for isp brightness
  Parameters:
	lValue:	brightness range [MIN_BRIGHT_CONTRAST, MAX_BRIGHT_CONTRAST]
  Returns:
  Note:
  Remarks:
*********************************************************************************/
void V558_IspGetBrightness(UINT8 *lValue)
{
	*lValue = gSnrContext.pInfo->disp.prp.brightness;
}

/********************************************************************************
  Description:
	Set isp brightness
  Parameters:
	lValue:	brightness range [MIN_BRIGHT_CONTRAST, MAX_BRIGHT_CONTRAST]
  Returns:
  Note:
  Remarks:
*********************************************************************************/
void V558_IspSetBrightness(UINT8 lValue)
{
	SINT8 sBrightness;
	SINT8 sContrast;

	
	UINT8 RegGamma[17];
	
	if(lValue > MAX_BRIGHT_CONTRAST)
	{
		lValue = MAX_BRIGHT_CONTRAST;
	}
	if(lValue < MIN_BRIGHT_CONTRAST)
	{
		lValue =MIN_BRIGHT_CONTRAST;
	}

	gSnrContext.pInfo->disp.prp.brightness=lValue;

	sBrightness = (SINT8)gSnrContext.pInfo->disp.prp.brightness - 32;
	sContrast = (SINT8)gSnrContext.pInfo->disp.prp.contrast - 32;

	CalculateNewGammaCurve(gSnrContext.pInfo->disp.gammaRGB, sContrast, sBrightness, RegGamma);

	V558_IspSetRGammaStart(RegGamma);
	V558_IspSetGGammaStart(RegGamma);
	V558_IspSetBGammaStart(RegGamma);
}

/********************************************************************************
  Description:
	Get current setting for isp contrast
  Parameters:
	lValue:	contrast range [MIN_BRIGHT_CONTRAST, MAX_BRIGHT_CONTRAST]
  Returns:
  Note:
  Remarks:
*********************************************************************************/
void V558_IspGetContrast(UINT8 *lValue) 
{
	*lValue = gSnrContext.pInfo->disp.prp.contrast;
}

/********************************************************************************
  Description:
	Set isp contrast
  Parameters:
	lValue:	contrast range [MIN_BRIGHT_CONTRAST, MAX_BRIGHT_CONTRAST]
  Returns:
  Note:
  Remarks:
*********************************************************************************/
void V558_IspSetContrast(UINT8 lValue)
{
	SINT8 sBrightness;
	SINT8 sContrast;

	

	UINT8 RegGamma[17];
	
	if(lValue > MAX_BRIGHT_CONTRAST)
	{
		lValue = MAX_BRIGHT_CONTRAST;
	}
	if(lValue < MIN_BRIGHT_CONTRAST)
	{
		lValue =MIN_BRIGHT_CONTRAST;
	}
	gSnrContext.pInfo->disp.prp.contrast=lValue;


	sBrightness = (SINT8)gSnrContext.pInfo->disp.prp.brightness - 32;
	sContrast = (SINT8)gSnrContext.pInfo->disp.prp.contrast - 32;

	CalculateNewGammaCurve(gSnrContext.pInfo->disp.gammaRGB, sContrast, sBrightness, RegGamma);

	V558_IspSetRGammaStart(RegGamma);

	V558_IspSetGGammaStart(RegGamma);
	V558_IspSetBGammaStart(RegGamma);
}


//==============================================================================//
//	static function of image adjusting math methord                         //
//==============================================================================//
void CalculateNewGammaCurve(UINT8 * OriginalGammaCurve, 
							SINT8 DeltaContrast, 
							SINT8 DeltaBrightness, 
							UINT8 * NewGammaCurve )
{

	SINT16 deltaGma[17] ;
	SINT16 steps = DeltaContrast ;
	SINT16 i, temp ;

	// step 1: Calculate deltaGma[] based on DeltaContrast
	// for simplicity, all the division is based on 64(can be done by >>8 )
	deltaGma[0] = 0 ;
	deltaGma[1] = - 96 * steps / 64 ;
	deltaGma[3] = - 72 * steps / 64 ;
	deltaGma[6] = 0 ;
	deltaGma[11] = 146 * steps / 64 ;
	deltaGma[16] = 166 * steps / 64 ;
	
	deltaGma[2] = ( deltaGma[1] + deltaGma[3] ) / 2 ;
	
	deltaGma[4] = - 72 * (6-4) * steps / (6-3) / 64 ;
	deltaGma[5] = - 72 * (6-5) * steps / (6-3) / 64 ;
	
	deltaGma[7] =  146 * (7-6) * steps / (11-6) / 64 ;
	deltaGma[8] =  146 * (8-6) * steps / (11-6) / 64 ;
	deltaGma[9] =  146 * (9-6) * steps / (11-6) / 64 ;
	deltaGma[10] = 146 * (10-6) * steps / (11-6) / 64 ;
	
	deltaGma[12] = deltaGma[11] + (deltaGma[16]-deltaGma[11]) * (12-11) / (16-11) ;
	deltaGma[13] = deltaGma[11] + (deltaGma[16]-deltaGma[11]) * (13-11) / (16-11) ;
	deltaGma[14] = deltaGma[11] + (deltaGma[16]-deltaGma[11]) * (14-11) / (16-11) ;
	deltaGma[15] = deltaGma[11] + (deltaGma[16]-deltaGma[11]) * (15-11) / (16-11) ;
	
	// step 2: update deltaGma[] based on DeltaBrightness
	for ( i = 0 ; i < 17; i ++ )
		deltaGma[i] += DeltaBrightness * 5 ;
	
	// step 3: Calculate NewGammaCurve
	for ( i = 0 ; i < 17; i ++ )



	{
		temp = OriginalGammaCurve[i] + deltaGma[i] ;
		if ( temp < 0 ) 
			temp = 0 ;
		else 
			if ( temp > 255 )
				temp = 255 ;  
			NewGammaCurve[i] = temp ;
	}
}
UINT32 PowTwo(UINT32 x, UINT16 y, UINT8 denominator, UINT8 numerator)
{
	UINT16 i;

	for(i = 0; i < y; i++)
	{
		x *= numerator;
		x /= denominator;
	}

	return x;
}
