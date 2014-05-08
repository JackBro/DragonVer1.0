/**********************************************************************************/
/*  Name:      biu938.h                                                           */
/*  Function:  Biu control of vc0938                                              */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/04/12                                                     */
/**********************************************************************************/


#ifndef _RDK_BIU938_H_
#define _RDK_BIU938_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef void   SetRegFunc938(UINT32 adr, UINT32 val, UINT8 regWType);
typedef UINT32 GetRegFunc938(UINT32 adr, UINT8 regWType);
typedef UINT32 ReadSramFunc938(UINT32 adr, UINT8 *buf, UINT32 size);
typedef UINT32 WriteSramFunc938(UINT32 adr, UINT8 *buf, UINT32 size);

// Biu module context
typedef struct tag_TBiuContext938
{
	UINT8                         busType; // Bus type, parrell, serial or SPI
	SetRegFunc938*                pSetRegFunc; // Set register function
	GetRegFunc938*                pGetRegFunc; // Get register function
	ReadSramFunc938*              pReadSramFunc938;//Read sram function ---fpga version can read
	WriteSramFunc938*             pWriteSramFunc; //Write sram function
} TBiuContext938, *PTBiuContext938;

extern TBiuContext938 gBiuContext938;

void   V9B_BiuSetDatDlySel(UINT8 val);
UINT8  V9B_BiuGetDatDlySel(void);
void   V9B_BiuSetA0DlySel(UINT8 val);
UINT8  V9B_BiuGetA0DlySel(void);
void   V9B_BiuSetRefDlySel(UINT8 val);
UINT8  V9B_BiuGetRefDlySel(void);
void   V9B_BiuSetPhaDlySel(UINT8 val);
UINT8  V9B_BiuGetPhaDlySel(void);
void   V9B_BiuSetRefPhaSel(UINT8 val);
UINT8  V9B_BiuGetRegPhaSel(void);
void   V9B_BiuSetDatInDlySel(UINT8 val);
UINT8  V9B_BiuGetDatInDlySel(void);

void   V9_InitBusType(UINT8 type);
void   V9B_SetBusType(UINT8 type);

void   V9_SetReg(UINT32 adr, UINT32 val, UINT8 regWType);
UINT32 V9_GetReg(UINT32 adr, UINT8 regWType);
UINT32 V9_ReadSram(UINT32 adr, UINT8 *buf, UINT32 size);
UINT32 V9_WriteSram(UINT32 adr, UINT8 *buf, UINT32 size);
void   V9_Delay(UINT32 count);

UINT8 V9B_SetRegMaskBit8(UINT32 adr, UINT8 mask, UINT8 op);

#ifdef __cplusplus
}
#endif

#endif //_RDK_BIU938_H_

