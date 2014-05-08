#include <string.h>
#include <stdlib.h>
#include "..\inc\def.h"
#include "..\inc\44b.h"
#include "..\inc\44blib.h"
#include "..\inc\serial.h"
#include "..\inc\timer.h"
#include "..\inc\test.h"

#include "..\..\driver\vdef.h"
#include "..\..\driver\vdevdef.h"
#include "..\..\driver\vdev.h"
#include "..\..\driver\vhighapi.h"
#include "..\..\driver\vhostctrl.h"


extern UINT8  uImageBuffer[DATA_FILE_LEN];
extern UINT32 iBufferImageEndPointer;

extern UINT8 uEventBuf[EVENT_FILE_LEN];
extern UINT32 uPtEventCur;
extern UINT32 uPtEventEnd;

extern UINT8 uStream0Buf[STREAM0_FILE_LEN];
extern UINT32 uPtStream0Cur;
extern UINT32 uPtStream0End;

extern UINT8 uStream1Buf[STREAM1_FILE_LEN];
extern UINT32 uPtStream1Cur;
extern UINT32 uPtStream1End;

extern UINT8 uStream2Buf[STREAM2_FILE_LEN];
extern UINT32 uPtStream2Cur;
extern UINT32 uPtStream2End;

extern UINT8 uStream3Buf[STREAM3_FILE_LEN];
extern UINT32 uPtStream3Cur;
extern UINT32 uPtStream3End;

extern UINT8 uCommandBuf[COMMAND_FILE_LEN];
extern UINT32 uPtCommandCur;
extern UINT32 uPtCommandEnd;

extern OBW g_OBW;
extern RSW g_RSW;

unsigned long  g_dwEventFifoEMPLen = 512;
unsigned long  g_dwStm0FifoEMPLen =  3776>>1;
unsigned long  g_dwStm1FifoEMPLen =  3776>>1;
unsigned long  g_dwStm2FifoEMPLen =  3776>>1;
unsigned long  g_dwStm3FifoEMPLen =  3776>>1;
unsigned long  g_dwMp3FifoEMPLen =    1760>>1;

unsigned char  g_uEventBufExist = 0;
unsigned char  g_uStm0BufExist  = 0;
unsigned char  g_uStm1BufExist  = 0;
unsigned char  g_uStm2BufExist  = 0;
unsigned char  g_uStm3BufExist  = 0;
unsigned char  g_uCommBufExist  = 0;

unsigned char  g_uAesKey[4][16];  //Aes ECB or CBC mode decrypted Key
unsigned char  g_uInitVec[4][16]; //InitVector if CBC mode encrypted
unsigned char  g_bECBorCBC = 0;   // 0x00, no encrypted; 
								  // 0x20, TYPE_PLAY_MODE_ENCRYPT_CBC
								  // 0x40, TYPE_PLAY_MODE_ENCRYPT_CBC

unsigned char  g_bPollingPlay = 0;
unsigned char  g_bPlayEvent = 0;

/*
iChnNum :    total num of the preload and stream files
wBaseAddr:   each channel's baseaddress
wEndPtr:     each channel's end point
wWaterMark:  each channel's water mark
*/
typedef struct
{
  UINT8    uChnNum;           
  UINT16   wBaseAddr[8];      
  UINT16   wEndPtr[8];
  UINT8    uWaterMark[8];
}REGINFO;

REGINFO   g_regInfo = {0};

//
//Descriptions:
//         Play back and stop real Midi functions.
//
UINT8      g_RealMidiEnd = 0;
UINT8      g_bRealPlay = 0;
UINT32	   g_dFuncID;
UINT32	   g_dFuncID1;
UINT32	   g_dFuncID2;
UINT32	   g_dFuncID3;
UINT32	   g_dFuncID4;

void EOS_CLFunc(void)
{
	g_RealMidiEnd = 1;
}

void VC938_RealPlay(void)
{
	KVRESULT kVresult;
	
	kVresult = VPlr_Initialize();
	if (kVresult != RESULT_SUCCESS)
		return;
	
	kVresult = VPlr_Create();
	if (kVresult != RESULT_SUCCESS)
		return;
	
	kVresult = VPlr_Open();
	if (kVresult != RESULT_SUCCESS)
		return;

	VPlr_SetSPVol(0x0b);
//	VPlr_SetMidiVol(0xc0, 0xc0);
//	VPlr_SetWaveVol(0xc0, 0xc0);
	
	kVresult = VPlr_Load(uEventBuf, uPtEventEnd, 0, &g_dFuncID);
	if (kVresult != RESULT_SUCCESS)
		return;
	
	kVresult = VPlr_Play(g_dFuncID, 0, 0, EOS_CLFunc);
	if (kVresult != RESULT_SUCCESS)
		return;

	if (uPtStream0End)
	{
		kVresult = VPlr_Load(uStream0Buf, uPtStream0End, 0, &g_dFuncID1);
		if (kVresult == RESULT_SUCCESS)
		{
			kVresult = VPlr_Play(g_dFuncID1, 0, 20, EOS_CLFunc);
		}
	}
	
	if (uPtStream1End)
	{
		kVresult = VPlr_Load(uStream1Buf, uPtStream1End, 0, &g_dFuncID2);
		if (kVresult == RESULT_SUCCESS)
		{
			kVresult = VPlr_Play(g_dFuncID2, 0, 20, EOS_CLFunc);
		}
	}

	if (uPtStream2End)
	{
		kVresult = VPlr_Load(uStream2Buf, uPtStream2End, 0, &g_dFuncID3);
		if (kVresult == RESULT_SUCCESS)
		{
			kVresult = VPlr_Play(g_dFuncID3, 0, 20, EOS_CLFunc);
		}
	}

	if (uPtStream3End)
	{
		kVresult = VPlr_Load(uStream3Buf, uPtStream3End, 0, &g_dFuncID4);
		if (kVresult == RESULT_SUCCESS)
		{
			kVresult = VPlr_Play(g_dFuncID4, 0, 20, EOS_CLFunc);
		}
	}

	g_bRealPlay = 1;
	g_RealMidiEnd = 0;
}

