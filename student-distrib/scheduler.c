#include "scheduler.h"

int32_t scheduler_array[NUM_SCHEDULERS] = {UNINITIALIZED_TERM, UNINITIALIZED_TERM, UNINITIALIZED_TERM};
int32_t current_scheduler_index = 0;
/* 
 * pit_init
 *   DESCRIPTION: Initialize the pit_init
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Initialize the pit_init
 */
void pit_init(void) {
    outb(PIT_CONTROL_WORD, PIT_COMMAND_PORT);
    outb(PIT_FREQUENCY & LOW_BYTE_MASK, PIT_DATA_PORT);
    outb((PIT_FREQUENCY >> HIGH_BYTE_SHIFT) & LOW_BYTE_MASK, PIT_DATA_PORT);
    enable_irq(PIT_IRQ);
}
/* 
 * pit_handler
 *   DESCRIPTION: pit_handler
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: pit_handler
 */
void pit_handler(void) {
    send_eoi(PIT_IRQ);
    process_scheduler();
}
/* 
 * process_scheduler
 *   DESCRIPTION: process_scheduler
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: process_scheduler
 */
void process_scheduler(void) {
    int32_t current_pid = get_current_pid();
    pcb* current_pcb = get_pcb(current_pid);
    register uint32_t saved_ebp asm("ebp");
    current_pcb->scheduler_ebp = saved_ebp;

    current_scheduler_index = (current_scheduler_index + 1) % NUM_SCHEDULERS;
    int32_t next_pid = scheduler_array[current_scheduler_index];

    if (next_pid == UNINITIALIZED_TERM) {
        remap_vid_page(current_scheduler_index);
        execute((uint8_t*)"shell");
    }

    set_page(next_pid);
    remap_vid_page(current_scheduler_index);

    tss.ss0 = KERNEL_DS;
    tss.esp0 = (uint32_t)get_pcb(next_pid - 1) - 4;

    pcb* next_pcb = get_pcb(next_pid);
    asm volatile(
        "movl %0, %%ebp \n\
         leave          \n\
         ret            \n"
        : 
        : "r"(next_pcb->scheduler_ebp)
        : "ebp");
}

