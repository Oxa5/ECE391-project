//
//  rtc.h
//  88
//
//  Created by 学习 on 15/10/2023.
//

#ifndef rtc_h
#define rtc_h

#include "types.h"
#include "lib.h"
#include "i8259.h"
#include "tests.h"
// rtc ports
#define RTC_PORT 0x70
// rtc datas
#define RTC_DATA 0x71


#define real_interrupt_freq 1024
#define RTC_MIN_RATE 2
#define RTC_MAX_RATE 32768
//rtc register address
/*	 0A  RTC Status register A:
;
;	    |7|6|5|4|3|2|1|0|  RTC Status Register A
;	     | | | | `---------- rate selection Bits for divider output
;	     | | | |		 frequency (set to 0110 = 1.024kHz, 976.562æs)
;	     | `-------------- 22 stage divider, time base being used;
;	     |			  (initialized to 010 = 32.768kHz)
;	     `-------------- 1=time update in progress, 0=time/date available
;	 
;	 0B  RTC Status register B:
;
;	    |7|6|5|4|3|2|1|0|  RTC Status Register B
;	     | | | | | | | `---- 1=enable daylight savings, 0=disable (default)
;	     | | | | | | `----- 1=24 hour mode, 0=12 hour mode (24 default)
;	     | | | | | `------ 1=time/date in binary, 0=BCD (BCD default)
;	     | | | | `------- 1=enable square wave frequency, 0=disable
;	     | | | `-------- 1=enable update ended interrupt, 0=disable
;	     | | `--------- 1=enable alarm interrupt, 0=disable
;	     | `---------- 1=enable periodic interrupt, 0=disable
;	     `----------- 1=disable clock update, 0=update count normally
;
;	 0C  RTC Status register C (read only):
;
;	    |7|6|5|4|3|2|1|0|  RTC Status Register C  (read only)
;	     | | | | `---------- reserved (set to 0)
;	     | | | `---------- update ended interrupt enabled
;	     | | `---------- alarm interrupt enabled
;	     | `---------- periodic interrupt enabled
;	     `---------- IRQF flag
*/
#define RTC_REGA 0x8A
#define RTC_REGB 0x8B
#define RTC_REGC 0x0C


extern void rtc_init();
extern void rtc_handler();
extern int32_t get_rtc_rate(int32_t frequency);
extern int rtc_open(const uint8_t* filename);
extern int rtc_close(int32_t fd);
extern int rtc_read(int32_t fd, void* buf, int32_t nbytes);
extern int rtc_write(int32_t fd, const void* buf, int32_t nbytes);
#endif /* rtc_h */
