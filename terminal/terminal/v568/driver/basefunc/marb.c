

#include "basefunc.h"
#ifndef	V578CMD_DISABLE


/********************************************************************************
  Description:
	Set Marb control value.
  Parameters:
	uVal : write value, see enum V5B_MARB_CTRLenum
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetMarbCtrl(UINT8 uVal)
{
	V5_SetReg(V5_REG_MARB_MARB_CTRL, uVal, BIT_8);
}

void V5B_MarbStartCapture(void)
{
	V5B_MarbSetMarbCtrl(V5B_MARB_CTRL_START_CAP_CAM);
}

void V5B_MarbStopCapture(void)
{
	V5B_MarbSetMarbCtrl(V5B_MARB_CTRL_STOP_CAP);
}

void V5B_MarbStartDecoder(void)
{
	V5B_MarbSetMarbCtrl(V5B_MARB_CTRL_START_DECODE);
}

void V5B_MarbUpdatePriority(void)
{
	V5B_MarbSetMarbCtrl(V5B_MARB_CTRL_SET_PRIORITY);
}

void V5B_MarbStartTimer(void)
{
	V5B_MarbSetMarbCtrl(V5B_MARB_CTRL_START_TIMER);
}

void V5B_MarbStopTimer(void)
{
	V5B_MarbSetMarbCtrl(V5B_MARB_CTRL_STOP_TIMER);
}
void V5B_MarbLockTimer(void)
{
	V5B_MarbSetMarbCtrl(V5B_MARB_CTRL_LOCK_TIMER);
}


/********************************************************************************
  Description:
	Reset marb sub modules.
  Parameters:
	uVal : each bit correspond to each sub modules, see enum V5B_MARB_SUB_RESETenum
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSubReset(UINT8 uVal)
{
	V5_SetReg(V5_REG_MARB_MARB_RESET, uVal, BIT_8);
}

void V5B_MarbResetIpp(void)
{
	V5B_MarbSubReset(V5B_MARB_SUB_RESET_IPP);
	V5B_MarbSubReset(0);
}

void V5B_MarbResetJpeg(void)
{
	V5B_MarbSubReset(V5B_MARB_SUB_RESET_JPEG);
	V5B_MarbSubReset(0);
}

void V5B_MarbResetMem0(void)
{
	V5B_MarbSubReset(V5B_MARB_SUB_RESET_1TSRAM0);
	V5B_MarbSubReset(0);
}

void V5B_MarbResetMem1(void)
{
	V5B_MarbSubReset(V5B_MARB_SUB_RESET_1TSRAM1);
	V5B_MarbSubReset(0);
}

void V5B_MarbResetMem2(void)
{
	V5B_MarbSubReset(V5B_MARB_SUB_RESET_1TSRAM2);
	V5B_MarbSubReset(0);
}

/********************************************************************************
  Description:
	Set marb mode control value.
  Parameters:
	uVal : marb mode control value, bit correspond, see V5B_MARB_MODECTRLenum
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetModeCtrl0(UINT8 uVal)
{
	V5_SetReg(V5_REG_MARB_MODE_CTRL_0, uVal, BIT_8);
}

UINT8 V5B_MarbGetModeCtrl0(void)
{
	return (UINT8)V5_GetReg(V5_REG_MARB_MODE_CTRL_0, BIT_8);
}

UINT8 V5B_MarbEnableJbufFrmIntval(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x | V5B_MARB_MODECTRL_JPEG_USE_FRM_INTV));
	return x;
}

UINT8 V5B_MarbDisableJbufFrmIntval(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x & ~V5B_MARB_MODECTRL_JPEG_USE_FRM_INTV));
	return x;
}

UINT8 V5B_MarbEnableWithJpeg(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x & ~V5B_MARB_MODECTRL_CAP_WITHOUT_JPEG));
	return x;
}

UINT8 V5B_MarbDisableWithJpeg(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x | V5B_MARB_MODECTRL_CAP_WITHOUT_JPEG));
	return x;
}

UINT8 V5B_MarbEnableAviIndex(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x & ~V5B_MARB_MODECTRL_NOT_WRITE_AVI_IDX));
	return x;
}

UINT8 V5B_MarbDisableAviIndex(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x | V5B_MARB_MODECTRL_NOT_WRITE_AVI_IDX));
	return x;
}

UINT8 V5B_MarbEnableVideoHeader(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x & ~V5B_MARB_MODECTRL_NOT_WRITE_VIDEO_HEAD));
	return x;
}

UINT8 V5B_MarbDisableVideoHeader(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x | V5B_MARB_MODECTRL_NOT_WRITE_VIDEO_HEAD));
	return x;
}

UINT8 V5B_MarbEnableThumbBuf(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x & ~V5B_MARB_MODECTRL_WITHOUT_THUMB));
	return x;
}

UINT8 V5B_MarbDisableThumbBuf(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x | V5B_MARB_MODECTRL_WITHOUT_THUMB));
	return x;
}

UINT8 V5B_MarbEncodeDir(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x & ~V5B_MARB_MODECTRL_DECODE));
	return x;
}

UINT8 V5B_MarbDecodeDir(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0((UINT8)(x | V5B_MARB_MODECTRL_DECODE));
	return x;
}

void V5B_MarbSetCaptureMode(UINT8 mode)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	V5B_MarbSetModeCtrl0( (x & 0xfc) | (mode & 0x3) );
}

UINT8 V5B_MarbGetCaptureMode(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl0();
	return x & 3;
}
//============================================

void V5B_MarbSetModeCtrl1(UINT8 uVal)
{
	V5_SetReg(V5_REG_MARB_MODE_CTRL_1, uVal, BIT_8);
}

UINT8 V5B_MarbGetModeCtrl1(void)
{
	return (UINT8)V5_GetReg(V5_REG_MARB_MODE_CTRL_1, BIT_8);
}

UINT8 V5B_MarbEnableThumbFifo(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl1();
	V5B_MarbSetModeCtrl1((UINT8)(x & ~V5B_MARB_MODECTRL_TBUF_TYPE_BUFFER));
	return x;
}

UINT8 V5B_MarbDisableThumbFifo(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl1();
	V5B_MarbSetModeCtrl1((UINT8)(x | V5B_MARB_MODECTRL_TBUF_TYPE_BUFFER));
	return x;
}

UINT8 V5B_MarbEnableJBufFifo(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl1();
	V5B_MarbSetModeCtrl1((UINT8)(x & ~V5B_MARB_MODECTRL_JBUF_TYPE_BUFFER));
	return x;
}

UINT8 V5B_MarbDisableJbufFifo(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl1();
	V5B_MarbSetModeCtrl1((UINT8)(x | V5B_MARB_MODECTRL_JBUF_TYPE_BUFFER));
	return x;
}

UINT8 V5B_MarbStopVidHost(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl1();
	V5B_MarbSetModeCtrl1((UINT8)(x & ~V5B_MARB_CAP_VID_AUTO_STOP));
	return x;
}

UINT8 V5B_MarbStopVidTimer(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl1();
	V5B_MarbSetModeCtrl1((UINT8)(x | V5B_MARB_CAP_VID_AUTO_STOP));
	return x;
}

UINT8 V5B_MarbTimerRstHost(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl1();
	V5B_MarbSetModeCtrl1((UINT8)(x & ~V5B_MARB_TIMER_MODE));
	return x;
}

UINT8 V5B_MarbTimerRstAuto(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl1();
	V5B_MarbSetModeCtrl1((UINT8)(x | V5B_MARB_TIMER_MODE));
	return x;
}

UINT8 V5B_MarbRecapErrReset(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl1();
	V5B_MarbSetModeCtrl1((UINT8)(x | V5B_MARB_RECAP_ERR_OP));
	return x;
}

UINT8 V5B_MarbRecapErrNoOp(void)
{
	UINT8 x;

	x = V5B_MarbGetModeCtrl1();
	V5B_MarbSetModeCtrl1((UINT8)(x & ~V5B_MARB_RECAP_ERR_OP));
	return x;
}

/********************************************************************************
  Description:
	Set marb JPEG buffer start address.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetJbufStart(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_JBUF_START_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get JPEG buffer start address.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetJbufStart(void)
{
	return V5_GetReg(V5_REG_MARB_JBUF_START_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Set marb JPEG buffer end address.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetJbufEnd(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_JBUF_END_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get JPEG buffer end address.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetJbufEnd(void)
{
	return V5_GetReg(V5_REG_MARB_JBUF_END_0, BIT_24_HTOL);
}

UINT32 V5B_MarbGetJbufSize(void)
{
	UINT32 x, y;

	x = V5B_MarbGetJbufEnd();
	y = V5B_MarbGetJbufStart();
	return x - y + 4;
}

/********************************************************************************
  Description:
	Set marb Thumbnail buffer start address.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetTbufStart(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_TBUF_START_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get Thumbnail buffer start address.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetTbufStart(void)
{
	return V5_GetReg(V5_REG_MARB_TBUF_START_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Set marb Thumbnail buffer end address.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetTbufEnd(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_TBUF_END_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get Thumbnail buffer end address.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetTbufEnd(void)
{
	return V5_GetReg(V5_REG_MARB_TBUF_END_0, BIT_24_HTOL);
}

UINT32 V5B_MarbGetTbufSize(void)
{
	UINT32 x, y;

	x = V5B_MarbGetTbufEnd();
	y = V5B_MarbGetTbufStart();
	return x - y + 4;
}

/********************************************************************************
  Description:
	Set marb AVI index buffer start address.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetIdxStart(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_IDX_START_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get AVI index buffer start address.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetIdxStart(void)
{
	return V5_GetReg(V5_REG_MARB_IDX_START_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Set marb AVI index buffer end address.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetIdxEnd(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_IDX_END_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get AVI index buffer end address.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetIdxEnd(void)
{
	return V5_GetReg(V5_REG_MARB_IDX_END_0, BIT_24_HTOL);
}

UINT32 V5B_MarbGetIdxSize(void)
{
	UINT32 x, y;

	x = V5B_MarbGetIdxEnd();
	y = V5B_MarbGetIdxStart();
	return x - y + 4;
}

/********************************************************************************
  Description:
	Set marb JPEG buffer interval.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetJbufInterval(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_JBUF_INTV_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get JPEG buffer interval.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetJbufInterval(void)
{
	return V5_GetReg(V5_REG_MARB_JBUF_INTV_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Set JPEG buffer FIFO counter upper threshold.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetJbufUp(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_JBUF_UP_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get JPEG buffer FIFO counter upper threshold.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetJbufUp(void)
{
	return V5_GetReg(V5_REG_MARB_JBUF_UP_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Set JPEG buffer FIFO counter lower threshold.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetJbufLow(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_JBUF_LOW_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get JPEG buffer FIFO counter lower threshold.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetJbufLow(void)
{
	return V5_GetReg(V5_REG_MARB_JBUF_LOW_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Set Thumbnail buffer interval.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetTbufInterval(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_TBUF_INTV_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get Thumbnail buffer interval.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetTbufInterval(void)
{
	return V5_GetReg(V5_REG_MARB_TBUF_INTV_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Set Thumbnail buffer FIFO counter upper threshold.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetTbufUp(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_TBUF_UP_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get Thumbnail buffer FIFO counter upper threshold.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetTbufUp(void)
{
	return V5_GetReg(V5_REG_MARB_TBUF_UP_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Set marb Thumbnail buffer FIFO counter lower threshold.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetTbufLow(UINT32 uVal)
{
	V5_SetReg(V5_REG_MARB_TBUF_LOW_0, uVal, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get Thumbnail buffer FIFO counter lower threshold.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetTbufLow(void)
{
	return V5_GetReg(V5_REG_MARB_TBUF_LOW_0, BIT_24_HTOL);
}


/********************************************************************************
  Description:
	Set frame interval for AVI or multi-shot file.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetFrameInterval(UINT8 uVal)
{
	V5_SetReg(V5_REG_MARB_FRAME_INTERV, uVal, BIT_8);
}

/********************************************************************************
  Description:
	Get  frame interval for AVI or multi-shot file.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT8 V5B_MarbGetFrameInterval(void)
{
	return (UINT8)V5_GetReg(V5_REG_MARB_FRAME_INTERV, BIT_8);
}

/********************************************************************************
  Description:
	Set memory control value.
  Parameters:
	uVal : value, see V5B_MARB_MEM_CTRLenum
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetMemCtrl(UINT8 uVal)
{
	V5_SetReg(V5_REG_MARB_MEMORY_CTRL, uVal, BIT_8);
}

UINT8 V5B_MarbGetMemCtrl(void)
{
	return (UINT8)V5_GetReg(V5_REG_MARB_MEMORY_CTRL, BIT_8);
}


UINT8 V5B_MarbMem2PowerOn(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x | MMC_POWER2));
	return x;
}

UINT8 V5B_MarbMem2PowerDown(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x & (~MMC_POWER2)));
	return x;
}

UINT8 V5B_MarbMem1PowerOn(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x | MMC_POWER1));
	return x;
}

UINT8 V5B_MarbMem1PowerDown(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x & (~MMC_POWER1)));
	return x;
}

UINT8 V5B_MarbMem0PowerOn(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x | MMC_POWER0));
	return x;
}

UINT8 V5B_MarbMem0PowerDown(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x & (~MMC_POWER0)));
	return x;
}

UINT8 V5B_MarbMem2ClkOn(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x & (~MMC_CLK2)));
	return x;
}

UINT8 V5B_MarbMem2ClkOff(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x | MMC_CLK2));
	return x;
}

UINT8 V5B_MarbMem1ClkOn(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x & (~MMC_CLK1)));
	return x;
}

UINT8 V5B_MarbMem1ClkOff(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x | MMC_CLK1));
	return x;
}

UINT8 V5B_MarbMem0ClkOn(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x & (~MMC_CLK0)));
	return x;
}

UINT8 V5B_MarbMem0ClkOff(void)
{
	UINT8 x;

	x = V5B_MarbGetMemCtrl();
	V5B_MarbSetMemCtrl((UINT8)(x | MMC_CLK0));
	return x;
}


/********************************************************************************
  Description:
	Set Multi-shot frame count value.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetMshotFcnt(UINT8 uVal)
{
	V5_SetReg(V5_REG_MARB_MSHOT_FCNT, uVal, BIT_8);
}

/********************************************************************************
  Description:
	Get  Multi-shot frame count value.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT8 V5B_MarbGetMshotFcnt(void)
{
	return (UINT8)V5_GetReg(V5_REG_MARB_MSHOT_FCNT, BIT_8);
}

/********************************************************************************
  Description:
	Set Multi-shot frame count value.
  Parameters:
	uVal : value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSetPriorTable(UINT8* uVal)
{
	UINT8 i;
	
	for(i = 0; i < 3; i++)
	{
		V5_SetReg(V5_REG_MARB_P_TABLE_0+i, uVal[i], BIT_8);
	}
}

/********************************************************************************
  Description:
	Get  Multi-shot frame count value.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbGetPriorTable(UINT8* uVal)
{
	UINT8 i;
	
	for(i = 0; i < 3; i++)
	{
		uVal[i] = (UINT8)V5_GetReg(V5_REG_MARB_P_TABLE_0+i, BIT_8);
	}
}

/********************************************************************************
  Description:
	Get JPEG buffer read/write pointer.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetJbufPointer(void)
{
	V5_SetReg(V5_REG_MARB_JBUF_PT_0, 0, BIT_8);
	return V5_GetReg(V5_REG_MARB_JBUF_PT_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get Thumbnail buffer read/write pointer.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetTbufPointer(void)
{
	V5_SetReg(V5_REG_MARB_TBUF_PT_0, 0, BIT_8);
	return V5_GetReg(V5_REG_MARB_TBUF_PT_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get Last frame JPEG file end point.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetLstJbufPointer(void)
{
	V5_SetReg(V5_REG_MARB_LAST_PT_0, 0, BIT_8);
	return V5_GetReg(V5_REG_MARB_LAST_PT_0, BIT_24_HTOL);
}

/********************************************************************************
  Description:
	Get Total frame counter.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetFrmCount(void)
{
	return V5_GetReg(V5_REG_MARB_FRAME_CNT_0, BIT_32_HTOL);
}

/********************************************************************************
  Description:
	Get Total jpeg file size.
  Parameters:
	uVal : the pointer to value
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetVideoSize(void)
{
	return V5_GetReg(V5_REG_MARB_VIDEO_SIZE_0, BIT_32_HTOL);
}

/********************************************************************************
  Description:
	Get Marb status.
  Parameters:
	uVal : the pointer to value, bit correspond see V5B_MARB_STATUSenum
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT8 V5B_MarbGetMarbStatus(void)
{
	return (UINT8)V5_GetReg(V5_REG_MARB_MARB_STAT_0, BIT_8);
}

/********************************************************************************
  Description:
	Get Marb sram write temp data.
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT32 V5B_MarbGetCurWData(void)
{
	return V5_GetReg(V5_REG_MARB_WRITE_DATA_0, BIT_32_HTOL);
}

void V5B_MarbSetClkDiv(UINT16 val)
{
	V5_SetReg(V5_REG_MARB_CLK32_DIV_0, val, BIT_16_HTOL);
}

UINT16 V5B_MarbGetClkDiv(void)
{
	return (UINT16)V5_GetReg(V5_REG_MARB_CLK32_DIV_0, BIT_16_HTOL);
}

/********************************************************************************
  Description:
	Set Marb one T ctrl
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
void V5B_MarbSet1TCtrl(UINT8 val)
{
	V5_SetReg(V5_REG_MARB_1T_CTRL1, val, BIT_8);
}

/********************************************************************************
  Description:
	Get Marb one T ctrl
  Parameters:
  Note:
  Returns:
  Remarks:
*********************************************************************************/
UINT8 V5B_MarbGet1TCtrl(void)
{
	return (UINT8)V5_GetReg(V5_REG_MARB_1T_CTRL1, BIT_8);
}

