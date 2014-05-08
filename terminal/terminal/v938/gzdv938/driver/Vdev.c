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


 Module:		Vdev.c

 Description:	Device Driver functions definitions

 Update History:

	1.0			Basic version					2004.07.15		gzd

	1.1			Add timer functions				2004.08.02		gzd

	1.1.1		Add initialize chip device 		2004.10.25		gzd
				functions as follows:
			 		Vdev_Standby2Idle();
			 		Vdev_Idle2Normal();
			 		Vdev_Normal2Idel();
			 		Vdev_Idle2Standby();

	1.1.2		Fix a bug in Vdev_WriteFifo 	2004.10.27		gzd
				fix a bug about the volume of the output sound will be 
				variational if the PCM/ADPCM data which length can not 
				be divided by 4 to stream fifos.

	1.1.3										2004.10.29		gzd
				Add the function Vdev_GetCurPolyphonyNum(void)
				for get current polyphony numbers.

	1.1.4		Coding style					2004.11.03		gzd

	1.1.5		fix a bug about reset seq 0x80 -> 0x08 
				in function Vdev_CtrlSeq() in Vdev.c
												2004.11.15		gzd

	1.1.6										2004.11.18		gzd
				Add functions Vdev_SelectV8051RAM() and Vdev_UnSelectV8051()
				for Select V8051 ram area or not.

	1.1.7										2004.11.24		gzd
				Add Set global VOLSTEP case in Vdev_SetVol() function.

	1.1.8										2004.11.29		gzd
				Remove Mp3 reset, Int reset, WS reset Operations in case Mp3
				in function Vdev_ClearFifo().
				Place the operations Clear interrupt flags in function Vdev_WriteFifo().

	1.1.9										2004.12.08		gzd
				Add MCLKREF control and select V8051 address in function Vdev_Idle2Normal()
				for V8051 mode.

	1.2.0										2004.12.14		gzd
				Modify the control flow in Vdev_MainIsr().

	1.2.1										2004.12.28		gzd
				force to convert data types of parameters.

	1.2.2										2005.01.24		gzd
				Add Check CLK ready code in initializing functions.
	1.2.3			Add support VC0937 chip                                        2005.01.26                 Andy,liu

	1.2.4										2005.02.25		gzd
				Add code for stop mp3 channel.
				
-----------------------------------------------------------------------*/

#include "vdef.h"
#include "vmd.h"
#include "vdevdef.h"
#include "vhostctrl.h"
#include "vdev.h"


/********************************************************************
 Description:
 		Dummy ISR function.

 Parameters:
 		VOID.
	
 Returns:
 		VOID.

 Remarks:
 		
*********************************************************************/
static void DummyIsr(void){}


/********************************************************************
 Description:
		interrupt service routine.
*********************************************************************/
static VDEV_ISR g_VDEVIsr[VDEV_INT_NUM] = { 
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr,
											DummyIsr												
										};


/***************************************************************
 Description:
 		Add  an interrupt service routine to ISR table.
 
 Parameters:
 		bIntType:	[input] One of the following <U>VDEV_INTERRUPTID</U>
 					enumeration values that specify interrupt type.
 		fDev_Isr:	[input] Pointer to the interrupt service routine.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void	Vdev_AddIsr(UINT8 bIntType, VDEV_ISR fDev_Isr)
{
	g_VDEVIsr[bIntType] = fDev_Isr;
}


/***************************************************************
 Description:
 		Remove  an interrupt service routine fromISR table.
 
 Parameters:
 		bIntType:	[input] One of the following <U>VDEV_INTERRUPTID</U>
 					enumeration values that specify interrupt type.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void	Vdev_RemoveIsr(UINT8 bIntType)
{
	UINT32 i;
	
	if (bIntType == VDEV_INT_ALL)
	{
		for (i=0; i<VDEV_INT_NUM; i++)
			g_VDEVIsr[i] = DummyIsr;
	}
	else
		g_VDEVIsr[bIntType] = DummyIsr;
}

/***************************************************************
 Description:
 		Disable  an interrupt of device chip.
 
 Parameters:
 		bIntType:	[input] One of the following <U>VDEV_INTERRUPTID</U>
 					enumeration values that specify interrupt type.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void Vdev_MaskInt(UINT8 bIntType)
{
	UINT8 bIntEn0 = 0;
	UINT8 bIntEn1 = 0;
	UINT8 bIntEn2 = 0;
	
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	if (bIntType == VDEV_INT_ALL)
	{
		Vreg_WriteReg(VDEV_REG_INTEN0, 0x00);
		Vreg_WriteReg(VDEV_REG_INTEN1, 0x00);
		Vreg_WriteReg(VDEV_REG_INTEN2, 0x00);
	}
	else if (bIntType < VDEV_INT_MFIFOEMPTY)
	{
		bIntEn0 = Vreg_ReadReg(VDEV_REG_INTEN0);
		bIntEn0 &= ~(0x01 << bIntType);
		Vreg_WriteReg(VDEV_REG_INTEN0, bIntEn0);
	}
	else if (bIntType < VDEV_INT_STM0_END)
	{
		bIntEn1 = Vreg_ReadReg(VDEV_REG_INTEN1);
		bIntEn1 &= ~(0x01 << (bIntType-BITS_PER_BYTE));
		Vreg_WriteReg(VDEV_REG_INTEN1, bIntEn1);
	}else
	{
		bIntEn2 = Vreg_ReadReg(VDEV_REG_INTEN2);
		bIntEn2 &= ~(0x01 << (bIntType-BITS_PER_WORD));
		Vreg_WriteReg(VDEV_REG_INTEN2, bIntEn2);
	}	
}

/***************************************************************
 Description:
 		Enable  an interrupt of device chip.
 
 Parameters:
 		bIntType:	[input] One of the following <U>VDEV_INTERRUPTID</U>
 					enumeration values that specify interrupt type.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void Vdev_UnMaskInt(UINT8 bIntType)
{
	UINT8 bIntEn0 = 0;
	UINT8 bIntEn1 = 0;
	UINT8 bIntEn2 = 0;
	
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	if (bIntType == VDEV_INT_ALL)
	{
		Vreg_WriteReg(VDEV_REG_INTEN0, 0xFF);
		Vreg_WriteReg(VDEV_REG_INTEN1, 0xFF);
		Vreg_WriteReg(VDEV_REG_INTEN2, 0xFF);
	}
	else if (bIntType < VDEV_INT_MFIFOEMPTY)
	{
		bIntEn0 = Vreg_ReadReg(VDEV_REG_INTEN0);
		bIntEn0 |= (0x01 << bIntType);
		Vreg_WriteReg(VDEV_REG_INTEN0, bIntEn0);
	}
	else if (bIntType < VDEV_INT_STM0_END)
	{
		bIntEn1 = Vreg_ReadReg(VDEV_REG_INTEN1);
		bIntEn1 |= (0x01 << (bIntType-BITS_PER_BYTE));
		Vreg_WriteReg(VDEV_REG_INTEN1, bIntEn1);
	}else
	{
		bIntEn2 = Vreg_ReadReg(VDEV_REG_INTEN2);
		bIntEn2 |= (0x01 << (bIntType-BITS_PER_WORD));
		Vreg_WriteReg(VDEV_REG_INTEN2, bIntEn2);
	}
}

/***************************************************************
 Description:
        Clear the special Fifo in VC0938 chip.
 
 Parameters:
		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
	                <P>VDEV_FIFO_MP3
 Returns:
 		VOID.		
 
****************************************************************/
void	Vdev_ClearFifo(UINT8 bFifoType)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	switch(bFifoType)
	{
		case VDEV_FIFO_EVENT:
			Vreg_WriteReg(VDEV_REG_SWRST1, 0x40);
			break;
			
		case VDEV_FIFO_COMMAND:
			Vreg_WriteReg(VDEV_REG_SWRST2, 0x04);
			break;
			
		case VDEV_FIFO_STREAM0:
			Vreg_WriteReg(VDEV_REG_SWRST3, 0x01);
			break;
			
		case VDEV_FIFO_STREAM1:
			Vreg_WriteReg(VDEV_REG_SWRST3, 0x02);
			break;
			
		case VDEV_FIFO_STREAM2:
			Vreg_WriteReg(VDEV_REG_SWRST3, 0x04);
			break;
			
		case VDEV_FIFO_STREAM3:
			Vreg_WriteReg(VDEV_REG_SWRST3, 0x08);
			break;
			
		case VDEV_FIFO_MP3:
			//Vreg_WriteReg(VDEV_REG_SWRST1, 0x10);
			//Vreg_WriteReg(VDEV_REG_SWRST2, 0x11);	//the mp3 reset option will make EQ reset.
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);
			Vreg_WriteReg(VDEV_REG_MP3_CLR_A, 0xFF);
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

			break;

		case VDEV_FIFO_ALL:
			Vreg_WriteReg(VDEV_REG_SWRST1, 0x40);
			Vreg_WriteReg(VDEV_REG_SWRST2, 0x05);
			Vreg_WriteReg(VDEV_REG_SWRST3, 0x0F);
	}
}

