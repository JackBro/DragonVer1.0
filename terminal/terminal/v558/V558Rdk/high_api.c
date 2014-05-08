/*************************************************************************/
/*                                                                       			*/
/* FILE NAME                                      VERSION                	*/
/*                                                                       			*/
/* highapi.c                                	  1.2                    		*/
/*                                                                       			*/
/* DESCRIPTION                                                           	*/
/*                                                                       			*/
/*     Vimicro558 User's API		                                 		*/
/*									 				*/
/* The last modification date:  06-23-2004                             */
/* REMARKS:  Created initial version 1.2                                */
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


#include "vregdef.h"
#include "v558Api.h"
#include "vjpeg.h"
#include "vlcd.h"
#include "vipp.h"

#include "vctrl.h"
#include "visp.h"
#include "high_api.h"
#include "vexif.h"

UINT8 const HAPI_VERSION[5]={04,07,20,2,0};
UINT8 g_558_WorkStatus=STATE558_OFF;
US558_PREVIEW_PARA  g_Preview_Para;
UINT8 *g_pBUF;
UINT32 g_Length;


USER558_CallBack gUSER_CallBack;
UINT8 g_VideoIndex=0; 
UINT32 g_CaptureSize;
//-------FOR multishot--------------------------------
#define MAX_MUTISHOT_COUNT 9
UINT8 g_MutiShot_Flag=0;
UINT8 g_MutiShot_TotCount=0;
UINT8 g_MutiShot_NowCount=0;
UINT32 g_MutiShot_FrameLength[MAX_MUTISHOT_COUNT];
UINT32 g_MutiShot_ThumbLength[MAX_MUTISHOT_COUNT];
UINT8* g_MutiShot_pFRAME;
UINT8* g_MutiShot_pTHUMB;
UINT8 g_MutiShot_OneFrame=0;
UINT16 g_Thumb_X;
UINT16 g_Thumb_Y;
UINT8 g_VideoStop=0;
//--------------------------------------------------

/////////////////////////////////////////
extern V558_PANELINFO Panel_Info;
extern V558_PANELINFO SlavePanel_Info;
extern V558_SENSORINFO SensorInfo;
extern V558_IPPINFO IppInfo;
extern V558_JPEGINFO JpegInfo;
extern V558_CTRL_INFO CtrlInfo;
//////////////////////////////////////////
UINT16 Find_common_divisor(UINT16 x,UINT16 y)
{
       UINT16 Temp,i;
	if(x>y) Temp=y;
	else Temp=x;
	for(i=Temp;i>0;i--)
	{
		if((x%i==0)&(y%i==0))
		{
			return i;
		}
	}
	return 0;
}

UINT16 FindMaxRatio(UINT16 Width,UINT16 Height) //640*480 
{
	UINT16 ratio=1;
       while(1)
       	{
       	 	if(ratio*Width>640||ratio*Height>480)
       	 		break;
       	 	ratio++;
       	}
       return (ratio-1);
}
UINT8 FindSameRatio(UINT16 Width1,UINT16 Height1,UINT16 Width2,UINT16 Height2)
{
UINT16 divisor1,divisor2;
      divisor1=Find_common_divisor(Width1,Height1);
      divisor2=Find_common_divisor(Width2,Height2);
      if(((Width1/divisor1)==(Width2/divisor2))&&((Height1/divisor1)==(Height2/divisor2)))
      		return TRUE;
      else
      		return FALSE;
}
void V558_ChangePanelCs(UINT8 Mode)
{
	UINT16 LC;
	UINT8 RET;
	LC=V558_LcdGetIFControl();
	if(Mode)
		V558_LcdSetIFControl(LC|0x10);		//sub panel
	else
		V558_LcdSetIFControl(LC&0xffef);
	V558_GetReg( V558_REG_LCD_LC, &RET);
}
/********************************************************************************
Description: 
	Initialize 558(global parameters, LCD panel, Sensor module)
 
Note:
      This function should be called before V558 can work

Remarks:    
      State:Valid

*********************************************************************************/
void USER558_Init(void)
{
	g_Preview_Para.Capture_W=128;
       g_Preview_Para.Capture_H=160;

	g_558_WorkStatus=STATE558_Initial;
	g_Preview_Para.Lcd_Gamma=0;
	V558_CtrlInitContext();
	V558_LcdInitContext();
	V558_IppInitContext();
	V558_SifInitContext();
	V558_JpegInitContext();
	
	V558_CtrlSetInfo(&CtrlInfo);
	V558_LcdSetPanelInfo(&Panel_Info);
	#if TwoPanel
	V558_LcdSetSlavePanelInfo(&SlavePanel_Info);
	#endif
	V558_SifSetSensorInfo(&SensorInfo);
	V558_IppSetInfo(&IppInfo);
	V558_JpegSetInfo(&JpegInfo);
	
	V558_CtrlOpen();	
	V558_LcdOpen();
	V558_SifOpen();
	V558_IppOpen();

	V558_LbufOpen();
	V558_JbufOpen();
	V558_JpegOpen();
	
	V558_SifDisableSyncGen();
	USER558_SetCaptureSize(0x2000);
	USER558_SetPreviewParameter(128,160,0,0);
       USER558_SetCaptureParameter(128,160);	
	g_558_WorkStatus=STATE558_Initial;
	
}
/********************************************************************************
Description: 
	Open the 558 camera function

Note:
       This function must used before view finder mode start

Remarks:    
        State:Valid
*********************************************************************************/
void USER558_CamOpen(void)  
{	

	V558_CtrlSetModClkOn(V558_MOD_CLK_SIF);
	V558_CtrlSetModClkOn(V558_MOD_CLK_ISP);
	V558_CtrlSetModClkOn(V558_MOD_CLK_IPP);
	V558_CtrlSetModClkOn(V558_MOD_CLK_LB);
	V558_CtrlSetModClkOn(V558_MOD_CLK_JPG);	

	V558_Delay(20);
	V558_LbufOpen();
	V558_JbufOpen();
	V558_JpegOpen();

	g_558_WorkStatus=STATE558_CamOpen;
	V558_Delay(20);
}
/********************************************************************************
Description: 
	Close the 558 camera function

Remarks:    
	State:Valid
*********************************************************************************/
void USER558_CamClose(void)
{

	V558_CtrlSetModClkOff(V558_MOD_CLK_SIF);
	V558_CtrlSetModClkOff(V558_MOD_CLK_ISP);
	V558_CtrlSetModClkOff(V558_MOD_CLK_IPP);
	V558_CtrlSetModClkOff(V558_MOD_CLK_JPG);	
	g_558_WorkStatus=STATE558_Initial;	
}
/*********************************************************************************
Description: 
	This function only for test.
	Test read/write V558's registers right or not

Note:
	If return 0, read/write register right, otherwise error.

Remarks:    
	State:Valid
*********************************************************************************/
UINT8 Test_RW_Register(void)
{
     UINT8 tmp;
     V558_GetReg(V558_REG_HCTRL_CHIPCTRL, &tmp);
     if(tmp!=0x58) return 1;
       V558_GetReg(V558_REG_HCTRL_CHIPCTRL+1, &tmp);
     if(tmp!=0x05) return 2;
     V558_SetReg(V558_REG_IPP_SIZWD, 120);
     V558_GetReg(V558_REG_IPP_SIZWD, &tmp);
     if(tmp!=120) return 3;
     V558_SetReg(V558_REG_IPP_SIZHT, 160);
     V558_GetReg(V558_REG_IPP_SIZHT, &tmp);
     if(tmp!=160) return 4;
     V558_SetReg(V558_REG_BIU_OP_MODE, 0xaa);
     V558_GetReg(V558_REG_BIU_OP_MODE, &tmp);
     if(tmp!=0xaa) return 5;
      V558_SetReg(V558_REG_IPP_SIZWD+1, 1);
     V558_GetReg(V558_REG_IPP_SIZWD+1, &tmp);
     if(tmp!=1) return 6;
     return 0;
}
/*********************************************************************************
Description: 
	This function is the interrupt process routine for 558
	Users should add this function to the related interrupt handler, or put it in the mainloop.

Remarks:    
	State:Checking
*********************************************************************************/
void USER558_CtrlIntHandle(void)
{
	V558_CtrlIntHandle();
}
/********************************************************************************
Description: 
  	Set 558 PLL
  
Parameters:
  	xclk: input clk(KHZ);
  	mclk: pll output clk(KHZ);
  	

Note:

Remarks:    

*********************************************************************************/
void USER558_Set558CLK(UINT16 xclk, UINT16 mclk)
{  
	V558_CtrlSetChipClk(xclk, mclk, 1, 1);
	V558_SetReg(V558_REG_HCTRL_CLKOFF,0x00);
}

/********************************************************************************

	Description:
		set Interrupt parameter

	Parameters:
		LeverOrEdge: Trigger type, 0 for edge trigger and 1 for level trigger;
		HighOrLow:  Edge active fall or rise, Level active low or high;
			    1 for high active,0 for low active;
		Width: hexadecimal number.how many clk width of interrupt trigger.

	Note:
	       This function must be used before use 558 function related to interrupt
		
	Remarks:
		state: checking

*********************************************************************************/
void USER558_Set558Interrupt(UINT8 LeverOrEdge,UINT8 HighOrLow, UINT8 Width)
{
	UINT8 Temp=0;
	if(LeverOrEdge)		
	{
		Temp|=1;
		if(!HighOrLow)		
			Temp|=0x02;
		else					
			Temp&=0xfd;
	}
	else
	{
		Temp&=0xfe;
		if(!HighOrLow)		
			Temp|=0x04;
		else					//high
			Temp&=0xfb;
	}
	if(Width)
		Width--;
	Width<<=3;
	Temp|=Width;
	V558_SetReg((UINT16)V558_REG_HCTRL_INTCTRL,Temp);
}
/********************************************************************************

	Description:
		set preview parameter

	Parameters:
		Width: the width of display on lcd.
		height: the height of display on lcd.
		Lcd_OffsetW: offset x value on lcd.
		Lcd_OffsetH: offset y value on lcd.
		Return: error number

	Note:
	       This function must be used before first previewor change the resolution of display.
	       <TABLE>
		value					meaning
		-----					-------
		1                                       
		0                                       right
		</TABLE>

	Remarks:
		state: Valid

*********************************************************************************/
UINT8 USER558_SetPreviewParameter(UINT16 Width, UINT16 Height, UINT16 Lcd_OffsetW,UINT16 Lcd_OffsetH)
{
    	g_Preview_Para.Display_OffsetH= Lcd_OffsetH;
	g_Preview_Para.Display_OffsetW= Lcd_OffsetW&0xfffc;

	g_Preview_Para.Display_H=Height;		
       g_Preview_Para.Display_W=Width;
	return 0;
}
/********************************************************************************

	Description:
		Set Captured jpeg file size

	Parameters:
		ByteSize: the captured jpeg file size
	       
	Remarks:
		State:Valid.
*********************************************************************************/
void USER558_SetCaptureSize(UINT32 ByteSize)
{
	g_CaptureSize=ByteSize;
	V558_JpegSetTWC(ByteSize>>2);		
}
/********************************************************************************

	Description:
		Set Capture still quality

	Parameters:
		rate: quality step

       Note: 
		<TABLE>
		value					meaning
		-----					-------
		0					low quality
		1					normal quality
		2					high quality
           	
	Remarks:
		State:Valid.
*********************************************************************************/
void USER558_SetCaptureQuaility(UINT8 rate)
{
	if(rate==2)
		USER558_SetCaptureSize(0xe000);
	else if(rate==1)
		USER558_SetCaptureSize(0x7000);
	else
		USER558_SetCaptureSize(0x1000);
}
/********************************************************************************

	Description:
		Set Frequency type 50hz or 60 hz

	Parameters:
		freqType: Frequency type

    Note: 
		<TABLE>
		value					meaning
		-----					-------
		0					50 hz
		1					60 hz
      
 	Remarks:
 	    RGB sensor only
		State:Valid.
*********************************************************************************/
void USER558_SetFrequencyType(UINT8 freqType)
{
	UINT8 NowFreq,temp=0;
	temp=V558_IspGetFreq(&NowFreq);
	if(temp)
		{
		if(NowFreq!=freqType)
			V558_IspSetFreq(freqType);
		else
			return;
		}
		
}


