#ifndef _RDK_ISP_H_
#define _RDK_ISP_H_

#ifdef __cplusplus
extern "C" {
#endif


//ISP edge enhancement parameter
typedef struct tag_TV5IspEdgeParm
{
	UINT8	femm;
	UINT8	femp;
	UINT8	femx1;
	UINT8	femx2;
	UINT8	femmin;
	UINT8	femmax;
} TV5IspEdgeParm, *PTV5IspEdgeParm;

//ISP lens fall off parameter
typedef struct tag_TV5IspLensFalloff
{
	UINT16	lfcenx;
	UINT16	lfceny;
	UINT16	lffrec;
} TV5IspLensFalloff, *PTV5IspLensFalloff;

typedef struct tag_TV5IspColorCvt
{
	UINT8	cradj;
	UINT8	cbadj;
} TV5IspColorCvt, *PTV5IspColorCvt;

typedef struct tag_TV5IspGrayImg
{
	UINT8	croft;
	UINT8	cboft;
} TV5IspGrayImg, *PTV5IspGrayImg;

//ISP color correction matrix
typedef struct tag_TV5IspColorMatrix
{
	UINT8	cmat11, cmat12, cmat13;
	UINT8	cmat21, cmat22, cmat23;
	UINT8	cmat31, cmat32, cmat33;
	UINT16	ofsr;
	UINT16	ofsg;
	UINT16	ofsb;
} TV5IspColorMatrix, *PTV5IspColorMatrix;

//ISP AWB color range struct
typedef struct tag_TV5IspAwbClrRange
{
	UINT8	ybot;
	UINT8	ytop;
	UINT8	qtop;
	UINT8	itop;
} TV5IspAwbClrRange, *PTV5IspAwbClrRange;

//ISP AWB gain limit struct
typedef struct tag_TV5IspAwbGainLmt
{
	UINT8	glow;
	UINT8	gtop;
} TV5IspAwbGainLmt, *PTV5IspAwbGainLmt;

//AE batch parameter
typedef struct tag_TI2cAeParm
{
	UINT8	chipadr;
	UINT8	len;
	TI2cBatch *pbatch;
} TI2cAeParm, *PTI2cAeParm;

typedef struct tag_TV5IspHistogramResult
{
	UINT32 result0;
	UINT32 result1;
	UINT32 result2;
	UINT32 result3;
	UINT32 result4;
	UINT32 result5;
	UINT32 result6;
	UINT32 result7;
	UINT32 result8;
	UINT32 result9;
	UINT32 result10;
	UINT32 result11;
	UINT32 resultc0;
	UINT32 resultc4;
	UINT32 resultc8;
	UINT32 resultcc;
	UINT32 resultd0;
	UINT32 resultd4;
	UINT32 resultd8;
	UINT32 resultdc;
	UINT32 resulte0;
	UINT32 resulte4;
	UINT32 resulte8;
	UINT32 resultec;
	UINT32 resultf0;
	UINT32 resultf4;
	UINT32 resultf8;
	UINT32 resultfc;
} TV5IspHistogramResult, *PTV5IspHistogramResult;

enum V5B_ISPHISTOGRAM_SELECT
{
	HISTOGRAM_SELECT_Y = 0x0,
	HISTOGRAM_SELECT_CB = 0x1,
	HISTOGRAM_SELECT_CR = 0x2
};


void	V5B_IspSetBasctl(UINT16 x);
UINT16	V5B_IspGetBasctl(void);
void	V5B_IspSetPixelRate(UINT8 rate);
UINT8	V5B_IspGetPixelRate(void);
void	V5B_IspEnableGFirst(void);
void	V5B_IspDisableGFirst(void);
void	V5B_IspEnableBgFirst(void);
void	V5B_IspDisableBgFirst(void);
void	V5B_IspEnableYuvFmt(void);
void	V5B_IspDisableYuvFmt(void);
void	V5B_IspEnableHPF(void);
void	V5B_IspEnableLPF(void);
void	V5B_IspEnableEdgeEnh(void);
void	V5B_IspDisableEdgeEnh(void);
void	V5B_IspEnableLensOff(void);
void	V5B_IspDisableLensOff(void);
void	V5B_IspEnableDpc(void);
void	V5B_IspDisableDpc(void);
void	V5B_IspEnableRgbGamma(void);
void	V5B_IspDisableRgbGamma(void);
void	V5B_IspEnableColorMatrix(void);
void	V5B_IspDisableColorMatrix(void);
void	V5B_IspEnableYGamma(void);
void	V5B_IspDisableYGamma(void);
void	V5B_IspEnableRgbGain(void);
void	V5B_IspDisableRgbGain(void);
void	V5B_IspEnableGrayImage(void);
void	V5B_IspDisableGrayImage(void);

void	V5B_IspSetTestBarWin(TSize size);
void	V5B_IspGetTestBarWin(TSize *size);
void	V5B_IspSetImageSize(TSize size);
void	V5B_IspGetImageSize(TSize *size);

void	V5B_IspSetEdgeParm(TV5IspEdgeParm edge);
void	V5B_IspGetEdgeParm(PTV5IspEdgeParm pedge);
void	V5B_IspSetLensParm(TV5IspLensFalloff lens);
void	V5B_IspGetLensParm(PTV5IspLensFalloff plens);

void	V5B_IspSetAfLnCtl(UINT16 x);
UINT16	V5B_IspGetAfLnCtl(void);
void	V5B_IspSetAfLnSt(UINT16 val);
UINT16	V5B_IspGetAfLnSt(void);
void	V5B_IspSetAfLnCnt(UINT8 cnt);
UINT8	V5B_IspGetAfLnCnt(void);
void	V5B_IspEnableAfLnMode(void);
void	V5B_IspDisableAfLnMode(void);
UINT32	V5B_IspGetAfSta(void);
void	V5B_IspSetAfLnStep(UINT8 step);
UINT8	V5B_IspGetAfLnStep(void);

void	V5B_IspSetTailBlank(UINT32 x);
UINT32	V5B_IspGetTailBlank(void);

void	V5B_IspSetRgain(UINT8 x);
UINT8	V5B_IspGetRgain(void);
void	V5B_IspSetGgain(UINT8 x);
UINT8	V5B_IspGetGgain(void);
void	V5B_IspSetBgain(UINT8 x);
UINT8	V5B_IspGetBgain(void);

void	V5B_IspEnableVsyncReset(void);
void	V5B_IspDisableVsyncReset(void);
UINT8	V5B_IspIsVsyncResetDisable(void);

void	V5B_IspSetGlbGain(UINT8 x);
UINT8	V5B_IspGetGlbGain(void);
void	V5B_IspSetGlbGainCtrl(UINT8 x);
UINT8	V5B_IspGetGlbGainCtrl(void);
void	V5B_IspEnableGlbGain(void);
void	V5B_IspDisableGlbGain(void);
void	V5B_IspSetGlbGainDelay(UINT8 val);
UINT8	V5B_IspGetGlbGainDelay(void);
void	V5B_IspSetGlbGainStep(UINT8 val);
UINT8	V5B_IspGetGlbGainStep(void);

void	V5B_IspSetColorCvt(TV5IspColorCvt val);
void	V5B_IspGetColorCvt(PTV5IspColorCvt pval);
void	V5B_IspSetGrayImg(TV5IspGrayImg val);
void	V5B_IspGetGrayImg(PTV5IspGrayImg pval);
void	V5B_IspSetColorMatrix(TV5IspColorMatrix mtx);
void	V5B_IspGetColorMatrix(PTV5IspColorMatrix pmtx);

void	V5B_IspSetYGammaStart(UINT8 *x);
void	V5B_IspGetYGammaStart(UINT8 *x);
void	V5B_IspSetYGammaSlope(UINT8 *x);
void	V5B_IspGetYGammaSlope(UINT8 *x);
void	V5B_IspSetRGammaStart(UINT8 *x);
void	V5B_IspGetRGammaStart(UINT8 *x);
void	V5B_IspSetGGammaStart(UINT8 *x);
void	V5B_IspGetGGammaStart(UINT8 *x);
void	V5B_IspSetBGammaStart(UINT8 *x);
void	V5B_IspGetBGammaStart(UINT8 *x);

void	V5B_IspSetAfCtrl(UINT16 x);
UINT16	V5B_IspGetAfCtrl(void);
void	V5B_IspEnableAntiFlk(void);
void	V5B_IspDisableAntiFlk(void);
UINT8	V5B_IspIsAntiFlk(void);
void	V5B_IspSetAfIntFrq(UINT8 frq);
UINT8	V5B_IspGetAfIntFrq(void);
void	V5B_IspEnableAeSmallYrange(void);
void	V5B_IspEnableAeLargeYrange(void);
void	V5B_IspSetAeWeightMode(UINT8 mode);
UINT8	V5B_IspGetAeWeightMode(void);
void	V5B_IspEnableAwb(void);
void	V5B_IspDisableAwb(void);
UINT8	V5B_IspIsAwb(void);
void	V5B_IspEnableAwbRev(void);
void	V5B_IspDisableAwbRev(void);
void	V5B_IspEnableAwbOneWin(void);
void	V5B_IspDisableAwbOneWin(void);
void	V5B_IspEnableNoiseRmv(void);
void	V5B_IspDisableNoiseRmv(void);
void	V5B_IspEnableUvNoiseRmv(void);
void	V5B_IspDisableUvNoiseRmv(void);
void	V5B_IspEnableAutoFocus(void);
void	V5B_IspDisableAutoFocus(void);
void	V5B_IspSetEdgeMatrix(UINT8 sel);
UINT8	V5B_IspGetEdgeMatrix(void);

void	V5B_IspSetCtrl1(UINT8 x);
UINT8	V5B_IspGetCtrl1(void);
void	V5B_IspEnableHomoImg(void);
void	V5B_IspDisableHomoImg(void);
void	V5B_IspSetFilterMode(UINT8 sel);
UINT8	V5B_IspGetFilterMode(void);
void	V5B_IspSetTestBarPtn(UINT8 sel);
UINT8	V5B_IspGetTestBarPtn(void);
void	V5B_IspEnableTestBar(void);
void	V5B_IspDisableTestBar(void);

void	V5B_IspSetCtrlUpdate(UINT8 x);
void	V5B_IspUpdateGlbGain(void);
void	V5B_IspUpdateDigitalGain(void);
void	V5B_IspUpdateBGamma(void);
void	V5B_IspUpdateGGamma(void);
void	V5B_IspUpdateRGamma(void);
void	V5B_IspUpdateColMat(void);
void	V5B_IspUpdateGray(void);
void	V5B_IspUpdateFem(void);

void	V5B_IspSetColorRange(TV5IspAwbClrRange x);
void	V5B_IspGetColorRange(PTV5IspAwbClrRange px);
void	V5B_IspSetAwbGainLmt(TV5IspAwbGainLmt x);
void	V5B_IspGetAwbGainLmt(PTV5IspAwbGainLmt px);

void	V5B_IspSetAwbCtrl(UINT16 x);
UINT16	V5B_IspGetAwbCtrl(void);
void	V5B_IspSetAwbStep(UINT8 val);
UINT8	V5B_IspGetAwbStep(void);
void	V5B_IspSetAwbThd1(UINT8 val);
UINT8	V5B_IspGetAwbThd1(void);
void	V5B_IspSetAwbThd2(UINT8 val);
UINT8	V5B_IspGetAwbThd2(void);

UINT8	V5B_IspGetRMean(void);
UINT8	V5B_IspGetGMean(void);
UINT8	V5B_IspGetBMean(void);
UINT8	V5B_IspGetYMean(void);

UINT8	V5B_IspGetAfStatus(void);

void	V5B_IspSetDpdThd(UINT8 x);
UINT8	V5B_IspGetDpdThd(void);
void	V5B_IspSetNoiseTable(UINT8 *x);
void	V5B_IspGetNoiseTable(UINT8 *x);
void	V5B_IspSetDpdThdUV(UINT8 sel);
UINT8	V5B_IspGetDpdThdUV(void);
void	V5B_IspSetNoiseTableUV(UINT8 *x);
void	V5B_IspGetNoiseTableUV(UINT8 *x);
void	V5B_IspSetNormalChromaNoiseTableUV(void);
void	V5B_IspSetLowChromaNoiseTableUV(void);

void	V5B_IspSetBLROffset(UINT16 x);
UINT16	V5B_IspGetBLROffset(void);
void	V5B_IspSetBLBOffset(UINT16 x);
UINT16	V5B_IspGetBLBOffset(void);
void	V5B_IspSetBLG1Offset(UINT16 x);
UINT16	V5B_IspGetBLG1Offset(void);
void	V5B_IspSetBLG2Offset(UINT16 x);
UINT16	V5B_IspGetBLG2Offset(void);

UINT32	V5B_IspGetEdgeSum(void);

void	V5B_IspSetAHStart(UINT8 *x);
void	V5B_IspGetAHStart(UINT8 *x);
void	V5B_IspSetAVStart(UINT8 *x);
void	V5B_IspGetAVStart(UINT8 *x);

void	V5B_IspSetHistCtrl(UINT8 x);
UINT8	V5B_IspGetHistCtrl(void);
void	V5B_IspEnableHistEffect(void);
void	V5B_IspDisableHistEffect(void);
void	V5B_IspEnableHistManual(void);
void	V5B_IspDisableHistManual(void);
void	V5B_IspSetHistGain(UINT8 x);
UINT8	V5B_IspGetHistGain(void);

void	V5B_IspSetYMeanTarget(UINT8 x);
UINT8	V5B_IspGetYMeanTarget(void);
void	V5B_IspSetYMeanThd(UINT8 x);
UINT8	V5B_IspGetYMeanThd(void);

UINT8	V5B_IspGetAfInt(void);
void	V5B_IspClrAFocusInt(void);
void	V5B_IspClrAFlickerInt(void);
void	V5B_IspClrYMeanInt(void);

void	V5B_IspSetAeWeight(UINT8 *x);
void	V5B_IspGetAeWeight(UINT8 *x);

void	V5B_IspGetWinYMean(UINT8 *x);

void	V5B_IspEnableHue(void);
void	V5B_IspDisableHue(void);
UINT8	V5B_IspIsHueEnable(void);
void	V5B_IspSetHueCos(int x);
int		V5B_IspGetHueCos(void);
void	V5B_IspSetHueSin(int x);
int		V5B_IspGetHueSin(void);
void	V5B_IspSetSat(UINT8 x);
UINT8	V5B_IspGetSat(void);
void	V5B_IspSetHueYOff(int x);
int		V5B_IspGetHueYOff(void);
void	V5B_IspSetHueUOff(int x);
int		V5B_IspGetHueUOff(void);
void	V5B_IspSetHueVOff(int x);
int		V5B_IspGetHueVOff(void);

void	V5B_IspModuleReset(void);
void	V5B_IspRegReset(void);

#ifdef __cplusplus
}
#endif

#endif
