#include <string.h>
#include <stdio.h>
#include "..\inc\option.h"
#include "..\inc\def.h"
#include "..\inc\44b.h"
#include "..\inc\44blib.h"

//*******************************

volatile unsigned int err=0;
extern unsigned char *downPt;
extern unsigned int fileSize;
extern int Image$$RO$$Limit;
extern int Image$$RW$$Base;

extern int Test_Cache(void);
extern void Syswork(void);

void Isr_Init(void);
void __irq Uerror(void);

void Main(void)
{
	rSYSCFG=CACHECFG;	// Using 8KB Cache//
	
	Port_Init();
	Isr_Init();
	Mem_Init(0);

	Led_Display(0xf);
	Delay(0);
	downPt=(unsigned char *)DOWNLOAD_ADDRESS;

	CLock_Init(24000000, 57600);

	Uart_Select(0); //Select UART0//
	/*	Cache test	*/
	if(Test_Cache())
		Uart_Printf("\nCache Test:OK!");
	else
		Uart_Printf("\nCache Test:FAIL!");

	Syswork();

	while(1);
}

void Isr_Init(void)
{
	rINTCON=0x5;	      //Non-vectored,IRQ enable,FIQ disable //
	rINTMOD=0x0;	      //All=IRQ mode//
	rINTMSK=~(BIT_EINT0 | BIT_GLOBAL);	//Default value=0x7ffffff//
    
	//pISR_FIQ,pISR_IRQ must be initialized//
//	pISR_EINT0=(unsigned)usb_isr;
}

void __irq Uerror(void)
{
	rI_ISPC=BIT_UERR01 ;   //clear pending bits,Default value=0x0000000//
	err++;
}


