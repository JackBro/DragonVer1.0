/********************************************************************/
/*                                                                  */
/*      Peripheral Interrupt Definition for KS32C41100              */
/*                                                                  */
/*------------------------------------------------------------------*/
/*      Copyright (C) 1999 Samsung Electronics.                     */
/*------------------------------------------------------------------*/
/*                                                                  */
/*                                                                  */
/*   Modified, programmed by Sun Xin                                */
/*                                                                  */
/*            History : 1999-04-06 first edited                     */
/*			2000-07-20 second modified by J. Byun	    			*/
/********************************************************************/

#ifndef _WMAINTCONTROL_H_
#define _WMAINTCONTROL_H_


/*-------------------------------------------------------*/
/* Interrupt source bit position in interrupt controller */
/*-------------------------------------------------------*/
#define INT_ADC			(0x1)
#define INT_RTC			(0x1<<1)
#define INT_UTXD1			(0x1<<2)
#define INT_UTXD0			(0x1<<3)
#define INT_SIO			(0x1<<4)
#define INT_IIC			(0x1<<5)
#define INT_URXD1			(0x1<<6)
#define INT_URXD0			(0x1<<7)
#define INT_TIMER5		(0x1<<8)
#define INT_TIMER4		(0x1<<9)
#define INT_TIMER3		(0x1<<10)
#define INT_TIMER2		(0x1<<11)
#define INT_TIMER1		(0x1<<12)
#define INT_TIMER0		(0x1<<13)
#define INT_UERR01		(0x1<<14)
#define INT_WDT			(0x1<<15)
#define INT_BDMA1			(0x1<<16)
#define INT_BDMA0			(0x1<<17)
#define INT_ZDMA1			(0x1<<18)
#define INT_ZDMA0			(0x1<<19)
#define INT_TICK			(0x1<<20)
#define INT_EXT47			(0x1<<21)
#define INT_EXT3			(0x1<<22)
#define INT_EXT2			(0x1<<23)
#define INT_EXT1			(0x1<<24)
#define INT_EXT0			(0x1<<25)

#define INT_GLOBAL		(0x1<<26)

/*-------------------------*/
/* Interrupt source number */
/*-------------------------*/
#define nINT_ADC			0
#define nINT_RTC			1
#define nINT_UTXD1		2
#define nINT_UTXD0		3
#define nINT_SIO			4
#define nINT_IIC			5
#define nINT_URXD1		6
#define nINT_URXD0		7
#define nINT_TIMER5		8
#define nINT_TIMER4		9
#define nINT_TIMER3		10
#define nINT_TIMER2		11
#define nINT_TIMER1		12
#define nINT_TIMER0		13
#define nINT_UERR01		14
#define nINT_WDT			15
#define nINT_BDMA1		16
#define nINT_BDMA0		17
#define nINT_ZDMA1		18
#define nINT_ZDMA0		19
#define nINT_TICK			20
#define nINT_EXT47		21
#define nINT_EXT3			22
#define nINT_EXT2			23
#define nINT_EXT1			24
#define nINT_EXT0			25

#define nINT_GLOBAL		26

/*----------------------------------*/
/*  Macros for interrupt controller */
/*----------------------------------*/

#define Enable_Intr(n)				rINTMSK &= (~(1<<n))
#define Disable_Intr(n)			rINTMSK |= (1<<n)

#define Clear_PendingBit(n)		if((rINTMOD & (1<<n)))	\
									rF_ISPC = (1<<n);	\
								else					\
									rI_ISPC = (1<<n);	\
								while(0)

#define Clear_Pend_Bits(n)			if((rINTMOD & n))		\
									rF_ISPC = n;		\
								else					\
									rI_ISPC = n;		\
								while(0)



#endif /*  _WMAINTCONTROL_H_ */