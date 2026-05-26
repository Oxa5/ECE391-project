//
//  keyboard.c
//  88
//
//  Created by 学习 on 15/10/2023.
//
#include "keyboard.h"
#include "terminal.h"
volatile unsigned char CAPS_LOCK_FLAG = 0;
volatile unsigned char CTRL_FLAG = 0;
volatile unsigned char ALT_FLAG = 0;
volatile unsigned char L_SHIFT_FLAG = 0;
volatile unsigned char R_SHIFT_FLAG = 0;
volatile unsigned char SCAN_MODE = 0;

//static uint8_t keyboard_buffer_num = 0;;
//static uint8_t temp_buffer_num = 0;; // don't use them in checkpoint5, only in checkpoint 3 because if don't use them it will behave strange
//extern  terminal_struct1 terminal_current;

   char scancode_set[4][0x3A] = {
        // didn't press shift and cpas lock 
    {  0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', 0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', 0, 0, 'a', 's', 'd', 'f', 'g', 'h', 
    'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, 0,0,' '},
        // press shift
    {0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', 0, 0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', 0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 
    'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' '},
        // press caps lock
    {0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', 0, 0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '[', ']', 0, 0, 'A', 'S', 'D', 'F', 'G', 'H', 
    'J', 'K', 'L', ';', '\'', '`', 0, '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 0, 0, 0,' '},

        // press shift and caps lock
    {0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', 0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '{', '}', 0, 0, 'a', 's', 'd', 'f', 'g', 'h', 
    'j', 'k', 'l', ':', '\"', '~', 0, '|', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', '<', '>', '?', 0, 0, 0, ' '}
    };
     
/* 
 * keyboard_init
 *   DESCRIPTION: Initialize the keyboard_init
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Initialize the keyboard_init
 */
extern void keyboard_init(){
    CAPS_LOCK_FLAG = 0;
    CTRL_FLAG = 0;
    ALT_FLAG = 0;
    L_SHIFT_FLAG = 0;
    R_SHIFT_FLAG = 0;
    SCAN_MODE = 0;
    enable_irq(KEYBOARD_IRQ);

}
extern uint8_t get_temp_buffer_num(){
    uint8_t current_terminal_num = get_current_terminal_num();
    return terminals[current_terminal_num].temp_buffer_num;
}
extern void set_temp_buffer_num(uint8_t num_input){
    uint8_t current_terminal_num = get_current_terminal_num();
    terminals[current_terminal_num].temp_buffer_num = num_input;
}

/* 
 * keyboard_handler
 *   DESCRIPTION: keyboard_handler, handler several cases
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: hander several cases
 */
extern void keyboard_handler(){
    //printf("123");
    uint8_t scancode = inb(KEYBOARD_DATA); 
    SCAN_MODE = (CAPS_LOCK_FLAG<<1) | (R_SHIFT_FLAG | L_SHIFT_FLAG);// only shift is 1, only cap is 2, both pressed is 3, and neither is 0
    int i;
    cli();
    uint8_t current_terminal_num = get_current_terminal_num();
    switch(scancode) {
        case F1:
                        // F1
            if(ALT_FLAG){
                terminal_switch(0);
                break;// change terminal in checkpoint 5
            }
            break;
        case F2:
                       // F2
            if(ALT_FLAG){
                terminal_switch(1);
                break;// change termianl in checkpoint 5
            }
            break;
        case F3:
                         // F3
            if(ALT_FLAG){
                terminal_switch(2);
                break;// change terminal in checkpint 5
            }
            break;
        case CAPS_LOCK:              // CAPS_LOCK
            
            CAPS_LOCK_FLAG ^= 1; // or caps_lock_flag != caps_lock_flag
            break;
        case L_SHIFT_PRESS:              // L_SHIFT_PRESS
            
            L_SHIFT_FLAG = 1;
            break;
        case L_SHIFT_RELEASE:              // L_SHIFT_RELEASE
            
            L_SHIFT_FLAG = 0;
            break;
        case R_SHIFT_PRESS:              // R_SHIFT_PRESS
            
            R_SHIFT_FLAG = 1;
            key_pressed = R_SHIFT_PRESS;
            break;
        case R_SHIFT_RELEASE:              // R_SHIFT_RELEASE
            
            R_SHIFT_FLAG= 0;
            break;
        case CTRL_PRESS:              // CTRL_PRESS
            
            CTRL_FLAG = 1 ;
            break;
        case CTRL_RELEASE:              // CTRL_RELEASE
            
            CTRL_FLAG = 0;
            break;
        case ALT_PRESS:               // ALT_PRESS
            
            ALT_FLAG = 1;
            break;
        case ALT_RELEASE:              // ALT_RELEASE
            
            ALT_FLAG = 0;
            break;

        case TAB:              // TAB'\t'
            if (SCAN_MODE == 2 ) {
                switch (terminals[current_terminal_num].keyboard_buffer[0]) {
                    case 'l':
                        if (terminals[current_terminal_num].keyboard_buffer_num == 1) {
                            terminals[current_terminal_num].keyboard_buffer[1] = 's';
                            terminals[current_terminal_num].temp_buffer[1] = 's';
                            terminals[current_terminal_num].keyboard_buffer_num++;
                            terminals[current_terminal_num].temp_buffer_num++;
                            putc_user('s');
                        }
                        break;
                    case 'f':
                        if (terminals[current_terminal_num].keyboard_buffer_num == 1) {
                            char* c;
                            c = "ish";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i+ j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i+ j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                            char* c;
                            c = "sh";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 3) {
                            char* c;
                            c = "h";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        break;
                    case 'r':
                        if (terminals[current_terminal_num].keyboard_buffer_num == 1) {
                            char* c;
                            c = "tc";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                            char* c;
                            c = "c";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        break;
                    case 'g':
                        if (terminals[current_terminal_num].keyboard_buffer_num == 1) {
                            char* c;
                            c = "rep";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                            char* c;
                            c = "ep";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 3) {
                            char* c;
                            c = "p";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        break;
                    case 'e':
                        if (terminals[current_terminal_num].keyboard_buffer_num == 1) {
                            char* c;
                            c = "xit";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                            char* c;
                            c = "it";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 3) {
                            char* c;
                            c = "t";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        break;
                    case 'h':
                        if (terminals[current_terminal_num].keyboard_buffer_num == 1) {
                            char* c;
                            c = "ello";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                            char* c;
                            c = "llo";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 3) {
                            char* c;
                            c = "lo";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 4) {
                            char* c;
                            c = "o";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        break;
                    case 'c':
                        if (terminals[current_terminal_num].keyboard_buffer_num == 1) {
                            break;
                        }
                        if (terminals[current_terminal_num].keyboard_buffer[1] == 'a' && terminals[current_terminal_num].keyboard_buffer_num == 2) {
                            char* c;
                            c = "t";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                            break;
                        }
                        if (terminals[current_terminal_num].keyboard_buffer[1] == 'o') {
                            if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                                char* c;
                                c = "unter";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 3) {
                                char* c;
                                c = "nter";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 4) {
                                char* c;
                                c = "ter";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 5) {
                                char* c;
                                c = "er";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 6) {
                                char* c;
                                c = "r";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            break;
                        }
                    case 't':
                        if (terminals[current_terminal_num].keyboard_buffer_num == 1) {
                            char* c;
                            c = "estprint";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                            char* c;
                            c = "stprint";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 3) {
                            char* c;
                            c = "tprint";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 4) {
                            char* c;
                            c = "print";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 5) {
                            char* c;
                            c = "rint";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 6) {
                            char* c;
                            c = "int";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 7) {
                            char* c;
                            c = "nt";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        if (terminals[current_terminal_num].keyboard_buffer_num == 8) {
                            char* c;
                            c = "t";
                            int i;
                            uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                            for (i = 0; i < strlen(c); i++) {
                                terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                terminals[current_terminal_num].keyboard_buffer_num++;
                                terminals[current_terminal_num].temp_buffer_num++;
                                putc_user(c[i]);
                            }
                        }
                        break;
                    case 'p':
                         if (terminals[current_terminal_num].keyboard_buffer_num == 1) {
                             char* c;
                             c = "ingpong";
                             int i;
                             uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                             for (i = 0; i < strlen(c); i++) {
                                 terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].keyboard_buffer_num++;
                                 terminals[current_terminal_num].temp_buffer_num++;
                                 putc_user(c[i]);
                             }
                         }
                         if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                             char* c;
                             c = "ngpong";
                             int i;
                             uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                             for (i = 0; i < strlen(c); i++) {
                                 terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].keyboard_buffer_num++;
                                 terminals[current_terminal_num].temp_buffer_num++;
                                 putc_user(c[i]);
                             }
                         }
                         if (terminals[current_terminal_num].keyboard_buffer_num == 3) {
                             char* c;
                             c = "gpong";
                             int i;
                             uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                             for (i = 0; i < strlen(c); i++) {
                                 terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].keyboard_buffer_num++;
                                 terminals[current_terminal_num].temp_buffer_num++;
                                 putc_user(c[i]);
                             }
                         }
                         if (terminals[current_terminal_num].keyboard_buffer_num == 4) {
                             char* c;
                             c = "pong";
                             int i;
                             uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                             for (i = 0; i < strlen(c); i++) {
                                 terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].keyboard_buffer_num++;
                                 terminals[current_terminal_num].temp_buffer_num++;
                                 putc_user(c[i]);
                             }
                         }
                         if (terminals[current_terminal_num].keyboard_buffer_num == 5) {
                             char* c;
                             c = "ong";
                             int i;
                             uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                             for (i = 0; i < strlen(c); i++) {
                                 terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].keyboard_buffer_num++;
                                 terminals[current_terminal_num].temp_buffer_num++;
                                 putc_user(c[i]);
                             }
                         }
                         if (terminals[current_terminal_num].keyboard_buffer_num == 6) {
                             char* c;
                             c = "ng";
                             int i;
                             uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                             for (i = 0; i < strlen(c); i++) {
                                 terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].keyboard_buffer_num++;
                                 terminals[current_terminal_num].temp_buffer_num++;
                                 putc_user(c[i]);
                             }
                         }
                         if (terminals[current_terminal_num].keyboard_buffer_num == 7) {
                             char* c;
                             c = "g";
                             int i;
                             uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                             for (i = 0; i < strlen(c); i++) {
                                 terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                 terminals[current_terminal_num].keyboard_buffer_num++;
                                 terminals[current_terminal_num].temp_buffer_num++;
                                 putc_user(c[i]);
                             }
                         }
                         break;
                    case 's':
                        if (terminals[current_terminal_num].keyboard_buffer_num == 1) {
                            break;
                        }
                        if (terminals[current_terminal_num].keyboard_buffer[1] == 'i') {
                            if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                                char* c;
                                c = "gtest";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 3) {
                                char* c;
                                c = "test";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 4) {
                                char* c;
                                c = "est";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 5) {
                                char* c;
                                c = "st";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 6) {
                                char* c;
                                c = "t";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            break;
                        }
                        if (terminals[current_terminal_num].keyboard_buffer[1] == 'y') {
                            if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                                char* c;
                                c = "serr";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 3) {
                                char* c;
                                c = "err";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 4) {
                                char* c;
                                c = "rr";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 5) {
                                char* c;
                                c = "r";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            } 
                            break;
                        }
                        if (terminals[current_terminal_num].keyboard_buffer[1] == 'h') {
                            if (terminals[current_terminal_num].keyboard_buffer_num == 2) {
                                char* c;
                                c = "ell";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 3) {
                                char* c;
                                c = "ll";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            if (terminals[current_terminal_num].keyboard_buffer_num == 4) {
                                char* c;
                                c = "l";
                                int i;
                                uint8_t j = terminals[current_terminal_num].keyboard_buffer_num;
                                for (i = 0; i < strlen(c); i++) {
                                    terminals[current_terminal_num].keyboard_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].temp_buffer[i + j] = c[i];
                                    terminals[current_terminal_num].keyboard_buffer_num++;
                                    terminals[current_terminal_num].temp_buffer_num++;
                                    putc_user(c[i]);
                                }
                            }
                            break;
                        }
                }
            }
            else {
                for (i = 0; i < TAB_SPACE; i++) {
                    terminals[current_terminal_num].keyboard_buffer[terminals[current_terminal_num].keyboard_buffer_num] = ' ';
                    terminals[current_terminal_num].keyboard_buffer_num++;
                    //printf("%c",' ');//print "    "
                    //remap_vid_page(current_terminal_num);
                    putc_user(' ');
                    //remap_vid_page(current_scheduler_index);
                    //update_cursor(get_screen_x(),get_screen_y());
                }
            }
            break;
        case BACKSPACE:              // BACKSPACE"\b"
            
            if(terminals[current_terminal_num].keyboard_buffer_num == 0){
                sti();
                send_eoi(KEYBOARD_IRQ);
                return; 
            }
           
            terminals[current_terminal_num].keyboard_buffer_num--;
            terminals[current_terminal_num].temp_buffer_num--;
            remap_vid_page(current_terminal_num);
            *(uint8_t*)((char *)0xB8000 + ((80 * terminals[current_terminal_num].cursor_y +terminals[current_terminal_num].cursor_x-1)<<1)) = 0; // oB8000 is the video memory 80is the colomun number
            *(uint8_t*)((char *)0xB8000 + ((80 * terminals[current_terminal_num].cursor_y + terminals[current_terminal_num].cursor_x-1)<<1)+1) = ATTRIB;
            remap_vid_page(current_scheduler_index);
            if(terminals[current_terminal_num].cursor_x == 0){
                terminals[current_terminal_num].cursor_x = 79;
                terminals[current_terminal_num].cursor_y--;
                
            }else{
                terminals[current_terminal_num].cursor_x--;
            }
            //set_cursor(terminals[current_terminal_num].cursor_x,terminals[current_terminal_num].cursor_y);
            update_cursor(terminals[current_terminal_num].cursor_x,terminals[current_terminal_num].cursor_y);
            break;

        case ENTER:              // ENTER"\n"

            terminals[current_terminal_num].keyboard_buffer[terminals[current_terminal_num].keyboard_buffer_num]='\n';
            terminals[current_terminal_num].temp_buffer[terminals[current_terminal_num].temp_buffer_num]='\n';
            // terminals[current_terminal_num].cursor_y++;
            // terminals[current_terminal_num].cursor_x = 0;
            // if(terminals[current_terminal_num].cursor_y>= 25){ // the number is rows, NUM_ROWS
            //     scrolling();
            //     set_cursor(0,24);
            //     update_cursor(terminals[current_terminal_num].cursor_x,terminals[current_terminal_num].cursor_y);
            // }
            //printf("%c", '\n');
            //remap_vid_page(current_terminal_num);
            putc_user('\n');
            //remap_vid_page(current_scheduler_index);
            
            int j ;
            terminals[current_terminal_num].read_from_keyboard = 1;
            terminals[current_terminal_num].keyboard_buffer_num = 0;
            for(j = 0; j < 128; j++){// the buffer size
            terminals[current_terminal_num].keyboard_buffer[j] = '\0';
            }
            //printf("%d", keyboard_buffer_num);
            key_pressed = ENTER;
            update_cursor(terminals[current_terminal_num].cursor_x,terminals[current_terminal_num].cursor_y);
            break;
        default :
        // if ctrl_flag is 1 and enter l/L clear the screen and set the cursor to left top
            if(CTRL_FLAG) {
                if(scancode_set[SCAN_MODE][scancode]==0 || (scancode > 0x3A)) {
                    //printf("Invalid input, please use wright keyboard");
                    break;
                }
                if(scancode == 0x26){// L = 0x26
                    remap_vid_page(current_terminal_num);
                    clear();
                    //set_cursor(0,0);
                    remap_vid_page(current_scheduler_index);
                    update_cursor(0,0);
                    //printf("1234");
                }
                if(scancode == 0x2E){// C/c = 0x2E        ?????是否权限不匹配
                    int32_t cur_pid;
                    pcb* current_pcb ;
                    cli();
                    cur_pid = get_current_pid();
                    current_pcb = get_pcb(cur_pid);
                    // if (sig_num == SIG_INTERRUPT)
                    current_pcb = get_pcb(scheduler_array[current_terminal_num]);
                    current_pcb->signal[SIG_INTERRUPT] = SIG_ACTIVATE;
                    sti();
                }
                if(scancode == 0x10){play_sound(DO);	break;}
                if(scancode == 0x11){play_sound(RE);	break;}
                if(scancode == 0x12){play_sound(MI);	break;}
                if(scancode == 0x13){play_sound(FA);	break;}
                if(scancode == 0x14){play_sound(SO);	break;}
                if(scancode == 0x15){play_sound(LA);	break;}
                if(scancode == 0x16){play_sound(XI);	break;}
                if(scancode == 0x17){play_sound(DO2);	break;}
                if(scancode == 0x18){play_sound(RE2);	break;}
                if(scancode == 0x19){nosound();	break;}


                //if ((keyboard_buffer_num < KEYBOARD_BUFFFER_SIZE-1)&&(scancode != 0x26)&&(scancode != 0x2E)){ // ox26 is l
                if ((terminals[current_terminal_num].keyboard_buffer_num < KEYBOARD_BUFFFER_SIZE-1)&&(scancode != 0x26)&&(scancode != 0x2E)){ 
                    terminals[current_terminal_num].keyboard_buffer[terminals[current_terminal_num].keyboard_buffer_num]=scancode_set[SCAN_MODE][scancode];// add the input into buffer
                    terminals[current_terminal_num].temp_buffer[terminals[current_terminal_num].temp_buffer_num]=scancode_set[SCAN_MODE][scancode];
                    terminals[current_terminal_num].keyboard_buffer_num++; 
                    terminals[current_terminal_num].temp_buffer_num++; 
                    
                    key_pressed = scancode_set[SCAN_MODE][scancode];
                }
                    
                break;
            }else{  
                if(scancode_set[SCAN_MODE][scancode]==0 || (scancode > 0x3A)) { // the bound
                    break;
                }
                if (terminals[current_terminal_num].keyboard_buffer_num < KEYBOARD_BUFFFER_SIZE-1){
                    terminals[current_terminal_num].keyboard_buffer[terminals[current_terminal_num].keyboard_buffer_num]=scancode_set[SCAN_MODE][scancode];// add the input into buffer
                    terminals[current_terminal_num].temp_buffer[terminals[current_terminal_num].temp_buffer_num]=scancode_set[SCAN_MODE][scancode];
                    terminals[current_terminal_num].keyboard_buffer_num++; 
                    terminals[current_terminal_num].temp_buffer_num++; 
                    //printf("%c", scancode_set[SCAN_MODE][scancode]);
                    //remap_vid_page(current_terminal_num);
                    putc_user(scancode_set[SCAN_MODE][scancode]);
                    //remap_vid_page(current_scheduler_index);
                    //printf("%d", terminal_current.keyboard_buffer_num);
                    //update_cursor(get_screen_x(),get_screen_y());
                    key_pressed = scancode_set[SCAN_MODE][scancode];
                    update_cursor(terminals[current_terminal_num].cursor_x,terminals[current_terminal_num].cursor_y);
                }
                
            }
    } 
    sti(); 
     send_eoi(KEYBOARD_IRQ);   
}