void VC938_RealStop(void)
{
	KVRESULT kVresult;
	
	kVresult = VPlr_Stop(g_dFuncID);
	if (kVresult != RESULT_SUCCESS)
		return;
	
	kVresult = VPlr_Unload(g_dFuncID);
	if (kVresult != RESULT_SUCCESS)
		return;

	if (g_dFuncID1 > 0)
	{
		VPlr_Stop(g_dFuncID1);
		VPlr_Unload(g_dFuncID1);
	}
	if (g_dFuncID2 > 0)
	{
		VPlr_Stop(g_dFuncID2);
		VPlr_Unload(g_dFuncID2);
	}
	if (g_dFuncID3 > 0)
	{
		VPlr_Stop(g_dFuncID3);
		VPlr_Unload(g_dFuncID3);
	}
	if (g_dFuncID4 > 0)
	{
		VPlr_Stop(g_dFuncID4);
		VPlr_Unload(g_dFuncID4);
	}
	kVresult = VPlr_Close();
	if (kVresult != RESULT_SUCCESS)
		return;
	
	kVresult = VPlr_Destory();
	if (kVresult != RESULT_SUCCESS)
		return;
	
	g_RealMidiEnd = 0;
	g_bRealPlay = 0;
}

void VC938_Play(void)
{
	KVRESULT kVresult;
	
	kVresult = VPlr_Play(g_dFuncID, 0, 0, EOS_CLFunc);
	if (kVresult != RESULT_SUCCESS)
		return;

	g_bRealPlay = 1;
	g_RealMidiEnd = 0;
}

void VC938_Stop(void)
{
	KVRESULT kVresult;
	
	kVresult = VPlr_Stop(g_dFuncID);
	if (kVresult != RESULT_SUCCESS)
		return;
	
	g_RealMidiEnd = 0;
	g_bRealPlay = 0;
}

//////////////Get Music Data/////////////////////////////////////////////
void GetEventData(UINT8* pDataBuffer,UINT32 *dwDataSize)
{
	UINT32 dwReadData,i;
	if(pDataBuffer == NULL)
	{
		*dwDataSize = 0;
		return;
	}
	
	dwReadData = *dwDataSize;
	*dwDataSize = 0;
	for(i=0 ;i< dwReadData; i++)
	{
		if(uPtEventCur >= uPtEventEnd)	
		{
			Vdev_MaskInt(VDEV_INT_EFIFOEMPTY);
			break;
		}
		*(pDataBuffer+i) = uEventBuf[uPtEventCur];
		uPtEventCur++;
	}
	*dwDataSize = i;
}

void GetStream0Data(UINT8* pDataBuffer,UINT32 *dwDataSize)
{
	UINT32 dwReadData,i;
	if(pDataBuffer == NULL)
	{
		*dwDataSize = 0;
		return;
	}

	dwReadData = *dwDataSize;
	*dwDataSize = 0;

	for(i=0 ;i< dwReadData; i++)
	{
		if(uPtStream0Cur >= uPtStream0End)	
		{
			Vdev_MaskInt(VDEV_INT_SFIFO0EMPTY);
			Vdev_MaskInt(VDEV_INT_MFIFOEMPTY);
			break;
		}
		*(pDataBuffer+i) = uStream0Buf[uPtStream0Cur];
		uPtStream0Cur++;
	}
	*dwDataSize = i;
}

void GetStream1Data(UINT8* pDataBuffer,UINT32 *dwDataSize)
{
	UINT32 dwReadData,i;
	if(pDataBuffer == NULL)
	{
		*dwDataSize = 0;
		return;
	}

	dwReadData = *dwDataSize;
	*dwDataSize = 0;
	for(i=0 ;i< dwReadData; i++)
	{
		if(uPtStream1Cur >= uPtStream1End)	
		{
			Vdev_MaskInt(VDEV_INT_SFIFO1EMPTY);
			break;
		}
		*(pDataBuffer+i) = uStream1Buf[uPtStream1Cur];
		uPtStream1Cur++;
	}
	*dwDataSize = i;
}

void GetStream2Data(UINT8* pDataBuffer,UINT32 *dwDataSize)
{
	UINT32 dwReadData,i;
	if(pDataBuffer == NULL)
	{
		*dwDataSize = 0;
		return;
	}

	dwReadData = *dwDataSize;
	*dwDataSize = 0;
	for(i=0 ;i< dwReadData; i++)
	{
		if(uPtStream2Cur >= uPtStream2End)	
		{
			Vdev_MaskInt(VDEV_INT_SFIFO2EMPTY);
			break;
		}
		*(pDataBuffer+i) = uStream2Buf[uPtStream2Cur];
		uPtStream2Cur++;
	}
	*dwDataSize = i;
}

void GetStream3Data(UINT8* pDataBuffer,UINT32 *dwDataSize)
{
	UINT32 dwReadData,i;
	if(pDataBuffer == NULL)
	{
		*dwDataSize = 0;
		return;
	}

	dwReadData = *dwDataSize;
	*dwDataSize = 0;
	for(i=0 ;i< dwReadData; i++)
	{
		if(uPtStream3Cur >= uPtStream3End)	
		{
			Vdev_MaskInt(VDEV_INT_SFIFO3EMPTY);
			break;
		}
		*(pDataBuffer+i) = uStream3Buf[uPtStream3Cur];
		uPtStream3Cur++;
	}
	*dwDataSize = i;
}

void GetCommandData(UINT8* pDataBuffer,UINT32 *dwDataSize)
{
	UINT32 dwReadData,i;
	if(pDataBuffer == NULL)
	{
		*dwDataSize = 0;
		return;
	}

	dwReadData = *dwDataSize;
	*dwDataSize = 0;
	for(i=0 ;i< dwReadData; i++)
	{
		if(uPtCommandCur >= uPtCommandEnd)	
		{
			break;
		}
		*(pDataBuffer+i) = uCommandBuf[uPtCommandCur];
		uPtCommandCur++;
	}
	*dwDataSize = i;
}

///////////////////Test play music start routines///////////////////////////
void TestPlayEvent()
{
	UINT8 uRegVal;

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(VDEV_REG_SWRST1, (UINT8)0x48);	//Event fifo reset,Sequencer reset
	
	TestEventInt();
	uRegVal = Vreg_ReadReg(VDEV_REG_GLBCTRL);
	uRegVal |= 0x80;
	Vreg_WriteReg(VDEV_REG_GLBCTRL, uRegVal);//Sequencer start
	Vdev_UnMaskInt(VDEV_INT_EFIFOEMPTY);
	g_bPlayEvent = 1;
}

