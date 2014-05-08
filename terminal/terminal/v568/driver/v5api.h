
#ifndef _RDK_LAYER2_V5API_H_
#define _RDK_LAYER2_V5API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../tools/trmtype.h"
#include "./basefunc/v5btype.h"
#include "v5type.h"
#include "../tools/tools.h"

void V5_GetRdkVersion(SINT8 *sting);

//==========================
//
//		VMARB module
//
//============================

void V5_MarbOpen(void);
void V5_MarbClose(void);
void V5_MarbInitContext(void);
void V5_MarbSetInfo(TMarbInfo *info);
TMarbContext * V5_MarbGetContext(void);
void V5_MarbInitMemClk(void);

//====================================================

UINT32 V5_MarbGetJbufRWgap(void);
UINT32 V5_MarbJbufRead(UINT8 *pbuf, UINT32 len);
UINT32 V5_MarbJbufWrite(UINT8 *pbuf, UINT32 len);

UINT32 V5_MarbGetThumbRWgap(void);
UINT32 V5_MarbThumbRead(UINT8 *pbuf, UINT32 len);

UINT32 V5_MarbIndexRead(UINT8 *pbuf, UINT32 len);

UINT32 V5_MarbLbuf0Read(UINT8 *pbuf, UINT32 len);
UINT32 V5_MarbLbuf1Read(UINT8 *pbuf, UINT32 len);

UINT32	V5_MarbB0Write(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size);
UINT32	V5_MarbB0Read(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size);
UINT32	V5_MarbB0WriteRepeat(UINT32 val, UINT32 len, UINT32 start, UINT32 span, UINT32 size);

UINT32	V5_MarbB1Write(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size);
UINT32	V5_MarbB1Read(UINT8 *pbuf, UINT32 len, UINT32 start, UINT32 span, UINT32 size);

//====================================================
void	V5_MarbStartCapture(void);
void	V5_MarbStartDecoder(void);

void	V5_MarbSetAllocMode(UINT8 mode);
UINT8	V5_MarbGetAllocMode(void);

//==============================================

void	V5_MarbSetJbufStartAdr(UINT32 adr);
void	V5_MarbSetIndexMem(UINT32 len);
void	V5_MarbAutoSetMem(void);
void	V5_MarbSetGapRatio(UINT8 ratio);

//==============================================
UINT32	V5_MarbGetIndexMem(void);
UINT8	V5_MarbGetGapRatio(void);

void	V5_MarbSetMode(UINT8 mode);
UINT8	V5_MarbGetMode(void);

UINT32	V5_MarbGetB0StartAddr(void);
UINT32	V5_MarbGetB1StartAddr(void);
void	V5_MarbGetMap(TMarbMap *map);

//======================================
void	V5_MarbSetMemStart(UINT32 adr);
UINT32	V5_MarbGetMemStart(void);
void	V5_MarbSetMemEnd(UINT32 adr);
UINT32	V5_MarbGetMemEnd(void);

void	V5B_MarbSetMshotFcnt(UINT8 uVal);
UINT8	V5B_MarbGetMshotFcnt(void);

void	V5B_MarbSetPriorTable(UINT8* uVal);
void	V5B_MarbGetPriorTable(UINT8* uVal);

UINT32	V5B_MarbGetFrmCount(void);
UINT32	V5B_MarbGetVideoSize(void);
UINT8	V5B_MarbGetMarbStatus(void);

UINT32	V5_MarbWriteSpanBuf(UINT8 *pbuf, UINT32 len, UINT32 adr, UINT32 span, UINT32 size);
UINT32	V5_MarbReadSpanBuf(UINT8 *pbuf, UINT32 len, UINT32 adr, UINT32 span, UINT32 size);
UINT32	V5_MarbWriteSpanBufRepeat(UINT32 val, UINT32 len, UINT32 adr, UINT32 span, UINT32 size);
UINT32  V5_MarbWriteSpanBufRev(UINT8 *pbuf, UINT32 len, UINT32 adr, UINT32 span, UINT32 size);

