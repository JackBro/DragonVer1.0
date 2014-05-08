#include "internal.h"
#ifndef USBCMD_DISABLE

void Usb_BitClr(UINT8 adr, UINT8 mask)
{
	UINT8 val;

	val = Usb_GetReg(adr);
	val &= ~mask;
	Usb_SetReg(adr, val);
}

void Usb_BitSet(UINT8 adr, UINT8 mask)
{
	UINT8 val;

	val = Usb_GetReg(adr);
	val |= mask;
	Usb_SetReg(adr, val);
}

/*-------------------------------- MCNTRL ------------------------*/
void Usb_SetMainCtrl(UINT8 val)
{
	Usb_SetReg(USB_REG_MCNTRL, val);
}

UINT8 Usb_GetMainCtrl(void)
{
	return Usb_GetReg(USB_REG_MCNTRL);
}

void Usb_SoftReset(void)
{
	Usb_SetMainCtrl(SRST);
}

UINT8 Usb_EnableVoltRegulator(void)
{
	UINT8 x;

	x = Usb_GetMainCtrl();
	Usb_SetMainCtrl(x | VGE);
	return x;
}

UINT8 Usb_DisableVoltRegulator(void)
{
	UINT8 x;

	x = Usb_GetMainCtrl();
	Usb_SetMainCtrl(x & ~VGE);
	return x;
}

UINT8 Usb_EnableNodeAttached(void)
{
	UINT8 x;

	x = Usb_GetMainCtrl();
	Usb_SetMainCtrl(x | NAT);
	return x;
}

UINT8 Usb_DisableNodeAttached(void)
{
	UINT8 x;

	x = Usb_GetMainCtrl();
	Usb_SetMainCtrl(x & ~NAT);
	return x;
}

void Usb_SetInterruptCtrl(UINT8 val)
{
	UINT8 x;

	x = Usb_GetMainCtrl();
	x &= 0x3f;
	val &= 0xc0;
	Usb_SetMainCtrl(x | val);
}

UINT8 Usb_GetInterruptCtrl(void)
{
	UINT8 x;

	x = Usb_GetMainCtrl();
	return x & 0xc0;
}

/*---------------------------------- Clock control -------------------------*/
void Usb_SetClkCtrl(UINT8 val)
{
	Usb_SetReg(USB_REG_CCONF, val);
}

UINT8 Usb_GetClkCtrl(void)
{
	return Usb_GetReg(USB_REG_CCONF);
}

void Usb_SetClkDiv(UINT8 val)
{
	UINT8 x;

	x = Usb_GetClkCtrl();
	x &= 0x80;
	val &= 0xf;
	Usb_SetClkCtrl(x | val);
}

UINT8 Usb_GetClkDiv(void)
{
	UINT8 x;

	x = Usb_GetClkCtrl();
	return x & 0xf;
}

UINT8 Usb_DisableClkOutput(void)
{
	UINT8 x;

	x = Usb_GetClkCtrl();
	Usb_SetClkCtrl(x | CODIS);
	return x;
}

UINT8 Usb_EnableClkOutput(void)
{
	UINT8 x;

	x = Usb_GetClkCtrl();
	Usb_SetClkCtrl(x & ~CODIS);
	return x;
}

/*---------------------------------- Revision -------------------------*/
UINT8 Usb_GetRevision(void)
{
	return Usb_GetReg(USB_REG_RID);
}

/*--------------------------------- NFSR -------------------------*/
void Usb_SetNodeState(UINT8 val)
{
	Usb_SetReg(USB_REG_NFSR, val);
}

UINT8 Usb_GetNodeState(void)
{
	return Usb_GetReg(USB_REG_NFSR);
}

/*---------------------------- MAEV -----------------------*/
UINT8 Usb_GetMainEvent(void)
{
	return Usb_GetReg(USB_REG_MAEV);
}

/*---------------------------- MAMSK -----------------------*/
void Usb_SetMainEventMask(UINT8 val)
{
	Usb_SetReg(USB_REG_MAMSK, val);
}

UINT8 Usb_GetMainEventMask(void)
{
	return Usb_GetReg(USB_REG_MAMSK);
}

/*---------------------------- ALTEV -----------------------*/
UINT8 Usb_GetAltEvent(void)
{
	return Usb_GetReg(USB_REG_ALTEV);
}

/*---------------------------- ALTMSK -----------------------*/
void Usb_SetAltEventMask(UINT8 val)
{
	Usb_SetReg(USB_REG_ALTMSK, val);
}

