/*-----------------------------------------------------------------------------


			COPYRIGHT (C) 2004, Vimicro Corporation
                        ALL RIGHTS RESERVED

 This source code has been made available to you by VIMICRO on an
 AS-IS basis. Anyone receiving this source code is licensed under VIMICRO
 copyrights to use it in any way he or she deems fit, including copying it,
 modifying it, compiling it, and redistributing it either with or without
 modifications. Any person who transfers this source code or any derivative
 work must include the VIMICRO copyright notice and this paragraph in
 the transferred software.
-----------------------------------------------------------------------------*/

#ifndef __VVMD_H__
#define __VVMD_H__

#if defined( __cplusplus )
extern "C" {
#endif

#define	VMHB	0x42484d56
#define VMXB	0x42584d56
#define	VMMB	0x424d4d56
#define	VMSB	0x42534d56
#define	VMEB	0x42454d56
#define	VMIB	0x42494d56
#define	VM3B	0x42334d56
#define	VMLB	0x424c4d56
#define	VMPB	0x42504d56
#define	VMKB	0x424b4d56
#define	ETrk	0x6b725445
#define DTrk	0x6b725444

#define	VMCB	0x42434d56
#define ATrk	0x6b725441
#define TTrk	0x6b725454
#define CTrk	0x6b725443

//--for vmd parser-------------------------------------------------------------
#define STREAM_TRUNK		4
#define	STREAM_BLOCK 		16
#define STREAM_DAT_NUM		64
#define	MP3_TRUNK			1
#define MP3_DAT_NUM			16
#define	PIC_DAT_NUM			16
#define CMT_A_LEN			64
#define CMT_T_LEN			64
#define	CMT_C_LEN			64
#define FS_READ_LEN			64

#define WAIT_QUE_LEN		128
#define STEAM_CHN_NUM		16
#define WAVE_TIME_INIT		200	//ms
#define TIMER_INTERVAL		100
#define	WAVE_MAX_LEN		536870
#define	WAVE_MAX_LEN1		6291456 //6M

#define VMD_MP3_MONO         3

#define VMD_STONE_VERSION	0x0018

typedef struct _MSGQUEUE
{
	UINT8	bID; 
	UINT8	bChn;
	UINT32	dPara1;		//Note 
	UINT32	dPara2;
	UINT32	dPara3;
	SINT32	sdTime;
	struct _MSGQUEUE* pNext;
} MSGQUEUE, *PMSGQUEUE;

typedef struct _MIDI_CHN_INFO
{
	UINT8	timbre;
	UINT8	volume;
	UINT8	expression;
	UINT8	modulation;
	UINT8	pitchbendH;
	UINT8	pitchbendL;
	UINT8	bendrange;
	UINT8	panpot;
	UINT8	hold1;
	UINT8	bkselid;
} MIDI_CHN_INFO, *PMIDI_CHN_INFO;

typedef enum _MEDIA_TYPE
{
	NOTHING	= 0,
	MIDI	= 1,
	WAVE	= 2,
	UDSE	= 3,
	UDI		= 4,
	MP3		= 5,
	LYRIC	= 6,
	PIC		= 7,
	CTRL	= 8,
	QUE		= 99
}MEDIA_TYPE;

typedef struct _MIDIBLK
{
	HUGE UINT8	*pbBuf;
	HUGE UINT8	*pbCurPtr;	 
	UINT32	dBufLen;
	UINT32	dFBasOffSet;
	UINT32  dFCurOffSet;
	UINT32	dOffSet;
} MIDIBLK, *PMIDIBLK;

typedef struct _VMD_ADPCM_STATE
{
    SINT16	swPreVal;	/* Previous output value */
    UINT8	bIndex;		/* Index into stepsize table */
}VMD_ADPCM_STATE, *PVMD_ADPCM_STATE;

typedef struct _STEAM_BUF
{
	UINT8	bFormat;
	UINT8	bChn;
	UINT8	bBitsPerSmpl;
	UINT8	bSigned;
	UINT32	dBufLen;
	UINT32	dOffSet;
	UINT32  dTotalTime;
	UINT32	dFBasOffSet;
	UINT32  dFCurOffSet;	
	UINT16	wSmplRate;
	HUGE UINT8	*pbBuf;
	VMD_ADPCM_STATE	AdpcmState;
}STEAM_BUF, *PSTRAM_BUF;

typedef struct _STMBLK
{
	UINT8		bEvtTrkNum;
	SINT8		sbStmTrkFlag[STREAM_TRUNK][6];	//0:Trunk ID corresponding to FIFO ID, 1:FIFO ID corresponding to Trunk ID, 2:Data Trunk Num, 3:Cur Data Trunk Num, 4:dCurTime 5:ReadBlockFlag
	UINT8		bStmTrkIndex[STREAM_TRUNK][STREAM_BLOCK];	//dat id corresponding to event
	UINT32		dEvtBufLen;
	UINT32		dFBasOffSet;
	UINT32  	dFCurOffSet;	
	UINT32		dOffSet;
	HUGE UINT8		*pbEvtBuf;
	HUGE UINT8		*pbCurEvtPtr;
	STEAM_BUF	DatBuf[STREAM_DAT_NUM];
} STMBLK, *PSTMBLK;

typedef struct _UDSEBLK
{
	UINT32	dEvtBufLen;
	UINT32	dDatBufLen;
	UINT32	dFBasEvtOffSet;
	UINT32	dFBasDatOffSet;
	UINT32  dFCurEvtOffSet;	
	UINT32	dFCurDatOffSet;
	UINT32	dOffSet;
	HUGE UINT8	*pbEvtBuf;
	HUGE UINT8	*pbCurEvtPtr;
	HUGE UINT8	*pbDatBuf;	
}UDSEBLK, *pUDSEBLK;

typedef struct _UDIBLK
{
	UINT32	dBufLen;
	UINT32	dFBasOffSet;
	UINT32  dFCurOffSet;	
	HUGE UINT8	*pbBuf;	
	UINT16	wMapWord;
} UDIBLK, *PUDIBLK;

typedef struct _MP3_BUF
{
	UINT16	wSmplRate;
	UINT16	wBitRate;
	UINT32	dBufLen;
	UINT32	dOffSet;
	UINT32  dTotalTime;
	UINT32	dFBasOffSet;
	UINT32  dFCurOffSet;	
	UINT8	bETC;
	UINT8	bSigned;
	HUGE UINT8	*pbBuf;	
}MP3_BUF, *PMP3_BUF;

typedef struct _MP3BLK
{
	HUGE UINT8	*pbEvtBuf;
	HUGE UINT8	*pbCurEvtPtr;
	UINT8	bDatNum;
	UINT8	bCurDatNum;
	UINT32	dEvtBufLen;
	UINT32	dFBasOffSet;
	UINT32  dFCurOffSet;	
	UINT32	dOffSet;
	UINT8	bMp3TrkIndex[MP3_DAT_NUM];	//dat id corresponding to event
	MP3_BUF	DatBuf[MP3_DAT_NUM];	
}MP3BLK, *PMP3BLK;

typedef struct _LYRICBLK
{
	UINT32	dBufLen;
	UINT32	dFBasOffSet;
	UINT32  dFCurOffSet;
	UINT32	dOffSet;
	HUGE UINT8	*pbBuf;
	HUGE UINT8	*pbCurPtr;		
}LYRICBLK, *PLYRICBLK;

typedef struct _PIC_BUF
{
	UINT32	dBufLen;
	UINT32	dFBasOffSet;
	UINT32	dFCurOffSet;
	HUGE UINT8	*pbBuf;	
	HUGE UINT8	*pbCurBuf;
	UINT8	bType;	
}PIC_BUF, *PPIC_BUF;

typedef struct _PICTUREBLK
{
	UINT32	dEvtBufLen;
	UINT32	dFBasOffSet;
	UINT32	dFCurOffSet;
	UINT32	dOffSet;
	HUGE UINT8	*pbEvtBub;
	HUGE UINT8	*pbCurEvtPtr;
	UINT8	bDatTrkNum;
	PIC_BUF DatBuf[PIC_DAT_NUM];
}PICBLK, *PPICBLK;

typedef struct _CONTROLBLK
{
	UINT32	dBufLen;
	UINT32	dOffSet;
	UINT32	dFBasOffSet;
	UINT32  dFCurOffSet;	
	HUGE UINT8	*pbBuf;
	HUGE UINT8	*pbCurPtr;		
}CTRLBLK, *PCTRLBLK;

typedef struct _VMDCMT
{
	UINT32	dAuthorLen;
	UINT32	dTitleLen;	
	UINT32	dCmtLen;
	UINT32	dFABasOffSet;
	UINT32	dFTBasOffSet;
	UINT32	dFCBasOffSet;
	UINT16	wEncoding;
	UINT8	bTitleBuf[CMT_T_LEN];
	UINT8	bAuthorBuf[CMT_A_LEN];
	UINT8	bCmtBuf[CMT_C_LEN];	
}VMDCMTBLK, *PVMDCMTBLK;

typedef struct _VMD_FILEINFO
{
	UINT32		dTotalTime;
	UINT16		wBlkNum;
	UINT16		wVmdFileVersion;
	UINT32		bEnabled;	//midi|wave|udse|udi|comment|mp3|lyric|pic|led|motor|filestream
	MIDIBLK		MidiBlk;
	STMBLK		WaveBlk;
	UDSEBLK		UdseBlk;
	UDIBLK		UdiBlk;
	MP3BLK		Mp3Blk;
	LYRICBLK	LyricBlk;
	PICBLK		PicBlk;
	CTRLBLK		CtrlBlk;
	VMDCMTBLK	CmtBlk;
}VMDFILEINFO, *PVMDFILEINFO;
//-----------------------------------------------------------------------------


VRESULT VVmd_Init			(void);
VRESULT VVmd_Load			(HUGE UINT8* File, UINT32 Length, UINT32 EmptyRes, UINT8 SrcFlag);
VRESULT	VVmd_Seek			(UINT32 dPos, VDEV_FIFO_TYPE FiFoType);
VRESULT VVmd_GetEvent		(PVM_UNI_MSG EventPtr, VDEV_FIFO_TYPE FiFoType);
VRESULT VVmd_GetFileInfo	(PVOID pvFileInfo, VDEV_FIFO_TYPE FiFoType);
VRESULT VVmd_GetCurPos		(HUGE UINT32 *pdCurTime, VDEV_FIFO_TYPE FiFoType);
VRESULT VVmd_ReadBlock		(HUGE UINT8 *pbOutBuf, UINT32 dBufLen, HUGE UINT32 *pdDataSize, VDEV_FIFO_TYPE FiFoType);
VRESULT VVmd_Unload			(VDEV_FIFO_TYPE FiFoType);
VRESULT VVmd_End 			(void);
VRESULT VVmd_Configure		(UINT32 dVal, UINT32 dDat, UINT8 bConfigID);
VRESULT VVmd_Reset			(VDEV_FIFO_TYPE FiFoType);

#if defined( __cplusplus )
}
#endif

#endif	/* __VVMD_H__ */