void TestPlayStream(UINT8 uType)
{
    int i;
	unsigned long dwReadDataSize;

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(VDEV_REG_SWRST1, 0x20);
	Vreg_WriteReg(VDEV_REG_INTFLAG0, 0xF0);

	//set stream data Base Address and EndPointor. unit is sample number.
	//and set each stream water mark. 
	Vreg_WriteReg(VDEV_REG_SFIFO0_AW_BL, (UINT8)g_regInfo.wBaseAddr[0]);
	Vreg_WriteReg(VDEV_REG_SFIFO0_AW_BH, (UINT8)(g_regInfo.wBaseAddr[0] >> 8));
	Vreg_WriteReg(VDEV_REG_SFIFO0_AW_EL, (UINT8)g_regInfo.wEndPtr[0]);
	Vreg_WriteReg(VDEV_REG_SFIFO0_AW_EH, (UINT8)(g_regInfo.wEndPtr[0] >> 8));
	Vreg_WriteReg(VDEV_REG_SFIFO0_WM, g_regInfo.uWaterMark[0]);
	Vreg_WriteReg(VDEV_REG_SFIFO1_AW_BL, (UINT8)g_regInfo.wBaseAddr[1]);
	Vreg_WriteReg(VDEV_REG_SFIFO1_AW_BH, (UINT8)(g_regInfo.wBaseAddr[1] >> 8));
	Vreg_WriteReg(VDEV_REG_SFIFO1_AW_EL, (UINT8)g_regInfo.wEndPtr[1]);
	Vreg_WriteReg(VDEV_REG_SFIFO1_AW_EH, (UINT8)(g_regInfo.wEndPtr[1] >> 8));
	Vreg_WriteReg(VDEV_REG_SFIFO1_WM, (UINT8)g_regInfo.uWaterMark[1]);
	Vreg_WriteReg(VDEV_REG_SFIFO2_AW_BL, (UINT8)g_regInfo.wBaseAddr[2]);
	Vreg_WriteReg(VDEV_REG_SFIFO2_AW_BH, (UINT8)(g_regInfo.wBaseAddr[2] >> 8));
	Vreg_WriteReg(VDEV_REG_SFIFO2_AW_EL, (UINT8)g_regInfo.wEndPtr[2]);
	Vreg_WriteReg(VDEV_REG_SFIFO2_AW_EH, (UINT8)(g_regInfo.wEndPtr[2] >> 8));
	Vreg_WriteReg(VDEV_REG_SFIFO2_WM, (UINT8)g_regInfo.uWaterMark[2]);
	Vreg_WriteReg(VDEV_REG_SFIFO3_AW_BL, (UINT8)g_regInfo.wBaseAddr[3]);
	Vreg_WriteReg(VDEV_REG_SFIFO3_AW_BH, (UINT8)(g_regInfo.wBaseAddr[3] >> 8));
	Vreg_WriteReg(VDEV_REG_SFIFO3_AW_EL, (UINT8)g_regInfo.wEndPtr[3]);
	Vreg_WriteReg(VDEV_REG_SFIFO3_AW_EH, (UINT8)(g_regInfo.wEndPtr[3] >> 8));
	Vreg_WriteReg(VDEV_REG_SFIFO3_WM, (UINT8)g_regInfo.uWaterMark[3]);
	Vreg_WriteReg(VDEV_REG_LOADADDR,0x01);

	//config encrypted mode if you have select encrypt mode ECB or CBC.
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
	if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB)			//ECB encrypted mode
	{
		Vreg_WriteReg(VDEV_REG_AES_CORE_CMD, 0x22);
		for (i=0; i<16; i++)
		{
			Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
		}
	}
	else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)		//CBC encrypted mode
	{
		Vreg_WriteReg(VDEV_REG_AES_CORE_CMD, 0x23);
		for (i=0; i<16; i++)
		{
			Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
		}
		Vreg_Delay(1);
		for (i=0; i<16; i++)
		{
			Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[0][i]);
		}
	}
	Vreg_Delay(1);
	
	switch(uType)
	{
	case TYPE_PLAY_MIDI_WAVE_1CH:
		dwReadDataSize= 0x760<<2;
		GetStream0Data(uImageBuffer,&dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x10);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
	        Vreg_Delay(1);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
			if (dwReadDataSize > 16 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
			{
				for (i=0; i<16; i++)
					g_uInitVec[0][i] = uImageBuffer[dwReadDataSize-16+i];
			}
		}
		Vdev_UnMaskInt(VDEV_INT_SFIFO0EMPTY);	//Enable stream.
		break;

	case TYPE_PLAY_MIDI_WAVE_2CH:
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_LOADADDR,0x03);
		dwReadDataSize= 0x760<<2;
		GetStream0Data(uImageBuffer,&dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x10);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
	        Vreg_Delay(1);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
			if (dwReadDataSize > 16 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
			{
				for (i=0; i<16; i++)
					g_uInitVec[0][i] = uImageBuffer[dwReadDataSize-16+i];
			}
		}

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB)			//ECB encrypted mode
		{
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[1][i]);
			}
		}
		else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)		//CBC encrypted mode
		{
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[1][i]);
			}
			Vreg_Delay(1);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[1][i]);
			}
		}
		Vreg_Delay(1);
		dwReadDataSize= 0x760<<2;
		GetStream1Data(uImageBuffer,&dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x20);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
	        Vreg_Delay(1);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
			if (dwReadDataSize > 16 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
			{
				for (i=0; i<16; i++)
					g_uInitVec[1][i] = uImageBuffer[dwReadDataSize-16+i];
			}
		}
		Vdev_UnMaskInt(VDEV_INT_SFIFO0EMPTY);	//Enable stream.
		Vdev_UnMaskInt(VDEV_INT_SFIFO1EMPTY);	//Enable stream.
		break;

	case TYPE_PLAY_MIDI_WAVE_3CH:
		break;

	case TYPE_PLAY_MIDI_WAVE_4CH:
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_LOADADDR,0x0F);
		dwReadDataSize= 0x760<<1;
		GetStream0Data(uImageBuffer,&dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x10);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
	        Vreg_Delay(1);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
			if (dwReadDataSize > 16 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
			{
				for (i=0; i<16; i++)
					g_uInitVec[0][i] = uImageBuffer[dwReadDataSize-16+i];
			}
		}

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB)			//ECB encrypted mode
		{
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[1][i]);
			}
		}
		else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)		//CBC encrypted mode
		{
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[1][i]);
			}
			Vreg_Delay(1);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[1][i]);
			}
		}
		Vreg_Delay(1);
		dwReadDataSize= 0x760<<1;
		GetStream1Data(uImageBuffer,&dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x20);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
	        Vreg_Delay(1);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
			if (dwReadDataSize > 16 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
			{
				for (i=0; i<16; i++)
					g_uInitVec[1][i] = uImageBuffer[dwReadDataSize-16+i];
			}
		}

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB)			//ECB encrypted mode
		{
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[2][i]);
			}
		}
		else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)		//CBC encrypted mode
		{
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[2][i]);
			}
			Vreg_Delay(1);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[2][i]);
			}
		}
		Vreg_Delay(1);
		dwReadDataSize= 0x760<<1;
		GetStream2Data(uImageBuffer,&dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x40);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
	        Vreg_Delay(1);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
			if (dwReadDataSize > 16 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
			{
				for (i=0; i<16; i++)
					g_uInitVec[2][i] = uImageBuffer[dwReadDataSize-16+i];
			}
		}
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB)			//ECB encrypted mode
		{
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[3][i]);
			}
		}
		else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)		//CBC encrypted mode
		{
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[3][i]);
			}
			Vreg_Delay(1);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[3][i]);
			}
		}
		Vreg_Delay(1);
		dwReadDataSize= 0x760<<1;
		GetStream3Data(uImageBuffer,&dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x80);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
	        Vreg_Delay(1);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
			if (dwReadDataSize > 16 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
			{
				for (i=0; i<16; i++)
					g_uInitVec[3][i] = uImageBuffer[dwReadDataSize-16+i];
			}
		}
		Vdev_UnMaskInt(VDEV_INT_SFIFO0EMPTY);	//Enable stream.
		Vdev_UnMaskInt(VDEV_INT_SFIFO1EMPTY);	//Enable stream.
		Vdev_UnMaskInt(VDEV_INT_SFIFO2EMPTY);	//Enable stream.
		Vdev_UnMaskInt(VDEV_INT_SFIFO3EMPTY);	//Enable stream.
		break;
	default:
		return;
	}

	TestPlayEvent();
}

