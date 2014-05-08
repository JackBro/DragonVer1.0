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

	1.1												2004.12.09	gzd
				Remove the obsolescent functions VMidi_GetEmStream()
				and VMidi_GetVoice().

	1.2												2004.12.16	zy
				Add other midi file support.
				
-------------------------------------------------------------------------*/

#include "vmd.h"
#include "vmidi.h"


VM_MIDI_PARSER_INFO	g_MidiInfo;


/***************************************************************
Description:
		Initialize the start time of each MIDI track.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
void SetBeginTime(void)
{
	UINT8 bIdx, bTemp;
	UINT32 dTimeCode;

	for(bIdx=0; bIdx<g_MidiInfo.bTrackNum; bIdx++)
	{
		bTemp = *(g_MidiInfo.Track[bIdx].pbCurnData++);
		dTimeCode = (UINT32)bTemp&0x7f;
		while((bTemp&0x80) != 0)
		{
			bTemp = *(g_MidiInfo.Track[bIdx].pbCurnData);
			dTimeCode = dTimeCode*0x80 + (bTemp&0x7f);
			g_MidiInfo.Track[bIdx].pbCurnData++;
		}

		g_MidiInfo.Track[bIdx].dNextTick	+= dTimeCode;
	}
	
	return;
}



/***************************************************************
Description:
		Return the track number of the next message.
		
Parameters:
		void.
		
Returns:
		Track number.
		
****************************************************************/
UINT8 FindLeastTimeTrack(void)
{
	UINT8 bIdx, bTrkID;
	UINT32 dMinTick;

	dMinTick = 0xffffffff;
	bTrkID = 0;

	for(bIdx=0; bIdx<g_MidiInfo.bTrackNum; bIdx++)
	{
		if(bIdx<32)
		{
			if(((g_MidiInfo.dTrackEndL>>bIdx)&0x01) == 0) //This track is not end
			{
				if(dMinTick > g_MidiInfo.Track[bIdx].dNextTick)
				{
					dMinTick = g_MidiInfo.Track[bIdx].dNextTick;
					bTrkID = bIdx;
				}
			}

		}
		else
		{
			if(((g_MidiInfo.dTrackEndH>>(bIdx-32))&0x01) == 0) //This track is not end
			{
				if(dMinTick > g_MidiInfo.Track[bIdx].dNextTick)
				{
					dMinTick = g_MidiInfo.Track[bIdx].dNextTick;
					bTrkID = bIdx;
				}
			}
		}
			
	}
	
	g_MidiInfo.dCurTime+= (g_MidiInfo.Track[bTrkID].dNextTick - g_MidiInfo.dCurTick)*g_MidiInfo.dTempoMS;
	g_MidiInfo.dCurTick = g_MidiInfo.Track[bTrkID].dNextTick;

	return bTrkID;
}


