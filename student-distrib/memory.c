#include "memory.h"
// static mem_list memlist;
mem_list slab_cache_list;
mem_list slabs_list;
/*
    mem: start address of new slab
    size: structure's size
 */
void mem_init(uint32_t mem, uint32_t size, mem_list* list) {
    uint32_t max, i;
    /* Calculate max units we can have */
    max = FOUR_KB / (sizeof(node) + size);

    /* Initialize linked list */
    list->max = max;
    list->used_units = 0;
    list->n_base = (node*)mem;
    list->u_base = (void*)(mem + max * sizeof(node));
    list->head = (node*)mem;
    list->next = NULL;
    list->size = size;
    node* cur_node;
    for (i = 0; i < max - 1; i++) {
        cur_node = list->head + i;
        cur_node->next = cur_node + 1;  // Make each next ptr pointing to next node
        cur_node->ptr = 0;
    }
    cur_node = list->head + max;
    cur_node->next = NULL; // The last node points to NULL
}


/* Allocate a memory in fixed length memory allocation
    input: a fixed length memory list pointer */
void* malloc(mem_list* memlist) {
    void* result = NULL;
    if (memlist->head != NULL) {
        node* new_node = memlist->head; // The memory will be allocated will be the head of the linked list
        uint32_t addr = (uint32_t)new_node - (uint32_t)memlist->n_base;
        int position = addr / sizeof(node); // This node's position
        result = (void*)(memlist->u_base + position * (memlist->size));
        new_node->ptr = result; // Store the allocated memory address. It will be used later in free process.

        /* Update head of the linked list */
        memlist->head = new_node->next;
        memlist->used_units += 1;
    }
    return result;
}

/*
    memlist -- a pointer to a specific
    ptr -- the pointer we want to free
    size -- size of the structure in current slab cache
 */
int32_t free(mem_list* memlist, void* ptr) {
    int32_t result = 0;
    if (ptr != NULL) {
        uint32_t index = ((uint32_t)ptr - (uint32_t)memlist->u_base) / memlist->size;
        node* free_node = memlist->n_base + index;
        if (index < memlist->max) {
            if (free_node->ptr == ptr) {
                /* We can free a chunk of memory only if it is assigned by us */
                /* Insert this node in the front of the linked list */
                free_node->next = memlist->head;
                memlist->head = free_node;
                memlist->used_units -= 1;
                free_node->ptr = NULL;
                result = 1;
            }
        }
    }
    return result;
}


/* Initialize slab cache */
void slab_cache_init() {
    mem_init(MEMORY_START, sizeof(slab_cache), &slab_cache_list);
    mem_init(MEMORY_START + FOUR_KB, sizeof(mem_list), &slabs_list);
}

/*
    Create a slab cache
    input:  name -- name of the slab cache
            size -- objects' size in this slab cache
 */
slab_cache* slab_cache_create(const char* name, uint32_t size) {
    int i;
    slab_cache* result = NULL;
    /* Sanity check */
    if (size >= (FOUR_KB - sizeof(node))) {
        return NULL;
    }
    if (name == NULL) {
        return NULL;
    }
    /* Find a free page */
    for (i = 2; i < 1024; i++) {
        if (slab_page_table[i].PAGE_4KB.present == 0) {
            slab_page_table[i].PAGE_4KB.present = 1;
            break;
        }
    }

    /* Allocate space from slab_cache page and slab_list page */
    result = (slab_cache*)malloc(&slab_cache_list);
    strncpy((int8_t*)result->name, (int8_t*)name, 10);
    result->name[10] = 0;
    result->slabs = (mem_list*)malloc(&slabs_list);
    result->size = size;
    /* Assign a new 4kb space for this cache */
    mem_init(i * FOUR_KB + MEMORY_START, size, result->slabs);

    return result;
}

/* Allocate a object from slab cache
    input: a pointer to a slab cache */
void* slab_cache_alloc(slab_cache* cache) {
    void* result = NULL;
    /* Sanity check */
    if (cache == NULL)
        return result;
    /* Malloc space from this slab cache */
    mem_list* cur_slab = cache->slabs;
    while (1) {
        result = malloc(cur_slab);
        if (result == NULL) {
            if (cur_slab->next == NULL) {
                printf("\nAllocation fail: no free slabs\n");
                break;
            }
            cur_slab = cur_slab->next;
            continue;
        }
        break;
    }
    return result;
}

/* Free a memory from slab_cache
    input: a pointer to slab cache and object pointer */
int32_t slab_cache_free(slab_cache* cache, void* ptr) {
    int32_t result = 0;
    /* Sanity check */
    if (cache == NULL || ptr == NULL) {
        return result;
    }
    /* Find which slab owns this ptr */
    mem_list* cur_slab = cache->slabs;
    mem_list* prev_slab = cur_slab; // Used to shrink slab cache
    while (1) {
        if ((uint32_t)cur_slab->n_base + FOUR_KB > (uint32_t)ptr) {
            break;
        }
        prev_slab = cur_slab;
        cur_slab = cur_slab->next;  
        if (cur_slab == NULL) {
            break;
        }
        
    }
    if (cur_slab == NULL) { 
        return result;
    }
    /* Free the ptr */
    result = free(cur_slab, ptr);

    if (cur_slab->used_units == 0) {
        uint32_t index;
        index = ((uint32_t)cur_slab->head - MEMORY_START) / (FOUR_KB);
        slab_page_table[index].PAGE_4KB.present = 0;  // Free page
        prev_slab->next = cur_slab->next;   // Update linked list
        free(&slabs_list, cur_slab); // Free current slab in the slabs_list
    }
    return result;
}

void show_slab_cache(slab_cache* slab_cache) {
    printf("Slab name: %s / slab_cache's ptr: %x / size: %d / max units: %d\n", slab_cache->name, (uint32_t)slab_cache, slab_cache->size, slab_cache->slabs->max);
    /* Go through all the slabs owned by this slab cache */
    int32_t slabs_num = 0, used_unit = 0, i;
    mem_list* cur_slab;
    node* temp;
    cur_slab = slab_cache->slabs;
    while (1) {
        slabs_num += 1;
        temp = cur_slab->n_base;
        for (i = 0; i < cur_slab->max; i++) {
            if (temp->ptr != 0) {
                printf("memory ptr:%x  ", temp->ptr);
            }
            temp = temp + 1;
        }
        used_unit += cur_slab->used_units;
        cur_slab = cur_slab->next;
        if (cur_slab == NULL) {
            break;
        }
    }
    printf("\ntotal slabs:%d  used units:%d\n", slabs_num, used_unit);
}

/* Visualize all the slab caches' information */
void show_slab_caches() {
    /* Go through all the nodes in slab_cache_list and find those are used */
    node* cur_node;
    cur_node = slab_cache_list.n_base;
    int32_t i;
    printf("\n---------------Info of current slab caches---------------\n");
    for (i = 0; i < slab_cache_list.max; i++) {
        if (cur_node->ptr != 0) {
            show_slab_cache((slab_cache*)cur_node->ptr);
        }
        cur_node = cur_node + 1;
    }
}

