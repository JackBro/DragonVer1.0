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


 Module:		Vsyn.h

 Description:	VC0938 synthesizer module functions declarations

 Update History:
   1.0		   Basic version		   20040803 	   gzd

-----------------------------------------------------------------------*/

#ifndef __VSYN_H__
#define __VSYN_H__


#include "vhostctrl.h"

#if(FILESTREAM_MODE == FILESTREAM_SYNC)

#if (BUFFER_MP3_SIZE == BUFFER_EMPTY_1_4)
#define	SECOND_BUFFER_SIZE	2048
#endif

#if (BUFFER_MP3_SIZE == BUFFER_EMPTY_1_2)
#define	SECOND_BUFFER_SIZE	4096
#endif

#if (BUFFER_MP3_SIZE == BUFFER_EMPTY_3_4)
#define	SECOND_BUFFER_SIZE	6144
#endif

#else

#if (BUFFER_MP3_SIZE == BUFFER_EMPTY_1_4)
#define	SECOND_BUFFER_CELL	2048
#define	SECOND_BUFFER_SIZE	2048 * BUFFER_EMPTY_TIME
#endif

#if (BUFFER_MP3_SIZE == BUFFER_EMPTY_1_2)
#define	SECOND_BUFFER_CELL	4096
#define	SECOND_BUFFER_SIZE	4096 * BUFFER_EMPTY_TIME
#endif

#if (BUFFER_MP3_SIZE == BUFFER_EMPTY_3_4)
#define	SECOND_BUFFER_CELL	6144
#define	SECOND_BUFFER_SIZE	6144 * BUFFER_EMPTY_TIME
#endif

#endif

/********************************************************************
 Description:
 		the callback function that indicate the playback of the special 
 		channel is end.

 Parameters:
		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_EVENT
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
	                <P>VDEV_FIFO_MP3
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

*********************************************************************/
typedef void (* PCALLBACKEOF)(UINT8 bFifoType, PVOID pExtPara);

/********************************************************************
 Description:
 		Get the unified music message data from the special parser.

 Parameters:
		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_EVENT
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
	                <P>VDEV_FIFO_MP3
		pbMsgBuf:	[input] Pointer to the unified music message data buffer.
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		<P>VRES_SUCCESS:	the implementation is successful.
 		<P>ERROR CODE:		the implementation is failed.

*********************************************************************/
typedef VRESULT (* PGETMESSAGE)(UINT8 bFifoType, PUINT8 pbMsgBuf, PVOID pExtPara);

/********************************************************************
 Description:
 		Get the stream data or mp3 data from the special parser.

 Parameters:
		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
	                <P>VDEV_FIFO_MP3
	    ppbStmBuf:	<output> pointer to stream buffer.
	    dStmLen:	<input> the length of the stream data that the synthsizer
	    			module want to get from the special parser.
 		pExtPara:	the extension parameter used for the future.
	
 Returns:
 		the actual size of the stream data gotten from the special parser.

 Remarks:
 		
*********************************************************************/
typedef UINT32  (* PGETSTREAM)(UINT8 bFifoType, PUINT8 pbStmBuf, UINT32 dStmLen, PVOID pExtPara);


/********************************************************************
 Description:
		the definition of AUDIO mode enumeration
*********************************************************************/
typedef enum _ENUM_AUDIO_MODE
{
	VDEV_AUDIO_MODE_HOST_WAVE_MIDI		=	0x00,
	VDEV_AUDIO_MODE_HOST_WAVE_SD_MIDI	=	0x20,
	VDEV_AUDIO_MODE_SD_WAVE_SD_MIDI		=	0x30,
	VDEV_AUDIO_MODE_HOST_MP3_MIDI		=	0x40,
	VDEV_AUDIO_MODE_HOST_MP3_SD_MIDI	=	0x60,
	VDEV_AUDIO_MODE_SD_MP3_SD_MIDI		=	0x70,
	VDEV_AUDIO_MODE_SD_PLAYER			=	0xF0,
	VDEV_AUDIO_MODE_UNDEF				=	0xFF

}ENUM_AUDIO_MODE;

