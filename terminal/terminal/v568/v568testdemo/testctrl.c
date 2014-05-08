#include "internal.h"
#ifndef	V578CMD_DISABLE

#include "../driver/basefunc/basefunc.h"


static void isrHandle(UINT8 type, UINT16 flg);

static void __isrINT_SIF		(UINT16 flg){ isrHandle(ISR_INT_SIF,	flg);}
static void __isrINT_ISP		(UINT16 flg){ isrHandle(ISR_INT_ISP,	flg);}
static void __isrINT_LBUF		(UINT16 flg){ isrHandle(ISR_INT_LBUF,	flg);}
static void __isrINT_JPEG		(UINT16 flg){ isrHandle(ISR_INT_JPEG,	flg);}
static void __isrINT_GE			(UINT16 flg){ isrHandle(ISR_INT_GE,		flg);}
static void __isrINT_MARB0		(UINT16 flg){ isrHandle(ISR_INT_MARB0,	flg);}
static void __isrINT_LCDC		(UINT16 flg){ isrHandle(ISR_INT_LCDC,	flg);}
static void __isrINT_MARB1		(UINT16 flg){ isrHandle(ISR_INT_MARB1,	flg);}

TIntIsr gIntisr[] =
{
	{	ISR_INT_SIF		,	"SIF_VSYNC			\n",	INT_SIF		,	__isrINT_SIF	},
	{	ISR_INT_ISP		,	"ISP_Y_MEAN_INT		\n",	INT_ISP		,	__isrINT_ISP	},
	{	ISR_INT_LBUF	,	"LBUF_READ			\n",	INT_LBUF	,	__isrINT_LBUF	},
	{	ISR_INT_JPEG	,	"JPEG_DEC_ERR		\n",	INT_JPEG	,	__isrINT_JPEG	},
	{	ISR_INT_GE		,	"GE_BUF_DONE		\n",	INT_GE		,	__isrINT_GE		},
	{	ISR_INT_MARB0	,	"MARB0				\n",	INT_MARB0	,	__isrINT_MARB0	},
	{	ISR_INT_LCDC	,	"LCDC_LAYERB_DONE	\n",	INT_LCDC	,	__isrINT_LCDC	},
	{	ISR_INT_MARB1	,	"MARB1				\n",	INT_MARB1	,	__isrINT_MARB1	}
};


int gIntnum = sizeof(gIntisr)/sizeof(TIntIsr);


void TestCtrlInit()
{
	V5_ChipCtrlOpen();
}

void V5_ChipInitContext(void)
{
	V5_CtrlInitContext();
	V5_DispInitContext();
}

void V5_ChipCtrlOpen(void)
{
	V5_CtrlInitContext();
	V5_CtrlSetInfo((PTCtrlInfo)&gCtrlInfo);
	V5_CtrlOpen();
}

void testSramRw(UINT32 adr, UINT32 size)
{
	UINT32 i, len;
	char str[100];

	if(size > 0x60000)
	{
		size = 0x60000;
	}

	V5B_MarbSubReset(0);

	for(i=0; i<size; i++)
	{
		gTransferBuf[i] = (SINT8)i;
	}

	len = V5_WriteSram(adr, (UINT8*)gTransferBuf, size);
	if(len != size)
	{
		sprintf(str, "write sram length != size 0x%lx\n", len);
		tmPrintf(str);
	}

	len = V5_ReadSram(adr, (UINT8*)gTransferBuf, size);
	if(len != size)
	{
		sprintf(str, "read sram length != size 0x%lx\n", len);
		tmPrintf(str);
	}

	for(i=0; i<size; i++)
	{
		if(gTransferBuf[i] != ((SINT8)i))
		{
			sprintf(str, "data mismatch at 0x%lx\n", i);
			sprintf(str, "%sval = 0x%x, val1 = 0x%x\n", str, (SINT8)i, gTransferBuf[i]);
			tmPrintf(str);
			break;
		}
	}

	if(i == size)
	{
		tmPrintf("Read write function is ok!\n");
	}
}

