#ifndef __VPLAYER_H_
#define __VPLAYER_H_

#include <string.h>
#include "vmd.h"
#include "vdef.h"
#include "vdev.h"
#include "vdevdef.h"
#include "VhostCtrl.h"
#include "Vsyn.h"
#include "Vparser.h"


#define	  PLL_BYPASS       			0
#define	  PLL_WORKING	   			1

#define	  STANDBY_IDLE     			0
#define	  IDLE_NORMAL	   			1
#define	  NORMAL_IDLE	   			2
#define	  IDLE_STANDBY	   			3


/* Define 938 VMD File Type */
#define	 UDI_VMD					0
#define   WAVE_VMD				1
#define   MP3_VMD					2

/* Define 938 Hardware Channel Type */
#define	 EVENT_CH				0
#define	 COMMAND_CH				1
#define	 STREAM0_CH				2
#define   STREAM1_CH				3
#define	 STREAM2_CH				4
#define   STREAM3_CH				5
#define	 MP3_CH					6


/* Define 938 File Type */
#define   MIDI_FILE  					0x6468544d
#define	  RIFF_HEADER					0x46464952
#define	  RMID_FILE						0x44494D52
#define   WAVE_FILE 					0x45564157
#define   VMD_FILE 						0x42484d56
#define   OTHERMUSIC_FILE 				0x444d4d4d
#define   MP3_FILETYPE1					0x49443301
#define   MP3_FILETYPE2					0xffe00000
#define   MP3_FILETYPE3                 	0x56495200
#define   IMELODY_FILE        			0x49474542    // Added by zou
#define   XMF_FILE            				0x5f464d58


/* Define 938 Function ID */
#define		MIDI_ID					0x11
#define		WAVE1_ID				0x12
#define		WAVE2_ID				0x13
#define		WAVE3_ID				0x14
#define		WAVE4_ID				0x15
#define		VMD1_ID				0x16
#define		MP3_ID					0x17
#define		IMELODY_ID				0x18
#define		XMF_ID					0x19



/* Define 938 Device State */
#define		STANDBY				0
#define		IDLE				1
#define		WORKING				2
#define		PAUSEING				3

/* Define 938 Play State */
#define		W_IDLE				0
#define		W_LOADED			1
#define		W_PLAYING			2
#define		W_PAUSE				3

/* Define 938 Channel Occupation */
#define		NOTUSED				0
#define		USED				1

/* Define GPIO Mode */
#define		GENERAL_GPIO		1
#define		LEDMTR_GPIO			2
#define		IIS_GPIO			3
#define		RESERVE_GPIO		4
#define		PATEST_GPIO			5

/* Define IIS Mode */
#define		M_IIS				1
#define		S1_IIS				2
#define		S2_IIS				3

/* Define IIS Frequence Mode */
#define		FREQ1_IIS			1
#define		FREQ2_IIS			2
#define		FREQ3_IIS			3
#define		FREQ4_IIS			4


typedef		void	(*PEFUNCTION)(void);

typedef union __KCHANGE
{
	UINT32	time;
	UINT8	data[4];
}KCHANGE;