/********************************************************************************

	Description:
		Set Exposure Mode

	Parameters:
		MODE: Exposure Mode

       Note: 
		<TABLE>
		value					meaning
		-----					-------
		0						IN door mode
		1						out door mode
		2 						Night mode
           	
	Remarks:
		State:Valid.
*********************************************************************************/
void USER558_Set_ExposureMode(UINT8 MODE)
{
	UINT8 NowMode,temp=0;
	temp=V558_IspGetLight(&NowMode);
	if(temp)
		{
		if(NowMode!=MODE)
			V558_IspSetLight(MODE);
		else
			return;
		}
}

/********************************************************************************

	Description:
		Set Capture still parameter

	Parameters:
		Width: the width of display on lcd.
		height: the height of display on lcd.
		return: Error number

	Note:
	This function must be used before first previewor change the resolution of display.
	       <TABLE>
		value					meaning
		-----					-------
		1                                       
		0                                       right
		</TABLE>	

	Remarks:
		State:Valid.
*********************************************************************************/
UINT8 USER558_SetCaptureParameter(UINT16 CapWidth, UINT16 CapHeight)
{
	UINT16 ratio;
	UINT16 Width,Height,W,H;
	UINT8 ErrorNumber=1,Sel,temp;
	V558_POINT pt;
	V558_SIZE size;
	V558_LAYER_PROPERTY proty;
	UINT16 MaxDiv;

	//CapWidth&=0xfff0;
	//CapHeight&=0xfff0;	
	ratio=Find_common_divisor(CapWidth,CapHeight);
	W=CapWidth/ratio;
	H=CapHeight/ratio;
	
	ratio=FindMaxRatio(W,H);// max ratio
	
	Width=W*ratio;
	Height=H*ratio;

        g_Preview_Para.Source_H=Height;
        g_Preview_Para.Source_W=Width;
	 while(g_Preview_Para.Source_W%16)
		{
			ErrorNumber=0;
		  	g_Preview_Para.Source_W-=W;
		  	g_Preview_Para.Source_H-=H;
		}
        
        g_Preview_Para.Source_OffsetH=(480-g_Preview_Para.Source_H)/2;
        g_Preview_Para.Source_OffsetW=(640-g_Preview_Para.Source_W)/2;
        g_Preview_Para.Capture_W=CapWidth;
        g_Preview_Para.Capture_H=CapHeight;

        
        pt.x=  g_Preview_Para.Source_OffsetW;
        pt.y=  g_Preview_Para.Source_OffsetH;
        size.cx= g_Preview_Para.Source_W;
        size.cy=  g_Preview_Para.Source_H;

	if(!FindSameRatio(g_Preview_Para.Source_W, g_Preview_Para.Source_H, g_Preview_Para.Display_W,g_Preview_Para.Display_H))
	{
		MaxDiv=Find_common_divisor(g_Preview_Para.Source_W,g_Preview_Para.Source_H);
		W=g_Preview_Para.Source_W/MaxDiv;
		H=g_Preview_Para.Source_H/MaxDiv;
		ratio=1;
		while(1)
		{
				if((W*ratio>=g_Preview_Para.Display_W)&&(H*ratio>=g_Preview_Para.Display_H))
					break;
				ratio++;
		}
			
		proty.Size.cx = W*ratio;
		proty.Size.cy = H*ratio;
		while(proty.Size.cx%16)
		{
				proty.Size.cx+=W;
				proty.Size.cy+=H;
		}
			
		proty.DisRect.left = (W*ratio-g_Preview_Para.Display_W)/2;
		if(proty.DisRect.left %2)		
			proty.DisRect.left --;
		proty.DisRect.top =  (H*ratio-g_Preview_Para.Display_H)/2;
		proty.DisRect.width = g_Preview_Para.Display_W;
		proty.DisRect.height =g_Preview_Para.Display_H;

		proty.DisPoint.x = g_Preview_Para.Display_OffsetW;
		proty.DisPoint.y = g_Preview_Para.Display_OffsetH;			
	}
	else
	{	
		proty.Size.cx = g_Preview_Para.Display_W;
		proty.Size.cy = g_Preview_Para.Display_H;			
		proty.DisRect.left = 0;
		proty.DisRect.top = 0;
		proty.DisRect.width = g_Preview_Para.Display_W;
		proty.DisRect.height =g_Preview_Para.Display_H;
		proty.DisPoint.x = g_Preview_Para.Display_OffsetW;
		proty.DisPoint.y = g_Preview_Para.Display_OffsetH;
	}
	Sel=V558_SifDisableSyncGen();
	temp=V558_CtrlGetWorkMode();
	V558_CtrlSetWorkMode(V558_MODE_VIEW);	
	V558_IppSetSizerCapture(pt,size,CapWidth);
	//V558_IppSetSizerDisplay(pt, size, proty.Size.cx);
	V558_IppSetDisplay( proty.Size.cx);
	
       V558_LcdSetALProperty(&proty);
	V558_SifSetSensorOpmode(Sel);
	V558_CtrlSetWorkMode(temp);	
	return 0;
}
/********************************************************************************

	Description:
		Set Frame On or Off and Set the key Color when frame on.

	Parameters:
		OnOff :  Frame On Or Off.
		KeyColor: The KeyColor Which can be transparent.
		*BMP: The header pointer of the BMP(not standard bmp file)

	Note:
		This function must be used in preview mode.
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_FrameOn(UINT8 OnOff,UINT16 KeyColor, UINT8 *BMP) 
{
	V558_POINT point;

      if(OnOff)
      	{
      		V558_LcdALDisable();      		
      		point.x = g_Preview_Para.Display_OffsetW;
		point.y = g_Preview_Para.Display_OffsetH;		
		V558_Delay(100);			
		V558_LcdDrawBmp(point,BMP,0);	
		//USER558_LCDDrawRctngl(point.x,point.y,128,160,BMP);
		V558_Delay(100);					
		USER558_LCDDisplayOn(3,0,KeyColor);
      	}
      else
      	{
    		USER558_LCDDisplayOn(3,1,0);
      	}
}
/********************************************************************************

	Description:
		Set Blending On or Off 
		
	Parameters:
		OnOff :  Blending On Or Off.
		*BMP: The head point of rgb(not standard bmp) 

	Note:
		This function must be used in preview mode.
		<TABLE>
		value					meaning
		-----					-------
		1                                       start Blending;
		0                                       stop Blending;
		</TABLE>
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_BlendingOnOff(UINT8 OnOff,UINT8 *BMP) 
{
	V558_POINT point;

	if(OnOff)
	{
	        point.x = g_Preview_Para.Display_OffsetW;
		 point.y = g_Preview_Para.Display_OffsetH;
	   	 USER558_LCDDisplayOn(3,1,0);
	   	 V558_LcdDispDisable();

      		 V558_LCDDrawBL(point.x,point.y,128,160,BMP);
      		 V558_LcdDispEnable();
      		 V558_LcdEnableBlend();
	}
	else
	{
		V558_LcdDisableBlend();
      	}
}
/********************************************************************************

	Description:
		Set Blending Value
		
	Parameters:
		Value :  Blending Value 0-255

	Note:
		This function must be used after Blending  start.		
		
	Remarks:
		state: Valid

*********************************************************************************/
void  USER558_blanding_Value(UINT8 Value)
{
	V558_LcdSetBlendRatio(Value);
}
/********************************************************************************

	Description:
		Up or Down Blending Value
		
	Parameters:
		mode: Add or Dec 
		step:  The step Value 0-255 

	Note:
		This function must be used after Blending  start.			
		<TABLE>
		value					meaning
		-----					-------
		1                                       Add; 
		0                                       Dec;
		</TABLE>
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_AddDecBlending(UINT8 mode,UINT8 step)  
{
     if(mode)
	V558_LcdUpBlendRatio(step);
     else
     	V558_LcdDownBlendRatio(step);
}


/********************************************************************************

	Description:
		preview on or off

	Parameters:
		ONOFF: start preview or stop preview

	Note:
	       This function must be used when capture or display had stoped .
		<TABLE>
		value					meaning
		-----					-------
		1                                       start preview;
		0                                       stop preview;
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void USER558_PreviewOnOff(UINT8 OnOff)
{
	V558_LAYER_PROPERTY proty;
	V558_SIZE   pSrcSize;
	V558_POINT pt ;
	UINT16 MaxDiv,W,H,Ratio=1;

	V558_Delay(10);
	if(OnOff)
	{

		if(!FindSameRatio(g_Preview_Para.Source_W, g_Preview_Para.Source_H, g_Preview_Para.Display_W,g_Preview_Para.Display_H))
		{
			MaxDiv=Find_common_divisor(g_Preview_Para.Source_W,g_Preview_Para.Source_H);
			W=g_Preview_Para.Source_W/MaxDiv;
			H=g_Preview_Para.Source_H/MaxDiv;
			while(1)
				{
					if((W*Ratio>=g_Preview_Para.Display_W)&&(H*Ratio>=g_Preview_Para.Display_H))
						break;
					Ratio++;
				}
			
			proty.Size.cx = W*Ratio;
			proty.Size.cy = H*Ratio;
			while(proty.Size.cx%16)
				{
					proty.Size.cx+=W;
					proty.Size.cy+=H;
				}
			
			proty.DisRect.left = (W*Ratio-g_Preview_Para.Display_W)/2;
			if(proty.DisRect.left %2)		
				proty.DisRect.left --;
			proty.DisRect.top =  (H*Ratio-g_Preview_Para.Display_H)/2;
			proty.DisRect.width = g_Preview_Para.Display_W;
			proty.DisRect.height =g_Preview_Para.Display_H;

			proty.DisPoint.x = g_Preview_Para.Display_OffsetW;
			proty.DisPoint.y = g_Preview_Para.Display_OffsetH;			
		}
		else
		{
	
			proty.Size.cx = g_Preview_Para.Display_W;
			proty.Size.cy = g_Preview_Para.Display_H;

			
			proty.DisRect.left = 0;
			proty.DisRect.top = 0;
			proty.DisRect.width = g_Preview_Para.Display_W;
			proty.DisRect.height =g_Preview_Para.Display_H;

			proty.DisPoint.x = g_Preview_Para.Display_OffsetW;
			proty.DisPoint.y = g_Preview_Para.Display_OffsetH;

		}
			pt.x=g_Preview_Para.Source_OffsetW;
			pt.y=g_Preview_Para.Source_OffsetH;
			pSrcSize.cx  = g_Preview_Para.Source_W;
			pSrcSize.cy  = g_Preview_Para.Source_H;
	
			V558_SifDisableSyncGen();	
	 		V558_CtrlSetWorkMode(V558_MODE_VIEW);	  			
			
			V558_IppSetSizerCapture(pt,pSrcSize,g_Preview_Para.Capture_W);
			V558_IppSetDisplay( proty.Size.cx);
			
			V558_LcdALEnable();	
       		V558_LcdSetALProperty(&proty);			     		
			V558_SifEnableSyncGen();					

		 	g_558_WorkStatus=STATE558_Preview;
	}
	else
	{
		V558_SifDisableSyncGen();
	       V558_LcdALDisable();
		g_558_WorkStatus=STATE558_Idle;
	}
				
}
////////////////////////////////////////////////////////////////////////////
void JpegNote(UINT8 msg, PV558_JPEGBUF ptr)
{
	V558_JPEGBUF Thumb_buf;
	V558_SIZE Thumb_size;

       if(msg==MSG_JBUF_FULL) // fifo full
       {
       	g_pBUF=(gUSER_CallBack)(FIFO_FULL,ptr->ActLen,0);
       		if(g_VideoStop)
       		{
       		V558_StopCapVideo();

       		}
       }
       else if(msg==MSG_READ_YUV)
       	{
       		g_pBUF=(gUSER_CallBack)(YUV_READ,ptr->ActLen,0);

       	}
       else if(msg==MSG_READ_YUV_DONE)
       	{
             	      g_pBUF=(gUSER_CallBack)(YUV_END,ptr->ActLen,0);
			
       	}
       else if(msg==MSG_CAPTURE_DONE)						     //end 
       {
		g_pBUF=(gUSER_CallBack)(FIFO_END,ptr->ActLen,0);
		if(g_VideoStop)
			{
				while(1)
					{
						if(*g_pBUF==0xd8)
							{
								*g_pBUF=0;
								g_pBUF--;
								if(*g_pBUF==0xff)
									{
									break;
									}
							}
						*g_pBUF=0;
						g_pBUF--;
					}
				g_VideoStop=0;
			}
		
       }
       
       if(!g_MutiShot_Flag)
      		 ptr->pData=g_pBUF;
       else
       {
       	 g_MutiShot_pFRAME+=ptr->ActLen;
      		 ptr->pData=g_MutiShot_pFRAME; 
      		 g_MutiShot_FrameLength[g_MutiShot_NowCount]+=ptr->ActLen;
      		 if(msg==MSG_CAPTURE_DONE)
      		 {
			Thumb_buf.ActLen = 0;
			Thumb_buf.Len = 4096;
			Thumb_buf.pData = g_MutiShot_pTHUMB;
			V558_JbufReadThumb(&Thumb_buf, &Thumb_size);
			g_Thumb_X=Thumb_size.cx;
			g_Thumb_Y=Thumb_size.cy;			
			g_MutiShot_ThumbLength[g_MutiShot_NowCount]=4096;
			g_MutiShot_pTHUMB+=4096;
			g_MutiShot_NowCount++;
			if(g_MutiShot_NowCount<g_MutiShot_TotCount)
			{
				g_MutiShot_OneFrame=1;
			}
			else
			{
				g_MutiShot_Flag=0;
				(gUSER_CallBack)(MUTISHOT_END,0,0);
			}
      		 }
       }
}

/********************************************************************************

	Description:
		Capture a still JPEG

	Parameters:
		pUCallBack: User's call back function for memory assigning  and JPEG saving

	Note:
	       Before capture a still, the V558 should be in preview mode

	Remarks:
	       Please refer to USER558_SetCaptureParameter() for capture size

*********************************************************************************/
void USER558_CaptureStill(USER558_CallBack pUCallBack)
{
	V558_JPEGBUF g_buf[2];
	V558_LcdDispDisable();

     	gUSER_CallBack=pUCallBack;
       g_Length=0;
       g_pBUF=(gUSER_CallBack)(FIFO_INIT,0,0);

	g_buf[0].ActLen = 0;
	g_buf[0].Len = 0x8000;
	g_buf[0].pData = g_pBUF;

	V558_CaptureStill(g_buf, JpegNote);
	g_558_WorkStatus=STATE558_Capture;
}
/********************************************************************************

	Description:
		Capture a still JPEG from LCD display(include A&B layer)

	Parameters:
		pUCallBack: User's call back function for memory assigning  and JPEG saving

	Note:
	       Before capture a still, the V558 should be in preview mode

	Remarks:
		Please refer to USER558_SetCaptureParameter() for capture size

*********************************************************************************/
void USER558_CaptureLCDStill(USER558_CallBack pUCallBack)
{
	V558_JPEGBUF g_buf[2];

	gUSER_CallBack=pUCallBack;
       g_Length=0;
       g_pBUF=(gUSER_CallBack)(FIFO_INIT,0,0);

	g_buf[0].ActLen = 0;
	g_buf[0].Len = 0x8000;
	g_buf[0].pData = g_pBUF;

	V558_CaptureFrame(g_buf, JpegNote);
	g_558_WorkStatus=STATE558_Capture;
}

