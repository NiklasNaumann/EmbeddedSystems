/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
 * The FreeRTOS kernel's RISC-V port is split between the the code that is
 * common across all currently supported RISC-V chips (implementations of the
 * RISC-V ISA), and code which tailors the port to a specific RISC-V chip:
 *
 * + The code that is common to all RISC-V chips is implemented in
 *   FreeRTOS\Source\portable\GCC\RISC-V-RV32\portASM.S.  There is only one
 *   portASM.S file because the same file is used no matter which RISC-V chip is
 *   in use.
 *
 */

#include <educore-v.asm.h>

#define portWORD_SIZE 		(4)
#define portCONTEXT_SIZE 	(30 * portWORD_SIZE)

#define portasmHANDLE_INTERRUPT		(NULL)
#ifndef portasmHANDLE_INTERRUPT
	#error portasmHANDLE_INTERRUPT must be defined to the function to be called to handle external/peripheral interrupts.  portasmHANDLE_INTERRUPT can be defined on the assmbler command line or in the appropriate freertos_risc_v_chip_specific_extensions.h header file.
#endif


.global xPortStartFirstTask
.global pxPortInitialiseStack

.extern vTaskSwitchContext

.extern pxCurrentTCB
.extern uxTimerDelay
.extern xISRStackTop

.extern default_exception_isr

/*-----------------------------------------------------------*/

.align 8
.func
freertos_risc_v_trap_handler:
	addi sp, sp, -portCONTEXT_SIZE
	sw x1,   1 * portWORD_SIZE( sp )	/* ra */
	sw x5,   2 * portWORD_SIZE( sp )	/* t0 */
	sw x6,   3 * portWORD_SIZE( sp )	/* t1 */
	sw x7,   4 * portWORD_SIZE( sp )	/* t2 */
	sw x8,   5 * portWORD_SIZE( sp )	/* s0 */
	sw x9,   6 * portWORD_SIZE( sp )	/* s1 */
	sw x10,  7 * portWORD_SIZE( sp )	/* a0 */
	sw x11,  8 * portWORD_SIZE( sp )	/* a1 */
	sw x12,  9 * portWORD_SIZE( sp )	/* a2 */
	sw x13, 10 * portWORD_SIZE( sp )	/* a3 */
	sw x14, 11 * portWORD_SIZE( sp )	/* a4 */
	sw x15, 12 * portWORD_SIZE( sp )	/* a5 */
	sw x16, 13 * portWORD_SIZE( sp )	/* a6 */
	sw x17, 14 * portWORD_SIZE( sp )	/* a7 */
	sw x18, 15 * portWORD_SIZE( sp )	/* s2 */
	sw x19, 16 * portWORD_SIZE( sp )	/* s3 */
	sw x20, 17 * portWORD_SIZE( sp )	/* s4 */
	sw x21, 18 * portWORD_SIZE( sp )	/* s5 */
	sw x22, 19 * portWORD_SIZE( sp )	/* s6 */
	sw x23, 20 * portWORD_SIZE( sp )	/* s7 */
	sw x24, 21 * portWORD_SIZE( sp )	/* s8 */
	sw x25, 22 * portWORD_SIZE( sp )	/* s9 */
	sw x26, 23 * portWORD_SIZE( sp )	/* s10 */
	sw x27, 24 * portWORD_SIZE( sp )	/* s11 */
	sw x28, 25 * portWORD_SIZE( sp )	/* t3 */
	sw x29, 26 * portWORD_SIZE( sp )	/* t4 */
	sw x30, 27 * portWORD_SIZE( sp )	/* t5 */
	sw x31, 28 * portWORD_SIZE( sp )	/* t6 */

	csrr t0, mstatus
	sw t0,  29 * portWORD_SIZE( sp )	/* mstatus */

	lw  t0, pxCurrentTCB				/* Load pxCurrentTCB. */
	sw  sp, 0( t0 )						/* Write sp to first TCB member. */
	
	csrr t0, mepc
	sw t0,   0 * portWORD_SIZE( sp )	/* mret */
	


test_if_exception:
	csrr a0, mcause						/* MSB of mcause is 1 if handing an asynchronous interrupt. */
	bge a0, zero, handle_exception		/* Branch past interrupt handing if not asynchronous. */

