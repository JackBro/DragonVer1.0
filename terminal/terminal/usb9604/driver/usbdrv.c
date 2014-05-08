#include "internal.h"
#ifndef USBCMD_DISABLE

#define USB_CONT_BUFSIZE	0x2000
#define USB_BULK_BUFSIZE	0x20000
#define USB_RX_THD_UP		0x100
#define USB_RX_THD_DOWN		0x200

static UINT8 gUsbPipe0InBuf[USB_CONT_BUFSIZE];
static UINT8 gUsbPipe0OutBuf[USB_CONT_BUFSIZE];
static UINT8 gUsbPipe1InBuf[USB_CONT_BUFSIZE];
static UINT8 gUsbPipe1OutBuf[USB_CONT_BUFSIZE];
static UINT8 gUsbPipe2InBuf[USB_BULK_BUFSIZE];
static UINT8 gUsbPipe2OutBuf[USB_BULK_BUFSIZE];
static UINT8 gUsbPipe3InBuf[USB_CONT_BUFSIZE];
static UINT8 gUsbPipe3OutBuf[USB_CONT_BUFSIZE];

static UINT8* gUsbBuf[USB_TOTALPIPE<<1] = {
	gUsbPipe0InBuf,
	gUsbPipe0OutBuf,
	gUsbPipe1InBuf,
	gUsbPipe1OutBuf,
	gUsbPipe2InBuf,
	gUsbPipe2OutBuf,
	gUsbPipe3InBuf,
	gUsbPipe3OutBuf
};

static UINT32 gUsbBufSize[USB_TOTALPIPE<<1] = {
	USB_CONT_BUFSIZE,
	USB_CONT_BUFSIZE,
	USB_CONT_BUFSIZE,
	USB_CONT_BUFSIZE,
	USB_BULK_BUFSIZE,
	USB_BULK_BUFSIZE,
	USB_CONT_BUFSIZE,
	USB_CONT_BUFSIZE
};

static TUsbContext gUsbContext;

const struct {
    UINT8	bLength;
    UINT8	bDescriptorType;
    UINT16	bstring;
} gLangStr = {sizeof(gLangStr), STRING_DESCRIPTOR, ENGLISH_US};

const TUsbStringDesc gMenuFacStr = 
	{ sizeof(MANUFACTURER_STR) + 2, STRING_DESCRIPTOR, MANUFACTURER_STR };
const TUsbStringDesc gProductStr = 
	{ sizeof(PRODUCT_STR) + 2, STRING_DESCRIPTOR, PRODUCT_STR };
const TUsbStringDesc gVersionStr = 
	{ sizeof(VERSION_STR) + 2, STRING_DESCRIPTOR, VERSION_STR };

static const TUsbStringDesc *gUsbStrDesc[] =
{
	(TUsbStringDesc *)&gLangStr, &gMenuFacStr, &gProductStr, &gVersionStr
};

static const TUsbDeviceDesc gUsbDevDesc =
{
    sizeof(TUsbDeviceDesc),
    DEVICE_DESCRIPTOR,
    USB_SPEC_VERSION,
    CLASS_NOT_DEFINED, 		/* CLASS_VENDOR, vendor specific */
    0,						/* Device Sub-Class */
    0,						/* Device Protocol */
    EP0_FIFO_SIZE,			/* Max Packet Size for EndPoint Zero*/
    VIMICRO_ID_VENDOR,
    VIMICRO_ID_PRODUCT,
    0x0101, 				/* device release number: 01.01 VIMICRO_BCDDEVICE */
    STR_MANUFACTURER,
    STR_PRODUCT,
    0,						/* Device's serial number */
    1						/* Num of configurations */
};

#define TOTAL_INTERFACE		1
#define SELECT_CONFIG		1

static const TUsbLongCfgDesc gUsbLongCfgDesc =
{
	{
		sizeof(TUsbConfigDesc),
		CONFIG_DESCRIPTOR,
		(UINT8)sizeof(TUsbLongCfgDesc),
		(UINT8)(sizeof(TUsbLongCfgDesc) >> 8),
		TOTAL_INTERFACE,
		SELECT_CONFIG,
		0,			//index of config description string
		0x80,		//self-power, modify it as 0xc0 use bus-power, bus-power need support getstatus
		0xf0		//max power: 480mA
	},
	{
		sizeof(TUsbIfDesc),
		INTERFACE_DESCRIPTOR,
		0,		/* The only interface concurrently supported by this configuration */
		0,		/* Alternate Setting */
		2,		//total endpoint number
		CLASS_VENDOR,	//vender interface
		0xff,
		0xff,
		0
	},
	{
		{
			sizeof(TUsbEndPointDesc),
			ENDPOINT_DESCRIPTOR,
			//{ 3, 0, IN_DIRECT },
			0x83,
			BULK_ENDPOINT,
			TX_BULK_EP_FIFO_SIZE, 0,
			0
		},
		{
			sizeof(TUsbEndPointDesc),
			ENDPOINT_DESCRIPTOR,
			//{ 4, 0, OUT_DIRECT },
			4,
			BULK_ENDPOINT,
			TX_BULK_EP_FIFO_SIZE, 0,
			0
		}
	}
#if TOTAL_INTERFACE == 2
	, {
		sizeof(TUsbIfDesc),
		INTERFACE_DESCRIPTOR,
		1,		/* The only interface concurrently supported by this configuration */
		0,		/* Alternate Setting */
		2,		//total endpoint number
		CLASS_MASS_STORAGE,	//interface0 act as U-disk
		2,		//ATAPI protocol
		0x50,	//bulk-only, look for in "usb mass-storage class spec overview"
		0
	},
	{
		{
			sizeof(TUsbEndPointDesc),
			ENDPOINT_DESCRIPTOR,
			//{ 1, 0, IN_DIRECT },
			0x81,
			BULK_ENDPOINT,
			TX_BULK_EP_FIFO_SIZE, 0,
			0
		},
		{
			sizeof(TUsbEndPointDesc),
			ENDPOINT_DESCRIPTOR,
			//{ 2, 0, OUT_DIRECT },
			0x2,
			BULK_ENDPOINT,
			TX_BULK_EP_FIFO_SIZE, 0,
			0
		}
	}
#endif
};

