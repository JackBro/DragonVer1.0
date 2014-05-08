/*
 *  VC0938RDK/vhostctrl.c
 *
 *  Copyright (C) 2004 Vimicro Corporation
 *  
 *  Written by TangMian ...
 *
 *  Overview:
 *   This file is used for porting 938 Driver, and will be modified by User.
 *
 *   
 *
 *	2004,9,30:
 *		Release. By TangMian
 *
 *	2004,10,12:
 *		Add VPlr_Init() function. By TangMian
 *
 *	2004,10,14:
 *		Delete VPlr_Delay(..) function. By TangMian
 *
 *	2004,10,19:
 *		Fix bug in VPlr_ReadFile(..) function. By TangMian 
 *
 *	2004,10,21:
 *		Add Serial3 or 4 function.
 *
 *	2004,11,3
 *		Update code.
 *  2004,12,24
 *      Add code for PC player,By Andy,liu
 *
 *	2004,12,28
 *		Add 3M,3.5M, 4MHz input clock support in Vreg_SetPLL() function. by gzd.
 *
 */

#include "vdef.h"
#include "vdev.h"
#include "vdevdef.h"
#include "vHostCtrl.h"
#include "vplayer.h"



#if 0
/********* For RAMDisk **********/
UINT32	file_size = 152136;
UINT32	file_offset = 0;
UINT8*	file_ptr;
/********* For RAMDisk **********/
#endif
#if __PC_PLATFORM__

#include "stdio.h"
#include "..\VMDSim\VC09x8UserApi.h"

#endif

#if(FILESTREAM_MODE == FILESTREAM_ASYNC)

UINT8	bBufferFlag[BUFFER_EMPTY_TIME];
UINT32	bBufferNum[BUFFER_EMPTY_TIME];
UINT8	bWBufferFlag;
UINT8	bRBufferFlag;
UINT8	bBufferEnd;	//0: Not End; 1: End
UINT8	bBufferStart;	//0: Not Start; 1: Start

#endif

/***************************************************************
Description:
		Maybe User want to do something before playing music.
		This function will be called in VPlr_Initialize() function.

Parameters:
		void.

Returns:
		void.		

****************************************************************/
void VPlr_Init(void)
{
	;
}

/***************************************************************
Description:
		Reset 938 Chip. Maybe by using GPIO.

Parameters:
		void.

Returns:
		void.		

****************************************************************/
void VPlr_ResetChip(void)
{
//	rPDATD &= 0xfe;
//	Vreg_Delay(20);
//	rPDATD |= 0x01;
}


/***************************************************************
Description:
		Lock 938 Hardware

Parameters:
		void.

Returns:
		void.

****************************************************************/
void VPlr_Lock(void)
{
	;
}

/***************************************************************
Description:
		Unlock 938 Hardware

Parameters:
		void.

Returns:
		void.		

****************************************************************/
void VPlr_Unlock(void)
{
	;
}


/***************************************************************
Description:
		Enter Critical Section

Parameters:
		void.

Returns:
		void.

****************************************************************/
void VPlr_EnterCriticalSection(void)
{
	;
}


/***************************************************************
Description:
		Leave Critical Section

Parameters:
		void.

Returns:
		void.

****************************************************************/
void VPlr_LeaveCriticalSection(void)
{
	;
}

/***************************************************************
Description:
		Read File based on customer's file system.

Parameters:
		pFile_Ptr:  File Handler
		pbBuffer:   data buffer
		pFile_Size: number of bytes to be read	

Returns:
		>=0: Number of bytes be read, <0: Error

****************************************************************/
SINT32 VPlr_ReadFile(void* pFile_Ptr, UINT8* pbBuffer, UINT32 dFile_Size)
{
	SINT32	err = 0;
#if	__PC_PLATFORM__
	err = fread((void*)pbBuffer, (size_t)dFile_Size, 1, (FILE*)pFile_Ptr);
	if (err <= 0)
	{
		err = -1;
		return err;
	}
	else
	{
		return dFile_Size;
	}
#endif

#if 0	//For RAMDisk
	UINT32	dNumber;
	UINT8*	pbPtr;

	if((dFile_Size + file_offset) > file_size)
		dNumber = file_size - file_offset;
	else
		dNumber = dFile_Size;
	pbPtr = file_ptr + file_offset;
	Vmd_MemCpy(pbBuffer, pbPtr, dNumber);
	file_offset += dNumber;
	err = dNumber;

#endif
	return err;

}