/***************************************************************
 Description:
        Stop Event chnnals or Stream chnnals.
 
 Parameters:
		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_EVENT
	                <P>VDEV_FIFO_COMMAND
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
	                <P>VDEV_FIFO_MP3
 		
 Returns:
 		void.		
 
****************************************************************/
void		Vdev_StopChnl(UINT8 bFifoType)
{
	UINT8	i;
	
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	if (bFifoType == VDEV_FIFO_ALL)
	{
		for (i=0; i<8; i++)
			Vreg_WriteReg((UINT8)(VDEV_REG_CHNSTOP0 + i), 0xFF);
	}
	else if (bFifoType == VDEV_FIFO_EVENT || bFifoType == VDEV_FIFO_COMMAND)
	{
		Vreg_WriteReg(VDEV_REG_CHNSTOP0, 0xF0);
		for (i=0; i<7; i++)
			Vreg_WriteReg((UINT8)(VDEV_REG_CHNSTOP1 + i), 0xFF);
	}
	else if (bFifoType == VDEV_FIFO_MP3)
		Vreg_WriteReg(VDEV_REG_CHNSTOP0, 0x01);
	else
		Vreg_WriteReg(VDEV_REG_CHNSTOP0, (UINT8)(0x01 << (bFifoType - (UINT8)VDEV_FIFO_STREAM0)));
}

/***************************************************************
 Description:
        Clear the interrupt flag specified.
 
 Parameters:
 		bIntType:	[input] One of the following <U>VDEV_INTERRUPTID</U>
 					enumeration values that specify interrupt type.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_ClearFlag(UINT8 bIntType)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	if (bIntType == VDEV_INT_ALL)
	{
		Vreg_WriteReg(VDEV_REG_INTFLAG0, 0xFF);
		Vreg_WriteReg(VDEV_REG_INTFLAG1, 0xFF);
	}
	else if (bIntType < VDEV_INT_MFIFOEMPTY)
	{
		Vreg_WriteReg(VDEV_REG_INTFLAG0, (UINT8)(0x01 << bIntType));
	}
	else if (bIntType < VDEV_INT_STM0_END)
	{
		Vreg_WriteReg(VDEV_REG_INTFLAG1, (UINT8)(0x01 << (bIntType-VDEV_INT_MFIFOEMPTY)));
	}
	else 
	{
		Vreg_WriteReg(VDEV_REG_INTFLAG2, (UINT8)(0x01 << (bIntType-VDEV_INT_STM0_END)));
	}
}

/***************************************************************
 Description:
        Clear the special software interrupt marker.
 
 Parameters:
        bIntType:	[input] One of the following <U>VDEV_INTERRUPTID</U>
        			enumeration values that specify the software interrupt type.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_ClearSwi(UINT8 bIntType)
{
	if (bIntType > VDEV_INT_MFIFOEMPTY && bIntType < (VDEV_INT_SWI7 + 1))
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INT_SWI_SEL, (UINT8)(Vreg_ReadReg(VDEV_REG_INT_SWI_SEL) & (~(UINT8)(0x01 << (bIntType - VDEV_INT_SWI1 + 1)))));
	}
	else if (bIntType == VDEV_INT_ALL)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INT_SWI_SEL, 0x00);
	}	
}

/***************************************************************
 Description:
        Get the status of the special software interrupt.
 
 Parameters:
        bSwiFlag:	[input] the software interrupt flag.
 		
 Returns:
 		The state value.		
 
****************************************************************/
UINT8		Vdev_GetSwiState(UINT8 bSwiFlag)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	return (Vreg_ReadReg(VDEV_REG_INT_SWI_SEL) & bSwiFlag);
}

/***************************************************************
 Description:
        Clear the special flag for stream end in vmd playing.
 
 Parameters:
        bEndNum:	[input] One of the values that specify the 
        			special stream fifo end.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_ClearStmFlag(UINT8 bEndNum)
{
	if (bEndNum < 4)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg((UINT8)(VDEV_REG_FRATIO_DLT0+bEndNum), 0x00);
	}
}

/***************************************************************
 Description:
        Get the status of the special stream flag.
 
 Parameters:
        bEndNum:	[input] One of the values that specify the 
        			special stream fifo end.
 		
 Returns:
 		The state value.		
 
****************************************************************/
UINT8		Vdev_GetStmState(UINT8 bEndNum)
{
	if (bEndNum < 4)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		return Vreg_ReadReg((UINT8)(VDEV_REG_FRATIO_DLT0+bEndNum));
	}
	else
		return 0;
}

/***************************************************************
 Description:
        Specify Audio mode of the audio processor.
 
 Parameters:
        bAudioMode:	[input] One of the following <U>ENUM_VDEV_AUDIO_MODE</U>
        			enumeration values that specify the Audio mode.
					<P>VDEV_AUDIO_MODE_HOST_MP3_MIDI
					<P>VDEV_AUDIO_MODE_HOST_WAVE_MIDI
 		
 Returns:
 		VOID.		
 
****************************************************************/
void	Vdev_SetAudioMode(UINT8 bAudioMode)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(VDEV_REG_OP_MODE, bAudioMode);
}


/***************************************************************
 Description:
        Get command fifo state.
 
 Parameters:
 		VOID.
 		
 Returns:
 		return 1 if command fifo is empty, else returns 0.		
 
****************************************************************/
UINT8		Vdev_GetCFifoState(void)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	if(Vreg_ReadReg(VDEV_REG_BUFFLAG0) & 0x04)
		return 1;
	else 
		return 0;
}

/***************************************************************
 Description:
        Set the volume of the voice output.
 
 Parameters:
        bCtrlID:	[input] One of the following <U>ENUM_VDEV_VOL_CTRL_ID</U>
        			enumeration values that specify volume Control ID.
        bVolL:		[input] the volume value of left channel.
        bVolR:		[input] the volume value of right channel.
 		
 Returns:
 		VOID.		

 Remarks:
		Only the parameter "bVolL" is valid when setting MP3_VOLSTEP or SPVOL,
		and the range of bVolL is 0-31 for MP3_VOLSETP, 0-15 for SPVOL, 0-31 
		for Global VOLSTEP(9 is max volume).
****************************************************************/
void	Vdev_SetVol(UINT8 bCtrlID, UINT8 bVolL, UINT8 bVolR)
{
	if (bCtrlID == VDEV_CTRL_SET_SPVOL)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
		Vreg_WriteReg(VDEV_REG_SPVOL, bVolL);
	}
	else if (bCtrlID == VDEV_CTRL_SET_HPVOL)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_HPNVOL_L, bVolL);
		Vreg_WriteReg(VDEV_REG_HPNVOL_R, bVolR);
	}
	else if (bCtrlID == VDEV_CTRL_SET_MIDIVOL)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_VOLMID_L, bVolL);
		Vreg_WriteReg(VDEV_REG_VOLMID_R, bVolR);
	}
	else if (bCtrlID == VDEV_CTRL_SET_WAVEVOL)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_VOLWAV_L, bVolL);
		Vreg_WriteReg(VDEV_REG_VOLWAV_R, bVolR);
	}
	else if (bCtrlID == VDEV_CTRL_SET_GAINVOL)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_VOL_GAIN_L, bVolL);
		Vreg_WriteReg(VDEV_REG_VOL_GAIN_R, bVolR);
	}
	else if (bCtrlID == VDEV_CTRL_SET_MP3_VOLSTEP)
	{
		if(bVolL!=0)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);
			Vreg_WriteReg(VDEV_REG_MP3_VOLSTEP, (0x1F-bVolL*2));
			Vreg_WriteReg(VDEV_REG_OUT_SEL,0x00);
		}

	}
	else if (bCtrlID == VDEV_CTRL_SET_GLOBAL_VOLSTEP)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);
		Vreg_WriteReg(VDEV_REG_VOLSTEP, bVolL);
	}
	else if (bCtrlID == VDEV_CTRL_SET_DACMIXVOL)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_DACPGG, bVolL);
	}
}

