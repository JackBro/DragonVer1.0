


#ifndef _TRM_USB_BASEFUNC_H_
#define _TRM_USB_BASEFUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------*/
/*-------------------   Configuration Register Bitmaps   -------------*/
/*--------------------------------------------------------------------*/

/*-------------------------------- MCNTRL ------------------------*/
#define SRST       0x01                 /*software reset          */   
#define DBG        0x02                 /*debug mode              */  
#define VGE        0x04                 /*voltage regulator enable*/    
#define NAT        0x08                 /*node attached           */  
#define INT_DIS    0x00                 /*interrupts disabled     */  
#define INT_L_O    0x40                 /*act lo ints, open drain */  
#define INT_H_P    0x80                 /*act hi in	ts, push pull  */  
#define INT_L_P    0xC0                 /*act lo ints, push pull  */  
 
/*------------------------ CCONF -------------------------*/
#define CODIS      0x80                 /*address enable          */   
 
/*--------------------------------- NFSR -------------------------*/
#define RST_ST     0x00                 /*reset       state       */   
#define RSM_ST     0x01                 /*resume      state       */  
#define OPR_ST     0x02                 /*operational state       */  
#define SUS_ST     0x03                 /*suspend     state       */  
 
/*---------------------------- MAEV, MAMSK -----------------------*/
#define WARN       0x01                 /*warning bit has been set*/   
#define ALT        0x02                 /*alternate event         */  
#define TX_EV      0x04                 /*transmit event          */  
#define FRAME      0x08                 /*SOF packet received     */  
#define NAK        0x10                 /*NAK event               */  
#define ULD        0x20                 /*unlock locked detected  */   
#define RX_EV      0x40                 /*receive event           */   
#define INTR_E     0x80                 /*master interrupt enable */  
 
/*---------------------------- ALTEV, ALTMSK ---------------------*/
#define ALT_EOP    0x08                 /*end of packet           */   
#define ALT_SD3    0x10                 /*3 ms suspend            */  
#define ALT_SD5    0x20                 /*5 ms suspend            */  
#define ALT_RESET  0x40                 /*reset  detected         */  
#define ALT_RESUME 0x80                 /*resume detected         */ 
 
/*---------------------------- TXEV, TXMSK -----------------------*/
#define TX_FIFO0    0x01                 /*TX_DONE, FIFO 0         */   
#define TX_FIFO1    0x02                 /*TX_DONE, FIFO 1         */   
#define TX_FIFO2    0x04                 /*TX_DONE, FIFO 2         */   
#define TX_FIFO3    0x08                 /*TX_DONE, FIFO 3         */   
#define TX_UDRN0    0x10                 /*TX_URUN, FIFO 0         */   
#define TX_UDRN1    0x20                 /*TX_URUN, FIFO 1         */  
#define TX_UDRN2    0x40                 /*TX_URUN, FIFO 2         */  
#define TX_UDRN3    0x80                 /*TX_URUN, FIFO 3         */  
 
/*---------------------------- RXEV, RXMSK -----------------------*/
#define RX_FIFO0    0x01                 /*RX_DONE, FIFO 0         */   
#define RX_FIFO1    0x02                 /*RX_DONE, FIFO 1         */   
#define RX_FIFO2    0x04                 /*RX_DONE, FIFO 2         */   
#define RX_FIFO3    0x08                 /*RX_DONE, FIFO 3         */   
#define RX_OVRN0    0x10                 /*RX_OVRN, FIFO 0         */   
#define RX_OVRN1    0x20                 /*RX_OVRN, FIFO 1         */  
#define RX_OVRN2    0x40                 /*RX_OVRN, FIFO 2         */  
#define RX_OVRN3    0x80                 /*RX_OVRN, FIFO 3         */  
 
/*-------------------------- NAKEV, NAKMSK -----------------------*/
#define NAK_IN0		0x01                 /*IN  NAK, FIFO 0         */   
#define NAK_IN1     0x02                 /*IN  NAK, FIFO 1         */  
#define NAK_IN2     0x04                 /*IN  NAK, FIFO 2         */  
#define NAK_IN3     0x08                 /*IN  NAK, FIFO 3         */  
#define NAK_OUT0	0x10                 /*OUT NAK, FIFO 0         */  
#define NAK_OUT1    0x20                 /*OUT NAK, FIFO 1         */ 
#define NAK_OUT2    0x40                 /*OUT NAK, FIFO 2         */ 
#define NAK_OUT3    0x80                 /*OUT NAK, FIFO 3         */ 
 