static const TUsbConfigDesc *gUsbCfgDesc[] = { &gUsbLongCfgDesc.devcfg };
static const TUsbEndPointDesc *gUsbEp[TOTAL_INTERFACE][USB_TOTALENDP] =
{
	{
		NULL,
		NULL,
		NULL,
		&(gUsbLongCfgDesc.if0ep[0]),
		&(gUsbLongCfgDesc.if0ep[1]),
		NULL,
		NULL
	}
#if 0
	, {
		NULL,
		&(gUsbLongCfgDesc.if1ep[0]),
		&(gUsbLongCfgDesc.if1ep[1]),
		NULL,
		NULL,
		NULL,
		NULL
	}
#endif
};

#ifdef WIN32
#include "ComChannel/simhost.h"
#else
#define SmPrintf	tmPrintf
#endif

//void UsbStructPrint(void)
//{
//	UINT8 *ptr;
//	int i, len;
//
//	SmPrintf("sizeof(TUsbStringDesc) = %d\n", sizeof(TUsbStringDesc));
//	SmPrintf("sizeof(TUsbRequest) = %d\n", sizeof(TUsbRequest));
//	SmPrintf("sizeof(TUsbDeviceDesc) = %d\n", sizeof(TUsbDeviceDesc));
//	SmPrintf("sizeof(TUsbLongCfgDesc) = %d\n", sizeof(TUsbLongCfgDesc));
//	SmPrintf("sizeof(TUsbConfigDesc) = %d\n", sizeof(TUsbConfigDesc));
//	SmPrintf("sizeof(TUsbEndPointDesc) = %d\n", sizeof(TUsbEndPointDesc));
//	SmPrintf("sizeof(TUsbIfDesc) = %d\n", sizeof(TUsbIfDesc));
//	SmPrintf("RECIPIENT_MASK = %d\n", RECIPIENT_MASK);
//	SmPrintf("REQTYPE_MASK = %d\n", REQTYPE_MASK);
//	SmPrintf("ENDPADR_MASK = %d\n", ENDPADR_MASK);
//
//	len = sizeof(TUsbLongCfgDesc);
//	ptr = (UINT8 *)&gUsbLongCfgDesc;
//	SmPrintf("\n=================\n gUsbLongCfgDesc =\n");
//	for(i = 0; i < len; i++)
//		SmPrintf("\t0x%x,\n", ptr[i]);
//	SmPrintf("==============\n");
//
//	len = sizeof(TUsbDeviceDesc);
//	ptr = (UINT8 *)&gUsbDevDesc;
//	SmPrintf("\n=================\n gUsbDevDesc =\n");
//	for(i = 0; i < len; i++)
//		SmPrintf("\t0x%x,\n", ptr[i]);
//	SmPrintf("==============\n");
//}

//=========================================
//
//		read, write fifo 
//
//==================================
static void __inline EnableTx(UINT8 index)
{
	if(index > USB_TOTALPIPE - 1)
		return;
	gUsbContext.txtoggle[index] = gUsbContext.txtoggle[index] ? 0 : TX_TOGL;
	gUsbContext.txbuf[index].status = FIFO_BUSY;
	if(index)
		Usb_SetTxCmd(index, TX_LAST | TX_EN | gUsbContext.txtoggle[index]);
	else
	{
		//tmPrintf("EnableTx: 0x%x\n", TX_EN | gUsbContext.txtoggle[0]);
		Usb_SetTxCmd(0, TX_EN | gUsbContext.txtoggle[0]);
		//Usb_SetRxCmd(0, 0); //l_yong delete it. when first when tx, rxdata num = 0. second when rx disable, nack is occur rx will be enabled. 
	}
}

static void __inline EnableRx(UINT8 index)
{
	if(index > USB_TOTALPIPE - 1)
		return;
	if(index)
	{
		Usb_SetRxCmd(index, RX_EN);
		return;
	}
	if(gUsbContext.txbuf[0].status == FIFO_BUSY)
		return;
	Usb_SetRxCmd(0, RX_EN);
}

