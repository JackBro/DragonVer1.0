/*************************************************************************/
/*									 */
/* The last modification date:  02-23-2004                               */
/* REMARKS:  Created initial version 1.0                                 */
/*                                                                       */
/*                Copyright (C) 2004 Vimicro CO.,LTD     		 */
/*************************************************************************/
#include "v558def.h"

#include "vregdef.h"

#include "v558api.h"


#ifdef WIN32
#include "../../../BasicFunc/simhost.h"


void V558_Delay(UINT32 time)
{
	time = 0;
}

void V558_GetReg(UINT32 uAddr, UINT8 *uVal)
{
	*uVal = SimHost_558GetReg(uAddr);
}

void V558_SetReg(UINT32 uAddr, UINT8 uVal)
{
	SimHost_558SetReg(uAddr, uVal);
}

void V558_ReadSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize)
{
	SimHost_558ReadSram(uStartAddr, pData, uSize);
}

void V558_WriteSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize)
{
	SimHost_558WriteSram(uStartAddr, pData, uSize);
}

#else


//****************************************************************************************
//Provide the functions in the vrdk.raf. 
//****************************************************************************************
void DrvSetMultiSel(VMV_MULTI_SEL sel);	//Set register 188c ,(multi8)

#define NOINT           0xc0

UINT16 g_V558HoldClk = 0;
UINT8 g_V558DelayFactor = 1;


UINT16 g_V558HoldClkChg = 0;

#define MEM_HOLDCLK() {if(g_V558HoldClk) {g_V558HoldClkChg = g_V558HoldClk; while(g_V558HoldClkChg--);} }

#define JBUF_DELAY 200


#define  OS_ENTER_CRITICAL() IRQFIQDE /* Disable interrupts                        */
/* Note: R0 register need not be saved, for it gets saved outside. Refer SDT userguide 6-5 */
#define  OS_EXIT_CRITICAL() IRQFIQRE /* Restore  interrupts                       */


#define IRQFIQDE
#define IRQFIQRE

#if 0
#define IRQFIQDE __asm                       \
{                                            \
        mrs r0,CPSR;                         \
        stmfd sp!,{r0};                      \
        orr r0,r0,#NOINT;                    \
        msr CPSR_c,r0;                       \
}

#define IRQFIQRE __asm                       \
{                                            \
        ldmfd sp!,{r0};                      \
        msr CPSR_c,r0;                       \
}
#endif

extern void Delay(int time); 
void V558_Delay(UINT32 time)
{
	if(time)
	{
		Delay(time*g_V558DelayFactor);
	}
	
}
#if V558_BUS_TYPE == V558_BUS_SEPERATE

void V558_GetReg(UINT32 uAddr, UINT8 *uVal)
{
	if(uAddr < 0x10000)
	{
		OS_ENTER_CRITICAL()
		*(volatile UINT8 *)(HOST_ADDR_REGUIA) = (UINT8)((uAddr&0xff00)>>8);
		MEM_HOLDCLK()
		*uVal = *(volatile UINT8 *)((UINT32)INTER_ADDR_REGUIA + (UINT32)(uAddr&0xff));
		MEM_HOLDCLK()
		OS_EXIT_CRITICAL()
	}
	else		//sram debug
	{
		if((uAddr >= 0x20000) && (uAddr < 0x30000))	//Jbuf
		{
			//reg V558_REG_BIU_SEL_PORT = 0, not select port access (only effect Jbuf sram)
			V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,0);
		}
		OS_ENTER_CRITICAL()
		*(volatile UINT8 *)(HOST_ADDR_REGUIA) = (UINT8)((uAddr&0xff00)>>8);
		MEM_HOLDCLK()
		*uVal = *(volatile UINT8 *)((UINT32)INTER_ADDR_REGUIA + (UINT32)(uAddr&0xff)+(UINT32)((uAddr>>8)&0x300));
		MEM_HOLDCLK()
		OS_EXIT_CRITICAL()
		if((uAddr >= 0x20000) && (uAddr < 0x30000))	//Jbuf
			V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,1);

	}
}
void V558_SetReg(UINT32 uAddr, UINT8 uVal)
{
	if(uAddr < 0x10000)
	{
		OS_ENTER_CRITICAL()
		*(volatile UINT8 *)(HOST_ADDR_REGUIA) = (UINT8)((uAddr&0xff00)>>8);
		MEM_HOLDCLK()
		*(volatile UINT8 *)((UINT32)INTER_ADDR_REGUIA + (UINT32)(uAddr&0xff)) = (UINT8)(uVal&0xff);
		MEM_HOLDCLK()
		OS_EXIT_CRITICAL()
	}
	else		//sram debug
	{
		if((uAddr >= 0x20000) && (uAddr < 0x30000))	//Jbuf
		{
			//reg V558_REG_BIU_SEL_PORT = 0, not select port access (only effect Jbuf sram)
			V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,0);
		}
		OS_ENTER_CRITICAL()
		*(volatile UINT8 *)(HOST_ADDR_REGUIA) = (UINT8)((uAddr&0xff00)>>8);
		MEM_HOLDCLK()
		*(volatile UINT8 *)((UINT32)INTER_ADDR_REGUIA + (UINT32)(uAddr&0xff) +(UINT32)((uAddr>>8)&0x300)) = (UINT8)(uVal&0xff);
		MEM_HOLDCLK()
		OS_EXIT_CRITICAL()
		if((uAddr >= 0x20000) && (uAddr < 0x30000))	//Jbuf
			V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,1);

	}
}

