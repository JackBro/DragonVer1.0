/*------------------------------------------------------------------------

				COPYRIGHT (C) 2004, Vimicro Corporation
                        	ALL RIGHTS RESERVED

	This source code has been made available to you by VIMICRO on an 
	AS-IS basis. Anyone receiving this source code is licensed under
	VIMICRO copyrights to use it in any way he or she deems fit, including 
	copying it,  modifying it, compiling it, and redistributing it either
	with or without  modifications. Any person who transfers this source 
	code or any derivative  work must include the VIMICRO copyright notice 
	and	this paragraph in the transferred software.

	Version		Description        					Date         	Author

	1.0			Basic version						2004.11.4	ZY

	1.1 		fix a bug about Offset initialize	2004.11.18		gzd
				add pWave->dOffset = 0 in VWave_Load

	1.2												2004.12.09		gzd
				Remove the obsolescent functions VWave_GetEmStream() 
				and VWave_GetVoice().
	1.21												2005.1.19		ZY
				Add dOffsetBack to backup dOffset
				Modify for File Stream 
				
-------------------------------------------------------------------------*/

#include "vmd.h"
#include "vwave.h"


VM_WAVE_PARSER_INFO	g_WaveInfo;


extern void ReadBuffer(UINT8* bBuffer, UINT32 dBufferSize, UINT32* pdRealSize, UINT32 dOffset);



