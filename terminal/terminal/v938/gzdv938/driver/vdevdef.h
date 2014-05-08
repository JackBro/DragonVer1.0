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


 Module:		Vdevdef.h

 Description:	VC0938 Device Driver register definitions

				Register Allocation
				Type			Location	Description
				global register         
								page 0		asynchonuse registers for CPM
								page 1		midi & global control register
								page 2		mp3 and EQ control register
								page 3		decrypt control register
								page 4		SD card access control register
								page 5		led and motor control register
								page 6-255	reserved

				channel register			in-direct access by   refer to 
											channel register section sequencer 
				 
 Update History:
	1.0			Basic version				2004.07.14		gzd
	1.2			Add support VC0937 chip			2005.01.26                 Andy,liu
-----------------------------------------------------------------------*/

#ifndef __VDEVDEF_H__
#define __VDEVDEF_H__


//////////////////////////////////////////////////////////////////////////////
// Page 0
//
// Description:	Register definition in page 0, all registers in this page are 
// 				asynchronous register to which accessing have no relationship 
// 				with clock.
//
//////////////////////////////////////////////////////////////////////////////

// global control register
#define   VDEV_REG_PAGE_NUM                                    0x00    //R/W

// power and pll control registers
#define   VDEV_REG_PMCTRL                                      0x12    //R/W
#define   VDEV_REG_PACFG                                       0x13    //R/W
#define   VDEV_REG_DACCFG                                      0x14    //R/W  (or DACCON)
#define   VDEV_REG_SPVOL_CTRL                                  0x15    //R/W
#define   VDEV_REG_DACCTRL                                     0x16    //R/W
#define   VDEV_REG_SPVOL                                       0x17    //R/W
#define   VDEV_REG_TEST_O                                      0x18    //R/W
#define   VDEV_REG_STRAP                                       0x19    //RO
#define   VDEV_REG_CINFO_REN_ADD                               0x1A    //R/W
#define   VDEV_REG_CINFO_ADD                                   0x1B    //RO
#define   VDEV_REG_DUMMY_REG0                                  0x1C    //R/W
#define   VDEV_REG_DUMMY_REG1                                  0x1D    //R/W
#define   VDEV_REG_DUMMY_REG2                                  0x1E    //R/W
#define   VDEV_REG_DUMMY_REG3                                  0x1F    //R/W
#define   VDEV_REG_RES_20_23                                   0x20    //20-23 reserved
#define   VDEV_REG_MISC_CON                                    0x24    //R/W
#define   VDEV_REG_I2S_CFG                                     0x25    //R/W
#define   VDEV_REG_FUNC_SEL                                    0x26    //R/W
#define   VDEV_REG_SRD_PORT                                    0x27    //serial read port
#define   VDEV_REG_UMCTRL                                      0x28    //R/W
#define   VDEV_REG_GPIO_CFG                                    0x29    //R/W
#define   VDEV_REG_P0_MODE                                     0x2A    //R/W
#define   VDEV_REG_P0_DIR                                      0x2B    //R/W
#define   VDEV_REG_PORT0                                       0x2C    //R/W
#define   VDEV_REG_D_REN                                       0x2D    //R/W
#define   VDEV_REG_GPIO_REN                                    0x2E    //R/W
#define   VDEV_REG_MISC_TEST                                   0x2F    //R/W
#define   VDEV_REG_CLKCTRL0                                    0x30    //R/W
#define   VDEV_REG_CLKCTRL1                                    0x31    //R/W
#define   VDEV_REG_CLKCTRL2                                    0x32    //R/W
#define   VDEV_REG_CLKCTRL3                                    0x33    //R/W
#define   VDEV_REG_CLKCTRL4                                    0x34    //R/W

#define   VDEV_REG_DATA_OUTPUT_DLYSEL                          0x40    //R/W
#define   VDEV_REG_A0_DLYSEL                                   0x41    //R/W
#define   VDEV_REG_DATA_OEN_DLYSEL                             0x42    //R/W
#define   VDEV_REG_MCLKREF_DLYSEL                              0x43    //R/W
#define   VDEV_REG_MCLKPHASE_DLYSEL                            0x44    //R/W
#define   VDEV_REG_MCLKREF_PHASESEL                            0x45    //R/W
#define   VDEV_REG_DATA_INPUT_DLYSEL                           0x46    //R/W

