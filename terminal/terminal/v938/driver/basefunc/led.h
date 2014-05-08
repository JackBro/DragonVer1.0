/**********************************************************************************/
/*  Name:      led.h                                                              */
/*  Function:  Control led of vc0938                                              */
/*  Author:    zhangjunqi                                                         */
/*  Created date:  2005/04/12                                                     */
/**********************************************************************************/


#ifndef _RDK_LED_H_
#define _RDK_LED_H_

#ifdef __cplusplus
extern "C" {
#endif


void   V9B_LedSetTimSlic(UINT16 val);
UINT16 V9B_LedGetTimSlic(void);

void   V9B_LedSetPwmRLed(UINT16 val);
UINT16 V9B_LedGetPwmRLed(void);

void   V9B_LedSetPwmBLed(UINT16 val);
UINT16 V9B_LedGetPwmBLed(void);

void   V9B_LedSetPwmGLed(UINT16 val);
UINT16 V9B_LedGetPwmGLed(void);

void   V9B_LedSetMotConf(UINT8 val);
UINT8  V9B_LedGetMotConf(void);

void   V9B_LedSetCirPeriod(UINT8 val);
UINT8  V9B_LedGetCirPeriod(void);

void   V9B_LedSetIdx(UINT8 val);
UINT8  V9B_LedGetIdx(void);

void   V9B_LedSetPattern0(UINT16 val);
UINT16 V9B_LedGetPattern0(void);
void   V9B_LedSetPattern1(UINT16 val);
UINT16 V9B_LedGetPattern1(void);
void   V9B_LedSetPattern2(UINT16 val);
UINT16 V9B_LedGetPattern2(void);
void   V9B_LedSetPattern3(UINT16 val);
UINT16 V9B_LedGetPattern3(void);
void   V9B_LedSetPattern4(UINT16 val);
UINT16 V9B_LedGetPattern4(void);
void   V9B_LedSetPattern5(UINT16 val);
UINT16 V9B_LedGetPattern5(void);
void   V9B_LedSetPattern6(UINT16 val);
UINT16 V9B_LedGetPattern6(void);
void   V9B_LedSetPattern7(UINT16 val);
UINT16 V9B_LedGetPattern7(void);


#ifdef __cplusplus
}
#endif

#endif