/***************************************************************
Description:
		Seek File based on customer's file system.

Parameters:
		pFile_Ptr:  File Handler
		dOffset:	   File offset

Returns:
		0: Success, <0: Error

****************************************************************/
SINT32 VPlr_SeekFile(void* pFile_Ptr, UINT32 dOffset)
{
	SINT32	err = 0;
#if	__PC_PLATFORM__
	err = fseek((FILE*)pFile_Ptr, (int)dOffset, SEEK_SET);
	if(err != 0)
		err = -1;
#endif

#if 0	//For RAMDisk
	if (file_offset <= file_size)
		file_offset = dOffset;
	else
		err = -1;
#endif
	
	return err;
}


/***************************************************************
Description:
		Main interrupt service rountine.

Parameters:
		void.


Returns:
		void.		

****************************************************************/
void	VPlat_MainIsr(void)
{
	if (VPlr_GetDeviceState() == WORKING)
		Vdev_MainIsr();
}
#if(FILESTREAM_MODE == FILESTREAM_ASYNC)

void VPlr_InitGetStream(void)
{
	UINT8 i;

	for(i=0; i<BUFFER_EMPTY_TIME; i++)
	{
		bBufferFlag[i] = 1;
		bBufferNum[i] = (UINT32)(SECOND_BUFFER_SIZE/BUFFER_EMPTY_TIME);
	}
	bWBufferFlag = 1;
	bRBufferFlag = 1;
	bBufferEnd = 0;
	bBufferStart = 0;
}

/***************************************************************
Description:
		In Async FileStream mode, Set Start Flag

Parameters:
		void.


Returns:
		void.		

****************************************************************/
void VPlr_StartStream(void)
{
	bBufferStart = 1;
}

/***************************************************************
Description:
		In Async FileStream mode, Clear Start Flag

Parameters:
		void.


Returns:
		void.		

****************************************************************/
void VPlr_StopStream(void)
{
	bBufferStart = 0;
}

/***************************************************************
Description:
		In Async FileStream mode, Get Start Flag

Parameters:
		void.


Returns:
		void.		

****************************************************************/
UINT8 VPlr_GetStartStream(void)
{
	return bBufferStart;
}

/********************************************************************
 Description:
 		In Async FileStream mode, Check Write-Buffer.

 Parameters:
 		NONE.
	
 Returns:
 		flag: Which buffer
*********************************************************************/
UINT8 VPlr_CheckWBuffer(void)
{
	UINT8 flag = 0;

	if (bBufferFlag[bWBufferFlag -1] != 0)
	{
		flag = bWBufferFlag;
		bBufferFlag[bWBufferFlag - 1] = 0;
		if(bWBufferFlag == BUFFER_EMPTY_TIME)
			bWBufferFlag = 1;
		else
			bWBufferFlag ++;

	}

	return flag;
}

/********************************************************************
 Description:
 		In Async FileStream mode, Check Read-Buffer.

 Parameters:
 		NONE.
	
 Returns:
 		flag: Which buffer
*********************************************************************/
UINT8 VPlr_CheckRBuffer(void)
{
	UINT8 flag = 0;

	if (bBufferEnd == 1)
		goto retur;
	if (bBufferFlag[bRBufferFlag -1] != 1)
	{
		flag = bRBufferFlag;
		bBufferFlag[bRBufferFlag - 1] = 1;
		if(bRBufferFlag == BUFFER_EMPTY_TIME)
			bRBufferFlag = 1;
		else
			bRBufferFlag ++;
	}
retur:
	return flag;
}


