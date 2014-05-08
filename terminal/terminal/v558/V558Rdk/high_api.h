/*************************************************************************/
/*                                                                      		 	*/
/* FILE NAME                                      VERSION                	*/
/*                                                                       			*/
/* highapi.h                                	  1.0                    		*/
/*                                                                       			*/
/* DESCRIPTION                                                           	*/
/*                                                                       			*/
/*     Vimicro558 User's API		                                 		*/
/*										 			*/
/* The last modification date:  02-23-2004                             */
/* REMARKS:  Created initial version 1.0                                */
/*                                                                       			*/
/*                Copyright (C) 2004 Vimicro CO.,LTD     		*/
/*************************************************************************/
/****************************************************************************
This source code has been made available to you by VIMICRO on an
AS-IS basis. Anyone receiving this source code is licensed under VIMICRO
copyrights to use it in any way he or she deems fit, including copying it,
modifying it, compiling it, and redistributing it either with or without
modifications. Any person who transfers this source code or any derivative
work must include the VIMICRO copyright notice and this paragraph in
the transferred software.
****************************************************************************/
#ifndef _HIGH_API_H_
#define _HIGH_API_H_

#include "vregdef.h"
#include "v558def.h"
#include "v558Api.h"


#ifdef __cplusplus
extern "C" {
#endif
#define FIFO_INIT 0
#define FIFO_FULL 1
#define FIFO_END  2
#define MUTISHOT_END 3
#define DISPLAY_THUMB 4
#define YUV_READ 5
#define YUV_END 6

typedef enum
{       //W*H
	 R160_120=0,
	 R128_160,
	 R128_96,
	 R96_128,
	 R112_84,
	 R96_72,
	R80_60
}_US558_Resolution;

#if 0
typedef enum _V558_SNR_POWER_STATE
{
	V558_SNR_POWER_OFF		= 0X0,
	V558_SNR_POWER_SLEEP		,
	V558_SNR_POWER_ON
}V558_SNR_POWER_STATE;

#endif //liuyong 2004.3.2
typedef enum
{
	STATE558_OFF	= 0,  
	STATE558_Initial,  
       STATE558_Sleep,
	STATE558_CamOpen,
	STATE558_Preview,
	STATE558_Video,
	STATE558_Capture,
	STATE558_Display,
	STATE558_EnDecode,
	STATE558_DisplayPause,
	STATE558_Idle,	
	STATE558_ErrorPara,
       STATE558_NoWorkMemory,
       STATE558_UnknowError
}_US558_WorkMode;

typedef struct _US558_PREVIEW_PARA
{
	UINT16 Source_H,Source_W;
	UINT16 Display_H,Display_W;
	UINT16 Capture_H,Capture_W;
	UINT16 Source_OffsetH,Source_OffsetW;
	UINT16 Display_OffsetH,Display_OffsetW;
	UINT16 Lcd_Gamma,reserve2;
} US558_PREVIEW_PARA, *	pUS558_PREVIEW_PARA;

extern  US558_PREVIEW_PARA  g_Preview_Para;
extern UINT16 g_Thumb_X;
extern UINT16 g_Thumb_Y;

extern void V558_SetVal_By(UINT16 uVal);
extern void V558_SetCmd_By(UINT16 uVal);
extern void V558_SetData_By(UINT16 uCmd, UINT8* pData, UINT32 uSize);
extern UINT32 V558_YuvToRaw(UINT8* pOut,UINT8* pIn,UINT8 rawFormat,UINT8 yuvFormat,V558_SIZE size,UINT8 rate) ;

typedef	UINT8* (* USER558_CallBack)(UINT8 Status, UINT16 Byte_Length, UINT32 *BufLen);
extern USER558_CallBack gUSER_CallBack;
extern UINT8 g_VideoIndex;

UINT8 Test_RW_Register(void);
void USER558_CtrlIntHandle(void);
void USER558_Set558CLK(UINT16 xclk, UINT16 mclk);
void USER558_Set558Interrupt(UINT8 LeverOrEdge,UINT8 HighOrLow, UINT8 Width);
void USER558_Init558BUS(void);
void USER558_Init(void);
void USER558_CamOpen(void);
void USER558_CamClose(void);
void USER558_PreviewOnOff(UINT8 OnOff);
UINT8 USER558_SetPreviewParameter(UINT16 Width, UINT16 Height, UINT16 Lcd_OffsetW,UINT16 Lcd_OffsetH);
UINT8 USER558_SetCaptureParameter(UINT16 CapWidth, UINT16 CapHeight);
//----------------capture-----------------------------------------------------------------
void USER558_CaptureStill(USER558_CallBack pUCallBack);
void USER558_CaptureLCDStill(USER558_CallBack pUCallBack);
void USER558_CaptureThumbStill(UINT8* Framebuf);
void USER558_CaptureVideo_Start(USER558_CallBack pUCallBack);
void USER558_CaptureLCDVideo_Start(USER558_CallBack pUCallBack);
void USER558_CaptureVideo_Stop(void);
void USER558_MultiShot_Start(UINT8 *FrameBUF, UINT8 *ThumbBUF, UINT8 ShotNumber,USER558_CallBack pUCallBack);
void USER558_MultiShot_Stop(void);
UINT8* USER558_GetMultiShotFrame(UINT8 *BUF,UINT8 Number, UINT32* Length);
UINT8* USER558_GetMultiShotThumb(UINT8 *BUF,UINT8 Number, UINT32* Length);
void USER558_UpdateThum( UINT8 rawFormat,UINT16 startx, UINT16 starty,UINT16 Width,UINT16 Height, UINT8*uspImage );
//----------display----------------------------------------------------------------------
void USER558_DisplayJpegFile(UINT8 *jpegBuf, UINT16 Lcd_OffsetX, UINT16 Lcd_OffsetY, UINT16 Display_W, UINT16 Display_H);
void USER558_DisplayVideo(UINT8 FrameRate, USER558_CallBack pUCallBack);
void USER558_DisplayVideoStop(void);
void USER558_DisplayVideoPause(void);
//-----------format transfer----------------------------------------------------------
UINT8 USER558_JpegDecode(UINT8 *pJPEG, UINT8 *pDest,UINT16 Target_Width,USER558_CallBack pUCallBack);
void USER558_JpegEncode(UINT8 *pJPEG, UINT8 *pDest,UINT16 Source_Width,UINT16 Source_height,USER558_CallBack pUCallBack);
void USER558BmpToYuv(UINT8*BmpBuf,UINT8*YuvBuf,UINT8 TYPE);
void USER558_YUV2RGB(UINT8* pYUV,UINT8* pBMP,UINT8 rawFormat,UINT8 yuvFormat,UINT16 Width,UINT16 Height);
//---------image effect--------------------------------------------------------------
void USER558_SetCaptureSize(UINT32 ByteSize);
void USER558_SetFrequencyType(UINT8 freqType);
void USER558_Set_ExposureMode(UINT8 MODE);
void USER558_SetCaptureQuaility(UINT8 rate);
void USER558_BrightnessAdjust(UINT8 Mode,UINT8 Step);
void USER558_ContrastAdjust(UINT8 Mode,UINT8 Step);
void USER558_SharpnessAdjust(UINT8 Value);
void USER558_SetLightCondition(UINT8 Mode);
//--------special effect--------------------------------------------------------------
UINT16  USER558_GetZoomSteps(UINT8 mode);
UINT16 USER558_PreviewZoom(UINT8 mode,UINT16 step,UINT16 x,UINT16 y);
UINT16  USER558_GetZoomDisplaySteps(UINT8 mode);
UINT16 USER558_PreviewZoomDisplay(UINT8 mode,UINT16 step);
void USER558_Rotation(UINT8 mode);
void USER558_Set_Special_Effect(UINT8 Mode);
void USER558_Mirror(UINT8 mode);
void USER558_FrameOn(UINT8 OnOff,UINT16 KeyColor, UINT8 *BMP);
void USER558_BlendingOnOff(UINT8 OnOff,UINT8 *BMP) ;
void  USER558_blanding_Value(UINT8 value);
void USER558_AddDecBlending(UINT8 mode,UINT8 step);
//---------lcd control--------------------------------------------------------------
void USER558_Set_Bypass(UINT8 Mode,UINT8 PANEL);
void USER558_SetData_By(UINT16 uCmd, UINT8* pData, UINT32 uSize);
void USER558_SetCmd_By(UINT16 uVal);
void USER558_SetVal_By(UINT16 uVal);
void USER558_SetPanelStatus(UINT8 Panel,UINT8 Status);
UINT8 USER558_GetCurrentLCD(void);
//-------user's api for Graphic-------------------------------------------------------
void USER558_LCDUpdate(void);
void USER558_LCDUpdateRegion(UINT16 startx, UINT16 starty,UINT16 endx,UINT16 endy);
void USER558_InitLCD( UINT16 OffX,UINT16 OffY,UINT16 Width, UINT16 Height ,UINT8 BPP);
void USER558_LCDDisplayOn( UINT8 OnSelect, UINT8 DisplayMode, UINT16 KeyColor );
void USER558_LCDDisplayOff(UINT8 OffSelect);
UINT8 USER558_LCDStartDrawDot(void);
void USER558_LCDStopDrawDot(UINT8 DisType) ;
void USER558_LCDDrawOneDot( UINT16 x, UINT16 y, UINT16 color );
void USER558_LCDDrawPureColor(UINT16 color);
void USER558_LCDDrawLine( UINT16 startx, UINT16 starty,UINT16 endx,UINT16 endy,UINT32 Color );
void USER558_LCDDrawHorizontalLine(UINT32 x, UINT32 y, UINT8 len, UINT16 color);
void USER558_LCDDrawVerticalLine(UINT32 x, UINT32 y, UINT8 len, UINT16 color);
void USER558_LCDDrawRctngl( UINT16 startx, UINT16 starty,UINT16 endx,UINT16 endy,UINT8*uspImage );
void USER558_LCDGeCopy(UINT16 Startx,UINT16 Starty,UINT16 Stopx,UINT16 Stopy,UINT16 Whith,UINT16 Heigh);
void USER558_LCDDrawBMP( UINT16 startx, UINT16 starty,UINT8*BmpBuf,UINT8 Converse);
//----------gpio---------------------------------------------------------------------------
void USER558_GetGpioData(UINT8 *GpioData);
void USER558_SetGpioData(UINT8 data);
void USER558_SetGpioMode(UINT8 OorI);
//--------------------------------------------------------------------------------------
UINT8* USER558_GetRDKVersion(void); //length=46 byte

#ifdef __cplusplus
}
#endif



#endif
