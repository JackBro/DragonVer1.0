#include "..\inc\def.h"
#include "..\inc\44b.h"

#include "..\inc\IntControl.h"
#include "..\inc\timer.h"

extern UINT32 g_mclk;


void __irq Timer0_isr(void);		//For deal command.
static	void (*CommandDeal)() = (void (*)())0;
void 	InitTimer0ForCommandParase(void (*func)())
{
	UINT32 Freq, Tcount;

	CommandDeal= func;
#ifndef WIN32
	pISR_TIMER0 = (unsigned)Timer0_isr;
#endif
	rTCFG0 = 0xa;
	
	rTCFG1 = TIMER0_CLK_DIVISOR_8;
	Freq = 	g_mclk/8/0xa	;	// Timer2_clock (in Hz)
        							// Prescaler=256=2^8, Divisor=4=2^2
        							// Timer2_clock=MCLK_FREQ/prescale/Divisor

	Tcount = (10*Freq)/1000;
	rTCNTB0 = Tcount;

	rTCON = TIMER0_LOAD;
	rTCON = TIMER0_RUN | TIMER0_INTERVAL;

	Enable_Intr(nINT_TIMER0);

}

void Timer0_isr(void)
{
    rI_ISPC=BIT_TIMER0;
	CommandDeal();
}


void __irq Timer1_isr(void);		//For deal command.
static	void (*Timer1)() = (void (*)())0;


void 	InitTimer1ForTest(void (*func)())
{
	UINT32 Freq, Tcount;

	Timer1 = func;
#ifndef WIN32
	pISR_TIMER1 = (unsigned)Timer1_isr;
#endif
	rTCFG0 = 0xa;
	
	rTCFG1 = TIMER0_CLK_DIVISOR_8|TIMER1_CLK_DIVISOR_8;
	Freq = 	g_mclk/8/0xa	;	// Timer2_clock (in Hz)
        							// Prescaler=256=2^8, Divisor=4=2^2
        							// Timer2_clock=MCLK_FREQ/prescale/Divisor

	Tcount = (500*Freq)/1000;
	rTCNTB1 = Tcount;

	rTCON = TIMER0_LOAD|TIMER1_LOAD;
	rTCON = TIMER0_RUN|TIMER0_INTERVAL|TIMER1_RUN | TIMER1_INTERVAL;

	Enable_Intr(nINT_TIMER1);

}

void Timer1_isr(void)
{
    rI_ISPC=BIT_TIMER1;
	Timer1();
}

