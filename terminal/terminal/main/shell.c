
#include "main.h"

volatile int gArmRun = 1;
volatile unsigned char gKey[3];	// [0] : temp key; 
								// [1] : confirm flag;
								// [2] : actual key;

static void PrintCmdHeader(void)
{
	char *str = NULL;
	str = tmGetCmdHeader();
	tmPrintf(str);
}

static void PrintErrorNote(void)
{
	tmPrintf("Bad command or file name!\n");
}

static void ExecuteCmd(int argc, char *argv[])
{
	int i, j, cnt = 0;
	char cmd[10];

	if(argc == 0)
		return;
	for(i = 0; i < gCmdGroupCount; i++)
	{
		for(j = 0; j < gCmdGroup[i].count; j++, cnt++)
		{
			sprintf(cmd, "c%d", cnt);
			if( (strcmp(argv[0], gCmdGroup[i].cmdlist[j].cmdstr) == 0) || 
				(strcmp(argv[0], cmd) == 0) )
			{
				if( (argc >= gCmdGroup[i].cmdlist[j].minpara) && 
					(argc <= gCmdGroup[i].cmdlist[j].maxpara) )
					gCmdGroup[i].cmdlist[j].pcall(argc, argv);
				else
					tmPrintf("c%d | %s\n", cnt, gCmdGroup[i].cmdlist[j].helpstr);
				return;
			}
		}
	}
	PrintErrorNote();
}

int GetCmdLocation(int index, int *x, int *y)
{
	int i;

	*x = 0;
	for(i = 0; i < gCmdGroupCount; i++)
	{
		if(index > gCmdGroup[i].count)
		{
			index -= gCmdGroup[i].count;
			(*x)++;
		}
		else
			break;
	}
	if(*x >= gCmdGroupCount)
		return 0xff;

	*y = index;
	return 0;
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
	if(strlen(argv[0]) != 0)
		ExecuteCmd(argc, argv);
	PrintCmdHeader();
}

/*****************************  Key Process Start *****************************/

static void _isrKeyScan(void)
{
	unsigned char i = 0, j = 0, key[2];
	const int sel[2] = { SEL6KEY, SEL5KEY };
	const unsigned char pad[] = { UPKEY, DOWNKEY, LEFTKEY, RIGHTKEY };

	for(i = 0; i < 2; i++)
	{
		SetMemory(REG_PDATG, ~sel[i], AND_OPCODE);
		key[i] = (unsigned char)ReadMemory(REG_PDATG);
		SetMemory(REG_PDATG, sel[i], OR_OPCODE);
		if((key[i] & KEYMASK) != KEYMASK)
		{
			if(gKey[1])									// Key confirm
			{
				gKey[1] = 0;							// Clear confirm flag
				if(key[i] != gKey[0])					// Different keys
				{
					gKey[2] = NOKEYDOWN;
					return;
				}
				for(j = 0; j < sizeof(pad); j++)		// Key confirmed, save actual key to gKey[2]
				{
					if( (key[i] & pad[j]) == 0 )
					{
						gKey[2] = j + (i << 2);
						return;
					}
				}
			}
			else										// Detect key first
			{
				gKey[0] = key[i];						// save current key to temp key
				gKey[1] = 1;							// key need confirm
				return;
			}
		}
	}
}

void KeyScanStart(void)
{
	gKey[2] = NOKEYDOWN;
	DisableTimerInt(TIMER4);
	SetTimerCall(TIMER4, _isrKeyScan);
	SetHostTimer(TIMER4, 600);							// Timer4 : Key scan interval is 60 ms
}

void KeyScanStop(void)
{
	DisableTimerInt(TIMER4);
}

void KeyProcess(void)
{
	const UINT8 keymap[] = 
	{
		MENU_KEY_UP,
		MENU_KEY_DOWN,
		MENU_KEY_LEFT,
		MENU_KEY_RIGHT,
		MENU_KEY_SELECT
	};

	if(gKey[2] > sizeof(keymap))
		return;

#ifndef	V578CMD_DISABLE
	SmpSendKey(keymap[gKey[2]]);
#endif
	 gKey[2] = NOKEYDOWN;
}

/******************************  Key Process End ******************************/

void shell(void)
{
	int port = 0;
	char str[COMM_CMD_BUFSIZE];

	PrintCmdHeader();
	//KeyScanStart();
	while(gArmRun)
	{
		//KeyProcess();
		//tmPortSelect(CHANNEL_UART0);
		for(port = CHANNEL_UART0; port < CHANNEL_TOTAL; port ++)
		{
			tmPortSelect(port);
			if(tmReadCmd(str, COMM_CMD_BUFSIZE))
				ParseRunCmd(str);
		}
	}
}
