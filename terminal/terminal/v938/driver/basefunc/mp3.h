/**********************************************************************************/
/*  Name:      mp3.h                                                              */
/*  Function:  Control MP3 of vc0938                                              */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/04/12                                                     */
/**********************************************************************************/


#ifndef _RDK_MP3_H_
#define _RDK_MP3_H_

#ifdef __cplusplus
extern "C" {
#endif


void   V9B_Mp3SetClrA(UINT8 val);
UINT16 V9B_Mp3GetRdPtr(void);

UINT16 V9B_Mp3GetWrPtr(void);

void   V9B_Mp3SetWMark(UINT8 val);
UINT8  V9B_Mp3GetWMark(void);

void   V9B_Mp3SetDatDone(UINT8 val);
UINT8  V9B_Mp3GetDatDone(void);

UINT32 V9B_Mp3GetPara(void);

void  V9B_Mp3SetEqBand(UINT8 *pVal);
void V9B_Mp3GetEqBand(UINT8 *pVal);

void   V9B_Mp3SetVolStep(UINT8 val);
UINT8  V9B_Mp3GetVolStep(void);

void  V9B_Mp3WrMFifo(UINT8 *pDat, UINT16 wSize);


#ifdef __cplusplus
}
#endif

#endif