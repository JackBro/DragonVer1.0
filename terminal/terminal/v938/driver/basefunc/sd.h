/**********************************************************************************/
/*  Name:      sd.h                                                               */
/*  Function:  Control SD card access of vc0938                                   */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/04/12                                                     */
/**********************************************************************************/


#ifndef _RDK_SD_H_
#define _RDK_SD_H_

#ifdef __cplusplus
extern "C" {
#endif


void  V9B_SdSetCmd0Fifo(UINT8 val);
UINT8 V9B_SdGetCmd0Fifo(void);

void  V9B_SdSetCmd1Fifo(UINT8 val);
UINT8 V9B_SdGetCmd1Fifo(void);

void  V9B_SdSetDatFifo(UINT8 val);
UINT8 V9B_SdGetDatFifo(void);


#ifdef __cplusplus
}
#endif

#endif