/***************************************************************
 Description:
        Control the sequencer of the device.
 
 Parameters:
        bCtrlID:	[input] One of the following <U>ENUM_VDEV_CONTROL_ID</U>
        			enumeration values that specify Control ID.
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_CtrlSeq(UINT8 bCtrlID)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	switch (bCtrlID)
	{
		case VDEV_CTRL_START_SEQUENCER:
			Vreg_WriteReg(VDEV_REG_GLBCTRL, (UINT8)(Vreg_ReadReg(VDEV_REG_GLBCTRL) | VDEV_GLBCTRL_START_SEQ));
#if(AUDIO_CHIP==CHIP_VC0937)
			//Stop the ADC Mode
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
			Vreg_WriteReg(VDEV_REG_PA_ADC_MODE,PA_ADC_PLAY_BACK);
#endif
			break;

		case VDEV_CTRL_STOP_SEQUENCER:
			Vreg_WriteReg(VDEV_REG_GLBCTRL, (UINT8)(Vreg_ReadReg(VDEV_REG_GLBCTRL) & (~VDEV_GLBCTRL_START_SEQ)));
			break;

		case VDEV_CTRL_RESET_SEQUENCER:
			Vreg_WriteReg(VDEV_REG_SWRST1, 0x08);
			break;
	}
}

/***************************************************************
 Description:
        Control device state.
 
 Parameters:
        bCtrlID:	[input] One of the following <U>VDEV_CTRL_ID</U>
        			enumeration values that specify Control ID.
        pbInbuf:	[input] Pointer to the input buffer that store
        			control data.
        dInSize:	[input] the size of the input buffer.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void	Vdev_DeviceControl(UINT8 bCtrlID, UINT32 dParam1, UINT32 dParam2)
{
	UINT8	bReg;
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	switch (bCtrlID)
	{
		case VDEV_CTRL_SET_SEQ:
			break;

		case VDEV_CTRL_SET_MP3EQ:
			(void)dParam2;
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);
			Vreg_WriteReg(VDEV_REG_OUT_SEL, 0x00);
			switch(dParam1)
			{
				case MP3_EQ_ORIGINAL:
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND0, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND1, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND2, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND3, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND4, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND5, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND6, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND7, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND8, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND9, 0x1f);
					break;
				case MP3_EQ_CLASSICAL:
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND0, 0x2f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND1, 0x2e);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND2, 0x2a);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND3, 0x1d);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND4, 0x1c);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND5, 0x1e);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND6, 0x20);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND7, 0x2a);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND8, 0x2e);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND9, 0x2f);

					break;
				case MP3_EQ_CLUB:
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND0, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND1, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND2, 0x1b);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND3, 0x19);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND4, 0x19);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND5, 0x19);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND6, 0x1b);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND7, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND8, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND9, 0x1f);

					break;
				case MP3_EQ_POP:
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND0, 0x21);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND1, 0x1a);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND2, 0x18);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND3, 0x17);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND4, 0x1a);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND5, 0x21);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND6, 0x22);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND7, 0x22);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND8, 0x21);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND9, 0x21);

					break;
				case MP3_EQ_BASE:
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND0, 0x16);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND1, 0x16);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND2, 0x16);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND3, 0x19);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND4, 0x1d);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND5, 0x24);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND6, 0x28);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND7, 0x29);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND8, 0x2f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND9, 0x2f);

					break;
				case MP3_EQ_TREBLE:
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND0, 0x29);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND1, 0x29);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND2, 0x29);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND3, 0x24);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND4, 0x1c);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND5, 0x14);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND6, 0x2f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND7, 0x2f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND8, 0x2f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND9, 0x2f);

					break;
				case MP3_EQ_BASE_TREBLE:
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND0, 0x18);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND1, 0x19);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND2, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND3, 0x27);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND4, 0x24);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND5, 0x1d);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND6, 0x2a);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND7, 0x29);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND8, 0x28);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND9, 0x28);

					break;
				case MP3_EQ_ROCK:
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND0, 0x17);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND1, 0x1a);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND2, 0x25);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND3, 0x27);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND4, 0x23);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND5, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND6, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND7, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND8, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND9, 0x1f);
					
					break;

				case MP3_EQ_DANCE:
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND0, 0x16);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND1, 0x18);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND2, 0x1d);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND3, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND4, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND5, 0x25);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND6, 0x27);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND7, 0x27);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND8, 0x1f);
					Vreg_WriteReg(VDEV_REG_MP3_EQBAND9, 0x1f);

					break;
				default:
					break;
			}
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			break;
			
		case VDEV_CTRL_SET_LED_928:

			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x05);	
			Vreg_WriteReg(VDEV_REG_LEDMOT_CONF, 0x80);	
			Vreg_WriteReg(VDEV_REG_LED_CTRL1, 0x00);	//Close LED ZY:2004.11.4
			Vreg_WriteReg(VDEV_REG_LED_CTRL2, 0x00);	
			Vreg_WriteReg(VDEV_REG_LED_CTRL3, 0x00);	
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			
			break;
		case VDEV_CTRL_SET_LED_938:

			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x05);	

			Vreg_WriteReg(VDEV_REG_LED_CTRL1, 0x00);	//Close LED ZY:2004.11.4
			Vreg_WriteReg(VDEV_REG_LED_CTRL2, 0x00);	
			Vreg_WriteReg(VDEV_REG_LED_CTRL3, 0x00);
			
			Vreg_WriteReg(VDEV_REG_TIMESLICE_HIGH, (UINT8)((dParam1>>8)&0xff));
			Vreg_WriteReg(VDEV_REG_TIMESLICE_LOW, (UINT8)(dParam1&0xff));
			Vreg_WriteReg(VDEV_REG_PWM_RLED_LOW, 0xf0);
			Vreg_WriteReg(VDEV_REG_PWM_BLED_LOW, 0xf0 );
			Vreg_WriteReg(VDEV_REG_PWM_GLED_LOW,0xf0 );
			Vreg_WriteReg(VDEV_REG_CIRCLING_PERIOD,0x05 );

			Vreg_WriteReg(VDEV_REG_PATTERN0_HIGH, 0x04);
			Vreg_WriteReg(VDEV_REG_PATTERN0_LOW, 0x83);
			Vreg_WriteReg(VDEV_REG_PATTERN1_HIGH, 0x0c);
			Vreg_WriteReg(VDEV_REG_PATTERN1_LOW , 0x4b);
			Vreg_WriteReg(VDEV_REG_PATTERN2_HIGH, 0x08);
			Vreg_WriteReg(VDEV_REG_PATTERN2_LOW , 0xb7);
			Vreg_WriteReg(VDEV_REG_PATTERN3_HIGH, 0x05);
			Vreg_WriteReg(VDEV_REG_PATTERN3_LOW , 0x23);
			Vreg_WriteReg(VDEV_REG_PATTERN4_HIGH, 0x0f);
			Vreg_WriteReg(VDEV_REG_PATTERN4_LOW , 0xcb);
			Vreg_WriteReg(VDEV_REG_PATTERN5_HIGH, 0x05);
			Vreg_WriteReg(VDEV_REG_PATTERN5_LOW , 0xc7);
			Vreg_WriteReg(VDEV_REG_PATTERN6_HIGH, 0x01);
			Vreg_WriteReg(VDEV_REG_PATTERN6_LOW , 0xa6);
			Vreg_WriteReg(VDEV_REG_PATTERN7_HIGH, 0x06);
			Vreg_WriteReg(VDEV_REG_PATTERN7_LOW , 0x47);
			
			if(dParam2==1)
			{
				bReg = Vreg_ReadReg(VDEV_REG_LEDMOT_CONF);
				bReg |=0x29;
				Vreg_WriteReg(VDEV_REG_LEDMOT_CONF, bReg);

			}
			else
			{
				bReg = Vreg_ReadReg(VDEV_REG_LEDMOT_CONF);
				bReg &=(~0x01);
				Vreg_WriteReg(VDEV_REG_LEDMOT_CONF, bReg);
			}
				
				
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			
			break;
			
		case VDEV_CTRL_SET_MOTOR_928:
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x05);	
			Vreg_WriteReg(VDEV_REG_LEDMOT_CONF, 0x80);	
			Vreg_WriteReg(VDEV_REG_MOTOR_CTRL, 0x00);		//Close Motor ZY:2004.11.4
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			break;
			
		case VDEV_CTRL_SET_MOTOR_938:

			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x05);	

			Vreg_WriteReg(VDEV_REG_MOTOR_CTRL, 0x00); 		//Close Motor ZY:2004.11.4
			
			Vreg_WriteReg(VDEV_REG_TIMESLICE_HIGH, (UINT8)((dParam1>>8)&0xff));
			Vreg_WriteReg(VDEV_REG_TIMESLICE_LOW, (UINT8)(dParam1&0xff));
			Vreg_WriteReg(VDEV_REG_PWM_RLED_LOW, 0xf0);
			Vreg_WriteReg(VDEV_REG_PWM_BLED_LOW, 0xf0 );
			Vreg_WriteReg(VDEV_REG_PWM_GLED_LOW,0xf0 );
			Vreg_WriteReg(VDEV_REG_CIRCLING_PERIOD,0x05 );

			Vreg_WriteReg(VDEV_REG_PATTERN0_HIGH, 0x04);
			Vreg_WriteReg(VDEV_REG_PATTERN0_LOW, 0x83);
			Vreg_WriteReg(VDEV_REG_PATTERN1_HIGH, 0x0c);
			Vreg_WriteReg(VDEV_REG_PATTERN1_LOW , 0x4b);
			Vreg_WriteReg(VDEV_REG_PATTERN2_HIGH, 0x08);
			Vreg_WriteReg(VDEV_REG_PATTERN2_LOW , 0xb7);
			Vreg_WriteReg(VDEV_REG_PATTERN3_HIGH, 0x05);
			Vreg_WriteReg(VDEV_REG_PATTERN3_LOW , 0x23);
			Vreg_WriteReg(VDEV_REG_PATTERN4_HIGH, 0x0f);
			Vreg_WriteReg(VDEV_REG_PATTERN4_LOW , 0xcb);
			Vreg_WriteReg(VDEV_REG_PATTERN5_HIGH, 0x05);
			Vreg_WriteReg(VDEV_REG_PATTERN5_LOW , 0xc7);
			Vreg_WriteReg(VDEV_REG_PATTERN6_HIGH, 0x01);
			Vreg_WriteReg(VDEV_REG_PATTERN6_LOW , 0xa6);
			Vreg_WriteReg(VDEV_REG_PATTERN7_HIGH, 0x06);
			Vreg_WriteReg(VDEV_REG_PATTERN7_LOW , 0x47);

			
			if(dParam2==1)
			{
				bReg = Vreg_ReadReg(VDEV_REG_LEDMOT_CONF);
				bReg |=0x2A;
				Vreg_WriteReg(VDEV_REG_LEDMOT_CONF, bReg);

			}
			else
			{
				bReg = Vreg_ReadReg(VDEV_REG_LEDMOT_CONF);
				bReg &=(~0x02);
				Vreg_WriteReg(VDEV_REG_LEDMOT_CONF, bReg);
			}
			
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			
			break;
			
		case VDEV_CTRL_RESET_EQ:
			Vreg_WriteReg(VDEV_REG_SWRST1, 0x04);
			break;
			
		case VDEV_CTRL_RESET_WS:
			Vreg_WriteReg(VDEV_REG_SWRST1, 0x10);
			break;
			
		case VDEV_CTRL_RESET_MP3:
			Vreg_WriteReg(VDEV_REG_SWRST2, 0x01);
			break;
			
		case VDEV_CTRL_RESET_VIBRATOR:
			Vreg_WriteReg(VDEV_REG_SWRST2, 0x02);
			break;
			
		case VDEV_CTRL_RESET_MCLK:
			Vreg_WriteReg(VDEV_REG_SWRST2, 0x08);
			break;
			
		case VDEV_CTRL_RESET_INT:
			Vreg_WriteReg(VDEV_REG_SWRST2, 0x10);
			break;

		case VDEV_CTRL_RESET_SYS:
			Vreg_WriteReg(VDEV_REG_SWRST2, 0x20);
			break;
			
		case VDEV_CTRL_RESET_I2S:
			Vreg_WriteReg(VDEV_REG_SWRST2, 0x40);
			break;
				
		case VDEV_CTRL_RESET_V8051:
			Vreg_WriteReg(VDEV_REG_SWRST1, 0x20);
			break;

		case VDEV_CTRL_RESET_LED:
			Vreg_WriteReg(VDEV_REG_SWRST2, 0x80);
			break;
	}
}

/***************************************************************
 Description:
        Specify Aes mode and write AES key data (and initial vector 
        data if CBC mode) to device.
 
 Parameters:
        bAesMode:	[input] One of the following <U>VDEV_AES_MODE</U>
        			enumeration values that specify AES mode.
        pbKey:		[input] Pointer to the AES key data.
        pbInitVec:	[input] Pointer to the initial vector data.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_WriteAes(UINT8 bAesMode, PUINT8 pbKey, PUINT8 pbInitVec)
{
	UINT32	i;
	
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x03);
	if (bAesMode == VDEV_AES_MODE_ECB)
	{
		Vreg_WriteReg(VDEV_REG_AES_CORE_CMD, bAesMode);
		if (pbKey != NULL)
		{
			for (i=0; i<VDEV_AES_KEY_LENGTH; i++)
			{
				Vreg_WriteReg((UINT8)(VDEV_REG_AES_KEY0 + i), pbKey[i]);
			}
		}
	}else if (bAesMode == VDEV_AES_MODE_CBC)
	{
		Vreg_WriteReg(VDEV_REG_AES_CORE_CMD, bAesMode);
		if (pbKey != NULL)
		{
			for (i=0; i<VDEV_AES_KEY_LENGTH; i++)
			{
				Vreg_WriteReg((UINT8)(VDEV_REG_AES_KEY0+i), pbKey[i]);
			}
		}
		if (pbInitVec != NULL)
		{
			for (i=0; i<VDEV_AES_INITVEC_LENGTH; i++)
			{
				Vreg_WriteReg((UINT8)(VDEV_REG_AES_INITVEC0+i), pbInitVec[i]);
			}
		}
	}
	Vreg_Delay(5);
}

/***************************************************************
 Description:
        Write general data to the special fifo.
 
 Parameters:
        bFifoType:	[input] One of the Following <U>VDEV_FIFO_TYPE</U>
        			enumeration values that specify fifo type.
        pbFifodata:	[input] Pointer to the general data buffer.
        wSize:		[input] the size of the general data buffer.
 		
 Returns:
 		VOID.		

 20041027 by gzd 
 	fix a bug about the volume of the output sound will be variational 
 	if the PCM/ADPCM data which length can not be divided by 4 to stream
 	fifos.
****************************************************************/
void		Vdev_WriteFifo(UINT8 bFifoType, PUINT8 pbFifodata, UINT16 wSize)
{
	UINT16 wLen;

	wLen = wSize%4;
	
	if ((wLen > 0) && (bFifoType > VDEV_FIFO_COMMAND) && (bFifoType < VDEV_FIFO_MP3))
		wSize = wSize - wLen;
	
	if (pbFifodata != NULL && wSize > 0)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

		if (bFifoType == VDEV_FIFO_EVENT)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, VDEV_INTSERV_EVENT);

			Vreg_WriteMulRegs(VDEV_REG_EFIFO, pbFifodata, wSize);

			Vdev_ClearFlag(VDEV_INT_EFIFOEMPTY);
		}
		else if (bFifoType == VDEV_FIFO_MP3)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, VDEV_INTSERV_MP3);

			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);
			Vreg_WriteMulRegs(VDEV_REG_MP3_FIFO, pbFifodata, wSize);

			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
			Vdev_ClearFlag(VDEV_INT_MFIFOEMPTY);
		}
		else if (bFifoType == VDEV_FIFO_STREAM0)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, VDEV_INTSERV_STREAM0);

			Vreg_WriteMulRegs(VDEV_REG_SFIFO, pbFifodata, wSize);
			
			Vdev_ClearFlag(VDEV_INT_SFIFO0EMPTY);
		}
		else if (bFifoType == VDEV_FIFO_STREAM1)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, VDEV_INTSERV_STREAM1);

			Vreg_WriteMulRegs(VDEV_REG_SFIFO, pbFifodata, wSize);

			Vdev_ClearFlag(VDEV_INT_SFIFO1EMPTY);
		}
		else if (bFifoType == VDEV_FIFO_STREAM2)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, VDEV_INTSERV_STREAM2);

			Vreg_WriteMulRegs(VDEV_REG_SFIFO, pbFifodata, wSize);

			Vdev_ClearFlag(VDEV_INT_SFIFO2EMPTY);
		}
		else if (bFifoType == VDEV_FIFO_STREAM3)
		{
			Vreg_WriteReg(VDEV_REG_INTSERV, VDEV_INTSERV_STREAM3);

			Vreg_WriteMulRegs(VDEV_REG_SFIFO, pbFifodata, wSize);

			Vdev_ClearFlag(VDEV_INT_SFIFO3EMPTY);
		}
		else if (bFifoType == VDEV_FIFO_COMMAND)
		{
			Vreg_WriteMulRegs(VDEV_REG_CFIFO, pbFifodata, wSize);
		}
		else if (bFifoType == VDEV_FIFO_V8051RAM)
		{
			Vreg_WriteMulRegs(VDEV_REG_V8051RAM, pbFifodata, wSize);
		}

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_INTSERV, VDEV_INTSERV_RESET);
	}
}