void V558_ReadSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize)
{
	V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,0x1);

	OS_ENTER_CRITICAL()
	*(volatile UINT8 *)(HOST_ADDR_MEMUIA) = (UINT8)((uStartAddr>>10)&0xff);
	MEM_HOLDCLK()
	*(volatile UINT8 *)(HOST_ADDR_MEMLIA) = (UINT8)((uStartAddr>>2)&0xff);
	MEM_HOLDCLK()
	*(volatile UINT8 *)(HOST_ADDR_MEMLIA0) = (UINT8)(uStartAddr&0x3);
	MEM_HOLDCLK()
	if((uStartAddr>=0x20000) && (uStartAddr<0x30000))//Jpeg buffer
	{
		while(uSize--)
		{
			*(pData++) = *(volatile UINT8 *)(HOST_ADDR_MEMPORT);
			MEM_HOLDCLK()
		}
	}
	else
	{
		while(uSize--)
		{
			*(pData++) = *(volatile UINT8 *)(HOST_ADDR_MEMDATA);
			MEM_HOLDCLK()
		}
	}
	OS_EXIT_CRITICAL()
}

void V558_WriteSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize)
{
	UINT8 *p = pData+uSize;
	
	V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,0x1);

	OS_ENTER_CRITICAL()
	*(volatile UINT8 *)(HOST_ADDR_MEMUIA) = (UINT8)((uStartAddr>>10)&0xff);
	MEM_HOLDCLK()
	*(volatile UINT8 *)(HOST_ADDR_MEMLIA) = (UINT8)((uStartAddr>>2)&0xff);
	MEM_HOLDCLK()
	*(volatile UINT8 *)(HOST_ADDR_MEMLIA0) = (UINT8)(uStartAddr&0x3);
	MEM_HOLDCLK()
	if((uStartAddr>=0x20000) && (uStartAddr<0x30000))//Jpeg buffer
	{
		while(pData < p)
		{
			*(volatile UINT8 *)(HOST_ADDR_MEMPORT) = *(pData++);
			MEM_HOLDCLK()
		}
	}
	else
	{
		while(pData < p)
		{
			*(volatile UINT8 *)(HOST_ADDR_MEMPORT) = *(pData++);
			MEM_HOLDCLK()
		}
	}
	OS_EXIT_CRITICAL()
}


#endif

