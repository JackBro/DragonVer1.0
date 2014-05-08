
#ifndef _USBREG_H_
#define _USBREG_H_

/*--------------------------------------------------------------------------*/
/*-----------------------   Configuration Registers   ----------------------*/
/*--------------------------------------------------------------------------*/
#define USB_REG_MCNTRL     0x00                 /*Main control register   */
#define USB_REG_CCONF      0x01                 /*Clk. config. register   */    
#define USB_REG_RID        0x03                 /*Rev. ID      register   */  
#define USB_REG_FAR        0x04                 /*Func address register   */  
#define USB_REG_NFSR       0x05                 /*Node func st register   */   
#define USB_REG_MAEV       0x06                 /*Main event   register   */  
#define USB_REG_MAMSK      0x07                 /*Main mask    register   */  
#define USB_REG_ALTEV      0x08                 /*Alt. event   register   */  
#define USB_REG_ALTMSK     0x09                 /*ALT  mask    register   */  
#define USB_REG_TXEV       0x0A                 /*TX   event   register   */ 
#define USB_REG_TXMSK      0x0B                 /*TX   mask    register   */   
#define USB_REG_RXEV       0x0C                 /*RX   event   register   */ 
#define USB_REG_RXMSK      0x0D                 /*RX   mask    register   */   
#define USB_REG_NAKEV      0x0E                 /*NAK  event   register   */  
#define USB_REG_NAKMSK     0x0F                 /*NAK  mask    register   */  
#define USB_REG_FWEV       0x10                 /*FIFO warning register   */   
#define USB_REG_FWMSK      0x11                 /*FIFO warning mask       */  
#define USB_REG_FNH        0x12                 /*Frame nbr hi register   */   
#define USB_REG_FNL        0x13                 /*Frame nbr lo register   */  
#define USB_REG_DMACNTRL   0x14                 /*DMA  control register   */  

#define USB_REG_DMAEV	   0x15                 /*DMA  event register   */  
#define USB_REG_DMAMSK	   0x16                 /*DMA  event mask   */  
#define USB_REG_MIR		   0x17                 /*DMA  mirror register   */  
#define USB_REG_DMACNT	   0x18                 /*DMA  count register   */  
#define USB_REG_DMAERR	   0x19                 /*DMA  error register   */  

#define USB_REG_WKUP	   0x1b
 
#define USB_REG_EPC0       0x20                 /*Endpoint0    register   */   
#define USB_REG_TXD0       0x21                 /*TX   data    register 0 */  
#define USB_REG_TXS0       0x22                 /*TX   status  register 0 */ 
#define USB_REG_TXC0       0x23                 /*TX   command register 0 */  
 
#define USB_REG_RXD0       0x25                 /*RX   data    register 0 */   
#define USB_REG_RXS0       0x26                 /*RX   status  register 0 */   
#define USB_REG_RXC0       0x27                 /*RX   command register 0 */   
 
#define USB_REG_EPC1       0x28                 /*Endpoint1    register   */   
#define USB_REG_TXD1       0x29                 /*TX   data    register 1 */  
#define USB_REG_TXS1       0x2A                 /*TX   status  register 1 */ 
#define USB_REG_TXC1       0x2B                 /*TX   command register 1 */  
 
#define USB_REG_EPC2       0x2C                 /*Endpoint2    register   */   
#define USB_REG_RXD1       0x2D                 /*RX   data    register 1 */   
#define USB_REG_RXS1       0x2E                 /*RX   status  register 1 */   
#define USB_REG_RXC1       0x2F                 /*RX   command register 1 */   
 
#define USB_REG_EPC3       0x30                 /*Endpoint3    register   */   
#define USB_REG_TXD2       0x31                 /*TX   data    register 2 */  
#define USB_REG_TXS2       0x32                 /*TX   status  register 2 */ 
#define USB_REG_TXC2       0x33                 /*TX   command register 2 */  
 
#define USB_REG_EPC4       0x34                 /*Endpoint4    register   */   
#define USB_REG_RXD2       0x35                 /*RX   data    register 2 */   
#define USB_REG_RXS2       0x36                 /*RX   status  register 2 */   
#define USB_REG_RXC2       0x37                 /*RX   command register 2 */   
 
#define USB_REG_EPC5       0x38                 /*Endpoint5    register   */   
#define USB_REG_TXD3       0x39                 /*TX   data    register 3 */  
#define USB_REG_TXS3       0x3A                 /*TX   status  register 3 */ 
#define USB_REG_TXC3       0x3B                 /*TX   command register 3 */  
 
#define USB_REG_EPC6       0x3C                 /*Endpoint6    register   */   
#define USB_REG_RXD3       0x3D                 /*RX   data    register 3 */   
#define USB_REG_RXS3       0x3E                 /*RX   status  register 3 */   
#define USB_REG_RXC3       0x3F                 /*RX   command register 3 */   
 

#endif 

