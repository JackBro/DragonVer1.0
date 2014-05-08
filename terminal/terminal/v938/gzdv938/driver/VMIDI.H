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


 Module:		Vmidi.h

 Description:	VC0938 midi parser functions declarations

 Update History:
	1.0			Basic version				

-----------------------------------------------------------------------*/

#ifndef __VMIDI_H__
#define __VMIDI_H__

#define MAX_MIDI_TRACK			64
#define MAX_MSG_NUM			128	//The max number of message in the message que
#define TEMPO_CODE				500000		// 120 bpm

#define MELTA_TEMPO				0x51

#define MELTA_EVENT				0xff

#define MSG_KEYON				0x90
#define MSG_KEYOFF				0x80
#define MSG_PROGCHANGE			0xc0
#define MSG_SYSEXCLUDE			0xF0
#define MSG_PITCHBEND			0xe0
#define MSG_CTRLCHANGE			0xb0
#define MSG_CHNPRESSURE			0xd0


#define CTRL_MODULATION			0x01
#define CTRL_DATAENTRYMSB		0x06
#define CTRL_DATAENTRYLSB		0x26
#define CTRL_VOLUME				0x07
#define CTRL_PANPOT				0x0a
#define CTRL_EXPRESS			0x0b	
#define CTRL_HOLD1				0x40
#define CTRL_RESETCTRLS			0x79
#define CTRL_ALLNOTEOFF		0x7b
#define CTRL_RPNLSB				0x64
#define CTRL_RPNMSB				0x65


typedef struct _VM_MIDI_TRACK
{
	HUGE UINT8	*pbDataHead;
	HUGE UINT8 	*pbCurnData;
	HUGE UINT8 	*pbDataEnd;
	UINT32	dNextTick;
	UINT8 	bPreStatus;
}VM_MIDI_TRACK, *PVM_MIDI_TRACK;

typedef struct _VM_MIDI_CHN
{
	UINT8	bTimbre;
	UINT8	bPan;
	UINT8	bModulation;
	UINT8	bPitchBendH;
	UINT8	bPitchBendL;
	UINT8	bPressure;
	UINT8	bExpression;
	UINT8	bVolume;
	UINT8	bHold1;
	UINT8	bBendRage;
	UINT8	bBankID;
} VM_MIDI_CHN, *PVM_MIDI_CHN;


typedef struct _VM_MIDI_INFO_BACKUP
{
	UINT32			dTicksPerQN;
	UINT32			dTempoMS;
	UINT32			dTrackEndH;
	UINT32			dTrackEndL;
} VM_MIDI_INFO_BACKUP, *PVM_MIDI_INFO_BACKUP;


typedef struct _VM_MIDI_PARSER_INFO
{
	VM_MIDI_CHN	ChnInfo[MAX_MIDI_CHN];
	VM_MIDI_TRACK 	Track[MAX_MIDI_TRACK];
	VM_UNI_MSG		MsgQue[MAX_MSG_NUM];
	
	VM_MIDI_INFO_BACKUP  InfoBackup;
	UINT16			wRPN[MAX_MIDI_CHN];
	UINT8			bMsgBuf[5];

	UINT8			bTrackNum;
	UINT8			bSendEOS;
	UINT8			bMsgQueNum;
	UINT8			bMidiFmt;
	UINT32			dTicksPerQN;
	UINT32			dTempoMS;
	UINT32			dTrackEndH;
	UINT32			dTrackEndL;
	UINT32			dPreTime;
	UINT32			dCurTime;
	UINT32			dCurTick;
	UINT32			dTotalTime;
	UINT32			dSeekOverTime;
} VM_MIDI_PARSER_INFO, *PVM_MIDI_PARSER_INFO;

VRESULT VMidi_Init(VOID);
VRESULT VMidi_Load(HUGE UINT8* pbFile, UINT32 dFileLen, UINT32 EmptyRes, UINT8 SrcFlag);
VRESULT VMidi_GetEvent(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType);
VRESULT VMidi_GetFileInfo(PVOID pvFileInfo, VDEV_FIFO_TYPE FiFoType);
VRESULT VMidi_GetCurPos(PUINT32 pdCurTime, VDEV_FIFO_TYPE FiFoType);
VRESULT	VMidi_Seek(UINT32 dPos, VDEV_FIFO_TYPE FiFoType);
VRESULT VMidi_End(VOID);
VRESULT VMidi_GetVoice(PVOID pvBuf);
VRESULT VMidi_GetEmStream(PVOID pvBuf);
VRESULT VMidi_ReadBlock(PUINT8 pbOutBuf, UINT32 dBufLen, PUINT32 pdDataSize, VDEV_FIFO_TYPE FiFoType);
VRESULT VMidi_Unload(VDEV_FIFO_TYPE FiFoType);
VRESULT VMidi_Configure(UINT32 dVal, UINT32 dDat, UINT8 bConfigID);
VRESULT VMidi_Reset(VDEV_FIFO_TYPE FiFoType);


#endif /*__VMIDI_H__*/
