#include "..\inc\def.h"
#include "..\inc\44b.h"
#include "..\inc\44blib.h"
#include "..\inc\option.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define STACKSIZE    0xa00 //SVC satck size(do not use user stack)//
#define HEAPEND     (_ISR_STARTADDRESS-STACKSIZE-0x500) // = 0xc7ff000//
//SVC Stack Area:0xc(e)7ff000-0xc(e)7ffaff//


#define BUS8		(0)
#define BUS16		(1)
#define BUS32		(2)
#define ENWAIT	(1)
#define SRAMBE03	(1)

//Tcah and Tacc must != 0, The value is varified on arm7 board,liuyong 2003.11.13.
#define B4_Tacs		(0x0)	//0clk
#define B4_Tcos		(0x0)	//0clk
#define B4_Tacc		(0x4)	//6clk
#define B4_Toch		(0x0)	//0clk
#define B4_Tcah		(0x2)	//2clk
#define B4_Tacp		(0x0)	//2clk
#define B4_PMC		(0x0)	//no page mode


extern char Image$$RW$$Limit[];
volatile unsigned char *downPt;
unsigned int fileSize;
void *mallocPt=Image$$RW$$Limit;

void (*restart)(void)=(void (*)(void))0x0;
void (*run)(void)=(void (*)(void))DOWNLOAD_ADDRESS;
//--------------------------------SYSTEM---------------------------------//
static int delayLoopCount=400;

UINT32 g_mclk = 12000000;
UINT32 g_band = 57600;

void Delay(int time)
// time=0: adjust the Delay function by WatchDog timer.//
// time>0: the number of loop time//
// 100us resolution.//
{
	int i,adjust=0;
	if(time==0)
	{
		time=200;
		adjust=1;
		delayLoopCount=400;
		rWTCON=((g_mclk/1000000-1)<<8)|(2<<3);	// 1M/64,Watch-dog,nRESET,interrupt disable//
		rWTDAT=0xffff;
		rWTCNT=0xffff;	 
		rWTCON=((g_mclk/1000000-1)<<8)|(2<<3)|(1<<5); // 1M/64,Watch-dog enable,nRESET,interrupt disable //
	}
	for(;time>0;time--)
		for(i=0;i<delayLoopCount;i++);
	if(adjust==1)
	{
		rWTCON=((g_mclk/1000000-1)<<8)|(2<<3);
		i=0xffff-rWTCNT;   //  1count/16us?????????//
		delayLoopCount=8000000/(i*64);	//400*100/(i*64/200)   //
	}
}