/********************************************************************
 Description:
 		In Async FileStream mode, Set every Buffer size

 Parameters:
 		bNum:	Which Buffer
		dSize:	Read Size
	
 Returns:
 		NONE
*********************************************************************/
void VPlr_SetWBufferSize(UINT8 bNum, UINT32 dSize)
{
	bBufferNum[bNum] = dSize;
}


/********************************************************************
 Description:
 		In Async FileStream mode, Get every Buffer size

 Parameters:
 		bNum: Which Buffer
	
 Returns:
 		Buffer size
*********************************************************************/
UINT32 VPlr_GetWBufferSize(UINT8 bNum)
{
	return bBufferNum[bNum];
}

#endif

/********************************************************************
 Description:
 		Delay function.

 Parameters:
 		dTimer:	Delay count value.
	
 Returns:
 		VOID.
 		
 ReMarks:
 		this is an example of delay function. There are different methods
 		to realize the function on each mobile platform. So Please
 		reference the methods of the function on different platform.
*********************************************************************/
void	Vreg_Delay(UINT32 dTimer)
{
	UINT32	i = 0, j = 0, dwCount = 1000;

	for (i=0; i<dTimer; i++)
	{
		for(j=0; j<dwCount; j++) 
		{
			__asm{
				nop;
			}
		}
	}
}


#if(ACCESS_MODE == ACCESS_SPI)

	//For SPI

#endif


#if((ACCESS_MODE == ACCESS_SERIAL3)||(ACCESS_MODE == ACCESS_SERIAL4))

void SP_WriteBYTE(UINT8 bData)
{
	UINT8 bIndex;
	for(bIndex=7; bIndex>=0; bIndex--)
	{
		SPWBIT(dData, bIndex);
		SPCLCK_UP_DOWN;
	}
}
void SP_READBYTE(UINT8* pbData)
{
	UINT8 bIndex;
	*pbData = 0;
	for(bIndex=7; bIndex>=0; bIndex--)
	{
		SPCLCK_UP_DOWN;
		SPRBIT(pbData, bIndex);
	}
}

void SP_WriteRegister(UINT8 bAddr, UINT8 bValue)
{
	SP_WriteBYTE(bAddr);
	SP_WriteBYTE(bValue);
	__asm{
		nop;
		nop;
	}
	SPSYNC_UP_DOWN;
	__asm{
		nop;
		nop;
		nop;
	}
}

void SP_ReadRegister(UINT8 bAddr, UINT8* bValue)
{

	SP_WriteRegister(0x67,bAddr);
	__asm{
		nop;
		nop;
		nop;
	}
#if(ACCESS_MODE==ACCESS_SERIAL3)
	SERIAL_IN;
#endif
	__asm{
		nop;
		nop;
		nop;
		nop;
	}

	SP_READBYTE(bValue);
	__asm{
		nop;
		nop;
	}
	SPSYNC_UP_DOWN;
	__asm{
		nop;
		nop;
	}
#if(ACCESS_MODE==ACCESS_SERIAL3)
	SERIAL_OUT;
#endif
	__asm{
		nop;
		nop;
	}
}

#endif



/********************************************************************
 Description:
		Read a global register of the chip.

 Parameters:
		bAddr:	Register address.
		
 Returns:
		the value of the special register.		

*********************************************************************/
UINT8	Vreg_ReadReg(UINT8 bAddr)
{
#if	__PC_PLATFORM__
	
	return V9Sim_ReadGlbReg(bAddr);
#else
	UINT8 bData;

#if(ACCESS_MODE == ACCESS_PARELLELBUS)
	rVmdReg = bAddr;
	bData = rVmdData;
#elif((ACCESS_MODE == ACCESS_SERIAL3)||(ACCESS_MODE == ACCESS_SERIAL4))
	SP_ReadRegister(bAddr, &bData);
#elif(ACCESS_MODE == ACCESS_SPI)
	//For SPI
#endif

	return bData;
#endif
}

