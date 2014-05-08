/*--------------------------------------------------------------------

			COPYRIGHT (C) 2004, Vimicro Corporation
                        ALL RIGHTS RESERVED

 This source code has been made available to you by VIMICRO on an
 AS-IS basis. Anyone receiving this source code is licensed under VIMICRO
 copyrights to use it in any way he or she deems fit, including copying it,
 modifying it, compiling it, and redistributing it either with or without
 modifications. Any person who transfers this source code or any derivative
 work must include the VIMICRO copyright notice and this paragraph in
 the transferred software.


 Module:		Vsyn.c

 Description:	VC0938 synthesizer module functions definitions

 Update History:
	1.0			Basic version				2004.08.03		gzd

	1.1.2		fix a bug in Vsyn_StopSeq	2004.10.27		gzd
				Correct the input parameter of Vsyn_RemoveIsr
				Delete variable evaluate sentence of 
				pVsyn->uSFifo_Info.sStmFifo_Info.wSFifoBufLen
				
	1.1.3									2004.11.01		gzd
				Add a function Vsyn_SetMp3Speed() to set mp3 playing speed.

	1.1.4									2004.11.04		zy
				Add a function Vcnt_ConverterReset to reset Converter module.

	1.1.5									2004.11.18		gzd
				add a function Vsyn_SetupDLS() to support playback DLS music data.

	1.1.6									2004.11.19		gzd
				Call Vdev_CtrlSeq(VDEV_CTRL_RESET_SEQUENCER) in Vsyn_StopSeq()
				for stop seq counter.

	1.1.7									2004.11.23		gzd
				realize Key_Effect feature in Vsyn_EventSetup().

	1.1.8									2004.11.29		gzd
				Remove Clear interrupt Flags in function Vsyn_StreamSetup() and Vsyn_StreamUpdate().
				Remove Reset WS operation in case Mp3 in function Vsyn_StreamSetup().
				Move Vcnt_ConverterReset() to case Event in function Vsyn_StopSeq().
				Add Clear interrupt Flags in ISR functions.
				Add Reset sequencer operation in function Vsyn_StartSeq().
				Add Padding zero to Fifos when stream or mp3 data size < one buffer.

	1.1.9									2004.12.12		gzd
				Add VMD control flag registers 0x1A, 0x1B, 0x1C, 0x1D.

	1.2.0									2004.12.15		gzd
				Add wave end flag and vmd flag.

	1.2.1									2004.12.17		gzd
				Remove the operation clear stream fifos in stream end interrupt
				handlers.

	1.2.2									2004.12.24		gzd
				Add initializing the pointer psAesInfo and call	Vdev_WriteAes() for mp3 case
				in the function Vsyn_StreamUpdate().

	1.2.3									2004.12.28		gzd
				Add the operation of clear command fifo before calling Vdev_GetCFifoState().

	1.2.4									2005.03.03		gzd
				Fix a bug about a loud noise will output before playback a mp3 after stoping stream playbacking.


	1.2.5									2005.04.11		gzd
				Add Mp3 end callback code in Vsyn_StreamUpdate().
				
-----------------------------------------------------------------------*/

#include "vdef.h"
#include "vdev.h"
#include "vdevdef.h"
#include "vmd.h"
#include "vsyn.h"
#include "vconv.h"

//the declaration of structure variable about the synthesizer module information
VSYN_INFO	g_sVsyn_Info;
PVSYN_INFO	g_psVsyn_Info = &g_sVsyn_Info;

const UINT16		g_EFifoLen[3] = {VSYN_EFIFO_BUFLEN_1_4, VSYN_EFIFO_BUFLEN_1_2, VSYN_EFIFO_BUFLEN_3_4};
const UINT16		g_MFifoLen[3] = {VSYN_MFIFO_BUFLEN_1_4, VSYN_MFIFO_BUFLEN_1_2, VSYN_MFIFO_BUFLEN_3_4};
const UINT16		g_SFifoLen[4][3] = {{VSYN_SFIFO_DIV1_BUFLEN_1_4, VSYN_SFIFO_DIV1_BUFLEN_1_2, VSYN_SFIFO_DIV1_BUFLEN_3_4},
						{VSYN_SFIFO_DIV2_BUFLEN_1_4, VSYN_SFIFO_DIV2_BUFLEN_1_2, VSYN_SFIFO_DIV2_BUFLEN_3_4},
						{VSYN_SFIFO_DIV3_BUFLEN_1_4, VSYN_SFIFO_DIV3_BUFLEN_1_2, VSYN_SFIFO_DIV3_BUFLEN_3_4},
						{VSYN_SFIFO_DIV4_BUFLEN_1_4, VSYN_SFIFO_DIV4_BUFLEN_1_2, VSYN_SFIFO_DIV4_BUFLEN_3_4}
					};
const UINT16		g_SFifoBAddr[4][4] = {{VDEV_SFIFO_DIV_1_BADDR0},
						{VDEV_SFIFO_DIV_2_BADDR0, VDEV_SFIFO_DIV_2_BADDR1},
						{VDEV_SFIFO_DIV_3_BADDR0, VDEV_SFIFO_DIV_3_BADDR1, VDEV_SFIFO_DIV_3_BADDR2},
						{VDEV_SFIFO_DIV_4_BADDR0, VDEV_SFIFO_DIV_4_BADDR1, VDEV_SFIFO_DIV_4_BADDR2, VDEV_SFIFO_DIV_4_BADDR3}
					};
const UINT16		g_SFifoEndPtr[] = {VDEV_SFIFO_DIV_1_ENDPTR,	VDEV_SFIFO_DIV_2_ENDPTR, VDEV_SFIFO_DIV_3_ENDPTR, VDEV_SFIFO_DIV_4_ENDPTR};
const UINT8			g_sFifoWm[4][3] = {{VDEV_SFIFO_DIV_1_WM_1_4,VDEV_SFIFO_DIV_1_WM_1_2,VDEV_SFIFO_DIV_1_WM_3_4},
						{VDEV_SFIFO_DIV_2_WM_1_4,VDEV_SFIFO_DIV_2_WM_1_2,VDEV_SFIFO_DIV_2_WM_3_4},
						{VDEV_SFIFO_DIV_3_WM_1_4,VDEV_SFIFO_DIV_3_WM_1_2,VDEV_SFIFO_DIV_3_WM_3_4},
						{VDEV_SFIFO_DIV_4_WM_1_4,VDEV_SFIFO_DIV_4_WM_1_2,VDEV_SFIFO_DIV_4_WM_3_4}
					};

UINT32	dGetBufferLen = 0;

#if(FILESTREAM_MODE == FILESTREAM_ASYNC)

extern	UINT8	bBufferFlag[BUFFER_EMPTY_TIME];
extern	UINT8	bWBufferFlag;
extern	UINT8	bRBufferFlag;

#endif

//malloc static memory space for stream data.
UINT8		g_bStmBuf[VDEV_MAX_STREAM_FIFO_LEN];

//the declaration of internal functions
VRESULT	Vsyn_EventCnt		(UINT8 bFifoType, UINT16 wBufLen);
VRESULT Vsyn_StreamUpdate	(UINT8 bFifoType, PVOID pExtPara);
VRESULT Vsyn_EventUpdate	(UINT8 bFifoType, PVOID pExtPara);
VOID	Vsyn_AddIsr			(UINT8 bFifoType);
VOID	Vsyn_RemoveIsr		(UINT8 bFifoType);

