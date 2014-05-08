/***********************************************************************************/
/** 	OSTask.c   	                                                              **/
/**     Function:   Task function and global variables definition                 **/
/** 	Author: 	zhangjunqi                                                    **/
/**	    Date:		2005-05-24                                                    **/
/**	    Copyright:	vimicro company                                               **/
/***********************************************************************************/


#include "OSInternal.h"


/*
*********************************************************************************************************
*                                            CREATE A TASK
*
* Description: This function is used to have OS manage the execution of a task.  Tasks can either
*              be created by a running task or prior to the start of multitasking.  A task cannot be
*              created by an ISR.
*
* Arguments  : task     is a pointer to the task's code
*
*              pdata    is a pointer to an optional data area which can be used to pass parameters to
*                       the task when the task first executes.  Where the task is concerned it thinks
*                       it was invoked and passed the argument 'pdata' as follows:
*
*                           void Task (void *pdata)
*                           {
*                               for (;;) {
*                                   Task code;
*                               }
*                           }
*
*              ptos     is a pointer to the task's top of stack.  If the configuration constant
*                       OS_STK_GROWTH is set to 1, the stack is assumed to grow downward (i.e. from high
*                       memory to low memory).  'pstk' will thus point to the highest (valid) memory
*                       location of the stack.  If OS_STK_GROWTH is set to 0, 'pstk' will point to the
*                       lowest memory location of the stack and the stack will grow with increasing
*                       memory locations.
*
*              prio     is the task's priority.  A unique priority MUST be assigned to each task and the
*                       lower the number, the higher the priority.
*
* Returns    : OS_FUNCPNT_NULL  if task == NULL
*              OS_DATPNT_NULL   if pdata == NULL
*              OS_NO_ERR        if the function was successful.
*              OS_PRIO_EXIT     if the task priority already exist
*                               (each task MUST have a unique priority).
*              OS_PRIO_INVALID  if the priority you specify is higher that the maximum allowed
*                               (i.e. >= OS_LOWEST_PRIO)
*********************************************************************************************************
*/

UINT8  OSTaskCreate (void (*task)(void *pd), void *pdata, OS_STK *ptos, UINT8 prio)
{
    OS_STK     *psp;
    UINT8      err;


#if OS_ARG_CHK_EN > 0
	if(task == NULL)                        /* Make sure input funcrion pointer is valid             */
		return (OS_FUNCPNT_NULL);
	if(pdata == NULL)                       /* Make sure input data pointer is valid                 */
		return (OS_DATPNT_NULL);
    if (prio > OS_LOWEST_PRIO)              /* Make sure priority is within allowable range           */
        return (OS_PRIO_INVALID);
#endif

    OS_ENTER_CRITICAL();
    if (OSTCBPrioTbl[prio] == (OS_TCB *)0)   /* Make sure task doesn't already exist at this priority  */
	{
        OSTCBPrioTbl[prio] = (OS_TCB *)1;    /* Reserve the priority to prevent others from doing ...  */
                                             /* ... the same thing until task is created.              */
        OS_EXIT_CRITICAL();
        psp = (OS_STK *)OSTaskStkInit(task, pdata, ptos, 0);    /* Initialize the task's stack         */
        err = OS_TCBInit(prio, psp, (OS_STK *)0, 0, 0, (void *)0, 0);
        if (err == OS_NO_ERR) 
		{
            OS_ENTER_CRITICAL();
            OSTaskCtr++;                                        /* Increment the #tasks counter        */
            OS_EXIT_CRITICAL();
            if (OSRunning == TRUE)           /* Find highest priority task if multitasking has started */
                OS_Sched();
        } 
		else 
		{
            OS_ENTER_CRITICAL();
            OSTCBPrioTbl[prio] = (OS_TCB *)0;/* Make this priority available to others                 */
            OS_EXIT_CRITICAL();
        }
        return (err);
    }
    OS_EXIT_CRITICAL();
    
	return (OS_PRIO_EXIST);
}

