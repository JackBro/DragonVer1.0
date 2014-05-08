/*
 *  VC0938RDK/vplayer.c
 *
 *  Copyright (C) 2004 Vimicro Corporation
 *  
 *  Written by TangMian	Email: tangmian@vimicro.com	Tel: 010-68948888-8902
 *
 *  Overview:
 *   This is the 938 Driver for 938 device. This file include many functions to be used to play music.
 *   We support PCM/ADPCM, WAVE, MIDI, MP3 and VMD file. 
 *   
 *
 *   2004,9,30:
 *	       Release. By TangMian
 *
 *   2004,10,11:
 * 		Fix bug in CheckChannel(..) function. Add GetCurTime(..) function. By TangMian
 *
 *   2004,10,12:
 *		Add VPlr_Pause() and VPlr_Resume() function. By TangMian
 *
 *   2004,10,13:
 *		Fix bug in ReleaseMidiSource(), ReleaseMp3Source(), ReleaseWaveSource() functions. 
 *		Can't set music state in these functions.
 *
 *   2004,10,13:
 *		Add some functions for getting information of mp3 file.
 *		VPlr_GetMp3Title(..), VPlr_GetMp3Artist(..), VPlr_GetMp3Album(..), VPlr_GetMp3Genre(..)
 *		VPlr_GetMp3Comment(..), VPlr_GetMp3Time(..)
 *
 *   2004,10,14:
 *		Add VPlr_Lock() and VPlr_Unlock() in some function.      
 *
 *   2004,10,14:
 *		Fix bug in VPlr_Load(..) Function. In MP3 Case, Call MP3 Parser's GetFileInfo(..)
 *
 *   2004,10,19:
 *		Fix bug in extern function VPlr_SeekFile(..) and VPlr_ReadFile(..)
 *
 *   2004,10,19:
 *		Fix bug in VPlr_Load(..) function. file_ptr is used in vParser->Load(..).
 *
 *   2004,10,21:
 *		Support FileStream on all type of files, but only one FileStream in the same time.
 *
 *   2004,10,21:
 *		Change VPlr_Pause(..) and VPlr_Resume(..) function.
 * 
 *   2004,10,21:
 *		Change VPlr_SetMidiVol(..), VPlr_SetWaveVol(..) and VPlr_SetSPVol(..) function.
 *
 *   2004,10,25:
 *		Add 938 State Switch function.	STANDY->IDLE->NORMAL; NORMAL->IDLE->STANDY.
 *
 *   2004,10,26:
 *		Add Set Total EQ and Get Total EQ function.
 *
 *   2004,10,26:
 *		Add Enable Total EQ and Disable Total EQ function.
 *
 *   2004,10,27:
 *		Change VPlr_Play function for supporting vmd seeking.
 *
 *   2004,10,28:
 *		Fix Bug in VPlr_Pause and VPlr_Resume function, Change some code in VPlr_Load(..) function.
 *
 *   2004,10,29:
 *		Add Vplr_GetCurPolyphonyNum() function for VMD Maker, not for user.
 *		Add Vplr_SetAESKey() function for VMD file AES encrypt.
 *
 *   2004,10,30:
 *		Add GetCurTime, Pause and Resume function.
 *
 *   2004,10,31:
 *		Fix Bug for Get Current Time Function.
 *
 *   2004,11,3:
 *		Update code.
 *
 *   2004,11,23:
 *		Add VPlr_PlayKeyEffect() function for Play Key Effect by gzd.
 *	 
 *   2004,11,29:
 *		Remove Set audio mode operation in case Midi in function VPlr_Play() by gzd.
 *		
 *   2004,12,08:
 *      		Move Unmute PA operations to case if (g_songlist == 1) in function VPlr_Stop()
 *      		for play multi format music by gzd.
 *
 *   2004,12,15
 *		Update parameter 2 of the function Vsyn_StreamSetup() for vmd flag.
 *   2004,12,28
 *      		Delete RESULT_FILESTREAM in Vpl_Load function by Andy,liu
 *
 *   2005,1,6
 *		Add Motor Shake without playing song.
 *
 *   2005,1,10
 *      		Add volume step control functions VPlr_SetMp3VolStep() and VPlr_SetGlbVolStep() by gzd.
 *
 *   2005,1,11
 *		Add VPlr_SetMP3EQEx(..) and VPlr_GetMP3EQ(..) function.
 *
 *   2005,1,11
 *		change 	pVapi->dFifoMap	&= ~(0x0F << 16) to pVapi->dFifoMap	&= ~(0x0F << 20) in VPlr_Load() for mp3 playback.
 *
 *   2005,1,24
 *		Change VPlr_SetHPVol() and VPlr_SetDacVol() function.
 *
 *   2005,01,31
 *		Correct the volume values of getting.
 *
 *	2005,03,03
 *		Add led & motor state.
 *
 *	2005,03,18
 *		Modify Motor control flow.
 *
 *	2005,03,20
 *		Add iMelody and Xmf parser support.
 *
 *	2005,03,21
 *		Add pa control when users listen music with headphone.
 *
 */

#include "vplayer.h"


static	UINT8			bSongFlag = NOTUSED;					// 0: No song, 1: has song

static	PEFUNCTION		pFunction[9];	// 0: MIDI_ID, 1-4: WAVE1_ID--WAVE4_ID, 5: VMD1_ID, 6: MP3_ID, 7: IMELODY_ID, 8: XMF_ID

static	KVMIDI			kMidi;
static	KVWAVE			kWave[4];
static	KVVMD			kVmd;
static	KVMP3			kMp3;
static	KVIMELODY		kImelody;
static	KVXMF			kXmf;

static	UINT32			dGameEffect_ID;

static	KVSOUNDINFO		kSoundInfo;

static	KVMDAPI			kVmdApi;
static	pKVMDAPI			pVapi;

static	KVVERSION		VC0938RDK_VER={2, 1, 6, 28, 'R', 5, 3, 22};	// 2.16028R 2005/3/22

extern IVPAR parser_array[];

static	UINT8*			pbFileStream_Ptr = NULL;

static	UINT8			bChannelTable[9];

		UINT8			g_bMotorState = 0;			//Added for Motorshake control by gzd on 2005.03.18
static	UINT8			g_bHeadphone = 0;			//Added for Headphone used by gzd on 2005.03.20

static	VRESULT			GetMessage(UINT8 bFifoType, PUINT8 pbMsgBuf, PVOID pExtPara);
static	UINT32			GetStream(UINT8 bFifoType, PUINT8 pbStmBuf, UINT32 dStmLen, PVOID pExtPara);
static	void			CheckChannel(UINT32* flag);
static	void			InitMidiSource(void);
static	void			InitWaveSource(UINT8 bChan);
static	void			InitVmdSource(void);
static	void			InitMP3Source(void);

static	UINT8			CheckMidiSource(void);
static	void			ReleaseMidiSource(void);
static	UINT8			CheckWaveSource(UINT8* statenum, UINT8* chnum);
static	void			ReleaseWaveSource(UINT8 statenum, UINT8 chnum);
static	UINT8			CheckMp3Resource(void);
static	void			ReleaseMp3Source(void);

static	void 			ReleaseVmdSource(void);

static	void			InitImelodySource(void);		//Added for initialize the iMelody global variables by gzd on 2005.03.18
static	void			InitXmfSource(void);			//Added for initialize the Xmf global variables by gzd on 2005.03.18

extern	UINT32			VPlr_GetCurTime(UINT32 func_id);
extern	VOID 			VPlr_Callback(UINT8 FifoType, PVOID pExtPara);

extern	SINT32 			VPlr_ReadFile(void* pFile_Ptr, UINT8* pbBuffer, UINT32 dFile_Size);
extern	SINT32			VPlr_SeekFile(void* pFile_Ptr, UINT32 dOffset);

KVRESULT VPlr_SetMp3VolStep(UINT8 bVol);

#if(FILESTREAM_MODE == FILESTREAM_ASYNC)

extern	UINT8	bBufferFlag[BUFFER_EMPTY_TIME];
extern	UINT8	bWBufferFlag;
extern	UINT8	bRBufferFlag;
extern	UINT8	g_bStmBuf[VDEV_MAX_STREAM_FIFO_LEN];

#endif

/***************************************************************
Description:
		Initialize  the Player,  and reset the 938 chip.
		
Parameters:
		void
		
Returns:
		if  success return  VRES_SUCCESS ,  else  VRES_VPLR_ERROR

****************************************************************/

