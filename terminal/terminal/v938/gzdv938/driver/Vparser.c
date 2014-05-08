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


 Module:		Vparser.c

 Description:	VC0938 parser functions definitions

 Update History:
	1.0			Basic version				

-----------------------------------------------------------------------*/

#include "vmd.h"
#include "vparser.h"


/* file parser array */
IVPAR parser_array[VMD_PARSER_NUM] =
{
	DUMMY_FUNC,							/* 0 */

#if VMD_PARSER_MIDI_SUPPORT				//Midi Parser Support
	{									/* 1 */
		VMidi_Init,
		VMidi_Load,
		VMidi_Seek,
		VMidi_GetEvent,
		VMidi_GetFileInfo,
		VMidi_GetCurPos,
		VMidi_ReadBlock,
		VMidi_Unload,
		VMidi_End,
		VMidi_Configure,
		VMidi_Reset,
		dummy_SetAtoB
	},
#else
	DUMMY_FUNC,
#endif

	DUMMY_FUNC,

#if VMD_PARSER_MP3_SUPPORT				//MP3 Parser Support
	{									// 3 //
		VMp3_Init,
		VMp3_Load,
		VMp3_Seek,
		VMp3_GetEvent,
		VMp3_GetFileInfo,
		VMp3_GetCurPos,
		VMp3_ReadBlock,
		VMp3_Unload,
		VMp3_End,
		VMp3_Configure,
		VMp3_Reset, 
		VMp3_SetAtoB
	},
#else
	
	DUMMY_FUNC,
#endif

#if VMD_PARSER_WAVE_SUPPORT				//Wave Parser Support
	{									// 4 // 
		VWave_Init,
		VWave_Load,
		VWave_Seek,
		VWave_GetEvent,
		VWave_GetFileInfo,
		VWave_GetCurPos,
		VWave_ReadBlock,
		VWave_Unload,
		VWave_End,
		VWave_Configure,
		VWave_Reset,
		dummy_SetAtoB
	},
#else
	DUMMY_FUNC,							
#endif

#if VMD_PARSER_VMD_SUPPORT				//Vmd Parser Support
	{									// 5 // 
		VVmd_Init,
		VVmd_Load,
		VVmd_Seek,
		VVmd_GetEvent,
		VVmd_GetFileInfo,
		VVmd_GetCurPos,
		VVmd_ReadBlock,
		VVmd_Unload,
		VVmd_End,
		VVmd_Configure,
		VVmd_Reset,
		dummy_SetAtoB
	},
#else
	DUMMY_FUNC,							
#endif
	
#if VMD_PARSER_IMELODY_SUPPORT				//IMELODY Parser Support
	{			
							// 6 // 
		VIMelody_Init,
		VIMelody_Load,
		dummy_Seek,
		VIMelody_GetEvent,
		dummy_GetFileInfo,	
		VIMelody_GetCurPos,
		dummy_ReadBlock,
		dummy_Unload,
		dummy_End,
		dummy_Configure,
		VIMelody_Reset,
		dummy_SetAtoB

	},
#else
	DUMMY_FUNC,							
#endif


#if VMD_PARSER_XMF_SUPPORT
{		// 7 //
	VXmf_Init,
	VXmf_Load,
	VXmf_Seek,
	VXmf_GetEvent,
	VXmf_GetFileInfo,
	VXmf_GetCurPos,
	dummy_ReadBlock,
	dummy_Unload,
	dummy_End,
	dummy_Configure,
	VXmf_Reset,
	dummy_SetAtoB
},
#else
	DUMMY_FUNC,
#endif
};

VRESULT dummy_Init			(void)
	{
		return VRES_SUCCESS;
	}
VRESULT dummy_Load			(HUGE UINT8* pFile, UINT32 dLength, UINT32 dEmptyRes, UINT8 bSrcFlag)
	{
		return VRES_SUCCESS;
	}
VRESULT	dummy_Seek			(UINT32 dPos, VDEV_FIFO_TYPE FiFoType)
	{
		return VRES_SUCCESS;
	}
VRESULT dummy_GetEvent		(PVM_UNI_MSG pUniMsg, VDEV_FIFO_TYPE FiFoType)
	{
		return VRES_SUCCESS;
	}
VRESULT dummy_GetFileInfo	(PVOID pFileInfo, VDEV_FIFO_TYPE FiFoType)
	{
		return VRES_SUCCESS;
	}
VRESULT dummy_GetCurPos		(PUINT32 pdCurTime, VDEV_FIFO_TYPE FiFoType)
	{
		return VRES_SUCCESS;
	}
VRESULT dummy_ReadBlock		(PUINT8 pbOutBuf, UINT32 dBufLen, PUINT32 pdDataSize, VDEV_FIFO_TYPE FiFoType)
	{
		return VRES_SUCCESS;
	}
VRESULT dummy_Unload		(VDEV_FIFO_TYPE FiFoType)
	{
		return VRES_SUCCESS;
	}
VRESULT dummy_End			(void)
	{
		return VRES_SUCCESS;
	}
VRESULT	dummy_Configure		(UINT32 dVal, UINT32 dDat, UINT8 bConfigID)
{
	return VRES_SUCCESS;
}

VRESULT	dummy_Reset		(VDEV_FIFO_TYPE FiFoType)
{
	return VRES_SUCCESS;
}

VRESULT dummy_SetAtoB	(UINT32 dTimerA, UINT32 dTimerB)
{
	return VRES_SUCCESS;
}