/////////////////////////////////////////////////////////////////////////////
//Andy,liu add for PA ADC register
//ADC VREF define
#define VDEV_REG_PA_REF_PWD				0x70	//R/W
#define VDEV_REG_PA_SVREF_EN				0x71	//R/W
#define VDEV_REG_PA_ADC_EN					0x73	//R/W
#define VDEV_REG_PA_ADC_CLEAR				0x74	//R/W

#define VDEV_REG_PA_ADC_CLK				0x75	//R/W
#define VDEV_REG_PA_ADC_SET				0x76	//R/W
#define VDEV_REG_PA_SVREF_D				0x77	//R/W
#define VDEV_REG_PA_MCLK_READY				0x80	//R/W

#define VDEV_REG_PA_TW_TGT					0x90	//R/W	
#define VDEV_REG_PA_TE_TGT					0x91	//R/W	
#define VDEV_REG_PA_TP_TGT					0x92	//R/W	
#define VDEV_REG_PA_TT_TGT					0x93	//R/W	

#define VDEV_REG_PA_ADC_MODE				0x1C	//R/W  
#define VDEV_REG_PA_DELAY					0x1D	//R/W
#define VDEV_REG_PA_AVERAGE_NUMBER				0x1E	//R/W
#define VDEV_REG_PA_SLOPE					0x1F	//R/W

//ADC VREF MODE
#define PA_ADC_POWER_ON     0
#define PA_ADC_POWER_OFF    1
#define PA_ADC_PLAY_BACK    2
#define PA_ADC_SET_BY_MSG   3
#define PA_ADC_WAVE_FORM    4
#define PA_ADC_VREF_RESET   5
#define PA_ADC_DEBUG        6
#define PA_ADC_RESERVE      7

#define PA_CLK_REPEAT       7

//The inner extern interrupt
#define PA_EXTERN_INTERRUPT 0x5E
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Page 1
//
// Description:	all registers define in this page are for global registers
//
//////////////////////////////////////////////////////////////////////////////

//global control registers
#define   VDEV_REG_CHIP_VER_L                                  0x01    //RO
#define   VDEV_REG_CHIP_VER_H                                  0x02    //RO

/*--------------------------------------------------------------------------
Bit number	7		6		5		4		3		2		1		0
Bit name	Seq		TMR		Pause	Stereo	SRate	FM		AM		STMR
[7]:  Start Sequencer
[6]:  Start TMR Timer(Timer1).
[5]:  Pause or resume the Wavetable synthesizer. 
		When written:
			0: resume
			1: pause
		When read:
			0: W.S. is not paused.
			1: W.S. is paused already.
[4]:  Output mono / stereo control:
		1: mono output
		0: stereo output
[3]: Sample Rate Conversion disabled 
		1: disabled
		0: enabled
[2]: FM disabled for all channels.
		1: disabled
		0: enabled
[1]:  AM disabled for all channels.
		1: disabled
		0: enabled
[0]:  Start STMR timer(Timer0).
--------------------------------------------------------------------------*/
#define   VDEV_REG_GLBCTRL                                     0x03    //R/W

#define   VDEV_GLBCTRL_START_SEQ                               0x80
#define   VDEV_GLBCTRL_START_TMR1                              0x40
#define   VDEV_GLBCTRL_WS_PAUSE                                0x20
#define   VDEV_GLBCTRL_MONO_OUT                                0x10
#define   VDEV_GLBCTRL_DIS_SRCON                               0x08
#define   VDEV_GLBCTRL_DIS_FM                                  0x04
#define   VDEV_GLBCTRL_DIS_AM                                  0x02
#define   VDEV_GLBCTRL_START_TMR0                              0x01

#define   VDEV_REG_STOPALL                                     0x04    //RO
#define   VDEV_REG_WS_MISC                                     0x05    //R/W
#define   VDEV_REG_GLBCFG                                      0x06    //R/W

//the registers related with C51 control in vc0938 chip
#define   VDEV_REG_SEL_CONV                                    0x07    //R/W
#define   VDEV_REG_BUF1_W_MARK                                 0x08    //R/W
#define   VDEV_REG_BUF2_ADDR_L                                 0x09    //R/W
#define   VDEV_REG_BUF2_ADDR_H                                 0x0A    //R/W
#define   VDEV_REG_BUF1_ADDR_L                                 0x0B    //R/W
#define   VDEV_REG_BUF1_ADDR_H                                 0x0C    //R/W


