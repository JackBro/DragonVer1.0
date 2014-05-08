
#include "../v558rdk/vregdef.h"
#include "../v558rdk/v558api.h"
#include "v558testdemo.h"
#include "../setupenv/setupenv.h"


static __irq void _isrEINT1(void)
{
	rI_ISPC = BIT_EINT1;
	//V558_CtrlIntHandle();
}

static void Isr_Init(void)
{
 	rPCONG |= 0xc;		//EINT1
   	rPUPG = 0x0;		//pull up enable
	rEXTINT &= ~(0xf0);
	rEXTINT |= 0x10; 	//"1" level mode
	pISR_EINT1 = (unsigned)_isrEINT1;
	rINTMSK &=~(BIT_EINT0 | BIT_GLOBAL);	//Default value=0x7ffffff//
}

static void ResetV558Chip()
{
	rPDATF = 0x0;
	Delay(100);
	rPDATF = 0x1;
}

static void Bus_Init(int bus)
{
	if(bus == V558_BUS_MULTI16)
		BusInit(BUS_IDX_4, BUS16);
	else
		BusInit(BUS_IDX_4, BUS8);
}

void V558_InitEnv(void)
{
	Bus_Init(V558_BUS_TYPE);
	ResetV558Chip();
	Isr_Init();
}

