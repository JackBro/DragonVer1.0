
#include "../../simchannel/simchannel.h"
#include "shell.h"
#include "cmdlist.h"

#define CMD_BUFLEN	0x100

static char gCmdBuffer[CMD_BUFLEN];

typedef struct tag_TTmContext
{
	char			*cmdbuf;
	int				pos;
} TmContext, *PTmContext;

static TmContext gTmContext;

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

static int tmReadCmd(char *str, int maxlen)
{
	char val;
	int len, flag = 0;

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
				gTmContext.pos--;
			}
			break;
		case '\n':
		case '\r':
			tmWriteBuf(&val, 1);
			gTmContext.cmdbuf[gTmContext.pos] = 0;
			maxlen = (maxlen > gTmContext.pos) ? (gTmContext.pos+1) : maxlen;
			memcpy(str, gTmContext.cmdbuf, maxlen);
			gTmContext.pos = 0;
			flag = 1;
			break;
		default:
			tmWriteBuf(&val, 1);
			gTmContext.cmdbuf[gTmContext.pos] = val;
			gTmContext.pos++;
			break;
		}
	}

	return flag;
}

int tmReadBuf(char *pt, int len)
{
	if(gpChannel)
		return gpChannel->hostReadData(pt, len);
	return 0;
}

int tmWriteBuf(char *pt, int len)
{
	int size = len, pos;

	if(gpChannel == NULL)
		return 0;
	do {
		pos = gpChannel->hostWriteData(pt, len);;
		len -= pos;
	} while( (pos != 0) && (len != 0) );

	return size - len;
}

static int GetCmdIndex(char *str)
{
	int i;

	for(i = 0; i < gCmodelCmdCount; i++)
	{
		if ( strcmp(str, gCmodelCmdList[i].cmdstr) == 0 )
			return i;
	}
	return i;
}

static void PrintCmdHeader(void)
{
	tmPrintf("\nCmodel:>");
}

static void PrintErrorNote(void)
{
	tmPrintf("Bad command or file name!\n");
}

static void ParseRunCmd(char *str)
{
	int len, i = 0, flag = 1;
	int argc = 0;
	char *argv[MAX_PARAMETER + 1];

	argv[0] = str;
	len = (int)strlen(str);
	while(i < len)
	{
		switch(str[i])
		{
		case ' ':
		case '\t':
			flag = 1;
			str[i] = 0;
			break;
		default:
			if(flag)
			{
				argv[argc] = str+i;
				flag = 0;
				argc++;
			}
			break;
		}
		i++;
	}
	if(strlen(argv[0]) == 0)
	{
		PrintCmdHeader();
		return;
	}
	i = GetCmdIndex(argv[0]);
	if ( i < gCmodelCmdCount )
	{
		if( (argc >= gCmodelCmdList[i].minpara) && (argc <= gCmodelCmdList[i].maxpara) )
			gCmodelCmdList[i].pcall(argc, argv);
		else
			tmPrintf("%s\n", gCmodelCmdList[i].helpstr);
	}
	else
		PrintErrorNote();
	PrintCmdHeader();
}

void shellInit(void)
{
	gTmContext.cmdbuf = gCmdBuffer;
	gTmContext.pos = 0;
}

void shell(void)
{
	char str[0x100];

	if(gpChannel == NULL)
		return;
	if(tmReadCmd(str, 0x100))
		ParseRunCmd(str);
}

