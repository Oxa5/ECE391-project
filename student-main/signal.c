#include "lib.h"
#include "signal.h"
#include "scheduler.h"
#include "sys_call.h"
#include "terminal.h"


int get_sigreturn_length(){
    return DO_SIG_RETURN_END - DO_SIG_RETURN;
}



void kill_the_task(){
    int i;
    pcb* cur_pcb;
    int32_t cur_pid;
    cur_pid = get_current_pid();
    cur_pcb = get_pcb(cur_pid);
    for (i = 0; i < SIGNUM; i++){
        cur_pcb->sig_mask[i] = SIG_UNMASK;
    }
    clear();
    halt(0);
    return;
}


void ignore(){
    int i;
    pcb* cur_pcb;
    int32_t cur_pid;
    cur_pid = get_current_pid();
    cur_pcb = get_pcb(cur_pid);
    for (i = 0; i < SIGNUM; i++){
        cur_pcb->sig_mask[i] = SIG_UNMASK;}
    return ;
}


void handle_signal(){
    pcb* cur_pcb;
    int32_t cur_pid;
    
    uint32_t i, j;  

   
    uint32_t sig_flag = 0;



    void* sig_handler;
    //register uint32_t saved_ebp asm("ebp");
    cur_pid = get_current_pid();
    cur_pcb = get_pcb(cur_pid);


    for (i = 0; i < SIGNUM; i++){
        if (cur_pcb->signal[i] == SIG_ACTIVATE){
            for (j = 0; j < SIGNUM; j++){
                cur_pcb->sig_mask[j] = SIG_MASK;
            }

            sig_flag = 1;
            cur_pcb->signal[i] = SIG_DEACTIVATE;
            break;
        }
    }
    if (sig_flag){
        
        sig_handler = cur_pcb->signal_handler[i];

        if (sig_handler == kill_the_task || sig_handler == ignore){
            ((void(*)())sig_handler)();
            return;
        }
        
    }
        return;// 

}
