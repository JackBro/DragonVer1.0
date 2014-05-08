/********************************************************************/
/*                                                                  */
/*      Header for UART of KS32C41100                               */
/*------------------------------------------------------------------*/
/*      Copyright (C) 1999 Samsung Electronics.                     */
/*------------------------------------------------------------------*/
/*                                                                  */
/*   Modified, programmed by Sun Xin                                */
/*                                                                  */
/*            History : 1999-04-06 first edited                     */
/*                      1999-11-20 modified UART_Intr_Enable()      */
/*                                 macro to get around the UART     */
/*                                 TX problem                       */
/*			2000-07-21 modified for KS32C41100	    */
/*				   by J. Byun			    */		
/********************************************************************/

#ifndef _BSP_UART_H_
#define _BSP_UART_H_


#ifndef PORT1
#define OPENPORT        0  // Default Serial Port = PORT0
#else
#define OPENPORT        1
#endif

/* Channel Number */
#define CHANNEL1        0x1
#define CHANNEL2     	0x2


/* Uart Clock Frequency */
#define	UCLK_FREQ		MCLK_FREQ	

#define BSP_SERIAL             2
#define BSP_NEW_SERIAL         YES//NO
#define BSP_SERIAL_MINBAUD     300
#define BSP_SERIAL_MAXBAUD     115200

/*	Baud rate	*/
#define BAUDRATE_9600	9600
#define BAUDRATE_19200	19200
#define BAUDRATE_38400	38400
#define BAUDRATE_57600	57600
#define BAUDRATE_115200	115200

/*
 *	Control Register Bit Definitions
 *
 *        9   8   7   6   5   4   3   2   1   0
 *      +---+---+---+---+---+---+---+---+---+---|
 *      |Tty|Rty|Rti|Rst|LBk|SBr|  TxM  |  RxM  |
 *      +---+---+---+---+---+---+---+---+---+---|
 *
 *	Tty: Tx intr type		0: pulse, 1:level 
 *	Rty: Rx intr type		0: pulse, 1:level
 *	Rti: Rx Time-out intr		0: disable, 1: enable
 *	Rst: Rcv stat intr enable	0: disable, 1: enable
 *	LBk: Loopback mode		0: disable, 1: enable
 *	SBr: Send Break			0: No break, 1: Send Break
 *  	TxM: Xmit Mode          	00: disable, 01: intr, 10:DMA0, 11: DMA1
 *	RxM: Rcv Mode			00: disable, 01: intr, 10:DMA0, 11: DMA1
 */
 
/* bit 0-1 */
#define	UART_CTRL_RCV_INTR_OFF		(0x00)
#define	UART_CTRL_RCV_INTR_ON		(0x01)

/* bit 2-3 */
#define	UART_CTRL_XMT_INTR_OFF		(0x00<<2)
#define	UART_CTRL_XMT_INTR_ON		(0x01<<2)
#define	UART_CTRL_TXMODE_BDMA0	    	(0x02<<2)

/* bit 4 */
#define	UART_CTRL_DONT_SEND_BREAK	(0x00<<4)
#define	UART_CTRL_SEND_BREAK		(0x01<<4)

/* bit 5 */
#define	UART_CTRL_NO_LOOPBACK		(0x00<<5)
#define UART_CTRL_LOOPBACK          	(0x01<<5)

/* bit 6 */
#define UART_CTRL_STAT_INTR_OFF    	(0x00<<6)
#define	UART_CTRL_STAT_INTR_ON		(0x01<<6)

/* bit 7 */
#define UART_CTRL_RCV_TOUT_INTR_OFF    	(0x00<<7)
#define	UART_CTRL_REV_TOUT_INTR_ON	(0x01<<7)

/* bit 8 */
#define UART_CTRL_RCV_INTR_PULSE     	(0x00<<8)
#define	UART_CTRL_RCV_INTR_LEVEL	(0x01<<8)

/* bit 9 */
#define UART_CTRL_XMT_INTR_PULSE     	(0x00<<9)
#define	UART_CTRL_XMT_INTR_LEVEL	(0x01<<9)

#define	UART_CTRL_RXMODE_BITS		0x03
#define	UART_CTRL_TXMODE_BITS		0x0c

#define	UART_CTRL_DEFAULTS		(UART_CTRL_NO_LOOPBACK |	\
					 UART_CTRL_DONT_SEND_BREAK |	\
           				 UART_CTRL_XMT_INTR_ON |	\
           				 UART_CTRL_STAT_INTR_ON |	\
           				 UART_CTRL_RCV_INTR_ON )	
           							 
