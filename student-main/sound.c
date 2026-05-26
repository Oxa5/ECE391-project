#include "sound.h"
#include "lib.h"
#include "terminal.h"
#include "rtc.h"
#define SOUND_BIT		3
#define SOUND_SHIFT	8
#define SOUND_MASK		0xFC

// reference https://wiki.osdev.org/Sound





// void sound_song() {
//     // Do nothing
//     printf("start sound");
//     uint32_t musscale[14] = {262,277,294,311,329,349,370,392,415,440,466,494,523,554};
//     play_sound(musscale[0]);
//     play_sound(musscale[0]);
//     play_sound(musscale[1]);
//     // play_sound(musscale[0]);
//     // play_sound(musscale[3]);
//     // play_sound(musscale[2]);

//     // play_sound(musscale[0]);
//     // play_sound(musscale[0]);
//     // play_sound(musscale[1]);
//     // play_sound(musscale[0]);
//     // play_sound(musscale[4]);
//     // play_sound(musscale[3]);

//     // play_sound(musscale[0]);
//     // play_sound(musscale[0]);
//     // play_sound(musscale[7]);
//     // play_sound(musscale[5]);
//     // play_sound(musscale[3]);
//     // play_sound(musscale[2]);
//     // play_sound(musscale[1]);
//     nosound ();

// }

/* 
 *   play_sound
 *   DESCRIPTION: play a sound once a time
 *   INPUTS: none
 *   OUTPUTS: none 
 *   RETURN VALUE:  none
 *   SIDE EFFECTS: produce a sound through the speaker
 */
void play_sound ( uint32_t inputfreq )  { 
 	uint32_t frequency ; 
 	uint8_t temp ;
 
    
 	frequency = PIT_FRQ /inputfreq;  //set frequency
 	outb (PIT_SOUND,PIT_MODEREG); 
    
    uint8_t low = (uint8_t)(frequency);// 1011 0110
    uint8_t high = (uint8_t)(frequency >> SOUND_SHIFT);

 	outb (low,PIT_CH2); 
 	outb (high,PIT_CH2);
 
    
    uint8_t shift = temp|SOUND_BIT;
 	temp = inb(PIT_INPUT); 
  	if (temp != shift){ 
 		outb (shift,PIT_INPUT) ; 
 	} 
    //printf("play");

 }

 /* 
 *   nosound
 *   DESCRIPTION: stop the sound
 *   INPUTS: none
 *   OUTPUTS: none 
 *   RETURN VALUE:  none
 *   SIDE EFFECTS: none
 */
void nosound ()  { 
 	uint8_t temp = inb(PIT_INPUT) &SOUND_MASK;
 
 	outb ( temp, PIT_INPUT); 
    //printf("stop");
 }

 



