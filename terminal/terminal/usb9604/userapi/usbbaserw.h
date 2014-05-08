#ifndef _TRM_USB_BASERW_H_
#define _TRM_USB_BASERW_H_

#ifdef __cplusplus
extern "C" {
#endif

#define		USB_HOSTADDR_ADDR	0x06000002
#define		USB_HOSTADDR_DATA	0x06000000

#ifdef	WIN32

	void USB_SET_ADDR(UINT8 val);
	void USB_SET_DATA(UINT8 val);
	UINT8 USB_GET_DATA(void);

#else

	#define USB_SET_ADDR(val) *(volatile UINT8 *)USB_HOSTADDR_ADDR = val
	#define USB_SET_DATA(val) *(volatile UINT8 *)USB_HOSTADDR_DATA = val
	#define USB_GET_DATA() *(volatile UINT8 *)USB_HOSTADDR_DATA

#endif


#ifdef __cplusplus
}
#endif

#endif


