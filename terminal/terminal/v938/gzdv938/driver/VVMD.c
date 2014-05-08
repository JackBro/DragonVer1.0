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

 Version		Description        					Date         	Author
 1.0			Basic Version						2004.08.26		stavy.sun
 1.1			first complete version				2004.10.29		stavy.sun
 1.2			add HUGE							2004.12.06      stavy.sun
 1.2.1			add udi block, remove lyric/pic		2004.12.11		stavy.sun
 				just ctrl block time to deltatime
 				modified stream on and mp3 on event,
 				add vol and pan
 1.2.2 			delete GetWaveID(), GetMp3ID()		2004.12.15		stavy.sun
 				add Fstm_LoadIdx()
 1.2.3 			fixed a bug in VVmd_Load			2004.12.17		stavy.sun
 				it is located in justing resource
 				of wave block.
 1.2.4			change wDeltaTime to dDeltatime.	2005.01.04		stavy.sun
 				form UINT16 to UINT32.	
 				modified the method of calculating
 				wave time.
 1.2.5			divide wave and mp3 duration based	2005.01.11		stavy.sun
 				on 0xFFFF. insert VMD_MSG_NOP message
 				before VMD_MSG_STREAM_OFF and
 				VMD_MSG_MP3_OFF.
 1.2.6			fixed some little bug in get_event_type(),			stavy.sun 
 				get_midi_event(), get_wave_event(), get_mp3_event().
 				all is about data type, change UINT16 to UINT32.
 				the bug is found by zhaoyuan from vmdmaker.
 1.2.7			fixed a bug in calculating the play 2005.02.05		stavy.sun
 				time of wave in vmd. we should multiple 
 				it with 4 becaulse it is adpcm buffer.
 1.2.8			modified the method of calculating  2005.02.17		stavy.sun
 				the play time of wave in vmd. 1.2.7 is wrong.
 1.2.9			fixed a bug in claculating wave		2005.03.09		stavy.sun
				playing time. there is difference 
				between adpcm and pcm. data length
				must multiple 4 if it is adpcm.		2005.03.19		stavy.sun
 1.3.0			function Fstm_LoadIdx is not stable.	
 				remove it now. but it need modifination
 				later.
-----------------------------------------------------------------------------*/

#include "vmd.h"
#include "vvmd.h"

#define		VFSTM	0
#if(1 == VFSTM)
UINT8	*gBuf = NULL;
UINT32	gLen  = 0;
#endif

//--------define global variable-----------------------------------------------
VMDFILEINFO	_FAR g_VmdInfo;

MSGQUEUE	_FAR g_WaitingMsgQue[WAIT_QUE_LEN];
MSGQUEUE	_FAR g_EmptyHdr;
MSGQUEUE	_FAR g_WaitingHdr;

UINT8		_FAR g_bEndFlag;						//end of all of event
UINT32		_FAR g_dEndEvent;

SINT32		_FAR g_sdLstMainTime;
SINT32		_FAR g_sdLstMidiTime;
SINT32		_FAR g_sdLstWaveTime;
SINT32		_FAR g_sdLstMp3Time;
SINT32		_FAR g_sdLstUdseTime;
SINT32		_FAR g_sdLstLyricTime;
SINT32		_FAR g_sdLstPicTime;
SINT32		_FAR g_sdLstCtrlTime;
SINT32		_FAR g_sdEndTime;

UINT8		_FAR g_bLstMsgCode;
UINT8		_FAR g_bLstCtrlCode;

UINT16		_FAR g_wRPN[MAX_MIDI_CHN];

MIDI_CHN_INFO	_FAR g_ChnInfo[MAX_MIDI_CHN];


UINT32 g_VmdMp3Bitrate[2][3][16] = 
{
   { {0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,},
     {0,32,48,56, 64, 80, 96,112,128,160,192,224,256,320,384,},
     {0,32,40,48, 56, 64, 80, 96,112,128,160,192,224,256,320,} },

   { {0,32,48,56,64,80,96,112,128,144,160,176,192,224,256,},
     {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,},
     {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,} }
};

UINT32 g_VmdMp3Frequency[9] = {44100,48000,32000,22050,24000,16000,11025,12000,8000 };


//--------declare function-----------------------------------------------------
UINT32		ScanMIDIBlk(HUGE UINT8 *pBuf, UINT32 len);
UINT32 		ScanStreamBlk(HUGE UINT8 *pbPtr);
UINT32 		ScanMp3Blk(HUGE UINT8 * pbPtr);
UINT32 		ScanCtrlBlk(HUGE UINT8 *pbPtr, UINT32 dLen);
VRESULT		SeekMidiMsg(UINT32 dSeekTime, VDEV_FIFO_TYPE FiFoType);
VRESULT		SeekWaveMsg(UINT32 dSeekTime, HUGE UINT32* dNewTime, VDEV_FIFO_TYPE FiFoType);
VRESULT		SeekMp3Msg(UINT32 dSeekTime, VDEV_FIFO_TYPE FiFoType);
VRESULT		SeekCtrlMsg(UINT32 dSeekTime, VDEV_FIFO_TYPE FiFoType);
PMSGQUEUE	GetMsgFromQue(void);
void		GetWaitingEvent(PVM_UNI_MSG p);
void		SendMsgToQue(PMSGQUEUE ptr);
void 		SendMidiMsg(void);
MEDIA_TYPE	Get_Event_Type(void);
VRESULT 	Get_Midi_Event(PVM_UNI_MSG EventPtr);
VRESULT 	Get_Wave_Event(PVM_UNI_MSG EventPtr);
VRESULT 	Get_Udse_Event(PVM_UNI_MSG EventPtr);
VRESULT 	Get_Mp3_Event(PVM_UNI_MSG EventPtr);
VRESULT 	Get_Ctrl_Event(PVM_UNI_MSG EventPtr);
VRESULT 	Get_Que_Event(PVM_UNI_MSG EventPtr);
VRESULT		CheckMp3Head(UINT32 dHead, UINT32 dTotalSize, HUGE UINT32 *pdTime);
VRESULT		Sync_Mp3Check(UINT32 dHead);
VRESULT 	Fstm_ReadBuf(HUGE UINT8* pbBuffer, UINT32 dBufferSize, HUGE UINT32* pdRealsize, UINT32 dOffSet);
VRESULT 	Fstm_Load(HUGE UINT8 *File, UINT32 Length, UINT32 EmptyRes);
VRESULT 	Fstm_LoadIdx(UINT32 EmptyRes, UINT32 OffSet, UINT16 Version);
UINT32 		Fstm_ScanMIDIBlk(UINT32 dBasOffSet, UINT32 dBufLen);
UINT32 		Fstm_ScanStreamBlk(UINT32 dBasOffSet);
UINT32 		Fstm_ScanMp3Blk(UINT32	dBasOffSet);
UINT32 		Fstm_ScanCtrlBlk(UINT32 dBasOffSet, UINT32 dBufLen);

extern void ReadBuffer(UINT8* bBuffer, UINT32 dBufferSize, UINT32* pdRealSize, UINT32 dOffset);


/******************************************************************************
Description:
		Initial all of the variable of VMD parser.
		
Parameters:
		void.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.

Remarks:		
		
******************************************************************************/
VRESULT VVmd_Init(void)
{
	UINT8	i = 0, j = 0;
	
	g_bEndFlag			= 0;
	g_dEndEvent			= 0;
	g_bLstMsgCode		= 0;
	g_bLstCtrlCode		= 0;

	g_sdLstMainTime		= 0;
	g_sdLstMidiTime		= 0;
	g_sdLstWaveTime		= 0;
	g_sdLstUdseTime		= 0;
	g_sdLstMp3Time		= 0;
	g_sdLstLyricTime	= 0;
	g_sdLstPicTime		= 0;
	g_sdLstCtrlTime		= 0;
	g_sdEndTime			= 0;

	for(i=0; i<MAX_MIDI_CHN; i++)
	{
		g_wRPN[i] = 0x7f7f;
	}

	for(i=1; i<WAIT_QUE_LEN; i++)
	{
		g_WaitingMsgQue[i-1].pNext 	= &g_WaitingMsgQue[i];
		g_WaitingMsgQue[i].dPara1 	= 0;
		g_WaitingMsgQue[i].dPara2 	= 0;
		g_WaitingMsgQue[i].dPara3	= 0;
	}
	g_EmptyHdr.pNext 	= g_WaitingMsgQue;
	g_WaitingMsgQue[WAIT_QUE_LEN-1].pNext = NULL;
	g_WaitingHdr.pNext 	= NULL;
	g_WaitingHdr.sdTime = 0;

	g_VmdInfo.dTotalTime		= 0;	
	g_VmdInfo.wBlkNum			= 0;
	g_VmdInfo.bEnabled			= 0;
	g_VmdInfo.wVmdFileVersion	= 0x0000;

	g_VmdInfo.MidiBlk.pbBuf			= NULL;
	g_VmdInfo.MidiBlk.pbCurPtr		= NULL;
	g_VmdInfo.MidiBlk.dBufLen		= 0;
	g_VmdInfo.MidiBlk.dFBasOffSet	= 0;
	g_VmdInfo.MidiBlk.dFCurOffSet	= 0;
	g_VmdInfo.MidiBlk.dOffSet		= 0;

	for(i=0; i<STREAM_DAT_NUM; i++)
	{
		g_VmdInfo.WaveBlk.DatBuf[i].bFormat				= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].bChn				= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].bBitsPerSmpl		= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].bSigned				= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].dBufLen				= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].dOffSet				= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].dTotalTime			= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].dFBasOffSet			= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].dFCurOffSet			= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].wSmplRate			= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].pbBuf				= NULL;
		g_VmdInfo.WaveBlk.DatBuf[i].AdpcmState.bIndex	= 0;
		g_VmdInfo.WaveBlk.DatBuf[i].AdpcmState.swPreVal	= 0;
	}
	for(i=0; i<STREAM_TRUNK; i++)
	{
		g_VmdInfo.WaveBlk.sbStmTrkFlag[i][0] 	= -1;	//trunk id
		g_VmdInfo.WaveBlk.sbStmTrkFlag[i][1] 	= -1;	//fifo id		
		g_VmdInfo.WaveBlk.sbStmTrkFlag[i][2] 	= 0;	//total data num
		g_VmdInfo.WaveBlk.sbStmTrkFlag[i][3] 	= 0;	//current data num	
		g_VmdInfo.WaveBlk.sbStmTrkFlag[i][4] 	= 0;	//dCurTime
		g_VmdInfo.WaveBlk.sbStmTrkFlag[i][5] 	= 0;	//ReadBlock Flag
	}
	for(i=0; i<STREAM_TRUNK; i++)
	{
		for(j=0; j<STREAM_BLOCK; j++)
		{
			g_VmdInfo.WaveBlk.bStmTrkIndex[i][j]	= 0xff;
		}
	}
	g_VmdInfo.WaveBlk.bEvtTrkNum	= 0;
	g_VmdInfo.WaveBlk.dEvtBufLen	= 0;
	g_VmdInfo.WaveBlk.dFBasOffSet	= 0;
	g_VmdInfo.WaveBlk.dFCurOffSet	= 0;
	g_VmdInfo.WaveBlk.dOffSet		= 0;
	g_VmdInfo.WaveBlk.pbEvtBuf		= NULL;
	g_VmdInfo.WaveBlk.pbCurEvtPtr	= NULL;	
	
	g_VmdInfo.UdseBlk.dEvtBufLen		= 0;
	g_VmdInfo.UdseBlk.dDatBufLen		= 0;
	g_VmdInfo.UdseBlk.dFBasEvtOffSet	= 0;
	g_VmdInfo.UdseBlk.dFBasDatOffSet	= 0;
	g_VmdInfo.UdseBlk.dFCurEvtOffSet	= 0;
	g_VmdInfo.UdseBlk.dFCurDatOffSet	= 0;
	g_VmdInfo.UdseBlk.dOffSet			= 0;
	g_VmdInfo.UdseBlk.pbDatBuf			= NULL;
	g_VmdInfo.UdseBlk.pbEvtBuf			= NULL;
	g_VmdInfo.UdseBlk.pbCurEvtPtr		= NULL;

	g_VmdInfo.UdiBlk.dBufLen		= 0;
	g_VmdInfo.UdiBlk.dFBasOffSet	= 0;
	g_VmdInfo.UdiBlk.dFCurOffSet	= 0;
	g_VmdInfo.UdiBlk.pbBuf			= NULL;
	g_VmdInfo.UdiBlk.wMapWord		= 0;

	g_VmdInfo.Mp3Blk.pbEvtBuf		= NULL;
	g_VmdInfo.Mp3Blk.pbCurEvtPtr	= NULL;
	g_VmdInfo.Mp3Blk.bDatNum		= 0;
	g_VmdInfo.Mp3Blk.bCurDatNum		= 0;	
	g_VmdInfo.Mp3Blk.dEvtBufLen		= 0;
	g_VmdInfo.Mp3Blk.dFBasOffSet	= 0;
	g_VmdInfo.Mp3Blk.dFCurOffSet	= 0;
	g_VmdInfo.Mp3Blk.dOffSet		= 0;
	for(i=0; i<MP3_DAT_NUM; i++)
	{
		g_VmdInfo.Mp3Blk.DatBuf[i].wSmplRate 	= 0;
		g_VmdInfo.Mp3Blk.DatBuf[i].wBitRate 	= 0;
		g_VmdInfo.Mp3Blk.DatBuf[i].dBufLen 		= 0;
		g_VmdInfo.Mp3Blk.DatBuf[i].dOffSet 		= 0;
		g_VmdInfo.Mp3Blk.DatBuf[i].dTotalTime 	= 0;
		g_VmdInfo.Mp3Blk.DatBuf[i].dFBasOffSet	= 0;
		g_VmdInfo.Mp3Blk.DatBuf[i].dFCurOffSet	= 0;
		g_VmdInfo.Mp3Blk.DatBuf[i].bETC 		= 0;		
		g_VmdInfo.Mp3Blk.DatBuf[i].bSigned 		= 0;		
		g_VmdInfo.Mp3Blk.DatBuf[i].pbBuf 		= NULL;

		g_VmdInfo.Mp3Blk.bMp3TrkIndex[i]		= 0xff;
	}
	
	g_VmdInfo.LyricBlk.dBufLen		= 0;
	g_VmdInfo.LyricBlk.dFBasOffSet	= 0;
	g_VmdInfo.LyricBlk.dFCurOffSet	= 0;
	g_VmdInfo.LyricBlk.dOffSet		= 0;
	g_VmdInfo.LyricBlk.pbBuf		= NULL;
	g_VmdInfo.LyricBlk.pbCurPtr		= NULL;

	g_VmdInfo.PicBlk.dEvtBufLen		= 0;
	g_VmdInfo.PicBlk.dFBasOffSet	= 0;
	g_VmdInfo.PicBlk.dFCurOffSet	= 0;
	g_VmdInfo.PicBlk.dOffSet		= 0;
	g_VmdInfo.PicBlk.pbEvtBub		= NULL;
	g_VmdInfo.PicBlk.pbCurEvtPtr	= NULL;
	g_VmdInfo.PicBlk.bDatTrkNum		= 0;
	for(i=0; i<PIC_DAT_NUM; i++)
	{
		g_VmdInfo.PicBlk.DatBuf[i].dBufLen		= 0;
		g_VmdInfo.PicBlk.DatBuf[i].dFBasOffSet	= 0;
		g_VmdInfo.PicBlk.DatBuf[i].dFCurOffSet	= 0;
		g_VmdInfo.PicBlk.DatBuf[i].pbBuf		= NULL;
		g_VmdInfo.PicBlk.DatBuf[i].pbCurBuf		= NULL;
		g_VmdInfo.PicBlk.DatBuf[i].bType		= 0;
	}

	g_VmdInfo.CtrlBlk.dBufLen		= 0;
	g_VmdInfo.CtrlBlk.dOffSet		= 0;
	g_VmdInfo.CtrlBlk.dFBasOffSet	= 0;
	g_VmdInfo.CtrlBlk.dFCurOffSet	= 0;
	g_VmdInfo.CtrlBlk.pbBuf			= NULL;
	g_VmdInfo.CtrlBlk.pbCurPtr		= NULL;

	g_VmdInfo.CmtBlk.dAuthorLen		= 0;
	g_VmdInfo.CmtBlk.dTitleLen		= 0;
	g_VmdInfo.CmtBlk.dCmtLen		= 0;
	g_VmdInfo.CmtBlk.dFABasOffSet	= 0;
	g_VmdInfo.CmtBlk.dFTBasOffSet	= 0;
	g_VmdInfo.CmtBlk.dFCBasOffSet	= 0;	
	g_VmdInfo.CmtBlk.wEncoding		= 0;
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Load the VMD data, it scan all the buf to get all of the block dat, 
		and store all of the event or data to specified buf. so vmd parser
		can use them in later procress freely.
		Validity of vmd file format will be checked in this function, and
		VRES_ERROR will be returned if error occured.
		
		
