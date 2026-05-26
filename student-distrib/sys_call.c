#include "sys_call.h"

uint32_t process_[MAX_PROCESS_NUM] = {0};

/*
 * halt
 *   DESCRIPTION: The halt system call terminates a process, 
                  returning the specified value to its parent process.
 *   INPUTS: status - the status to return to the parent process
 *   OUTPUTS: none
 *   RETURN VALUE: process's current status
 *   SIDE EFFECTS: Halt a process
 */
int32_t halt(uint8_t status){
    uint32_t i, result = 0;
    if (status == 255)
        result = HALT_RETURN; 

    int32_t current_pid = get_current_pid();
    pcb* current_pcb = get_pcb(current_pid);

    if (current_pid == 0 || current_pid == 1|| current_pid == 2) {// forget to add current_pid == 1 and current_pid == 2 page fault happened
        process_[current_pid] = 0; 
        printf("Can't not close base shell, open a new shell for you\n");          
        execute((uint8_t*)"shell"); 
    }

    for (i = 0; i < 8; i++)
    {
        if (current_pcb->file_array[i].flags == 1) {
            current_pcb->file_array[i].file_operation.close(i);
            current_pcb->file_array[i].flags = 0;
        }
    }
     /* Deactivate current process */
    process_[current_pid] = 0;
    for(i = 0; i < 3; i++){
        if(scheduler_array[i] == current_pid)
            scheduler_array[i] = current_pcb->parent_pid;
    }

    process_[current_pid] = 0;
    pcb* parent_pcb = get_pcb(current_pcb->parent_pid);

	set_page(current_pcb->parent_pid);
    tss.ss0 = KERNEL_DS;
    tss.esp0 = EIGHT_MB - parent_pcb->current_pid * EIGHT_KB - 4; 

    asm volatile("movl %0, %%eax \n\
                  movl %1, %%ebp \n\
                  movl %2, %%esp \n\
                  leave          \n\
                  ret            \n"
                : /* no output */
                : "r" (result), \
                  "r" (current_pcb->current_ebp), \
                  "r" (current_pcb->current_esp)
                : "eax", "ebp", "esp");
     return 0;
}

/*
 * execute
 *   DESCRIPTION: The execute system call attempts to load and 
 *                execute a new program, handing off the processor 
*                  to the new program until it terminates
 *   INPUTS: command -  a space-separated sequence of words
 *   OUTPUTS: none
 *   RETURN VALUE:  0 to 255 by calling halt , 256 by exception
 *   SIDE EFFECTS: none
 */
