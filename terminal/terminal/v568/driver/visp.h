#ifndef _RDK_VISP_H_
#define _RDK_VISP_H_

#ifdef __cplusplus
extern "C" {
#endif

//#ifdef WIN32
//#ifndef _DEBUG_ISP_AE
//#define	_DEBUG_ISP_AE
//#endif
//#endif

#ifdef _DEBUG_ISP_AE
extern FILE *fp;
#endif

#define EPFREQ_50HZ				100000
#define EPFREQ_60HZ				120000
#define ET_ONE_STEP				1000
#define ET_DEFAULT				2000

#define MAX_FLK_ET				25			//auto flicker 

#define FLK_THRESHOLD			0x5000
#define CLOCK_MUL				1000

#define IPP_CSIZER_HIN_MAX		0x400
#define IPP_CSIZER_VIN_MAX		0x1000
#define IPP_CSIZER_HOUT_MAX		0x800
#define IPP_CSIZER_VOUT_MAX		0x1000

#define IPP_TSIZER_HOUT_MIN		0x28
#define IPP_TSIZER_VOUT_MIN		0x1e
#define IPP_TSIZER_HOUT_MAX		0xf0
#define IPP_TSIZER_VOUT_MAX		0x100

enum ADJUSTCOLORenum
{
	BRIGHTNESS = 0,
	CONTRAST   = 0x01
};

enum LIGHTTYPEenum
{
	LIGHT_IN  = 0,
	LIGHT_OUT = 0x01
};

enum SENSORCOMTYPEenum
{
	SENSOR_I2C,
	SENSOR_OVI2C,
	SENSOR_SERIAL
};

enum SENSORFORMATenum
{
	SENSORRGBFORMAT = 0,
	SENSORYUVFORMAT = 0x01
};

enum VISP_SIF_INT_TYPEenum
{
	INT_SIF_VSYNC		= 0,
	INT_SIF_FRAMEEND	= 0x01,
	INT_SIF_ALL			= V5_SIF_INT_NUM
};


/*********************************************************************
*                            SIF Module                              *
*********************************************************************/

void	V5_VispSifIntSetCallback(UINT8 index, PVispIsrCallback pcall);
void	V5_VispSifIntEnable(UINT8 index);
void	V5_VispSifIntDisable(UINT8 index);
void	V5_VispSetSensorRegList(PTRegGroup parm);
void	V5_VispResetSensor(void);
void	V5_VispPoweronSensor(void);
void	V5_VispPoweroffSensor(void);
UINT16	V5_VispSetSnrState(UINT8 state);
UINT8	V5_VispGetSnrState(void);
void	V5_VispSetI2cPin(UINT8 mode);
void	V5_VispGetSnrReg(UINT8 uAddr, UINT16 *uVal);
void	V5_VispSetSnrMirrorFlip(UINT8 val);

/*********************************************************************
*                            ISP Module                              *
*********************************************************************/

void	V5_VispIspIntSetCallback(UINT8 index, PVispIsrCallback pcall);
void	_ISR_VispIspIntHandle(UINT16 flag);
void	V5_VispIspIntEnable(UINT8 index);
void	V5_VispIspIntDisable(UINT8 index);

/*********************************************************************
*                            IPP Module                              *
*********************************************************************/

void	V5_IppSetDispSizerByWidth(UINT16 width);
void	V5_IppSetDispSizerByWidthEx1(TPoint winpt, TSize winsize, UINT16 width);
void	V5_IppSetDispSizerByWidthEx2(TSize imgsize, TPoint winpt, TSize winsize, UINT16 width);
void	V5_IppSetDispSizer(TSize size);
void	V5_IppSetDispSizerEx1(TPoint winpt, TSize winsize, TSize dsize);
void	V5_IppSetDispSizerEx2(TSize imgsize, TPoint winpt, TSize winsize, TSize dsize);
void	V5_IppSetCapSizerByWidth(UINT16 width);
void	V5_IppSetCapSizerByWidthEx1(TPoint winpt, TSize winsize, UINT16 width);
void	V5_IppSetCapSizerByWidthEx2(TSize imgsize, TPoint winpt, TSize winsize, UINT16 width);
void	V5_IppSetCapSizer(TSize size);
void	V5_IppSetCapSizerEx1(TPoint winpt, TSize winsize, TSize csizer);
void	V5_IppSetCapSizerEx2(TSize imgsize, TPoint winpt, TSize winsize, TSize csize);
void	V5_IppSetThumbSizerByWidth(UINT8 width);
void	V5_IppThumbSizerUpdateByWidth(void);
void	V5_IppSetThumbSizer(TSize size);
void	V5_IppThumbSizerUpdate(void);
void	V5_IppSetDataSrc(UINT8 sel);

/*********************************************************************
*                               VISP                                 *
*********************************************************************/

void	V5_VispInitContext(void);
PTVispContext V5_VispGetContext(void);
void	V5_VispSetSensorInfo(void);
void	V5_VispSetMode(UINT8 mode);
UINT8	V5_VispGetMode(void);
UINT8	V5_VispOpen(void);
void	V5_VispClose(void);
void	V5_VispSetDefaultParm(void);
void	V5_VispGetHistStat(PTHistStat pdata);

/*********************************************************************
*                       Brightness & Contrast                        *
*********************************************************************/

void	V5_VispSetBrightness(UINT8 val);
UINT8	V5_VispGetBrightness(void);
void	V5_VispSetContrast(UINT8 val);
UINT8	V5_VispGetContrast(void);
void	V5_VispSetSaturation(UINT8 val);
UINT8	V5_VispGetSaturation(void);

/*********************************************************************
*                        Global Size Adjust                          *
*********************************************************************/

void	V5_VispSetDispSizerByWidth(UINT8 index, TPoint winpt, TSize winsize, UINT16 width);
void	V5_VispSetDispSizer(UINT8 index, TPoint winpt, TSize winsize, TSize dsize);
void	V5_VispSetCapSizerByWidth(UINT8 index, TPoint winpt, TSize winsize, UINT16 width);
void	V5_VispSetCapSizer(UINT8 index, TPoint winpt, TSize winsize, TSize csize);

/*********************************************************************
*                            Image Size                              *
*********************************************************************/

void	V5_VispSetImageIndex(UINT8 index);
UINT8	V5_VispGetImageIndex(void);
UINT8	V5_VispGetImageIndexBySize(TSize size);
UINT8	V5_VispGetImageIndexByWidth(UINT16 width);
void	V5_VispGetSize(UINT8 index, TSize *size);
UINT8	V5_VispGetCurrentSize(TSize *size);

/*********************************************************************
*                            Auto Focus                              *
*********************************************************************/

void	V5_VispStartAutoFocus(void);
void	V5_VispStopAutoFocus(void);

/*********************************************************************
*                                 AE                                 *
*********************************************************************/

void	V5_VispSetYTarget(UINT8 val);
UINT8	V5_VispGetYTarget(void);
void	V5_VispSetYThd(UINT8 val);
UINT8	V5_VispGetYThd(void);
void	V5_VispSetYTop(UINT8 val);
UINT8	V5_VispGetYTop(void);
UINT8	V5_VispGetYTopDefault(void);
void	V5_VispSetMaxEt(UINT8 et);
UINT8	V5_VispGetMaxEt(void);
UINT8	V5_VispGetMaxEtDefault(void);
void	V5_VispSetMinEt(UINT8 et);
UINT8	V5_VispGetMinEt(void);
void	V5_VispSetMaxGain(UINT8 gain);
UINT8	V5_VispGetMaxGain(void);
void	V5_VispSetMinGain(UINT8 gain);
UINT8	V5_VispGetMinGain(void);

void	V5_VispSetAeSpeed(UINT8 speed);
UINT8	V5_VispGetAeSpeed(void);
void	V5_VispSetCurEt(UINT32 et);
UINT32	V5_VispGetCurEt(void);
UINT8	V5_VispGetCurGain(void);

void	V5_VispEnableAe(void);
void	V5_VispDisableAe(void);
void	V5_VispEnableAeQuery(void);
void	V5_VispDisableAeQuery(void);
UINT8	V5_VispIsAe(void);
void	V5_VispAutoExpose(void);
void	V5_VispSetEt(void);

/*********************************************************************
*                            Anti-Flicker                            *
*********************************************************************/

void	V5_VispSetEnv(UINT8 env);
void	V5_VispJudgeEnv(void);
UINT8	V5_VispGetEnv(void);
void	V5_VispDoFlk(void);

/*********************************************************************
*                            Flash Light                             *
*********************************************************************/

void	V5_VispSetFlashYThd(UINT8 x);
UINT8	V5_VispGetFlashYThd(void);
void	V5_VispSetFlashEt(UINT8 x);
UINT8	V5_VispGetFlashEt(void);
void	V5_VispEnableFlash(void);
void	V5_VispDisableFlash(void);

void	V5_VispStartFlashCharge(void);
void	V5_VispStopFlashCharge(void);
void	V5_VispFlashLight(void);
void	V5_VispStopFlashLight(void);

#ifdef __cplusplus
}
#endif

#endif/* _RDK_VISP_H_ */