//==============================================
void	_ISR_V5MarbIntHandle(UINT16 intflg);
void	V5_MarbIntSetCallback(UINT8 index, PVJpegIsrCallback pcall);
void	V5_MarbIntEnable(UINT8 index);
void	V5_MarbIntDisable(UINT8 index);
void	_ISR_V5MarbSlvIntHandle(UINT16 intflg);
void	V5_MarbSlvIntSetCallback(UINT8 index, PVJpegIsrCallback pcall);
void	V5_MarbSlvIntEnable(UINT8 index);
void	V5_MarbSlvIntDisable(UINT8 index);

//timer
void	V5_MarbSetTimerCfg(UINT8 mode, UINT32 ms);
UINT32	V5_MarbGetTimerVal(void);
void	V5B_MarbStartTimer(void);
void	V5B_MarbStopTimer(void);

//repeat capture
void	V5B_MarbRpEnable(void);
void	V5B_MarbRpDisable(void);
void	V5B_MarbSetRpThd(UINT8 val);
UINT8	V5B_MarbGetRpThd(void);
void	V5B_MarbSetRpSize(UINT32 val);
UINT32	V5B_MarbGetRpSize(void);
UINT8	V5B_MarbGetRpActTimes(void);

void	V5B_MarbSubReset(UINT8 uVal);

UINT32	V5B_MarbGetJbufPointer(void);
UINT32	V5B_MarbGetTbufPointer(void);
UINT32	V5B_MarbGetLstJbufPointer(void);



//====================================


//==========================
//
//		VCTRL module
//
//============================

void	V5_CtrlInitContext(void);
void	V5_CtrlSetInfo(TCtrlInfo *pinfo);
TCtrlContext* V5_CtrlGetContext(void);

void	V5_CtrlOpen(void);
void	V5_CtrlClose(void);

void	V5_CtrlSwReset(UINT16 uType);
UINT16	V5_CtrlSetModClkOn(UINT16 uType);
UINT16	V5_CtrlSetModClkOff(UINT16 uType);
void	V5_CtrlSetIntCtrl(UINT8 triggerType, UINT8 levelType, UINT8 edgeType, UINT8 sigWidth);
void	V5_CtrlGetIntCtrl(UINT8* triggerType, UINT8* levelType, UINT8* edgeType, UINT8* sigWidth);
void	V5_CtrlIntSetCallback(UINT8 index, PV5IsrCallback pcall);
void	V5_CtrlIntEnable(UINT8 index);
void	V5_CtrlIntDisable(UINT8 index);
void	_ISR_V5IntHandle(void);

void    V5B_BiuSetBypassSelType(UINT8 selType);
void	V5_CtrlNormalToBypass(void);
void	V5_CtrlBypassToNormal(void);

UINT32	V5_CtrlSetChipClk(UINT32 clkin, UINT32 mclk);
UINT32	V5_CtrlSetChipClkDirect(UINT32 clkin, UINT8 m, UINT8 n, UINT8 od0, UINT8 od1, UINT8 outdiv);
UINT32	V5_CtrlGetMclk(void);
void	V5_CtrlPllOn(void);
void	V5_CtrlPllOff(void);
void    V5_CtrlAllModReset(void);

UINT8  V5B_CpmGetIntFlag(void);
UINT16 V5B_CpmGetIntFlagSec(UINT8 offset);

void	V5B_CpmSetGpioCfg(UINT8 cfg);
UINT8	V5B_CpmGetGpioCfg(void);
void	V5B_CpmSetGpioMode(UINT8 mode);
UINT8	V5B_CpmGetGpioMode(void);
void	V5B_CpmSetGpioDirection(UINT8 dir);
UINT8	V5B_CpmGetGpioDirection(void);
void	V5B_CpmSetGpioData(UINT8 data);
UINT8	V5B_CpmGetGpioData(void);

void	V5B_CpmModGlbReset(void);
void	V5B_CpmSetModClkVal(UINT16 uVal);

UINT32 V5B_CpmGetChipID(void);


