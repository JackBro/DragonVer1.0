
#include "internal.h"

#define MAX_FVCO	170000 //(Fpllo*2^s) <= 170MHZ

#define MAX_S 3
#define MIN_S 0

#define MAX_M 263
#define MIN_M 8

#define MAX_P 49
#define MIN_P 2

typedef struct _CLK_PARA
{
	unsigned short m;
	unsigned short p;
	unsigned short s;
} CLK_PARA, *PCLK_PARA;

typedef struct _HOST_CLK
{
	CLK_PARA clkpara;
	unsigned long mclk;
} HOSTCLK, *PHOSTCLK;

typedef struct tag_THostContext
{
	int mclk;
	int i2sclk;
} THostContext, *PTHostContext;

static THostContext gHostContext;

static void CalMaxMinClkPara(unsigned long pllin, unsigned long mclk, PCLK_PARA max_mps, PCLK_PARA min_mps);
static void CalHostMclk(unsigned long pllin, unsigned long mclk, PHOSTCLK pHostClk);


#ifndef WIN32
static void HaltUndef(void)
{
	int flag = 1;

    tmPrintf("Undefined instruction exception!!!\n");
    while(flag);
}

static void HaltSwi(void)
{
	int flag = 1;

	tmPrintf("SWI exception!!!\n");
	while(flag);
}

static void HaltPabort(void)
{
	int flag = 1;

	tmPrintf("Pabort exception!!!\n");
	while(flag);
}

static void HaltDabort(void)
{
	int flag = 1;

	tmPrintf("Dabort exception!!!\n");
	while(flag);
}
#endif

static void Isr_Init(void)
{
	const int setting[][2] =
	{
		{ REG_INTCON, 5 },			//Non-vectored,IRQ enable,FIQ disable //
		{ REG_INTMOD, 0 },			//All=IRQ mode//
		{ REG_I_ISPC, 0x3ffffff },
		{ REG_INTMSK, ~BIT_GLOBAL }	//Default value=0x7ffffff//
	};
	int i, len;

#ifndef WIN32
    pISR_UNDEF	= (unsigned)HaltUndef;
    pISR_SWI	= (unsigned)HaltSwi;
    pISR_PABORT	= (unsigned)HaltPabort;
    pISR_DABORT	= (unsigned)HaltDabort;
#endif
	len = (sizeof(setting) / sizeof(int)) >> 1;
	for(i = 0; i < len; i++)
		WriteMemory(setting[i][0], setting[i][1]);
}

/*
void SetHostMclk(int mclk)
{
	int sdiv, mdiv, pdiv = 3;
	int temp, max, min, i, diff;

	if( (mclk < MIN_MCLK) || (mclk > MAX_MCLK) )
		mclk = DEFAULT_MCLK;
	temp = 160000 / mclk;
	for(sdiv = 0; sdiv < 3; sdiv++)
		if((temp >> sdiv) == 0)
			break;
	max = FCLKIN / 1000;
	min = FCLKIN / 2000;
	diff = mclk;
	for(i = max; i >= min; i--)
	{
		mdiv = (mclk * ( i << sdiv)) / FCLKIN;
		mdiv = mdiv > 263 ? 263 : mdiv;
		temp = (mdiv * FCLKIN) / (i << sdiv) - mclk;
		if(temp < diff)
		{
			diff = temp;
			pdiv = i;
		}
	}
	pdiv -= 2;
	mdiv = ((mclk * ((pdiv+2) << sdiv)) / FCLKIN) - 8;
	mdiv = mdiv > 255 ? 255 : mdiv;
	mclk = ((mdiv+8) * FCLKIN) / ((pdiv+2) << sdiv);
    //rPLLCON = (mdiv << 12) | (pdiv << 4) | sdiv;
	WriteMemory(REG_PLLCON, (mdiv << 12) | (pdiv << 4) | sdiv);

	temp = 0xff;
	while(temp--);

	gHostContext.mclk = mclk;
}
*/

void SetHostMclk(int mclk)
{
	int temp;
	HOSTCLK hostclk;

	if( (mclk < MIN_MCLK) || (mclk > MAX_MCLK) )
		mclk = DEFAULT_MCLK;

	CalHostMclk(FCLKIN, mclk, &hostclk);

	WriteMemory(REG_PLLCON, (hostclk.clkpara.m << 12) | (hostclk.clkpara.p << 4) | hostclk.clkpara.s);

	temp = 0xff;
	while(temp--);

	gHostContext.mclk = (int)hostclk.mclk;
}


static void CachInit(void)
{
//	rNCACHBE0 = ((_RAM_STARTADDRESS & 0xfffff000) << 4) | (NONECACH_START >> 12);
	WriteMemory(REG_NCACHBE0, ((_RAM_STARTADDRESS & 0xfffff000) << 4) | (NONECACH_START >> 12));
}

const char gDiv[] = {0X01, 0X02, 0X03, 0X04, 0X05, 0X06, 0X07, 0X08, 0X0A, 0X0C, 0X0E, 0X10};
const char gFac[] = {0X08, 0X00, 0X0A, 0X01, 0X0C, 0X02, 0X0E, 0X03, 0X04, 0X05, 0X06, 0X07};