#define   VDEV_REG_E_EVENT_TYPE                                0x0D    //R/W
#define   VDEV_REG_C_EVENT_TYPE                                0x0E    //R/W
#define   VDEV_REG_RES_0F                                      0x0F    
#define   VDEV_REG_TMR_L                                       0x10    //RO
#define   VDEV_REG_TMR_M                                       0x11    //RO
#define   VDEV_REG_TMR_H                                       0x12    //RO
#define   VDEV_REG_TMRTGT_L                                    0x13    //R/W
#define   VDEV_REG_TMRTGT_M                                    0x14    //R/W
#define   VDEV_REG_TMRTGT_H                                    0x15    //R/W
#define   VDEV_REG_STMR                                        0x16    //RO
#define   VDEV_REG_STMRTGT                                     0x17    //R/W
#define   VDEV_REG_TMRBASE_L                                   0x18    //R/W
#define   VDEV_REG_TMRBASE_H                                   0x19    //R/W
#define   VDEV_REG_FRATIO_DLT0                                 0x1A    //R/W
#define   VDEV_REG_FRATIO_DLT1                                 0x1B    //R/W
#define   VDEV_REG_FRATIO_DLT2                                 0x1C    //R/W
#define   VDEV_REG_FRATIO_DLT3                                 0x1D    //R/W
#define   VDEV_REG_ANAERRFLAG                                  0x1E    //R/W
#define   VDEV_REG_ANAERRMASK                                  0x1F    //R/W
#define   VDEV_REG_ERRORFLAG                                   0x20    //R/W
#define   VDEV_REG_ERRORMASK                                   0x21    //R/W
#define   VDEV_REG_BUFFLAG0                                    0x22    //RO
#define   VDEV_REG_BUFFLAG1                                    0x23    //RO
#define   VDEV_REG_DLYTIME                                     0x24    //R/W
#define   VDEV_REG_CHNCTRL                                     0x25    //R/W
#define   VDEV_REG_26_28                                       0x26    //26-28 reserved

#define   VDEV_REG_BUG2_CMD_ADDR_L                             0x29    //R/W
#define   VDEV_REG_BUF2_CMD_ADDR_H                             0x2A    //R/W
#define   VDEV_REG_2B_2C                                       0x2B    //2B-2C reserved
#define   VDEV_REG_V8051_ADDR_HH                               0x2D    //R/W
#define   VDEV_REG_V8051_ADDR_L                                0x2E    //R/W
#define   VDEV_REG_V8051_ADDR_H                                0x2F    //R/W

#define   VDEV_REG_CMDSTATUS                                   0x30    //RC read to clear
#define   VDEV_REG_CMDDATA0                                    0x31    //RO
#define   VDEV_REG_CMDDATA1                                    0x32    //RO
#define   VDEV_REG_CMDDATA2                                    0x33    //RO
#define   VDEV_REG_CMDDATA3                                    0x34    //RO
#define   VDEV_REG_STARTLOAD                                   0x35    //R/W
#define   VDEV_REG_LOADDONE                                    0x36    //RO
#define   VDEV_REG_EFIFO_RMN_L                                 0x37    //RO
#define   VDEV_REG_EFIFO_RMN_H                                 0x38    //RO
#define   VDEV_REG_EFIFO_EPT_TT                                0x39    //R/W
#define   VDEV_REG_VOL_GAIN_L                                  0x3A    //R/W
#define   VDEV_REG_VOL_GAIN_R                                  0x3B    //R/W
#define   VDEV_REG_RES_3C_3F                                   0x3C    //3C-3F reserved


/*--------------------------------------------------------------------------
Bit number	7		6		5		4		3		2		1		0
Bit name	SWRst	EfClr	res		WSRst	SeqRest	EQRst	TMR1Rst	TMR0Rst

SW reset for each block. Auto clear by HW.
	[7]:		Global SW reset
	[6]:		EFIFO clear
	[5]:		reserved
	[4]:		WS reset
	[3]:		Sequencer reset
	[2]:		eq reset
	[1]:		TMR timer(Timer1) reset.
	[0]:		STMR timer(Time0) reset
--------------------------------------------------------------------------*/
#define   VDEV_REG_SWRST1                                      0x40    //WO

/*--------------------------------------------------------------------------
Bit number	7		6		5		4		3		2		1		0
Bit name	LEDRst	I2SRst	SysRst	IntRst	MClkRst	CFRst	VibRst	Mp3Rst

SW reset for each block. Auto clear by HW.
	[7]:		LED reset
	[6]:		I2S sw reset
	[5]:		System reset
	[4]:		int reset
	[3]:		BIU MCLK domain reset
	[2]:		Command FIFO reset
	[1]:		Vibrator reset
	[0]:		MP3 reset
--------------------------------------------------------------------------*/
#define   VDEV_REG_SWRST2                                      0x41    //WO