/***************************************************************
Description:
		Get a message from the current track.
		
Parameters:
		bTrkNum:	The id of current track.
		
Returns:
		1:			Successful.
		0:			Failed.
		
****************************************************************/
UINT8 GetOneMessage(UINT8 bTrkNum)
{
	UINT8 bMsgID, bMsgLen, bReturn,bStatus, bTemp;
	UINT32 dTempoCode, i, dTimeCode, dMeltaLen,dDeltaTime;
	HUGE UINT8 *pbCurData;



	bReturn = 0;
	
	pbCurData = g_MidiInfo.Track[bTrkNum].pbCurnData;
	
	if(pbCurData >= g_MidiInfo.Track[bTrkNum].pbDataEnd)
	{
		if(bTrkNum<32)
			g_MidiInfo.dTrackEndL |=((UINT32)0x00000001<<bTrkNum);
		else
			g_MidiInfo.dTrackEndH|=((UINT32)0x00000001<<(bTrkNum-32));
		
		return bReturn;
	}

	bStatus = *pbCurData;

	if( (bStatus&0x80) == 0)//MsgID be omitted
	{
		bMsgID = (g_MidiInfo.Track[bTrkNum].bPreStatus) & 0xf0;

		switch(bMsgID)
		{
		case MSG_CHNPRESSURE:
		case MSG_PROGCHANGE:
			bMsgLen = 1;
			break;
		default:
			bMsgLen = 2;
			break;
		}

		if(bMsgLen>0)//write to msgBuf
		{
			dDeltaTime = (g_MidiInfo.dCurTime - g_MidiInfo.dPreTime)/1000;
			g_MidiInfo.bMsgBuf[0] = (UINT8)(dDeltaTime & 0xff);
			g_MidiInfo.bMsgBuf[1] = (UINT8)((dDeltaTime>>8) & 0xff);
		
			g_MidiInfo.dPreTime = g_MidiInfo.dCurTime;

			g_MidiInfo.bMsgBuf[2] = g_MidiInfo.Track[bTrkNum].bPreStatus;

			for(i=0; i<bMsgLen; i++)
			{
				g_MidiInfo.bMsgBuf[3 + i] = *pbCurData;
				pbCurData++;
			}

			bReturn = 1;
		}
	}
	else
	{
		if(bStatus== MELTA_EVENT)
		{
			pbCurData++;
			switch(*pbCurData)
			{
			case MELTA_TEMPO:
				pbCurData++;
				if(*pbCurData != 0x03)
				{
					//Unknown tempo format,was ignored
					dMeltaLen = (UINT32)*pbCurData;
					for(i=0; i<dMeltaLen+1; i++)
						pbCurData++;
				}
				else
				{
					dTempoCode = 0;
					pbCurData++;
					for(i=0; i<3; i++)
					{
						dTempoCode = dTempoCode*0x100 + *pbCurData;
						pbCurData++;
					}
					g_MidiInfo.dTempoMS= dTempoCode/g_MidiInfo.dTicksPerQN;
				}
				bMsgLen = 0;
				break;

			default:
				//not supported
				if(*pbCurData==0x7f)
				{
					pbCurData++;
					bTemp= (*pbCurData);
					dMeltaLen = (UINT32)(bTemp&0x7f);
					pbCurData++;
					while((bTemp&0x80) != 0)
					{
						bTemp= *pbCurData;
						dMeltaLen = dMeltaLen*0x80 + (UINT32)(bTemp&0x7f);
						pbCurData++;
					}
					pbCurData--;
				}
				else
				{
					pbCurData++;
					dMeltaLen = (UINT32)*pbCurData;
				}
				for(i=0; i<=dMeltaLen; i++)
					pbCurData++;
				bMsgLen = 0;
				break;
			}
		}
		else if(bStatus==MSG_SYSEXCLUDE)
		{
			while(*pbCurData != 0xf7)	//no system exclusive message supported
				pbCurData++;
			pbCurData++;
			bMsgLen = 0;
		}
		else
		{
			bMsgID = bStatus&0xf0;
			if(bMsgID==MSG_PROGCHANGE||bMsgID==MSG_CHNPRESSURE)
				bMsgLen= 2;
			else
				bMsgLen = 3;
		}

		if(bMsgLen>0)
		{
			dDeltaTime = (g_MidiInfo.dCurTime - g_MidiInfo.dPreTime)/1000;
			g_MidiInfo.bMsgBuf[0] = (UINT8)(dDeltaTime & 0xff);
			g_MidiInfo.bMsgBuf[1] = (UINT8)((dDeltaTime>>8) & 0xff);
		
			g_MidiInfo.dPreTime = g_MidiInfo.dCurTime;
			
			g_MidiInfo.Track[bTrkNum].bPreStatus = bStatus;

			for(i=0; i<bMsgLen; i++)
			{
				g_MidiInfo.bMsgBuf[2 + i] = *pbCurData;
				pbCurData++;
			}

			bReturn = 1;
		}
	}

	if(pbCurData >= g_MidiInfo.Track[bTrkNum].pbDataEnd)
	{
		if(bTrkNum<32)
			g_MidiInfo.dTrackEndL|=((UINT32)0x00000001<<bTrkNum);
		else
			g_MidiInfo.dTrackEndH|=((UINT32)0x00000001<<(bTrkNum-32));
	}
	else
	{

		bTemp = *(pbCurData);
		dTimeCode = bTemp&0x7f;
		pbCurData++;
		while((bTemp&0x80) != 0)
		{
			bTemp = *pbCurData;
			dTimeCode = dTimeCode*0x80 + (bTemp&0x7f);
			pbCurData++;
		}
		g_MidiInfo.Track[bTrkNum].dNextTick += dTimeCode;
		g_MidiInfo.Track[bTrkNum].pbCurnData = pbCurData;
	}
	
	return bReturn;
}



