/***********************************************************************************/
/** 	OSType.h   	                                                              **/
/**     Function:   Data struct type and macro definition                         **/
/** 	Author: 	zhangjunqi                                                    **/
/**	    Date:		2005-05-24                                                    **/
/**	    Copyright:	vimicro company                                               **/
/***********************************************************************************/


#ifndef _OSTYPE_H_
#define _OSTYPE_H_


#include "OSCfg.h"
#include "../../v568/tools/type.h"


/*
*********************************************************************************************************
*                                             ERROR CODES
*********************************************************************************************************
*/
#define OS_NO_ERR                 0

#define OS_ERR_EVENT_TYPE         1
#define OS_ERR_PEND_ISR           2
#define OS_ERR_POST_NULL_PTR      3
#define OS_ERR_PEVENT_NULL        4
#define OS_ERR_POST_ISR           5
#define OS_ERR_QUERY_ISR          6
#define OS_ERR_INVALID_OPT        7
#define OS_ERR_TASK_WAITING       8

#define OS_TIMEOUT               10
#define OS_TASK_NOT_EXIST        11

#define OS_MBOX_FULL             20

#define OS_Q_FULL                30

#define OS_PRIO_EXIST            40
#define OS_PRIO_ERR              41
#define OS_PRIO_INVALID          42

#define OS_SEM_OVF               50

#define OS_TASK_DEL_ERR          60
#define OS_TASK_DEL_IDLE         61
#define OS_TASK_DEL_REQ          62
#define OS_TASK_DEL_ISR          63

#define OS_NO_MORE_TCB           70

#define OS_TIME_NOT_DLY          80
#define OS_TIME_INVALID_MINUTES  81
#define OS_TIME_INVALID_SECONDS  82
#define OS_TIME_INVALID_MILLI    83
#define OS_TIME_ZERO_DLY         84

#define OS_TASK_SUSPEND_PRIO     90
#define OS_TASK_SUSPEND_IDLE     91

#define OS_TASK_RESUME_PRIO     100
#define OS_TASK_NOT_SUSPENDED   101

#define OS_MEM_INVALID_PART     110
#define OS_MEM_INVALID_BLKS     111
#define OS_MEM_INVALID_SIZE     112
#define OS_MEM_NO_FREE_BLKS     113
#define OS_MEM_FULL             114
#define OS_MEM_INVALID_PBLK     115
#define OS_MEM_INVALID_PMEM     116
#define OS_MEM_INVALID_PDATA    117
#define OS_MEM_INVALID_ADDR     118

#define OS_ERR_NOT_MUTEX_OWNER  120

#define OS_TASK_OPT_ERR         130

#define OS_ERR_DEL_ISR          140
#define OS_ERR_CREATE_ISR       141

#define OS_FLAG_INVALID_PGRP    150
#define OS_FLAG_ERR_WAIT_TYPE   151
#define OS_FLAG_ERR_NOT_RDY     152
#define OS_FLAG_INVALID_OPT     153
#define OS_FLAG_GRP_DEPLETED    154

#define OS_FUNCPNT_NULL         160
#define OS_DATPNT_NULL          161


typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide */


/**********************************************************************************************/
/************************************ miscellaneous define ************************************/
/**********************************************************************************************/

#define  OS_VERSION    100

#define  OS_EXT  extern

#define  OS_PRIO_SELF           0xFF                    /* Indicate SELF priority                      */

#if OS_TASK_STAT_EN > 0
#define  OS_N_SYS_TASKS            2                    /* Number of system tasks                      */
#else
#define  OS_N_SYS_TASKS            1
#endif

#define  OS_STAT_PRIO       (OS_LOWEST_PRIO - 1)        /* Statistic task priority                     */
#define  OS_IDLE_PRIO       (OS_LOWEST_PRIO)            /* IDLE      task priority                     */

#define  OS_EVENT_TBL_SIZE ((OS_LOWEST_PRIO) / 8 + 1)   /* Size of event table                         */
#define  OS_RDY_TBL_SIZE   ((OS_LOWEST_PRIO) / 8 + 1)   /* Size of ready table                         */

#define  OS_TASK_IDLE_ID       65535                    /* I.D. numbers for Idle and Stat tasks        */
#define  OS_TASK_STAT_ID       65534

/*
*********************************************************************************************************
*                              TASK STATUS (Bit definition for OSTCBStat)
*********************************************************************************************************
*/
#define  OS_STAT_RDY            0x00        /* Ready to run                                            */
#define  OS_STAT_SEM            0x01        /* Pending on semaphore                                    */
#define  OS_STAT_MBOX           0x02        /* Pending on mailbox                                      */
#define  OS_STAT_Q              0x04        /* Pending on queue                                        */
#define  OS_STAT_SUSPEND        0x08        /* Task is suspended                                       */
#define  OS_STAT_MUTEX          0x10        /* Pending on mutual exclusion semaphore                   */
#define  OS_STAT_FLAG           0x20        /* Pending on event flag group                             */


/***********************************************************************************************************/
/****************************************** Data structure define ******************************************/
/***********************************************************************************************************/

/*
*********************************************************************************************************
*                                          CPU STATUS REGISTER
*********************************************************************************************************
*/
typedef struct os_cpu_sr {
	UINT32      cpcsr;
} OS_CPU_SR;

/*
*********************************************************************************************************
*                                          TASK CONTROL BLOCK
*********************************************************************************************************
*/

typedef struct os_tcb {
    OS_STK        *OSTCBStkPtr;        /* Pointer to current top of stack                              */

    struct os_tcb *OSTCBNext;          /* Pointer to next     TCB in the TCB list                      */
    struct os_tcb *OSTCBPrev;          /* Pointer to previous TCB in the TCB list                      */

    UINT16         OSTCBDly;           /* Nbr ticks to delay task or, timeout waiting for event        */
    UINT8          OSTCBStat;          /* Task status                                                  */
    UINT8          OSTCBPrio;          /* Task priority (0 == highest, 63 == lowest)                   */

    UINT8          OSTCBX;             /* Bit position in group  corresponding to task priority (0..7) */
    UINT8          OSTCBY;             /* Index into ready table corresponding to task priority        */
    UINT8          OSTCBBitX;          /* Bit mask to access bit position in ready table               */
    UINT8          OSTCBBitY;          /* Bit mask to access bit position in ready group               */
} OS_TCB;


#endif  // _OSTYPE_H_