UINT8	V5B_Marb1TCtrlMem2ZzbOn(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x | MARB_1TCTRL_ZZB2));
	return x;
}

UINT8	V5B_Marb1TCtrlMem2ZzbOff(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x & (~MARB_1TCTRL_ZZB2)));
	return x;
}

UINT8	V5B_Marb1TCtrlMem1ZzbOn(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x | MARB_1TCTRL_ZZB1));
	return x;
}

UINT8	V5B_Marb1TCtrlMem1ZzbOff(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x & (~MARB_1TCTRL_ZZB1)));
	return x;
}

UINT8	V5B_Marb1TCtrlMem0ZzbOn(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x | MARB_1TCTRL_ZZB0));
	return x;
}

UINT8	V5B_Marb1TCtrlMem0ZzbOff(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x & (~MARB_1TCTRL_ZZB0)));
	return x;
}

UINT8	V5B_Marb1TCtrlMem2EccOn(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x | MARB_1TCTRL_ECC2));
	return x;
}

UINT8	V5B_Marb1TCtrlMem2EccOff(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x & (~MARB_1TCTRL_ECC2)));
	return x;
}

UINT8	V5B_Marb1TCtrlMem1EccOn(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x | MARB_1TCTRL_ECC1));
	return x;
}

UINT8	V5B_Marb1TCtrlMem1EccOff(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x & (~MARB_1TCTRL_ECC1)));
	return x;
}

