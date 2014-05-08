/***********************************************************************************/
/** 	OSCPUARM.h   	                                                          **/
/**     Function:   Base Functions declare responding to specified ARM CPU        **/
/**				    Some macro definition                                         **/
/** 	Author: 	zhangjunqi                                                    **/
/**	    Date:		2005-05-24                                                    **/
/**	    Copyright:	vimicro company                                               **/
/***********************************************************************************/


#ifndef _OSCPUARM_H_
#define _OSCPUARM_H_


#define	SVC32MODE	0x13
#define  OS_EVENT_EN       (((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0))


#define	OS_ENTER_CRITICAL()	                 DisableInt()
#define	OS_EXIT_CRITICAL()	                 EnableInt() 


/******** Close and Open interrupt functions,   defined in ../s3c44box/44BLIB_A.S ***********/
void DisableInt(void);
void EnableInt(void);


#endif  // _OSCPUARM_H_

