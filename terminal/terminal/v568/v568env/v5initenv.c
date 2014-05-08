
#include "../driver/basefunc/basefunc.h"
#include "../v568testdemo/testctrl.h"
#include "../driver/v5api.h"
#include "../../s3c44box/s3c44box.h"
#include "../userapi/userapi.h"
#include "v5initenv.h"
#include "../v568testdemo/testjpeg.h"
#ifndef	V578CMD_DISABLE

static void ResetV5Chip()
{
	int x;

	x = ReadMemory(REG_PCONG);
	x &= 0x3fff;
	x |= 0x4000;
	WriteMemory(REG_PCONG, x);		//GPG7 set to output

	SetMemory(REG_PDATG, 0x7f, AND_OPCODE);
#ifndef NCSC
	Delay(100);
#endif
	SetMemory(REG_PDATG, 0x80, OR_OPCODE);
#ifndef NCSC
	Delay(10);
#endif
}

static void Bus_Init(int bus)
{
	if(bus == V5_BUS_MULTI16)
		BusInit(BUS_IDX_4, BUS16);
	else
		BusInit(BUS_IDX_4, BUS8);
}

void V5_InitEnv(void)
{
	Bus_Init(V5_BUS_TYPE);
	V5_InitBusType(V5_BUS_TYPE);
	ResetV5Chip();

	/*Adjust bus type to mul or sep*/
	//V5_SetBusTypeMulOrSep();

	////Add this ,reg can be r/w before V5_Open has been called. for vmonitor enviornment.
	V5_CtrlSetChipClk(I2S_DEFAULT_CLK, gCtrlInfo.mclk);
	V5_MarbInitMemClk();
	//V5B_CpmGetChipID();
	V5B_CpmSetIntCtl(gCtrlInfo.intprop);

	//SetPortCall(1, _ISR_V5IntHandle);
	//ExtInt_Init(EXINT1);
}

void testRegRw(UINT32 adr, UINT32 count)
{
	UINT32 i;
	char str[100];
	UINT8 val, val1;

	for(i = 0; i < count; i++)
	{
		val = (UINT8)i;

		V5_SetReg(adr, val, BIT_8);
		val1 = (UINT8)V5_GetReg(adr, BIT_8);

		if(val1 != val)
		{
			sprintf(str, "data mismatch at 0x%lx\n", i);
			sprintf(str, "%sval = 0x%x, val1 = 0x%x\n", str, val, val1);
			tmPrintf(str);
			break;
		}
	}
	if(i == count)
		tmPrintf("Read write function is ok!\n");
}

#ifdef WIN32
UINT32 RepeatWriteReg(UINT32 adr, UINT32 times)
{
	char str[0x100];

	sprintf(str, "RepeatWriteReg %x %x\n", adr, times);
	adr = SendDelayRetCmd(str);
	return adr;
}

void SetGbufMode(UINT8 mode)
{
	char str[100];

	sprintf(str, "SetGbufMode %x\n", mode);
	SendCmd(str);
}

void SetGbufAdrInfo(UINT32 adr, UINT32 jadr, UINT32 tadr, UINT32 iadr)
{
	char str[100];

	sprintf(str, "SetGbufAdrInfo %x %x %x %x\n", adr, jadr, tadr, iadr);
	SendCmd(str);
}

void ClearGbufPoint(void)
{
	char str[100] = "ClearGbufPoint\n";

	SendCmd(str);
}

UINT32 Sram2Gbuf(UINT32 adr, UINT32 len)
{
	char str[0x100];

	sprintf(str, "Sram2Gbuf %x %x\n", adr, len);
	len = SendRetCmd(str);
	return len;
}

#else
UINT32 RepeatWriteReg(UINT32 adr, UINT32 times)
{
	UINT32 i;

	for(i = 0; i < times; i++)
		V5_SetReg(adr, gTransferBuf[i], BIT_8);
	return times;
}

static TGbufInfo gBufInfo;

void SetGbufMode(UINT8 mode)
{
	gBufInfo.mode = mode;
}

void SetGbufAdrInfo(UINT32 adr, UINT32 jadr, UINT32 tadr, UINT32 iadr)
{
	gBufInfo.adr = adr;
	gBufInfo.jbufadr = jadr;
	gBufInfo.tbufadr = tadr;
	gBufInfo.ibufadr = iadr;
}

void ClearGbufPoint(void)
{
	memset(&gBufInfo, 0, sizeof(TGbufInfo));
}

UINT32 Sram2Gbuf(UINT32 adr, UINT32 len)
{
	switch(gBufInfo.mode)
	{
	case JBUF_MODE:
		len = V5_ReadSram(adr, (UINT8 *)gTransferBuf + gBufInfo.jbufadr, len);
		gBufInfo.jbufadr += len;
		break;
	case TBUF_MODE:
		len = V5_ReadSram(adr, (UINT8 *)gTransferBuf + gBufInfo.tbufadr, len);
		gBufInfo.tbufadr += len;
		break;
	case IBUF_MODE:
		len = V5_ReadSram(adr, (UINT8 *)gTransferBuf + gBufInfo.ibufadr, len);
		gBufInfo.ibufadr += len;
		break;
	case NORMAL_MODE:
	default:
		len = V5_ReadSram(adr, (UINT8*)gTransferBuf + gBufInfo.adr, len);
		gBufInfo.adr += len;
		break;
	}
	return len;
}


#endif


#ifdef WIN32

UINT32 RtReqJbuf(UINT32 jbufst, UINT32 jbuflen)
{
	char str[0x100];

	sprintf(str, "RtReqJbuf %x %x\n", jbufst, jbuflen);
	jbufst = SendDelayRetCmd(str);
	return jbufst;
}

#else

UINT32 RtReqJbuf(UINT32 jbufst, UINT32 jbuflen)
{
#ifdef HJPEG_RT_CAP
	return RtReqCapJbuf(jbufst, jbuflen);
#else
	return 0;
#endif
}

#endif

#endif //V578CMD_DISABLE