/********************************************************************
 Description:
		the definition of fifo water mark enumeration
*********************************************************************/
typedef enum _ENUM_FIFO_WM
{
	VDEV_FIFO_WATERM_1_4 = 0x00,
	VDEV_FIFO_WATERM_1_2 = 0x01,
	VDEV_FIFO_WATERM_3_4 = 0x02
		
}ENUM_FIFO_WM;

/********************************************************************
 Description:
		the definition of stream fifo water mark value enumeration
*********************************************************************/
typedef enum _ENUM_SFIFO_WM
{
	VDEV_SFIFO_DIV_1_WM_1_4	= 0x3B,
	VDEV_SFIFO_DIV_1_WM_1_2	= 0x76,
	VDEV_SFIFO_DIV_1_WM_3_4	= 0xB1,
	VDEV_SFIFO_DIV_2_WM_1_4 = 0x1D,
	VDEV_SFIFO_DIV_2_WM_1_2 = 0x3B,
	VDEV_SFIFO_DIV_2_WM_3_4	= 0x58,
	VDEV_SFIFO_DIV_3_WM_1_4 = 0x13,
	VDEV_SFIFO_DIV_3_WM_1_2 = 0x27,
	VDEV_SFIFO_DIV_3_WM_3_4 = 0x3A,
	VDEV_SFIFO_DIV_4_WM_1_4 = 0x0E,
	VDEV_SFIFO_DIV_4_WM_1_2 = 0x1D,
	VDEV_SFIFO_DIV_4_WM_3_4 = 0x2C
}ENUM_SFIFO_WM;

/********************************************************************
 Description:
		the definition of stream fifo base address value enumeration
*********************************************************************/
typedef enum _ENUM_SFIFO_BASEADDR
{
	VDEV_SFIFO_DIV_1_BADDR0 = 0x00,
	VDEV_SFIFO_DIV_2_BADDR0 = 0x00,
	VDEV_SFIFO_DIV_2_BADDR1 = 0x760,
	VDEV_SFIFO_DIV_3_BADDR0 = 0x00,
	VDEV_SFIFO_DIV_3_BADDR1 = 0x4E8,
	VDEV_SFIFO_DIV_3_BADDR2 = 0x9D0,
	VDEV_SFIFO_DIV_4_BADDR0 = 0x00,
	VDEV_SFIFO_DIV_4_BADDR1 = 0x3B0,
	VDEV_SFIFO_DIV_4_BADDR2 = 0x760,
	VDEV_SFIFO_DIV_4_BADDR3 = 0xB10
}ENUM_SFIFO_BASEADDR;

/********************************************************************
 Description:
		the definition of stream fifo end pointer value enumeration
*********************************************************************/
typedef enum _ENUM_SFIFO_ENDPTR
{
	VDEV_SFIFO_DIV_1_ENDPTR = 0xEBF,
	VDEV_SFIFO_DIV_2_ENDPTR = 0x75F,
	VDEV_SFIFO_DIV_3_ENDPTR = 0x4e7,
	VDEV_SFIFO_DIV_4_ENDPTR = 0x3AF
}ENUM_SFIFO_ENDPTR;

/********************************************************************
 Description:
		the definition of Event fifo empty buffer length enumeration
*********************************************************************/
typedef enum _ENUM_EFIFO_BUFLEN
{
	VSYN_EFIFO_BUFLEN_1_4 = 0x80,
	VSYN_EFIFO_BUFLEN_1_2 = 0x100,
	VSYN_EFIFO_BUFLEN_3_4 = 0x180
}ENUM_EFIFO_BUFLEN;

/********************************************************************
 Description:
		the definition of mp3 fifo empty buffer length enumeration
*********************************************************************/
typedef enum _ENUM_MFIFO_BUFLEN
{
	VSYN_MFIFO_BUFLEN_1_4 = 2048,
	VSYN_MFIFO_BUFLEN_1_2 = 4096,
	VSYN_MFIFO_BUFLEN_3_4 = 6144
}ENUM_MFIFO_BUFLEN;

