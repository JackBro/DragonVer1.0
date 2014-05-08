
#include "main.h"
#include "../v568/568hapi/v5hcmd.h"

typedef struct tag_Tparam{
	UINT32  adr;
	UINT16  type;
	UINT32	size;
	UINT16	offset;
	UINT32	oldptr;
}Tparam;

//======================================
//
//		general command
//
//=================================
static void _help(int argc, char **argv)
{
	int i, j, cnt = 0;

	if(argc != 2)
	{
		for(i = 0; i < gCmdGroupCount; i++)
		{
			for(j = 0; j < gCmdGroup[i].count; j++, cnt++)
				tmPrintf("c%d | %s\n", cnt, gCmdGroup[i].cmdlist[j].helpstr);
		}
	}
	else
	{
		for(i = 0; i < gCmdGroupCount; i++)
		{
			for(j = 0; j < gCmdGroup[i].count; j++, cnt++)
			{
				if(strcmp(argv[1], gCmdGroup[i].cmdlist[j].cmdstr) == 0)
				{
					tmPrintf("c%d | %s\n", cnt, gCmdGroup[i].cmdlist[j].helpstr);
					return;
				}
			}
		}
		if(i == gCmdGroupCount)
			tmPrintf("System do not support this command yet!\n");
	}
}

static void _vmalloc_print(int argc, char **argv)
{
	argv = argv;
	
	if(argc < 1)
		return;

	VmallocPrint();
}

static const TCmdUnit gCmdList[] =
{
	{	"help",			1, 2, "help [cmd]",					SETCMD_TYPE,	_help			},
	{	"ls",			2, 2, "ls <directory>",				SETCMD_TYPE,	ls_command		},
	{	"format",		1, 1, "format",						SETCMD_TYPE,	format_command	},
	{	"rm",			2, 2, "rm <filename>",				SETCMD_TYPE,	rm_command		},
	{	"mv",			3, 3, "mv <name1> <name2>",			SETCMD_TYPE,	mv_command		},
	{	"cp",			3, 3, "cp <file1> <file2>",			SETCMD_TYPE,	cp_command		},
	{	"mkdir",		2, 2, "mkdir <directory>",			SETCMD_TYPE,	mkdir_command	},
	{	"upload",		2, 2, "upload <filename>",			BUFCMD_TYPE,	upload_command	},
	{	"vmalloc_print",1, 1, "vmalloc_print",				SETCMD_TYPE,	_vmalloc_print	}
	
};

//=================================================
//
//		Host command
//
//=============================
#ifndef HOSTCMD_DISABLE

static void _WriteBuf(int argc, char **argv)
{
	int len, adr;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &len);
	len = WriteBuf(adr, gTransferBuf, len);
	tmPrintf("%x\n", len);
}

static void _ReadBuf(int argc, char **argv)
{
	int len, adr;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &len);
	len = ReadBuf(adr, NULL, len);
	tmPrintf("%x\n", len);
}

static void _GetHostMclk(int argc, char **argv)
{
	int mclk;
	
	argc = (int)argv[0][0];
	mclk = GetHostMclk();
	tmPrintf("%d\n", mclk);
}

static void _GetI2sClk(int argc, char **argv)
{
	int clk;
	
	argc = (int)argv[0][0];
	clk = GetI2sClk();
	tmPrintf("%d\n", clk);
}

static void _AdjustHostMclk(int argc, char **argv)
{
	int mclk;

	if(argc < 2)
		return;
	sscanf(argv[1], "%d", &mclk);
	AdjustHostMclk(mclk);
	mclk = GetHostMclk();
	tmPrintf("Host clk is changed to : %d[KHz]\n", mclk);
}

static void _AdjustI2sClk(int argc, char **argv)
{
	int clk;

	if(argc < 2)
		return;
	sscanf(argv[1], "%d", &clk);
	AdjustI2sClk(clk);
	clk = GetI2sClk();
	tmPrintf("I2s clk is changed to : %d[KHz]\n", clk);
	clk = GetHostMclk();
	tmPrintf("Host clk is changed to : %d[KHz]\n", clk);
}

