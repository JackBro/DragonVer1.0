
#include "internal.h"


#define TIMER01_PRE		(0xf0-1)
#define TIMER01_DIV		0x10
#define TIMER23_PRE		(0xf0-1)
#define TIMER23_DIV		0x10
#define TIMER45_PRE		(0xf0-1)
#define TIMER45_DIV		0x10


typedef struct tag_TTimerContext
{
	TTimerCall tcall[6];
	int	dtime[6];			//unit: 0.1ms
	int maxtime[3];
	unsigned wdt_cnt;
} TTimerContext, *PTTimerContext;

static TTimerContext gTimerContext;

const int gTmrIntBitMask[] = {BIT_TIMER0, BIT_TIMER1, BIT_TIMER2, BIT_TIMER3, BIT_TIMER4, BIT_TIMER5};
const int gTmrPreFactor[]  = {TIMER01_PRE, TIMER23_PRE, TIMER45_PRE};
const int gTmrDivider[]    = {TIMER01_DIV, TIMER23_DIV, TIMER45_DIV};

#ifndef WIN32
static void __irq Timer0Done(void)
{
    //rI_ISPC = BIT_TIMER0;
	WriteMemory(REG_I_ISPC, BIT_TIMER0);
//	tmPrintf("timer0\n");
}

static void __irq Timer1Done(void)
{
	WriteMemory(REG_I_ISPC, BIT_TIMER1);
//	tmPrintf("timer1\n");
}

static void __irq Timer2Done(void)
{
	WriteMemory(REG_I_ISPC, BIT_TIMER2);
//	tmPrintf("timer2\n");
}

static void __irq Timer3Done(void)
{
	WriteMemory(REG_I_ISPC, BIT_TIMER3);
//	tmPrintf("timer3\n");
}

static void __irq Timer4Done(void)
{
	WriteMemory(REG_I_ISPC, BIT_TIMER4);
//	tmPrintf("timer4\n");
	if(gTimerContext.tcall[4])				// for key scan
		gTimerContext.tcall[4]();
}

static void __irq Timer5Done(void)
{
	WriteMemory(REG_I_ISPC, BIT_TIMER5);
//	tmPrintf("timer5\n");
	if(gTimerContext.tcall[5])
		gTimerContext.tcall[5]();
}
#endif

void SetTimerCall(int index, TTimerCall pcall)
{
	if(index > 5)
		index = 5;
	gTimerContext.tcall[index] = pcall;
}

static int GetTimerMaxIntval(int idx)
{
	int mclk;

	idx = idx >> 1;

	mclk = GetHostMclk();
	mclk *= 1000;
	mclk /= gTmrPreFactor[idx] + 1;
	mclk >>= 4;

	return (0xffff / mclk) * 10000;
}

void EnableTimerInt(int idx)
{
	SetMemory(REG_INTMSK, ~(gTmrIntBitMask[idx] | BIT_GLOBAL), AND_OPCODE);
}

void DisableTimerInt(int idx)
{
	SetMemory(REG_INTMSK, gTmrIntBitMask[idx], OR_OPCODE);
}

void SetHostTimer(int idx, int dtime)
{
	int mclk, val, idx1;
	const int reglist[] = {REG_TCNTB0, REG_TCNTB1, REG_TCNTB2, REG_TCNTB3, REG_TCNTB4, REG_TCNTB5};
	const int mask[]    = {0x0f, 0x0f00, 0xf000, 0xf0000, 0xf00000, 0x7000000};
	const int up_val[]  = {0x02, 0x0200, 0x2000, 0x20000, 0x200000, 0x2000000};
	const int st_val[]  = {0x09, 0x0900, 0x9000, 0x90000, 0x900000, 0x5000000};

	idx1 = idx >> 1;

	if(dtime == 0)
	{
		DisableTimerInt(idx);
		return;
	}
#ifdef WIN32
	if(gTimerContext.maxtime[idx1] == 0)
		gTimerContext.maxtime[idx1] = GetTimerMaxIntval(idx);
#endif
	if(dtime > gTimerContext.maxtime[idx1])
		dtime = gTimerContext.maxtime[idx1];
	if(dtime == gTimerContext.dtime[idx])
	{
		EnableTimerInt(idx);
		return;
	}
	gTimerContext.dtime[idx] = dtime;
	val = ReadMemory(REG_TCON);
	WriteMemory(REG_TCON, 0);					//stop all timer

	mclk = GetHostMclk();
	dtime *= mclk / 10;
	dtime /= gTmrPreFactor[idx1] + 1;
	dtime /= gTmrDivider[idx1];
	WriteMemory(reglist[idx], dtime);

	val &= ~mask[idx];
	WriteMemory(REG_TCON, val | up_val[idx]);	//update timer
	WriteMemory(REG_TCON, val | st_val[idx]);	//start timer
	EnableTimerInt(idx);
}