/********************************************************************
 Description:
		the definition of stream fifos empty buffer length enumeration
*********************************************************************/
typedef enum _ENUM_SFIFO_BUFLEN
{
	VSYN_SFIFO_DIV1_BUFLEN_1_4 = 0xEC0,
	VSYN_SFIFO_DIV1_BUFLEN_1_2 = 0x1D80,
	VSYN_SFIFO_DIV1_BUFLEN_3_4 = 0x2C40,
	VSYN_SFIFO_DIV2_BUFLEN_1_4 = 0x740,
	VSYN_SFIFO_DIV2_BUFLEN_1_2 = 0xEC0,
	VSYN_SFIFO_DIV2_BUFLEN_3_4 = 0x1600,
	VSYN_SFIFO_DIV3_BUFLEN_1_4 = 0x4C0,
	VSYN_SFIFO_DIV3_BUFLEN_1_2 = 0x9C0,
	VSYN_SFIFO_DIV3_BUFLEN_3_4 = 0xE80,
	VSYN_SFIFO_DIV4_BUFLEN_1_4 = 0x380,
	VSYN_SFIFO_DIV4_BUFLEN_1_2 = 0x740,
	VSYN_SFIFO_DIV4_BUFLEN_3_4 = 0xB00
}ENUM_SFIFO_BUFLEN;

/********************************************************************
 Description:
		the definition of synthesizer module state enumeration
*********************************************************************/
typedef	enum _ENUM_VSYN_STATE
{
	VSYN_STATE_UNINIT	= 0x00	,
	VSYN_STATE_INIT		= 0x01	,
	VSYN_STATE_CREATE	= 0x02
}ENUM_VSYN_STATE;

/********************************************************************
 Description:
		the definition of fifo state enumeration
*********************************************************************/
typedef enum _ENUM_FIFO_STATE
{
	VSYN_FIFO_STATE_UNDEF	= 0x00,
	VSYN_FIFO_STATE_FREE	= 0x01,
	VSYN_FIFO_STATE_USED	= 0x02
	
}ENUM_FIFO_STATE;

/********************************************************************
 Description:
		the structure definition of Event buffer information
*********************************************************************/
typedef struct _EBUF_INFO
{
	UINT8	bEbuf[VDEV_MAX_EVENT_FIFO_LEN << 1];	//Event buffer array
	UINT16  wEbufLen;								//the length of the Event buffer
	UINT16	wEdataLen;								//the actual length of the event
	UINT16	wWritePtr;								//Write pointer
	UINT16	wReadPtr;								//Read pointer
		
}EBUF_INFO, *PEBUF_INFO;

/********************************************************************
 Description:
		the structure definition of Command buffer information
*********************************************************************/
typedef struct _CBUF_INFO
{
	UINT8	bCbuf[VDEV_MAX_COMMAND_FIFO_LEN];		//Command buffer array
	UINT16  wCbufLen;								//the length of the command buffer
	UINT16	wCdataLen;								//the actual length of the command event
	UINT16	wWritePtr;								//Write pointer
	UINT16	wReadPtr;								//Read pointer
	
}CBUF_INFO, *PCBUF_INFO;

/********************************************************************
 Description:
		the structure definition of AES encryption buffer information
*********************************************************************/
typedef struct _AES_INFO
{
	UINT8	bAesMode;								//the AES mode
	UINT8	bAesKey[VDEV_AES_KEY_LENGTH];			//AES key array
	UINT8	bAesInitVec[VDEV_AES_INITVEC_LENGTH];	//AES initial vector array
}AES_INFO, *PAES_INFO;