static void _Gbuf2File(int argc, char **argv)
{
	int len, adr;

	if(argc < 4)
		return;
	sscanf(argv[2], "%x", &len);
	sscanf(argv[3], "%x", &adr);
	Gbuf2File(argv[1], len, adr);
}

static void _File2Gbuf(int argc, char **argv)
{
	int adr;
	if(argc < 3)
		return;
	sscanf(argv[2], "%x", &adr);
	File2Gbuf(argv[1], adr);
}

static void _File2pc(int argc, char **argv)
{
	if(argc < 2)
		return;
	File2pc(argv[1], NULL);
}

static void _WriteMemory(int argc, char **argv)
{
	int adr, val;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &val);
	WriteMemory(adr, val);
}

static void _ReadMemory(int argc, char **argv)
{
	int adr, val;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &adr);
	val = ReadMemory(adr);
	tmPrintf("%x\n", val);
}

static void _AdjustBusTiming(int argc, char **argv)
{
	int idx, tacs, tcos, tacc, toch, tcah, tacp;

	if(argc < 8)
		return;
	sscanf(argv[1], "%x", &idx);
	sscanf(argv[2], "%x", &tacs);
	sscanf(argv[3], "%x", &tcos);
	sscanf(argv[4], "%x", &tacc);
	sscanf(argv[5], "%x", &toch);
	sscanf(argv[6], "%x", &tcah);
	sscanf(argv[7], "%x", &tacp);
	AdjustBusTiming(idx, tacs, tcos, tacc, toch, tcah, tacp);
}

static void _PrintBusTiming(int argc, char **argv)
{
	int idx;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &idx);
	PrintBusTiming(idx);
}

static void _SetUartBaud(int argc, char **argv)
{
	int adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%d", &size);
	SetUartBaud(adr, size);
}

static void _testDelay(int argc, char **argv)
{
	int count;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &count);
	testDelay(count);
}

static void _PrintSdramInfo(int argc, char **argv)
{
	argc = argv[0][0];	//avoid warning
	PrintSdramInfo();
}

static void _GetWdtCnt(int argc, char **argv)
{
	unsigned cnt;
	
	argc = (int)argv[0][0];
	cnt = WdtGetCount();
	tmPrintf("%d\n", cnt);
}

static const TCmdUnit gHostCmdList[] =
{
	{	"PrintSdramInfo",	1, 1, "PrintSdramInfo",			SETCMD_TYPE,	_PrintSdramInfo	},

	{	"Gbuf2File",	4, 4, "Gbuf2File <filename> <len> <adr>",	SETCMD_TYPE,	_Gbuf2File		},
	{	"File2Gbuf",	3, 3, "File2Gbuf <filename> <adr>",	SETCMD_TYPE,	_File2Gbuf		},
	{	"WriteMemory",	3, 3, "WriteMemory <addr> <val>",	SETCMD_TYPE,	_WriteMemory	},
	{	"AdjustHostMclk",2, 2, "AdjustHostMclk <khz>",		SETCMD_TYPE,	_AdjustHostMclk	},
	{	"AdjustI2sClk",	2, 2, "AdjustI2sClk <khz>",			SETCMD_TYPE,	_AdjustI2sClk	},
	{	"SetUartBaud",	3, 3, "SetUartBaud <port> <baud>",	SETCMD_TYPE,	_SetUartBaud	},

	{	"PrintBusTiming",	2, 2, "PrintBusTiming",			SETCMD_TYPE,	_PrintBusTiming	},
	{	"AdjustBusTiming",	8, 8, "AdjustBusTiming",		SETCMD_TYPE,	_AdjustBusTiming	},

	{	"testDelay",	2, 2, "testDelay <count>",			SETCMD_TYPE,	_testDelay	},
	{	"ReadMemory",	2, 2, "ReadMemory <addr>",			READCMD_TYPE,	_ReadMemory		},
	{	"GetHostMclk",	1, 1, "GetHostMclk",				READCMD_TYPE,	_GetHostMclk	},
	{	"GetI2sClk",	1, 1, "GetI2sClk",					READCMD_TYPE,	_GetI2sClk	},
	{	"GetWdtCnt",	1, 1, "GetWdtCnt",					READCMD_TYPE,	_GetWdtCnt	},

	{	"WriteBuf",		3, 3, "WriteBuf <adr> <size>",		BUFCMD_TYPE,	_WriteBuf		},
	{	"ReadBuf",		3, 3, "ReadBuf <adr> <size>",		BUFCMD_TYPE,	_ReadBuf		},
	{	"File2pc",		2, 2, "File2pc <filename>",			BUFCMD_TYPE,	_File2pc		}
};
#endif