/***************************************************************
 Description:
        Set the water mark level of the special fifo.
 
 Parameters:
        bFifoType:	[input] One of the Following <U>VDEV_FIFO_TYPE</U>
        			enumeration values that specify fifo type.
 		bWaterM:	[input] the water mark level of the special fifo.
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_SetWaterM(UINT8 bFifoType, UINT8 bWaterM)
{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		if (bFifoType == VDEV_FIFO_EVENT)
		{
			Vreg_WriteReg(VDEV_REG_BUF1_W_MARK, bWaterM);
		}
		else if (bFifoType == VDEV_FIFO_MP3)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);
			Vreg_WriteReg(VDEV_REG_MP3_W_MARK, (UINT8)((bWaterM+1)<<2));
		}
		else if (bFifoType == VDEV_FIFO_STREAM0)
		{
			Vreg_WriteReg(VDEV_REG_SFIFO0_WM, bWaterM);
		}
		else if (bFifoType == VDEV_FIFO_STREAM1)
		{
			Vreg_WriteReg(VDEV_REG_SFIFO1_WM, bWaterM);
		}
		else if (bFifoType == VDEV_FIFO_STREAM2)
		{
			Vreg_WriteReg(VDEV_REG_SFIFO2_WM, bWaterM);
		}
		else if (bFifoType == VDEV_FIFO_STREAM3)
		{
			Vreg_WriteReg(VDEV_REG_SFIFO3_WM, bWaterM);
		}
}

/***************************************************************
 Description:
        Set the base address in special stream fifo.
 
 Parameters:
        bFifoType:	[input] One of the Following <U>VDEV_FIFO_TYPE</U>
        			enumeration values that specify fifo type.
        wBaseAddr:	[input] the base address of stream data.
        
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_SetBaseAddr(UINT8 bFifoType, UINT16 wBaseAddr)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	if (bFifoType > VDEV_FIFO_COMMAND && bFifoType < VDEV_FIFO_MP3)
	{
		Vreg_WriteReg((UINT8)(VDEV_REG_SFIFO0_AW_BL + ((bFifoType-VDEV_FIFO_STREAM0) << 2)), (UINT8)wBaseAddr);
		Vreg_WriteReg((UINT8)(VDEV_REG_SFIFO0_AW_BH + ((bFifoType-VDEV_FIFO_STREAM0) << 2)), (UINT8)(wBaseAddr >> 8));
	}
}

/***************************************************************
 Description:
        Set End pointer in the special stream fifo.
 
 Parameters:
        bFifoType:	[input] One of the Following <U>VDEV_FIFO_TYPE</U>
        			enumeration values that specify fifo type.
        wEndPtr:	[input] the End pointer of stream data.
        
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_SetEndPtr(UINT8 bFifoType, UINT16 wEndPtr)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	if (bFifoType > VDEV_FIFO_COMMAND && bFifoType < VDEV_FIFO_MP3)
	{
		Vreg_WriteReg((UINT8)(VDEV_REG_SFIFO0_AW_EL + ((bFifoType-VDEV_FIFO_STREAM0) << 2)), (UINT8)wEndPtr);
		Vreg_WriteReg((UINT8)(VDEV_REG_SFIFO0_AW_EH + ((bFifoType-VDEV_FIFO_STREAM0) << 2)), (UINT8)(wEndPtr >> 8));
	}
}
/***************************************************************
 Description:
        Load stream fifo address.
 
 Parameters:
        bFifoType:	[input] One of the Following <U>VDEV_FIFO_TYPE</U>
        			enumeration values that specify fifo type.
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_LoadAddr(UINT8 bFifoType)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	if ((bFifoType > VDEV_FIFO_COMMAND) && (bFifoType < VDEV_FIFO_MP3))
		Vreg_WriteReg(VDEV_REG_LOADADDR, (UINT8)(0x01 << (bFifoType-VDEV_FIFO_STREAM0)));
}

/***************************************************************
 Description:
        Reset the MCLK of the device chip.
 
 Parameters:
        void.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_ResetClk(void)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	
	Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0x1E);
	Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0x0E);
	Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0x00);
	Vreg_Delay(1);
}

/***************************************************************
 Description:
        Start the MCLK of the device chip.
 
 Parameters:
        bPllMode:	set working mode of the PLL in device chip.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_StartClk(UINT8 bPllMode)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	
	if(bPllMode == VREG_PLL_WORK_MODE)			//PLL working mode
	{
		Vreg_SetPLL();
		Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0xFE);
		Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0x0E);
		Vreg_Delay(2);
		Vdev_ResetClk();
	} else if(bPllMode == VREG_PLL_BYPASS_MODE)	//PLL bypass mode 
	{
		Vreg_WriteReg(VDEV_REG_CLKCTRL3, 0xFF);
		Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0xFE);
		Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0xFE);
		Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0x1E);
		Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0x0E);
		Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0xC0);
	}
}

/***************************************************************
 Description:
        Check the MCLK of the device chip.
 
 Parameters:
        void.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_CheckClk(void)
{
	UINT8	bVal1, bVal2;
	UINT8	i,j;
	UINT8	bReady = 0;

	for (i=0; i<200; i++)
	{
		for (j=0; j<4; j++)
		{
			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

			Vreg_WriteReg(VDEV_REG_DLYTIME, 0xAA);
			Vreg_WriteReg(VDEV_REG_EFIFO_EPT_TT, 0x55);

			Vreg_Delay(0);

			bVal1 = Vreg_ReadReg(VDEV_REG_DLYTIME);
			bVal2 = Vreg_ReadReg(VDEV_REG_EFIFO_EPT_TT);

			if ((bVal1 == 0xAA) && (bVal2 == 0x55))
				bReady = 0x01;
			else
				bReady = 0x00;

			Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

			Vreg_WriteReg(VDEV_REG_DLYTIME, 0x55);
			Vreg_WriteReg(VDEV_REG_EFIFO_EPT_TT, 0xAA);

			Vreg_Delay(0);

			bVal1 = Vreg_ReadReg(VDEV_REG_DLYTIME);
			bVal2 = Vreg_ReadReg(VDEV_REG_EFIFO_EPT_TT);

			if ((bVal1 == 0x55) && (bVal2 == 0xAA) && bReady)
				bReady = 0x01;
			else
				bReady = 0x00;
		}

		if (bReady)
		{
			Vdev_ResetClk();
			break;
		}

		Vdev_ResetClk();
	}
}

/***************************************************************
 Description:
        Stop the MCLK of the device chip.
 
 Parameters:
        VOID.
 		
 Returns:
 		void.		
 
****************************************************************/
void		Vdev_StopClk(void)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	
	Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0xFE);
	Vreg_WriteReg(VDEV_REG_CLKCTRL0, 0xFF);
}