/***************************************************************
Description:
		Initial the Wave parser.
		
Parameters:
		void.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/

VRESULT VWave_Init(void)
{
	UINT8 bIdx;

	for(bIdx=0; bIdx<MAX_STREAM_VOICE; bIdx++)
	{
		g_WaveInfo.WaveData[bIdx].pbDataBuf = NULL;
		g_WaveInfo.WaveData[bIdx].wBitsPerSmpl = 0;
		g_WaveInfo.WaveData[bIdx].wChn = 0;
		g_WaveInfo.WaveData[bIdx].bUnSigned = 0;
		g_WaveInfo.WaveData[bIdx].bStatus = 0;
		g_WaveInfo.WaveData[bIdx].dDataSize = 0;
		g_WaveInfo.WaveData[bIdx].wWavFmt = 0;
		g_WaveInfo.WaveData[bIdx].dStmBufLen = 0;
		g_WaveInfo.WaveData[bIdx].dOffset = 0;
		g_WaveInfo.WaveData[bIdx].dOffsetBack = 0;
		g_WaveInfo.WaveData[bIdx].dTotalTime = 0;
		g_WaveInfo.WaveData[bIdx].dCurTime = 0;
		g_WaveInfo.WaveData[bIdx].wSmplRate = 0;
		g_WaveInfo.WaveData[bIdx].bSentMsgNum = 0;
		g_WaveInfo.pWaveChn[bIdx] = NULL;
		g_WaveInfo.WaveData[bIdx].bSrcFlag = 0;
		g_WaveInfo.WaveData[bIdx].bStmBufOffset = 0;
	}

	g_WaveInfo.bEmptyNum = MAX_STREAM_VOICE;
	g_WaveInfo.bAssignedWav = 0xff;
	
	return VRES_SUCCESS;
}



/***************************************************************
Description:
		Load the PCM/ADPCM data.
		
Parameters:
		pbFile:		Pointer to the PCM/ADPCM data buffer.
		dLength:		The size of PCM/ADPCM data.
		dEmptyRes:	Empty resource, 0 means unused , 1 means used.
					Bit 0: Event FIFO
					Bit 1: Command FIFO
					Bit 2: Stream buffer 0
					Bit 3: Stream buffer 1
					Bit 4: Stream buffer 2
					Bit 5: Stream buffer 3
		bSrcFlag:		0 means load file from RAM.
					1 means load file by file stream.
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/

VRESULT VWave_Load(HUGE UINT8* pbFile, UINT32 dLength, UINT32 dEmptyRes, UINT8 bSrcFlag)
{
	UINT8 bIdx;
	HUGE UINT8* pbPtr;
	UINT32 dTemp, dID, dLen, dRealLen;
	PVM_WAVE_DATA_INFO	pWave;

	if(pbFile==NULL || dLength<=44)
		return VRES_ERROR;

	if( g_WaveInfo.bEmptyNum==0) //there is no resource to store this file infomation
		return VRES_ERROR;
	
	//Find empty "WaveData"
	for(bIdx=0; bIdx<MAX_STREAM_VOICE; bIdx++)
	{
		if(g_WaveInfo.WaveData[bIdx].bStatus==0)
		{
			g_WaveInfo.WaveData[bIdx].bSrcFlag = bSrcFlag;
			pWave = &g_WaveInfo.WaveData[bIdx];
			break;
		}
	}

	//Add by gzd for fix a bug about Offset initialize	
	pWave->dOffset = 0;

	if(pWave->bSrcFlag==0) //Load form RAM
	{
		pbPtr = pbFile;
	}
	else
	{	dRealLen = 0;
		ReadBuffer(pWave->bFileStmBuf, 44, &dRealLen,0); //Read header

		if(dRealLen!=44)
			return VRES_ERROR;
		pbPtr = (HUGE UINT8*)(pWave->bFileStmBuf);
	}


	dTemp = (UINT32)pbPtr[8] + ((UINT32)pbPtr[9]<<8) + 
		((UINT32)pbPtr[10]<<16) + ((UINT32)pbPtr[11]<<24);
	if(dTemp != 0x45564157 ) //"WAVE"
	{
		return VRES_ERROR;
	}

	dTemp = (UINT32)pbPtr[12] + ((UINT32)pbPtr[13]<<8) + 
		((UINT32)pbPtr[14]<<16) + ((UINT32)pbPtr[15]<<24);
	if(dTemp != 0x20746d66 ) //"fmt "
	{
		return VRES_ERROR;
	}

	pWave->wWavFmt = (UINT16)pbPtr[20] + ((UINT16)pbPtr[21]<<8); //File format
	pWave->wChn = (UINT16)pbPtr[22] + ((UINT16)pbPtr[23] << 8);
	pWave->wBitsPerSmpl= (UINT16)pbPtr[34] + ((UINT16)pbPtr[35] << 8);
	pWave->wSmplRate= (UINT16)pbPtr[24] + ((UINT16)pbPtr[25] << 8); 
	pWave->dCurTime = 0;
	pWave->dTotalTime = 0;
	pWave->dOffset = 0;

	if(pWave->wSmplRate>48000) 
		return VRES_ERROR; 
							
	if(pWave->wBitsPerSmpl==8)
		pWave->bUnSigned= 1;
	else if(pWave->wBitsPerSmpl==16)
		pWave->bUnSigned= 0;
	else if(pWave->wBitsPerSmpl==4)
		pWave->bUnSigned= 0;
	else
		return VRES_ERROR; 

	dLen = (UINT32)pbPtr[16] + ((UINT32)pbPtr[17]<<8) + 
		((UINT32)pbPtr[18]<<16) + ((UINT32)pbPtr[19]<<24);

	pbPtr += 20+dLen;
	pWave->dOffset +=20+dLen;
	
	dID = (UINT32)pbPtr[0] + ((UINT32)pbPtr[1] << 8) + 
		((UINT32)pbPtr[2] << 16) + ((UINT32)pbPtr[3] << 24);
	dLen = (UINT32)pbPtr[4] + ((UINT32)pbPtr[5] << 8) + 
		((UINT32)pbPtr[6] << 16) +	((UINT32)pbPtr[7] << 24);
	
//	pbPtr += 8+dLen;
//	pWave->dOffset +=8+dLen;
	
	if(pWave->wWavFmt == 1) //PCM
	{
		if(pWave->bSrcFlag==0) //Load form RAM
		{
			if(dID!=0x61746164) //It's the WAVEFORMATEX
			{
/*				dLen = (UINT32)pbPtr[0] + ((UINT32)pbPtr[1] << 8);
				pbPtr += (2+dLen);

				dID = (UINT32)pbPtr[0] + ((UINT32)pbPtr[1]<<8) + 
					((UINT32)pbPtr[2]<<16) + ((UINT32)pbPtr[3]<<24);
				dLen = (UINT32)pbPtr[4] + ((UINT32)pbPtr[5]<<8) + 
					((UINT32)pbPtr[6]<<16) + ((UINT32)pbPtr[7]<<24);
*/
				if(dID!=0x74636166) //It's not fact chunk
				{
					return VRES_ERROR;
				}
				else
				{
					pbPtr += (8+dLen);
					pWave->dOffset += (8+dLen);
				}

				dID = (UINT32)pbPtr[0] + ((UINT32)pbPtr[1]<<8) + 
					((UINT32)pbPtr[2]<<16) + ((UINT32)pbPtr[3]<<24);
				dLen = (UINT32)pbPtr[4] + ((UINT32)pbPtr[5]<<8) + 
					((UINT32)pbPtr[6]<<16) + ((UINT32)pbPtr[7]<<24);

				if(dID!=0x61746164)
					return VRES_ERROR;
			}

			pWave->dDataSize = dLen;
			pWave->pbDataBuf = pbFile;
			pWave->dOffset += 8;