//=================================================
//
//		USB command
//
//=============================
#ifndef USBCMD_DISABLE

static void _Usb_SetReg(int argc, char **argv)
{
	UINT8 adr, val;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &val);
	Usb_SetReg(adr, val);
}

static void _Usb_GetReg(int argc, char **argv)
{
	UINT8 adr;
	UINT8 val;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &adr);
	val = Usb_GetReg(adr);
	tmPrintf("%x\n", val);
}

static void _Usb_ReadFifo(int argc, char **argv)
{
	int adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	Usb_ReadFifo((UINT8)adr, (UINT8 *)gTransferBuf, (UINT8)size);
	size = ReadBuf(0, gTransferBuf, size);
	tmPrintf("%x\n", size);
}

static void _Usb_WriteFifo(int argc, char **argv)
{
	UINT8 adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	WriteBuf(0, gTransferBuf, size);
	size = Usb_WriteFifo(adr, (UINT8 *)gTransferBuf, size);
	tmPrintf("%x\n", size);
}

static void _OpenUsb(int argc, char **argv)
{
	argc = argv[0][0];	//avoid warning
	OpenUsb();
}

//static void _UsbStructPrint(int argc, char **argv)
//{
//	argc = argv[0][0];	//avoid warning
//	UsbStructPrint();
//}

static const TCmdUnit gUsbCmdList[] =
{
	{	"Usb_SetReg",	3, 3, "Usb_SetReg <adr> <val>",		SETCMD_TYPE,	_Usb_SetReg	},
	{	"OpenUsb",		1, 1, "OpenUsb",		SETCMD_TYPE,	_OpenUsb	},

//	{	"UsbStructPrint",	1, 1, "UsbStructPrint",		SETCMD_TYPE,	_UsbStructPrint	},
	{	"Usb_GetReg",	2, 2, "Usb_GetReg <addr>",			READCMD_TYPE,	_Usb_GetReg		},

	{	"Usb_WriteFifo",3, 3, "Usb_WriteFifo <adr> <size>",	BUFCMD_TYPE,	_Usb_WriteFifo	},
	{	"Usb_ReadFifo",	3, 3, "Usb_ReadFifo <adr> <size>",	BUFCMD_TYPE,	_Usb_ReadFifo	}
};
#endif

//=================================================
//
//		V578 command
//
//=============================
#ifndef V578CMD_DISABLE

static void _V5_SetReg(int argc, char **argv)
{
	UINT32 adr;
	UINT32 val;
	UINT8 regWType;

	if(argc < 4)
		return;

	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &val);
	sscanf(argv[3], "%x", &regWType);

	V5_SetReg(adr, val, regWType);
}

static void _V5_GetReg(int argc, char **argv)
{
	UINT32 adr;
	UINT32 val;
	UINT8 regWType;

	if(argc < 3)
		return;

	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &regWType);

	val = V5_GetReg(adr, regWType);
	
	tmPrintf("%x\n", val);
}