Parameters:
		File:			buf pointer of vmd file.
		Length:			the length of vmd file.
		EmptyRes:		empty resource, 
						it is event fifo, command fifo, stream0 - stream3 fifo
						from bit 0 to bit 5. 0 means unused, 1 means used.
		SrcFlag:		0 means data is from RAM, 1 means data is from ROM
						(file stream).
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT VVmd_Load(HUGE UINT8* File, UINT32 Length, UINT32 EmptyRes, UINT8 SrcFlag)
{
	HUGE UINT8	*pbBuf = File, *pbTrk = NULL, *pbTmp = NULL;
	UINT8 	i = 0, bcmtlen = 0, budilen = 0;
	UINT16  wNum = 0, wType = 0, wVersion = 0;
	UINT32	dBlkID = 0, dBlkLen = 0, dCounter = 0, dBlkTime = 0, dTrkLen = 0, dTrkID = 0, dTrkNum = 0;

#if(1 == VFSTM)
	gLen = Length;
	gBuf = (UINT8*)calloc(gLen, 1);
	memmove(gBuf, File, gLen);
	g_VmdInfo.bEnabled	|= 0x0400;
	return Fstm_Load(File, Length, EmptyRes);
#endif

	if(SrcFlag)
	{
		g_VmdInfo.bEnabled	|= 0x0400;
		return Fstm_Load(File, Length, EmptyRes);
	}
	
	dCounter = 0;

	dBlkID		= (((UINT32)pbBuf[3])<<24) + (((UINT32)pbBuf[2])<<16) + (((UINT32)pbBuf[1])<<8) + (UINT32)pbBuf[0];
	dBlkLen		= (((UINT32)pbBuf[7])<<24) + (((UINT32)pbBuf[6])<<16) + (((UINT32)pbBuf[5])<<8) + (UINT32)pbBuf[4];
	wVersion	= (((UINT16)pbBuf[9])<<8)  + (UINT16)pbBuf[8];
	wType		= (((UINT16)pbBuf[11])<<8) + (UINT16)pbBuf[10];
	wNum		= (((UINT16)pbBuf[13])<<8) + (UINT16)pbBuf[12];

	if(wVersion < VMD_STONE_VERSION)
	{
		bcmtlen = 2;
		budilen = 2;
	}
	else
	{
		bcmtlen = 0;
		budilen = 0;
	}
	
	g_VmdInfo.wVmdFileVersion 	= wVersion;

	if( (dBlkID!=VMHB) || ((wNum==1)&&(wType==1)) || ((wNum>1)&&(wType==0)) )	//control vmd file version
	{
		return VRES_VMD_INVALID_DATA;
	}
	else
	{
		g_VmdInfo.wBlkNum = (UINT8)wNum;
		pbBuf += 14;
	}
	
	for(dCounter=0; dCounter<g_VmdInfo.wBlkNum; dCounter++)
	{
		dBlkID	= (((UINT32)pbBuf[3])<<24) + (((UINT32)pbBuf[2])<<16) + (((UINT32)pbBuf[1])<<8) + (UINT32)pbBuf[0];
		dBlkLen	= (((UINT32)pbBuf[7])<<24) + (((UINT32)pbBuf[6])<<16) + (((UINT32)pbBuf[5])<<8) + (UINT32)pbBuf[4];
		switch(dBlkID)
		{
		case VMMB:	//midi, event fifo
			if(EmptyRes&0x01)
			{
				return VRES_ERROR;
			}
			if( ( ((((UINT16)pbBuf[9])<<8)	+ (UINT16)pbBuf[8])  == 0 ) && 
				( ((((UINT16)pbBuf[11])<<8)	+ (UINT16)pbBuf[10]) == 1 ) && 
				( ((((UINT32)pbBuf[15])<<24)	+ (((UINT32)pbBuf[14])<<16) + (((UINT32)pbBuf[13])<<8) + (UINT32)pbBuf[12]) == ETrk )
			  )
			{
				g_VmdInfo.bEnabled		   |= 0x01;
				g_VmdInfo.MidiBlk.pbBuf		= pbBuf+20;
				g_VmdInfo.MidiBlk.pbCurPtr	= pbBuf+20;
				g_VmdInfo.MidiBlk.dBufLen	= (((UINT32)pbBuf[19])<<24) + (((UINT32)pbBuf[18])<<16)
							+ (((UINT32)pbBuf[17])<<8) + (UINT32)pbBuf[16];
				
				dBlkTime = ScanMIDIBlk(g_VmdInfo.MidiBlk.pbBuf, g_VmdInfo.MidiBlk.dBufLen);
				g_VmdInfo.dTotalTime		= dBlkTime>g_VmdInfo.dTotalTime?dBlkTime:g_VmdInfo.dTotalTime;
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		case VMSB:	//wave, event+stream fifo
			if(EmptyRes&0x01)
			{
				return VRES_ERROR;
			}
			if( ( (((UINT16)pbBuf[9])<<8) 	+ (UINT16)pbBuf[8] == 1 ) && 
				( (((UINT16)pbBuf[11])<<8) 	+ (UINT16)pbBuf[10] >= 2 ) &&
				( ((((UINT32)pbBuf[15])<<24)	+ (((UINT32)pbBuf[14])<<16) + (((UINT32)pbBuf[13])<<8) + (UINT32)pbBuf[12]) == ETrk )
			   )
			{
				g_VmdInfo.bEnabled	|= 0x02;
				dBlkTime = ScanStreamBlk(pbBuf);
				g_VmdInfo.dTotalTime = dBlkTime>g_VmdInfo.dTotalTime?dBlkTime:g_VmdInfo.dTotalTime;

				if((4 - ((EmptyRes&0x04)>>2) - ((EmptyRes&0x08)>>3) - ((EmptyRes&0x10)>>4) - ((EmptyRes&0x20)>>5))<g_VmdInfo.WaveBlk.bEvtTrkNum)
				{
					return VRES_ERROR;
				}
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		case VMEB:	//udse, event+stream fifo
			if( ( (((UINT16)pbBuf[9])<<8) 	 + (UINT16)pbBuf[8] == 1 ) && 
				( (((UINT16)pbBuf[11])<<8) 	 + (UINT16)pbBuf[10] == 2 ) &&
				( ((((UINT32)pbBuf[15])<<24) + (((UINT32)pbBuf[14])<<16) + (((UINT32)pbBuf[13])<<8) + (UINT32)pbBuf[12]) == ETrk )
			   )
			{
				g_VmdInfo.bEnabled			   |= 0x04;
				g_VmdInfo.UdseBlk.dEvtBufLen	= (((UINT32)pbBuf[19])<<24)+ (((UINT32)pbBuf[18])<<16) 
					 + (((UINT32)pbBuf[17])<<8) + (UINT32)pbBuf[16];
				g_VmdInfo.UdseBlk.pbEvtBuf		= pbBuf+20;
				
				pbTmp	= g_VmdInfo.UdseBlk.pbEvtBuf + g_VmdInfo.UdseBlk.dEvtBufLen;
				g_VmdInfo.UdseBlk.dDatBufLen	= (((UINT32)pbTmp[7])<<24)+ (((UINT32)pbTmp[6])<<16) 
					+ (((UINT32)pbTmp[5])<<8) + (UINT32)pbTmp[4];
				g_VmdInfo.UdseBlk.pbDatBuf	= pbTmp+8;
				pbTmp	= NULL;
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		case VMIB:	//udi, event+stream fifo
			if(EmptyRes&0x01)
			{
				return VRES_ERROR;
			}
			if( ( (((UINT16)pbBuf[9])<<8)	+ (UINT16)pbBuf[8]  == 0 ) && 
				( (((UINT16)pbBuf[11])<<8)	+ (UINT16)pbBuf[10] == 1 ) &&
				( (((UINT32)pbBuf[15+budilen])<<24)	+ (((UINT32)pbBuf[14+budilen])<<16) + (((UINT32)pbBuf[13+budilen])<<8) + (UINT32)pbBuf[12+budilen] == DTrk )
			   )
			{
				g_VmdInfo.bEnabled		   |= 0x08;
				if(wVersion < VMD_STONE_VERSION)
				{
					g_VmdInfo.UdiBlk.wMapWord	= (((UINT16)pbBuf[13])<<8)	+ (UINT16)pbBuf[12];
				}
				else
				{
					g_VmdInfo.UdiBlk.wMapWord	= 0;
				}
				g_VmdInfo.UdiBlk.pbBuf		= pbBuf+20+budilen;
				g_VmdInfo.UdiBlk.dBufLen	= (((UINT32)pbBuf[19+budilen])<<24)	+ (((UINT32)pbBuf[18+budilen])<<16) 
						+ (((UINT32)pbBuf[17+budilen])<<8) + (UINT32)pbBuf[16+budilen];

				if((4 - ((EmptyRes&0x04)>>2) - ((EmptyRes&0x08)>>3) - ((EmptyRes&0x10)>>4) - ((EmptyRes&0x20)>>5))<1)
				{
					return VRES_ERROR;
				}
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		case VMCB:	//comment info
			dTrkID	= 0;
			dTrkLen	= 0;
			dTrkNum	= (((UINT16)pbBuf[11])<<8) + (UINT16)pbBuf[10];	
			if(wVersion < VMD_STONE_VERSION)
			{
				g_VmdInfo.CmtBlk.wEncoding = (((UINT16)pbBuf[13])<<8) + (UINT16)pbBuf[12];	
			}
			else
			{
				g_VmdInfo.CmtBlk.wEncoding = 0;
			}
			pbTrk = &pbBuf[12+bcmtlen];
			g_VmdInfo.bEnabled	|= 0x10;

			for(i=0; i<dTrkNum; i++)
			{
				dTrkID	= (((UINT32)pbTrk[3])<<24) + (((UINT32)pbTrk[2])<<16) + (((UINT32)pbTrk[1])<<8) + (UINT32)pbTrk[0];
				dTrkLen	= (((UINT32)pbTrk[7])<<24) + (((UINT32)pbTrk[6])<<16) + (((UINT32)pbTrk[5])<<8) + (UINT32)pbTrk[4];
				switch(dTrkID)
				{
				case TTrk:
					g_VmdInfo.CmtBlk.dTitleLen	= (dTrkLen > CMT_T_LEN)?(CMT_T_LEN):(dTrkLen);
					Vmd_MemCpy((HUGE UINT8*)(g_VmdInfo.CmtBlk.bTitleBuf), (HUGE UINT8*)pbTrk+8, g_VmdInfo.CmtBlk.dTitleLen);
					break;
				case ATrk:
					g_VmdInfo.CmtBlk.dAuthorLen		= (dTrkLen > CMT_A_LEN)?(CMT_A_LEN):(dTrkLen);
					Vmd_MemCpy((HUGE UINT8*)(g_VmdInfo.CmtBlk.bAuthorBuf), (HUGE UINT8*)pbTrk+8, g_VmdInfo.CmtBlk.dAuthorLen);					
					break;
				case CTrk:
					g_VmdInfo.CmtBlk.dCmtLen		= (dTrkLen > CMT_C_LEN)?(CMT_C_LEN):(dTrkLen);
					Vmd_MemCpy((HUGE UINT8*)(g_VmdInfo.CmtBlk.bCmtBuf), (HUGE UINT8*)pbTrk+8, g_VmdInfo.CmtBlk.dCmtLen);										
					break;
				}
				pbTrk += dTrkLen + 8;
			}
			break;
		case VM3B:	//mp3, event+stream fifo
			if(EmptyRes&0x01)
			{
				return VRES_ERROR;
			}
			if( ( (((UINT16)pbBuf[9])<<8)	+ (UINT16)pbBuf[8]  == 1 ) && 
				( (((UINT16)pbBuf[11])<<8)	+ (UINT16)pbBuf[10] >= 2 ) &&
				( (((UINT32)pbBuf[15])<<24)	+ (((UINT32)pbBuf[14])<<16) + (((UINT32)pbBuf[13])<<8) + (UINT32)pbBuf[12] == ETrk )
			   )
			{
				g_VmdInfo.bEnabled 	|= 0x20;
				dBlkTime = ScanMp3Blk(pbBuf);
				g_VmdInfo.dTotalTime = dBlkTime>g_VmdInfo.dTotalTime?dBlkTime:g_VmdInfo.dTotalTime;

				if((4 - ((EmptyRes&0x04)>>2) - ((EmptyRes&0x08)>>3) - ((EmptyRes&0x10)>>4) - ((EmptyRes&0x20)>>5))<1)
				{
					return VRES_ERROR;
				}
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		case VMKB:	//control, event fifo
			if(EmptyRes&0x01)	//perhaps modify later
			{
				return VRES_ERROR;
			}
			if( ( (((UINT16)pbBuf[9])<<8)	+ (UINT16)pbBuf[8]  == 0 ) && 
				( (((UINT16)pbBuf[11])<<8)	+ (UINT16)pbBuf[10] == 1 ) &&
				( (((UINT32)pbBuf[15])<<24)	+ (((UINT32)pbBuf[14])<<16) + (((UINT32)pbBuf[13])<<8) + (UINT32)pbBuf[12] == ETrk )
			   )
			{
				g_VmdInfo.CtrlBlk.pbBuf		= pbBuf+20;
				g_VmdInfo.CtrlBlk.pbCurPtr	= pbBuf+20;
				g_VmdInfo.CtrlBlk.dOffSet	= 0;
				g_VmdInfo.CtrlBlk.dBufLen	= (((UINT32)pbBuf[19])<<24)+ (((UINT32)pbBuf[18])<<16) 
					+ (((UINT32)pbBuf[17])<<8) + (UINT32)pbBuf[16];

				ScanCtrlBlk(g_VmdInfo.CtrlBlk.pbBuf, g_VmdInfo.CtrlBlk.dBufLen);
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		default:
			break;
		}
		pbBuf += dBlkLen + 8;
	}
	
	return VRES_SUCCESS;
}



/*****************************************************************************
Description:
		Seek VMD to a specified position from which vmd file will be played.
		
Parameters:
		pos:		Position to be seeked. the unit is ms.
		FiFoType:	It mean the resource type, and the VDEV_FIFO_TYPE is 
					defined as
					typedef enum VDEV_FIFO_TYPE
					{
						VDEV_FIFO_EVENT = 0,
						VDEV_FIFO_COMMAND  ,
						VDEV_FIFO_STREAM0  ,
						VDEV_FIFO_STREAM1  ,
						VDEV_FIFO_STREAM2  ,
						VDEV_FIFO_STREAM3  ,
						VDEV_FIFO_MP3      ,
						VDEV_FIFO_ALL	= 0xFF
					}VDEV_FIFO_TYPE;
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT VVmd_Seek(UINT32 dPos, VDEV_FIFO_TYPE FiFoType)
{
	if(!dPos)
		return VRES_SUCCESS;

	g_sdLstMainTime	= (SINT32)dPos;

	if(VRES_SUCCESS != SeekWaveMsg(dPos, &dPos, FiFoType))
	{
		return VRES_ERROR;
	}
	
	g_sdLstMainTime	= (SINT32)dPos;
	
	if(VRES_SUCCESS == SeekMidiMsg(dPos, FiFoType))
	{
		SendMidiMsg();
	}
	SeekMp3Msg(dPos, FiFoType);
	SeekCtrlMsg(dPos, FiFoType);

	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Get a event message. Many event such as midi, wave, mp3 and control 
		message may be included in vmd file, so we must judge which message
		will be performed first.
		
Parameters:
		EventPtr:		Pointer to message structure.
		FifoType:		resource type.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT VVmd_GetEvent(PVM_UNI_MSG EventPtr, VDEV_FIFO_TYPE FiFoType)
{
	VRESULT		Rst	  = VRES_SUCCESS;
	MEDIA_TYPE	MType = NOTHING;

	EventPtr->sdDeltaTime	= 0;
	EventPtr->bMsgID		= 0;
	EventPtr->bChn			= 0;
	EventPtr->dPara1		= 0;
	EventPtr->dPara2		= 0;
	EventPtr->dPara3		= 0;

	MType = Get_Event_Type();

	switch(MType)
	{
		case NOTHING:
			if(!g_bEndFlag)
			{
				EventPtr->sdDeltaTime 	= ((g_sdEndTime-g_sdLstMainTime)>0)?(g_sdEndTime-g_sdLstMainTime):0;
				EventPtr->bMsgID	 	= VMD_MSG_EOS_EVENT;
				EventPtr->bChn		 	= 0;
				EventPtr->dPara1	 	= 0x7f;
				EventPtr->dPara2	 	= 0;
				EventPtr->dPara3	 	= 0;
				g_bEndFlag   = 1;
				return VRES_SUCCESS;
			}
			else
			{
				return VRES_END_FLAG;
			}
			break;
			
		case MIDI:	//1//1
			Rst = Get_Midi_Event(EventPtr);	
			if(Rst == VRES_END_FLAG)
			{
				g_dEndEvent |= 0x01;
			}
			break;
			
		case WAVE:	//2//2
			Rst = Get_Wave_Event(EventPtr);
			if(Rst == VRES_END_FLAG)
			{
				g_dEndEvent |= 0x02;
			}
			break;
			
		case UDSE:	//3//3
			Rst = Get_Udse_Event(EventPtr);	
			if(Rst == VRES_END_FLAG)
			{
				g_dEndEvent |= 0x04;
			}
			break;
			
		case MP3:	//4//4
			Rst = Get_Mp3_Event(EventPtr);
			if(Rst == VRES_END_FLAG)
			{
				g_dEndEvent |= 0x08;
			}
			break;
			
		case LYRIC:	//5
		case PIC:	//6
			break;
			
		case CTRL:	//7
			Rst = Get_Ctrl_Event(EventPtr);	
			if(Rst == VRES_END_FLAG)
			{
				g_dEndEvent |= 0x40;
			}
			break;
			
		case QUE:	//8
			Rst = Get_Que_Event(EventPtr);
			if(Rst == VRES_END_FLAG)
			{
				g_dEndEvent |= 0x80;
			}
			break;
	}

	if(Rst == VRES_ERROR)
		return VRES_ERROR;

	if( (!(g_dEndEvent&0x01)&&(g_VmdInfo.bEnabled&0x01))   ||	//midi
		(!(g_dEndEvent&0x02)&&(g_VmdInfo.bEnabled&0x02))   ||	//wave
		(!(g_dEndEvent&0x04)&&(g_VmdInfo.bEnabled&0x04))   ||	//udse
		(!(g_dEndEvent&0x08)&&(g_VmdInfo.bEnabled&0x20))   ||	//mp3
		(!(g_dEndEvent&0x40)&&(g_VmdInfo.bEnabled&0x0300)) ||	//control		
		(!(g_dEndEvent&0x80))								    //que
	 )
	{
		return VRES_SUCCESS;
	}
	else
	{
		if(!g_bEndFlag)
		{
			EventPtr->sdDeltaTime 	= ((g_sdEndTime-g_sdLstMainTime)>0)?(g_sdEndTime-g_sdLstMainTime):0;;
			EventPtr->bMsgID	 	= VMD_MSG_EOS_EVENT;
			EventPtr->bChn		 	= 0;
			EventPtr->dPara1	 	= 0x7f;
			EventPtr->dPara2	 	= 0;
			EventPtr->dPara3	 	= 0;
			g_bEndFlag   			= 1;
			return VRES_SUCCESS;
		}
		else
		{
			return VRES_END_FLAG;
		}	
	}
}


/*****************************************************************************
Description:
		Get VMD data information, such as the total time of VMD file and so on.
		the struct is defined as
		typedef struct _FILE_INFO
		{
			UINT32 	dVmdFlag;	       //it means whether midi,wave,udse,udi,
									   //comment,mp3,lyric,pic,led,motor is 
									   //included in vmd file from bit0 to bit9.
			UINT32	dTotalTime;		   	
			UINT32	dCurTime;
			UINT16	wVmdFileVersion;
			UINT8	bVmdMidiNum;	   //midi num in vmd file
			UINT8	bVmdWavTrkNum;	   //wave trunk num in vmd file
			UINT8	bVmdWavDatNum[MAX_STREAM_CHN]; 		//wave num in every trunk
			UINT8	bVmdCurWavDatNum[MAX_STREAM_CHN];	//current wave num from 
														//which to play in every 
														//trunk
			UINT8	bVmdMp3Num;		  //mp3 num in vmd file
			UINT8	bVmdCurMp3Num;	  //current mp3 num from which to begin playing
			UINT8	bVmdMp3Key;		  //0->not encrypt, 1->CBC, 2->ECB
			UINT8	*pVmdCmtTitle;	  //title of vmd file
			UINT8	*pVmdCmtAuthor;	  //author of vmd file
			UINT8	*pVmdCmtCpy;      //copyright of vmd file
			UINT32	dVmdCmtTitleLen;  //title length
			UINT32	dVmdCmtAuthorLen; //author length
			UINT32	dVmdCmtCpyLen; 	  //copyright length
			UINT8*	mp3_title;		  
			UINT8*	mp3_artist;		
			UINT8*	mp3_album;		
			UINT8*	mp3_comment;	
			UINT8*	mp3_year;		
			UINT8*	mp3_genre;		
		}FILE_INFO;
		
Parameters:
		pvFileInfo:		Pointer to file information structure.
		FiFoType:		resource type.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT VVmd_GetFileInfo(PVOID pvFileInfo, VDEV_FIFO_TYPE FiFoType)
{
	UINT8	i = 0, *pbBuf = NULL;
	UINT32	dlen = 0;
	
	FILE_INFO	*PFileInfo = (FILE_INFO*)pvFileInfo;

	PFileInfo->dVmdFlag			= (g_VmdInfo.bEnabled & 0x3ff);
	PFileInfo->dTotalTime		= g_VmdInfo.dTotalTime;
	PFileInfo->dCurTime			= (UINT32)g_sdLstMainTime;
	PFileInfo->wVmdFileVersion	= g_VmdInfo.wVmdFileVersion;

	if(g_VmdInfo.bEnabled & 0x01)	//midi
	{
		PFileInfo->bVmdMidiNum		= 1;
		if(g_VmdInfo.bEnabled & 0x0400)
		{
			if(g_VmdInfo.MidiBlk.dFCurOffSet > (g_VmdInfo.MidiBlk.dFBasOffSet + g_VmdInfo.MidiBlk.dBufLen))
				PFileInfo->bVmdMidiNum		= 0;
		}
		else
		{
			if(g_VmdInfo.MidiBlk.pbCurPtr > (g_VmdInfo.MidiBlk.pbBuf + g_VmdInfo.MidiBlk.dBufLen))
				PFileInfo->bVmdMidiNum		= 0;
		}
	}
	else
	{
		PFileInfo->bVmdMidiNum		= 0;
	}
	
	if(g_VmdInfo.bEnabled & 0x02)	//wave
	{
		PFileInfo->bVmdWavTrkNum	= g_VmdInfo.WaveBlk.bEvtTrkNum;
		for(i=0; i<STREAM_TRUNK; i++)
		{
			PFileInfo->bVmdWavDatNum[i] 	= g_VmdInfo.WaveBlk.sbStmTrkFlag[i][2];
			PFileInfo->bVmdCurWavDatNum[i] 	= g_VmdInfo.WaveBlk.sbStmTrkFlag[i][3];			
		}
	}
	else
	{
		PFileInfo->bVmdWavTrkNum	= 0;
		for(i=0; i<STREAM_TRUNK; i++)
		{
			PFileInfo->bVmdWavDatNum[i] = 0;
		}
	}

	if(g_VmdInfo.bEnabled & 0x08)	//udi
	{
		pbBuf = g_VmdInfo.UdiBlk.pbBuf;
		dlen  = (((UINT32)pbBuf[7] << 24) + ((UINT32)pbBuf[6] << 16) + ((UINT32)pbBuf[5] << 8) + (UINT32)pbBuf[4]);
		PFileInfo->pVmdUdiHead	= (pbBuf + 8);
		PFileInfo->dVmdUdiHead	= dlen;
		pbBuf += (8 + dlen);
		dlen  = (((UINT32)pbBuf[7] << 24) + ((UINT32)pbBuf[6] << 16) + ((UINT32)pbBuf[5] << 8) + (UINT32)pbBuf[4]);
		PFileInfo->pVmdUdiData	= (pbBuf + 8);
		PFileInfo->dVmdUdiData	= dlen;
	}

	if(g_VmdInfo.bEnabled & 0x20)	//mp3
	{
		PFileInfo->bVmdMp3Num 		= g_VmdInfo.Mp3Blk.bDatNum;
		PFileInfo->bVmdCurMp3Num	= g_VmdInfo.Mp3Blk.bCurDatNum;
		if(g_VmdInfo.Mp3Blk.DatBuf[0].bETC & 0x10)
			PFileInfo->bVmdMp3Key	= (g_VmdInfo.Mp3Blk.DatBuf[0].bETC & 0x0f);
		else
			PFileInfo->bVmdMp3Key	= 0;
	}
	else
	{
		PFileInfo->bVmdMp3Num		= 0;
		PFileInfo->bVmdCurMp3Num	= 0;
		PFileInfo->bVmdMp3Key		= 0;
	}

	PFileInfo->dVmdCmtTitleLen	= g_VmdInfo.CmtBlk.dTitleLen;
	PFileInfo->dVmdCmtAuthorLen	= g_VmdInfo.CmtBlk.dAuthorLen;
	PFileInfo->dVmdCmtCpyLen	= g_VmdInfo.CmtBlk.dCmtLen;
	
	PFileInfo->pVmdCmtTitle		= g_VmdInfo.CmtBlk.bTitleBuf;
	PFileInfo->pVmdCmtAuthor	= g_VmdInfo.CmtBlk.bAuthorBuf;
	PFileInfo->pVmdCmtCpy		= g_VmdInfo.CmtBlk.bCmtBuf;
			
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Get current playback position of vmd file. 
		
Parameters:
		pdCurTime:		Store the current pos, and the unit is ms.
		FiFoType:		Resource type.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT VVmd_GetCurPos(HUGE UINT32 *pdCurTime, VDEV_FIFO_TYPE FiFoType)
{
	*pdCurTime = (UINT32)g_sdLstMainTime;
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Get wave or mp3 data of vmd file. wave or mp3 may be too large, so it
		can't be loaded to buf once. In fact, it load data when register buf 
		is empty. Register will send a interrupt when it is empty, and player 
		call this function to get data then.
		
Parameters:
		pbOutBuf:		Point to the buffer got from wave or mp3.
		dBufLen:		The length of data excepted to read.
		pdDataSize:		The length of data readed realy.
		FiFoType:		Resource type from which we can know to read wave or mp3.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT VVmd_ReadBlock(HUGE UINT8 *pbOutBuf, UINT32 dBufLen, HUGE UINT32 *pdDataSize, VDEV_FIFO_TYPE FiFoType)
{
	SINT8		sbTrkIdx = 0, sbIdx = 0, sbDatType = -1;
	HUGE UINT8	*pbPtr = NULL;
	UINT32 		dWaveTime = 0, dLeftLen = 0;
	STEAM_BUF	*PStmBuf = NULL;
	MP3_BUF		*PMp3Buf = NULL;

	if(FiFoType == VDEV_FIFO_STREAM0)
	{
		sbDatType = 0;
		sbTrkIdx = g_VmdInfo.WaveBlk.sbStmTrkFlag[0][0];
	}
	else if(FiFoType == VDEV_FIFO_STREAM1)
	{
		sbDatType = 0;
		sbTrkIdx = g_VmdInfo.WaveBlk.sbStmTrkFlag[1][0];
	}
	else if(FiFoType == VDEV_FIFO_STREAM2)
	{
		sbDatType = 0;
		sbTrkIdx = g_VmdInfo.WaveBlk.sbStmTrkFlag[2][0];
	}
	else if(FiFoType == VDEV_FIFO_STREAM3)
	{
		sbDatType = 0;
		sbTrkIdx = g_VmdInfo.WaveBlk.sbStmTrkFlag[3][0];
	}
	else if(FiFoType == VDEV_FIFO_MP3)
	{
		sbDatType = 1;
	}
	else
	{
		return VRES_ERROR;
	}

	if(0 == sbDatType)	//### Read Wave Block =================================
	{
		sbIdx 		= g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][3];	//### cur dat num
		sbIdx		= g_VmdInfo.WaveBlk.bStmTrkIndex[sbTrkIdx][sbIdx];
		if(-1 == sbIdx)
			return VRES_ERROR;

		if(g_VmdInfo.bEnabled & 0x0400)	//### read from file stream -----------
		{
			if(g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFCurOffSet >= (g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFBasOffSet + g_VmdInfo.WaveBlk.DatBuf[sbIdx].dBufLen))
			{
				g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFCurOffSet = g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFBasOffSet;
			}

			dLeftLen = (g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFBasOffSet + g_VmdInfo.WaveBlk.DatBuf[sbIdx].dBufLen - g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFCurOffSet);
			if(dLeftLen > dBufLen) //Have enough data
			{
				Fstm_ReadBuf(pbOutBuf, dBufLen, pdDataSize, g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFCurOffSet);
				g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFCurOffSet += (*pdDataSize);
				
				dWaveTime = (UINT32)(dBufLen*1000)/
					((g_VmdInfo.WaveBlk.DatBuf[sbIdx].bChn * g_VmdInfo.WaveBlk.DatBuf[sbIdx].bBitsPerSmpl * g_VmdInfo.WaveBlk.DatBuf[sbIdx].wSmplRate)>>3);
				g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][4] += (UINT8)dWaveTime;	//must modify data type later

				if(g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFCurOffSet >= (g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFBasOffSet + g_VmdInfo.WaveBlk.DatBuf[sbIdx].dBufLen))
				{
					if(g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][3] < g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][2])
					{			
						g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][3]++;	//###
					}
					return VRES_STREAM_DATA_END;
				}

				return VRES_SUCCESS;
			}
			else
			{
				Fstm_ReadBuf(pbOutBuf, dLeftLen, pdDataSize, g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFCurOffSet);
				*pdDataSize = dLeftLen;	//###
				g_VmdInfo.WaveBlk.DatBuf[sbIdx].dFCurOffSet += (*pdDataSize);
	
				dWaveTime = (UINT32)((*pdDataSize)*1000)/
					((g_VmdInfo.WaveBlk.DatBuf[sbIdx].bChn * g_VmdInfo.WaveBlk.DatBuf[sbIdx].bBitsPerSmpl * g_VmdInfo.WaveBlk.DatBuf[sbIdx].wSmplRate)>>3);
				g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][4] += (UINT8)dWaveTime;	//must modify data type later

				if(g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][3] < g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][2])
				{			
					g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][3]++;	//###
				}

				return VRES_STREAM_DATA_END;
			}
		}
		else	//### read from buffer ----------------------------------------
		{
			PStmBuf = &g_VmdInfo.WaveBlk.DatBuf[sbIdx];
			
			if(PStmBuf->dOffSet >= PStmBuf->dBufLen)
			{
				PStmBuf->dOffSet = 0;
			}
	
			if (PStmBuf->pbBuf == NULL)
				return VRES_BUFFER_NULL;

			pbPtr = PStmBuf->pbBuf + PStmBuf->dOffSet;
			if((PStmBuf->dBufLen - PStmBuf->dOffSet)>dBufLen) //Have enough data
			{
				Vmd_MemCpy((HUGE UINT8*)pbOutBuf, (HUGE UINT8*)pbPtr, dBufLen);
				*pdDataSize = dBufLen;
				PStmBuf->dOffSet += (*pdDataSize);
				
				dWaveTime = (UINT32)(dBufLen*1000)/
					((PStmBuf->bChn*PStmBuf->bBitsPerSmpl*PStmBuf->wSmplRate)>>3);
				g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][4] += (UINT8)dWaveTime;	//must modify data type later

				if(PStmBuf->dOffSet >= PStmBuf->dBufLen)
				{
					if(g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][3] < g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][2])
					{			
						g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][3]++;	//###
					}
					return VRES_STREAM_DATA_END;
				}

				return VRES_SUCCESS;
			}
			else
			{
				Vmd_MemCpy((HUGE UINT8*)pbOutBuf, (HUGE UINT8*)pbPtr, (PStmBuf->dBufLen - PStmBuf->dOffSet));
				*pdDataSize = (PStmBuf->dBufLen - PStmBuf->dOffSet);
				PStmBuf->dOffSet += (*pdDataSize);
	
				dWaveTime = (UINT32)((*pdDataSize)*1000)/
					((PStmBuf->bChn*PStmBuf->bBitsPerSmpl*PStmBuf->wSmplRate)>>3);
				g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][4] += (UINT8)dWaveTime;	//must modify data type later

				if(g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][3] < g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][2])
				{			
					g_VmdInfo.WaveBlk.sbStmTrkFlag[sbTrkIdx][3]++;	//###
				}

				return VRES_STREAM_DATA_END;
			}
		}
	}
	else if(1 == sbDatType)	//### Read Mp3 Block ==============================
	{
		sbIdx 		= g_VmdInfo.Mp3Blk.bCurDatNum;	//### cur dat num
		sbIdx		= g_VmdInfo.Mp3Blk.bMp3TrkIndex[sbIdx];
		if(-1 == sbIdx)
			return VRES_ERROR;

		if(g_VmdInfo.bEnabled & 0x0400)	//### read from file stream -----------
		{
			if(g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFCurOffSet >= (g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFBasOffSet + g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dBufLen))
			{
				g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFCurOffSet = g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFBasOffSet;
			}

			dLeftLen = (g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFBasOffSet + g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dBufLen - g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFCurOffSet);
			if(dLeftLen > dBufLen) //Have enough data
			{
				Fstm_ReadBuf(pbOutBuf, dBufLen, pdDataSize, g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFCurOffSet);
				g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFCurOffSet += (*pdDataSize);

				if(g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFCurOffSet >= (g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFBasOffSet + g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dBufLen))
				{
					if(g_VmdInfo.Mp3Blk.bCurDatNum	< g_VmdInfo.Mp3Blk.bDatNum)
					{			
						g_VmdInfo.Mp3Blk.bCurDatNum++;	//###
					}
					return VRES_STREAM_DATA_END;
				}
			
				return VRES_SUCCESS;
			}
			else
			{
				Fstm_ReadBuf(pbOutBuf, dLeftLen, pdDataSize, g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFCurOffSet);
				*pdDataSize = dLeftLen;	//###
				g_VmdInfo.Mp3Blk.DatBuf[sbIdx].dFCurOffSet += (*pdDataSize);
				
				if(g_VmdInfo.Mp3Blk.bCurDatNum	< g_VmdInfo.Mp3Blk.bDatNum)
				{			
					g_VmdInfo.Mp3Blk.bCurDatNum++;	//###
				}
		
				return VRES_STREAM_DATA_END;
			}
		}
		else	//### read from buffer ----------------------------------------
		{
			PMp3Buf = &g_VmdInfo.Mp3Blk.DatBuf[sbIdx];
			if(PMp3Buf->dOffSet >= PMp3Buf->dBufLen)
			{
				PMp3Buf->dOffSet = 0;
			}

			if (PMp3Buf->pbBuf == NULL)
				return VRES_BUFFER_NULL;

			pbPtr = PMp3Buf->pbBuf + PMp3Buf->dOffSet;
			if((PMp3Buf->dBufLen - PMp3Buf->dOffSet)>dBufLen) //Have enough data
			{
				Vmd_MemCpy((HUGE UINT8*)pbOutBuf, (HUGE UINT8*)pbPtr, dBufLen);
				*pdDataSize = dBufLen;
				PMp3Buf->dOffSet += (*pdDataSize);

				if(PMp3Buf->dOffSet >= PMp3Buf->dBufLen)
				{
					if(g_VmdInfo.Mp3Blk.bCurDatNum	< g_VmdInfo.Mp3Blk.bDatNum)
					{			
						g_VmdInfo.Mp3Blk.bCurDatNum++;	//###
					}
					return VRES_STREAM_DATA_END;
				}
			
				return VRES_SUCCESS;
			}
			else
			{
				Vmd_MemCpy((HUGE UINT8*)pbOutBuf, (HUGE UINT8*)pbPtr, (PMp3Buf->dBufLen - PMp3Buf->dOffSet));
				*pdDataSize = (PMp3Buf->dBufLen - PMp3Buf->dOffSet);
				PMp3Buf->dOffSet += (*pdDataSize);
				
				if(g_VmdInfo.Mp3Blk.bCurDatNum	< g_VmdInfo.Mp3Blk.bDatNum)
				{			
					g_VmdInfo.Mp3Blk.bCurDatNum++;	//###
				}
		
				return VRES_STREAM_DATA_END;
			}
		}
	}

	return VRES_ERROR;
}


/*****************************************************************************
Description:
		Unload the VMD Parser, many variables may be released here.
		
Parameters:
		FiFoType:		Resource type.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT VVmd_Unload(VDEV_FIFO_TYPE FiFoType)
{
	UINT8 i = 0;

	g_VmdInfo.MidiBlk.pbBuf		= NULL;
	g_VmdInfo.MidiBlk.pbCurPtr	= NULL;
	
	for(i=0; i<STREAM_DAT_NUM; i++)
	{
		g_VmdInfo.WaveBlk.DatBuf[i].pbBuf	= NULL;
	}
	g_VmdInfo.WaveBlk.pbEvtBuf		= NULL;
	g_VmdInfo.WaveBlk.pbCurEvtPtr	= NULL;
	
	g_VmdInfo.UdseBlk.pbEvtBuf		= NULL;
	g_VmdInfo.UdseBlk.pbDatBuf		= NULL;
	g_VmdInfo.UdseBlk.pbCurEvtPtr	= NULL;
	
	g_VmdInfo.UdiBlk.pbBuf			= NULL;
	
	g_VmdInfo.Mp3Blk.pbEvtBuf		= NULL;
	g_VmdInfo.Mp3Blk.pbCurEvtPtr	= NULL;
	for(i=0; i<MP3_DAT_NUM; i++)
	{
		g_VmdInfo.Mp3Blk.DatBuf[i].pbBuf	= NULL;
	}
	
	g_VmdInfo.LyricBlk.pbBuf		= NULL;
	g_VmdInfo.LyricBlk.pbCurPtr		= NULL;
	
	g_VmdInfo.PicBlk.pbEvtBub		= NULL;
	g_VmdInfo.PicBlk.pbCurEvtPtr	= NULL;
	for(i=0; i<PIC_DAT_NUM; i++)
	{
		g_VmdInfo.PicBlk.DatBuf[i].pbBuf	= NULL;
		g_VmdInfo.PicBlk.DatBuf[i].pbCurBuf	= NULL;
	}
	
	g_VmdInfo.CtrlBlk.pbBuf			= NULL;
	g_VmdInfo.CtrlBlk.pbCurPtr		= NULL;
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Close the VMD parser.
		
Parameters:
		void.
				
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT VVmd_End(void)
{
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Configure the Vmd parser.
		
Parameters:
		dVal:		Conifg Item.
		dDat:		Config Data.
		bConfigID:	Config Catalog ID.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT VVmd_Configure	(UINT32 dVal, UINT32 dDat, UINT8 bConfigID)
{
		
	UINT8 bIdx = 0xff;

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
	case VM_PARSER_SET_FIFO:	
		g_VmdInfo.WaveBlk.sbStmTrkFlag[bIdx][0] 	= (SINT8)dDat;
		g_VmdInfo.WaveBlk.sbStmTrkFlag[dDat][1] 	= (SINT8)bIdx;
		break;
	default:
		break;
	}
	
	return VRES_SUCCESS;//*/
}


/*****************************************************************************
Description:
		Reset the Vmd parser.
		
Parameters:
		FiFoType:		Resourec type.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT VVmd_Reset(VDEV_FIFO_TYPE FiFoType)
{
	UINT8	i = 0;
	
	g_bEndFlag			= 0;
	g_dEndEvent			= 0;
	g_bLstMsgCode		= 0;
	g_bLstCtrlCode		= 0;

	g_sdLstMainTime		= 0;
	g_sdLstMidiTime		= 0;
	g_sdLstWaveTime		= 0;
	g_sdLstUdseTime		= 0;
	g_sdLstMp3Time		= 0;
	g_sdLstLyricTime	= 0;
	g_sdLstPicTime		= 0;
	g_sdLstCtrlTime		= 0;
	g_sdEndTime			= 0;

	for(i=0; i<MAX_MIDI_CHN; i++)
	{
		g_wRPN[i] = 0x7f7f;
	}

	for(i=1; i<WAIT_QUE_LEN; i++)
	{
		g_WaitingMsgQue[i-1].pNext 	= &g_WaitingMsgQue[i];
		g_WaitingMsgQue[i].dPara1 	= 0;
		g_WaitingMsgQue[i].dPara2 	= 0;
		g_WaitingMsgQue[i].dPara3	= 0;
	}
	g_EmptyHdr.pNext 	= g_WaitingMsgQue;
	g_WaitingMsgQue[WAIT_QUE_LEN-1].pNext = NULL;
	g_WaitingHdr.pNext 	= NULL;
	g_WaitingHdr.sdTime = 0;

	g_VmdInfo.MidiBlk.pbCurPtr		= g_VmdInfo.MidiBlk.pbBuf;
	g_VmdInfo.MidiBlk.dFCurOffSet	= g_VmdInfo.MidiBlk.dFBasOffSet;
	if(g_VmdInfo.bEnabled & 0x0400)
	{
		g_VmdInfo.MidiBlk.dOffSet	= g_VmdInfo.MidiBlk.dFBasOffSet;
	}
	else
	{
		g_VmdInfo.MidiBlk.dOffSet	= 0;
	}

	for(i=0; i<STREAM_TRUNK; i++)
	{
		g_VmdInfo.WaveBlk.sbStmTrkFlag[i][3] 	= 0;	//current data num	
		g_VmdInfo.WaveBlk.sbStmTrkFlag[i][4] 	= 0;	//dCurTime
		g_VmdInfo.WaveBlk.sbStmTrkFlag[i][5] 	= 0;	//ReadBlock Flag		
	}
	for(i=0; i<STREAM_DAT_NUM; i++)
	{
		if(g_VmdInfo.bEnabled & 0x0400)
		{
			g_VmdInfo.WaveBlk.DatBuf[i].dOffSet = g_VmdInfo.WaveBlk.DatBuf[i].dFBasOffSet;
		}
		else
		{
			g_VmdInfo.WaveBlk.DatBuf[i].dOffSet		= 0;
		}
		g_VmdInfo.WaveBlk.DatBuf[i].dFCurOffSet	= g_VmdInfo.WaveBlk.DatBuf[i].dFBasOffSet;
	}
	g_VmdInfo.WaveBlk.pbCurEvtPtr	= g_VmdInfo.WaveBlk.pbEvtBuf;
	g_VmdInfo.WaveBlk.dFCurOffSet	= g_VmdInfo.WaveBlk.dFBasOffSet;
	if(g_VmdInfo.bEnabled & 0x0400)
	{	
		g_VmdInfo.WaveBlk.dOffSet   = g_VmdInfo.WaveBlk.dFBasOffSet;
	}
	else
	{
		g_VmdInfo.WaveBlk.dOffSet	= 0;
	}
	
	g_VmdInfo.UdseBlk.pbCurEvtPtr		= g_VmdInfo.UdseBlk.pbEvtBuf;
	g_VmdInfo.UdseBlk.dFCurEvtOffSet	= g_VmdInfo.UdseBlk.dFBasEvtOffSet;
	g_VmdInfo.UdseBlk.dFCurDatOffSet	= g_VmdInfo.UdseBlk.dFBasDatOffSet;
	if(g_VmdInfo.bEnabled & 0x0400)
	{
		g_VmdInfo.UdseBlk.dOffSet 		= g_VmdInfo.UdseBlk.dFBasEvtOffSet;
	}
	else
	{
		g_VmdInfo.UdseBlk.dOffSet 		= 0;
	}

	g_VmdInfo.Mp3Blk.pbCurEvtPtr	= g_VmdInfo.Mp3Blk.pbEvtBuf;
	g_VmdInfo.Mp3Blk.bCurDatNum		= 0;
	g_VmdInfo.Mp3Blk.dFCurOffSet	= g_VmdInfo.Mp3Blk.dFBasOffSet;
	if(g_VmdInfo.bEnabled & 0x0400)
	{
		g_VmdInfo.Mp3Blk.dOffSet	= g_VmdInfo.Mp3Blk.dFBasOffSet;
	}
	else
	{
		g_VmdInfo.Mp3Blk.dOffSet	= 0;
	}
	for(i=0; i<MP3_DAT_NUM; i++)
	{
		g_VmdInfo.Mp3Blk.DatBuf[i].dOffSet		= 0;
		g_VmdInfo.Mp3Blk.DatBuf[i].dFCurOffSet	= g_VmdInfo.Mp3Blk.DatBuf[i].dFBasOffSet;
	}
	
	/*g_VmdInfo.LyricBlk.pbCurPtr		= g_VmdInfo.LyricBlk.pbBuf;
	g_VmdInfo.LyricBlk.dFCurOffSet	= g_VmdInfo.LyricBlk.dFBasOffSet;
	if(g_VmdInfo.bEnabled & 0x0400)
	{
		g_VmdInfo.LyricBlk.dOffSet	= g_VmdInfo.LyricBlk.dFBasOffSet;
	}
	else
	{
		g_VmdInfo.LyricBlk.dOffSet	= 0;
	}

	g_VmdInfo.PicBlk.pbCurEvtPtr	= g_VmdInfo.PicBlk.pbEvtBub;
	g_VmdInfo.PicBlk.dFCurOffSet	= g_VmdInfo.PicBlk.dFBasOffSet;
	if(g_VmdInfo.bEnabled & 0x0400)
	{
		g_VmdInfo.PicBlk.dOffSet 	= g_VmdInfo.PicBlk.dFBasOffSet;
	}
	else
	{
		g_VmdInfo.PicBlk.dOffSet	= 0;
	}
	for(i=0; i<PIC_DAT_NUM; i++)
	{
		g_VmdInfo.PicBlk.DatBuf[i].dFCurOffSet	= g_VmdInfo.PicBlk.DatBuf[i].dFBasOffSet;
		g_VmdInfo.PicBlk.DatBuf[i].pbCurBuf		= g_VmdInfo.PicBlk.DatBuf[i].pbBuf;
	}*/

	g_VmdInfo.CtrlBlk.pbCurPtr		= g_VmdInfo.CtrlBlk.pbBuf;
	g_VmdInfo.CtrlBlk.dFCurOffSet	= g_VmdInfo.CtrlBlk.dFBasOffSet;
	if(g_VmdInfo.bEnabled & 0x0400)
	{
		g_VmdInfo.CtrlBlk.dOffSet 	= g_VmdInfo.CtrlBlk.dFBasOffSet;
	}
	else
	{
		g_VmdInfo.CtrlBlk.dOffSet 	= 0;
	}

	g_WaitingHdr.pNext	= NULL;	//###

	return VRES_SUCCESS;
}

/*****************************************************************************
Description:
		Get a empty node from the message list. It get a node from the waiting
		list if the message list is empty. It return NULL too if the node from
		waiting list is VMD_MSG_STREAM_OFF or VMD_MSG_MP3_OFF.
		
Parameters:
		void.
		
Returns:
		Pointer to the empty node in the message list.
		
******************************************************************************/
PMSGQUEUE GetMsgFromQue(void)
{
	PMSGQUEUE pQue = g_EmptyHdr.pNext;
	if(pQue!=NULL)
	{
		g_EmptyHdr.pNext = pQue->pNext;	
		return (pQue);
	}
	else
	{
		//return NULL;

		//get resource from waiting que if there isn't resource in empty que
		pQue		= g_WaitingHdr.pNext;
		if(pQue == NULL)
		{
			return NULL;
		}
		
		if(pQue->bID	== VMD_MSG_NOTE_OFF ||
		   pQue->bID	== VMD_MSG_STREAM_OFF ||
		   pQue->bID	== VMD_MSG_MP3_OFF)
		{
			return NULL;
		}
		
		g_WaitingHdr.pNext	= pQue->pNext;
		
		return (pQue);
	}
}


/*****************************************************************************
Description:
		Get a message from the waiting message list.
		
Parameters:
		p:		Pointer to message structure.
		
Returns:
		void.
		
******************************************************************************/
void GetWaitingEvent(PVM_UNI_MSG p)
{
	PMSGQUEUE 	pQue;

	pQue		= g_WaitingHdr.pNext;
	p->bMsgID	= pQue->bID;
	p->bChn		= pQue->bChn;
	p->dPara1	= pQue->dPara1;
	p->dPara2	= pQue->dPara2;
	p->dPara3	= pQue->dPara3;	
	
	p->sdDeltaTime 	= pQue->sdTime - g_sdLstMainTime; 	//###***###

	g_WaitingHdr.pNext	= pQue->pNext;
	pQue->pNext			= g_EmptyHdr.pNext;	
	g_EmptyHdr.pNext	= pQue;	

	g_sdLstMainTime		= pQue->sdTime;					//###	

	return;
}


/*****************************************************************************
Description:
		Add a message to the waiting message list. It compares the time with
		all of the node in the waiting message list, so it can be inserted to 
		the proptery position. The waiting message list is sorted by time from
		little to large.
		
Parameters:
		ptr:		Pointer to the message list.
		
Returns:
		void.
		
******************************************************************************/
void SendMsgToQue(PMSGQUEUE ptr)
{
	PMSGQUEUE pCur, pNxt;
	pCur = &g_WaitingHdr;

	while(pCur->pNext != NULL)
	{
		pNxt = pCur->pNext;
		
		if(ptr->sdTime < pNxt->sdTime)	
			break;
		pCur = pNxt;
	}

	if(pCur->pNext == NULL)
	{
		pCur->pNext = ptr;
		ptr->pNext 	= NULL;
	}
	else
	{
		pCur->pNext = ptr;
		ptr->pNext 	= pNxt;
	}

}


/*****************************************************************************
Description:
		Scan the MIDI block of VMD.
		
Parameters:
		pBuf:		Pointer to MIDI block.
		len:		Size of MIDI block.
		
Returns:
		Total time of the MIDI block.
		
******************************************************************************/
UINT32 ScanMIDIBlk(HUGE UINT8* pbBuf, UINT32 dLen)
{
	HUGE UINT8* pbPtr = NULL;
	UINT8  bMsgID = 0, bCounter = 0, bTimeCode = 0, bCtrlID = 0, bLstMsgCode = 0, bLstCtrlCode = 0;
	UINT32 dTime = 0, dDeltaTime = 0;
	
	pbPtr	= pbBuf;
	dTime	= 0;

	while(pbPtr < pbBuf+dLen)
	{
		bTimeCode	= *pbPtr++;
		dDeltaTime	= bTimeCode&0x7f;
		bCounter = 0;
		while(bTimeCode&0x80)
		{
			bCounter++;
			bTimeCode	= *pbPtr++;
			dDeltaTime += (UINT16)(((UINT32)bTimeCode&0x7f)<<(7*bCounter));
		}
		dTime += dDeltaTime;
		
		bMsgID = *pbPtr++;

		if((bMsgID&0x80)==0)
		{
			if(bLstMsgCode&0xf0==0xB0)
				bCtrlID = bLstCtrlCode;

			bMsgID = bLstMsgCode;
			pbPtr--; 
		}

		if((bMsgID&0xf0)==0x80)	//(B0) control message in midi
		{
			bCtrlID = bMsgID;
			bMsgID  = bLstMsgCode;
			pbPtr--;
		}
		bLstMsgCode = bMsgID;

		switch(bMsgID&0xf0)
		{
		case 0x90:
			pbPtr	 += 2;
			bTimeCode = *pbPtr++;
			while(bTimeCode&0x80)
				bTimeCode = *pbPtr++;
			break;

		case 0xC0:
			pbPtr++;
			break;

		case 0xD0:
			pbPtr++;
			break;

		case 0xE0:
			pbPtr += 2;
			break;
		case 0xB0:
			bCtrlID = *pbPtr++;
			if((bCtrlID&0x80)==0)
			{
				bCtrlID = bLstCtrlCode;
				pbPtr--;
			}
			bLstCtrlCode = bCtrlID;
			pbPtr++;
			break;
		}
	}

	return dTime;
}


/*****************************************************************************
Description:
		Scan the stream block of VMD.
		
Parameters:
		ptr:		Pointer to stream block.
		
Returns:
		Total time of the stream block.
		
******************************************************************************/
UINT32 ScanStreamBlk(HUGE UINT8 *pbPtr)
{
	HUGE UINT8* pbTrk = NULL, *pbEvent = NULL;
	UINT8		bCounter = 0, bWaveID = 0, bTimeCode = 0, bEventNum = 0;
	SINT8		sbDatNum = 0;
	UINT16		wTrkNum = 0;
	UINT32		dSize = 0, dFenzi = 0, dFenmu = 0, dBlkTime = 0, dDeltaTime = 0, dTotalTime = 0, dDuration = 0, dTrkID = 0, dTrkSize = 0, dCutTime = 0, dAddTime = 0;

	wTrkNum		= (((UINT16)pbPtr[11])<<8) + (UINT16)pbPtr[10];
	
	pbTrk		= pbPtr+12;
	bWaveID		= 0;
	bEventNum	= 0;
	sbDatNum	= 0;

	dCutTime	= 0;
	dAddTime	= 0;

	for(bCounter=0; bCounter<wTrkNum; bCounter++)
	{
		dTrkID		= (((UINT32)pbTrk[3])<<24) + (((UINT32)pbTrk[2])<<16) + (((UINT32)pbTrk[1])<<8) + (UINT32)pbTrk[0];
		dTrkSize	= (((UINT32)pbTrk[7])<<24) + (((UINT32)pbTrk[6])<<16) + (((UINT32)pbTrk[5])<<8) + (UINT32)pbTrk[4];

		if(dTrkID==ETrk)
		{
			pbEvent = pbTrk;
			g_VmdInfo.WaveBlk.pbEvtBuf		= pbEvent + 8;
			g_VmdInfo.WaveBlk.pbCurEvtPtr	= pbEvent + 8;
			g_VmdInfo.WaveBlk.dEvtBufLen	= dTrkSize;
		}
		else
		{
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].bFormat		= *(pbTrk+8);
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].bChn			= *(pbTrk+9);
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].bBitsPerSmpl	= *(pbTrk+10);
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].bSigned		= *(pbTrk+11);
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].wSmplRate		= (((UINT16)pbTrk[13])<<8) + (UINT16)pbTrk[12];
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].pbBuf			= pbTrk+14;
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].dBufLen		= dTrkSize-6;
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].dOffSet		= 0;
			dSize = (dTrkSize-6) * ((1==g_VmdInfo.WaveBlk.DatBuf[bWaveID].bFormat)?4:1);
			if(dSize < WAVE_MAX_LEN)
			{
				dFenzi	= dSize * 8000;
				/* dFenmu	= (*(pbTrk+9)) * (*(pbTrk+10)) * ( (((UINT16)pbTrk[13])<<8) + (UINT16)pbTrk[12] ); */
				/* must be 16 bitsmple and mono */
				dFenmu	= 16 * 1 * ( (((UINT16)pbTrk[13])<<8) + (UINT16)pbTrk[12] );
			}
			else if((dTrkSize-6) < WAVE_MAX_LEN1)
			{
				dFenzi	= dSize * 500;
				dFenmu	= ( (((UINT16)pbTrk[13])<<8) + (UINT16)pbTrk[12] );
			}
			else
			{
				dFenzi	= dSize; 
				dFenmu	= ( (((UINT16)pbTrk[13])<<8) + (UINT16)pbTrk[12] );
				dFenmu /= 500;
			}
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].dTotalTime	= dFenzi/dFenmu;

			bWaveID++;
		}
	
		pbTrk += (dTrkSize + 8);
	}

	dTrkSize = (((UINT32)pbEvent[7])<<24) + (((UINT32)pbEvent[6])<<16) + (((UINT32)pbEvent[5])<<8) + (UINT32)pbEvent[4];

	pbEvent		+= 8;
	dDeltaTime	=  0;
	dTotalTime	=  0;
	dBlkTime	=  0;
	pbTrk		=  pbEvent;

	while(pbEvent < pbTrk+dTrkSize)
	{
		bTimeCode  = *pbEvent++;
		dDeltaTime = bTimeCode&0x7f;
		bCounter=0;
		while(bTimeCode&0x80)
		{
			bCounter++;
			bTimeCode = *pbEvent++;
			dDeltaTime += (UINT16)(((UINT32)bTimeCode&0x7f)<<(7*bCounter));
		}
		
		switch(*pbEvent++)
		{
		case 0xA0:
			bWaveID = *pbEvent++;
			bEventNum |= 0x01;
			sbDatNum = g_VmdInfo.WaveBlk.sbStmTrkFlag[0][2]++;
			g_VmdInfo.WaveBlk.bStmTrkIndex[0][sbDatNum] 	= bWaveID;
			break;
		case 0xA1:
			bWaveID = *pbEvent++;
			bEventNum |= 0x02;
			sbDatNum = g_VmdInfo.WaveBlk.sbStmTrkFlag[1][2]++;
			g_VmdInfo.WaveBlk.bStmTrkIndex[1][sbDatNum] 	= bWaveID;	
			break;
		case 0xA2:
			bWaveID = *pbEvent++;
			bEventNum |= 0x04;
			sbDatNum = g_VmdInfo.WaveBlk.sbStmTrkFlag[2][2]++;
			g_VmdInfo.WaveBlk.bStmTrkIndex[2][sbDatNum] 	= bWaveID;	
			break;
		case 0xA3:
			bWaveID = *pbEvent++;
			bEventNum |= 0x08;
			sbDatNum = g_VmdInfo.WaveBlk.sbStmTrkFlag[3][2]++;
			g_VmdInfo.WaveBlk.bStmTrkIndex[3][sbDatNum] 	= bWaveID;	
			break;
		default:
			bWaveID = 0xff;
			break;
		}

		if(g_VmdInfo.wVmdFileVersion >= VMD_STONE_VERSION)
		{
			pbEvent++;	/* vol */
			pbEvent++;	/* pan */
		}

		if(bWaveID != 0xff)	//##########
		{
			dSize = g_VmdInfo.WaveBlk.DatBuf[bWaveID].dBufLen * ((1==g_VmdInfo.WaveBlk.DatBuf[bWaveID].bFormat)?4:1);
			if(dSize < WAVE_MAX_LEN)
			{
				dFenzi = dSize * 8000;
				/* dFenmu = g_VmdInfo.WaveBlk.DatBuf[bWaveID].bBitsPerSmpl * g_VmdInfo.WaveBlk.DatBuf[bWaveID].bChn * g_VmdInfo.WaveBlk.DatBuf[bWaveID].wSmplRate; */
				/* must be 16 bitsmpl and mono */
				dFenmu = 16 * 1 * g_VmdInfo.WaveBlk.DatBuf[bWaveID].wSmplRate;
			}
			else if(dSize < WAVE_MAX_LEN1)
			{
				dFenzi = dSize * 500;
				dFenmu = g_VmdInfo.WaveBlk.DatBuf[bWaveID].wSmplRate;
			}
			else
			{
				dFenzi = dSize; 
				dFenmu = g_VmdInfo.WaveBlk.DatBuf[bWaveID].wSmplRate;
				dFenmu /= 500;
			}
			dDuration = dFenzi/dFenmu;
			
			dTotalTime  += dDeltaTime;
			dBlkTime	 = dBlkTime<(dTotalTime +dDuration)?(dTotalTime +dDuration):dBlkTime;
		}
	}

	g_VmdInfo.WaveBlk.bEvtTrkNum 	= 0;
	if(bEventNum & 0x01)
		g_VmdInfo.WaveBlk.bEvtTrkNum++;
	if(bEventNum & 0x02)
		g_VmdInfo.WaveBlk.bEvtTrkNum++;
	if(bEventNum & 0x04)
		g_VmdInfo.WaveBlk.bEvtTrkNum++;
	if(bEventNum & 0x08)
		g_VmdInfo.WaveBlk.bEvtTrkNum++;

	return dBlkTime;
}


