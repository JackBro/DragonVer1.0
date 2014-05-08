
#include "basefunc.h"
#ifndef	V868CMD_DISABLE

void V8B_SdifSetStoMode(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_STOMODE, x, BIT_8);
}

UINT8 V8B_SdifGetStoMode(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_STOMODE, BIT_8);
	return x;
}

UINT8 V8B_SdifEnableDmaMode(void)
{
	UINT8 x;

	x = V8B_SdifGetStoMode();
	V8B_SdifSetStoMode((UINT8)(x | SD_DMAMODE));
	return x;
}

UINT8 V8B_SdifDisableDmaMode(void)
{
	UINT8 x;

	x = V8B_SdifGetStoMode();
	V8B_SdifSetStoMode((UINT8)(x & ~SD_DMAMODE));
	return x;
}

UINT8 V8B_SdifEnableSpi(void)
{
	UINT8 x;

	x = V8B_SdifGetStoMode();
	V8B_SdifSetStoMode((UINT8)(x | SD_SPIEN));
	return x;
}

UINT8 V8B_SdifDisableSpi(void)
{
	UINT8 x;

	x = V8B_SdifGetStoMode();
	V8B_SdifSetStoMode((UINT8)(x & ~SD_SPIEN));
	return x;
}

UINT8 V8B_SdifEnableTrans4(void)
{
	UINT8 x;

	x = V8B_SdifGetStoMode();
	V8B_SdifSetStoMode((UINT8)(x | SD_TRANS4));
	return x;
}

UINT8 V8B_SdifDisableTrans4(void)
{
	UINT8 x;

	x = V8B_SdifGetStoMode();
	V8B_SdifSetStoMode((UINT8)(x & ~SD_TRANS4));
	return x;
}

//===========================================

void V8B_SdifSoftReset(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_SOFTRESET, x, BIT_8);
}

void V8B_SdifRegReset(void)
{
	V8B_SdifSoftReset(SD_REG_RST);
}

void V8B_SdifCtrlReset(void)
{
	V8B_SdifSoftReset(SD_CTRL_RST);
}

void V8B_SdifBufClear(void)
{
	V8B_SdifSoftReset(SD_BUF_CLR);
}

void V8B_SdifSetHostBufRd(void)
{
	V8B_SdifSoftReset(SD_HOST_BUF_RD);
}

UINT8 V8B_SdifGetHostBufRd( void )
{
	return (UINT8)(V8_GetReg(V8_REG_SDIF_SOFTRESET, BIT_8) & SD_HOST_BUF_RD);
}

//===========================================

void V8B_SdifSetBlkSize(UINT16 x)
{
	V8_SetReg(V8_REG_SDIF_BLKSIZE_H, x, BIT_16_HTOL);
}

UINT16 V8B_SdifGetBlkSize(void)
{
	UINT16 x;

	x = (UINT16)V8_GetReg(V8_REG_SDIF_BLKSIZE_H, BIT_16_HTOL);

	return x;
}

//===========================================

void V8B_SdifSetBlkCnt(UINT16 x)
{
	V8_SetReg(V8_REG_SDIF_TRANSBLKCNT_H, x, BIT_16_HTOL);
}

UINT16 V8B_SdifGetBlkCnt(void)
{
	UINT16 x;

	x = (UINT16)V8_GetReg(V8_REG_SDIF_TRANSBLKCNT_H, BIT_16_HTOL);

	return x;
}

//===========================================

void V8B_SdifSetCmdArgu(UINT32 x)
{

	V8_SetReg(V8_REG_SDIF_CMDARGU_3, x, BIT_32_HTOL);
}

UINT32 V8B_SdifGetCmdArgu(void)
{
	UINT32 x;

	x = V8_GetReg(V8_REG_SDIF_CMDARGU_3, BIT_32_HTOL);

	return x;
}

//===========================================
void V8B_SdifSetCmdCtrl0(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_CMDCTRL0, x, BIT_8);
}

UINT8 V8B_SdifGetCmdCtrl0(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_CMDCTRL0, BIT_8);

	return x;
}

void V8B_SdifSetCmdIndex(UINT8 x)
{
	V8B_SdifSetCmdCtrl0( x );
}

UINT8 V8B_SdifGetCmdIndex(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl0();

	return x;
}

//===========================================

void V8B_SdifSetCmdCtrl1(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_CMDCTRL1, x, BIT_8);
}

UINT8 V8B_SdifGetCmdCtrl1(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_CMDCTRL1, BIT_8);

	return x;
}