/***************************************************************
Description:
		Scan the MIDI data.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
void SeekMsg()
{
	UINT8 bIdx, bFlag, bStatus, bChn;

	bFlag = 0;

	while(bFlag == 0)
	{
		bIdx = FindLeastTimeTrack();
			
		bFlag = GetOneMessage(bIdx);

		if((g_MidiInfo.dTrackEndH == 0xffffffff) && (g_MidiInfo.dTrackEndL == 0xffffffff) )
			return;
	}

	bStatus = g_MidiInfo.bMsgBuf[2]&0xf0;
	bChn = g_MidiInfo.bMsgBuf[2]&0x0f;

	switch(bStatus)
	{
	case MSG_PITCHBEND:
		g_MidiInfo.ChnInfo[bChn].bPitchBendH = g_MidiInfo.bMsgBuf[4]&0x7f;
		g_MidiInfo.ChnInfo[bChn].bPitchBendL = g_MidiInfo.bMsgBuf[3]&0x7f;
		break;

	case MSG_PROGCHANGE:
		g_MidiInfo.ChnInfo[bChn].bTimbre = g_MidiInfo.bMsgBuf[3];
		break;

	case MSG_CTRLCHANGE:

		switch(g_MidiInfo.bMsgBuf[3])
		{
		case CTRL_MODULATION:
			g_MidiInfo.ChnInfo[bChn].bModulation = g_MidiInfo.bMsgBuf[4];
			break;
			
		case CTRL_VOLUME:
			g_MidiInfo.ChnInfo[bChn].bVolume = g_MidiInfo.bMsgBuf[4];
			break;
			
		case CTRL_PANPOT:
			g_MidiInfo.ChnInfo[bChn].bPan= g_MidiInfo.bMsgBuf[4];
			break;
			
		case CTRL_EXPRESS:
			g_MidiInfo.ChnInfo[bChn].bExpression = g_MidiInfo.bMsgBuf[4];
			break;
			
		case CTRL_HOLD1:
			g_MidiInfo.ChnInfo[bChn].bHold1= g_MidiInfo.bMsgBuf[4];
			break;
			
		case CTRL_RESETCTRLS:
			g_MidiInfo.wRPN[bChn] = 0x7f7f;
			//g_MidiInfo.ChnInfo[bChn].volume     	= 0x7f;
			g_MidiInfo.ChnInfo[bChn].bExpression	= 0x7f;
			g_MidiInfo.ChnInfo[bChn].bModulation 	= 0;
			//g_MidiInfo.ChnInfo[bChn].bendrange  	= 0x02;  //bendrange= 0x02*100;
			//g_MidiInfo.ChnInfo[bChn].panpot     	= 0x40;
			g_MidiInfo.ChnInfo[bChn].bHold1      	= 0;

			break;
			
		case CTRL_RPNLSB:
			g_MidiInfo.wRPN[bChn] = ((g_MidiInfo.wRPN[bChn] & 0x7F00) | ((UINT16)g_MidiInfo.bMsgBuf[4]));
			break;
			
		case CTRL_RPNMSB:
			g_MidiInfo.wRPN[bChn] = ((g_MidiInfo.wRPN[bChn] & 0x007F) | ((UINT16)g_MidiInfo.bMsgBuf[4])<<8);
			break;
			
		case CTRL_DATAENTRYMSB:
			if (g_MidiInfo.wRPN[bChn] == 0) 
				g_MidiInfo.ChnInfo[bChn].bBendRage= g_MidiInfo.bMsgBuf[4];
			break;

		default:
			break;
		}

	default:
		break;
	}

	g_MidiInfo.bMsgBuf[0] = 0;
	g_MidiInfo.bMsgBuf[1] = 0;
	g_MidiInfo.bMsgBuf[2] = 0;
	g_MidiInfo.bMsgBuf[3] = 0;
	g_MidiInfo.bMsgBuf[4] = 0;

	return;

}





/***************************************************************
Description:
		Add available messages into the message queue.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
void AddMsgQue()
{
	UINT8 bChn;
	PVM_UNI_MSG pUniMsg;

	for(bChn=0; bChn<MAX_MIDI_CHN; bChn++)
	{
		if(g_MidiInfo.ChnInfo[bChn].bTimbre != 0)
		{
			pUniMsg = &g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum];
			
			pUniMsg->bChn = bChn;
			pUniMsg->sdDeltaTime = 0;
			pUniMsg->bMsgID = VMD_MSG_PROGRAM_CHANGE;
			pUniMsg->dPara1 = (UINT32)g_MidiInfo.ChnInfo[bChn].bTimbre ;
			
			g_MidiInfo.bMsgQueNum++;
		}

		if(g_MidiInfo.ChnInfo[bChn].bVolume != 0x7f)
		{
			pUniMsg = &g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum];
			
			pUniMsg->bChn = bChn;
			pUniMsg->sdDeltaTime = 0;
			pUniMsg->bMsgID = VMD_MSG_CHANNEL_VOLUME;
			pUniMsg->dPara1 = (UINT32)g_MidiInfo.ChnInfo[bChn].bVolume ;
			
			g_MidiInfo.bMsgQueNum++;
		}

		if(g_MidiInfo.ChnInfo[bChn].bExpression != 0x7f)
		{
			pUniMsg = &g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum];
			
			pUniMsg->bChn = bChn;
			pUniMsg->sdDeltaTime = 0;
			pUniMsg->bMsgID = VMD_MSG_EXPRESSION;
			pUniMsg->dPara1 = (UINT32)g_MidiInfo.ChnInfo[bChn].bExpression ;
			
			g_MidiInfo.bMsgQueNum++;
		}

		if(g_MidiInfo.ChnInfo[bChn].bModulation != 0)
		{
			pUniMsg = &g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum];
			
			pUniMsg->bChn = bChn;
			pUniMsg->sdDeltaTime = 0;
			pUniMsg->bMsgID = VMD_MSG_MODULATION_DEPTH;
			pUniMsg->dPara1 = (UINT32)g_MidiInfo.ChnInfo[bChn].bModulation ;
			
			g_MidiInfo.bMsgQueNum++;
		}

		if(g_MidiInfo.ChnInfo[bChn].bPitchBendH != 0x40 ||g_MidiInfo.ChnInfo[bChn].bPitchBendL != 0 )
		{
			pUniMsg = &g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum];
			
			pUniMsg->bChn = bChn;
			pUniMsg->sdDeltaTime = 0;
			pUniMsg->bMsgID = VMD_MSG_PITCH_BEND;
			pUniMsg->dPara1	= ((UINT32)(g_MidiInfo.ChnInfo[bChn].bPitchBendH )<<7) +
				(UINT32)(g_MidiInfo.ChnInfo[bChn].bPitchBendL);
			
			g_MidiInfo.bMsgQueNum++;
		}

		if(g_MidiInfo.ChnInfo[bChn].bBendRage!= 2)
		{
			pUniMsg = &g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum];
			
			pUniMsg->bChn = bChn;
			pUniMsg->sdDeltaTime = 0;
			pUniMsg->bMsgID = VMD_MSG_BEND_RANGE;
			pUniMsg->dPara1 = (UINT32)g_MidiInfo.ChnInfo[bChn].bBendRage ;
			
			g_MidiInfo.bMsgQueNum++;
		}

		if(g_MidiInfo.ChnInfo[bChn].bPan!= 0x40)
		{
			pUniMsg = &g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum];
			
			pUniMsg->bChn = bChn;
			pUniMsg->sdDeltaTime = 0;
			pUniMsg->bMsgID = VMD_MSG_PANPOT;
			pUniMsg->dPara1 = (UINT32)g_MidiInfo.ChnInfo[bChn].bPan ;
			
			g_MidiInfo.bMsgQueNum++;
		}

		if(g_MidiInfo.ChnInfo[bChn].bHold1!= 0)
		{
			pUniMsg = &g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum];
			
			pUniMsg->bChn = bChn;
			pUniMsg->sdDeltaTime = 0;
			pUniMsg->bMsgID = VMD_MSG_HOLD1;
			pUniMsg->dPara1 = (UINT32)g_MidiInfo.ChnInfo[bChn].bHold1 ;
			
			g_MidiInfo.bMsgQueNum++;
		}

		if(bChn==9)
		{
			if(g_MidiInfo.ChnInfo[bChn].bBankID!= 0x80)
			{
				pUniMsg = &g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum];
				
				pUniMsg->bChn = bChn;
				pUniMsg->sdDeltaTime = 0;
				pUniMsg->bMsgID = VMD_MSG_BANK_SELECT;
				pUniMsg->dPara1 = (UINT32)g_MidiInfo.ChnInfo[bChn].bBankID ;
				
				g_MidiInfo.bMsgQueNum++;
			}
		}
		else
		{
			if(g_MidiInfo.ChnInfo[bChn].bBankID!= 0)
			{
				pUniMsg = &g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum];
				
				pUniMsg->bChn = bChn;
				pUniMsg->sdDeltaTime = 0;
				pUniMsg->bMsgID = VMD_MSG_BANK_SELECT;
				pUniMsg->dPara1 = (UINT32)g_MidiInfo.ChnInfo[bChn].bBankID ;
				
				g_MidiInfo.bMsgQueNum++;
			}
		}
	}

}



/***************************************************************
Description:
		Initial the MIDI parser.
		
Parameters:
		void.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/

VRESULT VMidi_Init()
{
	UINT8 bIdx;

	g_MidiInfo.bSendEOS		= 0;
	g_MidiInfo.bMidiFmt			= 0xff;
	g_MidiInfo.bMsgQueNum		= 0;
	g_MidiInfo.bTrackNum		= 0;
	g_MidiInfo.dCurTick			= 0;
	g_MidiInfo.dCurTime		= 0;
	g_MidiInfo.dPreTime			= 0;
	g_MidiInfo.dSeekOverTime 	= 0;
	g_MidiInfo.dTempoMS		= 0;
	g_MidiInfo.dTicksPerQN		= 0;
	g_MidiInfo.dTotalTime		= 0;
	g_MidiInfo.dTrackEndH		= 0xffffffff;
	g_MidiInfo.dTrackEndL		= 0xffffffff;
	

	for(bIdx=0; bIdx<MAX_MIDI_CHN; bIdx++)
	{
		g_MidiInfo.wRPN[bIdx] = 0x7f7f;
	}
	
	return VRES_SUCCESS;
}



/***************************************************************
Description:
		Load the MIDI data.
		
Parameters:
		pbFile:		Pointer to the MIDI  data buffer.
		dLength:		The size of MIDI data.
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
VRESULT VMidi_Load(HUGE UINT8* pbFile, UINT32 dLength, UINT32 dEmptyRes, UINT8 bSrcFlag)
{
	HUGE UINT8 * pbPtr;
	UINT8	bIdx;
	UINT8	bTrackNum;
	UINT32	dSize, dTmp;

	if(pbFile==NULL || dLength<=22)
		return VRES_ERROR;

	pbPtr = pbFile;

	//Support RMIDI
	if ((pbPtr[0] != 'M') || (pbPtr[1] != 'T') || (pbPtr[2] != 'h') ||(pbPtr[3] != 'd'))
	{
		dSize =  ((UINT32)pbPtr[7] << 24) + ((UINT32)pbPtr[6] << 16) +
			((UINT32)pbPtr[5] << 8) + (UINT32)pbPtr[4];

		/*
		if (dLength != (dSize+8)) 
		{
			return (VRES_ERROR);
		}
		*/

		while(dLength>=4) //find "data"
		{
			if((pbPtr[0]=='d')&&(pbPtr[1]=='a')&&(pbPtr[2]=='t')&&(pbPtr[3]=='a'))
			{
				/*
				if (dLength != (dSize+8)) 
				{
					return (VRES_ERROR);
				}
										
				if((pbPtr[0]=='M')&&(pbPtr[1]=='T')&&(pbPtr[2]=='h')&&(pbPtr[3]=='d'))
				{
					dSize = ((UINT32)pbPtr[7] << 24) + ((UINT32)pbPtr[6] << 16) + 
						((UINT32)pbPtr[5] << 8) + (UINT32)pbPtr[4];
				
					break;
				}
				else
				{
					pbPtr++;
					dLength--;
				}
				*/
				
				pbPtr += 8;
				dLength -= 8;
				break;
			}
			else
			{
				pbPtr++;
				dLength--;
			}

		}
	}

	if(dLength<4)	//There is no "data"
		return (VRES_ERROR);

	//--- Midi data size ----//
	dSize = ((UINT32)pbPtr[4] << 24) + ((UINT32)pbPtr[5] << 16) +
		         ((UINT32)pbPtr[6] << 8) + (UINT32)pbPtr[7];
	if (dSize != 6) //Has no data.
	{
		return (VRES_ERROR);
	}


	//Midi format
	dTmp= ((UINT32)pbPtr[8] << 8) + (UINT32)pbPtr[9];  
	if(dTmp!=0 && dTmp!=1)
		return (VRES_ERROR);
	else
		g_MidiInfo.bMidiFmt = (UINT8)dTmp;

	//Track num
	dTmp = ((UINT32)pbPtr[10] << 8) + (UINT32)pbPtr[11];
	if (dTmp == 0 || (g_MidiInfo.bMidiFmt == 0) && (dTmp != 1) )
	{
		return (VRES_ERROR);
	}
	else
		g_MidiInfo.bTrackNum = (UINT8)dTmp;

	//Tempo
	dTmp = ((UINT32)pbPtr[12] << 8) + (UINT32)pbPtr[13];
	g_MidiInfo.dTicksPerQN = dTmp & 0x7fff;
	g_MidiInfo.dTempoMS = TEMPO_CODE/g_MidiInfo.dTicksPerQN;
	if (((dTmp & 0x8000) != 0) || (g_MidiInfo.dTicksPerQN == 0))
	{
		return (VRES_ERROR);
	}


	//Load tracks

	pbPtr += 14;
	dLength -= 14;

	bTrackNum = 0;

	for (bIdx = 0; bIdx < g_MidiInfo.bTrackNum; bIdx++)
	{
		if (dLength < 8) 
		{
			break;
		}
		
		if ((pbPtr[0] != 'M') ||(pbPtr[1] != 'T') || (pbPtr[2] != 'r') ||(pbPtr[3] != 'k'))
		{
			break;
		}
		
		dSize = ((UINT32)pbPtr[4] << 24) + ((UINT32)pbPtr[5] << 16) +
		         ((UINT32)pbPtr[6] << 8) + (UINT32)pbPtr[7];
	
		if (dLength < (dSize + 8))
		{
			break;
		}

		if(dSize > 4) //This track maybe have useful message
		{
			if(bTrackNum>=MAX_MIDI_TRACK)
				break;
			else
			{
				g_MidiInfo.Track[bTrackNum].pbDataHead 	= &pbPtr[8];
				g_MidiInfo.Track[bTrackNum].pbCurnData 	= g_MidiInfo.Track[bTrackNum].pbDataHead;
				g_MidiInfo.Track[bTrackNum].pbDataEnd   	= g_MidiInfo.Track[bTrackNum].pbDataHead+dSize;
				g_MidiInfo.Track[bTrackNum].dNextTick	= 0;
				g_MidiInfo.Track[bTrackNum].bPreStatus	= 0xff;

				if(bTrackNum<32)
					g_MidiInfo.dTrackEndL&= ~((UINT32)0x00000001<<bTrackNum);
				else
					g_MidiInfo.dTrackEndH&= ~((UINT32)0x00000001<<(bTrackNum-32));

				bTrackNum++;
			}

			pbPtr += (dSize + 8);
			dLength -= (dSize + 8);
		}
		else
		{
			pbPtr += (dSize + 8);
			dLength -= (dSize + 8);
			continue;
		}
	}

	
	if(bTrackNum==0)
		return (VRES_ERROR);
	else
	{
		g_MidiInfo.bTrackNum = bTrackNum;
		SetBeginTime();

		return VRES_SUCCESS;
	}
}