/*--------------------------------------------------------------------------
Bit number	7		6		5		4		3		2		1		0
Bit name	res		res		res		res		SF3CLR	SF2CLR	SF1CLR	SF0CLR

	[7:4]		Reserved
	[3]:		SFIFO3 clear;
	[2]:		SFIFO2 clear;
	[1]:		SFIFO1 clear;
	[0]:		SFIFO0 clear; 
--------------------------------------------------------------------------*/
#define   VDEV_REG_SWRST3	                                   0x42    //WO


#define   VDEV_REG_RES_43_4D                                   0x43    //43-4D RESERVED
#define   VDEV_REG_INTERNAL_VER_L                              0x4E    //R/W
#define   VDEV_REG_INTERNAL_VER_H                              0x4F    //R/W

//Interrupt control Registers
#define   VDEV_REG_INTCTRL                                     0x50    //R/W
#define   VDEV_REG_INTEN0                                      0x51    //R/W
#define   VDEV_REG_INTFLAG0                                    0x52    //R/W read to clear or write to clear	
#define   VDEV_REG_INTEN1                                      0x53    //R/W
#define   VDEV_REG_INTFLAG1                                    0x54    //R/W
#define   VDEV_REG_INTLOCK                                     0x55    //R/W

/*--------------------------------------------------------------------------
Bit number	7	6	5	4	3	2	1	0
Bit name	sf0 sf1 sf2 sf3	mp3 res	Evt res

When corresponding bit is set, host can start to processing the corresponding 
interrupt in showed in INTFLAG register, the HW internal status is in-active 
that is to say no new interrupt can be create.
[7:4]:		sfifo int serve
[3]:		mp3 int serve
[1]:		Event FIFO int serve
[0]:		Res.
--------------------------------------------------------------------------*/
#define   VDEV_REG_INTSERV                                     0x56    //R/W

#define	  VDEV_INTSERV_RESET                                   0x00
#define	  VDEV_INTSERV_EVENT                                   0x02
#define	  VDEV_INTSERV_MP3                                     0x08
#define	  VDEV_INTSERV_STREAM0                                 0x10
#define	  VDEV_INTSERV_STREAM1                                 0x20
#define	  VDEV_INTSERV_STREAM2                                 0x40
#define	  VDEV_INTSERV_STREAM3                                 0x80


#define   VDEV_REG_INTEN2	                                   0x57    //R/W
#define   VDEV_REG_INTFLAG2									   0x58	   //R/W
#define   VDEV_REG_INT_DLY_CNT0                                0x59    //R/W
#define   VDEV_REG_INT_DLY_CNT1                                0x5A    //R/W
#define   VDEV_REG_INT_DLY_SEL                                 0x5B    //R/W
#define   VDEV_REG_INT_SWI_SEL                                 0x5C    //R/W
#define   VDEV_REG_CHN_AUTOSTOPEN							   0x5D    //R/W
#define   VDEV_REG_RES_5E_5F                                   0x5E    //5E-5F reserved
#define   VDEV_REG_INT_REPEAT_CNT0                             0x60    //R/W
#define   VDEV_REG_INT_REPEAT_CNT1                             0x61    //R/W
#define   VDEV_REG_INT_GLB_MOD_SEL                             0x62    //R/W
#define   VDEV_REG_INTFLAG0_WR2CLR                             0x63    //R/W
#define   VDEV_REG_INTFLAG1_WR2CLR                             0x64    //R/W
#define   VDEV_REG_INT_OTHER_WR2CLR                            0x65    //R/W
#define   VDEV_REG_RES_66_6F                                   0x66    //66-6F reserved except of 68
#define   VDEV_REG_STREAM_CLR_BYPASS                           0x68    //R/W

//FIFO write Registers
#define   VDEV_REG_EFIFO                                       0x70    //WO
#define   VDEV_REG_SFIFO                                       0x71    //WO
#define   VDEV_REG_RES_72_75                                   0x72    //72-75 reserved
#define   VDEV_REG_CFIFO                                       0x76    //WO
#define   VDEV_REG_RES_77_78                                   0x77    //77-78 reserved
#define	  VDEV_REG_V8051RAM									   0x79	   //WO
#define   VDEV_REG_RES_7A                                      0x7A    //7A reserved
#define   VDEV_REG_SBUF0_FULL                                  0x7B    //R/W
#define   VDEV_REG_SBUF1_FULL                                  0x7C    //R/W

