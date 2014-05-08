


#ifndef _RDK_MARB_H_
#define _RDK_MARB_H_

#ifdef __cplusplus
extern "C" {
#endif


//DOM-IGNORE-BEG
/*=============================================================================*/
/*                         base level functions                                */
/*=============================================================================*/
//DOM-IGNORE-END


void V5B_MarbSetMarbCtrl(UINT8 uVal);
void V5B_MarbStartCapture(void);
void V5B_MarbStopCapture(void);
void V5B_MarbStartDecoder(void);
void V5B_MarbUpdatePriority(void);
void V5B_MarbStartTimer(void);
void V5B_MarbStopTimer(void);
void V5B_MarbLockTimer(void);

//==============================

void V5B_MarbSubReset(UINT8 uVal);
void V5B_MarbResetIpp(void);
void V5B_MarbResetJpeg(void);
void V5B_MarbResetMem0(void);
void V5B_MarbResetMem1(void);
void V5B_MarbResetMem2(void);
//==============================

void  V5B_MarbSetModeCtrl0(UINT8 uVal);
UINT8 V5B_MarbGetModeCtrl0(void);
UINT8 V5B_MarbEnableJbufFrmIntval(void);
UINT8 V5B_MarbDisableJbufFrmIntval(void);
UINT8 V5B_MarbEnableWithJpeg(void);
UINT8 V5B_MarbDisableWithJpeg(void);
UINT8 V5B_MarbEnableAviIndex(void);
UINT8 V5B_MarbDisableAviIndex(void);
UINT8 V5B_MarbEnableVideoHeader(void);
UINT8 V5B_MarbDisableVideoHeader(void);
UINT8 V5B_MarbEnableThumbBuf(void);
UINT8 V5B_MarbDisableThumbBuf(void);
UINT8 V5B_MarbEncodeDir(void);
UINT8 V5B_MarbDecodeDir(void);
void  V5B_MarbSetCaptureMode(UINT8 mode);
UINT8 V5B_MarbGetCaptureMode(void);
//============================================

void  V5B_MarbSetModeCtrl1(UINT8 uVal);
UINT8 V5B_MarbGetModeCtrl1(void);
UINT8 V5B_MarbEnableThumbFifo(void);
UINT8 V5B_MarbDisableThumbFifo(void);
UINT8 V5B_MarbEnableJBufFifo(void);
UINT8 V5B_MarbDisableJbufFifo(void);
UINT8 V5B_MarbStopVidHost(void);
UINT8 V5B_MarbStopVidTimer(void);
UINT8 V5B_MarbTimerRstHost(void);
UINT8 V5B_MarbTimerRstAuto(void);
UINT8 V5B_MarbRecapErrReset(void);
UINT8 V5B_MarbRecapErrNoOp(void);

//============================================

void	V5B_MarbSetJbufStart(UINT32 uVal);
UINT32	V5B_MarbGetJbufStart(void);
void	V5B_MarbSetJbufEnd(UINT32 uVal);
UINT32	V5B_MarbGetJbufEnd(void);
UINT32	V5B_MarbGetJbufSize(void);
//============================================

void	V5B_MarbSetTbufStart(UINT32 uVal);
UINT32	V5B_MarbGetTbufStart(void);
void	V5B_MarbSetTbufEnd(UINT32 uVal);
UINT32	V5B_MarbGetTbufEnd(void);
UINT32	V5B_MarbGetTbufSize(void);
//============================================

void	V5B_MarbSetIdxStart(UINT32 uVal);
UINT32	V5B_MarbGetIdxStart(void);
void	V5B_MarbSetIdxEnd(UINT32 uVal);
UINT32	V5B_MarbGetIdxEnd(void);
UINT32	V5B_MarbGetIdxSize(void);
//============================================

void	V5B_MarbSetJbufInterval(UINT32 uVal);
UINT32	V5B_MarbGetJbufInterval(void);
void	V5B_MarbSetJbufUp(UINT32 uVal);
UINT32	V5B_MarbGetJbufUp(void);
void	V5B_MarbSetJbufLow(UINT32 uVal);
UINT32	V5B_MarbGetJbufLow(void);
//============================================

void	V5B_MarbSetTbufInterval(UINT32 uVal);
UINT32	V5B_MarbGetTbufInterval(void);
void	V5B_MarbSetTbufUp(UINT32 uVal);
UINT32	V5B_MarbGetTbufUp(void);
void	V5B_MarbSetTbufLow(UINT32 uVal);
UINT32	V5B_MarbGetTbufLow(void);
//============================================

void	V5B_MarbSetFrameInterval(UINT8 uVal);
UINT8	V5B_MarbGetFrameInterval(void);
//============================================

void	V5B_MarbSetMemCtrl(UINT8 uVal);
UINT8	V5B_MarbGetMemCtrl(void);
UINT8	V5B_MarbMem2PowerOn(void);
UINT8	V5B_MarbMem2PowerDown(void);
UINT8	V5B_MarbMem1PowerOn(void);
UINT8	V5B_MarbMem1PowerDown(void);
UINT8	V5B_MarbMem0PowerOn(void);
UINT8	V5B_MarbMem0PowerDown(void);
UINT8	V5B_MarbMem2ClkOn(void);
UINT8	V5B_MarbMem2ClkOff(void);
UINT8	V5B_MarbMem1ClkOn(void);
UINT8	V5B_MarbMem1ClkOff(void);
UINT8	V5B_MarbMem0ClkOn(void);
UINT8	V5B_MarbMem0ClkOff(void);

//============================================

void  V5B_MarbSetMshotFcnt(UINT8 uVal);
UINT8 V5B_MarbGetMshotFcnt(void);
//============================================

void V5B_MarbSetPriorTable(UINT8* uVal);
void V5B_MarbGetPriorTable(UINT8* uVal);
//============================================

UINT32	V5B_MarbGetJbufPointer(void);
UINT32	V5B_MarbGetTbufPointer(void);
UINT32	V5B_MarbGetLstJbufPointer(void);
UINT32	V5B_MarbGetFrmCount(void);
UINT32	V5B_MarbGetVideoSize(void);
UINT8	V5B_MarbGetMarbStatus(void);

UINT32	V5B_MarbGetCurWData(void);

void	V5B_MarbSetClkDiv(UINT16 val);
UINT16	V5B_MarbGetClkDiv(void);

void	V5B_MarbSet1TCtrl(UINT8 val);
UINT8	V5B_MarbGet1TCtrl(void);
UINT8	V5B_Marb1TCtrlMem2ZzbOn(void);
UINT8	V5B_Marb1TCtrlMem2ZzbOff(void);
UINT8	V5B_Marb1TCtrlMem1ZzbOn(void);
UINT8	V5B_Marb1TCtrlMem1ZzbOff(void);
UINT8	V5B_Marb1TCtrlMem0ZzbOn(void);
UINT8	V5B_Marb1TCtrlMem0ZzbOff(void);
UINT8	V5B_Marb1TCtrlMem2EccOn(void);
UINT8	V5B_Marb1TCtrlMem2EccOff(void);
UINT8	V5B_Marb1TCtrlMem1EccOn(void);
UINT8	V5B_Marb1TCtrlMem1EccOff(void);
UINT8	V5B_Marb1TCtrlMem0EccOn(void);
UINT8	V5B_Marb1TCtrlMem0EccOff(void);

void	V5B_MarbSetRefPoint(UINT8 val);
UINT8	V5B_MarbGetRefPoint(void);

void	V5B_MarbRpEnable(void);
void	V5B_MarbRpDisable(void);
void	V5B_MarbSetRpThd(UINT8 val);
UINT8	V5B_MarbGetRpThd(void);
void	V5B_MarbSetRpSize(UINT32 val);
UINT32	V5B_MarbGetRpSize(void);
UINT8	V5B_MarbGetRpActTimes(void);

void	V5B_MarbSetTimerBase(UINT16 val);
void	V5B_MarbSetTimerVal(UINT32 val);
UINT16	V5B_MarbGetTimerBase(void);
UINT32	V5B_MarbGetTimerVal(void);
UINT16	V5B_MarbGetCurTimerBase(void);
UINT32	V5B_MarbGetCurTimerVal(void);
void	V5B_MarbGetCurTimer(UINT16* base, UINT32* val);

#ifdef __cplusplus
}
#endif


#endif