/********************************************************************
 Description:
		the structure definition of stream fifos information
*********************************************************************/
typedef struct _STMFIFO_INFO
{
	UINT8		bFifoNum;									//the numbers of the stream fifos that 
															//  could be played back at the same time
	UINT8		bStmFifoState[VDEV_MAX_STREAM_FIFO_NUM];	//the fifo state array
	UINT8		bSFifoWaterM[VDEV_MAX_STREAM_FIFO_NUM];		//the fifo water mark array
	UINT16		wSFifoBufLen[VDEV_MAX_STREAM_FIFO_NUM];		//the fifo buffer empty length
	UINT16		wSFifoBaseAddr[VDEV_MAX_STREAM_FIFO_NUM];	//the stream fifos based address array
	UINT16		wSFifoEndPtr[VDEV_MAX_STREAM_FIFO_NUM];		//the stream fifos End pointer array
	AES_INFO	sAes_Info[VDEV_MAX_STREAM_FIFO_NUM];		//the AES information structure
	
}STMFIFO_INFO, *PSTMFIFO_INFO;

/********************************************************************
 Description:
		the structure definition of mp3 fifo information
*********************************************************************/
typedef struct _MP3FIFO_INFO
{
	UINT8		bFifoState;		//the MP3 fifo state
	UINT8		bMFifoWaterM;	//the mp3 fifo water mark
	UINT16		wMFifoBufLen;	//the mp3 fifo buffer empty length
	AES_INFO	sAes_Info;		//the AES information structure
	
}MP3FIFO_INFO, *PMP3FIFO_INFO;

/********************************************************************
 Description:
		the structure definition of synthesizer fifos state information
*********************************************************************/
typedef struct _VSYN_FIFO_STATE
{
	UINT8	bEventFifoState;						//the state of the Event fifo
	UINT8	bCommandFifoState;						//the state of the command fifo
	UINT8	bStmFifoState[VDEV_MAX_STREAM_FIFO_NUM];//the state of the stream fifo0~3
	UINT8	bMp3FState;								//the state of the mp3 fifo
}VSYN_FIFO_STATE, *PVSYN_FIFO_STATE;

/********************************************************************
 Description:
		the union definition of stream and mp3 fifos information
*********************************************************************/
typedef union _UNION_SFIFO_INFO
{
	STMFIFO_INFO 		sStmFifo_Info;	//the structure stream fifos information
	MP3FIFO_INFO 		sMp3Fifo_Info;	//the structure mp3 fifo information
	
}UNION_SFIFO_INFO, *PUNION_SFIFO_INFO;

/********************************************************************
 Description:
		the structure definition of event fifo information
*********************************************************************/
typedef struct _EVENTFIFO_INFO
{
	UINT8		bFifoState;		//the event fifo state
	UINT8		bEFifoWaterM;	//the event fifo water mark
	UINT16		wEFifoBufLen;	//the event fifo empty length
	
	EBUF_INFO	sEbuf_Info;		//the buffer information about event
	
}EVENTFIFO_INFO, *PEVENTFIFO_INFO;

/********************************************************************
 Description:
		the structure definition of command fifo information
*********************************************************************/
typedef struct _COMMANDFIFO_INFO
{
	UINT8		bFifoState;		//the command fifo state

	CBUF_INFO	sCbuf_Info;		//the buffer information about command event
}COMMANDFIFO_INFO, *PCOMMANDFIFO_INFO;

/********************************************************************
 Description:
		the structure definition of synthesizer information
*********************************************************************/
typedef struct _VSYN_INFO
{
	UINT8				bSynthID;		//the ID of the synthesizer module
	UINT8				bSynthState;	//the state of the synthesizer module
	UINT8				bAudioMode;		//the Audio mode 
	UINT8				bVmdMode;		//the vmd music mode for mp3 or streams
	UINT8				bStmEndFlag;	//the stream end flag

	VSYN_FIFO_STATE		sFifoState;		//the states of the stream and mp3 fifos
	UNION_SFIFO_INFO	uSFifo_Info;	//the union of stream and mp3 fifos

	EVENTFIFO_INFO		sEFifo_Info;	//the structure of the event fifo information
	COMMANDFIFO_INFO	sCFifo_Info;	//the structure of the command event fifo information

	PGETMESSAGE			pGetMessage;	//Pointer to the callback function GetMessage
	PGETSTREAM			pGetStream;		//Pointer to the callback function GetStream
	PCALLBACKEOF		pCallBackEof;	//Pointer to the callback function of end of fifo play
	
}VSYN_INFO, *PVSYN_INFO;