void TestPlayCommand()
{
	UINT32 dwReadDataSize = 64;
	unsigned char uRegVal;

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(VDEV_REG_SWRST2, 0x04);	//Command fifo reset.

	uRegVal = Vreg_ReadReg(VDEV_REG_GLBCTRL);
	uRegVal |= 0x80;
	Vreg_WriteReg(VDEV_REG_GLBCTRL, uRegVal);//Sequencer start

	while(dwReadDataSize>0)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		uRegVal = Vreg_ReadReg(VDEV_REG_BUFFLAG0);
		if (uRegVal & 0x04)
		{
			GetCommandData(uImageBuffer, &dwReadDataSize);
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			Vreg_WriteMulRegs(VDEV_REG_CFIFO, uImageBuffer, dwReadDataSize);
		}
		if (dwReadDataSize > 0)
			dwReadDataSize = 64;
	}
}

void TestPlayPreload(UINT8 uChnNum)
{
	UINT32 dwReadDataSize;

	if (uChnNum == 0x01)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_BL, 0x00);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_BH, 0x00);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_EL, 0xBF);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_EH, 0x0E);
		dwReadDataSize = 15104;
		GetStream2Data(uImageBuffer, &dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x10);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		}
	}else if (uChnNum == 0x02)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_BL, 0x00);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_BH, 0x00);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_EL, 0x5F);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_EH, 0x07);
		Vreg_WriteReg(VDEV_REG_SFIFO1_AW_BL, 0x60);
		Vreg_WriteReg(VDEV_REG_SFIFO1_AW_BH, 0x07);
		Vreg_WriteReg(VDEV_REG_SFIFO1_AW_EL, 0x5F);
		Vreg_WriteReg(VDEV_REG_SFIFO1_AW_EH, 0x07);
		Vreg_WriteReg(VDEV_REG_LOADADDR,0x03);
		dwReadDataSize = 15104 >> 1;
		GetStream2Data(uImageBuffer, &dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x10);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		}
		dwReadDataSize = 15104 >> 1;
		GetStream3Data(uImageBuffer, &dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x20);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		}
	}else if (uChnNum == 0x04)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_BL, 0x00);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_BH, 0x00);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_EL, 0xAF);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_EH, 0x03);
		Vreg_WriteReg(VDEV_REG_SFIFO1_AW_BL, 0xB0);
		Vreg_WriteReg(VDEV_REG_SFIFO1_AW_BH, 0x03);
		Vreg_WriteReg(VDEV_REG_SFIFO1_AW_EL, 0xAF);
		Vreg_WriteReg(VDEV_REG_SFIFO1_AW_EH, 0x03);
		Vreg_WriteReg(VDEV_REG_SFIFO2_AW_BL, 0x60);
		Vreg_WriteReg(VDEV_REG_SFIFO2_AW_BH, 0x07);
		Vreg_WriteReg(VDEV_REG_SFIFO2_AW_EL, 0xAF);
		Vreg_WriteReg(VDEV_REG_SFIFO2_AW_EH, 0x03);
		Vreg_WriteReg(VDEV_REG_SFIFO3_AW_BL, 0x10);
		Vreg_WriteReg(VDEV_REG_SFIFO3_AW_BH, 0x0B);
		Vreg_WriteReg(VDEV_REG_SFIFO3_AW_EL, 0xAF);
		Vreg_WriteReg(VDEV_REG_SFIFO3_AW_EH, 0x03);
		Vreg_WriteReg(VDEV_REG_LOADADDR,0x0F);
		dwReadDataSize = 15104 >> 2;
		GetStream0Data(uImageBuffer, &dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x10);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		}

		dwReadDataSize = 15104 >> 2;
		GetStream1Data(uImageBuffer, &dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x20);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		}
		
		dwReadDataSize = 15104 >> 2;
		GetStream2Data(uImageBuffer, &dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x40);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		}

		dwReadDataSize = 15104 >> 2;
		GetStream3Data(uImageBuffer, &dwReadDataSize);
		if(dwReadDataSize>0)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x80);
			Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
			Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		}
	}
	TestPlayEvent();
}

void TestPlayDLS(void)
{
	UINT32 dwReadDataSize;

	dwReadDataSize = 15104;
	GetStream3Data(uImageBuffer, &dwReadDataSize);
	if (dwReadDataSize > 0)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_BL, 0x00);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_BH, 0x00);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_EL, 0xBF);
		Vreg_WriteReg(VDEV_REG_SFIFO0_AW_EH, 0x0E);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x10);
		Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
	}
	TestPlayEvent();
}

