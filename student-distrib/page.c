#include "page.h"
#define VIDEO     0xB8000
#define OUR_SPACE 0x8000000
#define PRO_START 32// OUR_SPACE >> 22
#define PTB_MASK 0x003FF000

//get from lib.c 


/*
 * init_paging
 *   DESCRIPTION: Initializes the page directory and page table
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Map 4-8 MB of virtual mem as a 4mb page to 4-8 MB of physical memory
 *                 The first 4 MB of memory were broken down into 4kb pages. Set all unused pages to not present
 */
void init_paging() {
    // Set the page directory entry
    int i;
    int video_index = VIDEO >> 12;//get the table index
  
    // Moving 12 bits to the right is the same as dividing by 4096

    //index=0/1  0-4mb/4-8mb,pagedir[0/1] set to present

    page_directory[0].PAGE_4KB.present = 1;
    page_directory[0].PAGE_4KB.read_or_write = 1;
    page_directory[0].PAGE_4KB.user_or_supervisor = 0;
    page_directory[0].PAGE_4KB.write_through = 0;
    page_directory[0].PAGE_4KB.cache_disabled = 0;
    page_directory[0].PAGE_4KB.accessed = 0;
    page_directory[0].PAGE_4KB.page_size = 0;
    page_directory[0].PAGE_4KB.global_page = 0;
    page_directory[0].PAGE_4KB.avail = 0;
    page_directory[0].PAGE_4KB.reserved = 0;
    page_directory[0].PAGE_4KB.base_address = ((uint32_t)page_table) >> 12;  //the phys page num of this mem location

    page_directory[1].PAGE_4MB.present = 1;
    page_directory[1].PAGE_4MB.read_or_write = 1;
    page_directory[1].PAGE_4MB.user_or_supervisor = 0;
    page_directory[1].PAGE_4MB.write_through = 0;
    page_directory[1].PAGE_4MB.cache_disabled = 1;
    page_directory[1].PAGE_4MB.accessed = 0;
    page_directory[1].PAGE_4MB.page_size = 1;
    page_directory[1].PAGE_4MB.global_page = 1;
    page_directory[1].PAGE_4MB.avail = 0;
    page_directory[1].PAGE_4MB.pat = 0;
    page_directory[1].PAGE_4MB.dirty = 0;
    page_directory[1].PAGE_4MB.reserved = 0;
    page_directory[1].PAGE_4MB.base_address = 1; //the phys page num of this mem location


    //index=2-1023  8MB-4GB,pagedir[index] set to not present
    for (i = 2; i < 1024; i++) {
        page_directory[i].val = 0;
        page_directory[i].PAGE_4MB.present = 0;
    }
    //for 0-4MB break down to 4kb page
       //index=0-1023  page_table[index] set to not present,
       //except index == videomem_page_table_index
    for (i = 0; i < 1024; i++) {
        if (i == video_index) {
            continue;
        }
        else {
            page_table[i].val = 0;//set to not present,
        }
    }

    //for index == videomem_page_table_index
    for(i = 0; i <= 4 ; i++){
        if(i != 1){
            page_table[video_index +i].PAGE_4KB.present = 1;//set to present,
            page_table[video_index +i].PAGE_4KB.read_or_write = 1;
            page_table[video_index +i].PAGE_4KB.user_or_supervisor = 0;
            page_table[video_index +i].PAGE_4KB.write_through = 0;
            page_table[video_index +i].PAGE_4KB.cache_disabled = 0;
            page_table[video_index +i].PAGE_4KB.accessed = 0;
            page_table[video_index +i].PAGE_4KB.dirty = 0;
            page_table[video_index +i].PAGE_4KB.pat = 0;
            page_table[video_index +i].PAGE_4KB.global_page = 0;
            page_table[video_index +i].PAGE_4KB.avail = 0;
            page_table[video_index +i].PAGE_4KB.base_address = video_index + i;  //the phys page num of this mem location  VIDEO>>12
        }
    }

    page_directory[MEMORY_START >> 22].PAGE_4KB.present = 1;
    page_directory[MEMORY_START >> 22].PAGE_4KB.read_or_write = 1;
    page_directory[MEMORY_START >> 22].PAGE_4KB.user_or_supervisor = 0;
    page_directory[MEMORY_START >> 22].PAGE_4KB.write_through = 0;
    page_directory[MEMORY_START >> 22].PAGE_4KB.cache_disabled = 0;
    page_directory[MEMORY_START >> 22].PAGE_4KB.accessed = 0;
    page_directory[MEMORY_START >> 22].PAGE_4KB.page_size = 0;//dont change
    page_directory[MEMORY_START >> 22].PAGE_4KB.global_page = 0;
    page_directory[MEMORY_START >> 22].PAGE_4KB.avail = 0;
    page_directory[MEMORY_START >> 22].PAGE_4KB.base_address = ((uint32_t)&slab_page_table) >> 12;
    
    for (i = 0; i < 1024; i++) {
        slab_page_table[i].PAGE_4KB.present = 0;
        slab_page_table[i].PAGE_4KB.read_or_write = 1;
        slab_page_table[i].PAGE_4KB.user_or_supervisor = 0;
        slab_page_table[i].PAGE_4KB.write_through = 0;
        slab_page_table[i].PAGE_4KB.cache_disabled = 0;
        slab_page_table[i].PAGE_4KB.accessed = 0;
        slab_page_table[i].PAGE_4KB.pat = 0;
        slab_page_table[i].PAGE_4KB.dirty = 0;//dont change
        slab_page_table[i].PAGE_4KB.global_page = 0;
        slab_page_table[i].PAGE_4KB.avail = 0;
        slab_page_table[i].PAGE_4KB.base_address = (MEMORY_START >> 12) + i;
    }
    slab_page_table[0].PAGE_4KB.present = 1;
    slab_page_table[1].PAGE_4KB.present = 1;
// SET_PTE(page_table, (VIDEO >> 12) + 2, 0, (VIDEO >> 12) + 2);//!!!!!
//     SET_PTE(page_table, (VIDEO >> 12) + 3, 0, (VIDEO >> 12) + 3);//
//     SET_PTE(page_table, (VIDEO >> 12) + 4, 0, (VIDEO >> 12) + 4);//

    //enable and give base of physical_mem to cr3
    asm volatile(
        "movl  %0, %%eax;           \
         movl  %%eax, %%cr3;        \
         movl  %%cr4, %%eax;        \
         orl   $0x00000010, %%eax;  \
         movl  %%eax, %%cr4;        \
         movl  %%cr0, %%eax;        \
         orl   $0x80000000, %%eax;  \
         movl  %%eax, %%cr0;"
        : //none
    : "r" (&page_directory)
        : "%eax"
        );

}