void testSramWriteBackRw(UINT32 adr, UINT32 size)
{
	UINT32 i, len;
	char str[100];

	if(size > 0x60000)
	{
		size = 0x60000;
	}

	V5B_MarbSubReset(0);

	for(i=0; i<size; i++)
	{
		gTransferBuf[i] = (SINT8)i;
	}

	len = V5_WriteSramTest(adr, (UINT8*)gTransferBuf, size);
	if(len != size)
	{
		sprintf(str, "write sram length != size 0x%lx\n", len);
		tmPrintf(str);
	}

	len = V5_ReadSramTest(adr, (UINT8*)gTransferBuf, size);
	if(len != size)
	{
		sprintf(str, "read sram length != size 0x%lx\n", len);
		tmPrintf(str);
	}

	for(i=0; i<size; i++)
	{
		if(gTransferBuf[i] != ((SINT8)i))
		{
			sprintf(str, "data mismatch at 0x%lx\n", i);
			sprintf(str, "%sval = 0x%x, val1 = 0x%x\n", str, (SINT8)i, gTransferBuf[i]);
			tmPrintf(str);
			break;
		}
	}

	if(i == size)
	{
		tmPrintf("Read write function is ok!\n");
	}
}

UINT8 testSramRwTime(UINT32 adr, UINT32 size, UINT8 w)
{
	UINT32 len;

	if(size > 0x60000)
	{
		size = 0x60000;
	}

	if(size == 0)
	{
		return 1;
	}

	if(w)
	{
		if(adr > V5_SRAM_BASE)
		{
			while(size--)
			{
				V5_SetReg(adr, w, BIT_8);
			}
			len = size;
		}
		else
		{
			len = V5_WriteSram(adr, (UINT8*)gTransferBuf, size);
		}
	}
	else
	{
		if(adr > V5_SRAM_BASE)
		{
			while(size--)
			{
				w = (UINT8)V5_GetReg(adr, BIT_8);
			}
			len = size;
		}
		else
		{
			len = V5_ReadSram(adr, (UINT8*)gTransferBuf, size);
		}
	}

	if(len != size)
	{
		return 2;
	}
	return 0;
}

void isrHandle(UINT8 type, UINT16 flg)
{
	static unsigned long count = 0;
	count++;
	tmDbgPrintf("==========ocurr counter = %d ++++++flg = %x +++++ %20s", count, flg, gIntisr[type].Desc);
}

void V5_SetNoPortBusType(UINT8 type)
{
	switch(type)
	{
	case V5_BUS_MULTI16:
		 /*Need via Mul8 if bus type changed from Sep to Mul16*/
		if(gBiuContext.busType == V5_BUS_SEPERATE)
		{
			gBiuContext.busType = V5_BUS_MULTI8;
            gBiuContext.pSetRegFunc = V5_Mul8SetRegByWType;
		}

		V5B_BiuSetMul_8_16(1); //chage to bus 16
		gBiuContext.busType = V5_BUS_MULTI16;
		gBiuContext.pSetRegFunc = V5_Mul16SetRegByWType;
		gBiuContext.pGetRegFunc = V5_Mul16GetRegByWType;
		gBiuContext.pReadSramFunc = V5_Mul16NoPortReadSram;
		gBiuContext.pWriteSramFunc = V5_Mul16NoPortWriteSram;
		break;

	case V5_BUS_MULTI8:
		V5B_BiuSetMul_8_16(0); //chage to bus 8
		gBiuContext.busType = V5_BUS_MULTI8;
		gBiuContext.pSetRegFunc = V5_Mul8SetRegByWType;
		gBiuContext.pGetRegFunc = V5_Mul8GetRegByWType;
		gBiuContext.pReadSramFunc = V5_Mul8NoPortReadSram;
		gBiuContext.pWriteSramFunc = V5_Mul8NoPortWriteSram;
		break;

	case V5_BUS_SEPERATE:
		V5B_BiuSetMul_8_16(0); //chage to bus 8
		gBiuContext.busType = V5_BUS_SEPERATE;
		gBiuContext.pSetRegFunc = V5_SepSetRegByWType;
		gBiuContext.pGetRegFunc = V5_SepGetRegByWType;
		gBiuContext.pReadSramFunc = V5_SepNoPortReadSram;
		gBiuContext.pWriteSramFunc = V5_SepNoPortWriteSram;
		break;
	
	default:
		break;

	}
}

#ifdef WIN32

void testIntInit(void)
{
	char str[0x100];

	sprintf(str, "testIntInit \n ");
	SendCmd(str);
}