#if V558_BUS_TYPE == V558_BUS_MULTI8
void V558_GetReg(UINT32 uAddr, UINT8 *uVal)
{
	if(uAddr < 0x8000)
	{
		OS_ENTER_CRITICAL()
		*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_WORDH;
		MEM_HOLDCLK()
		*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)((uAddr&0xff00)>>8);
		MEM_HOLDCLK()
		*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_WORDL;
		MEM_HOLDCLK()
		*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)(uAddr&0xff);
		MEM_HOLDCLK()
		*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_PORT;
		MEM_HOLDCLK()
		*uVal = *(volatile UINT8 *)(MULTI8_ADDR_DATA);
		MEM_HOLDCLK()
		OS_EXIT_CRITICAL()
	}
	else		//sram debug
	{
		if((uAddr >= 0x20000) && (uAddr < 0x30000))//Jpeg buffer
		{
			//reg V558_REG_BIU_SEL_PORT = 0, not select port access (only effect Jbuf sram)
			V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,0x0);
		}


		V558_SetReg((UINT16)V558_REG_BIU_INCREMENT,0x0);	//Addr not auto increate(only effect sram)

		V558_SetReg((UINT16)V558_REG_BIU_MEM_HIGH_WORD,(UINT8)((uAddr&0x30000)>>16));
		V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_H,(UINT8)((uAddr&0xff00)>>8));	
		V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_L,(UINT8)(uAddr&0xff));	
	
		OS_ENTER_CRITICAL()
		//Set multi8 emory port addr
		*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = MULTI8_MEM_PORT;
		MEM_HOLDCLK()
		*uVal = *(volatile UINT8 *)(MULTI8_ADDR_DATA);
		MEM_HOLDCLK()
		OS_EXIT_CRITICAL()
		if((uAddr >= 0x20000) && (uAddr < 0x30000))	//Jbuf
			V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,1);
	}
}
void V558_SetReg(UINT32 uAddr, UINT8 uVal)
{
	if(uAddr < 0x8000)
	{
		OS_ENTER_CRITICAL()
		*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_WORDH;
		MEM_HOLDCLK()
		*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)((uAddr&0xff00)>>8);
		MEM_HOLDCLK()
		*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_WORDL;
		MEM_HOLDCLK()
		*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)(uAddr&0xff);
		MEM_HOLDCLK()
		*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_PORT;
		MEM_HOLDCLK()
		*(volatile UINT8 *)(MULTI8_ADDR_DATA) = uVal;
		MEM_HOLDCLK()
		OS_EXIT_CRITICAL()
	}
	else			//sram debug
	{
		if((uAddr >= 0x20000) && (uAddr < 0x30000))//Jpeg buffer
		{
			//reg V558_REG_BIU_SEL_PORT = 0, not select port access (only effect Jbuf sram)
			V558_SetReg(V558_REG_BIU_SEL_PORT,0x0);
		}

		V558_SetReg((UINT16)V558_REG_BIU_INCREMENT,0x0);	//Addr not auto increate(only effect sram)

		V558_SetReg((UINT16)V558_REG_BIU_MEM_HIGH_WORD,(UINT8)((uAddr&0x30000)>>16));
		V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_H,(UINT8)((uAddr&0xff00)>>8));	
		V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_L,(UINT8)(uAddr&0xff));	
	
		OS_ENTER_CRITICAL()
		//Set multi8 emory port addr
		*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = MULTI8_MEM_PORT;
		MEM_HOLDCLK()
		*(volatile UINT8 *)(MULTI8_ADDR_DATA) = uVal;
		MEM_HOLDCLK()
		OS_EXIT_CRITICAL()
		if((uAddr >= 0x20000) && (uAddr < 0x30000))	//Jbuf
			V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,1);

	}
}

void V558_ReadSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize)
{
	if((uStartAddr>=0x08000) && (uStartAddr<0x20000))//Lcd 40k buffer or lbuf
	{
		V558_SetReg(V558_REG_BIU_MUL_CLR_AUTO, 0x1);
	}

	V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,0x1);

	V558_SetReg((UINT16)V558_REG_BIU_INCREMENT,0x1); //Addr auto increate(only effect sram)

	V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_H, (UINT8)((uStartAddr&0xff00)>>8));	
	V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_L, (UINT8)(uStartAddr&0xff));
	V558_SetReg((UINT16)V558_REG_BIU_MEM_HIGH_WORD, (UINT8)((uStartAddr&0x30000)>>16));	

	OS_ENTER_CRITICAL()
	//Set multi8 emory port addr
	*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = MULTI8_MEM_PORT;
	MEM_HOLDCLK()
	while(uSize>0)
	{
		*pData = *(volatile UINT8 *)(MULTI8_ADDR_DATA);
		MEM_HOLDCLK()
			
		pData++;
		uSize--;
	}
	OS_EXIT_CRITICAL()

	if((uStartAddr>=0x08000) && (uStartAddr<0x20000))//Lcd 40k buffer or lbuf
	{
		V558_SetReg(V558_REG_BIU_MUL_CLR_AUTO, 0x0);
	}

}