/***************************************************************
 Description:
        Set the count value for the special timer interrupt.
 
 Parameters:
        bTimerNo:	the timer interrupt source No. Set 0 for Timer0
        			(Short timer) interrupt and 1 for Timer1(Long timer)
        			interrupt.
        dCount:		the count value that specify the interval time
        			of timer interrupts occur.
					 The count unit of this timer is based on TMRBASE.
					 Time = dCount*(MCLK*TMRBASE)
					 If MCLK*TMRBASE = 1ms then the interval time is
					 dCount ms.
 		
 Returns:
 		VOID.		

 Remarks:
 		the max count value is 0xFF for timer0 and 0xFFFFFF for timer1.
 		
****************************************************************/
void		Vdev_SetTimer(UINT8 bTimerNo, UINT32 dCount)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

	if (bTimerNo)		//For Timer1(Long timer)
	{
		Vreg_WriteReg(VDEV_REG_TMRTGT_L, (UINT8)(dCount >> 0));
		Vreg_WriteReg(VDEV_REG_TMRTGT_M, (UINT8)(dCount >> 8));
		Vreg_WriteReg(VDEV_REG_TMRTGT_H, (UINT8)(dCount >> 16));
	}
	else				//For Timer0(Short timer)
	{
		Vreg_WriteReg(VDEV_REG_STMRTGT, (UINT8)dCount);
	}
}


