#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "types.h"
#include "rtc.h"
#include "page.h"
#include "keyboard.h"
#include "file_sys.h"
#include "memory.h"


#define PASS 0
#define FAIL -1 //??

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "FAIL" : "PASS");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */
/* 
 * idt_test
 *   DESCRIPTION: idt_test_
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: idt_test_
 */
// int idt_test_(){
// 	clear(); //clear the screen
// 	set_cursor(0, 0);
// 	TEST_HEADER;
// 	int i;
// 	int result = PASS;
// 	for (i = 0; i < 10; ++i){
// 		if ((idt[i].offset_15_00 == NULL) && 
// 			(idt[i].offset_31_16 == NULL)){
// 			assertion_failure();
// 			result = FAIL;
// 		}
// 	}
// 	printf("Press Enter to test System Call\n");
// 	while (key_pressed != 0x1c && key_pressed != 0x36);
// 	if (key_pressed == 0x1c)
// 		asm ("int $0x80");
// 	key_pressed = 0;

// 	return result;
// }

/* 
 * div_zero
 *   DESCRIPTION: check DIVIDE_ERROR_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: div_zero
 */
// int div_zero(){
// 	TEST_HEADER;
// 	int a, b, c;
// 	a = 1;
// 	b = 0;
// 	c = a/b;
// 	return FAIL;// test if ie can handle divide zero
// }

/*
 * deref_page_bound
 *   DESCRIPTION: check DIVIDE_ERROR_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: div_zero
 */

// int deref_page_bound(){
// 	TEST_HEADER;
// 	uint32_t ptr;
// 	//try to reach a address out of bound
// 	uint32_t* addr_nonexist=(uint32_t*)(0x800000+1); 
// 	ptr =* addr_nonexist;
// 	return FAIL;// test if it can handle page bound
// }

/*
 * deref_page_bound
 *   DESCRIPTION: check DIVIDE_ERROR_EXCEPTION
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: div_zero
 */
// int deref_page_NULL(){
// 	TEST_HEADER;
// 	uint32_t ptr;
// 	//try to point to NULL
// 	uint32_t* addr=NULL; 
// 	ptr =* addr;
// 	return FAIL;// test if it can handle the deref page null exception
// }

/* 
 * exception_test_
 *   DESCRIPTION: exception_test_
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: exception_test_
 */
// int exception_test_() {
// 	int result = PASS;
// 	result &= div_zero();// test if ie can handle divide zero
// 	result &= deref_page_bound(); // test if it can handle page bound
// 	result &= deref_page_NULL(); // test if it can handle the deref page null exception

// 	return result;
// }


//cp1
/* 
 * keyboard_test_
 *   DESCRIPTION: keyboard_test_
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: keyboard_test_
 */
// int keyboard_test_() {
// 	clear(); //clear the screen
// 	set_cursor(0, 0);
// 	TEST_HEADER;

// 	printf("\nPress 1 to exit test.\n");
// 	printf("Input: ");
// 	while (key_pressed != '1'); key_pressed = 0; // press 1 to exit
// 	return PASS;
// }
/* 
 * blue_screen
 *   DESCRIPTION: blue_screen
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: blue_screen
 */
// int blue_screen() {
// 	asm("int $0x01");
// 	return PASS;
// }




// /* Checkpoint 2 tests */
// //cp2

// /*
//  * open_file_test_   ->file_open(const uint8_t* filename)
//  *   DESCRIPTION: open_file_test_
//  *   INPUTS: uint8_t* filename
//  *   OUTPUTS: none
//  *   RETURN VALUE: Fail for fali, PASS for success
//  *   SIDE EFFECTS: none
//  */
// int open_file_test_(uint8_t* filename) {
// 	TEST_HEADER;

// 	if (file_open(filename) != 0) {
// 		return FAIL;
// 	}
// 	return PASS;
// }


// /*
//  * close_file_test_  ->file_close(const uint8_t* filename)
//  *   DESCRIPTION: close_file_test_
//  *   INPUTS: uint8_t* filename
//  *   OUTPUTS: none
//  *   RETURN VALUE: Fail for fali, PASS for success
//  *   SIDE EFFECTS: none
//  */
// int close_file_test_() {
// 	TEST_HEADER;

// 	uint8_t* file_name = 0;
// 	if (file_close(file_name) != 0){
// 		return FAIL;
// 	}
// 	return PASS;
// }



// /*
//  * read_file_test_		->file_read(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t count)
//  *   DESCRIPTION: read_file_test_
//  *   INPUTS: none
//  *   OUTPUTS: none
//  *   RETURN VALUE:  Fail for fali, PASS for success
//  *   SIDE EFFECTS: none
//  */
// int read_file_test_() {
// 	TEST_HEADER;