/********************************************************************************

	Description:
		Decod a jpeg file to yuv .

	Parameters:
		pJPEG: the buffer for source jpeg  frame saving.
		pDest: the buffer for destination yuv file.	
		Target_Width: destination yuv file's width
		(if jpeg is type420, this parameter is negelect, the target YUV size is same as source JPEG size)
		pUCallBack: User's call back function .
		
	Return:
		The YUV data format: 0--YUV422;1--YUV420;2--YUV411;3--YUV400;
		else error;
	Note:
		Target_Width:the targt width must less than the width of source image.
	       the length width ratio should be as same as the source image

*********************************************************************************/
UINT8 USER558_JpegDecode(UINT8 *pJPEG, UINT8 *pDest,UINT16 Target_Width,USER558_CallBack pUCallBack)
{
	V558_JPEGBUF decBuf;
	V558_JPEGBUF SourBuf;
	V558_JPEG_INDEX pinfo;
	UINT8 temp=0xff;
	gUSER_CallBack=pUCallBack;
       g_Length=0;
       g_pBUF=(gUSER_CallBack)(FIFO_INIT,0,0);


	SourBuf.ActLen= 0x10000;
	SourBuf.Len = 0x10000;
	SourBuf.pData = pJPEG;

	decBuf.ActLen = 0;
	decBuf.Len = 0x80000;
	decBuf.pData = pDest;

	if(V558_JpegParse(&SourBuf, &pinfo)) return temp;
	temp=pinfo.YUVType;
	if(temp==1)
	{
		V558_HostDecode(&SourBuf, &decBuf,0, &pinfo, JpegNote);
	}
	else
	{	
		V558_HostDecodeIpp(&SourBuf,&decBuf, 1,&pinfo,JpegNote,Target_Width);
	}

//UINT8 V558_HostDecode(PV558_JPEGBUF psrc, PV558_JPEGBUF pdst,UINT8 decdatatype, PV558_JPEG_INDEX pinfo, PV558_JPEGCALLBACK pcall)
	
	g_558_WorkStatus=STATE558_EnDecode;
	return temp;
}
/********************************************************************************

	Description:
		Decod a yuv file to jpeg .

	Parameters:
		pJPEG: the buffer for source yuv  frame saving.
		pDest: the buffer for destination jpeg file.	
		Source_Width: the width of source yuv file.
		Source_height: the height of source yuv file.
		pUCallBack: User's call back function .
		
	Note:
		Target_Width:the targt width must less than the width of source image.
	       the length width ratio should be as same as the source image

*********************************************************************************/
void USER558_JpegEncode(UINT8 *pJPEG, UINT8 *pDest,UINT16 Source_Width,UINT16 Source_height,USER558_CallBack pUCallBack)
{
	V558_JPEGBUF EncBuf;
	V558_JPEGBUF SourBuf;
	V558_JPEGBUF BlockBuf;
	V558_SIZE size;//,NewSize;
	
	UINT8 Block[0x10000];
	
	gUSER_CallBack=pUCallBack;
       g_Length=0;
       g_pBUF=(gUSER_CallBack)(FIFO_INIT,0,0);


	SourBuf.ActLen= 0x10000;
	SourBuf.Len = 0x10000;
	SourBuf.pData = pJPEG;

	EncBuf.ActLen = 0;
	EncBuf.Len = 0x80000;
	EncBuf.pData = pDest;

	BlockBuf.ActLen = 0;
	BlockBuf.Len = 0x80000;
	BlockBuf.pData = Block;

	size.cx=Source_Width;
	size.cy=Source_height;

//	V558_YUVPointToBlock(&SourBuf, &BlockBuf,&size,&NewSize,0);				
	V558_HostEncode(&SourBuf,& EncBuf, JpegNote,size,5);//NewSize, 0);
	g_558_WorkStatus=STATE558_EnDecode;
}