//------------------------PORTS------------------------------//
void Port_Init(void)
{
	//CAUTION:Follow the configuration order for setting the ports. 
	// 1) setting value 
	 // 2) setting control register 
	 // 3) configure pull-up resistor.  

	//16bit data bus configuration  

	// PORT A GROUP
	/*  BIT 9		8	7	6	5	4	3	2	1	0	*/
	/*        A24	A23	A22	A21	A20	A19	A18	A17	A16	A0	*/	      
	/*        1		1	1	1	1	1	1	1	1	1	*/
	rPCONA = 0x1ff;	

	// PORT B GROUP
	/*  BIT 10	9		8		7		6		5		4		3		2		1		0		*/
	/*	/CS5	/CS4	/CS3	/CS2	/CS1	nWBE3	nWBE2	/SRAS	/SCAS	SCLK	SCKE	*/
	/*	EXT		NIC		USB		IDE		SMC		NC		NC		Sdram	Sdram	Sdram	Sdram	*/
	/*      1,        1,   1,   1,    1,    1,       1,     1,    1,    1,   1          */
	rPDATB = 0x3ff;
	rPCONB = 0x3ff;
    
	//PORT C GROUP
	//BUSWIDTH=16													*/
	/*  PC15		14		13		12		11		10		9		8	*/
	/*	I		I		RXD1	TXD1	I		I		I		I	*/
	/*	NC		NC		Uart1	Uart1	NC		NC		NC		NC	*/
	/*   00		00		11		11		00		00		00		00	*/

	/*  PC7		6		5		4		3		2		1		0	*/
	/*   I		I		I		I		I		I		I		I	*/
	/*   NC		NC		NC		NC		SMCALE	SMCCLE	SMCCE	SMCRB*/
	/*   00		00		00		00		01		01		01		00	*/
	rPDATC = 0xffff;	//All IO is high
	rPCONC = 0x0f000054;	
	rPUPC  = 0x3000;	//PULL UP RESISTOR should be enabled to I/O

	//PORT D GROUP
	/*  PORT D GROUP(I/O OR LCD)										*/
	/*  BIT7		6		5		4		3		2		1		0	*/
	/*      VF		VM		VLINE	VCLK	VD3		VD2		VD1		VD0	*/
	/*	   00		00		00		00		00		00		00		00	*/
	rPDATD= 0xff;
	rPCOND= 0x0;	
	rPUPD = 0x0;
	//These pins must be set only after CPU's internal LCD controller is enable
	
	//PORT E GROUP 
	/*  Bit 8		7		6		5		4		3		2		1		0		*/
	/*      LcdBL	LED4	LED5	LED6	LED7	BEEP	RXD0	TXD0	LcdDisp	*/ 
	/*      01		01		01		01		01		01		10		10		01		*/
	rPDATE	= 0x1ff;
	rPCONE	= 0x1556a;				//************************* PE0 used for FPGA clock
	rPUPE	= 0x6;
  	//rPCONE=rPCONE&(~(3))|(2);		//PE0=CLKOUT
	
	//PORT F GROUP
	/*  Bit8		7		6		5		 4		3		2		1		0		*/   
	/*  IISCLK	IISDI	IISDO	IISLRCK	Input	Input	Input	IICSDA	IICSCL	*/
	/*	100		100		100		100		00		00		00		10		10		*/
	rPDATF = 0x0;
	rPCONF = 0x24900a;
	rPUPF  = 0x1e3;

	//PORT G GROUP
	/*  BIT7		6		5		4		3		2		1		0	 */
	/*	INT7		INT6		INT5		INT4		INT3		INT2		INT1		INT0	*/
	/*    S3		S4		S5		S6		NIC		EXT		IDE		USB	*/
	/*      11      11      11      11      11      11      11      11       */
	rPDATG = 0xff;
	rPCONG = 0x7fff;	//V558 chip reset, Port G pg7 -- output,
	rPUPG  = 0x0;	//should be enabled  
	rSPUCR=0x7;  //D15-D0 pull-up disable

	/*�����Cache��*/
	rNCACHBE0 = ((unsigned long)(Non_Cache_End>>12)<<16)|(Non_Cache_Start>>12); 
	/*���е��ⲿӲ���ж�Ϊ�͵�ƽ����*/
	rEXTINT=0x0;

//Misc
//USBN9603's A0 pin	->	CPU's A1
//IDE's CS0			->	CPU's A2
//IDE's CS1			->	CPU's A1
//IDE's A2			->	CPU's A3
//IDE's A1			->	CPU's A5
//IDE's A0			->	CPU's A4
}

void Port_V938ChipReset(void)
{
	rPDATG = 0x7f;
	Delay(20);
	rPDATG = 0xff;
}

/************************* UART ****************************/
static int whichUart=0;

void Uart_Init(int mclk,int baud)
{
    int i;
    if(mclk==0)
	mclk=g_mclk;

    rUFCON0=0x0;     //FIFO disable
    rUFCON1=0x0;
    rUMCON0=0x0;
    rUMCON1=0x0;
//UART0
    rULCON0=0x3;     //Normal,No parity,1 stop,8 bit
    rUCON0=0x245;    //rx=edge,tx=level,disable timeout int.,enable rx error int.,normal,interrupt or polling
    rUBRDIV0=( (int)(mclk/16./baud + 0.5) -1 );
//UART1
    rULCON1=0x3;
    rUCON1=0x245;
    rUBRDIV1=( (int)(mclk/16./baud + 0.5) -1 );

    for(i=0;i<100;i++);
}


void Uart_Select(int ch)
{
    whichUart=ch;
}


void Uart_TxEmpty(int ch)
{
    if(ch==0)
		while(!(rUTRSTAT0 & 0x4)); //wait until tx shifter is empty.
    else
    	while(!(rUTRSTAT1 & 0x4)); //wait until tx shifter is empty.
}


char Uart_Getch(void)
{
    if(whichUart==0)
    {	    
		while(!(rUTRSTAT0 & 0x1)); //Receive data read
		return RdURXH0();

    }
    else
    {
		while(!(rUTRSTAT1 & 0x1)); //Receive data ready
		return	rURXH1;
    }
}