void TestPlayMP3(void)
{
	UINT32 dwReadDataSize;
	int    i;

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
	if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB)			//ECB encrypted mode
	{
		Vreg_WriteReg(VDEV_REG_AES_CORE_CMD, 0x22);
	}
	else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)		//CBC encrypted mode
	{
		Vreg_WriteReg(VDEV_REG_AES_CORE_CMD, 0x23);
	}

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(VDEV_REG_SWRST1, 0x20);
	Vreg_WriteReg(VDEV_REG_INTFLAG1, 0x01);
	
	dwReadDataSize = 1760;  
	GetStream0Data(uImageBuffer,&dwReadDataSize);
	if(dwReadDataSize>0)
	{
		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(10);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[0][i]);
			}
		}else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB){
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(10);
		}
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x08);
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);
		Vreg_WriteMulRegs(VDEV_REG_MP3_FIFO, uImageBuffer, dwReadDataSize);
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		if (dwReadDataSize > 15 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			for (i=0; i<16; i++)
				g_uInitVec[0][i] = uImageBuffer[dwReadDataSize-16+i];
		}
	}
	Vreg_Delay(0x100);
	Vdev_UnMaskInt(VDEV_INT_MFIFOEMPTY);
	TestPlayEvent();
}

////////////Interrupt service routines//////////////////////
//
// Descriptions:
//     Event Fifo empty Interrupt service routine.
//
// Parameters:
//     None.
//
// Returns:
//     None.
//
void	TestEventInt(void)
{
	UINT32 dwReadDataSize = 0;

	dwReadDataSize = g_dwEventFifoEMPLen;
	GetEventData(uImageBuffer,&dwReadDataSize);
	if(dwReadDataSize>0)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x02);
		Vreg_WriteMulRegs(VDEV_REG_EFIFO, uImageBuffer, dwReadDataSize);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
	}
}

//
// Descriptions:
//     Stream0 Fifo Empty Interrupt service routine.
//
// Parameters:
//     None.
//
// Returns:
//     None.
//
void	TestStream0Int(void)
{
	UINT32 dwReadDataSize;
	UINT8  i;

	dwReadDataSize= g_dwStm0FifoEMPLen;
	GetStream0Data(uImageBuffer,&dwReadDataSize);
	if(dwReadDataSize>0)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);

		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(1);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[0][i]);
			}
		}else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB){
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(1);
		}
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB)			//ECB encrypted mode
		{
			Vreg_WriteReg(VDEV_REG_AES_CORE_CMD, 0x22);
		}
		else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC) 	//CBC encrypted mode
		{
			Vreg_WriteReg(VDEV_REG_AES_CORE_CMD, 0x23);
		}

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
//		Vreg_WriteReg(VDEV_REG_INTSERV, 0x10);
		Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
              Vreg_Delay(1);
//		Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		if (dwReadDataSize > 16 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			for (i=0; i<16; i++)
				g_uInitVec[0][i] = uImageBuffer[dwReadDataSize-16+i];
		}
	}
}

//
// Descriptions:
//     Stream1 Fifo Empty Interrupt service routine.
//
// Parameters:
//     None.
//
// Returns:
//     None.
//
void	TestStream1Int(void)
{
	UINT32 dwReadDataSize;
	UINT8  i;

	dwReadDataSize = g_dwStm1FifoEMPLen;
	GetStream1Data(uImageBuffer,&dwReadDataSize);
	if(dwReadDataSize>0)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(10);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[0][i]);
			}
		}else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB){
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(10);
		}

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x20);
		Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
		Vreg_Delay(1);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		if (dwReadDataSize > 16 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			for (i=0; i<16; i++)
				g_uInitVec[1][i] = uImageBuffer[dwReadDataSize-16+i];
		}
	}
}

//
// Descriptions:
//     Stream2 Fifo Empty Interrupt service routine.
//
// Parameters:
//     None.
//
// Returns:
//     None.
//
void	TestStream2Int(void)
{
	UINT32 dwReadDataSize;
	UINT8  i;

	dwReadDataSize = g_dwStm2FifoEMPLen;
	GetStream2Data(uImageBuffer, &dwReadDataSize);
	if(dwReadDataSize>0)
	{
		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(10);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[0][i]);
			}
		}else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB){
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(10);
		}


		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x40);
		Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
		Vreg_Delay(1);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		if (dwReadDataSize > 15 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			for (i=0; i<16; i++)
				g_uInitVec[2][i] = uImageBuffer[dwReadDataSize-16+i];
		}
	}
}

//
// Descriptions:
//     Stream3 Fifo Empty Interrupt service routine.
//
// Parameters:
//     None.
//
// Returns:
//     None.
//
void	TestStream3Int(void)
{
	UINT32 dwReadDataSize;
	UINT8  i;

	dwReadDataSize = g_dwStm3FifoEMPLen;
	GetStream3Data(uImageBuffer,&dwReadDataSize);
	if(dwReadDataSize>0)
	{
		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(10);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[0][i]);
			}
		}else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB){
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(10);
		}

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x80);
		Vreg_WriteMulRegs(VDEV_REG_SFIFO, uImageBuffer, dwReadDataSize);
		Vreg_Delay(1);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		if (dwReadDataSize > 15 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			for (i=0; i<16; i++)
				g_uInitVec[3][i] = uImageBuffer[dwReadDataSize-16+i];
		}
	}
}

//
// Descriptions:
//     Mp3 Fifo Empty Interrupt service routine.
//
// Parameters:
//     None.
//
// Returns:
//     None.
//
void	TestMp3Int(void)
{
	UINT32 dwReadDataSize;
	UINT8  i;
	
	dwReadDataSize = g_dwMp3FifoEMPLen;   
	GetStream0Data(uImageBuffer,&dwReadDataSize);
	if(dwReadDataSize>0)
	{
		if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(10);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_INITVEC0+i, g_uInitVec[0][i]);
			}
		}else if (g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_ECB){
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
			for (i=0; i<16; i++)
			{
				Vreg_WriteReg(VDEV_REG_AES_KEY0+i, g_uAesKey[0][i]);
			}
			Vreg_Delay(10);
		}
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x08);
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);
		Vreg_WriteMulRegs(VDEV_REG_MP3_FIFO, uImageBuffer, dwReadDataSize);
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
		if (dwReadDataSize > 15 && g_bECBorCBC == TYPE_PLAY_MODE_ENCRYPT_CBC)
		{
			for (i=0; i<16; i++)
				g_uInitVec[0][i] = uImageBuffer[dwReadDataSize-16+i];
		}
	}
}

/**********************************************************************************
*
*	Command
*
***********************************************************************************/

void GetRegInfo(UINT8 *pEventData)
{
	g_regInfo.uChnNum = 1;
	g_regInfo.wBaseAddr[0] = 0;
	g_regInfo.wEndPtr[0]     = 0x760 - 0x01;
	g_regInfo.uWaterMark[0] = 0x3B;
}

