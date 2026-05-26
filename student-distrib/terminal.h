#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "i8259.h"
#include "lib.h"
#include "types.h"
#include "keyboard.h"
#include "page.h"
#include "scheduler.h"
typedef struct terminal_struct {
    uint8_t cursor_x;
    uint8_t cursor_y;
    uint8_t backcolor;
    uint8_t forecolor;
    uint8_t keyboard_buffer[128];
    uint8_t temp_buffer[128];
    uint8_t keyboard_buffer_num;
    uint8_t temp_buffer_num;
    volatile uint8_t read_from_keyboard;
} terminal_struct;
terminal_struct terminal_current;
extern terminal_struct terminals[3];
extern uint8_t* backup_buf_add[3];
 //terminal_struct terminal_1;
//  uint8_t background_buffer_1 ;
// terminal_struct terminal_2;
//  uint8_t background_buffer_2 ;
// terminal_struct terminal_3;
//  uint8_t background_buffer_3 ;
uint8_t current_terminal_num;
//extern volatile terminal_struct terminal_current;
extern void terminal_init();
extern int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);
extern int32_t terminal_open(const uint8_t* filename);
extern int32_t terminal_close(int32_t fd);
extern void terminal_switch(uint8_t terminal_num);
extern uint8_t get_current_terminal_num();
extern terminal_struct* get_current_terminal();
#endif
