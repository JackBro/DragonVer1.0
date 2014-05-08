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

	Version		Description		Date         	Author
    1.0         Add test for PC player             2004.12.24           Andy,liu
    1.1         Add support for VC0937             2005.01,26           Andy,liu
-------------------------------------------------------------------------*/

#ifndef __VMD_H__
#define __VMD_H__

#if defined( __cplusplus )
extern "C" {
#endif

//Audio chip select 

#define CHIP_VC0938 0
#define CHIP_VC0937 1
#define CHIP_VC0908 2
#define CHIP_VC0907 3

#define VC0937_PLAYBACK 0

#define AUDIO_CHIP CHIP_VC0938


#define __PC_PLATFORM__ 0

#define __INFINEON_PLATFORM__				0
//------Infineon------//
#if __INFINEON_PLATFORM__

#define 	HUGE		huge
#define	SHUGE		shuge
#define 	_FAR		_far	

#else

#define 	HUGE
#define	SHUGE
#define	_FAR

#endif


#ifndef SINT8
#define SINT8 signed char
#endif

#ifndef PSINT8
#define PSINT8 signed char *
#endif

#ifndef SINT16
#define SINT16 signed short
#endif

#ifndef PSINT16
#define PSINT16 signed short *
#endif

#ifndef SINT32
#define SINT32 signed long
#endif

#ifndef PSINT32
#define PSINT32 signed long *
#endif

#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef PUINT8
#define PUINT8 unsigned char *
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef PUINT16
#define PUINT16 unsigned short *
#endif

#ifndef UINT32
#define UINT32 unsigned long
#endif

#ifndef PUINT32
#define PUINT32 unsigned long *
#endif

#ifndef NULL
#define	NULL	((void *)0)
#endif

#ifndef	TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef	VOID
#define VOID	void
#endif

#ifndef	PVOID
#define PVOID	void *
#endif

#ifndef	BOOL
#define	BOOL	unsigned char
#endif

#define	VDEV_MAX_STREAM_FIFO_NUM		4
//#define	VDEV_MAX_STREAM_FIFO_LEN		15104

#define	VDEV_MAX_EVENT_FIFO_LEN			512
#define	VDEV_MAX_COMMAND_FIFO_LEN		256

#define VDEV_UDIBANK_HEADER_MAXLEN		2048
#define VDEV_UDIBANK_DATA_MAXLEN		15104

#define	VDEV_MAX_MP3_FIFO_LEN			8192
#define	VDEV_AES_KEY_LENGTH				16
#define	VDEV_AES_INITVEC_LENGTH			16



#define MAX_STREAM_VOICE		4		//Hard channel for synthesizing Stream/MP3
#define MAX_MIDI_CHN			16		//MIDI channel 
#define MAX_STREAM_CHN			4		//PCM/ADPCM channel 
#define MAX_MP3_CHN				1		//MP3 channel 
#define MAX_MIDI_VOICE			60		//Hard channel for synthesizing MIDI/SMAF
#define MAX_MUSIC_VOICE			(MAX_MIDI_VOICE + MAX_STREAM_VOICE)


#define VMD_MSG_NOTE_ON					(0)
#define VMD_MSG_NOTE_OFF				(1)
#define VMD_MSG_PROGRAM_CHANGE			(2)
#define VMD_MSG_MODULATION_DEPTH		(3)
#define VMD_MSG_CHANNEL_VOLUME			(4)
#define VMD_MSG_PANPOT					(5)
#define VMD_MSG_EXPRESSION				(6)
#define VMD_MSG_HOLD1					(7)
#define VMD_MSG_PITCH_BEND				(8)
#define VMD_MSG_BEND_RANGE				(9)
#define VMD_MSG_CHANNEL_PRESSURE		(10)
#define VMD_MSG_RESET_ALL_CONTROLLERS	(11)
#define VMD_MSG_ALL_NOTE_OFF			(12)
#define VMD_MSG_ALL_SOUND_OFF			(13)
#define VMD_MSG_STREAM_ON				(14)
#define VMD_MSG_STREAM_OFF				(15)
#define VMD_MSG_STREAM_PANPOT			(16)
#define VMD_MSG_MASTER_VOLUME			(17)
#define VMD_MSG_BANK_SELECT				(18)
#define VMD_MSG_REVERVB_ENABLE			(19)
#define VMD_MSG_REVERVB_ON				(20)
#define VMD_MSG_REVERVB_OFF				(21)
#define VMD_MSG_EOS_EVENT				(22)
#define VMD_MSG_NOP						(23)
#define VMD_MSG_SET_TIMER				(24)
#define VMD_MSG_SET_REG_PAGE			(25)
#define VMD_MSG_USER_EVENT				(26)
#define VMD_MSG_UDSE_ON					(27)
#define VMD_MSG_MP3_ON					(28)
#define VMD_MSG_MP3_OFF					(29)
#define VMD_MSG_SOFT_INT				(30)
#define VMD_MSG_INT_ENABLE				(31)
#define VMD_MSG_INT_DISABLE				(32)
#define VMD_MSG_LED_ON					(33)
#define VMD_MSG_LED_OFF					(34)
#define VMD_MSG_MOTOR_ON				(35)
#define VMD_MSG_MOTOR_OFF				(36)
#define VMD_MSG_RESET             (37)
#define VMD_MSG_CONTROL           (38)
#define VMD_MSG_SPEC_EVENT        (39)

#define	VM_PARSER_SET_FIFO				1
#define VM_PARSER_SET_WATER_MARK		2


typedef	struct _VM_SEND_MSG
{
	UINT8	bMsgID;			/* message_ID*/
	UINT16	wDeltaTime ;	/* in ms. <0: immediate event */
	UINT8   bPara1;			/* parameters for this message */
	UINT8 	bPara2;
	UINT8 	bPara3;
	UINT8	bPara4;
	UINT8 	bPara5;
} VM_SEND_MSG, *PVM_SEND_MSG;

typedef	struct _VM_UNI_MSG
{
	UINT8	bMsgID;			/* message_ID*/
	SINT32	sdDeltaTime ;	/* in ms. <0: immediate event */
	UINT8	bChn; 			/* channel */
	UINT32	dPara1;			/* parameters for this message */
	UINT32	dPara2;
	UINT32	dPara3;
} VM_UNI_MSG, *PVM_UNI_MSG;	

typedef enum VDEV_FIFO_TYPE
{
	VDEV_FIFO_EVENT = 0,
	VDEV_FIFO_COMMAND  ,
	VDEV_FIFO_STREAM0  ,
	VDEV_FIFO_STREAM1  ,
	VDEV_FIFO_STREAM2  ,
	VDEV_FIFO_STREAM3  ,
	VDEV_FIFO_MP3      ,
	VDEV_FIFO_V8051RAM ,
	VDEV_FIFO_ALL	= 0xFF

}VDEV_FIFO_TYPE;


/** API result enum  */
typedef enum _VRESULT
{

	VRES_SUCCESS							=   0,
	VRES_ERROR								= -1,
	VRES_OUT_BUFFER_FULL					= -2,
	VMAMIDI_INVALID_DATA					= -3,
	VRES_BUFFER_NULL						= -4,
	VRES_FILE_TOO_SMALL					= -5,
	VRES_FILE_TOO_LARGE					= -6,
	VRES_HWEVTP_EMPTY					= -7,
	VRES_EVTP_EMPTY						= -8,
	VRES_END_FLAG							= -9,
	VRES_HAVE_SAME_FILE_TYPE				= -10,
	VRES_OUT_OF_STREAMFIFO				= -11,
	VRES_STREAM_PORT_ENABLE				= -12,
	VRES_REVERB_ACTIVE					= -13,
	VRES_REVERB_NOT_START				= -14,
	VRES_SPORT_NOT_OPEN					= -15,
	VRES_SPORT_NOT_STOP					= -16,
	VRES_SPORT_NOT_START					= -17,
	VRES_THE_PARSER_NOT_FOUND			= -18,
	VRES_SEEK_TO_END						= -19,
	VRES_INVALID_INPUT_PARAMETERS		= -20,
	VRES_NOT_SEEKABLE						= -21,
	VRES_WAVE_NOT_LOADED_ALL			= -22,
	VRES_SPORT_LOCKED						= -23,
	VRES_ERROR_MUSIC_TYPE				= -24,
	VRES_QUEUE_EMPTY						= - 25,
	VRES_WAVEMSG_ERROR						= -26,
	VRES_RESERVED							= -99,
	VRES_SMAF_WAVE_INFO2_DATA_UNLOAD	= -100,
	VRES_SMAF_ATR_WATE_FINISH       		= -101,
	VRES_VMD_INVALID_DATA				= -102,
    VRES_VMD_SMAF_LOAD_NOT_SMAF_FILE	= -103,
    VRES_ERROR_PARAMETERS				  	= -104,
    VRES_SYNTH_HAS_CREATED				= -105,
    VRES_SYNTH_NO_EXIST					= -106,
    VRES_ERROR_AUDIO_MODE				= -107,
    VRES_MP3FIFO_NOT_FREE				= -108,
    VRES_STMFIFO0_NOT_FREE				= -109,
    VRES_STMFIFO1_NOT_FREE				= -110,
    VRES_STMFIFO2_NOT_FREE				= -111,
    VRES_STMFIFO3_NOT_FREE				= -112,
    VRES_EVENTFIFO_NOT_FREE				= -113,
    VRES_COMMANDFIFO_NOT_FREE			= -114,
    VRES_STREAM_DATA_NO_EXIST			= -115,
    VRES_STREAM_DATA_END				= -116,
    VRES_COMMANDFIFO_NOT_EMPTY			= -117

}VRESULT;

typedef struct _FILE_INFO
{
	/* for vmd */
	UINT32 	dVmdFlag;	/* bit0:10, tag whether midi|wave|udse|udi|comment|mp3|lyric|pic|led|motor is included in vmd file*/
	UINT32	dTotalTime;
	UINT32	dCurTime;
	UINT16	wVmdFileVersion;
	UINT8	bVmdMidiNum;
	UINT8	bVmdWavTrkNum;
	UINT8	bVmdWavDatNum[MAX_STREAM_CHN];
	UINT8	bVmdCurWavDatNum[MAX_STREAM_CHN];	
	UINT8	bVmdMp3Num;
	UINT8	bVmdCurMp3Num;
	UINT8	bVmdMp3Key;		//0->not encrypt, 1->CBC, 2->ECB
	UINT8	*pVmdCmtTitle;
	UINT8	*pVmdCmtAuthor;
	UINT8	*pVmdCmtCpy;
	UINT32	dVmdCmtTitleLen;
	UINT32	dVmdCmtAuthorLen;
	UINT32	dVmdCmtCpyLen; 
	UINT8   *pVmdUdiHead;
	UINT8	*pVmdUdiData;
	UINT32	dVmdUdiHead;
	UINT32 	dVmdUdiData;
	/* for mid */
	/* for wave */
	/* for mp3 */
	UINT8*	mp3_title;		//Added by TangMian
	UINT8*	mp3_artist;		//Added by TangMian
	UINT8*	mp3_album;		//Added by TangMian
	UINT8*	mp3_comment;		//Added by TangMian
	UINT8*	mp3_year;		//Added by TangMian
	UINT8*	mp3_genre;		//Added by TangMian

}FILE_INFO;

void	Vmd_MemSet(HUGE UINT8 *psrc, const UINT8	uValue, UINT32	size);
void	Vmd_MemCpyEx(HUGE UINT8 *pdst, HUGE UINT8 *psrc);
void	Vmd_MemCpy(HUGE UINT8 *pdst, const HUGE UINT8 *psrc, UINT32	size);
SINT8	Vmd_MemCmp (const HUGE UINT8 *s1, const HUGE UINT8 *s2, UINT32 n);

#if defined( __cplusplus )
}
#endif

#endif /*__VMD_H__*/