void TestPlay(void)
{
	unsigned char i;
	unsigned char uCtype = 0;
	unsigned char uWMark0 = 0, uWMark1 =0;
	
	Uart_SendBuffer((char*)&g_RSW,SIZE_RSW);

	//global varials initialize
	uPtEventCur = 0;
	uPtStream0Cur= 0;
	uPtStream1Cur= 0;
	uPtStream2Cur= 0;
	uPtStream3Cur= 0;
	uPtCommandCur = 0;
	g_bECBorCBC = 0;

	uCtype  = g_OBW.byPara1_b1;   //Get Test Play command
	uWMark0 = g_OBW.byPara1_b3;   //Get water mark for Event, stm0 or Mp3, stm1, stm2
	uWMark1 = g_OBW.byPara1_b4;   //Get water mark for stm3

	Vdev_AddIsr(VDEV_INT_EFIFOEMPTY,  TestEventInt);
	Vdev_AddIsr(VDEV_INT_SFIFO0EMPTY, TestStream0Int);
	Vdev_AddIsr(VDEV_INT_SFIFO1EMPTY, TestStream1Int);
	Vdev_AddIsr(VDEV_INT_SFIFO2EMPTY, TestStream2Int);
	Vdev_AddIsr(VDEV_INT_SFIFO3EMPTY, TestStream3Int);
	Vdev_AddIsr(VDEV_INT_MFIFOEMPTY,  TestMp3Int);

	GetRegInfo(uEventBuf);

	if (uCtype & TYPE_PLAY_MODE_ENCRYPT_ECB)	//if encryped ECB mode 
		g_bECBorCBC = TYPE_PLAY_MODE_ENCRYPT_ECB;	//get encrypted ECB mode
	if (uCtype & TYPE_PLAY_MODE_ENCRYPT_CBC)	//if encryped ECB mode 
		g_bECBorCBC = TYPE_PLAY_MODE_ENCRYPT_CBC;	//get encrypted ECB mode

	if (g_bECBorCBC > 0)
	{
		for (i=0; i<16; i++)
		{
			g_uAesKey[0][i] = 0x31+i;
			g_uAesKey[1][i] = 0x31+i;
			g_uAesKey[2][i] = 0x31+i;
			g_uAesKey[3][i] = 0x31+i;
			g_uInitVec[0][i] = 0x00;
			g_uInitVec[1][i] = 0x00;
			g_uInitVec[2][i] = 0x00;
			g_uInitVec[3][i] = 0x00;
		}
		for (i=0; i<7; i++)
		{
			g_uAesKey[0][i+9] = 0x30+i;
			g_uAesKey[1][i+9] = 0x30+i;
			g_uAesKey[2][i+9] = 0x30+i;
			g_uAesKey[3][i+9] = 0x30+i;
		}
	}

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	switch(uWMark0 & 0x03)      //Event Empty Fifo Water mark
	{
		case 0:				//Fifo water Mark 1/4
			g_dwEventFifoEMPLen = 256;
			Vreg_WriteReg(VDEV_REG_E_W_MARK, 0x00);
			break;
		case 1:				//Fifo water Mark 1/2
			g_dwEventFifoEMPLen = 512;
			Vreg_WriteReg(VDEV_REG_E_W_MARK, 0x01);
			break;
		case 2:				//Fifo water Mark 3/4
			g_dwEventFifoEMPLen = 768;
			Vreg_WriteReg(VDEV_REG_E_W_MARK, 0x02);
			break;
	}

	switch(uCtype&0x0F)
	{
	case TYPE_PLAY_PURE_MIDI:                 //Play Pure Midi
		if ((uCtype&0xF0) & TYPE_PLAY_MODE_REAL_MIDI)
		{
			VC938_RealPlay();
		}
		else
			TestPlayEvent();
		break;
			
	case TYPE_PLAY_MIDI_WAVE_1CH:         //Play 1ch Stream and Midi

		if (!g_uEventBufExist || !g_uStm0BufExist)
			return;
		if (g_regInfo.uChnNum < 1)
			return;
		
		switch(uWMark0&0x0C)      //Stream0 Empty Fifo Water mark
		{
			case 0x00:				//Fifo water Mark 1/4
				g_regInfo.uWaterMark[0] = 0x760 >> 2 >> 4;
				break;
			case 0x04:				//Fifo water Mark 1/2
				g_regInfo.uWaterMark[0] = 0x760 >> 1 >> 4;
				break;
			case 0x08:				//Fifo water Mark 3/4
				g_regInfo.uWaterMark[0] = ((0x760 >> 2) * 3) >> 4;
				break;
		}
		
		g_dwStm0FifoEMPLen = ((UINT32)(g_regInfo.uWaterMark[0]) & 0xFF) << 4 << 2;
		TestPlayStream(TYPE_PLAY_MIDI_WAVE_1CH);
		break;
			
	case TYPE_PLAY_MIDI_WAVE_2CH:               //Play Mix 2ch Stream and Midi
		if (!g_uEventBufExist || !g_uStm0BufExist || !g_uStm1BufExist)
			return;
		g_regInfo.uChnNum = 2;
		g_regInfo.wBaseAddr[0] = 0;
		g_regInfo.wEndPtr[0] = 0x075F;
		g_regInfo.wBaseAddr[1] = 0x0760;
		g_regInfo.wEndPtr[1] = 0x075F;
		switch(uWMark0&0x0C)      //Stream0 Empty Fifo Water mark
		{
			case 0x00:				//Fifo water Mark 1/4
				g_regInfo.uWaterMark[0] = (g_regInfo.wEndPtr[0] + 1) >> 2 >> 4;
				break;
			case 0x04:				//Fifo water Mark 1/2
				g_regInfo.uWaterMark[0] = (g_regInfo.wEndPtr[0] + 1) >> 1 >> 4;
				break;
			case 0x08:				//Fifo water Mark 3/4
				g_regInfo.uWaterMark[0] = (((g_regInfo.wEndPtr[0] + 1) >> 2) * 3) >> 4;
				break;
		}
		
		switch(uWMark0&0x30)      //Stream1 Empty Fifo Water mark
		{
			case 0x00:				//Fifo water Mark 1/4
				g_regInfo.uWaterMark[1] = (g_regInfo.wEndPtr[1] + 1) >> 2 >> 4;
				break;
			case 0x10:				//Fifo water Mark 1/2
				g_regInfo.uWaterMark[1] = (g_regInfo.wEndPtr[1] + 1) >> 1 >> 4;
				break;
			case 0x20:				//Fifo water Mark 3/4
				g_regInfo.uWaterMark[1] = (((g_regInfo.wEndPtr[1] + 1) >> 2) * 3) >> 4;
				break;
		}

		g_dwStm0FifoEMPLen = ((UINT32)(g_regInfo.uWaterMark[0]) & 0xFF) << 4 << 2;
		g_dwStm1FifoEMPLen = ((UINT32)(g_regInfo.uWaterMark[1]) & 0xFF) << 4 << 2;
		TestPlayStream(TYPE_PLAY_MIDI_WAVE_2CH);
		break;
			
	case TYPE_PLAY_MIDI_WAVE_3CH:                //Play 3ch Stream and Midi
		if (!g_uEventBufExist || !g_uStm0BufExist || !g_uStm1BufExist || !g_uStm2BufExist)
			return;
		if (g_regInfo.uChnNum < 3)
			return;
		switch(uWMark0&0x0C)      //Stream0 Empty Fifo Water mark
		{
			case 0x00:				//Fifo water Mark 1/4
				g_regInfo.uWaterMark[0] = (g_regInfo.wEndPtr[0]+1) >> 2 >> 4;
				break;
			case 0x04:				//Fifo water Mark 1/2
				g_regInfo.uWaterMark[0] = (g_regInfo.wEndPtr[0]+1) >> 1 >> 4;
				break;
			case 0x08:				//Fifo water Mark 3/4
				g_regInfo.uWaterMark[0] = (((g_regInfo.wEndPtr[0]+1) >> 2) * 3) >> 4;
				break;
		}
		
		switch(uWMark0&0x30)      //Stream1 Empty Fifo Water mark
		{
			case 0x00:				//Fifo water Mark 1/4
				g_regInfo.uWaterMark[1] = (g_regInfo.wEndPtr[1]+1) >> 2 >> 4;
				break;
			case 0x10:				//Fifo water Mark 1/2
				g_regInfo.uWaterMark[1] = (g_regInfo.wEndPtr[1]+1) >> 1 >> 4;
				break;
			case 0x20:				//Fifo water Mark 3/4
				g_regInfo.uWaterMark[1] = (((g_regInfo.wEndPtr[1]+1) >> 2) * 3) >> 4;
				break;
		}


		switch(uWMark0&0xC0)      //Stream2 Empty Fifo Water mark
		{
			case 0x00:				//Fifo water Mark 1/4
				g_regInfo.uWaterMark[2] = (g_regInfo.wEndPtr[2]+1) >> 2 >> 4;
				break;
			case 0x40:				//Fifo water Mark 1/2
				g_regInfo.uWaterMark[2] = (g_regInfo.wEndPtr[2]+1) >> 1 >> 4;
				break;
			case 0x80:				//Fifo water Mark 3/4
				g_regInfo.uWaterMark[2] = (((g_regInfo.wEndPtr[2]+1) >> 2) * 3) >> 4;
				break;
		}

		g_dwStm0FifoEMPLen = ((UINT32)(g_regInfo.uWaterMark[0]) & 0xFF) << 4 << 2;
		g_dwStm1FifoEMPLen = ((UINT32)(g_regInfo.uWaterMark[1]) & 0xFF) << 4 << 2;
		g_dwStm2FifoEMPLen = ((UINT32)(g_regInfo.uWaterMark[2]) & 0xFF) << 4 << 2;
		TestPlayStream(TYPE_PLAY_MIDI_WAVE_3CH);
		break;
		
	case TYPE_PLAY_MIDI_WAVE_4CH:			//Play Mix 4 ch Stream and Midi
		if (!g_uEventBufExist || !g_uStm0BufExist || !g_uStm1BufExist || !g_uStm2BufExist || !g_uStm3BufExist)
			return;

		g_regInfo.uChnNum = 4;
		g_regInfo.wBaseAddr[0] = 0;
		g_regInfo.wEndPtr[0] = 0x03AF;
		g_regInfo.wBaseAddr[1] = 0x03B0;
		g_regInfo.wEndPtr[1] = 0x03AF;
		g_regInfo.wBaseAddr[2] = 0x0760;
		g_regInfo.wEndPtr[2] = 0x03AF;
		g_regInfo.wBaseAddr[3] = 0x0B10;
		g_regInfo.wEndPtr[3] = 0x03AF;

		switch(uWMark0&0x0C)      //Stream0 Empty Fifo Water mark
		{
			case 0x00:				//Fifo water Mark 1/4
				g_regInfo.uWaterMark[0] = (g_regInfo.wEndPtr[0]+1) >> 2 >> 4;
				break;
			case 0x04:				//Fifo water Mark 1/2
				g_regInfo.uWaterMark[0] = (g_regInfo.wEndPtr[0]+1) >> 1 >> 4;
				break;
			case 0x08:				//Fifo water Mark 3/4
				g_regInfo.uWaterMark[0] = (((g_regInfo.wEndPtr[0]+1) >> 2) * 3) >> 4;
				break;
		}
		
		switch(uWMark0&0x30)      //Stream1 Empty Fifo Water mark
		{
			case 0x00:				//Fifo water Mark 1/4
				g_regInfo.uWaterMark[1] = (g_regInfo.wEndPtr[1]+1) >> 2 >> 4;
				break;
			case 0x10:				//Fifo water Mark 1/2
				g_regInfo.uWaterMark[1] = (g_regInfo.wEndPtr[1]+1) >> 1 >> 4;
				break;
			case 0x20:				//Fifo water Mark 3/4
				g_regInfo.uWaterMark[1] = (((g_regInfo.wEndPtr[1]+1) >> 2) * 3) >> 4;
				break;
		}

		switch(uWMark0&0xC0)      //Stream2 Empty Fifo Water mark
		{
			case 0x00:				//Fifo water Mark 1/4
				g_regInfo.uWaterMark[2] = (g_regInfo.wEndPtr[2]+1) >> 2 >> 4;
				break;
			case 0x40:				//Fifo water Mark 1/2
				g_regInfo.uWaterMark[2] = (g_regInfo.wEndPtr[2]+1) >> 1 >> 4;
				break;
			case 0x80:				//Fifo water Mark 3/4
				g_regInfo.uWaterMark[2] = (((g_regInfo.wEndPtr[2]+1) >> 2) * 3) >> 4;
				break;
		}

		switch(uWMark1&0x03)      //Stream3 Empty Fifo Water mark
		{
			case 0:				//Fifo water Mark 1/4
				g_regInfo.uWaterMark[3] = (g_regInfo.wEndPtr[3]+1) >> 2 >> 4;
				break;
			case 1:				//Fifo water Mark 1/2
				g_regInfo.uWaterMark[3] = (g_regInfo.wEndPtr[3]+1) >> 1 >> 4;
				break;
			case 2:				//Fifo water Mark 3/4
				g_regInfo.uWaterMark[3] = (((g_regInfo.wEndPtr[3]+1) >> 2) * 3) >> 4;
				break;
		}

		g_dwStm0FifoEMPLen = ((UINT32)(g_regInfo.uWaterMark[0]) & 0xFF) << 4 << 2;
		g_dwStm1FifoEMPLen = ((UINT32)(g_regInfo.uWaterMark[1]) & 0xFF) << 4 << 2;
		g_dwStm2FifoEMPLen = ((UINT32)(g_regInfo.uWaterMark[2]) & 0xFF) << 4 << 2;
		g_dwStm3FifoEMPLen = ((UINT32)(g_regInfo.uWaterMark[3]) & 0xFF) << 4 << 2;
		TestPlayStream(TYPE_PLAY_MIDI_WAVE_4CH);
		break;
			
	case TYPE_PLAY_MIDI_PRELOAD_1CH:               //Play Mix Midi and Preload
		if (!g_uEventBufExist || !g_uStm2BufExist)
			return;
		TestPlayPreload(0x01);
		break;
		
	case TYPE_PLAY_MIDI_PRELOAD_2CH:			   //Play Mix Midi and Preload
		if (!g_uEventBufExist || !g_uStm2BufExist || !g_uStm3BufExist)
			return;
		TestPlayPreload(0x02);
		break;
			
	case TYPE_PLAY_MIDI_PRELOAD_4CH:			   //Play Mix Midi and Preload
		if (!g_uEventBufExist || !g_uStm0BufExist || !g_uStm1BufExist ||!g_uStm2BufExist || !g_uStm3BufExist)
			return;
		TestPlayPreload(0x04);
		break;
					
	case TYPE_PLAY_MIDI_DLS:                               //Play Mix Midi and DLS
		if (!g_uEventBufExist || !g_uStm3BufExist)
			return;
		TestPlayDLS();
		break;
		

	case TYPE_PLAY_MIDI_STREAM_DLS_PRELOAD:   //Play Mix Midi, Stream, DLS and Preload
		break;
		

	case TYPE_PLAY_MIDI_MP3:      //Play Pure Mp3 or Mix mp3 and Midi
		if (!g_uEventBufExist || !g_uStm0BufExist)
			return;

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);
		switch(uWMark0&0x0C)      //MP3 Empty Fifo Water mark
		{
			case 0x00:				//Fifo water Mark 1/4
				g_dwMp3FifoEMPLen = 440;
				Vreg_WriteReg(VDEV_REG_MP3_W_MARK, 0x00);
				break;
			case 0x04:				//Fifo water Mark 1/2
				g_dwMp3FifoEMPLen = 880;
				Vreg_WriteReg(VDEV_REG_MP3_W_MARK, 0x01);
				break;
			case 0x08:				//Fifo water Mark 3/4
				g_dwMp3FifoEMPLen =  1320;
				Vreg_WriteReg(VDEV_REG_MP3_W_MARK, 0x02);
				break;
		}
		TestPlayMP3();
		break;

	case TYPE_PLAY_MIDI_REVERB:   //Play Midi or Event with reverb
		break;

	case TYPE_PLAY_COMMAND:        //Play command fifo testing
		if (!g_uCommBufExist)
			return;
		TestPlayCommand();
		break;

	default:
		break;
	}

	if (uWMark1 & 0x04)
		g_bPollingPlay = 0;
	else
		g_bPollingPlay = 1;
}

