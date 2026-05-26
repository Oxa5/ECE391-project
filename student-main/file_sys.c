#include "file_sys.h"

int index = 0;

/*
 * file_system_init
 *   DESCRIPTION: initialize the file system
 *   INPUTS: bootBlock_addr - address of the boot block
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void file_system_init(uint32_t file_add) {

    boot_block = (bb_t*)file_add; // Find boot block
}

/*
 *
 *read_dentry_by_name
 * DESCRIPTION: Fill in the dentry_t block passed as their second argument
 * with the file name, file type, and inode number for the file, then return 0
 * INPUTS : fname - file's name
 * dentry - the denttry block to be filled
 * OUTPUTS : none
 * RETURN VALUE : 0 on success, otherwise - 1
 * SIDE EFFECTS : Fill the dentry block
 */
uint32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) {
    int i, length;
    length = strlen((int8_t*)fname);
    if (dentry == NULL) {
        return FAIL;
    }
    if (length <= 0 || length > 32) { //32 is the maximum name length
        return FAIL;
    }
    for (i = 0; i < boot_block->dentry_num; i++) {
        if (strncmp((int8_t*)fname, (int8_t*)boot_block->dentries[i].file_name, 32) == 0) {
            *dentry = boot_block->dentries[i];
            return 0;
        }
    }
    return FAIL;
}

/*
 * read_dentry_by_index
 *   DESCRIPTION: Fill in the dentry_t block passed as their second argument
 *				  with the file name, file type, and inode number for the file, then return 0
 *   INPUTS: index - file's index
 *			 dentry - the denttry block to be filled
 *   OUTPUTS: none
 *   RETURN VALUE: 0 on success, otherwise -1
 *   SIDE EFFECTS: Fill the dentry block
 */
uint32_t read_dentry_by_index(uint8_t index, dentry_t* dentry) {
    /* Sanity check */
    if (dentry == NULL) {
        return FAIL;
    }
    if ( index > boot_block->dentry_num || index > 63) { //63 is the maximum inode number
        return FAIL;
    }
    *dentry = boot_block->dentries[index];
    return 0;
}


/*
 * read_data
 *   DESCRIPTION: reading up to length bytes starting from position offset in the
 *				  file with inode number inode and returning the number of bytes
 *				  read and placed in the buffer
 *   INPUTS: inode - index of inoddes
 *			 offset - start position in the file
 *			 buf - buffer to be placed in
 *			 length - length of bytes to be read
 *   OUTPUTS: none
 *   RETURN VALUE:  the number of bytes read , otherwise 0
 *   SIDE EFFECTS: none
 */

uint32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
    /* Sanity check */
    if (inode < 0 || inode >= boot_block->inode_num || inode > 63) { //63 is the maximum inode number
        return 0;
    }
    inode_t* cur_inode = (inode_t*)(boot_block + inode + 1);
    if (offset < 0 || length <= 0 || offset >= cur_inode->length) {
        return 0;
    }
    if (length + offset > cur_inode->length) {
        length = cur_inode->length - offset;
    }
    if (length <= 0) {
        return 0;
    }

    int i, counter = 0;
    uint32_t first_datab = offset / 4096; //4kb per block; the data block we begin to read 
    uint32_t last_datab = first_datab + (length - 1) / 4096; //the last data block we read 
    uint32_t data_start_position = offset % 4096; //the position we start to read in the start data block
    uint32_t data_end_position = 4096; //4kb per block

    for (i = first_datab; i <= last_datab; i++) {
        db_t* datab = (db_t*)(boot_block + boot_block->inode_num + 1 + cur_inode->data_block[i]); //find the true data block
        if (first_datab == last_datab) {
            memcpy((char*)buf + counter, (char*)datab + data_start_position, length);
            counter += length;
            return counter;
        }
        if (i != first_datab) {
            data_start_position = 0;
        }
        if (i == last_datab) {
            data_end_position = data_start_position + (length - 1) % 4096; //4kb per block
        }

        memcpy((char*)buf + counter, (char*)datab + data_start_position, data_end_position - data_start_position); //copy the data in the data block to buffer's corresponding position
        counter += data_end_position - data_start_position; //increase counter of bytes
    }
    return counter;
}

