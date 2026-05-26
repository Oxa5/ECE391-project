#ifndef ASM_LINKAGE_H
#define ASM_LINKAGE_H
#ifndef ASM


#include "rtc.h"
#include "keyboard.h"
#include "sys_call.h"
#include "scheduler.h"
#include "signal.h"
// linkages for handlers
extern void rtc_handler_linkage();
extern void keyboard_handler_linkage();
extern void sys_call_linkage();
extern void pit_handler_linkage();
#endif
#endif
