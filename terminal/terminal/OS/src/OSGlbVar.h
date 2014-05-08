/***********************************************************************************/
/** 	OSGlbVar.h	                                                              **/
/**     Function:   OS global variables declare                                   **/
/** 	Author: 	zhangjunqi                                                    **/
/**	    Date:		2005-05-24                                                    **/
/**	    Copyright:	vimicro company                                               **/
/***********************************************************************************/


#ifndef _OSGLBVAR_H_
#define _OSGLBVAR_H_


#include "OSType.h"
#include "OSCfg.h"


OS_EXT UINT32            OSCtxSwCtr;               /* Counter of number of context switches           */
OS_EXT UINT8             OSCPUUsage;               /* Percentage of CPU used                          */
OS_EXT UINT32            OSIdleCtrMax;             /* Max. value that idle ctr can take in 1 sec.     */
OS_EXT UINT32            OSIdleCtrRun;             /* Val. reached by idle ctr at run time in 1 sec.  */
OS_EXT UINT8             OSStatRdy;                /* Flag indicating that the statistic task is rdy  */
OS_EXT OS_STK            OSTaskStatStk[OS_TASK_STAT_STK_SIZE];      /* Statistics task stack          */

OS_EXT OS_TCB           *OSTCBCur;                        /* Pointer to currently running TCB         */
OS_EXT OS_TCB           *OSTCBFreeList;                   /* Pointer to list of free TCBs             */
OS_EXT OS_TCB           *OSTCBHighRdy;                    /* Pointer to highest priority TCB R-to-R   */
OS_EXT OS_TCB           *OSTCBList;                       /* Pointer to doubly linked list of TCBs    */
OS_EXT OS_TCB           *OSTCBPrioTbl[OS_LOWEST_PRIO + 1];/* Table of pointers to created TCBs        */
OS_EXT OS_TCB            OSTCBTbl[OS_MAX_TASKS + OS_N_SYS_TASKS];   /* Table of TCBs                  */

OS_EXT UINT8             OSIntNesting;             /* Interrupt nesting level                         */
OS_EXT UINT8             OSIntExitY;

OS_EXT UINT8             OSLockNesting;            /* Multitasking lock nesting level                 */

OS_EXT UINT8             OSPrioCur;                /* Priority of current task                        */
OS_EXT UINT8             OSPrioHighRdy;            /* Priority of highest priority task               */

OS_EXT UINT8             OSRdyGrp;                        /* Ready list group                         */
OS_EXT UINT8             OSRdyTbl[OS_RDY_TBL_SIZE];       /* Table of tasks which are ready to run    */

OS_EXT UINT8             OSRunning;                       /* Flag indicating that kernel is running   */

OS_EXT UINT8             OSTaskCtr;                       /* Number of tasks created                  */
OS_EXT UINT32            OSIdleCtr;                                 /* Idle counter                   */

OS_EXT OS_STK            OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE];      /* Idle task stack                */

OS_EXT  volatile  UINT32  OSTime;                   /* Current value of system time (in ticks)         */

#endif  // _OSGLBVAR_H_