static void _V5_ReadSram(int argc, char **argv)
{
	UINT32 adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	V5_ReadSram(adr, (UINT8 *)gTransferBuf, size);
	size = ReadBuf(0, gTransferBuf, (int)size);
	tmPrintf("%x\n", size);
}

static void _V5_WriteSram(int argc, char **argv)
{
	UINT32 adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	WriteBuf(0, gTransferBuf, (int)size);
	size = V5_WriteSram(adr, (UINT8 *)gTransferBuf, size);
	tmPrintf("%x\n", size);
}

static void _V5_WriteSramRev(int argc, char **argv)
{
	UINT32 adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	WriteBuf(0, gTransferBuf, (int)size);
	size = V5_WriteSramRev(adr, (UINT8 *)gTransferBuf, size);
	tmPrintf("%x\n", size);
}

static void _V5_WriteSramOnWord(int argc, char **argv)
{
	UINT32 adr, dat, size;

	if(argc < 4)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &dat);
	sscanf(argv[3], "%x", &size);

	size = V5_WriteSramOnWord(adr, dat, size);
	tmPrintf("%x\n", size);
}

static void _V5B_SetBusType(int argc, char **argv)
{
	UINT8 type;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &type);

	V5B_SetBusType(type);

	if(type == V5_BUS_MULTI16)
		BusInit(BUS_IDX_4, BUS16);
	else
		BusInit(BUS_IDX_4, BUS8);
}

static void _V5B_BiuSetBypassSelType(int argc, char **argv)
{
	UINT8 selType;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &selType);

	V5B_BiuSetBypassSelType(selType);
}

static void _testRegRw(int argc, char **argv)
{
	UINT32 adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	testRegRw(adr, size);
}

static void _testSramRw(int argc, char **argv)
{
	UINT32 adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	testSramRw(adr, size);
}

static void _testSramWriteBackRw(int argc, char **argv)
{
	UINT32 adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	testSramWriteBackRw(adr, size);
}

static void _V5_GetRegTest(int argc, char **argv)
{
	UINT32 adr;
	UINT8 val;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &adr);
	val = V5_GetRegTest(adr);
	tmPrintf("%x\n", val);
}

static void _V5_SetRegTest(int argc, char **argv)
{
	UINT32 adr, val;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &val);
	V5_SetRegTest(adr, (UINT8)val);
}

static void _testSramRwTime(int argc, char **argv)
{
	UINT32 adr, size;
	UINT8 w;

	if(argc < 4)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	sscanf(argv[3], "%x", &w);

	w = testSramRwTime(adr, size, w);

	tmPrintf("%x\n", w);
}

static void _testIntInit(int argc, char **argv)
{
	argv = argv;

	if(argc < 1)
		return;

    testIntInit(); 	
}

static void _V5_PortSwitchTest(int argc, char **argv)
{
	UINT32 val;

	if(argc < 2)
		return;
	
	sscanf(argv[1], "%x", &val);

    V5_PortSwitchTest((UINT8)val); 	
}

static void _testRegConRead(int argc, char **argv)
{
	UINT32 addr, readCnt;

	if(argc < 3)
		return;

	sscanf(argv[1], "%x", &addr);
	sscanf(argv[2], "%x", &readCnt);

	testRegConRead(addr, readCnt);
}

static void _V5B_SetBypassOrNormal(int argc, char **argv)
{
	UINT32 byOrNormal;

	if(argc < 2)
		return;
	
	sscanf(argv[1], "%x", &byOrNormal);

    V5B_SetBypassOrNormal((UINT8)byOrNormal); 
}

static void _V5_BySetPanelSel(int argc, char **argv)
{
	UINT32 panelSel;

	if(argc < 2)
		return;
	
	sscanf(argv[1], "%x", &panelSel);

    V5_BySetPanelSel((UINT8)panelSel); 
}

static void _V5_BySetRSLow(int argc, char **argv)
{
	UINT32 val;

	if(argc < 2)
		return;
	
	sscanf(argv[1], "%x", &val);

    V5_BySetRSLow((unsigned short)val); 
}

