/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */
/* 
 * i8259_init
 *   DESCRIPTION: Initialize the 8259 PIC
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Initialize the 8259 PIC
 */
/* Initialize the 8259 PIC */
void i8259_init(void) {
    
    master_mask = 0xFF;
    slave_mask = 0xFF;
    // mask the interrupt
    outb(INIT_MASK, MASTER_8259_DATA);// mask all of IRQs0-7
    outb(INIT_MASK, SLAVE_8259_DATA);// maks all of IRQs 8-15
    // initial the pic ICW1-4
    //master pic
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW2_MASTER, MASTER_8259_DATA);
    outb(ICW3_MASTER, MASTER_8259_DATA);
    outb(ICW4, MASTER_8259_DATA);
    // slave pic
    outb(ICW1, SLAVE_8259_PORT);
    outb(ICW2_SLAVE, SLAVE_8259_DATA);
    outb(ICW3_SLAVE, SLAVE_8259_DATA);
    outb(ICW4, SLAVE_8259_DATA);

    enable_irq(2);//enable the slave pic
    
}
/* 
 * enable_irq
 *   DESCRIPTION: Enable (unmask) the specified IRQ
 *   INPUTS: uint32_t irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Enable (unmask) the specified IRQ
 */
/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    // master pic
    if (irq_num >= 0 && irq_num <= MAX_MASTER_IRQ_NUM) {
        master_mask = master_mask & (~(1 << irq_num));
		outb(master_mask, (MASTER_8259_DATA));
        return;
        //slave pic
    }else if (irq_num >MAX_MASTER_IRQ_NUM && irq_num <= MAX_IRQ_NUM ) {
        irq_num = irq_num - 8;
		slave_mask = slave_mask & (~(1 << irq_num));
        outb(slave_mask, SLAVE_8259_DATA);
        return;
    }else{
        return;
    }

}
/* 
 * disable_irq
 *   DESCRIPTION: Disable (mask) the specified IRQ 
 *   INPUTS: uint32_t irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:Disable (mask) the specified IRQ 
 */
/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    // master pic
    if (irq_num >= 0 && irq_num <= MAX_MASTER_IRQ_NUM) {
        master_mask = master_mask | (1 << irq_num);
		outb(master_mask, (MASTER_8259_DATA));
        return;
        //slave pic
    }else if (irq_num >MAX_MASTER_IRQ_NUM && irq_num <= MAX_IRQ_NUM ) {
		irq_num = irq_num - 8;  
        slave_mask = slave_mask | (1 << irq_num);
        outb(slave_mask, (SLAVE_8259_DATA));
        return;
    }else{
        return;
    }
}
/* 
 * send_eoi
 *   DESCRIPTION: Send end-of-interrupt signal for the specified IRQ
 *   INPUTS: uint32_t irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: Send end-of-interrupt signal for the specified IRQ
 *   SIDE EFFECTS: none
 */
/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    //master pic
    if(irq_num >= 0 && irq_num <= MAX_MASTER_IRQ_NUM) {
		outb((EOI | irq_num), MASTER_8259_PORT);
        return;
	}else if(irq_num >MAX_MASTER_IRQ_NUM && irq_num <= MAX_IRQ_NUM)
	{
        //slave pic
        irq_num = irq_num - 8;
        outb(EOI | (irq_num ), SLAVE_8259_PORT);
        outb(EOI | SLAVE_PORT, MASTER_8259_PORT);
        return;
	}else{
        return;
    }
}