/********************************************************************************

	Description:
		Display a still JPEG on LCD(A layer)

	Parameters:
		jpegBuf: the JPEG buffer point
		Lcd_OffsetX: display offset X on LCD
		Lcd_OffsetY: display offset Y on LCD
		Display_W:   display width on LCD
		Display_H:    display hight on LCD

*********************************************************************************/
void USER558_DisplayJpegFile(UINT8 *jpegBuf, UINT16 Lcd_OffsetX, UINT16 Lcd_OffsetY, UINT16 Display_W, UINT16 Display_H)
{

	V558_LAYER_PROPERTY proty;
	V558_JPEGBUF decBuf;
	V558_JPEG_INDEX info;
	V558_LcdALEnable();
	proty.Size.cx = Display_W;
	proty.Size.cy = Display_H;
	proty.DisRect.left = 0;
	proty.DisRect.top = 0;
	proty.DisRect.width = Display_W;
	proty.DisRect.height =Display_H;
	proty.DisPoint.x = Lcd_OffsetX;
	proty.DisPoint.y = Lcd_OffsetY;
 	//V558_LcdSetALProperty(&proty);
       
	decBuf.ActLen = 0x10000;
	decBuf.pData = (UINT8 *)jpegBuf;
	decBuf.Len = 0x8000;

	if(V558_JpegParse(&decBuf, &info) == 0)
	{
		V558_DisplayStill(&decBuf, &info, &proty);
	}

	g_558_WorkStatus=STATE558_Display;
}
/********************************************************************************

	Description:
		Start capture video

	Parameters:
		pUCallBack: User's call back function for memory assigning  and video saving
		
	Note:
		Before capture a video, the V558 should be in preview mode
	Remarks:
		Please refer to USER558_SetVideoeParameter() for video size

*********************************************************************************/
void USER558_CaptureVideo_Start(USER558_CallBack pUCallBack)
{
	V558_JPEGBUF g_buf[2];

	gUSER_CallBack=pUCallBack;
       g_Length=0;
       g_pBUF=(gUSER_CallBack)(FIFO_INIT,0,0);

	//V558_IppSetCapture(g_Preview_Para.Capture_W);
	g_buf[0].ActLen = 0;
	g_buf[0].Len = 0x8000;
	g_buf[0].pData = g_pBUF;

	V558_StartCapVideo(g_buf, JpegNote);
	g_558_WorkStatus=STATE558_Video;
}
/********************************************************************************

	Description:
		Start capture video from LCD(include A&B layer),

	Parameters:
		pUCallBack: User's call back function for memory assigning  and video saving
	Note:
		Before capture a video, the V558 should be in preview mode
	Remarks:
		Please refer to USER558_SetVideoeParameter() for video size

*********************************************************************************/
void USER558_CaptureLCDVideo_Start(USER558_CallBack pUCallBack)
{
	V558_JPEGBUF g_buf[2];
	UINT16       LcdGamma;
	
	LcdGamma=V558_LcdGetIFControl();   
	if(LcdGamma&0x40)
		g_Preview_Para.Lcd_Gamma=1;
	else
		g_Preview_Para.Lcd_Gamma=0;
	V558_LcdDisableGamma();	
	
	gUSER_CallBack=pUCallBack;
       g_Length=0;
       g_pBUF=(gUSER_CallBack)(FIFO_INIT,0,0);

	g_buf[0].ActLen = 0;
	g_buf[0].Len = 0x8000;
	g_buf[0].pData = g_pBUF;

	V558_CaptureFrameVideo(g_buf, JpegNote);
	g_558_WorkStatus=STATE558_Video;

}
/********************************************************************************

	Description:
		Stop video capture
		
	Note:
		This function only can be callled after capture (LCD)video start

*********************************************************************************/
void USER558_CaptureVideo_Stop(void)
{
	//g_558_WorkStatus=STATE558_Idle;
	g_VideoStop=1;
}



/********************************************************************************

	Description:
		Capture a still attached thumbnail 

	Parameters:
		Framebuf: the head point of frame buffer

	Note:
	       Before capture a still, the V558 should be in preview mode

	Remarks:
		Please refer to USER558_SetCaptureParameter() for capture size

*********************************************************************************/
void USER558_CaptureThumbStill(UINT8* Framebuf)
{
        UINT16                width;
	V558_JPEGBUF g_buf[2];
	
	V558_SIZE pSrcSize;
	V558_POINT pt;

	V558_IppGetSizerStartp(&pt);
       width= g_Preview_Para.Capture_W;
	V558_IppGetSizerSize(&pSrcSize);
	V558_IppSetSizerCapture(pt, pSrcSize, width);      
	g_MutiShot_OneFrame=0;
	g_buf[0].ActLen = 0;
	g_buf[0].Len = 0x8000;
	g_buf[0].pData =Framebuf;
	V558_CaptureStillThumb(g_buf, JpegNote);
	g_558_WorkStatus=STATE558_Capture;
}
/********************************************************************************

	Description:
		muti-shot

	Parameters:
		FrameBUF: the buffer for still frame saving, users should designate the buffer size according to the ShotNumber;
		ThumbBUF: the buffer for thumbnail saving, users should designate the buffer size according the ShotNumber(4k for one);		
		ShotNumber: muti-shot number;		
	Note:
	       Before muti-shot, the V558 should be in preview mode

	Remarks:
		Please refer to USER558_SetCaptureParameter() for capture size

*********************************************************************************/
void USER558_MultiShot_Start(UINT8 *FrameBUF, UINT8 *ThumbBUF, UINT8 ShotNumber,USER558_CallBack pUCallBack)
{
	UINT32 i,timeout=0;
	g_558_WorkStatus=STATE558_Capture;
	g_MutiShot_Flag=1;
	g_MutiShot_TotCount=ShotNumber;
	g_MutiShot_NowCount=0;
	g_MutiShot_pFRAME=FrameBUF;
	g_MutiShot_pTHUMB=ThumbBUF;
	gUSER_CallBack=pUCallBack;
	for(i=0;i<g_MutiShot_TotCount;i++)
	{
		g_MutiShot_FrameLength[i]=0;
		g_MutiShot_ThumbLength[i]=0;
	}
	USER558_CaptureThumbStill(g_MutiShot_pFRAME);
      while(g_MutiShot_Flag)
      	{
      		USER558_CtrlIntHandle();   
      		timeout++;
		if(g_MutiShot_OneFrame)
		{		
				timeout=0;
				V558_CtrlSetWorkMode(V558_MODE_VIEW);	
				(gUSER_CallBack)(DISPLAY_THUMB,0,0);				
				USER558_CaptureThumbStill(g_MutiShot_pFRAME);
		}
		if(timeout>2000000)
		{
			g_MutiShot_OneFrame=0;
			g_MutiShot_Flag=0;
			break;
		}
      	}  
      if(timeout<2000000)
      (gUSER_CallBack)(DISPLAY_THUMB,0,0);	
      g_558_WorkStatus=STATE558_Idle;      
}
/********************************************************************************

	Description:
		stop muti-shot
	Parameters:
		None		
	Note:
	       This function will stop the mutishot arbitrary.

	Remarks:
		state: valid
*********************************************************************************/
void USER558_MultiShot_Stop(void)
{
	if(g_MutiShot_Flag) g_MutiShot_Flag=0;
	g_558_WorkStatus=STATE558_Idle;
}
/********************************************************************************

	Description:
		Change the width of  thum image 
	Parameters:
		width: the width of thum image	
	Note:
	       This function will stop the mutishot arbitrary.

	Remarks:
		state: valid
*********************************************************************************/
void USER558_MultiShot_SetWidth(UINT16 width)
{
	V558_SetThumbWidth(width);
}
/********************************************************************************

	Description:
		Get one frame frome muti-shot buffer
	Parameters:
		BUF: the buffer saving the still frame, please refer USER558_MultiShot_Start();
		Number: the frame number which to get (1 is the first one)
		Length:  the frame length(byte count)  
		return: the point of this frame

	Remarks:
		state: valid
*********************************************************************************/
UINT8* USER558_GetMultiShotFrame(UINT8 *BUF,UINT8 Number, UINT32* Length)
{
       UINT8 i;
	if(Number>g_MutiShot_NowCount||Number==0)
	{
		*Length=0;
		return 0;
	}
	*Length=g_MutiShot_FrameLength[Number-1];
	for(i=0;i<(Number-1);i++)
	{
		BUF+=g_MutiShot_FrameLength[i];
	}
	return BUF;
	
}	
/********************************************************************************

	Description:
		Get one thumbnail  frome muti-shot thumb buffer
	Parameters:
		BUF: the buffer saving the thumbnail, please refer USER558_MultiShot_Start();
		Number: the thumbnail number which to get (1 is the first one)
		Length:  thumbnail length(byte count)  
		return: the point of this thumbnail

	state: valid
*********************************************************************************/
UINT8* USER558_GetMultiShotThumb(UINT8 *BUF,UINT8 Number, UINT32* Length)
{
       UINT8 i;
	if(Number>g_MutiShot_NowCount||Number==0)
	{
		*Length=0;
		return 0;
	}
	*Length=g_MutiShot_ThumbLength[Number-1];
	for(i=0;i<(Number-1);i++)
	{
		BUF+=g_MutiShot_ThumbLength[i];
	}
	return BUF;
}


