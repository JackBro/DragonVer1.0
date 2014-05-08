


#ifndef _TRM_USBDRV_H_
#define _TRM_USBDRV_H_

#ifdef __cplusplus
extern "C" {
#endif


#define USB_CLKDIV		4
#define USB_TOTALPIPE	4
#define USB_TOTALENDP	7

#define USB_BULK1_BULK2_BULK3	0
#define USB_BULK1_BULK23		1

#define USBOPTION	USB_BULK1_BULK2_BULK3

#define USB_SPEC_VERSION 0x0110 

//=====================================
typedef struct tag_TUsbStringDesc {
    UINT8	bLength;
    UINT8	bDescriptorType;
    char	bstring[100];
} TUsbStringDesc, *PTUsbStringDesc;

enum STRINGINDEXenum{
    STR_LANGID			= 0,
    STR_MANUFACTURER	= 1,
    STR_PRODUCT,
    STR_VERSION,
    STR_LAST_INDEX
};

enum LANGIDenum {
    NEUTRAL		= 0x0000,
    SYTEM_DEFAULT	= 0x0400,
    ENGLISH_NEUTRAL	= 0x0009,
    ENGLISH_US		= 0x0409,
    ENGLISH_UK		= 0x0809,
    RUSSIAN_NEUTRAL	= 0x0019
};

#define VIMICRO_ID_VENDOR  0x0AC8	/* assigned by USB Forum */
#define VIMICRO_ID_PRODUCT 0x0166	/* assigned by Vimicro */

#define MANUFACTURER_STR	"N\0a\0t\0i\0o\0n\0a\0l\0 \0S\0e\0m\0i\0c\0o\0n\0d\0u\0c\0t\0o\0r"
#define PRODUCT_STR			"U\0S\0B\0 \0D\0e\0m\0o\0 \0D\0e\0v\0i\0c\0e" 
#define VERSION_STR			"1\0_\01" 

//=============================

typedef struct tag_TUsbDeviceDesc {
    UINT8	bLength;
    UINT8	bDescriptorType;
    UINT16	bcdUSB;
    UINT8	bDeviceClass;
    UINT8	bDeviceSubClass;
    UINT8	bDeviceProtocol;
    UINT8	bMaxPacketSize;
    UINT16	idVendor;
    UINT16	idProduct;
    UINT16	bcdDevice;
    UINT8	iManufacturer;
    UINT8	iProduct;
    UINT8	iSerialNumber;
    UINT8	bNumConfigs;
} TUsbDeviceDesc, *PTUsbDeviceDesc;

//=======================================
enum USBBMATTRIenum {
	REMOTE_WAKE	=	BIT5,
	SELF_POWER	=	BIT6
};

typedef struct tag_TUsbConfigDesc {
    UINT8	bLength;
    UINT8	bDescriptorType;
	UINT8	TotalLengthL;
	UINT8	TotalLengthH;
    UINT8	bNumInterfaces;
    UINT8	bConfigurationValue;
    UINT8	iConfiguration;
    UINT8	bmAttributes;
    UINT8	MaxPower;
} TUsbConfigDesc, *PTUsbConfigDesc;

enum USBDIRECenum {
    OUT_DIRECT = 0,
    IN_DIRECT
};

enum USBENDPTINTTYPEenum {
    CONTROL_ENDPOINT 	= 0,
    ISOCHRONOUS_ENDPOINT,
    BULK_ENDPOINT,
    INTERRUPT_ENDPOINT
} ;

enum ENDPOINTADRenum {
	ENDPADR_MASK =	BIT4 - 1,
	ENDP_DIRECT	=	BIT7
};

typedef struct tag_TUsbEndPointDesc {
    UINT8	bLength;
    UINT8	bDescriptorType;
	UINT8	bEndpointAddress;
	UINT8	bmAttributes;
	UINT8	MaxPacketSizeL;
	UINT8	MaxPacketSizeH;
    UINT8	bInterval;
} TUsbEndPointDesc, *PTUsbEndPointDesc;

typedef struct tag_TUsbIfDesc {
    UINT8	bLength;
    UINT8	bDescriptorType;
    UINT8	bInterfaceNumber;
    UINT8	bAlternateSetting;
    UINT8	bNumEndpoints;
    UINT8	bInterfaceClass;
    UINT8	bInterfaceSubClass;
    UINT8	bInterfaceProtocol;
    UINT8	iInterface;
} TUsbIfDesc, *PTUsbIfDesc;

typedef struct tag_TUsbLongCfgDesc {
    TUsbConfigDesc		devcfg;
	TUsbIfDesc			if0;
	TUsbEndPointDesc	if0ep[2];
//	TUsbIfDesc			if1;
//	TUsbEndPointDesc	if1ep[2];
} TUsbLongCfgDesc, *PTUsbLongCfgDesc;

//==========================================

enum USBFEATURESELenum {
    ENDPOINT_STALL = 	0,
    DEVICE_REMOTE_WAKEUP = 1
};

enum USBREQTYPEenum {
	RECIPIENT_MASK	=	BIT5 - 1,
	REQTYPE_MASK	=	BIT5 | BIT6,
	DATAXFER_DIRC	=	BIT7
};

typedef struct tag_TUsbRequest 
{
    UINT8 bmRequestType;
    union {
		UINT8 Device_req;
		UINT8 Vendor_req; 
    } bRequest;
    union {
		struct {
			UINT8 bDescriptorIndex; //bStringIndex;
			UINT8 bDescriptorType;
		} descriptor;
		struct {
			UINT8 bSelector;
			UINT8 msb;
		} feature;
		struct {
			UINT8 lsb;
			UINT8 msb;
		} as_bytes;
		UINT16 lsw;
    } wValue;
    
	union {
		struct {
			UINT8 inf_no;
			UINT8 msb;
		} Interface;
		struct {
			UINT8 lsb;
			UINT8 msb;
		} as_bytes;
		UINT16 msw;
    } wIndex;

    UINT16	wLength;
} TUsbRequest, *PTUsbRequest;

enum ENDPFIFOSTATUSenum {
	FIFO_IDEL,
	FIFO_BUSY 
};

typedef struct tag_TUsbBuf {
	UINT8	*buf;
	UINT32	total;
	volatile UINT32 start;
	volatile UINT32 end;
	UINT8	status;
	UINT8	flag;
} TUsbBuf, *PTUsbBuf;

enum USBSTATEenum{
	DEV_NOTINIT,
    DEV_DETACHED,
    DEV_ATTACH_WAITING,
    DEV_ATTACHED,
    DEV_SETADDRESS,
    DEV_ADDRESS,
    DEV_CONFIGURED
};

#define SELFPOWER_STATUS	BIT0
#define WAKEUP_STATUS		BIT1
#define STALLED_STATUS		BIT0

typedef struct tag_TUsbDevStatus {
	UINT8 msb;
    UINT8 lsb;
} TUsbDevStatus, *PTUsbDevStatus;

typedef struct tag_TUsbContext {
	UINT8		state;
	TUsbRequest	req;
	UINT8		ifsel;
	UINT8		cfgsel;
	UINT8		addr;
	TUsbBuf		txbuf[USB_TOTALPIPE];
	TUsbBuf		rxbuf[USB_TOTALPIPE];
	UINT8		txtoggle[USB_TOTALPIPE];
	UINT8		rxtoggle[USB_TOTALPIPE];
	TUsbDevStatus	status;
} TUsbContext, *PTUsbContext;

/*Request Type*/
enum USBREQUESTenum {
	USB_STANDARD_REQUEST	=	0,
	USB_CLASS_REQUEST		=	0x20,
	USB_VENDOR_REQUEST		=	0x40
};

enum USBREQUESTVALenum {
	GET_STATUS			=	0x00,
	CLEAR_FEATURE		=	0x01,
	SET_FEATURE			=	0x03,
	SET_ADDRESS			=	0x05,
	GET_DESCRIPTOR		=	0x06,
	SET_DESCRIPTOR		=	0x07,
	GET_CONFIGURATION	=	0x08,
	SET_CONFIGURATION	=	0x09,
	GET_INTERFACE		=	0x0A,
	SET_INTERFACE		=	0x0B,
	SYNCH_FRAME			=	0x0C
};

enum USBREQUESTRECIPIENTenum {
    DEVICE_REQ		=	0,
    INTERFACE_REQ	=	1,
    ENDPOINT_REQ	=	2,
    OTHER_REQ		=	3
};

enum USBDESCTYPEenum {
    DEVICE_DESCRIPTOR 	= 1,
    CONFIG_DESCRIPTOR,
    STRING_DESCRIPTOR,
    INTERFACE_DESCRIPTOR,
    ENDPOINT_DESCRIPTOR,
    USB_DEVICE_DESCRIPTOR	= 0x21
};

enum DEVICECLASSenum{
    CLASS_NOT_DEFINED 	= 0x0,
    CLASS_AUDIO			= 0x01,
    CLASS_COMMUNICATION = 0x02,
    CLASS_HID 			= 0x03,
    CLASS_MONITOR 		= 0x04,
    CLASS_PRINTING 		= 0x07,
    CLASS_MASS_STORAGE 	= 0x08,
    CLASS_HUB	 		= 0x09,
    CLASS_VENDOR		= 0xFF
};


#define EP0_FIFO_SIZE		8
#define RX_BULK_EP_FIFO_SIZE		64
#define TX_BULK_EP_FIFO_SIZE		64

#define RX_ISO_EP_FIFO_SIZE			64
#define TX_ISO_EP_FIFO_SIZE			64

#define RX_INTR_EP_FIFO_SIZE		1
#define TX_INTR_EP_FIFO_SIZE		1


//============================

void	InitUsbContext(void);
void	InitUsb9604(void);
void	_ISR_UsbHandle(void);

UINT32	URdRxBuf(UINT8 index, UINT8 *buf, UINT32 len);
UINT32	UWrTxBuf(UINT8 index, UINT8 *buf, UINT32 len);

//#ifdef WIN32
//void	UsbStructPrint(void);
//#endif

#ifdef __cplusplus
}
#endif

#endif //_TRM_USBDRV_H_