void V5_PortSwitchTest(UINT8 port)
{
	char str[0x100];
	
	sprintf(str, "V5_PortSwitchTest %x \n", port);
	SendCmd(str);
}

void testRegConRead(UINT32 addr, UINT32 readCnt)
{
	char str[0x100];
	
	sprintf(str, "testRegConRead %x %x \n", addr, readCnt);
	SendCmd(str);
}

#else

void testIntInit(void)
{
	int i;

	for(i=0; i< gIntnum; i++)
	{
		V5_CtrlIntSetCallback(gIntisr[i].INTtype, gIntisr[i].pIsr);
	}

#ifndef NCSC
	EnableExInt(EXINT1);
#endif

}

void V5_PortSwitchTest(UINT8 port)
{
	if(port)
	{
		V5B_BiuPortEnable();
		V5B_SetBusType(gBiuContext.busType);
	}
	else
	{
		V5B_BiuPortDisable();
		V5_SetNoPortBusType(gBiuContext.busType);
	}
}

void testRegConRead(UINT32 addr, UINT32 readCnt)
{
	UINT32 count = readCnt;
    UINT8  outVal1, outVal2;

	outVal1 = (UINT8)V5_GetReg(addr, BIT_8);

    while(count--)
	{
		if( (outVal2 = (UINT8)V5_GetReg(addr, BIT_8)) != outVal1)
		{
			tmPrintf("addr: %x,   count: %x,   intial value: %x,   value: %x \n", addr, readCnt - count, outVal1, outVal2);
			break;
		}
	}
}

#endif

static UINT8 V5_DistBusType(UINT8 busType)
{
    UINT8 val;

	V5B_SetBusType(busType);

#ifndef WIN32// set arm bus type
	if(busType == V5_BUS_MULTI16)
		BusInit(BUS_IDX_4, BUS16);
	else
		BusInit(BUS_IDX_4, BUS8);
#endif

    V5_SetReg(V5_REG_BIU_CMD, 0xff, BIT_8);
    val = (UINT8)V5_GetReg(V5_REG_BIU_CMD, BIT_8);
    
	if(val == 0xff)
	{
		 return busType;
	}

	return 0x0;
}

UINT8 V5_SetBusTypeMulOrSep(void)
{
	UINT8 busType = 0x0;

	if(V5_DistBusType(V5_BUS_MULTI16) == V5_BUS_MULTI16)
	{
		busType = V5_BUS_MULTI16;
	}
	else if(V5_DistBusType(V5_BUS_SEPERATE) == V5_BUS_SEPERATE)
	{
		busType = V5_BUS_SEPERATE;
	}
	else if(V5_DistBusType(V5_BUS_MULTI8) == V5_BUS_MULTI8)
	{
		busType = V5_BUS_MULTI8;
	}

	return busType;
}

UINT32 V5_Mul16NoPortReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 1;

	actSize = size << 1;

	while(size--)
	{
		V5_Mul16ReadSram(adr, buf, 2);

		adr += 2;
		buf += 2;
	}
	
	return actSize;
}

UINT32 V5_Mul16NoPortWriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 1;
	
	actSize = size << 1;

	while(size--)
	{
		V5_Mul16WriteSram(adr, buf, 2);

		adr += 2;
		buf += 2;
	}

	return actSize;
}

UINT32 V5_Mul8NoPortReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	while(size--)
	{
		V5_Mul8ReadSram(adr++, buf++, 1);
	}

	return actSize;
}

UINT32 V5_Mul8NoPortWriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	while(size--)
	{
		V5_Mul8WriteSram(adr++, buf++, 1);
	}

	return actSize;
}

UINT32 V5_SepNoPortReadSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	adr += V5_SRAM_BASE;

	while(size--)
	{
		*buf++ = V5_SepGetReg(adr++);
	}

	return actSize;
}

UINT32 V5_SepNoPortWriteSram(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	adr += V5_SRAM_BASE;

	while(size--)
	{
		V5_SepSetReg(adr++, *buf++);
	}

	return actSize;
}

/******************************************************************************************/
/* the follow code written for sram write back and register and sram context restore test */
/******************************************************************************************/
void V5_CtrlStoreSramCont(UINT32 *pAddr, UINT8  *pDat, UINT8 *pBacknum, UINT8 *pRw);
void V5_CtrlRestoreSramCont(UINT32 addr, UINT8  *dat, UINT8 backnum, UINT8 rw);

