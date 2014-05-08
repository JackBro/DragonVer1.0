/***********************************************************************************/
/** 	OSCore.c   	                                                              **/
/**     Function:   OS kernal function define                                     **/
/** 	Author: 	zhangjunqi                                                    **/
/**	    Date:		2005-05-24                                                    **/
/**	    Copyright:	vimicro company                                               **/
/***********************************************************************************/


#include "OSInternal.h"


/*
*********************************************************************************************************
*                              MAPPING TABLE TO MAP BIT POSITION TO BIT MASK
*
* Note: Index into table is desired bit position, 0..7
*       Indexed value corresponds to bit mask
*********************************************************************************************************
*/

UINT8  const  OSMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/*
*********************************************************************************************************
*                                       PRIORITY RESOLUTION TABLE
*
* Note: Index into table is bit pattern to resolve highest priority
*       Indexed value corresponds to highest priority bit position (i.e. 0..7)
*********************************************************************************************************
*/

UINT8  const  OSUnMapTbl[] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};


UINT32            OSCtxSwCtr;               /* Counter of number of context switches           */
UINT8             OSCPUUsage;               /* Percentage of CPU used                          */
UINT32            OSIdleCtrMax;             /* Max. value that idle ctr can take in 1 sec.     */
UINT32            OSIdleCtrRun;             /* Val. reached by idle ctr at run time in 1 sec.  */
UINT8             OSStatRdy;                /* Flag indicating that the statistic task is rdy  */
OS_STK            OSTaskStatStk[OS_TASK_STAT_STK_SIZE];      /* Statistics task stack          */

OS_TCB           *OSTCBCur;                        /* Pointer to currently running TCB         */
OS_TCB           *OSTCBFreeList;                   /* Pointer to list of free TCBs             */
OS_TCB           *OSTCBHighRdy;                    /* Pointer to highest priority TCB R-to-R   */
OS_TCB           *OSTCBList;                       /* Pointer to doubly linked list of TCBs    */
OS_TCB           *OSTCBPrioTbl[OS_LOWEST_PRIO + 1];/* Table of pointers to created TCBs        */
OS_TCB            OSTCBTbl[OS_MAX_TASKS + OS_N_SYS_TASKS];   /* Table of TCBs                  */

UINT8             OSIntNesting;             /* Interrupt nesting level                         */
UINT8             OSIntExitY;

UINT8             OSLockNesting;            /* Multitasking lock nesting level                 */

UINT8             OSPrioCur;                /* Priority of current task                        */
UINT8             OSPrioHighRdy;            /* Priority of highest priority task               */

UINT8             OSRdyGrp;                        /* Ready list group                         */
UINT8             OSRdyTbl[OS_RDY_TBL_SIZE];       /* Table of tasks which are ready to run    */

UINT8             OSRunning;                       /* Flag indicating that kernel is running   */

UINT8             OSTaskCtr;                       /* Number of tasks created                  */

UINT32            OSIdleCtr;                                 /* Idle counter                   */

OS_STK            OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE];      /* Idle task stack                */


/*
*********************************************************************************************************
*                                             OS INITIALIZATION
*
* Description: This function is used to initialize the internals of OS and MUST be called prior to
*              creating any OS object and, prior to calling OSStart().
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

void  OSInit (void)
{
    UINT16    i;
    UINT8     *prdytbl;
    OS_TCB    *ptcb1;
    OS_TCB    *ptcb2;


    OSCtxSwCtr    = 0;                                           /* Clear the context switch counter         */
    OSRdyGrp      = 0x00;                                        /* Clear the ready list                     */
    prdytbl       = &OSRdyTbl[0];
    for (i = 0; i < OS_RDY_TBL_SIZE; i++) 
        *prdytbl++ = 0x00;

    OSPrioCur     = 0;
    OSPrioHighRdy = 0;
    OSTCBHighRdy  = (OS_TCB *)0;                                 /* TCB Initialization                       */
    OSTCBCur      = (OS_TCB *)0;
    OSTCBList     = (OS_TCB *)0;
    for (i = 0; i < (OS_LOWEST_PRIO + 1); i++)                   /* Clear the priority table                 */
        OSTCBPrioTbl[i] = (OS_TCB *)0;
    
    ptcb1 = &OSTCBTbl[0];
    ptcb2 = &OSTCBTbl[1];
    for (i = 0; i < (OS_MAX_TASKS + OS_N_SYS_TASKS - 1); i++)    /* Init. list of free TCBs                  */
	{
		ptcb1->OSTCBNext = ptcb2;
        ptcb1++;
        ptcb2++;
    }
    ptcb1->OSTCBNext = (OS_TCB *)0;                              /* Last OS_TCB                              */
    OSTCBFreeList    = &OSTCBTbl[0];
}