static void _V5_BySetRSHigh(int argc, char **argv)
{
	UINT32 val;

	if(argc < 2)
		return;
	
	sscanf(argv[1], "%x", &val);

    V5_BySetRSHigh((unsigned short)val); 
}

static void _V5_ByGetRSLow(int argc, char **argv)
{
	unsigned short val;

	argv = argv;

	if(argc < 1)
		return;

    val = V5_ByGetRSLow(); 

	tmPrintf("%x\n", val);
}

static void _V5_ByGetRSHigh(int argc, char **argv)
{
	unsigned short val;

	argv =argv;

	if(argc < 1)
		return;

    val = V5_ByGetRSHigh();

	tmPrintf("%x\n", val);
}

static void _RepeatWriteReg(int argc, char **argv)
{
	UINT32 adr, size, len;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	len = RepeatWriteReg(adr, size);

	tmPrintf("%x\n", len);
}

static void _AdjustAe(int argc, char **argv)
{
	UINT32 et;
	UINT8 ygain;
	UINT8 env;

	if(argc < 4)
		return;
	sscanf(argv[1], "%ld", &et);
	sscanf(argv[2], "%d", &ygain);
	sscanf(argv[3], "%d", &env);

	AdjustAe(et, ygain, env);
}

static void _ClearGbufPoint(int argc, char **argv)
{
	argc = argv[0][0];
	ClearGbufPoint();
}

static void _Sram2Gbuf(int argc, char **argv)
{
	UINT32 adr, size, val;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	val = Sram2Gbuf(adr, size);
	tmPrintf("%x\n", val);
}

static void _SetGbufMode(int argc, char **argv)
{
	int mode;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &mode);
	SetGbufMode((UINT8)mode);
}

static void _SetGbufAdrInfo(int argc, char **argv)
{
	UINT32 adr, jadr, tadr, iadr;

	if(argc < 5)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &jadr);
	sscanf(argv[3], "%x", &tadr);
	sscanf(argv[4], "%x", &iadr);
	SetGbufAdrInfo(adr, jadr, tadr, iadr);
}


static void _SmpSendKey(int argc, char **argv)
{
	UINT32 val;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &val);
	SmpSendKey((UINT8)val);
}

static void _V5_SendCommand(int argc, char **argv)
{
	char paramEntity[300];    //memory can also be allocated  according totalsize;
	UINT32 ret, curadr, msg;
	int  ptrcount = 0, i = 0;
	Tparam ptr[10];

	curadr = 0;
	ptrcount = (argc - 2) / 3;
	sscanf(argv[1], "%d", &msg);

	while ( i < ptrcount)
	{
		sscanf(argv[i*3 + 2], "%d", &ptr[i].type);
		sscanf(argv[i*3 + 3], "%x", &ptr[i].size);
		sscanf(argv[i*3 + 4], "%x", &ptr[i].offset);
		ptr[i].adr = curadr;
		curadr += ptr[i].size;
		i ++ ;		
	}
	
	for (i = 0; i < ptrcount; i++ )
	{
		if ((ptr[i].type == PTR_OUT) || (ptr[i].type == PTR_INOUT))
			tmReadBuf4hcmd((char *) (paramEntity + ptr[i].adr), (int) ptr[i].size);

		if (i > 0)
			{
				ptr[i].oldptr = *((UINT32 *)(paramEntity + ptr[i].offset));
				*((UINT32 *)(paramEntity + ptr[i].offset)) = (UINT32)(paramEntity + ptr[i].adr);
			}
	}

	ret = V5_SendCommand(msg, paramEntity, 0);
	tmWriteBuf((char *) &ret, sizeof(ret));

	for (i = ptrcount - 1; i >= 0; i-- )
	{
		if ((ptr[i].type == PTR_IN) || (ptr[i].type == PTR_INOUT))
			tmWriteBuf((char *)(paramEntity + ptr[i].adr), (int) ptr[i].size);
		if (i > 0)
			*((UINT32 *)(paramEntity + ptr[i].offset)) = ptr[i].oldptr;
	}
}