void   V5_SetReg(UINT32 adr, UINT32 val, UINT8 regWType);
UINT32 V5_GetReg(UINT32 adr, UINT8 regWType);
UINT32 V5_ReadSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_WriteSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_WriteSramRev(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_WriteSramOnWord(UINT32 adr, UINT32 dat, UINT32 size);


/*set register and write data to LCD directly in bypass mode*/
void   V5_BySetPanelSel(UINT8 panelSel);
void   V5_BySetRSLow(UINT16 val);
void   V5_BySetRSHigh(UINT16 val);

/*the follow functions written for sram write back test*/
UINT32 V5_WriteSramTest(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V5_ReadSramTest(UINT32 adr, UINT8 *buf, UINT32 size);

void   V5_InitBusType(UINT8 type);
void   V5B_SetBusType(UINT8 type);
UINT8  V5B_SetRegMaskBit8(UINT32 adr, UINT8 mask, UINT8 op);
UINT16 V5B_SetRegMaskBit16(UINT32 adr, UINT16 mask, UINT8 op);
void   testRegConRead(UINT32 addr, UINT32 readCnt);
void   V5_Delay(UINT32 count);

//==========================
//
//		VJPEG module
//
//============================

//These follow founctions are used for JPEG initialization
void	V5_JpegInitContext(void);
void	V5_JpegSetInfo(TJpegInfo *info);
TJpegContext * V5_JpegGetContext(void);

void	V5_JpegOpen(void);
void	V5_JpegClose(void);

//These follow founctions are used for JPEG contorl and adjustment 
void	V5B_JpegSetTCR(UINT8 val);
UINT8	V5B_JpegGetTCR(void);

void	V5_JpegSetQFactor(UINT8 val);
void	V5_JpegDisAbrc(void);

void	V5_JpegSetImage(UINT8 fmt, TSize size);
void	V5_JpegAutoSetImage(void);

void	V5_JpegSetMode(UINT8 mode);
UINT8	V5_JpegGetMode(void);

//These follow founctions are used for JPEG main functions 
void	V5_JpegCapture(void);
void	V5_JpegSifCapture(void);
void	V5_JpegReCapture(void);
void	V5_JpegEncode(UINT8 fmt, TSize size);
void	V5_JpegDecode(PTJpegIndex pinfo);
void	V5_JpegDecodeVideo(UINT8 fmt, TSize size);
void	V5_JpegDecodeAuto(UINT8 fmt, TSize size);
void	V5_JpegYuvDecode(TSize size);
void	V5_JpegDecodeNext(void);

void	V5_JpegSetRudeBrcPara(PTJpegRudeBrc pRude);
void	V5_JpegSetEnhBrcPara(PTJpegEnhBrc pEnh);

void	_ISR_V5JpegJpegIntHandle(UINT16 intflg);
void	V5_JpegJpegIntSetCallback(UINT8 index, PVJpegIsrCallback pcall);
void	V5_JpegJpegIntEnable(UINT8 index);
void	V5_JpegJpegIntDisable(UINT8 index);

void	_ISR_V5JpegLbufIntHandle(UINT16 intflg);
void	V5_JpegLbufIntSetCallback(UINT8 index, PVJpegIsrCallback pcall);
void	V5_JpegLbufIntEnable(UINT8 index);
void	V5_JpegLbufIntDisable(UINT8 index);

UINT8	V5B_JpegEnAbrc(void);
UINT8	V5B_JpegDisAbrc(void);

UINT8	V5B_JpegSimHeadEnable(void);
UINT8	V5B_JpegSimHeadDisable(void);

UINT8	V5B_JpegAviFmtEnable(void);
UINT8	V5B_JpegAviFmtDisable(void);

UINT8	V5B_LbufDec2Yuv422(void);
UINT8	V5B_LbufDec2Rgb565(void);

//==========================
//
//		VDISP module
//
//============================
UINT8	V5B_LcdifEnableCs0(void);
UINT8	V5B_LcdifEnableCs1(void);
void	V5B_LcdifRsLExW(UINT16 uVal);
void	V5B_LcdifRsHExW(UINT16 uVal);
void	V5B_LcdifResetPanel(void);

UINT16	V5B_LcdcEnableDither(void);
UINT16	V5B_LcdcDisableDither(void);
void	V5B_LcdcSetDitherSel(UINT8 sel);

UINT16	V5B_LcdcLayerB0First(void);
UINT16	V5B_LcdcLayerB0FstOff(void);
UINT16	V5B_LcdcLayerB1First(void);
UINT16	V5B_LcdcLayerB1FstOff(void);
void	V5B_LcdcLayerAFirst(void);

void	V5B_LcdcClearEOF(void);
UINT8	V5B_LcdcCheckEOF(void);
UINT8	V5B_LcdcEnableAOperation(void);
UINT8	V5B_LcdcEnableAlayer(void);
UINT8	V5B_LcdcDisableAlayer(void);
UINT8	V5B_LcdcEnableB0(void);
UINT8	V5B_LcdcDisableB0(void);
UINT8	V5B_LcdcEnableBG(void);
UINT8	V5B_LcdcDisableBG(void);
UINT8	V5B_LcdcEnableB1(void);
UINT8	V5B_LcdcDisableB1(void);

void	V5B_LcdcResetState(void);

UINT8	V5B_LcdcGetUpdate(void);
void	V5B_LcdcBgUpdate(void);

void	V5B_LcdcSetAlayerRM(UINT8 x);
UINT8	V5B_LcdcGetAlayerRM(void);
void	V5B_LcdcSetBlayerRM(UINT8 x);

void	V5B_LcdcSetB0OverKeyColor(UINT32 x);
void	V5B_LcdcSetB1OverKeyColor(UINT32 x);
void	V5B_LcdcSetBgColor(UINT32 x);
void	V5B_LcdcSetPalette(UINT16 total, UINT32 *x);

void	V5B_LcdcSetDEdelay(UINT8 x);
void	V5B_LcdcSetDEinteval(UINT8 x);
void	V5B_LcdcSetLineinteval(UINT8 x);

UINT8	V5B_GeMonoSrcUseSrc(void);
UINT8	V5B_GeMonoSrcUsePtnSrc(void);

UINT8	V5B_GeEnableSrcMask(void);
UINT8	V5B_GeDisableSrcMask(void);

UINT8	V5B_GeEnableTransparent(void);
UINT8	V5B_GeDisableTransparent(void);
void	V5B_GeSetTransparent(UINT8 mode);

UINT8	V5B_GeEnablePtnMask(void);
UINT8	V5B_GeDisablePtnMask(void);

UINT8	V5B_GeMonoPtn(void);
UINT8	V5B_GeColorPtn(void);

void	V5B_GeReset(void);

void	V5B_GeSetPattern(UINT8 cnt, UINT32 *val);

void	V5B_GeSetPtnBkColor(UINT32 x);
void	V5B_GeSetPtnForeColor(UINT32 x);
void	V5B_GeSetSrcBkColor(UINT32 x);
void	V5B_GeSetSrcForeColor(UINT32 x);

void	V5B_GeSetSrcSpan(UINT16 x);
void	V5B_GeSetDstSpan(UINT16 x);

void	V5_DispOpen(void);
void	V5_DispClose(void);

void	V5_DispInitContext(void);
void	V5_DispSetPanelInfo(PTPanelInfo info);
TDispContext * V5_DispGetContext(void);

void	V5_DispSetMode(UINT8 mode);
UINT8	V5_DispGetMode(void);

void	V5_DispGetPanelSize(TSize *size);

void	V5_DispSetB0BlendRatio(UINT8 ratio);
UINT8	V5_DispGetB0BlendRatio(void);
void	V5_DispSetB1BlendRatio(UINT8 ratio);
UINT8	V5_DispGetB1BlendRatio(void);

void	V5_DispSetB0Overlay(UINT8 mode);
UINT8	V5_DispGetB0Overlay(void);
void	V5_DispSetB1Overlay(UINT8 mode);
UINT8	V5_DispGetB1Overlay(void);

void	V5_DispSetALayerWin(TPoint pt, TSize size);
void	V5_DispSetB0LayerWin(TPoint pt, TSize size);
void	V5_DispSetB1LayerWin(TPoint pt, TSize size);
void	V5_DispSetB0RefreshWin(TPoint pt, TSize size);
void	V5_DispSetB1RefreshWin(TPoint pt, TSize size);
void	V5_DispUpdateB0Win(PTWinPos pwin);
void	V5_DispUpdateB1Win(PTWinPos pwin);

void	V5_DispSwitchPanel(UINT8 sel);
UINT8	V5_DispGetPanelSel(void);
UINT8	V5_DispGetPanelType(void);

void	V5_DispSetB0format(UINT8 fmt);
void	V5_DispSetB1format(UINT8 fmt);
UINT8	V5_DispGetB0format(void);
UINT8	V5_DispGetB1format(void);

void	V5_DispSetB0Proty(PTBLayerProty proty);
void	V5_DispGetB0Proty(PTBLayerProty proty);
void	V5_DispSetB1Proty(PTBLayerProty proty);
void	V5_DispGetB1Proty(PTBLayerProty proty);

void	V5_DispSetAmemSize(PTALayerProty proty);
void	V5_DispGetAmemSize(PTALayerProty proty);

void	V5_DispB0Mono(TPoint pt, TSize size, UINT32 val);
void	V5_DispB0Rev(TPoint pt, TSize size, UINT8 *pImage);
void	V5_DispB0Image(TPoint point, TSize size, UINT8 *pImage);
void	V5_DispB1Image(TPoint point, TSize size, UINT8 *pImage); 

void	V5_DispB0DrawBmp(TPoint pt, UINT8 *pData); 
void	V5_DispB1DrawBmp(TPoint pt, UINT8 *pData); 

void	V5_GeSetDst(UINT8 sel);
UINT8	V5_GeGetDst(void);

void	V5_GeSetSrc(UINT8 sel);
UINT8	V5_GeGetSrc(void);

UINT8	V5_GetDstFmt(void);
UINT8	V5_GetSrcFmt(void);

void	V5_GeAutoSetProp(void);
void	V5_GeSetResSpan(UINT8 index, UINT16 value);
void	V5_GeSetResfmt(UINT8 index, UINT8 value);

void	V5_GeClip(TClip clip);
void	V5_GeDrawLine(TPoint ps, TPoint pe, UINT32 color);
void	V5_GeBitblt(TPoint srcp, TPoint dstp, TSize size, UINT8 bitwise);

#ifdef WIN32
extern TDispContext gDispContext;
extern TVispContext	gVispContext;
#endif

//==========================
//
//		VISP module
//
//============================

void	V5B_SifStartCapture(void);
UINT8	V5B_SifEnableM10usCnt(void);

void	V5B_SifI2cAeBatch(UINT8 uNum, TI2cBatch *AeData);
UINT8	V5B_SifI2cWriteByte(UINT8 uAddr, UINT8 uVal);
UINT8	V5B_SifI2cWriteWord(UINT8 uAddr, UINT16 uVal);
UINT8	V5B_SifI2cWriteTriWord(UINT8 uAddr, UINT32 uVal);
UINT8	V5B_SifI2cReadByte(UINT8 uAddr, UINT8 *uVal);
UINT8	V5B_SifI2cReadWord(UINT8 uAddr, UINT16 *uVal);
UINT8	V5B_SifI2cReadTriByte(UINT8 uAddr, UINT32 *uVal);

UINT8	V5B_IspGetYMean(void);

void	V5B_IppGetImageSize(PTSize psize);
void	V5B_IppGetWinPoint(PTPoint ppt);
void	V5B_IppGetWinSize(PTSize psize);
void	V5B_IppGetDispSize(PTSize psize);
void	V5B_IppGetCapSize(PTSize psize);
void	V5B_IppGetThumbSize(PTSize psize);
void	V5B_IppSetThumOutFmt(UINT8 x);
UINT8	V5B_IppGetThumOutFmt(void);

void	V5B_IppEnableSpecial(void);
void	V5B_IppDisableSpecial(void);
UINT8	V5B_IppIsSpecial(void);
void	V5B_IppReset(void);
void	V5B_IppSetSpecialEffect(UINT8 x);
UINT8	V5B_IppGetSpecialEffect(void);
void	V5B_IppSetUOffset(UINT8 x);
UINT8	V5B_IppGetUOffset(void);
void	V5B_IppSetVOffset(UINT8 x);
UINT8	V5B_IppGetVOffset(void);
void	V5B_IppSetUTopThrd(UINT8 x);
UINT8	V5B_IppGetUTopThrd(void);
void	V5B_IppSetUBottomThrd(UINT8 x);
UINT8	V5B_IppGetUBottomThrd(void);
void	V5B_IppSetVTopThrd(UINT8 x);
UINT8	V5B_IppGetVTopThrd(void);
void	V5B_IppSetVBottomThrd(UINT8 x);
UINT8	V5B_IppGetVBottomThrd(void);

void	V5_VispInitContext(void);
PTVispContext V5_VispGetContext(void);
void	V5_VispSetSensorInfo(void);
void	V5_VispSetDefaultParm(void);
UINT8	V5_VispOpen(void);
void	V5_VispClose(void);
void	V5_VispGetHistStat(PTHistStat pdata);

UINT16	V5_VispSetSnrState(UINT8 state);
UINT8	V5_VispGetSnrState(void);
void	V5_VispSetMode(UINT8 mode);
UINT8	V5_VispGetMode(void);
void	V5_VispSetI2cPin(UINT8 mode);
void	V5_VispSetSnrMirrorFlip(UINT8 val);

UINT8	V5_VispGetBrightness(void);
void	V5_VispSetBrightness(UINT8 lValue);
UINT8	V5_VispGetContrast(void);
void	V5_VispSetContrast(UINT8 lValue);
void	V5_VispSetSaturation(UINT8 val);
UINT8	V5_VispGetSaturation(void);

void	V5_VispStartAutoFocus(void);
void	V5_VispStopAutoFocus(void);
void	V5_VispEnableAe(void);
void	V5_VispDisableAe(void);
void	V5_VispEnableAeQuery(void);
void	V5_VispDisableAeQuery(void);
UINT8	V5_VispIsAe(void);

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

void	V5_VispSetDispSizerByWidth(UINT8 index, TPoint winpt, TSize winsize, UINT16 width);
void	V5_VispSetDispSizer(UINT8 index, TPoint winpt, TSize winsize, TSize dsize);
void	V5_VispSetCapSizerByWidth(UINT8 index, TPoint winpt, TSize winsize, UINT16 width);
void	V5_VispSetCapSizer(UINT8 index, TPoint winpt, TSize winsize, TSize csize);

void	V5_VispSetImageIndex(UINT8 index);
UINT8	V5_VispGetImageIndex(void);
UINT8	V5_VispGetImageIndexBySize(TSize size);
UINT8	V5_VispGetImageIndexByWidth(UINT16 width);
void	V5_VispGetSize(UINT8 index, TSize *size);
UINT8	V5_VispGetCurrentSize(TSize *size);

void	V5_VispStartFlashCharge(void);
void	V5_VispStopFlashCharge(void);
void	V5_VispFlashLight(void);
void	V5_VispStopFlashLight(void);

void	V5_VispIspIntSetCallback(UINT8 index, PVispIsrCallback pcall);
void	V5_VispIspIntEnable(UINT8 index);
void	V5_VispIspIntDisable(UINT8 index);
void	V5_VispSifIntSetCallback(UINT8 index, PVispIsrCallback pcall);
void	V5_VispSifIntEnable(UINT8 index);
void	V5_VispSifIntDisable(UINT8 index);

UINT8	V5_VispGetEnv(void);
void	V5_VispSetEnv(UINT8 env);
void	V5_VispJudgeEnv(void);
UINT8	V5_VispGetYTarget(void);
void	V5_VispSetYTarget(UINT8 val);
UINT8	V5_VispGetYThd(void);
void	V5_VispSetYThd(UINT8 val);
UINT8	V5_VispGetYTop(void);
void	V5_VispSetYTop(UINT8 val);
UINT8	V5_VispGetYTopDefault(void);
UINT8	V5_VispGetMaxEt(void);
void	V5_VispSetMaxEt(UINT8 et);
UINT8	V5_VispGetMaxEtDefault(void);
UINT8	V5_VispGetMinEt(void);
void	V5_VispSetMinEt(UINT8 et);
UINT8	V5_VispGetMaxGain(void);
void	V5_VispSetMaxGain(UINT8 gain);
UINT8	V5_VispGetMinGain(void);
void	V5_VispSetMinGain(UINT8 gain);
UINT8	V5_VispGetAeSpeed(void);
void	V5_VispSetAeSpeed(UINT8 speed);
void	V5_VispSetFlashYThd(UINT8 x);
UINT8	V5_VispGetFlashYThd(void);
void	V5_VispSetFlashEt(UINT8 x);
UINT8	V5_VispGetFlashEt(void);
void	V5_VispEnableFlash(void);
void	V5_VispDisableFlash(void);
void	V5_VispSetCurEt(UINT32 et);
void	V5_VispSetEt(void);

void	VComputerSizeByWidth(TSize src, UINT16 width, TSize *dst);
void	VComputerSizeByHeight(TSize src, UINT16 height, TSize *dst);

UINT32	V5_VispGetCurEt(void);
UINT8	V5_VispGetCurGain(void);

#ifdef __cplusplus
}
#endif

#endif 
