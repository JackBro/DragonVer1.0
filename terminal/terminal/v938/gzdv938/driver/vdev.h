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


 Module:		Vdev.h

 Description:	VC0938 Device Driver functions declarations

 Update History:
	1.0			Basic version				2004.07.15		gzd

-----------------------------------------------------------------------*/

#ifndef __VDEV_H__
#define __VDEV_H__

#include "vhostctrl.h"

#define 	VDEV_INT_NUM		24
#define 	BITS_PER_BYTE		8
#define		BITS_PER_WORD		16

#define VREG_PLL_WORK_MODE 			0x01
#define	VREG_PLL_BYPASS_MODE		0x02
#define VREG_PLL_MODE				VREG_PLL_WORK_MODE

#define	VDEV_MAX_STREAM_FIFO_NUM		4

#if(FILESTREAM_MODE == FILESTREAM_SYNC)

#define	VDEV_MAX_STREAM_FIFO_LEN		15104

#else

#if (BUFFER_EMPTY_SIZE == BUFFER_EMPTY_1_4)
#if ((2048 * BUFFER_EMPTY_TIME) >= 15104)
#define	VDEV_MAX_STREAM_FIFO_LEN	2048 * BUFFER_EMPTY_TIME
#else
#define VDEV_MAX_STREAM_FIFO_LEN	15104
#endif
#endif

#if (BUFFER_EMPTY_SIZE == BUFFER_EMPTY_1_2)
#if ((4096 * BUFFER_EMPTY_TIME) >= 15104)
#define	VDEV_MAX_STREAM_FIFO_LEN	4096 * BUFFER_EMPTY_TIME
#else
#define VDEV_MAX_STREAM_FIFO_LEN	15104
#endif
#endif

#if (BUFFER_EMPTY_SIZE == BUFFER_EMPTY_3_4)
#if ((6144 * BUFFER_EMPTY_TIME) >= 15104)
#define	VDEV_MAX_STREAM_FIFO_LEN	6144 * BUFFER_EMPTY_TIME
#else
#define VDEV_MAX_STREAM_FIFO_LEN	15104
#endif
#endif

#endif

/********************************************************************
 Description:
		the definition of AES mode enumeration
*********************************************************************/
typedef enum _ENUM_AES_MODE
{
	VDEV_AES_MODE_UNDEF		= 0x00,
	VDEV_AES_MODE_ECB		= 0x23,
	VDEV_AES_MODE_CBC		= 0x22
}ENUM_AES_MODE;


typedef enum _ENUM_VDEV_VOL_CTRL_ID
{
	VDEV_CTRL_SET_SPVOL			= 0x00,
	VDEV_CTRL_SET_HPVOL			= 0x01,
	VDEV_CTRL_SET_MIDIVOL		= 0x02,
	VDEV_CTRL_SET_WAVEVOL		= 0x03,
	VDEV_CTRL_SET_GAINVOL		= 0x04,
	VDEV_CTRL_SET_MP3_VOLSTEP	= 0x05,
	VDEV_CTRL_SET_GLOBAL_VOLSTEP= 0x06,
	VDEV_CTRL_SET_DACMIXVOL		= 0x07
}ENUM_VDEV_VOL_CTRL_ID;

typedef enum _ENUM_VDEV_CONTROL_ID
{
	VDEV_CTRL_START_SEQUENCER	= 0x00,
	VDEV_CTRL_STOP_SEQUENCER	,
	VDEV_CTRL_RESET_SEQUENCER	,
	VDEV_CTRL_SET_SEQ			,
	VDEV_CTRL_SET_MP3EQ			,
	VDEV_CTRL_SET_LED_928		,
	VDEV_CTRL_SET_LED_938		,
	VDEV_CTRL_SET_MOTOR_928		,
	VDEV_CTRL_RESET_EQ			,
	VDEV_CTRL_RESET_WS			,
	VDEV_CTRL_RESET_MP3			,
	VDEV_CTRL_RESET_VIBRATOR	,
	VDEV_CTRL_RESET_MCLK		,
	VDEV_CTRL_RESET_INT			,
	VDEV_CTRL_RESET_SYS			,
	VDEV_CTRL_RESET_I2S			,
	VDEV_CTRL_RESET_V8051 		,
	VDEV_CTRL_SET_MOTOR_938		,
	VDEV_CTRL_RESET_LED			= 0xFF
	
}ENUM_VDEV_CONTROL_ID;

typedef enum _ENUM_MP3_EQ_CTRL_ID
{
	MP3_EQ_ORIGINAL	= 0x00,
	MP3_EQ_CLASSICAL	= 0x01,
	MP3_EQ_CLUB		= 0x02,
	MP3_EQ_POP		= 0x03,
	MP3_EQ_BASE		= 0x04,
	MP3_EQ_TREBLE		= 0x05,
	MP3_EQ_BASE_TREBLE	= 0x06,
	MP3_EQ_ROCK		= 0x07,
	MP3_EQ_DANCE		= 0x08
}ENUM_MP3_EQ_CTRL_ID;