typedef enum __KVRESULT
{
	RESULT_SUCCESS			= 0,

	RESULT_DEVICESTATE		= -1,
	RESULT_PLAYSTATE		= -2,
	RESULT_UNKNOWNFILE		= -3,
	RESULT_CREATESYNC		= -4,
	RESULT_OPENSYNC			= -5,
	RESULT_NOMIDICHN		= -6,
	RESULT_PARSERINIT		= -7,
	RESULT_PARSERLOAD		= -8,
	RESULT_PARSERINFO		= -9,
	RESULT_UNKNOWNID		= -10,
	RESULT_NOCALLBACK		= -11,
	RESULT_PARSESEEK		= -12,
	RESULT_SYNCRESET		= -13,
	RESULT_STARTEPORT		= -14,
	RESULT_OPENEPORT		= -15,
	RESULT_DELETESYNC		= -16,
	RESULT_STOPERROR		= -17,
	RESULT_NOWAVECHN		= -18,
	RESULT_GPIOERROR		= -19,
	RESULT_NOMP3			= -20,
	RESULT_FILESTREAM		= -21,
	RESULT_RANGEVOLUME		= -22,
	RESULT_NOMP3CHN			= -23,
	RESULT_MP3ATOB			= -24,
	RESULT_OUTRANGE			= -25,
	RESULT_PARSERCONFIGURE		= -26,
	RESULT_NOVMDCHN			= -27,
	RESULT_SETWATERMARK		= -28,
	RESULT_EVENTSETUP		= -29,
	RESULT_SETAUDIOMODE		= -30,
	RESULT_STREAMSETUP		= -31,
	RESULT_STARTSEQ			= -32,
	RESULT_STOPSEQ			= -33,
	RESULT_MUSICSTOP		= -34,
	RESULT_MUSICPLAY		= -35,
	RESULT_UNKNOWN			= -36

} KVRESULT;


typedef struct __KMIDI
{
	HUGE UINT8*		pbFilePtr;			//	Point to header address
	UINT8			bSource;				//	Source of MIDI
	UINT8			bState;				//	Current Status
	UINT32			dTotalTime;			//	Total Time
	UINT32			dFileSize;			//	File Size
	UINT32			dRepeatNum;			//	Number of Repeat
	UINT32			dPauseTime;			//	Pause Time
	UINT32			dStartTime;			//	Start Time
	UINT32			dStopTime;			//	Stop Time
	PIVPAR			pVParser;			//	Point to MIDI Parser
}KVMIDI, *pKVMIDI;


typedef struct __KWAVE
{
	HUGE UINT8*		pbFilePtr;			//	Point to header address
	UINT8			bSource;				//	Source of WAVE
	UINT8			bState;				//	Current Status
	UINT32			dTotalTime;			//	Total Time
	UINT32			dFileSize;			//	File Size
	UINT32			dRepeatNum;			//	Number of Repeat
	UINT32			dPauseTime;			//	Pause Time
	UINT32			dStartTime;			//	Start Time
	UINT32			dStopTime;			//	Stop Time
	PIVPAR			pVParser;			//	Point to WAVE Parser
}KVWAVE, *pKVWAVE;


typedef struct __KVMD
{
	HUGE UINT8*		pbFilePtr;			//	Point to header address
	UINT8			bSource;				//	Source of VMD
	UINT8			bState;				//	Current Status
	UINT32			dTotalTime;			//	Total Time
	UINT32			dFileSize;			//	File Size
	UINT32			dRepeatNum;			//	Number of Repeat
	UINT32			dPauseTime;			//	Pause Time
	UINT32			dStartTime;			//	Start Time
	UINT32			dStopTime;			//	Stop Time
	UINT8			bMidi;				//	Whether include midi
	UINT8			bWave;				//	Whether include wave
	UINT8			bMp3;				//	Whether include mp3
	UINT8			bTrackNum;			//	Wave Track Number
	UINT8			bSTrackNum;			//	Record Wave Track Number
	UINT8			bTrackFile[MAX_STREAM_CHN];	//	number of files in each wave Track
	UINT8			bSTrackFile[MAX_STREAM_CHN];	//	Record number of files in each wave Track
	UINT8			bMp3Num;			//	number of mp3 in vmd file
	UINT8			bSMp3Num;		//	Record number of mp3 in vmd file
	UINT8			bSFileNum;		//	Record File Number
	UINT8			bFileNum;			//	File Number
	UINT8			bVmdch[5];
	UINT8			bAesMode;			//  Aes Mode: CBC or ECB
	PIVPAR			pVParser;			//	Point to VMD Parser
}KVVMD, *pKVVMD;