void TestStop(void)
{
	Uart_SendBuffer((char*)&g_RSW,SIZE_RSW);

	if (g_bRealPlay)
		VC938_RealStop();
	else
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vdev_MaskInt(VDEV_INT_ALL);
		Vreg_WriteReg(VDEV_REG_SWRST1, 0x80);
	}
}

////////////////////Download file type //////////////////////
void TestDownloadFile(void)
{
	UINT32 uLen = 0;
	Uart_SendBuffer((char*)&g_RSW,SIZE_RSW);
	switch(g_OBW.byPara1_b1)
	{
	case TYPE_DOWNLOAD_EVENT:
		g_uEventBufExist = 1;
		uLen = iBufferImageEndPointer;
		if(uLen > EVENT_FILE_LEN)
		{
			uLen = EVENT_FILE_LEN;
		}
		memcpy(uEventBuf, uImageBuffer, uLen);
		uPtEventEnd = uLen;
		break;

	case TYPE_DOWNLOAD_STREAM0:
		g_uStm0BufExist = 1;
		uLen = iBufferImageEndPointer;
		if(uLen > STREAM0_FILE_LEN)
		{
			uLen = STREAM0_FILE_LEN;
		}
		memcpy(uStream0Buf, uImageBuffer, uLen);
		uPtStream0End = uLen;
		break;

	case TYPE_DOWNLOAD_STREAM1:
		g_uStm1BufExist = 1;
		uLen = iBufferImageEndPointer;
		if(uLen > STREAM1_FILE_LEN)
		{
			uLen = STREAM1_FILE_LEN;
		}
		memcpy(uStream1Buf, uImageBuffer, uLen);
		uPtStream1End = uLen;
		break;

	case TYPE_DOWNLOAD_STREAM2:
		g_uStm2BufExist = 1;
		uLen = iBufferImageEndPointer;
		if(uLen > STREAM2_FILE_LEN)
		{
			uLen = STREAM2_FILE_LEN;
		}
		memcpy(uStream2Buf, uImageBuffer, uLen);
		uPtStream2End = uLen;
		break;

	case TYPE_DOWNLOAD_STREAM3:
		g_uStm3BufExist = 1;
		uLen = iBufferImageEndPointer;
		if(uLen > STREAM3_FILE_LEN)
		{
			uLen = STREAM3_FILE_LEN;
		}
		memcpy(uStream3Buf, uImageBuffer, uLen);
		uPtStream3End = uLen;
		break;

	case TYPE_DOWNLOAD_COMMAND:
		g_uCommBufExist = 1;
		uLen = iBufferImageEndPointer;
		if(uLen > COMMAND_FILE_LEN)

		{
			uLen = COMMAND_FILE_LEN;
		}
		memcpy(uCommandBuf, uImageBuffer, uLen);
		uPtCommandEnd  = uLen;
		break;
	case TYPE_PLAY_GAME_EFFECT:
		VPlr_PlayGameEffect(uStream3Buf,uPtStream3End );		

		break;
		
	default:
		break;
	}
}