/*
*********************************************************************************************************
*                                          START MULTITASKING
*
* Description: This function is used to start the multitasking process which lets OS manages the
*              task that you have created.  Before you can call OSStart(), you MUST have called OSInit()
*              and you MUST have created at least one task.
*
* Arguments  : none
*
* Returns    : none
*
* Note       : OSStartHighRdy() MUST:
*                 a) Call OSTaskSwHook() then,
*                 b) Set OSRunning to TRUE.
*********************************************************************************************************
*/

void  OSStart (void)
{
    UINT8 y;
    UINT8 x;


    if (OSRunning == FALSE) 
	{
        y             = OSUnMapTbl[OSRdyGrp];        /* Find highest priority's task priority number   */
        x             = OSUnMapTbl[OSRdyTbl[y]];
        OSPrioHighRdy = (UINT8)((y << 3) + x);
        OSPrioCur     = OSPrioHighRdy;
        OSTCBHighRdy  = OSTCBPrioTbl[OSPrioHighRdy]; /* Point to highest priority task ready to run    */
        OSTCBCur      = OSTCBHighRdy;
        OSStartHighRdy();                            /* Execute target specific code to start task     */
    }
}


/*
*********************************************************************************************************
*                                              ENTER ISR
*
* Description: This function is used to notify OS that you are about to service an interrupt
*              service routine (ISR).  This allows uC/OS-II to keep track of interrupt nesting and thus
*              only perform rescheduling at the last nested ISR.
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) Your ISR can directly increment OSIntNesting without calling this function because
*                 OSIntNesting has been declared 'global'.  You MUST, however, be sure that the increment
*                 is performed 'indivisibly' by your processor to ensure proper access to this critical
*                 resource.
*              2) You MUST still call OSIntExit() even though you increment OSIntNesting directly.
*              3) You MUST invoke OSIntEnter() and OSIntExit() in pair.  In other words, for every call
*                 to OSIntEnter() at the beginning of the ISR you MUST have a call to OSIntExit() at the
*                 end of the ISR.
*********************************************************************************************************
*/

void  OSIntEnter (void)
{
    OS_ENTER_CRITICAL();
    if (OSIntNesting < 255) 
        OSIntNesting++;                          /* Increment ISR nesting level                        */
    OS_EXIT_CRITICAL();
}


/*
*********************************************************************************************************
*                                               EXIT ISR
*
* Description: This function is used to notify OS that you have completed serviving an ISR.  When
*              the last nested ISR has completed, OS will call the scheduler to determine whether
*              a new, high-priority task, is ready to run.
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) You MUST invoke OSIntEnter() and OSIntExit() in pair.  In other words, for every call
*                 to OSIntEnter() at the beginning of the ISR you MUST have a call to OSIntExit() at the
*                 end of the ISR.
*              2) Rescheduling is prevented when the scheduler is locked (see OSSchedLock())
*********************************************************************************************************
*/

void  OSIntExit (void)
{
    OS_ENTER_CRITICAL();
    if (OSIntNesting > 0)                              /* Prevent OSIntNesting from wrapping           */
        OSIntNesting--;
    
    if ((OSIntNesting == 0) && (OSLockNesting == 0))   /* Reschedule only if all ISRs complete ...     */
	{
		OSIntExitY    = OSUnMapTbl[OSRdyGrp];          /* ... and not locked.                          */
        OSPrioHighRdy = (UINT8)((OSIntExitY << 3) + OSUnMapTbl[OSRdyTbl[OSIntExitY]]);
        if (OSPrioHighRdy != OSPrioCur)                /* No Ctx Sw if current task is highest rdy     */
		{
			OSTCBHighRdy  = OSTCBPrioTbl[OSPrioHighRdy];
            OSCtxSwCtr++;                              /* Keep track of the number of context switches */
            OSIntCtxSw();                              /* Perform interrupt level context switch       */
        }
    }
    OS_EXIT_CRITICAL();
}


