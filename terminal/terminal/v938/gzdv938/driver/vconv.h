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


-------------------------------------------------------------------------*/


#ifndef	__VCONV_H__
#define	__VCONV_H__

#if defined( __cplusplus )
extern "C" {
#endif

#include "vmd.h"

#define MAX_STREAM_VOICE		4		//Hard channel for synthesizing Stream/MP3
#define MAX_MIDI_CHN			16		//MIDI channel 
#define MAX_MIDI_VOICE			60		//Hard channel for synthesizing MIDI/SMAF
#define MAX_MUSIC_VOICE		(MAX_MIDI_VOICE + MAX_STREAM_VOICE)

#define EVENT_BUF_SIZE			256
#define CMD_BUF_SIZE			64
#define MAX_SYN_LAYER			4
#define WE_SAMPLE_RATE			48000

#define EVLP_PEAK_STEP			1 	// 1: high, 2:middle, 3:low
#define PER_NOTEOFF_DISABLE	1 	// percussion channel: 1: sample loop disable, 0:sample loop enable

#define EVLP_R_MOD				1
#define EVLP_S_MOD				3
#define EVLP_D_MOD				1
#define EVLP_A_MOD				0

#define FM_CTRL_STEP			0x02
#define EVLP_D_CHANGE			1	//  1:pReg->EVLPCHG1 = pSynPara->wSLevel
									// 0:pReg->EVLPCHG1 = pSynPara->wSLevel - wEvlpPeak;

// zou
#define SETUP_5_LEN			  	25
#define SETUP_4_LEN				  36
#define GLB_EVENT_LEN		     5
#define MODIFY_EVENT_LEN		13

#define REG_PAG1_CHNSTART		0x80
#define REG_PAG1_CHNSTOP		0x88
#define REG_PAG1_DLYEND			0x98
#define	OPTIMIZE_FOR_51			0

#define REG_PAGE1_TMRTGT_L   0X13
#define REG_PAGE1_STMRTGT    0x17
#define REG_PAGE1_GLBCTRL    0x03
#define REG_PAGE1_INTEN0     0x51
#define REG_PAGE1_SWI_SEL    0x5c
#define REG_PAGE1_VOLWAVR    0xB4
#define REG_PAGE1_VOLWAVL    0xB3
#define REG_PAGE1_VOLMIDIR   0xB2
#define REG_PAGE1_VOLMIDIL   0xB1
#define REG_PAGE1_CHNCTRL  	 0x25 
#define REG_LED_CTRL		0x28
#define REG_MOTOR_CTRL		0x26

#define SAMPLE_RATE					 48000
#define MODU_PRELOAD		     160
#define MAX_MUSIC_CHN	       64
#define VDEV_REG_INT_SWI_SEL 0x5C   


typedef enum _ENUM_VCONV_CONTROL_ID
{
	VCONV_CTRL_MIDI_LED	= 0x00,
	VCONV_CTRL_MIDI_MOTOR	= 0x01
}ENUM_VCONV_CONTROL_ID;

typedef struct _VM_MIDI_CHN
{
	UINT8	bTimbre;
	UINT8	bPan;
	UINT8	bModulation;
	UINT8	bPitchBendH;
	UINT8	bPitchBendL;
	UINT8	bPressure;
	UINT8	bExpression;
	UINT8	bVolume;
	UINT8	bHold1;
	UINT8	bBendRage;
	UINT8	bBankID;
} VM_MIDI_CHN, *PVM_MIDI_CHN;


typedef struct _VM_LED_STATUS
{
	UINT8	bColor;
	UINT8	bLighting;
	UINT8	bHardChn;
} VM_LED_STATUS, *PVM_LED_STATUS;

typedef struct _VM_HARD_CHN
{
	UINT8	bChnNum;
	UINT8	bMidiChn;
	UINT8	bNote;
	UINT8	bVelocity;
	UINT8	bSamplePitch;
	SINT8	sbPitchCorrect;
	UINT8	bChnOn;
	UINT16 	wAttenuation;
	UINT16	wSampleRate;

	struct _VM_HARD_CHN * pNext;
	
} VM_HARD_CHN, *PVM_HARD_CHN;
// zou

typedef struct _VM_BH
{
	UINT32 dBankID;					//dwBankID
	UINT32 dVersion;					//dwVersion
	UINT16 wPresetNum;				//wPresetNum
	UINT16 wInstrumentNum;		//wInstrumentNum
	UINT16 wSampleNum;	      //wSampleNum
	UINT32 dSampleDataLen;	  //dwSampleDataLen
} VM_BH, *PVM_BH;

typedef struct _VM_PT
{
	UINT8  bBankIdx;		      
	UINT8  bSampleNum;
	UINT16 wFirSample;		
} VM_PT, *PVM_PT;

typedef struct _VM_SPH
{
	UINT8 	bLowNote;	
	UINT8 	bHighNote;	
	UINT8 	bSamplePitch;	
	SINT8 	sbPitchCorrect;	

	UINT16 	wStartOffset;	
	UINT16 	wEndOffset;	
	UINT32 	dLoopAddress;	
	UINT16 	wSampleRate;	

	UINT8 	bACounter;	
	UINT8 	bDCounter;	
	UINT8 	bRCounter;	
	UINT16 	wSLevel;	
	UINT16 	wAttenuation;
		
	UINT8 	bBoolCtrl;
	UINT8	  bLfoCounter;
}VM_SPH, *PVM_SPH;

//  zou
typedef struct _VM_BK
{
	VM_BH			*pBH;	  //pBankHead
	VM_PT     *pPT;	  //pPreset
	VM_SPH    *pSPH;	//pSampleHeader
} VM_BK;

typedef struct _VM_CHN_REG
{
	UINT16	CURPTR;
	UINT8	CURSIGN;
	UINT8	LFOCNT;
	UINT8 	EVLPCNT;

	UINT16	FRATIO;
	UINT16	ENDPTR;
	UINT8	EVLPMOD2;

	UINT16	INTPCOEF;
	UINT16	EVLPCHG0;
	UINT16	CUREVLP;
	UINT8	SINVAL;
	UINT8	SINDIR;
	UINT8	LFOSIGN;
	UINT8	EVLPSEG;

	UINT8	RVOL;
	UINT8	CHNVOLEN;
	UINT8	ECNTINIT0;
	UINT8	ECNTINIT1;
	UINT8	ECNTINIT2;

	UINT32	BASEADDR;
	UINT8	SIGNCTRL;
	UINT8	BITCTRL;
	UINT8	LOOPEN;
	UINT8	CHNATRB;
	UINT8	EVLPMOD1;
	UINT8	FMCTRL;
	UINT8	ASTEP;

	UINT8	FSTEP;
	UINT16	PREVAL;
	UINT8	PREINDEX;

	UINT16	EVLPCHG1;
	UINT16	EVLPCHG2;
	UINT8	EVLPMOD0;

	UINT8	CHNVOL_L;
	UINT8	CHNVOL_R;
	UINT8	LFOINIT;
	UINT8	ECNTINIT3;
	
} VM_CHN_REG, *PVM_CHN_REG;

typedef struct _VM_CNVT
{
	VM_MIDI_CHN 		MidiChnPara[MAX_MIDI_CHN];
	
	VM_HARD_CHN		  HardChnPara[MAX_MUSIC_VOICE];
	PVM_HARD_CHN		pFreeChnHdr;
	PVM_HARD_CHN		pFreeChnTal;
	PVM_HARD_CHN		pBusyChnHdr;
	PVM_HARD_CHN		pBusyChnTal;

	VM_SPH        	SynPara[MAX_SYN_LAYER];
	VM_CHN_REG			ChnReg;

	UINT8*	pbEventBuf;
	UINT16	wEventLen;
	
	UINT8*	pbBank;
	UINT8*	pbUDIBank;

	UINT32	dTotalTime;					//The sum of delta time
	UINT32	dMissTime;

	UINT8   bFifoNum;           // added by zou
	UINT32  dMP3SampleRate;
	UINT8   	bLedEnable;
	UINT8	bLedSynChn;
	UINT8	bLedIdx;
	VM_LED_STATUS	LedStatus[3];
	
	UINT8	bMotorEnable;
	UINT8	bMotorSynChn;
	UINT8	bMotorInterval;
	UINT8	bMotorOn;
	UINT8	bMotorHardChn;
	UINT8	bMotorIdx;
}VM_CNVT, *PVM_CNVT;	

typedef struct _VM_FIFOADDR
{
	UINT16  wBaseAddr;		      
	UINT16  wEndPtr;		
} VM_FIFOADDR, *PVM_FIFOADDR;


VRESULT Vcnt_Convert(void *pvMsg,  UINT8* pbOutBuf, UINT16* pbEventLen);
VRESULT Vcnt_ConverterInit(void);
VRESULT Vcnt_ConverterReset(VDEV_FIFO_TYPE FiFoType);
VRESULT Vcnt_LoadBank(UINT8 *pbBank, UINT8 bBankType);

VOID Vcnt_SetFifoNum(UINT8 bNum);
VOID Vcnt_ModifyMP3Fratio(UINT8 bTimes, void *pBuf);
VOID Vcnt_ConverterControl(UINT8 bCtrlID, UINT32 dParam1, UINT32 dParam2, UINT32 dParam3);

#if defined( __cplusplus )
}
#endif

#endif

