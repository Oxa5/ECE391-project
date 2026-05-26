

#define ASM 1
#include "asm_linkage.h"
.globl rtc_handler_linkage, keyboard_handler_linkage, sys_call_linkage,pit_handler_linkage



#   pit_handler_linkage
#   Description: asm linkage for pit
#   Input: none
#   Output: none
#   Notice: pit_handler_linkage
#
pit_handler_linkage:
    pushal
    call pit_handler
    call handle_signal
    popal
    iret


#   rtc_handler_linkage
#   Description: asm linkage for rtc_handler
#   Input: none
#   Output: none
#   Notice: rtc_handler_linkage
#
rtc_handler_linkage:
    pushfl
    pushal
    call rtc_handler
    call handle_signal
    popal
    popfl
    iret



#   keyboard_handler_linkage
#   Description: asm linkage for keyboard_handler
#   Input: none
#   Output: none
#   Notice: keyboard_handler_linkage
#
keyboard_handler_linkage:
    pushfl
    pushal
    call keyboard_handler
    call handle_signal
    popal
    popfl
    iret



#   sys_call_linkage
#   Description: asm linkage for sys_call
#   Input: none
#   Output: none
#   Notice: sys_call_linkage
#
sys_call_linkage:
    pushfl # push flags
    pushl %eax
    pushl %ebp

    pushl %esi
    pushl %edi # callee saved
    

    # then check if the input is valid which is 
    cmpl $1, %eax
    jl invalid_input
    cmpl $15, %eax
    jg invalid_input
    # then the input is valid

    pushl %edx #3rd argument
    pushl %ecx #2nd argument
    pushl %ebx #1st argument
    addl $-1, %eax # make eax fit the sys_call_jump_table
    cmpl $9, %eax
    jne other
    call *sys_call_jump_table(,%eax,4)
    popl %ebx
    popl %ecx
    popl %edx
    
    popl %edi
    popl %esi

    popl %ebp
    popl %eax
    popfl # pop all flags
    iret
    
invalid_input:
    movl $-1, %eax
other:
    call *sys_call_jump_table(,%eax,4)
    popl %ebx
    popl %ecx
    popl %edx
    popl %edi
    popl %esi
    
    popl %ebp
    addl $4, %esp
    popfl # pop all flags
    iret

sys_call_jump_table:
    .long halt
    .long execute
    .long read
    .long write
    .long open
    .long close
    .long getargs
    .long vidmap
    .long set_handler
    .long sigreturn