static void _RtReqJbuf(int argc, char **argv)
{
	UINT32 jbufst, jbuflen, val;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &jbufst);
	sscanf(argv[2], "%x", &jbuflen);
	val = RtReqJbuf(jbufst, jbuflen);
	tmPrintf("%x\n", val);
}


static const TCmdUnit gV578CmdList[] =
{
	{	"V5_SetReg",	4, 4, "V5_SetReg <addr> <val><regWType>",		SETCMD_TYPE,	_V5_SetReg	},
	{	"V5_GetReg",	3, 3, "V5_GetReg <addr><regWType>", 			READCMD_TYPE,	_V5_GetReg	},
	{	"ClearGbufPoint",1, 1, "ClearGbufPoint <adr> <times>",SETCMD_TYPE,	_ClearGbufPoint	},
	{	"SmpSendKey",		2, 2, "SmpSendKey <val>",		SETCMD_TYPE,	_SmpSendKey	},
	{	"SetGbufMode",	2, 2, "SetGbufMode <mode>",			SETCMD_TYPE,	_SetGbufMode},
	{	"SetGbufAdrInfo",	5, 5, "SetGbufAdrInfo <adr> <jadr> <tadr> <iadr>",SETCMD_TYPE,	_SetGbufAdrInfo},
	{	"V5B_SetBusType",	2, 2, "V5B_SetBusType <type>",SETCMD_TYPE,	_V5B_SetBusType},
	{	"V5B_BiuSetBypassSelType",	2, 2, "V5B_BiuSetBypassSelType <selType>",SETCMD_TYPE,	_V5B_BiuSetBypassSelType},

//=========
	{	"testRegRw",	3, 3, "testRegRw <adr> <count>",	SETCMD_TYPE,	_testRegRw	},
	{	"testSramRw",	3, 3, "testSramRw <adr> <size>",	SETCMD_TYPE,	_testSramRw	},
	{	"testSramWriteBackRw",	3, 3, "testSramWriteBackRw <adr> <size>",	SETCMD_TYPE,	_testSramWriteBackRw	},
	{	"V5_SetRegTest",	3, 3, "V5_SetRegTest <adr> <size>",	SETCMD_TYPE,	_V5_SetRegTest	},
	{	"V5_GetRegTest",	2, 2, "V5_GetRegTest <adr>",	SETCMD_TYPE,	_V5_GetRegTest },
	{	"testSramRwTime",4, 4, "testSramRwTime <adr> <size> <w>",READCMD_TYPE,	_testSramRwTime	},
    
	{	"testIntInit",	1, 1, "testIntInit ",		        SETCMD_TYPE,	_testIntInit	},
	{	"V5_PortSwitchTest",	2, 2, "V5_PortSwitchTest <port>",		SETCMD_TYPE,	_V5_PortSwitchTest },
	{ 	"testRegConRead",	3, 3, "testRegConRead <addr><count>",	SETCMD_TYPE,	_testRegConRead	},

	{	"V5B_SetBypassOrNormal",	2, 2, "V5B_SetBypassOrNormal<val>",	SETCMD_TYPE,	_V5B_SetBypassOrNormal },
	{	"V5_BySetPanelSel",	2, 2, "V5_BySetPanelSel <val>",	SETCMD_TYPE,	_V5_BySetPanelSel },
    {	"V5_BySetRSLow",	2, 2, "V5_BySetRSLow <val>",	SETCMD_TYPE,	_V5_BySetRSLow },
	{	"V5_BySetRSHigh",	2, 2, "V5_BySetRSHigh <val>",	SETCMD_TYPE,	_V5_BySetRSHigh },
	{	"V5_ByGetRSLow",	1, 1, "V5_ByGetRSLow",	        SETCMD_TYPE,	_V5_ByGetRSLow },
	{	"V5_ByGetRSHigh",	1, 1, "V5_ByGetRSHigh",	        SETCMD_TYPE,	_V5_ByGetRSHigh },

	{	"AdjustAe",		4, 4, "AdjustAe <et> <ygain><env>",	SETCMD_TYPE,	_AdjustAe	},
//==============

	{	"Sram2Gbuf",	3, 3, "Sram2Gbuf <adr> <len>",		READCMD_TYPE,	_Sram2Gbuf	},
	{	"RepeatWriteReg",3, 3, "RepeatWriteReg <adr> <times>",READCMD_TYPE,	_RepeatWriteReg	},

	{	"V5_ReadSram",	3, 3, "V5_ReadSram <adr> <size>",	READCMD_TYPE,	_V5_ReadSram	},
	{	"V5_WriteSram",	3, 3, "V5_WriteSram <adr> <size>",	SETCMD_TYPE,	_V5_WriteSram	},
	{	"V5_WriteSram",	3, 3, "V5_WriteSramRev <adr> <size>",	SETCMD_TYPE,	_V5_WriteSramRev	},
	{	"V5_WriteSramOnWord",	4, 4, "V5_WriteSramOnWord <adr> <data><size>",	SETCMD_TYPE,	_V5_WriteSramOnWord	},
	{	"V5_SendCommand", 2, 15, "V5_SendCommand <msg> <paramsize> ...",	READCMD_TYPE,	_V5_SendCommand },
	{	"RtReqJbuf",	3, 3, "RtReqJbuf <jbufst> <jbuflen>",		READCMD_TYPE,	_RtReqJbuf	}
};
#endif

