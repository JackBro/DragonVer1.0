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

	1.0			Basic Version						2004.11.5	ZY

	1.1			Add g_FirstEvent initialize			2004.11.29	gzd

	1.2												2004.12.09	gzd
				Modify calculate bHardChn value in function Vcnt_StreamOn().

	1.3												2004.12.12	gzd
				Add stream end flag in Vcnt_StreamOff() when play back vmd. 		

	1.4												2004.12.24	gzd
				Fix a bug about more added deltatime in Vcnt_Mp3On() function.

	1.5												2005.01.11	gzd
				change nop operation in function Vcnt_Nop() for vmd file(include
				long wave or mp3).
	1.51												2005.01.19	ZY
				Support  the DeltaTime over 0xffff
	1.52                       Add VC0937 play back mode,but not open default				2005.02.01        Andy,liu

	1.53												2005.03.18	gzd
				Modify Motor control flow.
				
-------------------------------------------------------------------------*/

#include "vmd.h"
#include "vconv.h"
#include "vsyn.h"
#include "vHostCtrl.h"



static VM_CNVT	g_CnvtInfo;

static	VM_HARD_CHN		  g_HardChnPara[MAX_MUSIC_VOICE];

static void 	Vcnt_Reset(void *pvMsg);
static void 	Vcnt_Control(void *pvMsg);
static void 	Vcnt_Special(void *pvMsg);
static void 	Vcnt_SendEvent(UINT8 bLen, void *pvReg);

static void   	Vcnt_SendGlobalEvent(UINT16 wDeltaTime, UINT8 bPage, UINT8 bReg, UINT8 bVal);
static void 	Vcnt_SendStreamEvent(UINT16 wDeltaTime, UINT8 bChn, void *pvReg);

static void Vcnt_NoteOn(void *pvMsg);
static void Vcnt_NoteOff(void *pvMsg);
static void Vcnt_Program(void *pvMsg);
static void Vcnt_Modulation(void *pvMsg);
static void Vcnt_Volume(void *pvMsg);
static void Vcnt_Pan(void *pvMsg);
static void Vcnt_Expression(void *pvMsg);
static void Vcnt_Hold1(void *pvMsg);
static void Vcnt_BendRange(void *pvMsg);
static void Vcnt_PitchBend(void *pvMsg);
static void Vcnt_Pressure(void *pvMsg);
static void Vcnt_ResetControllers(void *pvMsg);
static void Vcnt_AllNoteOff(void *pvMsg);
static void Vcnt_AllSoundOff(void *pvMsg);
static void Vcnt_Bank(void *pvMsg);
static void Vcnt_Nop(void *pvMsg);

// zou
static void Vcnt_StreamOn(void *pvMsg);
static void Vcnt_StreamOff(void *pvMsg);
static void Vcnt_ReverbOn(void *pvMsg);
static void Vcnt_ReverbOff(void *pvMsg);
static void Vcnt_ReverbEnable(void *pvMsg);
static void Vcnt_MasterVolume(void *pvMsg);
static void Vcnt_EosEvent(void *pvMsg);
static void Vcnt_Mp3On(void *pvMsg);
static void Vcnt_Mp3Off(void *pvMsg);
static void Vcnt_SetPage(void *pvMsg);
static void Vcnt_SoftInt(void *pvMsg);
static void Vcnt_IntEnable(void *pvMsg);
static void Vcnt_IntDisable(void *pvMsg);

VRESULT Vcnt_GetAddr(PVM_FIFOADDR pVm_fifoAddr, UINT8 bNum, UINT8 bChn, UINT8 bBitsPerSam, UINT8 bChnNum);

extern UINT16		g_SFifoBAddr[4][4];
extern UINT16		g_SFifoEndPtr[4];
extern UINT8		g_bMotorState;

UINT8		g_FirstEvent;

/***************************************************************
Description:
		Convert unified music message to hardware .
				
Parameters:
		pbMsg:		The pointer to the structure of unified music message.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT Vcnt_Convert(void *pvMsg,  UINT8* pbOutBuf, UINT16* pwEventLen)
{
		PVM_UNI_MSG pMsg;
		VM_SEND_MSG  sendMsg;
		PVM_SEND_MSG  pSendMsg = &sendMsg;
	
		pMsg = (PVM_UNI_MSG)pvMsg;
	 
		if(pbOutBuf==NULL)
			return VRES_ERROR;
		else
			g_CnvtInfo.pbEventBuf = pbOutBuf;
	
		pMsg->sdDeltaTime = (pMsg->sdDeltaTime<0)?0:pMsg->sdDeltaTime;
		
		if(g_FirstEvent==0)
		{
	//------------------------------//
/*			pSendMsg->bMsgID = 0x41;
			pSendMsg->wDeltaTime =0;	
			pSendMsg->bPara1 = 5;
			pSendMsg->bPara2 = 0;
			pSendMsg->bPara3 = 0;
		
			Vcnt_SendEvent(6, (void*)pSendMsg);
*/		
				pSendMsg->bMsgID = 0x41;		//Led
				pSendMsg->wDeltaTime =	  0; 
				pSendMsg->bPara1 = (g_CnvtInfo.bLedEnable)?5:6;
				pSendMsg->bPara2 = (UINT8)g_CnvtInfo.bLedSynChn;
				pSendMsg->bPara3 = (UINT8)0;
					
				Vcnt_SendEvent(6, (void*)pSendMsg);

			if (g_bMotorState) 
			{
/*
				pSendMsg->bMsgID = 0x41;
				pSendMsg->wDeltaTime =0;	
				pSendMsg->bPara1 = 7;
				pSendMsg->bPara2 = 0;
				pSendMsg->bPara3 = 1;
			
				Vcnt_SendEvent(6, (void*)pSendMsg);
*/
				pSendMsg->bMsgID = 0x41;		//Motor
				pSendMsg->wDeltaTime =	  0; 
				pSendMsg->bPara1 = (g_CnvtInfo.bMotorEnable)?7:8;
				pSendMsg->bPara2 = (UINT8)g_CnvtInfo.bMotorSynChn;
				pSendMsg->bPara3 = (UINT8)g_CnvtInfo.bMotorInterval;
				
				Vcnt_SendEvent(6, (void*)pSendMsg);
			}
#if (AUDIO_CHIP==CHIP_VC0937)
#if VC0937_PLAYBACK
			//Start ADC play back mode
			pSendMsg->bMsgID = 0x50;			//ADC playback
			pSendMsg->wDeltaTime =	  0;                			
			pSendMsg->bPara1 = 0x02;
			pSendMsg->bPara2 = 0x01;
			pSendMsg->bPara3 = 0x04;
			
			Vcnt_SendEvent(6, (void*)pSendMsg);
			
			//Start timer1 for ADC play back
			pSendMsg->bMsgID = 0x48;			//Timer1 start
			pSendMsg->wDeltaTime =	  0; 
			pSendMsg->bPara1 = 0x0A;
			pSendMsg->bPara2 = 0x30;
			
			Vcnt_SendEvent(5, (void*)pSendMsg);