/*---------------------------- FWEV, FWMSK -----------------------*/
#define TXFIFO1		0x02                
#define TXFIFO2     0x04                
#define TXFIFO3     0x08                
#define RXFIFO1     0x08                
#define RXFIFO2		0x10                
#define RXFIFO3		0x20                

/*------------------------------- FNH ---------------------------*/
#define RFC			0x20                

/*---------------------------------- FAR -------------------------*/
#define AD_EN      0x80                 /*address enable          */   


/*---------------- EPCX   bits ------------------------------------*/
#define     DEF        0x40                 /*force def. adr (0 only) */
#define     STALL      0x80                 /*force stall handshakes  */
#define     ISO        0x20                 /*set for isochr. (1-3)   */
#define     EP_EN      0x10                 /*enables endpt.  (1-3)   */

/*------------------------------- TXC0 ---------------------------*/
#define IGN_IN     0x10                 /*ignore in  tokens       */   

/*------------------------------- TXCx ---------------------------*/
#define TX_EN      0x01                 /*transmit enable         */   
#define TX_LAST    0x02                 /*specifies last byte     */ 
#define TX_TOGL    0x04                 /*specifies PID used      */ 
#define FLUSH      0x08                 /*flushes all FIFO data   */
#define RFF		   0x10					/*refill FIFO*/
 
/*------------------------------- TXS0 ---------------------------*/
#define TX_EBYTES  0x1F					/*available empty bytes   */
#define TX_DONE    0x20                 /*transmit done           */   
#define ACK_STAT   0x40                 /*ack status of xmission  */
 
/*------------------------------- RXC0 ---------------------------*/
#define RX_EN      0x01                 /*receive  enable         */   
#define IGN_OUT    0x02                 /*ignore out tokens       */   
#define IGN_SETUP  0x04                 /*ignore setup tokens     */  

/*------------------------------- RXSx ---------------------------*/
#define RX_LAST    0x10                 /*indicates RCOUNT valid  */   
#define RX_TOGL    0x20                 /*last pkt was DATA1 PID  */   
#define SETUP_R    0x40                 /*setup packet received   */  
#define RX_ERR     0x80                 /*last packet had an error*/   





void Usb_BitClr(UINT8 adr, UINT8 mask);
void Usb_BitSet(UINT8 adr, UINT8 mask);

/*-------------------------------- MCNTRL ------------------------*/
void Usb_SetMainCtrl(UINT8 val);
UINT8 Usb_GetMainCtrl(void);
void Usb_SoftReset(void);
UINT8 Usb_EnableVoltRegulator(void);
UINT8 Usb_DisableVoltRegulator(void);
UINT8 Usb_EnableNodeAttached(void);
UINT8 Usb_DisableNodeAttached(void);
void Usb_SetInterruptCtrl(UINT8 val);
UINT8 Usb_GetInterruptCtrl(void);

/*---------------------------------- Clock control -------------------------*/
void Usb_SetClkCtrl(UINT8 val);
UINT8 Usb_GetClkCtrl(void);
void Usb_SetClkDiv(UINT8 val);
UINT8 Usb_GetClkDiv(void);
UINT8 Usb_DisableClkOutput(void);
UINT8 Usb_EnableClkOutput(void);

/*---------------------------------- Revision -------------------------*/
UINT8 Usb_GetRevision(void);

/*--------------------------------- NFSR -------------------------*/
void Usb_SetNodeState(UINT8 val);
UINT8 Usb_GetNodeState(void);

/*---------------------------- MAEV -----------------------*/
UINT8 Usb_GetMainEvent(void);

/*---------------------------- MAMSK -----------------------*/
void Usb_SetMainEventMask(UINT8 val);
UINT8 Usb_GetMainEventMask(void);

/*---------------------------- ALTEV -----------------------*/
UINT8 Usb_GetAltEvent(void);

/*---------------------------- ALTMSK -----------------------*/
void Usb_SetAltEventMask(UINT8 val);
UINT8 Usb_GetAltEventMask(void);

/*---------------------------- TXEV -----------------------*/
UINT8 Usb_GetTransEvent(void);

/*---------------------------- TXMSK -----------------------*/
void Usb_SetTransEventMask(UINT8 val);
UINT8 Usb_GetTransEventMask(void);

/*---------------------------- RXEV -----------------------*/
UINT8 Usb_GetRcvEvent(void);