#define	UART_CTRL_DEFAULTS_BDMA0 	(UART_CTRL_DEFAULTS &          \
				  	 (~UART_CTRL_TXMODE_BITS) |    \
				  	 UART_CTRL_TXMODE_BDMA0 ) 


/*
 *	Line Control Register Bit Definitions
 *
 *        7   6   5   4   3   2   1   0
 *      +---+---+---+---+---+---+---+---+
 *      | x |IRM|ParityMode |Stp| WdLen |
 *      +---+---+---+---+---+---+---+---+
 *	IRM: IR mode select		0: Serial, 1: IR/RF Tx/Rx mode
 *	Pari: Parity Mode		0xx: No Parity, 100: odd, 101: Even
 *					110: Parity forced/checked as 1
 *					111: Parity forced/checked as 0
 *	Stp: Number of Stop Bit		0: 1stop bit, 1: 2stop bit
 *	WdLen: Word length		00: 5bit, 01: 6bit, 10: 7bit, 11: 8bit
 */

#define	UART_MODE_WORD_5		0x00
#define	UART_MODE_WORD_6		0x01
#define	UART_MODE_WORD_7		0x02
#define	UART_MODE_WORD_8		0x03


#define	UART_MODE_STOP_1		(0x00<<2)
#define	UART_MODE_STOP_2		(0x01<<2)

#define UART_MODE_PARITY_NONE		(0x00<<3)
#define UART_MODE_PARITY_ODD		(0x04<<3)
#define UART_MODE_PARITY_EVEN 		(0x05<<3)

#define	UART_MODE_SERIAL		(0x00<<7)

#define UART_MODE_DEFAULTS		(UART_MODE_SERIAL |       \
           				 UART_MODE_PARITY_NONE |  \
           				 UART_MODE_STOP_1 |       \
           				 UART_MODE_WORD_8)


/*
 *	TX/RX Status Register Bit Definitions
 *
 *        7   6   5   4   3   2   1   0
 *	+---+---+---+---+---+---+---+---+
 *      | x | x | x | x | x | 2 | 1 | 0 |
 *	+---+---+---+---+---+---+---+---+
 *
 *	Bit 0: Rcv FIFO ready/Data Rcv		0: no data, 1: data rcved.
 *	Bit 1: Xmit FIFO empty/Xmit Buf Emp	0: not empty, 1: empty
 *	Bit 2: Transmit shifter empty		0: Tx in progress, 1: empty
 */

#define UART_STAT_RCV_DATA		0x01
#define	UART_STAT_XMIT_EMPTY		0x02
#define	UART_STAT_XMITTER_EMPTY		0x04


/*
 *	ERROR Status Register Bit Definitions
 *
 *        7   6   5   4   3   2   1   0
 *	+---+---+---+---+---+---+---+---+
 *      | x | x | x | x | 3 | 2 | 1 | 0 |
 *	+---+---+---+---+---+---+---+---+
 *
 *	Bit 0: Overrun error		0: no error, 1: overrun
 *	Bit 1: Parity error		0: no error, 1: parity error
 *	Bit 2: Frame error		0: no error, 1: frame error
 *	Bit 3: Break intr occurred	0: no break, 1: break rcved
 */

#define UART_STAT_OVERRUN		0x01
#define UART_STAT_PARITY		0x02
#define	UART_STAT_FRAME			0x04
#define	UART_STAT_BREAK			0x08

#define	UART_STAT_ERROR	    (UART_STAT_OVERRUN | UART_STAT_PARITY | UART_STAT_FRAME)


/*
 *	FIFO Control Register Bit Definitions
 *
 *        7   6   5   4   3   2   1   0
 *	+---+---+---+---+---+---+---+---+
 *      | TxLvl | RxLvl | x |Trt|Rrt|Fen|
 *	+---+---+---+---+---+---+---+---+
 *
 *	TxLvl: Tx FIFO Triggrt Level	00: empty, 01: 4byte, 10: 8byte, 11:12byte
 *	RxLvl: Rx FIFO Triggrt Level	00: empty, 01: 4byte, 10: 8byte, 11:12byte
 *	Trt: Tx FIFO Reset		0: normal, 1: Tx FIFO reset
 *	Rrt: Rx FIFO Reset		0: normal, 1: Rx FIFO reset
 *	Fen: FIFO enable		0: disable, 1: enable
 */

#define UART_FIFO_DISABLE		(0x00)
#define UART_FIFO_ENSABLE		(0x01)