test_if_mtimer:			
	li t0, 0x80000007					/* == machine timer interrupt. */
	beq a0, t0, handle_mtimer

test_if_msoftware:
	li t0, 0x80000003					/* == machine software interrupt. */
	beq a0, t0, handle_msoftware

	j processed_source

handle_mtimer:
	/* Update the 64-bit mtimer compare match value in two 32-bit writes. */
	li t4, MMIO_BASE_ADDR
	lw t0, MMIO_MTIMECMP_L_OFFSET(t4)	/* Load the low word of ullNextTime into t2. */
	lw t1, MMIO_MTIMECMP_H_OFFSET(t4)	/* Load the high word of ullNextTime into t3. */
	lw t2, uxTimerDelay					/* Load the value of ullTimerIncrementForOneTick into t0 (could this be optimized by storing in an array next to pullNextTime?). */
	add t0, t0, t2						/* Add the low word of ullNextTime to the timer increments for one tick (assumes timer increment for one tick fits in 32-bits). */
	sltu t3, t0, t2						/* See if the sum of low words overflowed (what about the zero case?). */
	add t1, t1, t3						/* Add overflow to high word of ullNextTime. */
	li t3, 0xFFFFFFFF
	sw t3, MMIO_MTIMECMP_L_OFFSET(t4)	/* Max Low Half. */
	sw t1, MMIO_MTIMECMP_H_OFFSET(t4)	/* Store new high word. */
	sw t0, MMIO_MTIMECMP_L_OFFSET(t4)	/* Store new low word. */

	lw sp, xISRStackTop				/* Switch to ISR stack before function call. */
	jal xTaskIncrementTick
	beqz a0, processed_source		/* Don't switch context if incrementing tick didn't unblock a task. */

	lw sp, xISRStackTop				/* Switch to ISR stack before function call. */
	jal vTaskSwitchContext
	j processed_source

handle_msoftware:
	li t0, MMIO_BASE_ADDR
	sw zero, MMIO_MSIP_OFFSET(t0)	/* Clear Software Interrupt pending bit. */

	lw sp, xISRStackTop				/* Switch to ISR stack before function call. */
	jal vTaskSwitchContext
	j processed_source

processed_source:
	lw  t0, pxCurrentTCB			/* Load pxCurrentTCB. */
	lw  sp, 0( t0 )				 	/* Read sp from first TCB member. */

	/* Load mret with the address of the next instruction in the task to run next. */
	lw t0,    0 * portWORD_SIZE( sp )	/* mret */
	csrw mepc, t0

	/* Load mstatus with the interrupt enable bits used by the task. */
	lw  t0,  29 * portWORD_SIZE( sp )	/* mstatus */
	csrw mstatus, t0					/* Required for MPIE bit. */

	lw  x1,   1 * portWORD_SIZE( sp )	/* ra */
	lw  x5,   2 * portWORD_SIZE( sp )	/* t0 */
	lw  x6,   3 * portWORD_SIZE( sp )	/* t1 */
	lw  x7,   4 * portWORD_SIZE( sp )	/* t2 */
	lw  x8,   5 * portWORD_SIZE( sp )	/* s0 */
	lw  x9,   6 * portWORD_SIZE( sp )	/* s1 */
	lw  x10,  7 * portWORD_SIZE( sp )	/* a0 */
	lw  x11,  8 * portWORD_SIZE( sp )	/* a1 */
	lw  x12,  9 * portWORD_SIZE( sp )	/* a2 */
	lw  x13, 10 * portWORD_SIZE( sp )	/* a3 */
	lw  x14, 11 * portWORD_SIZE( sp )	/* a4 */
	lw  x15, 12 * portWORD_SIZE( sp )	/* a5 */
	lw  x16, 13 * portWORD_SIZE( sp )	/* a6 */
	lw  x17, 14 * portWORD_SIZE( sp )	/* a7 */
	lw  x18, 15 * portWORD_SIZE( sp )	/* s2 */
	lw  x19, 16 * portWORD_SIZE( sp )	/* s3 */
	lw  x20, 17 * portWORD_SIZE( sp )	/* s4 */
	lw  x21, 18 * portWORD_SIZE( sp )	/* s5 */
	lw  x22, 19 * portWORD_SIZE( sp )	/* s6 */
	lw  x23, 20 * portWORD_SIZE( sp )	/* s7 */
	lw  x24, 21 * portWORD_SIZE( sp )	/* s8 */
	lw  x25, 22 * portWORD_SIZE( sp )	/* s9 */
	lw  x26, 23 * portWORD_SIZE( sp )	/* s10 */
	lw  x27, 24 * portWORD_SIZE( sp )	/* s11 */
	lw  x28, 25 * portWORD_SIZE( sp )	/* t3 */
	lw  x29, 26 * portWORD_SIZE( sp )	/* t4 */
	lw  x30, 27 * portWORD_SIZE( sp )	/* t5 */
	lw  x31, 28 * portWORD_SIZE( sp )	/* t6 */
	addi sp, sp, portCONTEXT_SIZE

	mret