static void __inline ReenableTx(UINT8 index)
{
	//tmPrintf("Re enable TX\n");
	if( (index > USB_TOTALPIPE - 1) || (index == 0) )
		return;
	gUsbContext.txtoggle[index] = gUsbContext.txtoggle[index] ? 0 : TX_TOGL;
	gUsbContext.txbuf[index].status = FIFO_BUSY;
	Usb_SetTxCmd(index, TX_LAST | TX_EN | gUsbContext.txtoggle[index] | RFF);
}

static __inline void ReadRx(TUsbBuf *pbuf, UINT8 cnt, UINT8 index)
{
	const UINT8 adr[] = { USB_REG_RXD0, USB_REG_RXD1, USB_REG_RXD2, USB_REG_RXD3 };
	UINT32 size;

	while(cnt)
	{
		if(pbuf->end < pbuf->start)
		{
			Usb_ReadFifo(adr[index], pbuf->buf + pbuf->end, cnt);
			pbuf->end += cnt;
			cnt = 0;
		}
		else
		{
			size = pbuf->total - pbuf->end;
			Usb_ReadFifo(adr[index], pbuf->buf + pbuf->end, (UINT8)( (cnt > size) ? size : cnt) );
			if(cnt < size)
			{
				pbuf->end += cnt;
				cnt = 0;
			}
			else
			{
				pbuf->end = 0;
				cnt = cnt - (UINT8)size;
			}
		}
	}
}

static void __inline writefifo(UINT8 index, TUsbBuf *pbuf)
{
	UINT32 len;
	UINT8 stltend;
	const UINT32 txsize[] = 
	{
		EP0_FIFO_SIZE, 
		TX_BULK_EP_FIFO_SIZE, 
		TX_BULK_EP_FIFO_SIZE, 
		TX_BULK_EP_FIFO_SIZE
	};
	const UINT8 adr[] = { USB_REG_TXD0, USB_REG_TXD1, USB_REG_TXD2, USB_REG_TXD3 };

	stltend = (UINT8)(pbuf->start < pbuf->end);
	if(stltend)
		len = pbuf->end - pbuf->start;
	else
		len = pbuf->total - pbuf->start;
	if(len > txsize[index])
		len = txsize[index];

	Usb_WriteFifo(adr[index], pbuf->buf + pbuf->start, (UINT8)len);

	if(stltend)
		pbuf->start += len;
	else
		pbuf->start = (pbuf->start + len)%pbuf->total;
}

static void WriteTxFifo(UINT8 index)
{
	TUsbBuf *pbuf;
	if(index > USB_TOTALPIPE - 1)
		return;
	pbuf = gUsbContext.txbuf + index;

	if(pbuf->status == FIFO_BUSY)
		return;

	if(pbuf->start == pbuf->end)
	{
		if(index == 0)
		{
			if(gUsbContext.state == DEV_CONFIGURED)
				EnableRx(0);
			else {
				if(gUsbContext.state == DEV_SETADDRESS)
				{
					Usb_SetFuncAddr(gUsbContext.addr | AD_EN);
					Usb_SetRxCmd(0, RX_EN);
					gUsbContext.state = DEV_ADDRESS;
				}
				else
				{
					//Usb_SetRxCmd(0, FLUSH);
					Usb_SetTxCmd(0, FLUSH);
					EnableTx(0);
				}
			}
		}
		return;
	}
	Usb_SetTxCmd(index, FLUSH);
	writefifo(index, pbuf);
//	pbuf->flag = 0;
	EnableTx(index);
}

enum TX_RX_CTRL_BITenum{
	TX_RX_INT	= BIT0,
	TX_RX_RW	= BIT1
};

static void ReadRxFifo(UINT8 index)
{
	UINT32 cnt;
	TUsbBuf *pbuf;
	UINT8 status;

	if(index > USB_TOTALPIPE - 1)
		return;
	pbuf = gUsbContext.rxbuf + index;

	if(pbuf->end < pbuf->start)
		cnt = pbuf->start - pbuf->end - 1;
	else
		cnt = pbuf->total - pbuf->end + pbuf->start-1;

	if(cnt < USB_RX_THD_UP)
	{
		pbuf->flag = 1;
	}

	status = Usb_GetRxStatus(index);
	while(status)
	{
		ReadRx(pbuf, status, index);
		status = Usb_GetRxStatus(index);
	}

	Usb_SetRxCmd(index, FLUSH);

}

static void RxBuf(TUsbBuf *pbuf, UINT8 *buf, UINT32 len)
{
	UINT32 size;
	while(len)
	{
		if(pbuf->start < pbuf->end)
		{
			memcpy(buf, pbuf->buf + pbuf->start, (unsigned int)len);
			pbuf->start += len;
			len = 0;
		}
		else
		{
			size = pbuf->total - pbuf->start;
			memcpy(buf, pbuf->buf + pbuf->start, (unsigned int)((len > size) ? size : len));
			if(len < size)
			{
				pbuf->start += len;
				len = 0;
			}
			else
			{
				pbuf->start = 0;
				len -= size;
				buf += size;
			}
		}
	}
}

