#include "internal.h"

#include "../terminal/terminal/usb9604/driver/usbreg.h"



const TRegUnit gUsbCtrlReg[] =
{
	{ USB9604_REG, "USB_REG_MCNTRL", USB_REG_MCNTRL, 0, 0 },
	{ USB9604_REG, "USB_REG_CCONF", USB_REG_CCONF, 0, 0 },
	{ USB9604_REG, "USB_REG_RID", USB_REG_RID, 0, 0 },
	{ USB9604_REG, "USB_REG_NFSR", USB_REG_NFSR, 0, 0 },
	{ USB9604_REG, "USB_REG_MAEV", USB_REG_MAEV, 0, 0 },
	{ USB9604_REG, "USB_REG_MAMSK", USB_REG_MAMSK, 0, 0 },
	{ USB9604_REG, "USB_REG_ALTEV", USB_REG_ALTEV, 0, 0 },
	{ USB9604_REG, "USB_REG_ALTMSK", USB_REG_ALTMSK, 0, 0 },
	{ USB9604_REG, "USB_REG_TXEV", USB_REG_TXEV, 0, 0 },
	{ USB9604_REG, "USB_REG_TXMSK", USB_REG_TXMSK, 0, 0 },
	{ USB9604_REG, "USB_REG_RXEV", USB_REG_RXEV, 0, 0 },
	{ USB9604_REG, "USB_REG_RXMSK", USB_REG_RXMSK, 0, 0 },
	{ USB9604_REG, "USB_REG_NAKEV", USB_REG_NAKEV, 0, 0 },
	{ USB9604_REG, "USB_REG_NAKMSK", USB_REG_NAKMSK, 0, 0 }
};

const TRegUnit gUsbTransReg[] =
{
	{ USB9604_REG, "USB_REG_FWEV", USB_REG_FWEV, 0, 0 },
	{ USB9604_REG, "USB_REG_FWMSK", USB_REG_FWMSK, 0, 0 },
	{ USB9604_REG, "USB_REG_FNH", USB_REG_FNH, 0, 0 },
	{ USB9604_REG, "USB_REG_FNL", USB_REG_FNL, 0, 0 },
	{ USB9604_REG, "USB_REG_FAR", USB_REG_FAR, 0, 0 },
	{ USB9604_REG, "USB_REG_WKUP", USB_REG_WKUP, 0, 0 }
};

const TRegUnit gUsbDmaReg[] =
{
	{ USB9604_REG, "USB_REG_DMACNTRL", USB_REG_DMACNTRL, 0, 0 },
	{ USB9604_REG, "USB_REG_DMAEV", USB_REG_DMAEV, 0, 0 },
	{ USB9604_REG, "USB_REG_DMAMSK", USB_REG_DMAMSK, 0, 0 },
	{ USB9604_REG, "USB_REG_MIR", USB_REG_MIR, 0, 0 },
	{ USB9604_REG, "USB_REG_DMACNT", USB_REG_DMACNT, 0, 0 },
	{ USB9604_REG, "USB_REG_DMAERR", USB_REG_DMAERR, 0, 0 }
};

const TRegUnit gUsbEpc0Reg[] =
{
	{ USB9604_REG, "USB_REG_EPC0", USB_REG_EPC0, 0, 0 },
	{ USB9604_REG, "USB_REG_TXD0", USB_REG_TXD0, 0, 0 },
	{ USB9604_REG, "USB_REG_TXS0", USB_REG_TXS0, 0, 0 },
	{ USB9604_REG, "USB_REG_TXC0", USB_REG_TXC0, 0, 0 },
	{ USB9604_REG, "USB_REG_RXD0", USB_REG_RXD0, 0, 0 },
	{ USB9604_REG, "USB_REG_RXS0", USB_REG_RXS0, 0, 0 },
	{ USB9604_REG, "USB_REG_RXC0", USB_REG_RXC0, 0, 0 }
};