int32_t execute(const uint8_t* command){
    //printf("enter excute\n");
    int i = 0;// for command
    int32_t file_start_address;
    int j = 0; // for file name
    int z = 0;// for arguments
    int pid_count = 0;
    uint8_t filename[MAX_FILE_NAME_NUM] = {'\0'}; // initial them to be null
    // uint8_t arg1[MAX_ARG_NUM] = {'\0'}; // initial them to be null
    // uint8_t arg2[MAX_ARG_NUM] = {'\0'}; // initial them to be null
    // uint8_t arg3[MAX_ARG_NUM] = {'\0'}; // initial them to be null
    uint8_t arg_tot[MAX_ARG_NUM+1] = {'\0'}; // initial them to be null
    uint8_t ELF_buf[MAGIC_NUM] = {'\0'};
    dentry_t execute_dentry;
    pcb* pcb_;
    // first check if it is a valid input command
    if(command == NULL){
        return -1;
    }
    while(command[i] == ' '){
        i++;
    }
    //printf("%d\n",i);
    //then get the file name
    for(; i < strlen((int8_t*)command)&&(command[i] != '\0')&&(command[i] != ' ')&& (i < MAX_ARG_NUM-1); i++){
        if(j < MAX_FILE_NAME_NUM){
            filename[j] = command[i];
            j++;
        }else{
            return -1;
        }
    }
    // printf("length of filename ");
    // printf("%d\n",j);
    // check if there are any ' ' between file name and arguments
    while(command[i] == ' '){
        i++;//for '  '
    }

    for(z = 0; i < strlen((int8_t*)command)&&(command[i] != '\0')&& (i < MAX_ARG_NUM); i++){
        arg_tot[z] = command[i];// get the args after command and " "
        z++;
    }
    

    // check my file name and argument are right
    // then check if it is a valid file 
   // printf("3\n");
    if(read_dentry_by_name(filename, &execute_dentry) == -1){
        return -1;
    }
    // then check if we can read the first 4 bytes
    //printf("%d",read_data(execute_dentry.inode, 0, ELF_buf, 5) );
    //printf("4\n");
    if(read_data(execute_dentry.inode, 0, ELF_buf, 4) != 4){
        return -1;
    }
    // printf("3");
    // then check the magic number
    //printf("5\n");
    if(ELF_buf[0] != MAGIC_NUM_ONE) return -1;
    if(ELF_buf[1] != MAGIC_NUM_TWO) return -1;
    if(ELF_buf[2] != MAGIC_NUM_THREE) return -1;
    if(ELF_buf[3] != MAGIC_NUM_FOUR) return -1;
    // then get the start point of process
    read_data(execute_dentry.inode , 24, (uint8_t*)&file_start_address, 4);

    // printf("4");
    // Then create pcb
    // get pid
    for(pid_count = 0; pid_count <= MAX_PROCESS_NUM; pid_count ++){
        if(pid_count == MAX_PROCESS_NUM){
            return -1;
        }
        if(process_[pid_count] == 0){
            // already get the pid then set it to be 1
            process_[pid_count] = 1;
            break;
        }
    }
    // printf("5");
    // the get the pcb address 
   // printf("6\n");
    pcb_ = get_pcb(pid_count);
    pcb_->current_pid = pid_count;
    if(pid_count == 0|| pid_count == 1 || pid_count == 2){
        /* If target pid = 0, which means it is shell, the parent is 0 itself. */
        pcb_->parent_pid = 255;
    }else{
        /* If target pid is not 0, assign parent pid as current pid. */
        pcb_->parent_pid = get_current_pid();
    }
    /* Set schedule array */
    for(i = 0; i < 3; i++){
        if(scheduler_array[i] == UNINITIALIZED_TERM || scheduler_array[i] == pcb_->parent_pid){
            scheduler_array[i] = pcb_->current_pid;
            break;
        }
    }
    // stdin stdout
    pcb_->file_array[0].file_operation = stdin_operation;
    pcb_->file_array[0].inode = 0;
    pcb_->file_array[0].file_position = 0;
    pcb_->file_array[0].flags = 1;


    pcb_->file_array[1].file_operation = stdout_operation;
    pcb_->file_array[1].inode = 0;
    pcb_->file_array[1].file_position = 0;
    pcb_->file_array[1].flags = 1;


//????

    for (i = 0; i < SIGNUM; i++){
        pcb_->signal[i] = SIG_DEACTIVATE;
        if (i <= 2)
            pcb_->signal_handler[i] = kill_the_task;
        else
            pcb_->signal_handler[i] = ignore;

        pcb_->sig_mask[i] = SIG_UNMASK;
    }
    memcpy(pcb_->args_buffer, arg_tot, MAX_ARG_NUM + 1);// copy the args from arg_tot to pcb_->args_buffer

    set_page(pid_count);// set page for user
    read_data(execute_dentry.inode , 0, (uint8_t*)PRO_IMG, STACK - PRO_IMG);
    register uint32_t saved_ebp asm("ebp");
    register uint32_t saved_esp asm("esp");
    
    tss.ss0 = KERNEL_DS;
    tss.esp0 = (uint32_t)get_pcb(pid_count - 1) - 4; 
    pcb_ -> current_esp = saved_esp;
    pcb_ -> current_ebp = saved_ebp;
    
    sti();
    //printf("7\n");
    asm volatile("pushl %0 \n\
                  pushl %1 \n\
                  pushfl   \n\
                  pushl %2 \n\
                  pushl %3 \n\
                  iret     \n"
                :
                : "r" (USER_DS), \
                  "r" (0x8400000 - sizeof(uint32_t)), \
                  "r" (USER_CS), \
                  "r" (file_start_address)
                : "memory");

    printf("exit exectue");
    return 0;
}