UINT32 URdRxBuf(UINT8 index, UINT8 *buf, UINT32 len)
{
	UINT32 size;
	TUsbBuf *ptr = &gUsbContext.rxbuf[index];

	if(index > USB_TOTALPIPE - 1)
		return 0;
	if(ptr->start == ptr->end)
		return 0;

	if(ptr->start < ptr->end)
		size = ptr->end - ptr->start;
	else
		size = ptr->total - ptr->start + ptr->end;
	if(len > size)
		len = size;

	RxBuf(ptr, buf, len);
	if(ptr->flag)
	{
		if(ptr->end < ptr->start)
			size = ptr->start - ptr->end - 1;
		else
			size = ptr->total - ptr->end + ptr->start-1;

		if(size > USB_RX_THD_DOWN)
		{
			ptr->flag = 0;
			EnableRx(index);
		}
	}

	return len;
}

UINT32 UWrTxBuf(UINT8 index, UINT8 *buf, UINT32 len)
{
	UINT32 size;
	TUsbBuf *ptr;
	UINT32 len1 = len;

	if( !len )
		return 0;
	if(index > USB_TOTALPIPE - 1)
		return 0;
	ptr = &gUsbContext.txbuf[index];
	ptr->flag = 1;

	//while(ptr->status == FIFO_BUSY);

	if(ptr->end < ptr->start)
	{
		size = ptr->start - ptr->end - 1;
		if(size > len)
			size = len;
		memcpy(ptr->buf + ptr->end, buf, (unsigned int)size);
		ptr->end += size;
		len -= size;
		buf += size;
	}
	else
	{
		size = ptr->total - ptr->end;
		if(ptr->start == 0)
			size -= 1;
		if(size > len)
			size = len;
		memcpy(ptr->buf + ptr->end, buf, (unsigned int)size);
		ptr->end += size;
		len -= size;
		buf += size;
		if(ptr->start != 0)
			ptr->end %= ptr->total;
	}

	ptr->flag = 0;
	WriteTxFifo(index);
	if(len)
	{
		Delay(100);
		UWrTxBuf(index, buf, len);
	}
	return len1;
}


//===================================

static void __inline ClearContext(UINT8 index)
{
	gUsbContext.txbuf[index].start = 0;
	gUsbContext.txbuf[index].end = 0;
	gUsbContext.rxbuf[index].start = 0;
	gUsbContext.rxbuf[index].end = 0;
	gUsbContext.txbuf[index].status = FIFO_IDEL;
	gUsbContext.rxbuf[index].status = FIFO_IDEL;
}

static void ClearFifo(void)
{
	UINT8 i;

	for(i = 0; i < USB_TOTALPIPE; i++)
		ClearContext(i);
}

void InitUsbContext(void)
{
	int i;

	memset(&gUsbContext, 0, sizeof(TUsbContext));
	for(i = 0; i < USB_TOTALPIPE; i++)
	{
		gUsbContext.rxbuf[i].buf = gUsbBuf[i<<1];
		gUsbContext.rxbuf[i].total = gUsbBufSize[i<<1];
		gUsbContext.txbuf[i].buf = gUsbBuf[(i<<1) + 1];
		gUsbContext.txbuf[i].total = gUsbBufSize[(i<<1) + 1];
		gUsbContext.txtoggle[i] = TX_TOGL;
		gUsbContext.rxtoggle[i] = TX_TOGL;
	}
	gUsbContext.txtoggle[0] = 0;
	gUsbContext.rxtoggle[0] = 0;
}

static void ResetUsb(void)
{
	UINT8 i;

	Usb_SetTxEpc(0, 0);
	Usb_SetFuncAddr(0);
	Usb_EnableAddress();
	for(i = 0; i < USB_TOTALPIPE; i++)
	{
		Usb_SetTxCmd(i, FLUSH);
		Usb_SetRxCmd(i, FLUSH);
	}
	ClearFifo();
	//Usb_SetRxCmd(0, RX_EN);
	EnableRx(0);
}

void InitUsb9604(void)
{
	Usb_SetMainCtrl(SRST);
	Usb_SetClkCtrl(USB_CLKDIV-1);
    while(Usb_GetMainCtrl() & SRST);
	Usb_SetMainCtrl(VGE | INT_L_O);

	Usb_SetFuncAddr(AD_EN);
	Usb_SetTxEpc(0, 0);

	Usb_SetMainEventMask(0);
	Usb_SetNakEventMask(NAK_OUT0);
	Usb_SetTransEventMask(TX_FIFO0 | TX_FIFO1 | TX_FIFO2 | TX_FIFO3);
	Usb_SetRcvEventMask(RX_FIFO0 | RX_FIFO1 | RX_FIFO2 | RX_FIFO3);
	Usb_SetAltEventMask(ALT_SD3 | ALT_RESET | ALT_RESUME);
	Usb_SetMainEventMask(INTR_E | RX_EV | NAK | TX_EV | ALT);
	//Usb_SetMainEventMask(INTR_E | RX_EV | NAK | TX_EV);
	Usb_SetReg(0x1f, 0x40);	//?? why
	ResetUsb();
	Usb_SetNodeState(OPR_ST);
	Usb_EnableNodeAttached();
	gUsbContext.state = DEV_DETACHED;
}



//============== interrupt services ===================

static void __inline StallEp0(void)
{
	Usb_EnableTxStall(0);
	Usb_SetTxCmd(0, TX_EN);
}

