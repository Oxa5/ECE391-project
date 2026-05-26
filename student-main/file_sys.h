#ifndef FILE_SYS_H
#define FILE_SYS_H

#include "types.h"
#include "lib.h"
#include "sys_call.h"

#define DATA_BLOCK_MAX_NUM 1023 //(4096-4) / 4 = 1023
#define FAIL -1

//dentry struct
typedef struct directory_entry {
    char file_name[32]; //32 is max name length
    uint32_t file_type;
    uint32_t inode;
    uint8_t reserved[24];
} dentry_t;

//boot block struct
typedef struct  boot_block {
    uint32_t dentry_num;
    uint32_t inode_num;
    uint32_t data_blocks_num;
    uint8_t reserved[52];
    dentry_t dentries[63]; //Both the statistics and each directory entry occupy 64B, so the file system can hold up to 63 files
}   bb_t;

//inodes struct
typedef struct  inode {
    uint32_t length;
    uint32_t data_block[DATA_BLOCK_MAX_NUM];
}   inode_t;

typedef struct  data_block {
    uint8_t data[4096]; //4kb
}   db_t;

//file system functions
uint32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
uint32_t read_dentry_by_index(uint8_t index, dentry_t* dentry);
uint32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
void file_system_init(uint32_t boot_block_init);
int32_t file_open(const uint8_t* filename);
int32_t file_close(const int32_t filename);
int32_t file_read(int32_t fd, void* buf, int32_t length);
int32_t file_write(const int32_t fd, const void* buf, int32_t length);
int32_t d_open(const uint8_t* dir_name);
int32_t d_close(const int32_t dir_name);
int32_t d_read(int32_t fd, void* buf, int32_t length);
int32_t d_write(int32_t fd, const void* buf, int32_t length);
bb_t* boot_block;

#endif