static TAcceContext gAcceContTest = {
	       0,
		   0,
		   0,
		   0,
};

void V5_RestoreReg2SramContTest(void)
{
	switch(gAcceContTest.stat)
	{
  	    case SEP_REG_SET:
		case SEP_REG_GET:
			V5_SEPSETREGADDR(gAcceContTest.addr);
			break;

		case SEP_SRAM_WRITE:
			V5_SEPWRITESRAMSETCONT(gAcceContTest.addr);
			break;

		case SEP_SRAM_READ:
			V5_SEPREADSRAMSETCONT(gAcceContTest.addr);
			break;

		case MUL8_REG_SET:
		case MUL8_REG_GET:
			V5_MUL8SETREGADDR(gAcceContTest.addr);
			break;

		case MUL8_SRAM_WRITE:
			V5_MUL8WRITESRAMSETCONT(gAcceContTest.addr);
			break;
		
		case MUL8_SRAM_READ:
			V5_MUL8READSRAMSETCONT(gAcceContTest.addr);
			break;

		case MUL16_REG_SET:
		case MUL16_REG_GET:
		case MUL16_REG_SET_16:
		case MUL16_REG_GET_16:
			V5_MUL16SETREGADDR(gAcceContTest.addr);
			break;

		case MUL16_SRAM_WRITE:
			V5_MUL16WRITESRAMSETCONT(gAcceContTest.addr);
			break;

		case MUL16_SRAM_READ:
			V5_MUL16READSRAMSETCONT(gAcceContTest.addr);
			break;

		default:
			break;
	}
}

static void TestIntSram( )
{
	UINT32 addr;
	UINT8  rw, backnum;
	UINT8  dat[4];
	UINT8 datw[8] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};

	V5_CtrlStoreSramCont(&addr, dat, &backnum, &rw);

	V5_WriteSram(0x5c000, datw, 8);

	V5_CtrlRestoreSramCont(addr, dat, backnum, rw);

}

void V5_Mul16SetRegTest(UINT32 adr, UINT8 val)
{
	UINT16 uTemp;
    
	Push2DisIntEnFlag();

	gAcceContTest.addr = adr;
	gAcceContTest.stat = MUL16_REG_SET;
    
	V5_HOSTSETADDR16( V5_MULTI16_ADDR_ADDR, (UINT16)adr );
	V5_Mul16SetReg(0x1802, 0xff);
	V5_RestoreReg2SramContTest();

	uTemp = V5_HOSTGETVAL16( V5_MULTI16_ADDR_DATA );
	if((adr & 0x1) == 0)
	{
		uTemp = (uTemp&0xff00)|val;
	}
	else
	{
		uTemp = (uTemp&0xff)|(val<<8);
	}

	/*host send data*/
	V5_HOSTSETVAL16( V5_MULTI16_ADDR_DATA, uTemp );

	gAcceContTest.stat = 0;

	PopIntEnFlag();
}

UINT8 V5_Mul16GetRegTest(UINT32 adr)
{
	UINT8 regVal;

	Push2DisIntEnFlag();
    
	gAcceContTest.addr = adr;
	gAcceContTest.stat = MUL16_REG_GET;
    
	V5_HOSTSETADDR16( V5_MULTI16_ADDR_ADDR, (UINT16)adr );
	V5_Mul16GetReg(0x1802);
	V5_RestoreReg2SramContTest();
		
    /*host get data*/
	if((adr & 0x1) == 0)
	{
		regVal = (UINT8)V5_HOSTGETVAL16( V5_MULTI16_ADDR_DATA );
	}
	else
	{
        regVal = (UINT8)( V5_HOSTGETVAL16( V5_MULTI16_ADDR_DATA ) >> 8 );
	}

	gAcceContTest.stat = 0;

	PopIntEnFlag();

	return regVal;
}

void V5_Mul16SetReg16Test(UINT32 adr, UINT16 val)
{
	Push2DisIntEnFlag();

	gAcceContTest.addr = adr;
	gAcceContTest.stat = MUL16_REG_SET_16;

	V5_MUL16SETREGADDR(adr);
	V5_Mul16SetReg16(0x1802, 0xaa);
	V5_RestoreReg2SramContTest();

	/*host send data*/
	V5_HOSTSETVAL16( V5_MULTI16_ADDR_DATA, val );

    gAcceContTest.stat = 0;

	PopIntEnFlag();
}