/*****************************************************************************
Description:
		Scan the mp3 block of VMD.
		
Parameters:
		ptr:		Pointer to mp3 block.
		
Returns:
		Total time of the mp3 block.
		
******************************************************************************/
UINT32 ScanMp3Blk(HUGE UINT8 *pbPtr)
{
	HUGE UINT8* pbTrk = NULL, *pbEvent = NULL;
	UINT8		bCounter = 0, bMp3ID = 0, bTimeCode = 0;
	UINT16		wTrkNum = 0;
	UINT32		dBlkTime = 0, dDeltaTime = 0, dTotalTime = 0, dDuration = 0, dTrkID = 0, dTrkSize = 0, dCutTime = 0, dAddTime = 0, dHeader = 0;

	wTrkNum		= (((UINT16)pbPtr[11])<<8) + (UINT16)pbPtr[10];
	
	pbTrk		= pbPtr+12;
	bMp3ID		= 0;

	dCutTime	= 0;
	dAddTime	= 0;

	for(bCounter=0; bCounter<wTrkNum; bCounter++)
	{
		dTrkID		= (((UINT32)pbTrk[3])<<24) + (((UINT32)pbTrk[2])<<16) + (((UINT32)pbTrk[1])<<8) + (UINT32)pbTrk[0];
		dTrkSize	= (((UINT32)pbTrk[7])<<24) + (((UINT32)pbTrk[6])<<16) + (((UINT32)pbTrk[5])<<8) + (UINT32)pbTrk[4];

		if(dTrkID==ETrk)
		{
			pbEvent = pbTrk;
			g_VmdInfo.Mp3Blk.pbEvtBuf		= pbEvent + 8;
			g_VmdInfo.Mp3Blk.pbCurEvtPtr	= pbEvent + 8;
			g_VmdInfo.Mp3Blk.dEvtBufLen		= dTrkSize;
		}
		else
		{
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].bETC		= *(pbTrk+8);
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].wSmplRate	= ( ((UINT16)(*(pbTrk+10))) <<8) + (UINT16)(*(pbTrk+9));
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].wBitRate	= ( ((UINT16)(*(pbTrk+12))) <<8) + (UINT16)(*(pbTrk+11));
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].bSigned		= *(pbTrk+13);
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].pbBuf		= pbTrk+14;
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dBufLen		= dTrkSize-6;
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dOffSet		= 0;			
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dTotalTime	= 0;

			dHeader = (((UINT32)(*(pbTrk+14)))<<24) + (((UINT32)(*(pbTrk+15)))<<16) + (((UINT32)(*(pbTrk+16)))<<8) + ((UINT32)(*(pbTrk+17)));
			CheckMp3Head(dHeader, dTrkSize-6, &g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dTotalTime);

			bMp3ID++;
		}
	
		pbTrk += (dTrkSize + 8);
	}

	dTrkSize = (((UINT32)pbEvent[7])<<24) + (((UINT32)pbEvent[6])<<16) + (((UINT32)pbEvent[5])<<8) + (UINT32)pbEvent[4];

	pbEvent		+= 8;
	dDeltaTime	=  0;
	dTotalTime	=  0;
	dBlkTime	=  0;
	pbTrk		=  pbEvent;

	while(pbEvent < pbTrk+dTrkSize)
	{
		bTimeCode  = *pbEvent++;
		dDeltaTime = bTimeCode&0x7f;
		bCounter=0;
		while(bTimeCode&0x80)
		{
			bCounter++;
			bTimeCode = *pbEvent++;
			dDeltaTime += (UINT16)(((UINT32)bTimeCode&0x7f)<<(7*bCounter));
		}
		
		switch(*pbEvent++)
		{
		case 0xA0:
			bMp3ID 	 = *pbEvent++;
			g_VmdInfo.Mp3Blk.bMp3TrkIndex[g_VmdInfo.Mp3Blk.bDatNum] = bMp3ID;
			g_VmdInfo.Mp3Blk.bDatNum++;
			break;
		default:
			bMp3ID = 0xff;
			break;
		}

		if(g_VmdInfo.wVmdFileVersion >= VMD_STONE_VERSION)
		{
			pbEvent++;	//vol
			pbEvent++;	//pan
		}

		if(bMp3ID != 0xff)	//##########
		{
			dDuration = g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dTotalTime;
			
			dTotalTime  += dDeltaTime;
			dBlkTime	 = dBlkTime<(dTotalTime +dDuration)?(dTotalTime +dDuration):dBlkTime;
		}
	}

	return dBlkTime;
}