#define UART_RX_FIFO_NORMAL		(0x00<<1)
#define UART_RX_FIFO_RESET		(0x01<<1)

#define UART_TX_FIFO_NORMAL		(0x00<<2)
#define UART_TX_FIFO_RESET		(0x01<<2)

#define UART_RX_FIFO_TRLVL_4		(0x00<<4)
#define UART_RX_FIFO_TRLVL_8		(0x01<<4)
#define UART_RX_FIFO_TRLVL_12		(0x02<<4)
#define UART_RX_FIFO_TRLVL_16		(0x03<<4)

#define UART_TX_FIFO_TRLVL_4		(0x00<<6)
#define UART_TX_FIFO_TRLVL_8		(0x01<<6)
#define UART_TX_FIFO_TRLVL_12		(0x02<<6)
#define UART_TX_FIFO_TRLVL_16		(0x03<<6)


/*
 *	MODEM CONTROL Register Bit Definitions
 *
 *        7   6   5   4   3   2   1   0
 *	+---+---+---+---+---+---+---+---+
 *      | x | x | x |AFC| x | x | x |Rqt|
 *	+---+---+---+---+---+---+---+---+
 *
 *	AFC: Auto Flow Control		0: disable, 1: enable
 *	Rqt: Request to Send		0: 'H' level, 1: 'L' level
 *
 */

#define UART_MODEM_CON_REQ_H		(0x00)
#define UART_MODEM_CON_REQ_L		(0x01)

#define UART_MODEM_CON_AFC_DISABLE	(0x00<<4)
#define UART_MODEM_CON_AFC_ENABLE	(0x01<<4)


/*
 *	FIFO STATUS Register Bit Definitions
 *
 *        9   8   7   6   5   4   3   2   1   0
 *      +---+---+---+---+---+---+---+---+---+---|
 *      |Tfl|Rfl| #Tx FIFO data | #Rx FIFO data |
 *      +---+---+---+---+---+---+---+---+---+---|
 *
 *	Tfl: Tx FIFO full		0: not full, 1: full
 *	Rfl: Rx FIFO full		0: not full, 1: full
 *	#Tx FIFO data : Number of data in Tx FIFO
 *	#Rx FIFO data : Number of data in Tx FIFO
 *
 */

#define UART_TX_FIFO_NOTFULL		(0x01<<9)
#define UART_TX_FIFO_FULL		(0x01<<8)


/*
 *	MODEM STATUS Register Bit Definitions



 *
 *        7   6   5   4   3   2   1   0
 *	+---+---+---+---+---+---+---+---+
 *      | x | x | x | x | x |DLC| x |CTS|
 *	+---+---+---+---+---+---+---+---+
 *
 *	DLC: Delta CTS			0: not changed, 1: changed
 *	CTS: Clear to Send signal	0: not activated, 1: activated
 *
 */
 
#define UART_MODEM_CTS_CHANGED		(0x01<<2)
#define UART_MODEM_CTS_ACTIVATE		(0x01) 
 
 
/* --------------------------------------------------------------------
 *	UART Control Register Access Macros
 * --------------------------------------------------------------------
 */

/* Wait for Xmitter to be available to be written */
#define	WaitXmitter(chnl)								\
				if(chnl == 2)						\
					while(!(rUTRSTAT1 & UART_STAT_XMIT_EMPTY));	\
				else							\
					while(!(rUTRSTAT0 & UART_STAT_XMIT_EMPTY));	\
				while(0)

/* Wait for XShifter empty */
#define	WaitXShifter(chnl)								\
				if(chnl == 2)						\
					while(!(rUTRSTAT1 & UART_STAT_XMITTER_EMPTY));	\
				else							\
					while(!(rUTRSTAT0 & UART_STAT_XMITTER_EMPTY));	\
				while(0)
								
/* Wait for Rcver to be available to be read */
#define	WaitRcver(chnl)									\
				if(chnl == 2)						\
					while( !(rUTRSTAT1 & UART_STAT_RCV_DATA));	\
				else							\
					while( !(rUTRSTAT0 & UART_STAT_RCV_DATA));	\
				while(0)			

/* Macros for UART Register Access */	
#define SetUartFCON(chnl, value) 	if(chnl == 2)			\
						rUFCON1 = value;	\
					else				\
						rUFCON0 = value;	\
					while(0)
									
#define SetUartLCON(chnl, value) 	if(chnl == 2)			\
						rULCON1 = value;	\
					else				\
						rULCON0 = value;	\
					while(0)	