UINT16 V5_Mul16GetReg16Test(UINT32 adr)
{
	UINT16 regVal;

	Push2DisIntEnFlag();
    
	gAcceContTest.addr = adr;
	gAcceContTest.stat = MUL16_REG_GET_16;

	V5_MUL16SETREGADDR(adr);
	V5_Mul16GetReg16(0x1802);
	V5_RestoreReg2SramContTest();
		
    /*host get data*/
	regVal = V5_HOSTGETVAL16( V5_MULTI16_ADDR_DATA );

	gAcceContTest.stat = 0;

	PopIntEnFlag();

	return regVal;
}

UINT32 V5_Mul16ReadSramTest(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;
	UINT16 uTemp;
	UINT32 temp = 0;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 1;
	
	actSize = size << 1;

	//set sram start address
	adr += V5_SRAM_BASE;

	gAcceContTest.addr = adr;
	gAcceContTest.stat = MUL16_SRAM_READ;
 
	V5_Mul16SetReg(V5_REG_BIU_SEL_WRITE_READ, 0x0);

	V5_Mul16SetReg(V5_REG_BIU_MEM_LOW_WORD_L, (UINT8)adr);
	V5_Mul16SetReg(V5_REG_BIU_MEM_LOW_WORD_H, (UINT8)(adr>>8));
	V5_Mul16SetReg(V5_REG_BIU_MEM_HIGH_WORD,  (UINT8)(adr>>16));
    
	V5_Mul16ReadSram(0x40000, (UINT8*)&temp, 0x4);
	V5_RestoreReg2SramContTest();

	//set multi16 memory port addr
	V5_HOSTSETADDR16(V5_MULTI16_ADDR_ADDR, (UINT16)V5_MULTI16_MEM_FLG);

	gAcceContTest.stat = 0;

	while(size--)
	{
        uTemp = V5_HOSTGETVAL16(V5_MULTI16_ADDR_DATA);
		*buf++ = (UINT8)uTemp;
		*buf++ = (UINT8)(uTemp>>8);

		temp += 2;

	    if((temp &0xfe) == 0xfe)
		{
			TestIntSram();
		}

	}
	
	return actSize;
}

UINT32 V5_Mul16WriteSramTest(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;
	UINT16 uTemp;
	UINT32 temp = 0;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	size >>= 1;
	
	actSize = size << 1;

	//set sram start address
	adr += V5_SRAM_BASE;

	gAcceContTest.addr = adr;
	gAcceContTest.stat = MUL16_SRAM_WRITE;

	V5_Mul16SetReg(V5_REG_BIU_SEL_WRITE_READ, 0x1);

	V5_Mul16SetReg(V5_REG_BIU_MEM_LOW_WORD_L, (UINT8)adr);
	V5_Mul16SetReg(V5_REG_BIU_MEM_LOW_WORD_H, (UINT8)(adr>>8));
	V5_Mul16SetReg(V5_REG_BIU_MEM_HIGH_WORD,  (UINT8)(adr>>16));

	V5_Mul16WriteSram(0x40000, (UINT8*)&temp, 0x4);
	V5_RestoreReg2SramContTest();

	//set multi16 memory port addr
	V5_HOSTSETADDR16(V5_MULTI16_ADDR_ADDR, (UINT16)V5_MULTI16_MEM_FLG);

	gAcceContTest.stat = 0;

	while(size--)
	{
		uTemp = *buf++;
		uTemp += ((UINT16)(*buf++))<<8;
		V5_HOSTSETVAL16(V5_MULTI16_ADDR_DATA, uTemp);

		temp += 2;
		if((temp &0xfe) == 0xfe)
		{
			TestIntSram();
		}
	}

	return actSize;
}

void V5_Mul8SetRegTest(UINT32 adr, UINT8 val)
{
	Push2DisIntEnFlag();

	gAcceContTest.addr = adr;
	gAcceContTest.stat = MUL8_REG_SET;

    V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_REG_WORDH );
 	V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, (UINT8)(adr>>8) );
	V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_REG_WORDL );
	V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, (UINT8)adr );

	V5_Mul8SetReg(0x1802, 0xaa);
	V5_RestoreReg2SramContTest();

 	V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_REG_PORT );
	V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, val);

	gAcceContTest.stat = 0;

	PopIntEnFlag();
}