/*
 * read
 *   DESCRIPTION: The read system call reads data from the 
 *                keyboard, a file, device (RTC), or directory
 *   INPUTS: fd - the file descriptor we read from
 *			 buf - the buffer to be written
 *           length - the length of bytes to br read
 *   OUTPUTS: the words input from keyboard
 *   RETURN VALUE: number of bytes read
 *   SIDE EFFECTS: none
 */
int32_t read(int32_t fd, void* buf, int32_t length){
    int32_t pid = get_current_pid();
    pcb* current_pcb = get_pcb(pid);
    int32_t result;
	/* Can't read if args are invalid */
	if (fd < 0 || fd >= MAX_FILE_NUM || buf == NULL || length <= 0 || current_pcb->file_array[fd].flags == 0) {
		return -1;
	}

	result = current_pcb->file_array[fd].file_operation.read(fd, buf, length);
    current_pcb->file_array[fd].file_position += result;
    return result;
}

/*
 * write
 *   DESCRIPTION: The write system call writes data 
 *                to the terminal or to a device
 *   INPUTS: fd - the file descriptor we write to
 *			 buf - the buffer to be written
 *           length - the length of bytes to br read
 *   OUTPUTS: the words input from keyboard
 *   RETURN VALUE: number of bytes read
 *   SIDE EFFECTS: none
 */
int32_t write(int32_t fd, const void* buf, int32_t length){
    int32_t pid = get_current_pid();
    pcb* current_pcb = get_pcb(pid);
    int32_t result;
    /* Can't read if args are invalid */
    if (fd < 0 || fd >= MAX_FILE_NUM || buf == NULL || length <= 0 || current_pcb->file_array[fd].flags == 0) {
        return -1;
    }

    result = current_pcb->file_array[fd].file_operation.write(fd, buf, length);
	return result;
}
 
 /*
 * open
 *   DESCRIPTION: The open system call provides 
 *                access to the file system
 *   INPUTS: file - the file name to open
 *   OUTPUTS: none
 *   RETURN VALUE: the filename on success, otherwise -1
 *   SIDE EFFECTS: none
 */
int32_t open(const uint8_t* file){
    int i;
    dentry_t dentry;
    // check whether the filename is valid
    if (read_dentry_by_name(file, &dentry) == -1) {
        return -1;
    }
    // get current pcb
    int32_t pid = get_current_pid();
    pcb* current_pcb = get_pcb(pid);
    // check whether number of opened files reach max
    for (i = 0; i < MAX_FILE_NUM; i++){
        if (current_pcb->file_array[i].flags == 0){
            current_pcb->file_array[i].flags = 1;
            current_pcb->file_array[i].inode = dentry.inode;
            current_pcb->file_array[i].file_position = 0;
            break;
        }
    }
    if (i == MAX_FILE_NUM){ 
        return -1;
    }

    switch(dentry.file_type){
        case 0:// 0 : file_type of rtc
            current_pcb->file_array[i].file_operation = rtc_operation;
            break;

        case 1://  1 file_type of directory
            current_pcb->file_array[i].file_operation= directory_operation;
            break;
        
        case 2://  2 file_type of regular file
            current_pcb->file_array[i].file_operation = other_file_operation;
            break;
        default://  file_type of abnormal
            current_pcb->file_array[i].flags = 0;
            return -1;
    }


    current_pcb->file_array[i].file_operation.open(file);
    return i;
}

/*
 * close
 *   DESCRIPTION: The close system call closes the specified file 
 *                descriptor and makes it available for return 
 *                from later calls to open.
 *   INPUTS: fd - the file descriptor
 *   OUTPUTS: none
 *   RETURN VALUE: 0 on success, otherwise -1
 *   SIDE EFFECTS: none
 */
