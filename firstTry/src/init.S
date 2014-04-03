
/*
 * init.S
 *
 *  Created on: 14 Oct 2010
 *      Author: psampaio
 */

/*****************************************************************
	Module includes the interrupt vectors and start-up code.
 ******************************************************************/

.section .startup, "ax"

.equ MODE_IRQ,0x12
.equ I_BIT, 1<<7
.equ MODE_SVR,0x13
.equ MODE_SYS,0x1f


.global _start

/*
	Exception Vectors
*/
_vectors:
	ldr     PC, ResetAddr
    ldr     PC, UndefAddr
    ldr     PC, SWIAddr
    ldr     PC, PAbtAddr
    ldr     PC, DAbtAddr
    .word   0
    ldr 	pc, [PC, #-0xFF0]
    ldr     PC, FIQAddr

ResetAddr:		.word   ResetHandler
UndefAddr:     	.word   UndefHandler
SWIAddr:       	.word   SWIHandler
PAbtAddr:      	.word   PAbortHandler
DAbtAddr:      	.word   DAbortHandler
			   	.word	0
IRQAddr:	   	.word	IRQHandler
FIQAddr:       	.word   FIQHandler

ResetHandler:
_start:
	ldr r0, =__stack_top__
	msr CPSR_c, #MODE_IRQ|I_BIT
	mov sp,r0
	msr CPSR_c, #MODE_SYS|I_BIT
	sub r0, r0, #100
	mov sp,r0

	/* Clear .bss section */
	ldr   	r1, =__bss_start__
	ldr   	r2, =__bss_end__
	ldr   	r3, =0
bss_clear_loop:
	cmp   	r1, r2
	strne 	r3, [r1], #+4
	bne   	bss_clear_loop

	/* Initialise .data section */
	ldr		r1, =__data_start__	    	/* Start of memory block */
	ldr		r2, =__rom_data_start__	    /* End of memory block */
	ldr		r3, =__data_end__
	cmp		r1, r2
	beq		end_set_loop
set_loop:
	cmp		r1, r3
	ldrne	r4, [r2], #4
	strne	r4, [r1], #4
	bne		set_loop
end_set_loop:

    ldr   	r10, =main
    mov   	lr, pc
    bx    	r10                       /* enter main() */

    b		.

/****************************************************************************/
/*                         Stack area                                       */
/****************************************************************************/
.section .stack
	.space	1024 * 4
__stack_top__:

/****************************************************************************/
/*                         Default interrupt handler                        */
/****************************************************************************/

.text
UndefHandler:
   b 		UndefHandler

SWIHandler:
   b 		SWIHandler

PAbortHandler:
   b 		PAbortHandler

DAbortHandler:
   b 		DAbortHandler

IRQHandler:
   b		IRQHandler

FIQHandler:
   b 		FIQHandler


.global timer0ISRH
timer0ISRH:
	sub lr, lr, #4
	stmfd sp!, {r0-r12, lr}
	ldr r1, =timer0ISR
	mov lr, pc
	bx r1
	ldmfd sp!, {r0-r12, pc}^


.global setEnableIntr
setEnableIntr:
	mrs r0, CPSR
	bic r0, r0, #I_BIT
	msr CPSR, r0
	mov pc, lr

.end
