
#include "internal.h"


#define TOTAL_UART_PORT		2
#define UART_RX_BUFSIZE		0x50000
#define UART_TX_BUFSIZE		0x50000


/*************************UART Channel*************************/
static char	gUartSndBuf[TOTAL_UART_PORT][UART_TX_BUFSIZE];
static char	gUartRcvBuf[TOTAL_UART_PORT][UART_RX_BUFSIZE];
static TUartContext gPortContext[TOTAL_UART_PORT];

//================ interrupt services ============

#ifndef WIN32
static void __irq _isrURXD1(void)
{
	WriteMemory(REG_I_ISPC, BIT_URXD1);
    while( (ReadMemory(REG_UFSTAT1) & 0xf) > 0 )	    //until FIFO is empty
    {
		gPortContext[1].rcvbuf[gPortContext[1].rcvend ++] = (char)ReadMemory(REG_URXH1);
		gPortContext[1].rcvend %= UART_RX_BUFSIZE;
    }
}

static void __irq _isrURXD0(void)
{
	WriteMemory(REG_I_ISPC, BIT_URXD0);
    while((ReadMemory(REG_UFSTAT0) & 0xf) > 0)	    //until FIFO is empty
    {
		gPortContext[0].rcvbuf[gPortContext[0].rcvend++] = (char)ReadMemory(REG_URXH0);
		gPortContext[0].rcvend %= UART_RX_BUFSIZE;
    }
}

static void __irq _isrUTXD1(void)
{
	while((ReadMemory(REG_UFSTAT1) & 0x200) == 0) 
    {
		if(gPortContext[1].sndstart == gPortContext[1].sndend)
		{
			gPortContext[1].sndflag = 0;
			SetMemory(REG_INTMSK, BIT_UTXD1, OR_OPCODE);
			break;
		}
		WriteMemory(REG_UTXH1, gPortContext[1].sndbuf[gPortContext[1].sndstart]);
		gPortContext[1].sndstart++;
		gPortContext[1].sndstart %= UART_TX_BUFSIZE;
	}
	WriteMemory(REG_I_ISPC, BIT_UTXD1);
}

static void __irq _isrUTXD0(void)
{
	while((ReadMemory(REG_UFSTAT0) & 0x200) == 0) 
    {
		if(gPortContext[0].sndstart == gPortContext[0].sndend)
		{
			gPortContext[0].sndflag = 0;
			SetMemory(REG_INTMSK, BIT_UTXD0, OR_OPCODE);
			break;
		}
		WriteMemory(REG_UTXH0, gPortContext[0].sndbuf[gPortContext[0].sndstart]);
		gPortContext[0].sndstart++;
		gPortContext[0].sndstart %= UART_TX_BUFSIZE;
	}
	WriteMemory(REG_I_ISPC, BIT_UTXD0);
}

static void __irq _isrUERR01(void)
{
	WriteMemory(REG_I_ISPC, BIT_UERR01);

	while((ReadMemory(REG_UFSTAT0) & 0xf) > 0)
    {
		gPortContext[0].rcvbuf[gPortContext[0].rcvend++] = (char)ReadMemory(REG_URXH0);
		gPortContext[0].rcvend %= UART_RX_BUFSIZE;
    }
	while((ReadMemory(REG_UFSTAT1) & 0xf) > 0)	    //until FIFO is empty
    {
		gPortContext[1].rcvbuf[gPortContext[1].rcvend++] = (char)ReadMemory(REG_URXH1);
		gPortContext[1].rcvend %= UART_RX_BUFSIZE;
    }
}
#endif

//============= base Uart service =====================
static __inline void UartEnableSend(int sel)
{
	if(sel == 0)
		SetMemory(REG_INTMSK, ~BIT_UTXD0, AND_OPCODE);
	else
		SetMemory(REG_INTMSK, ~BIT_UTXD1, AND_OPCODE);
}

static int UartWriteBuf(int channel, char *pt, int len)
{
	int i = 0;

	if(channel >= TOTAL_UART_PORT)
		channel = TOTAL_UART_PORT - 1;
	while(i < len)
	{
		if( (((gPortContext[channel].sndend+1) % UART_TX_BUFSIZE)
			== gPortContext[channel].sndstart)
			&& (gPortContext[channel].sndflag == 1) )
		{
			Delay(1);
			continue;
		}
		gPortContext[channel].sndbuf[gPortContext[channel].sndend] = pt[i];
		gPortContext[channel].sndend++;
		i++;
		gPortContext[channel].sndend %= UART_TX_BUFSIZE;
		gPortContext[channel].sndflag = 1;
		UartEnableSend(channel);
	}
	return i;
}

