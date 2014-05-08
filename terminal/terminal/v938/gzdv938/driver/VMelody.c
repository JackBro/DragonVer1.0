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

	1.0			Basic version						2005.03.18	Zxh

				
-------------------------------------------------------------------------*/



#include "vmd.h"
//#include "VMelody.h"
#include "vparser.h"

#if VMD_PARSER_IMELODY_SUPPORT

static MELODYFILETRACK 	g_MelodyTrack[1];
static UINT32 g_dwCurIdx = 0;
static UINT32	g_dwTrackEnd = 0;
static UINT32 g_dwTotalTime = 0;
static UINT32 g_dwCurPos = 0;
static UINT32 g_dwIdx = 0;
static UINT32 g_dwRepeatBg = 0;
static UINT32 g_dwRepeatEd = 0;
static UINT32		g_dwDataLen = 0;
static UINT16	g_wDeltaTime = 0;
static UINT8 g_bBeat = 120;
static UINT8 g_bStyle = 0;
static UINT8 g_bGlobalVol = 119;
static UINT8	g_TrackNum = 1;
static UINT8  g_bEnd = 0;
static UINT8 g_bVol = 127;
static UINT8 g_bSel = 2;
static UINT8 g_bDot = 0;
static UINT8 g_bTime = 0;
static UINT8 g_bNote = 0;
static UINT8 g_bRepeatTimes = 0;
static UINT8 g_bCount = 0;
static UINT8	g_biMelodyEnd = 0;

static UINT8*	g_pImelodyFilePtr ;
static UINT32 	g_dwImelodyLen   ;

#define DUMP_EVENT 0

#if DUMP_EVENT
#include "stdio.h"
FILE *fp;
#endif


