#ifndef _USBCHANNEL_H_
#define _USBCHANNEL_H_


#ifndef CHANNELUSB_DLL
	#define CHANNELUSB_DLL  __declspec(dllimport)
#endif

#include <setupapi.h>
#include "toolbuf/fifobuf.h"

// Copy from ddk usb.h to avoid build error
typedef struct tagTUSB_DEVICE_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT bcdUSB;
    UCHAR bDeviceClass;
    UCHAR bDeviceSubClass;
    UCHAR bDeviceProtocol;
    UCHAR bMaxPacketSize0;
    USHORT idVendor;
    USHORT idProduct;
    USHORT bcdDevice;
    UCHAR iManufacturer;
    UCHAR iProduct;
    UCHAR iSerialNumber;
    UCHAR bNumConfigurations;
} TUSB_DEVICE_DESCRIPTOR, *PTUSB_DEVICE_DESCRIPTOR;


class CHANNELUSB_DLL CUsbChannel : public CUartUsbBaseChannel
{
public:
	CUsbChannel(void);
	virtual ~CUsbChannel(void);

	virtual void	Open(void);
	virtual void	Close(void);
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int size);
	virtual int		IsOpened(void);

protected:
	virtual int		ReadChannel(char *buf, int size);
	virtual int		WriteChannel(char *buf, int size);

public:
	void	SetPipe(int pipe = 0);
	int		GetPipe(void);

protected:
	void	Init(void);
	void	CloseUsb(void);

	void	StartThrd(void);
	void	TerminalThrd(void);
	void	ThrdProc(void);
	friend DWORD	UsbThrdProc(LPVOID *ptr);

private:
	BOOL EnumUsbDevName(LPGUID pGuid, char* devName);
	BOOL GetOneDevName(HDEVINFO HardwareDeviceInfo, PSP_DEVICE_INTERFACE_DATA DeviceInfoData, char *devName);

private:
	HANDLE	m_HIn;
	HANDLE	m_HOut;
	int		m_Pipe;
	volatile int m_enrun;
	HANDLE		m_hthrd;
	CFifoBuf	m_rdpool;

    HANDLE		m_hrdlap;
    HANDLE		m_hwrlap;
};


#endif //_USBCHANNEL_H_