/*
			while(dID!=0x61746164)//find"data"
			{
				pbPtr++;
				if(pbPtr >=pbFile + dLength) //Hasn't "data"
					return VRES_ERROR;
				
				dID = (UINT32)pbPtr[0] + ((UINT32)pbPtr[1]<<8) + 
					((UINT32)pbPtr[2]<<16) + ((UINT32)pbPtr[3]<<24);
				dLen = (UINT32)pbPtr[4] + ((UINT32)pbPtr[5]<<8) + 
					((UINT32)pbPtr[6]<<16) + ((UINT32)pbPtr[7]<<24);
			}
			pWave->dDataSize = dLen;
			pWave->pbDataBuf = pbPtr+8;
*/
		}
		else
		{

			if(dID!=0x61746164)
			{

//				dLen = (UINT32)pbPtr[0] + ((UINT32)pbPtr[1] << 8);
				
				dRealLen = 0;
				//To find "fact" chunk and size of "fact" chunk
/*				ReadBuffer(pWave->bFileStmBuf, VM_WAVE_FILE_STREAM_BUF_LEN, &dRealLen,pWave->dOffset + (dLen + 8));
				if(dRealLen!=VM_WAVE_FILE_STREAM_BUF_LEN)
					return VRES_ERROR;

				pWave->dOffset += (dLen+8) + VM_WAVE_FILE_STREAM_BUF_LEN;
				pbPtr = (HUGE UINT8*)(pWave->bFileStmBuf);

				dID = (UINT32)pbPtr[0] + ((UINT32)pbPtr[1]<<8) + 
					((UINT32)pbPtr[2]<<16) + ((UINT32)pbPtr[3]<<24);
				dLen = (UINT32)pbPtr[4] + ((UINT32)pbPtr[5]<<8) + 
					((UINT32)pbPtr[6]<<16) + ((UINT32)pbPtr[7]<<24);
*/				
				if(dID!=0x74636166) //It's not fact chunk
					return VRES_ERROR;
					
				//To find "data" chunk and size of "data" chunk
				pWave->dOffset += dLen + 8;
				ReadBuffer(pWave->bFileStmBuf, VM_WAVE_FILE_STREAM_BUF_LEN, &dRealLen,pWave->dOffset);
				if(dRealLen!=VM_WAVE_FILE_STREAM_BUF_LEN)
					return VRES_ERROR;

				pbPtr = (HUGE UINT8*)(pWave->bFileStmBuf);

				dID = (UINT32)pbPtr[0] + ((UINT32)pbPtr[1]<<8) + 
					((UINT32)pbPtr[2]<<16) + ((UINT32)pbPtr[3]<<24);
				dLen = (UINT32)pbPtr[4] + ((UINT32)pbPtr[5]<<8) + 
					((UINT32)pbPtr[6]<<16) + ((UINT32)pbPtr[7]<<24);

				if(dID!=0x61746164)
					return VRES_ERROR;
			}
			
			pWave->dDataSize = dLen;
			pWave->dOffset += 8;

/*
			while(dID!=0x61746164)//find"data"
			{
				dRealLen = 0;
				ReadBuffer(pWave->bFileStmBuf, 64, &dRealLen,pWave->dOffset); //Read header

				if(dRealLen!=8)
					return VRES_ERROR;

				pWave->dOffset +=dRealLen;
				pbPtr = (HUGE UINT8*)(pWave->bFileStmBuf);
				
				//pbPtr++;
				//if(pbPtr >=pbFile + dFileLen) //Hasn't "data"
				//	return VRES_ERROR;
				
				dID = (UINT32)pbPtr[0] + ((UINT32)pbPtr[1]<<8) + 
					((UINT32)pbPtr[2]<<16) + ((UINT32)pbPtr[3]<<24);
				dLen = (UINT32)pbPtr[4] + ((UINT32)pbPtr[5]<<8) + 
					((UINT32)pbPtr[6]<<16) + ((UINT32)pbPtr[7]<<24);
			}
			pWave->dDataSize = dLen;
			pWave->dOffset += 8;
*/
		}
	}
	else if(pWave->wWavFmt == 0)//ADPCM
	{
		if(dID!=0x61746164)
			return VRES_ERROR;
		else
		{
			pWave->dDataSize = dLen;
			pWave->pbDataBuf = pbFile;
			pWave->dOffset += 8;
		}
	}
	else
	{
		return VRES_ERROR;
	}

	//Calculate playback time
	pWave->dTotalTime = (UINT32)(pWave->dDataSize*1000)/
		(((UINT32)pWave->wChn*(UINT32)pWave->wBitsPerSmpl*(UINT32)pWave->wSmplRate)>>3);

	pWave->AdpcmState.swPreVal = 0;
	pWave->AdpcmState.bIndex = 0;
	pWave->bStatus = 1;	

	pWave->dOffsetBack= pWave->dOffset; //2005.1.19 ZY
	
	g_WaveInfo.bEmptyNum--;
	g_WaveInfo.bAssignedWav = bIdx;
	
	return VRES_SUCCESS;
}