/********************************************************************************

	Description:
		Start display video on LCD

	Parameters:
		FrameRate: the video framerate, F/S.
		pUCallBack: User's call back function for reading video file to buffer
		
*********************************************************************************/
void USER558_DisplayVideo(UINT8 FrameRate, USER558_CallBack pUCallBack)
{

        UINT32 Length;

	V558_JPEGBUF decBuf;
	V558_JPEG_INDEX info;
	V558_LAYER_PROPERTY proty;	

        gUSER_CallBack=pUCallBack;       
	proty.Size.cx = g_Preview_Para.Display_W;
	proty.Size.cy = g_Preview_Para.Display_H;
	proty.DisRect.left = 0;
	proty.DisRect.top = 0;
	proty.DisRect.width = g_Preview_Para.Display_W;
	proty.DisRect.height =g_Preview_Para.Display_H;
	proty.DisPoint.x = g_Preview_Para.Display_OffsetW;	
	proty.DisPoint.y = g_Preview_Para.Display_OffsetH;

	V558_IppSetDisplay(g_Preview_Para.Display_W);	
	if( g_558_WorkStatus==STATE558_DisplayPause)
		{
		decBuf.pData = g_pBUF;
		}
	else 
		decBuf.pData = (UINT8 *)(gUSER_CallBack)(FIFO_INIT,0,&Length); 
	
	decBuf.Len = 0x80000;
	decBuf.ActLen = Length;	
	
	 g_558_WorkStatus=STATE558_Display;
	 
	if(V558_JpegParse(&decBuf, &info) == 0)
		V558_DisplayStill(&decBuf, &info, &proty);

	V558_Delay(1000);
	decBuf.ActLen -= info.endpos;
	decBuf.pData += info.endpos;

	while(V558_JpegParse(&decBuf, &info) == 0&&g_558_WorkStatus==STATE558_Display)
	{
		V558_DisplayStill(&decBuf, &info, &proty);
		V558_Delay(300);
		decBuf.ActLen -= info.endpos;
		decBuf.pData += info.endpos;
	}
	
	if(g_558_WorkStatus==STATE558_DisplayPause)
	{
		g_pBUF=decBuf.pData;
	}
	else 
		g_558_WorkStatus=STATE558_Idle;
}
/********************************************************************************

	Description:
		Stop video display

*********************************************************************************/
void USER558_DisplayVideoStop(void)
{
	g_558_WorkStatus=STATE558_Idle;
}
/********************************************************************************

	Description:
		Pause video display

*********************************************************************************/
void USER558_DisplayVideoPause(void)
{
	g_558_WorkStatus=STATE558_DisplayPause;
}
/********************************************************************************

	Description:
		The image in LCD will  Rotate  with the function.

	Parameters:
		mode: Rotation mode

	Note:
	       This function must be used in preview mode
		<TABLE>
		value					meaning
		-----					-------
		0						normal
		1						90 
		2						180
		3						270
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void USER558_Rotation(UINT8 mode) 
{
	V558_SIZE SrcSize,Size;
	V558_RECT rect;
	V558_POINT DisPoint;
	V558_POINT pt;
	UINT8 Ratio;
	UINT16 lc;
	
	V558_LcdALDisable();
	V558_LcdDispDisable();
     	pt.x=g_Preview_Para.Source_OffsetW;
     	pt.y=g_Preview_Para.Source_OffsetH;
    	SrcSize.cx=g_Preview_Para.Source_W;
    	SrcSize.cy=g_Preview_Para.Source_H;

   	rect.height=g_Preview_Para.Display_H;
   	rect.width=g_Preview_Para.Display_W;
   	rect.left=0;
   	rect.top=0;

   	Size.cx=g_Preview_Para.Display_W;
   	Size.cy=g_Preview_Para.Display_H;

   	DisPoint.x=g_Preview_Para.Display_OffsetW;
   	DisPoint.y=g_Preview_Para.Display_OffsetH;
   	V558_LcdSetALDisPoint(DisPoint);

   	Ratio=g_Preview_Para.Source_W/g_Preview_Para.Display_W;
   	switch(mode)
   	{
   	case 0:   		
 		SrcSize.cy=g_Preview_Para.Source_H;
   		SrcSize.cx=g_Preview_Para.Source_W;
   		
   		 Size.cx=g_Preview_Para.Display_W;
  		 Size.cy=g_Preview_Para.Display_H;
  		 
  		 rect.height=g_Preview_Para.Display_H;
               rect.width=g_Preview_Para.Display_W;

   		V558_IppSetSizerDisplay(pt,SrcSize,rect.width);
  		V558_LcdSetALSize(Size);
   		V558_LcdSetALDisRect(rect);
   		V558_LcdSetDispMode(4);

   		lc=V558_LcdGetIFControl();
   		V558_LcdSetIFControl(lc|0x08);

   		  V558_LcdBLEnable();
   		break;
   	case 1:
   		 V558_LcdBLDisable();
   		if((SrcSize.cx>SrcSize.cy)&&(SrcSize.cx>480))
   			{
   				SrcSize.cy=g_Preview_Para.Source_H;
  	 			SrcSize.cx=g_Preview_Para.Source_H;
  	 			
  	 			Size.cx=g_Preview_Para.Display_W;
 	   			Size.cy=g_Preview_Para.Display_W;

 			}
   		else
   			{
   				SrcSize.cy=g_Preview_Para.Source_W;
  	 			SrcSize.cx=g_Preview_Para.Source_H;
  	 			Size.cx=g_Preview_Para.Display_H;
 	   			Size.cy=g_Preview_Para.Display_W;
   			}

 	  	rect.height=g_Preview_Para.Display_H;
         	rect.width=g_Preview_Para.Display_W;

		
   		lc=V558_LcdGetIFControl();
   		V558_LcdSetIFControl(lc&0xfff7);  
   		
   		V558_LcdSetALSize(Size);
   		V558_IppSetSizerDisplay(pt,SrcSize,Size.cx);
   		V558_LcdSetALDisRect(rect);
  		V558_LcdSetDispMode(0);
   		break;
   	case 2:
   		 V558_LcdBLDisable();
   		SrcSize.cy=g_Preview_Para.Source_H;
   		SrcSize.cx=g_Preview_Para.Source_W;
   		Size.cx=g_Preview_Para.Display_W;
  		 Size.cy=g_Preview_Para.Display_H;
  		 rect.height=g_Preview_Para.Display_H;
              rect.width=g_Preview_Para.Display_W;

   		V558_IppSetSizerDisplay(pt,SrcSize,rect.width);
   		V558_LcdSetALSize(Size);
   		V558_LcdSetALDisRect(rect);
   		V558_LcdSetDispMode(2);
   		
   		lc=V558_LcdGetIFControl();
   		V558_LcdSetIFControl(lc|0x08);
   		break;
   	case 3:
   		 V558_LcdBLDisable();
     			if((SrcSize.cx>SrcSize.cy)&&(SrcSize.cx>480))
   			{
   				SrcSize.cy=g_Preview_Para.Source_H;
  	 			SrcSize.cx=g_Preview_Para.Source_H;
  	 			Size.cx=g_Preview_Para.Display_W;
 	   			Size.cy=g_Preview_Para.Display_W;;

   			}
   		else
   			{
   				SrcSize.cy=g_Preview_Para.Source_W;
  	 			SrcSize.cy=g_Preview_Para.Source_H;
  	 			Size.cx=g_Preview_Para.Display_H;
 	   			Size.cy=g_Preview_Para.Display_W;
   			}
 	       rect.height=g_Preview_Para.Display_H;
              rect.width=g_Preview_Para.Display_W;
              
		 lc= V558_LcdGetIFControl();
   		V558_LcdSetIFControl(lc&0xfff7);  
   		
  		V558_LcdSetALSize(Size);
  		V558_IppSetSizerDisplay(pt,SrcSize,Size.cx);
   		V558_LcdSetALDisRect(rect);
   		
  		V558_LcdSetDispMode(1);
   		break;
   	default:
   		break;
   	}
		
		V558_LcdDispEnable();
   		V558_LcdALEnable();	
}

/********************************************************************************

	Description:
		Set special effect

	Parameters:
		Mode: Set Special Effect mode

	Note:
	       This function must be used in viewfiner situation
		<TABLE>
		value					meaning
		-----					-------
		0                                       special effect disable;
		1                                       Monochrome;
		2                                       Sephia;
		3  					     ColorRange;
		4					     Negative;
		5 					      sketch;
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void USER558_Set_Special_Effect(UINT8 Mode)
{
	switch(Mode)
	{
	case 1:
		V558_IppSetUoffset(108);
		V558_IppSetVoffset(158);
		V558_IppSetSpeCtrl(0x01);
		break;
	case 2:
		V558_IppSetUoffset(0x80);
		V558_IppSetVoffset(0x40);
		V558_IppSetSpeCtrl(0x01);
		break;
	case 3:
		V558_IppSetUTopThrd(0x40);
		V558_IppSetUBottomThrd(0xc0);
		V558_IppSetVTopThrd(0x00);
		V558_IppSetVBottomThrd(0x20);
		V558_IppSetSpeCtrl(0x03);
		break;
	case 4:
		V558_IppSetSpeCtrl(0x05);
		break;
	case 0:
		V558_IppDisableSpecial();
		break;
	default:
		break;
	}
}

/********************************************************************************

	Description:
		Set the value of color range  (special effect)

	Parameters:
		UStart: U start value
		UStop: U stop value
		VStart: V Start  value
		VStop: V Stop value

	Remarks:
		state: valid

*********************************************************************************/

void USER558_Set_ColorRange(UINT8	UStart,UINT8 UStop,UINT8 VStart,UINT8 VStop)
{
		V558_IppSetUTopThrd(UStart);
		V558_IppSetUBottomThrd(UStop);
		V558_IppSetVTopThrd(VStart);
		V558_IppSetVBottomThrd(VStop);
		V558_IppSetSpeCtrl(0x03);
}
/********************************************************************************

	Description:
		The image in LCD will be mirrored  by using the function.

	Parameters:
		mode: Mirror mode

	Note:
	       This function must be used in preview mode
		<TABLE>
		value					meaning
		-----					-------
		0					      normal;
		1					     horizontal mirror;
		2					     vertical mirror.
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void USER558_Mirror(UINT8 mode)
{
	V558_LcdDispDisable();
	switch(mode)
		{
		case 0:
			 V558_LcdBLEnable();
	          	V558_LcdSetDispMode(4);
	     		 break;	      
	       case 1:
	            V558_LcdBLDisable();
	      	     V558_LcdSetDispMode(3);
	     		break;	      
	      case 2:
	      	       V558_LcdBLDisable();
			 V558_LcdSetDispMode(5);
	       	break;	      
	      default:
   			break;
		}
	V558_LcdDispEnable();
}



/********************************************************************************

 Description:
  	Get the valid zoom steps( USER558_PreviewZoom() )

 Parameters:
  	mode: 0:zoom in .1:zoom out

Return:
	The total valid zoom steps(please refer the 'step' parameter in USER558_PreviewZoom() )

 Note:

 Remarks:
  	state: valid

*********************************************************************************/
UINT16  USER558_GetZoomSteps(UINT8 mode)
{
 	UINT16 DeWith,devisor,temp;
 	UINT16 XRatio, YRatio;
 	V558_SIZE Size,DisSize,CapSize;

  	V558_IppGetSizerSize(&Size);
  	V558_IppGetDispSize(&DisSize);
	V558_IppGetCapSize(&CapSize);

  	devisor=Find_common_divisor(g_Preview_Para.Source_W,g_Preview_Para.Source_H);

  	XRatio=g_Preview_Para.Source_W/devisor;
  	YRatio=g_Preview_Para.Source_H/devisor;  
	if(XRatio==0) return 0;
		
 	if(!mode)
 	{	
 		if(Size.cx<=CapSize.cx) return 0;
 		DeWith=Size.cx-CapSize.cx;
 		
 	}
 	else
 	{
 		if(g_Preview_Para.Source_W<=Size.cx) return 0;
 		DeWith=g_Preview_Para.Source_W-Size.cx;		
 	}
 	temp=(DeWith/XRatio)>>2;
  		
	return temp;
}
/********************************************************************************

 Description:
  	set 558 Previw Zoom

 Parameters:
  	mode: 0:zoom in .1:zoom out
  	step: zoom step(1 step=4pixel);
  	x: offset x in zoom image;if x=0xff and y=0xff than auto find the middle
  	y: offset y in zoom image;

  return: 
  	With current zoom parameter, how many times this function can be called valid
  	(otherwise it will have no effect)

 Note:
  	This function must be used in Preview mode, and will  effect capture
 	step:1-5

 Remarks:
  	state: valid

*********************************************************************************/
UINT16 USER558_PreviewZoom(UINT8 mode,UINT16 step,UINT16 x,UINT16 y)
{
 	UINT16 DeWith,Dehigh,devisor;
 	UINT16 XRatio, YRatio, ZoomTime;
 	V558_SIZE Size,DisSize,CapSize;
 	V558_POINT StartPoint;
//ZoomTime=USER558_GetZoomSteps(mode);
  	V558_IppGetSizerSize(&Size);
  	V558_IppGetDispSize(&DisSize);
	V558_IppGetCapSize(&CapSize);

  	devisor=Find_common_divisor(g_Preview_Para.Source_W,g_Preview_Para.Source_H);


  	XRatio=g_Preview_Para.Source_W/devisor;
  	YRatio=g_Preview_Para.Source_H/devisor;  
  	DeWith=XRatio*4*step;
  	Dehigh=YRatio*4*step;
  	if(DeWith==0)  return 0;
 	StartPoint.x=x;
 	StartPoint.y=y;	
 	if(!mode)
 	{	
 		if(Size.cx>DeWith && Size.cy>Dehigh)
 		{
  			Size.cx-=DeWith;
  			Size.cy-=Dehigh;
 		}
 		else return 0;
 		if(Size.cx<CapSize.cx||Size.cx<DisSize.cx) return 0;
 		if((x==0xff)&&(y==0xff))
  		{
  			V558_IppGetSizerStartp(&StartPoint);
  			StartPoint.x+=(DeWith/2);
  			StartPoint.y+=(Dehigh/2);
  		}
 	}
 	else
 	{
  		Size.cx+=DeWith;
  		Size.cy+=Dehigh;
  		if(Size.cx>g_Preview_Para.Source_W) return 0;
 		if((x==0xff)&&(y==0xff))
  		{
  			V558_IppGetSizerStartp(&StartPoint);
  			StartPoint.x-=(DeWith/2);
  			StartPoint.y-=(Dehigh/2);
  		}  		
 	}
 	
	V558_LcdDispDisable();
	V558_IppSetZoomDisplay(StartPoint,Size,DisSize.cx);  	
	//V558_IppSetCapture(g_Preview_Para.Capture_W);
	//V558_LcdSetALSize(DisSize);	
	V558_LcdDispEnable();
	
  	ZoomTime=0;
	if(!mode)
	{
		 while(Size.cx>DeWith && Size.cy>Dehigh)
 		{
  			Size.cx-=DeWith;
  			Size.cy-=Dehigh;
 			if(Size.cx<CapSize.cx||Size.cx<DisSize.cx) break;	
 			ZoomTime++;
 		}

	}
	else
	{
		while(1)
		{
  			Size.cx+=DeWith;
  			Size.cy+=Dehigh;
  			if(Size.cx>g_Preview_Para.Source_W) break;
  			ZoomTime++;
		}
	}
  		
	return ZoomTime;
}