UINT8 Usb_GetAltEventMask(void)
{
	return Usb_GetReg(USB_REG_ALTMSK);
}

/*---------------------------- TXEV -----------------------*/
UINT8 Usb_GetTransEvent(void)
{
	return Usb_GetReg(USB_REG_TXEV);
}

/*---------------------------- TXMSK -----------------------*/
void Usb_SetTransEventMask(UINT8 val)
{
	Usb_SetReg(USB_REG_TXMSK, val);
}

UINT8 Usb_GetTransEventMask(void)
{
	return Usb_GetReg(USB_REG_TXMSK);
}

/*---------------------------- RXEV -----------------------*/
UINT8 Usb_GetRcvEvent(void)
{
	return Usb_GetReg(USB_REG_RXEV);
}

/*---------------------------- RXMSK -----------------------*/
void Usb_SetRcvEventMask(UINT8 val)
{
	Usb_SetReg(USB_REG_RXMSK, val);
}

UINT8 Usb_GetRcvEventMask(void)
{
	return Usb_GetReg(USB_REG_RXMSK);
}

/*---------------------------- NAKEV -----------------------*/
UINT8 Usb_GetNakEvent(void)
{
	return Usb_GetReg(USB_REG_NAKEV);
}

/*---------------------------- NAKMSK -----------------------*/
void Usb_SetNakEventMask(UINT8 val)
{
	Usb_SetReg(USB_REG_NAKMSK, val);
}

UINT8 Usb_GetNakEventMask(void)
{
	return Usb_GetReg(USB_REG_NAKMSK);
}

/*---------------------------- FWEV -----------------------*/
UINT8 Usb_GetFifoWEvent(void)
{
	return Usb_GetReg(USB_REG_FWEV);
}

/*---------------------------- FWMSK -----------------------*/
void Usb_SetFifoWEventMask(UINT8 val)
{
	Usb_SetReg(USB_REG_FWMSK, val);
}

UINT8 Usb_GetFifoWEventMask(void)
{
	return Usb_GetReg(USB_REG_FWMSK);
}

/*---------------------------- FN -----------------------*/
UINT16 Usb_GetFrameNumber(void)
{
	UINT8 x1, x2;

	x1 = Usb_GetReg(USB_REG_FNL);
	x2 = Usb_GetReg(USB_REG_FNH);
	x2 &= 0x7;
	return ((UINT16)x2 << 8) | ((UINT16)x1 & 0xff);
}

void Usb_ResetFrameCount(void)
{
	Usb_SetReg(USB_REG_FNL, RFC);
}

/*---------------------------- FAR -----------------------*/
void Usb_SetFuncAddr(UINT8 val)
{
	Usb_SetReg(USB_REG_FAR, val);
}

UINT8 Usb_GetFuncAddr(void)
{
	return Usb_GetReg(USB_REG_FAR);
}

void Usb_SetAddress(UINT8 val)
{
	UINT8 x;

	x = Usb_GetFuncAddr();
	x &= AD_EN;
	val &= ~AD_EN;
	Usb_SetFuncAddr(x | val);
}

UINT8 Usb_GetAddress(void)
{
	UINT8 x;

	x = Usb_GetFuncAddr();
	return x & ~AD_EN;
}

UINT8 Usb_EnableAddress(void)
{
	UINT8 x;

	x = Usb_GetFuncAddr();
	Usb_SetFuncAddr(x | AD_EN);
	return x;
}

UINT8 Usb_DisableAddress(void)
{
	UINT8 x;

	x = Usb_GetFuncAddr();
	Usb_SetFuncAddr(x | ~AD_EN);
	return x;
}

/*---------------------------- DMACTRL -----------------------*/
void Usb_SetDmaCtrl(UINT8 val)
{
	Usb_SetReg(USB_REG_DMACNTRL, val);
}

UINT8 Usb_GetDmaCtrl(void)
{
	return Usb_GetReg(USB_REG_DMACNTRL);
}

/*---------------------------- DMAEV -----------------------*/
void Usb_SetDmaEvent(UINT8 val)
{
	Usb_SetReg(USB_REG_DMAEV, val);
}

UINT8 Usb_GetDmaEvent(void)
{
	return Usb_GetReg(USB_REG_DMAEV);
}

/*---------------------------- DMAMSK -----------------------*/
void Usb_SetDmaEventMask(UINT8 val)
{
	Usb_SetReg(USB_REG_DMAMSK, val);
}