void V558_WriteSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize)
{

	if((uStartAddr>=0x08000) && (uStartAddr<0x20000))//Lcd 40k buffer or lbuf
	{
		V558_SetReg(V558_REG_BIU_MUL_CLR_AUTO, 0x1);
	}

	V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,0x1);

	V558_SetReg((UINT16)V558_REG_BIU_INCREMENT,0x1); //Addr auto increate(only effect sram)

	V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_H,(UINT8)((uStartAddr&0xff00)>>8));	
	V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_L,(UINT8)(uStartAddr&0xff));	
	V558_SetReg((UINT16)V558_REG_BIU_MEM_HIGH_WORD,(UINT8)((uStartAddr&0x30000)>>16));

	OS_ENTER_CRITICAL()
	//Set multi8 emory port addr

	*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = MULTI8_MEM_PORT;
	MEM_HOLDCLK()
	while(uSize>0)
	{
		*(volatile UINT8 *)(MULTI8_ADDR_DATA) = *pData;
		MEM_HOLDCLK()
		
		pData++;
		uSize--;
	}
	OS_EXIT_CRITICAL()
	
	if((uStartAddr>=0x08000) && (uStartAddr<0x20000))//Lcd 40k buffer or lbuf
	{
		V558_SetReg(V558_REG_BIU_MUL_CLR_AUTO, 0x0);
	}

}


#endif



#if V558_BUS_TYPE == V558_BUS_MULTI16


void V558_GetReg(UINT32 uAddr, UINT8 *uVal)
{
	if(uAddr < 0x8000)
	{
		OS_ENTER_CRITICAL()

		if(uAddr < 0x1430 || uAddr > 0x144e)
		{
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)uAddr;

			MEM_HOLDCLK()
			if((uAddr&0x1) == 0)
			{
				*uVal = (UINT8)((*(volatile UINT16 *)(MULTI16_ADDR_VAL))&0xff);
			}
			else
			{
				*uVal = (UINT8)((*(volatile UINT16 *)(MULTI16_ADDR_VAL)&0xff00)>>8);
			}
			MEM_HOLDCLK()

		}
		else
		{

			//Set to multi8
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_SEL_8_16;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0;
			MEM_HOLDCLK()

			*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_WORDH;
			MEM_HOLDCLK()
			*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)((uAddr&0xff00)>>8);
			MEM_HOLDCLK()
			*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_WORDL;
			MEM_HOLDCLK()
			*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)(uAddr&0xff);
			MEM_HOLDCLK()
			*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_PORT;
			MEM_HOLDCLK()
			*uVal = *(volatile UINT8 *)(MULTI8_ADDR_DATA);
			MEM_HOLDCLK()

			//Set to multi16
			*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_WORDH;
			MEM_HOLDCLK()
			*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)((((UINT16)V558_REG_BIU_SEL_8_16)&0xff00)>>8);
			MEM_HOLDCLK()
			*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_WORDL;
			MEM_HOLDCLK()
			*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)(((UINT16)V558_REG_BIU_SEL_8_16)&0xff);
			MEM_HOLDCLK()
			*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_PORT;
			MEM_HOLDCLK()
			*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)1;
			MEM_HOLDCLK()
		}

		OS_EXIT_CRITICAL()

	}
	else	//sram debug
	{
		OS_ENTER_CRITICAL()

		if((uAddr>=0x20000) && (uAddr<0x30000))//Jpeg buffer
		{
			//reg V558_REG_BIU_SEL_PORT = 0,not select port access (only effect Jbuf sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_SEL_PORT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x0;
			MEM_HOLDCLK()
		}

		//reg V558_REG_BIU_INCREMENT = 0,Addr not auto increate(only effect sram)
		*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_INCREMENT;
		MEM_HOLDCLK()
		*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x0;
		MEM_HOLDCLK()


		//reg V558_REG_BIU_MEM_HIGH_WORD
		*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_HIGH_WORD;
		MEM_HOLDCLK()
		*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (UINT16)((uAddr&0x30000)>>16);
		MEM_HOLDCLK()
		//reg V558_REG_BIU_MEM_LOW_WORD
		*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_LOW_WORD_L;
		MEM_HOLDCLK()
		*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (UINT16)(uAddr&0xffff);
		MEM_HOLDCLK()

		//Set multi6 emory port addr
		*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
		MEM_HOLDCLK()
		if((uAddr&0x1) == 0)
		{
			*uVal = (UINT8)(*(volatile UINT16 *)(MULTI16_ADDR_VAL));
		}
		else
		{
			*uVal = (UINT8)((*(volatile UINT16 *)(MULTI16_ADDR_VAL)&0xff00)>>8);
		}
		MEM_HOLDCLK()

		if((uAddr>=0x20000) && (uAddr<0x30000))//Jpeg buffer
		{
			//reg V558_REG_BIU_SEL_PORT = 1,select port access (only effect Jbuf sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_SEL_PORT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x1;
			MEM_HOLDCLK()
		}

		OS_EXIT_CRITICAL()
	}
}
void V558_SetReg(UINT32 uAddr, UINT8 uVal)
{
	UINT16 uTemp;

	if(uAddr < 0x8000)
	{
		OS_ENTER_CRITICAL()

		*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)uAddr;
		MEM_HOLDCLK()
		uTemp = *(volatile UINT16 *)(MULTI16_ADDR_VAL);
		MEM_HOLDCLK()
		if((uAddr&0x1) == 0)
		{
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (uTemp&0xff00)|uVal;
		}
		else
		{
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (uTemp&0xff)|(uVal<<8);
		}
		MEM_HOLDCLK()

		if(uAddr>=0x41a&&uAddr<=0x42b)
		{
         		uTemp = 0x8001;
   			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_LCD_LCDTEST;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = uTemp;
			MEM_HOLDCLK()
		}

		OS_EXIT_CRITICAL()
	}
	else		//sram debug
	{
		OS_ENTER_CRITICAL()

		if((uAddr>=0x20000) && (uAddr<0x30000))//Jpeg buffer
		{
			//reg V558_REG_BIU_SEL_PORT = 0,not select port access (only effect Jbuf sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_SEL_PORT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x0;
			MEM_HOLDCLK()
		}

		//reg V558_REG_BIU_INCREMENT = 0,Addr not auto increate(only effect sram)
		*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_INCREMENT;
		MEM_HOLDCLK()
		*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x0;
		MEM_HOLDCLK()



		//reg V558_REG_BIU_MEM_HIGH_WORD
		*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_HIGH_WORD;
		MEM_HOLDCLK()
		*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (UINT16)((uAddr&0x30000)>>16);
		MEM_HOLDCLK()
		//reg V558_REG_BIU_MEM_LOW_WORD

		*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_LOW_WORD_L;
		MEM_HOLDCLK()
		*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (UINT16)(uAddr&0xffff);
		MEM_HOLDCLK()



		//Set multi6 emory port addr
		*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
		MEM_HOLDCLK()
		uTemp = *(volatile UINT16 *)(MULTI16_ADDR_VAL);
		MEM_HOLDCLK()
		if((uAddr&0x1) == 0)
		{
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (uTemp&0xff00)|uVal;
		}
		else
		{
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (uTemp&0xff)|(uVal<<8);
		}
		MEM_HOLDCLK()
		if((uAddr>=0x20000) && (uAddr<0x30000))//Jpeg buffer
		{
			//reg V558_REG_BIU_SEL_PORT = 1,select port access (only effect Jbuf sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_SEL_PORT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x1;
			MEM_HOLDCLK()
		}

		OS_EXIT_CRITICAL()
	}

}