/*---------------------------- RXMSK -----------------------*/
void Usb_SetRcvEventMask(UINT8 val);
UINT8 Usb_GetRcvEventMask(void);

/*---------------------------- NAKEV -----------------------*/
UINT8 Usb_GetNakEvent(void);

/*---------------------------- NAKMSK -----------------------*/
void Usb_SetNakEventMask(UINT8 val);
UINT8 Usb_GetNakEventMask(void);

/*---------------------------- FWEV -----------------------*/
UINT8 Usb_GetFifoWEvent(void);

/*---------------------------- FWMSK -----------------------*/
void Usb_SetFifoWEventMask(UINT8 val);
UINT8 Usb_GetFifoWEventMask(void);

/*---------------------------- FN -----------------------*/
UINT16 Usb_GetFrameNumber(void);
void Usb_ResetFrameCount(void);

/*---------------------------- FAR -----------------------*/
void Usb_SetFuncAddr(UINT8 val);
UINT8 Usb_GetFuncAddr(void);
void Usb_SetAddress(UINT8 val);
UINT8 Usb_GetAddress(void);
UINT8 Usb_EnableAddress(void);
UINT8 Usb_DisableAddress(void);

/*---------------------------- DMACTRL -----------------------*/
void Usb_SetDmaCtrl(UINT8 val);
UINT8 Usb_GetDmaCtrl(void);

/*---------------------------- DMAEV -----------------------*/
void Usb_SetDmaEvent(UINT8 val);
UINT8 Usb_GetDmaEvent(void);

/*---------------------------- DMAMSK -----------------------*/
void Usb_SetDmaEventMask(UINT8 val);
UINT8 Usb_GetDmaEventMask(void);

/*---------------------------- DMACNT -----------------------*/
void Usb_SetDmaCount(UINT8 val);
UINT8 Usb_GetDmaCount(void);

/*---------------------------- DMAERR -----------------------*/
void Usb_SetDmaError(UINT8 val);
UINT8 Usb_GetDmaError(void);

/*---------------------------- WKUP -----------------------*/
void Usb_SetWakeup(UINT8 val);
UINT8 Usb_GetWakeup(void);



/*---------------------------- TXEPC0 - TXEPC3 -----------------------*/
void	Usb_SetTxEpc(UINT8 index, UINT8 val);
UINT8	Usb_GetTxEpc(UINT8 index);
UINT8	Usb_EnableTxStall(UINT8 index);
UINT8	Usb_DisableTxStall(UINT8 index);
UINT8	Usb_DefEpc0(void);

/*---------------------------- RXEPC - RXEPC3 -----------------------*/
void	Usb_SetRxEpc(UINT8 index, UINT8 val);
UINT8	Usb_GetRxEpc(UINT8 index);
UINT8	Usb_EnableRxStall(UINT8 index);
UINT8	Usb_DisableRxStall(UINT8 index);

/*---------------------------- TXS1 - TXS3 -----------------------*/
UINT8	Usb_GetTxStatus(UINT8 index);

/*---------------------------- TXC1 - TXC3 -----------------------*/
void	Usb_SetTxCmd(UINT8 index, UINT8 val);
UINT8	Usb_GetTxCmd(UINT8 index);
UINT8	Usb_EnableTx(UINT8 index);
UINT8	Usb_DisableTx(UINT8 index);
UINT8	Usb_FlushTx(UINT8 index);

/*---------------------------- TXD1 - TXD3 -----------------------*/
void	Usb_SetTxData(UINT8 index, UINT8 val);
UINT8	Usb_GetTxData(UINT8 index);

/*---------------------------- RXS1 - RXS3 -----------------------*/
UINT8	Usb_GetRxStatus(UINT8 index);

/*---------------------------- RXC1 - RXC3 -----------------------*/
void	Usb_SetRxCmd(UINT8 index, UINT8 val);
UINT8	Usb_GetRxCmd(UINT8 index);
UINT8	Usb_EnableRx(UINT8 index);
UINT8	Usb_DisableRx(UINT8 index);
UINT8	Usb_FlushRx(UINT8 index);

/*---------------------------- RXD1- RXD3 -----------------------*/
void	Usb_SetRxData(UINT8 index, UINT8 val);
UINT8	Usb_GetRxData(UINT8 index);



#ifdef __cplusplus
}
#endif

#endif //_TRM_USB_BASEFUNC_H_

