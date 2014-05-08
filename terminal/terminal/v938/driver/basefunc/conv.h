/**********************************************************************************/
/*  Name:      conv.h                                                             */
/*  Function:  channel setting                                                    */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/06/13                                                     */
/**********************************************************************************/


#ifndef _RDK_CONV_H_
#define _RDK_CONV_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "../../../v568/tools/type.h"


enum CONV_BUFFLAGenum
{
    CONV_DLYFIFO_EMPTY                = BIT0,
	CONV_DLYFIFO_FULL                 = BIT1,
	CONV_CFIFO_EMPTY                  = BIT2,
	CONV_CFIFO_FULL                   = BIT3,
	CONV_EFIFO_EMPTY                  = BIT4,
	CONV_EFIFO_FULL                   = BIT5,
	CONV_E_HALF_EMPTY                 = BIT6,
	CONV_SFIFO_EMPTY_REVERB           = BIT8,
	CONV_SFIFO_FULL_REVERB            = BIT9
};

enum CONV_CMDSTATUSenum
{
    CONV_CMD_FINISHED                 = BIT0,
	CONV_CMD_FAILED                   = BIT1
};


void V9B_ConvEnSelConv(void);
void V9B_ConvDisSelConv(void);

void V9B_ConvSetBuf1WMark(UINT8 val);
UINT8 V9B_ConvGetBuf1WMark(void);

void V9B_ConvSetBuf1Addr(UINT16 val);
UINT16 V9B_ConvGetBuf1Addr(void);
void V9B_ConvSetBuf2Addr(UINT16 val);
UINT16 V9B_ConvGetBuf2Addr(void);

void V9B_ConvSetEEvtTyp(UINT8 val);
UINT8 V9B_ConvGetEEvtTyp(void);

void V9B_ConvSetCEvtTyp(UINT8 val);
UINT8 V9B_ConvGetCEvtTyp(void);

UINT16 V9B_ConvGetBufFlg(void);

void V9B_ConvSetDlyTim(UINT8 val);
UINT8 V9B_ConvGetDlyTim(void);

void V9B_ConvEnSBack(void);
void V9B_ConvDisSBack(void);
void V9B_ConvEnStream(void);
void V9B_ConvDisStream(void);

void V9B_ConvSetBuf2CmdAddr(UINT16 val);
UINT16 V9B_ConvGetBuf2CmdAddr(void);

void V9B_ConvSetV8051Addr(UINT32 val);
UINT32 V9B_ConvGetV8051Addr(void);

UINT8 V9B_ConvGetCmdSta(void);

UINT32 V9B_ConvGetCmdDat(void);

void V9B_ConvEnStartLoad(void);
void V9B_ConvDisStartLoad(void);


UINT8 V9B_ConvGetLoadDone(void);

UINT16 V9B_ConvGetEFifoRmn(void);

void V9B_ConvSetEfifoEptTT(UINT8 val);
UINT8 V9B_ConvGetEfifoEptTT(void);

void V9B_ConvSetVolGainL(UINT8 val);
UINT8 V9B_ConvGetVolGainL(void);
void V9B_ConvSetVolGainR(UINT8 val);
UINT8 V9B_ConvGetVolGainR(void);


#ifdef __cplusplus
}
#endif

#endif