/***************************************************************
Description:
		Configure the wave parser.
		
Parameters:
		dVal:		Conifg Item.
		dDat:		Config Data.
		bConfigID:	Config Catalog ID.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VWave_Configure(UINT32 dVal, UINT32 dDat, UINT8 bConfigID)
{
	UINT8 bIdx;

	if(dVal ==VDEV_FIFO_STREAM0)
		bIdx = 0;
	else if(dVal ==VDEV_FIFO_STREAM1)
		bIdx = 1;
	else if(dVal ==VDEV_FIFO_STREAM2)
		bIdx = 2;
	else if(dVal ==VDEV_FIFO_STREAM3)
		bIdx = 3;
	else
		return VRES_ERROR;
	
	switch(bConfigID)
	{
	case VM_PARSER_SET_FIFO:	//Assigned a PCM/ADPCM to a FiFo
	
		if(g_WaveInfo.pWaveChn[bIdx] == NULL)
		{
			if(g_WaveInfo.bAssignedWav==0xff) //There is no wave which hasn't be assigned to a fifo
				return VRES_ERROR;
			else
			{
				g_WaveInfo.pWaveChn[bIdx] = &g_WaveInfo.WaveData[g_WaveInfo.bAssignedWav];
				g_WaveInfo.bAssignedWav = 0xff;
			}
		}
		else		//There are no available fifo resource
			return VRES_ERROR;
		
		break;
	case VM_PARSER_SET_WATER_MARK:
		if(g_WaveInfo.pWaveChn[bIdx] == NULL)
		{
			g_WaveInfo.pWaveChn[bIdx]->dStmBufLen = dVal;
		}
		else		//There are no available fifo resource
			return VRES_ERROR;
		break;
	default:
		break;
	}
	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Get a message.
		
Parameters:
		pUniMsg:		Pointer to unified music message structure.
		FiFoType:		The FIFO which the event sent to .
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/

VRESULT VWave_GetEvent(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType)
{
	UINT8 bIdx;
	PVM_WAVE_DATA_INFO	pWave;

	if(FiFoType ==VDEV_FIFO_STREAM0)
	{
		pUniMsg->bChn = 20;
		bIdx = 0;
	}
	else if(FiFoType ==VDEV_FIFO_STREAM1)
	{
		pUniMsg->bChn = 21;
		bIdx = 1;
	}
	else if(FiFoType ==VDEV_FIFO_STREAM2)
	{
		pUniMsg->bChn = 22;
		bIdx = 2;
	}
	else if(FiFoType ==VDEV_FIFO_STREAM3)
	{
		pUniMsg->bChn = 23;
		bIdx = 3;
	}
	else
		return VRES_ERROR;

	if(g_WaveInfo.pWaveChn[bIdx])
		pWave = g_WaveInfo.pWaveChn[bIdx];
	else
		return VRES_ERROR;

	if(pWave->bSentMsgNum==0)
	{
		pUniMsg->sdDeltaTime = -1;	//Wave event has to be sent to command FiFo.
		pUniMsg->bMsgID = VMD_MSG_STREAM_ON;
		pUniMsg->dPara1 = ((UINT32)pWave->wWavFmt&0xf) +  ((UINT32)(pWave->wChn&0x3)<<4) +
					((UINT32)(pWave->bUnSigned&0x3)<<6)  + ((UINT32)(pWave->wBitsPerSmpl)<<8)+
					((UINT32)(pWave->wSmplRate)<<16);
		
		pUniMsg->dPara2 = pWave->dTotalTime ;//- pWave->dCurTime;
		pUniMsg->dPara3 = (UINT32)((UINT16)pWave->AdpcmState.swPreVal) + 
					((UINT32)(pWave->AdpcmState.bIndex)<<16);

		pWave->bSentMsgNum++;
	}
	else if (pWave->bSentMsgNum==1)
	{
/*		pUniMsg->sdDeltaTime = pWave->dTotalTime;	//Wave event has to be sent to command FiFo.
		pUniMsg->bMsgID = VMD_MSG_STREAM_OFF;
		pUniMsg->dPara1 = 0;
		pUniMsg->dPara2 = 0;//- pWave->dCurTime;
		pUniMsg->dPara3 = 0;

		pWave->bSentMsgNum++;
*/
		return VRES_END_FLAG;
	}
	else
		return VRES_END_FLAG;
		
	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Get PCM/ADPCM data information, including  the total time of this PCM/ADPCM.
		