void SetI2sClk(int clk)
{
	int mclk, div, i, len = sizeof(gDiv);

	if(clk < MIN_I2SCLK)
		clk = I2S_DEFAULT_CLK;
	mclk = GetHostMclk();
	div = mclk / clk;
	for(i = 0; i < len; i++)
	{
		if(div == gDiv[i])
		{
			//rIISCON &= 0xfffe;
			//rIISPSR = gFac[i];
			//rIISCON |= 0x1;
			SetMemory(REG_IISCON, 0xfffe, AND_OPCODE);
			WriteMemory(REG_IISPSR, gFac[i]);
			SetMemory(REG_IISCON, 1, OR_OPCODE);
			gHostContext.i2sclk = mclk / div;
			break;
		}
	}
	if(i == len)
	{
		//rIISCON &= 0xfffe;
		//rIISPSR = gFac[0];
		//rIISCON |= 0x1;
		SetMemory(REG_IISCON, 0xfffe, AND_OPCODE);
		WriteMemory(REG_IISPSR, gFac[0]);
		SetMemory(REG_IISCON, 1, OR_OPCODE);
		gHostContext.i2sclk = mclk;
	}
}

static void I2sClkInit(void)
{
	//rIISCON = 0x02; //Tx DMA enable,Rx idle,prescaler enable
	//rIISMOD = 0x089; //Master,Tx,L-ch=low,iis,16bit ch.,codeclk=256fs,lrck=32fs
	//SetI2sClk(0);
	//rPCONE	= 0x25568; //************************* PE7(CODECLK) is used for 558 clock
	WriteMemory(REG_IISCON, 2);
	WriteMemory(REG_IISMOD, 0x89);
	SetI2sClk(0);
	WriteMemory(REG_PCONE, 0x25568);
}

void HostInit(void)
{
	WriteMemory(REG_SYSCFG, CACHECFG);	// Using 8KB Cache//
	CachInit();
	memset(&gHostContext, 0, sizeof(THostContext));
	Isr_Init();
	Port_Init();
	SetHostMclk(DEFAULT_MCLK);
	I2sClkInit();
	tmInit();
	Uart_Init(DEFAULT_BAUD, DEFAULT_BAUD);
	TimerInit();
}


#ifdef WIN32

int GetHostMclk(void)
{
	return SimHost_GetHostMclk();
}

int GetI2sClk(void)
{
	return SimHost_GetI2sClk();
}

#else
int GetHostMclk(void)
{
	return gHostContext.mclk;
}

int GetI2sClk(void)
{
	return gHostContext.i2sclk;
}

#endif


static void CalMaxMinClkPara(unsigned long pllin, unsigned long mclk, PCLK_PARA max_mps, PCLK_PARA min_mps)
{
	max_mps->p = (unsigned short)(pllin/1000);
	min_mps->p = (unsigned short)(pllin/2000);
	max_mps->p = max_mps->p > MAX_P ? MAX_P : max_mps->p;
	min_mps->p = min_mps->p < MIN_P ? MIN_P : min_mps->p;

	min_mps->s = MIN_S+1;
	for(max_mps->s = (MAX_S+1); max_mps->s >= min_mps->s; max_mps->s--)
	{
		if(mclk*(1<<(max_mps->s-1)) < MAX_FVCO)
			break;
	}

	max_mps->m = (unsigned short)( (mclk * max_mps->p * (1<<(max_mps->s-1)))/pllin );
	min_mps->m = (unsigned short)( (mclk * min_mps->p * (1<<(min_mps->s-1)))/pllin );
	max_mps->m = max_mps->m > MAX_M ? MAX_M : max_mps->m;
	min_mps->m = min_mps->m < MIN_M ? MIN_M : min_mps->m;
}

static void CalHostMclk(unsigned long pllin, unsigned long mclk, PHOSTCLK pHostClk)
{
    unsigned long clkout, clkdiff, temp;
	unsigned short m, p, s;
	CLK_PARA max_mps;
	CLK_PARA min_mps;

	CalMaxMinClkPara(pllin, mclk, &max_mps, &min_mps);

	clkdiff = mclk;

	for(s = max_mps.s; s >= min_mps.s; s--)
	{
		for(p = min_mps.p; p <= max_mps.p; p++)
		{
			for(m = max_mps.m; m >= min_mps.m; m--)
			{
				clkout = (pllin * m) / ( p * (1<<(s-1)) );
				temp = clkout > mclk ? (clkout - mclk) : (mclk - clkout);
				if(temp < clkdiff)
				{
					clkdiff = temp;
					pHostClk->mclk = clkout;
					pHostClk->clkpara.m = m;
					pHostClk->clkpara.p = p;
					pHostClk->clkpara.s = s;

					if(clkdiff == 0)
						break;
				}
			}
		}
	}

	pHostClk->clkpara.m -= 8;
	pHostClk->clkpara.p -= 2;
	pHostClk->clkpara.s -= 1;
}
