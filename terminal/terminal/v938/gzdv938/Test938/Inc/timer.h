
#ifndef _TIMER_H_
#define _TIMER_H_



/* ----- Timer Configuration Register1 Bit Pattern ------ */

#define TIMER0_CLK_DIVISOR_2	(0 << 0)
#define TIMER0_CLK_DIVISOR_4	(1 << 0)
#define TIMER0_CLK_DIVISOR_8	(2 << 0)
#define TIMER0_CLK_DIVISOR_16	(3 << 0)
#define TIMER0_CLK_DIVISOR_32	(4 << 0)

#define TIMER1_CLK_DIVISOR_2	(0 << 4)
#define TIMER1_CLK_DIVISOR_4	(1 << 4)
#define TIMER1_CLK_DIVISOR_8	(2 << 4)
#define TIMER1_CLK_DIVISOR_16	(3 << 4)
#define TIMER1_CLK_DIVISOR_32	(4 << 4)

#define TIMER2_CLK_DIVISOR_2	(0 << 8)
#define TIMER2_CLK_DIVISOR_4	(1 << 8)
#define TIMER2_CLK_DIVISOR_8	(2 << 8)
#define TIMER2_CLK_DIVISOR_16	(3 << 8)
#define TIMER2_CLK_DIVISOR_32	(4 << 8)

#define TIMER3_CLK_DIVISOR_2	(0 << 12)
#define TIMER3_CLK_DIVISOR_4	(1 << 12)
#define TIMER3_CLK_DIVISOR_8	(2 << 12)
#define TIMER3_CLK_DIVISOR_16	(3 << 12)
#define TIMER3_CLK_DIVISOR_32	(4 << 12)

#define TIMER4_CLK_DIVISOR_2	(0 << 16)
#define TIMER4_CLK_DIVISOR_4	(1 << 16)
#define TIMER4_CLK_DIVISOR_8	(2 << 16)
#define TIMER4_CLK_DIVISOR_16	(3 << 16)
#define TIMER4_CLK_DIVISOR_TCLK	(4 << 16)

#define TIMER5_CLK_DIVISOR_2	(0 << 20)
#define TIMER5_CLK_DIVISOR_4	(1 << 20)
#define TIMER5_CLK_DIVISOR_8	(2 << 20)
#define TIMER5_CLK_DIVISOR_16	(3 << 20)
#define TIMER5_CLK_DIVISOR_32	(4 << 20)

#define DMA_MODE_NO_SEL		(0 << 24)
#define DMA_MODE_TIMER0		(1 << 24)
#define DMA_MODE_TIMER1		(2 << 24)
#define DMA_MODE_TIMER2		(3 << 24)
#define DMA_MODE_TIMER3		(4 << 24)
#define DMA_MODE_TIMER4		(5 << 24)
#define DMA_MODE_TIMER5		(6 << 24)

/* ----- Timer Control Register Bit Pattern ------ */

#define TIMER0_RUN			(1 << 0)
#define TIMER1_RUN			(1 << 8)
#define TIMER2_RUN			(1 << 12)
#define TIMER3_RUN			(1 << 16)
#define TIMER4_RUN			(1 << 20)
#define TIMER5_RUN			(1 << 24)
 
#define TIMER0_LOAD			(1 << 1)
#define TIMER1_LOAD			(1 << 9)
#define TIMER2_LOAD			(1 << 13)
#define TIMER3_LOAD			(1 << 17)
#define TIMER4_LOAD			(1 << 21)
#define TIMER5_LOAD			(1 << 25)

#define TIMER0_INTERVAL			(1 << 3)
#define TIMER1_INTERVAL			(1 << 11)
#define TIMER2_INTERVAL			(1 << 15)
#define TIMER3_INTERVAL			(1 << 18)
#define TIMER4_INTERVAL			(1 << 22)
#define TIMER5_INTERVAL			(1 << 26)

/* -----	Common macros ----- */

#define Timer0_Stop()    (rTCON &= ~TIMER0_RUN)
#define Timer1_Stop()    (rTCON &= ~TIMER1_RUN)
#define Timer2_Stop()    (rTCON &= ~TIMER2_RUN)
#define Timer3_Stop()    (rTCON &= ~TIMER3_RUN)
#define Timer4_Stop()    (rTCON &= ~TIMER4_RUN)
#define Timer5_Stop()    (rTCON &= ~TIMER5_RUN)

void 	InitTimer0ForCommandParase(void (*func)());
void 	InitTimer1ForTest(void (*func)());


#endif  /* _TIMER_H_ */