/*
*********************************************************************************************************
*                                         PROCESS SYSTEM TICK
*
* Description: This function is used to signal to uC/OS-II the occurrence of a 'system tick' (also known
*              as a 'clock tick').  This function should be called by the ticker ISR but, can also be
*              called by a high priority task.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

void  OSTimeTick (void)
{
    OS_TCB    *ptcb;


    //OSTimeTickHook();                                      /* Call user definable hook                 */
#if OS_TIME_GET_SET_EN > 0   
    OS_ENTER_CRITICAL();                                   /* Update the 32-bit tick counter           */
    OSTime++;
    OS_EXIT_CRITICAL();
#endif    
    ptcb = OSTCBList;                                      /* Point at first TCB in TCB list           */
    while (ptcb != NULL && ptcb->OSTCBPrio != OS_IDLE_PRIO) {              /* Go through all TCBs in TCB list          */
        OS_ENTER_CRITICAL();
        if (ptcb->OSTCBDly != 0) {                         /* Delayed or waiting for event with TO     */
            if (--ptcb->OSTCBDly == 0) {                   /* Decrement nbr of ticks to end of delay   */
                if ((ptcb->OSTCBStat & OS_STAT_SUSPEND) == 0x00) {   /* Is task suspended?             */
                    OSRdyGrp               |= ptcb->OSTCBBitY; /* No,  Make task Rdy to Run (timed out)*/
                    OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
                } else {                                       /* Yes, Leave 1 tick to prevent ...     */
                    ptcb->OSTCBDly = 1;                        /* ... loosing the task when the ...    */
                }                                              /* ... suspension is removed.           */
            }
        }
        ptcb = ptcb->OSTCBNext;                                /* Point at next TCB in TCB list        */
        OS_EXIT_CRITICAL();
    }
}


/*
*********************************************************************************************************
*                                            INITIALIZE TCB
*
* Description: This function is internal to OS and is used to initialize a Task Control Block when
*              a task is created (see OSTaskCreate() and OSTaskCreateExt()).
*
* Arguments  : prio          is the priority of the task being created
*
*              ptos          is a pointer to the task's top-of-stack assuming that the CPU registers
*                            have been placed on the stack.  Note that the top-of-stack corresponds to a
*                            'high' memory location is OS_STK_GROWTH is set to 1 and a 'low' memory
*                            location if OS_STK_GROWTH is set to 0.  Note that stack growth is CPU
*                            specific.
*
*              pbos          is a pointer to the bottom of stack.  A NULL pointer is passed if called by
*                            'OSTaskCreate()'.
*
*              id            is the task's ID (0..65535)
*
*              stk_size      is the size of the stack (in 'stack units').  If the stack units are INT8Us
*                            then, 'stk_size' contains the number of bytes for the stack.  If the stack
*                            units are INT32Us then, the stack contains '4 * stk_size' bytes.  The stack
*                            units are established by the #define constant OS_STK which is CPU
*                            specific.  'stk_size' is 0 if called by 'OSTaskCreate()'.
*
*              pext          is a pointer to a user supplied memory area that is used to extend the task
*                            control block.  This allows you to store the contents of floating-point
*                            registers, MMU registers or anything else you could find useful during a
*                            context switch.  You can even assign a name to each task and store this name
*                            in this TCB extension.  A NULL pointer is passed if called by OSTaskCreate().
*
*              opt           options as passed to 'OSTaskCreateExt()' or,
*                            0 if called from 'OSTaskCreate()'.
*
* Returns    : OS_NO_ERR         if the call was successful
*              OS_DATPNT_NULL    if ptos == NULL, or pbos == NULL, or pext == NULL 
*              OS_NO_MORE_TCB    if there are no more free TCBs to be allocated and thus, the task cannot
*                                be created.
*
* Note       : This function is INTERNAL to uC/OS-II and your application should not call it.
*********************************************************************************************************
*/