UINT8 Usb_GetDmaEventMask(void)
{
	return Usb_GetReg(USB_REG_DMAMSK);
}

/*---------------------------- DMACNT -----------------------*/
void Usb_SetDmaCount(UINT8 val)
{
	Usb_SetReg(USB_REG_DMACNT, val);
}

UINT8 Usb_GetDmaCount(void)
{
	return Usb_GetReg(USB_REG_DMACNT);
}

/*---------------------------- DMAERR -----------------------*/
void Usb_SetDmaError(UINT8 val)
{
	Usb_SetReg(USB_REG_DMAERR, val);
}

UINT8 Usb_GetDmaError(void)
{
	return Usb_GetReg(USB_REG_DMAERR);
}

/*---------------------------- WKUP -----------------------*/
void Usb_SetWakeup(UINT8 val)
{
	Usb_SetReg(USB_REG_WKUP, val);
}

UINT8 Usb_GetWakeup(void)
{
	return Usb_GetReg(USB_REG_WKUP);
}


//===================
/*----------------------- Transfer register ---------------------*/
//===================

static const UINT8 gTxEpcAdr[] = { USB_REG_EPC0, USB_REG_EPC1, USB_REG_EPC3, USB_REG_EPC5 };
static const UINT8 gRxEpcAdr[] = { USB_REG_EPC0, USB_REG_EPC2, USB_REG_EPC4, USB_REG_EPC6 };
static const UINT8 gEpTxStatusAdr[] = { USB_REG_TXS0, USB_REG_TXS1,	USB_REG_TXS2, USB_REG_TXS3 };
static const UINT8 gEpTxCmdAdr[] = { USB_REG_TXC0, USB_REG_TXC1, USB_REG_TXC2, USB_REG_TXC3};
static const UINT8 gEpTxDataAdr[] = { USB_REG_TXD0, USB_REG_TXD1, USB_REG_TXD2, USB_REG_TXD3};
static const UINT8 gEpRxStatusAdr[] = { USB_REG_RXS0, USB_REG_RXS1,	USB_REG_RXS2, USB_REG_RXS3};
static const UINT8 gEpRxCmdAdr[] = { USB_REG_RXC0, USB_REG_RXC1, USB_REG_RXC2, USB_REG_RXC3};
static const UINT8 gEpRxDataAdr[] = { USB_REG_RXD0, USB_REG_RXD1, USB_REG_RXD2, USB_REG_RXD3};

//===================
/*---------------------------- TXEPC0 - TXEPC3 -----------------------*/
//===================
void Usb_SetTxEpc(UINT8 index, UINT8 val)
{
	if(index >= sizeof(gTxEpcAdr) / sizeof(UINT8))
		index = 0;
	Usb_SetReg(gTxEpcAdr[index], val);
}

UINT8 Usb_GetTxEpc(UINT8 index)
{
	if(index >= sizeof(gTxEpcAdr) / sizeof(UINT8))
		index = 0;
	return Usb_GetReg(gTxEpcAdr[index]);
}

UINT8 Usb_EnableTxStall(UINT8 index)
{
	UINT8 x;

	x = Usb_GetTxEpc(index);
	Usb_SetTxEpc(index, x | STALL);
	return x;
}

UINT8 Usb_DisableTxStall(UINT8 index)
{
	UINT8 x;

	x = Usb_GetTxEpc(index);
	Usb_SetTxEpc(index, x & ~STALL);
	return x;
}

/*---------------------------- RXEPC0 - RXEPC3 -----------------------*/
void Usb_SetRxEpc(UINT8 index, UINT8 val)
{
	if(index >= sizeof(gRxEpcAdr) / sizeof(UINT8))
		index = 0;
	Usb_SetReg(gRxEpcAdr[index], val);
}

UINT8 Usb_GetRxEpc(UINT8 index)
{
	if(index >= sizeof(gRxEpcAdr) / sizeof(UINT8))
		index = 0;
	return Usb_GetReg(gRxEpcAdr[index]);
}

UINT8 Usb_EnableRxStall(UINT8 index)
{
	UINT8 x;

	x = Usb_GetRxEpc(index);
	Usb_SetRxEpc(index, x | STALL);
	return x;
}

UINT8 Usb_DisableRxStall(UINT8 index)
{
	UINT8 x;

	x = Usb_GetRxEpc(index);
	Usb_SetRxEpc(index, x & ~STALL);
	return x;
}