/********************************************************************
 Description:
 		Initialize synthsizer module and low level modules.

 Parameters:
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_Initialize(PVOID pExtPara)
{
	PVSYN_INFO	pVsyn = g_psVsyn_Info;

	pVsyn->bSynthID		= 0x00;
	pVsyn->bSynthState	= VSYN_STATE_INIT;

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
 		Terminate synthsizer module and low level modules.

 Parameters:
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_Terminate(PVOID pExtPara)
{
	PVSYN_INFO	pVsyn = g_psVsyn_Info;

	pVsyn->bSynthID		= 0x00;
	pVsyn->bSynthState	= VSYN_STATE_UNINIT;

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
 		Create the data model of synthsizer module.

 Parameters:
 		pGetMsg:		[input] pointer to the callback function of getting message
 						data from parser modules.
 		pGetStm:		[input] pointer to the callback function of getting stream
 						data from parser modules.
 		pCbEof:			[input] pointer to the callback function that handles the
 						end of the playback of the special fifo.
 		pExtPara:		the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_Create(PGETMESSAGE pGetMsg, PGETSTREAM pGetStm, PCALLBACKEOF pCbEof, PVOID pExtPara)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;

	if(pVsyn->bSynthState == VSYN_STATE_CREATE)
		return VRES_SYNTH_HAS_CREATED;

	if (pGetMsg == NULL || pGetStm == NULL || pCbEof == NULL)
		return VRES_ERROR_PARAMETERS;

	pVsyn->bAudioMode 					= VDEV_AUDIO_MODE_UNDEF;
	pVsyn->bVmdMode					= 0;
	pVsyn->bStmEndFlag				= 0;
	pVsyn->sEFifo_Info.bFifoState 	= VSYN_FIFO_STATE_FREE;
	pVsyn->sEFifo_Info.bEFifoWaterM	= VDEV_FIFO_WATERM_1_2;
	pVsyn->sEFifo_Info.wEFifoBufLen	= VSYN_EFIFO_BUFLEN_1_2;
	pVsyn->sEFifo_Info.sEbuf_Info.wEbufLen  = VDEV_MAX_EVENT_FIFO_LEN << 1;
	pVsyn->sEFifo_Info.sEbuf_Info.wEdataLen = 0x00;
	pVsyn->sEFifo_Info.sEbuf_Info.wReadPtr  = 0x00;
	pVsyn->sEFifo_Info.sEbuf_Info.wWritePtr = 0x00;

	pVsyn->sCFifo_Info.bFifoState = VSYN_FIFO_STATE_FREE;
	pVsyn->sCFifo_Info.sCbuf_Info.wCbufLen  = VDEV_MAX_COMMAND_FIFO_LEN;
	pVsyn->sCFifo_Info.sCbuf_Info.wCdataLen = 0x00;
	pVsyn->sCFifo_Info.sCbuf_Info.wReadPtr  = 0x00;
	pVsyn->sCFifo_Info.sCbuf_Info.wWritePtr = 0x00;

	pVsyn->pGetMessage = pGetMsg;
	pVsyn->pGetStream  = pGetStm;
	pVsyn->pCallBackEof = pCbEof;

	pVsyn->bSynthID += 1;
	pVsyn->bSynthState = VSYN_STATE_CREATE;

	Vcnt_ConverterInit();		//Converter module initialization

	return VRES_SUCCESS;
}


/********************************************************************
 Description:
 		Delete the data model of synthsizer module.

 Parameters:
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_Delete(PVOID pExtPara)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;

	if (pVsyn->bSynthID < 1 || pVsyn->bSynthState != VSYN_STATE_CREATE)
		return VRES_SYNTH_NO_EXIST;
	
	pVsyn->bSynthID 		-= 1;
	pVsyn->bSynthState 	= VSYN_STATE_INIT;
	pVsyn->bAudioMode 	= VDEV_AUDIO_MODE_UNDEF;
	
	pVsyn->sEFifo_Info.bFifoState 	= VSYN_FIFO_STATE_UNDEF;
	pVsyn->sCFifo_Info.bFifoState 	= VSYN_FIFO_STATE_UNDEF;
	
	pVsyn->pGetMessage = NULL;
	pVsyn->pGetStream  = NULL;
	pVsyn->pCallBackEof = NULL;

	return VRES_SUCCESS;	
}


/********************************************************************
 Description:
 		Set audio mode and stream fifo number if select wave+midi mode.

 Parameters:
 		bAudioMode:		[input] One of the following <U>ENUM_AUDIO_MODE</U>
 						enumeration values to specify an audio mode 
 						<P>VDEV_AUDIO_MODE_HOST_WAVE_MIDI 
 						<P>VDEV_AUDIO_MODE_HOST_MP3_MIDI.
 		bStmFifoNum:	[input] if the audio mode is wave+midi, this parameter
 						indicate the number of the streams which can be 
 						played back at the same time. Currently the range
 						of the number is from 1 to 4.

 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.
		
 Remarks:

********************************************************************/
VRESULT		Vsyn_SetAudioMode(UINT8 bAudioMode, UINT8 bStmFifoNum)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;
	UINT8	   i;
	
	if (bAudioMode == VDEV_AUDIO_MODE_HOST_WAVE_MIDI)
	{
		if (bStmFifoNum == 0 || bStmFifoNum > VDEV_MAX_STREAM_FIFO_NUM)
			return VRES_ERROR_PARAMETERS;
	
		pVsyn->bAudioMode	= bAudioMode;
		pVsyn->uSFifo_Info.sStmFifo_Info.bFifoNum = bStmFifoNum;

		for (i=0; i<VDEV_MAX_STREAM_FIFO_NUM; i++)
			pVsyn->sFifoState.bStmFifoState[i] = VSYN_FIFO_STATE_UNDEF;
		
		for (i=0; i<bStmFifoNum; i++)
		{
			pVsyn->uSFifo_Info.sStmFifo_Info.bStmFifoState[i]	= VSYN_FIFO_STATE_FREE;
			pVsyn->uSFifo_Info.sStmFifo_Info.bSFifoWaterM[i] 	= g_sFifoWm[bStmFifoNum-1][1];
			pVsyn->uSFifo_Info.sStmFifo_Info.wSFifoBufLen[i] 	= g_SFifoLen[bStmFifoNum-1][1];
			pVsyn->uSFifo_Info.sStmFifo_Info.wSFifoBaseAddr[i]	= g_SFifoBAddr[bStmFifoNum-1][i];
			pVsyn->uSFifo_Info.sStmFifo_Info.wSFifoEndPtr[i]		= g_SFifoEndPtr[bStmFifoNum-1];
			pVsyn->uSFifo_Info.sStmFifo_Info.sAes_Info[i].bAesMode	= VDEV_AES_MODE_UNDEF;

			pVsyn->sFifoState.bStmFifoState[i] = VSYN_FIFO_STATE_FREE;
		}
		pVsyn->sFifoState.bMp3FState  = VSYN_FIFO_STATE_UNDEF;
	
	}else if (bAudioMode == VDEV_AUDIO_MODE_HOST_MP3_MIDI)
	{
		pVsyn->bAudioMode	= bAudioMode;
		pVsyn->uSFifo_Info.sMp3Fifo_Info.bFifoState 	 = VSYN_FIFO_STATE_FREE;
		pVsyn->uSFifo_Info.sMp3Fifo_Info.bMFifoWaterM = VDEV_FIFO_WATERM_1_2;
		pVsyn->uSFifo_Info.sMp3Fifo_Info.sAes_Info.bAesMode = VDEV_AES_MODE_UNDEF;

		//pVsyn->uSFifo_Info.sMp3Fifo_Info.wMFifoBufLen = VDEV_MAX_MP3_FIFO_LEN >> 1;
#if(FILESTREAM_MODE == FILESTREAM_SYNC)
		pVsyn->uSFifo_Info.sMp3Fifo_Info.wMFifoBufLen = SECOND_BUFFER_SIZE;
#else
		pVsyn->uSFifo_Info.sMp3Fifo_Info.wMFifoBufLen = SECOND_BUFFER_SIZE/BUFFER_EMPTY_TIME;
#endif

		pVsyn->sFifoState.bMp3FState  = VSYN_FIFO_STATE_FREE;
		for (i=0; i<VDEV_MAX_STREAM_FIFO_NUM; i++)
			pVsyn->sFifoState.bStmFifoState[i] = VSYN_FIFO_STATE_UNDEF;
	}else
		return VRES_ERROR_PARAMETERS;

	Vcnt_SetFifoNum(bStmFifoNum);

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
	   Set water mark level for the special Fifo.
   
 Parameters:
	   bFifoType:  [input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_EVENT
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
	                <P>VDEV_FIFO_MP3
	   bWaterM:    [input] One of the following <U>ENUM_FIFO_WM</U> 
	   				enumeration values that specify the water mark level.
	   				<P>VDEV_FIFO_WATERM_1_4
	                <P>VDEV_FIFO_WATERM_1_2
	                <P>VDEV_FIFO_WATERM_3_4
	   pExtPara:   the extension parameter used for the future.
	   
 Returns:
	   <P>VRES_SUCCESS: the implementation is successful. 
	   <P>ERROR CODE:   the implementation is failed.
   
 Remarks:                                                            
	   If the water mark level of special fifo is not set, it will be 
	   the default value that specify it to 1/2 level.
********************************************************************/
VRESULT Vsyn_SetWaterMark(UINT8 bFifoType, UINT8 bWaterM, PVOID pExtPara)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;
	UINT8	   bStmNo;

	if (pVsyn->bSynthState != VSYN_STATE_CREATE || pVsyn->bSynthID < 1)
		return VRES_SYNTH_NO_EXIST;

	if (bWaterM != VDEV_FIFO_WATERM_1_4 && bWaterM != VDEV_FIFO_WATERM_1_2 && bWaterM != VDEV_FIFO_WATERM_3_4)
		return VRES_ERROR_PARAMETERS;

	if (bFifoType > VDEV_FIFO_MP3)
		return VRES_ERROR_PARAMETERS;
	
	if (bFifoType == VDEV_FIFO_EVENT)
	{
		pVsyn->sEFifo_Info.bEFifoWaterM = bWaterM;
		pVsyn->sEFifo_Info.wEFifoBufLen = g_EFifoLen[bWaterM];
	}
	else if (bFifoType == VDEV_FIFO_MP3)
	{
		if (pVsyn->bAudioMode != VDEV_AUDIO_MODE_HOST_MP3_MIDI)
			return VRES_ERROR_AUDIO_MODE;

		pVsyn->uSFifo_Info.sMp3Fifo_Info.bMFifoWaterM = bWaterM;
		pVsyn->uSFifo_Info.sMp3Fifo_Info.wMFifoBufLen = g_MFifoLen[bWaterM];
	}
	else if (bFifoType > VDEV_FIFO_COMMAND && bFifoType < VDEV_FIFO_MP3)
	{
		bStmNo = bFifoType - VDEV_FIFO_STREAM0;
		
		if (pVsyn->bAudioMode != VDEV_AUDIO_MODE_HOST_WAVE_MIDI)
			return VRES_ERROR_AUDIO_MODE;

		pVsyn->uSFifo_Info.sStmFifo_Info.bSFifoWaterM[bStmNo] = 
						g_sFifoWm[pVsyn->uSFifo_Info.sStmFifo_Info.bFifoNum-1][bWaterM];
		pVsyn->uSFifo_Info.sStmFifo_Info.wSFifoBufLen[bStmNo] = 
						g_SFifoLen[pVsyn->uSFifo_Info.sStmFifo_Info.bFifoNum-1][bWaterM];
	}
	else
		return VRES_ERROR_PARAMETERS;

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
 		Set AES mode for the special fifo type and input key (and initvec
 		if CBC mode) data.

 Parameters:
 		bAESMode:		[input] One of the following <U>ENUM_AES_MODE</U> 
 						enumeration values that specify the AES encrypt mode
 						of streams or mp3 data.
 						<P>VDEV_AES_MODE_ECB
 						<P>VDEV_AES_MODE_CBC 						
	   bFifoType: 		[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                	enumeration values that specify the fifo type.
	                	<P>VDEV_FIFO_STREAM0
	                	<P>VDEV_FIFO_STREAM1
	                	<P>VDEV_FIFO_STREAM2
	                	<P>VDEV_FIFO_STREAM3
	                	<P>VDEV_FIFO_MP3
 		pbKeyBuf:		[input] Pointer to the AES key data. the length of 
 						the AES key data should be VDEV_AES_KEY_LENGTH.
 		pbInitVecBuf:	[input] Pointer to the initial vector data for AES 
 						CBC mode.the length of the initial vector data for
 						AES CBC mode should be VDEV_AES_INITVEC_LENGTH.
 		pExtPara:		the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_SetAESKey(UINT8 bAESMode, UINT8 bFifoType, PUINT8 pbKeyBuf,PUINT8 pbInitVecBuf, PVOID pExtPara)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;
	UINT8	   bStmNo;

	if (pVsyn->bSynthState != VSYN_STATE_CREATE || pVsyn->bSynthID < 1)
		return VRES_SYNTH_NO_EXIST;

	if (bAESMode == VDEV_AES_MODE_UNDEF)
		return VRES_ERROR_PARAMETERS;

	if (pbKeyBuf == NULL)
		return VRES_ERROR_PARAMETERS;

	if (bFifoType == VDEV_FIFO_MP3)
	{
		if (pVsyn->bAudioMode != VDEV_AUDIO_MODE_HOST_MP3_MIDI)
			return VRES_ERROR_AUDIO_MODE;

		pVsyn->uSFifo_Info.sMp3Fifo_Info.sAes_Info.bAesMode = bAESMode;
		Vmd_MemCpy((HUGE UINT8*)(pVsyn->uSFifo_Info.sMp3Fifo_Info.sAes_Info.bAesKey), 
			(HUGE UINT8*)pbKeyBuf, VDEV_AES_KEY_LENGTH);
		if (bAESMode == VDEV_AES_MODE_ECB)
		{
		}else if (bAESMode == VDEV_AES_MODE_CBC)
		{
			if (pbInitVecBuf == NULL)
				return VRES_ERROR_PARAMETERS;
			
			Vmd_MemCpy((HUGE UINT8*)(pVsyn->uSFifo_Info.sMp3Fifo_Info.sAes_Info.bAesKey), 
				(HUGE UINT8*)pbInitVecBuf, VDEV_AES_INITVEC_LENGTH);
		}
		else
			return VRES_ERROR_PARAMETERS;
	}
	else if (bFifoType >= VDEV_FIFO_STREAM0 && bFifoType <= VDEV_FIFO_STREAM3)
	{
		bStmNo = bFifoType - VDEV_FIFO_STREAM0;
		if (pVsyn->bAudioMode != VDEV_AUDIO_MODE_HOST_WAVE_MIDI)
			return VRES_ERROR_AUDIO_MODE;

		pVsyn->uSFifo_Info.sStmFifo_Info.sAes_Info[bStmNo].bAesMode = bAESMode;
		Vmd_MemCpy((HUGE UINT8*)(pVsyn->uSFifo_Info.sStmFifo_Info.sAes_Info[bStmNo].bAesKey),
					(HUGE UINT8*)pbKeyBuf, VDEV_AES_KEY_LENGTH);
		if (bAESMode == VDEV_AES_MODE_ECB)
		{
		}else if (bAESMode == VDEV_AES_MODE_CBC)
		{
			if (pbInitVecBuf == NULL)
				return VRES_ERROR_PARAMETERS;
			
			Vmd_MemCpy((HUGE UINT8*)(pVsyn->uSFifo_Info.sStmFifo_Info.sAes_Info[bStmNo].bAesInitVec),
						(HUGE UINT8*)pbInitVecBuf, VDEV_AES_INITVEC_LENGTH);
		}
		else
			return VRES_ERROR_PARAMETERS;
	}
	else
		return VRES_ERROR_PARAMETERS;

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
 		Supply stream data or mp3 data to the Stream Fifos or MP3 fifo.

 Parameters:
		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
	                <P>VDEV_FIFO_MP3
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_StreamSetup(UINT8 bFifoType, UINT8 bVmdMode, PVOID pExtPara)
{
	PVSYN_INFO  pVsyn   = g_psVsyn_Info;
	PAES_INFO	psAesInfo;
	UINT32		dGetStmLen;
	UINT32		dLenOfFifo;
	UINT32		i;
	UINT8		bStmNo;
	
	if (pVsyn->bSynthState != VSYN_STATE_CREATE || pVsyn->bSynthID < 1)
		return VRES_SYNTH_NO_EXIST;

	if (bFifoType == VDEV_FIFO_MP3)
	{
		if (pVsyn->bAudioMode != VDEV_AUDIO_MODE_HOST_MP3_MIDI)
			return VRES_ERROR_AUDIO_MODE;

		if (pVsyn->uSFifo_Info.sMp3Fifo_Info.bFifoState != VSYN_FIFO_STATE_FREE)
			return VRES_MP3FIFO_NOT_FREE;

		
		Vdev_SetAudioMode(VDEV_AUDIO_MODE_HOST_WAVE_MIDI);
		Vdev_SetBaseAddr(VDEV_FIFO_STREAM0, 0);
		Vdev_SetEndPtr(VDEV_FIFO_STREAM0, VDEV_SFIFO_DIV_1_ENDPTR);
		Vdev_LoadAddr(VDEV_FIFO_STREAM0);

		Vmd_MemSet(g_bStmBuf, 0, (UINT32)(VDEV_SFIFO_DIV_1_ENDPTR+1) << 2);
		Vdev_WriteFifo(VDEV_FIFO_STREAM0, g_bStmBuf, (UINT16)(VDEV_SFIFO_DIV_1_ENDPTR+1) << 2);		

		Vsyn_AddIsr(bFifoType);
		Vdev_SetAudioMode(pVsyn->bAudioMode);
		Vdev_ClearFifo(VDEV_FIFO_MP3);

		if (bVmdMode)
			pVsyn->bVmdMode = 1;
		else
			pVsyn->bVmdMode = 0;
		
		dGetStmLen = pVsyn->pGetStream(bFifoType, g_bStmBuf, VDEV_MAX_MP3_FIFO_LEN, NULL);
		if (pVsyn->bVmdMode)
		{
			Vdev_AddIsr(VDEV_INT_SWI2, Vsyn_SWI2Handler);
			Vdev_UnMaskInt(VDEV_INT_SWI2);
		}
		
		psAesInfo = &(pVsyn->uSFifo_Info.sMp3Fifo_Info.sAes_Info);
		Vdev_WriteAes(psAesInfo->bAesMode, psAesInfo->bAesKey, psAesInfo->bAesInitVec);
		
		Vdev_SetWaterM(VDEV_FIFO_MP3, pVsyn->uSFifo_Info.sMp3Fifo_Info.bMFifoWaterM);
		Vdev_WriteFifo(bFifoType, g_bStmBuf, (UINT16)((dGetStmLen<VDEV_MAX_MP3_FIFO_LEN)?dGetStmLen:VDEV_MAX_MP3_FIFO_LEN));
		if (dGetStmLen >= VDEV_MAX_MP3_FIFO_LEN)
		{
			dGetStmLen = pVsyn->pGetStream(bFifoType, g_bStmBuf, (UINT32)SECOND_BUFFER_SIZE, NULL);
			dGetBufferLen = dGetStmLen;

			Vdev_UnMaskInt(VDEV_INT_MFIFOEMPTY);
			if (psAesInfo->bAesMode == VDEV_AES_MODE_CBC)
			{
				for (i=0; i<VDEV_AES_INITVEC_LENGTH; i++)
					psAesInfo->bAesInitVec[i] = g_bStmBuf[VDEV_MAX_MP3_FIFO_LEN-VDEV_AES_INITVEC_LENGTH+i];
			}
		}		
		else if (!(pVsyn->bVmdMode))
		{
				Vdev_ClearFlag(VDEV_INT_MP3_END);	
				Vdev_UnMaskInt(VDEV_INT_MP3_END);
		}
		
		pVsyn->uSFifo_Info.sMp3Fifo_Info.bFifoState = VSYN_FIFO_STATE_USED;
	}
	else if (bFifoType >= VDEV_FIFO_STREAM0 && bFifoType <= VDEV_FIFO_STREAM3)
	{
		bStmNo = bFifoType - VDEV_FIFO_STREAM0;
		
		if (pVsyn->bAudioMode != VDEV_AUDIO_MODE_HOST_WAVE_MIDI)
			return VRES_ERROR_AUDIO_MODE;

		if (pVsyn->uSFifo_Info.sStmFifo_Info.bFifoNum < (bStmNo + 1))
			return VRES_ERROR_PARAMETERS;
		
		if (pVsyn->uSFifo_Info.sStmFifo_Info.bStmFifoState[bStmNo] != VSYN_FIFO_STATE_FREE)
			return VRES_STMFIFO0_NOT_FREE;

		if (bVmdMode)
			pVsyn->bVmdMode |= 0x01<<bStmNo;
		else
			pVsyn->bVmdMode &= ~(0x01<<bStmNo);

		pVsyn->bStmEndFlag &= ~(0x03<<(bStmNo<<1));
		
		Vsyn_AddIsr(bFifoType);
		Vdev_SetAudioMode(pVsyn->bAudioMode);
		Vdev_ClearFifo(bFifoType);

		dLenOfFifo = (g_SFifoEndPtr[pVsyn->uSFifo_Info.sStmFifo_Info.bFifoNum-1] + 1) << 2;
		dGetStmLen = pVsyn->pGetStream(bFifoType, g_bStmBuf, dLenOfFifo, NULL);

		if (pVsyn->bVmdMode & (0x01<<bStmNo))
		{
			Vdev_AddIsr(VDEV_INT_SWI2, Vsyn_SWI2Handler);
			Vdev_UnMaskInt(VDEV_INT_SWI2);
		}else if(dGetStmLen < dLenOfFifo)
		{
			Vmd_MemSet((HUGE UINT8*)(&g_bStmBuf[dGetStmLen]), 0, dLenOfFifo-dGetStmLen);
			dGetStmLen = dLenOfFifo;
			pVsyn->bStmEndFlag += 0x01<<(bStmNo<<1);
		}
		
		psAesInfo = &(pVsyn->uSFifo_Info.sStmFifo_Info.sAes_Info[bStmNo]);
		Vdev_WriteAes(psAesInfo->bAesMode, psAesInfo->bAesKey, psAesInfo->bAesInitVec);
		
		Vdev_SetBaseAddr(bFifoType, pVsyn->uSFifo_Info.sStmFifo_Info.wSFifoBaseAddr[bStmNo]);
		Vdev_SetEndPtr(bFifoType, pVsyn->uSFifo_Info.sStmFifo_Info.wSFifoEndPtr[bStmNo]);
		Vdev_SetWaterM(bFifoType, pVsyn->uSFifo_Info.sStmFifo_Info.bSFifoWaterM[bStmNo]);
		Vdev_LoadAddr(bFifoType);
		
		Vdev_WriteFifo(bFifoType, g_bStmBuf, (UINT16)((dGetStmLen<dLenOfFifo)?dGetStmLen:dLenOfFifo));		//supply stream data to fill the special fifo.
		if (dGetStmLen >= dLenOfFifo)
		{
			Vdev_UnMaskInt((UINT8)(bStmNo + VDEV_INT_SFIFO0EMPTY));
			if (psAesInfo->bAesMode == VDEV_AES_MODE_CBC)
			{
				for (i=0; i<VDEV_AES_INITVEC_LENGTH; i++)
					psAesInfo->bAesInitVec[i] = g_bStmBuf[dLenOfFifo-VDEV_AES_INITVEC_LENGTH+i];
			}
		}
		
		pVsyn->uSFifo_Info.sStmFifo_Info.bStmFifoState[bStmNo] = VSYN_FIFO_STATE_USED;
	}
	else
		return VRES_ERROR_PARAMETERS;

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
 		Stream data update.

 Parameters:
		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
	                <P>VDEV_FIFO_MP3
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_StreamUpdate(UINT8 bFifoType, PVOID pExtPara)
{
	PVSYN_INFO 	pVsyn = g_psVsyn_Info;
	PAES_INFO	psAesInfo;
	UINT32		dGetStmLen;
	UINT16	 	wFifoWmLen;
	UINT16 		i;
	UINT16		wReadPtr = 0, wWritePtr = 0;
	UINT8		bStmNo;

#if(FILESTREAM_MODE == FILESTREAM_ASYNC)
	UINT8*		pbPtr;
#endif
	
	if (bFifoType == VDEV_FIFO_MP3)
	{

		if (pVsyn->bAudioMode != VDEV_AUDIO_MODE_HOST_MP3_MIDI)
			return VRES_ERROR_AUDIO_MODE;

		wFifoWmLen = pVsyn->uSFifo_Info.sMp3Fifo_Info.wMFifoBufLen;

		dGetStmLen = dGetBufferLen;
		//dGetStmLen = pVsyn->pGetStream(bFifoType, g_bStmBuf, (UINT32)wFifoWmLen, NULL);

		psAesInfo = &(pVsyn->uSFifo_Info.sMp3Fifo_Info.sAes_Info);
		Vdev_WriteAes(psAesInfo->bAesMode, psAesInfo->bAesKey, psAesInfo->bAesInitVec);
		
#if(FILESTREAM_MODE == FILESTREAM_SYNC)
		Vdev_WriteFifo(bFifoType, g_bStmBuf, (UINT16)(((UINT16)dGetStmLen<wFifoWmLen)?(UINT16)dGetStmLen:wFifoWmLen));	//supply mp3 data to fill the mp3 fifo.
		if ((UINT16)dGetStmLen >= wFifoWmLen)
			dGetBufferLen = pVsyn->pGetStream(bFifoType, g_bStmBuf, (UINT32)wFifoWmLen, NULL);
#else
		i = VPlr_CheckWBuffer();
		if (i != 0)
		{
			pbPtr = g_bStmBuf + ((i - 1) * SECOND_BUFFER_CELL);
			dGetStmLen = VPlr_GetWBufferSize(i - 1);
			Vdev_WriteFifo(bFifoType, pbPtr, (UINT16)dGetStmLen);
		}
#endif
		if ((UINT16)dGetStmLen < wFifoWmLen)
		{
			Vdev_MaskInt(VDEV_INT_MFIFOEMPTY);
			Vdev_RemoveIsr(VDEV_INT_MFIFOEMPTY);

			if (!pVsyn->bVmdMode)
			{
				Vdev_ClearFlag(VDEV_INT_MP3_END);	

				Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);
				wReadPtr = (UINT16)Vreg_ReadReg(VDEV_REG_MP3_RD_PTR_L) + (UINT16)(Vreg_ReadReg(VDEV_REG_MP3_RD_PTR_H) << 8);
				wWritePtr = (UINT16)Vreg_ReadReg(VDEV_REG_MP3_WR_PTR_L) + (UINT16)(Vreg_ReadReg(VDEV_REG_MP3_WR_PTR_H) << 8);
				if (!(wReadPtr-wWritePtr))
				{
					Vsyn_Mp3EndHandler();	
					return VRES_STREAM_DATA_END;
				}

				Vdev_UnMaskInt(VDEV_INT_MP3_END);
			}
			return VRES_STREAM_DATA_END;
		}
		else
		{
			if (psAesInfo->bAesMode == VDEV_AES_MODE_CBC)
			{
				for (i=0; i<VDEV_AES_INITVEC_LENGTH; i++)
					psAesInfo->bAesInitVec[i] = g_bStmBuf[wFifoWmLen-VDEV_AES_INITVEC_LENGTH+i];
			}
		}
	}
	else if (bFifoType >= VDEV_FIFO_STREAM0 && bFifoType <= VDEV_FIFO_STREAM3)
	{
		if (pVsyn->bAudioMode != VDEV_AUDIO_MODE_HOST_WAVE_MIDI)
			return VRES_ERROR_AUDIO_MODE;

		bStmNo = bFifoType - VDEV_FIFO_STREAM0;

		if (pVsyn->uSFifo_Info.sStmFifo_Info.bFifoNum < (bStmNo + 1))
			return VRES_ERROR_PARAMETERS;
		
		wFifoWmLen = pVsyn->uSFifo_Info.sStmFifo_Info.wSFifoBufLen[bStmNo]; 
		dGetStmLen = pVsyn->pGetStream(bFifoType, g_bStmBuf, wFifoWmLen, NULL);

		if (((UINT16)dGetStmLen < wFifoWmLen))
		{
			if(!(pVsyn->bVmdMode & (0x01 << bStmNo)))	
			{
				if ((pVsyn->bStmEndFlag & (0x03<<(bStmNo<<1))) != (0x03<<(bStmNo<<1)))
				{
					Vmd_MemSet((HUGE UINT8*)(&g_bStmBuf[dGetStmLen]), 0, wFifoWmLen-dGetStmLen);
					dGetStmLen = wFifoWmLen;
					pVsyn->bStmEndFlag += 0x01<<(bStmNo<<1);
				}
			}
		}
		psAesInfo = &(pVsyn->uSFifo_Info.sStmFifo_Info.sAes_Info[bStmNo]);
		Vdev_WriteAes(psAesInfo->bAesMode, psAesInfo->bAesKey, psAesInfo->bAesInitVec);
		
		Vdev_WriteFifo(bFifoType, g_bStmBuf, (UINT16)(((UINT16)dGetStmLen<wFifoWmLen)?(UINT16)dGetStmLen:wFifoWmLen));	//supply stream data to fill the special fifo.
		if ((UINT16)dGetStmLen < wFifoWmLen)
		{
			Vdev_MaskInt((UINT8)(bStmNo + VDEV_INT_SFIFO0EMPTY));
			Vdev_RemoveIsr((UINT8)(bStmNo + VDEV_INT_SFIFO0EMPTY));

			if (!(pVsyn->bVmdMode & (0x01 << bStmNo)))
			{
				if (pVsyn->bStmEndFlag & (0x03 << (bStmNo<<1)))
				{
					Vdev_UnMaskInt((UINT8)(bStmNo + VDEV_INT_STM0_END));
					Vdev_EnableChnStop(bFifoType);
				}
			}
			return VRES_STREAM_DATA_END;
		}
		else
		{
			if (psAesInfo->bAesMode == VDEV_AES_MODE_CBC)
			{
				for (i=0; i<VDEV_AES_INITVEC_LENGTH; i++)
					psAesInfo->bAesInitVec[i] = g_bStmBuf[wFifoWmLen-VDEV_AES_INITVEC_LENGTH+i];
			}
		}
	}
	else
		return VRES_ERROR_PARAMETERS;
	
	return VRES_SUCCESS;
}

/********************************************************************
 Description:
 		Convert Event data and send the data to event buffer.

 Parameters:
		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
	                <P>VDEV_FIFO_MP3
 		wBufLen:	[input] the length of the max event buffer.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT	Vsyn_EventCnt(UINT8 bFifoType, UINT16 wBufLen)
{
	PVSYN_INFO		pVsyn;
	PEBUF_INFO		pEbuf_info;
	PCBUF_INFO		pCbuf_info;
	VM_UNI_MSG		sVMmsg;
	VRESULT 		vResult;
	UINT16			wCntEventLen;

	pVsyn = g_psVsyn_Info;

	if (bFifoType == VDEV_FIFO_EVENT)
	{
		pEbuf_info = &(pVsyn->sEFifo_Info.sEbuf_Info);
		while((vResult = pVsyn->pGetMessage(VDEV_FIFO_EVENT, (PUINT8)&sVMmsg, NULL)) == VRES_SUCCESS)
		{
			wCntEventLen = pEbuf_info->wEbufLen - pEbuf_info->wEdataLen;
			vResult = Vcnt_Convert((PVOID)&sVMmsg, 
									&(pEbuf_info->bEbuf[pEbuf_info->wWritePtr]), 
									&wCntEventLen);
			if (vResult == VRES_SUCCESS)
			{
				if (wCntEventLen > 0 ) 
				{
					pEbuf_info->wWritePtr += wCntEventLen;
					pEbuf_info->wEdataLen += wCntEventLen;
		
					if (pEbuf_info->wEdataLen >= wBufLen)
						break;
				}
			}
			else
				break;
		}
	}
	else
	{
		pCbuf_info = &(pVsyn->sCFifo_Info.sCbuf_Info);
		while((vResult = pVsyn->pGetMessage(bFifoType, (PUINT8)&sVMmsg, NULL)) == VRES_SUCCESS)
		{
			wCntEventLen = pCbuf_info->wCbufLen-pCbuf_info->wCdataLen;
			vResult = Vcnt_Convert((PVOID)&sVMmsg, 
									&(pCbuf_info->bCbuf[pCbuf_info->wWritePtr]), 
									&wCntEventLen);
			if (vResult == VRES_SUCCESS)
			{
				if (wCntEventLen > 0 ) 
				{
					pCbuf_info->wWritePtr += wCntEventLen;
					pCbuf_info->wCdataLen += wCntEventLen;
		
					if (pCbuf_info->wCdataLen >= wBufLen)
						break;
				}
			}
			else
				break;
		}
	}
	
	return vResult;
}

/********************************************************************
 Description:
 		Setup Event data.

 Parameters:
 		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
 					enumeration values that specify the fifo type.
 					<P>VDEV_FIFO_EVENT
 					<P>VDEV_FIFO_COMMAND
 					<P>VDEV_FIFO_STREAM0
 					<P>VDEV_FIFO_STREAM1
 					<P>VDEV_FIFO_STREAM2
 					<P>VDEV_FIFO_STREAM3
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_EventSetup(UINT8 bFifoType, PVOID pExtPara)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;
	PEBUF_INFO pEbuf_info;
	PCBUF_INFO pCbuf_info;
	PKEY_BUF   pKeyBuf;
	VRESULT    vResult = VRES_SUCCESS;
	UINT16	   wCntEventLen;
	
	if (pVsyn->bSynthState != VSYN_STATE_CREATE || pVsyn->bSynthID < 1)
		return VRES_SYNTH_NO_EXIST;

	if (bFifoType == VDEV_FIFO_EVENT)
	{
		if (pVsyn->sEFifo_Info.bFifoState != VSYN_FIFO_STATE_FREE)
			return VRES_EVENTFIFO_NOT_FREE;

		vResult = Vsyn_EventCnt(VDEV_FIFO_EVENT, (VDEV_MAX_EVENT_FIFO_LEN<<1)-64);
		if (vResult != VRES_SUCCESS && vResult != VRES_END_FLAG)
			return vResult;

		if (pVsyn->bVmdMode)
		{
			Vdev_ClearSwi(VDEV_INT_ALL);
			Vdev_ClearFlag(VDEV_INT_SWI1);
			Vdev_ClearFlag(VDEV_INT_SWI2);

			Vdev_ClearStmFlag(0);
			Vdev_ClearStmFlag(1);
			Vdev_ClearStmFlag(2);
			Vdev_ClearStmFlag(3);
		}
		Vsyn_AddIsr(bFifoType);
		Vdev_UnMaskInt(VDEV_INT_SWI1);
		Vdev_CtrlSeq(VDEV_CTRL_RESET_SEQUENCER);
		Vdev_ClearFifo(VDEV_FIFO_EVENT);	
		Vdev_ClearFlag(VDEV_INT_EFIFOEMPTY);

		pVsyn->sEFifo_Info.bFifoState = VSYN_FIFO_STATE_USED;
		pVsyn->sFifoState.bEventFifoState = VSYN_FIFO_STATE_USED;

		pEbuf_info = &pVsyn->sEFifo_Info.sEbuf_Info;
		wCntEventLen = (pEbuf_info->wEdataLen<VDEV_MAX_EVENT_FIFO_LEN)?pEbuf_info->wEdataLen:VDEV_MAX_EVENT_FIFO_LEN;

		Vdev_SetWaterM(VDEV_FIFO_EVENT, pVsyn->sEFifo_Info.bEFifoWaterM);		
		Vdev_WriteFifo(VDEV_FIFO_EVENT, &pEbuf_info->bEbuf[pEbuf_info->wReadPtr], wCntEventLen);
		pEbuf_info->wReadPtr += wCntEventLen;
		pEbuf_info->wEdataLen -= wCntEventLen;
		if (vResult == VRES_SUCCESS || vResult == VRES_END_FLAG)
		{
			if (pEbuf_info->wEdataLen < VDEV_MAX_EVENT_FIFO_LEN)
				Vmd_MemCpy((HUGE UINT8*)(pEbuf_info->bEbuf), (HUGE UINT8*)(&pEbuf_info->bEbuf[pEbuf_info->wReadPtr]), pEbuf_info->wEdataLen);

			pEbuf_info->wWritePtr = pEbuf_info->wEdataLen;
			pEbuf_info->wReadPtr  = 0;

			Vdev_UnMaskInt(VDEV_INT_EFIFOEMPTY);
		}
	}
	else if (bFifoType == VDEV_FIFO_COMMAND)
	{
		Vdev_ClearFifo(VDEV_FIFO_COMMAND);	

		if (Vdev_GetCFifoState())
		{
			pKeyBuf = (PKEY_BUF)pExtPara;
			pCbuf_info = &(pVsyn->sCFifo_Info.sCbuf_Info);

			pCbuf_info->bCbuf[0] = 0x00;
			pCbuf_info->bCbuf[1] = 0x00;
			pCbuf_info->bCbuf[2] = 0x46;
			pCbuf_info->bCbuf[3] = 0x0A;
			pCbuf_info->bCbuf[4] = 0x18;
			pCbuf_info->bCbuf[5] = 0x00;
			pCbuf_info->bCbuf[6] = 0x00;
			pCbuf_info->bCbuf[7] = 0x12;
			pCbuf_info->bCbuf[8] = 0x11;
			pCbuf_info->bCbuf[9] = pKeyBuf->bTimbreID;
			pCbuf_info->bCbuf[10] = 0x00;
			pCbuf_info->bCbuf[11] = 0x00;
			pCbuf_info->bCbuf[12] = 0x45;
			pCbuf_info->bCbuf[13] = 0x11;
			pCbuf_info->bCbuf[14] = pKeyBuf->bKeyID;
			pCbuf_info->bCbuf[15] = 0x7F;
			pCbuf_info->bCbuf[16] = (UINT8)(pKeyBuf->wDuration);
			pCbuf_info->bCbuf[17] = (UINT8)(pKeyBuf->wDuration >> 8);

			pCbuf_info->wCdataLen = 18;
			pCbuf_info->wReadPtr  = 0;
			pCbuf_info->wWritePtr = 0;
			
			Vdev_WriteFifo(VDEV_FIFO_COMMAND, pCbuf_info->bCbuf, pCbuf_info->wCdataLen);
			pCbuf_info->wCdataLen = 0;
		}
		else
			return VRES_COMMANDFIFO_NOT_FREE;
	}
	else if (bFifoType >= VDEV_FIFO_STREAM0 && bFifoType <= VDEV_FIFO_STREAM3)
	{
		Vdev_ClearFifo(VDEV_FIFO_COMMAND);	

		if (Vdev_GetCFifoState())
		{
			vResult = Vsyn_EventCnt(bFifoType, VDEV_MAX_COMMAND_FIFO_LEN);
			if (vResult != VRES_SUCCESS && vResult != VRES_END_FLAG)
				return vResult;

			pCbuf_info = &(pVsyn->sCFifo_Info.sCbuf_Info);
			wCntEventLen = (pCbuf_info->wCdataLen<VDEV_MAX_COMMAND_FIFO_LEN)?pCbuf_info->wCdataLen:VDEV_MAX_COMMAND_FIFO_LEN;
			Vdev_WriteFifo(VDEV_FIFO_COMMAND, &(pCbuf_info->bCbuf[pCbuf_info->wReadPtr]), pCbuf_info->wCdataLen);
			pCbuf_info->wCdataLen = 0;
			pCbuf_info->wReadPtr = 0;
			pCbuf_info->wWritePtr = 0;
		}
		else
			return VRES_COMMANDFIFO_NOT_EMPTY;
	}
	else if (bFifoType == VDEV_FIFO_MP3)
	{
		Vdev_ClearFifo(VDEV_FIFO_COMMAND);	
			
		if (Vdev_GetCFifoState())
		{
			vResult = Vsyn_EventCnt(VDEV_FIFO_MP3, VDEV_MAX_COMMAND_FIFO_LEN);
			if (vResult != VRES_SUCCESS && vResult != VRES_END_FLAG)
				return vResult;
			
			pCbuf_info = &(pVsyn->sCFifo_Info.sCbuf_Info);
			wCntEventLen = (pCbuf_info->wCdataLen<VDEV_MAX_COMMAND_FIFO_LEN)?pCbuf_info->wCdataLen:VDEV_MAX_COMMAND_FIFO_LEN;
			Vdev_WriteFifo(VDEV_FIFO_COMMAND, &(pCbuf_info->bCbuf[pCbuf_info->wReadPtr]), pCbuf_info->wCdataLen);
			pCbuf_info->wCdataLen = 0;
			pCbuf_info->wReadPtr = 0;
			pCbuf_info->wWritePtr = 0;
		}
		else
			return VRES_COMMANDFIFO_NOT_EMPTY;
	}
	else 
		return VRES_ERROR_PARAMETERS;

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
 		Event data update.

 Parameters:
 		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
 					enumeration values that specify the fifo type.
 					<P>VDEV_FIFO_EVENT
 					<P>VDEV_FIFO_COMMAND
 					<P>VDEV_FIFO_STREAM0
 					<P>VDEV_FIFO_STREAM1
 					<P>VDEV_FIFO_STREAM2
 					<P>VDEV_FIFO_STREAM3
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_EventUpdate(UINT8 bFifoType, PVOID pExtPara)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;
	PEBUF_INFO pEbuf_Info;
	VRESULT    vResult = VRES_SUCCESS;
	UINT16	   wCntEventLen;

	if (bFifoType == VDEV_FIFO_EVENT)
	{
		pEbuf_Info = &pVsyn->sEFifo_Info.sEbuf_Info;
		
		Vdev_ClearFlag(VDEV_INT_EFIFOEMPTY);

		wCntEventLen = (pEbuf_Info->wEdataLen < pVsyn->sEFifo_Info.wEFifoBufLen) ? pEbuf_Info->wEdataLen:pVsyn->sEFifo_Info.wEFifoBufLen;
		if (wCntEventLen > 0)
		{
			Vdev_WriteFifo(VDEV_FIFO_EVENT, pEbuf_Info->bEbuf, wCntEventLen);
			pEbuf_Info->wReadPtr += wCntEventLen;
			pEbuf_Info->wEdataLen -= wCntEventLen;

			if (pEbuf_Info->wEdataLen < VDEV_MAX_EVENT_FIFO_LEN)
				Vmd_MemCpy((HUGE UINT8*)(pEbuf_Info->bEbuf), (HUGE UINT8*)(&pEbuf_Info->bEbuf[pEbuf_Info->wReadPtr]), pEbuf_Info->wEdataLen);
			else
			{
				Vmd_MemCpy((HUGE UINT8*)(pEbuf_Info->bEbuf), (HUGE UINT8*)(&pEbuf_Info->bEbuf[pEbuf_Info->wReadPtr]), VDEV_MAX_EVENT_FIFO_LEN);
				Vmd_MemCpy((HUGE UINT8*)(&(pEbuf_Info->bEbuf[VDEV_MAX_EVENT_FIFO_LEN])), 
							(HUGE UINT8*)(&pEbuf_Info->bEbuf[pEbuf_Info->wReadPtr+VDEV_MAX_EVENT_FIFO_LEN]),
							pEbuf_Info->wEdataLen-VDEV_MAX_EVENT_FIFO_LEN);
			}
		}

		pEbuf_Info->wWritePtr = pEbuf_Info->wEdataLen;
		pEbuf_Info->wReadPtr  = 0;

		if (pEbuf_Info->wEdataLen < pVsyn->sEFifo_Info.wEFifoBufLen)
		{
			vResult = Vsyn_EventCnt(VDEV_FIFO_EVENT, pVsyn->sEFifo_Info.wEFifoBufLen);
			if (vResult != VRES_SUCCESS)
			{ 
				if (vResult == VRES_END_FLAG && pEbuf_Info->wEdataLen > 0)
				{}
				else
				{
					Vdev_MaskInt(VDEV_INT_EFIFOEMPTY);
					Vdev_RemoveIsr(VDEV_INT_EFIFOEMPTY);
					pEbuf_Info->wEdataLen = 0;
					pEbuf_Info->wWritePtr = 0;
					pEbuf_Info->wReadPtr  = 0;
					
					return vResult;
				}
			}
		}
	}
	else
		return VRES_ERROR_PARAMETERS;

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
 		Start to play the data in the special fifo.

 Parameters:
 		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
 					enumeration values that specify the fifo type.
 					<P>VDEV_FIFO_EVENT
 					<P>VDEV_FIFO_COMMAND
 					<P>VDEV_FIFO_STREAM0
 					<P>VDEV_FIFO_STREAM1
 					<P>VDEV_FIFO_STREAM2
 					<P>VDEV_FIFO_STREAM3
 					<P>VDEV_FIFO_MP3
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_StartSeq(UINT8 bFifoType, PVOID pExtPara)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;

	if (pVsyn->bSynthState != VSYN_STATE_CREATE || pVsyn->bSynthID < 1)
		return VRES_SYNTH_NO_EXIST;

//	if (bFifoType == VDEV_FIFO_EVENT)
//		Vdev_CtrlSeq(VDEV_CTRL_RESET_SEQUENCER);

	Vdev_CtrlSeq(VDEV_CTRL_START_SEQUENCER);

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
 		Stop the playing of the data in the special fifo and mask 
 		correspondent interrupts.

 Parameters:
		bFifoType:		[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                	enumeration values that specify the fifo type.
	                	<P>VDEV_FIFO_EVENT
		                <P>VDEV_FIFO_STREAM0
		                <P>VDEV_FIFO_STREAM1
		                <P>VDEV_FIFO_STREAM2
		                <P>VDEV_FIFO_STREAM3
		                <P>VDEV_FIFO_MP3
 		pExtPara:		the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_StopSeq(UINT8 bFifoType, PVOID pExtPara)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;

	if (pVsyn->bSynthState != VSYN_STATE_CREATE || pVsyn->bSynthID < 1)
		return VRES_SYNTH_NO_EXIST;

	switch (bFifoType)
	{
		case VDEV_FIFO_EVENT:
			Vdev_ClearFifo(VDEV_FIFO_EVENT);
			Vdev_MaskInt(VDEV_INT_EFIFOEMPTY);
			Vdev_MaskInt(VDEV_INT_SWI1);
			
			Vcnt_ConverterReset((VDEV_FIFO_TYPE)bFifoType); //ZY:2004.11.4
			Vdev_UnSelectV8051RAM();
			Vdev_DeviceControl(VDEV_CTRL_RESET_V8051,0,0);

			pVsyn->sFifoState.bEventFifoState 		= VSYN_FIFO_STATE_FREE;
			pVsyn->sEFifo_Info.bFifoState 			= VSYN_FIFO_STATE_FREE;
			pVsyn->sEFifo_Info.sEbuf_Info.wEdataLen = 0;
			pVsyn->sEFifo_Info.sEbuf_Info.wReadPtr 	= 0;
			pVsyn->sEFifo_Info.sEbuf_Info.wWritePtr = 0;
			break;

		case VDEV_FIFO_COMMAND:
			Vdev_ClearFifo(VDEV_FIFO_COMMAND);
			break;

		case VDEV_FIFO_STREAM0:
			Vdev_MaskInt(VDEV_INT_SFIFO0EMPTY);

			pVsyn->sFifoState.bStmFifoState[0]					= VSYN_FIFO_STATE_FREE;
			pVsyn->uSFifo_Info.sStmFifo_Info.bStmFifoState[0]	= VSYN_FIFO_STATE_FREE;
			pVsyn->sCFifo_Info.sCbuf_Info.wCdataLen			= 0;
			pVsyn->sCFifo_Info.sCbuf_Info.wReadPtr			= 0;
			pVsyn->sCFifo_Info.sCbuf_Info.wWritePtr			= 0;
			break;
			
		case VDEV_FIFO_STREAM1:
			Vdev_MaskInt(VDEV_INT_SFIFO1EMPTY);

			pVsyn->sFifoState.bStmFifoState[1]					= VSYN_FIFO_STATE_FREE;
			pVsyn->uSFifo_Info.sStmFifo_Info.bStmFifoState[1]	= VSYN_FIFO_STATE_FREE;
			pVsyn->sCFifo_Info.sCbuf_Info.wCdataLen			= 0;
			pVsyn->sCFifo_Info.sCbuf_Info.wReadPtr			= 0;
			pVsyn->sCFifo_Info.sCbuf_Info.wWritePtr			= 0;
			break;
			
		case VDEV_FIFO_STREAM2:
			Vdev_MaskInt(VDEV_INT_SFIFO2EMPTY);

			pVsyn->sFifoState.bStmFifoState[2]					= VSYN_FIFO_STATE_FREE;
			pVsyn->uSFifo_Info.sStmFifo_Info.bStmFifoState[2]	= VSYN_FIFO_STATE_FREE;
			pVsyn->sCFifo_Info.sCbuf_Info.wCdataLen			= 0;
			pVsyn->sCFifo_Info.sCbuf_Info.wReadPtr			= 0;
			pVsyn->sCFifo_Info.sCbuf_Info.wWritePtr			= 0;
			break;
			
		case VDEV_FIFO_STREAM3:
			Vdev_MaskInt(VDEV_INT_SFIFO3EMPTY);

			pVsyn->sFifoState.bStmFifoState[3]					= VSYN_FIFO_STATE_FREE;
			pVsyn->uSFifo_Info.sStmFifo_Info.bStmFifoState[3]	= VSYN_FIFO_STATE_FREE;
			pVsyn->sCFifo_Info.sCbuf_Info.wCdataLen			= 0;
			pVsyn->sCFifo_Info.sCbuf_Info.wReadPtr			= 0;
			pVsyn->sCFifo_Info.sCbuf_Info.wWritePtr			= 0;
			break;
			
		case VDEV_FIFO_MP3:
			Vdev_MaskInt(VDEV_INT_MFIFOEMPTY);

			pVsyn->sFifoState.bMp3FState					= VSYN_FIFO_STATE_FREE;
			pVsyn->uSFifo_Info.sMp3Fifo_Info.bFifoState		= VSYN_FIFO_STATE_FREE;
			pVsyn->sCFifo_Info.sCbuf_Info.wCdataLen			= 0;
			pVsyn->sCFifo_Info.sCbuf_Info.wReadPtr			= 0;
			pVsyn->sCFifo_Info.sCbuf_Info.wWritePtr			= 0;
			break;

		case VDEV_FIFO_ALL:
			Vdev_MaskInt(VDEV_INT_ALL);
			Vdev_ClearFifo(VDEV_FIFO_ALL);
			Vdev_CtrlSeq(VDEV_CTRL_STOP_SEQUENCER);
			Vdev_CtrlSeq(VDEV_CTRL_RESET_SEQUENCER);
			break;
	}
	Vsyn_RemoveIsr(bFifoType);
	Vdev_StopChnl(bFifoType);

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
        Set the volume of the voice output.
 
 Parameters:
        bCtrlID:	[input] One of the following <U>ENUM_VDEV_VOL_CTRL_ID</U>
        			enumeration values that specify volume Control ID.
					VDEV_CTRL_SET_SPVOL			
					VDEV_CTRL_SET_HPVOL			
					VDEV_CTRL_SET_MIDIVOL		
					VDEV_CTRL_SET_WAVEVOL
		bVolL:		[input] the volume value of left channel.
        bVolR:		[input] the volume value of right channel.
 		
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

 Remarks:
		Only the parameter "bVolL" is valid when setting MP3_VOLSTEP or SPVOL,
		and the range of bVolL is 0-31 for MP3_VOLSETP, 0-15 for SPVOL.
	
*********************************************************************/
VRESULT Vsyn_SetVol(UINT8 bCtrlID, UINT8 bVolL, UINT8 bVolR)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;

	if (pVsyn->bSynthState != VSYN_STATE_CREATE || pVsyn->bSynthID < 1)
		return VRES_SYNTH_NO_EXIST;

	Vdev_SetVol(bCtrlID, bVolL, bVolR);

	return VRES_SUCCESS;
}

/********************************************************************
 Description:
 		Add interrupt handler attach to a hardware interrupt.

 Parameters:
		bFifoType:		[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                	enumeration values that specify the fifo type.
	                	<P>VDEV_FIFO_EVENT
		                <P>VDEV_FIFO_STREAM0
		                <P>VDEV_FIFO_STREAM1
		                <P>VDEV_FIFO_STREAM2
		                <P>VDEV_FIFO_STREAM3
		                <P>VDEV_FIFO_MP3
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_AddIsr(UINT8 bFifoType)
{
	if (bFifoType == VDEV_FIFO_EVENT)
	{
		Vdev_AddIsr(VDEV_INT_EFIFOEMPTY, Vsyn_EFifoHandler);
		Vdev_AddIsr(VDEV_INT_SWI1, Vsyn_SWI1Handler);
	}
	else if (bFifoType == VDEV_FIFO_STREAM0)
	{
		Vdev_AddIsr(VDEV_INT_SFIFO0EMPTY, Vsyn_SFifo0Handler);
		Vdev_AddIsr(VDEV_INT_STM0_END, Vsyn_Stm0EndHandler);
	}
	else if (bFifoType == VDEV_FIFO_STREAM1)
	{
		Vdev_AddIsr(VDEV_INT_SFIFO1EMPTY, Vsyn_SFifo1Handler);
		Vdev_AddIsr(VDEV_INT_STM1_END, Vsyn_Stm1EndHandler);
	}
	else if (bFifoType == VDEV_FIFO_STREAM2)
	{
		Vdev_AddIsr(VDEV_INT_SFIFO2EMPTY, Vsyn_SFifo2Handler);
		Vdev_AddIsr(VDEV_INT_STM2_END, Vsyn_Stm2EndHandler);
	}
	else if (bFifoType == VDEV_FIFO_STREAM3)
	{
		Vdev_AddIsr(VDEV_INT_SFIFO3EMPTY, Vsyn_SFifo3Handler);
		Vdev_AddIsr(VDEV_INT_STM3_END, Vsyn_Stm3EndHandler);
	}
	else if (bFifoType == VDEV_FIFO_MP3)
	{
		Vdev_AddIsr(VDEV_INT_MFIFOEMPTY, Vsyn_MFifoHandler);
		Vdev_AddIsr(VDEV_INT_MP3_END, Vsyn_Mp3EndHandler);
	}
}

/********************************************************************
 Description:
 		Remove a interrupt handler attached to a hardware interrupt.

 Parameters:
		bFifoType:		[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                	enumeration values that specify the fifo type.
	                	<P>VDEV_FIFO_EVENT
		                <P>VDEV_FIFO_STREAM0
		                <P>VDEV_FIFO_STREAM1
		                <P>VDEV_FIFO_STREAM2
		                <P>VDEV_FIFO_STREAM3
		                <P>VDEV_FIFO_MP3
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_RemoveIsr(UINT8 bFifoType)
{
	if (bFifoType == VDEV_FIFO_EVENT)
	{
		Vdev_RemoveIsr(VDEV_INT_EFIFOEMPTY);
		Vdev_RemoveIsr(VDEV_INT_SWI1);
	}
	else if (bFifoType == VDEV_FIFO_STREAM0)
	{
		Vdev_RemoveIsr(VDEV_INT_SFIFO0EMPTY);
		Vdev_RemoveIsr(VDEV_INT_STM0_END);
	}
	else if (bFifoType == VDEV_FIFO_STREAM1)
	{
		Vdev_RemoveIsr(VDEV_INT_SFIFO1EMPTY);
		Vdev_RemoveIsr(VDEV_INT_STM1_END);
	}
	else if (bFifoType == VDEV_FIFO_STREAM2)
	{
		Vdev_RemoveIsr(VDEV_INT_SFIFO2EMPTY);
		Vdev_RemoveIsr(VDEV_INT_STM2_END);
	}
	else if (bFifoType == VDEV_FIFO_STREAM3)
	{
		Vdev_RemoveIsr(VDEV_INT_SFIFO3EMPTY);
		Vdev_RemoveIsr(VDEV_INT_STM3_END);
	}
	else if (bFifoType == VDEV_FIFO_MP3)
	{
		Vdev_RemoveIsr(VDEV_INT_MFIFOEMPTY);
		Vdev_RemoveIsr(VDEV_INT_MP3_END);
	}
}

/********************************************************************
 Description:
 		the interrupt handler of the short timer0.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_STmrHandler	(VOID)
{
	Vdev_ClearFlag(VDEV_INT_STMR);
}

/********************************************************************
 Description:
 		the interrupt handler of the event fifo empty.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_EFifoHandler	(VOID)
{
	Vsyn_EventUpdate(VDEV_FIFO_EVENT, NULL);
}

/********************************************************************
 Description:
 		the interrupt handler of the long timer1.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_TmrHandler		(VOID)
{
	Vdev_ClearFlag(VDEV_INT_TMR);
}

/********************************************************************
 Description:
 		the interrupt handler of the event fifo error.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_EFifoErrHandler(VOID)
{
	Vdev_ClearFlag(VDEV_INT_EFIFOERROR);
}

/********************************************************************
 Description:
 		the interrupt handler of the stream fifo0 empty.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SFifo0Handler	(VOID)
{
	Vsyn_StreamUpdate(VDEV_FIFO_STREAM0, NULL);
}

/********************************************************************
 Description:
 		the interrupt handler of the stream fifo1 empty.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SFifo1Handler	(VOID)
{
	Vsyn_StreamUpdate(VDEV_FIFO_STREAM1, NULL);
}

/********************************************************************
 Description:
 		the interrupt handler of the stream fifo2 empty.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SFifo2Handler	(VOID)
{
	Vsyn_StreamUpdate(VDEV_FIFO_STREAM2, NULL);
}

/********************************************************************
 Description:
 		the interrupt handler of the stream fifo3 empty.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SFifo3Handler	(VOID)
{
	Vsyn_StreamUpdate(VDEV_FIFO_STREAM3, NULL);
}


/********************************************************************
 Description:
 		the interrupt handler of the mp3 fifo empty.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_MFifoHandler	(VOID)
{
	Vsyn_StreamUpdate(VDEV_FIFO_MP3, NULL);
}

/********************************************************************
 Description:
 		the interrupt handler of the software interrupt 1.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SWI1Handler	(VOID)
{
	if (Vdev_GetSwiState(0x02))
	{
		Vdev_ClearSwi(VDEV_INT_SWI1);
		Vdev_ClearFlag(VDEV_INT_SWI1);
		Vdev_MaskInt(VDEV_INT_SWI1);

		g_psVsyn_Info->pCallBackEof(VDEV_FIFO_EVENT, 0);
	}
}

/********************************************************************
 Description:
 		the interrupt handler of the software interrupt 2.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SWI2Handler	(VOID)
{
	UINT8 i;	
	
	if (Vdev_GetSwiState(0x04))
	{
		Vdev_ClearSwi(VDEV_INT_SWI2);
		Vdev_ClearFlag(VDEV_INT_SWI2);

		if (g_psVsyn_Info->bAudioMode == VDEV_AUDIO_MODE_HOST_MP3_MIDI)
			g_psVsyn_Info->pCallBackEof(VDEV_FIFO_MP3,0);
		else
		{
			for (i=0; i<4; i++)
			{
				if (Vdev_GetStmState(i))
				{
					Vdev_ClearStmFlag(i);
					g_psVsyn_Info->pCallBackEof((UINT8)(VDEV_FIFO_STREAM0 + i), 0);
				}	
			}
		}
	}
}

/********************************************************************
 Description:
 		the interrupt handler of the software interrupt 3.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SWI3Handler	(VOID)
{
	if (Vdev_GetSwiState(0x08))
	{
		Vdev_ClearSwi(VDEV_INT_SWI3);
		Vdev_ClearFlag(VDEV_INT_SWI3);
		Vdev_MaskInt(VDEV_INT_SWI3);
	}
}

/********************************************************************
 Description:
 		the interrupt handler of the software interrupt 4.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SWI4Handler	(VOID)
{
	if (Vdev_GetSwiState(0x10))
	{
		Vdev_ClearSwi(VDEV_INT_SWI4);
		Vdev_ClearFlag(VDEV_INT_SWI4);
		Vdev_MaskInt(VDEV_INT_SWI4);
	}
}

/********************************************************************
 Description:
 		the interrupt handler of the software interrupt 5.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SWI5Handler	(VOID)
{
	if (Vdev_GetSwiState(0x20))
	{
		Vdev_ClearSwi(VDEV_INT_SWI5);
		Vdev_ClearFlag(VDEV_INT_SWI5);
		Vdev_MaskInt(VDEV_INT_SWI5);
	}
}

/********************************************************************
 Description:
 		the interrupt handler of the software interrupt 6.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SWI6Handler	(VOID)
{
	if (Vdev_GetSwiState(0x40))
	{
		Vdev_ClearSwi(VDEV_INT_SWI6);
		Vdev_ClearFlag(VDEV_INT_SWI6);
		Vdev_MaskInt(VDEV_INT_SWI6);
	}
}

/********************************************************************
 Description:
 		the interrupt handler of the software interrupt 7.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_SWI7Handler	(VOID)
{
	if (Vdev_GetSwiState(0x80))
	{
		Vdev_ClearSwi(VDEV_INT_SWI7);
		Vdev_ClearFlag(VDEV_INT_SWI7);
		Vdev_MaskInt(VDEV_INT_SWI7);
	}
}

/********************************************************************
 Description:
 		the interrupt handler of the stream0 end interrupt.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_Stm0EndHandler	(VOID)
{
	PVSYN_INFO	pVsyn = g_psVsyn_Info;
	
	Vdev_ClearFlag(VDEV_INT_STM0_END);
	Vdev_MaskInt(VDEV_INT_STM0_END);
	Vdev_DisableChnStop(VDEV_FIFO_STREAM0);
//	Vdev_ClearFifo(VDEV_FIFO_STREAM0);
	pVsyn->pCallBackEof(VDEV_FIFO_STREAM0, 0);
}

/********************************************************************
 Description:
 		the interrupt handler of the stream1 end interrupt.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_Stm1EndHandler	(VOID)
{
	Vdev_ClearFlag(VDEV_INT_STM1_END);
	Vdev_MaskInt(VDEV_INT_STM1_END);
	Vdev_DisableChnStop(VDEV_FIFO_STREAM1);
//	Vdev_ClearFifo(VDEV_FIFO_STREAM1);
	g_psVsyn_Info->pCallBackEof(VDEV_FIFO_STREAM1, 0);
}

/********************************************************************
 Description:
 		the interrupt handler of the stream2 end interrupt.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_Stm2EndHandler	(VOID)
{
	Vdev_ClearFlag(VDEV_INT_STM2_END);
	Vdev_MaskInt(VDEV_INT_STM2_END);
	Vdev_DisableChnStop(VDEV_FIFO_STREAM2);
//	Vdev_ClearFifo(VDEV_FIFO_STREAM2);
	g_psVsyn_Info->pCallBackEof(VDEV_FIFO_STREAM2, 0);
}

/********************************************************************
 Description:
 		the interrupt handler of the stream3 end interrupt.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_Stm3EndHandler	(VOID)
{
	Vdev_ClearFlag(VDEV_INT_STM3_END);
	Vdev_MaskInt(VDEV_INT_STM3_END);
	Vdev_DisableChnStop(VDEV_FIFO_STREAM3);
//	Vdev_ClearFifo(VDEV_FIFO_STREAM3);
	g_psVsyn_Info->pCallBackEof(VDEV_FIFO_STREAM3, 0);
}

/********************************************************************
 Description:
 		the interrupt handler of the stream0 end interrupt.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_Mp3EndHandler	(VOID)
{
	PVSYN_INFO	pVsyn = g_psVsyn_Info;
	
	Vdev_MaskInt(VDEV_INT_MP3_END);
	Vdev_DisableChnStop(VDEV_FIFO_STREAM0);
	Vdev_ClearFifo(VDEV_FIFO_MP3);
	pVsyn->pCallBackEof(VDEV_FIFO_MP3, 0);
}

/********************************************************************
 Description:
 		Enable MIDI Led mode.

 Parameters:
 		bSynChn: the channel number that the Led would flicker with it.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_MidiLedEnable	(UINT8 bSynChn)
{
	
	if(bSynChn<16)
		Vcnt_ConverterControl(VCONV_CTRL_MIDI_LED, 1,  bSynChn, 0);
	else
		return;
}


/********************************************************************
 Description:
 		Disaable MIDI Led mode.

 Parameters:
 		bSynChn: the channel number that the Led would flicker with it.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_MidiLedDisable	(UINT8 bSynChn)
{
	if(bSynChn<16)
		Vcnt_ConverterControl(VCONV_CTRL_MIDI_LED, 0,  bSynChn, 0);
	else
		return;
}


/********************************************************************
 Description:
 		Enable MIDI Motor mode.

 Parameters:
 		bSynChn: the channle number that the Motor would vibrate with it.
		bInterval: the interval time between the motor vibrating.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_MidiMotorEnable	(UINT8 bSynChn, UINT8 bInterval)
{
	if(bSynChn<16)
		Vcnt_ConverterControl(VCONV_CTRL_MIDI_MOTOR, 1,  bSynChn, bInterval);
	else
		return;
}


/********************************************************************
 Description:
 		Disaable MIDI Motor mode.

 Parameters:
 		bSynChn: the channle number that the Motor would vibrate with it.
		bInterval: the interval time between the motor vibrating.
	
 Returns:
 		VOID.
 		
 Remarks:
 		
*********************************************************************/
VOID	Vsyn_MidiMotorDisable	(UINT8 bSynChn, UINT8 bInterval)
{
	if(bSynChn<16)
		Vcnt_ConverterControl(VCONV_CTRL_MIDI_MOTOR, 0,  bSynChn, bInterval);
	else
		return;
}

/********************************************************************
 Description:
 		Set the mp3 playing speed when a mp3 file is playing.

 Parameters:
 		bMp3Times:	[input] One of the following values that specify
 					the speed of a mp3 file play back.
 					0 - the current speed is one of the fourth times
 						of the original playing speed.
 					1 - the current speed is one of the half times
 						of the original playing speed.
 					2 - the current speed is two times of the original
 						playing speed.
				    3 - the current speed is four times of the original
					    playing speed.
				    4 - the current speed is the same as the original
					    playing speed.
 					
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.
 		
 Remarks:
 		
*********************************************************************/
VRESULT	Vsyn_SetMp3Speed	(UINT8 bMp3Times)
{
	PVSYN_INFO pVsyn = g_psVsyn_Info;
	UINT8	bEventBuf[16];

	if (pVsyn->bAudioMode == VDEV_AUDIO_MODE_HOST_MP3_MIDI)
	{
		if(bMp3Times < 5)
			Vcnt_ModifyMP3Fratio(bMp3Times, bEventBuf);
		else
			return VRES_ERROR_PARAMETERS;

		Vdev_ClearFifo(VDEV_FIFO_COMMAND);	

		if (Vdev_GetCFifoState())
		{
			Vdev_WriteFifo(VDEV_FIFO_COMMAND, bEventBuf, 13);
		}
		else
			return VRES_COMMANDFIFO_NOT_EMPTY;
	}
	else 
		return VRES_ERROR_AUDIO_MODE;

	return VRES_SUCCESS;
}


/********************************************************************
 Description:
 		Setup UDI bank header and data for play back DLS data.

 Parameters:
 		pbHeader:	[input] Pointer to the UDI bank header data buffer.
		dHeaderLen:	[input] the length of the UDI bank header data buffer
		pbDLSdata:	[input] Pointer to the UDI bank data buffer.
		dDataLen:	[input] the length of the UDI bank data buffer.
		pExtPara:	[input] the parameter for future use.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.
 		
 Remarks:
 		
*********************************************************************/
VRESULT Vsyn_SetupDLS(PUINT8 pbHeader, UINT32 dHeaderLen, PUINT8 pbDLSdata, UINT32 dDataLen, PVOID pExtPara)
{
	PVSYN_INFO  pVsyn   = g_psVsyn_Info;
	
	if (pbHeader == NULL || pbDLSdata == NULL || 
		dHeaderLen == 0 || dHeaderLen > VDEV_UDIBANK_HEADER_MAXLEN ||
		dDataLen == 0 || dDataLen > VDEV_UDIBANK_DATA_MAXLEN)
		return VRES_ERROR_PARAMETERS;
	
	if (pVsyn->bSynthState != VSYN_STATE_CREATE || pVsyn->bSynthID < 1)
		return VRES_SYNTH_NO_EXIST;

	if (pVsyn->bAudioMode != VDEV_AUDIO_MODE_HOST_WAVE_MIDI)
		return VRES_ERROR_AUDIO_MODE;
	
	if (pVsyn->uSFifo_Info.sStmFifo_Info.bFifoNum != 1)
		return VRES_ERROR_PARAMETERS;
	
	if (pVsyn->uSFifo_Info.sStmFifo_Info.bStmFifoState[0] != VSYN_FIFO_STATE_FREE)
		return VRES_STMFIFO0_NOT_FREE;

	Vdev_SetAudioMode(pVsyn->bAudioMode);
	Vdev_ClearFifo(VDEV_FIFO_STREAM0);

	Vdev_SelectV8051RAM();

	Vdev_SetBaseAddr(VDEV_FIFO_STREAM0, pVsyn->uSFifo_Info.sStmFifo_Info.wSFifoBaseAddr[0]);
	Vdev_SetEndPtr(VDEV_FIFO_STREAM0, pVsyn->uSFifo_Info.sStmFifo_Info.wSFifoEndPtr[0]);
	Vdev_LoadAddr(VDEV_FIFO_STREAM0);

	Vdev_WriteFifo(VDEV_FIFO_V8051RAM, pbHeader, (UINT16)dHeaderLen);
	Vdev_WriteFifo(VDEV_FIFO_STREAM0, pbDLSdata, (UINT16)dDataLen);
	
	return VRES_SUCCESS;
}


