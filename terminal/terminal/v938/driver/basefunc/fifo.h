/**********************************************************************************/
/*  Name:      fifo.h                                                             */
/*  Function:  fifo process                                                       */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/06/13                                                     */
/**********************************************************************************/


#ifndef _RDK_FIFO_H_
#define _RDK_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif


void   V9B_FifoWrEFifo(UINT8 *pDat, UINT16 wSize);

void   V9B_FifoWrSFifo(UINT8 *pDat, UINT16 wSize);

void   V9B_FifoWrCFifo(UINT8 *pDat, UINT16 wSize);

void   V9B_FifoSetSBuf0Ful(UINT8 val);
UINT8  V9B_FifoGetSBuf0Ful(void);
void   V9B_FifoSetSBuf1Ful(UINT8 val);
UINT8  V9B_FifoGetSBuf1Ful(void);


#ifdef __cplusplus
}
#endif

#endif