UINT8 Usb_DefEpc0(void)
{
	UINT8 x;

	x = Usb_GetRxEpc(0);
	Usb_SetRxEpc(0, x | DEF);
	return x;
}

/*---------------------------- TXS1 - TXS3 -----------------------*/
UINT8 Usb_GetTxStatus(UINT8 index)
{
	if(index >= sizeof(gEpTxStatusAdr) / sizeof(UINT8))
		index = 0;
	return Usb_GetReg(gEpTxStatusAdr[index]);
}

/*---------------------------- TXC1 - TXC3 -----------------------*/
void Usb_SetTxCmd(UINT8 index, UINT8 val)
{
	if(index >= sizeof(gEpTxCmdAdr) / sizeof(UINT8))
		index = 0;
	Usb_SetReg(gEpTxCmdAdr[index], val);
}

UINT8 Usb_GetTxCmd(UINT8 index)
{
	if(index >= sizeof(gEpTxCmdAdr) / sizeof(UINT8))
		index = 0;
	return Usb_GetReg(gEpTxCmdAdr[index]);
}

UINT8 Usb_EnableTx(UINT8 index)
{
	UINT8 x;

	x = Usb_GetTxCmd(index);
	Usb_SetTxCmd(index, x | TX_EN);
	return x;
}

UINT8 Usb_DisableTx(UINT8 index)
{
	UINT8 x;

	x = Usb_GetTxCmd(index);
	Usb_SetTxCmd(index, x & ~TX_EN);
	return x;
}

UINT8 Usb_FlushTx(UINT8 index)
{
	UINT8 x;

	x = Usb_GetTxCmd(index);
	Usb_SetTxCmd(index, x | FLUSH);
	return x;
}

/*---------------------------- TXD1 - TXD3 -----------------------*/
void Usb_SetTxData(UINT8 index, UINT8 val)
{
	if(index >= sizeof(gEpTxDataAdr) / sizeof(UINT8))
		index = 0;
	Usb_SetReg(gEpTxDataAdr[index], val);
}

UINT8 Usb_GetTxData(UINT8 index)
{
	if(index >= sizeof(gEpTxDataAdr) / sizeof(UINT8))
		index = 0;
	return Usb_GetReg(gEpTxDataAdr[index]);
}

/*---------------------------- RXS1 - RXS3 -----------------------*/
UINT8 Usb_GetRxStatus(UINT8 index)
{
	if(index >= sizeof(gEpRxStatusAdr) / sizeof(UINT8))
		index = 0;
	return Usb_GetReg(gEpRxStatusAdr[index]);
}

/*---------------------------- RXC1 - RXC3 -----------------------*/
void Usb_SetRxCmd(UINT8 index, UINT8 val)
{
	if(index >= sizeof(gEpRxCmdAdr) / sizeof(UINT8))
		index = 0;
	Usb_SetReg(gEpRxCmdAdr[index], val);
}

UINT8 Usb_GetRxCmd(UINT8 index)
{
	if(index >= sizeof(gEpRxCmdAdr) / sizeof(UINT8))
		index = 0;
	return Usb_GetReg(gEpRxCmdAdr[index]);
}

UINT8 Usb_EnableRx(UINT8 index)
{
	UINT8 x;

	x = Usb_GetRxCmd(index);
	Usb_SetRxCmd(index, x | RX_EN);
	return x;
}

UINT8 Usb_DisableRx(UINT8 index)
{
	UINT8 x;

	x = Usb_GetRxCmd(index);
	Usb_SetRxCmd(index, x & ~RX_EN);
	return x;
}

UINT8 Usb_FlushRx(UINT8 index)
{
	UINT8 x;

	x = Usb_GetRxCmd(index);
	Usb_SetRxCmd(index, x | FLUSH);
	return x;
}

/*---------------------------- RXD1- RXD3 -----------------------*/
void Usb_SetRxData(UINT8 index, UINT8 val)
{
	if(index >= sizeof(gEpRxDataAdr) / sizeof(UINT8))
		index = 0;
	Usb_SetReg(gEpRxDataAdr[index], val);
}

UINT8 Usb_GetRxData(UINT8 index)
{
	if(index >= sizeof(gEpRxDataAdr) / sizeof(UINT8))
		index = 0;
	return Usb_GetReg(gEpRxDataAdr[index]);
}

#else
void nullUsbbase(void)
{
}

#endif