/*****************************************************************************
Description:
		Scan the control block of VMD.
		
Parameters:
		ptr:		Pointer to control block.
		
Returns:
		Total time of the control block.
		
******************************************************************************/
UINT32 ScanCtrlBlk(HUGE UINT8 *pbBuf, UINT32 dLen)
{
	HUGE UINT8 *pbPtr = pbBuf;
	UINT8	bCtrlID  = 0, bCtrlLen = 0, bCtrlDat = 0;
	UINT8	bTimeCode  = 0, bCounter = 0;
	UINT32	dDeltaTime = 0;

	while(pbPtr < pbBuf+dLen)
	{
		bTimeCode	= *pbPtr++;
		dDeltaTime	= bTimeCode&0x7f;
		bCounter=0;
		while(bTimeCode&0x80)
		{
			bCounter++;
			bTimeCode	= *pbPtr++;
			dDeltaTime += (UINT16)(((UINT32)bTimeCode&0x7f)<<(7*bCounter));
		}

		pbPtr++;	//skip message id "FF"
				
		bCtrlID 	= *pbPtr++;
		bCtrlLen	= *pbPtr++;
		if(bCtrlID == 8 || bCtrlID == 9)
		{
			bCtrlDat = *pbPtr;
			if(bCtrlDat&0x08)
				g_VmdInfo.bEnabled		|= 0x0100;	//led
			if(bCtrlDat&0x80)
				g_VmdInfo.bEnabled		|= 0x0200;	//motor
		}

		pbPtr += bCtrlLen;
	}
	
	return 0;
}


/*****************************************************************************
Description:
		Seek MIDI block  to a specified position. 
		
Parameters:
		dSeektime:		The position to be seeked, and the unit is ms.
		FiFoType:		Resource type.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT	SeekMidiMsg(UINT32 dSeekTime, VDEV_FIFO_TYPE FiFoType)
{
	HUGE UINT8	*pbPtr = NULL;
	UINT8		bBuf[FS_READ_LEN], bCounter = 0, bMsgID = 0, bCtrlID = 0, bChn = 0, bFlag = 0;
	UINT32		dTimeCode = 0, dDeltaTime  = 0, dDurTime = 0;
	UINT32		dPara1 = 0, dReadLen = FS_READ_LEN, dRealLen = 0;
	SINT32		sdCurTime = 0, sdDeltaTime = 0;

	if(0 == (g_VmdInfo.bEnabled & 0x01))
	{
		return VRES_ERROR;
	}

	for(bChn=0; bChn<MAX_MIDI_CHN; bChn++)
	{
		g_ChnInfo[bChn].timbre	  	= 0x00;
		g_ChnInfo[bChn].volume     	= 0x7f;
		g_ChnInfo[bChn].expression 	= 0x7f;
		g_ChnInfo[bChn].modulation 	= 0;
		g_ChnInfo[bChn].pitchbendH 	= 0x40;
		g_ChnInfo[bChn].pitchbendL 	= 0x00;
		g_ChnInfo[bChn].bendrange  	= 0x02;  //bendrange= 0x02*100;
		g_ChnInfo[bChn].panpot     	= 0x40;
		g_ChnInfo[bChn].hold1      	= 0x00;
		g_ChnInfo[bChn].bkselid		= 0x00;
	}

	if(g_VmdInfo.bEnabled & 0x0400)
	{
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&(dRealLen), g_VmdInfo.MidiBlk.dFCurOffSet);
		pbPtr = (HUGE UINT8 *)bBuf;

		bFlag = (g_VmdInfo.MidiBlk.dFCurOffSet < (g_VmdInfo.MidiBlk.dFBasOffSet + g_VmdInfo.MidiBlk.dBufLen))?1:0;
	}
	else
	{
		pbPtr 	= g_VmdInfo.MidiBlk.pbCurPtr;

		bFlag = (pbPtr < (g_VmdInfo.MidiBlk.pbBuf + g_VmdInfo.MidiBlk.dBufLen))?1:0;
	}
	
	while(bFlag)
	{
		dTimeCode	= *pbPtr++;
		dDeltaTime	= dTimeCode&0x7f;
		bCounter	= 0;
		while(dTimeCode&0x80)
		{
			bCounter++;
			dTimeCode   = *pbPtr++;
			dDeltaTime += (UINT16)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
		}

		sdDeltaTime	= (SINT32)dDeltaTime;
		sdCurTime 	= sdDeltaTime + g_sdLstMidiTime;	//###

		if((UINT32)sdCurTime > dSeekTime)
		{
			return VRES_SUCCESS;
		}

		g_sdLstMidiTime	= sdCurTime;	//### very important

		bMsgID = *pbPtr++;

		if((bMsgID&0x80)==0)
		{
			if(g_bLstMsgCode&0xf0==0xB0)
				bCtrlID = g_bLstCtrlCode;

			bMsgID = g_bLstMsgCode;
			pbPtr--; 
		}

		if((bMsgID&0xf0)==0x80)	//(B0) control message in midi
		{
			bCtrlID = bMsgID;
			bMsgID  = g_bLstMsgCode;
			pbPtr--;
		}

		switch(bMsgID&0xf0)
		{
		case 0x90:
			pbPtr++;
		
			dDurTime  = (*pbPtr++)&0x07;
			dTimeCode = *pbPtr++;
			dDurTime += ((dTimeCode&0x7f)<<3);
			bCounter=0;
			while(dTimeCode&0x80)
			{
				bCounter++;
				dTimeCode = *pbPtr++;
				dDurTime += (UINT16)(((UINT32)dTimeCode&0x7f)<<(7*bCounter+3));
			}
			break;

		case 0xC0:
			g_ChnInfo[bMsgID&0x0f].timbre = *pbPtr++;
			break;

		case 0xD0:
			pbPtr++;
			break;

		case 0xE0:
			g_ChnInfo[bMsgID&0x0f].pitchbendL = *pbPtr++;
			g_ChnInfo[bMsgID&0x0f].pitchbendH = *pbPtr++;
			break;
		
		case 0xB0:
			bCtrlID = *pbPtr++;
			if((bCtrlID&0x80)==0)
			{
				bCtrlID = g_bLstCtrlCode;
				pbPtr--;
			}
			g_bLstCtrlCode = bCtrlID;

			bChn			= bMsgID&0x0f;
			dPara1			= *pbPtr++;
			switch(bCtrlID&0x0f)
			{
			case 0:
				g_ChnInfo[bChn].modulation 	= (UINT8)dPara1;
				break;
			case 1:
				g_ChnInfo[bChn].volume 		= (UINT8)dPara1;
				break;
			case 2:
				g_ChnInfo[bChn].panpot 		= (UINT8)dPara1;
				break;
			case 3:
				g_ChnInfo[bChn].expression 	= (UINT8)dPara1;
				break;
			case 4:
				g_ChnInfo[bChn].hold1 		= (UINT8)dPara1;
				break;
			case 5:
				g_wRPN[bChn] = 0x7f7f;
				g_ChnInfo[bChn].expression	= 0x7f;
				g_ChnInfo[bChn].modulation 	= 0;
				g_ChnInfo[bChn].hold1      	= 0;
				break;
			case 6:
				break;
			case 7:
				g_wRPN[bChn] = ((g_wRPN[bChn] & 0x7F00) | ((UINT16)dPara1));
				break;
			case 8:
				g_wRPN[bChn] = ((g_wRPN[bChn] & 0x007F) | ((UINT16)dPara1)<<8);
				break;
			case 9:	
				if (g_wRPN[bChn] == 0) 
					g_ChnInfo[bChn].bendrange = (UINT8)dPara1;
				break;
			case 11:
				g_ChnInfo[bChn].bkselid	= (UINT8)dPara1;
				break;
			default:
				break;
			}
			break;
		}

		g_bLstMsgCode = bMsgID;

		if(g_VmdInfo.bEnabled & 0x0400)
		{
			g_VmdInfo.MidiBlk.dFCurOffSet	+= (pbPtr-bBuf);
			g_VmdInfo.MidiBlk.dOffSet 	= g_VmdInfo.MidiBlk.dFCurOffSet;	//###
		}
		else
		{
			g_VmdInfo.MidiBlk.pbCurPtr = pbPtr;
		}

		if(g_VmdInfo.bEnabled & 0x0400)
		{
			Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.MidiBlk.dFCurOffSet);
			pbPtr = (HUGE UINT8 *)bBuf;

			bFlag = (g_VmdInfo.MidiBlk.dFCurOffSet < (g_VmdInfo.MidiBlk.dFBasOffSet + g_VmdInfo.MidiBlk.dBufLen))?1:0;
		}
		else
		{
			pbPtr 	= g_VmdInfo.MidiBlk.pbCurPtr;

			bFlag = (pbPtr < (g_VmdInfo.MidiBlk.pbBuf + g_VmdInfo.MidiBlk.dBufLen))?1:0;
		}
	}
	
	return VRES_SUCCESS;
}



/*****************************************************************************
Description:
		Seek wave block  to a specified position. 
		
Parameters:
		dSeektime:		The position to be seeked, and the unit is ms.
		FiFoType:		Resource type.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT  SeekWaveMsg(UINT32 dSeekTime, HUGE UINT32* dNewTime, VDEV_FIFO_TYPE FiFoType)
{
	HUGE UINT8	*pbPtr = NULL;
	UINT8		bBuf[FS_READ_LEN], bWaveID = 0, bFlag = 0, bCounter = 0;
	SINT8		sbTrkID = 0, sbBlkNum[4] = {0, 0, 0, 0};
	UINT32		dTimeCode   = 0, dDeltaTime = 0;
	SINT32		sdDeltaTime = 0, sdCurTime  = 0;
	UINT32		dReadLen = FS_READ_LEN, dRealLen = 0;
	PSTRAM_BUF	pStm = NULL;

	if(0 == (g_VmdInfo.bEnabled & 0x02))
	{
		*dNewTime	= dSeekTime;
		return VRES_SUCCESS;
	}

	if(g_VmdInfo.bEnabled & 0x0400)
	{
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.WaveBlk.dFCurOffSet);
		pbPtr = (HUGE UINT8 *)bBuf;

		bFlag = (g_VmdInfo.WaveBlk.dFCurOffSet < (g_VmdInfo.WaveBlk.dFBasOffSet + g_VmdInfo.WaveBlk.dEvtBufLen))?1:0;
	}
	else
	{
		pbPtr 	= g_VmdInfo.WaveBlk.pbCurEvtPtr;

		bFlag = (pbPtr < (g_VmdInfo.WaveBlk.pbEvtBuf + g_VmdInfo.WaveBlk.dEvtBufLen))?1:0;
	}
	
	while(bFlag)
	{
		dTimeCode	= *pbPtr++;
		dDeltaTime	= dTimeCode&0x7f;
		bCounter	= 0;
		while(dTimeCode&0x80)
		{
			bCounter++;
			dTimeCode   = *pbPtr++;
			dDeltaTime += (UINT16)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
		}

		sdDeltaTime	= (SINT32)dDeltaTime;
		sdCurTime 	= sdDeltaTime + g_sdLstWaveTime;				//###

		if((UINT32)sdCurTime > dSeekTime)
		{
			//just data buf's offset
			g_VmdInfo.WaveBlk.sbStmTrkFlag[0][3] = sbBlkNum[0];
			g_VmdInfo.WaveBlk.sbStmTrkFlag[1][3] = sbBlkNum[1];
			g_VmdInfo.WaveBlk.sbStmTrkFlag[2][3] = sbBlkNum[2];
			g_VmdInfo.WaveBlk.sbStmTrkFlag[3][3] = sbBlkNum[3];

			if(g_VmdInfo.bEnabled & 0x0400)
			{
				g_VmdInfo.WaveBlk.DatBuf[bWaveID].dOffSet 		= g_VmdInfo.WaveBlk.DatBuf[bWaveID].dFBasOffSet;
				g_VmdInfo.WaveBlk.DatBuf[bWaveID].dFCurOffSet	= g_VmdInfo.WaveBlk.DatBuf[bWaveID].dFBasOffSet;
			}
			else
			{
				g_VmdInfo.WaveBlk.DatBuf[bWaveID].dOffSet	= 0;
			}

			*dNewTime	= g_sdLstMainTime;
			
			return VRES_SUCCESS;
		}
	
		sbTrkID = -1;
		switch(*pbPtr++)
		{
		case 0xA0:
			bWaveID = *pbPtr++;
			sbTrkID = 0;
			sbBlkNum[0]++;
			break;
		case 0xA1:
			bWaveID = *pbPtr++;
			sbTrkID = 1;
			sbBlkNum[1]++;			
			break;
		case 0xA2:
			bWaveID = *pbPtr++;
			sbTrkID = 2;
			sbBlkNum[2]++;			
			break;
		case 0xA3:
			bWaveID = *pbPtr++;
			sbTrkID = 3;
			sbBlkNum[3]++;			
			break;
		default:
			pbPtr++;
			if(g_VmdInfo.bEnabled & 0x0400)
			{
				g_VmdInfo.WaveBlk.dFCurOffSet += (pbPtr-bBuf);
				g_VmdInfo.WaveBlk.dOffSet	= g_VmdInfo.WaveBlk.dFCurOffSet;
			}
			else
			{
				g_VmdInfo.WaveBlk.pbCurEvtPtr	= pbPtr;
			}
			return VRES_ERROR;
			break;
		}

		if(g_VmdInfo.wVmdFileVersion >= VMD_STONE_VERSION)
		{
			pbPtr++;
			pbPtr++;
		}

		if(sbTrkID == -1)	//###
		{
			return VRES_ERROR;
		}

		pStm	= &(g_VmdInfo.WaveBlk.DatBuf[bWaveID]);

		if((sdCurTime + pStm->dTotalTime) > dSeekTime)
		{
			//just data buf's offset
			sbBlkNum[sbTrkID]--;
			
			g_VmdInfo.WaveBlk.sbStmTrkFlag[0][3] = sbBlkNum[0];	//cur dat num
			g_VmdInfo.WaveBlk.sbStmTrkFlag[1][3] = sbBlkNum[1];
			g_VmdInfo.WaveBlk.sbStmTrkFlag[2][3] = sbBlkNum[2];
			g_VmdInfo.WaveBlk.sbStmTrkFlag[3][3] = sbBlkNum[3];

			if(g_VmdInfo.bEnabled & 0x0400)
			{
				g_VmdInfo.WaveBlk.DatBuf[bWaveID].dOffSet 		= g_VmdInfo.WaveBlk.DatBuf[bWaveID].dFBasOffSet;
				g_VmdInfo.WaveBlk.DatBuf[bWaveID].dFCurOffSet	= g_VmdInfo.WaveBlk.DatBuf[bWaveID].dFBasOffSet;
			}
			else
			{
				g_VmdInfo.WaveBlk.DatBuf[bWaveID].dOffSet	= 0;
			}

			*dNewTime = (UINT32)sdCurTime;
			
			return VRES_SUCCESS;
		}	

		//just data buf's offset
		g_VmdInfo.WaveBlk.sbStmTrkFlag[0][3] = sbBlkNum[0];	//cur dat num
		g_VmdInfo.WaveBlk.sbStmTrkFlag[1][3] = sbBlkNum[1];
		g_VmdInfo.WaveBlk.sbStmTrkFlag[2][3] = sbBlkNum[2];
		g_VmdInfo.WaveBlk.sbStmTrkFlag[3][3] = sbBlkNum[3];

		if((UINT32)g_sdLstMainTime < (sdCurTime + pStm->dTotalTime))
			g_sdLstMainTime	 = (sdCurTime + pStm->dTotalTime);	//###
		g_sdLstWaveTime		 = sdCurTime;						//###

		if(g_VmdInfo.bEnabled & 0x0400)
		{
			g_VmdInfo.WaveBlk.dFCurOffSet	+= (pbPtr-bBuf);
			g_VmdInfo.WaveBlk.dOffSet	= g_VmdInfo.WaveBlk.dFCurOffSet;
		}
		else
		{
			g_VmdInfo.WaveBlk.pbCurEvtPtr	= pbPtr;
		}

		if(g_VmdInfo.bEnabled & 0x0400)
		{
			Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.WaveBlk.dFCurOffSet);
			pbPtr = (HUGE UINT8 *)bBuf;

			bFlag = (g_VmdInfo.WaveBlk.dFCurOffSet < (g_VmdInfo.WaveBlk.dFBasOffSet + g_VmdInfo.WaveBlk.dEvtBufLen))?1:0;
		}
		else
		{
			pbPtr 	= g_VmdInfo.WaveBlk.pbCurEvtPtr;

			bFlag = (pbPtr < (g_VmdInfo.WaveBlk.pbEvtBuf + g_VmdInfo.WaveBlk.dEvtBufLen))?1:0;
		}
	}
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Seek mp3 block  to a specified position. 
		
Parameters:
		dSeektime:		The position to be seeked, and the unit is ms.
		FiFoType:		Resource type.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.

		
******************************************************************************/
VRESULT  SeekMp3Msg(UINT32 dSeekTime, VDEV_FIFO_TYPE FiFoType)
{
	HUGE UINT8  *pbPtr = NULL;
	UINT8		bBuf[FS_READ_LEN], bMp3ID = 0, bCurMp3ID = 0, bFirst = 0, bFlag = 0, bCounter = 0;
	UINT32		dTimeCode   = 0, dDeltaTime = 0;
	UINT32		dPos = 0, dReadLen = FS_READ_LEN, dRealLen = 0;
	SINT32		sdDeltaTime = 0, sdCurTime  = 0;
	PMP3_BUF	pStm = NULL;

	if(0 == (g_VmdInfo.bEnabled & 0x20))
	{
		return VRES_ERROR;
	}

	if(g_VmdInfo.bEnabled & 0x0400)
	{
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.Mp3Blk.dFCurOffSet);
		pbPtr = (HUGE UINT8 *)bBuf;

		bFlag = (g_VmdInfo.Mp3Blk.dFCurOffSet < (g_VmdInfo.Mp3Blk.dFBasOffSet + g_VmdInfo.Mp3Blk.dEvtBufLen))?1:0;
	}
	else
	{
		pbPtr 	= g_VmdInfo.Mp3Blk.pbCurEvtPtr;

		bFlag = (pbPtr < (g_VmdInfo.Mp3Blk.pbEvtBuf + g_VmdInfo.Mp3Blk.dEvtBufLen))?1:0;
	}

	while(bFlag)
	{
		dTimeCode	= *pbPtr++;
		dDeltaTime	= dTimeCode&0x7f;
		bCounter	= 0;
		while(dTimeCode&0x80)
		{
			bCounter++;
			dTimeCode   = *pbPtr++;
			dDeltaTime += (UINT16)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
		}

		sdDeltaTime	= (SINT32)dDeltaTime;
		sdCurTime 	= sdDeltaTime + g_sdLstMp3Time;			//###

		if((UINT32)sdCurTime > dSeekTime)
		{
			//just data buf's offset
			g_VmdInfo.Mp3Blk.bCurDatNum	= bMp3ID + bFirst;

			if(g_VmdInfo.bEnabled & 0x0400)
			{
				g_VmdInfo.Mp3Blk.DatBuf[bMp3ID+bFirst].dFCurOffSet	= g_VmdInfo.Mp3Blk.DatBuf[bMp3ID+bFirst].dFBasOffSet;
				g_VmdInfo.Mp3Blk.DatBuf[bMp3ID+bFirst].dOffSet = g_VmdInfo.Mp3Blk.DatBuf[bMp3ID+bFirst].dFCurOffSet;
			}
			else
			{
				g_VmdInfo.Mp3Blk.DatBuf[bMp3ID+bFirst].dOffSet	= 0;
			}
			
			return VRES_ERROR;
		}
	
		switch(*pbPtr++)
		{
		case 0xA0:
			bMp3ID = *pbPtr++;
			bCurMp3ID++;
			break;
		default:
			pbPtr++;
			g_VmdInfo.Mp3Blk.pbCurEvtPtr	= pbPtr;
			return VRES_ERROR;
			break;
		}

		if(g_VmdInfo.wVmdFileVersion >= VMD_STONE_VERSION)
		{
			pbPtr++;
			pbPtr++;
		}

		bFirst	= 1;	//###

		pStm	= &(g_VmdInfo.Mp3Blk.DatBuf[bMp3ID]);

		if((sdCurTime + pStm->dTotalTime) > dSeekTime)
		{
			//just data buf's offset
			dPos = (UINT32)( ( (float)(dSeekTime-(UINT32)sdCurTime) / (float)(pStm->dTotalTime) )*pStm->dBufLen );

			if(g_VmdInfo.bEnabled & 0x0400)
			{
				pStm->dFCurOffSet	= pStm->dFBasOffSet + dPos;
				pStm->dOffSet		= pStm->dFBasOffSet;	//###

				if(pStm->dFCurOffSet < (pStm->dFBasOffSet + pStm->dBufLen))
					g_VmdInfo.Mp3Blk.bCurDatNum	= (bCurMp3ID-1); //bMp3ID;			
				else
					g_VmdInfo.Mp3Blk.bCurDatNum = ((bCurMp3ID)>g_VmdInfo.Mp3Blk.bDatNum)?g_VmdInfo.Mp3Blk.bDatNum:(bCurMp3ID);
			}
			else
			{
				pStm->dOffSet = dPos;

				if(pStm->dOffSet < pStm->dBufLen)
					g_VmdInfo.Mp3Blk.bCurDatNum	= bCurMp3ID-1; //bMp3ID;			
				else
					g_VmdInfo.Mp3Blk.bCurDatNum = ((bCurMp3ID)>g_VmdInfo.Mp3Blk.bDatNum)?g_VmdInfo.Mp3Blk.bDatNum:(bCurMp3ID);
			}
			
			return VRES_ERROR;
		}

		g_sdLstMp3Time	 	= sdCurTime;					    //###

		if(g_VmdInfo.bEnabled & 0x0400)
		{
			g_VmdInfo.Mp3Blk.dFCurOffSet	+= (pbPtr-bBuf);
			g_VmdInfo.Mp3Blk.dOffSet	= g_VmdInfo.Mp3Blk.dFCurOffSet;

			g_VmdInfo.Mp3Blk.bCurDatNum = ((bCurMp3ID)>g_VmdInfo.Mp3Blk.bDatNum)?g_VmdInfo.Mp3Blk.bDatNum:(bCurMp3ID);
		}
		else
		{
			g_VmdInfo.Mp3Blk.pbCurEvtPtr	= pbPtr;

			g_VmdInfo.Mp3Blk.bCurDatNum = ((bCurMp3ID)>g_VmdInfo.Mp3Blk.bDatNum)?g_VmdInfo.Mp3Blk.bDatNum:(bCurMp3ID);
		}

		if(g_VmdInfo.bEnabled & 0x0400)
		{
			Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.Mp3Blk.dFCurOffSet);
			pbPtr = (HUGE UINT8 *)bBuf;

			bFlag = (g_VmdInfo.Mp3Blk.dFCurOffSet < (g_VmdInfo.Mp3Blk.dFBasOffSet + g_VmdInfo.Mp3Blk.dEvtBufLen))?1:0;
		}
		else
		{
			pbPtr 	= g_VmdInfo.Mp3Blk.pbCurEvtPtr;

			bFlag = (pbPtr < (g_VmdInfo.Mp3Blk.pbEvtBuf + g_VmdInfo.Mp3Blk.dEvtBufLen))?1:0;
		}
	}
	
	return VRES_SUCCESS;
}

