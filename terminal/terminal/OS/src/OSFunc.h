/***********************************************************************************/
/** 	OSFunc.h   	                                                              **/
/**     Function:   Function declare                                              **/
/** 	Author: 	zhangjunqi                                                    **/
/**	    Date:		2005-05-24                                                    **/
/**	    Copyright:	vimicro company                                               **/
/***********************************************************************************/


#ifndef _OSFUNC_H_
#define _OSFUNC_H_


void       OSInit(void);
void       OSStart(void);
void       OS_Sched(void);
void       OSCtxSw(void);
void       OSIntCtxSw(void);
void       OSStartHighRdy(void);
UINT8      OS_TCBInit(UINT8 prio, OS_STK *ptos, OS_STK *pbos, UINT16 id, UINT32 stk_size, void *pext, UINT16 opt);
void       OSTimeTick(void);

UINT8      OSTaskCreate(void (*task)(void *pd), void *pdata, OS_STK *ptos, UINT8 prio);
OS_STK*    OSTaskStkInit(void (*task)(void *pd), void *pdata, OS_STK *ptos, UINT16 opt);
void       OSTaskSwHook(void);

void       OSTimeDly(UINT16 ticks);


#define    OS_TASK_SW            OSCtxSw

#endif  // _OSFUNC_H_