UINT8 V5_Mul8GetRegTest(UINT32 adr)
{
	UINT8 regVal;

	Push2DisIntEnFlag();

	gAcceContTest.addr = adr;
	gAcceContTest.stat = MUL8_REG_GET;

    V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_REG_WORDH );
 	V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, (UINT8)(adr>>8) );
	V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_REG_WORDL );
	V5_HOSTSETVAL8( V5_MULTI8_ADDR_DATA, (UINT8)adr );

	V5_Mul8GetReg(0x1802);
	V5_RestoreReg2SramContTest();

 	V5_HOSTSETADDR8( V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_REG_PORT );
   
	regVal = V5_HOSTGETVAL8( V5_MULTI8_ADDR_DATA );

	gAcceContTest.stat = 0;

	PopIntEnFlag();

	return regVal;
}

UINT32 V5_Mul8ReadSramTest(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;
	UINT32 temp = 0;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	//set sram start address
	adr += V5_SRAM_BASE;

	gAcceContTest.addr = adr;
	gAcceContTest.stat = MUL8_SRAM_READ;

	V5_Mul8SetReg(V5_REG_BIU_SEL_WRITE_READ, 0x0);

	V5_Mul8SetReg(V5_REG_BIU_MEM_LOW_WORD_L, (UINT8)adr);
	V5_Mul8SetReg(V5_REG_BIU_MEM_LOW_WORD_H, (UINT8)(adr>>8));
	V5_Mul8SetReg(V5_REG_BIU_MEM_HIGH_WORD,  (UINT8)(adr>>16));

	V5_Mul8ReadSram(0x40000, (UINT8*)&temp, 0x4);
	V5_RestoreReg2SramContTest();

	//set multi8 memory port addr
	V5_HOSTSETVAL8(V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_MEM_PORT);

	gAcceContTest.stat = 0;

	while(size--)
	{
		*buf++ = V5_HOSTGETVAL8(V5_MULTI8_ADDR_DATA);
		temp ++;
		if((temp &0xff) == 0xff)
		{
			TestIntSram();
		}
	}

	return actSize;
}

UINT32 V5_Mul8WriteSramTest(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;
	UINT32 temp = 0;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	//set sram start address
	adr += V5_SRAM_BASE;

	gAcceContTest.addr = adr;
	gAcceContTest.stat = MUL8_SRAM_WRITE;

	V5_Mul8SetReg(V5_REG_BIU_SEL_WRITE_READ, 0x1);

	V5_Mul8SetReg(V5_REG_BIU_MEM_LOW_WORD_L, (UINT8)adr);
	V5_Mul8SetReg(V5_REG_BIU_MEM_LOW_WORD_H, (UINT8)(adr>>8));
	V5_Mul8SetReg(V5_REG_BIU_MEM_HIGH_WORD,  (UINT8)(adr>>16));

	V5_Mul8WriteSram(0x40000, (UINT8*)&temp, 0x4);
	V5_RestoreReg2SramContTest();

	//set multi8 memory port addr
	V5_HOSTSETADDR8(V5_MULTI8_ADDR_ADDR, (UINT8)V5_MULTI8_MEM_PORT);

	gAcceContTest.stat = 0;

	while(size--)
	{
		V5_HOSTSETVAL8(V5_MULTI8_ADDR_DATA, *buf++);

		temp ++;
		if((temp &0xff) == 0xff)
		{
			TestIntSram();
		}
	}

	return actSize;
}

void V5_SepSetRegTest(UINT32 adr, UINT8 val)
{
	Push2DisIntEnFlag();

	gAcceContTest.addr = adr;
	gAcceContTest.stat = SEP_REG_SET;

	V5_HOSTSETADDR8( V5_HOST_ADDR_REGUIAL, (UINT8)(adr>>8) );
	V5_HOSTSETADDR8( V5_HOST_ADDR_REGUIAH, (UINT8)(adr>>16) );

	V5_SepSetReg(0x1802, 0xaa);
	V5_RestoreReg2SramContTest();

	V5_HOSTSETVAL8( ( V5_INTER_ADDR_REGUIA + (UINT32)(adr&0xff) ), val );

	gAcceContTest.stat = 0;

	PopIntEnFlag();
}