char Uart_GetKey(void)
{
    if(whichUart==0)
    {	    
		if(rUTRSTAT0 & 0x1)    //Receive data ready
    	    return RdURXH0();
		else
		    return 0;
    }
    else
    {
		if(rUTRSTAT1 & 0x1)    //Receive data ready
		    return rURXH1;
		else
		    return 0;
    }
}


void Uart_GetString(char *string)
{
    char *string2=string;
    char c;
    while((c=Uart_Getch())!='\r')
    {
		if(c=='\b')
		{
		    if(	(int)string2 < (int)string )
		    {
				Uart_Printf("\b \b");
				string--;
		    }
		}
		else 
		{
		    *string++=c;
		    Uart_SendByte(c);
		}
    }
    *string='\0';
    Uart_SendByte('\n');
}


int Uart_GetIntNum(void)
{
    char str[30];
    char *string=str;
    int base=10;
    int minus=0;
    int lastIndex;
    int result=0;
    int i;
    
    Uart_GetString(string);
    
    if(string[0]=='-')
    {
        minus=1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
		base=16;
		string+=2;
    }
    
    lastIndex=strlen(string)-1;
    if( string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
		base=16;
		string[lastIndex]=0;
		lastIndex--;
    }

    if(base==10)
    {
		result=atoi(string);
		result=minus ? (-1*result):result;
    }
    else
    {
		for(i=0;i<=lastIndex;i++)
		{
    	    if(isalpha(string[i]))
			{
				if(isupper(string[i]))
					result=(result<<4)+string[i]-'A'+10;
				else
				    result=(result<<4)+string[i]-'a'+10;
			}
			else
			{
				result=(result<<4)+string[i]-'0';
			}
		}
		result=minus ? (-1*result):result;
    }
    return result;
}


void Uart_SendByte(int data)
{
	if(whichUart==0)
    {
		while(!(rUTRSTAT0 & 0x2)); //Wait until THR is empty.
		Delay(10);
		WrUTXH0(data);
   	}
	else
    {
		while(!(rUTRSTAT1 & 0x2));  //Wait until THR is empty.
		Delay(10);
		rUTXH1=data;
    }	
}		


void Uart_SendString(char *pt)
{
    while(*pt)
	Uart_SendByte(*pt++);
}


//if you don't use vsprintf(), the code size is reduced very much.
void Uart_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString(string);


    va_end(ap);
}


/******************** S3C44B0X EV. BOARD LED **********************/

void Led_Display(int LedStatus)
{
	if((LedStatus&0x01)==0x01)
		rPDATE=rPDATE&0x17f;
	else
		rPDATE=rPDATE|0x80;
	
	if((LedStatus&0x02)==0x02)
		rPDATE=rPDATE&0x1bf;

	else
		rPDATE=rPDATE|0x40;

	if((LedStatus&0x04)==0x04)
		rPDATE=rPDATE&0x1df;
	else
		rPDATE=rPDATE|0x20;

	if((LedStatus&0x08)==0x08)
		rPDATE=rPDATE&0x1ef;
	else
		rPDATE=rPDATE|0x10;
}

/************************* Timer ********************************/

void Timer_Start(int divider)  //0:16us,1:32us 2:64us 3:128us
{
    rWTCON=((g_mclk/1000000-1)<<8)|(divider<<3);
    rWTDAT=0xffff;
    rWTCNT=0xffff;   

    // 1/16/(65+1),nRESET & interrupt  disable
    rWTCON=((g_mclk/1000000-1)<<8)|(divider<<3)|(1<<5);	
}


int Timer_Stop(void)
{
    rWTCON=((g_mclk/1000000-1)<<8);
    return (0xffff-rWTCNT);
}

void Beep(int BeepStatus)
{
	if (BeepStatus==0)
		rPDATE=rPDATE|0x8;
	else
		rPDATE=rPDATE&0x1f7;

}
/************************* PLL ********************************/
void ChangePllValue(int mdiv,int pdiv,int sdiv)
{
    rPLLCON=(mdiv<<12)|(pdiv<<4)|sdiv;
}


/************************* General Library **********************/