handle_exception:
	// call default_exception_isr
	1: j 1b

.endfunc
/*-----------------------------------------------------------*/

.align 8
.func
xPortStartFirstTask:
	/* Initialize trap handler. */
	la t0, freertos_risc_v_trap_handler
	csrw mtvec, t0

	/* Load stack pointer of the context of the first task. */
	lw  sp, pxCurrentTCB			/* Load pxCurrentTCB. */
	lw  sp, 0( sp )				 	/* Read sp from first TCB member. */

	/* Note for starting the scheduler the exception return address is used as the function return address. */
	lw  ra,   0 * portWORD_SIZE( sp )	/* mret */

	lw  t0,  29 * portWORD_SIZE( sp )	/* mstatus */
	csrrw  x0, mstatus, t0				/* Interrupts enabled from here! */

	lw  x5,   2 * portWORD_SIZE( sp )	/* t0 */
	lw  x6,   3 * portWORD_SIZE( sp )	/* t1 */
	lw  x7,   4 * portWORD_SIZE( sp )	/* t2 */
	lw  x8,   5 * portWORD_SIZE( sp )	/* s0 */
	lw  x9,   6 * portWORD_SIZE( sp )	/* s1 */
	lw  x10,  7 * portWORD_SIZE( sp )	/* a0 */
	lw  x11,  8 * portWORD_SIZE( sp )	/* a1 */
	lw  x12,  9 * portWORD_SIZE( sp )	/* a2 */
	lw  x13, 10 * portWORD_SIZE( sp )	/* a3 */
	lw  x14, 11 * portWORD_SIZE( sp )	/* a4 */
	lw  x15, 12 * portWORD_SIZE( sp )	/* a5 */
	lw  x16, 13 * portWORD_SIZE( sp )	/* a6 */
	lw  x17, 14 * portWORD_SIZE( sp )	/* a7 */
	lw  x18, 15 * portWORD_SIZE( sp )	/* s2 */
	lw  x19, 16 * portWORD_SIZE( sp )	/* s3 */
	lw  x20, 17 * portWORD_SIZE( sp )	/* s4 */
	lw  x21, 18 * portWORD_SIZE( sp )	/* s5 */
	lw  x22, 19 * portWORD_SIZE( sp )	/* s6 */
	lw  x23, 20 * portWORD_SIZE( sp )	/* s7 */
	lw  x24, 21 * portWORD_SIZE( sp )	/* s8 */
	lw  x25, 22 * portWORD_SIZE( sp )	/* s9 */
	lw  x26, 23 * portWORD_SIZE( sp )	/* s10 */
	lw  x27, 24 * portWORD_SIZE( sp )	/* s11 */
	lw  x28, 25 * portWORD_SIZE( sp )	/* t3 */
	lw  x29, 26 * portWORD_SIZE( sp )	/* t4 */
	lw  x30, 27 * portWORD_SIZE( sp )	/* t5 */
	lw  x31, 28 * portWORD_SIZE( sp )	/* t6 */
	addi	sp, sp, portCONTEXT_SIZE
	ret
.endfunc

/*-----------------------------------------------------------*/

