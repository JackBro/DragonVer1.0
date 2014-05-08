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
	1.8			Change MP3 frame error bug         2005.02.17       Andy,liu

-------------------------------------------------------------------------*/
#ifndef _VMP3_H_
#define _VMP3_H_
#include "Vmd.h"

#define         MP3_STEREO           0
#define         MP3_JOINT_STEREO     1
#define         MP3_DUAL_CHANNEL     2
#define         MP3_MONO             3
#define         MP3_GENRE_NUM        4
#define         MP3_ID3V2_HEADERSIZE 10
#define         MP3_ID3V2_FRAMESIZE  10
#define         MP3_VBR_FHGTOC_SIZE  1024
#define         MP3_VBR_XING_SIZE    100
#define         MP3_XING_FRAMES_FLAG 0x0001
#define         MP3_XING_BYTES_FLAG  0x0002
#define         MP3_XING_TOC_FLAG    0x0004
#define         MP3_XING_SCALE_FLAG  0x0008
#define         MP3_MAX_SCAN_FRAME   0x0200
#define         MP3_BUFFER_SIZE      0x800

//Special for NAND FLASH
//////////////////////////////////////////////////////////////////////////////////
//If you want play fast when using NAND FLASH,please set MP3_FASH_PLAY = 1
//If you use NAND FLASH and your file system has no cache,please set MP3_NAND_FLASH_SUPPORT 1
//Note: the MP3_NAND_BUFFER_SIZE has to less than 
#define         MP3_FASH_PLAY          0
#define         MP3_NAND_FLASH_SUPPORT 0
#define         MP3_NAND_BUFFER_SIZE   8*1024
typedef struct {
	UINT32 dStart;
	UINT32 dEnd;
}MP3_NAND;

#if MP3_FASH_PLAY
#define         MP3_MAX_FRAME_OFFSET 0x4000
#else
#define         MP3_MAX_FRAME_OFFSET 0x20000
#endif
//Andy,liu add,2005,1,25
//////////////////////////////////////////////////////////////////////////////////

//Mp3 header
typedef struct 
{
   UINT8  bSync;			               /* Valid sync is 1,else 0 */
   UINT8  bID;                                     /* MPEG Audio version     */                                   
   UINT8  bOption;			      /* Layer description      */
   UINT8  bProtection;                             /* Protection bit         */
   UINT8  bBitrateIndex;                           /* Bit rate index         */
   UINT8  bSampleRateIndex;                        /* Sample rate index      */
   UINT8  bPad;                                    /* Number bit of pad      */
   UINT8  bPrivateBit;                             /* Private bit            */
   UINT8  bChannelMode;                            /* Channel mode           */
   UINT8  bModeExtension ;                         /* Mode extension         */
   UINT8  bCopyright;                              /* Copyright              */
   UINT8  bOriginal;                               /* Original               */
   UINT8  bEmphasis;                               /* Emphasis               */
   UINT8  bMpeg_25;                                /* Mpeg_25 flag           */
}MP3_HEADER;

