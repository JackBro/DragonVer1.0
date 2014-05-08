#ifndef __DEF_H__
#define __DEF_H__

#ifdef __cplusplus
extern "C" {
#endif


#ifndef SINT8
#define SINT8 signed char
#endif

#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef SINT16
#define SINT16 signed short
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef SINT32
#define SINT32 signed long
#endif

#ifndef UINT32
#define UINT32 unsigned long
#endif

#ifndef NULL
#define  NULL	((void *)0)
#endif

#ifndef LPVOID
#define  LPVOID void *
#endif

#ifndef	BOOL
#define	BOOL	int
#endif

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef OK
#define OK	1
#endif

#ifndef FAIL
#define FAIL	0
#endif

///////////////////////////////////////////////////////////////////////////////////////////
//Pre compile.

#ifdef WIN32
#define __packed
#define __irq
#endif

typedef __packed struct
{
	UINT8	bySignature_b1;		//Signature that helps identify this data packet as an OBW
	UINT8	bySignature_b2;		//Signature that helps identify this data packet as an OBW
	UINT8	bySignature_b3;		//Signature that helps identify this data packet as an OBW
	UINT8	bySignature_b4;		//Signature that helps identify this data packet as an OBW
	UINT8	byDataLength;			//Data Lenth.
	UINT8	byOpenrationCode;
	UINT8	byPara1_b1;
	UINT8	byPara1_b2;
	UINT8	byPara1_b3;
	UINT8	byPara1_b4;
	UINT8	byPara2_b1;
	UINT8	byPara2_b2;
	UINT8	byPara2_b3;
	UINT8	byPara2_b4;
}OBW, *pOBW;

typedef __packed struct
{
	UINT8	bySignature_b1;		//Signature that helps identify this data packet as an RSW
	UINT8	bySignature_b2;		//Signature that helps identify this data packet as an RSW
	UINT8	bySignature_b3;		//Signature that helps identify this data packet as an RSW
	UINT8	bySignature_b4;		//Signature that helps identify this data packet as an RSW
	UINT8	Status;				//Status indicates the success or failure of the operation.0 = Success1 = Failed2 = Phase Error
	UINT8	byPara1_b1;
	UINT8	byPara1_b2;
	UINT8	byPara1_b3;
	UINT8	byPara1_b4;
	UINT8	byPara2_b1;
	UINT8	byPara2_b2;
	UINT8	byPara2_b3;
	UINT8	byPara2_b4;
}RSW, *pRSW;

#define 	SIGNATURE_RSW   0x534D5241
#define 	SIZE_OBW 	14
#define 	SIZE_RSW 	13

#define	UART_PACKET_LENGTH 255

typedef enum
{
	BEG_STATE	= 0,
	R_STATE,
	M_STATE,
	C_STATE,
	CMDDATA_STATE,
	RECIVEDATA_STATE
}RAMCOMD_STATE;

////////////////////////////////////////////////
//OBW CMD
////////////////////////////////////////////////
typedef enum
{
	CMD_Undefined		= 0x0,

	//Chip command.
	CMD_VMVGET_REG		,
	CMD_VMVSET_REG		,
	CMD_VMVTRANS_DATA	,
	CMD_VMVPLAY 		,
	CMD_VMVSTOP 		,
	CMD_VMVDOWNLOADFILE ,
	CMD_GETDEBUGINFO	
} ARMCMD;


////////////////////////////////////////////////
//Play type
////////////////////////////////////////////////
typedef enum _TYPE_PLAY{
	TYPE_PLAY_Undef		=0x0,
	TYPE_PLAY_PURE_MIDI		,
	TYPE_PLAY_MIDI_WAVE_1CH		,
	TYPE_PLAY_MIDI_WAVE_2CH		,
	TYPE_PLAY_MIDI_WAVE_3CH		,
	TYPE_PLAY_MIDI_WAVE_4CH		,
	TYPE_PLAY_MIDI_PRELOAD_1CH		,
	TYPE_PLAY_MIDI_PRELOAD_2CH		,
	TYPE_PLAY_MIDI_PRELOAD_4CH		,
	TYPE_PLAY_MIDI_DLS		,
	TYPE_PLAY_MIDI_STREAM_DLS_PRELOAD		,
	TYPE_PLAY_MIDI_REVERB			,
	TYPE_PLAY_MIDI_MP3			,
	TYPE_PLAY_COMMAND
}TYPE_PLAY;

/////////////////////////////////////////////////
//Play mode
/////////////////////////////////////////////////
typedef enum _TYPE_PLAY_MODE{
	TYPE_PLAY_MODE_EVENT        = 0x00 ,     //Event drive mode
	TYPE_PLAY_MODE_REAL_MIDI    = 0x10 ,		//real time Midi
	TYPE_PLAY_MODE_ENCRYPT_CBC	= 0x20 ,		//Encrypted CBC Mode
	TYPE_PLAY_MODE_ENCRYPT_ECB	= 0x40 ,		//encrypted ECB mode
	TYPE_PLAY_MODE_UNDEF
}TYPE_PLAY_MODE;

typedef	enum _TYPE_DOWNLOAD
{
	TYPE_DOWNLOAD_UNDEF	= 0x00,
	TYPE_DOWNLOAD_EVENT		,
	TYPE_DOWNLOAD_STREAM0	,
	TYPE_DOWNLOAD_STREAM1	,
	TYPE_DOWNLOAD_STREAM2	,
	TYPE_DOWNLOAD_STREAM3	,
	TYPE_DOWNLOAD_COMMAND	,
	TYPE_PLAY_GAME_EFFECT
}TYPE_DOWNLOAD;
////////////////////////////////////////////////
//File size
////////////////////////////////////////////////
#define DATA_FILE_LEN	 300*1024
#define COMMAND_FILE_LEN 10*1024
#define PRELOAD_FILE_LEN 20*1024
#define EVENT_FILE_LEN   300*1024
#define STREAM0_FILE_LEN 300*1024
#define STREAM1_FILE_LEN 200*1024
#define STREAM2_FILE_LEN 100*1024
#define STREAM3_FILE_LEN 100*1024



#ifdef __cplusplus
}
#endif
#endif /*__DEF_H___*/