//=================================================
//
//		V938 command
//
//=============================
#ifndef V938CMD_DISABLE

static void _V9_SetReg(int argc, char **argv)
{
	UINT32 adr;
	UINT32 val;
	UINT8 regWType;

	if(argc < 4)
		return;

	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &val);
	sscanf(argv[3], "%x", &regWType);

	V9_SetReg(adr, val, regWType);
}

static void _V9_GetReg(int argc, char **argv)
{
	UINT32 adr;
	UINT32 val;
	UINT8 regWType;

	if(argc < 3)
		return;

	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &regWType);

	val = V9_GetReg(adr, regWType);
	
	tmPrintf("%x\n", val);
}

static void _V9_WriteSram(int argc, char **argv)
{
	UINT32 adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	WriteBuf(0, gTransferBuf, (int)size);
	size = V9_WriteSram(adr, (UINT8 *)gTransferBuf, size);
	tmPrintf("%x\n", size);
}

static void _V9_ReadSram(int argc, char **argv)
{
	UINT32 adr, size;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &size);
	WriteBuf(0, gTransferBuf, (int)size);
	size = V9_ReadSram(adr, (UINT8 *)gTransferBuf, size);
	tmPrintf("%x\n", size);
}

static void _testIntInit938(int argc, char **argv)
{
	argc = argc;
	argv = argv;

	if(argc < 1)
		return;

	testIntInit938();
}

static const TCmdUnit gV938CmdList[] =
{
	{	"V9_SetReg",	4, 4, "V9_SetReg <addr> <val><regWType>",		SETCMD_TYPE,	_V9_SetReg	},
	{	"V9_GetReg",	3, 3, "V9_GetReg <addr><regWType>", 			READCMD_TYPE,	_V9_GetReg	},
	{	"V9_WriteSram",	3, 3, "V9_WriteSram <adr> <size>",           	SETCMD_TYPE,	_V9_WriteSram	},
	{	"V9_ReadSram",	3, 3, "V9_ReadSram <adr> <size>",           	SETCMD_TYPE,	_V9_ReadSram	},
	{	"testIntInit938",	1, 1, "testIntInit938",           	        SETCMD_TYPE,	_testIntInit938	}
};

#endif

//=================================================
//
//		Nandflash command
//
//=============================