void * malloc(unsigned nbyte) 
/*Very simple; Use malloc() & free() like Stack*/
//void *mallocPt=Image$$RW$$Limit;
{
    void *returnPt=mallocPt;

    mallocPt= (int *)mallocPt+nbyte/4+((nbyte%4)>0); //to align 4byte

    if( (int)mallocPt > HEAPEND )
    {
		mallocPt=returnPt;
		return NULL;
    }
    return returnPt;
}


void free(void *pt)
{
    mallocPt=pt;
}


void Cache_Flush(void)
{
    int i,saveSyscfg;
    
    saveSyscfg=rSYSCFG;

    rSYSCFG=SYSCFG_0KB; 		      
    for(i=0x10004000;i<0x10004800;i+=16)    
    {					   
		*((int *)i)=0x0;		   
    }
    rSYSCFG=saveSyscfg; 			    
}

/*****************************************************************************************
** Uart_GetLongNum()
** Description:	
**	Get a long number form uart.
** Parameters:
**	NULL.
** Returns:
**  	A long number data required.
******************************************************************************************/
long Uart_GetLongNum(void)
{
    char str[30];
    char *string=str;
    int base=10;
    int minus=0;
    int lastIndex;
    long result=0;
    int i;
    
    Uart_GetString(string);
    
    if(string[0]=='-')
    {
        minus=1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
		base=16;
		string+=2;
    }
    
    lastIndex=strlen(string)-1;
    if( string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
		base=16;
		string[lastIndex]=0;
		lastIndex--;
    }

    if(base==10)
    {
		result=atol(string);
		result=minus ? (-1*result):result;
    }
    else
    {
		for(i=0;i<=lastIndex;i++)
		{
			if(string[i] < '0' || string[i] >'9')
//    	    		if(isalpha(string[i]))
			{
				if(isupper(string[i]))
					result=(result<<4)+string[i]-'A'+10;
				else
				    result=(result<<4)+string[i]-'a'+10;
			}
			else
			{
				result=(result<<4)+string[i]-'0';
			}
		}
		result=minus ? (-1*result):result;
    }
    return result;

}

void Mem_Init(UINT8 uBus)
{
  	rPCONE=rPCONE&(~(3))|(3);	//PE0=CLKOUT
	if(uBus == 1)
	{
		rBWSCON=rBWSCON & ~(0xf<<16)|(BUS16<<16);	//nGCS4 BUS16 
	}
	else
	{
		rBWSCON=rBWSCON & ~(0xf<<16)|(BUS8<<16);		//nGCS4 BUS8 
	}
	rBANKCON4=((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Toch<<6)+(B4_Tcah<<4)+(B4_Tacp<<2)+(B4_PMC));
}

void CLock_Init(UINT32 clck, UINT32 band)
{
	int mid, pid, sid;
	switch(clck)
	{
	case 2000000:
		mid = 0x40;
		pid = 0x22;
		sid = 0x3;
		break;
	case 4000000:
		mid = 0x40;
		pid = 0x10;
		sid = 0x3;
		break;
	case 8000000:
		mid = 0x40;
		pid = 0x7;
		sid = 0x3;
		break;
	case 10000000:
		mid = 0x3e;
		pid = 0x05;
		sid = 0x03;
		break;
	case 16000000:
		mid = 0x48;
		pid = 0x3;
		sid = 0x3;
		break;
	case 20000000:
		mid = 0x48;
		pid = 0x2;
		sid = 0x3;
		break;
	case 24000000:
		mid = 0x40;
		pid = 0x1;
		sid = 0x3;
		break;
	case 32000000:
		mid = 0x58;
		pid = 0x1;
		sid = 0x3;
		break;
	case 36000000:
		mid = 0x40;
		pid = 0x2;
		sid = 0x2;
		break;
	default://12000000
		mid = 0x40;
		pid = 0x4;
		sid = 0x3;
		clck = 12000000;
		break;
	}
	ChangePllValue(mid, pid, sid);
	g_mclk = clck;
	g_band = band;
	Uart_Init(clck,g_band);
}

void Uart_SendBuffer(char *pt,long iLength)
{
	long i;
	for(i=0; i<iLength; i++)
	{
		Uart_SendByte(*(pt+i));
	}
}
void Uart_SendUart1Buffer(char *pt,long iLength)
{
	Uart_Select(1);
	Uart_SendBuffer(pt, iLength);
	Uart_Select(0);
}