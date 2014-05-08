
#include "internal.h"

static TmContext	gTmContext;
static TmChannel	gChannel[CHANNEL_TOTAL] = 
{
	{ "\nUart0_Vm:>", NULL, NULL },
	{ "\nUart1_Vm:>", NULL, NULL },
	{ "\nUsb_Vm:>"	, NULL, NULL }
};


//////////////////////////////////////////////////////////////////////

int tmWriteBuf(char *pt, int len)
{
	if(gChannel[gTmContext.channel].pwrite)
		return gChannel[gTmContext.channel].pwrite(pt, len);
	return 0;
}

int tmReadBuf(char *pt, int len)
{
	if(gChannel[gTmContext.channel].pread)
		return gChannel[gTmContext.channel].pread(pt, len);
	return 0;
}

void tmReadBuf4hcmd(char *buf, int len)
{
	int len1;

	while( len )
	{
		len1 = tmReadBuf(buf, len);
		len -= len1;
		buf += len1;
	}
}


#ifdef WIN32

void tmPrintf(char *fmt,...)
{
	va_list ap;
	char str[256];

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);

	PrintMsg(str);
}

#else

void tmPrintf(char *fmt,...)
{
	int len;
	va_list ap;
	char str[256];

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);

	len = (int)strlen(str);
	tmWriteBuf(str, len);
}

void tmUart0Printf(char *fmt,...)
{
	int len;
	va_list ap;
	char str[256];

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);

	len = (int)strlen(str);
	if(gChannel[CHANNEL_UART0].pwrite)
		gChannel[CHANNEL_UART0].pwrite(str, len);
}

void tmUsbPrintf(char *fmt,...)
{
	int len;
	va_list ap;
	char str[256];

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);

	len = (int)strlen(str);
	if(gChannel[CHANNEL_USB9604].pwrite)
		gChannel[CHANNEL_USB9604].pwrite(str, len);
}


#endif

int tmReadCmd(char *str, int maxlen)
{
	char val = 0;
	char *buf = NULL;
	int len = 0, flag = 0, port = 0;

	buf  = gTmContext.cmdbuf[gTmContext.channel];
	port = gTmContext.channel;
	while(flag == 0)
	{
		len = tmReadBuf(&val, 1);
		if(len == 0)
			break;
		switch(val)
		{
		case '\b':
			if(gTmContext.pos)
			{
				tmWriteBuf(&val, 1);
				gTmContext.pos[port] --;
			}
			break;
		case '\n':
		case '\r':
			tmWriteBuf(&val, 1);
			buf[gTmContext.pos[port]] = 0;
			maxlen = (maxlen > gTmContext.pos[port]) ? (gTmContext.pos[port] + 1) : maxlen;
			memcpy(str, buf, maxlen);
			gTmContext.pos[port] = 0;
			flag = 1;
			break;
		default:
			tmWriteBuf(&val, 1);
			buf[gTmContext.pos[port]] = val;
			gTmContext.pos[port] ++;
			break;
		}
	}
	return flag;
}

void tmPortSelect(int select)
{
	gTmContext.channel = select % CHANNEL_TOTAL;
}

int tmGetCurPort(void)
{
	return gTmContext.channel;
}

char* tmGetCmdHeader(void)
{
	return gChannel[gTmContext.channel].header;
}

void tmSetCallback(int port, ChannelRw pfunc, int type)
{
	if( (pfunc == NULL) || ((type != F_WRITE) && (type != F_READ)) )
		return;
	port %= CHANNEL_TOTAL;
	if(type == F_READ)
	{
		gChannel[port].pread  = pfunc;
		return;
	}
	gChannel[port].pwrite = pfunc;
}

void tmInit(void)
{
	memset(&gTmContext, 0, sizeof(gTmContext));
	tmPortSelect(TM_DEFAULT_CHANNEL);
}