int32_t close(int32_t fd){
    // can't kill stdin and stdout
    if (fd <= 1 || fd >= 8) {
        return -1;
    }
    int32_t pid = get_current_pid();
    pcb* pcb = get_pcb(pid);
    // check whether fd is valid
    if (pcb->file_array[fd].flags == 0) {
        return -1;
    }
    pcb->file_array[fd].file_operation.close(fd);
    pcb->file_array[fd].flags = 0;
    return 0;
}

/*
 * getargs
 *   DESCRIPTION: get the agrs from current pcb and store in buf
 *   INPUTS: uint8_t* buf, int32_t nbytes
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success, 1 for fail
 *   SIDE EFFECTS: none
 */
int32_t getargs(uint8_t* buf, int32_t nbytes){
    int32_t pid = get_current_pid();
    pcb* cur_pcb = get_pcb(pid);//get the current process control block 
    if (cur_pcb == NULL || cur_pcb->args_buffer[0] == '\0' ){
        return -1;//check invaild pcb
    }

    if (buf != NULL && nbytes > 0 &&  buf >= (uint8_t*) USER_SPACE) {
        int32_t buf_size = MAX_ARG_NUM;
        if (nbytes < MAX_ARG_NUM){  buf_size = nbytes;} //check invaild input
        memcpy(buf, cur_pcb->args_buffer, buf_size);
        return 0;
    }
    return -1;
}


/*
 * vidmap(uint8_t** screen_start)
 *   DESCRIPTION: map the VIPMAP_ADDR to video_addr in phys_mem
 *   INPUTS: uint8_t** screen_start : pointer of pointer to  video_addr
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: change this map from virtualmem to phys mem
 */
int32_t vidmap(uint8_t** screen_start){
    if (screen_start == NULL ||
        screen_start < (uint8_t**) USER_SPACE ||
        screen_start >= (uint8_t**) (USER_SPACE + FOUR_MB )) {//??
        return -1;
    }

    /* Update screen_start */
    *screen_start = (uint8_t*) VIPMAP_ADDR;

    set_vid_page();  
    return 0;  
}


//extra
int32_t set_handler(int32_t signum, void* handler_address){
    int32_t cur_pid;
    pcb* current_pcb;
    if (handler_address != NULL)
    {
        if (signum >= 0 && signum <= 4 )
        {   
            cur_pid = get_current_pid();
            current_pcb = get_pcb(cur_pid);
            current_pcb->signal_handler[signum] = handler_address;
            return 0;
        }
    }
    return -1;
}



/*
 * sigreturn
 * reset the h/w context, reset the sig_mask for a program, 
 * make signal handler return to user program
 * input: None
 * Output: None
 * side effect: None
 */
int32_t sigreturn(void){
    int i;
    int32_t cur_pid = get_current_pid();
    pcb* current_pcb = get_pcb(cur_pid);
    register uint32_t ebp0 asm("ebp");
    // get the h/w context for segreturn
    hwcontext* context = (hwcontext*)(ebp0 + 20);
    uint32_t user_esp = context->ESP;
    hwcontext* oldcontext = (hwcontext*)(user_esp + 4);
    memcpy(context, oldcontext, sizeof(hwcontext));
    for (i = 0; i < SIGNUM; i++)
        current_pcb->sig_mask[i] = SIG_UNMASK;
    return 0;
}



//=================================================================

// int32_t clear_sys(){
//     clear();
//     return 1;
// }

// /* Allocate a memory space based on the size */
// void* user_malloc(int32_t size){
//     int32_t i;
//     /* Firstly, find if there is a slab cache fits the size. If so, use fixed length memory allocation */
//     fmem_node* cur_node;
//     slab_cache* temp_slab_cache;
//     cur_node = slab_cache_list.node_base;
//     for(i=0; i<slab_cache_list.max_units; i++){
//         temp_slab_cache = (slab_cache*)(cur_node->ptr);
//         if(temp_slab_cache != NULL){
//             if(temp_slab_cache->size == size){
//                 /* Size matches */
//                 return slab_cache_alloc(temp_slab_cache);
//             }
//         }
//         cur_node = cur_node + 1;
//     }

