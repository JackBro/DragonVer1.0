
#include "../../s3c44box/s3c44box.h"
#include "../driver/driver938.h"
#include "v9initenv.h"

#ifndef	V938CMD_DISABLE

static void ResetV9Chip()
{
	int x;

	x = ReadMemory(REG_PCOND);
	x &= 0xffcc;
	x |= 0x11;
	WriteMemory(REG_PCOND, x);		//GPD0, 2 set to output

	SetMemory(REG_PDATD, 0xfe, AND_OPCODE);
#ifndef NCSC
	Delay(100);
#endif
	SetMemory(REG_PDATD, 0x1, OR_OPCODE);
#ifndef NCSC
	Delay(10);
#endif
}

void V9_InitEnv(void)
{
	/************ Set port E, PE0 as Clock out ***********/
	WriteMemory(REG_PCONE, 0x1556a);
    WriteMemory(REG_PDATE, 0x1ff);
	WriteMemory(REG_PUPE,  0x6);

	/************ Set port G, GP3 as EINT3 ***********/
	WriteMemory(REG_PCONG, 0x54c0);
    WriteMemory(REG_PDATG, 0xff);
	WriteMemory(REG_PUPG,  0x0);

	//BusInit(BUS_IDX_4, BUS8);	//V938 use bus4
	BusInit(BUS_IDX_3, BUS8);	//V938 use bus3
	ResetV9Chip();
	Lm4867ShutDown(0);

	SetPortCall(3, _ISR_V9IntHandle);
	ExtInt_Init(EXINT3);	//v938 use exint3

	/******************* start 938 clock *****************/
	V9_CtrlSetPll(V938_DEFAULTMCLK);
    V9_CtrlStartClk(CTRL_PLL_WORK_MODE);
}

void Lm4867ShutDown(int sel)
{
	if(sel)
		SetMemory(REG_PDATD, 0x4, OR_OPCODE);
	else
		SetMemory(REG_PDATD, 0xfb, AND_OPCODE);
}

#endif //V938CMD_DISABLE