//Channel control register
#define   VDEV_REG_CHNSTART0                                   0x80    //R/W
#define   VDEV_REG_CHNSTART1                                   0x81    //R/W
#define   VDEV_REG_CHNSTART2                                   0x82    //R/W
#define   VDEV_REG_CHNSTART3                                   0x83    //R/W
#define   VDEV_REG_CHNSTART4                                   0x84    //R/W
#define   VDEV_REG_CHNSTART5                                   0x85    //R/W
#define   VDEV_REG_CHNSTART6                                   0x86    //R/W
#define   VDEV_REG_CHNSTART7                                   0x87    //R/W
#define   VDEV_REG_CHNSTOP0                                    0x88    //R/W
#define   VDEV_REG_CHNSTOP1                                    0x89    //R/W
#define   VDEV_REG_CHNSTOP2                                    0x8A    //R/W
#define   VDEV_REG_CHNSTOP3                                    0x8B    //R/W
#define   VDEV_REG_CHNSTOP4                                    0x8C    //R/W
#define   VDEV_REG_CHNSTOP5                                    0x8D    //R/W
#define   VDEV_REG_CHNSTOP6                                    0x8E    //R/W
#define   VDEV_REG_CHNSTOP7                                    0x8F    //R/W
#define   VDEV_REG_CHNDLY0                                     0x90    //RO
#define   VDEV_REG_CHNDLY1                                     0x91    //RO
#define   VDEV_REG_CHNDLY2                                     0x92    //RO
#define   VDEV_REG_CHNDLY3                                     0x93    //RO
#define   VDEV_REG_CHNDLY4                                     0x94    //RO
#define   VDEV_REG_CHNDLY5                                     0x95    //RO
#define   VDEV_REG_CHNDLY6                                     0x96    //RO
#define   VDEV_REG_CHNDLY7                                     0x97    //RO
#define   VDEV_REG_DLYEND0                                     0x98    //WO
#define   VDEV_REG_DLYEND1                                     0x99    //WO
#define   VDEV_REG_DLYEND2                                     0x9A    //WO
#define   VDEV_REG_DLYEND3                                     0x9B    //WO
#define   VDEV_REG_DLYEND4                                     0x9C    //WO
#define   VDEV_REG_DLYEND5                                     0x9D    //WO
#define   VDEV_REG_DLYEND6                                     0x9E    //WO
#define   VDEV_REG_DLYEND7                                     0x9F    //WO
#define   VDEV_REG_RES_A0_A9                                   0xA0    //A0-A9 reserved

#define   VDEV_REG_OP_MODE                                     0xAA    //R/W
#define	  VDEV_REG_V8051_RAM_SELECT							   0xAB	   //R/W

//Audio control registers
#define   VDEV_REG_ADCTRL                                      0xB0    //R/W
#define   VDEV_REG_VOLMID_L                                    0xB1    //R/W
#define   VDEV_REG_VOLMID_R                                    0xB2    //R/W
#define   VDEV_REG_VOLWAV_L                                    0xB3    //R/W
#define   VDEV_REG_VOLWAV_R                                    0xB4    //R/W
#define   VDEV_REG_RES_B5                                      0xB5    //R/W
#define   VDEV_REG_HPNVOL_L                                    0xB6    //R/W  //DACGOL
#define   VDEV_REG_HPNVOL_R                                    0xB7    //R/W //DACGOR
#define   VDEV_REG_DACPGG                                      0xB8    //R/W //DACGOM
#define   VDEV_REG_MUTCTRL                                     0xB9    //R/W
#define   VDEV_REG_E_W_MARK                                    0xBA    //R/W
#define   VDEV_REG_DT_RETRI                                    0xBB    //R/W
#define   VDEV_REG_RES_BC_BF                                   0xBC    //BC-BF reserved

#define   VDEV_REG_REG000                                      0xC0    //R/W
#define   VDEV_REG_RES_C1_C2                                   0xC1    //C1-C2 reserved
#define   VDEV_REG_REG010                                      0xC3    //R/W
#define   VDEV_REG_INTPCOEF0                                   0xC4    //R/W
#define   VDEV_REG_INTPCOEF1                                   0xC5    //R/W
#define   VDEV_REG_REG020                                      0xC6    //R/W
#define   VDEV_REG_RES_C7                                      0xC7    //C7 reserved
#define   VDEV_REG_ECNTINIT2                                   0xC8    //R/W
#define   VDEV_REG_RES_C9_CA                                   0xC9    //C9-CA reserved
#define   VDEV_REG_PREVAL0                                     0xCB    //R/W
#define   VDEV_REG_PREVAL1                                     0xCC    //R/W
#define   VDEV_REG_REG111                                      0xCD    //R/W
#define   VDEV_REG_EVLPCHG20                                   0xCE    //R/W
#define   VDEV_REG_RES_CF                                      0xCF    //CF reserved

