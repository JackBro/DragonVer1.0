
#include <string.h>
#include <stdio.h>
#include "chiprw.h"
#include "cmdlist.h"
#include "shell.h"

static void _Cmodel_SetReg(int argc, char **argv)
{
	int adr;
	int val;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &val);
	Cmodel_SetReg(adr, val);
}

static void _Cmodel_GetReg(int argc, char **argv)
{
	int adr;
	int val;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &adr);
	val = Cmodel_GetReg(adr);
	tmPrintf("%x\n", val);
}

static void _help(int argc, char **argv)
{
	int i;

	if(argc != 2)
	{
		for(i = 0; i < gCmodelCmdCount; i++)
			tmPrintf("%s\n", gCmodelCmdList[i].helpstr);
	}
	else
	{
		for(i = 0; i < gCmodelCmdCount; i++)
		{
			if(strcmp(argv[1], gCmodelCmdList[i].cmdstr) == 0)
			{
				tmPrintf("%s\n", gCmodelCmdList[i].helpstr);
				break;
			}
		}
		if(i == gCmodelCmdCount)
			tmPrintf("System do not support this command yet!\n");
	}
}

static void _Cmodel_ReadSram(int argc, char **argv)
{
	argc = argv[0][0];
}

static void _Cmodel_WriteSram(int argc, char **argv)
{
	argc = argv[0][0];
}

const TCmdUnit gCmodelCmdList[] =
{
	{	"help",			1, 2, "help [cmd]",					SETCMD_TYPE,	_help			},
	{	"V5_SetReg",	3, 3, "V5_SetReg <addr> <val>",		SETCMD_TYPE,	_Cmodel_SetReg	},
	{	"V5_GetReg",	2, 2, "V5_GetReg <addr>",			SETCMD_TYPE,	_Cmodel_GetReg	},

	{	"V5_ReadSram",	3, 3, "V5_ReadSram <adr> <size>",	BUFCMD_TYPE,	_Cmodel_ReadSram	},
	{	"V5_WriteSram",	3, 3, "V5_WriteSram <adr> <size>",	BUFCMD_TYPE,	_Cmodel_WriteSram	}
};

const int gCmodelCmdCount = sizeof(gCmodelCmdList) / sizeof(TCmdUnit);