/*
 * set_page
 *   DESCRIPTION: set page for the user program from 128MB to 132 MB
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: page_directory[PRO_START] set to present
 *                 
 */
void set_page(uint32_t pid) {

    page_directory[PRO_START].PAGE_4KB.present = 1;
    page_directory[PRO_START].PAGE_4KB.read_or_write = 1;
    page_directory[PRO_START].PAGE_4KB.user_or_supervisor = 1;
    page_directory[PRO_START].PAGE_4KB.write_through = 0;
    page_directory[PRO_START].PAGE_4KB.cache_disabled = 1;
    page_directory[PRO_START].PAGE_4KB.accessed = 0;
    page_directory[PRO_START].PAGE_4KB.page_size = 1;//dont change
    page_directory[PRO_START].PAGE_4KB.global_page = 0;
    page_directory[PRO_START].PAGE_4KB.avail = 0;
    page_directory[PRO_START].PAGE_4KB.base_address = ((EIGHT_MB + pid * FOUR_MB) >> 22) << 10;

    asm volatile(
        "movl %0, %%eax \n\t"
        "movl %%eax, %%cr3 \n\t"//store 0 into cr3
        :
    : "r" (&page_directory)//The address of page_directory is passed as an input operand to the embedded assembly code.
        : "%eax"//Tell the compiler that the value of eax register has been modified in the embedded assembly code
        );
}


/*
 *  set_vid_page(uint32_t vidmap_addr)
 *   DESCRIPTION: set vidmap page after 132 MB to map a user level virtual_mem to vid in physcial_mm
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: page_directory[vidmap_index] set to present
 *              video_page_table[i] set to present
 *                 
 */