// 	//uint32_t result = file_read(0, 0, NULL, 0);
// 	if ( file_read(0, 0, NULL, 0) == 0) {
// 		return PASS;
// 	}
// 	else{
// 	return FAIL;
// 	}
// }



// /*
//  * read_by_name_test_		->read_dentry_by_name(const uint8_t* fname, dentry_t* dentry)
//  *   DESCRIPTION: read_data_test_
//  *   INPUTS: uint8_t* filename
//  *   OUTPUTS: none
//  *   RETURN VALUE: Fail for fali, PASS for success
//  *   SIDE EFFECTS: none
//  */
// int read_by_name_test_(uint8_t* filename) {
// 	TEST_HEADER;

// 	dentry_t cur_file;
// 	if (read_dentry_by_name(filename, &cur_file) != 0){
// 		return FAIL;}
// 	printf("File name is %s\n", cur_file.file_name);
// 	//printf("The file's type is %d!\n", cur_file.file_type);
// 	//printf("The file's size is %d!\n", cur_file.inode.length);
	
// 	if (strncmp((int8_t*)cur_file.file_name, (int8_t*)filename, 32) != 0)
// 		return FAIL;
// 	return PASS;
// }

// //=========================================================



// /*
//  * read_data_test_			-> read_dentry_by_name()  && read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
//  *   DESCRIPTION: read_data_test_
//  *   INPUTS: uint8_t* filename
//  *   OUTPUTS: none
//  *   RETURN VALUE: Fail for fali, PASS for success
//  *   SIDE EFFECTS: none
//  */
// int read_data_test_(uint8_t* filename) {
// 	TEST_HEADER;

// 	int32_t i,read_bytes_data;
// 	uint32_t signal;
// 	dentry_t cur_file;
// 	char buff[40000] = { '\0' };//????
	
// 	signal = read_dentry_by_name(filename, &cur_file);//store the file date to cur_file
// 	if(signal != 0){
// 		return FAIL;
// 	}
// 	read_bytes_data = read_data(cur_file.inode, 0, (uint8_t*)buff, 100000);//can change the length???
// 	printf("File: %s.\n", cur_file.file_name);
// 	for (i = 0; i < read_bytes_data; i++){
// 		if (buff[i] != NULL){
// 			putc(buff[i]);
// 		}
// 	}
// 	return PASS;
// }

// /*
//  * read_directory_test_ 		->d_read(uint32_t index, char* buf)
//  *   DESCRIPTION: read_directory_test_
//  *   INPUTS: none
//  *   OUTPUTS: none
//  *   RETURN VALUE: Fail for fali, PASS for success
//  *   SIDE EFFECTS: none
//  */
// int read_directory_test_() {
// 	TEST_HEADER;

// 	int32_t i;
// 	char buf[4096];
// 	// 63 is the max directory number in filesystem
// 	for (i = 0; i < 63; i++) {
// 		if (d_read(i, buf) == -1){  //read only one file once a time
// 			break;
// 		}
// 		printf((int8_t*)buf);
// 		printf("\n");
// 	}
// 	return PASS;
// }




// //cp2
// /* Terminal FUNCTION Test 
//  * 
//  *   DESCRIPTION: use loop to test terminal_read and terminal_write
//  *   INPUTS: read_nbytes, write_nbytes
//  *   OUTPUTS: none
//  *   RETURN VALUE: Fail for fali, PASS for success
//  *   SIDE EFFECTS: loop
//  */
// int terminal_test(int32_t read_nbytes, int32_t write_nbytes) {
// 	//TEST_HEADER;
// 	clear();
// 	int32_t r_, w_;
// 	uint8_t buf[128];
// 	while (1) {
// 		r_ = terminal_read(0, buf, read_nbytes);
// 		printf("Read %d bytes\n", r_);
// 		//update_cursor(get_screen_x(),get_screen_y());
// 		w_ = terminal_write(0, buf, write_nbytes);
// 		printf("write %d bytes\n", w_);
// 		//update_cursor(get_screen_x(),get_screen_y());
// 	}
// 	return PASS;
// }

// //cp2
// /* 
//  * rtc_test_
//  *   DESCRIPTION: rtc_test_
//  *   INPUTS: none
//  *   OUTPUTS: none
//  *   RETURN VALUE: Fail for fali, PASS for success
//  *   SIDE EFFECTS: rtc_test_
//  */
// int rtc_test_(){
// 	int rate = 0;
// 	int freq,k;
// 	clear(); //clear the screen
// 	set_cursor(0, 0);
// 	TEST_HEADER;

// 	rtc_open(NULL);//reset the virtual-freq(test by change the init virt_freq to 16)
// 	for (freq = RTC_MIN_RATE; freq <= real_interrupt_freq; freq <<= 1) {// k: freq = 2^k
// 		if(rtc_write(0, &freq, 4) == 0){//rate of print each number
// 		rate++;//rate: frequency == 2^ rate
// 		//rate = 15-k+1;//fomula to caculate rate of function get_rtc_rate(freq)
// 		//printf("\n");