void V8B_SdifSetRespType(UINT8 sel)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	sel &= (UINT8)(SD_CHKCRCEN - 1);
	x &= 0xfc;
	V8B_SdifSetCmdCtrl1(x | sel);
}

UINT8 V8B_SdifGetRespType(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	return x & (UINT8)(SD_CHKCRCEN - 1);
}

UINT8 V8B_SdifEnableCrcChk(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	V8B_SdifSetCmdCtrl1((UINT8)(x | SD_CHKCRCEN));
	return x;
}

UINT8 V8B_SdifDisableCrcChk(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	V8B_SdifSetCmdCtrl1((UINT8)(x & ~SD_CHKCRCEN));
	return x;
}

UINT8 V8B_SdifEnableInxChk(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	V8B_SdifSetCmdCtrl1((UINT8)(x | SD_CHKINXEN));
	return x;
}

UINT8 V8B_SdifDisableInxChk(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	V8B_SdifSetCmdCtrl1((UINT8)(x & ~SD_CHKINXEN));
	return x;
}

UINT8 V8B_SdifEnableReadDirc(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	V8B_SdifSetCmdCtrl1((UINT8)(x | SD_RDDIRC));
	return x;
}

UINT8 V8B_SdifDisableReadDirc(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	V8B_SdifSetCmdCtrl1((UINT8)(x & ~SD_RDDIRC));
	return x;
}

UINT8 V8B_SdifEnableDataPresent(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	V8B_SdifSetCmdCtrl1((UINT8)(x | SD_DATAPRENT));
	return x;
}

UINT8 V8B_SdifDisableDataPresent(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	V8B_SdifSetCmdCtrl1((UINT8)(x & ~SD_DATAPRENT));
	return x;
}

UINT8 V8B_SdifEnableAutoCmd12(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	V8B_SdifSetCmdCtrl1((UINT8)(x & ~SD_AUTOCMD12EN));
	return x;
}

UINT8 V8B_SdifDisableAutoCmd12(void)
{
	UINT8 x;

	x = V8B_SdifGetCmdCtrl1();
	V8B_SdifSetCmdCtrl1((UINT8)(x | SD_AUTOCMD12EN));
	return x;
}

//===========================================

void V8B_SdifGetResp(UINT8 *x)
{
	UINT32 i;

	for(i = 0; i < 16; i++)
	{
		x[i] = (UINT8)V8_GetReg(V8_REG_SDIF_RESPREG_00 - i, BIT_8);
	}
}

//===========================================

void V8B_SdifWrPort(UINT32 x)
{	
	V8_SetReg(V8_REG_SDIF_DATAPORT_3, x, BIT_32_HTOL);
}

UINT32 V8B_SdifRdPort(void)
{
	UINT32 x;

	x = V8_GetReg(V8_REG_SDIF_DATAPORT_3, BIT_32_HTOL);

	return x;;
}

//===========================================

void V8B_SdifSetDmaCtrl(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_DMACTRL, x, BIT_8);
}

UINT8 V8B_SdifGetDmaCtrl(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_DMACTRL, BIT_8);

	return x;
}

UINT8 V8B_SdifGetCtrlState(void)
{
	UINT8 x;

	x = V8B_SdifGetDmaCtrl();

	return (x >> 2) & 0xf;
}

UINT8 V8B_SdifEnableDmaStart(void)
{
	UINT8 x;

	x = V8B_SdifGetDmaCtrl();
	V8B_SdifSetDmaCtrl((UINT8)(x | SD_DMASTART));
	return x;
}

UINT8 V8B_SdifDisableDmaStart(void)
{
	UINT8 x;

	x = V8B_SdifGetDmaCtrl();
	V8B_SdifSetDmaCtrl((UINT8)(x & ~SD_DMASTART));
	return x;
}

UINT8 V8B_SdifEnableTranStop(void)
{
	UINT8 x;

	x = V8B_SdifGetDmaCtrl();
	V8B_SdifSetDmaCtrl((UINT8)(x | SD_TRANSTOP));
	return x;
}

UINT8 V8B_SdifDisableTranStop(void)
{
	UINT8 x;

	x = V8B_SdifGetDmaCtrl();
	V8B_SdifSetDmaCtrl((UINT8)(x & ~SD_TRANSTOP));
	return x;
}

//===========================================

UINT16 V8B_SdifGetPresentState(void)
{
	UINT16 x;

	x = (UINT16)V8_GetReg(V8_REG_SDIF_PRESTATE_L, BIT_16_LTOH);

	return x;
}