void set_vid_page() {

    int32_t vidmap_index = (OUR_SPACE + FOUR_MB) >> 22; //index for directory
    int32_t i = ((OUR_SPACE + FOUR_MB) & PTB_MASK) >> 12;
    page_directory[vidmap_index].PAGE_4KB.present = 1;
    page_directory[vidmap_index].PAGE_4KB.read_or_write = 1;
    page_directory[vidmap_index].PAGE_4KB.user_or_supervisor = 1;
    page_directory[vidmap_index].PAGE_4KB.write_through = 0;
    page_directory[vidmap_index].PAGE_4KB.cache_disabled = 0;
    //page_directory[vidmap_index].PAGE_4KB.cache_disabled          = 1;  original
    page_directory[vidmap_index].PAGE_4KB.accessed = 0;
    page_directory[vidmap_index].PAGE_4KB.page_size = 0;//!!!
    page_directory[vidmap_index].PAGE_4KB.global_page = 0;
    page_directory[vidmap_index].PAGE_4KB.avail = 0;
    page_directory[vidmap_index].PAGE_4KB.base_address = (uint32_t)&video_page_table >> 12;//????
    //privilege =1   
    video_page_table[i].PAGE_4KB.present = 1;//set to present,
    video_page_table[i].PAGE_4KB.read_or_write = 1;
    video_page_table[i].PAGE_4KB.user_or_supervisor = 1;
    video_page_table[i].PAGE_4KB.write_through = 0;
    video_page_table[i].PAGE_4KB.cache_disabled = 0;
    video_page_table[i].PAGE_4KB.accessed = 0;
    video_page_table[i].PAGE_4KB.dirty = 0;
    video_page_table[i].PAGE_4KB.pat = 0;
    video_page_table[i].PAGE_4KB.global_page = 0;
    video_page_table[i].PAGE_4KB.avail = 0;
    video_page_table[i].PAGE_4KB.base_address = VIDEO >> 12;  //the phys page num of this mem location  VIDEO>>12
    asm volatile(
        "movl %0, %%eax \n\t"
        "movl %%eax, %%cr3 \n\t"//store 0 into cr3
        :
    : "r" (&page_directory)//The address of page_directory is passed as an input operand to the embedded assembly code.
        : "%eax"//Tell the compiler that the value of eax register has been modified in the embedded assembly code
        );

}


