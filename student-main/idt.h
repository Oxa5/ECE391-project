//
//  idt.h
//  88
//
//  Created by 学习 on 15/10/2023.
//

#ifndef idt_h
#define idt_h

//#include <stdio.h>
#include "lib.h"
#include "asm_linkage.h"
#include "x86_desc.h"
#include "signal.h"
void DIVIDE_ERROR_EXCEPTION();
void DEBUG_EXCEPTION();
void NMI_INTERRUPT_EXCEPTION();
void BREAKPOINT_EXCEPTION();
void OVERFLOW_EXCEPTION();
void BOUND_RANGE_EXCEEDED_EXCEPTION();
void INVALID_OPCODE_EXCEPTION();
void DEVICE_NOT_AVAILABLE_EXCEPTION();
void DOUBLE_FAULT_EXCEPTION();
void COPROCESSOR_SEGMENT_OVERRUN();
void INVALID_TSS_EXCEPTION();
void SEGMENT_NOT_PRESENT();
void STACK_FAULT_EXCEPTION();
void GENERAL_PROTECTION_FAULT();
void PAGE_FAULT_EXCEPTION();
void x87_FPU_FLOATIMG_POINT_ERROR();
void ALIGNMENT_CHECK_EXCEPTION();
void MACHINE_CHECK_EXCEPTION();
void SIMD_FLOATING_POINT_EXCEPTION();
void SYSTEM_CALL();
void interrupt_init();

#endif /* idt_h */