// 		for (k = 0; k < freq; k++) {
// 			rtc_read(0, NULL, 0);//(only) can print rate with v(freq->rate)
// 			printf("%d", 15-rate+1);
// 		}

// 		printf("\n");}

// 		else{
// 			printf("input frequency %d not power of 2.\n",freq);
// 		}

// 	}

// 	return PASS;
// }


int memory_test() {
	TEST_HEADER;
	int32_t* ptr;
	char name[20] = { "TestCache" };
	slab_cache* test = slab_cache_create(name, 32);
	ptr = (int32_t*)slab_cache_alloc(test);
	show_slab_caches();

	char name2[20] = { "TestCache2" };
	slab_cache* test2 = slab_cache_create(name2, 16);
	slab_cache_alloc(test);
	slab_cache_alloc(test2);
	show_slab_caches();

	slab_cache_free(test, ptr);
	show_slab_caches();


	return PASS;
}

// int rtc_test_incorrect_input(){
// 	int rate = 0;
// 	int freq,k;
// 	clear(); //clear the screen
// 	set_cursor(0, 0);
// 	TEST_HEADER;

// 	rtc_open(NULL);//reset the virtual-freq(test by change the init virt_freq to 16)
// 	for (freq = RTC_MIN_RATE; freq <= 10; freq++) {// k: freq = 2^k
		
// 		if(rtc_write(0, &freq, 4) == 0){//rate of print each number
// 		rate++;//rate: frequency == 2^ rate
// 		//rate = 15-k+1;//fomula to caculate rate of function get_rtc_rate(freq)
// 		//printf("\n");

// 		for (k = 0; k < freq; k++) {
// 			rtc_read(0, NULL, 0);//(only) can print rate with v(freq->rate)
// 			printf("%d", 15-rate+1);
// 		}
// 		printf("\n");}

// 		else{
// 			printf("input frequency %d not power of 2.\n",freq);
// 		}
// 	}
// 	return PASS;
// }
// /* Checkpoint 3 tests */
// /* Checkpoint 4 tests */
// /* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// launch your tests here

	TEST_OUTPUT("memory test", memory_test());


/*checkpoint2 test*/
	//????
	
	// printf("Press Enter to start file system test 1.1\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_1.1", open_file_test_((uint8_t*)"frame0.txt"));// can open ->PASS

	// printf("Press Enter to start file system test 1.2\n");// test for incorrect input
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_1.2", open_file_test_((uint8_t*)"not_exist_filename.txt"));//can't open ->FAIL

	// printf("Press Enter to start file system test 2.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_2", close_file_test_());

	// printf("Press Enter to start file system test 3.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_3", read_file_test_());

	// printf("Press Enter to start file system test 4.1.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_4.1", read_by_name_test_((uint8_t*)"verylargetextwithverylongname.tx"));//??why .tx

	// printf("Press Enter to start file system test 4.2.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_4.2", read_by_name_test_((uint8_t*)"grep"));//?????

	// /*test for incorrect input*/
	// printf("Press Enter to start file system test 4.3.\n");// test for incorrect input
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_4.3", read_by_name_test_((uint8_t*)"verylargetextwithverylongname.txt"));// test for incorrect input ......txt

	// printf("Press Enter to start file system test5.1.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_5.1", read_data_test_((uint8_t*)"verylargetextwithverylongname.tx"));//??????   tx???

	// printf("Press Enter to start file system test5.2.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_5.2", read_data_test_((uint8_t*)"frame0.txt"));

	// printf("Press Enter to start file system test5.3.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_5.3", read_data_test_((uint8_t*)"frame1.txt"));

	// printf("Press Enter to start file system test5.4.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_5.4", read_data_test_((uint8_t*)"ls"));//????.exe   wrong input now

	// printf("Press Enter to start file system test5.5.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_5.5", read_data_test_((uint8_t*)"grep"));//wrong input now

	// printf("Press Enter to start file system test6.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtdc
	// TEST_OUTPUT("file_system_test_6", read_directory_test_());
	
	

	// printf("Press Enter to start rtc test.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtc
	// TEST_OUTPUT("rtc_test", rtc_test_());

	// printf("Press Enter to start rtc test.2.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test rtc
	// TEST_OUTPUT("rtc_test_incor_input", rtc_test_incorrect_input());
	 

	// //????
	// //cp2
	// printf("Press  to start terminal test.\n");
	// while (key_pressed != 0x1c); key_pressed = 0; // test idt system call 
	// TEST_OUTPUT("terminal_kbd_test_echo", terminal_test(128, 128));


	// clear(); //clear the screen
	// printf("Test Ends!\n");
}


