//
//  idt.c
//  88
//
//  Created by 学习 on 15/10/2023.
//

#include "idt.h"
#define SYSCALL 	0x80
#define RTC 		0x28
#define KEYBOARD 	0x21
#define PIT			0x20
#define DPL_KERNEL  0
#define DPL_USER    3

/* 
 * DIVIDE_ERROR_EXCEPTION
 *   DESCRIPTION: DIVIDE_ERROR_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Divide by Zero
 */
void DIVIDE_ERROR_EXCEPTION(){
    cli();
    printf("\nException Happened: Divide by Zero\n");
    while(1);
    sti();
}

/* 
 * DEBUG_EXCEPTION
 *   DESCRIPTION: DEBUG_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Debug Exception
 */
void DEBUG_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Debug Exception");
    while(1);
    sti();
}

/* 
 * NMI_INTERRUPT_EXCEPTION
 *   DESCRIPTION: NMI_INTERRUPT_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Non Maskable Interrupt Exception
 */
void NMI_INTERRUPT_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Non Maskable Interrupt Exception");
    while(1);
    sti();
}

/* 
 * BREAKPOINT_EXCEPTION
 *   DESCRIPTION: BREAKPOINT_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Breakpoint Exception
 */
void BREAKPOINT_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Breakpoint Exception");
    while(1);
    sti();
}

/* 
 * OVERFLOW_EXCEPTION
 *   DESCRIPTION: OVERFLOW_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:Exception Happened: Overflow Exception
 */
void OVERFLOW_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Overflow Exception");
    while(1);
    sti();
}

/* 
 * BOUND_RANGE_EXCEEDED_EXCEPTION
 *   DESCRIPTION: BOUND_RANGE_EXCEEDED_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Bound Range Exceeded Exception
 */
void BOUND_RANGE_EXCEEDED_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Bound Range Exceeded Exception");
    while(1);
    sti();
}

/* 
 * INVALID_OPCODE_EXCEPTION
 *   DESCRIPTION: INVALID_OPCODE_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Invalid Opcode Exception
 */
void INVALID_OPCODE_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Invalid Opcode Exception");
    while(1);
    sti();
}
/* 
 * DEVICE_NOT_AVAILABLE_EXCEPTION
 *   DESCRIPTION: DEVICE_NOT_AVAILABLE_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:Exception Happened: Device Not Available Exception
 */
void DEVICE_NOT_AVAILABLE_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Device Not Available Exception");
    while(1);
    sti();
}
/* 
 * DOUBLE_FAULT_EXCEPTION
 *   DESCRIPTION: DOUBLE_FAULT_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Double Fault Exception
 */
void DOUBLE_FAULT_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Double Fault Exception");
    while(1);
    sti();
}
/* 
 * COPROCESSOR_SEGMENT_OVERRUN
 *   DESCRIPTION: COPROCESSOR_SEGMENT_OVERRUN
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Coprocessor Segment Exception
 */
void COPROCESSOR_SEGMENT_OVERRUN(){
	cli();
	clear();
    printf("Exception Happened: Coprocessor Segment Exception");
    while(1);
    sti();
}
/* 
 * INVALID_TSS_EXCEPTION
 *   DESCRIPTION: INVALID_TSS_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Invalid TSS Exception
 */
void INVALID_TSS_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Invalid TSS Exception");
    while(1);
    sti();
}
/* 
 * SEGMENT_NOT_PRESENT
 *   DESCRIPTION: SEGMENT_NOT_PRESENT
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Segment Not Present
 */
void SEGMENT_NOT_PRESENT(){
	cli();
	clear();
    printf("Exception Happened: Segment Not Present");
    while(1);
    sti();
}
/* 
 * STACK_FAULT_EXCEPTION
 *   DESCRIPTION: STACK_FAULT_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Stack Fault Exception
 */
void STACK_FAULT_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Stack Fault Exception");
    while(1);
    sti();
}
/* 
 * GENERAL_PROTECTION_FAULT
 *   DESCRIPTION: GENERAL_PROTECTION_FAULT
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: General Protection Exception
 */
void GENERAL_PROTECTION_FAULT(){
	cli();
	clear();
    printf("Exception Happened: General Protection Exception");
    while(1);
    sti();
}
/* 
 * PAGE_FAULT_EXCEPTION
 *   DESCRIPTION: PAGE_FAULT_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Page Fault Exception\
 */
void PAGE_FAULT_EXCEPTION(){
	cli();
    printf("\nException Happened: Page Fault Exception\n");
    while(1);
    sti();
}

/* 
 * x87_FPU_FLOATIMG_POINT_ERROR
 *   DESCRIPTION: x87_FPU_FLOATIMG_POINT_ERROR
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Floating Point Exception
 */