Parameters:
		pvFileInfo:	Pointer to file information structure.
		FiFoType:		The FIFO ID which the PCM/ADPCM is assigned to .
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/

VRESULT VWave_GetFileInfo(PVOID pvFileInfo, VDEV_FIFO_TYPE FiFoType)
{
	FILE_INFO	*PFileInfo = (FILE_INFO*)pvFileInfo;	//Added By TangMian
	UINT8 bIdx;

	if(FiFoType ==VDEV_FIFO_STREAM0)
		bIdx = 0;
	else if(FiFoType ==VDEV_FIFO_STREAM1)
		bIdx = 1;
	else if(FiFoType ==VDEV_FIFO_STREAM2)
		bIdx = 2;
	else if(FiFoType ==VDEV_FIFO_STREAM3)
		bIdx = 3;
	else
		return VRES_ERROR;
	
	if(g_WaveInfo.bAssignedWav <=3)
	{
		PFileInfo->dTotalTime = g_WaveInfo.WaveData[g_WaveInfo.bAssignedWav].dTotalTime;	//Changed by TangMian
		return VRES_SUCCESS;
	}
	else
		return VRES_ERROR;
}


/***************************************************************
Description:
		Get current playback position. 
		
Parameters:
		pdCurTime:		Unit is millisecond.
		FiFoType:			The FIFO ID which the PCM/ADPCM is assigned to .
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VWave_GetCurPos(PUINT32 pdCurTime, VDEV_FIFO_TYPE FiFoType)
{
	UINT8 bIdx;

	if(FiFoType ==VDEV_FIFO_STREAM0)
		bIdx = 0;
	else if(FiFoType ==VDEV_FIFO_STREAM1)
		bIdx = 1;
	else if(FiFoType ==VDEV_FIFO_STREAM2)
		bIdx = 2;
	else if(FiFoType ==VDEV_FIFO_STREAM3)
		bIdx = 3;
	else
		return VRES_ERROR;

	if(g_WaveInfo.pWaveChn[bIdx])
	{
		*pdCurTime = g_WaveInfo.pWaveChn[bIdx]->dCurTime;
		return VRES_SUCCESS;
	}
	else
		return VRES_ERROR;
	

}


/***************************************************************
Description:
		Seek PCM/ADPCM to a specified position. This function is unuseful.
		
Parameters:
		dPos:		Unit is millisecond.
		FiFoType:		The FIFO ID which the PCM/ADPCM is assigned to .
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VWave_Seek(UINT32 dPos, VDEV_FIFO_TYPE FiFoType)
{
	(void)dPos;
	(void)FiFoType;
	return VRES_SUCCESS;
}



/***************************************************************
Description:
		NONE
		
Parameters:
		void
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VWave_End(void)
{
    return VRES_SUCCESS;
}

/***************************************************************
Description:
		Get a block of wave data from specified file.
		
Parameters:
		pbOutBuf:		Point to the wave data buffer.
		dBufLen:			The length of data excepted to read.
		pdDataSize:		The length of data readed realy.
		FiFoType:			Resource type indicates which wave file to read from.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VWave_ReadBlock(PUINT8 pbOutBuf, UINT32 dBufLen, PUINT32 pdDataSize, VDEV_FIFO_TYPE FiFoType)
{
	UINT8 bIdx;
	UINT32 dRealLen;
	PVM_WAVE_DATA_INFO	 pWave;
	UINT8 * pbOutPtr = pbOutBuf;

	
	if(pbOutBuf==NULL || dBufLen == 0)
		return VRES_ERROR;

	if(FiFoType ==VDEV_FIFO_STREAM0)
		bIdx = 0;
	else if(FiFoType ==VDEV_FIFO_STREAM1)
		bIdx = 1;
	else if(FiFoType ==VDEV_FIFO_STREAM2)
		bIdx = 2;
	else if(FiFoType ==VDEV_FIFO_STREAM3)
		bIdx = 3;
	else
		return VRES_ERROR;

	if(g_WaveInfo.pWaveChn[bIdx])
		pWave = g_WaveInfo.pWaveChn[bIdx];
	else
		return VRES_ERROR;

	*pdDataSize = 0;
	dRealLen = 0;
	
	if(pWave->bSrcFlag==0) //Read form RAM
	{
		if((pWave->dDataSize - pWave->dOffset)>dBufLen) //Have enough data
		{
			Vmd_MemCpy((HUGE UINT8*)pbOutBuf, pWave->pbDataBuf+pWave->dOffset, dBufLen);
			pWave->dOffset += dBufLen;
			*pdDataSize = dBufLen;

			pWave->dCurTime += (UINT32)(dBufLen*1000)/
				(((UINT32)pWave->wChn*(UINT32)pWave->wBitsPerSmpl*(UINT32)pWave->wSmplRate)>>3);

			return VRES_SUCCESS;
		}
		else
		{
			Vmd_MemCpy((HUGE UINT8*)pbOutBuf, pWave->pbDataBuf+pWave->dOffset, 
				(pWave->dDataSize - pWave->dOffset));
			*pdDataSize = (pWave->dDataSize - pWave->dOffset);
			pWave->dOffset = pWave->dDataSize ;

			pWave->dCurTime += (UINT32)(*pdDataSize*1000)/
				(((UINT32)pWave->wChn*(UINT32)pWave->wBitsPerSmpl*(UINT32)pWave->wSmplRate)>>3);

			return VRES_STREAM_DATA_END;
		}
	}
	else
	{
//		while(dBufLen!=0)
		{
			ReadBuffer(pbOutPtr, dBufLen, //pWave->bFileStmBuf, VM_WAVE_FILE_STREAM_BUF_LEN,
				&dRealLen, pWave->dOffset); //Read header

/*			if(dBufLen>=dRealLen)
			{
				Vmd_MemCpy((HUGE UINT8*)pbOutPtr, (HUGE UINT8*)(pWave->bFileStmBuf), dRealLen);
			}
			else
			{
				Vmd_MemCpy((HUGE UINT8*)pbOutPtr, (HUGE UINT8*)(pWave->bFileStmBuf), dBufLen);
				dRealLen = dBufLen;
			}
*/
			pWave->dOffset += dRealLen;
