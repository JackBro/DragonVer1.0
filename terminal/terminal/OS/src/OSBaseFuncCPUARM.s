;/************************************************************************/
; *
; *	OSBaseFuncCPUARM.s  
; *	
; * Function:   Base Functions definition responding to specified ARM CPU
; *	Author: 	zhangjunqi
; *	Date:		2005-05-23
; *	Copyright:	vimicro company
; *
;************************************************************************/


     AREA    BaseFun,CODE,READONLY


;/***********************************************************************
;
; Function: OSStartHighRdy
;
; Purpose:
; 	   To start the task with the highest priority during OS startup
;
; Parameters: void
;
; Outputs:  None
;
; Returns:  void
;
; Notes:
;   Called once during OSStart()
;
;*********************************************************************/
	EXPORT 	OSStartHighRdy
	IMPORT  OSTCBHighRdy 
	IMPORT  OSRunning
	IMPORT	OSTaskSwHook
OSStartHighRdy
        BL 	OSTaskSwHook                 ; Call user-defined hook function

        LDR 	r4,=OSRunning            ; Indicate that multitasking has started
        MOV 	r5, #1                   
        STRB 	r5, [r4]                 ; OSRunning = true

        LDR 	r4, =OSTCBHighRdy        ; Get highest priority task TCB address
        LDR 	r4, [r4]                 ; get stack pointer
        LDR 	sp, [r4]                 ; switch to the new stack

        LDMFD 	sp!, {r4}                ; pop new task s spsr_cxsf
        MSR 	spsr_cxsf, r4
        LDMFD 	sp!, {r4}                ; pop new task s psr
        MSR 	cpsr_cxsf, r4
        LDMFD 	sp!, {r0-r12,lr,pc}      ; pop new task s r0-r12,lr & pc
        
             
;/***********************************************************************
;
; Function: OSCtxSw
;
; Purpose:
; 	To perform a context switch in the Task Level.
;
; Parameters: void
;
; Outputs:  None
;
; Returns:  void
;
; Notes:
;   On entry, OSTCBCur and OSPrioCur hold the current TCB and priority
;   and OSTCBHighRdy and OSPrioHighRdy contain the same for the task
;   to be switched to.
; 
;   The following code assumes that the virtual memory is directly
;   mapped into  physical memory. If this is not true, the cache must 
;   be flushed at context switch to avoid address aliasing.
;
;*********************************************************************/
        EXPORT 	OSCtxSw
        IMPORT	OSPrioCur
        IMPORT	OSPrioHighRdy
        IMPORT	OSTCBCur
        IMPORT	OSTCBHighRdy
        IMPORT	OSTaskSwHook
        
OSCtxSw
        STMFD 	sp!, {lr}                ; push pc (lr is actually be pushed in place of PC)
        STMFD 	sp!, {r0-r12,lr}         ; push lr & register file
        MRS 	r4,  cpsr
        STMFD 	sp!, {r4}                ; push current psr
        MRS 	r4,  spsr
        STMFD 	sp!, {r4}                ; push current spsr_cxsf

_OSCtxSw
        LDR 	r4, =OSPrioCur           ; OSPrioCur = OSPrioHighRdy
        LDR 	r5, =OSPrioHighRdy
        LDRB 	r6, [r5]
        STRB 	r6, [r4]
        
        LDR 	r4, =OSTCBCur            ; Get current task TCB address
        LDR 	r5, [r4]
        STR 	sp, [r5]                 ; store sp in preempted tasks s TCB

        BL 	OSTaskSwHook                 ; call Task Switch Hook

        LDR 	r6, =OSTCBHighRdy        ; Get highest priority task TCB address
        LDR 	r6, [r6]
        LDR 	sp, [r6]                 ; get new task s stack pointer

        STR 	r6, [r4]                 ; set new current task TCB address

        LDMFD 	sp!, {r4}                ; pop new task spsr_cxsf
        MSR 	spsr_cxsf, r4
        LDMFD 	sp!, {r4}                ; pop new task cpsr
        MSR 	cpsr_cxsf, r4
        LDMFD 	sp!, {r0-r12,lr,pc}      ; pop new task r0-r12,lr & pc


;***********************************************************************
;
; Function: OSIntCtxSw
;
; Purpose:
; 	To perform a context switch from the interrupt level.
;
; Parameters: void
;
; Outputs:  None
;
; Returns:  void
;
; Notes:
;   Sets up the stacks and registers to call the task level
;   context switch
;
;*********************************************************************/
        EXPORT 	OSIntCtxSw
        IMPORT	OSIntCtxSwFlag
OSIntCtxSw
        LDR 	r0, =OSIntCtxSwFlag      ; OSIntCtxSwFlag = true
        MOV 	r1, #1
        STR 	r1, [r0]
        
        MOV 	pc, lr                   ; return 



NUINT	    EQU	0xc0
NUIRQ        EQU  0x80
NUFIQ        EQU  0x40

;/***********************************************************************
;
; Function: DisableInt
;
; Purpose:
; 	Disable IRQ interrupt if IRQ interrupt enable now and record the cprs.
;
; Parameters: void
;
; Outputs:  None
;
; Returns:  void
;
; Note: This function works only if the processor is in previliged mode
;*********************************************************************/

	EXPORT	DisableInt
DisableInt

    mrs	    r0, cpsr
    stmdb    r13!, {r0}       ;push the value of cpsr
    orr	    r0, r0, #NUINT   ;set irq and fiq flag bit
    msr	    cpsr_cxsf, r0		

    mov	    pc,lr


;/***********************************************************************
;
; Function: EnableInt
;
; Purpose:
; 	Resume IRQ interrupt flag according to recorded cprs just last time.
;
; Parameters: void
;
; Outputs:  None
;
; Returns:  void
;
; Note: This function works only if the processor is in previliged mode
;*********************************************************************/

	EXPORT	EnableInt
EnableInt
    
    ldmia    r13!, {r0}           ;pop the value of cpsr stored last time
    and      r0, r0, #NUINT       ;get irq and fiq setting in last disable interrupt entry
    mrs	    r1, cpsr
    bic	    r1, r1, #NUINT       ;clear irq and fiq flag bit 
    orr      r1, r1, r0           ;restore irq and fiq setting in last disable interrupt entry
    msr	    cpsr_cxsf, r1	

    mov	    pc,lr
    
;------------------------------------------------------------------------------------------------------------------
;------------------------------------------------------------------------------------------------------------------
        END
        
        