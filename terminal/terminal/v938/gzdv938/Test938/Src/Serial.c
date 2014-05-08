/********************************************************************/
/*                                                                  */
/*      Serial Driver Program for KS32C41100                        */
/*                                                                  */
/*------------------------------------------------------------------*/
/*      Copyright (C) 1999 Samsung Electronics.                     */
/*------------------------------------------------------------------*/
/*                                                                  */
/*   Modified, programmed by Sun Xin                                */
/*                                                                  */
/*            History : 1999-04-06 first edited                     */
/*			2000-07-21 second modified for KS32C41100   */
/*				   by J. Byun			    */	
/********************************************************************/



/***********************************************************************/
/*                                                                     */
/*           Copyright 1991 - 1995, Integrated Systems, Inc.           */
/*                      ALL RIGHTS RESERVED                            */
/*                                                                     */
/*   Permission is hereby granted to licensees of Integrated Systems,  */
/*   Inc. products to use or abstract this computer program for the    */
/*   sole purpose of implementing a product based on Integrated        */
/*   Systems, Inc. products.   No other rights to reproduce, use,      */
/*   or disseminate this computer program, whether in part or in       */
/*   whole, are granted.                                               */
/*                                                                     */
/*   Integrated Systems, Inc. makes no representation or warranties    */
/*   with respect to the performance of this computer program, and     */
/*   specifically disclaims any responsibility for any damages,        */
/*   special or consequential, connected with the use of this program. */
/*                                                                     */
/*---------------------------------------------------------------------*/
/*                                                                     */
/*   This module supplies the low-level serial routines for both the   */
/*   polled (pROBE+) and interrupt-driven (pSOS+) serial interface.    */
/*                                                                     */
/***********************************************************************/


#include "..\inc\def.h"
#include "..\inc\44b.h"

#include "..\inc\serial.h"
#include "..\inc\IntControl.h"

extern UINT32 g_mclk;

void __irq Uart0_TxFifoInt(void);
void __irq Uart0_RxFifoInt(void);
void __irq Uart01_RxFifoErrorInt(void);

void __irq Uart1_TxFifoInt(void);
void __irq Uart1_RxFifoInt(void);

static void (*Rx_Isr0)(UINT8 uOneByte) = (void (*)(UINT8 uOneByte))0;
static void (*Tx_Isr0)() = (void (*)())0;
static void (*Rx_Isr1)(UINT8 uOneByte) = (void (*)(UINT8 uOneByte))0;
static void (*Tx_Isr1)() = (void (*)())0;


/***********************************************************************/
/*   	Temporal Interrupt Serivce Routines for UART0 and UART1,       */
/*   only used during system startup.                                  */
/*                                                                     */
/*    isr_uart_rcv:  Interrupt service routine for rcved data          */
/*    isr_uart_xmt:  Interrupt service routine for xmition completion  */
/*    isr_uart_stat: Interrupt service routine for rcved status        */
/*                                                                     */
/***********************************************************************/

void Uart0_TxFifoInt(void)
{
    (*Tx_Isr0)(); 
    rI_ISPC=BIT_UTXD0;        
}
void Uart0_RxFifoInt(void)
{
	UINT32 dwNum,i,dwUfStat; 

    rI_ISPC=BIT_URXD0;

	dwUfStat= rUFSTAT0;
	dwNum = dwUfStat&0xf;
	for(i=0 ;i<dwNum; i++)
	{
//		UINT8 OneByte = rURXH0;
   		(*Rx_Isr0)(rURXH0);
  	}
}

void Uart1_TxFifoInt(void)
{
    (*Tx_Isr1)(); 
    rI_ISPC=BIT_UTXD1;        
}
void Uart1_RxFifoInt(void)
{
    rI_ISPC=BIT_URXD1;
   	(*Rx_Isr1)('0');
}

void Uart01_RxFifoErrorInt(void)
{
	UINT8 OneByte;
	rI_ISPC=BIT_UERR01;
	OneByte = rURXH0;

}


/***********************************************************************/
/* Channel_Init:Initialize the UART for polling/interrupt mode         */
/*              operation, which includes set Line Control Register,   */
/*              UART Control Register and Baudrate Register.           */
/*                                                                     */
/***********************************************************************/
void Channel_Init(UINT32  chnl, UINT32 lcon ,UINT32 ucon, UINT32 ufcon,UINT32 baud)
{
    int value;

    SetUartLCON(chnl, lcon);                
    SetUartCONT(chnl, ucon);
    SetUartFCON(chnl, ufcon);
    
    /*------------------------------------------------------------------*/
    /* Baudrate Register:                                               */
    /*------------------------------------------------------------------*/
    if(baud) 
        value = (g_mclk*10 / (16 * baud) + 5)/10 - 1;
    else	
        value = (g_mclk*10 / (16 * BAUDRATE_57600) + 5)/10 - 1; // default: 57600 bps
        
    SetUartBRD(chnl, value);    	
}

/***********************************************************************/
/* SerialIntInit: Initialize a serial channel for interrupt-driven     */
/*                  operation                                          */
/*      INPUTS: minor: Minor device number (0 - 3)                     */
/*              rx_isr: ptr to high-level code's receive ISR           */
/*              tx_isr: ptr to high-level code's transmit ISR          */
/*                                                                     */
/*     RETURNS: address of device's data area                          */
/*                                                                     */
/***********************************************************************/

void SerialIntInit(UINT32  minor, UINT32 lcon ,UINT32 ucon, UINT32 ufcon,UINT32 baud, void (*rx_isr)(UINT8 uOneByte), void (*tx_isr)())
{
    /*-----------------------------------------------------------------*/
    /* Initialize UART channel                                         */
    /*-----------------------------------------------------------------*/
    Channel_Init(minor, lcon ,ucon, ufcon, baud);
    

    /*-----------------------------------------------------------------*/
    /* Save global pointers to high-level interrupt service routines   */
    /*-----------------------------------------------------------------*/
    if(minor == 2)
    {
    		Rx_Isr1 = rx_isr;
    		Tx_Isr1 = tx_isr;
#ifndef WIN32
	    	pISR_UTXD1=(unsigned)Uart1_TxFifoInt;
	    	pISR_URXD1=(unsigned)Uart1_RxFifoInt;
#endif
   	}else
    {
    		Rx_Isr0 = rx_isr;
    		Tx_Isr0 = tx_isr;
#ifndef WIN32
	    	pISR_UTXD0=(unsigned)Uart0_TxFifoInt;
	    	pISR_URXD0=(unsigned)Uart0_RxFifoInt;
#endif
	} 
#ifndef WIN32
	pISR_UERR01=(unsigned)Uart01_RxFifoErrorInt;
#endif
    
    /*-----------------------------------------------------------------*/
    /* Set interrupts as IRQ mode                                      */
    /*-----------------------------------------------------------------*/
    Set_IRQ_UART_Intr(minor);
    
	Enable_Intr(nINT_URXD0);
}
