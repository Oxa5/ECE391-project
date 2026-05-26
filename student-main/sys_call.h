#ifndef SYS_CALL_H
#define SYS_CALL_H


#include "types.h"
#include "terminal.h"
#include "keyboard.h"
#include "lib.h"
#include "file_sys.h"
#include "rtc.h"
#include "page.h"
#include "i8259.h"
#include "x86_desc.h"
#include "asm_linkage.h"
#include "signal.h"

#define MAX_FILE_NUM 8
#define MAX_FILE_NAME_NUM 32
#define MAX_ARG_NUM 128
#define MAGIC_NUM 4
#define MAGIC_NUM_ONE 0x7F
#define MAGIC_NUM_TWO 0x45
#define MAGIC_NUM_THREE 0x4C
#define MAGIC_NUM_FOUR 0x46
#define MAX_PROCESS_NUM 6
#define USER_SPACE 0x8000000        //128MB
#define PRO_IMG  0x8048000
#define STACK 0x8400000
#define EIGHT_MB 0x800000
#define FOUR_MB 0x400000
#define EIGHT_KB 0x2000
#define HALT_RETURN 256
#define VIPMAP_ADDR (USER_SPACE + FOUR_MB ) //just after 132MB (128+4mb)

typedef struct file_operations{
    int32_t (*read)(int32_t fd, void* buf, int32_t nbytes) ;
    int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes) ;
    //int32_t (*open)(const uint8_t* fd);
    int32_t (*open)(const uint8_t* filename);
    int32_t (*close)(int32_t fd);
    
} fo;

typedef struct file_descriptor{
    fo file_operation;
    uint32_t inode;
    int32_t file_position;
    uint32_t flags;
} fd;

typedef struct pcb_struct{
    fd file_array[MAX_FILE_NUM];
    int32_t signal[SIGNUM];
    int sig_mask[SIGNUM];
    void* signal_handler[SIGNUM];
    int32_t current_pid;
    int32_t parent_pid;
    int32_t current_esp;
    int32_t current_ebp;
    int32_t scheduler_ebp;
    int32_t args_buffer[MAX_ARG_NUM+1]; //user buffer?
} pcb;

fo stdin_operation;
fo stdout_operation;
fo other_file_operation;
fo directory_operation;
fo rtc_operation;

extern uint32_t process_[MAX_PROCESS_NUM];
void fo_initial();
int32_t bad_open();
int32_t bad_close();
int32_t bad_read();
int32_t bad_write();
int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command);
int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t open (const uint8_t* file);
int32_t close(int32_t fd);
int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);
int32_t get_current_pid();
pcb* get_pcb(uint32_t pid);

#endif 