UINT8  OS_TCBInit (UINT8 prio, OS_STK *ptos, OS_STK *pbos, UINT16 id, UINT32 stk_size, void *pext, UINT16 opt)
{
//    OS_CPU_SR  cpu_sr;                                    /* Allocate storage for CPU status register */    
    OS_TCB    *ptcb;


#if OS_ARG_CHK_EN > 0
#if OS_TASK_CREATE_EXT_EN > 0
	if(ptos == NULL)                                     /* Make sure input data pointer is valid             */
		return (OS_DATPNT_NULL);
    if (prio > OS_LOWEST_PRIO)                           /* Make sure priority is within allowable range      */
        return (OS_PRIO_INVALID);
#endif
#endif

    OS_ENTER_CRITICAL();
    ptcb = OSTCBFreeList;                                  /* Get a free TCB from the free TCB list    */
    if (ptcb != (OS_TCB *)0) 
	{
        OSTCBFreeList        = ptcb->OSTCBNext;            /* Update pointer to free TCB list          */
        OS_EXIT_CRITICAL();
        ptcb->OSTCBStkPtr    = ptos;                       /* Load Stack pointer in TCB                */
        ptcb->OSTCBPrio      = prio;                       /* Load task priority into TCB              */
        ptcb->OSTCBStat      = OS_STAT_RDY;                /* Task is ready to run                     */
        ptcb->OSTCBDly       = 0;                          /* Task is not delayed                      */

        pext                 = pext;                       /* Prevent compiler warning if not used     */
        stk_size             = stk_size;
        pbos                 = pbos;
        opt                  = opt;
        id                   = id;

        ptcb->OSTCBY         = prio >> 3;                  /* Pre-compute X, Y, BitX and BitY          */
        ptcb->OSTCBBitY      = OSMapTbl[ptcb->OSTCBY];
        ptcb->OSTCBX         = prio & 0x07;
        ptcb->OSTCBBitX      = OSMapTbl[ptcb->OSTCBX];

        //OSTaskCreateHook(ptcb);                            /* Call user defined hook                   */
        
        OS_ENTER_CRITICAL();
        OSTCBPrioTbl[prio] = ptcb;
        ptcb->OSTCBNext    = OSTCBList;                    /* Link into TCB chain                      */
        ptcb->OSTCBPrev    = (OS_TCB *)0;
        if (OSTCBList != (OS_TCB *)0) 
            OSTCBList->OSTCBPrev = ptcb;
        
        OSTCBList               = ptcb;
        OSRdyGrp               |= ptcb->OSTCBBitY;         /* Make task ready to run                   */
        OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
        OS_EXIT_CRITICAL();
        return (OS_NO_ERR);
    }
    OS_EXIT_CRITICAL();
    return (OS_NO_MORE_TCB);
}


/*
*********************************************************************************************************
*                                             OS SCHEDULER
*
* Description: This function is called by other OS services to determine whether a new, high
*              priority task has been made ready to run.  This function is invoked by TASK level code
*              and is not used to reschedule tasks from ISRs (see OSIntExit() for ISR rescheduling).
*
* Arguments  : none
*
* Returns    : none
*
* Notes      : 1) This function is INTERNAL to OS and your application should not call it.
*              2) Rescheduling is prevented when the scheduler is locked (see OSSchedLock())
*********************************************************************************************************
*/

void  OS_Sched (void)
{
    UINT8      y;


    OS_ENTER_CRITICAL();
    if ((OSIntNesting == 0) && (OSLockNesting == 0))   /* Sched. only if all ISRs done & not locked    */
	{
		y             = OSUnMapTbl[OSRdyGrp];          /* Get pointer to HPT ready to run              */
        OSPrioHighRdy = (UINT8)((y << 3) + OSUnMapTbl[OSRdyTbl[y]]);
        if (OSPrioHighRdy != OSPrioCur)                /* No Ctx Sw if current task is highest rdy     */
		{
			OSTCBHighRdy = OSTCBPrioTbl[OSPrioHighRdy];
            OSCtxSwCtr++;                              /* Increment context switch counter             */
            OS_TASK_SW();                              /* Perform a context switch                     */
        }
    }
    OS_EXIT_CRITICAL();
}