UINT8	V5B_Marb1TCtrlMem0EccOn(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x | MARB_1TCTRL_ECC0));
	return x;
}

UINT8	V5B_Marb1TCtrlMem0EccOff(void)
{
	UINT8 x;

	x = V5B_MarbGet1TCtrl();
	V5B_MarbSet1TCtrl((UINT8)(x & (~MARB_1TCTRL_ECC0)));
	return x;
}


void V5B_MarbRpEnable(void)
{
	V5_SetReg(V5_REG_MARB_RECAP_CTRL, BIT0|BIT1, BIT_8);
}

void V5B_MarbRpDisable(void)
{
	V5_SetReg(V5_REG_MARB_RECAP_CTRL, 0x0, BIT_8);
}

void V5B_MarbSetRpThd(UINT8 val)
{
	V5_SetReg(V5_REG_MARB_RECAP_THRESHOLD, val, BIT_8);
}

UINT8 V5B_MarbGetRpThd(void)
{
	return (UINT8)V5_GetReg(V5_REG_MARB_RECAP_THRESHOLD, BIT_8);
}

UINT8 V5B_MarbGetRpActTimes(void)
{
	return (UINT8)V5_GetReg(V5_REG_MARB_RECAP_TIMES, BIT_8);
}

void V5B_MarbSetRpSize(UINT32 val)
{
	V5_SetReg(V5_REG_MARB_RECAP_FRM_SIZE0, (val>>2), BIT_32_HTOL);
}