static void GetDescReq(void)
{
	UINT32 len = 0;
	UINT8 *ptr = NULL;
	int strindex = gUsbContext.req.wValue.descriptor.bDescriptorIndex;

	//tmPrintf("GetDescReq\n");
	switch(gUsbContext.req.bmRequestType & RECIPIENT_MASK)
	{
	case DEVICE_REQ:
		switch(gUsbContext.req.wValue.descriptor.bDescriptorType)
		{
		case DEVICE_DESCRIPTOR:
			len = gUsbDevDesc.bLength;
			ptr = (UINT8 *)&gUsbDevDesc;
			//tmPrintf("\nGet Dev DescReq\n reqlen = %x, len = %x\n", gUsbContext.req.wLength, len);
			break;
		case CONFIG_DESCRIPTOR:
			if(gUsbContext.cfgsel == 0)
			{
				//len = gUsbLongCfgDesc.devcfg.wTotalLength;
				len = (gUsbLongCfgDesc.devcfg.TotalLengthH << 8) | gUsbLongCfgDesc.devcfg.TotalLengthL;
				ptr = (UINT8 *)&gUsbLongCfgDesc;
				//tmPrintf("\nGet Cfg DescReq\n reqlen = %x, len = %x\n", gUsbContext.req.wLength, len);
			}
			break;
		case STRING_DESCRIPTOR:
			if( (strindex >= 0) && (strindex < STR_LAST_INDEX) )
			{
				len = gUsbStrDesc[strindex]->bLength;
				ptr = (UINT8 *)gUsbStrDesc[strindex];
				//tmPrintf("\nGet Desc DescReq\n reqlen = %x, len = %x\n", gUsbContext.req.wLength, len);
				break;
			}
		case INTERFACE_DESCRIPTOR:
		case ENDPOINT_DESCRIPTOR:
		case USB_DEVICE_DESCRIPTOR:
		default:
			StallEp0();
			return;
		}
		if(ptr && len)
		{
			if(len > gUsbContext.req.wLength)
				len = gUsbContext.req.wLength;
			UWrTxBuf(0, ptr, len);
		}
		else
			StallEp0();
		break;
	case INTERFACE_REQ:
	case ENDPOINT_REQ:
	case OTHER_REQ:
	default:
		StallEp0();
		break;
	}
}

static void __inline ZeroResponse(UINT8 index)
{
	Usb_SetTxCmd(index, FLUSH);
	EnableTx(0);
}

static void ClrFeatureReq(void)
{
	UINT8 ep = (UINT8)(gUsbContext.req.wIndex.as_bytes.lsb & ENDPADR_MASK);
	UINT8 val = 0;

	//tmPrintf("ClrFeatureReq\n");
	if( (gUsbContext.state == DEV_ATTACHED) ||
		(gUsbContext.req.wLength != 0) )
	{
		StallEp0();
		return;
	}
	switch(gUsbContext.req.bmRequestType & RECIPIENT_MASK)
	{
	case DEVICE_REQ:
		if(gUsbLongCfgDesc.devcfg.bmAttributes & REMOTE_WAKE)
			val = 1;
		if((gUsbContext.req.wValue.feature.bSelector & 1) != val)
		{
			StallEp0();
			return;
		}
		break;
	case INTERFACE_REQ:
		break;
	case ENDPOINT_REQ:
		if( (gUsbContext.req.wValue.feature.bSelector != ENDPOINT_STALL) || (ep > USB_TOTALENDP - 1) )
		{
			StallEp0();
			return;
		}
		Usb_DisableTxStall(ep);
		ZeroResponse(ep);
		break;
	case OTHER_REQ:
	default:
		StallEp0();
		return;
	}
}

static void SetFeatureReq(void)
{
	UINT8 ep = (UINT8)(gUsbContext.req.wIndex.as_bytes.lsb & ENDPADR_MASK);
	UINT8 val = 0;

	//tmPrintf("SetFeatureReq\n");
	if( (gUsbContext.state == DEV_ATTACHED) ||
		(gUsbContext.req.wLength != 0) )
	{
		StallEp0();
		return;
	}
	switch(gUsbContext.req.bmRequestType & RECIPIENT_MASK)
	{
	case DEVICE_REQ:
		if(gUsbLongCfgDesc.devcfg.bmAttributes & REMOTE_WAKE)
			val = 1;
		if((gUsbContext.req.wValue.feature.bSelector & 1) != val)
		{
			StallEp0();
			return;
		}
		break;
	case INTERFACE_REQ:
		break;
	case ENDPOINT_REQ:
		if( (gUsbContext.req.wValue.feature.bSelector != ENDPOINT_STALL) || (ep > USB_TOTALENDP - 1) )
		{
			StallEp0();
			return;
		}
		Usb_DisableTxStall(ep);
		ZeroResponse(ep);
		break;
	case OTHER_REQ:
	default:
		StallEp0();
		return;
	}
}