void x87_FPU_FLOATIMG_POINT_ERROR(){
	cli();
	clear();
    printf("Exception Happened: Floating Point Exception");
    while(1);
    sti();
}
/* 
 * ALIGNMENT_CHECK_EXCEPTION
 *   DESCRIPTION: ALIGNMENT_CHECK_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Alignment Check Exception
 */
void ALIGNMENT_CHECK_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Alignment Check Exception");
    while(1);
    sti();
}
/* 
 * MACHINE_CHECK_EXCEPTION
 *   DESCRIPTION: MACHINE_CHECK_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: Machine Check Exception
 */
void MACHINE_CHECK_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: Machine Check Exception");
    while(1);
    sti();
}
/* 
 * SIMD_FLOATING_POINT_EXCEPTION
 *   DESCRIPTION: SIMD_FLOATING_POINT_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Exception Happened: SIMD Floating Poicase
 */
void SIMD_FLOATING_POINT_EXCEPTION(){
	cli();
	clear();
    printf("Exception Happened: SIMD Floating Poicase" );
    while(1);
    sti();
}
/* 
 * SYSTEM_CALL
 *   DESCRIPTION: SYSTEM_CALL
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: System Call Happened
 */
void SYSTEM_CALL(){
	cli();
    printf("\nSystem Call Happened\n" );
    while(1);
    sti();
}


/* 
 * interrupt_init
 *   DESCRIPTION: interrupt_init
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: interrupt_init
 */
void interrupt_init(void){
    int i;
    for (i = 0; i < NUM_VEC; i++){
        if ( i > 19 && i < 32) {
            idt[i].present = 0;
            continue;
        } 
        idt[i].seg_selector = KERNEL_CS;
        idt[i].reserved0 = 0;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = (i < 32 && i != 2) ? 1 : 0;
        idt[i].reserved4 = 0;
        
        idt[i].size      = 1;
        idt[i].dpl       = DPL_KERNEL; 
        idt[i].present   = 1;
    }
    SET_IDT_ENTRY(idt[0], DIVIDE_ERROR_EXCEPTION);
	SET_IDT_ENTRY(idt[1], DEBUG_EXCEPTION);
	SET_IDT_ENTRY(idt[2], NMI_INTERRUPT_EXCEPTION);
	SET_IDT_ENTRY(idt[3], BREAKPOINT_EXCEPTION);
    idt[3].dpl = DPL_USER;
	SET_IDT_ENTRY(idt[4], OVERFLOW_EXCEPTION);              
    idt[4].dpl = DPL_USER;
	SET_IDT_ENTRY(idt[5], BOUND_RANGE_EXCEEDED_EXCEPTION);  
    idt[5].dpl = DPL_USER;
	SET_IDT_ENTRY(idt[6], INVALID_OPCODE_EXCEPTION);
	SET_IDT_ENTRY(idt[7], DEVICE_NOT_AVAILABLE_EXCEPTION);
	SET_IDT_ENTRY(idt[8], DOUBLE_FAULT_EXCEPTION);
	SET_IDT_ENTRY(idt[9], COPROCESSOR_SEGMENT_OVERRUN);
	SET_IDT_ENTRY(idt[10], INVALID_TSS_EXCEPTION);
	SET_IDT_ENTRY(idt[11], SEGMENT_NOT_PRESENT);
	SET_IDT_ENTRY(idt[12], STACK_FAULT_EXCEPTION);
	SET_IDT_ENTRY(idt[13], GENERAL_PROTECTION_FAULT);
	SET_IDT_ENTRY(idt[14], PAGE_FAULT_EXCEPTION);
	SET_IDT_ENTRY(idt[16], x87_FPU_FLOATIMG_POINT_ERROR);
	SET_IDT_ENTRY(idt[17], ALIGNMENT_CHECK_EXCEPTION);
	SET_IDT_ENTRY(idt[18], MACHINE_CHECK_EXCEPTION);
    SET_IDT_ENTRY(idt[19], SIMD_FLOATING_POINT_EXCEPTION);

    SET_IDT_ENTRY(idt[SYSCALL], sys_call_linkage);
    idt[SYSCALL].dpl = DPL_USER; idt[SYSCALL].reserved3 =1; // if don't add reserved3 = 1, when call syscall can't type anything

    SET_IDT_ENTRY(idt[RTC], rtc_handler_linkage);
    idt[RTC].dpl = DPL_KERNEL;idt[RTC].reserved3 = 0; 

    SET_IDT_ENTRY(idt[KEYBOARD], keyboard_handler_linkage);
    idt[KEYBOARD].dpl = DPL_KERNEL;idt[KEYBOARD].reserved3 = 0;
    
    SET_IDT_ENTRY(idt[PIT], pit_handler_linkage); 
    idt[PIT].dpl = DPL_KERNEL;idt[PIT].reserved3 = 0; 
}