#endif
#endif
			g_FirstEvent = 1;
		}

	switch(pMsg->bMsgID)
	{
	case VMD_MSG_NOTE_ON:
		Vcnt_NoteOn(pvMsg);
		break;
	case VMD_MSG_NOTE_OFF:
		Vcnt_NoteOff(pvMsg);
		break;
	case VMD_MSG_PROGRAM_CHANGE:
		Vcnt_Program(pvMsg);
		break;
	case VMD_MSG_MODULATION_DEPTH	:
		Vcnt_Modulation(pvMsg);
		break;
	case VMD_MSG_CHANNEL_VOLUME:
		Vcnt_Volume(pvMsg);
		break;
	case VMD_MSG_PANPOT:
		Vcnt_Pan(pvMsg);
		break;
	case VMD_MSG_EXPRESSION:
		Vcnt_Expression(pvMsg);
		break;
	case VMD_MSG_HOLD1:
		Vcnt_Hold1(pvMsg);
		break;
	case VMD_MSG_PITCH_BEND:
		Vcnt_PitchBend(pvMsg);
		break;
	case VMD_MSG_BEND_RANGE:
		Vcnt_BendRange(pvMsg);
		break;
	case VMD_MSG_CHANNEL_PRESSURE:
		Vcnt_Pressure(pvMsg);
		break;
	case VMD_MSG_RESET_ALL_CONTROLLERS:
		Vcnt_ResetControllers(pvMsg);
		break;
	case VMD_MSG_ALL_NOTE_OFF:
		Vcnt_AllNoteOff(pvMsg);
		break;
	case VMD_MSG_ALL_SOUND_OFF:
		Vcnt_AllSoundOff(pvMsg);
		break;
	case VMD_MSG_NOP:
		Vcnt_Nop(pvMsg);
		break;
	case VMD_MSG_BANK_SELECT:
		Vcnt_Bank(pvMsg);
		break;
// zou
	case VMD_MSG_STREAM_ON:
		Vcnt_StreamOn(pvMsg);
		break;
	case VMD_MSG_STREAM_OFF:
		Vcnt_StreamOff(pvMsg);
		break;
		break;
	case VMD_MSG_REVERVB_ON:
		Vcnt_ReverbOn(pvMsg);
		break;
	case VMD_MSG_REVERVB_OFF:
		Vcnt_ReverbOff(pvMsg);
		break;
	case VMD_MSG_REVERVB_ENABLE:
		Vcnt_ReverbEnable(pvMsg);
		break;
	case VMD_MSG_MASTER_VOLUME:
		Vcnt_MasterVolume(pvMsg);
		break;
	case VMD_MSG_EOS_EVENT:
		Vcnt_EosEvent(pvMsg);
		break;	
	case VMD_MSG_MP3_ON:
		Vcnt_Mp3On(pvMsg);
		break;	
	case VMD_MSG_MP3_OFF:
		Vcnt_Mp3Off(pvMsg);
		break;
    case VMD_MSG_SET_REG_PAGE:
        Vcnt_SetPage(pvMsg);
        break;	
    case VMD_MSG_SOFT_INT:
        Vcnt_SoftInt(pvMsg);
        break;
    case VMD_MSG_INT_ENABLE:
        Vcnt_IntEnable(pvMsg);
        break;	
    case VMD_MSG_INT_DISABLE:
        Vcnt_IntDisable(pvMsg);
        break;
		
	case VMD_MSG_RESET:
		Vcnt_Reset(pvMsg);
		break;
	case VMD_MSG_CONTROL:
		Vcnt_Control(pvMsg);
		break;	
	case VMD_MSG_SPEC_EVENT:
		Vcnt_Special(pvMsg);
		break;
	default:
		break;
	}
	
	*pwEventLen = g_CnvtInfo.wEventLen;
	g_CnvtInfo.wEventLen = 0;

	return VRES_SUCCESS;
}		



