#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "x86_desc.h"
#include "types.h"
#include "lib.h"
#include "file_sys.h"
#include "terminal.h"
#include "sys_call.h"
#include "page.h"
#include "signal.h"
#include "sound.h"

#define PIT_IRQ 0
#define NUM_SCHEDULERS 3
#define UNINITIALIZED_TERM -2
#define TERMINAL_CLOSED -1
#define PIT_FREQUENCY 11931
#define PIT_CONTROL_WORD 0x36
#define PIT_DATA_PORT 0x40
#define PIT_COMMAND_PORT 0x43
#define LOW_BYTE_MASK 0x00FF
#define HIGH_BYTE_SHIFT 8


void pit_init(void);
void pit_handler(void);
void process_scheduler(void);

extern int32_t scheduler_array[NUM_SCHEDULERS];
extern int32_t current_scheduler_index;

#endif