//Mp3 Info
typedef struct{
   UINT32 dHeader;                                 /* Mp3 first Header       */
   UINT32 dBitrate;                                /* Bit rate               */
   UINT32 dSampleRate;                             /* Sample rate            */
   UINT32 dTotalSize;                              /* Mp3 total size         */
   UINT32 dStartPos;                               /* Mp3 current position   */
   UINT32 dTotalTime;                              /* Mp3 total time         */
   UINT32 dCurrentTime;                            /* Mp3 Current time       */
   UINT32 dFrameNum;                               /* Mp3 Frame num          */
   UINT32 dCurrentFrame;                           /* Mp3 current Frame      */
   UINT32 dFrameSize;                              /* Mp3 frame size         */
   UINT32 dReadOffset;                             /* Read MP3 data offset   */
   UINT32 dID3V2Length;                            /* Mp3 ID3V2 length       */
   UINT32 dJumpGarbage;                            /* Mp3 Jump Garbage       */
   UINT32 dSeekTime;                               /* Mp3 seek time          */
   HUGE UINT8*  pMp3Data;                               /* Mp3 data pointer       */
   UINT8  bStereo;                                 /* Mp3 Mono or stereo     */
   UINT8  bJumpFrame;                              /* First Jump frame       */
   UINT8  bFinish;                                 /* Mp3 parser finish flag */
   UINT8  bXingFlag;                               /* Xing MP3 exist flag    */
                                                   /* 1-XING 2-VBRI 3-INFO   */
   UINT8  bVbrFlag;                                /* CBR=0    VBR =1   FLAG */
   UINT8  bMillionSec;                             /* Mp3 time unit 1:ms 0:s */
   UINT8  bID3V1Tag;                               /* Mp3 ID3V1 exist flag   */
   UINT8  bID3V1Length;                            /* Mp3 ID3V1 length       */
   
   UINT8  bEventFlag;							   /* Mp3 StreamOn Event Flag*/
   UINT8  bStreamFlag;                             /* Mp3 Stream flag        */
   UINT8  bBuffer[MP3_BUFFER_SIZE];                /* Mp3 file stream buffer */
   UINT32 dBufferOffset;                           /* Mp3 buffer offset      */
   UINT32 dFileOffset;                             /* Mp3 file offset        */
   PVOID  PFile;                                  /* Mp3 file pointer       */
}MP3_INFO;

//Mp3 ID3V1 version 0
typedef struct{
	UINT8 bTag[3];                                 /* Tag flag               */
	UINT8 bTitle[30];                              /* Mp3 Title              */ 
	UINT8 bArtist[30];                             /* Mp3 artist             */
	UINT8 bAlbum[30];                              /* Mp3 album              */
	UINT8 bYear[4];                                /* Mp3 Year               */
	UINT8 bComment[30];                            /* Mp3 Comment            */
	UINT8 bGenre;                                  /* Mp3 Genre              */
}MP3ID3V1_0;

//Mp3 ID3V1 version 1
typedef struct{
	UINT8 bTag[3];                                 /* Tag flag               */
	UINT8 bTitle[30];                              /* Mp3 Title              */ 
	UINT8 bArtist[30];                             /* Mp3 artist             */
	UINT8 bAlbum[30];                              /* Mp3 album              */
	UINT8 bYear[4];                                /* Mp3 Year               */
	UINT8 bComment[28];                            /* Mp3 Comment            */
    UINT8 sZero;                                   /* Mp3 Zero               */
    UINT8 sTrack;                                  /* Mp3 Track              */
	UINT8 bGenre;                                  /* Mp3 Genre              */
}MP3ID3V1_1;

//Mp3 ID3V2 Info
typedef struct{
	UINT8 bTag[3];                                 /* Tag flag               */
	UINT8 bTitle[30];                              /* Mp3 Title              */ 
	UINT8 bArtist[30];                             /* Mp3 artist             */
	UINT8 bAlbum[30];                              /* Mp3 album              */
	UINT8 bYear[4];                                /* Mp3 Year               */
	UINT8 bComment[30];                            /* Mp3 Comment            */
	UINT8 bGenre;                                  /* Mp3 Genre              */
}MP3ID3V2;

//MP3 ID3V1 tag
typedef struct{
	UINT8 bVersion;                                /* The ID3 version        */
	PUINT8 bMp3Genre;                              /* The Mp3 Genre          */
    union {
        MP3ID3V1_0 V1_0;                           /* The Mp3 ID3V1_0 version*/
        MP3ID3V1_1 V1_1;                           /* The Mp3 ID3V1_1 version*/
		MP3ID3V2   V2;                             /* The Mp3 ID3V2 version  */
    }ID3;
} MP3_ID3TAG;

//MP3 ID3V2 header
typedef struct
{
	UINT8  bMajorVerison;							/*Major version  = 3 	 */
	UINT8  bRevisionVersion;    					/*Revision version		 */
	UINT8  bFlag;    								/*defined 3 bit			 */
	UINT32 dSize;    					    		/*the IDV3 FRAME SIZE 	 */
}MP3_ID3V2HEADER;