void V558_ReadSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize)
{
	UINT16 uTemp = 0;
	UINT8 uLeft = 0;

	if((uStartAddr>=0x08000) && (uStartAddr<0x20000))//Lcd 40k buffer or lbuf
	{
		V558_SetReg(V558_REG_BIU_MUL_CLR_AUTO, 0x1);
	}

	V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,0x1);

	V558_SetReg((UINT16)V558_REG_BIU_INCREMENT,0x1); //Addr auto increate(only effect sram)

	V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_L, (UINT8)(uStartAddr&0xff));	
	V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_H, (UINT8)((uStartAddr&0xff00)>>8));	
	V558_SetReg((UINT16)V558_REG_BIU_MEM_HIGH_WORD, (UINT8)((uStartAddr&0x30000)>>16));

	OS_ENTER_CRITICAL()
	
	//Set multi6 emory port addr
	*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
	MEM_HOLDCLK()

	if((uStartAddr>=0x10000) && (uStartAddr<0x20000))//Lcd 40k buffer
	{
		if(uStartAddr&0x1)
		{
			*pData = (UINT8)((*(volatile UINT16 *)(MULTI16_ADDR_VAL))&0xff);
			MEM_HOLDCLK()

			uSize--;
			pData++;
		}
		uLeft = (UINT8)(uSize & 0x1);
		uSize /= 2;


		while(uSize>0)
		{
			uTemp = *(volatile UINT16*)(MULTI16_ADDR_VAL);
			MEM_HOLDCLK()
			*pData = (UINT8)((uTemp&0xff00)>>8);

			pData++;

			*pData = (UINT8)uTemp;


			pData++;
			uSize--;
		}
		if(uLeft == 1)
		{
			*pData = (UINT8)((*(volatile UINT16 *)(MULTI16_ADDR_VAL)&0xff00)>>8);
			MEM_HOLDCLK()
		}
	}
	else
	{
		if(uStartAddr&0x1)
		{
			*pData = (UINT8)((*(volatile UINT16 *)(MULTI16_ADDR_VAL)&0xff00)>>8);
			MEM_HOLDCLK()

			uSize--;
			pData++;
		}
		uLeft = (UINT8)(uSize & 0x1);
		uSize /= 2;


		while(uSize>0)
		{
			uTemp = *(volatile UINT16*)(MULTI16_ADDR_VAL);
			MEM_HOLDCLK()
			*pData = (UINT8)uTemp;


			pData++;

			*pData = (UINT8)((uTemp&0xff00)>>8);

			pData++;
			uSize--;
		}
		if(uLeft == 1)
		{
			*pData = (UINT8)((*(volatile UINT16 *)(MULTI16_ADDR_VAL))&0xff);
			MEM_HOLDCLK()
		}

	}
	OS_EXIT_CRITICAL()

	if((uStartAddr>=0x08000) && (uStartAddr<0x20000))//Lcd 40k buffer or lbuf
	{
		V558_SetReg(V558_REG_BIU_MUL_CLR_AUTO, 0x0);
	}

}