#ifndef	HOSTCMD_DISABLE

static void _WriteNandCmd(int argc, char **argv)
{
	int cmd = 0, unit = 0;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &cmd);
	sscanf(argv[2], "%x", &unit);
	NandSetCmdBase(cmd, unit);
}

static void _WriteNandAddr(int argc, char **argv)
{
	int adr = 0, unit = 0;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &adr);
	sscanf(argv[2], "%x", &unit);
	NandSetAddrBase(adr, unit);
}

static void _WriteNandData(int argc, char **argv)
{
	int data = 0, unit = 0;

	if(argc < 3)
		return;
	sscanf(argv[1], "%x", &data);
	sscanf(argv[2], "%x", &unit);
	NandSetDataBase(data, unit);
}

static void _ReadNandData(int argc, char **argv)
{
	int val = 0, unit = 0;

	if(argc < 2)
		return;
	sscanf(argv[1], "%x", &unit);
	val = NandGetDataBase(unit);
	tmPrintf("%x\n", val);
}

static const TCmdUnit gNandCmdList[] =
{
	{	"WriteNandCmd",		3, 3, "WriteNandCmd <cmd> <unit>",		SETCMD_TYPE,	_WriteNandCmd	},
	{	"WriteNandAddr",	3, 3, "WriteNandAddr <addr> <unit>",	SETCMD_TYPE,	_WriteNandAddr	},
	{	"WriteNandData",	3, 3, "WriteNandData <data> <unit>",	SETCMD_TYPE,	_WriteNandData	},
	{	"ReadNandData",		2, 2, "ReadNandData <unit>",			READCMD_TYPE,	_ReadNandData	}
};

#endif

//=================================================
//
//		Utility command
//
//=============================

void _nullCmd(int argc, char **argv) 
{
	argc = argv[0][0];
}

//void _jpegdec(int argc, char **argv) 
//{
//	UINT32 mode;
//
//	if(argc < 2)
//		return;
//	sscanf(argv[1], "%x", &mode);
//	
//	jpegdec_entry(mode);
//	tmPrintf("%x ok\n", mode);
//}

static const TCmdUnit gUtilityCmdList[] =
{
	{	"No Func",		1, 1, "No Func yet",				SETCMD_TYPE,	_nullCmd	}
//	{	"jpegdec",		2, 2, "jpegdec <mode>",				SETCMD_TYPE,	_jpegdec	}
	// Utility command : for test / temp command, which is difficult to classified.
	// Add your command below, and mark the first command line, which is only to avoid compile error.
};




//=================================================
//
//		command group
//
//=============================

const TCmdGroup gCmdGroup[] = 
{
	{ (TCmdUnit *)gCmdList, sizeof(gCmdList) / sizeof(TCmdUnit) }
#ifndef HOSTCMD_DISABLE
	,{ (TCmdUnit *)gHostCmdList, sizeof(gHostCmdList) / sizeof(TCmdUnit) }
#endif

#ifndef HOSTCMD_DISABLE
	,{ (TCmdUnit *)gNandCmdList, sizeof(gNandCmdList) / sizeof(TCmdUnit) }
#endif

#ifndef V578CMD_DISABLE
	,{ (TCmdUnit *)gV578CmdList, sizeof(gV578CmdList) / sizeof(TCmdUnit) }
#endif

#ifndef USBCMD_DISABLE
	,{ (TCmdUnit *)gUsbCmdList, sizeof(gUsbCmdList) / sizeof(TCmdUnit) }
#endif

#ifndef V938CMD_DISABLE
	,{ (TCmdUnit *)gV938CmdList, sizeof(gV938CmdList) / sizeof(TCmdUnit) }
#endif

	,{ (TCmdUnit *)gUtilityCmdList, sizeof(gUtilityCmdList) / sizeof(TCmdUnit) }
};

const int gCmdGroupCount = sizeof(gCmdGroup) / sizeof(TCmdGroup);