/********************************************************************
 Description:
		Write a global register of the chip.

 Parameters:
		bAddr:	Register address.
		bVal:	the Value which is written to the special register.
		
 Returns:
		void.		

********************************************************************/
void	Vreg_WriteReg(UINT8 bAddr, UINT8 bVal)
{
#if	__PC_PLATFORM__
	V9Sim_WriteGlbReg(bAddr, bVal);
#else
#if(ACCESS_MODE == ACCESS_PARELLELBUS)
	rVmdReg = bAddr;
	rVmdData = bVal;
#elif((ACCESS_MODE == ACCESS_SERIAL3)||(ACCESS_MODE == ACCESS_SERIAL4))
	SP_WriteRegister(bAddr, bVal);
#elif(ACCESS_MODE == ACCESS_SPI)
	//For SPI
#endif
#endif	
}

/********************************************************************
 Description:
		Write multiple register of the chip.
		Use this functions write stream Fifos and Event Fifo

 Parameters:
		bAddr:	Register address.
		pData:	Pointer to the source data buffer.
		dSize:	The size of source data.
		
 Returns:
		void.		

********************************************************************/
void	Vreg_WriteMulRegs(UINT8 bAddr, PUINT8 pbData, UINT16 wSize)
{
#if	__PC_PLATFORM__
	
	switch (bAddr){
		
	case VDEV_REG_EFIFO:
		V9Sim_WriteEventFIFO(pbData, wSize);
		break;
		
	case VDEV_REG_SFIFO:
		V9Sim_WriteStreamBuf(pbData, wSize);
		break;
		
	case VDEV_REG_CFIFO:
		V9Sim_WriteCmdFIFO(pbData, wSize);
		break;
	case VDEV_REG_MP3_FIFO:
		V9Sim_WriteMP3FIFO(pbData, wSize);
		break;
	case VDEV_REG_V8051RAM:
		V9Sim_WriteUDIHeader(pbData, wSize);
		break;

	default:
		break;
	}
#else
	UINT8*	pbPtr=pbData;

#if(ACCESS_MODE == ACCESS_PARELLELBUS)
	rVmdReg = bAddr;
	while(wSize--)
	{
		rVmdData = *(pbPtr++);
	}
#elif((ACCESS_MODE == ACCESS_SERIAL3)||(ACCESS_MODE == ACCESS_SERIAL4))
	while(dwDataLen--)
	{
		VPlat_WriteReg(bAddr, *(pbPtr++));
	}
#elif(ACCESS_MODE == ACCESS_SPI)
	//For SPI
#endif
#endif
}

/********************************************************************
 Description:
 		Setting the chip PLL to run.
 
 Parameters:
 		dInFreq:	the input frequency value of Chip CLK pin. 
 					The range is from 3MHz to 36MHz.
 		
 Returns:
 		VOID.		
 
 Remarks:
 		
********************************************************************/
void	Vreg_SetPLL(void)
{
	UINT32 dInFreq = VREG_INPUT_CLK_FREQ;
	
	switch (dInFreq)
	{
	case 3000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x1E);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 3500000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x1A);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 4000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x16);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 5000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x12);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 6000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0E);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 7000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0C);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 8000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0A);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 9000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x09);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 10000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x08);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 11000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x07);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 12000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x41);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0E);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 13000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x41);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0D);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 14000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x42);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0C);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 15000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x43);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0B);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 16000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x21);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 17000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x21);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0F);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 18000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x21);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0E);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 19000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x21);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0D);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 20000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x22);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x12);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 21000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x22);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x11);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 22000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x21);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0B);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 23000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x21);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0B);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 24000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x21);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0A);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 25000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x23);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x12);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 26000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x23);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x11);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 27000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x23);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x10);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 28000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x23);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0F);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 29000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x23);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0F);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 30000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x21);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x08);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 31000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x23);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0E);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 32000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x23);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0D);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 33000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x23);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0D);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 34000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x23);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0C);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 35000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x23);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x0C);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	case 36000000:
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL1, 0x21);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL2, 0x06);
		Vreg_WriteReg((UINT8)VDEV_REG_CLKCTRL3, 0x11);
		break;
	}
#if(AUDIO_CHIP==CHIP_VC0937)
	Vreg_WriteReg(0x63, 0x33);
#endif

}