void V558_WriteSram(UINT32 uStartAddr, UINT8* pData, UINT32 uSize)
{
	UINT16 uTemp = 0;
	UINT8 uLeft = 0;

	if((uStartAddr>=0x08000) && (uStartAddr<0x20000))//Lcd 40k buffer or lbuf
	{
		V558_SetReg(V558_REG_BIU_MUL_CLR_AUTO, 0x1);
	}

	V558_SetReg((UINT16)V558_REG_BIU_SEL_PORT,0x1);

	V558_SetReg((UINT16)V558_REG_BIU_INCREMENT,0x1); //Addr auto increate(only effect sram)

	V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_L,(UINT8)(uStartAddr&0xff));	
	V558_SetReg((UINT16)V558_REG_BIU_MEM_LOW_WORD_H,(UINT8)((uStartAddr&0xff00)>>8));	
	V558_SetReg((UINT16)V558_REG_BIU_MEM_HIGH_WORD,(UINT8)((uStartAddr&0x30000)>>16));

	if((uStartAddr>=0x10000) && (uStartAddr<0x20000))//Lcd 40k buffer
	{

		OS_ENTER_CRITICAL()

		if(uStartAddr&0x1)
		{
			//reg V558_REG_BIU_INCREMENT = 0,Addr not auto increate(only effect sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_INCREMENT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x0;
			MEM_HOLDCLK()

			//Set multi6 emory port addr
			*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
			MEM_HOLDCLK()
			uTemp = *(volatile UINT16 *)(MULTI16_ADDR_VAL);
			MEM_HOLDCLK()

			//reg V558_REG_BIU_INCREMENT = 0,Addr not auto increate(only effect sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_INCREMENT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x1;
			MEM_HOLDCLK()

			//Set multi6 emory port addr
			*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (uTemp&0xff00)|*pData;
			MEM_HOLDCLK()

			uSize--;
			pData++;

		}
		uLeft = (UINT8)(uSize & 0x1);
		uSize /= 2;

		//Set multi6 emory port addr
		*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
		MEM_HOLDCLK()
		while(uSize>0)
		{
			uTemp = *(pData+1);
			uTemp += (UINT16)(*pData)<<8;

			pData+=2;

			*(volatile UINT16*)(MULTI16_ADDR_VAL) = uTemp;
			MEM_HOLDCLK()

			uSize--;

		}
		if(uLeft)
		{
			//reg V558_REG_BIU_INCREMENT = 0,Addr not auto increate(only effect sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_INCREMENT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x0;
			MEM_HOLDCLK()

			//Set multi6 emory port addr
			*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
			MEM_HOLDCLK()
			uTemp = *(volatile UINT16 *)(MULTI16_ADDR_VAL);
			MEM_HOLDCLK()

			//reg V558_REG_BIU_INCREMENT = 0,Addr not auto increate(only effect sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_INCREMENT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x1;
			MEM_HOLDCLK()

			//Set multi6 emory port addr
			*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (uTemp&0xff)|(*pData<<8);
			MEM_HOLDCLK()
		}
		OS_EXIT_CRITICAL()
	}
	else
	{
		OS_ENTER_CRITICAL()

		if(uStartAddr&0x1)
		{
			//reg V558_REG_BIU_INCREMENT = 0,Addr not auto increate(only effect sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_INCREMENT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x0;
			MEM_HOLDCLK()

			//Set multi6 emory port addr
			*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;

			MEM_HOLDCLK()
			uTemp = *(volatile UINT16 *)(MULTI16_ADDR_VAL);
			MEM_HOLDCLK()

			//reg V558_REG_BIU_INCREMENT = 0,Addr not auto increate(only effect sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_INCREMENT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x1;
			MEM_HOLDCLK()

			//Set multi6 emory port addr
			*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (uTemp&0xff)|(*pData<<8);
			MEM_HOLDCLK()
			
			uSize--;
			pData++;
			
		}
		
		uLeft = (UINT8)(uSize & 0x1);
		uSize /= 2;

		*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
		MEM_HOLDCLK()
		while(uSize>0)
		{
			uTemp = *pData;
			pData++;
			
			uTemp += (UINT16)(*pData)<<8;
			pData++;
			
			*(volatile UINT16*)(MULTI16_ADDR_VAL) = uTemp;
			MEM_HOLDCLK()
			
			uSize--;
			

		}
		if(uLeft == 1)
		{
			//reg V558_REG_BIU_INCREMENT = 0,Addr not auto increate(only effect sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_INCREMENT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x0;
			MEM_HOLDCLK()

			//Set multi6 emory port addr
			*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
			MEM_HOLDCLK()
			uTemp = *(volatile UINT16 *)(MULTI16_ADDR_VAL);
			MEM_HOLDCLK()

			//reg V558_REG_BIU_INCREMENT = 0,Addr not auto increate(only effect sram)
			*(volatile UINT16 *)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_INCREMENT;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = 0x1;
			MEM_HOLDCLK()

			//Set multi6 emory port addr
			*(volatile UINT16*)(MULTI16_ADDR_ADDR) = (UINT16)V558_REG_BIU_MEM_FLG;
			MEM_HOLDCLK()
			*(volatile UINT16 *)(MULTI16_ADDR_VAL) = (uTemp&0xff00)|*pData;
			MEM_HOLDCLK()
		}
		OS_EXIT_CRITICAL()
		
	}
	
	
	if((uStartAddr>=0x08000) && (uStartAddr<0x20000))//Lcd 40k buffer or lbuf
	{
		V558_SetReg(V558_REG_BIU_MUL_CLR_AUTO, 0x0);
	}
}