const TRegUnit gUsbEpc1Reg[] =
{
	{ USB9604_REG, "USB_REG_EPC1", USB_REG_EPC1, 0, 0 },
	{ USB9604_REG, "USB_REG_TXD1", USB_REG_TXD1, 0, 0 },
	{ USB9604_REG, "USB_REG_TXS1", USB_REG_TXS1, 0, 0 },
	{ USB9604_REG, "USB_REG_TXC1", USB_REG_TXC1, 0, 0 },
	{ USB9604_REG, "USB_REG_EPC2", USB_REG_EPC2, 0, 0 },
	{ USB9604_REG, "USB_REG_RXD1", USB_REG_RXD1, 0, 0 },
	{ USB9604_REG, "USB_REG_RXS1", USB_REG_RXS1, 0, 0 },
	{ USB9604_REG, "USB_REG_RXC1", USB_REG_RXC1, 0, 0 }
};

const TRegUnit gUsbEpc2Reg[] =
{
	{ USB9604_REG, "USB_REG_EPC3", USB_REG_EPC3, 0, 0 },
	{ USB9604_REG, "USB_REG_TXD2", USB_REG_TXD2, 0, 0 },
	{ USB9604_REG, "USB_REG_TXS2", USB_REG_TXS2, 0, 0 },
	{ USB9604_REG, "USB_REG_TXC2", USB_REG_TXC2, 0, 0 },
	{ USB9604_REG, "USB_REG_EPC4", USB_REG_EPC4, 0, 0 },
	{ USB9604_REG, "USB_REG_RXD2", USB_REG_RXD2, 0, 0 },
	{ USB9604_REG, "USB_REG_RXS2", USB_REG_RXS2, 0, 0 },
	{ USB9604_REG, "USB_REG_RXC2", USB_REG_RXC2, 0, 0 }
};

const TRegUnit gUsbEpc3Reg[] =
{
	{ USB9604_REG, "USB_REG_EPC5", USB_REG_EPC5, 0, 0 },
	{ USB9604_REG, "USB_REG_TXD3", USB_REG_TXD3, 0, 0 },
	{ USB9604_REG, "USB_REG_TXS3", USB_REG_TXS3, 0, 0 },
	{ USB9604_REG, "USB_REG_TXC3", USB_REG_TXC3, 0, 0 },
	{ USB9604_REG, "USB_REG_EPC6", USB_REG_EPC6, 0, 0 },
	{ USB9604_REG, "USB_REG_RXD3", USB_REG_RXD3, 0, 0 },
	{ USB9604_REG, "USB_REG_RXS3", USB_REG_RXS3, 0, 0 },
	{ USB9604_REG, "USB_REG_RXC3", USB_REG_RXC3, 0, 0 }
};

const TRegList gUsb9604Reglist[] =
{
	{ "Ctrl Reg", (PTRegUnit)gUsbCtrlReg, sizeof(gUsbCtrlReg) / sizeof(TRegUnit) },
	{ "Trans Reg", (PTRegUnit)gUsbTransReg, sizeof(gUsbTransReg) / sizeof(TRegUnit) },
	{ "DMA Reg", (PTRegUnit)gUsbDmaReg, sizeof(gUsbDmaReg) / sizeof(TRegUnit) },
	{ "EPC0 Reg", (PTRegUnit)gUsbEpc0Reg, sizeof(gUsbEpc0Reg) / sizeof(TRegUnit) },
	{ "EPC1 Reg", (PTRegUnit)gUsbEpc1Reg, sizeof(gUsbEpc1Reg) / sizeof(TRegUnit) },
	{ "EPC2 Reg", (PTRegUnit)gUsbEpc2Reg, sizeof(gUsbEpc2Reg) / sizeof(TRegUnit) },
	{ "EPC3 Reg", (PTRegUnit)gUsbEpc3Reg, sizeof(gUsbEpc3Reg) / sizeof(TRegUnit) }
};

const int gUsb9604ReglistCount = sizeof(gUsb9604Reglist) / sizeof(TRegList);