static void GetStatusReq(void)
{
	UINT8 ep = (UINT8)(gUsbContext.req.wIndex.as_bytes.lsb & ENDPADR_MASK);

	//tmPrintf("GetStatusReq\n");
	if( (gUsbContext.state == DEV_ATTACHED) ||
		(gUsbContext.req.wValue.lsw != 0) ||
		(gUsbContext.req.wLength != 2) )
	{
		StallEp0();
		return;
	}
	gUsbContext.status.msb = 0;
	switch(gUsbContext.req.bmRequestType & RECIPIENT_MASK)
	{
	case DEVICE_REQ:
		if(gUsbContext.req.wIndex.as_bytes.lsb != 0)
		{
			StallEp0();
			return;
		}
		if(gUsbLongCfgDesc.devcfg.bmAttributes & REMOTE_WAKE)
			gUsbContext.status.msb |= WAKEUP_STATUS;
		if(gUsbLongCfgDesc.devcfg.bmAttributes & SELF_POWER)
			gUsbContext.status.msb |= SELFPOWER_STATUS;
		break;
	case INTERFACE_REQ:
		gUsbContext.status.msb = 0;
		break;
	case ENDPOINT_REQ:
		if(ep > USB_TOTALENDP - 1)
		{
			StallEp0();
			return;
		}
		if(Usb_GetTxEpc(ep) & STALL)
			gUsbContext.status.msb |= STALLED_STATUS;
		break;
	case OTHER_REQ:
	default:
		StallEp0();
		return;
	}
	gUsbContext.status.lsb = 0;
	UWrTxBuf(0, (UINT8 *)&gUsbContext.status, sizeof(TUsbDevStatus));
}

static void SetAddressReq(void)
{
	//tmPrintf("SetAddressReq\n");
	if( (gUsbContext.req.wIndex.msw != 0) || 
		(gUsbContext.req.wLength != 0) ||
		(gUsbContext.state == DEV_CONFIGURED) )
	{
		StallEp0();
		return;
	}
	//tmPrintf("SetAddressReq1\n");
	switch(gUsbContext.req.bmRequestType & RECIPIENT_MASK)
	{
	case DEVICE_REQ:
	//	tmPrintf("DEVICE_REQ\n");
		Usb_DefEpc0();
		gUsbContext.addr = gUsbContext.req.wValue.as_bytes.lsb;
		if(gUsbContext.addr == 0)
			gUsbContext.state = DEV_ATTACHED;
		else
			gUsbContext.state = DEV_SETADDRESS;
		EnableTx(0);
		break;
	case INTERFACE_REQ:
	case ENDPOINT_REQ:
	case OTHER_REQ:
	default:
		StallEp0();
		return;
	}
}

static void GetConfigReq(void)
{
	UINT8 sel;

	//tmPrintf("GetConfigReq\n");
	if ( (gUsbContext.state == DEV_ATTACHED) ||
		(gUsbContext.req.wValue.lsw != 0) || 
		(gUsbContext.req.wIndex.msw != 0) || 
		(gUsbContext.req.wLength != 1) )
	{
		StallEp0();
		return;
	}
	switch(gUsbContext.req.bmRequestType & RECIPIENT_MASK)
	{
	case DEVICE_REQ:
		sel = gUsbCfgDesc[gUsbContext.cfgsel]->bConfigurationValue;
		UWrTxBuf(0, &sel, sizeof(UINT8));
		break;
	case INTERFACE_REQ:
	case ENDPOINT_REQ:
	case OTHER_REQ:
	default:
		StallEp0();
		return;
	}
}

static void disableep(void)
{
	UINT8 i, adr, index;

	for(i = 0; i < sizeof(gUsbEp[gUsbContext.ifsel]) / sizeof(TUsbEndPointDesc *); i++)
	{
		if(gUsbEp[gUsbContext.ifsel][i] == NULL)
			continue;
		adr = (UINT8)(gUsbEp[gUsbContext.ifsel][i]->bEndpointAddress & ENDPADR_MASK);
		index = (adr + 1) >> 1;
		if(gUsbEp[gUsbContext.ifsel][i]->bEndpointAddress & ENDP_DIRECT)	//IN
		{
			Usb_SetTxCmd(index, FLUSH);
			Usb_SetTxEpc(index, 0);
		}
		else
		{
			Usb_SetRxCmd(index, FLUSH);
			Usb_SetRxEpc(index, 0);
		}
		//tmPrintf("Disable ep %d\n", index);
	}
}

#if 1
static void enableep(void)
{
	UINT8 i, val = 1;

	for(i = 1; i < 4; i++, val += 2)
	{
		Usb_SetTxCmd(i, FLUSH);
		Usb_SetTxEpc(i, val | EP_EN);
		Usb_SetRxEpc(i, (val+1) | EP_EN);
		EnableRx(i);
	}
}

#else
static void enableep(void)
{
	UINT8 i, adr, index;

	//tmPrintf("ifsel = %d\n", gUsbContext.ifsel);
	for(i = 0; i < sizeof(gUsbEp[gUsbContext.ifsel]) / sizeof(TUsbEndPointDesc *); i++)
	{
		if(gUsbEp[gUsbContext.ifsel][i] == NULL)
			continue;
		adr = gUsbEp[gUsbContext.ifsel][i]->bEndpointAddress & ENDPADR_MASK;
		index = (adr + 1) >> 1;
		//tmPrintf("index = %d", index);
		if(gUsbEp[gUsbContext.ifsel][i]->bEndpointAddress & ENDP_DIRECT)	//IN
		{
			Usb_SetTxCmd(index, FLUSH);
			if(gUsbEp[gUsbContext.ifsel][i]->bmAttributes & ISOCHRONOUS_ENDPOINT)
				Usb_SetTxEpc(index, adr | EP_EN | ISO);
			else
				Usb_SetTxEpc(index, adr | EP_EN);
		}
		else
		{
			Usb_SetRxCmd(index, FLUSH);
			if(gUsbEp[gUsbContext.ifsel][i]->bmAttributes & ISOCHRONOUS_ENDPOINT)
				Usb_SetRxEpc(index, adr | EP_EN | ISO);
			else
				Usb_SetRxEpc(index, adr | EP_EN);
			//Usb_SetRxCmd(0, RX_EN);
			EnableRx(index);
		}
	}
}
#endif

