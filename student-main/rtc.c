#include "rtc.h"
volatile int32_t rtc_interrupt[3]; // Flag to indicate if a **virtual** RTC interrupt has occurred.
int32_t virtual_freq[3];                  // This is VIRTUALIZED (actual freq is always 1024 Hz)
volatile int32_t rtc_counter;



/* Ports that RTC sits on */
#define RTC_REG_PORT    0x70
#define RTC_DATA_PORT    0x71
#define RTC_IRQ_NUM 8

/* Select REG a,b,c with mask NMI */
#define REG_A 0x8A
#define REG_B 0x8B
#define REG_C 0x8C

/* About frequency */
#define ACTUAL_RATE 6
#define ACTUAL_FREQ 1024
  
/* 
 * rtc_init
 *   DESCRIPTION: Initialize the rtc_init, Set to allow periodic interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Initialize the rtc_init,Turn on IRQ at 1024hz by default
 */
void rtc_init(){
 
    int32_t rate;
    char prev,prev2;
    int32_t i;
    //disable_irq(RTC_IRQ);   //no interrupts happen??

    //disable NMI
    outb(inb(RTC_PORT)|0x80, RTC_PORT);//set 0x80 to disable NMI
    //set 3bit: 1=time/date in binary
    outb(RTC_REGB, RTC_PORT);
    prev = inb(RTC_DATA);//Stores the original value in the RTC_REGB register
    outb(RTC_REGB, RTC_PORT);
    //Set to allow periodic interrupt
    outb(prev|0x40, RTC_DATA);// set 6bit: 1=enable periodic interrupt, 0=disable，With all other bits unchanged, write to RTC_REGB 
   
    for(i = 0; i<3; i++){
        virtual_freq[i] = 2;
    }
    for(i = 0; i<3; i++){
        rtc_interrupt[i] = 0;
    }
   
    rtc_counter = 0;


    
    // Set the interrupt frequency of Reg_A - the true interrupt frequency of rtc, Default = 1024
    //cp2
    rate =  get_rtc_rate(real_interrupt_freq);// 
    //update the rate and fre
    outb(RTC_REGA,RTC_PORT);		               
    prev2=inb(RTC_DATA);//Stores the original value of RTC_REGA	                
    outb(RTC_REGA,RTC_PORT);		                
    outb((prev2 & 0xF0) | rate, RTC_DATA); //original value high4bits | （rate low4bits：rate divider output） 

    // turn on IRQ8
    enable_irq(8);//Be sure to install the IRQ handler IRQ before enabling RTC.

}


//cp2
 // get rate from input-frequency 
 /* 
 * get_rtc_rate(int32_t frequency)
 *   DESCRIPTION: Calculate the freq rate by bit caculation
 *   INPUTS: frequency
 *   OUTPUTS: none
 *   RETURN VALUE: return -1 if freq is out of range or not a power of 2, result rate else
 *   SIDE EFFECTS: none
 */   
int32_t get_rtc_rate(int32_t frequency){

    int32_t rate = 0;
    int32_t virtual_freq = RTC_MAX_RATE / frequency;//32768/freq
    
	while(virtual_freq > 0)
	{
		rate = rate + 1;
		virtual_freq= virtual_freq >> 1;
	}

    //start function 
    rate = rate & 0x0F;

    if (frequency >= 2 && ((frequency & (frequency - 1)) == 0) && (frequency <= real_interrupt_freq)) //2<= freq <=1024
    {
        return rate;
    }
    
    return -1;//frequency must be power of 2？
}


/* 
 * rtc_handler
 *   DESCRIPTION: rtc_handler with virtualization 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: rtc_handler
 */
void rtc_handler(){
    cli();
    char prev3;
    int32_t i;
    rtc_counter++;
    // if (rtc_counter ==(real_interrupt_freq / virtual_freq))  //if Reach the virtualization multiple
    // {
    //     rtc_interrupt = 1;//set interrupt signal as one times of interrupt
    //     rtc_counter = 0;//clean up 
    // } this is wrong
    for(i = 0; i<3 ; i++){
        if (rtc_counter % (real_interrupt_freq / virtual_freq[i]) == 0) rtc_interrupt[i] = 1;
    }
    
    // Read back Reg_C to make sure you get another interrupt
    outb(RTC_REGC, RTC_PORT);
    prev3 = inb(RTC_DATA);// just throw away contents
    send_eoi(8);// sent eoi
    //send_eoi(8);: sends the End of Interrupt (EOI) instruction to the interrupt controller to notify it that the RTC interrupt has ///been processed.
    sti();

}



/* 
 * rtc_open
 *   DESCRIPTION: open rtc and reset freq to 2hz
 *   INPUTS: filename
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *   SIDE EFFECTS: open rtc and reset freq to 2hz
 */
int rtc_open(const uint8_t* filename){
    virtual_freq[current_scheduler_index] = 2;//reset freq to 2Hz 
    rtc_interrupt[current_scheduler_index] = 0;//clean-up when open
    //rtc_counter = 0;//1.??？Whether to set zero when opened??
	return 0;


}


/* 
 * rtc_close
 *   DESCRIPTION:close rtc 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *   SIDE EFFECTS: close rtc
 */
int rtc_close(int32_t fd) {
    //printf("5555\n");
	return 0;
}


/* 
 * rtc_read
 *   DESCRIPTION: read rtc and return only after interrupt
 *   INPUTS: int32_t fd, void* buf, int32_t nbytes
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success
 *   SIDE EFFECTS: none
 */
int rtc_read(int32_t fd, void* buf, int32_t nbytes){
    rtc_interrupt[current_scheduler_index] = 0;
	while (!rtc_interrupt[current_scheduler_index]);	// return at the virtual interrupt
	return 0;

}


/* 
 * rtc_write
 *   DESCRIPTION: change rtc freq 
 *   INPUTS: int32_t fd, const void* buf, int32_t nbytes
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success, -1 for fail
 *   SIDE EFFECTS: change rtc freq
 */
int rtc_write(int32_t fd, const void* buf, int32_t nbytes) {

    cli();
	int32_t freq_input = *(int32_t*) buf;
    int32_t new_rate = get_rtc_rate(freq_input);
    if (new_rate == -1){ 
        sti();  
        return -1; } // input freq Out of range or not a power of two

    virtual_freq[current_scheduler_index] = freq_input;//virtualize 
    sti();
    return 0;

}
