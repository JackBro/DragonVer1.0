
#ifndef _HOSTCOMM_H_
#define _HOSTCOMM_H_

#ifdef __cplusplus
extern "C" {
#endif


	#define COMM_CMD_BUFSIZE	0x100

	typedef int (*ChannelRw)(char *pt, int len);

	enum COMMCHANNELenum {
		CHANNEL_UART0,
		CHANNEL_UART1,
		CHANNEL_USB9604,
		// Add New Channel Type Here.
		CHANNEL_TOTAL
	};

	enum ACCESSTYPELenum {
		F_READ,
		F_WRITE
	};

	typedef struct tag_TTmChannel
	{
		char		*header;
		ChannelRw	pread;
		ChannelRw	pwrite;
	} TmChannel, *PTmChannel;

	typedef struct tag_TTmContext
	{
		int		channel;
		char	cmdbuf[CHANNEL_TOTAL][COMM_CMD_BUFSIZE];
		int		pos[CHANNEL_TOTAL];
	} TmContext, *PTmContext;


#ifndef NCSC
	void	tmPrintf(char *fmt,...);

// liuyong add for debug print.
#ifdef WIN32
	#define tmUart0Printf tmPrintf
	#define tmUsbPrintf tmPrintf
#else
	void tmUart0Printf(char *fmt,...);
	void tmUsbPrintf(char *fmt,...);
#endif

	#define tmDbgPrintf tmUart0Printf


#endif

	void	tmInit(void);
	void	tmPortSelect(int select);
	int		tmGetCurPort(void);
	char*	tmGetCmdHeader(void);
	void	tmSetCallback(int port, ChannelRw pfunc, int type);

	int		tmReadCmd(char *str, int maxlen);
	int		tmReadBuf(char *pt, int len);
	int		tmWriteBuf(char *pt, int len);
	void	tmReadBuf4hcmd(char *buf, int len);




#ifdef __cplusplus
}
#endif

#endif