//===========================================

void V8B_SdifSetClkCtrl(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_CLKCTRL, x, BIT_8);
}

UINT8 V8B_SdifGetClkCtrl(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_CLKCTRL, BIT_8);

	return x;
}

//===========================================

UINT16 V8B_SdifGetNorIntStatus(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_NORINT_STATUS, BIT_8);

	return x;
}

//===========================================

UINT16 V8B_SdifGetErr1IntStatus(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_ERR1INT_STATUS, BIT_8);

	return x;
}

//===========================================

UINT16 V8B_SdifGetErr2IntStatus(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_ERR2INT_STATUS, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetNorIntMask(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_NORINT_MASK, x, BIT_8);
}

UINT8 V8B_SdifGetNorIntMask(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_NORINT_MASK, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetErr1IntMask(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_ERR1INT_MASK, x, BIT_8);
}

UINT8 V8B_SdifGetErr1IntMask(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_ERR1INT_MASK, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetErr2IntMask(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_ERR2INT_MASK, x, BIT_8);
}

UINT8 V8B_SdifGetErr2IntMask(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_ERR2INT_MASK, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetRespTmout(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_RESP_TMOUTCNT, x, BIT_8);
}

UINT8 V8B_SdifGetRespTmout(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_RESP_TMOUTCNT, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetBusyTmout(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_BUSYR1B_TMOUT, x, BIT_8);
}

UINT8 V8B_SdifGetBusyTmout(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_BUSYR1B_TMOUT, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetBusyCrcTmout(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_BUSYCRC_TMOUT, x, BIT_8);
}

UINT8 V8B_SdifGetBusyCrcTmout(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_BUSYCRC_TMOUT, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetWrCrcTmout(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_WRITECRC_TMOUT, x, BIT_8);
}

UINT8 V8B_SdifGetWrCrcTmout(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_WRITECRC_TMOUT, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetRdTmout(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_READ_TMOUT, x, BIT_8);
}

UINT8 V8B_SdifGetRdTmout(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_READ_TMOUT, BIT_8);

	return x;
}

//===========================================

UINT8 V8B_SdifGetCardAct(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_CARD_ACTLVL, BIT_8);
	return x;
}

//===========================================

void V8B_SdifSetRdSbCmdIndex(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_RDSBCMD_INDEX, x, BIT_8);
}

UINT8 V8B_SdifGetRdSbCmdIndex(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_RDSBCMD_INDEX, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetRdMbCmdIndex(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_RDMBCMD_INDEX, x, BIT_8);
}

UINT8 V8B_SdifGetRdMbCmdIndex(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_RDMBCMD_INDEX, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetWrSbCmdIndex(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_WRSBCMD_INDEX, x, BIT_8);
}

UINT8 V8B_SdifGetWrSbCmdIndex(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_WRSBCMD_INDEX, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetWrMbCmdIndex(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_WRMBCMD_INDEX, x, BIT_8);
}

UINT8 V8B_SdifGetWrMbCmdIndex(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_WRMBCMD_INDEX, BIT_8);

	return x;
}

//===========================================

void V8B_SdifSetIdRdRespDma(UINT32 x)
{
	V8_SetReg(V8_REG_SDIF_IDRDRESP_DMA3, x, BIT_32_HTOL);
}

UINT32 V8B_SdifGetIdRdRespDma(void)
{
	UINT32 val;

	val = V8_GetReg(V8_REG_SDIF_IDRDRESP_DMA3, BIT_32_HTOL);
	
	return val;
}

//===========================================

void V8B_SdifSetIdWrRespDma(UINT32 x)
{
	V8_SetReg(V8_REG_SDIF_IDWRRESP_DMA3, x, BIT_32_HTOL);
}

UINT32 V8B_SdifGetIdWrRespDma(void)
{
	UINT32 val;

	val = V8_GetReg(V8_REG_SDIF_IDWRRESP_DMA3, BIT_32_HTOL);
	
	return val;
}

//===========================================

void V8B_SdifSetCrcErrThrd(UINT8 x)
{
	V8_SetReg(V8_REG_SDIF_CRC_THRD, x, BIT_8);
}

UINT8 V8B_SdifGetCrcErrThrd(void)
{
	UINT8 x;

	x = (UINT8)V8_GetReg(V8_REG_SDIF_CRC_THRD, BIT_8);

	return x;
}

#endif	//V868CMD_DISABLE