/***************************************************************
Description:
		Configure the MIDI parser.
		
Parameters:
		dVal:		Conifg Item.
		dDat:		Config Data.
		bConfigID:	Config Catalog ID.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VMidi_Configure(UINT32 dVal, UINT32 dDat, UINT8 bConfigID)
{
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
VRESULT VMidi_GetEvent(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType)
{	
	UINT8 bIdx, bFlag, bSatus;


	pUniMsg->sdDeltaTime 	= 0;
	pUniMsg->bMsgID	     	= 0;
	pUniMsg->bChn	     		= 0;
	pUniMsg->dPara1	 		= 0;
	pUniMsg->dPara2	 		= 0;
	pUniMsg->dPara3	 		= 0;

	if(g_MidiInfo.bMsgQueNum)
	{
		pUniMsg->sdDeltaTime 	= g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum-1].sdDeltaTime;
		pUniMsg->bMsgID	 	= g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum-1].bMsgID;
		pUniMsg->bChn			= g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum-1].bChn;
		pUniMsg->dPara1	 		= g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum-1].dPara1;
		pUniMsg->dPara2	 		= g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum-1].dPara2;
		pUniMsg->dPara3	 		= g_MidiInfo.MsgQue[g_MidiInfo.bMsgQueNum-1].dPara3;

		g_MidiInfo.bMsgQueNum--;
		
		return (VRES_SUCCESS);

	}

	bFlag = 0;

	while(bFlag == 0)
	{
		bIdx = FindLeastTimeTrack();
			
		bFlag = GetOneMessage(bIdx);

		//All tracks to the end.
		if(g_MidiInfo.dTrackEndH == (UINT32)0xffffffff &&g_MidiInfo.dTrackEndL == (UINT32)0xffffffff) 
		{	
			if(!g_MidiInfo.bSendEOS)
			{
				pUniMsg->sdDeltaTime 	= (FiFoType==0)?0:-1;
				pUniMsg->bMsgID	 	= VMD_MSG_EOS_EVENT;
				pUniMsg->bChn	 		= 0;
				pUniMsg->dPara1			= 0x7f;
				pUniMsg->dPara2			= 0;
				pUniMsg->dPara3	 		= 0;
				g_MidiInfo.bSendEOS   		= 1;
				
				return VRES_SUCCESS;
			}
			else
				return VRES_END_FLAG;
		}

	}

	if(g_MidiInfo.dSeekOverTime)
	{
		pUniMsg->sdDeltaTime = (SINT32)g_MidiInfo.bMsgBuf[0] +  ((SINT32)g_MidiInfo.bMsgBuf[1]<<8) +
								g_MidiInfo.dSeekOverTime;
		g_MidiInfo.dSeekOverTime = 0;
	}
	else
	{
		pUniMsg->sdDeltaTime =  (SINT32)g_MidiInfo.bMsgBuf[0] +  ((SINT32)g_MidiInfo.bMsgBuf[1]<<8) ;

		pUniMsg->sdDeltaTime = (FiFoType==0)?pUniMsg->sdDeltaTime:-1;
	}

	pUniMsg->bChn	= g_MidiInfo.bMsgBuf[2]&0x0f;
	bSatus			= g_MidiInfo.bMsgBuf[2]&0xf0;

	switch(bSatus)
	{
	case MSG_KEYON:
		if (g_MidiInfo.bMsgBuf[4]) //Note on
		{
			pUniMsg->bMsgID	= VMD_MSG_NOTE_ON;
			pUniMsg->dPara1	= (UINT32)g_MidiInfo.bMsgBuf[3];
			pUniMsg->dPara2	= (UINT32)g_MidiInfo.bMsgBuf[4];
		}
		else //Note off
		{
			pUniMsg->bMsgID	= VMD_MSG_NOTE_OFF;
			pUniMsg->dPara1	= (UINT32)g_MidiInfo.bMsgBuf[3];
			pUniMsg->dPara2	= (UINT32)g_MidiInfo.bMsgBuf[4];
		}
		break;

	case MSG_KEYOFF:
		pUniMsg->bMsgID	= VMD_MSG_NOTE_OFF;
		pUniMsg->dPara1	= (UINT32)g_MidiInfo.bMsgBuf[3];
		pUniMsg->dPara2	= (UINT32)g_MidiInfo.bMsgBuf[4];
		break;

	case MSG_PITCHBEND:
		pUniMsg->bMsgID	= VMD_MSG_PITCH_BEND;
		pUniMsg->dPara1	= ((UINT32)(g_MidiInfo.bMsgBuf[4]&0x7f)<<7) + (UINT32)(g_MidiInfo.bMsgBuf[3]&0x7f);
		break;

	case MSG_PROGCHANGE:
		pUniMsg->bMsgID	= VMD_MSG_PROGRAM_CHANGE;
		pUniMsg->dPara1	= (UINT32)g_MidiInfo.bMsgBuf[3];
		break;

	case MSG_CHNPRESSURE:
		pUniMsg->bMsgID = VMD_MSG_CHANNEL_PRESSURE;
		pUniMsg->dPara1	= (UINT32)g_MidiInfo.bMsgBuf[3];
		break;

	case MSG_CTRLCHANGE:
		pUniMsg->dPara1	= (UINT32)g_MidiInfo.bMsgBuf[4];
		switch(g_MidiInfo.bMsgBuf[3])
		{
		case CTRL_MODULATION:
			pUniMsg->bMsgID = VMD_MSG_MODULATION_DEPTH;
			break;
		case CTRL_VOLUME:
			pUniMsg->bMsgID = VMD_MSG_CHANNEL_VOLUME;
			break;
		case CTRL_PANPOT:
			pUniMsg->bMsgID = VMD_MSG_PANPOT;
			break;
		case CTRL_EXPRESS:
			pUniMsg->bMsgID = VMD_MSG_EXPRESSION;
			break;
		case CTRL_HOLD1:
			pUniMsg->bMsgID = VMD_MSG_HOLD1;
			break;
		case CTRL_RESETCTRLS:
			pUniMsg->bMsgID = VMD_MSG_RESET_ALL_CONTROLLERS;
			g_MidiInfo.wRPN[pUniMsg->bChn] = 0x7f7f;
			break;
		case CTRL_ALLNOTEOFF:
			pUniMsg->bMsgID = VMD_MSG_ALL_NOTE_OFF;
			break;
		case CTRL_RPNLSB:
			g_MidiInfo.wRPN[pUniMsg->bChn] = (((UINT16)g_MidiInfo.wRPN[pUniMsg->bChn] & 0x7F00) | ((UINT16)g_MidiInfo.bMsgBuf[4]));
			pUniMsg->bMsgID = VMD_MSG_NOP;
			break;
		case CTRL_RPNMSB:
			g_MidiInfo.wRPN[pUniMsg->bChn] = (((UINT16)g_MidiInfo.wRPN[pUniMsg->bChn] & 0x007F) | ((UINT16)g_MidiInfo.bMsgBuf[4])<<8);
			pUniMsg->bMsgID = VMD_MSG_NOP;
			break;
		case CTRL_DATAENTRYMSB:
			if (g_MidiInfo.wRPN[pUniMsg->bChn] == 0) 
				pUniMsg->bMsgID = VMD_MSG_BEND_RANGE;
			else
				pUniMsg->bMsgID = VMD_MSG_NOP;
			break;

		default:
			pUniMsg->bMsgID = VMD_MSG_NOP;
			break;
		}
		break;
	}

	g_MidiInfo.bMsgBuf[0] = 0;
	g_MidiInfo.bMsgBuf[1] = 0;
	g_MidiInfo.bMsgBuf[2] = 0;
	g_MidiInfo.bMsgBuf[3] = 0;
	g_MidiInfo.bMsgBuf[4] = 0;

	return (VRES_SUCCESS);
}



/***************************************************************
Description:
		Get MIDI data information, including  the total time of this MIDI.
		
Parameters:
		pvFileInfo:	Pointer to file information structure.
		FiFoType:		Reserved.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VMidi_GetFileInfo(PVOID pvFileInfo, VDEV_FIFO_TYPE FiFoType)
{
	UINT8 bIdx;
	FILE_INFO	*PFileInfo = (FILE_INFO*)pvFileInfo;	//Added By TangMian
	
/*
	for(bIdx=0; bIdx<g_MidiInfo.bTrackNum; bIdx++) //store globle parameters
	{
		g_MidiInfo.Track[bIdx].dNextTick 	= g_MidiInfo.Track[bIdx].dNextTick;
		g_MidiInfo.Track[bIdx].bPreStatus	= g_MidiInfo.Track[bIdx].bPreStatus;
		g_MidiInfo.Track[bIdx].pbCurnData	= g_MidiInfo.Track[bIdx].pbCurnData;
		g_MidiInfo.Track[bIdx].pbDataEnd	= g_MidiInfo.Track[bIdx].pbDataEnd;
		g_MidiInfo.Track[bIdx].pbDataHead	= g_MidiInfo.Track[bIdx].pbDataHead;
	}
*/
	g_MidiInfo.InfoBackup.dTicksPerQN 	= g_MidiInfo.dTicksPerQN;
	g_MidiInfo.InfoBackup.dTempoMS  	= g_MidiInfo.dTempoMS;
	g_MidiInfo.InfoBackup.dTrackEndH 	= g_MidiInfo.dTrackEndH;
	g_MidiInfo.InfoBackup.dTrackEndL	= g_MidiInfo.dTrackEndL;


	while(g_MidiInfo.dTrackEndH !=0xffffffff || g_MidiInfo.dTrackEndL!=0xffffffff)
	{
		bIdx = FindLeastTimeTrack();
		GetOneMessage(bIdx);
	}

	PFileInfo->dTotalTime = g_MidiInfo.dCurTime/1000;		//Changed by TangMian
	g_MidiInfo.dTotalTime = g_MidiInfo.dCurTime;

	for(bIdx=0; bIdx<g_MidiInfo.bTrackNum; bIdx++)
	{
		g_MidiInfo.Track[bIdx].dNextTick		= 0;	
		g_MidiInfo.Track[bIdx].bPreStatus		= 0xff;	
		g_MidiInfo.Track[bIdx].pbCurnData	= g_MidiInfo.Track[bIdx].pbDataHead;
	}

	g_MidiInfo.dTicksPerQN 	=  g_MidiInfo.InfoBackup.dTicksPerQN;
	g_MidiInfo.dTempoMS		= g_MidiInfo.InfoBackup.dTempoMS ;
	g_MidiInfo.dTrackEndH	= g_MidiInfo.InfoBackup.dTrackEndH;
	g_MidiInfo.dTrackEndL		= g_MidiInfo.InfoBackup.dTrackEndL;
	g_MidiInfo.dPreTime		= 0;
	g_MidiInfo.dCurTime		= 0;
	g_MidiInfo.dCurTick		= 0;

	g_MidiInfo.bMsgBuf[0] = 0;
	g_MidiInfo.bMsgBuf[1] = 0;
	g_MidiInfo.bMsgBuf[2] = 0;
	g_MidiInfo.bMsgBuf[3] = 0;
	g_MidiInfo.bMsgBuf[4] = 0;

	SetBeginTime();

	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Get current playback position. 
		