/********************************************************************************

 Description:
  	Get the valid zoom steps( USER558_PreviewZoomDisplay() )

 Parameters:
  	mode: 0:zoom in .1:zoom out

Return:
	The total valid zoom steps(please refer the 'step' parameter in USER558_PreviewZoomDisplay() )

 Note:

 Remarks:
  	state: valid

*********************************************************************************/
UINT16  USER558_GetZoomDisplaySteps(UINT8 mode)
{
 	UINT16 DeWith,Dehigh,devisor;
 	UINT16 XRatio, YRatio, ZoomTime;
 	V558_SIZE DisSize;
 	V558_IppGetDispSize(&DisSize);
  	devisor=Find_common_divisor(g_Preview_Para.Source_W,g_Preview_Para.Source_H);

  	XRatio=g_Preview_Para.Source_W/devisor;
  	YRatio=g_Preview_Para.Source_H/devisor;  
  	DeWith=XRatio*4;
  	Dehigh=YRatio*4;
  	if(DeWith==0)  return 0;
  		
  	ZoomTime=0;
	if(mode)
	{

 		while(DisSize.cx>=(DeWith+g_Preview_Para.Display_W) && DisSize.cy>=(Dehigh+g_Preview_Para.Display_H))
 		{
  			DisSize.cx-=DeWith;
  			DisSize.cy-=Dehigh;
  			ZoomTime++;
 		}	
	}
	else
	{ 		
		while(1)
		{
  			DisSize.cx+=DeWith;
  			DisSize.cy+=Dehigh;
  			if(DisSize.cx>g_Preview_Para.Source_W||DisSize.cy>g_Preview_Para.Source_H) break;
  	   		if(DisSize.cx>=0x200||DisSize.cy>=0x200) break;		
  			ZoomTime++;
		}
	}  		
	return ZoomTime;
}

/********************************************************************************

 Description:
  	set 558 Previw Zoom
  	This only effect LCD display, it has no effection with capture. 

 Parameters:
  	mode: 0:zoom in .1:zoom out
  	step: zoom step(1 step=4pixel);

  return: 
  	With current zoom parameter, how many times this function can be called valid
  	(otherwise it will have no effect)

 Note:
  	This function must be used in Preview mode, and will NOT effect capture
 	step:1-5

 Remarks:
  	state: valid

*********************************************************************************/
UINT16 USER558_PreviewZoomDisplay(UINT8 mode,UINT16 step)
{
 	UINT16 DeWith,Dehigh,devisor;
 	UINT16 XRatio, YRatio, ZoomTime;
 	V558_SIZE Size,DisSize,CapSize;
 	V558_POINT StartPoint;
 	V558_LAYER_PROPERTY proty;
	//ZoomTime=USER558_GetZoomDisplaySteps(mode);
  	V558_IppGetSizerSize(&Size);
  	V558_IppGetDispSize(&DisSize);
	V558_IppGetCapSize(&CapSize);
	V558_IppGetSizerStartp(&StartPoint);
	
  	devisor=Find_common_divisor(g_Preview_Para.Source_W,g_Preview_Para.Source_H);

  	XRatio=g_Preview_Para.Source_W/devisor;
  	YRatio=g_Preview_Para.Source_H/devisor;  
  	DeWith=XRatio*4*step;
  	Dehigh=YRatio*4*step;
  	if(DeWith==0)  return 0;
  		
 	if(mode)
 	{	
 		if(DisSize.cx>=(DeWith+g_Preview_Para.Display_W) && DisSize.cy>=(Dehigh+g_Preview_Para.Display_H))
 		{
  			DisSize.cx-=DeWith;
  			DisSize.cy-=Dehigh;
 		}
 		else return 0;
 	}
 	else
 	{
  		DisSize.cx+=DeWith;
  		DisSize.cy+=Dehigh;
  		if(DisSize.cx>g_Preview_Para.Source_W) return 0;
   		if(DisSize.cy>g_Preview_Para.Source_H) return 0; 
   		if(DisSize.cx>=0x200) return 0;
   		if(DisSize.cy>=0x200) return 0;   		
 	}
	proty.Size.cx = DisSize.cx;
	proty.Size.cy = DisSize.cy;
	proty.DisRect.left = (DisSize.cx-g_Preview_Para.Display_W)/2;
	proty.DisRect.top = (DisSize.cy-g_Preview_Para.Display_H)/2;		
	proty.DisRect.width = g_Preview_Para.Display_W;
	proty.DisRect.height =g_Preview_Para.Display_H;
	proty.DisPoint.x = g_Preview_Para.Display_OffsetW;
	proty.DisPoint.y = g_Preview_Para.Display_OffsetH; 	
	V558_LcdDispDisable();
	V558_IppSetZoomDisplay(StartPoint,Size,DisSize.cx);  	
	//V558_IppSetCapture(g_Preview_Para.Capture_W);
	//V558_LcdSetALSize(DisSize);	
	V558_LcdSetALProperty(&proty);
	V558_LcdDispEnable();
	
  	ZoomTime=0;
	if(mode)
	{

 		while(DisSize.cx>=(DeWith+g_Preview_Para.Display_W) && DisSize.cy>=(Dehigh+g_Preview_Para.Display_H))
 		{
  			DisSize.cx-=DeWith;
  			DisSize.cy-=Dehigh;
  			ZoomTime++;
 		}	
	}
	else
	{ 		
		while(1)
		{
  			DisSize.cx+=DeWith;
  			DisSize.cy+=Dehigh;
  			if(DisSize.cx>g_Preview_Para.Source_W||DisSize.cy>g_Preview_Para.Source_H) break;
  	   		if(DisSize.cx>=0x200||DisSize.cy>=0x200) break;		
  			ZoomTime++;
		}
	}  		
	return ZoomTime;
}

/********************************************************************************

	Description:
		Set Bypass mode

	Parameters:
		Mode: Set Bypass mode
		PANEL: Set Panel Cs

	Note:
	       This function must be used in 16bit multplex mode
		<TABLE>
		value					meaning
		-----					-------
		0                                       normal mode;
		1                                       start bypass mode.
		</TABLE>
		
		<TABLE>
		value					meaning
		-----					-------
		0                                       Main Panel;
		1                                       Sub Panel.
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void USER558_Set_Bypass(UINT8 Mode,UINT8 PANEL)
{
		#if V558_BUS_TYPE == V558_BUS_SEPERATE
			return;
		#endif
		if(Mode == 1)
		{
			V558_SetReg(V558_REG_BIU_BYPASS_SEL ,0x0); 
			V558_CtrlSetChipClkOn();
			V558_CtrlSetModClkOn(V558_MOD_CLK_LCD);				
 			V558_ChangePanelCs(PANEL); 
			V558_CtrlSetChipClkOff(); 			
			V558_CtrlSetWorkMode(V558_MODE_BYPASS);

 		}
		else
		{
			V558_CtrlSetWorkMode(V558_MODE_VIEW);
 			V558_CtrlSetChipClkOn();
 			V558_CtrlSetModClkOn(V558_MOD_CLK_LCD);
 			V558_ChangePanelCs(PANEL);
		}

}

/********************************************************************************

	Description:
		Set the value of lcd driver ic register 

	Parameters:
		uVal: the value of the register which will  write to driver ic.
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_SetVal_By(UINT16 uVal)
{
	 V558_SetVal_By(uVal);
}
/********************************************************************************

	Description:
		Set the command of lcd driver ic register 

	Parameters:
		uVal: the command of the register which will  write to driver ic.
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_SetCmd_By(UINT16 uVal)
{
	V558_SetCmd_By(uVal);
}
/********************************************************************************

	Description:
		Write the Gram coutinually

	Parameters:
		uCmd: The command of write Gram 
		* pData: the head point of data.
		uSize: the length of data
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_SetData_By(UINT16 uCmd, UINT8* pData, UINT32 uSize)
{
	V558_SetData_By(uCmd, pData,  uSize);
}
/********************************************************************************

	Description:
		Set gpio mode

	Parameters:
		OorI: bit0-bit4 represent GPIO0 to GPIO4's direction'1'=Output mod,'0'=input
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_SetGpioMode(UINT8 OorI)
{
	V558_CtrlSetGpioCfg(0xff);
	V558_CtrlSetGpioMode(0xff);
	V558_CtrlSetGpioDirection(OorI);
}
/********************************************************************************

	Description:
		Set gpio Data

	Parameters:
		data:  bit0-bit4 represent GPIO0 to GPIO4's data
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_SetGpioData(UINT8 data)
{
	V558_CtrlSetGpioData(data);
}
/********************************************************************************

	Description:
		Get gpio Data

	Parameters:
		data:  bit0-bit4 represent GPIO0 to GPIO4's data
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_GetGpioData(UINT8 *GpioData)
{
	*GpioData=V558_CtrlGetGpioData();
	
}
/********************************************************************************

	Description:
		adjust the Brightness 

	Parameters:
		mode: add or dec the brightness 
		step: the step value 

	Note: 
		This function is must be used in view finder mode.
		This function is effect in Bayer format sensor.
	       <TABLE>
		value					meaning
		-----					-------
		0                                       add;
		1                                       dec.
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_BrightnessAdjust(UINT8 Mode,UINT8 Step)
{
	UINT8 NowBrightness;
	
	V558_IspGetBrightness(&NowBrightness);
	if(Mode)
		NowBrightness+=Step;
	else
		NowBrightness-=Step;
	V558_IspSetBrightness(NowBrightness);
}
/********************************************************************************

	Description:
		adjust the contrast

	Parameters:
		mode: add or dec the contrast 
		step: the step value 

	Note: 
		This function  must be used in view finder mode.
		This function is effect in Bayer format sensor.
	       <TABLE>
		value					meaning
		-----					-------
		0                                       add;
		1                                       dec.
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_ContrastAdjust(UINT8 Mode,UINT8 Step)
{
	UINT8 NowContrast;
	
	V558_IspGetContrast(&NowContrast);
	if(Mode)
		NowContrast+=Step;
	else
		NowContrast-=Step;
	V558_IspSetContrast(NowContrast);
}

/********************************************************************************

	Description:
		adjust the sharpness for edge enhancement 

	Parameters:
		Value: sharpness value(1-15 is valid value)

	Note: 
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_SharpnessAdjust(UINT8 Value)
{
	V558_EDGEPARM EdgeParameter;
	if(Value==0||Value>15) return;
	V558_IspGetEdgeParm(&EdgeParameter);
	EdgeParameter.femm=Value*16;
	EdgeParameter.femp=Value*16*EdgeParameter.femx2/(EdgeParameter.femx2-EdgeParameter.femx1);	
	V558_IspSetEdgeParm(&EdgeParameter);	
}

/********************************************************************************

	Description:
		Set light condition 

	Parameters:
		Mode: 
			0:daylight(D65) 
			1:incadescent
			2:fluorescence(TL84) 
	Note: 
		
	Remarks:
		state: valid

*********************************************************************************/
void USER558_SetLightCondition(UINT8 Mode)
{
	V558_COLORMATRIX ColorMatrix0={0x66,0xdb,0x06,0xf1,0x5f,0xf4,0xf7,0xeb,0x5b};
	V558_COLORMATRIX ColorMatrix2={0x41,0x06,0xf0,0xef,0x47,0x02,0xf3,0xd9,0x6b};	
	switch(Mode)
	{
		case 0:
			V558_IspSetColorMatrix(&ColorMatrix0);	
			break;
		case 1:
			break;
		case 2:
			V558_IspSetColorMatrix(&ColorMatrix2);	
			break;
	}
	
}
/********************************************************************************

	Description:
		set lcd panel to desired work state

	Parameters:
		status:	desired state
		panel:   current panel

	Note:
		<TABLE>
		value					meaning
		-----					-------
		1						power off the panel
		2						sleep the panel
		3						stand by the panel
		4						power on the panel
		5						Wake up the panel
		default					power on the panel
		</TABLE>
		
		<TABLE>
		value					meaning
		-----					-------
		0						Main panel
		1						Sub panel
		default 					main panel
		</TABLE>

	Remarks:
		state: valid

*********************************************************************************/
void USER558_SetPanelStatus(UINT8 Panel,UINT8 Status)
{
	UINT16 LC;
	UINT8 NowPanel,SourcStatus;
	NowPanel=USER558_GetCurrentLCD();
	if(NowPanel!=Panel)
		{
			LC=V558_LcdGetIFControl();
			if(Panel==1)
				V558_LcdSwitchPanel(1);
			else
				V558_LcdSwitchPanel(0);
			SourcStatus=V558_LcdGetPanelState();
			if(SourcStatus!=Status)
				V558_LcdSetPanelState(Status);
		}
	
}