/***************************************************************
Description:
		Reset the globel parameters of converter.
					
Parameters:
		FiFoType: 	Resource type.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT Vcnt_ConverterReset(VDEV_FIFO_TYPE FiFoType)
{
	g_FirstEvent = 0;

	return VRES_SUCCESS;
}

/***************************************************************
Description:
		Initialize the globel parameters of converter.
					
Parameters:
		void.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VRESULT Vcnt_ConverterInit()
{
	g_FirstEvent = 0;

	return VRES_SUCCESS;
}


/***************************************************************
Description:
		Control the parameters of converter.
					
Parameters:
		void.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
VOID Vcnt_ConverterControl(UINT8 bCtrlID, UINT32 dParam1, UINT32 dParam2, UINT32 dParam3)
{
	(void)dParam2;
	
	switch(bCtrlID)
	{
	case VCONV_CTRL_MIDI_LED:
		g_CnvtInfo.bLedEnable	= (UINT8)dParam1;
		g_CnvtInfo.bLedSynChn	= (UINT8)dParam2;
		break;
	case VCONV_CTRL_MIDI_MOTOR:
		g_CnvtInfo.bMotorEnable	 = (UINT8)dParam1;
		g_CnvtInfo.bMotorSynChn	 = (UINT8)dParam2;
		g_CnvtInfo.bMotorInterval	 = (UINT8)dParam3;
		break;

	default:
		break;
	}

	return ;
}

/***************************************************************
Description:
		Convert Midi Note on message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.
		
Returns:
		void.
		
****************************************************************/
static void Vcnt_NoteOn(void *pvMsg)
{
  	UINT16  wDeltaTime;

	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;

	PVM_UNI_MSG		  pMsg = (PVM_UNI_MSG)pvMsg;

	wDeltaTime  = (UINT16)(pMsg->sdDeltaTime&0xffff);

	g_CnvtInfo.dMissTime += wDeltaTime;
	wDeltaTime = 0;

	pSendMsg->bMsgID = 0x10;
	pSendMsg->wDeltaTime =	  wDeltaTime; 
	pSendMsg->bPara1 = pMsg->bChn;
	pSendMsg->bPara2 = (UINT8)pMsg->dPara1;
	pSendMsg->bPara3 = (UINT8)pMsg->dPara2;

	Vcnt_SendEvent(6, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi Note off message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_NoteOff(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;	  
  
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;

	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;

	pSendMsg->bMsgID = 0x11;
	pSendMsg->wDeltaTime = (UINT16)pMsg->sdDeltaTime;   
	pSendMsg->bPara1 = pMsg->bChn;
	pSendMsg->bPara2 = (UINT8)pMsg->dPara1;
	pSendMsg->bPara3 = (UINT8)pMsg->dPara2;

	Vcnt_SendEvent(6, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi Note off message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_Program(void *pvMsg)
{
    VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	
	  PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;
	  g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
		   pMsg->sdDeltaTime = 0;
	
	  pSendMsg->bMsgID = 0x12;
	  pSendMsg->wDeltaTime = (UINT16)pMsg->sdDeltaTime;   
	  pSendMsg->bPara1 = pMsg->bChn;
	  pSendMsg->bPara2 = (UINT8)pMsg->dPara1;
	
	  Vcnt_SendEvent(5, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi modulation change message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_Modulation(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;

	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;
	
	pSendMsg->bMsgID = 0x13;
	pSendMsg->wDeltaTime =  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = pMsg->bChn;
	pSendMsg->bPara2 = (UINT8)(pMsg->dPara1 & 0xff);//ZY:10.16

	Vcnt_SendEvent(5, (void*)pSendMsg);
}



/***************************************************************
Description:
		Convert Midi volume change message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_Volume(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;

	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;

	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;

	pSendMsg->bMsgID = 0x14;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = pMsg->bChn;
	pSendMsg->bPara2 = (UINT8)(pMsg->dPara1);

	Vcnt_SendEvent(5, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi pan change message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.
		
Returns:
		void.
		
****************************************************************/
void Vcnt_Pan(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;

	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;

	pSendMsg->bMsgID = 0x15;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = pMsg->bChn;
	pSendMsg->bPara2 = (UINT8)(pMsg->dPara1);

	Vcnt_SendEvent(5, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi expression change message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_Expression(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;

	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;

	pSendMsg->bMsgID = 0x16;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = pMsg->bChn;
	pSendMsg->bPara2 = (UINT8)(pMsg->dPara1);

	Vcnt_SendEvent(5, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi hold1 message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_Hold1(void *pvMsg)
{
	UINT8   bHold1;
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;
	
	bHold1 = (pMsg->dPara1==0)? 0 : 1;
	
	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;
	
	pSendMsg->bMsgID = 0x17;
	pSendMsg->wDeltaTime =  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = pMsg->bChn;
	pSendMsg->bPara2 = bHold1;
	
	Vcnt_SendEvent(5, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi pitch bend message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_PitchBend(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;
	
	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;
	
	pSendMsg->bMsgID = 0x18;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = pMsg->bChn;
	pSendMsg->bPara2 = (UINT8)(pMsg->dPara1&0x7f);			// PitchBendL	  
	pSendMsg->bPara3 = (UINT8)(pMsg->dPara1>>7);			 // PitchBendH
	
	Vcnt_SendEvent(6, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi bend range change message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_BendRange(void *pvMsg)
{
	VM_SEND_MSG	sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;
	
	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;
	
	pSendMsg->bMsgID = 0x19;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = pMsg->bChn;
	pSendMsg->bPara2 = (UINT8)pMsg->dPara1;
	
	Vcnt_SendEvent(5, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi pressure change message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_Pressure(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;
	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;

	pSendMsg->bMsgID = 0x1a;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = pMsg->bChn;
	pSendMsg->bPara2 = (UINT8)pMsg->dPara1;

	Vcnt_SendEvent(5, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi reset all controllers message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_ResetControllers(void *pvMsg)
{
	VM_SEND_MSG   sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;
	  
	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;
	
	pSendMsg->bMsgID = 0x1b;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = pMsg->bChn;
	
	Vcnt_SendEvent(4, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert Midi all note off message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_AllNoteOff(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;
	
	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;
	
	pSendMsg->bMsgID = 0x1c;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = 0;
	
	Vcnt_SendEvent(4, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert  all sound off message, including stream/mp3/reverb channel.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_AllSoundOff(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;
	
	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;
	
	pSendMsg->bMsgID = 0x1d;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = 0;
	
	Vcnt_SendEvent(4, (void*)pSendMsg);
}


/***************************************************************
Description:
		Convert  NOP message.
				
Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_Nop(void *pvMsg)
{
//	VM_SEND_MSG  sendMsg;
//	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG	  pMsg = (PVM_UNI_MSG)pvMsg;
	g_CnvtInfo.dMissTime += pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;

	Vcnt_SendGlobalEvent(0, 1, 0x39, 0x05);
	
/*	pSendMsg->bMsgID = 0x1e;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = 0;
	
	Vcnt_SendEvent(4, (void*)pSendMsg);
*/
}



/***************************************************************
Description:
		Convert  MIDI bank select change message.

Parameters:
		pvMsg:		Pointer to the unified music message.

Returns:
		void.
		
****************************************************************/
void Vcnt_Bank(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG   pMsg = (PVM_UNI_MSG)pvMsg;
	g_CnvtInfo.dMissTime += (UINT16)pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;
	
	pSendMsg->bMsgID = 0x1f;
	pSendMsg->wDeltaTime =	  (UINT16)pMsg->sdDeltaTime;  
	pSendMsg->bPara1 = (UINT8)pMsg->bChn;		// add by stavy.sun
	pSendMsg->bPara2 = (UINT8)pMsg->dPara1;	// bankID
	
	Vcnt_SendEvent(5, (void*)pSendMsg);	//modified by stavy.sun
}

/***************************************************************
Description:
		Send Midi note on hard event to syn buffer.
				
Parameters:
		wDeltaTime:	Delta Time.
		bChn:		Hard channel.
		pvReg:		Pointer to the channel register structure.
Returns:
		void.
		
****************************************************************/
void Vcnt_SendEvent(UINT8 bLen, void *pvReg)
{
	UINT8*	pbCur, bTimeCnt, bIdx;
	
	PVM_SEND_MSG pReg = (PVM_SEND_MSG)pvReg;

//-------ZY, 2005.1.19, Add for the DeltaTime over 0xffff-----//
	bTimeCnt = (UINT8)(g_CnvtInfo.dMissTime/0xffff);
	g_CnvtInfo.dMissTime %= 0xffff;

	for(bIdx=0; bIdx<bTimeCnt; bIdx++)
	{
		pbCur = g_CnvtInfo.pbEventBuf + g_CnvtInfo.wEventLen;
		
		pbCur[0] = 0xff;
		pbCur[1] = 0xff;
		pbCur[2] = (UINT8)(2);
		pbCur[3] = 0;
		pbCur[4] = 1;

		g_CnvtInfo.wEventLen +=	GLB_EVENT_LEN;

		g_CnvtInfo.dTotalTime += 0xffff;
	}
//-------ZY, 2005.1.19, Add for the DeltaTime over 0xffff-----//

	pReg->wDeltaTime += (UINT16)g_CnvtInfo.dMissTime;
	g_CnvtInfo.dMissTime = 0;
	pbCur = g_CnvtInfo.pbEventBuf + g_CnvtInfo.wEventLen;
	if (bLen == 8) 
	{
		pbCur[0] = (UINT8)(pReg->wDeltaTime & 0xff);
		pbCur[1] = (UINT8)(pReg->wDeltaTime>>8 & 0xff);
		pbCur[2] = (UINT8)(pReg->bMsgID);
		pbCur[3] = (UINT8)(pReg->bPara1);
		pbCur[4] = (UINT8)(pReg->bPara2);
		pbCur[5] = (UINT8)(pReg->bPara3);
		pbCur[6] = (UINT8)(pReg->bPara4);
		pbCur[7] = (UINT8)(pReg->bPara5);
	}
	else if (bLen == 7) 
	{
		pbCur[0] = (UINT8)(pReg->wDeltaTime & 0xff);
		pbCur[1] = (UINT8)(pReg->wDeltaTime>>8 & 0xff);
		pbCur[2] = (UINT8)(pReg->bMsgID);
		pbCur[3] = (UINT8)(pReg->bPara1);
		pbCur[4] = (UINT8)(pReg->bPara2);
		pbCur[5] = (UINT8)(pReg->bPara3);
		pbCur[6] = (UINT8)(pReg->bPara4);
	}
  else if (bLen == 6) 
	{
		pbCur[0] = (UINT8)(pReg->wDeltaTime & 0xff);
		pbCur[1] = (UINT8)(pReg->wDeltaTime>>8 & 0xff);
		pbCur[2] = (UINT8)(pReg->bMsgID);
		pbCur[3] = (UINT8)(pReg->bPara1);
		pbCur[4] = (UINT8)(pReg->bPara2);
		pbCur[5] = (UINT8)(pReg->bPara3);
  }
  else if (bLen == 5) 
	{
		pbCur[0] = (UINT8)(pReg->wDeltaTime & 0xff);
		pbCur[1] = (UINT8)(pReg->wDeltaTime>>8 & 0xff);
		pbCur[2] = (UINT8)(pReg->bMsgID);
		pbCur[3] = (UINT8)(pReg->bPara1);
		pbCur[4] = (UINT8)(pReg->bPara2);
  }
	else if (bLen == 4) 
	{
		pbCur[0] = (UINT8)(pReg->wDeltaTime & 0xff);
		pbCur[1] = (UINT8)(pReg->wDeltaTime>>8 & 0xff);
		pbCur[2] = (UINT8)(pReg->bMsgID);
		pbCur[3] = (UINT8)(pReg->bPara1);
	}

	g_CnvtInfo.wEventLen +=	bLen;

	g_CnvtInfo.dTotalTime += pReg->wDeltaTime;
}

/***************************************************************
Description:
		Send global hard event to syn buffer.
				
Parameters:
		wDeltaTime:	Delta Time.
		bChn:		Hard channel.
		pvReg:		Pointer to the channel register structure.
Returns:
		void.
		
****************************************************************/
void Vcnt_SendGlobalEvent(UINT16 wDeltaTime, UINT8 bPage, UINT8 bReg, UINT8 bVal)
{
	UINT8*	pbCur, bTimeCnt, bIdx;


//-------ZY, 2005.1.19, Add for the DeltaTime over 0xffff-----//
	bTimeCnt = (UINT8)(g_CnvtInfo.dMissTime/0xffff);
	g_CnvtInfo.dMissTime %= 0xffff;

	for(bIdx=0; bIdx<bTimeCnt; bIdx++)
	{
		pbCur = g_CnvtInfo.pbEventBuf + g_CnvtInfo.wEventLen;
		
		pbCur[0] = 0xff;
		pbCur[1] = 0xff;
		pbCur[2] = (UINT8)(2);
		pbCur[3] = 0;
		pbCur[4] = 1;

		g_CnvtInfo.wEventLen +=	GLB_EVENT_LEN;

		g_CnvtInfo.dTotalTime += 0xffff;
	}

//-------ZY, 2005.1.19, Add for the DeltaTime over 0xffff-----//

	wDeltaTime += (UINT16)g_CnvtInfo.dMissTime;
	g_CnvtInfo.dMissTime = 0;
	pbCur = g_CnvtInfo.pbEventBuf + g_CnvtInfo.wEventLen;

	if(bPage != 1)
	{
		pbCur[0] = (UINT8)(wDeltaTime&0xff);
		pbCur[1] = (UINT8)(wDeltaTime>>8);
		pbCur[2] = (UINT8)(2);
		pbCur[3] = 0;
		pbCur[4] = bPage;

		pbCur[5] = (UINT8)(0);
		pbCur[6] = (UINT8)(0);
		pbCur[7] = (UINT8)(2);
		pbCur[8] = bReg;
		pbCur[9] = bVal;

		pbCur[10] = (UINT8)(0);
		pbCur[11] = (UINT8)(0);
		pbCur[12] = (UINT8)(2);
		pbCur[13] = 0;
		pbCur[14] = 1;

		g_CnvtInfo.wEventLen += (GLB_EVENT_LEN*3);
	}
	else
	{
		pbCur[0] = (UINT8)(wDeltaTime&0xff);
		pbCur[1] = (UINT8)(wDeltaTime>>8);
		pbCur[2] = (UINT8)(2);
		pbCur[3] = bReg;
		pbCur[4] = bVal;

		g_CnvtInfo.wEventLen += GLB_EVENT_LEN;
	}
		
	g_CnvtInfo.dTotalTime += wDeltaTime;
}

/***************************************************************
Description:
		Send modify hard event to syn buffer.
				
Parameters:
		wDeltaTime:	Delta Time.
		bChn:		Hard channel.
		pvReg:		Pointer to the channel register structure.
Returns:
		void.
		
****************************************************************/
void Vcnt_SendModifyEvent(UINT16 wDeltaTime, UINT8 bChn, UINT8 bReg, UINT32 dMask, UINT32 dVal)
{
	UINT8* pbCur, bTimeCnt, bIdx;

	//-------ZY, 2005.1.19, Add for the DeltaTime over 0xffff-----//
	bTimeCnt = (UINT8)(g_CnvtInfo.dMissTime/0xffff);
	g_CnvtInfo.dMissTime %= 0xffff;

	for(bIdx=0; bIdx<bTimeCnt; bIdx++)
	{
		pbCur = g_CnvtInfo.pbEventBuf + g_CnvtInfo.wEventLen;
		
		pbCur[0] = 0xff;
		pbCur[1] = 0xff;
		pbCur[2] = (UINT8)(2);
		pbCur[3] = 0;
		pbCur[4] = 1;

		g_CnvtInfo.wEventLen +=	GLB_EVENT_LEN;

		g_CnvtInfo.dTotalTime += 0xffff;
	}

//-------ZY, 2005.1.19, Add for the DeltaTime over 0xffff-----//
	wDeltaTime += (UINT16)g_CnvtInfo.dMissTime;
	g_CnvtInfo.dMissTime = 0;
	pbCur = g_CnvtInfo.pbEventBuf + g_CnvtInfo.wEventLen;

	pbCur[0] = (UINT8)(wDeltaTime&0xff);
	pbCur[1] = (UINT8)(wDeltaTime>>8);
	pbCur[2] = (UINT8)(1);
	pbCur[3] = bChn;
	pbCur[4] = bReg;
	pbCur[5] = (UINT8)(dMask);
	pbCur[6] = (UINT8)(dMask>>8);
	pbCur[7] = (UINT8)(dMask>>16);
	pbCur[8] = (UINT8)(dMask>>24);
	pbCur[9] = (UINT8)(dVal);
	pbCur[10] = (UINT8)(dVal>>8);
	pbCur[11] = (UINT8)(dVal>>16);
	pbCur[12] = (UINT8)(dVal>>24);

	g_CnvtInfo.wEventLen += MODIFY_EVENT_LEN;

	g_CnvtInfo.dTotalTime += wDeltaTime;
}


/***************************************************************
Description:
		Convert VMD stream off message to event.  Only the VMD file contains 
		this message.
		
Parameters:
		delta_time:	Delta time. Unit is millisecond.
		ch:			The channel number of this message.
		id:			The index of the stream data. Range: 0~0xf.
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
void Vcnt_Reset(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG 	pMsg = (PVM_UNI_MSG)pvMsg;

	g_CnvtInfo.dMissTime += pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;

	pSendMsg->bMsgID = 0x40;
	pSendMsg->wDeltaTime =	(UINT16)pMsg->sdDeltaTime;	
	pSendMsg->bPara1 = 0;

	Vcnt_SendEvent(4, (void*)pSendMsg);
}
/***************************************************************
Description:
		Convert VMD stream off message to event.  Only the VMD file contains 
		this message.
		
Parameters:
		delta_time:	Delta time. Unit is millisecond.
		ch:			The channel number of this message.
		id:			The index of the stream data. Range: 0~0xf.
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
void Vcnt_Control(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG 	pMsg = (PVM_UNI_MSG)pvMsg;
	UINT8 bPeakStep = 1;                   //EVLP_PEAK_STEP: 1: high, 2:middle, 3:low
	//PER_NOTEOFF_DISABLE(): percussion channel :  1:sample loop disable, 0:sample loop enable
	UINT8 bSampleLoop = 1;
	//EVLP_D_CHANGE	1:pReg->EVLPCHG1 = pSynPara->wSLevel 0:pReg->EVLPCHG1 = pSynPara->wSLevel - wEvlpPeak
	UINT8 bEvlpChange = 1;
	UINT8 bFor51 = 1;                                //OPTIMIZE_FOR_51
		
	g_CnvtInfo.dMissTime += pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;

	pSendMsg->bMsgID = 0x41;
	pSendMsg->wDeltaTime =	(UINT16)pMsg->sdDeltaTime;	
	pSendMsg->bPara1 = bPeakStep;
	pSendMsg->bPara2 = bSampleLoop;
	pSendMsg->bPara3 = bEvlpChange;
	pSendMsg->bPara4 = bFor51;

	Vcnt_SendEvent(7, (void*)pSendMsg);

}
/***************************************************************
Description:
		Convert VMD stream off message to event.  Only the VMD file contains 
		this message.
		
Parameters:
		delta_time:	Delta time. Unit is millisecond.
		ch:			The channel number of this message.
		id:			The index of the stream data. Range: 0~0xf.
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
void Vcnt_Special(void *pvMsg)
{
	VM_SEND_MSG  sendMsg;
  	PVM_SEND_MSG  pSendMsg = &sendMsg;
	PVM_UNI_MSG 	pMsg = (PVM_UNI_MSG)pvMsg;

	g_CnvtInfo.dMissTime += pMsg->sdDeltaTime;
	pMsg->sdDeltaTime = 0;

	pSendMsg->bMsgID = 0x45;
  	pSendMsg->wDeltaTime =	(UINT16)pMsg->sdDeltaTime;	
	pSendMsg->bPara1 = pMsg->bChn;
  	pSendMsg->bPara2 = (UINT8)pMsg->dPara1;
	pSendMsg->bPara3 = (UINT8)pMsg->dPara2;
	pSendMsg->bPara4 = (UINT8)(pMsg->dPara3 & 0xff);
	pSendMsg->bPara5 = (UINT8)(pMsg->dPara3>>8 & 0xff);

	Vcnt_SendEvent(8, (void*)pSendMsg);
}

/***************************************************************
Description:
		Convert VMD stream on  message to event.  Only the VMD file contains 
		this message.
		
Parameters:
		delta_time:	Delta time. Unit is millisecond.
		ch:			The channel number of this message.
		id:			the index of the stream data. Range: 0~0xf.
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
void Vcnt_StreamOn(void *pvMsg)
{
		UINT8			bDataType, bChnNum, bSignCtrl, bBitsPerSam;
		UINT16    wDeltaTime = 0;
		UINT8			i, bChnAtrb, bBitCtrl;	
		UINT16		wFratio = 0;
		UINT32		dFreq;
		UINT8			bWaveChn, bHardChn;

	    VM_FIFOADDR  vmFifoAddr;

		PVM_SPH       	pSynPara;
	    PVM_CHN_REG			pReg;
		PVM_UNI_MSG			pMsg;

		pMsg = (PVM_UNI_MSG)pvMsg;
		bWaveChn = pMsg->bChn;
		
		if(pMsg->sdDeltaTime < 0)
		{
	    	wDeltaTime = 0;
		}
		else
			wDeltaTime	= (UINT16)(pMsg->sdDeltaTime&0xffff);

		g_CnvtInfo.dMissTime += wDeltaTime;

		pSynPara = &g_CnvtInfo.SynPara[bWaveChn];
		pReg = (PVM_CHN_REG)&g_CnvtInfo.ChnReg;
			
		bDataType = (UINT8)(pMsg->dPara1 & 0x0f); 

		if (bDataType)
		{
			bDataType = 0;
		}
		else
			bDataType = 1;
		
		if (((((pMsg->dPara1)>>4) & 0x3) == 1) || bDataType == 1) 
			bChnNum = 0;
		else 
			bChnNum = 1;

		bBitsPerSam	=	(UINT8)(((pMsg->dPara1)>>8) & 0x0ff);
		Vcnt_GetAddr(&vmFifoAddr, g_CnvtInfo.bFifoNum, bWaveChn, bBitsPerSam, bChnNum);	

		if(bDataType == 0 && bChnNum == 1)                     // pcm,stereo
			bBitsPerSam = bBitsPerSam*2;                          

		bHardChn = bWaveChn - 20;         // 20/21/22/23

	  dFreq	=	(UINT16)((pMsg->dPara1>>16) & 0x0ffff);
		wFratio = (UINT16)((dFreq / SAMPLE_RATE) << 12) ;
		dFreq %= SAMPLE_RATE;
		for ( i = 0 ; i < 12 ; i ++ )
		{	
		  wFratio |= ( (dFreq <<= 1) >= SAMPLE_RATE ) ? ((UINT16)1<<(11 - i)) : 0 ;
			dFreq = ( dFreq >= SAMPLE_RATE ) ? (dFreq - SAMPLE_RATE) : dFreq ;
		}
		
		bBitCtrl = 0;
		bSignCtrl  =	0;

		if ( 1 == bDataType )                                  // ADPCM
		{
			bBitCtrl = 0;
			bSignCtrl = 0;
			bChnAtrb = 4;
		}
		if (( 0 == bDataType ) && ( bBitsPerSam == 8 ))        // PCM
		{
			bBitCtrl = 1;
			bSignCtrl =	1;
			bChnAtrb = 5;
		}

		if (( 0 == bDataType ) && ( bBitsPerSam == 16 ))
		{
			if(!bChnNum)
			{
				bBitCtrl = 0;
				bSignCtrl =	0;
				bChnAtrb = 5;
			}
			else
			{
				bBitCtrl = 1;
				bSignCtrl =	1;
				bChnAtrb = 6;
			}
		}

		if ( ( 0 == bDataType ) && ( bBitsPerSam == 32 ) )
		{
			bBitCtrl = 0;
			bSignCtrl =	0;
			bChnAtrb = 6 ;
		}
		// bBitCtrl = bSignCtrl

		pReg->BASEADDR = vmFifoAddr.wBaseAddr;
		pReg->BITCTRL = bBitCtrl;
		pReg->CHNATRB = bChnAtrb;
		pReg->ENDPTR = vmFifoAddr.wEndPtr;
		pReg->SIGNCTRL = bSignCtrl;
		pReg->FRATIO = wFratio;
		pReg->LFOCNT = MODU_PRELOAD;
		pReg->LFOINIT = MODU_PRELOAD;
		pReg->RVOL = 0x40;
		pReg->LOOPEN = 1;                   // stream
		pReg->CHNVOLEN = 1;

		pReg->EVLPSEG	= 0;					
		pReg->INTPCOEF	= 0;
		pReg->ASTEP 		= 0;
		pReg->FSTEP 		= 0;
		pReg->EVLPCNT	= 0;
		pReg->SINDIR	= 0;
		pReg->SINVAL	= 0;
		pReg->FMCTRL	= 0;
		pReg->PREINDEX	= 0;
		pReg->PREVAL	= 0;
		pReg->EVLPCHG0 = 0;
	  	pReg->CUREVLP = 0;
		pReg->EVLPCHG1 = 0;
		pReg->EVLPCHG2	= 0;
		pReg->EVLPMOD0	= 0;
		pReg->EVLPMOD1	= 0;
		pReg->EVLPMOD2	= 0;
		pReg->ECNTINIT0 = 0;
		pReg->ECNTINIT1 = 0;
		pReg->ECNTINIT2 	= 0;
		pReg->ECNTINIT3 = 0;
		pReg->CURPTR = 0;
		pReg->CURSIGN	= 0;
		pReg->LFOSIGN	= 0;
		pReg->CHNVOL_L = 0;
		pReg->CHNVOL_R = 0;
			
		Vcnt_SendStreamEvent(0, bHardChn, (void*)pReg);
}

/***************************************************************
Description:
		Convert VMD stream off message to event.  Only the VMD file contains 
		this message.
		
Parameters:
		delta_time:	Delta time. Unit is millisecond.
		ch:			The channel number of this message.
		id:			The index of the stream data. Range: 0~0xf.
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
void Vcnt_StreamOff(void *pvMsg)
{
	UINT8	bReg, bVal, bChn;
	PVM_UNI_MSG 	pMsg = (PVM_UNI_MSG)pvMsg;
	
	g_CnvtInfo.dMissTime += pMsg->sdDeltaTime;
	bChn = pMsg->bChn - 20;
	  
	bReg = REG_PAG1_CHNSTOP + bChn/8;
	bVal = 1 << (bChn % 8);
	//-----ZY--2004.9.15------//
	Vcnt_SendGlobalEvent(0, (UINT8)1, bReg, bVal);

	Vcnt_SendGlobalEvent(0,1,0x5c,0x01 << 2);
	Vcnt_SendGlobalEvent(0,1,(UINT8)(0x1A+bChn),0x01);
}
/****************************************************************************
 *	VmaAudDrv_ReverbOn
 *
 *	Description:
 *			Start Reverb/Chorus, and WE begin to read PCM to 8kRAM
 *	Arguments:
 *			seq_id		sequence id (0..2)
 *			delta_time	delta time [tick]
 *			bCh			MIDI channel
 *			p2			unused
 *			p3			unused
 *	Return:887777
 *			0			success
 *			< 0			error code
 *
 ****************************************************************************/
void Vcnt_ReverbOn(void *pvMsg)
{
	UINT8	    bHardChn;	
	UINT8			bChnAtrb, bBitCtrl, bSignCtrl;	
	UINT16		wFratio;
	UINT16		wEndPtr, wBaseAddr;
	UINT16    wDeltaTime = 0;

	PVM_CHN_REG			pReg;
	PVM_UNI_MSG			pMsg;

	pMsg = (PVM_UNI_MSG)pvMsg;
	pReg = (PVM_CHN_REG)&g_CnvtInfo.ChnReg;

	if(pMsg->sdDeltaTime < 0)
	{
    wDeltaTime = 0;
	}
	else
		wDeltaTime	= (UINT16)(pMsg->sdDeltaTime&0xffff);

	g_CnvtInfo.dMissTime += wDeltaTime;
	
//	bHardChn = Vcnt_FindHardChn(pMsg->bChn);
	bHardChn = 0;

	g_HardChnPara[bHardChn].bMidiChn = pMsg->bChn;
	g_HardChnPara[bHardChn].bChnOn = 1;

	// 1. a Channel Setup Event to play Reverb/Chorus
	wFratio = 0x1000;
	bBitCtrl = 0;
	bSignCtrl = 0;
	bChnAtrb = 7;

	wBaseAddr = 0;
	wEndPtr = 0x1D7F;                    //((16<<10)/2)-1;

	pReg->FRATIO = wFratio;
	pReg->ENDPTR = wEndPtr;

	pReg->CHNVOLEN = 1;
	
	pReg->BASEADDR = wBaseAddr;
	pReg->SIGNCTRL = bSignCtrl;
	pReg->BITCTRL = bBitCtrl;
	pReg->LOOPEN = 1;
	pReg->CHNATRB = bChnAtrb;

	pReg->LFOINIT = MODU_PRELOAD;
    pReg->LFOCNT = MODU_PRELOAD;
 
	pReg->EVLPSEG 	= 0;					
	pReg->INTPCOEF 	= 0;
	pReg->ASTEP 		= 0;
	pReg->FSTEP 		= 0;
	pReg->EVLPCNT 	= 0;
	pReg->SINDIR 	= 0;
	pReg->SINVAL 	= 0;
	pReg->FMCTRL 	= 0;
	pReg->PREINDEX 	= 0;
	pReg->PREVAL 	= 0;
	pReg->EVLPCHG0 = 0;
    pReg->CUREVLP = 0;
	pReg->EVLPCHG1 = 0;
	pReg->EVLPCHG2 	= 0;
	pReg->EVLPMOD0 	= 0;
	pReg->EVLPMOD1 	= 0;
	pReg->EVLPMOD2 	= 0;
	pReg->ECNTINIT0 = 0;
	pReg->ECNTINIT1 = 0;
	pReg->ECNTINIT2 	= 0;
	pReg->ECNTINIT3 = 0;
	pReg->CURPTR = 0;
	pReg->CURSIGN	= 0;
	pReg->LFOSIGN	= 0;
	pReg->CHNVOL_L = 0;
    pReg->CHNVOL_R = 0;

	Vcnt_SendStreamEvent((UINT16)wDeltaTime, bHardChn, (void*)pReg);
}

/****************************************************************************
 *	VmaAudDrv_ReverbOff
 *
 *	Description:
 *			
 *	Arguments:
 *			seq_id		sequence id (0..2)
 *			delta_time	delta time [tick]
 *			ch			channel number (0..15)
 *			p2			unused
 *			p3			unused
 *	Return:
 *
 *
 ****************************************************************************/
void Vcnt_ReverbOff(void *pvMsg)
{
	UINT8	    bChn, bReg, bVal;	

	PVM_UNI_MSG			pMsg;

	pMsg = (PVM_UNI_MSG)pvMsg;
	
	g_CnvtInfo.dMissTime += pMsg->sdDeltaTime;
	bChn = pMsg->bChn;
	
	bReg = (UINT8)(REG_PAG1_CHNSTOP + bChn/8);
	bVal = 1 << (bChn % 8) ;

	Vcnt_SendGlobalEvent(0, 1, bReg, bVal);   // page1
}
/****************************************************************************
 *	VmaAudDrv_ReverbEnable
 *
 *	Description:
 *			Enable Reverb/Chorus, and begin to write PCM to 8kRAM
 *	Arguments:
 *			seq_id		sequence id (0..2)
 *			delta_time	delta time [tick]
 *			bCh			MIDI channel
 *			p2			unused
 *			p3			unused
 *	Return:887777
 *			0			success
 *			< 0			error code
 *
 ****************************************************************************/
void Vcnt_ReverbEnable(void *pvMsg)
{

	PVM_UNI_MSG			pMsg;

	pMsg = (PVM_UNI_MSG)pvMsg;

	g_CnvtInfo.dMissTime += pMsg->sdDeltaTime;

	Vcnt_SendGlobalEvent(0, 1, (UINT8)(REG_PAGE1_CHNCTRL), 0x01);   // page1
}
/****************************************************************************
 *	VmaAudDrv_MasterVolume
 *
 *	Description:
 *			Sets the master volume of the specified sequence.
 *	Argument:
 *			seq_id		sequence id (0..2)
 *			delta_time	delta time [tick]
 *			type		midi or wave master volume
 *			vol_l		left volume value (0..127)
 *			vol_r		right volume value (0..127)
 *	Return:
 *			0			success
 *			< 0			error code
 *
 ****************************************************************************/
void Vcnt_MasterVolume(void *pvMsg)
{
	PVM_UNI_MSG		pMsg;
    UINT32			dVolLeft, dVolRight, dType;
	UINT8			bReg, bVal;	
	UINT16          wDeltaTime = 0;

	pMsg = (PVM_UNI_MSG)pvMsg;

	if (pMsg->sdDeltaTime < 0) 
	{
     wDeltaTime = 0;
	}
	else
		wDeltaTime	= (UINT16)(pMsg->sdDeltaTime&0xffff);

    dType = pMsg->dPara1;
	dVolLeft = pMsg->dPara2;
    dVolRight = pMsg->dPara3;

	dVolLeft = (dVolLeft > 0xff) ? 0xff : dVolLeft;

	dVolRight = (dVolRight > 0xff) ? 0xff : dVolRight;
	//  	/* 0:midi or 1:wave master volume */
	bReg = (UINT8)((dType) ? REG_PAGE1_VOLWAVR : REG_PAGE1_VOLMIDIR);
	bVal = (UINT8)dVolRight;
	Vcnt_SendGlobalEvent((UINT16)pMsg->sdDeltaTime, 1, bReg, bVal);   // page1

	bReg = (UINT8)((dType) ? REG_PAGE1_VOLWAVL : REG_PAGE1_VOLMIDIL);
	bVal = (UINT8)dVolLeft;
	Vcnt_SendGlobalEvent((UINT16)pMsg->sdDeltaTime, 1, bReg, bVal);   // page1
}

/***************************************************************
Description:
		Convert EOS  message to event. It's the last event of a whole music.
		
Parameters:
		delta_time:	Delta time. Uint is ms.
		val:			Message value.
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
void Vcnt_EosEvent(void *pvMsg)
{
	PVM_UNI_MSG		pMsg;
	pMsg = (PVM_UNI_MSG)pvMsg;

	if(pMsg->bChn<MAX_MIDI_CHN) //Midi EOS
	{	
		Vcnt_SendGlobalEvent((UINT16)(pMsg->sdDeltaTime+200), 1, REG_PAGE1_SWI_SEL, 0x02);
		pMsg->sdDeltaTime = 0;
		Vcnt_Reset(pvMsg);
	}
}
/***************************************************************
Description:
		Send Stream on hard event to syn buffer.
				
Parameters:
		wDeltaTime:	Delta Time.
		bChn:		Hard channel.
		pvReg:		Pointer to the channel register structure.
Returns:
		void.
		
****************************************************************/
void Vcnt_SendStreamEvent(UINT16 wDeltaTime, UINT8 bChn, void *pvReg)
{
	UINT8*	pbCur;
	UINT8	bTimeCnt, bIdx;

	PVM_CHN_REG pReg = (PVM_CHN_REG)pvReg;

	bTimeCnt = (UINT8)(g_CnvtInfo.dMissTime/0xffff);
	g_CnvtInfo.dMissTime %= 0xffff;

	for(bIdx=0; bIdx<bTimeCnt; bIdx++)
	{
		pbCur = g_CnvtInfo.pbEventBuf + g_CnvtInfo.wEventLen;
		
		pbCur[0] = 0xff;
		pbCur[1] = 0xff;
		pbCur[2] = (UINT8)(2);
		pbCur[3] = 0;
		pbCur[4] = 1;

		g_CnvtInfo.wEventLen +=	GLB_EVENT_LEN;

		g_CnvtInfo.dTotalTime += 0xffff;
	}

	wDeltaTime += (UINT16)g_CnvtInfo.dMissTime;
	g_CnvtInfo.dMissTime = 0;
	pbCur = g_CnvtInfo.pbEventBuf + g_CnvtInfo.wEventLen;

	pbCur[0] = (UINT8)(wDeltaTime & 0xff);
	pbCur[1] = (UINT8)(wDeltaTime>>8);
	pbCur[2] = (UINT8)(4);                      // type 4 event
	pbCur[3] = (UINT8)(bChn);
	
	pbCur[4] = (UINT8)(pReg->CURPTR & 0xff);
	pbCur[5] = (UINT8)(pReg->CURPTR>>8) + ((UINT8)(pReg->CURSIGN & 0x01)<<7);
	pbCur[6] = (UINT8)(pReg->LFOCNT);
	pbCur[7] = (UINT8)(pReg->EVLPCNT);

	pbCur[8] = (UINT8)(pReg->FRATIO & 0xff);
	pbCur[9] = (UINT8)(pReg->FRATIO>>8);
	pbCur[10] = (UINT8)(pReg->ENDPTR & 0xff);
	pbCur[11] = (UINT8)(pReg->ENDPTR>>8) + ((UINT8)(pReg->EVLPMOD2 & 0x01)<<7);

	pbCur[12] = (UINT8)(pReg->INTPCOEF & 0xff);
	pbCur[13] = (UINT8)(pReg->INTPCOEF>>8) + ((UINT8)(pReg->EVLPCHG0 & 0x0f)<<4);
	pbCur[14] = (UINT8)(pReg->CUREVLP & 0xff);
	pbCur[15] = (UINT8)(pReg->SINVAL) + (UINT8)(pReg->SINDIR<<4) + (UINT8)(pReg->LFOSIGN<<5) 
		          + (UINT8)(pReg->EVLPSEG<<6);

  pbCur[16] = (UINT8)(pReg->RVOL) + (UINT8)(pReg->CHNVOLEN<<7);
	pbCur[17] = (UINT8)(pReg->ECNTINIT0);
	pbCur[18] = (UINT8)(pReg->ECNTINIT1);
	pbCur[19] = (UINT8)(pReg->ECNTINIT2);
	
	pbCur[20] = (UINT8)(pReg->BASEADDR & 0xff);
	pbCur[21] = (UINT8)(pReg->BASEADDR>>8);
	pbCur[22] = (UINT8)(pReg->BASEADDR>>16) + (UINT8)(pReg->SIGNCTRL<<3)
		          + (UINT8)(pReg->BITCTRL<<4) + (UINT8)(pReg->LOOPEN<<5)
							+ (UINT8)((pReg->CHNATRB & 0x03)<<6);
	pbCur[23] = (UINT8)(pReg->CHNATRB>>2) + (UINT8)((pReg->EVLPMOD1>>1)<<1)
		          + (UINT8)(pReg->FMCTRL<<2) + (UINT8)(pReg->ASTEP<<4);

	pbCur[24] = (UINT8)(pReg->FSTEP) + (UINT8)((pReg->CUREVLP>>8)<<4);
	pbCur[25] = (UINT8)(pReg->PREVAL & 0xff);
	pbCur[26] = (UINT8)(pReg->PREVAL>>8);
	pbCur[27] = (UINT8)(pReg->PREINDEX) + (UINT8)((pReg->EVLPMOD1 & 0x01)<<7);

	pbCur[28] = (UINT8)(pReg->EVLPCHG0>>4);
	pbCur[29] = (UINT8)(pReg->EVLPCHG1 & 0xff);
	pbCur[30] = (UINT8)(pReg->EVLPCHG1>>8) + (UINT8)((pReg->EVLPCHG2 & 0x0f)<<4);
	pbCur[31] = (UINT8)(pReg->EVLPCHG2>>4) + (UINT8)(pReg->EVLPMOD0<<5)
		          + (UINT8)((pReg->EVLPMOD2>>1)<<7);

	pbCur[32] = (UINT8)(pReg->CHNVOL_L);
	pbCur[33] = (UINT8)(pReg->CHNVOL_R);
	pbCur[34] = (UINT8)(pReg->LFOINIT);
	pbCur[35] = (UINT8)(pReg->ECNTINIT3);

	g_CnvtInfo.wEventLen +=	SETUP_4_LEN;

	g_CnvtInfo.dTotalTime += wDeltaTime;

}
/***************************************************************
Description:
		Convert VMD stream on  message to event.  Only the VMD file contains 
		this message.
		
Parameters:
		delta_time:	Delta time. Unit is millisecond.
		ch:			The channel number of this message.
		id:			the index of the stream data. Range: 0~0xf.
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
void Vcnt_Mp3On(void *pvMsg)
{
	UINT8			bChnNum, bSignCtrl;
        
	UINT8			i, bChnAtrb, bBitCtrl;	
	UINT16			wFratio = 0;
	UINT32			dFreq, wDeltaTime = 0;
    UINT8			bWaveChn, bHardChn;

    VM_FIFOADDR     vmFifoAddr;

	PVM_SPH       	pSynPara;
    PVM_CHN_REG			pReg;
	PVM_UNI_MSG			pMsg;

	pMsg = (PVM_UNI_MSG)pvMsg;
	bWaveChn = pMsg->bChn;                 // 26
	if (pMsg->sdDeltaTime < 0) 
	{
		wDeltaTime = 0;
	}
	else
		wDeltaTime	= (UINT16)(pMsg->sdDeltaTime&0xffff);

	g_CnvtInfo.dMissTime += wDeltaTime;

	pSynPara = &g_CnvtInfo.SynPara[bWaveChn];
	pReg = (PVM_CHN_REG)&g_CnvtInfo.ChnReg;
		
	if ((pMsg->dPara1>>4 & 0x3) == 1)
		bChnNum = 0;
	else 
		bChnNum = 1;
	
    vmFifoAddr.wBaseAddr = 0;
    vmFifoAddr.wEndPtr = 223;
	bHardChn = bWaveChn - 26;
	g_HardChnPara[bHardChn].bChnOn = 1;
	g_HardChnPara[bHardChn].bMidiChn = bHardChn;
	
	g_CnvtInfo.dMP3SampleRate = pMsg->dPara1;

    dFreq	=	(UINT16)(pMsg->dPara1>>16 & 0x0ffff);
	wFratio = (UINT16)((dFreq / SAMPLE_RATE) << 12) ;
	dFreq %= SAMPLE_RATE;
	for ( i = 0 ; i < 12 ; i ++ )
	{	
	    wFratio |= ( (dFreq <<= 1) >= SAMPLE_RATE ) ? ((UINT16)1<<(11 - i)) : 0 ;
		dFreq = ( dFreq >= SAMPLE_RATE ) ? (dFreq - SAMPLE_RATE) : dFreq ;
	}


	bBitCtrl = 0;
	bSignCtrl =	0;
	bChnAtrb = 6;


	// bBitCtrl = bSignCtrl
	
	pReg->BASEADDR = vmFifoAddr.wBaseAddr;
	pReg->BITCTRL = bBitCtrl;
	pReg->CHNATRB = bChnAtrb;
	pReg->ENDPTR = vmFifoAddr.wEndPtr;
	pReg->SIGNCTRL = bSignCtrl;
	pReg->FRATIO = wFratio;
	pReg->LFOCNT = MODU_PRELOAD;
	pReg->LFOINIT = MODU_PRELOAD;
	pReg->RVOL = 0x40;
	pReg->LOOPEN = 1;                   // stream
	pReg->CHNVOLEN = 1;

	pReg->EVLPSEG	= 0;					
	pReg->INTPCOEF	= 0;
	pReg->ASTEP 		= 0;
	pReg->FSTEP 		= 0;
	pReg->EVLPCNT	= 0;
	pReg->SINDIR	= 0;
	pReg->SINVAL	= 0;
	pReg->FMCTRL	= 0;
	pReg->PREINDEX	= 0;
	pReg->PREVAL	= 0;
	pReg->EVLPCHG0 = 0;
  	pReg->CUREVLP = 0;
	pReg->EVLPCHG1 = 0;
	pReg->EVLPCHG2	= 0;
	pReg->EVLPMOD0	= 0;
	pReg->EVLPMOD1	= 0;
	pReg->EVLPMOD2	= 0;
	pReg->ECNTINIT0 = 0;
	pReg->ECNTINIT1 = 0;
	pReg->ECNTINIT2 	= 0;
	pReg->ECNTINIT3 = 0;
	pReg->CURPTR = 0;
	pReg->CURSIGN	= 0;
	pReg->LFOSIGN	= 0;
	pReg->CHNVOL_L = 0;
	pReg->CHNVOL_R = 0;

	Vcnt_SendStreamEvent(0, bHardChn, (void*)pReg);
}

/***************************************************************
Description:
		Convert VMD stream off message to event.  Only the VMD file contains 
		this message.
		
Parameters:
		delta_time:	Delta time. Unit is millisecond.
		ch:			The channel number of this message.
		id:			The index of the stream data. Range: 0~0xf.
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
void Vcnt_Mp3Off(void *pvMsg)
{
	UINT8	bReg, bVal, bChn;
	PVM_UNI_MSG 	pMsg = (PVM_UNI_MSG)pvMsg;

	g_CnvtInfo.dMissTime += pMsg->sdDeltaTime;
	bChn  = pMsg->bChn - 26;
	bReg = REG_PAG1_CHNSTOP + bChn/8;
	bVal = 1 << (bChn % 8) ;

	Vcnt_SendGlobalEvent(0, 1, bReg, bVal);	// page1

	Vcnt_SendGlobalEvent(0, 1, 0x5c, 0x01 << 2);
}

/****************************************************************************
 *	Vcnt_SetPage
 *
 *	Description:
 *			Sets the master volume of the specified sequence.
 *	Argument:
 *			seq_id		sequence id (0..2)
 *			delta_time	delta time [tick]
 *			type		midi or wave master volume
 *			vol_l		left volume value (0..127)
 *			vol_r		right volume value (0..127)
 *	Return:
 *			0			success
 *			< 0			error code
 *
 ****************************************************************************/
void Vcnt_SetPage(void *pvMsg)
{

    PVM_UNI_MSG 	pMsg = (PVM_UNI_MSG)pvMsg;
    
    Vcnt_SendGlobalEvent((UINT16)pMsg->sdDeltaTime, 1, 0, (UINT8)pMsg->dPara1);

}
/****************************************************************************
 *	Vcnt_SetPage
 *
 *	Description:
 *			Sets the master volume of the specified sequence.
 *	Argument:
 *			seq_id		sequence id (0..2)
 *			delta_time	delta time [tick]
 *			type		midi or wave master volume
 *			vol_l		left volume value (0..127)
 *			vol_r		right volume value (0..127)
 *	Return:
 *			0			success
 *			< 0			error code
 *
 ****************************************************************************/
void Vcnt_SoftInt(void *pvMsg)
{

    PVM_UNI_MSG 	pMsg = (PVM_UNI_MSG)pvMsg;
    
    Vcnt_SendGlobalEvent((UINT16)pMsg->sdDeltaTime, 1, REG_PAGE1_SWI_SEL, (UINT8)pMsg->dPara1);
}
/****************************************************************************
 *	Vcnt_SetPage
 *
 *	Description:
 *			Sets the master volume of the specified sequence.
 *	Argument:
 *			seq_id		sequence id (0..2)
 *			delta_time	delta time [tick]
 *			type		midi or wave master volume
 *			vol_l		left volume value (0..127)
 *			vol_r		right volume value (0..127)
 *	Return:
 *			0			success
 *			< 0			error code
 *
 ****************************************************************************/
void Vcnt_IntEnable(void *pvMsg)
{
    PVM_UNI_MSG 	pMsg = (PVM_UNI_MSG)pvMsg;
    if(pMsg->dPara1 == 0)
        Vcnt_SendGlobalEvent((UINT16)pMsg->sdDeltaTime, 1, REG_PAGE1_INTEN0, 1);
    else if(pMsg->dPara1 == 1)
        Vcnt_SendGlobalEvent((UINT16)pMsg->sdDeltaTime, 1, REG_PAGE1_INTEN0 + 2, 1);
}
/****************************************************************************
 *	Vcnt_SetPage
 *
 *	Description:
 *			Sets the master volume of the specified sequence.
 *	Argument:
 *			seq_id		sequence id (0..2)
 *			delta_time	delta time [tick]
 *			type		midi or wave master volume
 *			vol_l		left volume value (0..127)
 *			vol_r		right volume value (0..127)
 *	Return:
 *			0			success
 *			< 0			error code
 *
 ****************************************************************************/
void Vcnt_IntDisable(void *pvMsg)
{

    PVM_UNI_MSG 	pMsg = (PVM_UNI_MSG)pvMsg;
    if(pMsg->dPara1 == 0)
        Vcnt_SendGlobalEvent((UINT16)pMsg->sdDeltaTime, 1, REG_PAGE1_INTEN0, 0);
    else if(pMsg->dPara1 == 1)
        Vcnt_SendGlobalEvent((UINT16)pMsg->sdDeltaTime, 1, REG_PAGE1_INTEN0 + 2, 0);
}
/***************************************************************
Description:
		Convert VMD stream off message to event.  Only the VMD file contains 
		this message.
		
Parameters:
		delta_time:	Delta time. Unit is millisecond.
		ch:			The channel number of this message.
		id:			The index of the stream data. Range: 0~0xf.
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
void Vcnt_SetFifoNum(UINT8 bNum)
{
	g_CnvtInfo.bFifoNum = bNum;
}

VRESULT Vcnt_GetAddr(PVM_FIFOADDR pVm_fifoAddr, UINT8 bNum, UINT8 bChn, UINT8 bBitsPerSam, UINT8 bChnNum)
{
	UINT8 bBytePerSmp;

	
  if (bNum == 1) 
	{
		pVm_fifoAddr->wBaseAddr = 	g_SFifoBAddr[0][0] << 2;
		pVm_fifoAddr->wEndPtr = g_SFifoEndPtr[0] << 2;   
  }
  else if(bNum > 1)
	{
		pVm_fifoAddr->wBaseAddr = g_SFifoBAddr[bNum - 1][bChn - 20] << 2;
		pVm_fifoAddr->wEndPtr = g_SFifoEndPtr[bNum - 1] << 2; 
	}
  else if(bNum == 0)
     return VRES_ERROR;

  if (bBitsPerSam >= 8) 
	{

		bBytePerSmp = (bBitsPerSam/8) * (bChnNum+1);
		pVm_fifoAddr->wBaseAddr = pVm_fifoAddr->wBaseAddr/bBytePerSmp;
    	pVm_fifoAddr->wEndPtr = (pVm_fifoAddr->wEndPtr+4)/bBytePerSmp - 1;
  }
  else if (bBitsPerSam < 8) 
  {
    if (bChnNum == 0) 
		{
			pVm_fifoAddr->wBaseAddr = pVm_fifoAddr->wBaseAddr << 1;
			pVm_fifoAddr->wEndPtr = ((pVm_fifoAddr->wEndPtr+4)  << 1) - 1;			
    }
  }
	return VRES_SUCCESS;
}

// modify mp3 fratio
void Vcnt_ModifyMP3Fratio(UINT8 bTimes, void *pBuf)
{
	UINT32 dSampleRate, dFreq, dFratio, dMask;
	UINT8 i;
	UINT8 *pbCur = (UINT8 *)pBuf;
	
	if(bTimes == 0)
		dSampleRate = (UINT32)(g_CnvtInfo.dMP3SampleRate>>2);
    else if(bTimes == 1)
		dSampleRate = (UINT32)(g_CnvtInfo.dMP3SampleRate>>1);
	else if(bTimes == 2)
		dSampleRate = (UINT32)(g_CnvtInfo.dMP3SampleRate<<1);
	else if(bTimes == 3)
		dSampleRate = (UINT32)(g_CnvtInfo.dMP3SampleRate<<2);
	else if(bTimes == 4) 
		dSampleRate = g_CnvtInfo.dMP3SampleRate;
		
	dFreq = (UINT16)(dSampleRate>>16 & 0x0ffff);
	dFratio= (UINT16)((dFreq / SAMPLE_RATE) << 12) ;
	dFreq %= SAMPLE_RATE;
	for ( i = 0 ; i < 12 ; i ++ )
	{	
		dFratio|= ( (dFreq <<= 1) >= SAMPLE_RATE ) ? ((UINT16)1<<(11 - i)) : 0 ;
		dFreq = ( dFreq >= SAMPLE_RATE ) ? (dFreq - SAMPLE_RATE) : dFreq ;
	}
	
	dMask = 0x0ffff;
	
	pbCur[0] = (UINT8)(0&0xff);
	pbCur[1] = (UINT8)(0>>8);
	pbCur[2] = (UINT8)(1);
	pbCur[3] = 0;
	pbCur[4] = 1;
	pbCur[5] = (UINT8)(dMask);
	pbCur[6] = (UINT8)(dMask>>8);
	pbCur[7] = (UINT8)(dMask>>16);
	pbCur[8] = (UINT8)(dMask>>24);
	pbCur[9] = (UINT8)(dFratio);
	pbCur[10] = (UINT8)(dFratio>>8);
	pbCur[11] = (UINT8)(dFratio>>16);
	pbCur[12] = (UINT8)(dFratio>>24);
}

