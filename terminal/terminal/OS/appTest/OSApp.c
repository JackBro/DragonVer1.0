/***********************************************************************************/
/** 	OSApp.c   	                                                              **/
/**     Function:   OS applcation test                                            **/
/** 	Author: 	zhangjunqi                                                    **/
/**	    Date:		2005-05-24                                                    **/
/**	    Copyright:	vimicro company                                               **/
/***********************************************************************************/

#include <stdio.h>
#include "../src/OSInternal.h"
#include "../../s3c44box/comm.h"  /* For tmPrintf */


#define	N_TASKS			3	// Number of tasks
#define	TASK_STK_SIZE	1024		// Stack size, in sizeof OS_STK, or int 32bit
static OS_STK	TaskStk[N_TASKS][TASK_STK_SIZE];	// Tasks stacks


static void Sleep(UINT16 uSec)
{
	OSTimeDly((UINT16) (uSec));
}

static void Task1(void * pParam)
{
	int i;
	OS_STK *sp = &TaskStk[0][TASK_STK_SIZE-1];
	pParam =pParam;

	while(1)	
	{
		tmPrintf( "@\r\n" );
		tmPrintf( "@\r\n" );
		tmPrintf( "@ task 1 running ... \r\n" );
		tmPrintf( "@    Q__Q    \r\n" );
		tmPrintf( "@   /____\\   \r\n" );
		tmPrintf( "@   \\____/   \r\n" );
		tmPrintf( "@    /\\/\\    \r\n" );
		tmPrintf( "@ __(\\\\//)__ \r\n" );
		tmPrintf( "@ >__/ww\\__< \r\n" );
		tmPrintf( "@\r\n" );
		tmPrintf( "@ go to sleep 50 time-ticks\r\n" );
		tmPrintf( "@\r\n" );
		tmPrintf( "@\r\n" );

	    tmPrintf( "+++ pc = %x  \n", *--sp );
		tmPrintf( "+++ lr = %x  \n", *--sp );
		for(i = 12; i>= 0; i--)
		    tmPrintf( "+++ r%d = %x  \n", i, *--sp );
		tmPrintf( "+++ cpsr = %x  \n", *--sp );
		tmPrintf( "+++ spsr = %x  \n", *--sp );

		Sleep(50);
	}

}

/* Task2 print_user_input */
static void Task2(void * pParam)
{
	int i;
	OS_STK *sp = &TaskStk[1][TASK_STK_SIZE-1];
	pParam = pParam;

	while(1)	
	{
		tmPrintf( "+++ \r\n" );
		tmPrintf( "+++ \r\n" );
		tmPrintf( "+++  task 2 running ... \r\n" );
		tmPrintf( "+++  ¨q¦ä¦ä¦ä¨r \r\n" );
		tmPrintf( "+++ {/¡¡o¡¡o¡¡\\} \r\n" );
		tmPrintf( "+++  (¡¡(oo)¡¡) \r\n" );
		tmPrintf( "+++      ¦á \r\n" );
		tmPrintf( "+++ \r\n" );
		tmPrintf( "+++  go to sleep 20 time-ticks\r\n" );
		tmPrintf( "+++ \r\n" );
		tmPrintf( "+++ \r\n" );
		
		tmPrintf( "+++ pc = %x  \n", *--sp );
		tmPrintf( "+++ lr = %x  \n", *--sp );
		for(i = 12; i>= 0; i--)
		    tmPrintf( "+++ r%d = %x  \n", i, *--sp );
		tmPrintf( "+++ cpsr = %x  \n", *--sp );
		tmPrintf( "+++ spsr = %x  \n", *--sp );

		Sleep(20);
	}
}

void TestOS( void )
{
	int	task_1 = 0;
	int	task_2 = 1;

	printf( "start kernel ok!\r\n" );
	
	OSInit();
	
	OSTaskCreate(Task1, &task_1, &TaskStk[0][TASK_STK_SIZE-1], 0);
	OSTaskCreate(Task2, &task_2, &TaskStk[1][TASK_STK_SIZE-1], 1);
	
	OSStart();
}