int Uart0WriteBuf(char *pt, int len)
{
	return UartWriteBuf(CHANNEL_UART0, pt, len);
}

int Uart1WriteBuf(char *pt, int len)
{
	return UartWriteBuf(CHANNEL_UART1, pt, len);
}

static int UartReadBuf(int channel, char *pt, int len)
{
	int i = 0;

	channel %= TOTAL_UART_PORT;
	while(i < len)
	{
		if(gPortContext[channel].rcvstart == gPortContext[channel].rcvend)
			return i;

		pt[i++] = gPortContext[channel].rcvbuf[gPortContext[channel].rcvstart++];
		gPortContext[channel].rcvstart %= UART_RX_BUFSIZE;
	}
	return i;
}

int Uart0ReadBuf(char *pt, int len)
{
	return UartReadBuf(CHANNEL_UART0, pt, len);
}

int Uart1ReadBuf(char *pt, int len)
{
	return UartReadBuf(CHANNEL_UART1, pt, len);
}


//================= global function =========

void SetUartBaud(int port, int baud)
{
	int mclk = 0;
	const int intmask[2] = {BIT_URXD0, BIT_URXD1};
	const int reglist[2][4] = {	{ REG_ULCON0, REG_UFCON0, REG_UCON0, REG_UBRDIV0 },
								{ REG_ULCON1, REG_UFCON1, REG_UCON1, REG_UBRDIV1 } };

	mclk = GetHostMclk();
	if(baud == 0)
		baud = DEFAULT_BAUD;
	if(port >= TOTAL_UART_PORT)
		port = TOTAL_UART_PORT - 1;
	SetMemory(REG_INTMSK, intmask[port], OR_OPCODE);
	WriteMemory(reglist[port][0], 3);
	WriteMemory(reglist[port][1], (2 << 6) | (1 << 4) | 6 | 1);
	WriteMemory(reglist[port][2], 0x2c5);
	WriteMemory(reglist[port][3], (((mclk * 1000) / baud + 8) >> 4) - 1);
	gPortContext[port].baud = baud;
	SetMemory(REG_INTMSK, ~intmask[port], AND_OPCODE);
}

int GetUartBaud(int port)
{
	if(port < TOTAL_UART_PORT)
		return gPortContext[port].baud;
	return 0;
}

static void UartInitContext(void)
{
	int i = 0;

	memset(gPortContext, 0, sizeof(gPortContext));
	for(i = 0; i < TOTAL_UART_PORT; i++)
	{
		gPortContext[i].sndbuf = gUartSndBuf[i];
		gPortContext[i].rcvbuf = gUartRcvBuf[i];
	}
}

void Uart_Init(int baud0, int baud1)
{
	UartInitContext();

	SetMemory(REG_PCONC,0xf0000000, OR_OPCODE );
	SetMemory(REG_PUPC,	0xc000,		OR_OPCODE );
	SetMemory(REG_PCONE,0x3ffeb,	AND_OPCODE);
	SetMemory(REG_PCONE,0x28,		OR_OPCODE );
	SetMemory(REG_PUPE, 0x6,		OR_OPCODE );
	SetMemory(REG_PCONF,0x3ff,		AND_OPCODE);
	SetMemory(REG_PCONF,0x124800,	OR_OPCODE );
	SetMemory(REG_PUPF, 0x1e0,		OR_OPCODE );

#ifndef WIN32
    pISR_UTXD0	= (unsigned)_isrUTXD0;
    pISR_URXD0	= (unsigned)_isrURXD0;
    pISR_UTXD1	= (unsigned)_isrUTXD1;
    pISR_URXD1	= (unsigned)_isrURXD1;
    pISR_UERR01	= (unsigned)_isrUERR01;
#endif
	WriteMemory(REG_UMCON0, 0);
	WriteMemory(REG_UMCON1, 0);
	SetUartBaud(0, baud0);
	SetUartBaud(1, baud1);

	tmSetCallback(CHANNEL_UART0, Uart0ReadBuf,  F_READ);
	tmSetCallback(CHANNEL_UART0, Uart0WriteBuf, F_WRITE);
	tmSetCallback(CHANNEL_UART1, Uart1ReadBuf,  F_READ);
	tmSetCallback(CHANNEL_UART1, Uart1WriteBuf, F_WRITE);
}