/***************************************************************
 Description:
        Get the count value of the special timer.
 
 Parameters:
        bTimerNo:	the timer interrupt source No. Set 0 for Timer0
        			(Short timer) interrupt and 1 for Timer1(Long timer)
        			interrupt.
 		
 Returns:
 		the count value of the timer.		

 Remarks:
 		the max count value is 0xFF for timer0 and 0xFFFFFF for timer1.
 		
****************************************************************/
UINT32		Vdev_GetTimerCount(UINT8 bTimerNo)
{
	UINT32  dwCount;

	dwCount = 0;
	
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

	if (bTimerNo)		//For Timer1(Long timer)
	{
		dwCount = (UINT32)(Vreg_ReadReg(VDEV_REG_TMR_L));
		dwCount += ((UINT32)(Vreg_ReadReg(VDEV_REG_TMR_M)) << 8);
		dwCount += ((UINT32)(Vreg_ReadReg(VDEV_REG_TMR_H)) << 16);

		return dwCount;
	}
	else				//For Timer0(Short timer)
	{
		return (UINT32)Vreg_ReadReg(VDEV_REG_STMR);
	}
}
/***************************************************************
 Description:
        Start the special timer count.
 
 Parameters:
        bTimerNo:	the timer interrupt source No. Set 0 for Timer0
        			(Short timer) interrupt and 1 for Timer1(Long timer)
        			interrupt.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_StartTimer(UINT8 bTimerNo)
{
	if (bTimerNo)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_GLBCTRL, (UINT8)(Vreg_ReadReg(VDEV_REG_GLBCTRL) | VDEV_GLBCTRL_START_TMR1));
	}
	else
	{
		Vdev_UnMaskInt(VDEV_INT_STMR);

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_GLBCTRL, (UINT8)(Vreg_ReadReg(VDEV_REG_GLBCTRL) | VDEV_GLBCTRL_START_TMR0));
	}
}

/***************************************************************
 Description:
        Stop the special timer count.
 
 Parameters:
        bTimerNo:	the timer interrupt source No. Set 0 for Timer0
        			(Short timer) interrupt and 1 for Timer1(Long timer)
        			interrupt.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_StopTimer(UINT8 bTimerNo)
{
	Vdev_ResetTimer(bTimerNo);
	if (bTimerNo)
	{
		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_GLBCTRL, (UINT8)(Vreg_ReadReg(VDEV_REG_GLBCTRL) & (~VDEV_GLBCTRL_START_TMR1)));
	}
	else
	{
		Vdev_MaskInt(VDEV_INT_STMR);

		Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
		Vreg_WriteReg(VDEV_REG_GLBCTRL, (UINT8)(Vreg_ReadReg(VDEV_REG_GLBCTRL) & (~VDEV_GLBCTRL_START_TMR0)));
	}
}

/***************************************************************
 Description:
        Reset the special timer.
 
 Parameters:
        bTimerNo:	the timer interrupt source No. Set 0 for Timer0
        			(Short timer) interrupt and 1 for Timer1(Long timer)
        			interrupt.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void		Vdev_ResetTimer(UINT8 bTimerNo)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	if (bTimerNo)
	{
		Vreg_WriteReg(VDEV_REG_SWRST1, 0x02);
	}
	else
	{
		Vreg_WriteReg(VDEV_REG_SWRST1, 0x01);
	}
}


/***************************************************************
 Description:
        the main interrupt service routine for all vc0938 interrupts.
 
 Parameters:
        VOID.
 		
 Returns:
 	VOID.		
 
****************************************************************/
void Vdev_MainIsr(void)
{
	UINT8 bIntEn0 = 0;
	UINT8 bIntEn1 = 0;
	UINT8 bIntEn2 = 0;
	UINT8 bIntFlag0 = 0;
	UINT8 bIntFlag1 = 0;
	UINT8 bIntFlag2 = 0;
	UINT8 i = 0;

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	//Lock chip interrupt at first.
	Vreg_WriteReg(VDEV_REG_INTLOCK, 0x01);

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	bIntEn2 = Vreg_ReadReg(VDEV_REG_INTEN2);

	if (bIntEn2)
	{
		bIntFlag2 = Vreg_ReadReg(VDEV_REG_INTFLAG2);

		if (bIntFlag2)
		{
			for (i=0; i<(BITS_PER_BYTE-3); i++)
			{
				if ((bIntEn2 & bIntFlag2) & (0x01 << i))
				{
					if (i == 4)
						Vreg_WriteReg(VDEV_REG_INTSERV, 0x04);
					else
						Vreg_WriteReg(VDEV_REG_INTSERV, (UINT8)(0x01 << (i+4)));
					
					g_VDEVIsr[VDEV_INT_STM0_END+i]();

					Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
					Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
				}
			}
		}
	}

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	bIntEn1 = Vreg_ReadReg(VDEV_REG_INTEN1);

	if (bIntEn1)
	{
		bIntFlag1 = Vreg_ReadReg(VDEV_REG_INTFLAG1);

		if (bIntFlag1)
		{
			for (i=0; i<BITS_PER_BYTE; i++)
			{
				if ((bIntEn1 & bIntFlag1) & (0x01 << i))
				{
					g_VDEVIsr[VDEV_INT_MFIFOEMPTY+i]();
				}
			}
		}
	}

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	bIntEn0 = Vreg_ReadReg(VDEV_REG_INTEN0);

	if (bIntEn0)
	{
		bIntFlag0 = Vreg_ReadReg(VDEV_REG_INTFLAG0);

		if (bIntFlag0)
		{
			for (i=0; i<BITS_PER_BYTE; i++)
			{
				if ((bIntEn0 & bIntFlag0) & (0x01 << i))
				{
					g_VDEVIsr[i]();
				}
			}
		}
	}

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(VDEV_REG_INTSERV, 0x00);
	//Unlock chip interrupt after processing
	Vreg_WriteReg(VDEV_REG_INTLOCK, 0x00);
}