/*****************************************************************************
Description:
		Seek control block  to a specified position. 
		
Parameters:
		dSeektime:		The position to be seeked, and the unit is ms.
		FiFoType:		Resource type.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.

		
******************************************************************************/
VRESULT  SeekCtrlMsg(UINT32 dSeekTime, VDEV_FIFO_TYPE FiFoType)
{
	HUGE  UINT8 *pbPtr = NULL;
	UINT8		bBuf[FS_READ_LEN], bFlag = 0, bCtrlID=0, bCtrlLen=0, bCtrlDat=0, bLedFlag=0, bMotorFlag=0, bCounter = 0;
	UINT32		dTimeCode   = 0, dDeltaTime = 0;
	SINT32		sdDeltaTime = 0, sdCurTime  = 0, dColor = 0;
	UINT32		dReadLen = FS_READ_LEN, dRealLen = 0;

	if( (0 == (g_VmdInfo.bEnabled & 0x100)) &&
		(0 == (g_VmdInfo.bEnabled & 0x200)) )
	{
		return VRES_ERROR;
	}

	if(g_VmdInfo.bEnabled & 0x0400)
	{
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.CtrlBlk.dFCurOffSet);
		pbPtr = (HUGE UINT8 *)bBuf;

		bFlag = (g_VmdInfo.CtrlBlk.dFCurOffSet < (g_VmdInfo.CtrlBlk.dFBasOffSet + g_VmdInfo.CtrlBlk.dBufLen))?1:0;
	}
	else
	{
		pbPtr 	= g_VmdInfo.CtrlBlk.pbCurPtr;

		bFlag = (pbPtr < (g_VmdInfo.CtrlBlk.pbBuf + g_VmdInfo.CtrlBlk.dBufLen))?1:0;
	}

	while(bFlag)
	{
		dTimeCode	= *pbPtr++;
		dDeltaTime	= dTimeCode&0x7f;
		bCounter	= 0;
		while(dTimeCode&0x80)
		{
			bCounter++;
			dTimeCode   = *pbPtr++;
			dDeltaTime += (UINT16)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
		}

		sdDeltaTime	= (SINT32)dDeltaTime;
		sdCurTime 	= sdDeltaTime + g_sdLstCtrlTime;		//###

		if((UINT32)sdCurTime > dSeekTime)
		{
			return VRES_SUCCESS;
		}

		sdDeltaTime	= sdCurTime   - g_sdLstMainTime;		//###***###
	
		g_sdLstCtrlTime		= sdCurTime;					//###

		if((*pbPtr++) != 0xff)
		{
			return VRES_ERROR;
		}
		else
		{
			bCtrlID 	= *pbPtr++;
			bCtrlLen 	= *pbPtr++;
			bCtrlDat 	= *pbPtr;
			bLedFlag	= bCtrlDat&0x08;
			bMotorFlag	= bCtrlDat&0x80;
		
			dColor = 0;
			dColor = bCtrlDat&0x07;
			if(8 == bCtrlID || 9 == bCtrlID)
			{
			}
			else
			{
				return VRES_ERROR;
			}

			pbPtr += bCtrlLen;

			if(g_VmdInfo.bEnabled & 0x0400)
			{
				g_VmdInfo.CtrlBlk.dFCurOffSet	+= (pbPtr-bBuf);
				g_VmdInfo.CtrlBlk.dOffSet	= g_VmdInfo.CtrlBlk.dFCurOffSet;
			}
			else
			{
				g_VmdInfo.CtrlBlk.pbCurPtr	= pbPtr;
			}
		}

		if(g_VmdInfo.bEnabled & 0x0400)
		{
			Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.CtrlBlk.dFCurOffSet);
			pbPtr = (HUGE UINT8 *)bBuf;

			bFlag = (g_VmdInfo.CtrlBlk.dFCurOffSet < (g_VmdInfo.CtrlBlk.dFBasOffSet + g_VmdInfo.CtrlBlk.dBufLen))?1:0;
		}
		else
		{
			pbPtr 	= g_VmdInfo.CtrlBlk.pbCurPtr;

			bFlag = (pbPtr < (g_VmdInfo.CtrlBlk.pbBuf + g_VmdInfo.CtrlBlk.dBufLen))?1:0;
		}
	}
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Send available MIDI block messages to the message list. All of these 
		message is part of midi block before seek position.
		
Parameters:
		void.
		
Returns:
		void.
		
******************************************************************************/
void SendMidiMsg(void)
{
	UINT8 idx;
	PMSGQUEUE pQue;

	for(idx=0; idx<MAX_MIDI_CHN; idx++)
	{
		if(g_ChnInfo[idx].bkselid != 0x00)
		{
			pQue = GetMsgFromQue();			
			if(pQue)
			{
				pQue->bID   	= VMD_MSG_BANK_SELECT;
				pQue->bChn  	= idx;
				pQue->sdTime 	= g_sdLstMainTime;
				pQue->dPara1	= g_ChnInfo[idx].bkselid;
				SendMsgToQue(pQue);
			}
		}
		
		if(g_ChnInfo[idx].timbre != 0x00)
		{
			pQue = GetMsgFromQue();			
			if(pQue)
			{
				pQue->bID   	= VMD_MSG_PROGRAM_CHANGE;
				pQue->bChn  	= idx;
				pQue->sdTime 	= g_sdLstMainTime;
				pQue->dPara1	= g_ChnInfo[idx].timbre;
				SendMsgToQue(pQue);
			}
		}

		if(g_ChnInfo[idx].volume != 0x7f)
		{
			pQue = GetMsgFromQue();			
			if(pQue)
			{
				pQue->bID   	= VMD_MSG_CHANNEL_VOLUME;
				pQue->bChn  	= idx;
				pQue->sdTime 	= g_sdLstMainTime;
				pQue->dPara1	= g_ChnInfo[idx].volume;
				SendMsgToQue(pQue);
			}		
		}

		if(g_ChnInfo[idx].expression != 0x7f)
		{
			pQue = GetMsgFromQue();		
			if(pQue)
			{
				pQue->bID   	= VMD_MSG_EXPRESSION;
				pQue->bChn  	= idx;
				pQue->sdTime 	= g_sdLstMainTime;
				pQue->dPara1	= g_ChnInfo[idx].expression;
				SendMsgToQue(pQue);
			}
		}
		
		if(g_ChnInfo[idx].modulation != 0)
		{
			pQue = GetMsgFromQue();		
			if(pQue)
			{
				pQue->bID  		= VMD_MSG_MODULATION_DEPTH;
				pQue->bChn  	= idx;
				pQue->sdTime 	= g_sdLstMainTime;
				pQue->dPara1	= g_ChnInfo[idx].modulation;
				SendMsgToQue(pQue);
			}
		}

		if(g_ChnInfo[idx].pitchbendH != 0x40 || g_ChnInfo[idx].pitchbendL != 0x00)
		{
			pQue = GetMsgFromQue();		
			if(pQue)
			{
				pQue->bID   	= VMD_MSG_PITCH_BEND;
				pQue->bChn  	= idx;
				pQue->sdTime 	= g_sdLstMainTime;
				pQue->dPara1	= (((UINT32)(g_ChnInfo[idx].pitchbendH &0x7f)) << 7 ) + 
					(g_ChnInfo[idx].pitchbendL&0x7f);
				SendMsgToQue(pQue);
			}
		}
		
		if(g_ChnInfo[idx].bendrange  != 0x02)
		{
			pQue = GetMsgFromQue();	
			if(pQue)
			{
				pQue->bID   	= VMD_MSG_BEND_RANGE;	//###
				pQue->bChn  	= idx;
				pQue->sdTime 	= g_sdLstMainTime;
				pQue->dPara1	= g_ChnInfo[idx].bendrange;
				SendMsgToQue(pQue);
			}
		}
		
		if(g_ChnInfo[idx].panpot != 0x40)
		{
			pQue = GetMsgFromQue();	
			if(pQue)
			{
				pQue->bID   	= VMD_MSG_PANPOT;
				pQue->bChn  	= idx;
				pQue->sdTime 	= g_sdLstMainTime;
				pQue->dPara1	= g_ChnInfo[idx].panpot;
				SendMsgToQue(pQue);
			}
		}

		if(g_ChnInfo[idx].hold1 != 0x00)
		{
			pQue = GetMsgFromQue();		
			if(pQue)
			{
				pQue->bID   	= VMD_MSG_HOLD1;
				pQue->bChn  	= idx;
				pQue->sdTime 	= g_sdLstMainTime;
				pQue->dPara1	= g_ChnInfo[idx].hold1;
				SendMsgToQue(pQue);
			}
		}
		
	}
}


/*****************************************************************************
Description:
		Get event type, so we know from which block to get event message.
		
Parameters:
		void.
				
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
MEDIA_TYPE	Get_Event_Type()
{
	HUGE UINT8 *pbPtr = NULL;
	UINT8   bCounter = 0;
	UINT32  dTimeCode = 0, dDeltaTime = 0;
	SINT32	sdQueTime = 0x7fffffff, sdMidiTime  = 0x7fffffff, sdWaveTime = 0x7fffffff, sdUdseTime = 0x7fffffff;
	SINT32	sdMp3Time = 0x7fffffff, sdCtrlTime = 0x7fffffff;
	UINT8	bBuf[FS_READ_LEN];
	UINT32	dReadLen = FS_READ_LEN, dRealLen = 0;

	SINT32  	sdMin 	= 0x7fffffff;
	MEDIA_TYPE	RstType = NOTHING;

	//-------------------------------------------------------------------------
	if(g_WaitingHdr.pNext != NULL)
	{
		sdQueTime = g_WaitingHdr.pNext->sdTime;	//###
	}

	//-------------------------------------------------------------------------
	if(g_VmdInfo.bEnabled & 0x01)
	{
		if(g_VmdInfo.bEnabled & 0x0400)
		{
			if(g_VmdInfo.MidiBlk.dOffSet >= (g_VmdInfo.MidiBlk.dFBasOffSet+g_VmdInfo.MidiBlk.dBufLen))
			{
				sdMidiTime = 0x7fffffff;
			}
			else
			{
				Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.MidiBlk.dOffSet);
				pbPtr = (HUGE UINT8 *)bBuf;

				dTimeCode	= *pbPtr++;
				dDeltaTime	= dTimeCode&0x7f;
				bCounter	= 0;
				while(dTimeCode&0x80)
				{
					bCounter++;
					dTimeCode   = *pbPtr++;
					dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
				}
				sdMidiTime = (SINT32)dDeltaTime + g_sdLstMidiTime;	//###
			}
		}
		else
		{
			pbPtr	= g_VmdInfo.MidiBlk.pbCurPtr;
			if(pbPtr >= g_VmdInfo.MidiBlk.pbBuf + g_VmdInfo.MidiBlk.dBufLen)
			{
				sdMidiTime	= 0x7fffffff;
			}
			else
			{
				dTimeCode	= *pbPtr++;
				dDeltaTime	= dTimeCode&0x7f;
				bCounter	= 0;
				while(dTimeCode&0x80)
				{
					bCounter++;
					dTimeCode   = *pbPtr++;
					dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
				}
				sdMidiTime = (SINT32)dDeltaTime + g_sdLstMidiTime;	//###
			}
		}
	}

	//-------------------------------------------------------------------------
	if(g_VmdInfo.bEnabled & 0x02)
	{
		if(g_VmdInfo.bEnabled & 0x0400)
		{
			if(g_VmdInfo.WaveBlk.dOffSet >= (g_VmdInfo.WaveBlk.dFBasOffSet+g_VmdInfo.WaveBlk.dEvtBufLen))
			{
				sdWaveTime = 0x7fffffff;
			}
			else
			{
				Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.WaveBlk.dOffSet);
				pbPtr = (HUGE UINT8 *)bBuf;

				dTimeCode	= *pbPtr++;
				dDeltaTime	= dTimeCode&0x7f;
				bCounter	= 0;
				while(dTimeCode&0x80)
				{
					bCounter++;
					dTimeCode   = *pbPtr++;
					dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
				}
				sdWaveTime = (SINT32)dDeltaTime + g_sdLstWaveTime;	//###
			}
		}
		else
		{
			pbPtr	= g_VmdInfo.WaveBlk.pbCurEvtPtr;
			if(pbPtr >= g_VmdInfo.WaveBlk.pbEvtBuf + g_VmdInfo.WaveBlk.dEvtBufLen)
			{
				sdWaveTime = 0x7fffffff;
			}
			else
			{
				dTimeCode	= *pbPtr++;
				dDeltaTime	= dTimeCode&0x7f;
				bCounter	= 0;
				while(dTimeCode&0x80)
				{
					bCounter++;
					dTimeCode   = *pbPtr++;
					dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
				}
				sdWaveTime = (SINT32)dDeltaTime + g_sdLstWaveTime;	//###
			}
		}
	}

	//-------------------------------------------------------------------------
	if(g_VmdInfo.bEnabled & 0x04)
	{
		if(g_VmdInfo.bEnabled & 0x0400)
		{
			if(g_VmdInfo.UdseBlk.dOffSet >= (g_VmdInfo.UdseBlk.dFBasEvtOffSet+g_VmdInfo.UdseBlk.dEvtBufLen))
			{
				sdUdseTime = 0x7fffffff;
			}
			else
			{
				Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.UdseBlk.dOffSet);
				pbPtr = (HUGE UINT8 *)bBuf;

				dTimeCode	= *pbPtr++;
				dDeltaTime	= dTimeCode&0x7f;
				bCounter	= 0;
				while(dTimeCode&0x80)
				{
					bCounter++;
					dTimeCode   = *pbPtr++;
					dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
				}
				sdUdseTime = (SINT32)dDeltaTime + g_sdLstUdseTime;	//###
			}
		}
		else
		{
			pbPtr	= g_VmdInfo.UdseBlk.pbCurEvtPtr;
			if(pbPtr >= g_VmdInfo.UdseBlk.pbEvtBuf + g_VmdInfo.UdseBlk.dEvtBufLen)
			{
				sdUdseTime = 0x7fffffff;
			}
			else
			{
				dTimeCode	= *pbPtr++;
				dDeltaTime	= dTimeCode&0x7f;
				bCounter	= 0;
				while(dTimeCode&0x80)
				{
					bCounter++;
					dTimeCode   = *pbPtr++;
					dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
				}
				sdUdseTime = (SINT32)dDeltaTime + g_sdLstUdseTime;	//###
			}
		}
	}

	//-------------------------------------------------------------------------
	if(g_VmdInfo.bEnabled & 0x20)
	{
		if(g_VmdInfo.bEnabled & 0x0400)
		{
			if(g_VmdInfo.Mp3Blk.dOffSet >= (g_VmdInfo.Mp3Blk.dFBasOffSet+g_VmdInfo.Mp3Blk.dEvtBufLen))
			{
				sdMp3Time = 0x7fffffff;
			}
			else
			{
				Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.Mp3Blk.dOffSet);
				pbPtr = (HUGE UINT8 *)bBuf;

				dTimeCode	= *pbPtr++;
				dDeltaTime	= dTimeCode&0x7f;
				bCounter	= 0;
				while(dTimeCode&0x80)
				{
					bCounter++;
					dTimeCode   = *pbPtr++;
					dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
				}
				sdMp3Time = (SINT32)dDeltaTime + g_sdLstMp3Time;	//###
			}
		}
		else
		{
			pbPtr	= g_VmdInfo.Mp3Blk.pbCurEvtPtr;
			if(pbPtr >= g_VmdInfo.Mp3Blk.pbEvtBuf + g_VmdInfo.Mp3Blk.dEvtBufLen)
			{
				sdMp3Time = 0x7fffffff;
			}
			else
			{
				dTimeCode	= *pbPtr++;
				dDeltaTime	= dTimeCode&0x7f;
				bCounter	= 0;
				while(dTimeCode&0x80)
				{
					bCounter++;
					dTimeCode   = *pbPtr++;
					dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
				}
				sdMp3Time = (SINT32)dDeltaTime + g_sdLstMp3Time;	//###
			}
		}
	}

	//-------------------------------------------------------------------------
	if( (g_VmdInfo.bEnabled & 0x100) || (g_VmdInfo.bEnabled & 0x100) )
	{
		if(g_VmdInfo.bEnabled & 0x0400)
		{
			if(g_VmdInfo.CtrlBlk.dOffSet >= (g_VmdInfo.CtrlBlk.dFBasOffSet+g_VmdInfo.CtrlBlk.dBufLen))
			{
				sdCtrlTime = 0x7fffffff;
			}
			else
			{
				Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.CtrlBlk.dOffSet);
				pbPtr = (HUGE UINT8 *)bBuf;

				dTimeCode	= *pbPtr++;
				dDeltaTime	= dTimeCode&0x7f;
				bCounter	= 0;
				while(dTimeCode&0x80)
				{
					bCounter++;
					dTimeCode   = *pbPtr++;
					dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
				}
				sdCtrlTime	= (SINT32)dDeltaTime + g_sdLstCtrlTime;	//###
			}
		}
		else
		{
			pbPtr	= g_VmdInfo.CtrlBlk.pbCurPtr;
			if(pbPtr >= g_VmdInfo.CtrlBlk.pbBuf + g_VmdInfo.CtrlBlk.dBufLen)
			{
				sdCtrlTime = 0x7fffffff;
			}
			else
			{
				dTimeCode	= *pbPtr++;
				dDeltaTime	= dTimeCode&0x7f;
				bCounter	= 0;
				while(dTimeCode&0x80)
				{
					bCounter++;
					dTimeCode   = *pbPtr++;
					dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
				}
				sdCtrlTime	= (SINT32)dDeltaTime + g_sdLstCtrlTime;	//###
			}
		}
	}

	//-------------------------------------------------------------------------
	if(sdMidiTime < sdMin)
	{
		sdMin 	= sdMidiTime;
		RstType	= MIDI;
	}
	if(sdWaveTime < sdMin)
	{
		sdMin 	= sdWaveTime;
		RstType = WAVE;
	}
	if(sdUdseTime < sdMin)
	{
		sdMin	= sdUdseTime;
		RstType = UDSE;
	}
	if(sdMp3Time < sdMin)
	{
		sdMin 	= sdMp3Time;
		RstType	= MP3;
	}
	if(sdCtrlTime < sdMin)
	{
		sdMin 	= sdCtrlTime;
		RstType	= CTRL;
	}
	if((sdQueTime != 0x7fffffff) && (sdQueTime <= sdMin))
	{
		sdMin = sdQueTime;
		RstType = QUE;
	}
	
	return RstType;
}

/*****************************************************************************
Description:
		Get a midi event message.
		
Parameters:
		EventPtr:		Pointer to message structure.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT Get_Midi_Event(PVM_UNI_MSG EventPtr)
{
	HUGE UINT8 *pbPtr = NULL;
	UINT8 		bBuf[FS_READ_LEN];
	UINT8		bMsgID = 0, bCtrlID = 0, bCounter  = 0;
	UINT32		dTimeCode = 0, dDeltaTime  = 0, dDurTime = 0;
	SINT32		sdCurTime = 0, sdDeltaTime = 0;
	UINT32		dReadLen = FS_READ_LEN, dRealLen = 0;
	PMSGQUEUE	pQue = NULL;

	if(g_VmdInfo.bEnabled & 0x0400)	//file stream
	{
		if(g_VmdInfo.MidiBlk.dOffSet >= (g_VmdInfo.MidiBlk.dFBasOffSet + g_VmdInfo.MidiBlk.dBufLen))
		{
			return VRES_END_FLAG;	//###
		}
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.MidiBlk.dOffSet);
		pbPtr = (HUGE UINT8*)bBuf;
	}
	else
	{
		pbPtr = g_VmdInfo.MidiBlk.pbCurPtr;
		if(pbPtr >= g_VmdInfo.MidiBlk.pbBuf + g_VmdInfo.MidiBlk.dBufLen)
		{
			return VRES_END_FLAG;	//###
		}
	}

	dTimeCode	= *pbPtr++;
	dDeltaTime	= dTimeCode&0x7f;
	bCounter	= 0;
	while(dTimeCode&0x80)
	{
		bCounter++;
		dTimeCode   = *pbPtr++;
		dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
	}

	sdDeltaTime	= (SINT32)dDeltaTime;
	sdCurTime 	= sdDeltaTime + g_sdLstMidiTime;	//###

	sdDeltaTime	= sdCurTime   - g_sdLstMainTime;	//###***###

	if(g_sdLstMainTime < sdCurTime)
		g_sdLstMainTime	= sdCurTime;				//###
	g_sdLstMidiTime		= sdCurTime;				//###
	
	if(sdDeltaTime < 0)
	{
		sdDeltaTime = 0;
	}
		
	bMsgID = *pbPtr++;

	if((bMsgID&0x80)==0)
	{
		if(g_bLstMsgCode&0xf0==0xB0)
			bCtrlID = g_bLstCtrlCode;

		bMsgID = g_bLstMsgCode;
		pbPtr--; 
	}

	if((bMsgID&0xf0)==0x80)	//(B0) control message in midi
	{
		bCtrlID = bMsgID;
		bMsgID  = g_bLstMsgCode;
		pbPtr--;
	}

	switch(bMsgID&0xf0)
	{
	case 0x90:
		EventPtr->sdDeltaTime	= sdDeltaTime;
		EventPtr->bMsgID		= VMD_MSG_NOTE_ON;
		EventPtr->bChn			= bMsgID&0x0f;
		EventPtr->dPara1		= (UINT32)(*pbPtr++);
		EventPtr->dPara2		= (((UINT32)(*pbPtr)>>3)<<2);	

		dDurTime  = (*pbPtr++)&0x07;
		dTimeCode = *pbPtr++;
		dDurTime += ((dTimeCode&0x7f)<<3);
		bCounter=0;
		while(dTimeCode&0x80)
		{
			bCounter++;
			dTimeCode = *pbPtr++;
			dDurTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter+3));
		}
		
		pQue = GetMsgFromQue();  
		if(pQue)
		{
			pQue->sdTime	= (SINT32)dDurTime + sdCurTime;
			pQue->dPara1 	= (UINT8)EventPtr->dPara1;
			pQue->dPara2	= 0;
			pQue->dPara3	= 0;
			pQue->bChn   	= EventPtr->bChn;
			pQue->bID		= VMD_MSG_NOTE_OFF;	
			SendMsgToQue(pQue);
		}
		break;

	case 0xC0:
		EventPtr->sdDeltaTime	= sdDeltaTime;
		EventPtr->bMsgID		= VMD_MSG_PROGRAM_CHANGE;
		EventPtr->bChn			= bMsgID&0x0f;
		EventPtr->dPara1		= *pbPtr++;
		break;

	case 0xD0:
		EventPtr->sdDeltaTime	= sdDeltaTime;
		EventPtr->bMsgID		= VMD_MSG_CHANNEL_PRESSURE;
		EventPtr->bChn			= bMsgID&0x0f;
		EventPtr->dPara1		= *pbPtr++;
		break;

	case 0xE0:
		EventPtr->sdDeltaTime	= sdDeltaTime;
		EventPtr->bMsgID		= VMD_MSG_PITCH_BEND;
		EventPtr->bChn			= bMsgID&0x0f;
		EventPtr->dPara1		= *pbPtr++;
		EventPtr->dPara2		= *pbPtr++;
		EventPtr->dPara1		= (((UINT32)(EventPtr->dPara2 &0x7f)) << 7 ) + (EventPtr->dPara1&0x7f);
		break;
		
	case 0xB0:
		bCtrlID = *pbPtr++;
		if((bCtrlID&0x80)==0)
		{
			bCtrlID = g_bLstCtrlCode;
			pbPtr--;
		}
		g_bLstCtrlCode = bCtrlID;

		EventPtr->sdDeltaTime	= sdDeltaTime;
		EventPtr->bChn			= bMsgID&0x0f;
		EventPtr->dPara1		= *pbPtr++;

		switch(bCtrlID&0x0f)
		{
		case 0:
			EventPtr->bMsgID = VMD_MSG_MODULATION_DEPTH;
			break;
		case 1:
			EventPtr->bMsgID = VMD_MSG_CHANNEL_VOLUME;
			break;
		case 2:
			EventPtr->bMsgID = VMD_MSG_PANPOT;
			break;
		case 3:
			EventPtr->bMsgID = VMD_MSG_EXPRESSION;
			break;
		case 4:
			EventPtr->bMsgID = VMD_MSG_HOLD1;
			break;
		case 5:
			EventPtr->bMsgID = VMD_MSG_RESET_ALL_CONTROLLERS;
			g_wRPN[EventPtr->bChn] = 0x7f7f;
			break;
		case 6:
			EventPtr->bMsgID = VMD_MSG_ALL_NOTE_OFF;
			break;
		case 7:
			g_wRPN[EventPtr->bChn] = ((g_wRPN[EventPtr->bChn] & 0x7F00) | ((UINT16)EventPtr->dPara1));
			EventPtr->bMsgID = VMD_MSG_NOP;
			break;
		case 8:
			g_wRPN[EventPtr->bChn] = ((g_wRPN[EventPtr->bChn] & 0x007F) | ((UINT16)EventPtr->dPara1)<<8);
			EventPtr->bMsgID = VMD_MSG_NOP;
			break;
		case 9:	
			if (g_wRPN[EventPtr->bChn] == 0) 
				EventPtr->bMsgID = VMD_MSG_BEND_RANGE;
			else
				EventPtr->bMsgID = VMD_MSG_NOP;
			break;
		case 11:
			EventPtr->bMsgID = VMD_MSG_BANK_SELECT;
			break;
		default:
			EventPtr->bMsgID = VMD_MSG_NOP;
			break;
		}
		break;
	}

	g_bLstMsgCode = bMsgID;

	if(g_VmdInfo.bEnabled & 0x0400)	//file stream
	{
		g_VmdInfo.MidiBlk.dOffSet	+= (pbPtr - bBuf);
	}
	else
	{
		g_VmdInfo.MidiBlk.pbCurPtr = pbPtr;	
	}
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Get a wave event message.
		
Parameters:
		EventPtr:		Pointer to message structure.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT Get_Wave_Event(PVM_UNI_MSG EventPtr)
{
	HUGE UINT8	*pbPtr = NULL;
	UINT8		bBuf[FS_READ_LEN], bCounter = 0, bWaveID = 0, bVol = 127, bPan = 64;
	SINT8		sdFifoID = 0;
	UINT32		dTimeCode   = 0, dDeltaTime = 0;
	SINT32		sdDeltaTime = 0, sdCurTime  = 0;
	UINT32		dReadLen = FS_READ_LEN, dRealLen = 0;
	PMSGQUEUE	pQue = NULL;
	PSTRAM_BUF	pStm = NULL;

	if(g_VmdInfo.bEnabled & 0x0400)
	{
		if(g_VmdInfo.WaveBlk.dOffSet >= (g_VmdInfo.WaveBlk.dFBasOffSet + g_VmdInfo.WaveBlk.dEvtBufLen))
			return VRES_END_FLAG;
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.WaveBlk.dOffSet);
		pbPtr = (HUGE UINT8*)bBuf;
	}
	else
	{
		pbPtr 		= g_VmdInfo.WaveBlk.pbCurEvtPtr;
		if(pbPtr >= (g_VmdInfo.WaveBlk.pbEvtBuf + g_VmdInfo.WaveBlk.dEvtBufLen))
		{
			return VRES_END_FLAG;
		}
	}

	dTimeCode	= *pbPtr++;
	dDeltaTime	= dTimeCode&0x7f;
	bCounter	= 0;
	while(dTimeCode&0x80)
	{
		bCounter++;
		dTimeCode   = *pbPtr++;
		dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
	}

	sdDeltaTime	= (SINT32)dDeltaTime;
	sdCurTime 	= sdDeltaTime + g_sdLstWaveTime;		//###
	
	sdDeltaTime	= sdCurTime - g_sdLstMainTime;			//###***###

	if(g_sdLstMainTime < sdCurTime)
		g_sdLstMainTime	= sdCurTime;					//###
	g_sdLstWaveTime		= sdCurTime;					//###

	if(sdDeltaTime < 0)
	{
		sdDeltaTime = 0;
	}

	switch(*pbPtr++)
	{
	case 0xA0:
		bWaveID = *pbPtr++;
		sdFifoID = g_VmdInfo.WaveBlk.sbStmTrkFlag[0][1];
		break;
	case 0xA1:
		bWaveID = *pbPtr++;
		sdFifoID = g_VmdInfo.WaveBlk.sbStmTrkFlag[1][1];		
		break;
	case 0xA2:
		bWaveID = *pbPtr++;
		sdFifoID = g_VmdInfo.WaveBlk.sbStmTrkFlag[2][1];		
		break;
	case 0xA3:
		bWaveID = *pbPtr++;
		sdFifoID = g_VmdInfo.WaveBlk.sbStmTrkFlag[3][1];		
		break;
	default:
		pbPtr++;
		g_VmdInfo.WaveBlk.pbCurEvtPtr	= pbPtr;
		return VRES_WAVEMSG_ERROR;
		break;
	}

	if(g_VmdInfo.wVmdFileVersion >= VMD_STONE_VERSION)
	{
		bVol = *pbPtr++;
		bPan = *pbPtr++;
	}
	else
	{
		bVol = 127;
		bPan = 64;
	}

	if(sdFifoID == -1)	//###
	{
		return VRES_WAVEMSG_ERROR;
	}

	pStm	= &(g_VmdInfo.WaveBlk.DatBuf[bWaveID]);

	EventPtr->bMsgID		= VMD_MSG_STREAM_ON;
	EventPtr->bChn			= (UINT8)(sdFifoID + 20);
	EventPtr->sdDeltaTime	= sdDeltaTime; //-1;	//###

	EventPtr->dPara1 = ((UINT32)(pStm->bFormat^1)&0xf) +  ((UINT32)(pStm->bChn&0x3)<<4) +
				((UINT32)(pStm->bSigned&0x3)<<6)  + ((UINT32)(pStm->bBitsPerSmpl)<<8)+
				((UINT32)(pStm->wSmplRate)<<16);
	//EventPtr->dPara2 = pStm->dTotalTime ;
	EventPtr->dPara2 = (bPan << 8) + bVol;
	EventPtr->dPara3 = (UINT32)((UINT16)pStm->AdpcmState.swPreVal) + 
				((UINT32)(pStm->AdpcmState.bIndex)<<16);

	g_sdEndTime	= sdCurTime + pStm->dTotalTime;
	/*pQue = GetMsgFromQue();
	if(pQue)
	{
		pQue->bID		= VMD_MSG_STREAM_OFF;
		pQue->bChn		= (UINT8)(sdFifoID+20);
		pQue->sdTime 	= sdCurTime + pStm->dTotalTime; 
		SendMsgToQue(pQue);	//###
	}*/

	dDeltaTime = pStm->dTotalTime/0x7fff + ((pStm->dTotalTime%0x7fff)?1:0);
	for(dTimeCode=0; dTimeCode<dDeltaTime; dTimeCode++)
	{
		pQue = GetMsgFromQue();
		if(pQue)
		{
			pQue->bChn	= (UINT8)(sdFifoID+20);
			if(dTimeCode == (dDeltaTime-1))
			{
				pQue->bID		= VMD_MSG_STREAM_OFF;
				pQue->sdTime	= sdCurTime + (dTimeCode*0x7fff) + (pStm->dTotalTime%0x7fff);
			}
			else
			{
				pQue->bID		= VMD_MSG_NOP;
				pQue->sdTime	= sdCurTime + ((dTimeCode+1)*0x7fff);
			}
			SendMsgToQue(pQue);	//###
		}
	}

	if(g_VmdInfo.bEnabled & 0x0400)
	{
		g_VmdInfo.WaveBlk.dOffSet	+= (pbPtr - bBuf);
	}
	else
	{
		g_VmdInfo.WaveBlk.pbCurEvtPtr	= pbPtr;
	}
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Get a udse event message.
		
