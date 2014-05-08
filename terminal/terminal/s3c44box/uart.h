
#ifndef _HOSTUART_H_
#define _HOSTUART_H_

#ifdef __cplusplus
extern "C" {
#endif


	typedef struct tag_TUartContext
	{
		int baud;
		char *sndbuf;
		char *rcvbuf;
		volatile int sndstart;
		volatile int sndend;
		volatile int rcvstart;
		volatile int rcvend;
		volatile int sndflag;
	} TUartContext, *PTUartContext;


	void	Uart_Init(int baud0, int baud1);
	int		GetUartBaud(int port);
	void	SetUartBaud(int port, int baud);

	int		Uart0WriteBuf(char *pt, int len);
	int		Uart0ReadBuf(char *pt, int len);
	int		Uart1WriteBuf(char *pt, int len);
	int		Uart1ReadBuf(char *pt, int len);



#ifdef __cplusplus
}
#endif

#endif