#define   VDEV_REG_RES_D0_D5                                   0xD0    //D0-D5 reserved
#define   VDEV_REG_LOADADDR                                    0xD6    //R/W
#define   VDEV_REG_RES_D7_DB                                   0xD7    //D7-DB reserved
#define   VDEV_REG_SFIFO0_WM                                   0xDC    //R/W
#define   VDEV_REG_SFIFO1_WM                                   0xDD    //R/W
#define   VDEV_REG_SFIFO2_WM                                   0xDE    //R/W
#define   VDEV_REG_SFIFO3_WM                                   0xDF    //R/W

#define   VDEV_REG_SFIFO0_AW_BL                                0xE0    //R/W
#define   VDEV_REG_SFIFO0_AW_BH                                0xE1    //R/W
#define   VDEV_REG_SFIFO0_AW_EL                                0xE2    //R/W
#define   VDEV_REG_SFIFO0_AW_EH                                0xE3    //R/W
#define   VDEV_REG_SFIFO1_AW_BL                                0xE4    //R/W
#define   VDEV_REG_SFIFO1_AW_BH                                0xE5    //R/W
#define   VDEV_REG_SFIFO1_AW_EL                                0xE6    //R/W
#define   VDEV_REG_SFIFO1_AW_EH                                0xE7    //R/W
#define   VDEV_REG_SFIFO2_AW_BL                                0xE8    //R/W
#define   VDEV_REG_SFIFO2_AW_BH                                0xE9    //R/W
#define   VDEV_REG_SFIFO2_AW_EL                                0xEA    //R/W
#define   VDEV_REG_SFIFO2_AW_EH                                0xEB    //R/W
#define   VDEV_REG_SFIFO3_AW_BL                                0xEC    //R/W
#define   VDEV_REG_SFIFO3_AW_BH                                0xED    //R/W
#define   VDEV_REG_SFIFO3_AW_EL                                0xEE    //R/W
#define   VDEV_REG_SFIFO3_AW_EH                                0xEF    //R/W

#define   VDEV_REG_SFIFO0_AR_BL                                0xF0    //R/W
#define   VDEV_REG_SFIFO0_AR_BH                                0xF1    //R/W
#define   VDEV_REG_SFIFO0_AR_EL                                0xF2    //R/W
#define   VDEV_REG_SFIFO0_AR_EH                                0xF3    //R/W
#define   VDEV_REG_SFIFO1_AR_BL                                0xF4    //R/W
#define   VDEV_REG_SFIFO1_AR_BH                                0xF5    //R/W
#define   VDEV_REG_SFIFO1_AR_EL                                0xF6    //R/W
#define   VDEV_REG_SFIFO1_AR_EH                                0xF7    //R/W
#define   VDEV_REG_SFIFO2_AR_BL                                0xF8    //R/W
#define   VDEV_REG_SFIFO2_AR_BH                                0xF9    //R/W
#define   VDEV_REG_SFIFO2_AR_EL                                0xFA    //R/W
#define   VDEV_REG_SFIFO2_AR_EH                                0xFB    //R/W
#define   VDEV_REG_SFIFO3_AR_BL                                0xFC    //R/W
#define   VDEV_REG_SFIFO3_AR_BH                                0xFD    //R/W
#define   VDEV_REG_SFIFO3_AR_EL                                0xFE    //R/W
#define   VDEV_REG_SFIFO3_AR_EH                                0xFF    //R/W

//////////////////////////////////////////////////////////////////////////////
// Page 2
//
// Description:	MP3 & EQ  registers are defined in this page.
//
//////////////////////////////////////////////////////////////////////////////

