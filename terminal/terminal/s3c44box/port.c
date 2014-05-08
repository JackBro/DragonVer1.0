
#include "internal.h"


typedef struct tag_TPortContext {
	TPortCall pcall[5]; 
} TPortContext, *PTPortContext;

static TPortContext gPortContext;

static const int gExIntBitMask[] = {BIT_EINT0, BIT_EINT1, BIT_EINT2, BIT_EINT3, BIT_EINT4567};
static const int gExIntMask[] = {0x0f, 0xf0, 0x0f00, 0xf000, 0xffff0000};

void beep(int BeepStatus)
{
	if(BeepStatus)
		SetMemory(REG_PDATE, 0x1f7, AND_OPCODE);
	else
		SetMemory(REG_PDATE, 0x8, OR_OPCODE);
}

void Led_Display(int LedStatus)
{
	int val;

	LedStatus &= 0xf;
	val = ReadMemory(REG_PDATE);
	val &= ~(0xf0);
	val |= LedStatus << 4;
	WriteMemory(REG_PDATE, val);
}

void EnableExInt(int idx)
{
	SetMemory(REG_INTMSK, ~(gExIntBitMask[idx] | BIT_GLOBAL), AND_OPCODE);
}

void DisableExInt(int idx)
{
	SetMemory(REG_INTMSK, gExIntBitMask[idx], OR_OPCODE);
}

#ifndef WIN32
static __irq void _isrEINT1(void)
{
	WriteMemory(REG_I_ISPC, BIT_EINT1);
	if(gPortContext.pcall[1] != NULL)
		gPortContext.pcall[1]();
}

static __irq void _isrEINT0(void)
{
	WriteMemory(REG_I_ISPC, BIT_EINT0);
	if(gPortContext.pcall[0] != NULL)
		gPortContext.pcall[0]();
}

static __irq void _isrEINT2(void)
{
	WriteMemory(REG_I_ISPC, BIT_EINT2);
	if(gPortContext.pcall[2] != NULL)
		gPortContext.pcall[2]();
}

static __irq void _isrEINT3(void)
{
	WriteMemory(REG_I_ISPC, BIT_EINT3);
	if(gPortContext.pcall[3] != NULL)
		gPortContext.pcall[3]();
}

static __irq void _isrEINT4567(void)
{
	WriteMemory(REG_I_ISPC, BIT_EINT4567);
	if(gPortContext.pcall[4] != NULL)
		gPortContext.pcall[4]();
}
#endif

void ExtInt_Init(int idx)
{
	const int pcong_msk[] = {0x03, 0x0c, 0x30, 0xf0, 0xff00};
	const int pupg_msk[]  = {0xfe, 0xfd, 0xfb, 0xf7, 0x0f};

	DisableExInt(idx);
	SetMemory(REG_PCONG, pcong_msk[idx], OR_OPCODE);
	SetMemory(REG_PUPG,  pupg_msk[idx],  AND_OPCODE);
}

void SetExtIntTrgType(int idx, int type)
{
	const int val[] = {0x00, 0x01, 0x02, 0x04, 0x06};
	int reg_val = 0, tmp_val = val[type];

	reg_val = ReadMemory(REG_EXTINT);
	reg_val &= ~gExIntMask[idx];

	tmp_val = tmp_val << (idx << 2);
	reg_val |= tmp_val;

	if(idx == EXINT4567)
		reg_val = reg_val | (tmp_val << 4) | (tmp_val << 8) | (tmp_val << 12);

	WriteMemory(REG_EXTINT, reg_val);
}

int GetExtIntTrgType(int idx)
{
	int val = 0, type;

	val = ReadMemory(REG_EXTINT);
	type = (val >> (idx << 2)) & 0x07;

	if(type > HIGHLEVEL)
		type = (type >> 1) + 1;
	
	return type;
}

void SetPortCall(int index, TPortCall pcall)
{
	if(index > 4)
		index = 4;
	gPortContext.pcall[index] = pcall;
}

void Port_Init(void)
{
	const int setting[][2] =
	{
		{ REG_PCONA, 0x1ff },
		{ REG_PDATB, 0x3ff },
		{ REG_PCONB, 0x7ff },		//l_yong modify it for LCD main screen selection.
		
		{ REG_PDATC, 0xffff },		//All IO is high
		{ REG_PCONC, 0x0f000054 },
		{ REG_PUPC, 0x3000 },		//PULL UP RESISTOR should be enabled to I/O

		{ REG_PDATD, 0xff },
		{ REG_PCOND, 0x11 },		//GPD0, GPD2 is output
		{ REG_PUPD, 0 },

		{ REG_PDATE, 0x1ff },
		{ REG_PCONE, 0x25569 },		//************************* PE7(CODECLK) is used for 558 clock, PE0 as output
		{ REG_PUPE, 6 },

		{ REG_PDATF, 0 },
		{ REG_PCONF, 0x24900a },	//i2c
		{ REG_PUPF, 0x1e3 },

		{ REG_PCONG, 0x540c },		//V558 chip reset, Port G pg7 -- output,
		{ REG_PDATG, 0xff },
		{ REG_PUPG, 0 },			//should be enabled  

		{ REG_SPUCR, 7 },			//D15-D0 pull-up disable
		{ REG_EXTINT, 0 }			//All EINT[7:0] will be low level.
	};
	int i, len;

	len = (sizeof(setting) / sizeof(int)) >> 1;
	for(i = 0; i < len; i++)
		WriteMemory(setting[i][0], setting[i][1]);
	memset(&gPortContext, 0, sizeof(TPortContext));

#ifndef WIN32
	pISR_EINT0 = (unsigned)_isrEINT0;
	pISR_EINT1 = (unsigned)_isrEINT1;
	pISR_EINT2 = (unsigned)_isrEINT2;
	pISR_EINT3 = (unsigned)_isrEINT3;
	pISR_EINT4567 = (unsigned)_isrEINT4567;
#endif
}