/*
 * Unlike other ports pxPortInitialiseStack() is written in assembly code as it
 * needs access to the portasmADDITIONAL_CONTEXT_SIZE constant.  The prototype
 * for the function is as per the other ports:
 * StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters );
 *
 * As per the standard RISC-V ABI pxTopcOfStack is passed in in a0, pxCode in
 * a1, and pvParameters in a2.  The new top of stack is passed out in a0.
 *
 * RISC-V maps registers to ABI names as follows (X1 to X31 integer registers
 * for the 'I' profile, X1 to X15 for the 'E' profile, currently I assumed).
 *
 * Register		ABI Name	Description						Saver
 * x0			zero		Hard-wired zero					-
 * x1			ra			Return address					Caller
 * x2			sp			Stack pointer					Callee
 * x3			gp			Global pointer					-
 * x4			tp			Thread pointer					-
 * x5-7			t0-2		Temporaries						Caller
 * x8			s0/fp		Saved register/Frame pointer	Callee
 * x9			s1			Saved register					Callee
 * x10-11		a0-1		Function Arguments/return values Caller
 * x12-17		a2-7		Function arguments				Caller
 * x18-27		s2-11		Saved registers					Callee
 * x28-31		t3-6		Temporaries						Caller
 *
 * The RISC-V context is saved t FreeRTOS tasks in the following stack frame,
 * where the global and thread pointers are currently assumed to be constant so
 * are not saved:
 *
 * mstatus
 * x31
 * x30
 * x29
 * x28
 * x27
 * x26
 * x25
 * x24
 * x23
 * x22
 * x21
 * x20
 * x19
 * x18
 * x17
 * x16
 * x15
 * x14
 * x13
 * x12
 * x11
 * x10/a0 pvParameters
 * x9
 * x8
 * x7
 * x6
 * x5
 * portTASK_RETURN_ADDRESS
 * pxCode
 */
.align 8
.func
pxPortInitialiseStack:
	addi a0, a0, -portCONTEXT_SIZE
	sw x0,   1 * portWORD_SIZE( a0 )	/* ra = portTASK_RETURN_ADDRESS */
	sw x0,   2 * portWORD_SIZE( a0 )	/* t0 */
	sw x0,   3 * portWORD_SIZE( a0 )	/* t1 */
	sw x0,   4 * portWORD_SIZE( a0 )	/* t2 */
	sw x0,   5 * portWORD_SIZE( a0 )	/* s0 */
	sw x0,   6 * portWORD_SIZE( a0 )	/* s1 */
	sw x0,   7 * portWORD_SIZE( a0 )	/* a0 */
	sw x0,   8 * portWORD_SIZE( a0 )	/* a1 */
	sw a2,   9 * portWORD_SIZE( a0 )	/* a2 = pvParameters */
	sw x0,  10 * portWORD_SIZE( a0 )	/* a3 */
	sw x0,  11 * portWORD_SIZE( a0 )	/* a4 */
	sw x0,  12 * portWORD_SIZE( a0 )	/* a5 */
	sw x0,  13 * portWORD_SIZE( a0 )	/* a6 */
	sw x0,  14 * portWORD_SIZE( a0 )	/* a7 */
	sw x0,  15 * portWORD_SIZE( a0 )	/* s2 */
	sw x0,  16 * portWORD_SIZE( a0 )	/* s3 */
	sw x0,  17 * portWORD_SIZE( a0 )	/* s4 */
	sw x0,  18 * portWORD_SIZE( a0 )	/* s5 */
	sw x0,  19 * portWORD_SIZE( a0 )	/* s6 */
	sw x0,  20 * portWORD_SIZE( a0 )	/* s7 */
	sw x0,  21 * portWORD_SIZE( a0 )	/* s8 */
	sw x0,  22 * portWORD_SIZE( a0 )	/* s9 */
	sw x0,  23 * portWORD_SIZE( a0 )	/* s10 */
	sw x0,  24 * portWORD_SIZE( a0 )	/* s11 */
	sw x0,  25 * portWORD_SIZE( a0 )	/* t3 */
	sw x0,  26 * portWORD_SIZE( a0 )	/* t4 */
	sw x0,  27 * portWORD_SIZE( a0 )	/* t5 */
	sw x0,  28 * portWORD_SIZE( a0 )	/* t6 */
	
	sw a1,	 0 * portWORD_SIZE( a0 )	/* mret = pxCode */

	csrr t0, mstatus					/* Obtain current mstatus value. */
	li t1, 0x00001880					/* Generate the value 0x1880, which are the MPIE and MPP bits to set in mstatus. */
	or t0, t0, t1						/* Set MPIE and MPP bits in mstatus value. */
	sw t0,  29 * portWORD_SIZE( a0 )	/* mstatus */

	ret
.endfunc
/*-----------------------------------------------------------*/