//MP3 global control registers
#define   VDEV_REG_MP3_CLR_A                                   0x0F    //WO
#define   VDEV_REG_MP3_RD_PTR_L                                0x10    //RO
#define   VDEV_REG_MP3_RD_PTR_H                                0x11    //RO
#define   VDEV_REG_MP3_WR_PTR_L                                0x12    //RO
#define   VDEV_REG_MP3_WR_PTR_H                                0x13    //RO
#define   VDEV_REG_MP3_W_MARK                                  0x14    //R/W
#define   VDEV_REG_MP3_DONE                                    0x15    //R/W
#define   VDEV_REG_MP3_PARA0                                   0x16    //R/W
#define   VDEV_REG_MP3_PARA1                                   0x17    //R/W
#define   VDEV_REG_MP3_PARA2                                   0x18    //R/W
#define   VDEV_REG_RES_19_1F                                   0x19    //19-1F reserved

#define   VDEV_REG_MP3_EQBAND0                                 0x20    //R/W
#define   VDEV_REG_MP3_EQBAND1                                 0x21    //R/W
#define   VDEV_REG_MP3_EQBAND2                                 0x22    //R/W
#define   VDEV_REG_MP3_EQBAND3                                 0x23    //R/W
#define   VDEV_REG_MP3_EQBAND4                                 0x24    //R/W
#define   VDEV_REG_MP3_EQBAND5                                 0x25    //R/W
#define   VDEV_REG_MP3_EQBAND6                                 0x26    //R/W
#define   VDEV_REG_MP3_EQBAND7                                 0x27    //R/W
#define   VDEV_REG_MP3_EQBAND8                                 0x28    //R/W
#define   VDEV_REG_MP3_EQBAND9                                 0x29    //R/W
#define   VDEV_REG_MP3_VOLSTEP                                 0x2A    //R/W

#define   VDEV_REG_MP3_FIFO                                    0x30    //R/W

#define   VDEV_REG_EQ_BAND0                                    0x40    //R/W
#define   VDEV_REG_EQ_BAND1                                    0x41    //R/W
#define   VDEV_REG_EQ_BAND2                                    0x42    //R/W
#define   VDEV_REG_EQ_BAND3                                    0x43    //R/W
#define   VDEV_REG_EQ_BAND4                                    0x44    //R/W
#define   VDEV_REG_EQ_BAND5                                    0x45    //R/W

#define   VDEV_REG_SAMP_RATE                                   0x50    //R/W
#define   VDEV_REG_VOLSTEP                                     0x51    //R/W
#define   VDEV_REG_OUT_SEL                                     0x52    //R/W

//////////////////////////////////////////////////////////////////////////////
// Page 3
//
// Description:	Decrypt control  registers for DRM are defined in this page.
//
//////////////////////////////////////////////////////////////////////////////

#define   VDEV_REG_AES_CORE_CMD                                0x01    //R/W
#define   VDEV_REG_AES_MP3_STRB_DLY                            0x02    //WO
#define   VDEV_REG_AES_KEY0                                    0x10    //R/W
#define   VDEV_REG_AES_KEY1                                    0x11    //R/W
#define   VDEV_REG_AES_KEY2                                    0x12    //R/W
#define   VDEV_REG_AES_KEY3                                    0x13    //R/W
#define   VDEV_REG_AES_KEY4                                    0x14    //R/W
#define   VDEV_REG_AES_KEY5                                    0x15    //R/W
#define   VDEV_REG_AES_KEY6                                    0x16    //R/W
#define   VDEV_REG_AES_KEY7                                    0x17    //R/W
#define   VDEV_REG_AES_KEY8                                    0x18    //R/W
#define   VDEV_REG_AES_KEY9                                    0x19    //R/W
#define   VDEV_REG_AES_KEY10                                   0x1A    //R/W
#define   VDEV_REG_AES_KEY11                                   0x1B    //R/W
#define   VDEV_REG_AES_KEY12                                   0x1C    //R/W
#define   VDEV_REG_AES_KEY13                                   0x1D    //R/W
#define   VDEV_REG_AES_KEY14                                   0x1E    //R/W
#define   VDEV_REG_AES_KEY15                                   0x1F    //R/W

#define   VDEV_REG_AES_INITVEC0                                0x20    //R/W
#define   VDEV_REG_AES_INITVEC1                                0x21    //R/W
#define   VDEV_REG_AES_INITVEC2                                0x22    //R/W
#define   VDEV_REG_AES_INITVEC3                                0x23    //R/W
#define   VDEV_REG_AES_INITVEC4                                0x24    //R/W
#define   VDEV_REG_AES_INITVEC5                                0x25    //R/W
#define   VDEV_REG_AES_INITVEC6                                0x26    //R/W
#define   VDEV_REG_AES_INITVEC7                                0x27    //R/W
#define   VDEV_REG_AES_INITVEC8                                0x28    //R/W
#define   VDEV_REG_AES_INITVEC9                                0x29    //R/W
#define   VDEV_REG_AES_INITVEC10                               0x2A    //R/W
#define   VDEV_REG_AES_INITVEC11                               0x2B    //R/W
#define   VDEV_REG_AES_INITVEC12                               0x2C    //R/W
#define   VDEV_REG_AES_INITVEC13                               0x2D    //R/W
#define   VDEV_REG_AES_INITVEC14                               0x2E    //R/W
#define   VDEV_REG_AES_INITVEC15                               0x2F    //R/W