//MP3 ID3V2 frame header
typedef struct
{
	UINT8  bFrameID[4];								/*frame id				 */
	UINT32 dSize;									/*frame size ,not less 1 */ 
	UINT8  bFlags[2];								/*save flag,defined 6 bit*/
}MP3_ID3V2FRAMEHEADER;

//MP3 VBR info 
typedef struct{
    UINT16 wVbr_Version;                            /* MP3 VBR version       */         
    UINT16 wVbr_TableLength;                        /* MP3 VBR table length  */	
    UINT16 wVbr_Delay;                              /* MP3 VBR delay time    */
    UINT16 wVbr_Quality;                            /* MP3 VBR Quality       */
    UINT32 dVbr_StreamBytes;                        /* MP3 VBR Stream bytes  */
    UINT32 dVbr_StreamFrames;                       /* MP3 VBR total frames  */
    UINT16 wVbr_TableSize;                          /* MP3 VBR table size    */
    UINT32 wVbr_TableScale;                         /* MP3 VBR table scale   */
    UINT16 wVbr_EntryBytes;                         /* MP3 VBR entry bytes   */
    UINT16 wVbr_EntryFrames;                        /* MP3 VBR entry frames  */
    union{
		UINT32 Fhg[MP3_VBR_FHGTOC_SIZE];            /* FHG VBRI TAG TABLE    */
		UINT8  Xing[MP3_VBR_XING_SIZE];             /* XING TAG TABLE        */
    }VBR_TOC;

	UINT8  bTOCTag;                                 /* TOC exist tag         */
}MP3_VBRINFO;


//Header type
typedef enum _HEADER_TYPE
{
	VHEADER_HEADER = 1,                             /* the head is header    */
	VHEADER_ID3V2  = 2,                             /* the head is ID3V2     */
	VHEADER_RIFF   = 3,                             /* the head is RIFF      */
	VHEADER_ERROR = -1                              /* the head is error     */
}VHEADER;

//Loop struct
//Loop mode : direct loop; stopB and control loop by user;not stop and control loop by user
typedef struct{
	UINT32 dLoopPositionA;                          /* The loop positionA    */
	UINT32 dLoopPositionB;                          /* The loop positionB    */
	UINT32 dLoopTimeA;                              /* The loop timeA        */
	UINT32 dLoopTimeB;                              /* The loop timeB        */
	UINT8  bLoopFlag;                               /* The loop Flag         */
	UINT8  bHasSeek;                                /* Loop has seek A and B */
}MP3_LOOP;

//Functions exposed to outside
typedef VRESULT (* PREADBLOCKPROC)(PVOID pvoid);
VRESULT	VPlr_SetCallBackProc(PREADBLOCKPROC readBlockProc);

VRESULT	VMp3_Init(void);
VRESULT	VMp3_Load(HUGE UINT8* pbFile, UINT32 dFileLen, UINT32 EmptyRes, UINT8 SrcFlag);
VRESULT	VMp3_Seek(UINT32 dMSecond, VDEV_FIFO_TYPE FiFoType);
VRESULT	VMp3_GetEvent(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType);
VRESULT	VMp3_GetFileInfo(PVOID pvFileInfo, VDEV_FIFO_TYPE FiFoType);
VRESULT	VMp3_ReadBlock(PUINT8 pbOutBuf, UINT32 dBufLen, PUINT32 pdDataSize, VDEV_FIFO_TYPE FiFoType);
VRESULT	VMp3_Unload(VDEV_FIFO_TYPE FiFoType);
VRESULT	VMp3_End(VOID);
VRESULT	VMp3_SetAtoB(UINT32 bTimerA,UINT32 bTimerB);

VRESULT VMp3_GetCurPos(PUINT32 pdCurTime, VDEV_FIFO_TYPE FiFoType);
VRESULT VMp3_Configure(UINT32 dVal, UINT32 dDat, UINT8 bConfigID);
UINT32  VMp3_GetCurTime(void);
VRESULT VMp3_Reset(VDEV_FIFO_TYPE FiFoType);
#endif