//			dBufLen -= dRealLen;
//			pbOutPtr += dRealLen;
			*pdDataSize = dRealLen;


			pWave->dCurTime += (UINT32)(dRealLen*1000)/
				(((UINT32)pWave->wChn*(UINT32)pWave->wBitsPerSmpl*(UINT32)pWave->wSmplRate)>>3);
			if(dRealLen !=VM_WAVE_FILE_STREAM_BUF_LEN) //To the end
				return VRES_STREAM_DATA_END;
			dRealLen = 0;
		}

		return VRES_SUCCESS;
	}
}

/***************************************************************
Description:
		Reset the parameters of wave parser
		
Parameters:
		FiFoType:		The FIFO which the data sent to .
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VWave_Reset(VDEV_FIFO_TYPE FiFoType)
{
	UINT8 bIdx;
	PVM_WAVE_DATA_INFO	 pWave;

	if(FiFoType ==VDEV_FIFO_STREAM0)
		bIdx = 0;
	else if(FiFoType ==VDEV_FIFO_STREAM1)
		bIdx = 1;
	else if(FiFoType ==VDEV_FIFO_STREAM2)
		bIdx = 2;
	else if(FiFoType ==VDEV_FIFO_STREAM3)
		bIdx = 3;
	else
		return VRES_ERROR;

	if(g_WaveInfo.pWaveChn[bIdx])
	{
		pWave = g_WaveInfo.pWaveChn[bIdx];
		pWave->dOffset = pWave->dOffsetBack;
		pWave->AdpcmState.bIndex = 0;
		pWave->AdpcmState.swPreVal = 0;
		pWave->dCurTime = 0;
		pWave->bSentMsgNum = 0;

		return VRES_SUCCESS;
	}
	else
		return VRES_ERROR;
}

/***************************************************************
Description:
		Unload PCM/ADPCM data.
		
Parameters:
		FiFoType:	The FIFO which the data sent to .
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VWave_Unload(VDEV_FIFO_TYPE FiFoType)
{
	UINT8 bIdx;
	PVM_WAVE_DATA_INFO	 pWave;

	if(FiFoType ==VDEV_FIFO_STREAM0)
		bIdx = 0;
	else if(FiFoType ==VDEV_FIFO_STREAM1)
		bIdx = 1;
	else if(FiFoType ==VDEV_FIFO_STREAM2)
		bIdx = 2;
	else if(FiFoType ==VDEV_FIFO_STREAM3)
		bIdx = 3;
	else
		return VRES_ERROR;

	if(g_WaveInfo.pWaveChn[bIdx])
	{
		pWave = g_WaveInfo.pWaveChn[bIdx];
		pWave->dOffset = 0;
		pWave->AdpcmState.bIndex = 0;
		pWave->AdpcmState.swPreVal = 0;
		pWave->dCurTime = 0;
		pWave->dTotalTime = 0;
		pWave->bStatus = 0;

		g_WaveInfo.pWaveChn[bIdx] = NULL;

		g_WaveInfo.bEmptyNum++;
		return VRES_SUCCESS;
	}
	else
		return VRES_ERROR;
}