Parameters:
		pdCurTime:		Unit is millisecond.
		FiFoType:			Reserved;
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VMidi_GetCurPos(PUINT32 pdCurTime, VDEV_FIFO_TYPE FiFoType)
{
	*pdCurTime = (g_MidiInfo.dCurTime/1000);
	return VRES_SUCCESS;
}



/***************************************************************
Description:
		Seek MIDI to a specified position. 
		
Parameters:
		dPos:		Unit is millisecond.
		FiFoType:		Reserved.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT	VMidi_Seek(UINT32 dPos, VDEV_FIFO_TYPE FiFoType)
{
	if(dPos == 0)
		return VRES_SUCCESS;

	if((dPos*1000) >= g_MidiInfo.dTotalTime)
	{
		g_MidiInfo.dTrackEndH = 0xffffffff;
		g_MidiInfo.dTrackEndL = 0xffffffff;
		return VRES_SUCCESS;
	}

	VMidi_Reset(VDEV_FIFO_EVENT);

	while((dPos*1000) >= g_MidiInfo.dCurTime)
	{
		SeekMsg();
	}

	g_MidiInfo.dSeekOverTime= g_MidiInfo.dCurTime/1000 - dPos;
	AddMsgQue();

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
VRESULT VMidi_End(void)
{
    return VRES_SUCCESS;
}

/***************************************************************
Description:
		This function is unuseful.
		
Parameters:
		pbOutBuf:		Reserved.
		dBufLen:			Reserved.
		pdDataSize:		Reserved.
		FiFoType:			Reserved.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VMidi_ReadBlock(PUINT8 pbOutBuf, UINT32 dBufLen, PUINT32 pdDataSize, VDEV_FIFO_TYPE FiFoType)
{
	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Reset the parameters of Midi parser
		
Parameters:
		FiFoType:		Reserved.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VMidi_Reset(VDEV_FIFO_TYPE FiFoType)
{
	UINT8 bIdx;

	for(bIdx=0; bIdx<MAX_MIDI_TRACK; bIdx++)
	{
		g_MidiInfo.Track[bIdx].bPreStatus = 0xff;
		g_MidiInfo.Track[bIdx].dNextTick = 0;
		g_MidiInfo.Track[bIdx].pbCurnData = g_MidiInfo.Track[bIdx].pbDataHead;
	}

	for(bIdx=0; bIdx<MAX_MIDI_CHN; bIdx++)
	{
		g_MidiInfo.ChnInfo[bIdx].bTimbre	  	= 0x00;
		g_MidiInfo.ChnInfo[bIdx].bVolume     	= 0x7f;
		g_MidiInfo.ChnInfo[bIdx].bExpression 	= 0x7f;
		g_MidiInfo.ChnInfo[bIdx].bModulation 	= 0;
		g_MidiInfo.ChnInfo[bIdx].bPitchBendH 	= 0x40;
		g_MidiInfo.ChnInfo[bIdx].bPitchBendL 	= 0x00;
		g_MidiInfo.ChnInfo[bIdx].bBendRage  	= 0x02;  //bendrange= 0x02*100;
		g_MidiInfo.ChnInfo[bIdx].bPan     		= 0x40;
		g_MidiInfo.ChnInfo[bIdx].bHold1      	= 0x00;
		g_MidiInfo.ChnInfo[bIdx].bBankID      	= 0x00;

		if(bIdx==9)
			g_MidiInfo.ChnInfo[bIdx].bBankID      	= 0x80;
	}


	g_MidiInfo.bSendEOS		= 0;
	g_MidiInfo.bMsgQueNum	= 0;
	g_MidiInfo.dSeekOverTime = 0;
	g_MidiInfo.dTicksPerQN 	= g_MidiInfo.InfoBackup.dTicksPerQN;
	g_MidiInfo.dTempoMS		= g_MidiInfo.InfoBackup.dTempoMS ;
	g_MidiInfo.dTrackEndH	= g_MidiInfo.InfoBackup.dTrackEndH;
	g_MidiInfo.dTrackEndL		= g_MidiInfo.InfoBackup.dTrackEndL;
	g_MidiInfo.dPreTime		= 0;
	g_MidiInfo.dCurTime		= 0;
	g_MidiInfo.dCurTick		= 0;

	SetBeginTime();

	return VRES_SUCCESS;
}



/***************************************************************
Description:
		NONE
		
Parameters:
		void.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT VMidi_Unload(VDEV_FIFO_TYPE FiFoType)
{
	return VRES_SUCCESS;
}

