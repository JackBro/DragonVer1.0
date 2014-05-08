#include "internal.h"

/*
v568 map range: 0		--	0x3fff
usb  map range: 0x4000	-	0x4fff
host map range: 0x5000	-	0x7fff
*/

static void _SimReg_SetReg(int argc, char **argv)
{
	int adr, val, type;

	if(argc < 4)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &val);
	sscanf(argv[3], "%x", &type);
	SimReg_SetReg(adr, val, type);
}

static void _SimReg_GetReg(int argc, char **argv)
{
	int adr, val, type;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &type);
	val = SimReg_GetReg(adr, type);
	SimPrintf("%x\n", val);
}

static void _SimUsb_SetReg(int argc, char **argv)
{
	int adr, val;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &val);
	SimReg_SetReg(adr, val, 0);
}

static void _SimUsb_GetReg(int argc, char **argv)
{
	int adr, val;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &adr);
	val = SimReg_GetReg(adr, 0);
	SimPrintf("%x\n", val);
}

static void _SimHost_SetReg(int argc, char **argv)
{
	int adr, val;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &val);
	Simhost_SetReg(adr, val);
}

static void _SimHost_GetReg(int argc, char **argv)
{
	int adr, val;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &adr);
	val = Simhost_GetReg(adr);
	SimPrintf("%x\n", val);
}

static void _Sim_ReadSram(int argc, char **argv)
{
	int adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
}

static void _Sim_WriteSram(int argc, char **argv)
{
	int adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
}

static void _help(int argc, char **argv)
{
	int i;

	if(argc != 2)
	{
		for(i = 0; i < gSimRegCmdCount; i++)
			SimPrintf("%s\n", gSimRegCmdList[i].helpstr);
	}
	else
	{
		for(i = 0; i < gSimRegCmdCount; i++)
		{
			if(strcmp(argv[1], gSimRegCmdList[i].cmdstr) == 0)
			{
				SimPrintf("%s\n", gSimRegCmdList[i].helpstr);
				break;
			}
		}
		if(i == gSimRegCmdCount)
			SimPrintf("System do not support this command yet!\n");
	}
}



static void _SimGetHostMclk(int argc, char **argv)
{
	int mclk;
	
	argc = (int)argv[0][0];
	mclk = DEFAULT_MCLK;
	SimPrintf("%d\n", mclk);
}

const TCmdUnit gSimRegCmdList[] =
{
	{	"help",			1, 2, "help [cmd]",					SETCMD_TYPE,	_help			},

	{	"V5_SetReg",	3, 4, "V5_SetReg <addr> <val> <type>",	SETCMD_TYPE,	_SimReg_SetReg	},
	{	"V5_GetReg",	2, 3, "V5_GetReg <addr> <type>",		READCMD_TYPE,	_SimReg_GetReg	},

	{	"V9_SetReg",	3, 4, "V9_SetReg <addr> <val> <type>",	SETCMD_TYPE,	_SimReg_SetReg	},
	{	"V9_GetReg",	2, 3, "V9_GetReg <addr> <type>",		READCMD_TYPE,	_SimReg_GetReg	},

	{	"Usb_SetReg",	3, 3, "Usb_SetReg <addr> <val>",	SETCMD_TYPE,	_SimUsb_SetReg	},
	{	"Usb_GetReg",	2, 2, "Usb_GetReg <addr>",			READCMD_TYPE,	_SimUsb_GetReg	},

	{	"WriteMemory",	3, 3, "WriteMemory <addr> <val>",	SETCMD_TYPE,	_SimHost_SetReg	},
	{	"ReadMemory",	2, 2, "ReadMemory <addr>",			READCMD_TYPE,	_SimHost_GetReg	},

	{	"GetHostMclk",	1, 1, "GetHostMclk",				READCMD_TYPE,	_SimGetHostMclk	},

	{	"V5_ReadSram",	3, 3, "V5_ReadSram <adr> <size>",	BUFCMD_TYPE,	_Sim_ReadSram	},
	{	"V5_WriteSram",	3, 3, "V5_WriteSram <adr> <size>",	BUFCMD_TYPE,	_Sim_WriteSram	},
	{	"Usb_WriteFifo",3, 3, "Usb_WriteFifo <adr> <size>",	BUFCMD_TYPE,	_Sim_ReadSram	},
	{	"Usb_ReadFifo",	3, 3, "Usb_ReadFifo <adr> <size>",	BUFCMD_TYPE,	_Sim_WriteSram	}
};

const int gSimRegCmdCount = sizeof(gSimRegCmdList) / sizeof(TCmdUnit);
