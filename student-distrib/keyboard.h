//
//  keyboard.h
//  88
//
//  Created by 学习 on 15/10/2023.
//

#ifndef keyboard_h
#define keyboard_h

#include "i8259.h"
#include "types.h"
#include "lib.h"
#include "terminal.h"
#include "signal.h"
#include "sound.h"
#define F1        0x3B
#define F2        0x3C
#define F3        0x3D

#define TAB             0x0F
#define TAB_SPACE       4
#define CAPS_LOCK       0x3A
#define L_SHIFT_PRESS       0x2A
#define L_SHIFT_RELEASE     0xAA
#define CTRL_PRESS          0x1D
#define CTRL_RELEASE        0x9D

#define BACKSPACE           0x0E
#define ENTER               0x1C
#define R_SHIFT_PRESS       0x36
#define R_SHIFT_RELEASE     0xB6
#define ALT_PRESS           0x38
#define ALT_RELEASE         0xB8

#define UP       0x48
#define DOWN     0x50
#define LEFT     0x4B
#define RIGHT    0x4D

#define KEYBOARD_IRQ 1
#define KEYBOARD_DATA    0x60
#define KEYBOARD_BUFFFER_SIZE    128// the buffer size is 128
#define ATTRIB 0x7

uint8_t key_pressed;
//uint8_t to_read;
//unsigned int key_buffer_character_num;
//unsigned char keyboard_buffer[KEYBOARD_BUFFFER_SIZE];

extern void keyboard_init();
extern void keyboard_handler();
extern uint8_t get_temp_buffer_num();
extern void set_temp_buffer_num(uint8_t num_input);

#endif /* keyboard_h */