#define SetUartCONT(chnl, value) 	if(chnl == 2)			\
						rUCON1 = value;		\
					else				\
						rUCON0 = value;		\
					while(0)
									
#define SetUartBRD(chnl, value) 	if(chnl == 2)			\
						rUBRDIV1 = value;	\
					else				\
						rUBRDIV0 = value;	\
					while(0)

#define ReadUartTRSTAT(chnl) 		((chnl == 2) ? 	rUTRSTAT1 : rUTRSTAT0)

#define ReadUartERSTAT(chnl) 		((chnl == 2) ? 	rUERSTAT1 : rUERSTAT0)

#define WriteUartTXH(chnl, value) 	if(chnl == 2)			\
						rUTXH1 = value;		\
					else				\
						rUTXH0 = value;		\
					while(0)

#define ReadUartRXB(chnl) 		((chnl == 2) ? 	rURXH1 : rURXH0)


/* Disable all transfer mode including interrupt generation */
#define UART_Channel_Off(chnl)		if(chnl == 2) 			\
    						rUCON1 &= 0x30;		\
					else				\
						rUCON0 &= 0x30;		\
					while(0)
								
/* --------------------------------------------------------------------
 *	UART Interrupt Control Macros
 * --------------------------------------------------------------------
 */
 
// Corresponding interrupt control bits in INTMSK
#define	UART_CTRL_INTR_BITS(chnl)	((chnl == 2)?				\
					(INT_UTXD1 | INT_URXD1 | INT_UERR01) :	\
					(INT_UTXD0 | INT_URXD0 | INT_UERR01) )

#define Disable_All_UART_Intr(chnl)						\
					( rINTMSK |= UART_CTRL_INTR_BITS(chnl) )

#define Enable_All_UART_Intr(chnl)						\
					( rINTMSK &= ~UART_CTRL_INTR_BITS(chnl) )

#define Set_IRQ_UART_Intr(chnl)							\
					( rINTMOD &= ~UART_CTRL_INTR_BITS(chnl) )
					
#define Disable_UART_ERR_Intr(chnl)	if(chnl == 2)			\
						rUCON1 &= 0x3bf;	\
					else				\
						rUCON0 &= 0x3bf;	\
					while(0)

#define Enable_UART_ERR_Intr(chnl)	if(chnl == 2)			\
						rUCON1 |= 0x40;		\
					else				\
						rUCON0 |= 0x40;		\
					while(0)
											
/***********************************************************************/
/*	UART-related definitions for DISI                              */
/***********************************************************************/

 
/*---------------------------------------------------------------------*/
/* Uart interrupt vector ID                                            */
/*---------------------------------------------------------------------*/
#define V_RXD	0
#define V_TXD	1
#define V_USR	2
/*
static int vector_table[] = {
			   (nINT_URXD0),	
			   (nINT_UTXD0),	
			   (nINT_UERR01),	
			   (nINT_URXD1),	
			   (nINT_UTXD1),	
			   (nINT_UERR01),	
		    };
*/		   
/*---------------------------------------------------------------------*/
/* interrupt control  bit                                              */ 
/*---------------------------------------------------------------------*/
#define RXINT(chnl) 	((chnl == 2) ? 	INT_URXD1 : INT_URXD0)
#define TXINT(chnl) 	((chnl == 2) ? 	INT_UTXD1 : INT_UTXD0)
#define LSINT(chnl) 	((chnl == 2) ? 	INT_UERR01 : INT_UERR01) 


/*---------------------------------------------------------------------*/
/* UART interrupt control                                              */ 
/*---------------------------------------------------------------------*/
/*
#define UART_Intr_Enable(chnl, n)						\
				rINTMSK &= ~n;					\
				if(n & TXINT(chnl)) {				\
					SetUartCONT(chnl, 0);			\
					SetUartCONT(chnl, UART_CTRL_DEFAULTS);	\
				}						\
				while(0)
*/

				
#define UART_Intr_Enable(chnl, n)							\
				rINTMSK &= ~n;						\
				if(n & TXINT(chnl)) { 					\
					SetUartCONT(chnl, UART_CTRL_DEFAULTS_BDMA0); 	\
					SetUartCONT(chnl, UART_CTRL_DEFAULTS);		\
				}							\
				while(0)
				
void SerialIntInit(UINT32  minor, UINT32 lcon ,UINT32 ucon, UINT32 ufcon,UINT32 baud, void (*rx_isr)(UINT8 uOneByte), void (*tx_isr)());
				

#endif  /* _BSP_UART_H_ */