static void uncfgdev(void)
{
	disableep();
	gUsbContext.state = DEV_ADDRESS;
	ZeroResponse(0);
}

static void cfgdev(UINT8 sel)
{
	disableep();
	gUsbContext.ifsel = sel;
	enableep();
	ClearFifo();
}

static void SetConfigReq(void)
{
	//tmPrintf("SetConfigReq\n");
	if( (gUsbContext.state == DEV_ATTACHED) ||
		(gUsbContext.req.wIndex.msw != 0) || 
		(gUsbContext.req.wLength != 0) )
	{
		StallEp0();
		return;
	}
	switch(gUsbContext.req.bmRequestType & RECIPIENT_MASK)
	{
	case DEVICE_REQ:
		if(gUsbContext.req.wValue.as_bytes.lsb == 0)
		{
			if(gUsbContext.state != DEV_CONFIGURED)
			{
				StallEp0();
				return;
			}
			uncfgdev();
		}
		else
		{
			if(gUsbContext.req.wValue.as_bytes.lsb != gUsbLongCfgDesc.devcfg.bConfigurationValue)
			{
				StallEp0();
				return;
			}
			cfgdev(0);
			gUsbContext.state = DEV_CONFIGURED;
			ZeroResponse(0);
		}
		break;
	case INTERFACE_REQ:
	case ENDPOINT_REQ:
	case OTHER_REQ:
	default:
		StallEp0();
		break;
	}
}

static void GetIfReq(void)
{
	//tmPrintf("GetIfReq\n");
	if( (gUsbContext.state != DEV_CONFIGURED)
		|| (gUsbContext.req.wValue.lsw != 0)
		|| (gUsbContext.req.wLength != 1) )
		//|| (gUsbContext.req.wIndex.as_bytes.lsb != 0) )
	{
		StallEp0();
		return;
	}
	switch(gUsbContext.req.bmRequestType & RECIPIENT_MASK)
	{
	case DEVICE_REQ:
		StallEp0();
		break;
	case INTERFACE_REQ:
		UWrTxBuf(0, &gUsbContext.ifsel, sizeof(UINT8));
		break;
	case ENDPOINT_REQ:
	case OTHER_REQ:
	default:
		StallEp0();
		break;
	}
}

static void SetIfReq(void)
{
	UINT8 sel = gUsbContext.req.wValue.as_bytes.lsb;

	//tmPrintf("SetIfReq\n");
	if( (gUsbContext.state != DEV_CONFIGURED)
		|| (gUsbContext.req.wValue.lsw != 0)
		|| (gUsbContext.req.wLength != 0) )
	{
		StallEp0();
		return;
	}
	switch(gUsbContext.req.bmRequestType & RECIPIENT_MASK)
	{
	case DEVICE_REQ:
		StallEp0();
		break;
	case INTERFACE_REQ:
		if(sel >= TOTAL_INTERFACE)
		{
			StallEp0();
			return;
		}
		if(sel != gUsbContext.ifsel)
			cfgdev(sel);
		ZeroResponse(0);
		break;
	case ENDPOINT_REQ:
	case OTHER_REQ:
	default:
		StallEp0();
		break;
	}
}

typedef void (*ReqFunc)(void);

static void StandardReq(void)
{
	const ReqFunc reqfunc[] = 
	{
		GetStatusReq,	//GET_STATUS
		ClrFeatureReq,	//CLEAR_FEATURE
		NULL,			//
		SetFeatureReq,	//SET_FEATURE
		NULL,			//
		SetAddressReq,	//SET_ADDRESS
		GetDescReq,		//GET_DESCRIPTOR
		NULL,			//SET_DESCRIPTOR
		GetConfigReq,	//GET_CONFIGURATION
		SetConfigReq,	//SET_CONFIGURATION
		GetIfReq,		//GET_INTERFACE
		SetIfReq,		//SET_INTERFACE
		NULL			//SYNCH_FRAME
	};

	if(gUsbContext.req.bRequest.Device_req < sizeof(reqfunc) / sizeof(ReqFunc))
	{
		//tmPrintf("device_req = %d\n", gUsbContext.req.bRequest.Device_req);
		if(reqfunc[gUsbContext.req.bRequest.Device_req])
			reqfunc[gUsbContext.req.bRequest.Device_req]();
	}
	else
	{
		StallEp0();
	}
}