typedef struct __KMP3
{
	HUGE UINT8*		pbFilePtr;			//	Point to header address
	UINT8			bSource;				//	Source of MP3
	UINT8			bState;				//	Current Status
	UINT32			dTime;				//	Produce Time
	UINT8*			pbTitle;				//	Song title
	UINT8*			pbArtist;			//	Artist
	UINT8*			pbAlbum;				//	Picture
	UINT8*			pbGenre;				//	Grene
	UINT8*			pbComment;			//	Comment
	UINT32			dTotalTime;			//	Total Time
	UINT32			dFileSize;			//	File Size
	UINT32			dRepeatNum;			//	Number of Repeat
	UINT32			dPauseTime;			//	Pause Time
	UINT32			dStartTime;			//	Start Time
	UINT32			dStopTime;			//	Stop Time
	PIVPAR			pVParser;			//	Point to MP3 Parser
}KVMP3, *pKVMP3;


typedef struct __KIMELODY
{
	HUGE UINT8*		pbFilePtr;			//	Point to header address
	UINT8			bSource;				//	Source of MP3
	UINT8			bState;				//	Current Status
	UINT32			dTime;				//	Produce Time
	UINT32			dTotalTime;			//	Total Time
	UINT32			dFileSize;			//	File Size
	UINT32			dRepeatNum;			//	Number of Repeat
	UINT32			dPauseTime;			//	Pause Time
	UINT32			dStartTime;			//	Start Time
	UINT32			dStopTime;			//	Stop Time
	PIVPAR			pVParser;			//	Point to MP3 Parser
}KVIMELODY, *pKVIMELODY;


typedef struct __KXMF
{
	HUGE UINT8*		pbFilePtr;			//	Point to header address
	UINT8			bSource;				//	Source of MP3
	UINT8			bState;				//	Current Status
	UINT32			dTime;				//	Produce Time
	UINT32			dTotalTime;			//	Total Time
	UINT32			dFileSize;			//	File Size
	UINT32			dRepeatNum;			//	Number of Repeat
	UINT32			dPauseTime;			//	Pause Time
	UINT32			dStartTime;			//	Start Time
	UINT32			dStopTime;			//	Stop Time
	PIVPAR			pVParser;			//	Point to MP3 Parser
}KVXMF, *pKVXMF;


typedef struct __KVSOUNDINFO
{
	UINT8 bWaveVol_L;
	UINT8 bWaveVol_R;
	UINT8 bMidiVol_L;
	UINT8 bMidiVol_R;
	UINT8 bHPNVol_L;
	UINT8 bHPNVol_R;
	UINT8 bPAVol;
	UINT8 bDACVol;
	UINT8 bMp3VolStep;
	UINT8 bGlbVolStep;
}KVSOUNDINFO, *pKVSOUNDINFO;

typedef struct __KVVERSION{
	UINT8	bHighAPI_Ver;
	UINT8	bM_Ver;
	UINT8	bS_Ver;
	UINT8	bVmdlib_Ver;
	UINT8	bBR_Ver;
	
	UINT8	bYear;		//Based on 2000
	UINT8	bMonth;
	UINT8	bDay;
} KVVERSION, *pKVVERSION;


typedef struct __KVMDAPI
{
	UINT8		bGlobalState;				//	938 Device State
	UINT8		bMidiState;					//	MIDI Flag
	UINT8		bWaveState[4];				//	WAVE Flag
	UINT8		bVmdState;				//	VMD Flag
	UINT8		bMp3State;					//	MP3 Flag
	UINT8		bImelodyState;				//	Imelody Flag
	UINT8		bXmfState;					//	Xmf Flag
	UINT8		bGPIOState;					//	GPIO Status

	UINT32		dFifoMap;					//	the mapping connections between Fifos and Function ID


	pKVMIDI		pM_midi;						//	Point to MIDI Information
	pKVWAVE		pM_wave[4];					//	Point to WAVE Information
	pKVVMD		pM_vmd;						//	Point to VMD Information
	pKVMP3		pM_mp3;						//	Point to MP3 Information
	pKVIMELODY	pM_imelody;						//	Point to imelody Information
	pKVXMF		pM_xmf;						//	Point to XMF information

	pKVSOUNDINFO	pM_SoundInfo;					//	Point to Sound Information

}KVMDAPI, *pKVMDAPI;


#endif