Parameters:
		EventPtr:		Pointer to message structure.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT Get_Udse_Event(PVM_UNI_MSG EventPtr)
{
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Get a mp3 event message.
		
Parameters:
		EventPtr:		Pointer to message structure.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT Get_Mp3_Event(PVM_UNI_MSG EventPtr)
{
	HUGE UINT8* pbPtr = NULL;
	UINT8		bBuf[FS_READ_LEN], bCounter = 0, bMp3ID = 0, bStereo = 0, bVol = 127, bPan = 64;
	UINT32		dTimeCode   = 0, dDeltaTime = 0;
	UINT32 		dBitpersample = 16;
	SINT32		sdDeltaTime = 0, sdCurTime  = 0;
	UINT32		dReadLen = FS_READ_LEN, dRealLen = 0;
	PMSGQUEUE	pQue = NULL;
	PMP3_BUF	pStm = NULL;

	if(g_VmdInfo.bEnabled & 0x0400)
	{
		if(g_VmdInfo.Mp3Blk.dOffSet >= g_VmdInfo.Mp3Blk.dFBasOffSet + g_VmdInfo.Mp3Blk.dEvtBufLen)
			return VRES_END_FLAG;
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.Mp3Blk.dOffSet);
		pbPtr = (HUGE UINT8*)bBuf;
	}
	else
	{
		pbPtr 		= g_VmdInfo.Mp3Blk.pbCurEvtPtr;
		if(pbPtr >= g_VmdInfo.Mp3Blk.pbEvtBuf + g_VmdInfo.Mp3Blk.dEvtBufLen)
		{
			return VRES_END_FLAG;
		}
	}

	dTimeCode	= *pbPtr++;
	dDeltaTime	= dTimeCode&0x7f;
	bCounter	= 0;
	while(dTimeCode&0x80)
	{
		bCounter++;
		dTimeCode   = *pbPtr++;
		dDeltaTime += (UINT32)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
	}

	sdDeltaTime	= (SINT32)dDeltaTime;
	sdCurTime 	= sdDeltaTime + g_sdLstMp3Time;			//###
	
	sdDeltaTime	= sdCurTime - g_sdLstMainTime;			//###***###

	if(g_sdLstMainTime < sdCurTime)
		g_sdLstMainTime		= sdCurTime;				//###
	g_sdLstMp3Time 			= sdCurTime;				//###

	if(sdDeltaTime < 0)
	{
		sdDeltaTime = 0;
	}

	switch(*pbPtr++)
	{
	case 0xA0:
		bMp3ID = *pbPtr++;
		break;
	default:
		pbPtr++;
		g_VmdInfo.Mp3Blk.pbCurEvtPtr	= pbPtr;
		return VRES_WAVEMSG_ERROR;
		break;
	}

	if(g_VmdInfo.wVmdFileVersion >= VMD_STONE_VERSION)
	{
		bVol = *pbPtr++;
		bPan = *pbPtr++;
	}
	else
	{
		bVol = 127;
		bPan = 64;
	}

	pStm	= &(g_VmdInfo.Mp3Blk.DatBuf[bMp3ID]);

	if(pStm->bETC & 0x20) 
	{
		bStereo = 2;
	}
	else
	{		
	 	bStereo = 1;
	}

	EventPtr->bMsgID		= VMD_MSG_MP3_ON;
	EventPtr->bChn			= 26;
	EventPtr->sdDeltaTime	= sdDeltaTime; 
	
	EventPtr->dPara1 = ((pStm->wSmplRate & 0x0000FFFF) << 16) + ((bStereo & 0x3)<<4) + ((dBitpersample & 0xFF)<<6);
	/*if(g_sdLstMainTime > sdCurTime)
		EventPtr->dPara2 = (sdCurTime + pStm->dTotalTime - g_sdLstMainTime);
	else
		EventPtr->dPara2 = pStm->dTotalTime;*/
	EventPtr->dPara2 = (bPan << 8) + bVol;
	EventPtr->dPara3 = 0;

	g_sdEndTime	= sdCurTime + pStm->dTotalTime;
	/*pQue = GetMsgFromQue();
	if(pQue)
	{
		pQue->bID		= VMD_MSG_MP3_OFF;
		pQue->bChn		= 26;
		pQue->sdTime 	= sdCurTime + pStm->dTotalTime; 
		SendMsgToQue(pQue);	//###
	}*/

	dDeltaTime = pStm->dTotalTime/0x7fff + ((pStm->dTotalTime%0x7fff)?1:0);
	for(dTimeCode=0; dTimeCode<dDeltaTime; dTimeCode++)
	{
		pQue = GetMsgFromQue();
		if(pQue)
		{
			pQue->bChn	= 26;
			if(dTimeCode == (dDeltaTime-1))
			{
				pQue->bID		= VMD_MSG_MP3_OFF;
				pQue->sdTime	= sdCurTime + (dTimeCode*0x7fff) + (pStm->dTotalTime%0x7fff);
			}
			else
			{
				pQue->bID		= VMD_MSG_NOP;
				pQue->sdTime	= sdCurTime + ((dTimeCode+1)*0x7fff);
			}
			SendMsgToQue(pQue);	//###
		}
	}

	if(g_VmdInfo.bEnabled & 0x0400)
	{
		g_VmdInfo.Mp3Blk.dOffSet	+= (pbPtr - bBuf);
	}
	else
	{
		g_VmdInfo.Mp3Blk.pbCurEvtPtr	= pbPtr;
	}
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Get a control event message.
		
Parameters:
		EventPtr:		Pointer to message structure.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT Get_Ctrl_Event(PVM_UNI_MSG EventPtr)
{
	HUGE UINT8* pbPtr = NULL;
	UINT8		bBuf[FS_READ_LEN], bCounter = 0, bCtrlID=0, bCtrlLen=0, bCtrlDat=0, bLedFlag=0, bMotorFlag=0;
	UINT32		dTimeCode   = 0, dDeltaTime = 0;
	SINT32		sdDeltaTime = 0, sdCurTime  = 0, dColor = 0;
	UINT32		dReadLen = FS_READ_LEN, dRealLen = 0;
	PMSGQUEUE	pQue = NULL;

	if(g_VmdInfo.bEnabled & 0x0400)
	{
		if(g_VmdInfo.CtrlBlk.dOffSet >= g_VmdInfo.CtrlBlk.dFBasOffSet + g_VmdInfo.CtrlBlk.dBufLen)
			return VRES_END_FLAG;
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, g_VmdInfo.CtrlBlk.dOffSet);
		pbPtr = (HUGE UINT8*)bBuf;
	}
	else
	{
		pbPtr 		= g_VmdInfo.CtrlBlk.pbCurPtr;
		if(pbPtr >= g_VmdInfo.CtrlBlk.pbBuf + g_VmdInfo.CtrlBlk.dBufLen)
		{
			return VRES_END_FLAG;
		}
	}

	dTimeCode	= *pbPtr++;
	dDeltaTime	= dTimeCode&0x7f;
	bCounter	= 0;
	while(dTimeCode&0x80)
	{
		bCounter++;
		dTimeCode   = *pbPtr++;
		dDeltaTime += (UINT16)(((UINT32)dTimeCode&0x7f)<<(7*bCounter));
	}

	sdDeltaTime	= (SINT32)dDeltaTime;
	sdCurTime	= sdDeltaTime + g_sdLstCtrlTime;			//###						//###

	sdDeltaTime	= sdCurTime   - g_sdLstMainTime;			//###***###

	if(g_sdLstMainTime < sdCurTime)	
		g_sdLstMainTime		= sdCurTime;					//###
	g_sdLstCtrlTime 		= sdCurTime;					//###
	
	if(sdDeltaTime < 0)
	{
		sdDeltaTime = 0;
	}

	if((*pbPtr++) != 0xff)
	{
		return VRES_ERROR;
	}
	else
	{
		bCtrlID 	= *pbPtr++;
		bCtrlLen 	= *pbPtr++;
		bCtrlDat 	= *pbPtr;
		bLedFlag	= bCtrlDat&0x08;
		bMotorFlag	= bCtrlDat&0x80;
		
		dColor = 0;
		dColor = bCtrlDat&0x07;
		if(8 == bCtrlID)
		{
			if(bLedFlag)
			{
				if(0 == dColor)	//should not appear
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_ON;
					EventPtr->dPara1		= 0;	//R
				}
				else if(1 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_ON;
					EventPtr->dPara1		= 0;	//R
				}
				else if(2 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_ON;
					EventPtr->dPara1		= 1;	//G
				}
				else if(3 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_ON;
					EventPtr->dPara1		= 0;	//R

					pQue = GetMsgFromQue();  
					if(pQue)
					{
						pQue->sdTime	= sdCurTime + sdDeltaTime;
						pQue->bID		= VMD_MSG_LED_ON;
						pQue->dPara1	= 1;		//G
						SendMsgToQue(pQue);
					}
				}
				else if(4 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_ON;
					EventPtr->dPara1		= 2;	//B
				}
				else if(5 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_ON;
					EventPtr->dPara1		= 0;	//R

					pQue = GetMsgFromQue();  
					if(pQue)
					{
						pQue->sdTime	= sdCurTime + sdDeltaTime;
						pQue->bID		= VMD_MSG_LED_ON;
						pQue->dPara1	= 2;		//B
						SendMsgToQue(pQue);
					}
				}
				else if(6 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_ON;
					EventPtr->dPara1		= 1;	//G

					pQue = GetMsgFromQue();  
					if(pQue)
					{
						pQue->sdTime	= sdCurTime + sdDeltaTime;
						pQue->bID		= VMD_MSG_LED_ON;
						pQue->dPara1	= 2;		//B
						SendMsgToQue(pQue);
					}
				}
				else if(7 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_ON;
					EventPtr->dPara1		= 0;	//R

					pQue = GetMsgFromQue();  
					if(pQue)
					{
						pQue->sdTime	= sdCurTime + sdDeltaTime;
						pQue->bID		= VMD_MSG_LED_ON;
						pQue->dPara1	= 1;		//G
						SendMsgToQue(pQue);
					}

					pQue = GetMsgFromQue();  
					if(pQue)
					{
						pQue->sdTime	= sdCurTime + sdDeltaTime;
						pQue->bID		= VMD_MSG_LED_ON;
						pQue->dPara1	= 2;		//B
						SendMsgToQue(pQue);
					}
				}
			}
			else if(bMotorFlag)	//led and motor should not be in the ctrl at the same time
			{
				EventPtr->sdDeltaTime	= sdDeltaTime;
				EventPtr->bMsgID		= VMD_MSG_MOTOR_ON;
			}
		}
		else if(9 == bCtrlID)
		{
			if(bLedFlag)
			{
				if(0 == dColor)	//should not appear
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_OFF;
					EventPtr->dPara1		= 0;	//R
				}
				else if(1 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_OFF;
					EventPtr->dPara1		= 0;	//R
				}
				else if(2 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_OFF;
					EventPtr->dPara1		= 1;	//G
				}
				else if(3 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_OFF;
					EventPtr->dPara1		= 0;	//R

					pQue = GetMsgFromQue();  
					if(pQue)
					{
						pQue->sdTime	= sdCurTime + sdDeltaTime;
						pQue->bID		= VMD_MSG_LED_OFF;
						pQue->dPara1	= 1;		//G
						SendMsgToQue(pQue);
					}
				}
				else if(4 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_OFF;
					EventPtr->dPara1		= 2;	//B
				}
				else if(5 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_OFF;
					EventPtr->dPara1		= 0;	//R

					pQue = GetMsgFromQue();  
					if(pQue)
					{
						pQue->sdTime	= sdCurTime + sdDeltaTime;
						pQue->bID		= VMD_MSG_LED_OFF;
						pQue->dPara1	= 2;		//B
						SendMsgToQue(pQue);
					}
				}
				else if(6 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_OFF;
					EventPtr->dPara1		= 1;	//G

					pQue = GetMsgFromQue();  
					if(pQue)
					{
						pQue->sdTime	= sdCurTime + sdDeltaTime;
						pQue->bID		= VMD_MSG_LED_OFF;
						pQue->dPara1	= 2;		//B
						SendMsgToQue(pQue);
					}
				}
				else if(7 == dColor)
				{
					EventPtr->sdDeltaTime	= sdDeltaTime;
					EventPtr->bMsgID		= VMD_MSG_LED_OFF;
					EventPtr->dPara1		= 0;	//R

					pQue = GetMsgFromQue();  
					if(pQue)
					{
						pQue->sdTime	= sdCurTime + sdDeltaTime;
						pQue->bID		= VMD_MSG_LED_OFF;
						pQue->dPara1	= 1;		//G
						SendMsgToQue(pQue);
					}

					pQue = GetMsgFromQue();  
					if(pQue)
					{
						pQue->sdTime	= sdCurTime + sdDeltaTime;
						pQue->bID		= VMD_MSG_LED_OFF;
						pQue->dPara1	= 2;		//B
						SendMsgToQue(pQue);
					}
				}
			}
			else if(bMotorFlag)
			{
				EventPtr->sdDeltaTime	= sdDeltaTime;
				EventPtr->bMsgID		= VMD_MSG_MOTOR_OFF;
			}
		}
		else
		{
		}

		pbPtr += bCtrlLen;		//###
			
		if(g_VmdInfo.bEnabled & 0x0400)
		{
			g_VmdInfo.CtrlBlk.dOffSet	+= (pbPtr - bBuf);
		}
		else
		{
			g_VmdInfo.CtrlBlk.pbCurPtr	= pbPtr;
		}
	}
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Get a queue message.
		
Parameters:
		EventPtr:		Pointer to message structure.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT Get_Que_Event(PVM_UNI_MSG EventPtr)
{
	if(g_WaitingHdr.pNext != NULL) 
	{
		GetWaitingEvent(EventPtr);
		return VRES_SUCCESS;
	}

	return VRES_END_FLAG;	
}