UINT8 V5_SepGetRegTest(UINT32 adr)
{
	UINT8 regVal;

	Push2DisIntEnFlag();

	gAcceContTest.addr = adr;
	gAcceContTest.stat = SEP_REG_GET;

    V5_HOSTSETADDR8( V5_HOST_ADDR_REGUIAL, (UINT8)(adr>>8) );
	V5_HOSTSETADDR8( V5_HOST_ADDR_REGUIAH, (UINT8)(adr>>16) );

	V5_SepGetReg(0x1802);
	V5_RestoreReg2SramContTest();

	regVal = V5_HOSTGETVAL8( ( V5_INTER_ADDR_REGUIA + (UINT32)(adr&0xff) ) );

	gAcceContTest.stat = 0;

	PopIntEnFlag();

	return regVal;
}

UINT32 V5_SepReadSramTest(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;
	UINT32 temp = 0;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	//set sram start address
	adr += V5_SRAM_BASE;

	gAcceContTest.addr = adr;
	gAcceContTest.stat = SEP_SRAM_READ;

	V5_SepSetReg(V5_REG_BIU_SEL_WRITE_READ, 0x0);

	V5_HOSTSETADDR8(V5_HOST_ADDR_MEMUIA, (UINT8)(adr>>16));
	V5_HOSTSETADDR8(V5_HOST_ADDR_MEMLIA, (UINT8)(adr>>8));
	V5_HOSTSETADDR8(V5_HOST_ADDR_MEMLIA0, (UINT8)(adr));

	V5_SepReadSram(0x40000, (UINT8*)&temp, 0x4);
	V5_RestoreReg2SramContTest();

	gAcceContTest.stat = 0;

	while(size--)
	{
		*buf++ = V5_HOSTGETVAL8(V5_HOST_ADDR_MEMDATA);
		temp ++;
		if((temp &0xff) == 0xff)
		{
			TestIntSram();
		}
	}
	
	return actSize;
}

UINT32 V5_SepWriteSramTest(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 actSize;
	UINT32 temp = 0;

	if(adr > V5_SRAM_TOTALSIZE)
	{
		return 0;
	}
	if(size > (V5_SRAM_TOTALSIZE - adr))
	{
		size = V5_SRAM_TOTALSIZE - adr;
	}

	actSize = size;

	//set sram start address
	adr += V5_SRAM_BASE;

	gAcceContTest.addr = adr;
	gAcceContTest.stat = SEP_SRAM_WRITE;

	V5_SepSetReg(V5_REG_BIU_SEL_WRITE_READ, 0x1);

	V5_HOSTSETADDR8(V5_HOST_ADDR_MEMUIA, (UINT8)(adr>>16));
	V5_HOSTSETADDR8(V5_HOST_ADDR_MEMLIA, (UINT8)(adr>>8));
	V5_HOSTSETADDR8(V5_HOST_ADDR_MEMLIA0, (UINT8)(adr));

	V5_SepReadSram(0x40000, (UINT8*)&temp, 0x4);
	V5_RestoreReg2SramContTest();

	gAcceContTest.stat = 0;

	while(size--)
	{
		V5_HOSTSETVAL8(V5_HOST_ADDR_MEMDATA, *buf++);
		temp ++;
		if((temp &0xff) == 0xff)
		{
			TestIntSram();
		}
	}
	
	return actSize;
}