/********************************************************************************

	Description:
		Get the curent LCD TYPE (main or sub)

	Parameters:
		return: lcd type

	Note:
		 <TABLE>
		value					meaning
		-----					-------
		0                                       main
		1					      sub

	Remarks:
		state: valid

*********************************************************************************/
UINT8 USER558_GetCurrentLCD(void)
{
	UINT16 LC;
	LC=V558_LcdGetIFControl();
	if(!(LC&0x10))
		return 0;		//main
	else 
		return 1;
}
////////////////////////////////////////////////////////////////////////////
//----The list code is Graphic driver---------------------------------------------
////////////////////////////////////////////////////////////////////////////
extern UINT8	GetColorIndex(UINT8 colorDepth);
extern UINT8	GetColordepth(UINT8 colorIndex);
extern UINT16	GetSpanFromDepth(UINT16 width, UINT8 colordepth);
/********************************************************************************

	Description:
		Update lcd panel 

*********************************************************************************/
void USER558_LCDUpdate(void)
{
	UINT8 DisType;
	DisType=V558_LcdGetDispEn();
      if(g_558_WorkStatus==STATE558_Preview&&(DisType&0x02))
      	return;
	V558_LcdUpdate(2);
}
/********************************************************************************

	Description:
		Update lcd panel 

*********************************************************************************/
void USER558_LCDUpdateRegion(UINT16 startx, UINT16 starty,UINT16 endx,UINT16 endy)
{

	V558_RECT Rect;
	V558_POINT DisPoint;
	V558_LAYER_PROPERTY proty;
	

       V558_LcdGetBLProperty(&proty);
       Rect.height=endy-starty;
      	Rect.width=endx-startx;
      	Rect.left=startx;
      	Rect.top=starty;
      	DisPoint.x=startx;
      	DisPoint.y=starty;
       V558_LcdSetBLDisRect(Rect);
      	V558_LcdSetBLDisPoint(DisPoint);
	V558_LcdUpdate(2);

	V558_LcdSetBLProperty(&proty);
}
/********************************************************************************

	Description:
		Initial LCD(B Layer)

	Parameters:
		OffX: offset x value on lcd.
		OffY: offset y value on lcd.
		Width: B Layer display width(pixel)
		Height:  B Layer display hight(pixel)
		BPP: colordep;

	Remarks:
		state: valid

*********************************************************************************/
void USER558_InitLCD( UINT16 OffX,UINT16 OffY,UINT16 Width, UINT16 Height ,UINT8 BPP)
{
	       V558_LAYER_PROPERTY proty;
	       UINT8			colordep;
	       
		proty.DisPoint.x = OffX;
		proty.DisPoint.y =OffY;
		proty.DisRect.top = 0;
		proty.DisRect.left = 0;
		proty.DisRect.width = Width;
		proty.DisRect.height = Height;
		proty.Size.cx = Width;
		proty.Size.cy = Height;

		colordep = GetColorIndex(BPP);
		proty.Format =  colordep;
		V558_LcdSetBLProperty(&proty);
}
/********************************************************************************

	Description:
		Set LCD display on

	Parameters:
		OnSelect:      
		       1--A Layer on; 2--B Layer on;3--ALL on
		DisplayMode:
		       1--A Layer priority; 2--B Layer priority; else--B Transparent
		KeyColor:
			the transparent color in B Transparent mode

	Remarks:
		state: valid

*********************************************************************************/
void USER558_LCDDisplayOn( UINT8 OnSelect, UINT8 DisplayMode, UINT16 KeyColor )
{
	V558_COLOR keycolor;
	
	UINT16 i;

	V558_LcdDispDisable();
       switch(DisplayMode)
       {
		case 1: //A Layer priority
			V558_LcdSetALPriority(0);
			V558_LcdDisableBLOverlay();
			break;
		case 2://B Layer priority
			V558_LcdSetALPriority(1);
			V558_LcdALDisable();
			V558_LcdDisableBLOverlay();
		
			break;
		default://B Transparent
			if((V558_LcdGetBLFormat()&0x0f)==0x03)  //555
				{
					keycolor.r = (UINT8)((KeyColor>>10)&0x1f);
					keycolor.g = (UINT8)((KeyColor>>5)&0x1f);
					keycolor.b = (UINT8)((KeyColor)&0x1f);
				}
			else
				{
					keycolor.r = (UINT8)((KeyColor>>11)&0x1f);
					keycolor.g = (UINT8)((KeyColor>>5)&0x3f);
					keycolor.b = (UINT8)((KeyColor)&0x1f);
				}
				
               	V558_LcdSetALPriority(1);
               	V558_LcdSetBLKeyColor(keycolor);
      			V558_LcdEnableBLOverlay();
			for(i=0;i<500;i++);
			break;
		}
       if(OnSelect&0x02)
       	{
       	V558_Delay(2000);			
       	V558_LcdDispEnable();
       	}
	if(OnSelect&0x01)
		V558_LcdALEnable();

}
/********************************************************************************

	Description:
		Set LCD display off

	Parameters:
		OnSelect:      
		       1--A Layer off; 2--B Layer off;3--ALL off

	Remarks:
		state: valid

*********************************************************************************/
void USER558_LCDDisplayOff(UINT8 OffSelect)
{
	if(OffSelect&0x01)
		V558_LcdALDisable();
       if(OffSelect&0x02)
       	V558_LcdBLDisable();	
}
/********************************************************************************

	Description:
		Start draw dot(dots) on B Layer

	Parameters:
		return: the lcd interface mode A layer on or b layer on)bit1=a layer.bit2=b layer bit0=master switch)

	Remarks:
		This function should be called before draw dot or dots

*********************************************************************************/
UINT8 USER558_LCDStartDrawDot(void)
{
	UINT8 DisType;
	DisType=V558_LcdGetDispEn();
	V558_LcdALDisable();
	V558_LcdDispDisable();
	return DisType;
}
/********************************************************************************

	Description:
		Stopdraw dot(dots) and update B Layer

	Parameters:
		DisType: the lcd interface mode (A layer on or b layer on)bit1=a layer.bit2=b layer bit0=master switch)	

	Remarks:
		This function should be called after draw dot or dots, only after this the drawn dots will updtate on LCD

*********************************************************************************/
void USER558_LCDStopDrawDot(UINT8 DisType) 
{
	V558_LcdSetDispEn(DisType);
	
}
/********************************************************************************

	Description:
		Draw a dot on LCD( B Layer)

	Parameters:
		x:      horizontal coordinate of the dot(pixel)
		y:	  vertical    coordinate of the dot(pixel)
		color: the color of the dot

	Remarks:
		The dot will not update on the LCD immediately after drawing.
		Please refer to USER558_LCDStartDrawDot() and USER558_LCDStopDrawDot() when use this function

*********************************************************************************/
void USER558_LCDDrawOneDot( UINT16 x, UINT16 y, UINT16 color )
{
	UINT16	span;
	UINT32	colordep;
	UINT32 startAddr;
	UINT8 colortmp[3],Number;
	V558_SIZE size;

	V558_LcdGetBLSize(&size);		
	colordep = V558_LcdGetBLFormat();
	colordep = GetColordepth(colordep);
	span = GetSpanFromDepth(size.cx, colordep);

	Number=2;
	
	colortmp[0]=(color>>(Number-1)*8)&0xff;
	colortmp[1]=(color>>(Number-2)*8)&0xff;
	colortmp[2]=color&0xff;

	startAddr = 0x10000 + y*span+ x * Number;

	V558_WriteSram(startAddr, colortmp, Number);
}
/********************************************************************************

	Description:
		Draw a full screen pure color on LCD( B Layer)

	Parameters:
		color: the color 

	Remarks:
              valid
*********************************************************************************/
void USER558_LCDDrawPureColor(UINT16 color)
{
       UINT32 x,y;
       V558_SIZE size;
       UINT8 TEMP;
	 TEMP=USER558_LCDStartDrawDot();
	V558_LcdGetBLSize(&size);
		
	for(y=0;y<size.cy;y++)
	{
		for(x=0;x<size.cx;x++)
			USER558_LCDDrawOneDot(x, y, color);
	}	

       USER558_LCDStopDrawDot(TEMP);
       USER558_LCDUpdate();
}
/********************************************************************************

	Description:
		Draw a line on LCD( B Layer)

	Parameters:
		startx: the coordinate of the line start point(pixel)
        starty: the coordinate of the line start point(pixel)
		endx: the coordinate of the line end point(pixel)
		endy:  the coordinate of the line end   point(pixel)
		Color:               the color of the line

	Remarks:
              valid
*********************************************************************************/
void USER558_LCDDrawLine( UINT16 startx, UINT16 starty,UINT16 endx,UINT16 endy,UINT32 Color )
{

	V558_POINT pstart, pend;
	V558_COLOR LineColor;
	pstart.x=startx;
	pstart.y=starty;	
	pend.x=endx;
	pend.y=endy;
	LineColor.r=Color&0x1f;
	LineColor.g=(Color>>5)&0x1f;
	LineColor.b=(Color>>10)&0x1f;	
		
	V558_GeDrawLine(pstart, pend, LineColor);
			//grean
	USER558_LCDUpdate();	
}
/********************************************************************************

	Description:
		Draw a line on LCD( B Layer)

	Parameters:
		x: the coordinate of the line start point(pixel)
		y: the coordinate of the line start point(pixel)
		len:       the length    point(pixel)
		Color:               the color of the line

	Remarks:
              valid
*********************************************************************************/
void USER558_LCDDrawHorizontalLine(UINT32 x, UINT32 y, UINT8 len, UINT16 color)
{
	UINT16	span,i;
  	V558_SIZE		size;
	UINT32 startAddr;
	UINT8 colortmp[256];
	UINT8	colordep;
	
	V558_LcdGetBLSize(&size);
	colordep = V558_LcdGetBLFormat();
	colordep = GetColordepth(colordep);
	span = GetSpanFromDepth(size.cx, colordep);

	for(i=0;i<len;i++)
	{
		colortmp[i*2]=(color>>8)&0xff;
		colortmp[i*2+1]=color&0xff;
	}

	startAddr =  0x10000 + y*span+ x * 2;
	
	V558_WriteSram(startAddr, colortmp,len*2);
	
}
/********************************************************************************

	Description:
		Draw a line on LCD( B Layer)

	Parameters:
		x: the coordinate of the line start point(pixel)
            y: the coordinate of the line start point(pixel)
		len:       the length    point(pixel)
		Color:               the color of the line

	Remarks:
              valid
*********************************************************************************/
void USER558_LCDDrawVerticalLine(UINT32 x, UINT32 y, UINT8 len, UINT16 color)
{
	UINT16	span,i;
  	V558_SIZE		size;
	UINT32 startAddr;
	UINT8 colortmp[2];
	UINT8	colordep;
	
	V558_LcdGetBLSize(&size);
	colordep = V558_LcdGetBLFormat();
	colordep = GetColordepth(colordep);
	span = GetSpanFromDepth(size.cx, colordep);
	colortmp[0]=(color>>8)&0xff;
	colortmp[1]=color&0xff;
	startAddr =  0x10000 + y*span+ x * 2;	
	 for(i=0;i<len;i++)
       {
       	   	V558_WriteSram(startAddr+i*span, colortmp, 2);
       }	
}

