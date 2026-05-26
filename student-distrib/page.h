

#ifndef page_h
#define page_h

//#include <stdio.h>
#include "types.h"
#include "lib.h"
#include "sys_call.h"

#define MEMORY_START 160*1024*1024

typedef union page_directory_entry {
    uint32_t val;
    struct PAGE_4MB {
        uint32_t present : 1;
        uint32_t read_or_write : 1;
        uint32_t user_or_supervisor : 1;
        uint32_t write_through : 1;
        uint32_t cache_disabled : 1;
        uint32_t accessed : 1;
        uint32_t dirty : 1;
        uint32_t page_size : 1;
        uint32_t global_page : 1;
        uint32_t avail : 3;
        uint32_t pat : 1;
        uint32_t reserved : 9;
        uint32_t base_address : 10;
    } PAGE_4MB __attribute__((packed));
    struct PAGE_4KB {
        uint32_t present : 1;
        uint32_t read_or_write : 1;
        uint32_t user_or_supervisor : 1;
        uint32_t write_through : 1;
        uint32_t cache_disabled : 1;
        uint32_t accessed : 1;
        uint32_t reserved : 1;
        uint32_t page_size : 1;
        uint32_t global_page : 1;
        uint32_t avail : 3;
        uint32_t base_address : 20;
    } PAGE_4KB __attribute__((packed));
} page_dir_entry_t;

typedef union page_table_entry {
    uint32_t val;
    struct {
        uint32_t present : 1;
        uint32_t read_or_write : 1;
        uint32_t user_or_supervisor : 1;
        uint32_t write_through : 1;
        uint32_t cache_disabled : 1;
        uint32_t accessed : 1;
        uint32_t dirty : 1;
        uint32_t pat : 1;
        uint32_t global_page : 1;
        uint32_t avail : 3;
        uint32_t base_address : 20;
    } PAGE_4KB __attribute__((packed));
} page_table_entry_t;

page_table_entry_t page_table[1024] __attribute__((aligned(4096)));
page_dir_entry_t page_directory[1024] __attribute__((aligned(4096)));

page_table_entry_t video_page_table[1024] __attribute__((aligned(4 * 1024)));
page_table_entry_t slab_page_table[1024] __attribute__((aligned(4 * 1024)));

void init_paging(void);
void set_page(uint32_t pid);
void set_vid_page(void);
void remap_vid_page(uint8_t terminal_num);

//void show_page_dir(page_dir_entry_t *page_directory);
//void show_page_table(page_table_entry_t *page_table);
#endif /* page_h */