#endif

//********************************************************************************
//Private functions
//********************************************************************************
/*
void DrvInit_558BUS(void)
{
#if V558_BUS_TYPE == V558_BUS_MULTI8
	DrvSetMultiSel(VMV_MULTI_SEL8);
#endif

#if V558_BUS_TYPE == V558_BUS_MULTI16
	DrvSetMultiSel(VMV_MULTI_SEL16);
#endif
	//Delay(50);	
}*/

void DrvSetMultiSel(VMV_MULTI_SEL sel)	//Set register 188c ,(multi8)
{

	*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_WORDH;
	*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)((((UINT16)V558_REG_BIU_SEL_8_16)&0xff00)>>8);
	*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_WORDL;
	*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)(((UINT16)V558_REG_BIU_SEL_8_16)&0xff);
	*(volatile UINT8 *)(MULTI8_ADDR_CTRL) = (UINT8)MULTI8_REG_PORT;
	*(volatile UINT8 *)(MULTI8_ADDR_DATA) = (UINT8)sel;
}

void USER558_Init558BUS(void)
{
#if V558_BUS_TYPE == V558_BUS_MULTI8
	DrvSetMultiSel(VMV_MULTI_SEL8);
#endif

#if V558_BUS_TYPE == V558_BUS_MULTI16
	DrvSetMultiSel(VMV_MULTI_SEL16);
#endif
	//Delay(50);	
}




typedef struct _VERSIONINFO {
    UINT8 ChipVersion[5];
    UINT8 HAVersion[17];
    UINT8 DRVVersion[16]; 
    UINT8 BusType[2];
    UINT8 SensorType[3];   
    UINT8 LCDType[3];           
} VERSIONINFO, *PVERSIONINFO;


