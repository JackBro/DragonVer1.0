

	AREA |C$$code|, CODE, READONLY

	EXPORT	ChangeMemCon
;*************************************************************************
;*	The function for changing the Bank control and refresh registers) *
;**************************************************************************
;void ChangeMemCon();
ChangeMemCon
    stmfd   sp!,{r4-r9}	    ;Assembler uses the high registers(r4~). 
	   
    ldmia   r0,{r1-r9}
    ldr	    r0,=0x01c80004  ;BANKCON0 Address
    stmia   r0,{r1-r9}

    ldmfd   sp!,{r4-r9}

    mov	    pc,lr

NUINT	    EQU	0xc0
NUIRQ        EQU  0x80
NUFIQ        EQU  0x40

	EXPORT	Push2DisIntEnFlag
Push2DisIntEnFlag
;This function works only if the processor is in previliged mode

    mrs	    r0, cpsr
    stmdb    r13!, {r0}       ;push the value of cpsr
    orr	    r0, r0, #NUINT   ;set irq and fiq flag bit
    msr	    cpsr_cxsf, r0		

    mov	    pc,lr


	EXPORT	PopIntEnFlag
PopIntEnFlag
;This function works only if the processor is in previliged mode.

    
    ldmia    r13!, {r0}           ;pop the value of cpsr stored last time
    and      r0, r0, #NUINT       ;get irq and fiq setting in last disable interrupt entry
    mrs	    r1, cpsr
    bic	    r1, r1, #NUINT       ;clear irq and fiq flag bit 
    orr      r1, r1, r0           ;restore irq and fiq setting in last disable interrupt entry
    msr	    cpsr_cxsf, r1	

    mov	    pc,lr


	END
	
	END