//     /* If we can not find a slab cache, use variable length malloc */
//     return malloc_varlen(size);
// }

// /* Create a slab cache based on the name and size provided by user */
// void* user_slab_create(uint8_t* name ,int32_t size){
//     return slab_cache_create((char*)name, size);
// }

// /* Free a pointer provided by user 
//     input: ptr -- pointer address
//     return: 0 for Fail and 1 for Success*/
// int32_t user_free(void* ptr){
//     if((uint32_t)ptr>=VAR_LEN_MEMORY_START)
//         return free_varlen(ptr);

//     /* Go through slab cache, find a needed one */
//     fmem_node* cur_node;
//     slab_cache* temp_slab_cahe;
//     fmem_list* temp_slab;
//     int32_t i;
//     cur_node = slab_cache_list.node_base;
//     for(i=0; i<slab_cache_list.max_units; i++){
//         temp_slab_cahe = (slab_cache*)(cur_node->ptr);
//         if(temp_slab_cahe != NULL){
//             temp_slab = temp_slab_cahe->slabs;
//             while (temp_slab != NULL)
//             {
//                 /* If ptr is in a range */
//                 if( (uint32_t)(temp_slab->unit_base)<= (uint32_t)ptr && 
//                     (uint32_t)(temp_slab->node_base)+SLAB_SIZE > (uint32_t)ptr)
//                     return slab_cache_free(temp_slab_cahe,ptr);
//                 temp_slab = temp_slab->next;
//             }
//         }
//         cur_node = cur_node + 1;
//     }
//     return 0; // Fail
// }

// /* 
//     show memory status
//  */
// void memstat(){
//     visual_slab_caches();
//     visual_varmem();
// }



//================================================================
/*
 * get_current_pid
 *   DESCRIPTION: get current pid
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the current pid
 *   SIDE EFFECTS: none
 */
int32_t get_current_pid(){
    register int32_t esp_val asm("esp");
    return ((EIGHT_MB - esp_val)/EIGHT_KB);
}

/*
 * get_pcb
 *   DESCRIPTION: get pcb
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the pcb
 *   SIDE EFFECTS: none
 */
pcb* get_pcb(uint32_t pid){
    return (pcb*)(0x800000-(pid+1)*0x2000);
}

/*
 * fo_initial
 *   DESCRIPTION: fo_initial
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the pcb
 *   SIDE EFFECTS: none
 */

void fo_initial(){
    stdin_operation.open = bad_open;
    stdin_operation.close = bad_close;
    stdin_operation.read = terminal_read;
    stdin_operation.write = bad_write;

    stdout_operation.open = bad_open;
    stdout_operation.close = bad_close;
    stdout_operation.read = bad_read;
    stdout_operation.write = terminal_write;

    other_file_operation.open = file_open;
    other_file_operation.close = file_close;
    other_file_operation.read = file_read;
    other_file_operation.write = file_write;

    directory_operation.open = d_open;
    directory_operation.close = d_close;
    directory_operation.read = d_read;
    directory_operation.write = d_write;
    
    rtc_operation.open = rtc_open;
    rtc_operation.close = rtc_close;
    rtc_operation.read = rtc_read;
    rtc_operation.write = rtc_write;
}
/*
 * bad_open
 *   DESCRIPTION: bad_open
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the pcb
 *   SIDE EFFECTS: none
 */
int32_t bad_open(){
    return -1;
}
/*
 * bad_close
 *   DESCRIPTION: bad_close
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the pcb
 *   SIDE EFFECTS: none
 */
int32_t bad_close(){
    return -1;
}
/*
 * bad_read
 *   DESCRIPTION: bad_read
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the pcb
 *   SIDE EFFECTS: none
 */
int32_t bad_read(){
    return -1;
}
/*
 * bad_write
 *   DESCRIPTION: bad_write
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the pcb
 *   SIDE EFFECTS: none
 */
int32_t bad_write(){
    return -1;
}
