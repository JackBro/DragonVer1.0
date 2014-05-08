/**********************************************************************************/
/*  Name:      eq.h                                                               */
/*  Function:  Control EQ of vc0938                                               */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/04/12                                                     */
/**********************************************************************************/


#ifndef _RDK_EQ_H_
#define _RDK_EQ_H_

#ifdef __cplusplus
extern "C" {
#endif


void  V9B_EqSetBand0(UINT8 val);
UINT8 V9B_EqGetBand0(void);
void  V9B_EqSetBand1(UINT8 val);
UINT8 V9B_EqGetBand1(void);
void  V9B_EqSetBand2(UINT8 val);
UINT8 V9B_EqGetBand2(void);
void  V9B_EqSetBand3(UINT8 val);
UINT8 V9B_EqGetBand3(void);
void  V9B_EqSetBand4(UINT8 val);
UINT8 V9B_EqGetBand4(void);
void  V9B_EqSetBand5(UINT8 val);
UINT8 V9B_EqGetBand5(void);

void  V9B_EqSetSampRat(UINT8 val);
UINT8 V9B_EqGetSampRat(void);

void  V9B_EqSetVolStep(UINT8 val);
UINT8 V9B_EqGetVolStep(void);

void  V9B_EqSetOutSel(UINT8 val);
UINT8 V9B_EqGetOutSel(void);
void  V9B_EqSetEqMod(void);
void  V9B_EqSetBypassMod(void);


#ifdef __cplusplus
}
#endif

#endif