#if(AUDIO_CHIP==CHIP_VC0937)
/********************************************************************
 Description:
	Initialize ADC control..

 Parameters:
	VOID. 
	
 Returns:
 	VOID.

 Remarks:
	void.

*********************************************************************/
void SetADC()
{
	//Set ADC work delay
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);

	Vreg_WriteReg(VDEV_REG_PA_DELAY,0x1);
	Vreg_WriteReg(VDEV_REG_PA_AVERAGE_NUMBER,0x14);
	Vreg_WriteReg(VDEV_REG_PA_SLOPE,0x0);

	//Set ADC MCLK ready
	Vreg_WriteReg(VDEV_REG_PA_SVREF_EN,01);
	Vreg_WriteReg(VDEV_REG_PA_MCLK_READY,01);

	//Set ADC waveform
	Vreg_WriteReg(VDEV_REG_PA_TW_TGT,0x05);
	Vreg_WriteReg(VDEV_REG_PA_TE_TGT,0x07);
	Vreg_WriteReg(VDEV_REG_PA_TP_TGT,0x0A);
	Vreg_WriteReg(VDEV_REG_PA_TT_TGT,0x03);
}
#endif

/********************************************************************
 Description:
        Initialize chip device from standby mode to Idle mode.

 Parameters:
        VOID. 
	
 Returns:
 	VOID.

 Remarks:
        after execute this function, PA is mute. when play back music, 
        please unmute PA at first.

*********************************************************************/
void Vdev_Standby2Idle(void)
{
#if(AUDIO_CHIP==CHIP_VC0938)

	//Start VC0938 MCLK
	Vdev_StartClk(VREG_PLL_MODE);
	
	//write registers in Page 1
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	Vreg_WriteReg(VDEV_REG_SWRST1, 0x80);

	Vdev_CheckClk();
	
	//write registers in Page 0
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	
	Vreg_WriteReg(VDEV_REG_GPIO_CFG, 0xF0); 
	Vreg_WriteReg(VDEV_REG_FUNC_SEL, 0x05);
	Vreg_WriteReg(VDEV_REG_I2S_CFG, 0x46);	
	Vreg_WriteReg(VDEV_REG_DACCFG, 0x3E); 
	Vreg_WriteReg(VDEV_REG_PA_MCLK_READY,0x0);	
	
	//Vreg_WriteReg(VDEV_REG_DACCTRL, 0x09);	
	//Vreg_Delay(5);
	//Vreg_WriteReg(VDEV_REG_PACFG, 0xCD);	
	//Vreg_Delay(5);
	
	//write registers in Page 1
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	Vreg_WriteReg(VDEV_REG_INT_GLB_MOD_SEL, 0x02);	
	Vreg_WriteReg(VDEV_REG_INTCTRL, VREG_INT_CTRL_SET);
	Vreg_WriteReg(VDEV_REG_GLBCFG, 0x02); 	
	Vreg_WriteReg(VDEV_REG_WS_MISC, 0x01); 	
	
	//Stop chip MCLK
	Vdev_StopClk();
#endif

#if(AUDIO_CHIP==CHIP_VC0937)

	//Start VC0938 MCLK
	Vdev_StartClk(VREG_PLL_MODE);
	
	//write registers in Page 1
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	Vreg_WriteReg(VDEV_REG_SWRST1, 0x80);

	Vdev_CheckClk();
	
	//write registers in Page 0
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	
	Vreg_WriteReg(VDEV_REG_GPIO_CFG, 0xF0); 
	Vreg_WriteReg(VDEV_REG_FUNC_SEL, 0x05);
	Vreg_WriteReg(VDEV_REG_I2S_CFG, 0x46);	
	Vreg_WriteReg(VDEV_REG_DACCFG, 0x3E); 	
	
	//Vreg_WriteReg(VDEV_REG_DACCTRL, 0x09);	
	//Vreg_Delay(5);
	//Vreg_WriteReg(VDEV_REG_PACFG, 0xCD);	
	//Vreg_Delay(5);

	SetADC();
	//Stop the ADC
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	Vreg_WriteReg(VDEV_REG_PA_ADC_MODE,PA_ADC_RESERVE);


	//write registers in Page 1
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	Vreg_WriteReg(VDEV_REG_INT_GLB_MOD_SEL, 0x02);	
	Vreg_WriteReg(VDEV_REG_INTCTRL, VREG_INT_CTRL_SET);
	Vreg_WriteReg(VDEV_REG_GLBCFG, 0x02); 	
	Vreg_WriteReg(VDEV_REG_WS_MISC, 0x01); 	
	
	//Stop chip MCLK
	Vdev_StopClk();
#endif
}

/********************************************************************
 Description:
        Initialize chip device from Idle mode to Normal mode.

 Parameters:
        VOID. 
	
 Returns:
	VOID.

 Remarks:
        after execute this function, PA is mute. when play back music, 
        please unmute PA at first.

*********************************************************************/
void Vdev_Idle2Normal(void)
{

#if(AUDIO_CHIP==CHIP_VC0938)

	//Start VC0938 MCLK
	Vdev_StartClk(VREG_PLL_MODE);

	Vdev_CheckClk();
	
	//write registers in Page 0
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);

	Vreg_WriteReg(VDEV_REG_PMCTRL, 0x0F);
	Vreg_WriteReg(VDEV_REG_MISC_CON, 0x02);	
	
	Vreg_WriteReg(VDEV_REG_PACFG, 0x4D);
	Vreg_Delay(15);
	Vreg_WriteReg(VDEV_REG_DACCTRL, 0x08);
	Vreg_Delay(10);

	Vreg_WriteReg(VDEV_REG_SPVOL, 0x0F);	

	Vreg_WriteReg(VDEV_REG_MCLKREF_DLYSEL, 0x07);	
	Vreg_WriteReg(VDEV_REG_MCLKPHASE_DLYSEL, 0x05); 
	Vreg_WriteReg(VDEV_REG_MCLKREF_PHASESEL, 0x01); 
	
	//write registers in Page 2
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);

	Vreg_WriteReg(VDEV_REG_MP3_VOLSTEP, 0x00);
	Vreg_WriteReg(VDEV_REG_VOLSTEP, 0x09);
	
	//write registers in Page 1
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	Vreg_WriteReg(VDEV_REG_TMRBASE_L, 0xC0);
	Vreg_WriteReg(VDEV_REG_TMRBASE_H, 0x5D);

	Vreg_WriteReg(VDEV_REG_VOLMID_L, 0x0);
	Vreg_WriteReg(VDEV_REG_VOLMID_R, 0x0);
	Vreg_WriteReg(VDEV_REG_VOLWAV_L, 0x0);
	Vreg_WriteReg(VDEV_REG_VOLWAV_R, 0x0);
	Vreg_WriteReg(VDEV_REG_HPNVOL_L, 0x0);
	Vreg_WriteReg(VDEV_REG_HPNVOL_R, 0x0);

	Vreg_WriteReg(VDEV_REG_CHNCTRL, 0x02);	
	Vreg_WriteReg(VDEV_REG_OP_MODE, 0x00);
	
	Vreg_Delay(5);
	
	Vreg_WriteReg(VDEV_REG_V8051_ADDR_HH, 0x01);
	Vreg_WriteReg(VDEV_REG_V8051_ADDR_H, 0x6D);
	Vreg_WriteReg(VDEV_REG_V8051_ADDR_L, 0x60);
	
	Vreg_WriteReg(VDEV_REG_SEL_CONV, 0x01);
	Vreg_WriteReg(VDEV_REG_SWRST1, 0x20);
		
	Vreg_Delay(5);
#endif

