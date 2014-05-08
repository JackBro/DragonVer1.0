/**********************************************************************************/
/*  Name:      audio.h                                                            */
/*  Function:  audio cotrol                                                       */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/06/13                                                     */
/**********************************************************************************/


#ifndef _RDK_AUDIOCTL_H_
#define _RDK_AUDIOCTL_H_

#ifdef __cplusplus
extern "C" {
#endif


    //SFIFO index
	enum AUDIO_SFIFOINDEXenum
	{
		AUDIO_SFIFO			    = 0x0,
		AUDIO_SFIF1,         	
		AUDIO_SFIF2,      		
		AUDIO_SFIF3				
	};


void V9B_AudioSetAdCtrl(UINT8 val);
UINT8 V9B_AudioGetAdCtrl(void);
void V9B_AudioSetAdpcmCmpRat(UINT8 cmpRat);
void V9B_AudioSetLaw(UINT8 law);

void V9B_AudioSetVolMidL(UINT8 val);
UINT8 V9B_AudioGetVolMidL(void);
void V9B_AudioSetVolMidR(UINT8 val);
UINT8 V9B_AudioGetVolMidR(void);

void V9B_AudioSetVolWavL(UINT8 val);
UINT8 V9B_AudioGetVolWavL(void);
void V9B_AudioSetVolWavR(UINT8 val);
UINT8 V9B_AudioGetVolWavR(void);

void V9B_AudioSetHpnVolL(UINT8 val);
UINT8 V9B_AudioGetHpnVolL(void);
void V9B_AudioSetHpnVolR(UINT8 val);
UINT8 V9B_AudioGetHpnVolR(void);

void V9B_AudioSetDacPgg(UINT8 val);
UINT8 V9B_AudioGetDacPgg(void);

void V9B_AudioSetMutCtrl(UINT8 val);
UINT8 V9B_AudioGetMutCtrl(void);

void V9B_AudioSetEWMark(UINT8 val);
UINT8 V9B_AudioGetEWMark(void);

void V9B_AudioSetDTRetri(UINT8 val);
UINT8 V9B_AudioGetDTRetri(void);

void V9B_AudioSetReg000(UINT8 val);
UINT8 V9B_AudioGetReg000(void);

void V9B_AudioSetReg010(UINT8 val);
UINT8 V9B_AudioGetReg010(void);

void V9B_AudioSetIntpCoef(UINT16 val);
UINT16 V9B_AudioGetIntpCoef(void);

void V9B_AudioSetReg020(UINT8 val);
UINT8 V9B_AudioGetReg020(void);

void V9B_AudioSetECntInit2(UINT8 val);
UINT8 V9B_AudioGetECntInit2(void);

void V9B_AudioSetPreVal(UINT16 val);
UINT16 V9B_AudioGetPreVal(void);

void V9B_AudioSetReg111(UINT8 val);
UINT8 V9B_AudioGetReg111(void);

void V9B_AudioSetEvlpChg20(UINT8 val);
UINT8 V9B_AudioGetEvlpChg20(void);

void V9B_AudioSetLoadAddr(UINT8 val);
UINT8 V9B_AudioGetLoadAddr(void);

void V9B_AudioSetSFifoWm(UINT8 sFifoIdx, UINT8 val);
UINT8 V9B_AudioGetSFifoWm(UINT8 sFifoIdx);

void V9B_AudioSetSFifoAWB(UINT8 sFifoIdx, UINT16 val);
UINT16 V9B_AudioGetSFifoAWB(UINT8 sFifoIdx);
void V9B_AudioSetSFifoAWE(UINT8 sFifoIdx, UINT16 val);
UINT16 V9B_AudioGetSFifoAWE(UINT8 sFifoIdx);

UINT16 V9B_AudioGetSFifoARB(UINT8 sFifoIdx);
UINT16 V9B_AudioGetSFifoARE(UINT8 sFifoIdx);


#ifdef __cplusplus
}
#endif

#endif