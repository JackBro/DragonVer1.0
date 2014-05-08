/**********************************************************************************/
/*  Name:      aes.h                                                              */
/*  Function:  AES control of vc0938                                              */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/06/02                                                     */
/**********************************************************************************/

#ifndef _RDK_AES_H_
#define _RDK_AES_H_

#ifdef __cplusplus
extern "C" {
#endif


void   V9B_AesSetCorCmd(UINT8 val);
UINT8  V9B_AesGetCorCmd(void);
void   V9B_AesSetCorWorkMod(UINT8 workMod);
UINT8  V9B_AesGetCorWorkMod(void);
void   V9B_AesRstCorSta(void);
UINT8  V9B_AesGetDatVld(void);
UINT8  V9B_AesGetBusySta(void);

void   V9B_AesSetDecStrbDlyCnt(UINT8 dlyCnt);

void   V9B_AesSetKey(UINT8 *pKey);
void   V9B_AesGetKey(UINT8 *pKey);

void   V9B_AesSetIv(UINT8 *pIv);
void   V9B_AesGetIv(UINT8 *pIv);


#ifdef __cplusplus
}
#endif

#endif