#if 0
VERSIONINFO V558RDK_VERSION;
extern UINT8 const HAPI_VERSION[5];
extern UINT8 const DRIV_VERSION[5];

UINT8* USER558_GetRDKVersion(void) //length=44 bytes
{
	V558RDK_VERSION.ChipVersion[0]='V';
	V558RDK_VERSION.ChipVersion[1]='5';
	V558RDK_VERSION.ChipVersion[2]='5';
	V558RDK_VERSION.ChipVersion[3]='8';
	V558RDK_VERSION.ChipVersion[4]='-';	
	V558RDK_VERSION.HAVersion[0]='H';
	V558RDK_VERSION.HAVersion[1]='A';	
	V558RDK_VERSION.HAVersion[2]='P';	
	V558RDK_VERSION.HAVersion[3]='I';
	V558RDK_VERSION.HAVersion[4]=0x30+(HAPI_VERSION[0]/10);
	V558RDK_VERSION.HAVersion[5]=0x30+(HAPI_VERSION[0]%10);	
	V558RDK_VERSION.HAVersion[6]='-';
	V558RDK_VERSION.HAVersion[7]=0x30+(HAPI_VERSION[1]/10);
	V558RDK_VERSION.HAVersion[8]=0x30+(HAPI_VERSION[1]%10);	
	V558RDK_VERSION.HAVersion[9]='-';

	V558RDK_VERSION.HAVersion[10]=0x30+(HAPI_VERSION[2]/10);
	V558RDK_VERSION.HAVersion[11]=0x30+(HAPI_VERSION[2]%10);	
	V558RDK_VERSION.HAVersion[12]='-';
	V558RDK_VERSION.HAVersion[13]=0x30+(HAPI_VERSION[3]%10);	
	V558RDK_VERSION.HAVersion[14]='.';	
	V558RDK_VERSION.HAVersion[15]=0x30+(HAPI_VERSION[4]%10);
	V558RDK_VERSION.HAVersion[16]='-';		
	V558RDK_VERSION.DRVVersion[0]='D';
	V558RDK_VERSION.DRVVersion[1]='R';	
	V558RDK_VERSION.DRVVersion[2]='I';	
	V558RDK_VERSION.DRVVersion[3]='V';
	V558RDK_VERSION.DRVVersion[4]=0x30+(DRIV_VERSION[0]/10);
	V558RDK_VERSION.DRVVersion[5]=0x30+(DRIV_VERSION[0]%10);	
	V558RDK_VERSION.DRVVersion[6]='-';
	V558RDK_VERSION.DRVVersion[7]=0x30+(DRIV_VERSION[1]/10);
	V558RDK_VERSION.DRVVersion[8]=0x30+(DRIV_VERSION[1]%10);	
	V558RDK_VERSION.DRVVersion[9]='-';
	V558RDK_VERSION.DRVVersion[10]=0x30+(DRIV_VERSION[2]/10);
	V558RDK_VERSION.DRVVersion[11]=0x30+(DRIV_VERSION[2]%10);	
	V558RDK_VERSION.DRVVersion[12]='-';
	V558RDK_VERSION.DRVVersion[13]=0x30+(DRIV_VERSION[3]%10);	
	V558RDK_VERSION.DRVVersion[14]='.';	
	V558RDK_VERSION.DRVVersion[15]=0x30+(DRIV_VERSION[4]%10);

	V558RDK_VERSION.BusType[0]='-';		
	V558RDK_VERSION.BusType[1]=V558_BUS_TYPE+0x30;
	V558RDK_VERSION.SensorType[0]='-';		
	V558RDK_VERSION.SensorType[1]=0x30+((UINT8)SENSOR_TYPE/10);		
	V558RDK_VERSION.SensorType[2]=0x30+((UINT8)SENSOR_TYPE%10);	
	V558RDK_VERSION.LCDType[0]='-';			
	V558RDK_VERSION.LCDType[1]=0x30+((UINT8)LCD_TYPE/10);		
	V558RDK_VERSION.LCDType[2]=0x30+((UINT8)LCD_TYPE%10);		

	return (UINT8 *)&V558RDK_VERSION;
		
}
#endif
//HAPI VERSION: 04-03-30-1.0
//UINT8 const HAPI_VERSION[5]={04,03,31,1,0};

//DRIVE VERSION: 04-03-30-1.0
//UINT8 const DRIV_VERSION[5]={04,03,31,1,0};


#endif

