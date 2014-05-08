
#ifndef _ARM_S3C44BOX_HOSTTYPE_H_
#define _ARM_S3C44BOX_HOSTTYPE_H_

/* PENDING BIT */
//CAUTION:You must clear the pending bit as general special register.
//        it's different way with KS32C6x00 
#define BIT_ADC		(0x1)
#define BIT_RTC		(0x1<<1)
#define BIT_UTXD1	(0x1<<2)
#define BIT_UTXD0	(0x1<<3)
#define BIT_SIO		(0x1<<4)
#define BIT_IIC		(0x1<<5)
#define BIT_URXD1	(0x1<<6)
#define BIT_URXD0	(0x1<<7)
#define BIT_TIMER5	(0x1<<8)
#define BIT_TIMER4	(0x1<<9)
#define BIT_TIMER3	(0x1<<10)
#define BIT_TIMER2	(0x1<<11)
#define BIT_TIMER1	(0x1<<12)
#define BIT_TIMER0	(0x1<<13)
#define BIT_UERR01	(0x1<<14)
#define BIT_WDT		(0x1<<15)
#define BIT_BDMA1	(0x1<<16)
#define BIT_BDMA0	(0x1<<17)
#define BIT_ZDMA1	(0x1<<18)
#define BIT_ZDMA0	(0x1<<19)
#define BIT_TICK	(0x1<<20)
#define BIT_EINT4567	(0x1<<21)
#define BIT_EINT3	(0x1<<22)
#define BIT_EINT2	(0x1<<23)
#define BIT_EINT1	(0x1<<24)
#define BIT_EINT0	(0x1<<25)
#define BIT_GLOBAL	(0x1<<26)

#endif


