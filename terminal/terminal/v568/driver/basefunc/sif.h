#ifndef _RDK_SIF_H_
#define _RDK_SIF_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define V5_ISP_I2CWAITTIME			10
#else
#define V5_ISP_I2CWAITTIME			0x7f00
#endif

	void	V5B_SifSetSensorMode(UINT8 sel);
	UINT8	V5B_SifGetSensorMode(void);
	UINT8	V5B_SifEnableSnrSyncGen(void);
	UINT8	V5B_SifDisableSnrSyncGen(void);
	UINT8	V5B_SifSetDataFormat(UINT8 uFormat);
	void	V5B_SifSetSensorOpMode(UINT8 sel);
	UINT8	V5B_SifGetSensorOpMode(void);
	void	V5B_SifStartCapture(void);
	void	V5B_SifStopCapture(void);
	UINT8	V5B_SifEnableSyncGen(void);
	UINT8	V5B_SifDisableSyncGen(void);
	void	V5B_SifSetYuvFormat(UINT8 sel);
	UINT8	V5B_SifGetYuvFormat(void);
	void	V5B_SifSetVsyncCtrl(UINT8 sel);
	UINT8	V5B_SifGetVsyncCtrl(void);
	void	V5B_SifSetHsyncCtrl(UINT8 sel);
	UINT8	V5B_SifGetHsyncCtrl(void);
	void	V5B_SifSetSnrClk(UINT8 sel);
	UINT8	V5B_SifGetSnrClk(void);
	void	V5B_SifSetPixelRate(UINT8 sel);
	UINT8	V5B_SifGetPixelRate(void);
	void	V5B_SifSetSerialclk(UINT8 sel);
	UINT8	V5B_SifGetSerialclk(void);
	void	V5B_SifSetI2cclk(UINT16 sel);
	UINT16	V5B_SifGetI2cclk(void);
	void	V5B_SifSetI2cInterval(UINT16 sel);
	UINT16	V5B_SifGetI2cInterval(void);
	void	V5B_SifSetHrefStart(UINT16 startx);
	UINT16	V5B_SifGetHrefStart(void);
	void	V5B_SifSetHrefLength(UINT16 length);
	UINT16	V5B_SifGetHrefLength(void);
	void	V5B_SifSetVrefStart(UINT16 startx);
	UINT16	V5B_SifGetVrefStart(void);
	void	V5B_SifSetVrefStop(UINT16 stop);
	UINT16	V5B_SifGetVrefStop(void);
	void	V5B_SifSetOB(UINT8 x);
	UINT8	V5B_SifGetOB(void);
	void	V5B_SifSetAeWinStart(UINT16 start);
	UINT16	V5B_SifGetAeWinStart(void);
	void	V5B_SifSetAeWinStop(UINT16 x);
	UINT16	V5B_SifGetAeWinStop(void);
	void	V5B_SifSetEdgeChos(UINT8 sel);
	UINT8	V5B_SifGetEdgeChos(void);
	void	V5B_SifEnableSnrPClk(void);
	void	V5B_SifDisableSnrPClk(void);
	UINT8	V5B_SifIsSnrPClkEn(void);
	void	V5B_SifEnableVSyncDly(void);
	void	V5B_SifDisableVSyncDly(void);
	UINT8	V5B_SifIsVSyncDlyEn(void);
	void	V5B_SifEnableFifoVSyncDly(void);
	void	V5B_SifDisableFifoVSyncDly(void);
	UINT8	V5B_SifIsFifoVSyncDlyEn(void);
	void	V5B_SifSetPClkPosEdge(void);
	void	V5B_SifSetPClkNegEdge(void);
	UINT8	V5B_SifGetPClkEdge(void);
	void	V5B_SifEnableSnrDataShift(void);
	void	V5B_SifDisableSnrDataShift(void);
	UINT8	V5B_SifIsSnrDataShiftEn(void);
	void	V5B_SifSetCounterReset(UINT8 sel);
	UINT8	V5B_SifGetCounterReset(void);
	void	V5B_SifSetCaptureDelay(UINT8 sel);
	UINT8	V5B_SifGetCaptureDelay(void);
	void	V5B_SifSetFlashPowerEn (UINT8 sel);
	UINT8	V5B_SifGetFlashPowerEn(void);
	UINT8	V5B_SifDisableFlashPower(void);
	UINT8	V5B_SifEnableFlashPower(void);
	void	V5B_SifSetFlashPolarity(UINT8 sel);
	UINT8	V5B_SifGetFlashPolarity(void);
	void	V5B_SifSetFlashPowerCntTgt(UINT16 x);
	UINT16	V5B_SifGetFlashPowerCntTgt(void);
	void	V5B_SifSetFlashPowerCntDuty(UINT16 x);
	UINT16	V5B_SifGetFlashPowerCntDuty(void);
	void	V5B_SifSetFlashDuty(UINT8 sel);
	UINT8	V5B_SifGetFlashDuty(void);
	void	V5B_SifSetM10usCntEn (UINT8 sel);
	UINT8	V5B_SifGetM10usCntEn (void);
	UINT8	V5B_SifDisableM10usCnt(void);
	UINT8	V5B_SifEnableM10usCnt(void);
	void	V5B_SifSetM10usTgt(UINT8 sel);
	UINT8	V5B_SifGetM10usTgt(void);
	void	V5B_SifSetDelayFlashTgt(UINT32 x);
	UINT32	V5B_SifGetDelayFlashTgt(void);
	void	V5B_SifSetTriggerTgt(UINT16 x);
	UINT16	V5B_SifGetTriggerTgt(void);
	void	V5B_SifSetTriggerStyle(UINT8 sel);
	UINT8	V5B_SifGetTriggerStyle(void);
	void	V5B_SifSetTriggerStart(UINT8 sel);
	UINT8	V5B_SifGetTriggerStart(void);
	void	V5B_SifStartTrigger(void);
	void	V5B_SifStopTrigger(void);
	void	V5B_SifSetTriggerStartRow(UINT16 x);
	UINT16	V5B_SifGetTriggerStartRow(void);
	void	V5B_SifSetTriggerStartCol(UINT16 x);
	UINT16	V5B_SifGetTriggerStartCol(void);
	void	V5B_SifSetVSyncDelay(UINT16 x);
	UINT16	V5B_SifGetVSyncDelay(void);

	void	V5B_SifSetI2cDevAddr(UINT8 adr);
	UINT8	V5B_SifGetI2cDevAddr(void);
	void	V5B_SifI2cAeBatch(UINT8 uNum, TI2cBatch *AeData);
	void	V5B_SifI2cAeBatchNow(UINT8 uNum, TI2cBatch *AeData);
	UINT8	V5B_SifI2cWriteByte(UINT8 uAddr, UINT8 uVal);
	UINT8	V5B_SifI2cWriteWord(UINT8 uAddr, UINT16 uVal);
	UINT8	V5B_SifI2cWriteTriWord(UINT8 uAddr, UINT32 uVal);
	UINT8	V5B_SifI2cReadByte(UINT8 uAddr, UINT8 *uVal);
	UINT8	V5B_SifI2cReadWord(UINT8 uAddr, UINT16 *uVal);
	UINT8	V5B_SifI2cReadTriByte(UINT8 uAddr, UINT32 *uVal);

#ifdef __cplusplus
}
#endif

#endif