/********************************************************************************

	Description:
		Draw a Rectangle on LCD( B Layer)

	Parameters:
		startx: the left coordinate of the Rectangle(pixel)
            starty:   the lup coordinate of the Rectangle(pixel)
		endx: the right coordinate of the Rectangle(pixel)
             endy: the bottom coordinate of the Rectangle(pixel)
		uspImage:         the point of memory which to fill the Rectangle

	Remarks:
              valid
*********************************************************************************/
void USER558_LCDDrawRctngl( UINT16 startx, UINT16 starty,UINT16 endx,UINT16 endy,UINT8*uspImage)
{
       UINT8	TEMP;

       TEMP=USER558_LCDStartDrawDot();
	V558_LCDDrawBL(startx,starty,endx,endy,uspImage);	
       USER558_LCDStopDrawDot(TEMP);

	USER558_LCDUpdate();	
}


/********************************************************************************

	Description:
		Copy a rectangle from some place in 40 kb to another place.

	Parameters:
		startx: the source image coordinate of the Rectangle(pixel)
            starty:  the source image coordinate of the Rectangle(pixel)
		endx:  the target image coordinate of the Rectangle(pixel)
		endy:  the target image coordinate of the Rectangle(pixel)
		Whith: the whith of Rectangle.
		heigh:  the high of Rectangle.

	Remarks:
              valid
*********************************************************************************/
void USER558_LCDGeCopy(UINT16 Startx,UINT16 Starty,UINT16 Stopx,UINT16 Stopy,UINT16 Whith,UINT16 Heigh)
{
	V558_POINT src ;
	V558_POINT ptn = {10, 0};
	V558_RECT dst ;
	//V558_LcdALDisable();
	src.x=Startx;
	src.y=Starty;
	dst.left=Stopx;
	dst.top=Stopy;
	dst.height=Heigh;
	dst.width=Whith;
	V558_GeBitblt(src, ptn, dst, 0xcc);
	USER558_LCDUpdate();	
}

/********************************************************************************

	Description:
		Change Yuv to Rgb and Draw a Rectangle on LCD( B Layer)

	Parameters:
		startx: the left coordinate of the Rectangle(pixel)
            starty: the up coordinate of the Rectangle(pixel)
		uspImage:         the point of memory which to fill the Rectangle
		Width: The Bmp Frame's width .
		Height:   The Bmp Frame's height.
		rawFormat: the format of rgb
		
	Note:
	      if width and height is set to 0 ,The width and height will be as same as the 
	   width and the height of the thumnail.   
	      <TABLE>
		value					meaning
		-----					-------
		0					      888;
		1                                       565;
		2                                       555;
		default 				      565
		</TABLE>
		
	Remarks:
              valid
*********************************************************************************/
void USER558_UpdateThum( UINT8 rawFormat,UINT16 startx, UINT16 starty,UINT16 Width,UINT16 Height, UINT8*uspImage )
{
	UINT8 RGB[0x10000];
	V558_SIZE size;
	UINT32 LEN;

	size.cx=Width;//g_Thumb_X;
	size.cy=Height;//g_Thumb_Y;
	if((Width==0)&&(Height==0))
	{
			size.cx=g_Thumb_X;
			size.cy=g_Thumb_Y;
	}
	if(rawFormat>2)
		rawFormat=1;
	V558_LcdALDisable();
	
	LEN=V558_YuvToRaw(RGB,uspImage,rawFormat,0,size,1);
	
	V558_LCDDrawBL(startx,starty,startx+size.cx,starty+size.cy,RGB);
	USER558_LCDUpdate();
}

/********************************************************************************

	Description:
		Change standard bmp to yuv

	Parameters:
		BmpBuf: the head point of bmp file.
		YuvBuf: the head point of yuv file.
		TYPE: the colordep of bmp.
		
    Note:
             	<TABLE>
		value					meaning
		-----					-------
		1                                       555;
		0                                       565;
		</TABLE>
		
	Remarks:
              valid
*********************************************************************************/
void USER558BmpToYuv(UINT8*BmpBuf,UINT8*YuvBuf,UINT8 TYPE)
{
	UINT16  width, height, x0, x1;
	V558_BITMAPINFOHEADER *pHead;

	V558_JPEGBUF pbuf,outbuf;
	
	pHead = (PV558_BITMAPINFOHEADER)(BmpBuf + sizeof(V558_BITMAPFILEHEADER));
	x0 = (SINT16)(pHead->biWidth[0]) & 0Xff;
	x1 = (SINT16)(pHead->biWidth[1]) & 0Xff;
	width = (x1 << 8) | x0;
	x0 = (SINT16)(pHead->biHeight[0]) & 0Xff;
	x1 = (SINT16)(pHead->biHeight[1]) & 0Xff;
	height = (x1 << 8) | x0;

	TYPE++;

	pbuf.ActLen=width*height*2;
	pbuf.Len=width*height*2;
	pbuf.pData= BmpBuf+sizeof(V558_BITMAPFILEHEADER) + sizeof(V558_BITMAPINFOHEADER);

	outbuf.ActLen=width*height*2;
	outbuf.Len=width*height*2;
	outbuf.pData=YuvBuf;
		
	V558_RGB2YUV(&pbuf,&outbuf,TYPE,0);//422
}
/********************************************************************************

	Description:
		Change yuv to rgb 
		
	Parameters:
		pYUV: the head point of YUV file.
		pBMP: the head point of BMP file.
		Width: the width of YUV image
		height: the height of YUV image
		yuvFormat: the format of input yuv
		rawFormat: the fomat of output rgb
		
	Note:
		rawFormat:
             	<TABLE>
		value					meaning
		-----					-------
		0					      888;
		1                                       565;
		2                                       555;
		default 				      565
		</TABLE>

		yuvFormat:
             	<TABLE>
		value					meaning
		-----					-------
		0						V558_YUV_422 ,
		1						V558_YUV_420,
		2						V558_YUV_411,
		3						V558_YUV_400,
		default					V558_YUV_422
		</TABLE>
		
	Remarks:
              valid
*********************************************************************************/
void USER558_YUV2RGB(UINT8* pYUV,UINT8* pBMP,UINT8 rawFormat,UINT8 yuvFormat,UINT16 Width,UINT16 Height)
{
	V558_SIZE size;
	UINT32 LEN;

	size.cx=Width;
	size.cy=Height;
	if(rawFormat>2)
		rawFormat=1;
	if(yuvFormat>3)
		yuvFormat=0;
	LEN=V558_YuvToRaw(pBMP,pYUV,rawFormat,yuvFormat,size,1);
}
/********************************************************************************

	Description:
		Draw a bmp file on LCD( B Layer)

	Parameters:
		startx: left coordinate of the Rectangle(pixel)
		starty: the up coordinate of the Rectangle(pixel)
		uspImage:         the point of memory which to fill the Rectangle
		Converse:  the bmp file will converse the high and low  byte and save in the same buf.

	Note:
	       Don't update in lcd,please related to void USER558_LCDUpdata(void) to update.
	       
	Remarks:
              valid
*********************************************************************************/
void USER558_LCDDrawBMP( UINT16 startx, UINT16 starty,UINT8*BmpBuf,UINT8 Converse)
{
	V558_POINT point;
    	UINT8 TEMP;
	point.x=startx;
	point.y=starty;

        TEMP=USER558_LCDStartDrawDot();
	V558_LcdDrawBmp(point,BmpBuf,Converse);

       USER558_LCDStopDrawDot(TEMP);

	USER558_LCDUpdate();

}

/********************************************************************************

	Description:
		Set capture compression ratio

	Parameter:
		ComRatio:  JPEG SIZE= image_width*image_hight*2/ComRatio (BYTES)

	Status:
	       Invalid now

*********************************************************************************/
void USER558_SetCapureCompressionRatio(UINT8 ComRatio)
{
	UINT32 temp;
	V558_JpegSetTCR((ComRatio<<2)+1);	
	temp=g_Preview_Para.Capture_W*g_Preview_Para.Capture_H*2;
	temp=temp/ComRatio;
	temp=temp>>2;
	V558_JpegSetTWC(temp);
}