//define VC0938 Interrupt ID.
//
typedef enum _VDEV_INTERRUPTID
{
	VDEV_INT_STMR 	= 0,
	VDEV_INT_EFIFOEMPTY  ,
	VDEV_INT_TMR		   ,
	VDEV_INT_EFIFOERROR  ,
	VDEV_INT_SFIFO0EMPTY ,
	VDEV_INT_SFIFO1EMPTY ,
	VDEV_INT_SFIFO2EMPTY ,
	VDEV_INT_SFIFO3EMPTY ,
	VDEV_INT_MFIFOEMPTY  ,
	VDEV_INT_SWI1		   ,
	VDEV_INT_SWI2		   ,
	VDEV_INT_SWI3		   ,
	VDEV_INT_SWI4		   ,
	VDEV_INT_SWI5		   ,
	VDEV_INT_SWI6		   ,
	VDEV_INT_SWI7		   ,
	VDEV_INT_STM0_END	   ,
	VDEV_INT_STM1_END	   ,
	VDEV_INT_STM2_END	   ,
	VDEV_INT_STM3_END	   ,
	VDEV_INT_MP3_END	   ,
	VDEV_INT_ALL 	 = 0xFF
	
}VDEV_INTERRUPTID;


/* interrupt service routine */
typedef VOID (* VDEV_ISR)(VOID);

VOID 		Vdev_Standby2Idle(VOID);
VOID 		Vdev_Idle2Normal(VOID);
VOID 		Vdev_Normal2Idle(VOID);
VOID 		Vdev_Idle2Standby(VOID);

VOID		Vdev_ClearFifo			(UINT8 bFifoType);
VOID		Vdev_ClearFlag			(UINT8 bIntType);
VOID		Vdev_ClearSwi			(UINT8 bIntType);
UINT8		Vdev_GetSwiState		(UINT8 bSwiFlag);
VOID		Vdev_StopChnl			(UINT8 bFifoType);

void		Vdev_ClearStmFlag		(UINT8 bEndNum);
UINT8		Vdev_GetStmState		(UINT8 bEndNum);

VOID		Vdev_WriteAes			(UINT8 bAesMode, PUINT8 pbKey, PUINT8 pbInitVec);
VOID		Vdev_WriteFifo			(UINT8 bFifoType, PUINT8 pbFifodata, UINT16 wSize);

VOID		Vdev_SetAudioMode		(UINT8 bAudioMode);
VOID		Vdev_SetWaterM			(UINT8 bFifoType, UINT8 bWaterM);
VOID		Vdev_SetBaseAddr		(UINT8 bFifoType, UINT16 wBaseAddr);
VOID		Vdev_SetEndPtr			(UINT8 bFifoType, UINT16 wEndPtr);
VOID		Vdev_LoadAddr			(UINT8 bFifoType);

VOID		Vdev_SetVol				(UINT8 bCtrlID, UINT8 bVolL, UINT8 bVolR);
VOID		Vdev_CtrlSeq			(UINT8 bCtrlID);

UINT8		Vdev_GetCFifoState		(VOID);
VOID		Vdev_DeviceControl		(UINT8 bCtrlID, UINT32 dParam1, UINT32 dParam2);
VOID		Vdev_AddIsr				(UINT8 bIntType, VDEV_ISR fDev_Isr);
VOID		Vdev_RemoveIsr			(UINT8 bIntType);

VOID		Vdev_MaskInt			(UINT8 bIntType);
VOID		Vdev_UnMaskInt			(UINT8 bIntType);

VOID		Vdev_StartClk			(UINT8 bPllMode);
VOID		Vdev_StopClk			(VOID);

VOID		Vdev_SetTimer			(UINT8 bTimerNo, UINT32 dCount);
UINT32		Vdev_GetTimerCount		(UINT8 bTimerNo);
VOID		Vdev_StartTimer			(UINT8 bTimerNo);
VOID		Vdev_StopTimer			(UINT8 bTimerNo);
VOID		Vdev_ResetTimer			(UINT8 bTimerNo);

VOID		Vdev_MainIsr			(VOID);


void 		Vdev_Normal2Start		(void);
void 		Vdev_Normal2End			(void);

UINT32 		Vdev_GetCurPolyphonyNum	(VOID);
VOID		Vdev_EnableChnStop		(UINT8 bStmChn);
VOID		Vdev_DisableChnStop		(UINT8 bStmChn);
void 		Vdev_SelectV8051RAM		(void);
void 		Vdev_UnSelectV8051RAM	(void);


#endif /*__VDEV_H__*/

