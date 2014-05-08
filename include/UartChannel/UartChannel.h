#ifndef _UARTCHANNEL_H_
#define _UARTCHANNEL_H_


#ifndef CHANNELUART_DLL
	#define CHANNELUART_DLL  __declspec(dllimport)
#endif

#include "tools/baseitem.h"
#include "UartUsbBaseChannel/UartUsbBaseChannel.h"

//#define UART_DEFAULTSET		"115200,n,8,1"

#define UART_TERMINAL_TIME	2000
#define UART_INQUEUE_SIZE	4096
#define UART_OUTQUEUE_SIZE	4096


class CHANNELUART_DLL CUartChannel : public CUartUsbBaseChannel
{
public:
	CUartChannel(void);
	virtual ~CUartChannel(void);

	virtual void	Open(void);
	virtual void	Close(void);
	virtual int		Write(char *buf, int size);
	virtual int		Read(char *buf, int size);
	virtual int		IsOpened(void);

protected:
	virtual int		ReadChannel(char *buf, int size);
	virtual int		WriteChannel(char *buf, int size);

public:
	void	SetPort(int port = 1);
	int		GetPort(void);
	void	SetupBaud(int baud = CBR_115200, int dnum = 8, int enparity = 0, int stop = 0);
	void	SetupBaud(char *setting);
	int		GetBaud(void);
	void	ManualConfigPort(void);

private:
	void	Init(void);
	void	SetupTimesout(void);
	void	SetupHandShake(void);
	void	CloseUart(void);


private:
	HANDLE	m_Hcomm;
	int		m_Port;		//0 is error
	int		m_baud;
	
};


#endif