int GetHostTimer(int idx)
{
	return gTimerContext.dtime[idx];
}

void TimerInit(void)
{
	int mclk;

	mclk = GetHostMclk();
#ifndef WIN32
    pISR_TIMER0	= (unsigned)Timer0Done;
    pISR_TIMER1	= (unsigned)Timer1Done;
    pISR_TIMER2	= (unsigned)Timer2Done;
    pISR_TIMER3	= (unsigned)Timer3Done;
	pISR_TIMER4	= (unsigned)Timer4Done;
	pISR_TIMER5	= (unsigned)Timer5Done;
#endif
	memset(&gTimerContext, 0, sizeof(TTimerContext));

	WriteMemory(REG_TCFG0, 0xf0f0f0);
	WriteMemory(REG_TCFG1, 0x333333);
	WriteMemory(REG_TCON, 0x0);
	WriteMemory(REG_TCMPB0, 0x0);
	WriteMemory(REG_TCMPB1, 0x0);
	WriteMemory(REG_TCMPB2, 0x0);
	WriteMemory(REG_TCMPB3, 0x0);
	WriteMemory(REG_TCMPB4, 0x0);
	gTimerContext.maxtime[0] = GetTimerMaxIntval(TIMER0);
	gTimerContext.maxtime[1] = GetTimerMaxIntval(TIMER2);
	gTimerContext.maxtime[2] = GetTimerMaxIntval(TIMER4);

	WDTInit();
}


//////////////////////////////////////////////////////////////////////
/////                       Watch Dog                           //////
//////////////////////////////////////////////////////////////////////

#ifndef WIN32

/* For test OS*/
void  OSTimeTick (void);
/* Test end */

static void __irq WDTDone(void)
{
	WriteMemory(REG_I_ISPC, BIT_WDT);
	gTimerContext.wdt_cnt ++;

	/* For test OS*/
    //OSTimeTick();
	/* Test end */

	//if(gTimerContext.wdt_cnt % 10000 == 0)
	//	tmPrintf("%d\n", gTimerContext.wdt_cnt / 10000);
}
#endif

void WDTInit(void)
{
	int mclk;

	gTimerContext.wdt_cnt = 0;
	mclk = GetHostMclk();
#ifndef WIN32
    pISR_WDT = (unsigned)WDTDone;
#endif

	WriteMemory(REG_WTCON, ((mclk/4000 - 1) << 8) | (0x01 << 2));	//	1/(4000KHz * 1/16) = 0.004ms
	WriteMemory(REG_WTDAT, 0x19);									//	25 * 0.004 = 0.1 ms
	WriteMemory(REG_WTCNT, 0x19);

	EnableWdtInt();
	WdtStart();
}

void EnableWdtInt(void)
{
	SetMemory(REG_INTMSK, ~(BIT_WDT | BIT_GLOBAL), AND_OPCODE);
}

void DisableWdtInt(void)
{
	SetMemory(REG_INTMSK, BIT_WDT, OR_OPCODE);
}

void WdtStart(void)
{
	SetMemory(REG_WTCON, 0x20, OR_OPCODE);
}

#ifdef WIN32
unsigned WdtGetCount(void)
{
	return (unsigned)SimHost_GetWdtCnt();
}

#else
unsigned WdtGetCount(void)
{
	return gTimerContext.wdt_cnt;	// unit : 0.1 ms
}
#endif

void WdtStop(void)
{
	SetMemory(REG_WTCON, ~0x20, AND_OPCODE);
}

void Delay(int time)	//unit : ms
{
	unsigned cnt;

	cnt = WdtGetCount();
	cnt = cnt + 10 * time;
	while(cnt > WdtGetCount());
}