/********************************************************************
 Description:
 		the structure definition of Key effect buffer information
*********************************************************************/
typedef struct _KEY_EFFECT_BUFFER_INFO
{
	UINT8  bKeyID;
	UINT8  bTimbreID;
	UINT16 wDuration;
}KEY_BUF, *PKEY_BUF;

/********************************************************************
				the declaration of the export functions
*********************************************************************/

VRESULT Vsyn_Initialize		(PVOID pExtPara);
VRESULT Vsyn_Terminate		(PVOID pExtPara);

VRESULT Vsyn_Create			(PGETMESSAGE pGetMsg, PGETSTREAM pGetStm, PCALLBACKEOF pCbEof, PVOID pExtPara);
VRESULT Vsyn_Delete			(PVOID pExtPara);

VRESULT	Vsyn_SetAudioMode	(UINT8 bAudioMode, UINT8 bStmFifoNum);
VRESULT Vsyn_SetWaterMark	(UINT8 bFifoType, UINT8 bWaterM, PVOID pExtPara);
VRESULT Vsyn_SetAESKey		(UINT8 bAESMode, UINT8 bFifoType, PUINT8 pbKeyBuf, PUINT8 pbInitVecBuf, PVOID pExtPara);

VRESULT Vsyn_StreamSetup	(UINT8 bFifoType, UINT8 bVmdMode, PVOID pExtPara);

VRESULT Vsyn_EventSetup		(UINT8 bFifoType, PVOID pExtPara);

VRESULT Vsyn_StartSeq		(UINT8 bFifoType, PVOID pExtPara);
VRESULT Vsyn_StopSeq		(UINT8 bFifoType, PVOID pExtPara);
VRESULT Vsyn_SetVol			(UINT8 bCtrlID, UINT8 bVolL, UINT8 bVolR);

VOID	Vsyn_MidiLedEnable	(UINT8 bSynChn);
VOID	Vsyn_MidiLedDisable	(UINT8 bSynChn);
VOID	Vsyn_MidiMotorEnable	(UINT8 bSynChn, UINT8 bInterval);
VOID	Vsyn_MidiMotorDisable	(UINT8 bSynChn, UINT8 bInterval);

VRESULT	Vsyn_SetMp3Speed	(UINT8 bMp3Times);
VRESULT Vsyn_SetupDLS		(PUINT8 pbHeader, UINT32 dHeaderLen, PUINT8 pbDLSdata, UINT32 dDataLen, PVOID pExtPara);

/********************************************************************
				the declaration of the interrupt handlers
*********************************************************************/
VOID	Vsyn_STmrHandler	(VOID);
VOID	Vsyn_EFifoHandler	(VOID);
VOID	Vsyn_TmrHandler		(VOID);
VOID	Vsyn_EFifoErrHandler(VOID);
VOID	Vsyn_SFifo0Handler	(VOID);
VOID	Vsyn_SFifo1Handler	(VOID);
VOID	Vsyn_SFifo2Handler	(VOID);
VOID	Vsyn_SFifo3Handler	(VOID);
VOID	Vsyn_MFifoHandler	(VOID);
VOID	Vsyn_SWI1Handler	(VOID);
VOID	Vsyn_SWI2Handler	(VOID);
VOID	Vsyn_SWI3Handler	(VOID);
VOID	Vsyn_SWI4Handler	(VOID);
VOID	Vsyn_SWI5Handler	(VOID);
VOID	Vsyn_SWI6Handler	(VOID);
VOID	Vsyn_SWI7Handler	(VOID);
VOID	Vsyn_Stm0EndHandler	(VOID);
VOID	Vsyn_Stm1EndHandler	(VOID);
VOID	Vsyn_Stm2EndHandler	(VOID);
VOID	Vsyn_Stm3EndHandler	(VOID);
VOID	Vsyn_Mp3EndHandler	(VOID);


#endif /*__VSYN_H__*/

