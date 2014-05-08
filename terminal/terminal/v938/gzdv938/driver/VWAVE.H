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


-------------------------------------------------------------------------*/

#ifndef __VWAVE_H__
#define __VWAVE_H__

#define VM_WAVE_FILE_STREAM_BUF_LEN		128	


typedef struct _VM_ADPCM_STATE
{
    SINT16	swPreVal;	/* Previous output value */
    UINT8		bIndex;		/* Index into stepsize table */
}VM_ADPCM_STATE, *PVM_ADPCM_STATE;

typedef struct _VM_WAVE_DATA_INFO
{
	HUGE UINT8*	pbDataBuf;		//Point to wave data
	UINT8		bSentMsgNum;		
	UINT8		bStatus;			//0:Empty, 1:Nonempty
	UINT8		bUnSigned;		//Sampe is signed or Unsigned, 1:Unsigned; 0:Signed;
	UINT8		bSrcFlag;		//0: load from RAM, 1:Load from file stream
	UINT8		bFileStmBuf[VM_WAVE_FILE_STREAM_BUF_LEN];
	UINT8		bStmBufOffset;
	UINT16		wWavFmt;		//File format, 1:PCM 0:ADPCM	Others are note supported
	UINT16		wChn;			//Mono or stereo, 1:mono, 2:stereo
	UINT16		wBitsPerSmpl;	//The number of bit of one sample
	UINT16		wSmplRate;		//Sample rate
	UINT32		dStmBufLen;		//Buffer length
	UINT32		dDataSize;		//The size of PCM/ADPCM data without file header
	UINT32		dOffset;			//Offset to the "pbWavBuf" 
	UINT32		dOffsetBack;		//Backup dOffset
	UINT32		dTotalTime;		//Playback time of this file
	UINT32		dCurTime;		//The time of PCM/ADPCM which has be sent to FIFO

	VM_ADPCM_STATE AdpcmState;
} VM_WAVE_DATA_INFO, *PVM_WAVE_DATA_INFO;



typedef struct _VM_WAVE_PARSER_INFO
{
	VM_WAVE_DATA_INFO	WaveData[MAX_STREAM_VOICE];
	PVM_WAVE_DATA_INFO pWaveChn[MAX_STREAM_VOICE];

	UINT8	bEmptyNum;		//The number of "WaveData" can store PCM/ADPCM info.
	UINT8	bAssignedWav;	//The wave hasn't be assigned to a fifo;
} VM_WAVE_PARSER_INFO, *PVM_WAVE_PARSER_INFO;


VRESULT VWave_Init(VOID);
VRESULT VWave_Load(HUGE UINT8* pbFile, UINT32 dFileLen, UINT32 EmptyRes, UINT8 SrcFlag);
VRESULT VWave_GetEvent(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType);
VRESULT VWave_GetFileInfo(PVOID pvFileInfo, VDEV_FIFO_TYPE FiFoType);
VRESULT VWave_GetCurPos(PUINT32 pdCurTime, VDEV_FIFO_TYPE FiFoType);
VRESULT	VWave_Seek(UINT32 dPos, VDEV_FIFO_TYPE FiFoType);
VRESULT VWave_End(VOID);
VRESULT VWave_ReadBlock(PUINT8 pbOutBuf, UINT32 dBufLen, PUINT32 pdDataSize, VDEV_FIFO_TYPE FiFoType);
VRESULT VWave_Unload(VDEV_FIFO_TYPE FiFoType);
VRESULT	VWave_Configure(UINT32 dVal, UINT32 dDat, UINT8 bConfigID);
VRESULT VWave_Reset(VDEV_FIFO_TYPE FiFoType);

#endif /*__VWAVE_H__*/