/***************************************************************
Description:
		Initialize the start time of each MIDI track.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
VRESULT VIMelody_Init(void)
{
#if DUMP_EVENT
	fp = fopen("melody.lst", "wb");
#endif
	g_bStyle = 0;
	g_bBeat = 120;
	g_bGlobalVol = 119;
	  g_TrackNum = 1;
	  g_dwTrackEnd = 0;
	  g_dwTotalTime = 0;

	  g_bEnd = 0;
	  g_wDeltaTime = 0;
	  g_dwIdx = 0;
	  g_bVol = 119;
	  g_bSel = 2;
	  g_bDot = 0;
	  g_bTime = 0;
	  g_bNote = 0;
	g_bRepeatTimes = 0;
	g_dwRepeatBg = 0;
	g_dwRepeatEd = 0;
	  g_bCount = 0;
	  g_dwDataLen = 0;

	g_pImelodyFilePtr = NULL;
	g_dwImelodyLen   = 0;

	g_biMelodyEnd = 0;
	
	return VRES_SUCCESS;
}
/***************************************************************
Description:
		Initialize the start time of each MIDI track.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
VRESULT VIMelody_Load(HUGE UINT8* fp, UINT32 dLen, UINT32 EmptyRes, UINT8 SrcFlag)
{
	UINT32	g_dwDataLen;
	UINT32		i = 0;
	HUGE UINT8 *ptr;

	ptr = (HUGE UINT8 *)fp;	

	if (ptr == NULL) return (VRES_ERROR);
	if (dLen == 0) return (VRES_ERROR);
	if (dLen < 22)  
	{
		return (VMAMIDI_INVALID_DATA);
	}

	g_pImelodyFilePtr  = fp;
	g_dwImelodyLen     = dLen;

	if ((ptr[6] == 'I') || (ptr[7] == 'M') || (ptr[8] == 'E') ||(ptr[9] == 'L')
		|| (ptr[10] == 'O') || (ptr[11] == 'D') ||(ptr[12] == 'Y'))
	{
		i = 14;
		while(i < (dLen - 7))
		{
			if (((ptr[i] == 0x0a) && (ptr[i+1] == 'M') && (ptr[i+2] == 'E') 
				&& (ptr[i+3] == 'L') && (ptr[i+4] == 'O') && (ptr[i+5] == 'D')
				&& (ptr[i+6] == 'Y'))
				||((ptr[i] == 0x0a) && (ptr[i+1] == 'M') && (ptr[i+2] == 'e') 
				&& (ptr[i+3] == 'l') && (ptr[i+4] == 'o') && (ptr[i+5] == 'd')
				&& (ptr[i+6] == 'y')))
			{
				g_dwDataLen = dLen - i - 23;
				g_MelodyTrack[0].pDataHead 	= (HUGE UINT8*)&ptr[i+8];         //&ptr[i+8];
				g_MelodyTrack[0].pCurnData 	= (HUGE UINT8*)&ptr[i+8];
				g_MelodyTrack[0].pDataEnd   = (HUGE UINT8*)&ptr[dLen-15];
				g_MelodyTrack[0].dwNextTick  = 0;
        g_MelodyTrack[0].preStatus	= 0xff;

				g_dwTrackEnd &= ~((UINT32)0x00000001<<0);
        g_TrackNum = 1;

				return (VRES_SUCCESS);
			}
			else if ((ptr[i] == 0x0a) && (ptr[i+1] == 'V') && (ptr[i+2] == 'O') 
				&& (ptr[i+3] == 'L') && (ptr[i+4] == 'U') && (ptr[i+5] == 'M')
				&& (ptr[i+6] == 'E')) 
			{
				if ((ptr[i+9] >= '0') && (ptr[i+9] <= '9'))
				{
					if ((ptr[i+10] >= '0') && (ptr[i+10] <= '5'))
					{
						g_bGlobalVol = (((ptr[i+9] - 0x30)*10) + (ptr[i+10] - 0x30));
						i += 11;
					}
					else
					{
						g_bGlobalVol = (ptr[i+9] - 0x30);     // 0-9
						i += 10;
					}
					g_bGlobalVol = ((g_bGlobalVol>>1)*17) + ((g_bGlobalVol - (((g_bGlobalVol>>1)<<1)))<<3);
				}
				else
					i += 8;
			}	
			else if ((ptr[i] == 0x0a) && (ptr[i+1] == 'B') && (ptr[i+2] == 'E') 
				&& (ptr[i+3] == 'A') && (ptr[i+4] == 'T')) 
			{
				if ((ptr[i+6] >= '0') && (ptr[i+6] <= '9'))
				{
					if ((ptr[i+7] >= '0') && (ptr[i+7] <= '9')) 
					{
						if ((ptr[i+8] >= '0') && (ptr[i+8] <= '9')) 
						{
							g_bBeat = ((ptr[i+6] - 0x30)*100) + ((ptr[i+7] - 0x30)*10) + (ptr[i+8] - 0x30);
							i += 9;
						}
						else
						{
							g_bBeat = ((ptr[i+6] - 0x30)*10) + (ptr[i+7] - 0x30);
							i += 8;
						}
					}
					else
					{
							g_bBeat = (ptr[i+6] - 0x30);
							i += 7;
					}
				}
				else
					i += 6;
			}
			else if ((ptr[i] == 0x0a) && (ptr[i+1] == 'S') && (ptr[i+2] == 'T') 
				&& (ptr[i+3] == 'Y') && (ptr[i+4] == 'L') && (ptr[i+5] == 'E')) 
			{
				g_bStyle = ptr[i+8] - 0x30;
				i += 9;
			}
      else
				i++;
		}
		if (i == (dLen - 7))
			return (VMAMIDI_INVALID_DATA);
	}
  else 
		return (VMAMIDI_INVALID_DATA);

	return VRES_SUCCESS;
}


VRESULT VIMelody_GetCurPos(UINT32 *PosPtr, VDEV_FIFO_TYPE FifoType)
{
	g_dwCurPos = g_dwCurIdx;
	*PosPtr = g_dwCurIdx;     
	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Initialize the start time of each MIDI track.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
UINT16 CalculateNoteOnDtime(UINT8 g_bDot, UINT8 g_bTime, UINT8 g_bBeat)
{
	UINT16 g_wDeltaTime = 0;
	if (g_bDot == 0) 
	{
		g_wDeltaTime = 730>>g_bTime;
		if (g_bTime >= 2) 
			g_wDeltaTime = g_wDeltaTime + 1;
	}
	else if (g_bDot == 1) 
	{
		g_wDeltaTime = (730>>g_bTime) + (730>>(g_bTime+1));
		if ((g_bTime == 1) || (g_bTime == 2) || (g_bTime == 3))
			g_wDeltaTime = g_wDeltaTime + 1;
		else if ((g_bTime == 4) || (g_bTime == 5))
			g_wDeltaTime = g_wDeltaTime + 2;
	}
	else if (g_bDot == 2) 
	{
		g_wDeltaTime = (730>>g_bTime) + (730>>(g_bTime+1)) + (730>>(g_bTime+2));
		if (g_bTime == 1)
			g_wDeltaTime = g_wDeltaTime + 1;
		else if((g_bTime == 2) || (g_bTime == 3) || (g_bTime == 4) || (g_bTime == 5))
			g_wDeltaTime = g_wDeltaTime + 2;
	}
	else if (g_bDot == 3) 
	{
		g_wDeltaTime = ((243>>g_bTime)<<1);
		if (g_bTime >= 2) 
			g_wDeltaTime = g_wDeltaTime + 1;
	}

	g_wDeltaTime = (g_wDeltaTime*60000)/(g_bBeat*192);
	return g_wDeltaTime;
}
/***************************************************************
Description:
		Initialize the start time of each MIDI track.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
UINT16 CalculateNoteOffDtime(UINT8 g_bDot, UINT8 g_bTime, UINT8 g_bBeat)
{
	UINT16 g_wDeltaTime = 0;
	if (g_bDot == 0) 
	{
		g_wDeltaTime = 38>>g_bTime;
	}
	else if (g_bDot == 1) 
	{
		g_wDeltaTime = (38>>g_bTime) + (38>>(g_bTime+1));
		if ((g_bTime == 2) || (g_bTime == 3))
			g_wDeltaTime = g_wDeltaTime + 1;
	}
	else if (g_bDot == 2) 
	{
		g_wDeltaTime = (38>>g_bTime) + (38>>(g_bTime+1)) + (38>>(g_bTime+2));
		if ((g_bTime == 1) ||(g_bTime == 2) || (g_bTime == 3) || (g_bTime == 4) || (g_bTime == 5))
			g_wDeltaTime = g_wDeltaTime + 1;
	}
	else if (g_bDot == 3) 
	{
		g_wDeltaTime = (38>>(g_bTime-1))/3;
	}	
  
	g_wDeltaTime = (g_wDeltaTime*60000)/(g_bBeat*192);
	return g_wDeltaTime;
}
/***************************************************************
Description:
		Initialize the start time of each MIDI track.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
UINT16 CalculatePauseDtime(UINT8 g_bDot, UINT8 g_bTime, UINT8 g_bBeat)
{
	UINT16 wTime = 0;

	if ((g_bTime == 0) || (g_bTime == 1))
	{
		if (g_bTime == 0) 
			g_bTime = 2;
		else
			g_bTime = 1;

		if (g_bDot == 0) 
			wTime = 192<<g_bTime;
		else if (g_bDot == 1) 
			wTime = (192<<g_bTime) + ((192<<g_bTime)>>1);
		else if (g_bDot == 2) 
			wTime = (192<<g_bTime) + ((192<<g_bTime)>>1) + ((192<<g_bTime)>>2);
		else if (g_bDot == 3) 
			wTime = (128<<g_bTime);
	}
	else if (g_bTime >= 2) 
	{
		if (g_bDot == 0)
			wTime = (192>>(g_bTime-2));
		else if (g_bDot == 1) 
			wTime = (192>>(g_bTime-2)) + (192>>(g_bTime-1));
		else if (g_bDot == 2) 
			wTime = (192>>(g_bTime-2)) + (192>>(g_bTime-1)) + (192>>g_bTime);
		else if (g_bDot == 3)
			wTime = (128>>(g_bTime-2));
	}

	wTime = (wTime*60000)/(g_bBeat*192);
	return wTime;
}
/***************************************************************
Description:
		Initialize the start time of each MIDI track.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
#if DUMP_EVENT
VRESULT PrintEvent(PVEVENT p)
{
	UINT8			bEvent[7];
	bEvent[0] = p->bMsgID;
	bEvent[1] = (UINT8)(p->sdDeltaTime);
	bEvent[2] = (UINT8)((p->sdDeltaTime>>8)&0xff);
	bEvent[3] = p->bChn;
	bEvent[4] = (UINT8)(p->dPara1);
	bEvent[5] = (UINT8)(p->dPara2);
	bEvent[6] = (UINT8)(p->dPara3);

	fp = fopen("melody.lst", "ab+");
	fwrite((void*)bEvent,1,7,fp);
	fclose(fp);
	return (VRES_SUCCESS);
}
#endif
/***************************************************************
Description:
		Initialize the start time of each MIDI track.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
VRESULT VIMelody_GetEvent(PVM_UNI_MSG p, VDEV_FIFO_TYPE FiFoType)
{	
	HUGE UINT8	*pData;
	UINT32	 i = 0;
  	UINT16 wTime = 0;
	UINT16 wTempTime = 0;
  	UINT8 bTmpNote = 60;
	
	p->sdDeltaTime  = 0;
	p->bMsgID	 = 0;
	p->bChn		 = 0;
	p->dPara1	 = 0;
	p->dPara2	 = 0;
	p->dPara3	 = 0;

	if (g_biMelodyEnd)
		return VRES_END_FLAG;
	
	g_dwDataLen = g_MelodyTrack[0].pDataEnd - g_MelodyTrack[0].pDataHead + 1;
	pData = g_MelodyTrack[0].pDataHead;

	if(g_dwCurPos != 0)
	{
		g_dwIdx = g_dwCurPos;
		g_dwCurPos = 0;
		g_bSel = 2;
	}
	
	g_dwCurIdx = g_dwIdx;
	i = g_dwIdx;
  	if (g_bSel == 2) 
	{
		p->bMsgID = VMD_MSG_PROGRAM_CHANGE;
		p->sdDeltaTime = 0;
		p->bChn = 0;
		p->dPara1 = 0;
		p->dPara2 = 20;
		p->dPara3 = 0;
		g_bSel = 3;
#if DUMP_EVENT
		PrintEvent(p);
#endif
		return (VRES_SUCCESS);
	}
	else if (g_bSel == 3) 
	{
		p->bMsgID = VMD_MSG_CHANNEL_VOLUME;
		p->sdDeltaTime = 0;
		p->bChn = 0;
		p->dPara1 = g_bGlobalVol;
		p->dPara2 = 0;
		p->dPara3 = 0;
		g_bSel = 1;
#if DUMP_EVENT
		PrintEvent(p);
#endif
		return (VRES_SUCCESS);
	}
	else if (g_bSel == 0) 
	{
		if (g_bStyle == 1) 
		{
			wTempTime = g_wDeltaTime; 
		}
		else if (g_bStyle == 0) 
		{
			p->sdDeltaTime = g_wDeltaTime;  
		}
    		else if (g_bStyle == 2) 
		{
			wTempTime = g_wDeltaTime;
    }

		p->bMsgID = VMD_MSG_NOTE_OFF;
		p->bChn = 0;
		p->dPara1 = g_bNote;
		p->dPara2 = 0;
		p->dPara3 = 0;

		g_wDeltaTime = CalculateNoteOffDtime(g_bDot, g_bTime, g_bBeat);
		g_bSel = 1;
		if (g_bStyle == 1) 
		{
			p->sdDeltaTime = wTempTime + g_wDeltaTime; 
			g_wDeltaTime = 0;
		}
		else if (g_bStyle == 2) 
		{
			p->sdDeltaTime = (wTempTime + g_wDeltaTime)>>1; 
			g_wDeltaTime = p->sdDeltaTime;
		}
#if DUMP_EVENT		
    PrintEvent(p);
#endif
		return (VRES_SUCCESS);
	}
	else if((i < g_dwDataLen) && (g_bSel == 1))
	{
		while (i < g_dwDataLen) 
		{
			g_bDot = 0;
			g_bTime = 0;
			if (pData[i] == '(') 
			{
				g_dwRepeatBg = i;
				g_dwIdx++;
			}
			else if (pData[i] == '@') 
			{
				if (g_bEnd == 0) 
				{
					if ((pData[i+1] >= '0') && (pData[i+1] <= '9'))
					{
						if ((pData[i+2] >= '0') && (pData[i+2] <= '9')) 
						{
							if ((pData[i+3] >= '0') && (pData[i+3] <= '9') && (pData[i+4] == ')')) 
							{
								g_bRepeatTimes = ((pData[i+1] - 0x30)*100) + ((pData[i+2] - 0x30)*10) 
									            + (pData[i+3] - 0x30);
								g_dwIdx += 5;
								g_bCount = 3;
							}
							else if (pData[i+3] == ')')
							{
								g_bRepeatTimes = ((pData[i+1] - 0x30)*10) + (pData[i+2] - 0x30);
								g_dwIdx += 4;
								g_bCount = 2;
							}
							else
							{
								g_bRepeatTimes = 255;
								g_dwIdx += 6;
								g_bCount = 4;
							}
						}
						else if(pData[i+2] == ')')
						{
							g_bRepeatTimes = (pData[i+1] - 0x30);
							g_dwIdx += 3;
							g_bCount = 1;
						}
					}
					if (g_bRepeatTimes != 0) 
					{
						g_bRepeatTimes--;
						g_dwRepeatEd = i+1;
						g_dwIdx = g_dwRepeatBg;
						g_dwDataLen = g_dwRepeatEd - g_dwRepeatBg + 1;
						g_bEnd = 1;
					}
					else
					{
						g_dwIdx = i + 3;
						g_bEnd = 0;
					}
				}
				else if ((g_bEnd == 1) && (g_bRepeatTimes > 0)) 
				{
					g_dwRepeatEd = i+1;
					g_dwIdx = g_dwRepeatBg;
					g_dwDataLen = g_dwRepeatEd - g_dwRepeatBg + 1;
					g_bRepeatTimes--;
				}
        				else if ((g_bEnd == 1) && (g_bRepeatTimes == 0)) 
				{
					g_dwIdx = g_dwRepeatEd + g_bCount + 1;
					g_bEnd = 0;
        				}
				else
				{
					g_dwIdx++;					
					return (VRES_SUCCESS);
				}
			}
			else if (((pData[i] == 'r') || (pData[i] == 'R')) 
				&& ((pData[i+1] >= 0x30) && (pData[i+1] <= 0x323535)))
			{
				g_bTime = (pData[i+1] - 0x30);
				g_dwIdx += 2;

				if ((pData[i+2] == '.')) 
				{
					g_bDot = 1;
					g_dwIdx += 1;
				}
				else if (pData[i+2] == ':') 
				{
					g_bDot = 2;
					g_dwIdx += 1;
				}
				else if (pData[i+2] == ';') 
				{
					g_bDot = 3;
					g_dwIdx += 1;
				}
				
				wTime = CalculatePauseDtime(g_bDot, g_bTime, g_bBeat);

				p->bMsgID = VMD_MSG_NOP;
				p->sdDeltaTime = wTime;
#if DUMP_EVENT
				PrintEvent(p);
#endif
				return (VRES_SUCCESS);
			}
			else if (pData[i] == '*')
			{
				if (pData[i+2] == '&')                 // *0&d5
				{
					if ((pData[i+3] == 0x61) || (pData[i+3] == 0x62))
					{
						g_bNote = ((pData[i+1] - 0x30 + 1)*12) + (((pData[i+3]-0x61)<<1) + 9);
					}
					else
						g_bNote = ((pData[i+1] - 0x30 + 1)*12) + ((pData[i+3] - 0x63)<<1);          // note
					
					if ((pData[i+3] == 0x66) || (pData[i+3] == 0x67))
						g_bNote -= 1;
								
					g_bNote = g_bNote - 1;
					g_bTime = (pData[i+4] - 0x30);         // 0-5
					g_dwIdx += 5;

					if ((pData[i+5] == '.'))
					{
						g_bDot = 1;
						g_dwIdx += 1;
					}
					else if (pData[i+5] == ':')
					{
						g_bDot = 2;
						g_dwIdx += 1;
					}
					else if (pData[i+5] == ';') 
					{
						g_bDot = 3;
						g_dwIdx += 1;
					}
				}
				else if (pData[i+2] == '#')                                //  *0#d5
				{
					if ((pData[i+3] == 0x61) || (pData[i+3] == 0x62))
					{
						g_bNote = ((pData[i+1] - 0x30 + 1)*12) + (((pData[i+3]-0x61)<<1) + 9);
					}
					else
						g_bNote = ((pData[i+1] - 0x30 + 1)*12) + ((pData[i+3] - 0x63)<<1);          // note
					
					if ((pData[i+3] == 0x66) || (pData[i+3] == 0x67))
						g_bNote -= 1;
								
					g_bNote = g_bNote + 1;
					g_bTime = (pData[i+4] - 0x30);
					g_dwIdx += 5;

					if ((pData[i+5] == '.')) 
					{
						g_bDot = 1;
						g_dwIdx += 1;
					}
					else if (pData[i+5] == ':') 
					{
						g_bDot = 2;
						g_dwIdx += 1;
					}
					else if (pData[i+5] == ';') 
					{
						g_bDot = 3;
						g_dwIdx += 1;
					}
					
				}
				else if (((pData[i+2] >= 'a') && (pData[i+2] <= 'g')) 
					|| ((pData[i+2] >= 'A') && (pData[i+2] <= 'G')))
				{
					if ((pData[i+2] >= 'A') && (pData[i+2] <= 'G')) 
					{
						pData[i+2] += 0x20;
					}

					if ((pData[i+2] == 0x61) || (pData[i+2] == 0x62))
					{
						g_bNote = ((pData[i+1]-0x30 + 1)*12) + (((pData[i+2]-0x61)<<1) + 9);
					}
					else
						g_bNote = ((pData[i+1] - 0x30 + 1)*12) + ((pData[i+2] - 0x63)<<1);          // note
					
					if ((pData[i+2] == 0x66) || (pData[i+2] == 0x67))
						g_bNote -= 1;

					g_bTime = (pData[i+3] - 0x30);
					g_dwIdx += 4;
					if ((pData[i+4] == '.')) 
					{
						g_bDot = 1;
						g_dwIdx += 1;
					}
					else if (pData[i+4] == ':') 
					{
						g_bDot = 2;
						g_dwIdx += 1;
					}
					else if (pData[i+4] == ';') 
					{
						g_bDot = 3;
						g_dwIdx += 1;
					}
				}
				else
				{
					g_dwIdx++;					
					return (VRES_SUCCESS);
				}

				if (g_bSel == 1) 
				{
					p->bMsgID = VMD_MSG_NOTE_ON; 
					p->sdDeltaTime = g_wDeltaTime;
					p->bChn = 0;
					p->dPara1 = g_bNote;
					p->dPara2 = 127;
					p->dPara3 = 0;

					g_wDeltaTime = CalculateNoteOnDtime(g_bDot, g_bTime, g_bBeat);
					g_bSel = 0;
#if DUMP_EVENT
					PrintEvent(p);
#endif
					return (VRES_SUCCESS);
				}
			}
			else if (((pData[i] == 'V') || (pData[i] == 'v')) 
			&& (((pData[i+1] >= '0') && (pData[i+1] <= '9')) || ((pData[i+1] == '+') && (pData[i+1] == '-'))))
			{
				if ((pData[i+1] >= '0') && (pData[i+1] <= '9'))
				{
					if ((pData[i+2] >= '0') && (pData[i+2] <= '5')) 
					{
						g_bVol = (((pData[i+1] - 0x30)*10) + (pData[i+2] - 0x30));
						g_bVol = ((g_bVol>>1)*17) + ((g_bVol - (((g_bVol>>1)<<1)))<<3);
						g_dwIdx += 3;
					}
					else
					{
						g_bVol = (pData[i+1] - 0x30);     // 0-9
						g_bVol = ((g_bVol>>1)*17) + ((g_bVol - (((g_bVol>>1)<<1)))<<3);
						g_dwIdx += 2;
					}
                                             if(g_bVol > 127)
					  g_bVol = 127;

					p->bMsgID = VMD_MSG_CHANNEL_VOLUME;
					p->sdDeltaTime = g_wDeltaTime;  
					p->bChn = 0;
					p->dPara1 = g_bVol;
					p->dPara2 = 0;
					p->dPara3 = 0;
					g_wDeltaTime = 0;

					if (pData[g_dwIdx+2] == 'M') 
						g_dwIdx += 9;
#if DUMP_EVENT
					PrintEvent(p);
#endif
					return (VRES_SUCCESS);
				}
				else if (pData[i+1] == '+')
				{
					if ((g_bVol - (g_bVol/17)*17) == 0) 		
						g_bVol += 8;
					else
						g_bVol += 9;
					if (g_bVol > 127) 
						g_bVol = 127;

					g_dwIdx += 2;

					p->bMsgID = VMD_MSG_CHANNEL_VOLUME;
					p->sdDeltaTime = g_wDeltaTime;  
					p->bChn = 0;
					p->dPara1 = g_bVol;
					p->dPara2 = 0;
					p->dPara3 = 0;
					g_wDeltaTime = 0;
#if DUMP_EVENT
					PrintEvent(p);
#endif
					return (VRES_SUCCESS);
				}
				else if (pData[i+1] == '-') 
				{
					if ((g_bVol - (g_bVol/17)*17) == 0) 		
						g_bVol -= 9;
					else
						g_bVol -= 8;
					g_dwIdx += 2;

					p->bMsgID = VMD_MSG_CHANNEL_VOLUME;
					p->sdDeltaTime = g_wDeltaTime;  
					p->bChn = 0;
					p->dPara1 = g_bVol;
					p->dPara2 = 0;
					p->dPara3 = 0;
					g_wDeltaTime = 0;
#if DUMP_EVENT
					PrintEvent(p);
#endif
					return (VRES_SUCCESS);
				}
				else
				{
					g_dwIdx++;					
					return (VRES_SUCCESS);
				}
			}
			else if ((pData[i] == '#') || (pData[i] == '&') 
 			|| ((((pData[i] >= 'a') && (pData[i] <= 'g')) || ((pData[i] >= 'A') && (pData[i] <= 'G'))) 
 			&& (pData[i+1] >= '0') && (pData[i+1] <= '5')))
			{
				if ((pData[i] == '#') || (pData[i] == '&'))
				{
	 				if ((i >= 4) && (pData[i-1] >= '0') && (pData[i-1] <= '8') 
					&& (((pData[i-2] == 0x0) && (pData[i-3] == 0x0) && (pData[i-4] == '*'))
					|| ((pData[i-2] == 0x0) && (pData[i-3] == 0x0) && (pData[i-4] == 0x0) && (pData[i-5] == '*'))))
					{
						bTmpNote = ((pData[i-1]-0x30 + 1)*12);
          }
					else
						bTmpNote = 60;

					if ((pData[i+1] == 0x61) || (pData[i+1] == 0x62))
					{
						g_bNote = bTmpNote + (((pData[i+1]-0x61)<<1) + 9);
					}
					else
						g_bNote = bTmpNote + ((pData[i+1] - 0x63)<<1);          // note
					
					if ((pData[i+1] == 0x66) || (pData[i+1] == 0x67))
						g_bNote -= 1;
								
					if (pData[i] == '#') 
						g_bNote = g_bNote + 1;
					else if (pData[i] == '#')
						g_bNote = g_bNote - 1;

					g_bTime = (pData[i+2] - 0x30);
					g_dwIdx += 3;

					if ((pData[i+3] == '.')) 
					{
						g_bDot = 1;
						g_dwIdx += 1;
					}
					else if (pData[i+3] == ':') 
					{
						g_bDot = 2;
						g_dwIdx += 1;
					}
					else if (pData[i+3] == ';') 
					{
						g_bDot = 3;
						g_dwIdx += 1;
					}
				}
				else if (((pData[i] >= 'a') && (pData[i] <= 'g')) 
					|| ((pData[i] >= 'A') && (pData[i] <= 'G')))
				{
					if ((pData[i] >= 'A') && (pData[i] <= 'G')) 
					{
						pData[i] += 0x20;
					}
 
					 if ((i >= 4) && (pData[i-1] >= '0') && (pData[i-1] <= '8') 
					&& (((pData[i-2] == 0x0a) && (pData[i-3] == 0x0d) && (pData[i-4] == '*'))
					|| ((pData[i-2] == 0x20) && (pData[i-3] == 0x0a) && (pData[i-4] == 0x0d) && (pData[i-5] == '*'))))
						{
							bTmpNote = ((pData[i-1]-0x30 + 1)*12);
						}
						else
							bTmpNote = 60;
						
					if ((pData[i] == 0x61) || (pData[i] == 0x62))
					{
						g_bNote =  bTmpNote + (((pData[i]-0x61)<<1) + 9);
					}
					else
						g_bNote =  bTmpNote + ((pData[i] - 0x63)<<1);          // note
					
					if ((pData[i] == 0x66) || (pData[i] == 0x67))
						g_bNote -= 1;

					g_bTime = (pData[i+1] - 0x30);
					g_dwIdx += 2;
					if ((pData[i+2] == '.')) 
					{
						g_bDot = 1;
						g_dwIdx += 1;
					}
					else if (pData[i+2] == ':') 
					{
						g_bDot = 2;
						g_dwIdx += 1;
					}
					else if (pData[i+2] == ';') 
					{
						g_bDot = 3;
						g_dwIdx += 1;
					}
				}
				else
				{
					g_dwIdx++;					
					return (VRES_SUCCESS);
				}
				if (g_bSel == 1) 
				{
					p->bMsgID = VMD_MSG_NOTE_ON; 
					p->sdDeltaTime = g_wDeltaTime;
					p->bChn = 0;
					p->dPara1 = g_bNote;
					p->dPara2 = 127;
					p->dPara3 = 0;

					g_wDeltaTime = CalculateNoteOnDtime(g_bDot, g_bTime, g_bBeat);
					g_bSel = 0;
#if DUMP_EVENT
					PrintEvent(p);
#endif
					return (VRES_SUCCESS);
				}
			}
			else if ((pData[i] == '+') || (pData[i] == '-')) 
			{
				if (pData[i] == '+')
				{
					if ((g_bVol - (g_bVol/17)*17) == 0) 		
						g_bVol += 8;
					else
						g_bVol += 9;
					if (g_bVol > 127) 
						g_bVol = 127;
					
					g_dwIdx += 1;

					p->bMsgID = VMD_MSG_CHANNEL_VOLUME;
					p->sdDeltaTime = g_wDeltaTime;  
					p->bChn = 0;
					p->dPara1 = g_bVol;
					p->dPara2 = 0;
					p->dPara3 = 0;
					g_wDeltaTime = 0;
#if DUMP_EVENT
					PrintEvent(p);
#endif
					return (VRES_SUCCESS);
				}
				else 
				{
					if ((g_bVol - (g_bVol/17)*17) == 0) 		
						g_bVol -= 9;
					else
						g_bVol -= 8;
					g_dwIdx += 1;

					p->bMsgID = VMD_MSG_CHANNEL_VOLUME;
					p->sdDeltaTime = g_wDeltaTime;  
					p->bChn = 0;
					p->dPara1 = g_bVol;
					p->dPara2 = 0;
					p->dPara3 = 0;
					g_wDeltaTime = 0;
#if DUMP_EVENT
					PrintEvent(p);
#endif
					return (VRES_SUCCESS);
				}
			}
			else if (((pData[i] == 'l') || (pData[i] == 'L')) 
				&& ((pData[i+1] == 'e') || (pData[i+1] == 'E')))
			{
				if ((pData[i+4] == 'n') || (pData[i+4] == 'N'))
				{				
					p->bMsgID = VMD_MSG_LED_ON; 				
					p->sdDeltaTime = 0;
				  p->bChn = 0;		// bColor: 0/1/2
					g_dwIdx = g_dwIdx + 5;
				}
				else if ((pData[i+4] == 'f') || (pData[i+4] == 'F'))
				{
					p->bMsgID = VMD_MSG_LED_OFF; 				
					p->sdDeltaTime = 0;
				  p->bChn = 0;		// bColor: 0/1/2
					g_dwIdx = g_dwIdx + 6;
				}
				else
				{
					g_dwIdx++;					
					return (VRES_SUCCESS);
				}	
	//			PrintEvent(p);
				return (VRES_SUCCESS);			
			}
			else if (((pData[i] == 'v') || (pData[i] == 'V')) 
				&& ((pData[i+1] == 'i') || (pData[i+1] == 'I')))
			{
				if ((pData[i+5] == 'n') || (pData[i+5] == 'N'))
				{
					p->bMsgID = VMD_MSG_MOTOR_ON; 				
					p->sdDeltaTime = 0;
					g_dwIdx = g_dwIdx + 6;
				}
				else if ((pData[i+5] == 'f') || (pData[i+5] == 'F'))
				{
					p->bMsgID = VMD_MSG_MOTOR_OFF; 				
					p->sdDeltaTime = 0;
					g_dwIdx = g_dwIdx + 7;
				}
				else
				{
					g_dwIdx++;					
					return (VRES_SUCCESS);
				}	
		//		PrintEvent(p);
				return (VRES_SUCCESS);
			}
			else if ((pData[i] == 'b') && (pData[i+1] == 'a')) 
			{
				if (pData[i+5] == 'n')
				{
					g_dwIdx = g_dwIdx + 6;
				}
				else if (pData[i+5] == 'f')
				{
					g_dwIdx = g_dwIdx + 7;
				}
				//PrintEvent(p);
				return (VRES_SUCCESS);
			}

			else
				g_dwIdx++;
			i = g_dwIdx;
		}
	}	
  	else if(i >= g_dwDataLen)
	{
		p->bMsgID = VMD_MSG_EOS_EVENT; 
		p->sdDeltaTime = 0;
		p->dPara1 = 127;
#if DUMP_EVENT
		PrintEvent(p);		
#endif
		g_biMelodyEnd = 1;
//		return (VRES_END_FLAG);
	}

	return (VRES_SUCCESS);
}


/***************************************************************
Description:
		reset  the iMelody parser.
		
Parameters:
		void.
		
Returns:
		void.
		
****************************************************************/
VRESULT VIMelody_Reset(VDEV_FIFO_TYPE FiFoType)
{
	  g_dwIdx = 0;
	  g_biMelodyEnd = 0;
	  
/*	g_bGlobalVol = 119;
	  g_TrackNum = 1;
	  g_dwTrackEnd = 0;

	  g_bEnd = 0;
	  g_wDeltaTime = 0;
	  g_dwIdx = 0;
	  g_bSel = 2;
	  g_bDot = 0;
	  g_bTime = 0;
	  g_bNote = 0;
	g_bRepeatTimes = 0;
	g_dwRepeatBg = 0;
	g_dwRepeatEd = 0;
	  g_bCount = 0;
	  g_dwDataLen = 0;

	VIMelody_Load(g_pImelodyFilePtr, g_dwImelodyLen, 0, 0);
*/	
	return VRES_SUCCESS;
}


#endif //VMD_PARSER_IMELODY_SUPPORT