/*****************************************************************************
Description:
		Check mp3 head to judge whether it is valid.
		
Parameters:
		dHead:		mp3 head 
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT		CheckMp3Head(UINT32 dHead, UINT32 dTotalSize, HUGE UINT32 *pdTime)
{
	UINT8	bID = 0, bMpeg_25 = 0, bOption = 0, bSampleRateIndex = 0;
	UINT8	bProtection = 0, bBitrateIndex = 0, bPad = 0, bPrivateBit = 0;
	UINT8 	bChannelMode = 0, bModeExtension = 0, bCopyright = 0;
	UINT8	bOriginal = 0, bEmphasis = 0, bStereo = 0;
	UINT32 	dSize = 0, dSampleRate = 0, dBitrate = 0, dFrameSize = 0, dFrameNum = 0;
	UINT32	dTotalTime = 0;

    if(Sync_Mp3Check(dHead) != VRES_SUCCESS)
	{
		return VRES_ERROR;
	}
	
    if( dHead & (1<<20) ) 
	{
		bID = (dHead & (1<<19)) ? 0x0 : 0x1;
		bMpeg_25 = 0;
    }
    else 
	{
		bID = 1;
		bMpeg_25 = 1;
    }
    
    bOption = (UINT8) ((dHead>>17)&3);
    if( ((dHead>>10)&0x3) == 0x3) 
	{
		return VRES_ERROR;
    }
    if(bMpeg_25 == 1) 
	{
		bSampleRateIndex = 6 + (UINT8)((dHead>>10)&0x3);
    }
    else
	{
		bSampleRateIndex = (UINT8) ((dHead>>10)&0x3) + (bID*3);
	}
    bProtection = (UINT8)(((dHead>>16)&0x1)^0x1);
	
	bBitrateIndex = (UINT8) ((dHead>>12)&0xf);
    bPad          = (UINT8) ((dHead>>9)&0x1);
    bPrivateBit   = (UINT8) ((dHead>>8)&0x1);
    bChannelMode  = (UINT8) ((dHead>>6)&0x3);
    bModeExtension= (UINT8) ((dHead>>4)&0x3);
    bCopyright    = (UINT8) ((dHead>>3)&0x1);
    bOriginal     = (UINT8) ((dHead>>2)&0x1);
    bEmphasis     = (UINT8) (dHead & 0x3);
	
    bStereo    = (bChannelMode == VMD_MP3_MONO) ? 1 : 2;
	
	
	//Mp3 not support this format
    if(!bBitrateIndex) 
	{
		return VRES_ERROR;
    }
	
	dSampleRate = g_VmdMp3Frequency[bSampleRateIndex];
	dBitrate = g_VmdMp3Bitrate[bID][2][bBitrateIndex];
    if(bOption == 1) 
	{
		if(bID)
			dSize = (bStereo == 1) ? 9 : 17;
		else
			dSize = (bStereo == 1) ? 17 : 32;
		if(bProtection)
			dSize += 2;
		dFrameSize = (UINT32) g_VmdMp3Bitrate[bID][2][bBitrateIndex] * 144000;
		dFrameSize = dFrameSize /(g_VmdMp3Frequency[bSampleRateIndex]<<(bID));
		dFrameSize = dFrameSize + bPad;
		
		dFrameNum = dTotalSize / dFrameSize;
		//dTotalTime = dFrameNum * 26;
		dTotalTime = (dFrameSize) * 8 * dFrameNum/ (dBitrate);

		*pdTime	= dTotalTime;
	}
	else
	{
		//Mp3 not support layer 1 and layer 2
		*pdTime = 0;
		return VRES_ERROR;
	}
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Check whether mp3 head is valid.
		
Parameters:
		dHead:		Mp3 head 
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT		Sync_Mp3Check(UINT32 dHead)
{
	if( (dHead & 0xffe00000) != 0xffe00000)
		return VRES_ERROR;
    if(!((dHead>>17)&3))
		return VRES_ERROR;
    if( ((dHead>>12)&0xf) == 0xf)
		return VRES_ERROR;
    if( ((dHead>>10)&0x3) == 0x3 )
		return VRES_ERROR;

    return VRES_SUCCESS;
}


/***************************************************************
Description:
		Read Big File from ROM.

Parameters:
		pbBuffer: 		Point to data got from file stream
		dBufferSize: 	Data size except to read
		pdRealSize: 	Real bytes read from file stream
		dOffSet: 		Offset of file stream

Returns:
		  NULL
		  
****************************************************************/
VRESULT 	Fstm_ReadBuf(HUGE UINT8* pbBuffer, UINT32 dBufferSize, HUGE UINT32* pdRealsize, UINT32 dOffSet)
{
#if(1 == VFSTM)
	if(dOffSet > gLen)
		return VRES_ERROR;
	
	if((dOffSet+dBufferSize) > gLen)
		*pdRealsize = gLen - dOffSet;
	else
		*pdRealsize = dBufferSize;
	memmove(pbBuffer, gBuf+dOffSet, *pdRealsize);

	return VRES_SUCCESS;
#endif

	ReadBuffer((UINT8 *)pbBuffer, dBufferSize, (UINT32 *)pdRealsize, dOffSet);

	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Load the VMD data from file stream.
		
Parameters:
		File:			File stream pointer.
		Length:			File length.
		EmptyRes:		Empty Resource.
						It means event fifo, command fifo, stream0 - stream3 fifo 
						from bit0 to bit 5. 0 means unused, and 1 means used.
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT Fstm_Load(HUGE UINT8 *File, UINT32 Length, UINT32 EmptyRes)
{
	HUGE UINT8 *pbBuf = NULL;
	UINT8	bBuf[FS_READ_LEN], i = 0, bCmtLen = 0, bUdiLen = 0;
	UINT16  wNum = 0, wType = 0, wVersion = 0;
	UINT32	dOffSet = 0, dBlkID = 0, dBlkLen = 0, dCounter = 0, dBlkTime = 0, dTrkLen = 0, dTrkID = 0, dTrkNum = 0;
	UINT32	dReadLen = FS_READ_LEN, dRealLen = 0;
	
	dCounter = 0;
	dOffSet	 = 0;

	Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet);
	pbBuf = (HUGE UINT8 *)bBuf;

	dBlkID		= (((UINT32)pbBuf[3])<<24) + (((UINT32)pbBuf[2])<<16) + (((UINT32)pbBuf[1])<<8) + (UINT32)pbBuf[0];
	dBlkLen		= (((UINT32)pbBuf[7])<<24) + (((UINT32)pbBuf[6])<<16) + (((UINT32)pbBuf[5])<<8) + (UINT32)pbBuf[4];
	wVersion	= (((UINT16)pbBuf[9])<<8)  + (UINT16)pbBuf[8];
	wType		= (((UINT16)pbBuf[11])<<8) + (UINT16)pbBuf[10];
	wNum		= (((UINT16)pbBuf[13])<<8) + (UINT16)pbBuf[12];
	
	g_VmdInfo.wVmdFileVersion 	= wVersion;

	if(wVersion < VMD_STONE_VERSION)
	{
		bCmtLen	= 2;
		bUdiLen	= 2;
	}
	else
	{
		bCmtLen = 0;
		bUdiLen = 0;
	}

	if( (dBlkID!=VMHB) || ((wNum==1)&&(wType==1)) || ((wNum>1)&&(wType==0)) )	//control vmd file version
	{
		return VRES_VMD_INVALID_DATA;
	}
	else
	{
		g_VmdInfo.wBlkNum = (UINT8)wNum;
		dOffSet += 14;	//###
	}

	dBlkID		= (((UINT32)pbBuf[17])<<24) + (((UINT32)pbBuf[16])<<16) + (((UINT32)pbBuf[15])<<8) + (UINT32)pbBuf[14];
	/* function Fstm_LoadIdx is not stable. don't use it now */
	/*if(dBlkID == VMXB)
	{
		return Fstm_LoadIdx(EmptyRes, dOffSet, wVersion);
	}*/
	
	for(dCounter=0; dCounter<g_VmdInfo.wBlkNum; dCounter++)
	{
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet);
		pbBuf = (HUGE UINT8 *)bBuf;
	
		dBlkID	= (((UINT32)pbBuf[3])<<24) + (((UINT32)pbBuf[2])<<16) + (((UINT32)pbBuf[1])<<8) + (UINT32)pbBuf[0];
		dBlkLen	= (((UINT32)pbBuf[7])<<24) + (((UINT32)pbBuf[6])<<16) + (((UINT32)pbBuf[5])<<8) + (UINT32)pbBuf[4];
		switch(dBlkID)
		{
		case VMMB:	//midi, event fifo
			if(EmptyRes&0x01)
			{
				return VRES_ERROR;
			}
			if( ( ((((UINT16)pbBuf[9])<<8)	 + (UINT16)pbBuf[8])  == 0 ) && 
				( ((((UINT16)pbBuf[11])<<8)	 + (UINT16)pbBuf[10]) == 1 ) && 
				( ((((UINT32)pbBuf[15])<<24) + (((UINT32)pbBuf[14])<<16) + (((UINT32)pbBuf[13])<<8) + (UINT32)pbBuf[12]) == ETrk )
			  )
			{
				g_VmdInfo.bEnabled		   	   |= 0x01;
				g_VmdInfo.MidiBlk.dFBasOffSet	= dOffSet+20;
				g_VmdInfo.MidiBlk.dFCurOffSet	= dOffSet+20;
				if(g_VmdInfo.bEnabled & 0x0400)
				{
					g_VmdInfo.MidiBlk.dOffSet	= dOffSet+20;
				}
				else
				{
					g_VmdInfo.MidiBlk.dOffSet	= 0;
				}
				g_VmdInfo.MidiBlk.dBufLen	= (((UINT32)pbBuf[19])<<24) + (((UINT32)pbBuf[18])<<16)
							+ (((UINT32)pbBuf[17])<<8) + (UINT32)pbBuf[16];
				
				dBlkTime = Fstm_ScanMIDIBlk(dOffSet+20, g_VmdInfo.MidiBlk.dBufLen);
				g_VmdInfo.dTotalTime		= dBlkTime>g_VmdInfo.dTotalTime?dBlkTime:g_VmdInfo.dTotalTime;

				dOffSet	+= (dBlkLen+8);
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		case VMSB:	//wave, event+stream fifo
			if(EmptyRes&0x01)
			{
				return VRES_ERROR;
			}
			if( ( (((UINT16)pbBuf[9])<<8) 	 + (UINT16)pbBuf[8] == 1 ) && 
				( (((UINT16)pbBuf[11])<<8) 	 + (UINT16)pbBuf[10] >= 2 ) &&
				( ((((UINT32)pbBuf[15])<<24) + (((UINT32)pbBuf[14])<<16) + (((UINT32)pbBuf[13])<<8) + (UINT32)pbBuf[12]) == ETrk )
			   )
			{
				g_VmdInfo.bEnabled	|= 0x02;
				dBlkTime = Fstm_ScanStreamBlk(dOffSet);
				g_VmdInfo.dTotalTime = dBlkTime>g_VmdInfo.dTotalTime?dBlkTime:g_VmdInfo.dTotalTime;

				if((4 - (EmptyRes&0x04) - (EmptyRes&0x08) - (EmptyRes&0x10) - (EmptyRes&0x20))<g_VmdInfo.WaveBlk.bEvtTrkNum)
				{
					return VRES_ERROR;
				}

				dOffSet	+= (dBlkLen+8);
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		case VMEB:	//udse, event+stream fifo
			if( ( (((UINT16)pbBuf[9])<<8) 	 + (UINT16)pbBuf[8] == 1 ) && 
				( (((UINT16)pbBuf[11])<<8) 	 + (UINT16)pbBuf[10] == 2 ) &&
				( ((((UINT32)pbBuf[15])<<24) + (((UINT32)pbBuf[14])<<16) + (((UINT32)pbBuf[13])<<8) + (UINT32)pbBuf[12]) == ETrk )
			   )
			{
				g_VmdInfo.bEnabled			   |= 0x04;
				g_VmdInfo.UdseBlk.dEvtBufLen	= (((UINT32)pbBuf[19])<<24)+ (((UINT32)pbBuf[18])<<16) 
					 + (((UINT32)pbBuf[17])<<8) + (UINT32)pbBuf[16];
				
				g_VmdInfo.UdseBlk.dFBasEvtOffSet	= dOffSet+20;
				g_VmdInfo.UdseBlk.dFCurEvtOffSet	= dOffSet+20;
				if(g_VmdInfo.bEnabled & 0x0400)
				{
					g_VmdInfo.UdseBlk.dOffSet		= dOffSet+20;				
				}
				else
				{
					g_VmdInfo.UdseBlk.dOffSet		= 0;
				}

				Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, (dOffSet+20+g_VmdInfo.UdseBlk.dEvtBufLen));
				pbBuf = (HUGE UINT8 *)bBuf;
				
				g_VmdInfo.UdseBlk.dDatBufLen	= (((UINT32)pbBuf[7])<<24)+ (((UINT32)pbBuf[6])<<16) 
					  + (((UINT32)pbBuf[5])<<8) + (UINT32)pbBuf[4];
				
				g_VmdInfo.UdseBlk.dFBasDatOffSet	= (dOffSet+20+g_VmdInfo.UdseBlk.dEvtBufLen);
				g_VmdInfo.UdseBlk.dFCurDatOffSet	= (dOffSet+20+g_VmdInfo.UdseBlk.dEvtBufLen);

				dOffSet	+= (dBlkLen+8);
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		case VMIB:	//udi, event+stream fifo
			if(EmptyRes&0x01)
			{
				return VRES_ERROR;
			}
			if( ( (((UINT16)pbBuf[9])<<8)	+ (UINT16)pbBuf[8]  == 0 ) && 
				( (((UINT16)pbBuf[11])<<8)	+ (UINT16)pbBuf[10] == 1 ) &&
				( (((UINT32)pbBuf[15+bUdiLen])<<24)	+ (((UINT32)pbBuf[14+bUdiLen])<<16) + (((UINT32)pbBuf[13+bUdiLen])<<8) + (UINT32)pbBuf[12+bUdiLen] == DTrk )
			   )
			{
				g_VmdInfo.bEnabled		   		|= 0x80;
				g_VmdInfo.UdiBlk.dFBasOffSet	= dOffSet+22;
				g_VmdInfo.UdiBlk.dFCurOffSet	= dOffSet+22;
				if(wVersion < VMD_STONE_VERSION)
					g_VmdInfo.UdiBlk.wMapWord	= (((UINT16)pbBuf[13])<<8)	+ (UINT16)pbBuf[12];
				else
					g_VmdInfo.UdiBlk.wMapWord	= 0;
				g_VmdInfo.UdiBlk.dBufLen		= (((UINT32)pbBuf[19+bUdiLen])<<24)	+ (((UINT32)pbBuf[18+bUdiLen])<<16) 
					+ (((UINT32)pbBuf[17+bUdiLen])<<8) + (UINT32)pbBuf[16+bUdiLen];

				if((4 - (EmptyRes&0x04) - (EmptyRes&0x08) - (EmptyRes&0x10) - (EmptyRes&0x20))<1)
				{
					return VRES_ERROR;
				}

				dOffSet	+= (dBlkLen+8);
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		case VMCB:	//comment info
			dTrkID	= 0;
			dTrkLen	= 0;
			dTrkNum	= (((UINT16)pbBuf[11])<<8) + (UINT16)pbBuf[10];	
			if(wVersion < VMD_STONE_VERSION)
				g_VmdInfo.CmtBlk.wEncoding 	= (((UINT16)pbBuf[13])<<8) + (UINT16)pbBuf[12];	
			else
				g_VmdInfo.CmtBlk.wEncoding	= 0;
			g_VmdInfo.bEnabled	|= 0x10;
			dOffSet += (12+bCmtLen);	//###

			for(i=0; i<dTrkNum; i++)
			{
				Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet);
				pbBuf = (HUGE UINT8 *)bBuf;
				
				dTrkID	= (((UINT32)pbBuf[3])<<24) + (((UINT32)pbBuf[2])<<16) + (((UINT32)pbBuf[1])<<8) + (UINT32)pbBuf[0];
				dTrkLen	= (((UINT32)pbBuf[7])<<24) + (((UINT32)pbBuf[6])<<16) + (((UINT32)pbBuf[5])<<8) + (UINT32)pbBuf[4];
				switch(dTrkID)
				{
				case TTrk:
					g_VmdInfo.CmtBlk.dFTBasOffSet	= dOffSet+8;
					g_VmdInfo.CmtBlk.dTitleLen		= (dTrkLen > CMT_T_LEN)?(CMT_T_LEN):(dTrkLen);
					Fstm_ReadBuf(g_VmdInfo.CmtBlk.bTitleBuf, g_VmdInfo.CmtBlk.dTitleLen, (HUGE UINT32 *)&dRealLen, dOffSet+8);
					break;
				case ATrk:
					g_VmdInfo.CmtBlk.dFABasOffSet	= dOffSet+8;
					g_VmdInfo.CmtBlk.dAuthorLen		= (dTrkLen > CMT_A_LEN)?(CMT_A_LEN):(dTrkLen);
					Fstm_ReadBuf(g_VmdInfo.CmtBlk.bAuthorBuf, g_VmdInfo.CmtBlk.dAuthorLen, (HUGE UINT32 *)&dRealLen, dOffSet+8);					
					break;
				case CTrk:
					g_VmdInfo.CmtBlk.dFCBasOffSet	= dOffSet+8;
					g_VmdInfo.CmtBlk.dCmtLen		= (dTrkLen > CMT_C_LEN)?(CMT_C_LEN):(dTrkLen);
					Fstm_ReadBuf(g_VmdInfo.CmtBlk.bCmtBuf, g_VmdInfo.CmtBlk.dCmtLen, (HUGE UINT32 *)&dRealLen, dOffSet+8);					
					break;
				}
				dOffSet += (dTrkLen + 8);
			}
			break;
		case VM3B:	//mp3, event+stream fifo
			if(EmptyRes&0x01)
			{
				return VRES_ERROR;
			}
			if( ( (((UINT16)pbBuf[9])<<8)	+ (UINT16)pbBuf[8]  == 1 ) && 
				( (((UINT16)pbBuf[11])<<8)	+ (UINT16)pbBuf[10] >= 2 ) &&
				( (((UINT32)pbBuf[15])<<24)	+ (((UINT32)pbBuf[14])<<16) + (((UINT32)pbBuf[13])<<8) + (UINT32)pbBuf[12] == ETrk )
			   )
			{
				g_VmdInfo.bEnabled 	|= 0x20;
				dBlkTime = Fstm_ScanMp3Blk(dOffSet);
				g_VmdInfo.dTotalTime = dBlkTime>g_VmdInfo.dTotalTime?dBlkTime:g_VmdInfo.dTotalTime;

				if((4 - (EmptyRes&0x04) - (EmptyRes&0x08) - (EmptyRes&0x10) - (EmptyRes&0x20))<1)
				{
					return VRES_ERROR;
				}

				dOffSet	+= (dBlkLen+8);
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		case VMKB:	//control, event fifo
			if(EmptyRes&0x01)	//perhaps modify later
			{
				return VRES_ERROR;
			}
			if( ( (((UINT16)pbBuf[9])<<8)	+ (UINT16)pbBuf[8]  == 0 ) && 
				( (((UINT16)pbBuf[11])<<8)	+ (UINT16)pbBuf[10] == 1 ) &&
				( (((UINT32)pbBuf[15])<<24)	+ (((UINT32)pbBuf[14])<<16) + (((UINT32)pbBuf[13])<<8) + (UINT32)pbBuf[12] == ETrk )
			   )
			{
				g_VmdInfo.CtrlBlk.dFBasOffSet	= dOffSet+20;
				g_VmdInfo.CtrlBlk.dFCurOffSet	= dOffSet+20;
				if(g_VmdInfo.bEnabled & 0x0400)
				{
					g_VmdInfo.CtrlBlk.dOffSet 	= dOffSet+20;
				}
				else
				{
					g_VmdInfo.CtrlBlk.dOffSet	= 0;
				}
				g_VmdInfo.CtrlBlk.dBufLen		= (((UINT32)pbBuf[19])<<24)+ (((UINT32)pbBuf[18])<<16) 
					+ (((UINT32)pbBuf[17])<<8) + (UINT32)pbBuf[16];

				Fstm_ScanCtrlBlk(dOffSet+20, g_VmdInfo.CtrlBlk.dBufLen);

				dOffSet	+= (dBlkLen+8);
			}
			else
			{
				return VRES_VMD_INVALID_DATA;
			}
			break;
		default:
			dOffSet += (dBlkLen+8);
			break;
		}
	}
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Load the VMD data include index block from file stream.
		
Parameters:
		File:			File stream pointer.
		EmptyRes:		Empty Resource.
						It means event fifo, command fifo, stream0 - stream3 fifo 
						from bit0 to bit 5. 0 means unused, and 1 means used.
		OffSet:			begin offset
		Version:		file version
		
Returns:
		Return VRES_SUCCESS if success, else return error value.
		
******************************************************************************/
VRESULT Fstm_LoadIdx(UINT32 EmptyRes, UINT32 OffSet, UINT16 Version)
{
	HUGE UINT8 *pbBuf = NULL;
	HUGE UINT8 *pbCmtBuf = NULL;
	UINT8	bBuf[128], bCmtBuf[FS_READ_LEN], i = 0, j= 0, bCmtLen = 0, bUdiLen = 0, bTmp = 0;
	UINT16  wNum = 0, wType = 0;
	UINT32	dOffSet = 0, dBlkID = 0, dBlkLen = 0, dBlkTime = 0, dTrkLen = 0, dTrkID = 0, dTrkNum = 0;
	UINT32	dReadLen = 128, dRealLen = 0;
	
	Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet);
	pbBuf = (HUGE UINT8 *)bBuf;

	dBlkID		= (((UINT32)pbBuf[3])<<24) + (((UINT32)pbBuf[2])<<16) + (((UINT32)pbBuf[1])<<8) + (UINT32)pbBuf[0];
	dBlkLen		= (((UINT32)pbBuf[7])<<24) + (((UINT32)pbBuf[6])<<16) + (((UINT32)pbBuf[5])<<8) + (UINT32)pbBuf[4];
	wType		= (((UINT16)pbBuf[9])<<8)  + (UINT16)pbBuf[8];
	wNum		= (((UINT16)pbBuf[11])<<8) + (UINT16)pbBuf[10];
	
	if(Version < VMD_STONE_VERSION)
	{
		bCmtLen	= 2;
		bUdiLen	= 2;
	}
	else
	{
		bCmtLen = 0;
		bUdiLen = 0;
	}

	if( (dBlkID!=VMXB) || ((wNum==1)&&(wType==1)) || ((wNum>1)&&(wType==0)) )	//control vmd file version
	{
		return VRES_VMD_INVALID_DATA;
	}

	for(i=0; i<g_VmdInfo.wBlkNum; i++)
	{
		bTmp 	= (20 + i*12);
		dBlkID	= (((UINT32)pbBuf[3+bTmp])<<24) + (((UINT32)pbBuf[2+bTmp])<<16) + (((UINT32)pbBuf[1+bTmp])<<8) + (UINT32)pbBuf[bTmp];
		dOffSet	= (((UINT32)pbBuf[7+bTmp])<<24) + (((UINT32)pbBuf[6+bTmp])<<16) + (((UINT32)pbBuf[5+bTmp])<<8) + (UINT32)pbBuf[4+bTmp];
		dBlkLen	= (((UINT32)pbBuf[11+bTmp])<<24) + (((UINT32)pbBuf[10+bTmp])<<16) + (((UINT32)pbBuf[9+bTmp])<<8) + (UINT32)pbBuf[8+bTmp];
		switch(dBlkID)
		{
			case VMMB:
				if(EmptyRes&0x01)
				{
					return VRES_ERROR;
				}
				g_VmdInfo.bEnabled		   	   |= 0x01;
				g_VmdInfo.MidiBlk.dFBasOffSet	= dOffSet;
				g_VmdInfo.MidiBlk.dFCurOffSet	= dOffSet;
				if(g_VmdInfo.bEnabled & 0x0400)
				{
					g_VmdInfo.MidiBlk.dOffSet	= dOffSet;
				}
				else
				{
					g_VmdInfo.MidiBlk.dOffSet	= 0;
				}
				g_VmdInfo.MidiBlk.dBufLen	= dBlkLen - 20;
				
				dBlkTime = Fstm_ScanMIDIBlk(dOffSet+20, g_VmdInfo.MidiBlk.dBufLen);
				g_VmdInfo.dTotalTime		= dBlkTime>g_VmdInfo.dTotalTime?dBlkTime:g_VmdInfo.dTotalTime;
				break;
				
			case VMSB:
				if(EmptyRes&0x01)
				{
					return VRES_ERROR;
				}
				g_VmdInfo.bEnabled	|= 0x02;
				dBlkTime = Fstm_ScanStreamBlk(dOffSet);
				g_VmdInfo.dTotalTime = dBlkTime>g_VmdInfo.dTotalTime?dBlkTime:g_VmdInfo.dTotalTime;

				if((4 - (EmptyRes&0x04) - (EmptyRes&0x08) - (EmptyRes&0x10) - (EmptyRes&0x20))<g_VmdInfo.WaveBlk.bEvtTrkNum)
				{
					return VRES_ERROR;
				}
				break;
			case VMIB:
				if(EmptyRes&0x01)
				{
					return VRES_ERROR;
				}
				g_VmdInfo.bEnabled		   		|= 0x80;
				if(Version < VMD_STONE_VERSION)
				{
					g_VmdInfo.UdiBlk.wMapWord	= 0;	//###
					g_VmdInfo.UdiBlk.dFBasOffSet	= dOffSet+22;
					g_VmdInfo.UdiBlk.dFCurOffSet	= dOffSet+22;
					g_VmdInfo.UdiBlk.dBufLen		= dBlkLen-22;
				}
				else
				{
					g_VmdInfo.UdiBlk.wMapWord	= 0;
					g_VmdInfo.UdiBlk.dFBasOffSet	= dOffSet+20;
					g_VmdInfo.UdiBlk.dFCurOffSet	= dOffSet+20;
					g_VmdInfo.UdiBlk.dBufLen		= dBlkLen-20;
				}

				if((4 - (EmptyRes&0x04) - (EmptyRes&0x08) - (EmptyRes&0x10) - (EmptyRes&0x20))<1)
				{
					return VRES_ERROR;
				}
				break;
			case VMCB:
				Fstm_ReadBuf((HUGE UINT8 *)bCmtBuf, FS_READ_LEN, (HUGE UINT32 *)&dRealLen, dOffSet);
				pbCmtBuf = (HUGE UINT8 *)bCmtBuf;
				dTrkID	= 0;
				dTrkLen	= 0;
				dTrkNum	= (((UINT16)pbCmtBuf[11])<<8) + (UINT16)pbCmtBuf[10];	
				if(Version < VMD_STONE_VERSION)
					g_VmdInfo.CmtBlk.wEncoding 	= (((UINT16)pbCmtBuf[13])<<8) + (UINT16)pbCmtBuf[12];	
				else
					g_VmdInfo.CmtBlk.wEncoding	= 0;
				g_VmdInfo.bEnabled	|= 0x10;
				dOffSet += (12+bCmtLen);	//###

				for(j=0; j<dTrkNum; j++)
				{
					Fstm_ReadBuf((HUGE UINT8 *)bCmtBuf, FS_READ_LEN, (HUGE UINT32 *)&dRealLen, dOffSet);
					pbCmtBuf = (HUGE UINT8 *)bCmtBuf;
				
					dTrkID	= (((UINT32)pbCmtBuf[3])<<24) + (((UINT32)pbCmtBuf[2])<<16) + (((UINT32)pbCmtBuf[1])<<8) + (UINT32)pbCmtBuf[0];
					dTrkLen	= (((UINT32)pbCmtBuf[7])<<24) + (((UINT32)pbCmtBuf[6])<<16) + (((UINT32)pbCmtBuf[5])<<8) + (UINT32)pbCmtBuf[4];
					switch(dTrkID)
					{
					case TTrk:
						g_VmdInfo.CmtBlk.dFTBasOffSet	= dOffSet+8;
						g_VmdInfo.CmtBlk.dTitleLen		= (dTrkLen > CMT_T_LEN)?(CMT_T_LEN):(dTrkLen);
						Fstm_ReadBuf(g_VmdInfo.CmtBlk.bTitleBuf, g_VmdInfo.CmtBlk.dTitleLen, (HUGE UINT32 *)&dRealLen, dOffSet+8);
						break;
					case ATrk:
						g_VmdInfo.CmtBlk.dFABasOffSet	= dOffSet+8;
						g_VmdInfo.CmtBlk.dAuthorLen		= (dTrkLen > CMT_A_LEN)?(CMT_A_LEN):(dTrkLen);
						Fstm_ReadBuf(g_VmdInfo.CmtBlk.bAuthorBuf, g_VmdInfo.CmtBlk.dAuthorLen, (HUGE UINT32 *)&dRealLen, dOffSet+8);					
						break;
					case CTrk:
						g_VmdInfo.CmtBlk.dFCBasOffSet	= dOffSet+8;
						g_VmdInfo.CmtBlk.dCmtLen		= (dTrkLen > CMT_C_LEN)?(CMT_C_LEN):(dTrkLen);
						Fstm_ReadBuf(g_VmdInfo.CmtBlk.bCmtBuf, g_VmdInfo.CmtBlk.dCmtLen, (HUGE UINT32 *)&dRealLen, dOffSet+8);					
						break;
					}
					dOffSet += (dTrkLen + 8);
				}
				break;
			case VM3B:
				if(EmptyRes&0x01)
				{
					return VRES_ERROR;
				}
				g_VmdInfo.bEnabled 	|= 0x20;
				dBlkTime = Fstm_ScanMp3Blk(dOffSet);
				g_VmdInfo.dTotalTime = dBlkTime>g_VmdInfo.dTotalTime?dBlkTime:g_VmdInfo.dTotalTime;

				if((4 - (EmptyRes&0x04) - (EmptyRes&0x08) - (EmptyRes&0x10) - (EmptyRes&0x20))<1)
				{
					return VRES_ERROR;
				}
				break;
			case VMKB:
				if(EmptyRes&0x01)	//perhaps modify later
				{
					return VRES_ERROR;
				}
				g_VmdInfo.CtrlBlk.dFBasOffSet	= dOffSet+20;
				g_VmdInfo.CtrlBlk.dFCurOffSet	= dOffSet+20;
				if(g_VmdInfo.bEnabled & 0x0400)
				{
					g_VmdInfo.CtrlBlk.dOffSet 	= dOffSet+20;
				}
				else
				{
					g_VmdInfo.CtrlBlk.dOffSet	= 0;
				}
				g_VmdInfo.CtrlBlk.dBufLen		= dBlkLen-20;
				Fstm_ScanCtrlBlk(dOffSet+20, g_VmdInfo.CtrlBlk.dBufLen);
				break;
		}
	}
	
	return VRES_SUCCESS;
}


/*****************************************************************************
Description:
		Scan the MIDI block of VMD file stream.
		
Parameters:
		dBasOffset:	 Offset of MIDI block in vmd file.
		dBufLen:	 Length of Midi block.
		
Returns:
		Total time of the MIDI block.
		
******************************************************************************/
UINT32 Fstm_ScanMIDIBlk(UINT32 dBasOffSet, UINT32 dBufLen)
{
	HUGE UINT8 *pbPtr = NULL;
	HUGE UINT8 *pbBK = NULL;	
	UINT8  	bBuf[FS_READ_LEN], bMsgID = 0, bCounter = 0, bTimeCode = 0, bCtrlID = 0, bLstMsgCode = 0, bLstCtrlCode = 0;
	UINT32 	dDeltaTime = 0, dLstLen = 0, dScanLen = 0;
	UINT32 	dTime = 0, dCurLen = 0, dReadLen = FS_READ_LEN, dRealLen = 0, dOffSet = 0;
	
	dTime	= 0;
	dOffSet = dBasOffSet;
	pbPtr	= bBuf;

	while(dCurLen < dBufLen)
	{
		if(dCurLen && (dLstLen < (dReadLen-10)))
		{
		}
		else
		{
			Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet+dCurLen);
			pbPtr = (HUGE UINT8 *)bBuf;
			dScanLen = 0;
			dLstLen  = 0;
		}
		pbBK = pbPtr;
		
		bTimeCode	= *pbPtr++;
		dDeltaTime	= bTimeCode&0x7f;
		bCounter=0;
		while(bTimeCode&0x80)
		{
			bCounter++;
			bTimeCode	= *pbPtr++;
			dDeltaTime += (UINT16)(((UINT32)bTimeCode&0x7f)<<(7*bCounter));
		}
		dTime += dDeltaTime;
		
		bMsgID = *pbPtr++;

		if((bMsgID&0x80)==0)
		{
			if(bLstMsgCode&0xf0==0xB0)
				bCtrlID = bLstCtrlCode;

			bMsgID = bLstMsgCode;
			pbPtr--; 
		}

		if((bMsgID&0xf0)==0x80)	//(B0) control message in midi
		{
			bCtrlID = bMsgID;
			bMsgID  = bLstMsgCode;
			pbPtr--;
		}
		bLstMsgCode = bMsgID;

		switch(bMsgID&0xf0)
		{
		case 0x90:
			pbPtr	 += 2;
			bTimeCode = *pbPtr++;
			while(bTimeCode&0x80)
				bTimeCode = *pbPtr++;
			break;

		case 0xC0:
			pbPtr++;
			break;

		case 0xD0:
			pbPtr++;
			break;

		case 0xE0:
			pbPtr += 2;
			break;
		case 0xB0:
			bCtrlID = *pbPtr++;
			if((bCtrlID&0x80)==0)
			{
				bCtrlID = bLstCtrlCode;
				pbPtr--;
			}
			bLstCtrlCode = bCtrlID;
			pbPtr++;
			break;
		}

		dScanLen = (pbPtr-pbBK);
		dLstLen	+= dScanLen;
		dCurLen += dScanLen;
	}

	return dTime;
}

/*****************************************************************************
Description:
		Scan the wave block of VMD file stream.
		
Parameters:
		dBasOffset:	 Offset of wave block in vmd file.
		
Returns:
		Total time of the wave block.
		
******************************************************************************/
UINT32 Fstm_ScanStreamBlk(UINT32 dBasOffSet)
{
	HUGE UINT8  *pbPtr = NULL;
	HUGE UINT8  *pbBK = NULL;	
	UINT8		bBuf[FS_READ_LEN], bCounter = 0, bWaveID = 0, bTimeCode = 0, bEventNum = 0;
	SINT8		sbDatNum = 0;
	UINT16		wTrkNum = 0;
	UINT32		dSize = 0, dFenzi = 0, dFenmu = 0, dLstLen = 0, dScanLen = 0;
	UINT32		dOffSet = 0, dBlkTime = 0, dDeltaTime = 0, dTotalTime = 0, dDuration = 0, dTrkID = 0, dTrkSize = 0, dCutTime = 0, dAddTime = 0;
	UINT32		dReadLen = FS_READ_LEN, dRealLen = 0, dCurLen = 0;

	dOffSet	 = dBasOffSet;
	Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet);
	pbPtr = (HUGE UINT8 *)bBuf;
	
	wTrkNum		= (((UINT16)pbPtr[11])<<8) + (UINT16)pbPtr[10];
	
	dOffSet	   += 12;	//###
	bWaveID		= 0;
	bEventNum	= 0;
	sbDatNum	= 0;

	dCutTime	= 0;
	dAddTime	= 0;

	for(bCounter=0; bCounter<wTrkNum; bCounter++)
	{
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet);
		pbPtr = (HUGE UINT8 *)bBuf;
		
		dTrkID		= (((UINT32)pbPtr[3])<<24) + (((UINT32)pbPtr[2])<<16) + (((UINT32)pbPtr[1])<<8) + (UINT32)pbPtr[0];
		dTrkSize	= (((UINT32)pbPtr[7])<<24) + (((UINT32)pbPtr[6])<<16) + (((UINT32)pbPtr[5])<<8) + (UINT32)pbPtr[4];

		if(dTrkID==ETrk)
		{
			g_VmdInfo.WaveBlk.dFBasOffSet		= (dOffSet + 8);
			g_VmdInfo.WaveBlk.dFCurOffSet		= (dOffSet + 8);
			if(g_VmdInfo.bEnabled & 0x0400)
			{
				g_VmdInfo.WaveBlk.dOffSet		= (dOffSet + 8);
			}
			else
			{
				g_VmdInfo.WaveBlk.dOffSet		= 0;
			}
			g_VmdInfo.WaveBlk.dEvtBufLen		= dTrkSize;
		}
		else
		{
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].bFormat		= *(pbPtr+8);
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].bChn			= *(pbPtr+9);
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].bBitsPerSmpl	= *(pbPtr+10);
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].bSigned		= *(pbPtr+11);
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].wSmplRate		= (((UINT16)pbPtr[13])<<8) + (UINT16)pbPtr[12];
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].dFBasOffSet	= (dOffSet + 14);
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].dFCurOffSet	= (dOffSet + 14);
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].dBufLen		= dTrkSize-6;
			if(g_VmdInfo.bEnabled & 0x0400)
			{
				g_VmdInfo.WaveBlk.DatBuf[bWaveID].dOffSet	= (dOffSet + 14);
			}
			else
			{
				g_VmdInfo.WaveBlk.DatBuf[bWaveID].dOffSet	= 0;
			}

			dSize = (dTrkSize-6) * ((1==g_VmdInfo.WaveBlk.DatBuf[bWaveID].bFormat)?4:1);
			if(dSize < WAVE_MAX_LEN)
			{
				dFenzi	= dSize * 8000;
				/* dFenmu	= (*(pbPtr+9)) * (*(pbPtr+10)) * ( (((UINT16)pbPtr[13])<<8) + (UINT16)pbPtr[12] ); */
				/* must be 16 bitsmple and mono */
				dFenmu	= 16 * 1 * ( (((UINT16)pbPtr[13])<<8) + (UINT16)pbPtr[12] );
			}
			else if(dSize < WAVE_MAX_LEN1)
			{
				dFenzi	= dSize * 500;
				dFenmu	= ( (((UINT16)pbPtr[13])<<8) + (UINT16)pbPtr[12] );
			}
			else
			{
				dFenzi	= dSize; 
				dFenmu	= ( (((UINT16)pbPtr[13])<<8) + (UINT16)pbPtr[12] );
				dFenmu /= 500;
			}
			g_VmdInfo.WaveBlk.DatBuf[bWaveID].dTotalTime	= dFenzi/dFenmu;

			bWaveID++;
		}
	
		dOffSet += (dTrkSize + 8);
	}

	dOffSet = (dBasOffSet+12);
	Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet);
	pbPtr = (HUGE UINT8 *)bBuf;
	dTrkSize = (((UINT32)pbPtr[7])<<24) + (((UINT32)pbPtr[6])<<16) + (((UINT32)pbPtr[5])<<8) + (UINT32)pbPtr[4];

	dDeltaTime	=  0;
	dTotalTime	=  0;
	dBlkTime	=  0;
	
	dCurLen 	=  0;
	dOffSet 	+= 8;	//###

	while(dCurLen < dTrkSize)
	{
		if(dCurLen && (dLstLen < (dReadLen-10)))
		{
		}
		else
		{
			Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet+dCurLen);	//###
			pbPtr = (HUGE UINT8 *)bBuf;
			dScanLen = 0;
			dLstLen  = 0;
		}
		pbBK = pbPtr;
	
		bTimeCode  = *pbPtr++;
		dDeltaTime = bTimeCode&0x7f;
		bCounter=0;
		while(bTimeCode&0x80)
		{
			bCounter++;
			bTimeCode = *pbPtr++;
			dDeltaTime += (UINT16)(((UINT32)bTimeCode&0x7f)<<(7*bCounter));
		}
		
		switch(*pbPtr++)
		{
		case 0xA0:
			bWaveID = *pbPtr++;
			bEventNum |= 0x01;
			sbDatNum = g_VmdInfo.WaveBlk.sbStmTrkFlag[0][2]++;
			g_VmdInfo.WaveBlk.bStmTrkIndex[0][sbDatNum] 	= bWaveID;
			break;
		case 0xA1:
			bWaveID = *pbPtr++;
			bEventNum |= 0x02;
			sbDatNum = g_VmdInfo.WaveBlk.sbStmTrkFlag[1][2]++;
			g_VmdInfo.WaveBlk.bStmTrkIndex[1][sbDatNum] 	= bWaveID;	
			break;
		case 0xA2:
			bWaveID = *pbPtr++;
			bEventNum |= 0x04;
			sbDatNum = g_VmdInfo.WaveBlk.sbStmTrkFlag[2][2]++;
			g_VmdInfo.WaveBlk.bStmTrkIndex[2][sbDatNum] 	= bWaveID;	
			break;
		case 0xA3:
			bWaveID = *pbPtr++;
			bEventNum |= 0x08;
			sbDatNum = g_VmdInfo.WaveBlk.sbStmTrkFlag[3][2]++;
			g_VmdInfo.WaveBlk.bStmTrkIndex[3][sbDatNum] 	= bWaveID;	
			break;
		default:
			bWaveID = 0xff;
			break;
		}

		if(g_VmdInfo.wVmdFileVersion >= VMD_STONE_VERSION)
		{
			pbPtr++;	/* vol */
			pbPtr++;	/* pan */
		}

		if(bWaveID != 0xff)	//##########
		{
			dSize = g_VmdInfo.WaveBlk.DatBuf[bWaveID].dBufLen * ((1==g_VmdInfo.WaveBlk.DatBuf[bWaveID].bFormat)?4:1);
			if(dSize < WAVE_MAX_LEN)
			{
				dFenzi = dSize * 8000;
				/* dFenmu = g_VmdInfo.WaveBlk.DatBuf[bWaveID].bBitsPerSmpl * g_VmdInfo.WaveBlk.DatBuf[bWaveID].bChn * g_VmdInfo.WaveBlk.DatBuf[bWaveID].wSmplRate; */
				/* must be 16 bitsmple and mono */
				dFenmu = 16 * 1 * g_VmdInfo.WaveBlk.DatBuf[bWaveID].wSmplRate;
			}
			else if(dSize < WAVE_MAX_LEN1)
			{
				dFenzi = dSize * 500;
				dFenmu = g_VmdInfo.WaveBlk.DatBuf[bWaveID].wSmplRate;
			}
			else
			{
				dFenzi = dSize; 
				dFenmu = g_VmdInfo.WaveBlk.DatBuf[bWaveID].wSmplRate;
				dFenmu /= 500;
			}
			dDuration = dFenzi/dFenmu;
			
			dTotalTime  += dDeltaTime;
			dBlkTime	 = dBlkTime<(dTotalTime +dDuration)?(dTotalTime +dDuration):dBlkTime;
		}

		dScanLen = (pbPtr-pbBK);
		dLstLen	+= dScanLen;
		dCurLen += dScanLen;
	}

	g_VmdInfo.WaveBlk.bEvtTrkNum 	= 0;
	if(bEventNum & 0x01)
		g_VmdInfo.WaveBlk.bEvtTrkNum++;
	if(bEventNum & 0x02)
		g_VmdInfo.WaveBlk.bEvtTrkNum++;
	if(bEventNum & 0x04)
		g_VmdInfo.WaveBlk.bEvtTrkNum++;
	if(bEventNum & 0x08)
		g_VmdInfo.WaveBlk.bEvtTrkNum++;

	return dBlkTime;
}