static void Usb_Setup(void)
{
	gUsbContext.txtoggle[0] = 0;
	Usb_DisableTxStall(0);
	Usb_ReadFifo(USB_REG_RXD0, (UINT8 *)&gUsbContext.req, sizeof(TUsbRequest));
	Usb_SetRxCmd(0, FLUSH);
	Usb_SetTxCmd(0, FLUSH);
	switch(gUsbContext.req.bmRequestType & REQTYPE_MASK)	//request type
	{
	case USB_STANDARD_REQUEST:
		StandardReq();
		break;
	case USB_VENDOR_REQUEST:
	case USB_CLASS_REQUEST:
	default:
		StallEp0();
		break;
	}
}

static void _isr_AltevSD3(void)
{
	//tmPrintf("_isr_AltevSD3\n");
	Usb_SetAltEventMask(ALT_RESET | ALT_RESUME);
	Usb_SetNodeState(SUS_ST);
}

static void _isr_AltevReset(void)
{
	//tmPrintf("_isr_AltevReset\n");
	Usb_DisableTxStall(0);
	Usb_SetNodeState(RST_ST);
	Usb_SetFuncAddr(AD_EN);
	Usb_SetTxEpc(0, 0);
	ResetUsb();
	Usb_SetNodeState(OPR_ST);
    if( (gUsbContext.state == DEV_ADDRESS) || (gUsbContext.state == DEV_CONFIGURED) )
		gUsbContext.state = DEV_ATTACHED;
}

static void _isr_AltevResume(void)
{
	//tmPrintf("_isr_AltevResume\n");
	Usb_SetAltEventMask(ALT_SD3 | ALT_RESET | ALT_RESUME);
	Usb_SetNodeState(OPR_ST);
	//Usb_SetRxCmd(0, RX_EN);
	EnableRx(0);
}

static void _isr_RxEvent(UINT8 index)
{
	UINT8 status;

	status = Usb_GetRxStatus(index);
	if(index == 0)
	{
		if(status & SETUP_R)
			Usb_Setup();
		else
		{
			Usb_SetRxCmd(0, FLUSH);
			Usb_SetTxCmd(0, FLUSH);
			//Usb_SetRxCmd(0, RX_EN);
			EnableRx(0);
		}
		return;
	}

	if( (status & RX_ERR) || ((status & RX_LAST) == 0) )
	{
		Usb_SetRxCmd(index, FLUSH);
		//tmPrintf("Rx status = RX_ERR || RX_LAST\n");
		//Usb_SetRxCmd(index, RX_EN);
		EnableRx(index);
		return;
	}
	ReadRxFifo(index);
	if(gUsbContext.rxbuf[index].flag == 1)
		return;
	//Usb_SetRxCmd(index, FLUSH);
	EnableRx(index);
}

static void _isr_TxEvent(UINT8 index)
{
	UINT8 status;

	if(index > USB_TOTALPIPE - 1)
		return;
	//tmPrintf("Tx event\n");
	status = Usb_GetTxStatus(index);
	if((status & TX_DONE) == 0)
		return;
	if((status & ACK_STAT) == 0)
	{
		ReenableTx(index);
		return;
	}
	//tmPrintf("Fifo idle\n");
	Usb_SetTxCmd(index, FLUSH);
	gUsbContext.txbuf[index].status = FIFO_IDEL;
	if(gUsbContext.txbuf[index].flag == 1)
		return;

	WriteTxFifo(index);
}

void _ISR_UsbHandle(void)
{
	UINT8 event, evt, adr;

	adr = Usb_GetAdr();
	event = Usb_GetMainEvent();
	if(event & ALT)
	{
		evt = Usb_GetAltEvent();
		if(evt & ALT_SD3)
			_isr_AltevSD3();
		if(evt & ALT_RESET)
			_isr_AltevReset();
		if(evt & ALT_RESUME)
			_isr_AltevResume();
		Usb_SetAdr(adr);
		return;
	}
	if(event & NAK)
	{
		evt = Usb_GetNakEvent();
		if(evt & NAK_OUT0)
		{
			//tmPrintf("NAN_IN0 occured\n");
			ClearContext(0);
			Usb_FlushTx(0);
			Usb_FlushRx(0);
			Usb_DisableTx(0);
			//Usb_SetRxCmd(0, RX_EN);
			EnableRx(0);
			Usb_SetAdr(adr);
			return;
		}

	}

	if(event & RX_EV)
	{
		evt = Usb_GetRcvEvent();
		//tmPrintf("RX_EV: event = 0x%x\n", evt);
		if(evt & RX_FIFO0)
			_isr_RxEvent(0);
		if(evt & RX_FIFO1)
			_isr_RxEvent(1);
		if(evt & RX_FIFO2)
			_isr_RxEvent(2);
		if(evt & RX_FIFO3)
			_isr_RxEvent(3);
	}
	if(event & TX_EV)
	{
		evt = Usb_GetTransEvent();
		//tmPrintf("TX_EV: event = 0x%x\n", evt);
		if(evt & TX_FIFO0)
			_isr_TxEvent(0);
		if(evt & TX_FIFO1)
			_isr_TxEvent(1);
		if(evt & TX_FIFO2)
			_isr_TxEvent(2);
		if(evt & TX_FIFO3)
			_isr_TxEvent(3);
	}
	//EnableRx(0);
	Usb_SetAdr(adr);
}

#else
void nullUsbdrv(void)
{
}

#endif
