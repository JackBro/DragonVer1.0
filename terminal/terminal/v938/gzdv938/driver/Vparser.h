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


 Module:		Vparser.h

 Description:	VC0938 parser functions declarations

 Update History:
	1.0			Basic version				

-----------------------------------------------------------------------*/

#ifndef __VPARSER_H__
#define	__VPARSER_H__

#define VMD_PARSER_NUM					8

#define VMD_PARSER_MIDI_SUPPORT			1	/** midi support flag */
#define VMD_PARSER_MP3_SUPPORT			1	/** MP3 support flag */
#define VMD_PARSER_WAVE_SUPPORT			1	/** wave support flag */
#define VMD_PARSER_VMD_SUPPORT			1	/** Vmd sumpport flag */

#define VMD_PARSER_IMELODY_SUPPORT   		1 	/** IMelody support flag */
#define VMD_PARSER_XMF_SUPPORT			1	/** VXmf sumpport flag */

#if VMD_PARSER_MIDI_SUPPORT
 #include "vmidi.h"
#endif

#if VMD_PARSER_MP3_SUPPORT
 #include "vmp3.h"
#endif

#if VMD_PARSER_WAVE_SUPPORT
 #include "vwave.h"
#endif

#if VMD_PARSER_VMD_SUPPORT
 #include "vvmd.h"
#endif

#if VMD_PARSER_IMELODY_SUPPORT
 #include "vmelody.h"
#endif

#if VMD_PARSER_XMF_SUPPORT 
 #include "Vxmf.h"
#endif

/* Parser public member functions */
typedef VRESULT (*pfPar_Init)(void);
typedef VRESULT (*pfPar_Load)(HUGE UINT8* pbFile, UINT32 dLength, UINT32 dEmptyRes, UINT8 bSrcFlag);
typedef VRESULT (*pfPar_Seek)(UINT32 dPos, VDEV_FIFO_TYPE FiFoType);
typedef VRESULT (*pfPar_GetEvent)(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType);
typedef VRESULT (*pfPar_GetFileInfo)(PVOID pFileInfo, VDEV_FIFO_TYPE FiFoType);
typedef VRESULT (*pfPar_GetCurPos)(PUINT32 pdCurTime, VDEV_FIFO_TYPE FiFoType);
typedef VRESULT (*pfPar_ReadBlock)(PUINT8 pbOutBuf, UINT32 dBufLen, PUINT32 pdDataSize, VDEV_FIFO_TYPE FiFoType);
typedef VRESULT (*pfPar_Unload)(VDEV_FIFO_TYPE FiFoType);
typedef VRESULT (*pfPar_End)(void);
typedef VRESULT (*pfPar_Configure)(UINT32 dVal, UINT32 dDat, UINT8 bConfigID);
typedef VRESULT (*pfPar_Reset)(VDEV_FIFO_TYPE FiFoType);
typedef VRESULT (*pfPar_SetAtoB)(UINT32 dTimerA, UINT32 dTimerB);

VRESULT dummy_Init			(void);
VRESULT dummy_Load			(HUGE UINT8* pbFile, UINT32 dLength, UINT32 dEmptyRes, UINT8 bSrcFlag);
VRESULT	dummy_Seek			(UINT32 dPos, VDEV_FIFO_TYPE FiFoType);
VRESULT dummy_GetEvent		(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType);
VRESULT dummy_GetFileInfo	(PVOID pFileInfo, VDEV_FIFO_TYPE FiFoType);
VRESULT dummy_GetCurPos		(PUINT32 pdCurTime, VDEV_FIFO_TYPE FiFoType);
VRESULT dummy_ReadBlock		(PUINT8 pbOutBuf, UINT32 dBufLen, PUINT32 pdDataSize, VDEV_FIFO_TYPE FiFoType);
VRESULT dummy_Unload		(VDEV_FIFO_TYPE FiFoType);
VRESULT dummy_End			(void);
VRESULT	dummy_Configure		(UINT32 dVal, UINT32 dDat, UINT8 bConfigID);
VRESULT dummy_Reset			(VDEV_FIFO_TYPE FiFoType);
VRESULT dummy_SetAtoB		(UINT32 dTimerA, UINT32 dTimerB);

/* Parser interface */
typedef struct _IVPAR
{
	pfPar_Init 			Init;
	pfPar_Load 			Load;
	pfPar_Seek 			Seek;
	pfPar_GetEvent 		GetEvent;
	pfPar_GetFileInfo 	GetFileInfo;
	pfPar_GetCurPos 	GetCurPos;
	pfPar_ReadBlock 	ReadBlock;
	pfPar_Unload 		Unload;
	pfPar_End  			End;
	pfPar_Configure		Configure;
	pfPar_Reset			Reset;
	pfPar_SetAtoB		SetAtoB;
	
	// Public variables
  //  void* ContextPtr;	 // This is a pointer to any private data the module may want to maintain

} IVPAR, *PIVPAR;

#define DUMMY_FUNC	{				\
						dummy_Init,		\
						dummy_Load,		\
						dummy_Seek,		\
						dummy_GetEvent,	\
						dummy_GetFileInfo,	\
						dummy_GetCurPos,	\
						dummy_ReadBlock,	\
						dummy_Unload,	\
						dummy_End,		\
						dummy_Configure, \
						dummy_Reset, 	 \
						dummy_SetAtoB    \
					}


#endif /* __VPARSER_H__ */