#if(AUDIO_CHIP==CHIP_VC0937)

	//Start VC0938 MCLK
	Vdev_StartClk(VREG_PLL_MODE);

	Vdev_CheckClk();
	
	//write registers in Page 0
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);

	Vreg_WriteReg(VDEV_REG_PMCTRL, 0x0F);
	Vreg_WriteReg(VDEV_REG_MISC_CON, 0x02);	
	
	Vreg_WriteReg(VDEV_REG_PACFG, 0x4D);
	Vreg_Delay(15);
	Vreg_WriteReg(VDEV_REG_DACCTRL, 0x08);
	Vreg_Delay(10);

	Vreg_WriteReg(VDEV_REG_SPVOL, 0x0F);	
	
	//the delay is not same as the VC0938
	Vreg_WriteReg(VDEV_REG_MCLKREF_DLYSEL, 0x09);	
	Vreg_WriteReg(VDEV_REG_MCLKPHASE_DLYSEL, 0x01); 
	Vreg_WriteReg(VDEV_REG_MCLKREF_PHASESEL, 0x02); 
		

	//write registers in Page 2
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x02);

	Vreg_WriteReg(VDEV_REG_MP3_VOLSTEP, 0x00);
	Vreg_WriteReg(VDEV_REG_VOLSTEP, 0x09);
	
	//write registers in Page 1
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	Vreg_WriteReg(VDEV_REG_TMRBASE_L, 0xC0);
	Vreg_WriteReg(VDEV_REG_TMRBASE_H, 0x5D);

	Vreg_WriteReg(VDEV_REG_VOLMID_L, 0x0);
	Vreg_WriteReg(VDEV_REG_VOLMID_R, 0x0);
	Vreg_WriteReg(VDEV_REG_VOLWAV_L, 0x0);
	Vreg_WriteReg(VDEV_REG_VOLWAV_R, 0x0);
	Vreg_WriteReg(VDEV_REG_HPNVOL_L, 0x0);
	Vreg_WriteReg(VDEV_REG_HPNVOL_R, 0x0);

	Vreg_WriteReg(VDEV_REG_CHNCTRL, 0x02);	
	Vreg_WriteReg(VDEV_REG_OP_MODE, 0x00);

	SetADC();
	//Stop the ADC Mode
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	Vreg_WriteReg(VDEV_REG_PA_ADC_MODE,PA_ADC_VREF_RESET);
	

	Vreg_Delay(5);

	//write registers in Page 1
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	Vreg_WriteReg(VDEV_REG_V8051_ADDR_HH, 0x01);
	Vreg_WriteReg(VDEV_REG_V8051_ADDR_H, 0x6D);
	Vreg_WriteReg(VDEV_REG_V8051_ADDR_L, 0x60);
	

	Vreg_WriteReg(VDEV_REG_SEL_CONV, 0x01);
	Vreg_WriteReg(VDEV_REG_SWRST1, 0x20);
		
	Vreg_Delay(40);
#if VC0937_PLAYBACK
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	Vreg_WriteReg(VDEV_REG_PA_DELAY,0x20);
	Vreg_WriteReg(VDEV_REG_PA_ADC_MODE,PA_ADC_PLAY_BACK);
	Vreg_WriteReg(VDEV_REG_PA_SLOPE,0x80);
#endif

#endif

}


/********************************************************************
 Description:
        Terminate chip device from normal mode to Idle mode.

 Parameters:
        VOID.
	
 Returns:
 		VOID.

 Remarks:
 		
*********************************************************************/
void Vdev_Normal2Idle(void)
{
	//write registers in Page 0
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	
	Vreg_WriteReg(VDEV_REG_PACFG, 0xCD);
	Vreg_Delay(5);
	Vreg_WriteReg(VDEV_REG_FUNC_SEL, 0x00);
	
	Vreg_WriteReg(VDEV_REG_DACCTRL, 0x09);
	Vreg_Delay(15);
	Vreg_WriteReg(VDEV_REG_PMCTRL, 0x00);
	Vreg_Delay(5);

#if(AUDIO_CHIP==CHIP_VC0937)
	//Stop the ADC;	
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	Vreg_WriteReg(VDEV_REG_PA_ADC_MODE,PA_ADC_DEBUG);
	//Open the Inner timmer interrupt
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(PA_EXTERN_INTERRUPT,0x01);

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	Vreg_WriteReg(VDEV_REG_PA_MCLK_READY,0x0);
#endif	

	//write registers in Page 1
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	
	//Stop chip MCLK
	Vdev_StopClk();
}

/********************************************************************
 Description:
        Terminate chip device from Idle mode to Standby mode.

 Parameters:
        VOID.
	
 Returns:
 		VOID.

 Remarks:
 		
*********************************************************************/
void Vdev_Idle2Standby(void)
{
	//Start VC0938 MCLK
	Vdev_StartClk(VREG_PLL_MODE);

	Vdev_CheckClk();
	
	//write registers in Page 0
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	
	Vreg_WriteReg(VDEV_REG_PACFG, 0x8D);
	Vreg_Delay(5);
	Vreg_WriteReg(VDEV_REG_DACCTRL, 0x07);
	
	//write registers in Page 1
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

	Vreg_WriteReg(VDEV_REG_SWRST1, 0x88);

#if(AUDIO_CHIP==CHIP_VC0907)
	//write registers in Page 0
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	Vreg_WriteReg(VDEV_REG_PA_ADC_MODE,PA_ADC_DEBUG);

	//Open the Inner timmer interrupt
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(PA_EXTERN_INTERRUPT,0x01);

#endif	
	//Stop chip MCLK
	Vdev_StopClk();
}


/********************************************************************
 Description:
        Open PA while playing

 Parameters:
        VOID.
	
 Returns:
 		VOID.

 Remarks:
 		
*********************************************************************/
void Vdev_Normal2Start(void)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	Vreg_WriteReg(VDEV_REG_PACFG, 0x0D);
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
}


/********************************************************************
 Description:
        Close PA while Stop

 Parameters:
        VOID.
	
 Returns:
 		VOID.

 Remarks:
 		
*********************************************************************/
void Vdev_Normal2End(void)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x00);
	Vreg_WriteReg(VDEV_REG_PACFG, 0x4D);
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
}
	

/********************************************************************
 Description:
        Get the current polyphony number when music is playing.

 Parameters:
        VOID.
	
 Returns:
 		the current polyphony number.

 Remarks:
*********************************************************************/
UINT32 Vdev_GetCurPolyphonyNum(void)
{
	UINT8	i,j;
	UINT8	bVal;
	UINT32	dwPolyNum;

	dwPolyNum = 0;

	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);

	for (i=0; i<8; i++)
	{
		bVal = Vreg_ReadReg((UINT8)(VDEV_REG_CHNSTART0+i));
		
		for (j=0; j<8; j++)
		{
			if ((bVal>>j) & 0x01)
				dwPolyNum ++;
		}
	}

	return dwPolyNum;
}

/***************************************************************
 Description:
        Enable stream channels auto stop.
 
 Parameters:
		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
 Returns:
 		VOID.		
 
****************************************************************/
VOID		Vdev_EnableChnStop(UINT8 bFifoType)
{
	UINT8 uVal;
	
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	uVal = Vreg_ReadReg(VDEV_REG_CHN_AUTOSTOPEN);
	uVal |= 0x01 << (bFifoType-VDEV_FIFO_STREAM0);
	Vreg_WriteReg(VDEV_REG_CHN_AUTOSTOPEN, uVal);
}

/***************************************************************
 Description:
        Disable stream channels auto stop.
 
 Parameters:
		bFifoType:	[input] One of the following <U>VDEV_FIFO_TYPE</U>
	                enumeration values that specify the fifo type.
	                <P>VDEV_FIFO_STREAM0
	                <P>VDEV_FIFO_STREAM1
	                <P>VDEV_FIFO_STREAM2
	                <P>VDEV_FIFO_STREAM3
 Returns:
 		VOID.		
 
****************************************************************/
VOID		Vdev_DisableChnStop(UINT8 bFifoType)
{
	UINT8 uVal;
	
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	uVal = Vreg_ReadReg(VDEV_REG_CHN_AUTOSTOPEN);
	uVal &= ~(0x01 << (bFifoType-VDEV_FIFO_STREAM0));
	Vreg_WriteReg(VDEV_REG_CHN_AUTOSTOPEN,  uVal);
}

/***************************************************************
 Description:
        Set interrupt lock flag.
 
 Parameters:
 		None.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void	Vdev_SetIntLock(void)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(VDEV_REG_INTLOCK, 0x01);
}

/***************************************************************
 Description:
        ReSet interrupt lock flag.
 
 Parameters:
 		None.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void	Vdev_ResetIntLock(void)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(VDEV_REG_INTLOCK, 0x00);
}

/***************************************************************
 Description:
        Select V8051 RAM area.
 
 Parameters:
 		None.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void Vdev_SelectV8051RAM(void)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(VDEV_REG_V8051_RAM_SELECT, 0x01);
}

/***************************************************************
 Description:
        UnSelect V8051 RAM area.
 
 Parameters:
 		None.
 		
 Returns:
 		VOID.		
 
****************************************************************/
void Vdev_UnSelectV8051RAM(void)
{
	Vreg_WriteReg(VDEV_REG_PAGE_NUM, 0x01);
	Vreg_WriteReg(VDEV_REG_V8051_RAM_SELECT, 0x00);
}