/*
 * file_open
 *   DESCRIPTION: open the given file and read the dentry
 *   INPUTS: filename - the name of the file we need to open
 *   OUTPUTS: none
 *   RETURN VALUE:  0 on success , otherwise -1
 *   SIDE EFFECTS: none
 */
int32_t file_open(const uint8_t* filename) {
   // printf("open file:%s\n", (char*)filename);
    dentry_t dentry;
    if (read_dentry_by_name(filename, &dentry) == FAIL) {
        return FAIL;
    }
    //printf("finish file:%s\n", (char*)filename);
    return 0;
}

/*
 * file_close
 *   DESCRIPTION: close the file
 *   INPUTS: filename - the name of the file we need to close
 *   OUTPUTS: none
 *   RETURN VALUE:  0 on success , otherwise -1
 *   SIDE EFFECTS: none
 */
int32_t file_close(const int32_t filename) {
    return 0;
}

/*
 * file_read
 *   DESCRIPTION: read the file
 *   INPUTS: filename - the name of the file we need to read
 *			 buf - the buffer to be placed in
 *			 length - the length of data to read
 *   OUTPUTS: none
 *   RETURN VALUE: number of bytes read on success , otherwise -1
 *   SIDE EFFECTS: none
 */
int32_t file_read(int32_t fd, void* buf, int32_t length) {
    int32_t pid = get_current_pid();
    pcb* current_pcb = get_pcb(pid);

    uint32_t inode = current_pcb->file_array[fd].inode;
    uint32_t offset = current_pcb->file_array[fd].file_position;
    uint32_t result = read_data(inode, offset, buf, length);
    return result;
}

/*
 * file_write
 *   DESCRIPTION: wirte the file, but file is read only
 *   INPUTS: filename - the name of the file we need to write
 *			 buf - the buffer to be written
 *			 length - the length of data to write
 *   OUTPUTS: none
 *   RETURN VALUE:  0 on success , otherwise -1
 *   SIDE EFFECTS: none
 */
int32_t file_write(const int32_t fd, const void* buf, int32_t length) {
    return FAIL;
}

/*
 * d_open
 *   DESCRIPTION: open the given directory and read the dentry
 *   INPUTS: dir_name - the name of the directory we need to open
 *   OUTPUTS: none
 *   RETURN VALUE:  0 on success , otherwise -1
 *   SIDE EFFECTS: none
 */
int32_t d_open(const uint8_t* dir_name) {
    return 0;
}

/*
 * d_close
 *   DESCRIPTION: close the directory
 *   INPUTS: dir_name - the name of the directory we need to close
 *   OUTPUTS: none
 *   RETURN VALUE:  0 on success , otherwise -1
 *   SIDE EFFECTS: none
 */
int32_t d_close(const int32_t dir_name) {
    return 0;
}

/*
 * d_read
 *   DESCRIPTION: read the file name with index
 *   INPUTS: index - the index of the file
 *			 buf - the buffer to be written
 *   OUTPUTS: none
 *   RETURN VALUE:  0 on success , otherwise -1
 *   SIDE EFFECTS: none
 */

int32_t d_read(int32_t fd, void* buf, int32_t length) {
    int result;
    char* f_name;
    dentry_t dentry;

    if (index >= boot_block->dentry_num) {
        index = 0;
        return 0;
    }
    dentry = boot_block->dentries[index];
    f_name = (char*)dentry.file_name;
    result = strlen((int8_t*)f_name);
    memcpy(buf, f_name, 32); //copy all the name length
    index++;
    if (result >= 32) {
        result = 32;
    }
    return result ;
}

/*
 * d_write
 *   DESCRIPTION: wirte the directory, but directory is read only
 *   INPUTS: dir_name - the name of the directory we need to write
 *   OUTPUTS: none
 *   RETURN VALUE:  0 on success , otherwise -1
 *   SIDE EFFECTS: none
 */
int32_t d_write(int32_t fd, const void* buf, int32_t length) {
    return FAIL;
}
