#ifndef _TRM_USB_READWRITE_H_
#define _TRM_USB_READWRITE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct tag_TUsbRW
{
	UINT8	adr;
} TUsbRW, *PTUsbRW;

UINT8	Usb_GetAdr(void);
void	Usb_SetAdr(UINT8 adr);
void	Usb_SetReg(UINT8 adr, UINT8 val);
UINT8	Usb_GetReg(UINT8 adr);
void	Usb_Delay(UINT32 count);
UINT8	Usb_WriteFifo(UINT8 adr, UINT8 *buf, UINT8 len);
UINT8	Usb_ReadFifo(UINT8 adr, UINT8 *buf, UINT8 len);



#ifdef __cplusplus
}
#endif

#endif //_TRM_USB_BASEFUNC_H_