UINT32 V5B_MarbGetRpSize(void)
{
	return V5_GetReg(V5_REG_MARB_RECAP_FRM_SIZE0, BIT_32_HTOL);
}

void V5B_MarbSetRefPoint(UINT8 val)
{
	V5_SetReg(V5_REG_MARB_REF_MEET_POINT, val, BIT_8);
}

UINT8 V5B_MarbGetRefPoint(void)
{
	return (UINT8)V5_GetReg(V5_REG_MARB_REF_MEET_POINT, BIT_8);
}

void V5B_MarbSetTimerBase(UINT16 val)
{
	V5_SetReg(V5_REG_MARB_SUB_TIMER0, val, BIT_16_HTOL);
}

void V5B_MarbSetTimerVal(UINT32 val)
{
	V5_SetReg(V5_REG_MARB_TIMER0, val, BIT_24_HTOL);
}

UINT16 V5B_MarbGetTimerBase(void)
{
	return (UINT16)V5_GetReg(V5_REG_MARB_SUB_TIMER0, BIT_16_HTOL);
}

UINT32 V5B_MarbGetTimerVal(void)
{
	return V5_GetReg(V5_REG_MARB_TIMER0, BIT_24_HTOL);
}

UINT16 V5B_MarbGetCurTimerBase(void)
{
	V5B_MarbLockTimer();
	return (UINT16)V5_GetReg(V5_REG_MARB_SUB_TIMER_VAL0, BIT_16_HTOL);
}

UINT32 V5B_MarbGetCurTimerVal(void)
{
	V5B_MarbLockTimer();
	return V5_GetReg(V5_REG_MARB_TIMER_VAL0, BIT_24_HTOL);
}

void V5B_MarbGetCurTimer(UINT16* base, UINT32* val)
{
	V5B_MarbLockTimer();
	*val = (UINT32)V5_GetReg(V5_REG_MARB_TIMER_VAL0, BIT_24_HTOL);
	*base = (UINT16)V5_GetReg(V5_REG_MARB_SUB_TIMER_VAL0, BIT_16_HTOL);
}

#endif //V578CMD_DISABLE
