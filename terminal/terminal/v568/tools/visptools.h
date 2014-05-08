#ifndef _VISPTOOLS_H_
#define _VISPTOOLS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../driver/basefunc/v5btype.h"

#define AE_M				256
#ifndef ET_ONE_STEP
#define ET_ONE_STEP	1000
#endif

#define ET_GAINRANGE_STEP	4
#define ET_GAINRANGE_OVERLAY	2

enum AEADJUSTRET
{
	GAIN_ONLY = 0x0,
	GAIN_ET = 0x1,
	UN_ET_GAIN = 0x2
};

enum AE_SPEED
{
	SPEED_SLOW	= 0x0,
	SPEED_NORMAL =	0x1,
	SPEED_FAST =	0x2,
	SPEED_FASTEST =	0x3
};

enum AE_FOMULA_FLAG
{
	AE_GET_ET,
	AE_GET_GAIN
};

/* computer max divisor */
UINT16	MaxDivisor(UINT16 a, UINT16 b);

void	CalculateNewGammaCurve(UINT8 * OriginalGammaCurve, SINT16 DeltaContrast, SINT16 DeltaBrightness, UINT8 *NewGammaCurve);
UINT32	PowTwo(UINT32 x, UINT16 y, UINT8 denominator, UINT8 numerator);

UINT16	vsqrt(UINT32 M);		//square root
UINT32	Vabs(SINT32 x);

void	VComputerParm(TSize src, UINT16 width, TSize *dst, TV5BIppFactor *fct);
void	VComputerParmH(TSize src, UINT16 height, TSize *dst, TV5BIppFactor *fct);
void	VComputerSizeByWidth(TSize src, UINT16 width, TSize *dst);
void	VComputerSizeByHeight(TSize src, UINT16 height, TSize *dst);

UINT8	VComputeEt(PTAeAdjust ae);
void VComputeGainThd(UINT8 gain, UINT8 gainrange);

/*********************************************************************
*                    Tools For VC0578 IPP Sizer                      *
*********************************************************************/

void	VCalcSizerUpHPara(TSize ssize, UINT16 width, PTSize pdsize, PTV5BIppSzUpFct pfct);
void	VCalcSizerUpVPara(TSize ssize, UINT16 height, PTSize pdsize, PTV5BIppSzUpFct pfct);
void	VCalcSizerDownHPara(TSize ssize, UINT16 width, PTSize pdsize, PTV5BIppSzDnFct pfct);
void	VCalcSizerDownVPara(TSize ssize, UINT16 height, PTSize pdsize, PTV5BIppSzDnFct pfct);

#ifdef __cplusplus
}
#endif

#endif