#if 0
/*
*********************************************************************************************************
*                                              IDLE TASK
*
* Description: This task is internal to OS and executes whenever no other higher priority tasks
*              executes because they are ALL waiting for event(s) to occur.
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : 1) OSTaskIdleHook() is called after the critical section to ensure that interrupts will be
*                 enabled for at least a few instructions.  On some processors (ex. Philips XA), enabling
*                 and then disabling interrupts didn't allow the processor enough time to have interrupts
*                 enabled before they were disabled again.  OS would thus never recognize
*                 interrupts.
*              2) This hook has been added to allow you to do such things as STOP the CPU to conserve 
*                 power.
*********************************************************************************************************
*/

void  OS_TaskIdle (void *pdata)
{
   
    
    pdata = pdata;                               /* Prevent compiler warning for not using 'pdata'     */
    for (;;) 
	{
        OS_ENTER_CRITICAL();
        OSIdleCtr++;
        OS_EXIT_CRITICAL();
        OSTaskIdleHook();                        /* Call user definable HOOK                           */
    }
}


/*
*********************************************************************************************************
*                                            STATISTICS TASK
*
* Description: This task is internal to OS and is used to compute some statistics about the
*              multitasking environment.  Specifically, OS_TaskStat() computes the CPU usage.
*              CPU usage is determined by:
*
*                                          OSIdleCtr
*                 OSCPUUsage = 100 * (1 - ------------)     (units are in %)
*                                         OSIdleCtrMax
*
* Arguments  : pdata     this pointer is not used at this time.
*
* Returns    : none
*
* Notes      : 1) This task runs at a priority level higher than the idle task.  In fact, it runs at the
*                 next higher priority, OS_IDLE_PRIO-1.
*              2) You can disable this task by setting the configuration #define OS_TASK_STAT_EN to 0.
*              3) We delay for 5 seconds in the beginning to allow the system to reach steady state and
*                 have all other tasks created before we do statistics.  You MUST have at least a delay
*                 of 2 seconds to allow for the system to establish the maximum value for the idle
*                 counter.
*********************************************************************************************************
*/

void  OS_TaskStat (void *pdata)
{
    UINT32     run;
    UINT8      usage;


    pdata = pdata;                               /* Prevent compiler warning for not using 'pdata'     */
    while (OSStatRdy == FALSE)
	        OSTimeDly(2 * OS_TICKS_PER_SEC);         /* Wait until statistic task is ready                 */
    
    for (;;)
	{
        OS_ENTER_CRITICAL();
        OSIdleCtrRun = OSIdleCtr;                /* Obtain the of the idle counter for the past second */
        run          = OSIdleCtr;
        OSIdleCtr    = 0L;                       /* Reset the idle counter for the next second         */
        OS_EXIT_CRITICAL();
        if (OSIdleCtrMax > 0L) 
		{
            usage = (UINT8)(100L - 100L * run / OSIdleCtrMax);
            if (usage >= 0)                      /* Make sure we don't have a negative percentage      */
                OSCPUUsage = usage;
            else 
                OSCPUUsage = 0;
        } else 
		{
            OSCPUUsage = 0;
        }
        OSTaskStatHook();                        /* Invoke user definable hook                         */
        OSTimeDly(OS_TICKS_PER_SEC);             /* Accumulate OSIdleCtr for the next second           */
    }
}

#endif

/*
*********************************************************************************************************
*                                             GET OS VERSION
*
* Description: This function is used to return the version number of OS.  The returned value
*              corresponds to OS's version number multiplied by 100.  In other words, version 2.00
*              would be returned as 200.
*
* Arguments  : none
*
* Returns    : the version number of OS multiplied by 100.
*********************************************************************************************************
*/

UINT16  OSGetVersion (void)
{
    return (OS_VERSION);
}

