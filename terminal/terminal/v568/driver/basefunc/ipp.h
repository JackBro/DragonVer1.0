#ifndef _RDK_IPP_H_
#define _RDK_IPP_H_

#ifdef __cplusplus
extern "C" {
#endif

void	V5B_IppSetSpeCtrl(UINT8 sel);
UINT8	V5B_IppGetSpeCtrl(void);
void	V5B_IppEnableSpecial(void);
void	V5B_IppDisableSpecial(void);
UINT8	V5B_IppIsSpecial(void);
void	V5B_IppSetWorkMode(UINT8 x);
UINT8	V5B_IppGetWorkMode(void);
void	V5B_IppSetSpecialEffect(UINT8 x);
UINT8	V5B_IppGetSpecialEffect(void);

void	V5B_IppSetSizerCtrl0(UINT8 x);
UINT8	V5B_IppGetSizerCtrl0(void);
void	V5B_IppEnableDispSizer(void);
void	V5B_IppDisableDispSizer(void);
void	V5B_IppEnableCapDnSizer(void);
void	V5B_IppDisableCapDnSizer(void);
void	V5B_IppEnableCapUpSizer(void);
void	V5B_IppDisableCapUpSizer(void);
void	V5B_IppEnableThumbSizer(void);
void	V5B_IppDisableThumbSizer(void);
void	V5B_IppReset(void);
void	V5B_IppSetDataSelect(UINT8 sel);
UINT8	V5B_IppGetDataSelect(void);
void	V5B_IppEnableFrame(void);
void	V5B_IppDisableFrame(void);

void	V5B_IppSetImageWidth(UINT16 x);
UINT16	V5B_IppGetImageWidth(void);
void	V5B_IppSetImageHeight(UINT16 x);
UINT16	V5B_IppGetImageHeight(void);
void	V5B_IppSetImageSize(TSize size);
void	V5B_IppGetImageSize(PTSize psize);

void	V5B_IppSetWinWidth(UINT16 x);
UINT16	V5B_IppGetWinWidth(void);
void	V5B_IppSetWinHeight(UINT16 x);
UINT16	V5B_IppGetWinHeight(void);
void	V5B_IppSetWinStartX(UINT16 x);
UINT16	V5B_IppGetWinStartX(void);
void	V5B_IppSetWinStartY(UINT16 y);
UINT16	V5B_IppGetWinStartY(void);
void	V5B_IppSetWinPoint(TPoint pt);
void	V5B_IppGetWinPoint(PTPoint ppt);
void	V5B_IppSetWinSize(TSize size);
void	V5B_IppGetWinSize(PTSize psize);

void	V5B_IppSetDispWidth(UINT16 x);
UINT16	V5B_IppGetDispWidth(void);
void	V5B_IppSetDispHeight(UINT16 x);
UINT16	V5B_IppGetDispHeight(void);
void	V5B_IppSetDispSize(TSize size);
void	V5B_IppGetDispSize(PTSize psize);

void	V5B_IppSetDispHW1(UINT8 x);
UINT8	V5B_IppGetDispHW1(void);
void	V5B_IppSetDispHW2(UINT8 x);
UINT8	V5B_IppGetDispHW2(void);
void	V5B_IppSetDispHS1(UINT16 x);
UINT16	V5B_IppGetDispHS1(void);
void	V5B_IppSetDispHS2(UINT16 x);
UINT16	V5B_IppGetDispHS2(void);
void	V5B_IppSetDispHC(UINT8 x);
UINT8	V5B_IppGetDispHC(void);
void	V5B_IppSetDispHFactor(TV5BIppSzDnFct factor);
void	V5B_IppGetDispHFactor(PTV5BIppSzDnFct pfactor);
void	V5B_IppSetDispVH1(UINT8 x);
UINT8	V5B_IppGetDispVH1(void);
void	V5B_IppSetDispVH2(UINT8 x);
UINT8	V5B_IppGetDispVH2(void);
void	V5B_IppSetDispVS1(UINT16 x);
UINT16	V5B_IppGetDispVS1(void);
void	V5B_IppSetDispVS2(UINT16 x);
UINT16	V5B_IppGetDispVS2(void);
void	V5B_IppSetDispVC(UINT8 x);
UINT8	V5B_IppGetDispVC(void);
void	V5B_IppSetDispVFactor(TV5BIppSzDnFct factor);
void	V5B_IppGetDispVFactor(PTV5BIppSzDnFct pfactor);

void	V5B_IppSetCapWidth(UINT16 x);
UINT16	V5B_IppGetCapWidth(void);
void	V5B_IppSetCapHeight(UINT16 x);
UINT16	V5B_IppGetCapHeight(void);
void	V5B_IppSetCapSize(TSize size);
void	V5B_IppGetCapSize(PTSize psize);

void	V5B_IppSetCapDnHW1(UINT8 x);
UINT8	V5B_IppGetCapDnHW1(void);
void	V5B_IppSetCapDnHW2(UINT8 x);
UINT8	V5B_IppGetCapDnHW2(void);
void	V5B_IppSetCapDnHS1(UINT16 x);
UINT16	V5B_IppGetCapDnHS1(void);
void	V5B_IppSetCapDnHS2(UINT16 x);
UINT16	V5B_IppGetCapDnHS2(void);
void	V5B_IppSetCapDnHC(UINT8 x);
UINT8	V5B_IppGetCapDnHC(void);
void	V5B_IppSetCapDnHFactor(TV5BIppSzDnFct factor);
void	V5B_IppGetCapDnHFactor(PTV5BIppSzDnFct pfactor);
void	V5B_IppSetCapDnVH1(UINT8 x);
UINT8	V5B_IppGetCapDnVH1(void);
void	V5B_IppSetCapDnVH2(UINT8 x);
UINT8	V5B_IppGetCapDnVH2(void);
void	V5B_IppSetCapDnVS1(UINT16 x);
UINT16	V5B_IppGetCapDnVS1(void);
void	V5B_IppSetCapDnVS2(UINT16 x);
UINT16	V5B_IppGetCapDnVS2(void);
void	V5B_IppSetCapDnVC(UINT8 x);
UINT8	V5B_IppGetCapDnVC(void);
void	V5B_IppSetCapDnVFactor(TV5BIppSzDnFct factor);
void	V5B_IppGetCapDnVFactor(PTV5BIppSzDnFct pfactor);
void	V5B_IppSetCapUpHW1(UINT8 x);
UINT8	V5B_IppGetCapUpHW1(void);
void	V5B_IppSetCapUpHW2(UINT8 x);
UINT8	V5B_IppGetCapUpHW2(void);
void	V5B_IppSetCapUpHS(UINT16 x);
UINT16	V5B_IppGetCapUpHS(void);
void	V5B_IppSetCapUpHFactor(TV5BIppSzUpFct factor);
void	V5B_IppGetCapUpHFactor(PTV5BIppSzUpFct pfactor);
void	V5B_IppSetCapUpVH1(UINT8 x);
UINT8	V5B_IppGetCapUpVH1(void);
void	V5B_IppSetCapUpVH2(UINT8 x);
UINT8	V5B_IppGetCapUpVH2(void);
void	V5B_IppSetCapUpVS(UINT16 x);
UINT16	V5B_IppGetCapUPVS(void);
void	V5B_IppSetCapUpVFactor(TV5BIppSzUpFct factor);
void	V5B_IppGetCapUpVFactor(PTV5BIppSzUpFct pfactor);

void	V5B_IppSetThumbWidth(UINT16 x);
UINT16	V5B_IppGetThumbWidth(void);
void	V5B_IppSetThumbHeight(UINT16 x);
UINT16	V5B_IppGetThumbHeight(void);
void	V5B_IppSetThumbSize(TSize size);
void	V5B_IppGetThumbSize(PTSize psize);

void	V5B_IppSetThumbHW1(UINT8 x);
UINT8	V5B_IppGetThumbHW1(void);
void	V5B_IppSetThumbHW2(UINT8 x);
UINT8	V5B_IppGetThumbHW2(void);
void	V5B_IppSetThumbHS1(UINT16 x);
UINT16	V5B_IppGetThumbHS1(void);
void	V5B_IppSetThumbHS2(UINT16 x);
UINT16	V5B_IppGetThumbHS2(void);
void	V5B_IppSetThumbHC(UINT8 x);
UINT8	V5B_IppGetThumbHC(void);
void	V5B_IppSetThumbHFactor(TV5BIppSzDnFct factor);
void	V5B_IppGetThumbHFactor(PTV5BIppSzDnFct pfactor);
void	V5B_IppSetThumbVH1(UINT8 x);
UINT8	V5B_IppGetThumbVH1(void);
void	V5B_IppSetThumbVH2(UINT8 x);
UINT8	V5B_IppGetThumbVH2(void);
void	V5B_IppSetThumbVS1(UINT16 x);
UINT16	V5B_IppGetThumbVS1(void);
void	V5B_IppSetThumbVS2(UINT16 x);
UINT16	V5B_IppGetThumbVS2(void);
void	V5B_IppSetThumbVC(UINT8 x);
UINT8	V5B_IppGetThumbVC(void);
void	V5B_IppSetThumbVFactor(TV5BIppSzDnFct factor);
void	V5B_IppGetThumbVFactor(PTV5BIppSzDnFct pfactor);
void	V5B_IppSetThumbInput(UINT8 x);
UINT8	V5B_IppGetThumbInput(void);

void	V5B_IppSetSizerCtrl1(UINT8 x);
UINT8	V5B_IppGetSizerCtrl1(void);
void	V5B_IppEnableCPDHSBP(void);
void	V5B_IppDisableCPDHSBP(void);
void	V5B_IppEnableCPDVSBP(void);
void	V5B_IppDisableCPDVSBP(void);
void	V5B_IppEnableCPUHSBP(void);
void	V5B_IppDisableCPUHSBP(void);
void	V5B_IppEnableCPUVSBP(void);
void	V5B_IppDisableCPUVSBP(void);
void	V5B_IppEnableDPHSBP(void);
void	V5B_IppDisableDPHSBP(void);
void	V5B_IppEnableDPVSBP(void);
void	V5B_IppDisableDPVSBP(void);
void	V5B_IppEnableTMHSBP(void);
void	V5B_IppDisableTMHSBP(void);
void	V5B_IppEnableTMVSBP(void);
void	V5B_IppDisableTMVSBP(void);

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

void	V5B_IppSetCapFrmDrop(UINT16 x);
UINT16	V5B_IppGetCapFrmDrop(void);
void	V5B_IppSetDispFrmDrop(UINT16 x);
UINT16	V5B_IppGetDispFrmDrop(void);

void	V5B_IppEnableHist(void);
void	V5B_IppSetHistIn(UINT8 x);
UINT8	V5B_IppGetHistIn(void);
UINT8	V5B_IppGetHistXCur(void);
UINT8	V5B_IppGetHistXAvg(void);
void	V5B_IppSetHistQ(UINT32 x);
UINT32	V5B_IppGetHistQ(void);
void	V5B_IppGetHistResult(UINT8 *buf);

void	V5B_IppSetThumOutFmt(UINT8 x);
UINT8	V5B_IppGetThumOutFmt(void);

#ifdef __cplusplus
}
#endif

#endif