//////////////////////////////////////////////////////////////////////////////
// Page 4
//
// Description:	SD access control  registers are defined in this page.
//
//////////////////////////////////////////////////////////////////////////////

#define   VDEV_REG_SD_CMD0_FIFO                                0xF0    //R/W
#define   VDEV_REG_SD_CMD1_FIFO                                0xF1    //R/W
#define   VDEV_REG_SD_DATA_FIFO                                0xF2    //R/W


//////////////////////////////////////////////////////////////////////////////
// Page 5
//
// Description:	LED and MOTOR access control  registers are defined in this page.
//
//////////////////////////////////////////////////////////////////////////////

#define   VDEV_REG_TIMESLICE_HIGH                              0x01    //R/W
#define   VDEV_REG_TIMESLICE_LOW                               0x02    //R/W
#define   VDEV_REG_PWM_RLED_HIGH                               0x03    //R/W
#define   VDEV_REG_PWM_RLED_LOW                                0x04    //R/W
#define   VDEV_REG_PWM_BLED_HIGH                               0x05    //R/W
#define   VDEV_REG_PWM_BLED_LOW                                0x06    //R/W
#define   VDEV_REG_PWM_GLED_HIGH                               0x07    //R/W
#define   VDEV_REG_PWM_GLED_LOW                                0x08    //R/W
#define   VDEV_REG_PWM_MOT_HIGH                                0x09    //R/W
#define   VDEV_REG_PWM_MOT_LOW                                 0x0A    //R/W
#define   VDEV_REG_LEDMOT_CONF                                 0x0B    //R/W
#define   VDEV_REG_CIRCLING_PERIOD                             0x0C    //R/W
#define   VDEV_REG_LEDMOT_INDEX                                0x0D    //R/W
#define   VDEV_REG_PATTERN0_HIGH                               0x0E    //R/W
#define   VDEV_REG_PATTERN0_LOW                                0x0F    //R/W
#define   VDEV_REG_PATTERN1_HIGH                               0x11    //R/W
#define   VDEV_REG_PATTERN1_LOW                                0x12    //R/W
#define   VDEV_REG_PATTERN2_HIGH                               0x13    //R/W
#define   VDEV_REG_PATTERN2_LOW                                0x14    //R/W
#define   VDEV_REG_PATTERN3_HIGH                               0x15    //R/W
#define   VDEV_REG_PATTERN3_LOW                                0x16    //R/W
#define   VDEV_REG_PATTERN4_HIGH                               0x17    //R/W
#define   VDEV_REG_PATTERN4_LOW                                0x18    //R/W
#define   VDEV_REG_PATTERN5_HIGH                               0x19    //R/W
#define   VDEV_REG_PATTERN5_LOW                                0x1A    //R/W
#define   VDEV_REG_PATTERN6_HIGH                               0x1B    //R/W
#define   VDEV_REG_PATTERN6_LOW                                0x1C    //R/W
#define   VDEV_REG_PATTERN7_HIGH                               0x1D    //R/W
#define   VDEV_REG_PATTERN7_LOW                                0x1E    //R/W
#define   VDEV_REG_RES_1F_25                                   0x1F    //1F-25 reserved
#define   VDEV_REG_MOTOR_CTRL                                  0x26    //R/W
#define   VDEV_REG_MOTOR_SYNC                                  0x27    //R/W
#define   VDEV_REG_LED_CTRL1                                   0x28    //R/W
#define   VDEV_REG_LED_CTRL2                                   0x29    //R/W
#define   VDEV_REG_LED_CTRL3                                   0x2A    //R/W
#define   VDEV_REG_LED_SYNC1                                   0x2B    //R/W
#define   VDEV_REG_LED_SYNC2                                   0x2C    //R/W
#define   VDEV_REG_LED_SYNC3                                   0x2D    //R/W


#endif /*__VDEVDEF_H__*/

