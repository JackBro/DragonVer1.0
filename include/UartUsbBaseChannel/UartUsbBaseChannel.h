#ifndef _UARTUSB_BASECHANNEL_H_
#define _UARTUSB_BASECHANNEL_H_


#ifndef CHANNELUARTUSBBASE_DLL
	#define CHANNELUARTUSBBASE_DLL  __declspec(dllimport)
#endif

#include "tools/baseitem.h"
#include "toolbuf/fifobuf.h"

#define UART_USB_TERMINAL_TIME 100


class CHANNELUARTUSBBASE_DLL CUartUsbBaseChannel : public CBaseChannel
{
public:
	CUartUsbBaseChannel(void);
	virtual ~CUartUsbBaseChannel(void);

	virtual void	Open(void);
	virtual void	Close(void);
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int size);
	virtual int		IsOpened(void);

protected:
	virtual int		ReadChannel(char *buf, int size) = 0;
	virtual int		WriteChannel(char *buf, int size) = 0;

protected:
	void	ReadUartUsb(void);
	void	WriteUartUsb(void);
	void	Init(void);
	void	StartThrd(void);
	void	TerminalThrd(void);
	void	ThrdProc(void);
	friend DWORD	UartUsbThrdProc(LPVOID *ptr);

protected:
	
	HANDLE		m_hrdchanelsemp;

private:

	volatile int m_enrun;

	HANDLE		m_hthrd;
	HANDLE		m_hrdsemp[2];

	int			m_currdmode;
	int			m_incallback;

	CFifoBuf	m_wpool;
	CFifoBuf	m_rpool;

};


#endif //_UARTUSB_BASECHANNEL_H_