/*****************************************************************************
Description:
		Scan the mp3 block of VMD by use of file stream.
		
Parameters:
		dBasOffSet:		Offset of mp3 block in vmd file.
		
Returns:
		Total time of the mp3 block.
		
******************************************************************************/
UINT32 Fstm_ScanMp3Blk(UINT32	dBasOffSet)
{
	HUGE UINT8  *pbPtr = NULL;
	HUGE UINT8  *pbBK = NULL;
	UINT8		bBuf[FS_READ_LEN], bCounter = 0, bMp3ID = 0, bTimeCode = 0;
	UINT16		wTrkNum = 0;
	UINT32		dLstLen = 0, dScanLen = 0;
	UINT32		dBlkTime = 0, dDeltaTime = 0, dTotalTime = 0, dDuration = 0, dTrkID = 0, dTrkSize = 0, dCutTime = 0, dAddTime = 0, dHeader = 0;
	UINT32		dOffSet = 0, dReadLen = FS_READ_LEN, dRealLen = 0, dCurLen = 0;

	dOffSet		= dBasOffSet;

	Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet);
	pbPtr = (HUGE UINT8 *)bBuf;

	wTrkNum		= (((UINT16)pbPtr[11])<<8) + (UINT16)pbPtr[10];
	
	dOffSet	   += 12;	//###
	bMp3ID		= 0;

	dCutTime	= 0;
	dAddTime	= 0;

	for(bCounter=0; bCounter<wTrkNum; bCounter++)
	{
		Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet);
		pbPtr = (HUGE UINT8 *)bBuf;
		
		dTrkID		= (((UINT32)pbPtr[3])<<24) + (((UINT32)pbPtr[2])<<16) + (((UINT32)pbPtr[1])<<8) + (UINT32)pbPtr[0];
		dTrkSize	= (((UINT32)pbPtr[7])<<24) + (((UINT32)pbPtr[6])<<16) + (((UINT32)pbPtr[5])<<8) + (UINT32)pbPtr[4];

		if(dTrkID == ETrk)
		{
			g_VmdInfo.Mp3Blk.dEvtBufLen			= dTrkSize;
			g_VmdInfo.Mp3Blk.dFBasOffSet		= (dOffSet+8);
			g_VmdInfo.Mp3Blk.dFCurOffSet		= (dOffSet+8);
			if(g_VmdInfo.bEnabled & 0x0400)
			{
				g_VmdInfo.Mp3Blk.dOffSet		= (dOffSet+8);
			}
			else
			{
				g_VmdInfo.Mp3Blk.dOffSet 		= 0;
			}
		}
		else
		{
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].bETC		= *(pbPtr+8);
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].wSmplRate	= ( ((UINT16)(*(pbPtr+10))) <<8) + (UINT16)(*(pbPtr+9));
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].wBitRate	= ( ((UINT16)(*(pbPtr+12))) <<8) + (UINT16)(*(pbPtr+11));
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].bSigned		= *(pbPtr+13);
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dFBasOffSet	= (dOffSet+14);
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dFCurOffSet	= (dOffSet+14);
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dBufLen		= dTrkSize-6;
			if(g_VmdInfo.bEnabled & 0x0400)
			{
				g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dOffSet		= (dOffSet+14);			
			}
			else
			{
				g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dOffSet		= 0;			
			}
			g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dTotalTime	= 0;

			dHeader = (((UINT32)(*(pbPtr+14)))<<24) + (((UINT32)(*(pbPtr+15)))<<16) + (((UINT32)(*(pbPtr+16)))<<8) + ((UINT32)(*(pbPtr+17)));
			CheckMp3Head(dHeader, dTrkSize-6, &g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dTotalTime);

			bMp3ID++;
		}
	
		dOffSet += (dTrkSize + 8);
	}

	dOffSet = (dBasOffSet + 12);
	Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet);
	pbPtr 	= (HUGE UINT8 *)bBuf;

	dTrkSize = (((UINT32)pbPtr[7])<<24) + (((UINT32)pbPtr[6])<<16) + (((UINT32)pbPtr[5])<<8) + (UINT32)pbPtr[4];

	dDeltaTime	=  0;
	dTotalTime	=  0;
	dBlkTime	=  0;

	dCurLen 	=  0;
	dOffSet 	+= 8;	//###

	while(dCurLen < dTrkSize)
	{
		if(dCurLen && (dLstLen < (dReadLen-10)))
		{
		}
		else
		{
			Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet+dCurLen);	//###
			pbPtr 	= (HUGE UINT8 *)bBuf;
			dLstLen = 0;
			dScanLen = 0;
		}
		pbBK = pbPtr;
	
		bTimeCode  = *pbPtr++;
		dDeltaTime = bTimeCode&0x7f;
		bCounter=0;
		while(bTimeCode&0x80)
		{
			bCounter++;
			bTimeCode = *pbPtr++;
			dDeltaTime += (UINT16)(((UINT32)bTimeCode&0x7f)<<(7*bCounter));
		}
		
		switch(*pbPtr++)
		{
		case 0xA0:
			bMp3ID 	 = *pbPtr++;
			g_VmdInfo.Mp3Blk.bMp3TrkIndex[g_VmdInfo.Mp3Blk.bDatNum] = bMp3ID;
			g_VmdInfo.Mp3Blk.bDatNum++;
			break;
		default:
			bMp3ID = 0xff;
			break;
		}

		if(g_VmdInfo.wVmdFileVersion >= VMD_STONE_VERSION)
		{
			pbPtr++;	//vol
			pbPtr++;	//pan
		}

		if(bMp3ID != 0xff)	//##########
		{
			dDuration = g_VmdInfo.Mp3Blk.DatBuf[bMp3ID].dTotalTime;
			
			dTotalTime  += dDeltaTime;
			dBlkTime	 = dBlkTime<(dTotalTime +dDuration)?(dTotalTime +dDuration):dBlkTime;
		}

		dScanLen = (pbPtr-pbBK);
		dLstLen	+= dScanLen;
		dCurLen += dScanLen;
	}

	return dBlkTime;
}


/*****************************************************************************
Description:
		Scan the control block of VMD file stream.
		
Parameters:
		dBasOffSet:		OffSet of control block in vmd file.
		dBufLen:		Length of control block in vmd file.
		
Returns:
		Total time of the control block.
		
******************************************************************************/
UINT32 Fstm_ScanCtrlBlk(UINT32 dBasOffSet, UINT32 dBufLen)
{
	HUGE UINT8 *pbPtr = NULL;
	HUGE UINT8 *pbBK = NULL;
	UINT8	bBuf[FS_READ_LEN], bCtrlID  = 0, bCtrlLen = 0, bCtrlDat = 0;
	UINT8	bTimeCode  = 0, bCounter = 0;
	UINT32	dDeltaTime = 0, dLstLen = 0, dScanLen = 0;
	UINT32  dCurLen = 0, dOffSet = 0, dReadLen = FS_READ_LEN, dRealLen = 0;

	dOffSet	= dBasOffSet;
	dCurLen = 0;

	while(dCurLen < dBufLen)
	{
		if(dCurLen && (dLstLen < (dReadLen-10)))
		{
		}
		else
		{
			Fstm_ReadBuf((HUGE UINT8 *)bBuf, dReadLen, (HUGE UINT32 *)&dRealLen, dOffSet+dCurLen);
			pbPtr = (HUGE UINT8 *)bBuf;
			dLstLen = 0;
			dScanLen = 0;
		}
		pbBK = pbPtr;
		
		bTimeCode	= *pbPtr++;
		dDeltaTime	= bTimeCode&0x7f;
		bCounter=0;
		while(bTimeCode&0x80)
		{
			bCounter++;
			bTimeCode	= *pbPtr++;
			dDeltaTime += (UINT16)(((UINT32)bTimeCode&0x7f)<<(7*bCounter));
		}

		pbPtr++;	//skip message id "FF"
				
		bCtrlID 	= *pbPtr++;
		bCtrlLen	= *pbPtr++;
		if(bCtrlID == 8 || bCtrlID == 9)
		{
			bCtrlDat = *pbPtr;
			if(bCtrlDat&0x08)
				g_VmdInfo.bEnabled		|= 0x0100;	//led
			if(bCtrlDat&0x80)
				g_VmdInfo.bEnabled		|= 0x0200;	//motor
		}

		dScanLen = (pbPtr-pbBK);
		dLstLen	+= dScanLen;
		dCurLen += dScanLen;
	}
	
	return 0;
}


