#ifndef MEMORY_H
#define MEMORY_H
#include "types.h"
#include "sys_call.h"
#include "page.h"
#include "lib.h"


#define FOUR_KB 4*1024      // A slab's zie is 4kb = a page size

 /* Memory management structure node (node of a linked list) */
typedef struct  node {
    struct node* next;
    void* ptr;
}  node;

/* Linked list storing memory's information */
typedef struct  mem_list {
    struct mem_list* next;
    node* head;      
    uint32_t  max;  
    uint32_t  used_units;
    node* n_base; 
    void* u_base;   
    uint32_t  size;// Structure's size
}  mem_list;

/* Definition of a slab cache */
typedef struct  slab_cache {
    uint8_t   name[10 + 1];  
    mem_list* slabs; 
    uint32_t size; 
}  slab_cache;



/* Fixed length functions */
void mem_init(uint32_t mem, uint32_t size, mem_list* list);
void* malloc(mem_list* memlist);
int32_t free(mem_list* memlist, void* ptr);
/* Variable length functions */
/* Slab cache's functions */
void slab_cache_init();
slab_cache* slab_cache_create(const char* name, uint32_t size);
void* slab_cache_alloc(slab_cache* cache);
int32_t slab_cache_free(slab_cache* cache, void* ptr);

/* Functions for visualize */
extern void show_slab_caches();

#endif

