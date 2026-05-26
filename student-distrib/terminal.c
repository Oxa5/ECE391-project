//create the terminal
#include "terminal.h"
#define VIDEO 0xB8000
#define background_buffer_1 0xBA000
#define background_buffer_2 0xBB000
#define background_buffer_3 0xBC000
uint8_t* backup_buf_add[3] = {(uint8_t*)0xBA000, (uint8_t*)0xBB000, (uint8_t*)0xBC000}; // Store backup buffers' address
terminal_struct terminals[3];

extern uint8_t get_current_terminal_num(){
    return current_terminal_num;
}
/*
 * terminal_init
 *   DESCRIPTION: initialize the terminal
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */

void terminal_init(){
    clear();
    //set_cursor(0,0);
    int i;
    current_terminal_num = 0;
    // terminal_current.cursor_x = get_screen_x();
    // terminal_current.cursor_y = get_screen_y();
    // terminal_current.keyboard_buffer_num = 0;
    // terminal_current.read_from_keyboard = 0;
    // terminal_current.backcolor = 3;
    // terminal_current.forecolor = 14;
    // enable_cursor(0,14);// same
    // update_cursor(terminal_current.cursor_x ,terminal_current.cursor_y);
    // memset(terminal_current.keyboard_buffer,NULL,128);
    // for(i = 0; i < 128; i++){
    //     terminal_current.keyboard_buffer[i] = '\0';
    // }
    // settextcolor(terminal_current.forecolor,terminal_current.backcolor);
    for(i = 0; i<3 ; i++){
        terminals[i].cursor_x = 0;
        terminals[i].cursor_y = 0;
        terminals[i].keyboard_buffer_num = 0;
        terminals[i].read_from_keyboard = 0;
        terminals[i].backcolor = 3;
        terminals[i].forecolor = 14;
        enable_cursor(0,12);// same
        update_cursor(terminals[i].cursor_x ,terminals[i].cursor_y);
        memset(terminals[i].keyboard_buffer,NULL,128);
        for(i = 0; i < 128; i++){
            terminals[i].keyboard_buffer[i] = '\0';
        }
    }
    
     terminals[1].backcolor = 7;
     terminals[1].forecolor = 9;
     terminals[2].backcolor = 13;
     terminals[2].forecolor = 10;
    terminal_current = terminals[0];
}