void remap_vid_page(uint8_t terminal_num) {
    int video_index = VIDEO >> 12;
    int32_t vidmap_index;
    int32_t i;
    //uint8_t current_terminal_num = get_current_terminal_num();
    if(terminal_num == current_terminal_num){
        /* Make virtual 0xB8000 = physical 0xB8000 */
    page_directory[0].PAGE_4KB.present = 1;
    page_directory[0].PAGE_4KB.read_or_write = 1;
    page_directory[0].PAGE_4KB.user_or_supervisor = 0;
    page_directory[0].PAGE_4KB.write_through = 0;
    page_directory[0].PAGE_4KB.cache_disabled = 0;
    page_directory[0].PAGE_4KB.accessed = 0;
    page_directory[0].PAGE_4KB.page_size = 0;
    page_directory[0].PAGE_4KB.global_page = 0;
    page_directory[0].PAGE_4KB.avail = 0;
    page_directory[0].PAGE_4KB.reserved = 0;
    page_directory[0].PAGE_4KB.base_address = ((uint32_t)page_table) >> 12;


    page_table[video_index].PAGE_4KB.present = 1;//set to present,
    page_table[video_index].PAGE_4KB.read_or_write = 1;
    page_table[video_index].PAGE_4KB.user_or_supervisor = 0;
    page_table[video_index].PAGE_4KB.write_through = 0;
    page_table[video_index].PAGE_4KB.cache_disabled = 0;
    page_table[video_index].PAGE_4KB.accessed = 0;
    page_table[video_index].PAGE_4KB.dirty = 0;
    page_table[video_index].PAGE_4KB.pat = 0;
    page_table[video_index].PAGE_4KB.global_page = 0;
    page_table[video_index].PAGE_4KB.avail = 0;
    page_table[video_index].PAGE_4KB.base_address = video_index;

        // SET_PTE(video_page_table,
        //     (USER_VIDEO_ADDR & PG_TBL_NUMBER_MASK) >> 12, 1, VIDEO >> 12);

    vidmap_index = (OUR_SPACE + FOUR_MB) >> 22; //index for directory
    i = ((OUR_SPACE + FOUR_MB) & PTB_MASK) >> 12;
    page_directory[vidmap_index].PAGE_4KB.present = 1;
    page_directory[vidmap_index].PAGE_4KB.read_or_write = 1;
    page_directory[vidmap_index].PAGE_4KB.user_or_supervisor = 1;
    page_directory[vidmap_index].PAGE_4KB.write_through = 0;
    page_directory[vidmap_index].PAGE_4KB.cache_disabled = 0;
    //page_directory[vidmap_index].PAGE_4KB.cache_disabled          = 1;  original
    page_directory[vidmap_index].PAGE_4KB.accessed = 0;
    page_directory[vidmap_index].PAGE_4KB.page_size = 0;//!!!
    page_directory[vidmap_index].PAGE_4KB.global_page = 0;
    page_directory[vidmap_index].PAGE_4KB.avail = 0;
    page_directory[vidmap_index].PAGE_4KB.base_address = (uint32_t)&video_page_table >> 12;


    video_page_table[i].PAGE_4KB.present = 1;//set to present,
    video_page_table[i].PAGE_4KB.read_or_write = 1;
    video_page_table[i].PAGE_4KB.user_or_supervisor = 1;
    video_page_table[i].PAGE_4KB.write_through = 0;
    video_page_table[i].PAGE_4KB.cache_disabled = 0;
    video_page_table[i].PAGE_4KB.accessed = 0;
    video_page_table[i].PAGE_4KB.dirty = 0;
    video_page_table[i].PAGE_4KB.pat = 0;
    video_page_table[i].PAGE_4KB.global_page = 0;
    video_page_table[i].PAGE_4KB.avail = 0;
    video_page_table[i].PAGE_4KB.base_address = VIDEO >> 12;
   
    }
    
//====================================================================================
    else{
        /* Make virtual 0xB8000 point to backup buffer */
        //SET_PTE(page_table, VIDEO >> 12, 0, (VIDEO >> 12)+ 2 + terminal_num);

    page_directory[0].PAGE_4KB.present = 1;
    page_directory[0].PAGE_4KB.read_or_write = 1;
    page_directory[0].PAGE_4KB.user_or_supervisor = 0;
    page_directory[0].PAGE_4KB.write_through = 0;
    page_directory[0].PAGE_4KB.cache_disabled = 0;
    page_directory[0].PAGE_4KB.accessed = 0;
    page_directory[0].PAGE_4KB.page_size = 0;
    page_directory[0].PAGE_4KB.global_page = 0;
    page_directory[0].PAGE_4KB.avail = 0;
    page_directory[0].PAGE_4KB.reserved = 0;
    page_directory[0].PAGE_4KB.base_address = ((uint32_t)page_table) >> 12;



    page_table[video_index].PAGE_4KB.present = 1;//set to present,
    page_table[video_index].PAGE_4KB.read_or_write = 1;
    page_table[video_index].PAGE_4KB.user_or_supervisor = 0;
    page_table[video_index].PAGE_4KB.write_through = 0;
    page_table[video_index].PAGE_4KB.cache_disabled = 0;
    page_table[video_index].PAGE_4KB.accessed = 0;
    page_table[video_index].PAGE_4KB.dirty = 0;
    page_table[video_index].PAGE_4KB.pat = 0;
    page_table[video_index].PAGE_4KB.global_page = 0;
    page_table[video_index].PAGE_4KB.avail = 0;
    //page_table[video_index].PAGE_4KB.base_address = video_index;
    page_table[video_index].PAGE_4KB.base_address = (VIDEO >> 12)+ 2 + terminal_num;//corresponding to this content




        // SET_PTE(video_page_table,
        //     (USER_VIDEO_ADDR & PG_TBL_NUMBER_MASK) >> 12, 1,(VIDEO >> 12)+ 2 + terminal_num);

    vidmap_index = (OUR_SPACE + FOUR_MB) >> 22; //index for directory
    i = ((OUR_SPACE + FOUR_MB) & PTB_MASK) >> 12;
    page_directory[vidmap_index].PAGE_4KB.present = 1;
    page_directory[vidmap_index].PAGE_4KB.read_or_write = 1;
    page_directory[vidmap_index].PAGE_4KB.user_or_supervisor = 1;
    page_directory[vidmap_index].PAGE_4KB.write_through = 0;
    page_directory[vidmap_index].PAGE_4KB.cache_disabled = 0;
    //page_directory[vidmap_index].PAGE_4KB.cache_disabled          = 1;  original
    page_directory[vidmap_index].PAGE_4KB.accessed = 0;
    page_directory[vidmap_index].PAGE_4KB.page_size = 0;//!!!
    page_directory[vidmap_index].PAGE_4KB.global_page = 0;
    page_directory[vidmap_index].PAGE_4KB.avail = 0;
    page_directory[vidmap_index].PAGE_4KB.base_address = (uint32_t)&video_page_table >> 12;


    video_page_table[i].PAGE_4KB.present = 1;//set to present,
    video_page_table[i].PAGE_4KB.read_or_write = 1;
    video_page_table[i].PAGE_4KB.user_or_supervisor = 1;
    video_page_table[i].PAGE_4KB.write_through = 0;
    video_page_table[i].PAGE_4KB.cache_disabled = 0;
    video_page_table[i].PAGE_4KB.accessed = 0;
    video_page_table[i].PAGE_4KB.dirty = 0;
    video_page_table[i].PAGE_4KB.pat = 0;
    video_page_table[i].PAGE_4KB.global_page = 0;
    video_page_table[i].PAGE_4KB.avail = 0;
    //video_page_table[i].PAGE_4KB.base_address = VIDEO >> 12;
    video_page_table[i].PAGE_4KB.base_address = (VIDEO >> 12)+ 2 + terminal_num;

    }

    asm volatile(                           \
        "movl %0, %%eax     \n\t"           \
        "movl %%eax, %%cr3  \n\t"           \
        :                                   \
        : "r" (&page_directory)             \
        : "%eax"                            \
    );     

}