void V5_SetRegTest(UINT32 adr, UINT8 val)
{
	UINT32 testVal = 0xaabbccdd;

	switch(gBiuContext.busType)
	{
	    case V5_BUS_SEPERATE:
            V5_SepSetRegTest(adr, val);
			/*test by order type register operation*/
		    V5_SepSetRegByWType(adr, testVal, 0);
			V5_SepSetRegByWType(adr, testVal, 1);
			V5_SepSetRegByWType(adr, testVal, 2);
			V5_SepSetRegByWType(adr, testVal, 3);
			V5_SepSetRegByWType(adr, testVal, 4);
			V5_SepSetRegByWType(adr, testVal, 5);
			V5_SepSetRegByWType(adr, testVal, 6);
			break;

		case V5_BUS_MULTI8:
            V5_Mul8SetRegTest(adr, val);
			/*test by order type register operation*/
		    V5_Mul8SetRegByWType(adr, testVal, 0);
			V5_Mul8SetRegByWType(adr, testVal, 1);
			V5_Mul8SetRegByWType(adr, testVal, 2);
			V5_Mul8SetRegByWType(adr, testVal, 3);
			V5_Mul8SetRegByWType(adr, testVal, 4);
			V5_Mul8SetRegByWType(adr, testVal, 5);
			V5_Mul8SetRegByWType(adr, testVal, 6);
			break;

		case V5_BUS_MULTI16:
			V5_Mul16SetRegTest(adr, val);
			/*test by order type register operation*/
			V5_Mul16SetRegByWType(adr, testVal, 0);
			V5_Mul16SetRegByWType(adr, testVal, 1);
			V5_Mul16SetRegByWType(adr, testVal, 2);
			V5_Mul16SetRegByWType(adr, testVal, 3);
			V5_Mul16SetRegByWType(adr, testVal, 4);
			V5_Mul16SetRegByWType(adr, testVal, 5);
			V5_Mul16SetRegByWType(adr, testVal, 6);
			break;

		default:
			break;
	}
}

UINT8 V5_GetRegTest(UINT32 adr)
{
	UINT8 val;
	UINT32 testVal;

	switch(gBiuContext.busType)
	{
	    case V5_BUS_SEPERATE:
            val = V5_SepGetRegTest(adr);
			/*test by order type register operation*/
		    testVal = V5_SepGetRegByWType(adr, 0);
			testVal = V5_SepGetRegByWType(adr, 1);
			testVal = V5_SepGetRegByWType(adr, 2);
			testVal = V5_SepGetRegByWType(adr, 3);
			testVal = V5_SepGetRegByWType(adr, 4);
			testVal = V5_SepGetRegByWType(adr, 5);
			testVal = V5_SepGetRegByWType(adr, 6);
			break;

		case V5_BUS_MULTI8:
            val = V5_Mul8GetRegTest(adr);
			/*test by order type register operation*/
		    testVal = V5_Mul8GetRegByWType(adr, 0);
			testVal = V5_Mul8GetRegByWType(adr, 1);
			testVal = V5_Mul8GetRegByWType(adr, 2);
			testVal = V5_Mul8GetRegByWType(adr, 3);
			testVal = V5_Mul8GetRegByWType(adr, 4);
			testVal = V5_Mul8GetRegByWType(adr, 5);
			testVal = V5_Mul8GetRegByWType(adr, 6);
			break;

		case V5_BUS_MULTI16:
			val = V5_Mul16GetRegTest(adr);
			/*test by order type register operation*/
			testVal = V5_Mul16GetRegByWType(adr, 0);
			testVal = V5_Mul16GetRegByWType(adr, 1);
			testVal = V5_Mul16GetRegByWType(adr, 2);
			testVal = V5_Mul16GetRegByWType(adr, 3);
			testVal = V5_Mul16GetRegByWType(adr, 4);
			testVal = V5_Mul16GetRegByWType(adr, 5);
			testVal = V5_Mul16GetRegByWType(adr, 6);
			break;

		default:
			val = 0;
			break;
	}

	return val;
}

UINT32 V5_WriteSramTest(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 val;

	switch(gBiuContext.busType)
	{
	    case V5_BUS_SEPERATE:
            val = V5_SepWriteSramTest(adr, buf, size);
			break;

		case V5_BUS_MULTI8:
            val = V5_Mul8WriteSramTest(adr, buf, size);
			break;

		case V5_BUS_MULTI16:
			val = V5_Mul16WriteSramTest(adr, buf, size);
			break;

		default:
			val = 0;
			break;
	}

    return val;
}

UINT32 V5_ReadSramTest(UINT32 adr, UINT8 *buf, UINT32 size)
{
	UINT32 val;

	switch(gBiuContext.busType)
	{
	    case V5_BUS_SEPERATE:
            val = V5_SepReadSramTest(adr, buf, size);
			break;

		case V5_BUS_MULTI8:
            val = V5_Mul8ReadSramTest(adr, buf, size);
			break;

		case V5_BUS_MULTI16:
			val = V5_Mul16ReadSramTest(adr, buf, size);
			break;

		default:
			val = 0;
			break;
	}

	return val;
}

#endif //V578CMD_DISABLE