KVRESULT  VPlr_Initialize()
{
	UINT8	bIndex;

	//if (pVapi->bGlobalState != STANDBY) 
	//	return RESULT_DEVICESTATE;

	//Global Initialize
	pVapi = &kVmdApi;

	pVapi->pM_midi			= &kMidi;
	pVapi->pM_wave[0]			= &kWave[0];
	pVapi->pM_wave[1]			= &kWave[1];
	pVapi->pM_wave[2]			= &kWave[2];
	pVapi->pM_wave[3]			= &kWave[3];
	pVapi->pM_vmd			= &kVmd;
	pVapi->pM_mp3			= &kMp3;
	pVapi->pM_imelody		= &kImelody;
	pVapi->pM_xmf			= &kXmf;
	pVapi->pM_SoundInfo		= &kSoundInfo;

	pVapi->dFifoMap			= 0;
	pVapi->bGlobalState		= STANDBY;
	pVapi->bGPIOState			= GENERAL_GPIO;
	pVapi->bMidiState			= NOTUSED;
	pVapi->bMp3State			= NOTUSED;
	pVapi->bVmdState			= NOTUSED;

	for(bIndex=0; bIndex<4; bIndex++)
		pVapi->bWaveState[bIndex] = NOTUSED;

	InitMidiSource();
	for(bIndex=0; bIndex<4; bIndex++)
		InitWaveSource(bIndex);
	InitVmdSource();
	InitMP3Source();	
	InitImelodySource();
	InitXmfSource();


	pVapi->pM_SoundInfo->bDACVol		= 15;
	pVapi->pM_SoundInfo->bHPNVol_L	= 15;
	pVapi->pM_SoundInfo->bHPNVol_R	= 15;
	pVapi->pM_SoundInfo->bMidiVol_L	= 127;
	pVapi->pM_SoundInfo->bMidiVol_R	= 127;
	pVapi->pM_SoundInfo->bPAVol	= 15;
	pVapi->pM_SoundInfo->bWaveVol_L	= 127;
	pVapi->pM_SoundInfo->bWaveVol_R	= 127;
	pVapi->pM_SoundInfo->bMp3VolStep = 0;
	pVapi->pM_SoundInfo->bGlbVolStep = 0;

	for(bIndex=0; bIndex<9; bIndex++)
	{
		bChannelTable[bIndex] = NOTUSED;
		pFunction[bIndex] = NULL;
	}

	pbFileStream_Ptr = NULL;

	VPlr_Init();

#if(FILESTREAM_MODE == FILESTREAM_ASYNC)
	VPlr_InitGetStream();
#endif

	//Reset 938 Chip
	VPlr_ResetChip();

	g_bMotorState = 0;
	g_bHeadphone = 0;
	bSongFlag = 0;

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Create  the Player:  set the chip idle mode. create vsyn point and 
		initial the banktable
Parameters:
		void

Returns:
		if  success return  VRES_SUCCESS ,  else  VRES_VPLRCREATE_ERROR

****************************************************************/
KVRESULT  VPlr_Create(void)
{
	SINT8	sbFlag;

	if (pVapi->bGlobalState != STANDBY) 
		return RESULT_DEVICESTATE;

	/* Set the chip hardware mode STANDY -> IDLE -> NORMAL  */
	Vdev_Standby2Idle();

	//Initialize synthesizer module
	Vsyn_Initialize(NULL);

	/* Create the synther */
    	sbFlag = Vsyn_Create(GetMessage, GetStream, VPlr_Callback, NULL);
	if (sbFlag < 0)
		return RESULT_CREATESYNC;

	/* Set the chip system work state */
	pVapi->bGlobalState = IDLE;

	return  RESULT_SUCCESS;
}


/***************************************************************
Description:
		Destory  the Player:  reset the chip to standby mode. delete vsyn point and 
		reset the banktable
Parameters:
		void	
		
Returns:
		if  success return  VRES_SUCCESS ,  else  VRES_VPLRDETORY_ERROR

****************************************************************/
KVRESULT  VPlr_Destory(void)
{
	SINT8	sbFlag;

	if (pVapi->bGlobalState != IDLE)
		return RESULT_DEVICESTATE;

	
	/* Delete the synther */
	sbFlag = Vsyn_Delete(NULL);				//Provided by Software Group
	if (sbFlag < 0)
		return RESULT_DELETESYNC;

	//Terminate synthesizer module
	Vsyn_Terminate(NULL);
	

	/* MODE Switch  IDLE -> STANDBY */
	Vdev_Idle2Standby();

	/* Set the chip system work state */
	pVapi->bGlobalState = STANDBY;

	return  RESULT_SUCCESS;
}


/***************************************************************
Description:
		Open  the Player:  set the chip to normal mode.

Parameters:
		void	
		
Returns:
		if  success return  VRES_SUCCESS ,  else  VRES_VPLRCREATE_ERROR

****************************************************************/
KVRESULT  VPlr_Open(void)
{
	if(pVapi->bGlobalState != IDLE)
		return   RESULT_DEVICESTATE;
			
	/* Set the chip mode from idle to NORMAL */ 
	Vdev_Idle2Normal();					//Idle to Normal state

	VWave_Init();
	
	/* Set the player state */
	pVapi->bGlobalState  = WORKING;

	return  RESULT_SUCCESS;
}


/***************************************************************
Description:
		Close  the Player:  reset the chip to idle mode.

Parameters:
		void
		
Returns:
		if  success return  VRES_SUCCESS ,  else  VRES_VPLRCLOSE_ERROR

****************************************************************/
KVRESULT  VPlr_Close(void)
{
	if(pVapi->bGlobalState != WORKING)
		return RESULT_DEVICESTATE;

	/* Set the chip mode  IDLE */ 
	Vdev_Normal2Idle();		//Normal to Idle state

	/* Set the state */
	pVapi->bGlobalState = IDLE;

	return  RESULT_SUCCESS;
}

/***************************************************************
Description:
		Load  the music file to Player, and operate the prime initialization to 
		ready for play
Parameters:
		*pbFilePtr :	the start address of music data in RAM or file handler of music file in ROM    	
		dFileSize  :	Music file size
		dSrc       :	Music source type
				0: Music data in RAM
				1: Music data in ROM
		pdFuncId   :	Return Function ID to user

Returns:
		if  success return  VRES_SUCCESS ,  else  VRES_VPLRLOAD_ERROR

****************************************************************/
KVRESULT  VPlr_Load(HUGE UINT8 *pbFilePtr, UINT32 dFileSize, UINT8 bSrc, UINT32* pdFuncId)
{
	UINT8	bIndex, bLoop, bChannel, bCurWave;
	UINT8	bBuf[32];
	HUGE UINT8*	pbSptr,*pbVir;
	UINT32	dFileType, dRes = 0,dFileTypeEx;
	UINT16  wRiffFormat = 0;
	UINT8   bVolumeDb = 0;
	
	PVOID	pFileInfo;
	FILE_INFO FileInfo;
	KCHANGE	cag;
	KVRESULT	error = RESULT_SUCCESS;

	pFileInfo = (PVOID)&FileInfo;
	

	if(pVapi->bGlobalState != WORKING)
		return  RESULT_DEVICESTATE;

	if(bSrc == 0)						//Source From RAM
	{
		pbSptr = pbFilePtr;
		pbVir = pbFilePtr;
	}
	else							//Source From ROM
	{
		//if (pbFileStream_Ptr != NULL)
		//	return RESULT_FILESTREAM;
		pbFileStream_Ptr = (UINT8*)pbFilePtr;
		VPlr_SeekFile((void*)pbFilePtr, 0);
		VPlr_ReadFile((void*)pbFilePtr, bBuf, 16*2);
		pbSptr = (HUGE UINT8*)bBuf;
		pbVir = (HUGE UINT8*)pbFilePtr;
	}
	
	dFileType = ((UINT32)pbSptr[3] << 24) + ((UINT32)pbSptr[2] << 16) + ((UINT32)pbSptr[1] << 8) + ((UINT32)pbSptr[0] << 0);
	if(dFileType==RIFF_HEADER)
	{
		dFileType= (pbSptr[11] << 24) + (pbSptr[10] << 16) + (pbSptr[9] << 8) + (pbSptr[8] << 0);
		if(dFileType!=RMID_FILE)
		{
			wRiffFormat = (UINT16)pbSptr[20] + ((UINT16)pbSptr[21]<<8);
			if(wRiffFormat == 0x55)
				dFileType = MP3_FILETYPE1;
		}
	}
	if((dFileType & 0x00ffffff) == 0x00334449)
		dFileType = MP3_FILETYPE1;
	if((dFileType & 0x0000e0ff) == 0x0000e0ff)
		dFileType = MP3_FILETYPE2;


	switch(dFileType)
	{
		case MIDI_FILE:
		case RMID_FILE:
			VPlr_EnterCriticalSection();
			if(CheckMidiSource())
			{
				pVapi->bMidiState = USED;
				bChannelTable[EVENT_CH] = MIDI_ID;
			}
			else
				error = RESULT_NOMIDICHN;
			VPlr_LeaveCriticalSection();
			if (error < 0)
				return error;
			if (pVapi->pM_midi->bState != W_IDLE)
				return RESULT_PLAYSTATE;
			if (pVapi->pM_midi->pVParser->Init() == VRES_ERROR)
			{
				ReleaseMidiSource();
				return RESULT_PARSERINIT;
			}
			pVapi->pM_midi->pbFilePtr = pbFilePtr;
			CheckChannel(&dRes);
			if (pVapi->pM_midi->pVParser->Load(pbFilePtr, dFileSize, dRes, bSrc) == VRES_ERROR)
			{
				pVapi->pM_midi->pVParser->End();
				ReleaseMidiSource();
				return RESULT_PARSERLOAD;
			}
			if (pVapi->pM_midi->pVParser->GetFileInfo(pFileInfo, VDEV_FIFO_EVENT) == VRES_ERROR)
			{
				pVapi->pM_midi->pVParser->Unload(VDEV_FIFO_EVENT);
				pVapi->pM_midi->pVParser->End();
				ReleaseMidiSource();
				return RESULT_PARSERINFO;
			}
			pVapi->pM_midi->dFileSize = dFileSize;
			pVapi->pM_midi->bSource = bSrc;
			pVapi->pM_midi->dTotalTime = FileInfo.dTotalTime;
			pVapi->pM_midi->dPauseTime = 0;
			pVapi->pM_midi->dRepeatNum = 0;
			pVapi->pM_midi->bState = W_LOADED;
			*pdFuncId = MIDI_ID;
			pVapi->dFifoMap	&= ~0x0F;
			break;

		case WAVE_FILE:
			VPlr_EnterCriticalSection();
			if(CheckWaveSource(&bCurWave, &bChannel))
			{
				pVapi->bWaveState[bCurWave] = USED;
				bChannelTable[STREAM0_CH+bChannel] = WAVE1_ID + bCurWave;
			}
			else
				error = RESULT_NOWAVECHN;
			VPlr_LeaveCriticalSection();
			if (error < 0)
				return error;
			if (pVapi->pM_wave[bCurWave]->bState != W_IDLE)
				return RESULT_PLAYSTATE;
/*			if (pVapi->pM_wave[bCurWave]->pVParser->Init() == VRES_ERROR)
			{
				ReleaseWaveSource(bCurWave, (bChannel + STREAM0_CH));
				return RESULT_PARSERINIT;
			}
*/			pVapi->pM_wave[bCurWave]->pbFilePtr = pbFilePtr;
			CheckChannel(&dRes);
			if (pVapi->pM_wave[bCurWave]->pVParser->Load(pbFilePtr, dFileSize, dRes, bSrc) == VRES_ERROR)
			{
				pVapi->pM_wave[bCurWave]->pVParser->End();
				ReleaseWaveSource(bCurWave, (UINT8)(bChannel + STREAM0_CH));
				return RESULT_PARSERLOAD;
			}
			if (pVapi->pM_wave[bCurWave]->pVParser->GetFileInfo(pFileInfo, (VDEV_FIFO_TYPE)(VDEV_FIFO_STREAM0+bCurWave)) == VRES_ERROR)
			{
				pVapi->pM_wave[bCurWave]->pVParser->Unload((VDEV_FIFO_TYPE)(VDEV_FIFO_STREAM0+bCurWave));
				pVapi->pM_wave[bCurWave]->pVParser->End();
				ReleaseWaveSource(bCurWave, (UINT8)(bChannel + STREAM0_CH));
				return RESULT_PARSERINFO;
			}
			if (pVapi->pM_wave[bCurWave]->pVParser->Configure((UINT32)(VDEV_FIFO_STREAM0+bCurWave), 0, VM_PARSER_SET_FIFO) == VRES_ERROR)
			{
				pVapi->pM_wave[bCurWave]->pVParser->Unload((VDEV_FIFO_TYPE)(VDEV_FIFO_STREAM0+bCurWave));
				pVapi->pM_wave[bCurWave]->pVParser->End();
				ReleaseWaveSource(bCurWave, (UINT8)(bChannel + STREAM0_CH));
				return RESULT_PARSERCONFIGURE;
			}
			pVapi->pM_wave[bCurWave]->dFileSize = dFileSize;
			pVapi->pM_wave[bCurWave]->bSource = bSrc;
			pVapi->pM_wave[bCurWave]->dTotalTime = FileInfo.dTotalTime;
			pVapi->pM_wave[bCurWave]->dPauseTime = 0;
			pVapi->pM_wave[bCurWave]->dRepeatNum = 0;
			pVapi->pM_wave[bCurWave]->bState = W_LOADED;
			*pdFuncId = WAVE1_ID + bCurWave;
			pVapi->dFifoMap	&= ~(0xF0 << (bCurWave<<2));
			break;

		case VMD_FILE:
			VPlr_EnterCriticalSection();
			if(pVapi->bMidiState == USED)
			{
				error = RESULT_NOWAVECHN;
				goto vmderr;
			}
			if(pVapi->bVmdState == USED)
			{
				error = RESULT_NOVMDCHN;
				goto vmderr;
			}
vmderr:
			if (error < 0) 
			{
				VPlr_LeaveCriticalSection();
				return error;
			}
			else
			{
				pVapi->bMidiState = USED;
				bChannelTable[EVENT_CH] = VMD1_ID;
				pVapi->bVmdState = USED;
			}
			VPlr_LeaveCriticalSection();

			if (pVapi->pM_vmd->bState != W_IDLE)
				return RESULT_PLAYSTATE;
			if (pVapi->pM_vmd->pVParser->Init() == VRES_ERROR)
			{
				ReleaseVmdSource();
				return RESULT_PARSERINIT;
			}
			pVapi->pM_vmd->pbFilePtr = pbFilePtr;
			CheckChannel(&dRes);
			dRes &= 0xfe;		//Because MIDI Channel has been occupied by VMD.
			if (pVapi->pM_vmd->pVParser->Load(pbFilePtr, dFileSize, dRes, bSrc) == VRES_ERROR)
			{
				pVapi->pM_vmd->pVParser->End();
				ReleaseVmdSource();
				return RESULT_PARSERLOAD;
			}
			if (pVapi->pM_vmd->pVParser->GetFileInfo(&FileInfo, VDEV_FIFO_EVENT) == VRES_ERROR)
			{
				pVapi->pM_vmd->pVParser->Unload(VDEV_FIFO_STREAM0);
				pVapi->pM_vmd->pVParser->End();
				ReleaseVmdSource();
				return RESULT_PARSERINFO;
			}
			pVapi->pM_vmd->bFileNum = 0;
			
			if(FileInfo.dVmdFlag & 0x01)	//For MIDI
			{
				pVapi->pM_vmd->bFileNum += 1;
			}
			if (FileInfo.dVmdFlag & 0x2)	//For Wave
			{
				pVapi->pM_vmd->bWave = USED;
				VPlr_EnterCriticalSection();
				bLoop = 0;
				for(bIndex=0; bIndex<VMD_MAX_WAVE_NUM; bIndex++)
				{
					if(bChannelTable[STREAM0_CH+bIndex] == NOTUSED)
						bLoop++;
				}
				if(FileInfo.bVmdWavTrkNum > bLoop)
				{
					pVapi->bVmdState = NOTUSED;
					if(FileInfo.dVmdFlag & 0x01)
					{
						pVapi->bMidiState = NOTUSED;
						bChannelTable[EVENT_CH] = NOTUSED;
					}
					VPlr_LeaveCriticalSection();
					error = RESULT_NOWAVECHN;
					return error;
				}
				for(bIndex=0; bIndex<FileInfo.bVmdWavTrkNum; bIndex++)
				{
					for(bLoop=0; bLoop<VMD_MAX_WAVE_NUM; bLoop++)
					{
						if(bChannelTable[STREAM0_CH+bLoop] == NOTUSED)
						{
							pVapi->pM_vmd->bVmdch[bIndex] = bLoop;
							bChannelTable[STREAM0_CH+bLoop] = VMD1_ID;
							pVapi->bWaveState[bLoop] = USED;
							break;
						}
					}
				}
				VPlr_LeaveCriticalSection();
				for(bIndex=0; bIndex<FileInfo.bVmdWavTrkNum; bIndex++)
				{
					if (pVapi->pM_vmd->pVParser->Configure((VDEV_FIFO_TYPE)(VDEV_FIFO_STREAM0+pVapi->pM_vmd->bVmdch[bIndex]), bIndex, VM_PARSER_SET_FIFO) == VRES_ERROR)
					{
						pVapi->pM_vmd->pVParser->Unload(VDEV_FIFO_STREAM0);
						pVapi->pM_vmd->pVParser->End();
						ReleaseVmdSource();
						return RESULT_PARSERCONFIGURE;
					}
					pVapi->pM_vmd->bTrackFile[bIndex] = FileInfo.bVmdWavDatNum[bIndex];
					pVapi->pM_vmd->bFileNum += FileInfo.bVmdWavDatNum[bIndex];
				}
				pVapi->pM_vmd->bTrackNum = FileInfo.bVmdWavTrkNum;
			}
			if (FileInfo.dVmdFlag & 0x20)	//For MP3
			{
				bIndex = 0;
				VPlr_EnterCriticalSection();
				if (pVapi->bMp3State == NOTUSED)
				{
					bChannelTable[MP3_CH] = VMD1_ID;
					pVapi->bMp3State = USED;
					bIndex = 1;
				}
				VPlr_LeaveCriticalSection();
				if (bIndex == 0)
				{
					ReleaseVmdSource();
					return RESULT_NOMP3CHN;
				}
				pVapi->pM_vmd->bMp3 = USED;
				pVapi->pM_vmd->bMp3Num = FileInfo.bVmdMp3Num;
				pVapi->pM_vmd->bFileNum += FileInfo.bVmdMp3Num;
			}
			pVapi->pM_vmd->dFileSize = dFileSize;
			pVapi->pM_vmd->bSource = bSrc;
			pVapi->pM_vmd->dTotalTime = FileInfo.dTotalTime;
			pVapi->pM_vmd->bSFileNum = pVapi->pM_vmd->bFileNum;
			pVapi->pM_vmd->bSTrackNum = pVapi->pM_vmd->bTrackNum;
			for (bIndex=0; bIndex<pVapi->pM_vmd->bTrackNum; bIndex++)
				pVapi->pM_vmd->bSTrackFile[bIndex] = pVapi->pM_vmd->bTrackFile[bIndex];
			pVapi->pM_vmd->bSMp3Num = pVapi->pM_vmd->bMp3Num;
			pVapi->pM_vmd->dPauseTime = 0;
			pVapi->pM_vmd->dRepeatNum = 0;
			pVapi->pM_vmd->bState = W_LOADED;
			*pdFuncId = VMD1_ID;
			if(FileInfo.dVmdFlag & 0x01)
				pVapi->dFifoMap |= 1;
			if(FileInfo.dVmdFlag & 0x02)
			{
				for(bIndex=0; bIndex<FileInfo.bVmdWavTrkNum; bIndex++)
				{
					bLoop = pVapi->pM_vmd->bVmdch[bIndex] + 1;
					pVapi->dFifoMap |= (1 << (bLoop * 4));
				}
			}
			if(FileInfo.dVmdFlag & 0x20)
				pVapi->dFifoMap |= (1 << 20);
			pVapi->dFifoMap |= 1;
			break;

		case MP3_FILETYPE1:
		case MP3_FILETYPE2:
			VPlr_EnterCriticalSection();
			if(CheckMp3Resource())
			{
				pVapi->bMp3State = USED;
				bChannelTable[MP3_CH] = MP3_ID;
			}
			else
				error = RESULT_NOMP3CHN;
			VPlr_LeaveCriticalSection();
			if (error < 0)
				return error;
			if (pVapi->pM_mp3->bState != W_IDLE)
				return RESULT_PLAYSTATE;
			if (pVapi->pM_mp3->pVParser->Init() == VRES_ERROR)
			{
				ReleaseMp3Source();
				return RESULT_PARSERINIT;
			}
			pVapi->pM_mp3->pbFilePtr = pbFilePtr;
			CheckChannel(&dRes);
			
			////////////////////////////Andy,liu add for mp3/////////////////////////////////////////

			if(bSrc)							//Source From ROM
			{
				VPlr_SeekFile((void*)pbVir,dFileSize-5);
				VPlr_ReadFile((void*)pbVir, bBuf, 5);
				pbVir = (HUGE UINT8*)bBuf;
			}
			else
			{
				pbVir = pbVir + dFileSize-5;
			}
			dFileTypeEx = ((UINT32)pbVir[3] << 24) + ((UINT32)pbVir[2] << 16) + ((UINT32)pbVir[1] << 8) + ((UINT32)pbVir[0] << 0);

			if((dFileTypeEx & 0x00ffffff) == 0x00524956) 
			{
				dFileTypeEx = MP3_FILETYPE3;
				bVolumeDb = pbVir[3];
			}

			/////////////////////////////////////////////////////////////////////////////////////////
			if(dFileTypeEx == MP3_FILETYPE3)
			{
				if (pVapi->pM_mp3->pVParser->Load(pbFilePtr, dFileSize-4, dRes, bSrc) == VRES_ERROR)
				{
					pVapi->pM_mp3->pVParser->End();
					ReleaseMp3Source();
					return RESULT_PARSERLOAD;
				}
				if(bVolumeDb>6)
					bVolumeDb = 6;
				VPlr_SetMp3VolStep(bVolumeDb);
			}
			else
			{
				if (pVapi->pM_mp3->pVParser->Load(pbFilePtr, dFileSize, dRes, bSrc) == VRES_ERROR)
				{
					pVapi->pM_mp3->pVParser->End();
					ReleaseMp3Source();
					return RESULT_PARSERLOAD;
				}
				VPlr_SetMp3VolStep(0);
			}
			if (pVapi->pM_mp3->pVParser->GetFileInfo(pFileInfo, VDEV_FIFO_MP3) == VRES_ERROR)
			{
				pVapi->pM_mp3->pVParser->Unload(VDEV_FIFO_EVENT);
				pVapi->pM_mp3->pVParser->End();
				ReleaseMp3Source();
				return RESULT_PARSERINFO;
			}
			if (pVapi->pM_mp3->pVParser->Configure(VDEV_FIFO_MP3, 0, VM_PARSER_SET_FIFO) == VRES_ERROR)
			{
				pVapi->pM_mp3->pVParser->Unload(VDEV_FIFO_MP3);
				pVapi->pM_mp3->pVParser->End();
				ReleaseMp3Source();
				return RESULT_PARSERCONFIGURE;
			}
			pVapi->pM_mp3->dFileSize = dFileSize;
			pVapi->pM_mp3->bSource = bSrc;
			pVapi->pM_mp3->dTotalTime = FileInfo.dTotalTime;
			pVapi->pM_mp3->dPauseTime = 0;
			pVapi->pM_mp3->dRepeatNum = 0;
			pVapi->pM_mp3->pbTitle = FileInfo.mp3_title;
			pVapi->pM_mp3->pbArtist = FileInfo.mp3_artist;
			pVapi->pM_mp3->pbAlbum = FileInfo.mp3_album;
			pVapi->pM_mp3->pbComment = FileInfo.mp3_comment;
			pVapi->pM_mp3->pbGenre = FileInfo.mp3_genre;
			for(bIndex=0; bIndex<4; bIndex++)
				cag.data[bIndex] = FileInfo.mp3_year[bIndex];
			pVapi->pM_mp3->dTime = cag.time;
			pVapi->pM_mp3->bState = W_LOADED;
			*pdFuncId = MP3_ID;
			pVapi->dFifoMap	&= ~(0x0F << 20);
			break;

		case IMELODY_FILE:
			VPlr_EnterCriticalSection();
			if(CheckMidiSource())
			{
				pVapi->bMidiState = USED;
				bChannelTable[EVENT_CH] = IMELODY_ID;
			}
			else
				error = RESULT_NOMIDICHN;
			VPlr_LeaveCriticalSection();
			if (error < 0)
				return error;
			if (pVapi->pM_imelody->bState != W_IDLE)
				return RESULT_PLAYSTATE;
			if (pVapi->pM_imelody->pVParser->Init() == VRES_ERROR)
			{
				ReleaseMidiSource();
				return RESULT_PARSERINIT;
			}
			pVapi->pM_imelody->pbFilePtr = pbFilePtr;
			CheckChannel(&dRes);
			if (pVapi->pM_imelody->pVParser->Load(pbFilePtr, dFileSize, dRes, bSrc) == VRES_ERROR)
			{
				pVapi->pM_imelody->pVParser->End();
				ReleaseMidiSource();
				return RESULT_PARSERLOAD;
			}
			if (pVapi->pM_imelody->pVParser->GetFileInfo(pFileInfo, VDEV_FIFO_EVENT) == VRES_ERROR)
			{
				pVapi->pM_imelody->pVParser->Unload(VDEV_FIFO_EVENT);
				pVapi->pM_imelody->pVParser->End();
				ReleaseMidiSource();
				return RESULT_PARSERINFO;
			}
			pVapi->pM_imelody->dFileSize = dFileSize;
			pVapi->pM_imelody->bSource = bSrc;
			pVapi->pM_imelody->dTotalTime = FileInfo.dTotalTime;
			pVapi->pM_imelody->dPauseTime = 0;
			pVapi->pM_imelody->dRepeatNum = 0;
			pVapi->pM_imelody->bState = W_LOADED;
			*pdFuncId = IMELODY_ID;
			pVapi->dFifoMap	|= 0x0F000000;
			break;

		case XMF_FILE:
			VPlr_EnterCriticalSection();
			if(CheckMidiSource())
			{
				pVapi->bMidiState = USED;
				bChannelTable[EVENT_CH] = XMF_ID;
			}
			else
				error = RESULT_NOMIDICHN;
			VPlr_LeaveCriticalSection();
			if (error < 0)
				return error;
			if (pVapi->pM_xmf->bState != W_IDLE)
				return RESULT_PLAYSTATE;
			if (pVapi->pM_xmf->pVParser->Init() == VRES_ERROR)
			{
				ReleaseMidiSource();
				return RESULT_PARSERINIT;
			}
			pVapi->pM_xmf->pbFilePtr = pbFilePtr;
			CheckChannel(&dRes);
			if (pVapi->pM_xmf->pVParser->Load(pbFilePtr, dFileSize, dRes, bSrc) == VRES_ERROR)
			{
				pVapi->pM_xmf->pVParser->End();
				ReleaseMidiSource();
				return RESULT_PARSERLOAD;
			}
			if (pVapi->pM_xmf->pVParser->GetFileInfo(pFileInfo, VDEV_FIFO_EVENT) == VRES_ERROR)
			{
				pVapi->pM_xmf->pVParser->Unload(VDEV_FIFO_EVENT);
				pVapi->pM_xmf->pVParser->End();
				ReleaseMidiSource();
				return RESULT_PARSERINFO;
			}
			pVapi->pM_xmf->dFileSize = dFileSize;
			pVapi->pM_xmf->bSource = bSrc;
			pVapi->pM_xmf->dTotalTime = FileInfo.dTotalTime;
			pVapi->pM_xmf->dPauseTime = 0;
			pVapi->pM_xmf->dRepeatNum = 0;
			pVapi->pM_xmf->bState = W_LOADED;
			*pdFuncId = XMF_ID;
			pVapi->dFifoMap	|= 0xF0000000;
			break;


		default:
			return RESULT_UNKNOWN;
	}

	return  RESULT_SUCCESS;
}


/***************************************************************
Description:
		Unload  the Player:  delete  the music point. and  unload the parsered event

Parameters:
		dFuncId :	Music file ID	
		
Returns:
		if  success return  VRES_SUCCESS ,  else  VRES_VPLRUNLOAD_ERROR

****************************************************************/
KVRESULT  VPlr_Unload(UINT32 dFuncId)
{
	UINT8	bIndex, bChan;

	if (pVapi->bGlobalState != WORKING) 
		return  RESULT_DEVICESTATE;
	
	switch(dFuncId)
	{
		case MIDI_ID:
			if (pVapi->pM_midi->bState != W_LOADED)
				return RESULT_PLAYSTATE;
			pVapi->pM_midi->pVParser->Unload(VDEV_FIFO_EVENT);
			pVapi->pM_midi->pVParser->End();
			pVapi->pM_midi->bState = W_IDLE;
			if (pVapi->pM_midi->bSource)
					pbFileStream_Ptr = NULL;
			ReleaseMidiSource();
			break;

		case WAVE1_ID:
		case WAVE2_ID:
		case WAVE3_ID:
		case WAVE4_ID:
			bChan = (UINT8)dFuncId - WAVE1_ID;
			if (pVapi->pM_wave[bChan]->bState != W_LOADED)
				return RESULT_PLAYSTATE;
			for(bIndex=0; bIndex<VMD_MAX_WAVE_NUM; bIndex++)
			{
				if (bChannelTable[STREAM0_CH + bIndex] == (UINT8)dFuncId)
					break;
			}
			pVapi->pM_wave[bChan]->pVParser->Unload((VDEV_FIFO_TYPE)(VDEV_FIFO_STREAM0 + bIndex));
			pVapi->pM_wave[bChan]->pVParser->End();
			pVapi->pM_wave[bChan]->bState = W_IDLE;
			if (pVapi->pM_wave[bChan]->bSource)
					pbFileStream_Ptr = NULL;
			ReleaseWaveSource(bChan, (UINT8)(bIndex + STREAM0_CH));
			break;
			
		case VMD1_ID:
			if (pVapi->pM_vmd->bState != W_LOADED)
				return RESULT_PLAYSTATE;
			pVapi->pM_vmd->pVParser->Unload(VDEV_FIFO_STREAM0);
			pVapi->pM_vmd->pVParser->End();
			pVapi->pM_vmd->bState = W_IDLE;
			if (pVapi->pM_vmd->bSource)
					pbFileStream_Ptr = NULL;
			ReleaseVmdSource();
			break;
			
		case MP3_ID:
			if (pVapi->pM_mp3->bState != W_LOADED) 
				return RESULT_PLAYSTATE;
			pVapi->pM_mp3->pVParser->Unload(VDEV_FIFO_MP3);
			pVapi->pM_mp3->pVParser->End();
			pVapi->pM_mp3->bState = W_IDLE;
			if (pVapi->pM_mp3->bSource)
					pbFileStream_Ptr = NULL;
			ReleaseMp3Source();
			break;

		case IMELODY_ID:
			if (pVapi->pM_imelody->bState != W_LOADED)
				return RESULT_PLAYSTATE;
			pVapi->pM_imelody->pVParser->Unload(VDEV_FIFO_EVENT);
			pVapi->pM_imelody->pVParser->End();
			pVapi->pM_imelody->bState = W_IDLE;
			if (pVapi->pM_imelody->bSource)
					pbFileStream_Ptr = NULL;
			ReleaseMidiSource();
			break;

		case XMF_ID:
			if (pVapi->pM_xmf->bState != W_LOADED)
				return RESULT_PLAYSTATE;
			pVapi->pM_xmf->pVParser->Unload(VDEV_FIFO_EVENT);
			pVapi->pM_xmf->pVParser->End();
			pVapi->pM_xmf->bState = W_IDLE;
			if (pVapi->pM_xmf->bSource)
					pbFileStream_Ptr = NULL;
			ReleaseMidiSource();
			break;

		default:
			return RESULT_UNKNOWNID;
	}


	return  RESULT_SUCCESS;
}



/***************************************************************
Description:
		Play Music

Parameters:
		dFuncId	:	Music file ID
		dSeekPos :	the position of music , the unit is millisecond	
		dRepeat	:	Number of playback
		pFunc	:	Call Back function that will be called at the end of playing music

Returns:
		if  success return  VRES_SUCCESS ,  else  VRES_VPLRPLAY_ERROR

****************************************************************/
KVRESULT  VPlr_Play(UINT32 dFuncId, UINT32 dSeekPos, UINT32 dRepeat, PEFUNCTION pFunc)
{
	UINT8	bIndex, bType, bNum;
	PVOID	pFileInfo;
	FILE_INFO FileInfo;
	KVRESULT	bErr = RESULT_SUCCESS;


	if (pVapi->bGlobalState != WORKING) 
		return  RESULT_DEVICESTATE;


	pFileInfo = (PVOID)&FileInfo;
	bIndex = (UINT8)(dFuncId - (UINT32)MIDI_ID);
	pFunction[bIndex] = pFunc;

	switch(dFuncId)
	{
		case MIDI_ID:
			if ((pVapi->pM_midi->bState != W_LOADED)&&(pVapi->pM_midi->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			if(pVapi->pM_midi->pVParser->Seek(dSeekPos, VDEV_FIFO_EVENT) == VRES_ERROR)
				return RESULT_PARSESEEK;
			VPlr_Lock();
			if (Vsyn_SetWaterMark(VDEV_FIFO_EVENT, BUFFER_EVENT_SIZE, NULL) < 0)
			{
				bErr = RESULT_SETWATERMARK;
				goto playerr;
			}
			if (Vsyn_EventSetup(VDEV_FIFO_EVENT, NULL) < 0)
			{
				bErr = RESULT_EVENTSETUP;
				goto playerr;
			}
			pVapi->pM_midi->dRepeatNum = dRepeat;
			if (pVapi->pM_midi->bState == W_LOADED)
				pVapi->pM_midi->dPauseTime += dSeekPos;
			pVapi->pM_midi->bState = W_PLAYING;
			bType = VDEV_FIFO_EVENT;
			pVapi->pM_midi->dStartTime = Vdev_GetTimerCount(1);
			break;

		case WAVE1_ID:
		case WAVE2_ID:
		case WAVE3_ID:
		case WAVE4_ID:
			bIndex = (UINT8)(dFuncId - WAVE1_ID);
			if ((pVapi->pM_wave[bIndex]->bState != W_LOADED)&&(pVapi->pM_wave[bIndex]->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			if(pVapi->pM_wave[bIndex]->pVParser->Seek(dSeekPos, (VDEV_FIFO_TYPE)(VDEV_FIFO_STREAM0 + bIndex)) == VRES_ERROR)
				return RESULT_PARSESEEK;
			VPlr_Lock();
			
			if (Vsyn_SetAudioMode(VDEV_AUDIO_MODE_HOST_WAVE_MIDI, VMD_MAX_WAVE_NUM) < 0)
			{
				bErr = RESULT_SETAUDIOMODE;
				goto playerr;
			}
			if (Vsyn_SetWaterMark((UINT8)(VDEV_FIFO_STREAM0 + bIndex), (UINT8)BUFFER_STREAM_SIZE, NULL) < 0)
			{
				bErr = RESULT_SETWATERMARK;
				goto playerr;
			}
			if (Vsyn_StreamSetup((UINT8)(VDEV_FIFO_STREAM0 + bIndex), 0, NULL) < 0)
			{
				bErr = RESULT_STREAMSETUP;
				goto playerr;
			}
			if (Vsyn_EventSetup((UINT8)(VDEV_FIFO_STREAM0 + bIndex), NULL) < 0)
			{
				bErr = RESULT_EVENTSETUP;
				goto playerr;
			}
			pVapi->pM_wave[bIndex]->dRepeatNum = dRepeat;
			if (pVapi->pM_wave[bIndex]->bState == W_LOADED)
				pVapi->pM_wave[bIndex]->dPauseTime += dSeekPos;
			pVapi->pM_wave[bIndex]->bState = W_PLAYING;
			bType = VDEV_FIFO_STREAM0 + bIndex;
			pVapi->pM_wave[bIndex]->dStartTime = Vdev_GetTimerCount(1);
			break;
			
		case VMD1_ID:
			if ((pVapi->pM_vmd->bState != W_LOADED)&&(pVapi->pM_vmd->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			if (pVapi->pM_vmd->pVParser->Seek(dSeekPos, VDEV_FIFO_EVENT) == VRES_ERROR)
				return RESULT_PARSESEEK;
			VPlr_Lock();
			if(pVapi->pM_vmd->bWave)
			{
				if (Vsyn_SetAudioMode(VDEV_AUDIO_MODE_HOST_WAVE_MIDI, VMD_MAX_WAVE_NUM) < 0)
				{
					bErr = RESULT_SETAUDIOMODE;
					goto playerr;
				}
			}
			if(pVapi->pM_vmd->bMp3)
			{
				if (Vsyn_SetAudioMode(VDEV_AUDIO_MODE_HOST_MP3_MIDI, VMD_MAX_WAVE_NUM) < 0)
				{
					bErr = RESULT_SETAUDIOMODE;
					goto playerr;
				}
			}
			if (Vsyn_SetWaterMark(VDEV_FIFO_EVENT, BUFFER_EVENT_SIZE, NULL) < 0)
			{
				bErr = RESULT_SETWATERMARK;
				goto playerr;
			}

			//For VMD Seek**************************************************
			pVapi->pM_vmd->pVParser->GetFileInfo(&FileInfo, VDEV_FIFO_EVENT);
			pVapi->pM_vmd->bFileNum = 0;
			if(FileInfo.dVmdFlag & 0x01)	//For MIDI
			{
				pVapi->pM_vmd->bFileNum += FileInfo.bVmdMidiNum;
			}
			if (FileInfo.dVmdFlag & 0x2)	//For Wave
			{
				for(bIndex=0; bIndex<FileInfo.bVmdWavTrkNum; bIndex++)
				{
					pVapi->pM_vmd->bTrackFile[bIndex] = pVapi->pM_vmd->bSTrackFile[bIndex] - FileInfo.bVmdCurWavDatNum[bIndex];
					pVapi->pM_vmd->bFileNum += pVapi->pM_vmd->bSTrackFile[bIndex] - FileInfo.bVmdCurWavDatNum[bIndex];
				}
			}
			if (FileInfo.dVmdFlag & 0x20)	//For MP3
			{
				pVapi->pM_vmd->bMp3Num = pVapi->pM_vmd->bSMp3Num - FileInfo.bVmdCurMp3Num;
				pVapi->pM_vmd->bFileNum += pVapi->pM_vmd->bSMp3Num - FileInfo.bVmdCurMp3Num;
			}
			if (FileInfo.dVmdFlag & 0x08)	//for udi
			{
				Vsyn_SetAudioMode(VDEV_AUDIO_MODE_HOST_WAVE_MIDI, 1);
				Vsyn_SetupDLS(FileInfo.pVmdUdiHead, FileInfo.dVmdUdiHead, FileInfo.pVmdUdiData, FileInfo.dVmdUdiData, NULL);
			}
			//For VMD Seek***************************************************

			if(pVapi->pM_vmd->bWave)
			{
				for(bIndex=0; bIndex<pVapi->pM_vmd->bTrackNum; bIndex++)
				{
					bNum = pVapi->pM_vmd->bVmdch[bIndex];
					if (Vsyn_StreamSetup((UINT8)(VDEV_FIFO_STREAM0 + bNum), 1, NULL) < 0)
					{
						bErr = RESULT_STREAMSETUP;
						goto playerr;
					}
				}
			}
			if(pVapi->pM_vmd->bMp3)
			{
				if (Vsyn_StreamSetup(VDEV_FIFO_MP3, 1, NULL) < 0)
				{
					bErr = RESULT_STREAMSETUP;
					goto playerr;
				}
			}
			if (Vsyn_EventSetup(VDEV_FIFO_EVENT, NULL) < 0)
			{
				bErr = RESULT_EVENTSETUP;
				goto playerr;
			}
			if (pVapi->pM_vmd->bState == W_LOADED)
				pVapi->pM_vmd->dPauseTime += dSeekPos;
			pVapi->pM_vmd->dRepeatNum = dRepeat;
			pVapi->pM_vmd->bState = W_PLAYING;
			bType = VDEV_FIFO_EVENT;
			pVapi->pM_vmd->dStartTime = Vdev_GetTimerCount(1);
			break;
			
		case MP3_ID:
			if ((pVapi->pM_mp3->bState != W_LOADED)&&(pVapi->pM_mp3->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			if (pVapi->pM_mp3->pVParser->Seek(dSeekPos, VDEV_FIFO_MP3) == VRES_ERROR)
				return RESULT_PARSESEEK;
			VPlr_Lock();
			if (Vsyn_SetAudioMode(VDEV_AUDIO_MODE_HOST_MP3_MIDI, 0) < 0)
			{
				bErr = RESULT_SETAUDIOMODE;
				goto playerr;
			}
			if (Vsyn_SetWaterMark(VDEV_FIFO_MP3, BUFFER_MP3_SIZE, NULL) < 0)
			{
				bErr = RESULT_SETWATERMARK;
				goto playerr;
			}
			if (Vsyn_StreamSetup(VDEV_FIFO_MP3, 0, NULL) < 0)
			{
				bErr = RESULT_STREAMSETUP;
				goto playerr;
			}
			if (Vsyn_EventSetup(VDEV_FIFO_MP3, NULL) < 0)
			{
				bErr = RESULT_EVENTSETUP;
				goto playerr;
			}
			pVapi->pM_mp3->dRepeatNum = dRepeat;
			if (pVapi->pM_mp3->bState == W_LOADED)
				pVapi->pM_mp3->dPauseTime += dSeekPos;
			pVapi->pM_mp3->bState = W_PLAYING;
			bType = VDEV_FIFO_MP3;
			pVapi->pM_mp3->dStartTime = Vdev_GetTimerCount(1);
			break;
			
		case IMELODY_ID:
			if ((pVapi->pM_imelody->bState != W_LOADED)&&(pVapi->pM_imelody->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			if(pVapi->pM_imelody->pVParser->Seek(dSeekPos, VDEV_FIFO_EVENT) == VRES_ERROR)
				return RESULT_PARSESEEK;
			VPlr_Lock();
			if (Vsyn_SetWaterMark(VDEV_FIFO_EVENT, BUFFER_EVENT_SIZE, NULL) < 0)
			{
				bErr = RESULT_SETWATERMARK;
				goto playerr;
			}
			if (Vsyn_EventSetup(VDEV_FIFO_EVENT, NULL) < 0)
			{
				bErr = RESULT_EVENTSETUP;
				goto playerr;
			}
			pVapi->pM_imelody->dRepeatNum = dRepeat;
			if (pVapi->pM_imelody->bState == W_LOADED)
				pVapi->pM_imelody->dPauseTime += dSeekPos;
			pVapi->pM_imelody->bState = W_PLAYING;
			bType = VDEV_FIFO_EVENT;
			pVapi->pM_imelody->dStartTime = Vdev_GetTimerCount(1);
			break;

		case XMF_ID:
			if ((pVapi->pM_xmf->bState != W_LOADED)&&(pVapi->pM_xmf->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			if(pVapi->pM_xmf->pVParser->Seek(dSeekPos, VDEV_FIFO_EVENT) == VRES_ERROR)
				return RESULT_PARSESEEK;
			VPlr_Lock();
			if (Vsyn_SetWaterMark(VDEV_FIFO_EVENT, BUFFER_EVENT_SIZE, NULL) < 0)
			{
				bErr = RESULT_SETWATERMARK;
				goto playerr;
			}
			if (Vsyn_EventSetup(VDEV_FIFO_EVENT, NULL) < 0)
			{
				bErr = RESULT_EVENTSETUP;
				goto playerr;
			}
			pVapi->pM_xmf->dRepeatNum = dRepeat;
			if (pVapi->pM_xmf->bState == W_LOADED)
				pVapi->pM_xmf->dPauseTime += dSeekPos;
			pVapi->pM_xmf->bState = W_PLAYING;
			bType = VDEV_FIFO_EVENT;
			pVapi->pM_xmf->dStartTime = Vdev_GetTimerCount(1);
			break;

		default:
			return RESULT_UNKNOWNID;
	}

	if (!g_bHeadphone)
		Vdev_Normal2Start();

	//Start sequencer
	if (Vsyn_StartSeq(bType, NULL) < 0)
	{
		bErr = RESULT_STARTSEQ;
		goto playerr;	
	}

	if (!bSongFlag)
	{
		bSongFlag = USED;
		Vdev_SetTimer(1, 0xffffff);
		Vdev_StartTimer(1);
	}
	else
		bSongFlag ++;

#if(FILESTREAM_MODE == FILESTREAM_ASYNC)
	VPlr_StartStream();
#endif
	
	VPlr_Unlock();
	return RESULT_SUCCESS;

playerr:
	VPlr_Unlock();
	return bErr;
}


/***************************************************************
Description:
		Stop  Playing Music   

Parameters:
		dFuncId :	Music file ID
		
Returns:
		if  success return  VRES_SUCCESS ,  else  VRES_VPLRSTOP_ERROR

****************************************************************/
KVRESULT VPlr_Stop(UINT32 dFuncId)
{
	UINT8	bIndex;

	if (pVapi->bGlobalState != WORKING) 
		return  RESULT_DEVICESTATE;

	switch(dFuncId)
	{
		case MIDI_ID:
			if ((pVapi->pM_midi->bState != W_PLAYING) && (pVapi->pM_midi->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			VPlr_Lock();
			pVapi->pM_midi->pVParser->Reset(VDEV_FIFO_EVENT);
			if (pVapi->pM_midi->bState == W_PLAYING)
			{
				pVapi->pM_midi->dPauseTime = 0;
				pVapi->pM_midi->bState = W_LOADED;
			}
			if (Vsyn_StopSeq(VDEV_FIFO_EVENT, NULL) < 0)
			{
				VPlr_Unlock();
				return RESULT_STOPSEQ;
			}
			break;
			
		case WAVE1_ID:
		case WAVE2_ID:
		case WAVE3_ID:
		case WAVE4_ID:
			bIndex = (UINT8)(dFuncId - WAVE1_ID);
			if ((pVapi->pM_wave[bIndex]->bState != W_PLAYING) && (pVapi->pM_wave[bIndex]->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			VPlr_Lock();
			pVapi->pM_wave[bIndex]->pVParser->Reset((VDEV_FIFO_TYPE)(VDEV_FIFO_STREAM0 + bIndex));
			if (pVapi->pM_wave[bIndex]->bState == W_PLAYING)
			{
				pVapi->pM_wave[bIndex]->bState = W_LOADED;
				pVapi->pM_wave[bIndex]->dPauseTime = 0;
			}
			if (Vsyn_StopSeq((UINT8)(VDEV_FIFO_STREAM0 + bIndex), NULL) < 0)
			{
				VPlr_Unlock();
				return RESULT_STOPSEQ;
			}
			break;
			
		case VMD1_ID:
			if ((pVapi->pM_vmd->bState != W_PLAYING) && (pVapi->pM_vmd->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			VPlr_Lock();
			pVapi->pM_vmd->pVParser->Reset(VDEV_FIFO_STREAM0);
			if (pVapi->pM_vmd->bState == W_PLAYING)
			{
				pVapi->pM_vmd->bState = W_LOADED;
				pVapi->pM_vmd->dPauseTime = 0;
			}
			if (Vsyn_StopSeq(VDEV_FIFO_EVENT, NULL) < 0)
			{
				VPlr_Unlock();
				return RESULT_STOPSEQ;
			}
			if(pVapi->pM_vmd->bMp3)
			{
				if (Vsyn_StopSeq(VDEV_FIFO_MP3, NULL) < 0)
				{
					VPlr_Unlock();
					return RESULT_STOPSEQ;
				}
			}
			if(pVapi->pM_vmd->bWave)
			{
				for(bIndex=0; bIndex<(pVapi->pM_vmd->bTrackNum); bIndex++)
				{
					if (Vsyn_StopSeq((UINT8)(VDEV_FIFO_STREAM0+(pVapi->pM_vmd->bVmdch[bIndex])), NULL) < 0)
					{
						VPlr_Unlock();
						return RESULT_STOPSEQ;
					}
				}
			}
			break;
			
		case MP3_ID:
			if ((pVapi->pM_mp3->bState != W_PLAYING) && (pVapi->pM_mp3->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			VPlr_Lock();
			pVapi->pM_mp3->pVParser->Reset(VDEV_FIFO_MP3);
			if (pVapi->pM_mp3->bState == W_PLAYING)
			{
				pVapi->pM_mp3->bState = W_LOADED;
				pVapi->pM_mp3->dPauseTime = 0;
			}
			if (Vsyn_StopSeq(VDEV_FIFO_MP3, NULL) < 0)
			{
				VPlr_Unlock();
				return RESULT_STOPSEQ;
			}
			break;

		case IMELODY_ID:
			if ((pVapi->pM_imelody->bState != W_PLAYING) && (pVapi->pM_imelody->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			VPlr_Lock();
			pVapi->pM_imelody->pVParser->Reset(VDEV_FIFO_EVENT);
			if (pVapi->pM_imelody->bState == W_PLAYING)
			{
				pVapi->pM_imelody->dPauseTime = 0;
				pVapi->pM_imelody->bState = W_LOADED;
			}
			if (Vsyn_StopSeq(VDEV_FIFO_EVENT, NULL) < 0)
			{
				VPlr_Unlock();
				return RESULT_STOPSEQ;
			}
			break;
			
		case XMF_ID:
			if ((pVapi->pM_xmf->bState != W_PLAYING) && (pVapi->pM_xmf->bState != W_PAUSE)) 
				return RESULT_PLAYSTATE;
			VPlr_Lock();
			pVapi->pM_xmf->pVParser->Reset(VDEV_FIFO_EVENT);
			if (pVapi->pM_xmf->bState == W_PLAYING)
			{
				pVapi->pM_xmf->dPauseTime = 0;
				pVapi->pM_xmf->bState = W_LOADED;
			}
			if (Vsyn_StopSeq(VDEV_FIFO_EVENT, NULL) < 0)
			{
				VPlr_Unlock();
				return RESULT_STOPSEQ;
			}
			break;
			
		default:
			return RESULT_UNKNOWNID;
	}

	if (bSongFlag == 1)
	{
		bSongFlag = NOTUSED;

		if (!g_bHeadphone)
			Vdev_Normal2End();
		Vdev_StopTimer(1);
		if (Vsyn_StopSeq(VDEV_FIFO_ALL, NULL) < 0)
		{
			VPlr_Unlock();
			return RESULT_STOPSEQ;
		}
	}
	else
		bSongFlag --;

#if(FILESTREAM_MODE == FILESTREAM_ASYNC)
	VPlr_StopStream();
	VPlr_InitGetStream();
#endif

	if(pVapi->bGPIOState == LEDMTR_GPIO)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x05);
		Vreg_WriteReg(VDEV_REG_LED_CTRL1, 0x00);	//Close LED&Motor ZY:2004.12.17
		Vreg_WriteReg(VDEV_REG_LED_CTRL2, 0x00);	
		Vreg_WriteReg(VDEV_REG_LED_CTRL3, 0x00);
		if (g_bMotorState)
			Vreg_WriteReg(VDEV_REG_MOTOR_CTRL, 0x00);

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	}
	
	VPlr_Unlock();

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Pause  the playing music   

Parameters:
		dFuncId :	Music file ID
		
Returns:
		if  success return  VRES_SUCCESS ,  else  return VRES_VPLRPAUSE_ERROR

****************************************************************/
KVRESULT VPlr_Pause(UINT32 dFuncId)
{
	UINT8	bIndex;

	switch(dFuncId)
	{
		case MIDI_ID:
			if (pVapi->pM_midi->bState != W_PLAYING)
				goto pauerr;
			pVapi->pM_midi->dStopTime = Vdev_GetTimerCount(1);
			pVapi->pM_midi->dPauseTime += pVapi->pM_midi->dStopTime - pVapi->pM_midi->dStartTime;
			pVapi->pM_midi->bState = W_PAUSE;
			if (VPlr_Stop(MIDI_ID) < 0)
				return RESULT_MUSICSTOP;
			break;
		case WAVE1_ID:
		case WAVE2_ID:
		case WAVE3_ID:
		case WAVE4_ID:
			bIndex = (UINT8)(dFuncId - (UINT32)WAVE1_ID);
			if (pVapi->pM_wave[bIndex]->bState != W_PLAYING)
				goto pauerr;
			pVapi->pM_wave[bIndex]->dStopTime = Vdev_GetTimerCount(1);
			pVapi->pM_wave[bIndex]->dPauseTime += pVapi->pM_wave[bIndex]->dStopTime - pVapi->pM_wave[bIndex]->dStartTime;
			pVapi->pM_wave[bIndex]->bState = W_PAUSE;
			if (VPlr_Stop(WAVE1_ID + bIndex) < 0)
				return RESULT_MUSICSTOP;
			break;
		case VMD1_ID:
			if (pVapi->pM_vmd->bState != W_PLAYING)
				goto pauerr;
			pVapi->pM_vmd->dStopTime = Vdev_GetTimerCount(1);
			pVapi->pM_vmd->dPauseTime += pVapi->pM_vmd->dStopTime - pVapi->pM_vmd->dStartTime;
			pVapi->pM_vmd->bState = W_PAUSE;
			if (VPlr_Stop(VMD1_ID) < 0)
				return RESULT_MUSICSTOP;
			break;
		case MP3_ID:
			if (pVapi->pM_mp3->bState != W_PLAYING)
				goto pauerr;
			pVapi->pM_mp3->dStopTime = Vdev_GetTimerCount(1);
			pVapi->pM_mp3->dPauseTime += pVapi->pM_mp3->dStopTime - pVapi->pM_mp3->dStartTime;
			pVapi->pM_mp3->bState = W_PAUSE;
			if (VPlr_Stop(MP3_ID) < 0)
				return RESULT_MUSICSTOP;
			break;

		case IMELODY_ID:
			if (pVapi->pM_imelody->bState != W_PLAYING)
				goto pauerr;
			pVapi->pM_imelody->dStopTime = Vdev_GetTimerCount(1);
			pVapi->pM_imelody->dPauseTime += pVapi->pM_imelody->dStopTime - pVapi->pM_imelody->dStartTime;
			pVapi->pM_imelody->bState = W_PAUSE;
			if (VPlr_Stop(IMELODY_ID) < 0)
				return RESULT_MUSICSTOP;
			break;

		case XMF_ID:
			if (pVapi->pM_xmf->bState != W_PLAYING)
				goto pauerr;
			pVapi->pM_xmf->dStopTime = Vdev_GetTimerCount(1);
			pVapi->pM_xmf->dPauseTime += pVapi->pM_xmf->dStopTime - pVapi->pM_xmf->dStartTime;
			pVapi->pM_xmf->bState = W_PAUSE;
			if (VPlr_Stop(XMF_ID) < 0)
				return RESULT_MUSICSTOP;
			break;

		default:
			break;	
	}


	return RESULT_SUCCESS;
pauerr:
	return RESULT_PLAYSTATE;
}


/***************************************************************
Description:
		resume the paused music:   

Parameters:
		dFuncId :	Music file ID
		
Returns:
		if  success return  VRES_SUCCESS ,  else  VRES_VPLRSTOP_ERROR

****************************************************************/
KVRESULT VPlr_Resume(UINT32 dFuncId)
{
	UINT8	bIndex;

	switch(dFuncId)
	{
		case MIDI_ID:
			if (pVapi->pM_midi->bState != W_PAUSE)
				goto reserr;
			if (VPlr_Play(MIDI_ID, pVapi->pM_midi->dPauseTime, pVapi->pM_midi->dRepeatNum, pFunction[0]) < 0)
				return RESULT_MUSICPLAY;
			pVapi->pM_midi->bState = W_PLAYING;
			break;
		case WAVE1_ID:
		case WAVE2_ID:
		case WAVE3_ID:
		case WAVE4_ID:
			bIndex = (UINT8)(dFuncId - (UINT32)WAVE1_ID);
			if (pVapi->pM_wave[bIndex]->bState != W_PAUSE)
				goto reserr;
			if (VPlr_Play((WAVE1_ID + bIndex), pVapi->pM_wave[bIndex]->dPauseTime, pVapi->pM_wave[bIndex]->dRepeatNum, pFunction[1+bIndex]) < 0)
				return RESULT_MUSICPLAY;
			pVapi->pM_wave[bIndex]->bState = W_PLAYING;
			break;
		case VMD1_ID:
			if (pVapi->pM_vmd->bState != W_PAUSE)
				goto reserr;
			if (VPlr_Play(VMD1_ID, pVapi->pM_vmd->dPauseTime, pVapi->pM_vmd->dRepeatNum, pFunction[5]) < 0)
				return RESULT_MUSICPLAY;
			pVapi->pM_vmd->bState = W_PLAYING;
			break;
		case MP3_ID:
			if (pVapi->pM_mp3->bState != W_PAUSE)
				goto reserr;
			if (VPlr_Play(MP3_ID, pVapi->pM_mp3->dPauseTime, pVapi->pM_mp3->dRepeatNum, pFunction[6]) < 0)
				return RESULT_MUSICPLAY;
			pVapi->pM_mp3->bState = W_PLAYING;
			break;

		case IMELODY_ID:
			if (pVapi->pM_imelody->bState != W_PAUSE)
				goto reserr;
			if (VPlr_Play(IMELODY_ID, pVapi->pM_imelody->dPauseTime, pVapi->pM_imelody->dRepeatNum, pFunction[7]) < 0)
				return RESULT_MUSICPLAY;
			pVapi->pM_imelody->bState = W_PLAYING;
			break;

		case XMF_ID:
			if (pVapi->pM_xmf->bState != W_PAUSE)
				goto reserr;
			if (VPlr_Play(XMF_ID, pVapi->pM_xmf->dPauseTime, pVapi->pM_xmf->dRepeatNum, pFunction[8]) < 0)
				return RESULT_MUSICPLAY;
			pVapi->pM_xmf->bState = W_PLAYING;
			break;

		default:
			break;		
	}


	return RESULT_SUCCESS;
reserr:
	return RESULT_PLAYSTATE;
}

/***************************************************************
Description:
		Play Key Effect.
		this routine cann't run when MIDI is playing


  Parameters:
		bKeyId	  :	note value
		bTimebreId :	timbre index
		wDuration  :	the duration time of play key effect

		
		  Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/

KVRESULT VPlr_PlayKeyEffect(UINT8 bKeyId,UINT8 bTimbreId,UINT16 wDuration)
{
	KEY_BUF	sKeyBuf;
	
	if(pVapi->bGlobalState != WORKING)
		return  RESULT_DEVICESTATE;

	VPlr_Lock();

	sKeyBuf.bKeyID = bKeyId;
	sKeyBuf.bTimbreID = bTimbreId;
	sKeyBuf.wDuration = wDuration;
	
	Vsyn_EventSetup(VDEV_FIFO_COMMAND, (PVOID)&sKeyBuf);

	if (bSongFlag == NOTUSED)
	{
		if (!g_bHeadphone)
			Vdev_Normal2Start();
		Vsyn_StartSeq(VDEV_FIFO_COMMAND, NULL);
	}

	VPlr_Unlock();

	return RESULT_SUCCESS;
}

/***************************************************************
Description:
		Note On.

Parameters:
		bKeyId	  :	note value
		bTimebreId :	timbre index
		bVolume    :	effect volume

Returns:
		Return VRES_SUCCESS if success, else return error value.

****************************************************************/

KVRESULT VPlr_NoteOn(UINT8 bKeyId,UINT8 bTimbreId,UINT8 bVolume)
{
	return RESULT_SUCCESS;
}

/***************************************************************
Description:
		Note Off
		
Parameters:
		None

Returns:
		Return VRES_SUCCESS if success, else return error value.

****************************************************************/
KVRESULT VPlr_NoteOff()
{
	return RESULT_SUCCESS;
}

/**************************************************************
Description:
		Callback function for Playing Game effect.
		
Parameters:
		None
				
Returns:
		Return VRES_SUCCESS if success, else return error value.
****************************************************************/
void GameEffectCallback(void)
{
	KVRESULT result;
	
	result=VPlr_Stop(dGameEffect_ID);
	if(result!=RESULT_SUCCESS)
		return;
	VPlr_Unload(dGameEffect_ID);
}	
	
/**************************************************************
Description:
		Play Game effect.
			
Parameters:
		pdBuffer	    :	Data buffer pointer
		dBuffer_Size :	Buffer size
				
Returns:
		Return VRES_SUCCESS if success, else return error value.
****************************************************************/
KVRESULT VPlr_PlayGameEffect(HUGE UINT8* pbBuffer,UINT32 dBuffer_Size)
{
	KVRESULT result;	
	
	result = VPlr_Load(pbBuffer, dBuffer_Size, 0,&dGameEffect_ID);
	if(result!=RESULT_SUCCESS)
		return result;
	result = VPlr_Play(dGameEffect_ID, 0, 0, GameEffectCallback);

	return result;
}


/***************************************************************
Description: 
		Get the current Player state of api.

Parameters:
		void
Returns:
		the  value of player state.

***************************************************************/
UINT8    VPlr_GetDeviceState(void)
{
	return   pVapi->bGlobalState;
}

/***************************************************************
Description:
		Get the current music total time.  reverb channel playback..
		
Parameters:
		dFuncId :	Music file ID
		
Returns:
		The value is the music total time, its uint is  millisecond.		

****************************************************************/
UINT32	VPlr_GetTotalTime(UINT32 dFuncId)
{
	UINT32	dTime = 0;

	switch(dFuncId)
	{
		case MIDI_ID:
			dTime = pVapi->pM_midi->dTotalTime;
			break;
		case WAVE1_ID:
		case WAVE2_ID:
		case WAVE3_ID:
		case WAVE4_ID:
			dTime = pVapi->pM_wave[dFuncId - WAVE1_ID]->dTotalTime;
			break;
		case VMD1_ID:
			dTime = pVapi->pM_vmd->dTotalTime;
			break;
		case MP3_ID:
			dTime = pVapi->pM_mp3->dTotalTime;
			break;

		case IMELODY_ID:
			dTime = pVapi->pM_imelody->dTotalTime;
			
		case XMF_ID:
			dTime = pVapi->pM_xmf->dTotalTime;
			
		default:
			break;
	}

	return dTime;
}



/***************************************************************
Description:
		Get the current time of music playing.
		
Parameters:
		dFuncId :	Music file ID

Returns:
		The value is the music current time, its uint is  millisecond.		

****************************************************************/
UINT32	VPlr_GetCurTime(UINT32 dFuncId)
{
	UINT8	bIndex;
	UINT32	dTime = 0;

	for(bIndex=0; bIndex<7; bIndex++)
	{
		if(bChannelTable[bIndex] == dFuncId)
			break;
	}
	if (bIndex > 6)
		goto cur;
	dTime = Vdev_GetTimerCount(1);
	switch(dFuncId)
	{
		case MIDI_ID:
			if (pVapi->pM_midi->bState == W_PLAYING)
			{
				dTime -= pVapi->pM_midi->dStartTime;
				dTime += pVapi->pM_midi->dPauseTime;
			}
			if (pVapi->pM_midi->bState == W_PAUSE)
				dTime = pVapi->pM_midi->dPauseTime;
			break;
		case WAVE1_ID:
		case WAVE2_ID:
		case WAVE3_ID:
		case WAVE4_ID:
			if (pVapi->pM_wave[dFuncId - WAVE1_ID]->bState == W_PLAYING)
			{
				dTime -= pVapi->pM_wave[dFuncId - WAVE1_ID]->dStartTime;
				dTime += pVapi->pM_wave[dFuncId - WAVE1_ID]->dPauseTime;
			}
			if (pVapi->pM_wave[dFuncId - WAVE1_ID]->bState == W_PAUSE)
				dTime = pVapi->pM_wave[dFuncId - WAVE1_ID]->dPauseTime;
			break;
		case VMD1_ID:
			if (pVapi->pM_vmd->bState == W_PLAYING)
			{
				dTime -= pVapi->pM_vmd->dStartTime;
				dTime += pVapi->pM_vmd->dPauseTime;
			}
			if (pVapi->pM_vmd->bState == W_PAUSE)
				dTime = pVapi->pM_vmd->dPauseTime;
			break;
		case MP3_ID:
			if (pVapi->pM_mp3->bState == W_PLAYING)
			{
				dTime -= pVapi->pM_mp3->dStartTime;
				dTime += pVapi->pM_mp3->dPauseTime;
			}
			if (pVapi->pM_mp3->bState == W_PAUSE)
				dTime = pVapi->pM_mp3->dPauseTime;
			break;
		case IMELODY_ID:
			if (pVapi->pM_imelody->bState == W_PLAYING)
			{
				dTime -= pVapi->pM_imelody->dStartTime;
				dTime += pVapi->pM_imelody->dPauseTime;
			}
			if (pVapi->pM_imelody->bState == W_PAUSE)
				dTime = pVapi->pM_imelody->dPauseTime;
			break;
		case XMF_ID:
			if (pVapi->pM_xmf->bState == W_PLAYING)
			{
				dTime -= pVapi->pM_xmf->dStartTime;
				dTime += pVapi->pM_xmf->dPauseTime;
			}
			if (pVapi->pM_xmf->bState == W_PAUSE)
				dTime = pVapi->pM_xmf->dPauseTime;
			break;
		default:
			dTime = 0;
			break;
	}
cur:
	return dTime;
}


/***************************************************************
Description:
		Get Mp3's name
		
Parameters:
		pbAddr :	start address of name

Returns:
		Return VRES_SUCCESS if success, else return error value.		

****************************************************************/
KVRESULT VPlr_GetMp3Title(UINT8* pbAddr)
{
	if (pVapi->bMp3State == NOTUSED)
		return RESULT_NOMP3;

	Vmd_MemCpyEx((HUGE UINT8*)pbAddr, (HUGE UINT8*)(pVapi->pM_mp3->pbTitle));

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Get Mp3's Artist
		
Parameters:
		pbAddr :	start address of artist

Returns:
		Return VRES_SUCCESS if success, else return error value.		

****************************************************************/
KVRESULT VPlr_GetMp3Artist(UINT8* pbAddr)
{
	if (pVapi->bMp3State == NOTUSED)
		return RESULT_NOMP3;

	Vmd_MemCpyEx((HUGE UINT8*)pbAddr, (HUGE UINT8*)(pVapi->pM_mp3->pbArtist));

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Get Mp3's Album
		
Parameters:
		pbAddr :	start address of Album

Returns:
		Return VRES_SUCCESS if success, else return error value.		

****************************************************************/
KVRESULT VPlr_GetMp3Album(UINT8* pbAddr)
{
	if (pVapi->bMp3State == NOTUSED)
		return RESULT_NOMP3;

	Vmd_MemCpyEx((HUGE UINT8*)pbAddr, (HUGE UINT8*)(pVapi->pM_mp3->pbAlbum));

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Get Mp3's Genre
		
Parameters:
		pbAddr :	start address of Genre

Returns:
		Return VRES_SUCCESS if success, else return error value.		

****************************************************************/
KVRESULT VPlr_GetMp3Genre(UINT8* pbAddr)
{
	if (pVapi->bMp3State == NOTUSED)
		return RESULT_NOMP3;

	Vmd_MemCpyEx((HUGE UINT8*)pbAddr, (HUGE UINT8*)(pVapi->pM_mp3->pbGenre));

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Get Mp3's Comment
		
Parameters:
		pbAddr : start address of Comment

Returns:
		Return VRES_SUCCESS if success, else return error value.		

****************************************************************/
KVRESULT VPlr_GetMp3Comment(UINT8* pbAddr)
{
	if (pVapi->bMp3State == NOTUSED)
		return RESULT_NOMP3;


	Vmd_MemCpyEx((HUGE UINT8*)pbAddr, (HUGE UINT8*)(pVapi->pM_mp3->pbComment));

	return RESULT_SUCCESS;
}



/***************************************************************
Description:
		Get Mp3's Time
		
Parameters:
		pbAddr : start address of Time

Returns:
		Return VRES_SUCCESS if success, else return error value.		

****************************************************************/
KVRESULT VPlr_GetMp3Time(UINT32* pdAddr)
{
	if (pVapi->bMp3State == NOTUSED)
		return RESULT_NOMP3;

	*pdAddr = pVapi->pM_mp3->dTime;

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Set MP3 Playing speed
		
Parameters:
		bMode :	The mode that playing MP3
			0: slow mode
			1: quick
		bSpeed:	Playing Speed
			0: normal times
			1: two times
			2: three times

Returns:
		Return VRES_SUCCESS if success, else return error value.		

****************************************************************/
KVRESULT VPlr_SetMp3Speed(UINT8 bMode, UINT8 bSpeed)
{
	UINT8	bState;
	KVRESULT result = RESULT_SUCCESS;

	if ((bSpeed != 1) && (bSpeed != 2) && (bSpeed != 0))
		return RESULT_OUTRANGE;
	if ((bMode != 0) && (bMode != 1))
		return RESULT_OUTRANGE;

	VPlr_Lock();

	if (bSpeed != 0)
	{
		if (bMode == 0)
			bSpeed -= 1;
		if (bMode == 1)
			bSpeed += 1;
	}
	else
		bSpeed = 4;

	bState = pVapi->pM_mp3->bState;
	switch(bState)
	{
		case W_PLAYING:
			Vsyn_SetMp3Speed(bSpeed);
			break;
		default:
			result = RESULT_PLAYSTATE;
			break;
	}
	
	VPlr_Unlock();

	return result;
}



/***************************************************************
Description:
		MP3 A->B function
		
Parameters:
		dStart :	start point in millisecond
		dEnd   : end point in millisecond

Returns:
		Return VRES_SUCCESS if success, else return error value.		

****************************************************************/
KVRESULT VPlr_SetMp3AtoB(UINT32 dStart, UINT32 dEnd)
{
	UINT8	bState;
	KVRESULT result = RESULT_SUCCESS;

	if (dStart >= dEnd)
		return RESULT_MP3ATOB;

	bState = pVapi->pM_mp3->bState;
	switch(bState)
	{
		case W_LOADED:
		case W_PLAYING:
		case W_PAUSE:
			pVapi->pM_mp3->pVParser->SetAtoB(dStart, dEnd);
			break;
		default:
			result = RESULT_PLAYSTATE;
			break;
	}

	return result;
}


/***************************************************************
Description:
		Enable motor shaking.
		
Parameters:
		bFlag	 :	Enable or Disable Motor function
				0: Disable
				1: Enable
		bMode	 :	Motor Shaking Mode
				0: 928 mode
				1: 938 mode
		bChn	 :	MIDI channel. Synchronize the Motor with this channel.
		uInterval :	The number of note between motor on and motor off.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.		

****************************************************************/
KVRESULT VPlr_DriveMotor(UINT8 bFlag, UINT8 bMode, UINT8 bChn, UINT8 bInterval)
{	
	if(pVapi->bGPIOState != LEDMTR_GPIO)
		return RESULT_GPIOERROR;

	g_bMotorState = 1;

	VPlr_Lock();

	if(bMode)	//938 mode
	{
		if(bFlag)	
			Vdev_DeviceControl(VDEV_CTRL_SET_MOTOR_938, 0x0800, 1);	//Enable LED //ZY:10.17
		else
			Vdev_DeviceControl(VDEV_CTRL_SET_MOTOR_938, 0x0800, 0);	//Disable LED
	}
	else		//928 mode
	{
		Vdev_DeviceControl(VDEV_CTRL_SET_MOTOR_928, 0, 1);
		if(bFlag)
			Vsyn_MidiMotorEnable(bChn, bInterval);
		else
			Vsyn_MidiMotorDisable(bChn, bInterval);
	}

	VPlr_Unlock();

	return RESULT_SUCCESS;

}

/***************************************************************
Description:
		Drive Motor shaking without playing song

Parameters:
		bEn:	0: Disable  1: Enable
		dTimer:	Time interval
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT VPlr_MotorShake(UINT8 bEn, UINT32 dTimer)
{

	g_bMotorState = 0;

	VPlr_Lock();

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x05);	
	if (bEn)
	{
		Vreg_WriteReg(VDEV_REG_LEDMOT_CONF, 0x80);	
		Vreg_WriteReg(VDEV_REG_MOTOR_CTRL, 0x80);
	}
	else
	{
		Vreg_WriteReg(VDEV_REG_LEDMOT_CONF, 0x00);
		Vreg_WriteReg(VDEV_REG_MOTOR_CTRL, 0x00);
	}

	VPlr_Unlock();

	return RESULT_SUCCESS;
}



/***************************************************************
Description:
		Enable LED flickering.

Parameters:
		bFlag :		Enable or Disable LED flush function
				0: Disable
				1: Enable.
		bMode :		Led flush mode
				0: 928 mode 
				1: 938 mode
		bChn  :		MIDI channel. Synchronize the LED flickering with this channel.
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.		
		  
****************************************************************/
KVRESULT VPlr_ShowLed(UINT8 bFlag, UINT8 bMode, UINT8 bChn)
{
	if(pVapi->bGPIOState != LEDMTR_GPIO)
		return RESULT_GPIOERROR;

	
	VPlr_Lock();

	if(bMode)	//938 mode
	{
		if(bFlag)	
			Vdev_DeviceControl(VDEV_CTRL_SET_LED_938, 0x0800, 1);	//Enable LED //ZY:10.17
		else
			Vdev_DeviceControl(VDEV_CTRL_SET_LED_938, 0x0800, 0);	//Disable LED
	}
	else		//928 mode
	{
		Vdev_DeviceControl(VDEV_CTRL_SET_LED_928, 0, 1);
		if(bFlag)
			Vsyn_MidiLedEnable(bChn);
		else
			Vsyn_MidiLedDisable(bChn);
	}

	VPlr_Unlock();

	return RESULT_SUCCESS;
}



/***************************************************************
Description:
		Change PA gain.

Parameters:
		bVol :	The PA gain. Range: 0 to 0x0f.
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT VPlr_SetSPVol(UINT8 bVol)
{
	if (bVol > 0xf)
		return RESULT_RANGEVOLUME;

	VPlr_Lock();
	Vdev_SetVol(VDEV_CTRL_SET_SPVOL, bVol, bVol);
	VPlr_Unlock();
	pVapi->pM_SoundInfo->bPAVol = bVol;

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Change Mp3 volume step.

Parameters:
		bVol :	The Mp3 volume step. Range: 0 to 0x1f.
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT VPlr_SetMp3VolStep(UINT8 bVol)
{
	if (bVol > 0x1f)
		return RESULT_RANGEVOLUME;

	VPlr_Lock();
	Vdev_SetVol(VDEV_CTRL_SET_MP3_VOLSTEP, bVol, bVol);
	VPlr_Unlock();
	pVapi->pM_SoundInfo->bMp3VolStep = bVol;

	return RESULT_SUCCESS;
}



/***************************************************************
Description:
		Change global volume step.

Parameters:
		bVol :	The global volume step. Range: 0 to 0x1f.
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT VPlr_SetGlbVolStep(UINT8 bVol)
{
	if (bVol > 0x1f)
		return RESULT_RANGEVOLUME;

	VPlr_Lock();
	Vdev_SetVol(VDEV_CTRL_SET_GLOBAL_VOLSTEP, bVol, bVol);
	VPlr_Unlock();
	pVapi->pM_SoundInfo->bGlbVolStep = bVol;

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Change DAC Volume

Parameters:
		bVol :	The DAC Volume. Range: 0 to 0x0f.
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT VPlr_SetDACVol(UINT8 bVol)
{
	UINT8	bDacVol;

	if (bVol > 0x0F)
		return RESULT_RANGEVOLUME;

	bDacVol = 0x0F - bVol;
	VPlr_Lock();
	Vdev_SetVol(VDEV_CTRL_SET_DACMIXVOL, bDacVol, bDacVol);
	VPlr_Unlock();
	pVapi->pM_SoundInfo->bDACVol = bVol;

	return RESULT_SUCCESS;
}



/***************************************************************
Description:
		Change MIDI Volume.

Parameters:
		bVolL:		The left channel volume. Range: 0 to 0xff. 
		bVolR:		The right channel volume. Range: 0 to 0xff. 
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
KVRESULT VPlr_SetMidiVol(UINT8 bVolL, UINT8 bVolR)
{
	UINT8	bVol_l, bVol_r;

	bVol_l = 0xff - bVolL;
	bVol_r = 0xff - bVolR;

	VPlr_Lock();
	Vdev_SetVol(VDEV_CTRL_SET_MIDIVOL, bVol_l, bVol_r);
	VPlr_Unlock();
	pVapi->pM_SoundInfo->bMidiVol_L = bVolL;
	pVapi->pM_SoundInfo->bMidiVol_R = bVolR;

	return RESULT_SUCCESS;
}



/***************************************************************
Description:
		Change WAVE Volume.

Parameters:
		bVolL:		The left channel volume. Range: 0 to 0xff. 
		bVolR:		The right channel volume. Range: 0 to 0xff. 
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
KVRESULT VPlr_SetWaveVol(UINT8 bVolL, UINT8 bVolR )
{
	UINT8	bVol_l, bVol_r;

	bVol_l = 0xff - bVolL;
	bVol_r = 0xff - bVolR;

	VPlr_Lock();
	Vdev_SetVol(VDEV_CTRL_SET_WAVEVOL, bVol_l, bVol_r);
	VPlr_Unlock();
	pVapi->pM_SoundInfo->bWaveVol_L = bVolL;
	pVapi->pM_SoundInfo->bWaveVol_R = bVolR;

	return RESULT_SUCCESS;
}




/****************************************************************
Description:
		Change Earphone Volume.

Parameters:
		bVol:	The volume. Range: 0 to 0x0f. 
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT  VPlr_SetHPVol(UINT8  bVol)
{
	UINT8 bVolume;

	if (bVol > 0x0F)
		return RESULT_RANGEVOLUME;

	bVolume = 0x0F - bVol;
	
	VPlr_Lock();
	Vdev_SetVol(VDEV_CTRL_SET_HPVOL, bVolume, bVolume);
	VPlr_Unlock();
	pVapi->pM_SoundInfo->bHPNVol_L = bVol;
	pVapi->pM_SoundInfo->bHPNVol_R = bVol;

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Get Volume Information.

Parameters:
		pInfo : start address of Sound struct	
	
Return:
		Return VRES_SUCCESS if success, else return error value.
		
****************************************************************/
KVRESULT VPlr_GetVol(KVSOUNDINFO* pInfo)
{
	pInfo->bWaveVol_L = pVapi->pM_SoundInfo->bWaveVol_L;
	pInfo->bWaveVol_R = pVapi->pM_SoundInfo->bWaveVol_R;
	pInfo->bMidiVol_L = pVapi->pM_SoundInfo->bMidiVol_L;
	pInfo->bMidiVol_R = pVapi->pM_SoundInfo->bMidiVol_R;
	pInfo->bHPNVol_L = pVapi->pM_SoundInfo->bHPNVol_L;
	pInfo->bHPNVol_R = pVapi->pM_SoundInfo->bHPNVol_R;
	pInfo->bPAVol = pVapi->pM_SoundInfo->bPAVol;
	pInfo->bDACVol = pVapi->pM_SoundInfo->bDACVol;

	return RESULT_SUCCESS;
}


/****************************************************************
Description:
		Enable Total EQ.

Parameters:

Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT	VPlr_EnableTotalEQ(void)
{
	VPlr_Lock();

	Vreg_WriteReg(0x00, 0x02);
	Vreg_WriteReg(VDEV_REG_OUT_SEL, 0x00);
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

	VPlr_Unlock();

	return RESULT_SUCCESS;
}

/****************************************************************
Description:
		Disable Total EQ.

Parameters:

Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT	VPlr_DisableTotalEQ(void)
{
	VPlr_Lock();

	Vreg_WriteReg(0x00, 0x02);
	Vreg_WriteReg(VDEV_REG_OUT_SEL, 0x01);
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

	VPlr_Unlock();

	return RESULT_SUCCESS;
}

/****************************************************************
Description:
		Set Total EQ.

Parameters:
		pbBank :	Address of bank[6], each value is range of 0-31.

Returns:
		Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT	VPlr_SetTotalEQ(UINT8* pbBank)
{
	UINT8*	pbPtr, bIndex;

	pbPtr = pbBank;
	VPlr_Lock();

	Vreg_WriteReg(0x00, 0x02);
	for(bIndex=0; bIndex<6; bIndex++)
		Vreg_WriteReg((UINT8)(VDEV_REG_EQ_BAND0+bIndex), pbPtr[bIndex]);
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

	VPlr_Unlock();

	return RESULT_SUCCESS;
}

/****************************************************************
Description:
		Get Total EQ.

Parameters:
		pbBank : Address of bank[6], each value is range of 0-31.
		  
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT	VPlr_GetTotalEQ(UINT8* pbBank)
{
	UINT8*	pbPtr, bIndex;

	pbPtr = pbBank;
	VPlr_Lock();

	Vreg_WriteReg(0x00, 0x02);
	for(bIndex=0; bIndex<6; bIndex++)
		pbPtr[bIndex] = Vreg_ReadReg((UINT8)(VDEV_REG_EQ_BAND0+bIndex));
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

	VPlr_Unlock();

	return RESULT_SUCCESS;
}

/****************************************************************
Description:
		Set MP3 EQ mode.

Parameters:
		bMode: 	MP3_EQ_ORIGINAL
			MP3_EQ_CLASSICAL
			MP3_EQ_CLUB
			MP3_EQ_POP
			MP3_EQ_BASE
			MP3_EQ_TREBLE
			MP3_EQ_BASE_TREBLE
			MP3_EQ_ROCK
			MP3_EQ_DANCE
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT	 VPlr_SetMP3EQ(UINT8 bMode)
{
	VPlr_Lock();
	Vdev_DeviceControl(VDEV_CTRL_SET_MP3EQ, bMode, 0);
	VPlr_Unlock();

	return RESULT_SUCCESS;
}


/****************************************************************
Description:
		Set MP3 EQ As User Define.

Parameters:
		pbBank :	Address of bank[10], each value is range of 0-31.
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT	 VPlr_SetMP3EQEx(UINT8* pbBank)
{
	UINT8*	pbPtr, bIndex;

	pbPtr = pbBank;
	VPlr_Lock();

	Vreg_WriteReg(0x00, 0x02);
	for(bIndex=0; bIndex<10; bIndex++)
		Vreg_WriteReg((UINT8)(VDEV_REG_MP3_EQBAND0+bIndex), pbPtr[bIndex]);
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

	VPlr_Unlock();

	return RESULT_SUCCESS;
}


/****************************************************************
Description:
		Get MP3 EQ Information

Parameters:
		pbBank :	Address of bank[10], each value is range of 0-31.
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT	 VPlr_GetMP3EQ(UINT8* pbBank)
{
	UINT8*	pbPtr, bIndex;

	pbPtr = pbBank;
	VPlr_Lock();

	Vreg_WriteReg(0x00, 0x02);
	for(bIndex=0; bIndex<10; bIndex++)
		pbPtr[bIndex] = Vreg_ReadReg((UINT8)(VDEV_REG_MP3_EQBAND0+bIndex));
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

	VPlr_Unlock();

	return RESULT_SUCCESS;
}


/****************************************************************
Description:
		Get Current RDK Version

Parameters:
		pbBank : Address of bank[6], each value is range of 0-31.
		  
Returns:
		Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT	VPlr_GetCurVersion(KVVERSION* pVer)
{
	pVer->bHighAPI_Ver = VC0938RDK_VER.bHighAPI_Ver;
	pVer->bM_Ver = VC0938RDK_VER.bM_Ver;
	pVer->bS_Ver = VC0938RDK_VER.bS_Ver;
	pVer->bVmdlib_Ver = VC0938RDK_VER.bVmdlib_Ver;
	pVer->bBR_Ver = VC0938RDK_VER.bBR_Ver;

	pVer->bYear = VC0938RDK_VER.bYear;
	pVer->bMonth = VC0938RDK_VER.bMonth;
	pVer->bDay = VC0938RDK_VER.bDay;

	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		According different user to define what to do in the end of play.

Parameters:
		bDir    :	"0" input from pin (read) , "1" out to pin (write)
		bChan   :	which GPIO (0, 1, 2, 3)
		pbLevel :	"0" is low level, "1" is high level
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.		
		  
****************************************************************/
KVRESULT	VPlr_GPIO_Ctrl(UINT8 bDir, UINT8 bChan, UINT8 *pbLevel)
{
	UINT8	bRegDir, bRegP0;

	if(pVapi->bGPIOState != GENERAL_GPIO)
		return RESULT_GPIOERROR;

	if(bChan>3)
		return RESULT_GPIOERROR;
	
	VPlr_Lock();
		
	Vreg_WriteReg((UINT8)VDEV_REG_PAGE_NUM, 0x00);	//Write Page Register

	bRegDir = Vreg_ReadReg((UINT8)VDEV_REG_P0_DIR);
	bRegP0 = Vreg_ReadReg((UINT8)VDEV_REG_PORT0);
				
	if(bDir==0)		//Input
	{
		bRegDir&=~(1<<bChan);
		Vreg_WriteReg((UINT8)VDEV_REG_P0_DIR, bRegDir);
		*pbLevel=(bRegP0>>bChan)&0x01;
	}
	else if(bDir==1)		//Output
	{
		bRegDir|=(1<<bChan);
		Vreg_WriteReg((UINT8)VDEV_REG_P0_DIR, bRegDir);
		if(*pbLevel)
			bRegP0|=(1<<bChan);
		else
			bRegP0&=~(1<<bChan);
		Vreg_WriteReg((UINT8)VDEV_REG_PORT0, bRegP0);
	}
	
	VPlr_Unlock();
					
	return  RESULT_SUCCESS;	
}

/***************************************************************
Description:
		According different user to define what to do in the GPIO control and function configue.

Parameters:
		bMode : input the macro of GPIO working mode such as:
			GENERAL_GPIO
			LEDMTR_GPIO
			IIS_GPIO
			RESERVE_GPIO
			PATEST_GPIO
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT VPlr_SetGPIOConfig(UINT8 bMode)
{
	UINT8 bRegSel, bRegCfg, bRegMod;

	if(bMode>PATEST_GPIO)
		return RESULT_GPIOERROR;
				
	pVapi->bGPIOState = bMode;

	VPlr_Lock();

	Vreg_WriteReg((UINT8)VDEV_REG_PAGE_NUM, 0x00);	//Write Page Register
		
	bRegSel = Vreg_ReadReg((UINT8)VDEV_REG_FUNC_SEL);
	bRegSel&=0xF0;
	bRegCfg = Vreg_ReadReg((UINT8)VDEV_REG_GPIO_CFG);
	bRegMod = Vreg_ReadReg((UINT8)VDEV_REG_P0_MODE);

	switch(bMode)
	{
	case GENERAL_GPIO:
		 bRegCfg|=0x0f;
	     	 bRegMod|=0x0f;
		 break;
	case LEDMTR_GPIO:
		 bRegSel|=0x01;
		 bRegCfg&=0xf0;
		 break;
	case IIS_GPIO:
		 bRegSel|=0x02;
	     	 bRegCfg&=0xf0;
		 break;
	case RESERVE_GPIO:
		 bRegSel|=0x04;
		 bRegCfg&=0xf0;
		 break;
	case PATEST_GPIO:
		 bRegSel|=0x08;
		 bRegCfg&=0xf0;
		 break;
	default:
		 break;
	}
				
	Vreg_WriteReg((UINT8)VDEV_REG_FUNC_SEL, bRegSel);
	Vreg_WriteReg((UINT8)VDEV_REG_GPIO_CFG, bRegCfg);
	Vreg_WriteReg((UINT8)VDEV_REG_P0_MODE, bRegMod);
	Vreg_WriteReg((UINT8)VDEV_REG_GPIO_REN, 0xff);

	VPlr_Unlock();

	return RESULT_SUCCESS;
}



KVRESULT VPlr_PAControl(UINT8 bPdwn, UINT8 bMute)
{
	UINT8	bRegValue;
	
	VPlr_Lock();

	Vreg_WriteReg((UINT8)VDEV_REG_PAGE_NUM, 0x00);	
	bRegValue = Vreg_ReadReg((UINT8)VDEV_REG_PACFG);
	if(bPdwn == 1)
	{
		if(bMute == 1)
		{
			bRegValue |= 0xc0;	
		}
		else
		{
			bRegValue |= 0x80;	
			bRegValue &= 0xbf;
		}
	}
	else
	{
		if(bMute == 1)
		{
			bRegValue |= 0x40;
			bRegValue &= 0x7f;
		}
		else
		{
			bRegValue &= 0x3f;
		}
	}
				
	Vreg_WriteReg(VDEV_REG_PACFG, bRegValue);

	VPlr_Unlock();
		
	return RESULT_SUCCESS;
}


/***************************************************************
Description:
		Set headphone output or not flag.

Parameters:
		bFlag    :	The flag of the headphone output.
		
Returns:
		  Return the headphone flag user set.		
		  
****************************************************************/
UINT8 VPlr_SetHeadphoneFlag(UINT8 bFlag)
{
	g_bHeadphone  = bFlag;

	return bFlag;
}

#if(IIS_SUPPORT==1)

/***************************************************************
Description:
		Enable IIS Function

Parameters:
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.		
		  
****************************************************************/
KVRESULT VPlr_EnableIIS(void)
{
	if(pVapi->bGlobalState != WORKING)
		return  RESULT_DEVICESTATE;

	VPlr_Lock();

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);	//Write Page Register
	Vreg_WriteReg(VDEV_REG_SWRST2, 0x40);	//Reset IIS
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);	//Write Page Register
	Vreg_WriteReg(VDEV_REG_MISC_CON, 0x02);	//Enable IIS
	Vreg_WriteReg(VDEV_REG_PACFG, 0x09);	//REG_PACFG

	VPlr_Unlock();

	return RESULT_SUCCESS;
}

/***************************************************************
Description:
		Disable IIS Function

Parameters:
		
Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT VPlr_DisableIIS(void)
{
	UINT8	bReg;

	if(pVapi->bGlobalState != WORKING)
		return  RESULT_DEVICESTATE;

	VPlr_Lock();

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);	//Write Page Register
	Vreg_WriteReg(VDEV_REG_PACFG, 0x09);	//REG_PACFG
	bReg = Vreg_ReadReg(VDEV_REG_DACCTRL);
	bReg &= ~(1<<6);
	Vreg_WriteReg(VDEV_REG_DACCTRL, bReg);	//Select DAC Source(928 Input)
	Vreg_WriteReg(VDEV_REG_MISC_CON, 0x00);	//Disable IIS

	VPlr_Unlock();

	return RESULT_SUCCESS;
}

/***************************************************************
Description:
		Set IIS Mode

Parameters:
		bMode: IIS Mode(M_IIS, S1_IIS, S2_IIS)
		bFreq: DAC CLK (Divided from MCLK)
			FREQ1_IIS:  1/1 MCLK
			FREQ2_IIS:  1/2 MCLK
			FREQ3_IIS:  1/3 MCLK
			FREQ4_IIS:  1/4 MCLK

Returns:
		  Return VRES_SUCCESS if success, else return error value.
		  
****************************************************************/
KVRESULT VPlr_IISSetMode(UINT8 bMode, UINT8 bFreq)
{
	UINT8	bDaccon, bIiscfg;

	if(pVapi->bGlobalState != WORKING)
		return  RESULT_DEVICESTATE;

	VPlr_Lock();

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);	//Write Page Register
	bIiscfg = Vreg_ReadReg(VDEV_REG_I2S_CFG);	//Read IIS_CFG Register

	switch(bMode)
	{
		case M_IIS:
			bDaccon = Vreg_ReadReg(VDEV_REG_DACCTRL);		//Read DAC_CON Register
			bDaccon&=0xdf;					//output clock
			bIiscfg|=0x06;
			bIiscfg&=0x0f;
			Vreg_WriteReg(VDEV_REG_DACCTRL, bDaccon);		//Write DAC_CON register
			break;
		case S1_IIS:
			bDaccon = Vreg_ReadReg(VDEV_REG_DACCTRL);
			bIiscfg|=0x0e;
			bIiscfg&=0xf9;
			bDaccon |= 0x40;
			Vreg_WriteReg(VDEV_REG_DACCTRL, bDaccon);		//Select DAC Source(IIS Input)
			break;
		case S2_IIS:
			bIiscfg|=0x02;
			bIiscfg&=0x0b;
			break;
		default:
			break;
	}
	switch(bFreq)
	{
		case FREQ1_IIS:
			bIiscfg&=0x3f;
			break;
		case FREQ2_IIS:
			bIiscfg&=0x3f;
			bIiscfg|=(1<<7);
			break;
		case FREQ3_IIS:
			bIiscfg&=0x3f;
			bIiscfg|=(1<<8);
			break;
		case FREQ4_IIS:
			bIiscfg|=0xc0;
			break;
		default:
			break;
	}
	Vreg_WriteReg(VDEV_REG_I2S_CFG, bIiscfg);	//read iis_cfg register

	VPlr_Unlock();

	return RESULT_SUCCESS;
}

#endif

static VRESULT GetMessage(UINT8 bFifoType, PUINT8 pbMsgBuf, PVOID pExtPara)
{
	UINT8	bNum;
	VRESULT	vResult;
	
	switch (bFifoType)
	{
		case VDEV_FIFO_EVENT:
			if (pVapi->dFifoMap & 0x0F)
				vResult = pVapi->pM_vmd->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, VDEV_FIFO_EVENT);
			else if (pVapi->dFifoMap & 0xF000000)
				vResult = pVapi->pM_imelody->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, VDEV_FIFO_EVENT);
			else if (pVapi->dFifoMap & 0xF0000000)
				vResult = pVapi->pM_xmf->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, VDEV_FIFO_EVENT);
			else
				vResult = pVapi->pM_midi->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, VDEV_FIFO_EVENT);
			break;
			
		case VDEV_FIFO_MP3:
			if (pVapi->dFifoMap & 0xF00000)
				vResult = pVapi->pM_vmd->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, (VDEV_FIFO_TYPE)bFifoType);
			else
				vResult = pVapi->pM_mp3->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, (VDEV_FIFO_TYPE)bFifoType);
			break;
			
		case VDEV_FIFO_STREAM0:
			if (pVapi->dFifoMap & 0xF0)
				vResult = pVapi->pM_vmd->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, (VDEV_FIFO_TYPE)bFifoType);
			else
			{
				bNum = bChannelTable[STREAM0_CH] - WAVE1_ID;
				vResult = pVapi->pM_wave[bNum]->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, (VDEV_FIFO_TYPE)bFifoType);
			}
			break;
			
		case VDEV_FIFO_STREAM1:
			if (pVapi->dFifoMap & 0xF00)
				vResult = pVapi->pM_vmd->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, (VDEV_FIFO_TYPE)bFifoType);
			else
			{
				bNum = bChannelTable[STREAM1_CH] - WAVE1_ID;
				vResult = pVapi->pM_wave[bNum]->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, (VDEV_FIFO_TYPE)bFifoType);
			}
			break;

		case VDEV_FIFO_STREAM2:
			if (pVapi->dFifoMap & 0xF000)
				vResult = pVapi->pM_vmd->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, (VDEV_FIFO_TYPE)bFifoType);
			else
			{
				bNum = bChannelTable[STREAM2_CH] - WAVE1_ID;
				vResult = pVapi->pM_wave[bNum]->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, (VDEV_FIFO_TYPE)bFifoType);
			}
			break;
					
		case VDEV_FIFO_STREAM3:
			if (pVapi->dFifoMap & 0xF0000)
				vResult = pVapi->pM_vmd->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, (VDEV_FIFO_TYPE)bFifoType);
			else
			{
				bNum = bChannelTable[STREAM3_CH] - WAVE1_ID;
				vResult = pVapi->pM_wave[bNum]->pVParser->GetEvent((PVM_UNI_MSG)pbMsgBuf, (VDEV_FIFO_TYPE)bFifoType);
			}
			break;
						
		default:
			return VRES_ERROR_PARAMETERS;
	}
	return vResult;
}

#if(FILESTREAM_MODE == FILESTREAM_ASYNC)
UINT32	VPlr_GetStream(void)
{
	UINT8	i;
	UINT32	dBufferLen = 0;
	UINT8*	pbPtr;

	if(VPlr_GetStartStream())
	{
		i = VPlr_CheckRBuffer();
		if (i != 0)
		{
			pbPtr = g_bStmBuf + ((i - 1) * SECOND_BUFFER_CELL);
			dBufferLen = GetStream(VDEV_FIFO_MP3, pbPtr, SECOND_BUFFER_CELL, NULL);
			VPlr_SetWBufferSize((i - 1), dBufferLen);
		}
	}

	return dBufferLen;
}
#endif

static UINT32 GetStream(UINT8 bFifoType, PUINT8 pbStmBuf, UINT32 dStmLen, PVOID pExtPara)
{
	UINT8	bNum;
	UINT32	dDataSize;
	VRESULT 	vResult;
	
	switch (bFifoType)
	{
		case VDEV_FIFO_STREAM0:
			if (pVapi->dFifoMap & 0xF0)
				vResult = pVapi->pM_vmd->pVParser->ReadBlock(pbStmBuf, dStmLen, &dDataSize, VDEV_FIFO_STREAM0);
			else
			{
				bNum = bChannelTable[STREAM0_CH] - WAVE1_ID;
				vResult = pVapi->pM_wave[bNum]->pVParser->ReadBlock(pbStmBuf, dStmLen, &dDataSize, VDEV_FIFO_STREAM0);
			}
			break;

		case VDEV_FIFO_STREAM1:
			if (pVapi->dFifoMap & 0xF00)
				vResult = pVapi->pM_vmd->pVParser->ReadBlock(pbStmBuf, dStmLen, &dDataSize, VDEV_FIFO_STREAM1);
			else
			{
				bNum = bChannelTable[STREAM1_CH] - WAVE1_ID;
				vResult = pVapi->pM_wave[bNum]->pVParser->ReadBlock(pbStmBuf, dStmLen, &dDataSize, VDEV_FIFO_STREAM1);
			}
			break;

		case VDEV_FIFO_STREAM2:
			if (pVapi->dFifoMap & 0xF000)
				vResult = pVapi->pM_vmd->pVParser->ReadBlock(pbStmBuf, dStmLen, &dDataSize, VDEV_FIFO_STREAM2);
			else
			{
				bNum = bChannelTable[STREAM2_CH] - WAVE1_ID;
				vResult = pVapi->pM_wave[bNum]->pVParser->ReadBlock(pbStmBuf, dStmLen, &dDataSize, VDEV_FIFO_STREAM2);
			}
			break;

		case VDEV_FIFO_STREAM3:
			if (pVapi->dFifoMap & 0xF0000)
				vResult = pVapi->pM_vmd->pVParser->ReadBlock(pbStmBuf, dStmLen, &dDataSize, VDEV_FIFO_STREAM3);
			else
			{
				bNum = bChannelTable[STREAM3_CH] - WAVE1_ID;
				vResult = pVapi->pM_wave[bNum]->pVParser->ReadBlock(pbStmBuf, dStmLen, &dDataSize, VDEV_FIFO_STREAM3);
			}
			break;

		case VDEV_FIFO_MP3:
			if (pVapi->dFifoMap & 0xF00000)
				vResult = pVapi->pM_vmd->pVParser->ReadBlock(pbStmBuf, dStmLen, &dDataSize, VDEV_FIFO_MP3);
			else
				vResult = pVapi->pM_mp3->pVParser->ReadBlock(pbStmBuf, dStmLen, &dDataSize, VDEV_FIFO_MP3);
			break;

		default:
			return VRES_ERROR_PARAMETERS;
	}

	return dDataSize;
}



/***************************************************************
Description:
		Initialize Midi Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static void InitMidiSource()
{

	pVapi->pM_midi->bState = W_IDLE;
	pVapi->pM_midi->dTotalTime = 0;
	pVapi->pM_midi->dPauseTime = 0;
	pVapi->pM_midi->dStartTime = 0;
	pVapi->pM_midi->dStopTime = 0;
	pVapi->pM_midi->pVParser = &parser_array[1];

}

/***************************************************************
Description:
		Initialize Wave Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static void InitWaveSource(UINT8 bChan)
{

	pVapi->pM_wave[bChan]->bState = W_IDLE;
	pVapi->pM_wave[bChan]->dTotalTime = 0;
	pVapi->pM_wave[bChan]->dPauseTime = 0;
	pVapi->pM_wave[bChan]->dStartTime = 0;
	pVapi->pM_wave[bChan]->dStopTime = 0;
	pVapi->pM_wave[bChan]->pVParser = &parser_array[4];

}


/***************************************************************
Description:
		Initialize VMD Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static void InitVmdSource()
{

	pVapi->pM_vmd->bState = W_IDLE;
	pVapi->pM_vmd->dTotalTime = 0;
	pVapi->pM_vmd->dPauseTime = 0;
	pVapi->pM_vmd->dStartTime = 0;
	pVapi->pM_vmd->dStopTime = 0;
	pVapi->pM_vmd->bMidi = 0;
	pVapi->pM_vmd->bWave = 0;
	pVapi->pM_vmd->bMp3 = 0;
	pVapi->pM_vmd->bFileNum = 0;
	pVapi->pM_vmd->pVParser = &parser_array[5];

}


/***************************************************************
Description:
		Initialize MP3 Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static void InitMP3Source()
{

	pVapi->pM_mp3->bState = W_IDLE;
	pVapi->pM_mp3->dTotalTime = 0;
	pVapi->pM_mp3->dPauseTime = 0;
	pVapi->pM_mp3->dStartTime = 0;
	pVapi->pM_mp3->dStopTime = 0;
	pVapi->pM_mp3->pVParser = &parser_array[3];

}

/***************************************************************
Description:
		Check Midi Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static UINT8 CheckMidiSource()
{
	if (pVapi->bVmdState == USED)
		goto merr;
	if (pVapi->bMidiState == USED)
		goto merr;
	if(bChannelTable[EVENT_CH] != NOTUSED)
		goto merr;

	return 1;
merr:
	return 0;

}

/***************************************************************
Description:
		Release Midi Source

Parameters:

Returns:
		  
****************************************************************/
static void ReleaseMidiSource()
{
	VPlr_EnterCriticalSection();
	pVapi->bMidiState = NOTUSED;
	bChannelTable[EVENT_CH] = NOTUSED;
	//pVapi->pM_midi->state = W_IDLE;
	VPlr_LeaveCriticalSection();
}

/***************************************************************
Description:
		Check Wave Channel Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static UINT8 CheckWaveSource(UINT8* statenum, UINT8* chnum)
{
	UINT8	bIndex;

	if(pVapi->bMp3State == USED)
		goto werr;

	*statenum = 0xFF;
	*chnum = 0xFF;

	for(bIndex=0; bIndex<VMD_MAX_WAVE_NUM; bIndex++)
	{
		if (pVapi->bWaveState[bIndex] == NOTUSED)
		{
			*statenum = bIndex;
			break;
		}
	}
	if (*statenum > (VMD_MAX_WAVE_NUM-1))
		goto werr;
	for(bIndex=0; bIndex<VMD_MAX_WAVE_NUM; bIndex++)
	{
		if(bChannelTable[STREAM0_CH+bIndex] == NOTUSED)
		{
			*chnum = bIndex;
			break;
		}
	}
	if(*chnum > (VMD_MAX_WAVE_NUM-1))
		goto werr;

	return 1;
werr:
	return 0;

}

/***************************************************************
Description:
		Release Wave Channel Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static void ReleaseWaveSource(UINT8 statenum, UINT8 chnum)
{
	VPlr_EnterCriticalSection();
	pVapi->bWaveState[statenum] = NOTUSED;
	bChannelTable[chnum] = NOTUSED;
	//pVapi->pM_wave[statenum]->bState = W_IDLE;
	VPlr_LeaveCriticalSection();
}

/***************************************************************
Description:
		Check MP3 Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static UINT8 CheckMp3Resource(void)
{
	UINT8	bIndex;

	for(bIndex=0; bIndex<VMD_MAX_WAVE_NUM; bIndex++)
	{
		if ((pVapi->bWaveState[bIndex] == USED) || (bChannelTable[STREAM0_CH + bIndex] != NOTUSED)) 
			goto mp3err;
	}
	if (pVapi->bMp3State == USED)
		goto mp3err;

	return 1;
mp3err:
	return 0;
}

/***************************************************************
Description:
		Release MP3 Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static void ReleaseMp3Source(void)
{
	VPlr_EnterCriticalSection();
	pVapi->bMp3State = NOTUSED;
	bChannelTable[MP3_CH] = NOTUSED;
	//pVapi->pM_mp3->state = W_IDLE;
	VPlr_LeaveCriticalSection();
}


/***************************************************************
Description:
		Release VMD Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static void ReleaseVmdSource(void)
{
	UINT8	bIndex, bNum;

	VPlr_EnterCriticalSection();
	pVapi->pM_vmd->bMidi = 0;
	pVapi->bMidiState = NOTUSED;
	pVapi->bVmdState = NOTUSED;
	if(pVapi->pM_vmd->bWave)
	{
		pVapi->pM_vmd->bWave = 0;
		for(bIndex=0; bIndex<(pVapi->pM_vmd->bTrackNum); bIndex++)
		{
			bNum = pVapi->pM_vmd->bVmdch[bIndex];
			pVapi->bWaveState[bNum] = NOTUSED;
		}
	}
	if(pVapi->pM_vmd->bMp3)
	{
		pVapi->pM_vmd->bMp3 = 0;
		pVapi->bMp3State = NOTUSED;
	}
	for(bIndex=0; bIndex<7; bIndex++)
	{
		if(bChannelTable[bIndex] == VMD1_ID)
			bChannelTable[bIndex] = NOTUSED;
	}
	VPlr_LeaveCriticalSection();
}


/***************************************************************
Description:
		Initialize iMelody Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static void InitImelodySource(void)
{

	pVapi->pM_imelody->bState = W_IDLE;
	pVapi->pM_imelody->dTotalTime = 0;
	pVapi->pM_imelody->dPauseTime = 0;
	pVapi->pM_imelody->dStartTime = 0;
	pVapi->pM_imelody->dStopTime = 0;
	pVapi->pM_imelody->pVParser = &parser_array[6];

}


/***************************************************************
Description:
		Initialize Xmf Source

Parameters:

Returns:
		Return Value: 1: OK, 0: has been occupied
		  
****************************************************************/
static void InitXmfSource(void)
{

	pVapi->pM_xmf->bState = W_IDLE;
	pVapi->pM_xmf->dTotalTime = 0;
	pVapi->pM_xmf->dPauseTime = 0;
	pVapi->pM_xmf->dStartTime = 0;
	pVapi->pM_xmf->dStopTime = 0;
	pVapi->pM_xmf->pVParser = &parser_array[7];

}

/***************************************************************
Description:
		Check 938 Hardware Channel occupation

Parameters:
		flag: return value(1: Used 0: Not Used)
			  0bit: Event Fifo
			  1bit: Command Fifo
			  2bit: Stream0 Fifo
			  3bit: Stream1 Fifo
			  4bit: Stream2 Fifo
			  5bit: Stream3 Fifo
Returns:
		  
****************************************************************/
static void CheckChannel(UINT32* flag)
{
	UINT8	bIndex;
	UINT32	dData = 0;

	VPlr_EnterCriticalSection();
	for(bIndex=0; bIndex<7; bIndex++)
	{
		if (bChannelTable[bIndex] != NOTUSED)
		{
			dData |= (1<<bIndex);
		}
	}
	VPlr_LeaveCriticalSection();
	*flag = dData;
}

/***************************************************************
Description:
		Read Big File from ROM

Parameters:
		bBuffer	   : buffer address
		dBufferSize : number bytes to be read
		pdRealSize  : number bytes read in fact
		dOffset	   : file offset

Returns:
		  NULL
		  
****************************************************************/
void ReadBuffer(UINT8* bBuffer, UINT32 dBufferSize, UINT32* pdRealSize, UINT32 dOffset)
{
	SINT32	dErr;
	SINT32	sdSize;
	void*	pFileptr;


	pFileptr = (void*)pbFileStream_Ptr;
	//VPlr_SeekFile(pFileptr, 0);
	dErr = VPlr_SeekFile(pFileptr, dOffset);
	if(dErr < 0)
	{
		*pdRealSize = 0;
		return;
	}
	sdSize = VPlr_ReadFile(pFileptr, bBuffer, dBufferSize);
	if(sdSize <= 0)
	{
		*pdRealSize = 0;
		return;
	}
	*pdRealSize = sdSize;
}


/***************************************************************
Description:
		According different user to define what to do
		in the end of play.

Parameters:
		void.

Returns:
		void.

****************************************************************/
void VPlr_Callback(UINT8 bFifoType, PVOID pExtPara)
{
	UINT8	bFunc_id;
	UINT32	dIndex, bNumid;


	bFunc_id = bChannelTable[bFifoType];

	switch (bFunc_id)
	{
		case MIDI_ID:
			if(pVapi->pM_midi->dRepeatNum)
			{
				dIndex = -- (pVapi->pM_midi->dRepeatNum);
				VPlr_Stop(bFunc_id);
				pVapi->pM_midi->dPauseTime = 0;
				VPlr_Play(bFunc_id, 0, dIndex, pFunction[0]);
			}
			else
			{
				//if (pVapi->pM_midi->bSource)
				//	pbFileStream_Ptr = NULL;
				//ReleaseMidiSource();
				if(pFunction[0] != NULL)
					pFunction[0]();			//Play End Function
			}
			break;
		case WAVE1_ID:
		case WAVE2_ID:
		case WAVE3_ID:
		case WAVE4_ID:
			bNumid = bFunc_id- WAVE1_ID;
			if(pVapi->pM_wave[bNumid]->dRepeatNum)
			{
				dIndex = -- (pVapi->pM_wave[bNumid]->dRepeatNum);
				VPlr_Stop(bFunc_id);
				pVapi->pM_wave[bNumid]->dPauseTime = 0;
				VPlr_Play(bFunc_id, 0, dIndex, pFunction[bNumid+1]);
			}
			else
			{
				//if (pVapi->pM_wave[bNumid]->bSource)
				//	pbFileStream_Ptr = NULL;
				//ReleaseWaveSource(bNumid, bFifoType);
				if(pFunction[bNumid+1] != NULL)
					pFunction[bNumid+1]();		//Play End Function
			}
			break;
		case VMD1_ID:
			pVapi->pM_vmd->bFileNum --;
			if(pVapi->pM_vmd->bFileNum)
			{
				switch(bFifoType)
				{
					case VDEV_FIFO_EVENT:
						break;
					case VDEV_FIFO_STREAM0:
					case VDEV_FIFO_STREAM1:
					case VDEV_FIFO_STREAM2:
					case VDEV_FIFO_STREAM3:
						for(dIndex=0; dIndex<(pVapi->pM_vmd->bTrackNum); dIndex++)
						{
							if(pVapi->pM_vmd->bVmdch[dIndex] == (bFifoType - VDEV_FIFO_STREAM0))
								break;
						}
						pVapi->pM_vmd->bTrackFile[dIndex] --;
						if(pVapi->pM_vmd->bTrackFile[dIndex])
						{
							Vsyn_StopSeq(bFifoType, NULL);
							Vsyn_SetAudioMode(VDEV_AUDIO_MODE_HOST_WAVE_MIDI, VMD_MAX_WAVE_NUM);
							Vsyn_SetWaterMark(bFifoType, BUFFER_STREAM_SIZE, NULL);
							Vsyn_StreamSetup(bFifoType, 1, NULL);
						}
						break;
					case VDEV_FIFO_MP3:
						pVapi->pM_vmd->bMp3Num --;
						if(pVapi->pM_vmd->bMp3Num)
						{
							Vsyn_StopSeq(bFifoType, NULL);
							Vsyn_SetAudioMode(VDEV_AUDIO_MODE_HOST_MP3_MIDI, 0);
							Vsyn_SetWaterMark(bFifoType, BUFFER_MP3_SIZE, NULL);
							Vsyn_StreamSetup(bFifoType, 1, NULL);
						}
						break;
				}
				//Vsyn_StartSeq(bFifoType, NULL);
			}
			else
			{
				if(pVapi->pM_vmd->dRepeatNum)
				{
					bNumid = --(pVapi->pM_vmd->dRepeatNum);
					pVapi->pM_vmd->bFileNum = pVapi->pM_vmd->bSFileNum;
					pVapi->pM_vmd->bTrackNum = pVapi->pM_vmd->bSTrackNum;
					pVapi->pM_vmd->bMp3Num = pVapi->pM_vmd->bSMp3Num;
					for (dIndex=0; dIndex<pVapi->pM_vmd->bTrackNum; dIndex++)
						pVapi->pM_vmd->bTrackFile[dIndex] = pVapi->pM_vmd->bSTrackFile[dIndex];
					VPlr_Stop(bFunc_id);
					pVapi->pM_vmd->dPauseTime = 0;
					VPlr_Play(bFunc_id, 0, bNumid, pFunction[5]);
				}
				else
				{
					//if (pVapi->pM_vmd->bSource)
					//	pbFileStream_Ptr = NULL;
					//ReleaseVmdSource();
					if(pFunction[5] != NULL)
						pFunction[5]();	//Play End Function
				}
			}
			break;
		case MP3_ID:
			if(pVapi->pM_mp3->dRepeatNum)
			{
				dIndex = -- (pVapi->pM_mp3->dRepeatNum);
				VPlr_Stop(bFunc_id);
				pVapi->pM_mp3->dPauseTime = 0;
				VPlr_Play(bFunc_id, 0, dIndex, pFunction[6]);
			}
			else
			{
				//if (pVapi->pM_mp3->bSource)
				//	pbFileStream_Ptr = NULL;
				//ReleaseMp3Source();
				if(pFunction[6] != NULL)
					pFunction[6]();			//Play End Function
			}
			break;

		case IMELODY_ID:
			if (pVapi->pM_imelody->dRepeatNum)
			{
				dIndex = --(pVapi->pM_imelody->dRepeatNum);
				VPlr_Stop(bFunc_id);
				pVapi->pM_imelody->dPauseTime  = 0;
				VPlr_Play(bFunc_id, 0, dIndex, pFunction[7]);
			}else{
				if (pFunction[7] != NULL)
					pFunction[7]();			//Play End Funcion.
			}
			break;
			
		case XMF_ID:
			if (pVapi->pM_xmf->dRepeatNum)
			{
				dIndex = --(pVapi->pM_xmf->dRepeatNum);
				VPlr_Stop(bFunc_id);
				pVapi->pM_xmf->dPauseTime  = 0;
				VPlr_Play(bFunc_id, 0, dIndex, pFunction[8]);
			}else{
				if (pFunction[8] != NULL)
					pFunction[8]();			//Play End Funcion.
			}
			break;
			
		default:
			break;
	}
}

/***************************************************************
Description:
		Get current polyphony numbers when play back a file.

Parameters:
		void.

Returns:
		Current polyphony numbers.

****************************************************************/
UINT32	Vplr_GetCurPolyphonyNum(void)
{
	return Vdev_GetCurPolyphonyNum();
}

/***************************************************************
Description:
		Set AES key data and initial vector data when the music 
		data that will be played back is encrypted by AES.

Parameters:
		pbAESKey	       :	the key data for AES Encrypted data.
		pbAESInitialVec :	the initial vector data for Encrypted data in AES CBC mode.

Returns:
		void.

****************************************************************/
void	Vplr_SetAESKey(PUINT8 pbAESKey, PUINT8 pbAESInitialVec)
{
	if (pVapi->pM_vmd->bAesMode)
		Vsyn_SetAESKey(VDEV_AES_MODE_CBC, VDEV_FIFO_MP3, pbAESKey, pbAESInitialVec, NULL);
	else
		Vsyn_SetAESKey(VDEV_AES_MODE_ECB, VDEV_FIFO_MP3, pbAESKey, pbAESInitialVec, NULL);
}