/*
 * terminal_read
 *   DESCRIPTION: read
 *   INPUTS: int32_t fd, void* buf, int32_t nbytes
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
    //printf("diaoyong terminal read  ");
    int i;
    int j;
    // read fail can not find buffer or can not read any 
    if(buf == NULL){
        printf("Can't find the buffer, wrong input buffer");
        return -1;
    }
    if(nbytes <= 0){
        printf("Can't read less or equal to 0 bytes");
        return -1;
    }

    for(i = 0; i < 128; i++){//change
        //terminal_current.keyboard_buffer[i] = '\0';
        terminals[current_scheduler_index].keyboard_buffer[i] = '\0';
    }
    terminals[current_scheduler_index].keyboard_buffer_num = 0;
    //else read from keyboard buffer to terminal buffer
    // if  read_from_keyboard is 0, which means didn't press enter continue to wait
    while(terminals[current_scheduler_index].read_from_keyboard != 1){
        i = 0;
    }
    for(i = 0; i < 128; i++){
        ((char*)buf)[i] = '\0';
    }
    //else begin to move the key_board buffer to given buffer.
    // should read character less than 128 and not larger than nbytes 
    // also stop when meet /0

    for(i = 0; (i <nbytes)&&(i < 128)&&(terminals[current_scheduler_index].temp_buffer[i] != '\0')&&(i <= get_temp_buffer_num()) ; i++){
    //for(i = 0; (i <nbytes)&&(i < 128)&&(terminal_current.temp_buffer[i] != '\0') ; i++){
        ((char*)buf)[i] = terminals[current_scheduler_index].temp_buffer[i];
    }
    // then clear the keyboard buffer
    for(j = 0; j < 128; j++){
        terminals[current_scheduler_index].temp_buffer[j] = '\0';
    }
    set_temp_buffer_num(0);
    terminals[current_scheduler_index].read_from_keyboard = 0;
    return i;

}


/*
 * terminal_write
 *   DESCRIPTION: write
 *   INPUTS: int32_t fd, const void* buf, int32_t nbytes
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes){
    //printf("diaoyong write ");
    int i = 0;
    // write fail can not find buffer or can not write any 
    if(buf == NULL){
        printf("Can't find the buffer, wrong input buffer");
        return -1;
    }
    if(nbytes <= 0){
        printf("Can't read less or equal to 0 bytes");
        return -1;
    }
    //settextcolor(5,9);
    for(i = 0; (i <nbytes) ; i++){//changed   for print whole text
        //remap_vid_page(current_terminal_num);
        if (((char*)buf)[i] != NULL) {
            putc(((char*)buf)[i]);
        }
        //remap_vid_page(current_scheduler_index);
        remap_vid_page(current_terminal_num);
        update_cursor(terminals[current_terminal_num].cursor_x,terminals[current_terminal_num].cursor_y);//may need change
        remap_vid_page(current_scheduler_index);
    }
    //settextcolor(terminal_current.forecolor,terminal_current.backcolor);
    return i;

}
/*
 * terminal_open
 *   DESCRIPTION: open
 *   INPUTS: filename
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
int32_t terminal_open(const uint8_t* filename){
    return 0;
}
/*
 * terminal_close
 *   DESCRIPTION: close
 *   INPUTS: int32_t fd
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
int32_t terminal_close(int32_t fd){
    return -1;
}

extern void terminal_switch(uint8_t switch_terminal_num){
    // confused why for terminal 2 and 3 didn't show start the shell and 391os>>
    if(switch_terminal_num == current_terminal_num){
        return;
    }else if(switch_terminal_num >= 3){
        printf("can't do terminal switch");// will never print this sentence, because the code was writen by me, and I never let switch number over the range. (-_-)
        return;
    }else{
        // if it is legal, store the current screen in memory
        remap_vid_page(current_terminal_num);
        // switch(current_terminal_num){
        //     case 1:
        //         memcpy((void*)background_buffer_1,(void*)VIDEO, 4*1024 );
        //         terminal_1 = terminal_current;
        //         break;
        //     case 2:
        //         memcpy((void*)background_buffer_2,(void*)VIDEO, 4*1024 );
        //         terminal_2 = terminal_current;
        //         break;
        //     case 3:
        //         memcpy((void*)background_buffer_3,(void*)VIDEO, 4*1024 );
        //         terminal_3 = terminal_current;
        //         break;
        // }
        memcpy((void*)backup_buf_add[current_terminal_num],(void*)VIDEO, 4*1024 );
        // then load the switch terminal screen to screen memory
        // switch(switch_terminal_num){
        //     case 1:
        //         memcpy((void*)VIDEO, (void*)background_buffer_1, 4*1024 );
        //         terminal_current = terminal_1;
        //         break;
        //     case 2:
        //         memcpy((void*)VIDEO, (void*)background_buffer_2, 4*1024 );
        //         terminal_current = terminal_2;
        //         break;
        //     case 3:
        //         memcpy((void*)VIDEO, (void*)background_buffer_3, 4*1024 );
        //         terminal_current = terminal_3;
        //         break;
        // }
        memcpy((void*)VIDEO, (void*)backup_buf_add[switch_terminal_num], 4*1024);
        
        current_terminal_num = switch_terminal_num;
        update_cursor(terminals[switch_terminal_num].cursor_x,terminals[switch_terminal_num].cursor_y); // must add this line. fou ze it will show the cursor of the last terminal.(>_<)
        //set_cursor(terminals[switch_terminal_num].cursor_x,terminals[switch_terminal_num].cursor_y);
        settextcolor(terminals[switch_terminal_num].forecolor,terminals[switch_terminal_num].backcolor);
        remap_vid_page(current_scheduler_index);
    }
   
}
terminal_struct* get_current_terminal() {
	return &(terminals[current_terminal_num]);
}

